/**
 * \file CVWidget.cpp
 *
 * \author Raphael Montanari
 * \date 01/05/2013
 *
 * CVWidget source file.
 */

#include "CVWidget.h"

CVWidget::CVWidget() : QGLWidget(QGLFormat(QGL::SampleBuffers)) {
    connect( this, SIGNAL( showFrame() ), this, SLOT( updateGL() ) );
}

void CVWidget::initializeGL() {
    glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
    /*
    connect( &timer, SIGNAL( timeout() ), this, SLOT( updateGL() ) );
    if ( format().swapInterval() == -1 ) {
        // V_blank synchronization not available (tearing likely to happen)
        int fps = 1000 / 30;
        timer.setInterval( fps );
    }
    else {
        // V_blank synchronization available
        timer.setInterval( 0 );
    }
    */
    //timer.start();
}

void CVWidget::paintGL() {
    // Este mutex evita falha de segmentação em alta taxa de quadros.
    lock_guard<mutex> lock( m );
    glClear( GL_COLOR_BUFFER_BIT );
    glClearColor( 0.0, 0.0, 0.0, 1.0 );
    if ( !qframe.isNull() ) {
        // Texture mapping (hardware accelerated)
        glDisable( GL_DEPTH_TEST );
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        gluOrtho2D( 0, qframe.width(), qframe.height(), 0 );
        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();
        glEnable( GL_TEXTURE_2D );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
        glTexImage2D( GL_TEXTURE_2D, 0, 4, qframe.width(), qframe.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, qframe.bits() );
        glBegin( GL_QUADS );
        glTexCoord2f( 0, 0 ); glVertex2f( 0, qframe.height() );
        glTexCoord2f( 0, 1 ); glVertex2f( 0, 0 );
        glTexCoord2f( 1, 1 ); glVertex2f( qframe.width(), 0 );
        glTexCoord2f( 1, 0 ); glVertex2f( qframe.width(), qframe.height() );
        glEnd();
        glDisable( GL_TEXTURE_2D );
        glFlush();
    }
}

void CVWidget::resizeGL( int width, int height ) {
    glViewport( 0, 0, (GLsizei) width, (GLsizei) height );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluPerspective( 60, (GLfloat) width / (GLfloat) height, 1.0, 100.0 );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
}

void CVWidget::setImage( Mat img ) {
    QImage out = QGLWidget::convertToGLFormat( QImage( (const unsigned char*) img.data, img.cols, img.rows, img.step, QImage::Format_RGB888 ).rgbSwapped() );
    {
        lock_guard<mutex> lock( m );
        qframe = out;
    }
    // Usar Mutex para evitar tentar atualizar frame depois do programa fechado?
    emit( showFrame() );
}

void CVWidget::mouseDoubleClickEvent( QMouseEvent * event )
{
    emit( doubleClicked( this ) );
    QGLWidget::mouseDoubleClickEvent( event );
}
