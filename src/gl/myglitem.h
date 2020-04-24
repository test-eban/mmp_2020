#ifndef MYGLITEM_H
#define MYGLITEM_H

#include "glitem.h"
#include <QOpenGLBuffer>
#include "glcube.h"
#include "glsphere.h"
#include "glmouseray.h"
#include "gldisc.h"
#include "chessboard.h"
#include "glboard.h"
#include "gltoken.h"
#include "gltokentray.h"
#include "../sound/soundengine.h"

class MyGLItem : public GLItem
{
    Q_OBJECT
public:
    MyGLItem();
public slots:
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseMoved(int x, int y, int button);
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
     *
     */
    virtual void setupBuffers();

    bool isTokenNearBoard(const QVector3D pos);

    unsigned int getRowByPosition(const QVector3D pos);

    /**
     * @brief createCube
     * @param lbb left bottom back corner
     * @param rtf right top front corner
     */
    void createCube(QVector3D lbb, QVector3D rtf);
    void drawCube();

    void drawF();

public slots:
    void rotateY(float angle = 1.0);
    void rotateX(float angle = 1.0);
    void zoomIn();
    void zoomOut();

    void rotateLeft();
    void rotateRight();
    void stopRotation();
private:
    QOpenGLBuffer * m_vertexBuffer;
    QOpenGLBuffer * m_indexBuffer;
    GLMouseRay * m_mouseRay;
    GLDisc * m_disc;
    ChessBoard * m_ChessBoard;
    QMouseEvent * m_lastMouseEvent;
    QVector3D m_pressPosToDiscPos;
    bool m_mouseEventProcessed;
    bool m_mouseRaySet;
    SoundEngine * m_soundEngine;

    int m_selectedTokenIndex = -1;
    GLToken* m_selectedToken;
//    GLDisc* m_selectedToken;

    const int m_numberOfTokens = 10;
//    GLToken* m_tokens[42];
    QList<GLToken* > m_tokens;
//    QList<GLDisc* > m_tokens;
    GLTokenTray* m_tray1;
    GLTokenTray* m_tray2;
    GLBoard* m_board;

    //Animation
    int m_animationStep;
    int m_totalAnimationSteps;
    bool m_animationActive;

    //colors
    GLColorRgba* m_yellow;
    GLColorRgba* m_red;
    GLColorRgba* m_blue;
    GLColorRgba* m_grey;

    //these will affect m_cameraTransform
    float m_guiThreadXRotation = 0.0f;
    float m_guiThreadYRotation= 0.0f;
    float m_guiZoomFactor = 1.0f;
};

#endif // MYGLITEM_H
