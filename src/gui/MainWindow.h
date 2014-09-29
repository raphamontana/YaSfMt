/**
 * \file Gui.cpp
 *
 * \author Raphael Montanari
 * \date 20/01/2013
 *
 * Graphical User Interface.
 */

#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <atomic>
#include <condition_variable>
#include <exception>
#include <mutex>
#include <QtWidgets/QAction>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QWidget>
#include "CVWidget.h"
#include "../utils/InputSource.h"
#include "../utils/SignalType.h"
#include "../utils/SystemStatus.h"
//#include <pcl/visualization/cloud_viewer.h>

using namespace std;

/**
 * @brief The Gui class.
 */
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    /**
     * Private so that it can  not be called.
     *
     * @brief Gui constructor.
     * @param ready.
     * @param cv.
     * @param m.
     */
    explicit MainWindow(bool *ready, condition_variable *cv, mutex *m, atomic<bool> *signalReady );

    /**
     * \brief Destructor.
     */
    ~MainWindow();

    /**
     * \brief Gets the name of a file containing a video.
     * \return The file name.
     */
    string getFilename();

    /**
     * \brief Reset capture menu actions.
     */
    void captureFailed();

    /**
     * \brief Gets the origin of the input stream.
     * \return The origin of the input stream.
     */
    InputSource getInputSource();

    /**
     * \brief Gets the signal emmited.
     * \return The signal emmited.
     */
    SignalType getSignal();

    /**
     * \brief Sets a status message.
     * \param message String message to show.
     */
    void setStatusMessage( string message );

    /**
     * \brief Update the camera view.
     * \param img Image to show.
     */
    void updateCameraView( Mat img );

    /**
     * \brief Update the camera view.
     * \param img Image to show.
     */
    void updatePointCloudView( Mat img );

    /**
     * \brief Update the latest processed frame view.
     * \param img Image to show.
     */
    void updateLatestFrameView( Mat img );

    /**
     * \brief Sums one to the number of processed frames.
     */
    void incrementFramesCounter();

private slots:

    void calibrateCamera();
    void captureFromCamera();
    void captureFromROSNode();
    void captureFromVideoFile();
    void pauseCapture();
    void newPointCloud();
    bool openPointCloud();
    bool savePointCloud();
    void showFullScreen();
    void showFullScreen( QWidget *widget );
    void contents();
    void about();

public slots:

    void close();

private:

    void createActions();
    void createMenus();
    void createStatusBar();
    void setupUi();
    void sendSignal( SignalType s );

    QWidget *centralWidget;
    QGridLayout *layout;
    QLabel *cameraViewLabel;
    QLabel *latestFrameViewLabel;
    QLabel *pointCloudViewLabel;
    QLabel *statusLabel;
    CVWidget *cameraView;
    CVWidget *latestFrameView;
    CVWidget *pointCloudView;

    QMenu *fileMenu;
    QMenu *toolsMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;
    QAction *newPointCloudAction;
    QAction *openPointCloudAction;
    QAction *savePointCloudAction;
    QAction *exitAction;
    QAction *captureFromCameraAction;
    QAction *captureFromROSNodeAction;
    QAction *captureFromVideoFileAction;
    QAction *pauseCaptureAction;
    QAction *calibrateCameraAction;
    QAction *showFullScreenAction;
    QAction *contentsAction;
    QAction *aboutAction;

    atomic<bool> documentWasModified;
    atomic<bool> *signalReady;
    bool *ready;
    bool paused;
    condition_variable *cv;
    InputSource inputSource;
    unsigned long long int framesCounter;
    mutex *m;
    mutex fileNameMutex;
    mutex inputSourceMutex;
    mutex signalMutex;
    SignalType signal;
    string fileName;

};

#endif /* MAIN_WINDOW_H */
