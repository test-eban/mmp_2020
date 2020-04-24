#ifndef MYGLITEM_H
#define MYGLITEM_H

#include "glitem.h"
#include <QOpenGLBuffer>
#include "glboard.h"
#include "gltoken.h"
#include "gltokentray.h"
#include "../sound/soundengine.h"

class ConnectFour : public GLItem
{
    Q_OBJECT
public:
    ConnectFour();
public slots:
    /**
     * @brief mousePressed
     * @param x
     * @param y
     * @param button
     */
    void mousePressed(int x, int y, int button);
    /**
     * @brief mouseReleased
     * @param x
     * @param y
     * @param button
     */
    void mouseReleased(int x, int y, int button);
    /**
     * @brief mouseMoved
     * @param x
     * @param y
     * @param button
     */
    void mouseMoved(int x, int y, int button);

    /**
     * @brief rotateY
     * @param angle
     */
    void rotateY(float angle = 1.0);
    /**
     * @brief rotateX
     * @param angle
     */
    void rotateX(float angle = 1.0);
    /**
     * @brief zoomIn
     */
    void zoomIn();
    /**
     * @brief zoomOut
     */
    void zoomOut();

    /**
     * @brief rotateLeft
     */
    void rotateLeft();
    /**
     * @brief rotateRight
     */
    void rotateRight();
    /**
     * @brief stopRotation
     */
    void stopRotation();

protected:
    /**
     * @brief wheelEvent zoom in/out
     * @param e
     */
    void wheelEvent (QWheelEvent * e) Q_DECL_OVERRIDE;

    void paintUnderQmlScene() Q_DECL_OVERRIDE;

    void paintOnTopOfQmlScene() Q_DECL_OVERRIDE;

    void setupGeometry() Q_DECL_OVERRIDE;
    /**
     * @brief doSynchronizeThreads Copy data from GUI-thread to render-thread and vice versa.
     * Virtual function to be overridden by subclasses
     */
    virtual void doSynchronizeThreads()Q_DECL_OVERRIDE;

    /**
     * @brief setupBuffers Create buffer objects and copy data to buffers.
     */
    virtual void setupBuffers();

    /**
     * @brief isTokenNearBoard
     * @param pos
     * @return
     */
    bool isTokenNearBoard(const QVector3D pos);

    /**
     * @brief getRowByPosition
     * @param pos
     * @return
     */
    unsigned int getRowByPosition(const QVector3D pos);

private:
    // mouse-events and stone-movement
    QVector3D m_oldDiscPos;
    QVector3D m_pressPosToDiscPos;
    QMouseEvent * m_lastMouseEvent;
    bool m_mouseEventProcessed;
    // buffers
    QOpenGLBuffer * m_vertexBuffer;
    QOpenGLBuffer * m_indexBuffer;
    // sound engine
    SoundEngine * m_soundEngine;

    // tokens
    const int m_numberOfTokens = 42;
    int m_selectedTokenIndex = -1;
    QList<GLToken* > m_tokens;
    // trays
    GLTokenTray* m_tray1;
    GLTokenTray* m_tray2;
    // board
    GLBoard* m_board;
    // colors
    GLColorRgba* m_yellow;
    GLColorRgba* m_red;
    GLColorRgba* m_blue;
    GLColorRgba* m_grey;

    // animation
    int m_animationStep;
    int m_totalAnimationSteps;
    bool m_animationActive;

    // to transform the camera
    float m_guiThreadXRotation = 0.0f;
    float m_guiThreadYRotation= 0.0f;
    float m_guiZoomFactor = 1.0f;
};

#endif // MYGLITEM_H
