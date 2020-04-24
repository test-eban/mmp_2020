#ifndef GLCYLINDER_H
#define GLCYLINDER_H

#include "glbody.h"

/**
 * @brief The GLCylinder class is a Cylinder with axis parallel to z axis.
 * Textures must be created as follows:
 * The height must be a power of 2. width = 4 * height (e.g. width= 1024, height = 256)
 * from (0.0, 0.0) (0.25, 1.0) A circle with the texture data for the bottom of the cylinder
 * from (0.25, 0.0) to (0.75, 1.0) a rectangle with the texture data for the barrel of the cylinder
 * from (0.75, 0.0) to (1.0, 1.0) A circle with the texture data for the top of the cylinder
 */
class GLCylinder : public GLBody
{
public:
    GLCylinder(const QString & name, float height, float radius,  const GLColorRgba & m_color = GLColorRgba::clBlue, const QString m_textureFile = "", int slices = 16);

    /**
     * @brief makeSurface Create a cylinder from "tart pieces"
     * @param pointContainer
     * @param indexContainer
     */
    virtual void makeSurface(QVector<GLPoint> * pointContainer, QVector<GLushort> *indexContainer);

    float height() {return m_height;}
private:
    float m_height;
    int m_slices;
};

#endif // GLCYLINDER_H
