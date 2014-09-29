/**
 * \file MainWindow.cpp
 *
 * \author Raphael Montanari
 * \date 01/05/2013
 *
 * Main window source file.
 */

#include "MainWindow.h"

MainWindow::MainWindow( bool *ready, condition_variable *cv, mutex *m , atomic<bool> *signalReady )
{
    setWindowTitle( "YaSfMt" );
    createActions();
    createMenus();
    createStatusBar();
    setupUi();

    this->cv = cv;
    documentWasModified.store( false );
    framesCounter = 0;
    {
        lock_guard<mutex> lk( inputSourceMutex );
        inputSource = InputSource::NONE;
    }
    this->m = m;
    paused = false;
    this->ready = ready;
    {
        lock_guard<mutex> lk( signalMutex );
        signal = SignalType::NONE;
    }
    this->signalReady = signalReady;
}

MainWindow::~MainWindow()
{
    sendSignal( SignalType::QUIT );
    qApp->quit();
}

void MainWindow::setupUi() {
    centralWidget = new QWidget();
    layout = new QGridLayout( centralWidget );

    cameraViewLabel = new QLabel( tr( "Camera view" ) );
    latestFrameViewLabel = new QLabel( tr("Latest frame: 0" ) );
    pointCloudViewLabel = new QLabel( tr("Point cloud view" ) );
    statusLabel = new QLabel( tr( "Status: Not started." ) );

    cameraView = new CVWidget();
    latestFrameView = new CVWidget();
    pointCloudView = new CVWidget();
    cameraView->setToolTip( tr( "Video input" ) );
    latestFrameView->setToolTip( tr( "Last frame successfully merged into the map (with matched features)" ) );
    pointCloudView->setToolTip( tr( "Real-time visualization of the map being built" ) );

    cameraView->setMinimumSize( 320, 240 );
    latestFrameView->setMinimumSize( 320,240 );
    pointCloudView->setMinimumSize( 680, 510 );

    QSizePolicy sizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    //layout->setSizePolicy( sizePolicy );
    cameraView->setSizePolicy( sizePolicy );
    latestFrameView->setSizePolicy( sizePolicy );
    pointCloudView->setSizePolicy( sizePolicy );

    layout->addWidget( cameraView, 3, 1, Qt::AlignTop );
    layout->addWidget( cameraViewLabel, 2, 1, Qt::AlignBottom );
    layout->addWidget( pointCloudViewLabel, 0, 0, Qt::AlignBottom );
    layout->addWidget( statusLabel, 4, 0, 1, 2, Qt::AlignBottom );
    layout->addWidget( latestFrameViewLabel, 0, 1, Qt::AlignBottom );
    layout->addWidget( latestFrameView, 1, 1, Qt::AlignTop );
    layout->addWidget( pointCloudView, 1, 0, 3, 1, Qt::AlignTop );

    setCentralWidget( centralWidget );

    connect( cameraView, SIGNAL( doubleClicked( QWidget* ) ), this, SLOT( showFullScreen( QWidget* ) ) );
    connect( latestFrameView, SIGNAL( doubleClicked( QWidget* ) ), this, SLOT( showFullScreen( QWidget* ) ) );
    connect( pointCloudView, SIGNAL( doubleClicked( QWidget* ) ), this, SLOT( showFullScreen( QWidget* ) ) );
}

void MainWindow::createActions() {
    newPointCloudAction = new QAction( QIcon(":/icons/new.ico" ), tr( "&New point cloud" ), this );
    newPointCloudAction->setShortcuts( QKeySequence::New );
    newPointCloudAction->setStatusTip( tr( "New point cloud." ) );
    connect( newPointCloudAction, SIGNAL( triggered() ), this, SLOT( newPointCloud() ) );

    openPointCloudAction = new QAction( QIcon(":/icons/open.ico" ), tr( "&Open point cloud" ), this );
    openPointCloudAction->setShortcuts( QKeySequence::Open );
    openPointCloudAction->setStatusTip( tr( "Open point cloud." ) );
    connect( openPointCloudAction, SIGNAL( triggered() ), this, SLOT( openPointCloud() ) );

    savePointCloudAction = new QAction( QIcon( ":/icons/save.ico" ), tr( "&Save point cloud" ), this );
    savePointCloudAction->setShortcuts( QKeySequence::Save );
    savePointCloudAction->setStatusTip( tr( "Save point cloud." ) );
    connect(savePointCloudAction, SIGNAL( triggered() ), this, SLOT( savePointCloud() ) );
    savePointCloudAction->setEnabled( false );

    exitAction = new QAction( QIcon(":/icons/exit.ico" ), tr( "E&xit" ), this );
    exitAction->setShortcuts( QKeySequence::Quit );
    exitAction->setStatusTip( tr( "Exit." ) );
    connect( exitAction, SIGNAL( triggered() ), this, SLOT( close() ) );

    calibrateCameraAction = new QAction( QIcon(":/icons/settings.ico"), tr("Cali&brate the camera"), this );
    calibrateCameraAction->setShortcuts( QKeySequence::Bold );
    calibrateCameraAction->setStatusTip( tr( "Corrects distortion." ) );
    connect( calibrateCameraAction, SIGNAL( triggered() ), this, SLOT( calibrateCamera() ) );

    captureFromCameraAction = new QAction( QIcon(":/icons/camera.ico"), tr("Capture from &camera"), this );
    captureFromCameraAction->setShortcuts( QKeySequence::Copy );
    captureFromCameraAction->setStatusTip( tr( "Capture video from camera." ) );
    connect( captureFromCameraAction, SIGNAL( triggered() ), this, SLOT( captureFromCamera() ) );

    captureFromROSNodeAction = new QAction( QIcon( ":/icons/ros.jpg"), tr( "Capture from &ROS node" ), this );
    captureFromROSNodeAction->setShortcuts( QKeySequence::Refresh );
    captureFromROSNodeAction->setStatusTip( tr( "Subscribe to a ROS node." ) );
    connect( captureFromROSNodeAction, SIGNAL( triggered() ), this, SLOT( captureFromROSNode() ) );

    captureFromVideoFileAction = new QAction( QIcon(":/icons/video.ico" ), tr( "Capture from fi&le" ), this );
    captureFromVideoFileAction->setShortcuts( QKeySequence::Find );
    captureFromVideoFileAction->setStatusTip( tr( "Capture video from file." ) );
    connect( captureFromVideoFileAction, SIGNAL( triggered() ), this, SLOT( captureFromVideoFile() ) );

    pauseCaptureAction = new QAction( QIcon(":/icons/resume.ico" ), tr( "&Pause" ), this );
    pauseCaptureAction->setShortcuts( QKeySequence::Print );
    pauseCaptureAction->setStatusTip( tr( "Pause capture of video." ) );
    connect( pauseCaptureAction, &QAction::triggered, this, &MainWindow::pauseCapture );
    pauseCaptureAction->setEnabled( false );

    showFullScreenAction = new QAction( QIcon( ":/icons/fullscreen.ico" ), tr( "Full screen" ), this );
    showFullScreenAction->setShortcuts( QKeySequence::FullScreen );
    showFullScreenAction->setStatusTip( tr( "Enable/disable display of point cloud on full screen." ) );
    connect( showFullScreenAction, SIGNAL( triggered() ), this, SLOT( showFullScreen() ) );

    contentsAction = new QAction( QIcon( ":/icons/help.ico" ), tr( "Contents" ), this );
    contentsAction->setShortcuts( QKeySequence::HelpContents );
    contentsAction->setStatusTip( tr( "Help contents." ) );
    connect( contentsAction, SIGNAL( triggered() ), this, SLOT(contents() ) );

    aboutAction = new QAction( QIcon(":/icons/about.ico" ), tr( "&About" ), this );
    aboutAction->setShortcuts( QKeySequence::WhatsThis );
    aboutAction->setStatusTip( tr( "About YaSfMt." ) );
    connect( aboutAction, SIGNAL( triggered() ), this, SLOT( about() ) );
}

void MainWindow::createMenus() {
    fileMenu = menuBar()->addMenu( tr( "&File" ) );
    fileMenu->addAction( newPointCloudAction );
    fileMenu->addAction( openPointCloudAction );
    fileMenu->addAction( savePointCloudAction );
    fileMenu->addSeparator();
    fileMenu->addAction( exitAction );

    toolsMenu = menuBar()->addMenu( tr( "&Tools" ) );
    toolsMenu->addAction( calibrateCameraAction );
    toolsMenu->addSeparator();
    toolsMenu->addAction( captureFromCameraAction );
    toolsMenu->addAction( captureFromROSNodeAction );
    toolsMenu->addAction( captureFromVideoFileAction );
    toolsMenu->addSeparator();
    toolsMenu->addAction( pauseCaptureAction );

    viewMenu = menuBar()->addMenu( tr( "&View" ) );
    viewMenu->addAction( showFullScreenAction );

    menuBar()->addSeparator();

    helpMenu = menuBar()->addMenu( tr( "&Help" ) );
    helpMenu->addAction( contentsAction );
    helpMenu->addAction( aboutAction );
}

void MainWindow::createStatusBar() {
    statusBar()->showMessage( tr( "Ready." ) );
}

void MainWindow::updateCameraView( Mat camera ) {
    cameraView->setImage( camera );
}

void MainWindow::updatePointCloudView( Mat camera ) {
    pointCloudView->setImage( camera );
}

void MainWindow::updateLatestFrameView( Mat latestFrame ) {
    documentWasModified.store( true );
    latestFrameView->setImage( latestFrame );
}

string MainWindow::getFilename() {
    lock_guard<mutex> lk( fileNameMutex );
    return( fileName );
}

void MainWindow::captureFailed() {
    captureFromCameraAction->setEnabled( true );
    captureFromROSNodeAction->setEnabled( true );
    captureFromVideoFileAction->setEnabled( true );
    pauseCaptureAction->setEnabled( false );
    savePointCloudAction->setEnabled( false );
}

InputSource MainWindow::getInputSource() {
    lock_guard<mutex> lk( inputSourceMutex );
    return( inputSource );
}

SignalType MainWindow::getSignal() {
    lock_guard<mutex> lk( signalMutex );
    return( signal );
}

void MainWindow::setStatusMessage( string message ) {
    string msg = "Status: " + message;
    statusLabel->setText( tr( msg.c_str() ) );
}

void MainWindow::incrementFramesCounter() {
    latestFrameViewLabel->setText( QString( "Latest frame: " + QString::number( ++framesCounter ) ) );
}

void MainWindow::sendSignal( SignalType s ) {
    {
        lock_guard<mutex> lk( *m );
        *ready = true;
    }
    {
        lock_guard<mutex> lk( signalMutex );
        signal = s;
    }
    signalReady->store( true );
    cv->notify_one();
}

void MainWindow::calibrateCamera() {
    sendSignal( SignalType::CALIBRATE );
}

void MainWindow::captureFromCamera() {
    calibrateCameraAction->setEnabled( false );
    captureFromCameraAction->setEnabled( false );
    captureFromROSNodeAction->setEnabled( false );
    captureFromVideoFileAction->setEnabled( false );
    pauseCaptureAction->setEnabled( true );
    savePointCloudAction->setEnabled( true );
    {
        lock_guard<mutex> lk( inputSourceMutex );
        inputSource = InputSource::CAMERA;
    }
    sendSignal( SignalType::CAPTURE );
}

void MainWindow::captureFromROSNode() {
    calibrateCameraAction->setEnabled( false );
    captureFromCameraAction->setEnabled( false );
    captureFromROSNodeAction->setEnabled( false );
    captureFromVideoFileAction->setEnabled( false );
    pauseCaptureAction->setEnabled( true );
    savePointCloudAction->setEnabled( true );
    {
        lock_guard<mutex> lk( inputSourceMutex );
        inputSource = InputSource::ROS;
    }
    sendSignal( SignalType::CAPTURE );
}

void MainWindow::captureFromVideoFile() {
    calibrateCameraAction->setEnabled( false );
    captureFromCameraAction->setEnabled( false );
    captureFromROSNodeAction->setEnabled( false );
    captureFromVideoFileAction->setEnabled( false );
    pauseCaptureAction->setEnabled( true );
    savePointCloudAction->setEnabled( true );
    {
        lock_guard<mutex> lk( fileNameMutex );
        fileName = QFileDialog::getOpenFileName( this, tr( "Open video" ), ".",
                tr( "Video files (*.avi *.ogv)" ) ).toStdString();
    }
    {
        lock_guard<mutex> lk( inputSourceMutex );
        inputSource = InputSource::FILE;
    }
    sendSignal( SignalType::CAPTURE );
}

void MainWindow::pauseCapture() {
    if ( !paused ) {
        paused = true;
        pauseCaptureAction->setText( tr( "&Resume" ) );
        sendSignal( SignalType::PAUSE );
    }
    else {
        paused = false;
        pauseCaptureAction->setText( tr( "&Pause" ) );
        sendSignal( SignalType::RESUME );
    }
}

void MainWindow::newPointCloud() {
    captureFromCameraAction->setEnabled( true );
    captureFromROSNodeAction->setEnabled( true );
    captureFromVideoFileAction->setEnabled( true );
    savePointCloudAction->setEnabled( false );
    sendSignal( SignalType::NEW );
}

bool MainWindow::openPointCloud() {
    {
        lock_guard<mutex> lk( fileNameMutex );
        fileName = QFileDialog::getOpenFileName( this, tr( "Open point cloud" ),
            ".", tr( "Point Cloud Data (*.pcd)" ) ).toStdString();
    }
    if ( !fileName.empty() ) {
        sendSignal( SignalType::OPEN );
        return( true );
    }
    else {
        return( false );
    }
}

bool MainWindow::savePointCloud() {
    {
        lock_guard<mutex> lk( fileNameMutex );
        fileName = QFileDialog::getSaveFileName( this, tr( "Save point cloud" ),
            QDir::currentPath(), tr( "Point Cloud Data (*.pcd)" ) ).toStdString();
    }
    if ( !fileName.empty() ) {
        sendSignal( SignalType::SAVE );
        return( true );
    }
    else {
        return( false );
    }
}

void MainWindow::close() {
    if ( documentWasModified.load() ) {
        bool readyToClose = false;
        while ( !readyToClose ) {
            int ret = QMessageBox::warning( this, tr( "YaSfMt" ),
            tr( "The document has been modified.\n Do you want to save your changes?"),
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel, QMessageBox::Save);
            switch ( ret ) {
                case QMessageBox::Save:
                    if ( savePointCloud() ) {
                        readyToClose = true;
                        QMainWindow::close();
                    }
                    break;
                case QMessageBox::Discard:
                    readyToClose = true;
                    QMainWindow::close();
                    break;
                case QMessageBox::Cancel:
                    readyToClose = true;
                    break;
                default:
                    // Should never be reached.
                    break;
            }
        }
    } else {
        QMainWindow::close();
    }
}

void MainWindow::showFullScreen() {
    cameraView->setVisible( true );
    cameraViewLabel->setVisible( true );
    latestFrameView->setVisible( true );
    latestFrameViewLabel->setVisible( true );
    pointCloudView->setVisible( true );
    pointCloudViewLabel->setVisible( true );
    statusLabel->setVisible( true );
    if ( isFullScreen() ) {
        QMainWindow::showNormal();
    }
    else {
        QMainWindow::showFullScreen();
    }
}

void MainWindow::showFullScreen( QWidget *widget ) {
    showFullScreen();
    if ( isFullScreen() ) {
        if ( widget == cameraView ) {
            //cameraView->setFixedSize( 640, 480 );

            cameraView->setVisible( isFullScreen() );
            cameraViewLabel->setVisible( isFullScreen() );
            latestFrameView->setVisible( !isFullScreen() );
            latestFrameViewLabel->setVisible( !isFullScreen() );
            pointCloudView->setVisible( !isFullScreen() );
            pointCloudViewLabel->setVisible( !isFullScreen() );
            statusLabel->setVisible( isFullScreen() );
        } else if ( widget == latestFrameView ) {
            cameraView->setVisible( !isFullScreen() );
            cameraViewLabel->setVisible( !isFullScreen() );
            latestFrameView->setVisible( isFullScreen() );
            latestFrameViewLabel->setVisible( isFullScreen() );
            pointCloudView->setVisible( !isFullScreen() );
            pointCloudViewLabel->setVisible( !isFullScreen() );
            statusLabel->setVisible( isFullScreen() );
        } else {
            cameraView->setVisible( !isFullScreen() );
            cameraViewLabel->setVisible( !isFullScreen() );
            latestFrameView->setVisible( !isFullScreen() );
            latestFrameViewLabel->setVisible( !isFullScreen() );
            pointCloudView->setVisible( isFullScreen() );
            pointCloudViewLabel->setVisible( isFullScreen() );
            statusLabel->setVisible( isFullScreen() );
        }
    }
}

void MainWindow::contents() {
    QDesktopServices::openUrl( QUrl( "http://lar.icmc.usp.br/" ) );

	//pcl::PointCloud<pcl::PointXYZRGB>::Ptr cloud;
    //... populate cloud
    //pcl::visualization::CloudViewer viewer ("Simple Cloud Viewer");
    //viewer.showCloud (cloud);
    //while (!viewer.wasStopped ()) {
    //}
}

void MainWindow::about() {
    QMessageBox::about( this, tr( "About YaSfMt" ),
             tr( "The <b>Yet another Structure from Motion toolkit</b> is an application that demonstrates how to generate a point cloud using structure from motion techniques." ) );
}
