/**
  \file Planet.h
http://blog.coredumping.com/subdivision-of-icosahedrons/
  */
#pragma once
#include <G3D/G3DAll.h>

  /** \brief Application framework. */
class Planet {

protected:
    Any m_planetSpec;

    //variable sof edge collapsing   
    int m_edgesToCollapse;
    int m_landEdgesToCollapse;
    int m_mountainEdgesToCollapse;
    int m_oceanEdgesToCollapse;
    int m_angleLengthWeight;
    bool m_collapsingEnabled;
    bool m_hasClouds;

    //variables to use for the mesh
    int m_recursionLevel;
    int m_numberOfTrees;
    float m_frequency;
    float m_landBevel;
    float m_mountainBevel;
    float m_mountainHeight;
    float m_mountianDiversity;
    float m_mountianNoise1;
    float m_mountianNoise2;
    float m_mountianNoise3;
    float m_oceanLevel;
    float m_oceanBevel;
    int m_numberOfClouds;

    bool m_hasBirds;
    int m_numberOfBirds;

    String m_planetSource;
    String m_planetSave;

    float m_landNoise;
    float m_oceanNoise;

    bool m_waterMount;
    bool m_useWTexture;
    bool m_useLTexture;
    bool m_useMTexture;

    Color3 m_mountainColor;
    Color3 m_waterColor;
    Color3 m_landColor;

    Color4 m_mountainGloss;
    Color4 m_waterGloss;
    Color4 m_landGloss;

    String m_landTextureFile;
    String m_waterTextureFile;
    String m_mountainTextureFile;

    String m_waterObjFile;
    String m_landObjFile;
    String m_mountainObjFile;

    Array<Point3> m_treePositions;
    Array<Point3> m_cloudPositions;

    // Options for Planet as a whole
    float m_scale;
    Point3 m_position;
    String m_planetName;
    String m_objectToOrbit;
    float m_orbitDistance;
    bool m_useParticleClouds;

    //Creates an initial icohedron with the given radius to be tessellated to create a sphere
    void makeIcohedron(float radius, Array<Vector3>& vertices, Array<Vector3int32>& faces);
    void subdivideIcoHedron(float radius, Array<Vector3>& vertices, Array<Vector3int32>& faces);
    void getMiddle(float radius, Vector3& v1, Vector3& v2, Vector3& newVector);
    bool readSpec(const Any& planetSpec);

public:
    static void getMapping(const Vector3& vertex, int width, int height, Point2int32& map);

    //Writes a sphere to a given off file
    bool generatePlanet();
    void writeSphere(String filename, float radius, int depths, Array<Vector3>& vertices, Array<Vector3int32>& faces);
    void applyNoiseWater(Array<Vector3>& vertices, shared_ptr<Image> noise);
    void applyNoiseLand(Array<Vector3>& vertices, shared_ptr<Image> noise, shared_ptr<Image> test, float oceanLevel);
    void applyNoiseMountain(Array<Vector3>& vertices, shared_ptr<Image> noise, shared_ptr<Image> test, bool waterMount, float power, float multiplier);
    void createWaterAnyFile(Any& waterModel, Any& waterEntity);
    void createLandAnyFile(Any& landModel, Any& landEntity, const String& waterEntity);
    void createMountainAnyFile(Any& mountainModel, Any& mountainEntity, const String& waterEntity);
    void createCloudModelAnyFile(Any& cloudModel, const String& name, const String& planetName);
    void createEntityModelAnyFile(Any& model, const String& name, const String& fileName, const String& preprocess);

    void addCloudEntityToPlanet(Any& cloudEntity, const String& name, const Point3& position, const float orbitAngle, const float orbitSpeed);
    void addFlyingEntityToPlanet(Any& entity, const String& name, const Point3& position, const float orbitAngle, const float orbitSpeed);

    void findTreePositions(const shared_ptr<Image>& landMap, const Array<Vector3>& vertices, Array<Vector3>& positions);
    void getTreePositions(Array<Vector3>& vertices);

    Point3 getPosition();
    float getScale();
    String getName();
    bool hasClouds();
    bool useParticleClouds();
    int getNumberOfClouds();
    int getNumberOfTrees();

    bool hasBirds();
    int getNumberOfBirds();
    
    void findCloudPositions(const shared_ptr<Image>& landMap, const Array<Vector3>& vertices, Array<Vector3>& positions);
    void getCloudPosition(Array<Point3>& cloudPositions);

    void getPlanetOrbit(String& objectToOrbit, float& orbitDistance);

    Planet();

    Planet(const String& name, const Any& spec);
};
