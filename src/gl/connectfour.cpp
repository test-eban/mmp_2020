#include "connectfour.h"

ConnectFour::ConnectFour()
    :GLItem()
{
    // options
    m_yellow = new GLColorRgba(1, 0.75, 0);
    m_red = new GLColorRgba(0.78125, 0.0078125, 0.171875);
    m_blue = new GLColorRgba(0, 0.359375, 0.7734375);
    m_grey = new GLColorRgba(0.443f, 0.443f, 0.443f);

    m_guiThreadXRotation = 0.0f;
    m_guiThreadYRotation= 0.0f;
    m_guiZoomFactor = 1.0f;

    m_drawAxes = false;
    m_movementEnabled = true;
    m_eye = QVector3D(0,1.5,5.5);
    m_lightingEnabled = true;
    m_colorArrayEnabled = true;

    m_activePlayer = 1;
    m_selectedTokenIndex = -1;

    m_mouseEventProcessed = false;
    m_lastMouseEvent = nullptr;
    m_soundEngine = new SoundEngine(this);
    m_soundEngine->setEnabled(true);
    m_soundEngine->loadSound(":/sounds/knock.wav");
    m_animationStep = 0;
    m_totalAnimationSteps = 0;
    m_animationActive = false;
    m_movementEnabled = false;
    m_timer->start();

    // objects
    GLToken* token;
    for (int i = 0; i < m_numberOfTokens; i++)
    {
        if (i < 21)
        {
            token = new GLToken(*m_red);
        }
        else
        {
            token = new GLToken(*m_yellow);
        }
        m_tokens.append(token);
    }
    m_board = new GLBoard(*m_blue);
    m_tray1 = new GLTokenTray(*m_grey);
    m_tray2 = new GLTokenTray(*m_grey);

}

QString ConnectFour::getStatusText()
{
    QString result;
    if (m_activePlayer == 1)
    {
        result = "Nächster Spieler: Rot";
    }
    else if (m_activePlayer == 2)
    {
        result = "Nächster Spieler: Gelb";
    }
    else if (m_activePlayer == 3)
    {
        result = "Spieler Rot hat gewonnen!";
    }
    else if (m_activePlayer == 4)
    {
        result = "Spieler Gelb hat gewonnen!";
    }
    return result;
}

void ConnectFour::mousePressed(int x, int y, int button)
{
    qDebug() << " MyGLItem::mousePressEvent running on thread: " << QThread::currentThreadId();
    if(m_lastMouseEvent)
        delete m_lastMouseEvent;
    m_lastMouseEvent = new QMouseEvent(QMouseEvent::MouseButtonPress, QPointF(x, y), QPointF(x, y),
                                       static_cast<Qt::MouseButton>(button),static_cast<Qt::MouseButton>(button), Qt::NoModifier);
    m_lastMouseEvent->setAccepted(false);
    m_soundEngine->playSound(":/sounds/knock.wav");
    if(window())
        window()->update();
}

void ConnectFour::mouseReleased(int x, int y, int button)
{
    if(m_lastMouseEvent)
        delete m_lastMouseEvent;
    m_lastMouseEvent = new QMouseEvent(QMouseEvent::MouseButtonRelease, QPointF(x, y), QPointF(x, y),
                                       static_cast<Qt::MouseButton>(button),static_cast<Qt::MouseButton>(button), Qt::NoModifier);
    m_lastMouseEvent->setAccepted(false);
    if(window())
        window()->update();
}

void ConnectFour::mouseMoved(int x, int y, int button)
{
    if(m_lastMouseEvent)
        delete m_lastMouseEvent;
    m_lastMouseEvent = new QMouseEvent(QMouseEvent::MouseMove, QPointF(x, y), QPointF(x, y),
                                       static_cast<Qt::MouseButton>(button),static_cast<Qt::MouseButton>(button), Qt::NoModifier);
    m_lastMouseEvent->setAccepted(false);
    if(window())
        window()->update();
}

void ConnectFour::rotateY(float angle)
{
    m_guiThreadYRotation += angle;
    if (window())
        window()->update();
}

void ConnectFour::rotateX(float angle)
{
    m_guiThreadXRotation += angle;
    if (window())
        window()->update();
}

void ConnectFour::zoomIn()
{
   m_guiZoomFactor *= 1.01f;
   if (window())
       window()->update();
}

void ConnectFour::zoomOut()
{
   m_guiZoomFactor /= 1.01f;
   if (window())
       window()->update();
}

void ConnectFour::rotateLeft()
{
    m_rotationIncrement = -0.5;
    setMovementEnabled(true);
}

void ConnectFour::rotateRight()
{
    m_rotationIncrement = 0.5;
    setMovementEnabled(true);
}

void ConnectFour::stopRotation()
{
    setMovementEnabled(false);
}

void ConnectFour::wheelEvent(QWheelEvent *e)
{
    if( e->angleDelta().y() > 0)
        m_eye /= 1.1f;
    else m_eye *= 1.1f;
    if(window())
        window()->update();
    qDebug() << " MyGLItem::wheelEvent running on thread: " << QThread::currentThreadId();
}

void ConnectFour::paintUnderQmlScene()
{
}

void ConnectFour::paintOnTopOfQmlScene()
{
    renderer()->setLightingEnabled(m_lightingEnabled);

    for (int i = 0; i < m_numberOfTokens; i++)
    {
        m_tokens[i]->draw(renderer());
    }
    m_board->draw(renderer());
    m_tray1->draw(renderer());
    m_tray2->draw(renderer());
}

void ConnectFour::setupGeometry()
{
    GLItem::setupGeometry();

    for (int i = 0; i < m_tokens.size(); i++)
    {
        m_tokens[i]->readBinaryModelFile(m_tokens[i]->getFile());
        if (i < 21)
        {
            m_tokens[i]->move(QVector3D(-2.2f,0.3f,0) + QVector3D(0,0,1-i*0.1f));
        }
        else
        {
            m_tokens[i]->move(QVector3D(2.2f,0.3f,0) + QVector3D(0,0,1-i%21*0.1f));
        }
    }
    m_board->setModelOffset(QVector3D(0,-1.5,0));
    m_board->readBinaryModelFile(m_board->getFile());

    m_tray1->setModelOffset(QVector3D(-2.2f, -0.125f, 0));
    m_tray2->setModelOffset(QVector3D(2.2f, -0.125f, 0));
    m_tray1->readBinaryModelFile(m_tray1->getFile());
    m_tray2->readBinaryModelFile(m_tray2->getFile());

    //we have defined all geometry data, now copy them to GPU memory
    setupBuffers();
    qDebug() << "MyGLItem::setupGeometry() running on render thread Id:" << QThread::currentThreadId();
}

void ConnectFour::doSynchronizeThreads()
{
    GLItem::doSynchronizeThreads();

    m_cameraTransform.setToIdentity();
    m_cameraTransform.rotate(m_guiThreadXRotation, v_X);
    m_cameraTransform.rotate(m_guiThreadYRotation, v_Y);
    m_cameraTransform.scale(QVector3D(m_guiZoomFactor, m_guiZoomFactor, m_guiZoomFactor));
    m_cameraTransform.translate(QVector3D(0,-1.25,0));

    // Mouse pressed
    if(m_lastMouseEvent && (m_lastMouseEvent->type() == QMouseEvent::MouseButtonPress) && !m_lastMouseEvent->isAccepted())
    {
        m_lastMouseEvent->setAccepted(true);

        QVector3D nearPoint;
        QVector3D farPoint;
        qDebug() << " MyGLItem::paintOnTop running on thread: " << QThread::currentThreadId();
        renderer()->calculateMousePoints(&nearPoint, &farPoint, m_lastMouseEvent->pos());

        if (!m_animationActive)
        {
            for (int i = 0; i < m_numberOfTokens; i++)
            {
                if (m_tokens[i]->isHit(nearPoint, farPoint) )
                {
                    unsigned int selected = m_tokens[i]->color() == *m_red ? 1 : 2;
                    if (selected == m_activePlayer)
                    {
                        m_selectedTokenIndex = i;
                        break;
                    }
                }
                else
                {
                    m_selectedTokenIndex = -1;
                }
            }
        }

        if (m_selectedTokenIndex != -1 && m_tokens[m_selectedTokenIndex]->isMovable())
        {
            //Calculate distance from press point to disc center
            QVector3D pressPos;
            renderer()->mouseIntersection(&pressPos, v_Y, 0.0f, m_lastMouseEvent->pos());
            m_oldDiscPos = m_tokens[m_selectedTokenIndex]->getCenter(); //translation of disc
            m_pressPosToDiscPos = m_oldDiscPos - pressPos;

            m_tokens[m_selectedTokenIndex]->setSelected(true);
            m_tokens[m_selectedTokenIndex]->setShowFrame(true);
            m_tokens[m_selectedTokenIndex]->jumpUp();
            m_totalAnimationSteps = 5;
            m_animationActive = true;
            m_lastMouseEvent->setAccepted(true);
        }
    }

    //mouse release
    if(m_lastMouseEvent && (m_lastMouseEvent->type() == QMouseEvent::MouseButtonRelease) && !m_lastMouseEvent->isAccepted())
    {
        if (m_selectedTokenIndex != -1 && m_tokens[m_selectedTokenIndex]->isMovable())
        {
            m_tokens[m_selectedTokenIndex]->setShowFrame(false);
            m_tokens[m_selectedTokenIndex]->setSelected(false);

            QVector3D center = m_tokens[m_selectedTokenIndex]->getCenter();
            if(isTokenNearBoard(center))
            {
                unsigned int column = getColumnByPosition(m_tokens[m_selectedTokenIndex]->getCenter());
                if (column != 8)
                {
                    int y = m_board->insertToken(column, m_activePlayer);

                    if (y != -1)
                    {
                        m_tokens[m_selectedTokenIndex]->move(-m_tokens[m_selectedTokenIndex]->getCenter() + QVector3D(0, (2.75f-0.5f*y), 0) + QVector3D(-1.5f+0.5f*column, 0, 0));
                        m_tokens[m_selectedTokenIndex]->setMovable(false);
                        if (m_board->checkForWin())
                        {
                            gameWin();

                        }
                        else
                        {
                            switchPlayer();
                        }
                    }
                    else
                    {
                        m_tokens[m_selectedTokenIndex]->move(-m_tokens[m_selectedTokenIndex]->getCenter() + m_oldDiscPos);
                    }
                }
            }
            else
            {
                m_tokens[m_selectedTokenIndex]->move(-m_tokens[m_selectedTokenIndex]->getCenter() + m_oldDiscPos);
            }
            m_tokens[m_selectedTokenIndex]->jumpDown();
            m_totalAnimationSteps = 5;
            m_animationActive = true;
            m_lastMouseEvent->setAccepted(true);
        }
    }

    //mouse move
    if(m_lastMouseEvent && (m_lastMouseEvent->type() == QMouseEvent::MouseMove) && !m_lastMouseEvent->isAccepted())
    {
        if (m_selectedTokenIndex != -1 && m_tokens[m_selectedTokenIndex]->isMovable())
        {
            m_lastMouseEvent->setAccepted(true);
            QVector3D movePos;
            renderer()->mouseIntersection(&movePos, v_Y, 0.0f, m_lastMouseEvent->pos());
            QVector3D moveDistance = movePos + m_pressPosToDiscPos;
            m_tokens[m_selectedTokenIndex]->move(moveDistance - m_tokens[m_selectedTokenIndex]->getCenter());
        }
    }

    //Animation
    if(m_animationActive)
    {
        if(m_animationStep < m_totalAnimationSteps)
        {
            m_animationStep++;
            m_tokens[m_selectedTokenIndex]->updateAnimatedProperties(static_cast<float>(m_animationStep) / static_cast<float>(m_totalAnimationSteps) );
        }
        else
        {
            m_tokens[m_selectedTokenIndex]->finishAnimation();
            m_animationStep = 0;
            m_animationActive = false;
        }
    }
}

void ConnectFour::setupBuffers()
{
    //setup vertexbuffer
    m_vertexBuffer = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
    if(!m_vertexBuffer->create()){
        qDebug() <<"Vertex Buffer create failed";
        exit(1);
    }
    m_vertexBuffer->bind();
    m_vertexBuffer->setUsagePattern(QOpenGLBuffer::StaticDraw); // StaticDraw is default. However we set it for clearity.
    m_vertexBuffer->allocate(m_points.data(), m_points.size() * static_cast<int>(sizeof(GLPoint)) );
    m_vertexBuffer->release();

    //setup indexbuffer
    m_indexBuffer = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
    if(!m_indexBuffer->create()){
        qDebug() <<"Index Buffer create failed";
        exit(1);
    }
    m_indexBuffer->bind();
    m_indexBuffer->allocate(m_indices.data(), m_indices.size() * static_cast<int>(sizeof(IndexType)));
    m_indexBuffer->release();
}

bool ConnectFour::isTokenNearBoard(QVector3D pos)
{
    bool result = true;
    if ((pos.z() * pos.z()) > 1)
    {
        result = false;
    }
    if (pos.x() > 2.25f || pos.x() < (-1.75f))
    {
        result = false;
    }

    return result;
}

unsigned int ConnectFour::getColumnByPosition(const QVector3D pos)
{
    unsigned int index = 8;
    float base = -1.25;

    for (unsigned int i = 0; i < 7; i++)
    {
        if (pos.x() < (base + 0.5f * i))
        {
            index = i;
            break;
        }
    }

    return index;
}

void ConnectFour::switchPlayer()
{
    if (m_activePlayer == 1)
    {
        m_activePlayer = 2;
    }
    else
    {
        m_activePlayer = 1;
    }
}

void ConnectFour::gameWin()
{
    if (m_activePlayer == 1)
    {
        system("notify-send \"Spiel beendet\" \"Spieler Rot hat gewonnen!\nUm eine weitere Runde zu spielen, starten Sie das Spiel neu!\"");
        m_activePlayer = 3;
    }
    else
    {
        system("notify-send \"Spiel beendet\" \"Spieler Gelb hat gewonnen!\nUm eine weitere Runde zu spielen, starten Sie das Spiel neu!\"");
        m_activePlayer = 4;
    }
}
