#pragma once
#include <G3D/G3DAll.h>
#include "App.h"
#include "CoralGenerator.h"
#include "Rasterizer.h"
#include "CoralGenerator3D.h"

/** Coral Scene Generator generates two separate scenes for our Coral. Models are available online to add to the appearance of the scenes.
    NOTE: Models such as SHARK and SHELL were too large to commit, and are therefore commented out in the implementation of the code.*/
class CoralSceneGenerator {
private:
    // All Possible Fish Models
    /** Tropical Fish Created by ZivixLLC Found @ http://www.turbosquid.com/FullPreview/Index.cfm/ID/652729 */
    const String FISH01 = "Models/TropicalFish.zip/TropicalFish/TropicalFish01.obj";
    const String FISH02 = "Models/TropicalFish.zip/TropicalFish/TropicalFish02.obj";
    const String FISH03 = "Models/TropicalFish.zip/TropicalFish/TropicalFish03.obj";
    const String FISH04 = "Models/TropicalFish.zip/TropicalFish/TropicalFish04.obj";
    const String FISH05 = "Models/TropicalFish.zip/TropicalFish/TropicalFish05.obj";
    const String FISH06 = "Models/TropicalFish.zip/TropicalFish/TropicalFish06.obj";
    const String FISH07 = "Models/TropicalFish.zip/TropicalFish/TropicalFish07.obj";
    const String FISH08 = "Models/TropicalFish.zip/TropicalFish/TropicalFish08.obj";
    const String FISH09 = "Models/TropicalFish.zip/TropicalFish/TropicalFish09.obj";
    const String FISH10 = "Models/TropicalFish.zip/TropicalFish/TropicalFish10.obj";
    const String FISH11 = "Models/TropicalFish.zip/TropicalFish/TropicalFish11.obj";
    const String FISH12 = "Models/TropicalFish.zip/TropicalFish/TropicalFish12.obj";
    const String FISH13 = "Models/TropicalFish.zip/TropicalFish/TropicalFish13.obj";
    const String FISH14 = "Models/TropicalFish.zip/TropicalFish/TropicalFish14.obj";
    const String FISH15 = "Models/TropicalFish.zip/TropicalFish/TropicalFish15.obj";
    
    const Array<String> FISH = {FISH01, FISH02, FISH03, FISH04, FISH05, FISH06, FISH07, FISH08, FISH09, FISH10, FISH11, FISH12, FISH13, FISH14, FISH15};

    //All Other Models
    /**First castle created by Captainmarlowe Found @ http://www.turbosquid.com/FullPreview/Index.cfm/ID/552087 */
    const String CASTLE = "Models/Castle.zip/Castle/Castle.obj";

    /**Second castle created by Captainmarlowe Found @ http://www.turbosquid.com/FullPreview/Index.cfm/ID/552091 */
    const String CASTLE2 = "Models/Castle/Castle2.obj";

    /**Shark created by cartoomotion Found @ http://www.turbosquid.com/FullPreview/Index.cfm/ID/763930*/
    const String SHARK1 = "Models/Shark.zip/Shark/Shark.obj";

    /** Hammerhead (Shark2) created by Kay_xablau found @ http://www.turbosquid.com/FullPreview/Index.cfm/ID/1038510 */
    const String SHARK2 = "Models/Shark.zip/Shark/Shark2.obj";

    /** Low Poly Rocks created by aristarch found @ http://www.turbosquid.com/FullPreview/Index.cfm/ID/234978 */
    const String ROCK1 = "Models/outcrops.zip/granite_outcrops/RockOutcrop1.obj";
    const String ROCK2 = "Models/outcrops.zip/granite_outcrops/RockOutcrop2.obj";
    const String ROCK3 = "Models/outcrops.zip/granite_outcrops/RockOutcrop3.obj";
    const String ROCK4 = "Models/outcrops.zip/granite_outcrops/RockOutcrop4.obj";
    const String ROCK5 = "Models/outcrops.zip/granite_outcrops/RockOutcrop5.obj";

    /**Low Poly Chest created by HD_Ready Found @ http://www.turbosquid.com/FullPreview/Index.cfm/ID/615861 */
    const String CHEST = "Models/Chest/Chest.obj";

    const String CORALDIR = "Models/Coral/Coral";

    /** Realistic Shells created by Tecna found @ http://www.turbosquid.com/FullPreview/Index.cfm/ID/1038793 */
    const String SHELL1 = "Models/Shells.zip/lambis_truncata_shell.obj";
    const String SHELL2 = "Models/Shells.zip/nautilus_shell.obj";

protected:
    
    /** Called by writeTankScene and writeSeaScene. Both Scenes share the same models. */
    void writeModels(String& scene, int numCoralTypes, int totalCoral);

    /** Called by writeTankScene*/
    void writeTankEntities(String& scene, int numCoralTypes, int totalCoral);
    
    /**Called by writeSeaScene*/
    void writeSeaEntities(String& scene, int numCoralTypes, int totalCoral);

    /** Used to generate fish facing in random directions within the given bounds. 
        Called by writeTankEntities and writeSeaEntities. */
    String writeTankFish(const String& Fish, int schoolNum, int id, Point3& center, Point2& xBounds, Point2& yBounds, float zBound);
    
    /** Used to generate fish facing the same direction within the given bounds.
        Called by writeSeaEntities. */
    String writeSchoolOfFish(const String& Fish, int schoolNum, int id, Point3& center, Point2& xBounds, Point2& yBounds, float zBound);

    /** Generates bubble entities within the given bounds offset by the center. 
        Called by writeSeaEntities and writeTankEntities.*/
    String writeBubbles(Point2& xBounds, Point2& yBounds, Point3& center, float zBound);
    
    /** Generates Coral entities within the specified bounds and offset to the center*/
    void writeCoral(String& entities, int numCoralTypes, int totalCoral, Point2& xBounds, Point2& zBounds, Point3& center);
    
    /** Uses larger coral models to create entities for the sea scene. */
    void writeSeaCoral(String& entities, int numCoralTypes, int totalCoral, Point2& xBounds, Point2& zBounds, Point3& center, String id);

public:
    /** Entrance to create the Tank Scene. Fills in the passed in scene string. */
    void writeTankScene(String& scene, String& name, int numCoralTypes, int totalCoral);
    /** Entrance to create the Sea Scene. Fills in the passed in scene string. */
    void writeSeaScene(String& scene, String& name, int numCoralTypes, int totalCoral);
};