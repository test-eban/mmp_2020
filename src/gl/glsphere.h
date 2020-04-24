#ifndef GLSPHERE_H
#define GLSPHERE_H

#include "glbody.h"

/**
 * @brief The GlSphere class
 * A sphere around the origin in ECEF coordinates. Uses indexed TRIANGLE_STRIPS for drawing.
 * Uses an earthmap as texture. The map is a Mercator-projection with latitude = 0 and longitude = 0 at
 * center of texture (0.5, 0.5). For an ECEF coordinate globe the X-axis cuts through this point.
 * Z-Axis is at north pole (texcoord Y = 1.0), Y-axis cuts through longitude 90 and latitude 0 (texcoord 0.75, 0.5).
 * At longitude 180 degree we need an extra row of meridian points in order to switch from texcoord X = 1.0 to texcoord
 * x = 0.0.
 */
class GLSphere : public GLBody
{
public:
    GLSphere(const QString & name, float radius = 1.0, const GLColorRgba & color = GLColorRgba::clBlue, const QString textureFile = "",
             GLushort stacks = 18, GLushort slices = 36);
    /**
    * The points forming the surface are calculated in two for-loops. The inner loop runs along a meridian from south to north,
    * the outer loop turns the vertices of the inner loop from 0 to 360 degree from west to east. At 180 degree an extra meridian is
    * required to switch from end of texture to start of texture.
    * Uses an earthmap as texture. The map is a Mercator-projection with latitude = 0 and longitude = 0 at
    * center of texture (0.5, 0.5). For an ECEF coordinate system globe the X-axis cuts through this point.
    * Z-Axis is at north pole (texcoord Y = 1.0), Y-axis cuts through longitude 90 and latitude 0 (texcoord 0.75, 0.5).
    * At longitude 180 degree we need an extra row of meridian points in order to switch from texcoord X = 1.0 to texcoord
    * x = 0.0.
    */
    void makeSurface(QVector<GLPoint> * pointContainer, QVector<GLushort> *indexContainer);
    void draw(GLESRenderer * renderer, bool useBuffers = false)Q_DECL_OVERRIDE;
private:
    GLushort m_stacks;
    GLushort m_slices;
};

#endif // GLSPHERE_H
