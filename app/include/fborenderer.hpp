#ifndef FBORENDERER_HPP
#define FBORENDERER_HPP

#include <QtQuick/QQuickItem>
#include <QtGui/QOpenGLShaderProgram>
#include <QtGui/QOpenGLFunctions>
#include <QOpenGLFramebufferObject>
#include <QOpenGLBuffer>

#include "shapetofbo.hpp"

class Toto
{

};

class FBORenderer : public QObject, protected QOpenGLFunctions
{
public:
    ~FBORenderer();
    FBORenderer();

public Q_SLOTS:
    void paint();
    void setViewportSize(const QSize &size);
    void setWindow(QQuickWindow *window);
    void setBackgroundImage(const QImage& image);
    QImage getBackgroundImage() const;
private:
    BackgroundFBOPtr m_backgroundFbo;
    ShapeToMainFBOPtr m_shapeToMainFbo;
    QSize m_viewportSize;
    QQuickWindow *m_window;
};

#endif // FBORENDERER_HPP
