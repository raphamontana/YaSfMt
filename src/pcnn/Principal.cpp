#include "Principal.h"

//Itti
TW w;
TCMap0 IM;
TRGBDouble Cor;
TCMap Mi, Mr, Mg, Mb, My, M0, M1, M2, M3;
TCMapArray Mrg, Mgr, Myb, Mby, Cm_i, Cm_c, Cm_o;
TCMapCS MCSi, MCSrg, MCSby, MCS0, MCS1, MCS2, MCS3;
TCMapCS MCSrg_t, MCSby_t;

TMat Neuronios[640][480];
ArrayNeuronios KNeuronios;

int tmp = 0;
int knL = 480;
int knC = 640;
int cont_it = 0;
int t_geral =0;
int nit = 0;
float fat_dec = 0.0;
int a = 10;
int b = 10;
float *p;
int max_seg = 0;
double saliency = 0.0, saliency_max = 0.0;
bool treina_som = false;
int num[10];
double desv_P = 0.0;
int z = 0;
double limiarF = 0.0;
double Wz = 0.0;
int jump[ 2 ];
double thetap = 0.0;
double thetae = 0.0;
double thetaz = 0.0;
double Wc = 1; /// W Cor
double Wi = 0; /// W Intencidade
double Wo = 0; /// W Orientação
double WSum = 1.0;
bool first_pulse = false;
int nl = 0, nc = 0;
unsigned char c_r = '\0', c_g = '\0', c_b = '\0';
int imin = 0, imax = 0, jmin = 0, jmax = 0;
bool cbWInt = false; /// Fazendo a busca por : intensidade
bool cbWCor = true;  /// Cor
bool cbWOri = false; /// Orientação

//gamb
int seNb = 3; //Depende de um componente da aba parametros
double seDPb = 10.0;

void Principal::IttiModel()
{
    int n = 0, a = 0, b = 0, i = 0, s = 0, c = 0, knl_t = 0, knc_t = 0, i_ = 0, j_ = 0;
    TCMapArray MCStmp;
    double maxloc_rg[ 6 ], maxloc_by[ 6 ];
    double medloc_rg[ 6 ], medloc_by[ 6 ];
    double medmaxglobal = 0.0;

    if ( seNb % 2 == 0 ) {
        seNb = seNb + 1;
    }
    n = seNb;
    desv_P = seDPb; //sigma

    //w = gaussian2d(n, desv_P); //Feito embaixo

/*================================================================================================================================================================================================*/

    double sigmatmp = 0.0, sigmatmp2 = 0.0;
    int sa = 0, ta = 0, X = 0, y = 0;

    sigmatmp = 2 * desv_P * desv_P;
    sigmatmp2 = 2 * M_PI * desv_P * desv_P;

    //Calculando a gaussiana
    sa = ( n - 1 ) / 2;
    ta = s;

    for ( X = - sa; X <= sa; X++) {
        for ( y = - ta; y <= ta; y++) {
            w[y + ta][ X + sa] = pow( exp( pow( - ( sqrt( X ) + sqrt( y ) ), 2 ) / ( sigmatmp ) ), 2 ) / ( sigmatmp2 );
            cout << w[y + ta][ X + sa] << " ";
        }
        cout << endl;
    }


/*=================================================================================================================================================================================================*/
    //1 - suavizacao da imagem de entrada com filtro gaussiano
    //nivel 0 da piramede
    a = ( n - 1 ) / 2;
    b = a;
    for ( int stop = knL - 1, i = 0; i <= stop; i++ ) {
        for ( int stop = knC - 1, j = 0; j <= stop; j++ ) {
            IM[0][i][ j][0] = 0; //R
            IM[0][i][ j][1] = 0; //G
            IM[0][i][ j][2] = 0; //B
            tmp = 0;
            for ( int stop = a, s = - a; s <= stop; s++ ) {
                for ( int stop = b, t = - b; t <= stop; t++ ) {
                    if ( ( ( i + s >= 0 ) && ( i + s <= knL - 1 ) ) && ( ( j + t >= 0 ) && ( j + t <= knC - 1 ) ) ) {
                        IM[0][i][ j][0] = IM[0][i][ j][0] + trunc( w[s + a][ t + b] * Neuronios[i + s][ j + t].rgb[0] );
                        IM[0][i][ j][1] = IM[0][i][ j][1] + trunc( w[s + a][ t + b] * Neuronios[i + s][ j + t].rgb[1] );
                        IM[0][i][ j][2] = IM[0][i][ j][2] + trunc( w[s + a][ t + b] * Neuronios[i + s][ j + t].rgb[2] );
                    }
                }
            }

            Cor.r = IM[0][i][j][0];
            Cor.g = IM[0][i][j][1];
            Cor.b = IM[0][i][j][2];

            Mr[0][i][ j] = trunc( pow( ( Cor.r - ( Cor.g + Cor.b ) ), 2 ) / 2 );
            Mg[0][i][ j] = trunc( pow( ( Cor.g - ( Cor.r + Cor.b ) ), 2 ) / 2 );
            Mb[0][i][ j] = trunc( pow( ( Cor.b - ( Cor.r + Cor.g ) ), 2 ) / 2 );
            My[0][i][ j] = trunc( ( Cor.r + Cor.g ) - ( 2 * ( fabs( Cor.r - Cor.g ) + Cor.b ) ) );
        }
    }

    //Passo 01 - criando a piramede gaussiana
    //niveis 1 a 8
    c = 1;
    knl_t = trunc( pow( knL, 2 ) / 2 );
    knc_t = trunc( pow( knC, 2 ) / 2 );
    do {
        for ( int stop = knl_t - 1, i = 0; i <= stop; i++ ) {
            for ( int stop = knc_t - 1, j = 0; j <= stop; j++ ) {
                //suavizando a gaussiana
                i_ = trunc( i * 2 ); //a = trunc(i*2)
                j_ = trunc( j * 2 ); //s = trunc(j*2)
                IM[c][i][ j][0] = 0;
                IM[c][i][ j][1] = 0;
                IM[c][i][ j][2] = 0;

                for ( int stop = a, s = - a; s <= stop; s++ ) {
                    for ( int stop = b, t = - b; t <= stop; t++ ) {
                        if ( ( ( i_ + s >= 0 ) && ( i_ + s <= ( knl_t * 2 ) - 1 ) ) && ( ( j_ + t >= 0 ) && ( j_ + t <= ( knc_t * 2 ) - 1 ) ) ){
                            IM[c][i][ j][0] = IM[c][i][ j][0] + trunc( w[s + a][ t + b] * IM[c - 1][i_ + s][ j_ + t][0] );
                            IM[c][i][ j][1] = IM[c][i][ j][1] + trunc( w[s + a][ t + b] * IM[c - 1][i_ + s][ j_ + t][1] );
                            IM[c][i][ j][2] = IM[c][i][ j][2] + trunc( w[s + a][ t + b] * IM[c - 1][i_ + s][ j_ + t][2] );
                        }
                    }
                }

                Cor.r = IM[c][i][ j][0]; //R
                Cor.g = IM[c][i][ j][1]; //G
                Cor.b = IM[c][i][ j][2]; //B

                Mr[c][i][ j] = trunc( pow( ( Cor.r - ( Cor.g + Cor.b ) ), 2 ) / 2 );
                Mg[c][i][ j] = trunc( pow( ( Cor.g - ( Cor.r + Cor.b ) ), 2 ) / 2 );
                Mb[c][i][ j] = trunc( pow( ( Cor.b - ( Cor.r + Cor.g ) ), 2 ) / 2 );
                My[c][i][ j] = trunc( ( Cor.r + Cor.g ) - ( 2 * ( fabs( Cor.r - Cor.g ) + Cor.b ) ) );
            }
        }

        c++;
        knl_t = trunc( pow( knl_t, 2 ) / 2 );
        knc_t = trunc( pow( knc_t, 2 ) / 2 );
    } while ( ! ( c == 9 ) );

    // Interpolação dos Canais c{1,2,3} s{2,3}
    b = 0;
    knl_t = trunc( ( pow( knL, 2 ) / 2 ) );
    knc_t = trunc( ( pow( knC, 2 ) / 2 ) );

    do {
        if ( b == 0 ) {
            //CORES
            //Color center-surround difference maps (r-g) (g - r)
            subtractionColor( Mrg, Mr[1], Mg[1], knl_t, knc_t );
            subtractionColor( Mgr, Mg[3], Mr[3], knl_t, knc_t );
            interpolation( Mgr, MCSrg[b], knl_t, knc_t, - 2 ); // 1 <-- 3
            subtraction( Mrg, MCSrg[b], knl_t, knc_t ); // 1 - 3

            // Color center-surround difference maps (y-b) - (b-y)
            subtractionColor( Myb, My[1], Mb[1], knl_t, knc_t );
            subtractionColor( Mby, Mb[3], My[3], knl_t, knc_t ); //1 <-- 3
            interpolation( Mby, MCSby[b], knl_t, knc_t, - 2 ); // 1 - 3
            subtraction( Myb, MCSby[b], knl_t, knc_t );

            b++;

            //Cores
            //Color center-surround difference maps (r-g) - (g-r)
            subtractionColor( Mrg, Mr[1], Mg[1], knl_t, knc_t );
            subtractionColor( Mgr, Mg[4], Mr[4], knl_t, knc_t );
            interpolation( Mgr, MCSrg[b], knl_t, knc_t, - 3 ); //1 <-- 4
            subtraction( Mrg, MCSrg[b], knl_t, knc_t ); //1 - 4

            // Color center-surround difference maps (y-b) - (b-y)
            subtractionColor( Myb, My[1], Mb[1], knl_t, knc_t );
            subtractionColor( Mby, Mb[4], My[4], knl_t, knc_t );
            interpolation( Mby, MCSby[b], knl_t, knc_t, - 3 ); //1 <-- 4
            subtraction( Myb, MCSby[b], knl_t, knc_t ); //1 - 4

        }
        if ( b == 2 ) {
            //CORES
            //Color center-surround difference maps (r-g) - (g-r)
            subtractionColor( Mrg, Mr[2], Mg[2], knl_t, knc_t );
            subtractionColor( Mgr, Mg[4], Mr[4], knl_t, knc_t );
            interpolation( Mgr, MCSrg[b], knl_t, knc_t, - 2 ); // 2 <-- 4
            subtraction( Mrg, MCSrg[b], knl_t, knc_t ); //2 - 4

            //Color center-surround difference maps (y-b) - (b-y)
            subtractionColor( Myb, My[2], Mb[2], knl_t, knc_t );
            subtractionColor( Mby, Mb[4], My[4], knl_t, knc_t );
            interpolation( Mby, MCSby[b], knl_t, knc_t, - 2 ); // 2 <--4
            subtraction( Myb, MCSby[b], knl_t, knc_t ); //2 - 4

            b++;

            //CORES
            // Color center-surround difference maps (r-g) - (g-r)
            subtractionColor( Mrg, Mr[2], Mg[2], knl_t, knc_t );
            subtractionColor( Mgr, Mg[5], Mr[5], knl_t, knc_t );
            interpolation( Mgr, MCSrg[b], knl_t, knc_t, - 3 ); // 2 <--5
            subtraction( Mrg, MCSrg[b], knl_t, knc_t ); // 2 - 5

            //Color center-surround difference maps (y-b) - (b-y)
            subtractionColor( Myb, My[2], Mb[2], knl_t, knc_t );
            subtractionColor( Mby, Mb[5], My[5], knl_t, knc_t );
            interpolation( Mby, MCSby[b], knl_t, knc_t, - 3 ); // 2 <-- 5
            subtraction( Myb, MCSby[b], knl_t, knc_t ); // 2 - 5

        }
        if ( b == 4 ) {
            // Color center-surround difference maps (r-g) - (g-r)
            subtractionColor( Mrg, Mr[3], Mg[3], knl_t, knc_t );
            subtractionColor( Mgr, Mg[5], Mr[5], knl_t, knc_t );
            interpolation( Mgr, MCSrg[b], knl_t, knc_t, - 2 ); // 3 <-- 5
            subtraction( Mrg, MCSrg[b], knl_t, knc_t ); // 3 - 5

            //Color center-surround difference maps (y-b) - (b-y)
            subtractionColor( Myb, My[3], Mb[3], knl_t, knc_t );
            subtractionColor( Mby, Mb[5], My[5], knl_t, knc_t );
            interpolation( Mby, MCSby[b], knl_t, knc_t, - 2 );// 3 <-- 5
            subtraction( Myb, MCSby[b], knl_t, knc_t ); // 3 - 5

            b++;

            //CORES
            // Color center-surround difference maps (r-g) - (g-r)
            subtractionColor( Mrg, Mr[3], Mg[3], knl_t, knc_t );
            subtractionColor( Mgr, Mg[6], Mr[6], knl_t, knc_t );
            interpolation( Mgr, MCSrg[b], knl_t, knc_t, - 3 );//3 <-- 6
            subtraction( Mrg, MCSrg[b], knl_t, knc_t ); //3 - 6

            //Color center-surround difference maps (y-b) - (b-y)
            subtractionColor( Myb, My[3], Mb[3], knl_t, knc_t );
            subtractionColor( Mby, Mb[6], My[6], knl_t, knc_t );
            interpolation( Mby, MCSby[b], knl_t, knc_t, - 3 ); //3 <--6
            subtraction( Myb, MCSby[b], knl_t, knc_t ); // 3 - 6
        }

        b++;
        knl_t = trunc( pow( knl_t, 2 ) / 2 );
        knc_t = trunc( pow( knc_t,2  ) / 2 );
    } while ( ! ( b >= 5 ) );

    // Geração dos Mapas de Conspicuidades ---------------------------------------
    // Canais c{1,2,3} s{2,3}
    // Interpolação de todos para o nível 3
    b = 0;
    knl_t = trunc( ( pow( knL, 2 ) / pow( 2, 3 ) ) );
    knc_t = trunc( ( pow( knC, 2 ) / pow( 2, 3 ) ) );
    do {
        switch ( b ) {
            case 0:
            case 1:
                //MCStmp array assignment invalid??? Todos os 6
                //MCStmp = MCSrg[b];
                interpolation( MCStmp, MCSrg_t[b], knl_t, knc_t, 2 ); // 3 <-- 1
                //MCStmp = MCSby[b];
                interpolation( MCStmp, MCSby_t[b], knl_t, knc_t, 2 ); // 3 <--  1
                break;
            case 2:
            case 3:
                //MCStmp = MCSrg[b];
                interpolation( MCStmp, MCSrg_t[b], knl_t, knc_t, 1 ); // 3 <-- 2
                //MCStmp = MCSby[b];
                interpolation( MCStmp, MCSby_t[b], knl_t, knc_t, 1 ); // 3 <-- 2
                break;
            case 4:
            case 5:
                // MCStmp = MCSrg[b];
                interpolation( MCStmp, MCSrg_t[b], knl_t, knc_t, 0 ); // 3 <-- 3
                // MCStmp = MCSby[b];
                interpolation( MCStmp, MCSby_t[b], knl_t, knc_t, 0 ); // 3 <-- 3
                break;
            default: break;
        }
        b++;
    } while ( ! ( b == 6 ) );

    //Across-scale addition
    knl_t = trunc( ( pow( knL, 2 ) / pow( 2, 3 ) ) );
    knc_t = trunc( ( pow( knC, 2 ) / pow( 2, 3 ) ) );
    b = 0;
    medmaxglobal = 0;

    do {
        maxloc_rg[b] = 0;
        maxloc_by[b] = 0;
        medloc_rg[b] = 0;
        medloc_by[b] = 0;
        for ( int stop = knl_t - 1, i = 0; i <= stop; i++ ) {
            for ( int stop = knc_t - 1, j = 0; j <= stop; j++ ) {
                //Maps global maximum
                if ( maxloc_rg[b] < MCSrg_t[b][i][j] ) {
                    maxloc_rg[b] = MCSrg_t[b][i][j];
                }
                if ( maxloc_by[b] < MCSby_t[b][i][j] ) {
                    maxloc_by[b] = MCSby_t[b][i][ j];
                }
            }
        }

        medmaxglobal = medmaxglobal + ( maxloc_rg[b] + maxloc_by[b] );

        medloc_rg[b] = medloc_rg[i] + maxloc_rg[b];
        medloc_by[b] = medloc_by[i] + maxloc_by[b];

        b++;
    } while ( ! ( b == 6 ) );

    medmaxglobal = pow( medmaxglobal, 2 ) / 12;
    // Multiply the map with (M-m)^2.
    b = 0;
    do {
        for ( int stop = knl_t - 1, i = 0; i <= stop; i++ ) {
            for ( int stop = knc_t - 1, j = 0; j <= stop; j++ ) {
                MCSrg_t[b][i][ j] = MCSrg_t[b][i][ j] * sqrt( maxloc_rg[b] - ( pow( medloc_rg[b], 2 ) / 6 ) );
                MCSby_t[b][i][ j] = MCSby_t[b][i][ j] * sqrt( maxloc_by[b] - ( pow( medloc_by[b], 2 ) / 6 ) );

                //simplesmente zerando
                if ( b == 0 ){
                    Cm_i[i][ j] = 0;
                    Cm_c[i][ j] = 0;
                    Cm_o[i][ j] = 0;
                }
            }
        }
        b++;
    } while ( ! ( b == 6 ) );

    b = 0;
    do {
        normalization( MCSrg_t[b], knl_t, knc_t );
        normalization( MCSby_t[b], knl_t, knc_t );
        b++;
    } while ( ! ( b == 6 ) );

    b = 0;
    do {
        for ( int stop = knl_t - 1, i = 0; i <= stop; i++ ) {
            for ( int stop = knc_t - 1, j = 0; j <= stop; j++ ) {
                Cm_c[i][j] = Cm_c[i][j] + (  MCSrg_t[b][i][j] + MCSby_t[b][i][j] );
            }
        }
        b++;
    } while ( ! ( b == 6 ) );
    normalization( Cm_c, knl_t, knc_t );

    //Interpolaçao para o tamanho da imagem de entrada
    //Invalid array assigment
    //MCStmp = Cm_c;
    interpolation( MCStmp, Cm_c, knL, knC, - 3 );//3 <-- 3
}

//Inicialização da variável Neuronios
void Principal::convertMatToNeuron( Mat image )
{
    if ( image.rows > 480 || image.cols > 640 ) {
        cerr << "Image is too large." << endl;
        return;
    }
    default_random_engine generator;
    uniform_int_distribution<int> distribution( 0, 1000 );
    auto random = std::bind ( distribution, generator );

    //Popular a matriz Neuronio com as informaçoes da imagem.
    for ( int i = 0; i < image.rows; i++ ) {
        for ( int j = 0; j < image.cols; j++ ) {
            Vec3b pixel = image.at<Vec3b>( j, i );
            unsigned char r = pixel[2];
            unsigned char g = pixel[1];
            unsigned char b = pixel[0];
            Neuronios[i][j].rgb[0] = r;
            Neuronios[i][j].rgb[1] = g;
            Neuronios[i][j].rgb[2] = b;
            int pixelI = ( r + g + b ) / 3;
            Neuronios[i][j].rgb[3] = pixelI;
            Neuronios[i][j].rgb[4] = pixelI;
            Neuronios[i][j].rgbSColor[0] = r;
            Neuronios[i][j].rgbSColor[1] = g;
            Neuronios[i][j].rgbSColor[2] = b;
            Neuronios[i][j].v = random();
            Neuronios[i][j].p = 0;
            Neuronios[i][j].d = 0;
            Neuronios[i][j].pulsar = !( r == 0 && g == 0 && b == 0 );
            Neuronios[i][j].l = i;
            Neuronios[i][j].c = j;
            Neuronios[i][j].rn = 0;
            Neuronios[i][j].segN = -1;
        }
    }
}

Mat Principal::convertNeuronToMat( Mat imagem )
{
    //Popular a matriz Neuronio com as informaçoes da imagem.
    for ( int i = 0; i < imagem.rows; i++ ) {
        for ( int j = 0; j < imagem.cols; j++ ) {
            //Vec3b pixel = imagem.at<Vec3b>( j, i );
            unsigned char r = Neuronios[i][j].rgb[0];
            unsigned char g = Neuronios[i][j].rgb[1];
            unsigned char b = Neuronios[i][j].rgb[2];
            imagem.at<Vec3b>( j, i )[2] = r;
            imagem.at<Vec3b>( j, i )[1] = g;
            imagem.at<Vec3b>( j, i )[0] = b;
        }
    }
    return( imagem );
}

double Principal::WeightsConnections( int i, int j )
{
    double Res = 0.0;
    int n = 0;

    if ( cbWInt ) {
        n++;
        Res += pow( Neuronios[i][j].rgb_Itti[0], 2 ) / 255 * Wi;
    }

    if ( cbWCor ) {
        n++;
        Res += pow( Neuronios[i][j].rgb_Itti[1], 2 ) / 255 * Wc;
    }

    if ( cbWOri ) {
        n++;
        Res += pow( Neuronios[i][j].rgb_Itti[2], 2 ) / 255.0 * Wo;
    }

    return( Res / n );
}

void Principal::Select() {
    int i = 0, j = 0, max = 0, imax = 0, jmax = 0;
    max = - 1;
    for ( i = 0; i < knL; i++ ) {
        for ( j = 0; j < knC; j++ ) {
            Neuronios[i][j].trigger = 0;
            if ( ( Neuronios[i][j].lateral > thetap ) && ( Neuronios[i][j].ly != MAXIMO ) && ( Neuronios[i][j].ly > max ) ) {
                if ( WeightsConnections( i, j ) >= WSum ) {
                    max = Neuronios[i][j].ly;
                    imax = i;
                    jmax = j;
                }
            }
        }
    }
    if ( ( imax == 0 ) && ( jmax == 0 ) && ( WSum > 0 ) ) {
        for ( i = 0; i < knL; i++ ) {
            for ( j = 0; j < knC; j++ ) {
                Neuronios[i][ j].d = 1;
                Neuronios[i][ j].lx[1] = LP;
                Neuronios[i][ j].ly = - 1;
                Neuronios[i][ j].cycle = 1;
                z = 1;
            }
        }
    }

    if ( Neuronios[imax][jmax].cycle > 0 ) {
        for ( i = 0; i <= knL; i++)
            for ( j = 0; j <= knC; j++)
                if ( ( Neuronios[i][ j].ly == max ) && ( Neuronios[i][ j].cycle > 0 ) )
                    Neuronios[i][ j].trigger = 1;
    }
    else {
        Neuronios[imax][jmax].trigger = 1;
    }
    for ( int stop = knL - 1, i = 0; i <= stop; i++ ) {
        for ( int stop1 = knC - 1, j = 0; j <= stop1; j++ ) {
            if ( ( Neuronios[i][j].ly != MAXIMO ) && ( ( Neuronios[i][j].cycle > 0 ) || ( Neuronios[i][j].lateral > thetap ) ) ) {
                Neuronios[i][j].ly = Neuronios[i][j].ly + ( MAXIMO - max );
                Neuronios[i][j].lx[0] = pow( Neuronios[i][j].ly, 2 ) / MAXVAL;
            }
        }
    }
    c_r = Neuronios[imax][jmax].rgbSColor[0];
    c_g = Neuronios[imax][jmax].rgbSColor[1];
    c_b = Neuronios[imax][jmax].rgbSColor[2];
}

double Principal::Sim_Cor( int vl, int vc, int hl, int hc )
{
    double r = 0.0;
    for ( int stop = 2, i = 0; i <= stop; i++ ) {
        r += fabs( Neuronios[vl][vc].rgbSColor[i] - Neuronios[hl][hc].rgbSColor[i] );
    }
    return( pow( 255, 2 ) / ( 1 + r ) );
}

void Principal::Connections( )
{
    for ( int stop = knL - 1, i = 0; i <= stop; i++ ) {
        for ( int stop = knC - 1, j = 0; j <= stop; j++) {
            Neuronios[i][j].w[0] = 1.0;
            Neuronios[i][j].w[1] = 2.0;
            Neuronios[i][j].w[2] = 1.0;
            Neuronios[i][j].w[3] = 1.2;
            Neuronios[i][j].w[4] = 5.0;
            Neuronios[i][j].w[5] = 8.0;
            Neuronios[i][j].w[6] = 8.0;
            Neuronios[i][j].w[7] = 1.0;
            Neuronios[i][j].lateral = 0;
        }
    }
    for ( int stop = knL - 1, i = 0; i <= stop; i++ ) {
        for ( int stop = knC - 1, j = 0; j <= stop; j++ ) {
            if ( Neuronios[i][ j].stim >= limiarF ) {
                if ( j - 1 >= 0 )
                    Neuronios[i][j].w[0] = Sim_Cor( i, j, i, j - 1 );
                if ( ( i - 1 >= 0 ) && ( j - 1 >= 0 ) )
                    Neuronios[i][j].w[1] = Sim_Cor( i, j, i - 1, j - 1 );
                if ( i - 1 >= 0 )
                    Neuronios[i][j].w[2] = Sim_Cor( i, j, i - 1, j );
                if ( ( i - 1 >= 0 ) && ( j + 1 < knC ) )
                    Neuronios[i][j].w[3] = Sim_Cor( i, j, i - 1, j + 1 );
                if ( j + 1 < knC )
                    Neuronios[i][j].w[4] = Sim_Cor( i, j, i, j + 1 );
                if ( ( i + 1 < knL ) && ( j + 1 < knC ) )
                    Neuronios[i][j].w[5] = Sim_Cor( i, j, i + 1, j + 1 );
                if ( i + 1 < knL )
                    Neuronios[i][j].w[6] = Sim_Cor( i, j, i + 1, j );
                if ( ( i + 1 < knL ) && ( j - 1 >= 0 ) )
                    Neuronios[i][j].w[7] = Sim_Cor( i, j, i + 1, j - 1 );
                for ( int stop = 8, k = 0; k <= stop; k++)
                    Neuronios[i][j].lateral = Neuronios[i][j].lateral + Neuronios[i][j].w[k];
            }
        }
    }
}

void Principal::normalization( TCMapArray& d, int knl_t, int knc_t )
{
    int i = 0, j = 0;
    double fmin = 0.0, fmax = 0.0;
    fmin = 255;
    for ( i = 0; i <= knl_t; i++ ) {
        for ( j = 0; j <= knc_t; j++ ) {
            if ( fmin > d[i][j] ) {
                fmin = d[i][j];
            }
        }
    }
    fmax = 0;
    for ( i = 0; i <= knl_t; i++) {
        for ( j = 0; j <= knc_t; j++ ) {
            d[i][j] = d[i][j] - fmin;
            if ( fmax < d[i][j] ) {
                fmax = d[i][j];
            }
        }
    }
    for ( i = 0; i <= knl_t; i++) {
        for ( j = 0; j <= knc_t; j++) {
            if ( fmax != 0 ) {
                d[i][j] = ( trunc( 255 * ( d[i][j] / fmax ) ) );
            }
        }
    }
}


void Principal::subtraction( TCMapArray& O, TCMapArray& d, int knl_t, int knc_t )
{
    int i = 0, j = 0;
    for ( i = 0; i < knl_t; i++) {
        for ( j = 0; j < knc_t; j++) {
            d[i][ j] = fabs( O[i][j] - d[i][j] );
        }
    }
    normalization( d, knl_t, knc_t );
}

void Principal::interpolation( TCMapArray& O, TCMapArray& d, int knl_t, int knc_t, double f )
{
    int i = 0, j = 0, a = 0, s = 0;
    for ( i = 0; i < knl_t; i++) {
        for ( j = 0; j < knc_t; j++) {
            if ( f == 0 ) {
                a = i;
                s = j;
            }
            else
                if ( f < 0 ) {
                    a = fabs( trunc( i / ( pow( 2, fabs( f ) ) ) ) );
                    s = fabs( trunc( j / ( pow( 2, fabs( f ) ) ) ) );
                }
                else {
                    a = trunc( i * pow( 2, f ) );
                    s = trunc( j * pow( 2, f ) );
                }
            d[i][j] = O[a][s];
        }
    }
}

void Principal::subtractionColor( TCMapArray& r, TCMapArray& O, TCMapArray& d, int knl_t, int knc_t )
{
    for ( int i = 0; i < knl_t; i++ ) {
        for ( int j = 0; j < knc_t; j++ ) {
            r[i][j] = fabs( O[i][j] - d[i][j] );
        }
    }
}

void Principal::Saliencia( Mat image )
{
    convertMatToNeuron( image );
    IttiModel();

    // Variáveis locais e globais
    double pixelI = 0.0;
    int i = 0, j = 0, a = 0, b = 0, s = 0, c = 0, i_ = 0, j_ = 0;
    int knl_t = 0, knc_t = 0;
    double maxi = 0.0;

    // Variáveis locais para o LEGION
    // Primeiro pulso
    int count = 0;
    double temp = 0.0, temp2 = 0.0, temp3 = 0.0;

    //MLP
    int n = 0;

    TCMapArray MCStmp;
    double maxloc_rg[ 6 ], maxloc_by[ 6 ];
    double medloc_rg[ 6 ], medloc_by[ 6 ];
    double medmaxglobal = 0.0;

                                                        // INICIO DA FUNÇÃO //

    //Variaveis que deveriam ter sido pegas na interface
    knL = 480;
    knC = 640;
    Wc = 1;
    Wi = 0;
    Wo = 0;

    //Verificando numero de linhas e colunas
    if ( ( knL > MaxKnl ) || ( knC > MaxKnc ) ) {
        cout << "Imagem com dimensões não suportadas";
        return;
    }

    cont_it = 0;

     /*==================================================*/
    t_geral = 0;
    fat_dec = 0.9;
    nit = ( knL - 1 ) * ( knC - 1 );

    //Retirei 2 coisas que provavelmente sao de interface
    //Talvez essas variáveis sejam de interface tbm
    max_seg = - 1;
    saliency = 0;
    saliency_max = 0;


    //Carregamento da base MLP ===> Importante, aqui ele busca da interface
    treina_som = false;

    //Inicializando neuronios do SOM
    //randomize;

    for ( int stop = knL - 1, i = 0; i <= stop; i++ ) {
        for ( int stop = knC - 1, j = 0; j <= stop; j++ ) {
            //Intencidades RG e BY (SOM)
            if ( true ) { //cbExp4_Color.Checked
                KNeuronios[i][ j][0] = rand() % 256;
                KNeuronios[i][ j][1] = rand() % 256;
                KNeuronios[i][ j][2] = rand() % 256;
            } else {
                KNeuronios[i][ j][0] = rand() % 256;
                KNeuronios[i][ j][1] = KNeuronios[i][j][0];
                KNeuronios[i][ j][2] = KNeuronios[i][j][0];
            }

            //Posicoes iniciais do mapa (SOM)
            KNeuronios[i][ j][3] = rand() % knL;
            KNeuronios[i][ j][4] = rand() % knC;

            //Angulos inicias do mapa (SOM)
            KNeuronios[i][ j][5] = rand() % 256;
            KNeuronios[i][ j][6] = rand() % 256;
            KNeuronios[i][ j][7] = rand() % 256;
            KNeuronios[i][ j][8] = rand() % 256;

            //Reconhecimento (SOM)
            KNeuronios[i][ j][9] = 0;

            // R, G, B (SOM)
            KNeuronios[i][ j][10] = rand() % 256;
            KNeuronios[i][ j][11] = rand() % 256;
            KNeuronios[i][ j][12] = rand() % 256;

        }
    }

    //Onde é inicializadas essas variáveis? Nao tem nada em Neuronios
    for ( i = 0; i < knL; i++ ) {
        for ( j = 0; j <= knC; j++ ) {
            //Orientacoes
            Neuronios[i][j].ang[0] = Neuronios[i][ j].rgb[4];
            Neuronios[i][j].ang[1] = '0'; //Gradiente na direcao Horizontal
            Neuronios[i][j].ang[2] = '0'; //Gradiente na direcao vertical
            Neuronios[i][j].ang[3] = '0'; //Gradiente na direcao +45
            Neuronios[i][j].ang[4] = '0'; //Gradiente na direcao -45

            //cout << Neuronios[i][j].ang[3] << " ";

            //ADAM MUDOU Verificar
            Neuronios[i][j].rgb[4] = '2';
            Neuronios[i][j].rgb_Itti[1] = '3';
            Neuronios[i][j].rgb_Itti[2] = '4';
            Neuronios[i][j].rgbSColor[0] = '6';
            Neuronios[i][j].rgbSColor[1] = '2';
            Neuronios[i][j].rgbSColor[2] = '4';
            //Fim verificar

            //I RG BY
            Neuronios[i][ j].rgb_Itti[0] = Neuronios[i][ j].rgb[4]; //Itti intencidades
            pixelI = 0;
            maxi = max( Neuronios[i][ j].rgbSColor[0], max( Neuronios[i][ j].rgbSColor[1], Neuronios[i][ j].rgbSColor[2] ) );
            if ( maxi > 0 )
               pixelI = ( double( ( fabs( Neuronios[i][ j].rgbSColor[0] - Neuronios[i][ j].rgbSColor[1] ) ) ) / maxi );
            Neuronios[i][ j].rgb_Itti[1] = trunc( pixelI * 255 );
            pixelI = 0;
            if ( maxi > 0 )
              pixelI = ( double( ( fabs( Neuronios[i][ j].rgbSColor[2] - min( Neuronios[i][ j].rgbSColor[0], Neuronios[i][ j].rgbSColor[1] ) ) ) ) / maxi );
            Neuronios[i][ j].rgb_Itti[2] = trunc( pixelI * 255 );
            //cout << Neuronios[i][j].rgb_Itti[2];
        }
    }

    //Eh aqui que a porca torce o rabo
    //Criacao do template para o filtro Gaussiano com tamanho e desvio padrao ---
    //Isso é importante, mas tbm pega da interface
    if ( seNb % 2 == 0 ) {
      seNb = seNb + 1;
    }
    n = seNb;
    desv_P = seDPb; //sigma

    //w = gaussian2d(n, desv_P);

/*================================================================================================================================================================================================*/

    double sigmatmp = 0.0, sigmatmp2 = 0.0;
    int sa = 0, ta = 0, X = 0, y = 0;

    sigmatmp = 2 * desv_P * desv_P;
    sigmatmp2 = 2 * M_PI * desv_P * desv_P;

    //Calculando a gaussiana
    sa = ( n - 1 ) / 2;
    ta = s;

    for ( X = - sa; X <= sa; X++ ) {
        for ( y = - ta; y <= ta; y++ ) {
            w[y + ta][ X + sa] = double( exp( double( - ( sqrt( X ) + sqrt( y ) ) ) / ( sigmatmp ) ) ) / ( sigmatmp2 );
        }
    }


/*=================================================================================================================================================================================================*/
    //1 - suavizacao da imagem de entrada com filtro gaussiano
    //nivel 0 da piramede
    a = ( n - 1 ) / 2;
    b = a;
    for ( int stop = knL - 1, i = 0; i <= stop; i++ ) {
        for ( int stop = knC - 1, j = 0; j <= stop; j++ ) {
            IM[0][i][ j][0] = 0; //R
            IM[0][i][ j][1] = 0; //G
            IM[0][i][ j][2] = 0; //B
            tmp = 0;
            for ( int stop = a, s = - a; s <= stop; s++ ) {
                for ( int stop = b, t = - b; t <= stop; t++){
                    if ( ( ( i + s >= 0 ) && ( i + s <= knL - 1 ) ) && ( ( j + t >= 0 ) && ( j + t <= knC - 1 ) ) ) {
                        IM[0][i][ j][0] = IM[0][i][ j][0] + trunc( w[s + a][ t + b] * Neuronios[i + s][ j + t].rgb[0] );
                        IM[0][i][ j][1] = IM[0][i][ j][1] + trunc( w[s + a][ t + b] * Neuronios[i + s][ j + t].rgb[1] );
                        IM[0][i][ j][2] = IM[0][i][ j][2] + trunc( w[s + a][ t + b] * Neuronios[i + s][ j + t].rgb[2] );
                    }
                }
            }

            Cor.r = IM[0][i][j][0];
            Cor.g = IM[0][i][j][1];
            Cor.b = IM[0][i][j][2];

            Mr[0][i][ j] = trunc( double( ( Cor.r - ( Cor.g + Cor.b ) ) ) / 2 );
            Mg[0][i][ j] = trunc( double( ( Cor.g - ( Cor.r + Cor.b ) ) ) / 2 );
            Mb[0][i][ j] = trunc( double( ( Cor.b - ( Cor.r + Cor.g ) ) ) / 2 );
            My[0][i][ j] = trunc( ( Cor.r + Cor.g ) - ( 2 * ( fabs( Cor.r - Cor.g ) + Cor.b ) ) );
        }
    }

    //Passo 01 - criando a piramede gaussiana
    //niveis 1 a 8
    c = 1;
    knl_t = trunc( double( knL ) / 2 );
    knc_t = trunc( double( knC ) / 2 );
    do {
        for ( int stop = knl_t - 1, i = 0; i <= stop; i++ ) {
            for ( int stop = knc_t - 1, j = 0; j <= stop; j++ ) {
                //suavizando a gaussiana
                i_ = trunc( i * 2 ); //a = trunc(i*2)
                j_ = trunc( j * 2 ); //s = trunc(j*2)
                IM[c][i][ j][0] = 0;
                IM[c][i][ j][1] = 0;
                IM[c][i][ j][2] = 0;

                for ( int stop = a, s = - a; s <= stop; s++ ) {
                    for ( int stop = b, t = - b; t <= stop; t++ ) {
                        if ( ( ( i_ + s >= 0 ) && ( i_ + s <= ( knl_t * 2 ) - 1 ) ) && ( ( j_ + t >= 0 ) && ( j_ + t <= ( knc_t * 2 ) - 1 ) ) ) {
                            IM[c][i][ j][0] = IM[c][i][ j][0] + trunc( w[s + a][ t + b] * IM[c - 1][i_ + s][ j_ + t][0] );
                            IM[c][i][ j][1] = IM[c][i][ j][1] + trunc( w[s + a][ t + b] * IM[c - 1][i_ + s][ j_ + t][1] );
                            IM[c][i][ j][2] = IM[c][i][ j][2] + trunc( w[s + a][ t + b] * IM[c - 1][i_ + s][ j_ + t][2] );
                        }
                    }
                }

                Cor.r = IM[c][i][ j][0]; //R
                Cor.g = IM[c][i][ j][1]; //G
                Cor.b = IM[c][i][ j][2]; //B

                Mr[c][i][ j] = trunc( double( ( Cor.r - ( Cor.g + Cor.b ) ) ) / 2 );
                Mg[c][i][ j] = trunc( double( ( Cor.g - ( Cor.r + Cor.b ) ) ) / 2 );
                Mb[c][i][ j] = trunc( double( ( Cor.b - ( Cor.r + Cor.g ) ) ) / 2 );
                My[c][i][ j] = trunc( ( Cor.r + Cor.g ) - ( 2 * ( fabs( Cor.r - Cor.g ) + Cor.b ) ) );
            }
        }

      c++;
      knl_t = trunc( double( knl_t ) / 2 );
      knc_t = trunc( double( knc_t ) / 2 );
    } while ( ! ( c == 9 ) );

    // Interpolação dos Canais c{1,2,3} s{2,3}
    b = 0;
    knl_t = trunc( ( double( knL ) / 2 ) );
    knc_t = trunc( ( double( knC ) / 2 ) );

    do {
        if ( b == 0 ) {
            //CORES
            //Color center-surround difference maps (r-g) (g - r)
            subtractionColor( Mrg, Mr[1], Mg[1], knl_t, knc_t );
            subtractionColor( Mgr, Mg[3], Mr[3], knl_t, knc_t );
            interpolation( Mgr, MCSrg[b], knl_t, knc_t, - 2 ); // 1 <-- 3
            subtraction( Mrg, MCSrg[b], knl_t, knc_t ); // 1 - 3

            // Color center-surround difference maps (y-b) - (b-y)
            subtractionColor( Myb, My[1], Mb[1], knl_t, knc_t );
            subtractionColor( Mby, Mb[3], My[3], knl_t, knc_t ); //1 <-- 3
            interpolation( Mby, MCSby[b], knl_t, knc_t, - 2 ); // 1 - 3
            subtraction( Myb, MCSby[b], knl_t, knc_t );

            b++;

            //Cores
            //Color center-surround difference maps (r-g) - (g-r)
            subtractionColor( Mrg, Mr[1], Mg[1], knl_t, knc_t );
            subtractionColor( Mgr, Mg[4], Mr[4], knl_t, knc_t );
            interpolation( Mgr, MCSrg[b], knl_t, knc_t, - 3 ); //1 <-- 4
            subtraction( Mrg, MCSrg[b], knl_t, knc_t ); //1 - 4

            // Color center-surround difference maps (y-b) - (b-y)
            subtractionColor( Myb, My[1], Mb[1], knl_t, knc_t );
            subtractionColor( Mby, Mb[4], My[4], knl_t, knc_t );
            interpolation( Mby, MCSby[b], knl_t, knc_t, - 3 ); //1 <-- 4
            subtraction( Myb, MCSby[b], knl_t, knc_t ); //1 - 4
        }
        if ( b == 2 ) {
            //CORES
            //Color center-surround difference maps (r-g) - (g-r)
            subtractionColor( Mrg, Mr[2], Mg[2], knl_t, knc_t );
            subtractionColor( Mgr, Mg[4], Mr[4], knl_t, knc_t );
            interpolation( Mgr, MCSrg[b], knl_t, knc_t, - 2 ); // 2 <-- 4
            subtraction( Mrg, MCSrg[b], knl_t, knc_t ); //2 - 4

            //Color center-surround difference maps (y-b) - (b-y)
            subtractionColor( Myb, My[2], Mb[2], knl_t, knc_t );
            subtractionColor( Mby, Mb[4], My[4], knl_t, knc_t );
            interpolation( Mby, MCSby[b], knl_t, knc_t, - 2 ); // 2 <--4
            subtraction( Myb, MCSby[b], knl_t, knc_t ); //2 - 4

            b++;

            //CORES
            // Color center-surround difference maps (r-g) - (g-r)
            subtractionColor( Mrg, Mr[2], Mg[2], knl_t, knc_t );
            subtractionColor( Mgr, Mg[5], Mr[5], knl_t, knc_t );
            interpolation( Mgr, MCSrg[b], knl_t, knc_t, - 3 ); // 2 <--5
            subtraction( Mrg, MCSrg[b], knl_t, knc_t ); // 2 - 5

            //Color center-surround difference maps (y-b) - (b-y)
            subtractionColor( Myb, My[2], Mb[2], knl_t, knc_t );
            subtractionColor( Mby, Mb[5], My[5], knl_t, knc_t );
            interpolation( Mby, MCSby[b], knl_t, knc_t, - 3 ); // 2 <-- 5
            subtraction( Myb, MCSby[b], knl_t, knc_t ); // 2 - 5
        }
        if ( b == 4 ) {

            // Color center-surround difference maps (r-g) - (g-r)
            subtractionColor( Mrg, Mr[3], Mg[3], knl_t, knc_t );
            subtractionColor( Mgr, Mg[5], Mr[5], knl_t, knc_t );
            interpolation( Mgr, MCSrg[b], knl_t, knc_t, - 2 ); // 3 <-- 5
            subtraction( Mrg, MCSrg[b], knl_t, knc_t ); // 3 - 5

            //Color center-surround difference maps (y-b) - (b-y)
            subtractionColor( Myb, My[3], Mb[3], knl_t, knc_t );
            subtractionColor( Mby, Mb[5], My[5], knl_t, knc_t );
            interpolation( Mby, MCSby[b], knl_t, knc_t, - 2 );// 3 <-- 5
            subtraction( Myb, MCSby[b], knl_t, knc_t ); // 3 - 5

            b++;

            //CORES
            // Color center-surround difference maps (r-g) - (g-r)
            subtractionColor( Mrg, Mr[3], Mg[3], knl_t, knc_t );
            subtractionColor( Mgr, Mg[6], Mr[6], knl_t, knc_t );
            interpolation( Mgr, MCSrg[b], knl_t, knc_t, - 3 );//3 <-- 6
            subtraction( Mrg, MCSrg[b], knl_t, knc_t ); //3 - 6

            //Color center-surround difference maps (y-b) - (b-y)
            subtractionColor( Myb, My[3], Mb[3], knl_t, knc_t );
            subtractionColor( Mby, Mb[6], My[6], knl_t, knc_t );
            interpolation( Mby, MCSby[b], knl_t, knc_t, - 3 ); //3 <--6
            subtraction( Myb, MCSby[b], knl_t, knc_t ); // 3 - 6
        }

        b++;
        knl_t = trunc( double( knl_t ) / 2 );
        knc_t = trunc( double( knc_t ) / 2 );
    } while ( ! ( b >= 5 ) );

    // Geração dos Mapas de Conspicuidades ---------------------------------------
    // Canais c{1,2,3} s{2,3}
    // Interpolação de todos para o nível 3
    b = 0;
    knl_t = trunc( ( double( knL ) / pow( 2, 3 ) ) );
    knc_t = trunc( ( double( knC ) / pow( 2, 3 ) ) );
    do {
        switch ( b ) {
            case 0:
            case 1:
            {
                //MCStmp array assignment invalid??? Todos os 6
                //MCStmp = MCSrg[b];
                interpolation( MCStmp, MCSrg_t[b], knl_t, knc_t, 2 ); // 3 <-- 1
                //MCStmp = MCSby[b];
                interpolation( MCStmp, MCSby_t[b], knl_t, knc_t, 2 ); // 3 <--  1
            }
            break;

            case 2:
            case 3:
            {
                //MCStmp = MCSrg[b];
                interpolation( MCStmp, MCSrg_t[b], knl_t, knc_t, 1 ); // 3 <-- 2
                //MCStmp = MCSby[b];
                interpolation( MCStmp, MCSby_t[b], knl_t, knc_t, 1 ); // 3 <-- 2
            }
            break;

            case 4:
            case 5:
            {
                // MCStmp = MCSrg[b];
                interpolation( MCStmp, MCSrg_t[b], knl_t, knc_t, 0 ); // 3 <-- 3
                // MCStmp = MCSby[b];
                interpolation( MCStmp, MCSby_t[b], knl_t, knc_t, 0 ); // 3 <-- 3
            }
            break;
        }
        b++;
    } while ( ! ( b == 6 ) );

    //Across-scale addition
    knl_t = trunc( ( double( knL ) / pow( 2, 3 ) ) );
    knc_t = trunc( ( double( knC ) / pow( 2, 3 ) ) );
    b = 0;
    medmaxglobal = 0;

    do {
        maxloc_rg[b] = 0;
        maxloc_by[b] = 0;
        medloc_rg[b] = 0;
        medloc_by[b] = 0;
        for ( int stop = knl_t - 1, i = 0; i <= stop; i++ ) {
            for ( int stop = knc_t - 1, j = 0; j <= stop; j++ ) {
                //Maps global maximum
                if ( maxloc_rg[b] < MCSrg_t[b][i][ j] ) {
                    maxloc_rg[b] = MCSrg_t[b][i][ j];
                }
                if ( maxloc_by[b] < MCSby_t[b][i][ j] ) {
                    maxloc_by[b] = MCSby_t[b][i][ j];
                }
            }
        }

        medmaxglobal = medmaxglobal + ( maxloc_rg[b] + maxloc_by[b] );

        medloc_rg[b] = medloc_rg[i] + maxloc_rg[b];
        medloc_by[b] = medloc_by[i] + maxloc_by[b];

        b++;
    } while ( ! ( b == 6 ) );

    medmaxglobal = double( medmaxglobal ) / 12;
    // Multiply the map with (M-m)^2.
    b = 0;
    do {
        for ( int stop = knl_t - 1, i = 0; i <= stop; i++) {
            for ( int stop = knc_t - 1, j = 0; j <= stop; j++) {
                MCSrg_t[b][i][ j] = MCSrg_t[b][i][ j] * sqrt( maxloc_rg[b] - ( double( medloc_rg[b] ) / 6 ) );
                MCSby_t[b][i][ j] = MCSby_t[b][i][ j] * sqrt( maxloc_by[b] - ( double( medloc_by[b] ) / 6 ) );

                //simplesmente zerando
                if ( b == 0 ) {
                    Cm_i[i][ j] = 0;
                    Cm_c[i][ j] = 0;
                    Cm_o[i][ j] = 0;
                }
            }
        }
        b++;
    } while ( ! ( b == 6 ) );

    b = 0;
    do {
        normalization( MCSrg_t[b], knl_t, knc_t );
        normalization( MCSby_t[b], knl_t, knc_t );
        b++;
    } while ( ! ( b == 6 ) );

    b = 0;
    do {
        for ( int stop = knl_t - 1, i = 0; i <= stop; i++ ) {
            for ( int stop = knc_t - 1, j = 0; j <= stop; j++ ) {
                Cm_c[i][ j] = Cm_c[i][j] + (  MCSrg_t[b][i][j] + MCSby_t[b][i][j] );
            }
        }
        b++;
    } while ( ! ( b == 6 ) );
    normalization( Cm_c, knl_t, knc_t );

    //Interpolaçao para o tamanho da imagem de entrada
    //Invalid array assigment
    //MCStmp = Cm_c;
    interpolation( MCStmp, Cm_c, knL, knC, - 3 );//3 <-- 3

  /*============================================================ Fim do Itti ============================================================================*/

    // Parâmetros do LEGION
    // Randomizando os valores iniciais...
    tmp = 0;

    //Inicializando os neuronios LEGION
    for ( int stop = knL - 1, i = 0; i <= stop; i++) {
        for ( int stop = knC - 1, j = 0; j <= stop; j++) {
            Neuronios[i][j].matrix = 0;
            Neuronios[i][j].ly = rand();
            Neuronios[i][j].lx[0] = double( Neuronios[i][j].ly ) / MAXVAL;
            Neuronios[i][j].ly = Neuronios[i][j].ly - 2;

            if ( Neuronios[i][j].ly < 0 ) {
                Neuronios[i][j].ly = 0;
            }

            Neuronios[i][j].trigger = 0;
            Neuronios[i][j].cycle = 0;
            Neuronios[i][j].d = 0;
        }
    }

    z = 0;

    //Verificando conexoes
    Connections();

    //Avaliando
    jump[0] = 0;
    Select( );


    first_pulse = false;
    temp = 0;
    temp2 = 0;
    temp3 = 0.0;
    count = 0;
    n = 0;

    /*Timer.Enabled*/
    //bool Timer = true;
    bool Timer6 = false;
    //while ( Timer == true ) {
    for ( int meuTimer = 0; meuTimer < 100; meuTimer++ ) {
        // ProcessMessages
        tmp++;
        count = 0;
        if ( Timer6 == false ) {
            //Calculando entradas da rede input
            for ( int stop = knL - 1, i = 0; i <= stop; i++ ) {
                for ( int stop = knC - 1, j = 0; j <= stop; j++ ) {
                    //Application.ProcessImage;
                    if ( ( Neuronios[i][ j].lx[0] > thetae ) && ( jump[0] == 0 ) ) {
                        Neuronios[i][j].lx[1] = LP;
                        Neuronios[i][j].ly = - 1;
                        Neuronios[i][j].cycle = 1;
                        count++;
                    } else
                        if ( Neuronios[i][ j].lx[0] < thetae ) { /* jupping down */

                            temp = - Wz * bi( z, thetaz ); //Normalmente eh 50

                            /* the following is based on traversing through strongest link */
                            temp2 = 0.0;

                            if ( j - 1 >= 0 ) {
                                temp3 = Neuronios[i][j].w[0] * bi( Neuronios[i][ j - 1].lx[0], thetae );
                                if ( temp2 < temp3 ) {
                                    temp2 = temp3;
                                }
                            }

                            if ( ( i - 1 >= 0 ) && ( j - 1 >= 0 ) ) {
                                temp3 = Neuronios[i][ j].w[1] * bi( Neuronios[i - 1][ j - 1].lx[0], thetae );
                                if ( temp2 < temp3 ) {
                                    temp2 = temp3;
                                }
                            }

                            if ( i - 1 >= 0 ) {
                                temp3 = Neuronios[i][ j].w[2] * bi( Neuronios[i - 1][ j].lx[0], thetae );
                                if ( temp2 < temp3 ) {
                                    temp2 = temp3;
                                }
                            }

                            if ( ( i - 1 >= 0 ) && ( j + 1 < knC ) ) {
                                temp3 = Neuronios[i][ j].w[3] * bi( Neuronios[i - 1][ j + 1].lx[0], thetae );
                                if ( temp2 < temp3 ) {
                                    temp2 = temp3;
                                }
                            }

                            if ( j + 1 < knC ) {
                                temp3 = Neuronios[i][ j].w[4] * bi( Neuronios[i][ j + 1].lx[0], thetae );
                                if ( temp2 < temp3 ){
                                    temp2 = temp3;
                                }
                            }

                            if ( ( i + 1 < knL ) && ( j + 1 < knC ) ) {
                                temp3 = Neuronios[i][ j].w[5] * bi( Neuronios[i + 1][ j + 1].lx[0], thetae );
                                if ( temp2 < temp3 ) {
                                    temp2 = temp3;
                                }
                            }

                            if ( i + 1 < knL ) {
                                temp3 = Neuronios[i][ j].w[6] * bi( Neuronios[i + 1][ j].lx[0], thetae );
                                if ( temp2 < temp3 ){
                                    temp2 = temp3;
                                }
                            }

                            if ( ( i + 1 < knL ) && ( j - 1 >= 0 ) ) {
                                temp3 = Neuronios[i][ j].w[7] * bi( Neuronios[i + 1][ j - 1].lx[0], thetae );
                                if ( temp2 < temp3 ) {
                                    temp2 = temp3;
                                }
                            }

                        temp = temp + temp2;

                        if ( ( Neuronios[i][ j].trigger == 1 ) || ( temp > 0.25 ) ) {
                            Neuronios[i][ j].lx[1] = RP;
                            Neuronios[i][ j].ly = MAXIMO;
                            jump[1] = 1;
                            z++;
                        } else {
                            Neuronios[i][ j].lx[1] = Neuronios[i][ j].lx[0];
                        }
                    } else {
                        Neuronios[i][ j].lx[1] = Neuronios[i][ j].lx[0];
                    }
                }
            }

            z = z - count;
            if ( ( jump[1] == 0 ) && ( z == 0 ) ) {
                Select();
            }
        }
    }
    //Neuronio eh a saida do algoritmo
}

void mainPrincipal()
{
    srand( (unsigned)time( NULL ) );
    Principal prin;
    Mat imagem;

    prin.Saliencia( imagem );
}
