/**
 * \file CVWidget.h
 *
 * \author Raphael Montanari
 * \date 01/05/2013
 *
 * CVWidget header file.
 */

#ifndef CVWIDGET_H
#define CVWIDGET_H

#include <mutex>
#include <QGLWidget>
#include <QtOpenGL>
#include <GL/glu.h>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

class CVWidget : public QGLWidget
{
    Q_OBJECT

public:

    explicit CVWidget();

    void setImage( Mat img );

signals:

    void doubleClicked( QWidget *widget );

    void showFrame();

protected:

    void initializeGL();

    void paintGL();

    void resizeGL( int width, int height );

private:

    void mouseDoubleClickEvent( QMouseEvent * event );

    mutex m;
    QImage qframe;
    QTimer timer;

};

#endif /* CVWIDGET_H */
