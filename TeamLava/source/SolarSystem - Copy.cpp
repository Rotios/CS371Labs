#include "SolarSystem.h"
#include "Planet.h"
#include "Mesh.h"
#include "NoiseGen.h"

SolarSystem::SolarSystem() {

}

void SolarSystem::onInit() {
    m_planetTable = Table<String, Planet>();
    m_scene = Any(Any::TABLE);
    m_entities = Any(Any::TABLE);
    m_models = Any(Any::TABLE);

    initializeSceneTable(m_scene);
    initializeEntityTable(m_entities);
    initializeModelsTable(m_models);
}

bool SolarSystem::addPlanet(const String& name, Planet& planet) {
    if (m_planetTable.containsKey(name)) return false;
    m_planetTable.set(name, planet);
    String objectToOrbit;
    float orbitDistance;
    planet.getPlanetOrbit(objectToOrbit, orbitDistance);

    addPlanetToScene(m_entities, m_models, name, planet);

    return true;
}

void SolarSystem::addPlanetToScene(Any& entities, Any& models, const String& name, Planet& planet) {
    Any waterModelDescription(Any::TABLE, "ArticulatedModel::Specification");
    Any waterEntityDescription(Any::TABLE, "VisibleEntity");

    planet.createWaterAnyFile(waterModelDescription, waterEntityDescription);
    waterEntityDescription["model"] = name;
    entities[name] = waterEntityDescription;
    models[name] = waterModelDescription;

    Any landModelDescription(Any::TABLE, "ArticulatedModel::Specification");
    Any landEntityDescription(Any::TABLE, "VisibleEntity");

    String levelName = name + "land";
    planet.createLandAnyFile(landModelDescription, landEntityDescription, name);
    landEntityDescription["model"] = levelName;
    entities[levelName] = landEntityDescription;
    models[levelName] = landModelDescription;

    Any mountainModelDescription(Any::TABLE, "ArticulatedModel::Specification");
    Any mountainEntityDescription(Any::TABLE, "VisibleEntity");

    levelName = name + "mountain";
    planet.createMountainAnyFile(mountainModelDescription, mountainEntityDescription, name);
    mountainEntityDescription["model"] = levelName;
    entities[levelName] = mountainEntityDescription;
    models[levelName] = mountainModelDescription;


    Array<Point3> skyPositions;
    planet.getCloudPosition(skyPositions);
    if (planet.hasClouds()) {
        Any cloudModel = (planet.useParticleClouds()) ? Any(Any::TABLE, "ParticleSystemModel::Emitter::Specification") : Any(Any::TABLE, "ArticulatedModel::Specification");
        String cloudModelName = name + "cloud1";

        planet.createCloudModelAnyFile(cloudModel, cloudModelName, name);

        models[cloudModelName] = cloudModel;

        float orbitSpeed = 17.0f;
        int numberOfClouds = planet.getNumberOfClouds();
        bool useParticleClouds = planet.useParticleClouds();
        float orbitAngle = 35;
        for (int j(0); j < skyPositions.length() && j < numberOfClouds; ++j) {
            int rand = Random::threadCommon().integer(0, skyPositions.length() - 1);
            Point3 placement = skyPositions[rand];
            skyPositions.remove(rand);

            Any cloudEntityDescription = (useParticleClouds) ? Any(Any::TABLE, "ParticleSystem") : Any(Any::TABLE, "VisibleEntity");
            planet.addCloudEntityToPlanet(cloudEntityDescription, cloudModelName, placement, orbitAngle, orbitSpeed);
            entities[name + "cloud" + (String)std::to_string(j)] = cloudEntityDescription;
        }
    }

    int numberOfBirds = planet.getNumberOfBirds();
    if (numberOfBirds > 0) {
        Any birdModel = (planet.useParticleClouds()) ? Any(Any::TABLE, "ParticleSystemModel::Emitter::Specification") : Any(Any::TABLE, "ArticulatedModel::Specification");
        String birdModelName = name + "bird1";

        String preprocess = "{}";
        planet.createEntityModelAnyFile(birdModel, birdModelName, "model/bird/swallow.obj", preprocess);

        models[birdModelName] = birdModel;

        float orbitSpeed = 11.0f;

        for (int j(0); j < skyPositions.length() && j < numberOfBirds; ++j) {
            int rand = Random::threadCommon().integer(0, skyPositions.length() - 1);
            Point3 placement = skyPositions[rand];
            skyPositions.remove(rand);

            float orbitAngle = Random::threadCommon().uniform(-60, 60);
            Any birdEntityDescription(Any::TABLE, "VisibleEntity");

            planet.addFlyingEntityToPlanet(birdEntityDescription, birdModelName, placement, orbitAngle, orbitSpeed);

            entities[name + "bird" + (String)std::to_string(j)] = birdEntityDescription;
        }
    }

    int numTrees = planet.getNumberOfBirds();
    if (numTrees > 0) {
        Any treeModel(Any::TABLE, "ArticulatedModel::Specification");
        planet.createEntityModelAnyFile(treeModel, "tree", "model/lowpolytree.obj", "");
        models["tree"] = treeModel;

        Array<Vector3> treePositions;
        planet.getTreePositions(treePositions);

        for (int i(0); i < numTrees; ++i) {
            Any treeEntity(Any::TABLE, "VisibleEntity");
            Vector3 position = treePositions[i];
            Vector3 normal = (position - planet.getPosition()).unit();
            treeEntity["model"] = "tree";

            treeEntity["frame"] = CoordinateFrame::fromYAxis(normal, position);

            treeEntity["track"] = Any::parse("transform(entity(" + levelName + "), " + CoordinateFrame::fromYAxis(normal, (position + normal * 0.75f)*planet.getScale()).toXYZYPRDegreesString() + ")");
            entities["tree" + (String)std::to_string(i)] = treeEntity;
        }
    }

    makeSceneTable(m_scene, models, entities, name);
}

void SolarSystem::makeSceneTable(Any& scene, const Any& models, const Any& entities, const String& name) {
    scene["entities"] = entities;
    scene["models"] = models;
}

void SolarSystem::initializeSceneTable(Any& scene) {

    scene["name"] = "Solar System";

    Any lightingEnvironment(Any::TABLE, "LightingEnvironment");

    String occlusionSettings = (String) "AmbientOcclusionSettings { bias = 0.12; blurRadius = 4; blurStepSize = 2;" +
        "depthPeelSeparationHint = 0.2; edgeSharpness = 1; enabled = false; intensity = 3; " +
        "monotonicallyDecreasingBilateralWeights = false; numSamples = 19; radius = 2; " +
        "temporalFilterSettings = TemporalFilter::Settings { hysteresis = 0.9; falloffEndDistance = 0.07; " +
        "falloffStartDistance = 0.05; }; temporallyVarySamples = true; useDepthPeelBuffer = true; useNormalBuffer = true; " +
        "useNormalsInBlur = true;}";

    lightingEnvironment["ambientOcclusionSettings"] = Any::parse(occlusionSettings);

    String environmentMapSettings = (String) "Texture::Specification { encoding = Texture::Encoding { readMultiplyFirst = 1.9; " +
        "}; filename = \"cubemap/majestic/majestic512_*.jpg\"; }";
    //\"cubemap/hipshot_m9_sky/16_*.png\"
    lightingEnvironment["environmentMap"] = Any::parse(environmentMapSettings);

    scene["lightingEnvironment"] = lightingEnvironment;
}

void SolarSystem::initializeEntityTable(Any& entities) {
    //Create the light source
    Any light(Any::TABLE, "Light");
    light["attenuation"] = Vector3(0, 0, 1);
    light["bulbPower"] = Color3(1e+04, 1e+04, 0);
    light["castsShadows"] = false;
    //light["shadowMapBias"] = 0.05f;
    light["track"] = Any::parse("lookAt(Point3(0, -50, 250), Point3(0, 0, 0));");
    light["shadowMapSize"] = Vector2int16(2048, 2048);
    light["spotHalfAngleDegrees"] = 8;
    light["spotSquare"] = true;
    light["type"] = "SPOT";
    //light["varianceShadowSettings"] = Any::parse("VSMSettings {enabled = true; filterRadius = 11; blurMultiplier = 5.0f; downsampleFactor = 1; }");
    entities["sun"] = light;

    Any camera(Any::TABLE, "Camera");
    camera["frame"] = CFrame::fromXYZYPRDegrees(0.90151, 1.8599, 63.627, -0.39264, -1.1459, 0);
    //camera["frame"] = CFrame::fromXYZYPRDegrees(0.90151, 1.8599, 125.627, -0.39264, -1.1459, 0);
    camera["projection"] = Any::parse("Projection { farPlaneZ = -inf; fovDegrees = 50; fovDirection = \"VERTICAL\"; nearPlaneZ = -0.1; }");

    String filmSettings = (String) "FilmSettings{" +
        "antialiasingEnabled = true; antialiasingFilterRadius = 0; antialiasingHighQuality = true; bloomRadiusFraction = 0.009;"
        + "bloomStrength = 0.2; debugZoom = 1; gamma = 2.2; sensitivity = 1; toneCurve = \"CELLULOID\";" +
        "vignetteBottomStrength = 0.05; vignetteSizeFraction = 0.17; vignetteTopStrength = 0.5; }";
    camera["filmSettings"] = Any::parse(filmSettings);
    entities["camera"] = camera;

    Any skybox(Any::TABLE, "Skybox");
    skybox["texture"] = "cubemap/hipshot_m9_sky/16_*.png";
    entities["skybox"] = skybox;

    Any gradient(Any::TABLE, "VisibleEntity");
    gradient["model"] = "boardModel";
    gradient["frame"] = CFrame::fromXYZYPRDegrees(-4, 0, -50, 0, 0, 0);
    gradient["castsShadows"] = false;
    gradient["canChange"] = false;
    entities["gradient"] = gradient;
}

void SolarSystem::initializeModelsTable(Any& models) {
    String preprocess = "{setMaterial(all(), UniversalMaterial::Specification{ emissive = Texture::Specification{ filename = \"background.png\"; encoding = Texture::Encoding { readMultiplyFirst = Color4(Color3(0.5f), 1.0f);  }; }; lambertian = Color4(Color3(0.0f), 1.0f); }); }";
    Any boardModel(Any::TABLE, "ArticulatedModel::Specification");
    boardModel["filename"] = "ifs/square.ifs";
    boardModel["scale"] = 200;
    boardModel["preprocess"] = Any::parse(preprocess);
    models["boardModel"] = boardModel;
}

bool SolarSystem::containsPlanet(const String& name) {
    return m_planetTable.containsKey(name);
}

bool SolarSystem::removePlanet(const String &name) {
    return m_planetTable.remove(name);
}

bool SolarSystem::printSolarSystemToScene(const String& save) {
    try {
        m_scene.save(save + ".Scene.Any");
        return true;
    }
    catch (...) {
        return false;
    }
}