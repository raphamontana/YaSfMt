#include "RobustMatcher.h"

RobustMatcher::RobustMatcher() : ratio( 0.65f ),
                                 refineF( true ),
                                 confidence( 0.99 ),
                                 distance( 3.0 )
{
    // SURF is the default feature
    surf = Algorithm::create<Feature2D>( "Feature2D.SURF" );
    if (surf.empty()) {
        CV_Error( Error::StsNotImplemented,
                "OpenCV has been compiled without SURF support");
    }
    surf->set( "hessianThreshold", 400 );

    //detector = new SurfFeatureDetector();
    //extractor = new SurfDescriptorExtractor();
}


void RobustMatcher::setFeatureDetector( Ptr< FeatureDetector >& detect )
{
    detector = detect;
}


void RobustMatcher::setDescriptorExtractor( Ptr< DescriptorExtractor >& desc )
{
    extractor = desc;
}


void RobustMatcher::setMinDistanceToEpipolar( double d )
{
    distance = d;
}


void RobustMatcher::setConfidenceLevel( double c )
{
    confidence = c;
}


void RobustMatcher::setRatio( float r )
{
    ratio = r;
}


void RobustMatcher::refineFundamental( bool flag )
{
    refineF = flag;
}


int RobustMatcher::ratioTest( vector< vector< DMatch > >& matches )
{
    int removed = 0;
    // for all matches
    for ( vector< vector< DMatch > >::iterator matchIterator = matches.begin(); matchIterator != matches.end(); ++matchIterator ) {
        // if 2 NN has been identified
        if ( matchIterator->size() > 1 ) {
            // check distance ratio
            if ( (*matchIterator)[0].distance/(*matchIterator)[1].distance > ratio ) {
                matchIterator->clear(); // remove match
                removed++;
            }
        } else { // does not have 2 neighbours
            matchIterator->clear(); // remove match
            removed++;
        }
    }
    return( removed );
}


void RobustMatcher::symmetryTest( const vector< vector< DMatch > >& matches1,
                                  const vector< vector< DMatch > >& matches2,
                                  vector< DMatch >& symMatches )
{
    // for all matches image 1 -> image 2
    for ( vector< vector< DMatch > >::const_iterator matchIterator1 = matches1.begin(); matchIterator1 != matches1.end(); ++matchIterator1 ) {
        if ( matchIterator1->size() < 2 ) { // ignore deleted matches
            continue;
        }
        // for all matches image 2 -> image 1
        for ( vector< vector< DMatch > >::const_iterator matchIterator2 = matches2.begin(); matchIterator2 != matches2.end(); ++matchIterator2 ) {
            if (matchIterator2->size() < 2) { // ignore deleted matches
                continue;
            }
            // Match symmetry test
            if ( (*matchIterator1)[0].queryIdx == (*matchIterator2)[0].trainIdx  &&
                 (*matchIterator2)[0].queryIdx == (*matchIterator1)[0].trainIdx ) {
                // add symmetrical match
                symMatches.push_back( DMatch((*matchIterator1)[0].queryIdx,
                                             (*matchIterator1)[0].trainIdx,
                                             (*matchIterator1)[0].distance) );
                break; // next match in image 1 -> image 2
            }
        }
    }
}


Mat RobustMatcher::ransacTest( const vector< DMatch >&   matches,
                               const vector< KeyPoint >& keypoints1,
                               const vector< KeyPoint >& keypoints2,
                               vector< DMatch >&         outMatches )
{
    // Convert keypoints into Point2f
    vector< Point2f > points1, points2;
    for ( vector< DMatch >::const_iterator it = matches.begin(); it != matches.end(); ++it ) {
         // Get the position of left keypoints
         float x= keypoints1[it->queryIdx].pt.x;
         float y= keypoints1[it->queryIdx].pt.y;
         points1.push_back( Point2f( x, y ) );
         // Get the position of right keypoints
         x= keypoints2[it->trainIdx].pt.x;
         y= keypoints2[it->trainIdx].pt.y;
         points2.push_back( Point2f( x, y ) );
    }

    // Compute F matrix using RANSAC
    vector<uchar> inliers( points1.size(), 0 );
    Mat fundamental = findFundamentalMat(
        Mat( points1 ),
        Mat( points2 ), // matching points
        inliers,        // match status (inlier ou outlier)
        FM_RANSAC,   // RANSAC method
        distance,       // distance to epipolar line
        confidence );   // confidence probability

    // extract the surviving (inliers) matches
    vector<uchar>::const_iterator itIn = inliers.begin();
    vector<DMatch>::const_iterator itM = matches.begin();
    // for all matches
    for ( ; itIn != inliers.end(); ++itIn, ++itM) {

        if ( *itIn ) { // it is a valid match
            outMatches.push_back( *itM );
        }
    }

    cout << "Number of matched points (after cleaning): " << outMatches.size() << endl;

    if ( refineF ) {
        // The F matrix will be recomputed with all accepted matches

        // Convert keypoints into Point2f for final F computation
        points1.clear();
        points2.clear();

        for ( vector< DMatch >::const_iterator it = outMatches.begin(); it != outMatches.end(); ++it ) {
             // Get the position of left keypoints
             float x = keypoints1[it->queryIdx].pt.x;
             float y = keypoints1[it->queryIdx].pt.y;
             points1.push_back( Point2f( x, y ) );
             // Get the position of right keypoints
             x = keypoints2[it->trainIdx].pt.x;
             y = keypoints2[it->trainIdx].pt.y;
             points2.push_back( Point2f( x, y ) );
        }

        // Compute 8-point F from all accepted matches
        fundamental = findFundamentalMat(
                Mat( points1 ),
                Mat( points2 ), // matching points
                FM_8POINT ); // 8-point method
    }

    return( fundamental );
}


Mat RobustMatcher::match( Mat& image1,
                          Mat& image2, // input images
                          vector< DMatch >&   matches, // output matches and keypoints
                          vector< KeyPoint >& keypoints1,
                          vector< KeyPoint >& keypoints2 )
{

    // 1a. Detection of the SURF features
    detector->detect( image1, keypoints1 );
    detector->detect( image2, keypoints2 );

    cout << "Number of SURF points (1): " << keypoints1.size() << endl;
    cout << "Number of SURF points (2): " << keypoints2.size() << endl;

    // 1b. Extraction of the SURF descriptors
    Mat descriptors1, descriptors2;
    extractor->compute( image1, keypoints1, descriptors1 );
    extractor->compute( image2, keypoints2, descriptors2 );

    cout << "descriptor matrix size: " << descriptors1.rows << " by " << descriptors1.cols << endl;

    // 2. Match the two image descriptors

    // Construction of the matcher
    //BruteForceMatcher< L2< float > > matcher;
    BFMatcher matcher( NORM_L2, false );

    // from image 1 to image 2
    // based on k nearest neighbours (with k=2)
    vector< vector< DMatch > > matches1;
    matcher.knnMatch( descriptors1,
                      descriptors2,
                      matches1,   // vector of matches (up to 2 per entry)
                      2 );		  // return 2 nearest neighbours

    // from image 2 to image 1
    // based on k nearest neighbours (with k=2)
    vector< vector< DMatch > > matches2;
    matcher.knnMatch( descriptors2,
                      descriptors1,
                      matches2,   // vector of matches (up to 2 per entry)
                      2 );		  // return 2 nearest neighbours

    cout << "Number of matched points 1->2: " << matches1.size() << endl;
    cout << "Number of matched points 2->1: " << matches2.size() << endl;

    // 3. Remove matches for which NN ratio is > than threshold

    // clean image 1 -> image 2 matches
    int removed = ratioTest( matches1 );
    cout << "Number of matched points 1->2 (ratio test) : " << matches1.size() - removed << endl;
    // clean image 2 -> image 1 matches
    removed = ratioTest( matches2 );
    cout << "Number of matched points 2->1 (ratio test) : " << matches2.size() - removed << endl;

    // 4. Remove non-symmetrical matches
    vector< DMatch > symMatches;
    symmetryTest( matches1, matches2, symMatches );

    cout << "Number of matched points (symmetry test): " << symMatches.size() << endl;

    if ( symMatches.size() == 0 ) {
        cerr << "Not enough matches." << endl;
        Mat fundamental;
        return( fundamental );
    }

    // 5. Validate matches using RANSAC
    Mat fundamental = ransacTest( symMatches, keypoints1, keypoints2, matches );

    // return the found fundamental matrix
    return( fundamental );
}
