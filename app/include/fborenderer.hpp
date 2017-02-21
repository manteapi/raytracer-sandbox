#ifndef FBORENDERER_HPP
#define FBORENDERER_HPP

#include <QQuickFramebufferObject>
#include "viewer.hpp"

class FBORenderer : public QQuickFramebufferObject::Renderer
{
public:
    ~FBORenderer();
    FBORenderer();

    void render();
    void synchronize(QQuickFramebufferObject *item);

    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size);

private:
    Viewer* m_item;
};

#endif // FBORENDERER_HPP
