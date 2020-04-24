#ifndef GLBONE_H
#define GLBONE_H

#include "glbody.h"
#include "glsphere.h"
#include "glcylinder.h"

#include <QVector3D>

/**
 * @brief The Track class Represents a polyline on which the endPoint of a conrod shall run.
 * It may be transformed by a transformation matrix. Use the transformedTrackPoint function to retrieve transformed
 * points. The transformation matrix is NOT owned by the track. It must be created and deleted elsewhere.
 */
class Track: public QVector<QVector3D>
{
public:
    Track(QMatrix4x4* transformation = nullptr)
    {
        m_transformation = transformation;
    }
    QVector3D transformedTrackPoint(int index)const{
        if(m_transformation)
            return (*m_transformation) * at(index);
        else return at(index);
    }
    void setTransformation(QMatrix4x4 * t){m_transformation = t;}
    const QMatrix4x4 * transformation()const {return m_transformation;}
private:
    QMatrix4x4 * m_transformation;
};

/**
 * @brief The Bone class defines a bone with startPoint and endPoint.
 * The startpoint can be moved by a moveStartPointTo() command.
 * The endpoint will then slide on the track curve in the appropriate distance (boneLength) to the startPoint.
 * To achieve this, the translation caused by moveStartPoint and an appropriate rotation are multiplied
 * to the transformMatrix.
 */
class GLBone : public GLBody
{
public:
    typedef enum{
        BONETYPE_CRANK,
        BONETYPE_CONROD
    }BoneType;
    /**
     * @brief GLBone A bone with start and end points.
     * @param name
     * @param startPoint
     * @param endPoint
     * @param type Crank or conrod
     * @param thickness Thickness of debug cylinder
     * @param model Pointer to a GLbody to be used for rendering. If NULL, a GLBody will be created.
     * @param makeDebugBodies Create two spheres and a cylinder for debugging.
     */
    GLBone(const QString & name, const QVector3D startPoint, const QVector3D endPoint,
         BoneType type = BONETYPE_CRANK, float thickness = 0.0, GLBody * model = nullptr, bool makeDebugBodies = false);
    /**
     * @brief GLBone
     * @param name
     * @param modelStartPoint
     * @param modelEndPoint
     * @param modelRotation
     * @param startPoint
     * @param type
     * @param thickness
     * @param model
     * @param makeDebugBodies
     */
    GLBone(const QString & name,
           const QVector3D & modelStartPoint, const QVector3D & modelEndPoint, const QVector3D &modelRotationAxis, float modelRotation,
           const QVector3D & startPoint,
           BoneType type = BONETYPE_CRANK, float thickness = 0.0, GLBody * model = nullptr, bool makeDebugBodies = false);


    ~GLBone()Q_DECL_OVERRIDE;
    /**
     * @brief deleteTextureObjects from model, if ownModel
     */
    void destroyTextureObjects()Q_DECL_OVERRIDE;

    /**
     * @brief makeSurface creates a line with startPoint (red) and endPoint (green) as spheres.
     * Use this function for debugging.
     * @param pointContainer
     * @param indexContainer
     */
    void makeSurface(QVector<GLPoint> * pointContainer, QVector<GLushort> *indexContainer)Q_DECL_OVERRIDE;
    /** Set texture file. Needs a current OpenGL context.
     */
    bool setTextureFile(const QString & textureFile)Q_DECL_OVERRIDE;
    /**
     * @brief setTextures Alle Texturen aus textureFiles laden.
     * @param minfilter
     * @param magfilter
     * @param wrapmode
     * @return true wenn ok.
     */
    bool setTextureFiles(const QStringList &textureFiles)Q_DECL_OVERRIDE;

    /** Draws the surface and calls makeSurface if required.
     *  Needs an active (made current) GL-Context.
     */
     virtual void draw(GLESRenderer * renderer, bool useBuffers)Q_DECL_OVERRIDE;
    /**
     * @brief setMinMaxCoordinates sets coordinates for this and m_model
     * @param min
     * @param max
     */
    void setMinMaxCoordinates(const QVector3D min, const QVector3D max)Q_DECL_OVERRIDE;

    /**
     * @brief moveStartPointTo Moves start point of Bone
     * @param newStartPoint
     */
    virtual void moveStartPointTo(const QVector3D & newStartPoint);

    /**
     * @brief addTrack Adds track to m_endPointTracks and selects it.
     * @param track The track to add
     * @return the index of track in m_endPointTracks
     */
    virtual int addTrack(Track* track);

    /**
     * @brief selectTrack Select track to follow
     * @param trackIndex The index of the track in m_endPointTracks
     * @return trackIndex or -1 if index is not valid.
     */
    virtual int selectTrack(int trackIndex);


    /**
     * @brief setCrankAngle For BONETYPE_CRANK only
     * @param angle
     */
    void setCrankAngle(float angle);

    /**
     * @brief setCrankAxis For BONETYPE_CRANK only
     * @param angle
     */
    void setCrankAxis(const QVector3D & axis){m_crankAxis = axis;}
    /**
     * @brief setParent
     * @param parent Sets parent and moves startPoint to parent's transformedEndPoint
     */
    void setParent(GLBone * parent);
    /**
    * @brief readBinaryFile Read data into m_model
    */
   virtual bool readBinaryModelFile(const QString & dataFileName,
                                    QVector<GLPoint> *pointContainer = nullptr, QVector<GLushort> *indexContainer = nullptr)Q_DECL_OVERRIDE;       // Binaere Datei

    const QVector3D & transformedStartPoint()const {return m_transformedStartPoint;}
    const QVector3D & transformedEndPoint()const {return m_transformedEndPoint;}

    void showTrack(int trackNumber);

    void hideTrack(int trackNumber);
    /**
     * @brief makeTrackLines Set the flag for creation of visible track lines.
     * Lines will be created in makeSurface. Show flags for all tracks will be set to true;
     */
    void makeVisibleTrackLines(){m_makeTrackLines = true;}

    /**
     * @brief setModelTransformation Set the transformation for the model data in m_model.
     * Because m_model may be used by several Bones, transformation is stored in m_modelTransformation matrix.
     * @param i Index of transformation matrix. If i == -1, m is appended to m_modeltransformations
     * @param m
     */
    void setModelTransformation( const QMatrix4x4 m, int i = -1);

    const QMatrix4x4 & modelTransformation(int i)const{return m_modelTransformations.at(i);}

    /**
     * @brief startAnimation Start an animation process.
     *        Sets animationActive to true.
     * Overwrite in subclasses to set Begin and End values for animated properties.
     */
    virtual void startAnimation(float startValue = 0.0)Q_DECL_OVERRIDE;
    /**
     * @brief finishAnimation Finish an animation process.
     *        Sets animationActive to false.
     * Overwrite in subclasses to copy End values for animated properties to Begin values.
     */
    virtual void finishAnimation()Q_DECL_OVERRIDE;

    /**
     * @brief updateAnimatedProperties
     * Overwrite to set current values of animated properties.
     */
    virtual void updateAnimatedProperties(float animationState)Q_DECL_OVERRIDE;
    /**
     * @brief setFinalCrankAngle
     * @param angle Final angle for animation
     */
    void setFinalCrankAngle(float angle){m_finalCrankAngle = angle;}
    /**
     * @brief setFinalStartPoint
     * @param point Final position of startPoint for animation
     */
    void setFinalStartPoint(const QVector3D & point){m_finalStartPoint = point;}
    /**
     * @brief setShowFrame sets showFrame flag in m_model too.
     * @param show
     */
    void setShowFrame(bool show)Q_DECL_OVERRIDE;
protected:
    /**
     * @brief createTrackPoints Create the GLPoints for track visualization
     */
    void createTrackLines();
    /**
     * @brief drawTracks Draw the track lines for debugging
     * @param renderer
     */
    virtual void drawTracks(GLESRenderer * renderer);
    /**
    * @brief calculateDrawMatrix Virtual function to calculate the final matrix to be used for drawing.
    * May be overwritten in subclasses. GLBody::calculateDrawMatrix simply copies m_transformationMatrix.
    */
    void calculateDrawMatrix()Q_DECL_OVERRIDE;

    /**
     * @brief lineSphereIntersection Calculates the intersections of the sphere defined by center and radius
     * with the line defined by p1 and p2.
     * @return Zero, one or two intersection points. Only intersection point positioned betweeen p1 and p2 are returned.
     */
    QVector<QVector3D> lineSphereIntersection(const QVector3D &center, float radius, const QVector3D & p1, const QVector3D& p2);


    int trackSegment(){return m_trackSegment;}

    int selectedTrack(){return m_selectedTrack;}
    void setSelectedTrack(int newVal){m_selectedTrack = newVal;}

protected:
    QVector <Track *> * tracks(){return & m_tracks;}
    void setOwnTracks(bool newVal){m_ownTracks = newVal;}
    GLBody * model(){return m_model;}
    GLBone * parent(){return m_parent;}
    const QVector3D & startPoint(){return m_startPoint;}
private:
    void init(const QVector3D startPoint, BoneType type , float thickness, GLBody *model, bool makeDebugBodies);
    GLBody * m_model; //GLBody with model data
    GLBone * m_parent;//The parent GLBone. startPoints tracks parent's transformedEndPoint
    QVector3D m_startPoint;//The original rotation axis point
    QVector3D m_finalStartPoint; //for animation of CONROD
    QVector3D  m_translateDistance;//for animation of CONROD
    QVector3D m_transformedStartPoint; //position of startpoint aft er translation
    QVector3D m_endPoint;//The original end point
    QVector3D m_transformedEndPoint; //position of end point after translation and rotation
    float m_length; //length from start to end
    BoneType m_type;
    bool m_makeDebugBodies;
    bool m_makeTrackLines;

    //members for default geometry

    int m_slices;
    GLSphere * m_startSphere;
    GLSphere * m_endSphere;
    GLCylinder * m_cylinder;
    bool m_ownModel; //true, if model is owned by this bone
    QVector3D m_translation; //difference between translated startPoint and startPoint
    QVector3D m_axis; //direction of bone axis, normalized

    //members for CONROD type
    /**
     * @brief m_endPointracks The tracks, on which the endpoint shall run
     */
    QVector <Track *> m_tracks;
    /**
     * @brief m_ownTracks
     * We are the owner of the tracks
     */
    bool m_ownTracks;
    /**
     * @brief m_nextTrackIndices
     *  start and end indices for the visible track lines used for debugging
     */
    QVector <int> m_nextTrackIndices;

    /**
     * @brief m_visibleTracks Flags for visible tracks.
     */
    QVector<bool>m_visibleTracks;
    int m_selectedTrack;
    int m_trackSegment;

    //members for CRANK type
    float m_crankAngle;
    float m_startCrankAngle;
    float m_finalCrankAngle; //for animation of CRANK
    float m_crankDistance; //for animation of CRANK
    QVector3D m_crankAxis;

    QVector<QMatrix4x4> m_modelTransformations;
};

#endif // BONE_H
