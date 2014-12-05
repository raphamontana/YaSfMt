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
    systemStatus = SystemStatus::INITIATING;
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


void Core::halt()
{
    video.stop();
    gui.stop();
}


void Core::execute()
{
    // Acquisition()
    // -> Captura imagens.
    Mat frame;
    video.getFrame( &frame );

    switch ( vision.processStructureFromMotion( frame ) ) {
        case SfMStatus::INITIATING:
            gui.incrementFramesCounter();
            gui.setLatestFrameView( frame );
            gui.setStatusMessage( "First frame set." );
            break;
        case SfMStatus::MAPPING:
            gui.incrementFramesCounter();
            gui.setLatestFrameView( frame );
            gui.setStatusMessage( "Mapping." );
            break;
        case SfMStatus::NOT_ENOUGH_MOTION:
            break;
        case SfMStatus::LOST_TRACKING:
            gui.setStatusMessage( "Lost tracking. Try to match against last frame." );
            break;
        default:
            cerr << "Should never fall here." << endl;
            break;
    }

    if ( gui.gotSignal() ) {
        systemStatus = SystemStatus::FETCHING;
    }

    // Evita super processamento.
    this_thread::sleep_for( chrono::milliseconds( 100 ) ); //25
}
