/**
 * \file Gui.cpp
 *
 * \author Raphael Montanari
 * \date 19/06/2013
 *
 * Gui source file.
 */

#include "Gui.h"

Gui::Gui()
{
    readySignal.store( false );
    ready = false;
}

bool Gui::isRunning()
{
    return( t.joinable() );
}

void Gui::start()
{
    if ( isRunning() ) {
        stop();
    }
    t = thread( &Gui::run, this );
}

void Gui::run()
{
    // The QApplication class manages the GUI application's control flow and main settings.
    int argc = 1;
    char *argv[] = { (char *) "YaSfMt" };
    QApplication app( argc, argv );
    Q_INIT_RESOURCE( resources );
    mainWindow = new MainWindow( &ready, &cv, &m, &readySignal );
    // Shows the widget.
    mainWindow->show();
    // Enters the main event loop and waits until close() is called.
    app.exec();
    delete( mainWindow );
}

void Gui::stop()
{
    if ( isRunning() ) {
        try {
            t.join();
        } catch ( const exception &e ) {
            cerr << "An exception occurred. Exception Nr.: " << e.what() << endl;
        }
    }
}

void Gui::waitSignal()
{
    {
        unique_lock<mutex> lock( m );
        cv.wait( lock, [&](){ return ready; } );
        ready = false;
    }
    readySignal.store( false );
}

bool Gui::gotSignal()
{
    if ( readySignal.load() ) {
        readySignal.store( false );
        return( true );
    }
    return( false );
}

SignalType Gui::getSignal()
{
    return( mainWindow->getSignal() );
}

void Gui::captureFailed()
{
    mainWindow->captureFailed();
}

InputSource Gui::getInputSource()
{
    return( mainWindow->getInputSource() );
}

string Gui::getFilename()
{
    return( mainWindow->getFilename() );
}

void Gui::setStatusMessage( string message )
{
    mainWindow->setStatusMessage( message );
}

void Gui::incrementFramesCounter()
{
    mainWindow->incrementFramesCounter();
}

void Gui::setCameraView( Mat frame )
{
    mainWindow->updateCameraView( frame );
}

void Gui::setLatestFrameView( Mat frame )
{
    mainWindow->updateLatestFrameView( frame );
}

void Gui::setPointCloudView( Mat frame )
{
    mainWindow->updatePointCloudView( frame );
}
