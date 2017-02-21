#ifndef VIEWER_HPP
#define VIEWER_HPP

#include <QQuickFramebufferObject>

class Viewer : public QQuickFramebufferObject
{
    Q_OBJECT

public:
    virtual ~Viewer();
    Viewer();
    QQuickFramebufferObject::Renderer* createRenderer() const;

Q_SIGNALS:
    void computeHasEnded();

public Q_SLOTS:
    void compute();
};



#endif // VIEWER_HPP
