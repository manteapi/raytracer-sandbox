#ifndef VIEWER_HPP
#define VIEWER_HPP

#include <QtQuick/QQuickItem>
#include "fborenderer.hpp"

class Viewer : public QQuickItem
{
    Q_OBJECT

public:
    virtual ~Viewer();
    Viewer();

protected:
    void timerEvent(QTimerEvent* /*event*/);

Q_SIGNALS:
    void computeHasEnded();

public Q_SLOTS:
    void sync();
    void cleanup();
    void compute();
    bool save();
    void setBackgroundImage(const QString &pathToImage);

private Q_SLOTS:
    void handleWindowChanged(QQuickWindow *win);

private:
    FBORenderer * m_renderer;
    int m_timerId;
};

#endif // VIEWER_HPP
