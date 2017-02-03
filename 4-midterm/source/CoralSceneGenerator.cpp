#include "CoralSceneGenerator.h"

void CoralSceneGenerator::writeSeaEntities(String& scene, int numCoralTypes, int totalCoral) {
    String entities("entities = {");

    // Set the Skybox
    entities += (String) "" +
        "\n        skybox = Skybox {" +
        "\n            texture = Color3(0,0,.5);" +
        "\n        };" +
        "\n" +
        "\n        camera = Camera {" +
        "\n            frame = CFrame::fromXYZYPRDegrees(0.0, 1.0, 14.0, -1.2,4.0);" +
        "\n        };" +
        "\n        light1 = Light {" +
        "\n               attenuation = (0, 0, 1);" +
        "\n                bulbPower = Power3(1e+4); " +
        "\n                frame = CFrame::fromXYZYPRDegrees(2.4,19.8,5.2,26.6,-70.3,-171.7);" +
        "\n                shadowMapSize = Vector2int16(2048, 2048); " +
        "\n                spotHalfAngleDegrees = 60;"
        "\n                spotSquare = true; " +
        "\n                type = \"SPOT\"; " +
        "\n        };" +
        "\n" +
        "\n        water = VisibleEntity {" +
        "\n            model = \"seaWater\";" +
        "\n            frame = CFrame::fromXYZYPRDegrees(0,-3.0f,1.0f,0,0,0);" +
        "\n        };" +
        "\n" +
        "\n        ground = VisibleEntity {" +
        "\n            model = \"seaGround\";" +
        "\n            frame = CFrame::fromXYZYPRDegrees(0,-2.0f,0.9f,0,0,0);" +
        "\n        };" +
        "\n" +
  /*      "\n        shark1 = VisibleEntity {" +
        "\n            model = \"shark1\";" +
        "\n            frame = CFrame::fromXYZYPRDegrees(5.9f, 0.5f, -2.7f, -37.6f, 22.0f, 2.8f);" +
        "\n        };" +
        "\n" +
        "\n        shark2 = VisibleEntity {" +
        "\n            model = \"shark2\";" +
        "\n            frame = CFrame::fromXYZYPRDegrees(3.6f, 4.1f, 2.7f,-111.5f, 2.9f, -5.7);" +
        "\n        };" +
        "\n" +
        "\n        shark3 = VisibleEntity {" +
        "\n            model = \"shark1\";" +
        "\n            frame = CFrame::fromXYZYPRDegrees(-4.3f, 0.6f, -3.9f, 62.1f, 25.0f, 17.5f);" +
        "\n        };" +
        "\n" +
        "\n        shark4 = VisibleEntity {" +
        "\n            model = \"shark2\";" +
        "\n            frame = CFrame::fromXYZYPRDegrees(0.5f, 0, 5.0f, -164.2, .6f, 25.2f);" +
        "\n        };" +
        "\n" +
    */    "\n        aRock1 = VisibleEntity {" +
        "\n            model = \"largeRock1\";" +
        "\n            frame = CFrame::fromXYZYPRDegrees(-4.7f,-1.7f,-2.3,0,0,0);" +
        "\n        };" +
        "\n" +
        "\n        aRock2 = VisibleEntity {" +
        "\n            model = \"largeRock2\";" +
        "\n            frame = CFrame::fromXYZYPRDegrees(4.5f,-1.7f,-5,0,0,0);" +
        "\n        };" +
        "\n" +
        "\n        aRock3 = VisibleEntity {" +
        "\n            model = \"largeRock3\";" +
        "\n            frame = CFrame::fromXYZYPRDegrees(-.6f,-1.7f, 1.2f,174.3,0,0);" +
        "\n        };" +
        "\n" +
        "\n        aRock4 = VisibleEntity {" +
        "\n            model = \"largeRock4\";" +
        "\n            frame = CFrame::fromXYZYPRDegrees(-1.9f,-1.7f,-6.6f,0,0,0);" +
        "\n        };" +
        "\n" +
        "\n        chest1 = VisibleEntity {" +
        "\n            model = \"chest\";" +
        "\n            frame = CFrame::fromXYZYPRDegrees(-1.9f,-2.0f,-1.9f, 28.9f, 25.1f,-1.6f);" +
        "\n        };" +
        "\n" +
        "\n     ";

    int fishType = Random::threadCommon().integer(0, 5);
    entities += writeTankFish("fish" + (String)std::to_string(fishType), Random::threadCommon().integer(5, 10), 0, Point3(-1.5,0.5,-2.5), Point2(-1.5, 1.5), Point2(-0.5, 0.5), 1.2);
    fishType = Random::threadCommon().integer(0, 5);
    entities += writeTankFish("fish" + (String)std::to_string(fishType), Random::threadCommon().integer(5, 10), 1, Point3(-2,0.5,-2.5), Point2(-1.5, 1.5), Point2(-1, 1), 1.3);
    fishType = Random::threadCommon().integer(0, 5);
    entities += writeSchoolOfFish("fish" + (String)std::to_string(fishType), Random::threadCommon().integer(15, 25), 2, Point3(5.1,1.5,4.2), Point2(-1.5, 1.5), Point2(-1, 1), 1.2);
    fishType = Random::threadCommon().integer(0, 5);
    entities += writeSchoolOfFish("fish" + (String)std::to_string(fishType), Random::threadCommon().integer(15, 25), 3, Point3(-4.9,1,7.9), Point2(-3.5, 3.5), Point2(-1.6, 1.6), 1.1);

    entities += writeBubbles(Point2(-3.2f, 0.6f), Point2(-1, 1), Point3(-2.1f,1.0f,-4.8f), 1.6);

    writeSeaCoral(entities, 7, 10, Point2(1.0, 3.2), Point2(-7.4, -3.5), Point3(0,0.5f,0), "0");
    writeSeaCoral(entities, 7, 15, Point2(-4.3, 2.0), Point2(-1.9, 3.0), Point3(0,-0.5f,0), "1");
    writeSeaCoral(entities, 7, 10, Point2(-3.6, 0.2), Point2(-4.7, -2.5), Point3(0,-1.7f,0), "2");
    writeSeaCoral(entities, 7, 10, Point2(-6, -3.3), Point2(-4.1, -0.5f), Point3(0,-0.7f,0), "3");
    
    entities += "\n    ";
    entities += "};";
    entities += "\n";

    scene += entities;
}

void CoralSceneGenerator::writeTankEntities(String& scene, int numCoralTypes, int totalCoral) {
    String entities("entities = {");

    // Set the Skybox
    entities += (String) "" +
        "\n        skybox = Skybox {" +
        "\n            texture = Color3(0,0,.5);" +
        "\n        };" +
        "\n" +
        "\n        camera = Camera {" +
        "\n            frame = CFrame::fromXYZYPRDegrees(0.0, 1.0, 14.0, -1.2,4.0);" +
        "\n        };" +
        "\n        light1 = Light {" +
        "\n               attenuation = (0, 0, 1);" +
        "\n                bulbPower = Power3(2000); " +
        "\n                frame = CFrame::fromXYZYPRDegrees(3.9, 6.2, 1.4, -164, -77, 77);" +
        "\n                shadowMapSize = Vector2int16(2048, 2048); " +
        "\n                spotHalfAngleDegrees = 60;"
        "\n                spotSquare = true; " +
        "\n                type = \"SPOT\"; " +
        "\n        };" +
        "\n" +
        "\n        light2 = Light {" +
        "\n               attenuation = (0, 0, 1);" +
        "\n                bulbPower = Power3(2000); " +
        "\n                frame = CFrame::fromXYZYPRDegrees(-4.1, 5.3, .5, -164, -77, 77);" +
        "\n                shadowMapSize = Vector2int16(2048, 2048); " +
        "\n                spotHalfAngleDegrees = 60;"
        "\n                spotSquare = true; " +
        "\n                type = \"SPOT\"; " +
        "\n        };\n" +
        "\n        side1 = VisibleEntity {" +
        "\n            model = \"blueRectangle\";" +
        "\n            frame = CFrame::fromXYZYPRDegrees(0,0,-2,0,0,0);" +
        "\n        };" +
        "\n" +
        "\n        side2 = VisibleEntity {" +
        "\n            model = \"glassRectangle\";" +
        "\n            frame = CFrame::fromXYZYPRDegrees(0,0,4,0,0,0);" +
        "\n        };" +
        "\n" +
        "\n        bottom = VisibleEntity {" +
        "\n            model = \"glassRectangle\";" +
        "\n            frame = CFrame::fromXYZYPRDegrees(0,-2.7f,1,0,90,0);" +
        "\n        };" +
        "\n" +
        "\n        squareSide1 = VisibleEntity {" +
        "\n            model = \"glassSquare\";" +
        "\n            frame = CFrame::fromXYZYPRDegrees(6,0,1.0f,0,0,0);" +
        "\n        };" +
        "\n" +
        "\n        squareSide2 = VisibleEntity {" +
        "\n            model = \"glassSquare\";" +
        "\n            frame = CFrame::fromXYZYPRDegrees(-6,0,1.0f,0,0,0);" +
        "\n        };" +
        "\n" +
        "\n        WATER = VisibleEntity {" +
        "\n            model = \"tankWater\";" +
        "\n            frame = CFrame::fromXYZYPRDegrees(0,1.6f,0.9f,0,0,0);" +
        "\n        };" +
        "\n" +
        "\n        ground = VisibleEntity {" +
        "\n            model = \"tankGround\";" +
        "\n            frame = CFrame::fromXYZYPRDegrees(0,-2.0f,0.9f,0,0,0);" +
        "\n        };" +
        "\n" +
/*        "\n        castle1 = VisibleEntity {" +
        "\n            model = \"castle\";" +
        "\n            frame = CFrame::fromXYZYPRDegrees(4.1f,-1.9f, 2.3f,3,4,5);" +
        "\n        };" +
        "\n" +*/
        "\n        aRock1 = VisibleEntity {" +
        "\n            model = \"rock1\";" +
        "\n            frame = CFrame::fromXYZYPRDegrees(3.3,-1.8f,-0.2f,173.4f,0,0);" +
        "\n        };" +
        "\n" +
        "\n        aRock2 = VisibleEntity {" +
        "\n            model = \"rock3\";" +
        "\n            frame = CFrame::fromXYZYPRDegrees(-0.1,-2.0f,1.1f,-17.5,0,0);" +
        "\n        };" +
        "\n" +
        "\n        aRock3 = VisibleEntity {" +
        "\n            model = \"rock4\";" +
        "\n            frame = CFrame::fromXYZYPRDegrees(-4.0f,-1.8f,2.4f,135.0f,0,0);" +
        "\n        };" +
        "\n" +
        "\n        chest1 = VisibleEntity {" +
        "\n            model = \"chest\";" +
        "\n            frame = CFrame::fromXYZYPRDegrees(-4.3,-2.0f,-0.6f,-141.0f,2.2f,9.9f);" +
        "\n        };" +
        "\n" +
        "\n     ";

    for (int i(0); i < 3; ++i) {
        int fishType = Random::threadCommon().integer(0, 5);
        entities += writeTankFish("fish" + (String)std::to_string(fishType), Random::threadCommon().integer(15, 25), i, Point3(0,0,1), Point2(-4.4f, 4.4f), Point2(-.4f, .4f), 1.6);
    }

    entities += writeBubbles(Point2(-3.5f, 3.5f), Point2(-1.1f, 1.5f), Point3(0.0f, 0.0f, 1.0f), 1.6);
    
    writeCoral(entities, numCoralTypes, totalCoral, Point2(-5.4f, 5.4f), Point2(-2.3f, 2.0f), Point3(0, -1.7f, 1));

    entities += "\n    ";
    entities += "};";
    entities += "\n";

    scene += entities;
}

void CoralSceneGenerator::writeCoral(String& entities, int numCoralTypes, int totalCoral, Point2& xBounds, Point2& zBounds, Point3& center) {
    float y = center.y;
    for (int i(0); i < totalCoral; ++i) {
        String index = (String)std::to_string(Random::threadCommon().integer(1, numCoralTypes));
        int modelNum;
        if (index == "7") {
            modelNum = 1;
        }
        else if (index == "4" || index == "6") {
            modelNum = Random::threadCommon().integer(1, 3);
        }
        else {
            modelNum = Random::threadCommon().integer(1, 2);
        }
        String yPos;
        if (modelNum = 1){
            y += 0.5f;
        } else if (modelNum = 2){
            y+= 1.0f;
        } else if (modelNum = 3){
            y += 2.0f;
        }
        
        yPos = "f, " + (String) std::to_string(y) + "f, ";
        entities += (String) "" +
            "\n\n    coral" + (String)std::to_string(i) + " = VisibleEntity {" +
            "\n        model = \"genericCoralModel" + (String)std::to_string(modelNum) + "\";" +
            "\n        frame = CFrame::fromXYZYPRDegrees(" + (String)std::to_string(center.x + Random::threadCommon().uniform(xBounds.x, xBounds.y)) + yPos + (String)std::to_string(center.z + Random::threadCommon().uniform(zBounds.x, zBounds.y)) + "f , 0, 0, 0);" +
            "\n        articulatedModelPose = UniversalMaterial::Specification {" +
            "\n            lambertian = \"" + CORALDIR + index + ".png\";" +
            "\n            bump = BumpMap::Specification {" +
            "\n                 texture =\"" + CORALDIR + index + "_Bump.png\";" +
            "\n                 settings = BumpMap::Settings {" +
            "\n                     iterations = 0;" +
            "\n                 };" +
            "\n           };" +
            "\n        };" +
            "\n    };" +
            "\n";
        y = center.y;
    }
}


void CoralSceneGenerator::writeSeaCoral(String& entities, int numCoralTypes, int totalCoral, Point2& xBounds, Point2& zBounds, Point3& center, String id) {
    float y = center.y;
    for (int i(0); i < totalCoral; ++i) {
        String index = (String)std::to_string(Random::threadCommon().integer(1, numCoralTypes));
        int modelNum = Random::threadCommon().integer(3, 4);
        
        String yPos;
        if (modelNum = 3){
            y += 1.0f;
        } else if (modelNum = 4){
            y += 1.5f;
        }
        
        yPos = "f, " + (String) std::to_string(y) + "f, ";
        entities += (String) "" +
            "\n\n    coral" + id + (String)std::to_string(i) + " = VisibleEntity {" +
            "\n        model = \"genericCoralModel" + (String)std::to_string(modelNum) + "\";" +
            "\n        frame = CFrame::fromXYZYPRDegrees(" + (String)std::to_string(Random::threadCommon().uniform(xBounds.x, xBounds.y)) + yPos + (String)std::to_string(Random::threadCommon().uniform(zBounds.x, zBounds.y)) + "f , " + (String) std::to_string(Random::threadCommon().uniform(0,180)) + "f, " + (String) std::to_string(Random::threadCommon().uniform(-20,20))+ "f, 0);" +
            "\n        articulatedModelPose = UniversalMaterial::Specification {" +
            "\n            lambertian = \"" + CORALDIR + index + ".png\";" +
            "\n            bump = BumpMap::Specification {" +
            "\n                 texture =\"" + CORALDIR + index + "_Bump.png\";" +
            "\n                 settings = BumpMap::Settings {" +
            "\n                     iterations = 0;" +
            "\n                 };" +
            "\n           };" +
            "\n        };" +
            "\n    };" +
            "\n";
        y = center.y;
    }
}


String CoralSceneGenerator::writeBubbles(Point2& xBounds, Point2& yBounds, Point3& center, float zBound) {
    String bubbles("");
    for (int i(0); i < 75; ++i) {
        float radius = 1.0f;
        float z = Random::threadCommon().uniform(-radius, radius);
        float phi = Random::threadCommon().uniform(0, 2 * pif());
        float theta = asinf(z / radius);
        float x = radius * cosf(theta) * cosf(phi);
        float y = radius * cosf(theta) * sinf(phi);
        bubbles += (String) "    bubble" + (String)std::to_string(i) + " = VisibleEntity {" +
            "\n            model = \"bubble" + (String)std::to_string(Random::threadCommon().integer(0, 2)) + "\";" +
            "\n            frame = CFrame::fromXYZYPRDegrees(" + (String)std::to_string(x + Random::threadCommon().uniform(xBounds.x, xBounds.y) + center.x) + "f , " + (String)std::to_string((y  + Random::threadCommon().uniform(yBounds.x, yBounds.y) + center.y)) +
            "f , " + (String)std::to_string(z * zBound + center.z) + "f , 0.0f, 0.0f, 0.0f);" +
            "\n        };" +
            "\n";
    }
    return bubbles;
}

String CoralSceneGenerator::writeTankFish(const String& fish, int schoolNum, int id, Point3& center, Point2& xBounds, Point2& yBounds, float zBound) {
    String fishSchool("");
    for (int i(0); i < schoolNum; ++i) {
        float radius = 1.6f;
        float z = Random::threadCommon().uniform(-radius, radius);
        float phi = Random::threadCommon().uniform(0, 2 * pif());
        float theta = asinf(z / radius);
        float x = radius * cosf(theta) * cosf(phi);
        float y = radius * cosf(theta) * sinf(phi);
        fishSchool += (String) "    fish" + (String)std::to_string(id) + (String)std::to_string(i) + " = VisibleEntity {" +
            "\n            model = \"";
        fishSchool += fish + "\";" +
            "\n        frame = CFrame::fromXYZYPRDegrees(" + (String)std::to_string(x + Random::threadCommon().uniform(xBounds.x, xBounds.y) + center.x) + "f , ";
        fishSchool += (String)std::to_string(y + Random::threadCommon().uniform(yBounds.x, yBounds.y) + center.y) +
            "f , " + (String)std::to_string(z * zBound + center.z) + "f ," + (String)std::to_string(Random::threadCommon().uniform(0, 360)) + "f, " + (String)std::to_string(Random::threadCommon().uniform(-55, 55)) + "f, 0.0f);" +
            "\n        };" +
            "\n";
    }
    return fishSchool;
}

String CoralSceneGenerator::writeSchoolOfFish(const String& fish, int schoolNum, int id, Point3& center, Point2& xBounds, Point2& yBounds, float zBound) {
    String fishSchool("");
    float yaw = Random::threadCommon().uniform(0, 360);
    float pitch = Random::threadCommon().uniform(-35, 35);
    for (int i(0); i < schoolNum; ++i) {
        float radius = 1.0f;
        float z = Random::threadCommon().uniform(-radius, radius);
        float phi = Random::threadCommon().uniform(0, 2 * pif());
        float theta = asinf(z / radius);
        float x = radius * cosf(theta) * cosf(phi);
        float y = radius * cosf(theta) * sinf(phi);
        fishSchool += (String) "    fish" + (String)std::to_string(id) + (String)std::to_string(i) + " = VisibleEntity {" +
            "\n            model = \"";
        fishSchool += fish + "\";" +
            "\n        frame = CFrame::fromXYZYPRDegrees(" + (String)std::to_string(x + Random::threadCommon().uniform(xBounds.x, xBounds.y) + center.x) + "f , ";
        fishSchool += (String)std::to_string(y + Random::threadCommon().uniform(yBounds.x, yBounds.y) + center.y) +
            "f , " + (String)std::to_string(z * zBound + center.z) + "f ," + (String)std::to_string(yaw) + "f, " + (String)std::to_string(pitch) + "f, 0.0f);" +
            "\n        };" +
            "\n";
    }
    return fishSchool;
}

void CoralSceneGenerator::writeModels(String& scene, int numCoralTypes, int totalCoral) {
    scene += (String) "" +
        "models = {" +
        "\n        genericCoralModel1 = ArticulatedModel::Specification {" +
        "\n            filename = \"star.off\";" +
        "\n            preprocess = {" +
        "\n                setTwoSided(all(), true);" +
        "\n            };" +
        "\n        };" +
        "\n        " +
        "\n       genericCoralModel2 = ArticulatedModel::Specification {" +
        "\n            filename = \"star.off\";" +
        "\n            scale = 1.5;" +
        "\n            preprocess = {" +
        "\n                setTwoSided(all(), true);" +
        "\n            };" +
        "\n        };" +
        "\n        " +
        "\n       genericCoralModel3 = ArticulatedModel::Specification {" +
        "\n            filename = \"star.off\";" +
        "\n            scale = 2;" +
        "\n            preprocess = {" +
        "\n                setTwoSided(all(), true);" +
        "\n            };" +
        "\n        };" +
        "\n        " +
        "\n       genericCoralModel4 = ArticulatedModel::Specification {" +
        "\n            filename = \"star.off\";" +
        "\n            scale = 2.3;" +
        "\n            preprocess = {" +
        "\n                setTwoSided(all(), true);" +
        "\n            };" +
        "\n        };" +
        "\n        " +
        "\n        ";

    Array<String> allFish = FISH;
    for (int i(0); i < 6; ++i) {
        int whichFish = Random::threadCommon().integer(0, 14 - i);
        String fish = allFish[whichFish];
        scene += (String) "\n" +
            "fish" + (String)std::to_string(i) + " = ArticulatedModel::Specification {" +
            "\n            filename = \"" + fish + "\";" +
            "\n            scale = " +(String) std::to_string(Random::threadCommon().uniform(0.0005f, 0.001f)) + ";" +
            "\n        };" +
            "\n" +
            "\n        ";
        allFish.remove(whichFish);
    }
    
   /* for (int i(0); i < 6; ++i) {w
        int whichShell = Random::threadCommon().integer(0,1);
        String shell = (whichShell) ? SHELL1 : SHELL2;
        scene += "\n    shell" + (String)std::to_string(i) + " = ArticulatedModel::Specification {" +
            "\n            filename = \"" + shell + "\";" +
            "\n        scale = " + (String)std::to_string(Random::threadCommon().uniform(0.5f, 1.0f)) + ";" +
            "\n    };" +
            "\n";
    }
    */
    for (int i(0); i < 3; ++i) {
        scene += "\n    bubble" + (String)std::to_string(i) + " = ArticulatedModel::Specification {" +
            "\n        filename = \"ifs/sphere.ifs\";" +
            "\n        scale = " + (String)std::to_string(Random::threadCommon().uniform(0.025f, 0.05f)) + ";" +
            "\n        preprocess = {" +
            "\n            setMaterial(all(), " +
            "\n                UniversalMaterial::Specification {" +
            "\n                    lambertian = Color3(0.02);" +
            "\n                    glossy = Color4(Color3(0.3), 0.8);" +
            "\n                    transmissive = Color3(0.9, 1.0, 1.0);" +
            "\n                    emissive = Color3(0.01);" +
            "\n                    etaReflect = 1.03f;" +
            "\n                    etaTransmit = 1.00f;" +
            "\n            });" +
            "\n        };" +
            "\n    };" +
            "\n";
    }

    scene += (String) "" +
   /*     "\n    shark1 = ArticulatedModel::Specification {" +
        "\n        filename = \"" + SHARK1 + "\";" +
        "\n        scale = 0.15f;" +
        "\n    };" +
        "\n" +
        "\n    shark2 = ArticulatedModel::Specification {" +
        "\n        filename = \"" + SHARK2 + "\";" +
        "\n        scale = 50.0f;" +
        "\n    };" +
        "\n" +
        "\n    castle = ArticulatedModel::Specification {" +
        "\n        filename = \"" + CASTLE + "\";" +
        "\n        scale = .002f;" +
        "\n    };" +
        "\n" +*/
        "\n    chest = ArticulatedModel::Specification {" +
        "\n        filename = \"" + CHEST + "\";" +
        "\n        scale = 1.0f;" +
        "\n    };" +
        "\n" +
        "\n    tankWater = ArticulatedModel::Specification {" +
        "\n        filename = \"water.obj\";" +
        "\n        preprocess = {" +
        "\n            transformGeometry(all(), Matrix4::scale(5.8f,1.0f,2.8f));" +
        "\n        };" +
        "\n    };" +
        "\n" +
        "\n    seaWater = ArticulatedModel::Specification {" +
        "\n        filename = \"water.obj\";" +
        "\n        preprocess = {" +
        "\n            transformGeometry(all(), Matrix4::scale(50.0f,50.0f,50.0f));" +
        "\n        };" +
        "\n    };" +
        "\n" +
        "\n    blueRectangle = ArticulatedModel::Specification {" +
        "\n        filename = \"model/cube/cube.obj\";" +
        "\n        preprocess = {" +
        "\n            setMaterial(all(), " +
        "\n                UniversalMaterial::Specification {" +
        "\n                    lambertian = Color3(0, 0.807843, 0.819608);" +
        "\n            });" +
        "\n            transformGeometry(all(), Matrix4::scale(12.6f,6.0f,0.6f));" +
        "\n        };" +
        "\n    };" +
        "\n" +
        "\n    glassRectangle = ArticulatedModel::Specification {" +
        "\n        filename = \"model/cube/cube.obj\";" +
        "\n        preprocess = {" +
        "\n            setMaterial(all(), " +
        "\n                UniversalMaterial::Specification {" +
        "\n                    lambertian = Color3(0.02);" +
        "\n                    glossy = Color4(Color3(0.3), 0.8);" +
        "\n                    transmissive = Color3(0.9, 1.0, 1.0);" +
        "\n                    emissive = Color3(0.01);" +
        "\n                    etaReflect = 1.0;" +
        "\n                    etaTransmit = 1.03;" +
        "\n            });" +
        "\n            transformGeometry(all(), Matrix4::scale(12.6f,6.0f,0.6f));" +
        "\n        };" +
        "\n    };" +
        "\n" +
        "\n    glassSquare = ArticulatedModel::Specification {" +
        "\n        filename = \"model/cube/cube.obj\";" +
        "\n        preprocess = {" +
        "\n            setMaterial(all(), " +
        "\n                UniversalMaterial::Specification {" +
        "\n                    lambertian = Color3(0.02);" +
        "\n                    glossy = Color4(Color3(0.3), 0.8);" +
        "\n                    transmissive = Color3(0.9, 1.0, 1.0);" +
        "\n                    emissive = Color3(0.01);" +
        "\n                    etaReflect = 1.0;" +
        "\n                    etaTransmit = 1.03;" +
        "\n            });" +
        "\n            transformGeometry(all(), Matrix4::scale(0.6f,6.0f,6.0f));" +
        "\n        };" +
        "\n    };" +
        "\n" +
        "\n    tankGround = ArticulatedModel::Specification {" +
        "\n        filename = \"model/cube/cube.obj\";" +
        "\n        preprocess = {" +
        "\n            transformGeometry(all(), Matrix4::scale(11.6f,0.6f,5.6f));" +
        "\n            setMaterial(all()," +
        "\n                UniversalMaterial::Specification {" +
        "\n                    lambertian = \"ground.jpg\";" +
        "\n                    bump = \"groundBump.jpg\";" +
        "\n            });" +
        "\n        };" +
        "\n    };" +
        "\n" +
        "\n    seaGround = ArticulatedModel::Specification {" +
        "\n        filename = \"model/cube/cube.obj\";" +
        "\n        preprocess = {" +
        "\n            transformGeometry(all(), Matrix4::scale(50.0f,0.6f,50.0f));" +
        "\n            setMaterial(all()," +
        "\n                UniversalMaterial::Specification {" +
        "\n                    lambertian = \"ground.jpg\";" +
        "\n                    bump = \"groundBump.jpg\";" +
        "\n            });" +
        "\n        };" +
        "\n    };" +
        "\n" +
        "\n    rock1 = ArticulatedModel::Specification {" +
        "\n        filename = \"" + ROCK5 + "\";" +
        "\n        preprocess = {" +
        "\n            transformGeometry(all(), Matrix4::scale(0.1f,0.1f,0.1f));" +
        "\n        };" +
        "\n    };" +
        "\n" +
        "\n    rock2 = ArticulatedModel::Specification {" +
        "\n        filename = \"" + ROCK4 + "\";" +
        "\n        preprocess = {" +
        "\n            transformGeometry(all(), Matrix4::scale(0.045f,0.045f,0.045f));" +
        "\n        };" +
        "\n    };" +
        "\n" +
        "\n    rock3 = ArticulatedModel::Specification {" +
        "\n        filename = \"" + ROCK2 + "\";" +
        "\n        preprocess = {" +
        "\n            transformGeometry(all(), Matrix4::scale(0.1f,0.1f,0.1f));" +
        "\n        };" +
        "\n    };" +
        "\n" +
        "\n    rock4 = ArticulatedModel::Specification {" +
        "\n        filename = \"" + ROCK1 + "\";" +
        "\n        preprocess = {" +
        "\n            transformGeometry(all(), Matrix4::scale(0.1f,0.1f,0.1f));" +
        "\n        };" +
        "\n    };" +
        "\n" +
        "\n    largeRock1 = ArticulatedModel::Specification {" +
        "\n        filename = \"" + ROCK5 + "\";" +
        "\n        scale = 0.15f;" +
        "\n    };" +
        "\n" +
        "\n    largeRock2 = ArticulatedModel::Specification {" +
        "\n        filename = \"" + ROCK4 + "\";" +
        "\n        scale = 0.15f;" +
        "\n    };" +
        "\n" +
        "\n    largeRock3 = ArticulatedModel::Specification {" +
        "\n        filename = \"" + ROCK2 + "\";" +
        "\n        scale = 0.15f;" +
        "\n    };" +
        "\n" +
        "\n    largeRock4 = ArticulatedModel::Specification {" +
        "\n        filename = \"" + ROCK1 + "\";" +
        "\n        scale = 0.15f;" +
        "\n    };" +
        "\n" +
        "\n};" +
        "\n\n";


}

void CoralSceneGenerator::writeTankScene(String& scene, String& name, int numCoralTypes, int totalCoral) {
    scene = "/* -*- c++ -*- */\n{\nname = \"" + name + "\";\n\n";
    writeModels(scene, numCoralTypes, totalCoral);
    writeTankEntities(scene, numCoralTypes, totalCoral);
    scene += "};";
}

void CoralSceneGenerator::writeSeaScene(String& scene, String& name, int numCoralTypes, int totalCoral) {
    scene = "/* -*- c++ -*- */\n{\nname = \"" + name + "\";\n\n";
    writeModels(scene, numCoralTypes, totalCoral);
    writeSeaEntities(scene, numCoralTypes, totalCoral);
    scene += "};";
}
