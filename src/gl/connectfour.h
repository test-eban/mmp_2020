#ifndef MYGLITEM_H
#define MYGLITEM_H

#include "glitem.h"
#include <QOpenGLBuffer>
#include "glboard.h"
#include "gltoken.h"
#include "gltokentray.h"
#include "../sound/soundengine.h"

/**
 * @brief This class forms the core of the connect-4 programming. It constructs the tokens, the board and the token trays and makes sure that they are drawn.
 */
class ConnectFour : public GLItem
{
    Q_OBJECT
public:
    /**
     * @brief constructor. sets values and creates objects (tokens, board, tokentrays)
     */
    ConnectFour();

signals:
    void setText(QString text);

public slots:
    /**
     * @brief returns a fitting statusText for the upper left text field
     * @return QString that contains the new value for statusText
     */
    QString getStatusText();

    /**
     * @brief handles mousepressevents
     * @param x x-position of the mouse at click
     * @param y y-position of the mouse at click
     * @param button used mousebutton
     */
    void mousePressed(int x, int y, int button);
    /**
     * @brief handles mousepressevents
     * @param x x-position of the mouse at click
     * @param y y-position of the mouse at click
     * @param button used mousebutton
     */
    void mouseReleased(int x, int y, int button);
    /**
     * @brief handles mousemoveevents
     * @param x x-position of the mouse at click
     * @param y y-position of the mouse at click
     * @param button used mousebutton
     */
    void mouseMoved(int x, int y, int button);

    /**
     * @brief rotates along y axis
     * @param angle turn angle
     */
    void rotateY(float angle = 1.0);
    /**
     * @brief rotates along x axis
     * @param angle turn angle
     */
    void rotateX(float angle = 1.0);
    /**
     * @brief alters parameters so that the view zooms in
     */
    void zoomIn();
    /**
     * @brief alters parameters so that the view zooms out
     */
    void zoomOut();

    /**
     * @brief starts left-rotation
     */
    void rotateLeft();
    /**
     * @brief starts right-rotation
     */
    void rotateRight();
    /**
     * @brief stops the rotation
     */
    void stopRotation();

protected:
    /**
     * @brief wheelEvent zoom in/out
     * @param e
     */
    void wheelEvent (QWheelEvent * e) Q_DECL_OVERRIDE;

    /**
     * @brief called by draw(), paints scene
     */
    void paintUnderQmlScene() Q_DECL_OVERRIDE;

    /**
     * @brief called by draw(), paints scene
     */
    void paintOnTopOfQmlScene() Q_DECL_OVERRIDE;

    /**
     * @brief setups the geometry
     */
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
     * @brief checks if the position of the token is near the board
     * @param pos position that will be checked
     * @return  true if token is near board
     *          false if not
     */
    bool isTokenNearBoard(const QVector3D pos);

    /**
     * @brief assumes the column according to the given position
     * @details a columnis 0.5 wide. if the given position overshoots the column by +/- 0.25 it is sent to an adjacent column
     * @param pos position of the token
     * @return column of the token
     */
    unsigned int getColumnByPosition(const QVector3D pos);

    /**
     * @brief switches the player. 1 to 2, 2 to 1
     */
    void switchPlayer();

    /**
     * @brief handles a gamewin
     */
    void gameWin();

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
    // misc
    unsigned int m_activePlayer;

    // tokens
    const int m_numberOfTokens = 42;
    int m_selectedTokenIndex;
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
    float m_guiThreadXRotation;
    float m_guiThreadYRotation;
    float m_guiZoomFactor;
};

#endif // MYGLITEM_H
