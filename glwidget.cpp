/****************************************************************************
**
** Copyright (C) 2015 The Qt Company Ltd.
** Contact: http://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtGui>
#include <QtOpenGL>
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#endif

#include <math.h>

#include "glwidget.h"
#include "qtlogo.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

#include <iostream>
using namespace std;

GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
//    logo = 0;
    xRot = 0;
    yRot = 0;
    zRot = 0;

    qtGreen = QColor::fromCmykF(0.40, 0.0, 1.0, 0.0);
    qtPurple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);
}

GLWidget::~GLWidget()
{
}

QSize GLWidget::minimumSizeHint() const
{
    return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
    return QSize(400, 400);
}

static void qNormalizeAngle(int &angle)
{
    while (angle < 0)
        angle += 360 * 16;
    while (angle > 360 * 16)
        angle -= 360 * 16;
}

void GLWidget::setXRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != xRot) {
        xRot = angle;
        emit xRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::setYRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != yRot) {
        yRot = angle;
        emit yRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::setZRotation(int angle)
{
    qNormalizeAngle(angle);
    if (angle != zRot) {
        zRot = angle;
        emit zRotationChanged(angle);
        updateGL();
    }
}

void GLWidget::initializeGL()
{
    qglClearColor(qtPurple.dark());

//    logo = new QtLogo(this, 64);
//    logo->setColor(qtGreen.dark());

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_MULTISAMPLE);
    static GLfloat lightPosition[4] = { 0.5, 5.0, 7.0, 1.0 };
    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);

    cout << "QApplication::desktop()->physicalDpiX(): "
         << QApplication::desktop()->physicalDpiX()
         << ", QApplication::desktop()->logicalDpiX(): "
         << QApplication::desktop()->logicalDpiX()
         << endl;
    cout << "QApplication::desktop()->physicalDpiY(): "
         << QApplication::desktop()->physicalDpiY()
         << ", QApplication::desktop()->logicalDpiY(): "
         << QApplication::desktop()->logicalDpiY()
         << endl;
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    double translationZ = -5.0;
    glTranslatef(0.0, 0.0, translationZ);
//    glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
//    glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
//    glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);
    double screenRadiusRatio = 0.5; // device-independent
    double screenRadius = width() * devicePixelRatio() * screenRadiusRatio; // in device pixels
    cout << "screenRadius: " << screenRadius << endl;

    drawBall(0, 0, -10, screenRadius);
//    logo->draw();
}

void GLWidget::resizeGL(int width, int height)
{
    cout << "devicePixelRatio(): " << devicePixelRatio() << endl;
    cout << "width: " << width << ", width(): " << this->width() << endl;
    cout << "height: " << height << ", height(): " << this->height() << endl;
    int side = qMin(width, height);
//    glViewport((width - side) / 2, (height - side) / 2, side, side);
//    glViewport(
//                (width - side) / 2 * devicePixelRatio(),
//                (height - side) / 2 * devicePixelRatio(),
//                side * devicePixelRatio(),
//                side * devicePixelRatio());

//    glViewport(0, 0, width * devicePixelRatio(), height * devicePixelRatio());
    glViewport(0, 0, width, height);

    // Sanity check
    int viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    cout << "viewport sanity check" << endl;
    for (int i = 0; i < 4; ++i) {
        cout << viewport[i] << endl;
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#ifdef QT_OPENGL_ES_1
    glOrthof(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
#else
    glOrtho(-0.5, +0.5, -0.5, +0.5, 4.0, 15.0);
#endif
    glMatrixMode(GL_MODELVIEW);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
    lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
    int dx = event->x() - lastPos.x();
    int dy = event->y() - lastPos.y();

    if (event->buttons() & Qt::LeftButton) {
        setXRotation(xRot + 8 * dy);
        setYRotation(yRot + 8 * dx);
    } else if (event->buttons() & Qt::RightButton) {
        setXRotation(xRot + 8 * dy);
        setZRotation(zRot + 8 * dx);
    }
    lastPos = event->pos();
}

double distance(double x1, double y1, double z1,
                double x2, double y2, double z2) {
    double result = (x2-x1)*(x2-x1) + (y2-y1)*(y2-y1) + (z2-z1)*(z2-z1);
    return sqrt(result);
}

double radiusOfConstPixelSize(double objX, double objY, double objZ,
                              double screenRadius = 20)
{
    GLint  vport[4];
    glGetIntegerv(GL_VIEWPORT, vport);
    GLdouble modMat[16];
    glGetDoublev(GL_MODELVIEW_MATRIX, modMat);
    GLdouble projMat[16];
    glGetDoublev(GL_PROJECTION_MATRIX, projMat);
    GLdouble winX, winY, winZ;
    gluProject(objX, objY, objZ, modMat, projMat, vport, &winX, &winY, &winZ);
    cout << "winX: " << winX << ", winY: " << winY << ", winZ: " << winZ << endl;

    GLdouble obj2X, obj2Y, obj2Z;
    gluUnProject(winX + screenRadius, winY, winZ, modMat, projMat, vport,
                 &obj2X, &obj2Y, &obj2Z);

    double radius = distance(objX, objY, objZ, obj2X, obj2Y, obj2Z);
    cout << "radius: " << radius << endl;
    return radius;
}

void GLWidget::drawBall(double objX, double objY, double objZ, double screenRadius)
{
    double radius = radiusOfConstPixelSize(objX, objY, objZ, screenRadius);

    // Drawing
    static GLUquadricObj * qobj = 0;		// may not be thread safe (??)
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glTranslated(objX, objY, objZ);
    // allocate a new gluQuadric thingy
    if (!qobj)
    {
        qobj = gluNewQuadric();
        //GLUquadricObj * qobj = gluNewQuadric();
        assert(qobj);
        gluQuadricDrawStyle(qobj, GLU_FILL);
        //gluQuadricNormals(qobj, GLU_SMOOTH);
        gluQuadricNormals(qobj, GL_NONE);
    }
    double stacks = std::max((double)10, radius * (double)3);
    gluSphere(qobj, radius, stacks, stacks);
    // and don't forget to destroy the gluQuadric thingy
//    gluDeleteQuadric(qobj);
    glPopMatrix();
}

void drawBall(double winX, double winY, double winRadius)
{

}

