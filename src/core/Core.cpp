/**
 * \file Core.cpp
 *
 * \author Raphael Montanari
 * \date 01/05/2013
 *
 * Core source file.
 */

#include "Core.h"

Core::Core() : video( &gui )
{
    currentFrame = 0;
    systemStatus = SystemStatus::INITIATING;
    firstFrame = true;
}

void Core::exec()
{
    bool halted = false;
    while ( ! halted ) {
        switch ( systemStatus ) {
            case SystemStatus::INITIATING:
                init();
                break;
            case SystemStatus::FETCHING:
                fetch();
                break;
            case SystemStatus::DECODING:
                decode();
                break;
            case SystemStatus::EXECUTING:
                execute();
                break;
            case SystemStatus::HALTED:
                halt();
                halted = true;
                break;
            default:
                cerr << "Should never fall here." << endl;
                break;
        }
    }
}

void Core::init()
{
    gui.start();
    systemStatus = SystemStatus::FETCHING;
}

void Core::fetch()
{
    gui.waitSignal();
    systemStatus = SystemStatus::DECODING;
}

void Core::decode()
{
    SignalType signal = gui.getSignal();
    string message;
    switch ( signal ) {
        case SignalType::NEW:
            gui.setStatusMessage( "reseted." );
            systemStatus = SystemStatus::FETCHING;
            break;
        case SignalType::OPEN:
            gui.setStatusMessage( "opened." );
            systemStatus = SystemStatus::FETCHING;
            break;
        case SignalType::SAVE:
            gui.setStatusMessage( "saved." );
            systemStatus = SystemStatus::FETCHING;
            break;
        case SignalType::CALIBRATE:
            vision.calibrateCamera();
            gui.setStatusMessage( "calibrated." );
            systemStatus = SystemStatus::FETCHING;
            break;
        case SignalType::CAPTURE:
            if ( video.captureFrom( gui.getInputSource(), gui.getFilename(), &message ) ) {
                systemStatus = SystemStatus::EXECUTING;
            }
            else {
                gui.captureFailed();
                systemStatus = SystemStatus::FETCHING;
            }
            gui.setStatusMessage( message );
            break;
        case SignalType::PAUSE:
            gui.setStatusMessage( "paused." );
            systemStatus = SystemStatus::FETCHING;
            break;
        case SignalType::RESUME:
            gui.setStatusMessage( "resumed." );
            systemStatus = SystemStatus::EXECUTING;
            break;
        case SignalType::QUIT:
            systemStatus = SystemStatus::HALTED;
            break;
        default:
            cerr << "Should never fall here." << endl;
            break;
    }
}

void Core::execute()
{
    long long int newFrameNumber;
    Mat frame;

    // Acquisition()
    // -> Captura imagens.
    newFrameNumber = video.getFrame( &frame );

    // Processing()
    // -> Calibrate camera
    // -> Extrai caracteristicas locais.
    // -> Faz correspondencia entre os pontos.
    // -> Find fundamental matrix based on undistorted feature points.
    // -> Get essential matrix from fundamental matrix.
    // -> Compute projection matrix from cam1 to cam2
    // -> Triangulate
    // -> Calcula posicao da camera e dos pontos.
    // ---> Algoritmo dos 8 pontos.
    // ---> Matriz F.
    // ---> cv::LevMarqSparse lms;
    // ---> Triangulação
    if ( ( currentFrame != newFrameNumber ) && ( video.isValidFrame( frame ) ) ) {
        currentFrame = newFrameNumber;
        gui.incrementFramesCounter();

        if ( !firstFrame ) {
            // Roda uma iteraçao do algoritmo
            int matches = vision.getMotionMap( frame );
            if ( matches >= 8 ) {
                vision.estimateMotion();
                vision.reconstructionTriangulation();
            }
            else {
                gui.setStatusMessage( "Lost tracking. Try to match against last frame." );
            }
            //systemStatus = SystemStatus::HALTED;
        }
        else {
            vision.setInitialFrame( frame );
            firstFrame = false;
        }
    }

    // Publication()
    gui.setLatestFrameView( frame );

    if ( gui.gotSignal() ) {
        systemStatus = SystemStatus::FETCHING;
    }

    // Evita super processamento.
    this_thread::sleep_for( chrono::microseconds( 25000 ) );

    //unsigned int fps = 60;
    //usleep( 1000000 / fps );
}

void Core::halt()
{
    video.stop();
    gui.stop();
}
