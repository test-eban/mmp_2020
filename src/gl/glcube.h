#ifndef GLCUBE_H
#define GLCUBE_H

#include "glbody.h"

class GLCube : public GLBody
{
public:
    GLCube(const QString & name, const QVector3D & lbb, const QVector3D & rtf, const GLColorRgba & color,  const QString & textureFile = "");

    /** Creates the surface. Should be called, when a GL engine is already running.
      * To be overwritten by subclasses. GLESBody::createSurface should be called at the beginning
      * of overriding functions. It will create the pointContainer, if none is supplied
      * Is called automatically by draw, if required.
      * MUST NOT be called without a working GL engine.
      *
      * @param pointContainer The container for the geometry data. If nullptr, a new one is created.
      * @param indexContainer The container for the index data. If nullptr, a new one is created.
      * Created containers will be deleted by destructor.
      */
    void makeSurface(QVector<GLPoint> * pointContainer, QVector<IndexType> *indexContainer)Q_DECL_OVERRIDE;
protected:
    /**
     * @brief createCube
     * @param lbb left bottom back corner
     * @param rtf right top front corner
     */
    void createCube(QVector3D lbb, QVector3D rtf);

private:
    QVector3D m_lbb;
    QVector3D m_rtf;
};

#endif // GLCUBE_H
