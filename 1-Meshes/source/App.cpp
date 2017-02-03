/** \file App.cpp */
#include "App.h"

// Tells C++ to invoke command-line main() function even on OS X and Win32.
G3D_START_AT_MAIN();

int main(int argc, const char* argv[]) {
    {
        G3DSpecification g3dSpec;
        g3dSpec.audio = false;
        initGLG3D(g3dSpec);
    }

    GApp::Settings settings(argc, argv);

    // Change the window and other startup parameters by modifying the
    // settings class.  For example:
    settings.window.caption             = argv[0];

    // Set enable to catch more OpenGL errors
    // settings.window.debugContext     = true;

    // Some common resolutions:
    // settings.window.width            =  854; settings.window.height       = 480;
    // settings.window.width            = 1024; settings.window.height       = 768;
    settings.window.width               = 1280; settings.window.height       = 720;
    //settings.window.width             = 1920; settings.window.height       = 1080;
    // settings.window.width            = OSWindow::primaryDisplayWindowSize().x; settings.window.height = OSWindow::primaryDisplayWindowSize().y;
    settings.window.fullScreen          = false;
    settings.window.resizable           = ! settings.window.fullScreen;
    settings.window.framed              = ! settings.window.fullScreen;

    // Set to true for a significant performance boost if your app can't render at 60fps, or if
    // you *want* to render faster than the display.
    settings.window.asynchronous        = false;

    settings.hdrFramebuffer.depthGuardBandThickness = Vector2int16(64, 64);
    settings.hdrFramebuffer.colorGuardBandThickness = Vector2int16(0, 0);
    settings.dataDir                    = FileSystem::currentDirectory();
    settings.screenshotDirectory        = "../journal/";

    settings.renderer.deferredShading = true;
    settings.renderer.orderIndependentTransparency = false;

    return App(settings).run();
}


App::App(const GApp::Settings& settings) : GApp(settings) {
}


/** Called before the application loop begins.  Load data here and
// not in the constructor so that common exceptions will be
// automatically caught.*/
void App::onInit() {
    //makeGlass(37);
    //makeCylinder(5, 4);
    //generateHeightField("apple-icon.png");
    debugPrintf("Target frame rate = %f Hz\n", realTimeTargetDuration());
    GApp::onInit();
    setFrameDuration(1.0f / 120.0f);
    // Call setScene(shared_ptr<Scene>()) or setScene(MyScene::create()) to replace
    // the default scene here.
    
    showRenderingStats      = false;
    
    makeGUI();
    // For higher-quality screenshots:
    // developerWindow->videoRecordDialog->setScreenShotFormat("PNG");
    // developerWindow->videoRecordDialog->setCaptureGui(false);
    developerWindow->cameraControlWindow->moveTo(Point2(developerWindow->cameraControlWindow->rect().x0(), 0));
   
    loadScene(
        //"G3D Sponza"
        "White Cube" // Load something simple
        //developerWindow->sceneEditorWindow->selectedSceneName()  // Load the first scene encountered 
        );
}

/** Create a Cylinder OFF File that has a given radius, height and uses "slices" to determine how circular the shape will be*/
void App::makeCylinder(float radius, float height, int slices){
    TextOutput output = TextOutput("model/cylinder.off");
    
    //Start the file by demarking it as an OFF file
    output.writeSymbols("OFF");

    //Begin the Scene File
    output.writeNewlines(2);
    output.writeSymbols((String) std::to_string(2 * (slices + 1)) + " " + (String) std::to_string(slices*2 + slices) + " 1");
    output.writeNewlines(1);

    //Create the vertices
    String cylVertices(generateCylVertices(radius, height/2, slices));
    cylVertices += generateCylVertices(radius, -1 * height/2, slices);
    output.writeSymbols(cylVertices);

    String cylSurface("\n\n");
    cylSurface += generateCylLids(0, slices) + generateCylSides(0, 1, slices);
    output.writeSymbols(cylSurface);
    
    output.commit(true);
}

/** Create a glass OFF file with the specified number of slices 
    
    Depends on generateCylVertices, generateCylSides,generateCylLids, and generateDoughnutLid.
*/
void App::makeGlass(int slices){
    TextOutput output = TextOutput("model/glass.off");
    
    //Start the file by demarking it as an OFF File
    output.writeSymbols("OFF");

    // Create the glass from top to bottom
    // Generate the outside top of the glass (where water is held)
    String cylVertices(generateCylVertices(.8, 3, slices));
    cylVertices += generateCylVertices(1, 2.5, slices);
    cylVertices += generateCylVertices(1.2, 2, slices);
    cylVertices += generateCylVertices(1.2, 1.8, slices);
    cylVertices += generateCylVertices(1, 1.2, slices);

    //Generate the bottom of the glass (the parts that hold it up)
    cylVertices += generateCylVertices(.6, 0.5, slices);
    cylVertices += generateCylVertices(.1, 0, slices);
    cylVertices += generateCylVertices(.1, -2.5, slices);
    cylVertices += generateCylVertices(.8, -3, slices);

    //Generate the inside of the glass, which holds the water in place
    cylVertices += generateCylVertices(.75, 3, slices);
    cylVertices += generateCylVertices(.95, 2.5, slices);
    cylVertices += generateCylVertices(1.15, 2, slices);
    cylVertices += generateCylVertices(1.15, 1.8, slices);
    cylVertices += generateCylVertices(.95, 1.2, slices);
    cylVertices += generateCylVertices(.55, 0.7, slices);
    cylVertices += generateCylVertices(.25, 0.65, slices);

    // Generate the outward facing (outside) of the glass
    String cylSides("\n\n");
    for(int i(0); i < 8; ++i){
        cylSides += generateCylSides(i*(slices+1), 1, slices);
    }
    // Generate the inward facing (inside) of the glass
    for(int x(9); x < 15; ++x){
        cylSides += generateCylSides(x*(slices+1), 0, slices);
    }
    String cylLids("");
    cylLids += generateCylLids(6*(slices+1), slices);
    cylLids += generateCylLids(7*(slices+1), slices);
    cylLids += generateCylLids(14*(slices+1), slices);
    
    // Connect the inner portion to the outer cup
    cylLids += generateDoughnutLid(0, 9*(slices+1), slices);
    
    //Print out the off file
    output.writeNewlines(2);
    output.writeSymbols((String) std::to_string((slices+1) * 16) + " " + (String) std::to_string((slices)*14+(slices)*2*3+(slices)) + " 1");
    output.writeNewlines(1);
    
    output.writeSymbols(cylVertices, cylSides, cylLids);


    output.commit(true);
}

/** Creates a Doughnut shaped lid around two sets of vertices. These sets begin from their given start location (firstSet and secondSet) and end at that location plus the number of slices. 
    This generates a total of "slices" number of faces.
*/
String App::generateDoughnutLid(float firstSet, float secondSet, int slices){
    String doughnutLid("");
    for (int i(1); i < slices+1; ++i) {
            doughnutLid += "4 " + (String) std::to_string(((i + 1) % slices) + secondSet) + " " + (String) std::to_string((i%slices) + secondSet) + " " + (String) std::to_string(i-1 + firstSet)  + " " + (String) std::to_string(i%slices + firstSet) + "\n";
    }
    return doughnutLid;
}

/* Generate the Vertices for a cylinder with the given radius, at a height of y and with a given number of slices. 
    This generates a total of slices + 1 number of vertices, where the extra vertex is the center of the lids.
*/
String App::generateCylVertices(float radius, float y, int slices){
    String vertices("");
    vertices += "\n";

    for (int i(0); i < slices; ++i){
        float a(2 * pif() * i/slices);
        float z(cos(a)*radius);
        float x(sin(a) * radius);
        vertices += (String) std::to_string(x) + " " + (String) std::to_string(y) + " " + (String) std::to_string(z) + "\n"; 
    }
    vertices += "0 " + (String) std::to_string(y) + " 0";
    return vertices;
}

/** Generates the top and bottom "lids" of the cylinder. The offset denotes the location of the vertices to connect from the beginning of the vertex array in an OFF file. The sets are determined to be "slices" large.
    A total of slices*2 number of faces are created.
*/
String App::generateCylLids(int offset, int slices){
    String cylLids("");
    for(int i(0); i < slices; ++i){
        cylLids += "3 " + (String) std::to_string(offset + slices) + " " + (String) std::to_string(i + offset) + " " + (String) std::to_string((i+1) % slices + offset) + "\n";
        cylLids += "3 " + (String) std::to_string(offset + slices*2+1) + " " + (String) std::to_string(((i+1) % slices) + slices+1 + offset) + " " + (String) std::to_string(i+slices+1 + offset)  + "\n";
    }   

    return cylLids;
}

/** Generates the sides of a cylinder. The offset denotes the location of the vertex set to be connected in an OFF file vertex array. The sets are determined to be "slices" large.
    Outward denotes whether the direction which the faces should face, where outward is away from the y axis.
    A total of slices number of faces are created.
*/
String App::generateCylSides(int offset, char outward, int slices){
    String cylSides("");

    if(outward){
        for (int i(1); i < slices+1; ++i) {
            cylSides += "4 " + (String) std::to_string(i%slices + offset) + " " +  (String) std::to_string(i-1 + offset) + " " + (String) std::to_string((i%slices) + slices+1 + offset) + " " + (String) std::to_string(((i + 1) % slices) + slices+1 + offset) + "\n";
        }
    } else {
        for (int i(1); i < slices+1; ++i) {
            cylSides += "4 " + (String) std::to_string(((i + 1) % slices) + slices+1 + offset) + " " +  (String) std::to_string((i%slices) + slices+1 + offset) + " " + (String) std::to_string(i-1 + offset) + " " + (String) std::to_string(i%slices + offset) + "\n";
        }
    }
    return cylSides;
}

// Generate a height field based on a given image from the GUI.
void App::generateHeightField(shared_ptr<G3D::Image> image, float yScale, float xzScale){
    TextOutput output = TextOutput("model/heightfield.off");
    try{
        String heightField("");
        Color3 col = Color3();
        for(int x(0); x < (image->width() ); ++x){
            for(int z(0); z < (image->height() ); ++z){
                image->get(Point2int32(x,z), col);
                float grey = col.average();
                heightField += (String) std::to_string(x*xzScale) + " " + (String) std::to_string(grey*yScale) + " " + (String) std::to_string(z*xzScale) + "\n";
            }
        }
       
        String fieldSurface("");
        int imageHeight = image->height() - 1;
        int imageWidth = image->width() - 1;
        for(int i(1); i <= (imageHeight) ; ++i){
            for(int y(1); y <= (imageWidth); ++y){
                fieldSurface += "4 " + (String) std::to_string(i + (imageHeight*y)) + " " + (String) std::to_string((i + (imageHeight * y)) - 1) + " " + (String) std::to_string(((i-1) + ((y-1) * imageHeight))) + " " + (String) std::to_string(i + (imageHeight*(y-1)))  + "\n";
            }
        }

        String logistics("OFF\n\n" + (String) std::to_string((imageHeight+1) * (imageWidth+1)) + " " + (String) std::to_string((imageHeight) * (imageWidth)) + " 1" );

        output.writeSymbols(logistics,"\n\n",heightField, "\n\n", fieldSurface);
        output.commit(true);
    } catch (...){
        msgBox("NOOOOOOOOOO");
    }
}

/** Function used to create the staircase scene. It takes the name of the scene, 
    and then generates a .Scene.Any file for it through the TextOutput class.
*/
void App::createScene(String sceneName){
    TextOutput output = TextOutput("scene\\"+ sceneName + ".Scene.Any");
    
    //Start the file by demarking it as a C++ File
    output.writeSymbols("/* -*- c++ -*- */");

    //Begin the Scene File
    output.writeNewlines(2);
    output.writeSymbols("{\tname = \"" + sceneName + "\";\n    ");
    
    //Create the models for the scene
    String modelString(generateModels());
    output.writeSymbols(modelString);

    //Create the entities
    String entitiesString(generateEntities());
    output.writeSymbols(entitiesString);

    // End the File
    output.writeSymbols("}");
    output.commit(true);
}

/** Creates the string for the models that are to be used in the staircase scene.
*/
String App::generateModels(){
    String models("models = {");
    models += "\n        ";
    models += "stairModel = ArticulatedModel::Specification {";
    models += "\n            ";
    models += "filename = \"model/crate/crate.obj\";";
    models+= "\n            ";
    models += "preprocess = {";
    models += "\n                ";
    models += "transformGeometry(all(),Matrix4::scale(.05, .01, .03));";
    models += "\n                ";
    models += "transformGeometry(all(),Matrix4::translation(.05, 0, .05));";
    models += "\n                ";
    models += "setMaterial(all(),\"wood.jpg\");";
    models += "\n            ";
    models += "};";
    models += "\n        ";
    models += "};";
    models += "\n    ";
    models += "};";
    models += "\n";
    return models;
}

/** Creates the string for the entities that will be used in the staircase scene. */
String App::generateEntities(){
    String entities("entities = {");

    // Set the Skybox
    entities += "\n        ";
    entities += "skybox = Skybox {";
    entities += "\n            ";
    entities += "texture = \"cubemap/whiteroom/whiteroom-*.png\";";
    entities += "\n        ";
    entities += "};";

    // Set the Camera
    entities += "\n        ";
    entities += "camera = Camera {";
    entities += "\n            ";
    entities += "frame = CFrame::fromXYZYPRDegrees( -0.1, 1.4, 0, -90,-85);";
    entities += "\n        ";
    entities += "};";

    //Set the Stairs
    entities += "\n        ";
    entities += generateStairs();

    entities += "\n    ";
    entities += "};";
    entities += "\n";
    return entities;
}

/** Generates the code for the stairs' entities that will be used in the staircase scene.*/
String App::generateStairs(){
    int num(0);
    float y(0);
    float x(0);
    float z(0);
    int yaw(0);

    String stairs("");
    char c = 0;
    while (y<.5){
        stairs += "stair" + (String) std::to_string(num++) + " = VisibleEntity {";
        stairs += "\n            ";
        stairs += "model = \"stairModel\";";
        stairs += "\n            ";
        stairs += "frame = CFrame::fromXYZYPRDegrees(" + (String) std::to_string(x) + ", " + (String) std::to_string(y) + ", " + (String) std::to_string(z) + 
            ", " + (String) std::to_string(yaw) + ", 0.0, 0.0);";
        stairs += "\n        ";
        stairs += "};";
        stairs += "\n        ";
        yaw += 12;
        y+=0.01;
    }
    return stairs;
}

void App::makeGUI() {
    // Initialize the developer HUD
    createDeveloperHUD();

    debugWindow->setVisible(true);
    developerWindow->videoRecordDialog->setEnabled(true);
    
    GuiPane* heightfieldPane = debugPane->addPane("Heightfield");
    
    heightfieldPane->setNewChildSize(240);
    
    heightfieldPane->beginRow(); {
        heightfieldPane->addTextBox("Input Image", &m_heightfieldSource)->setWidth(210);
        
        
        heightfieldPane->addButton("...", [this]() {
            FileDialog::getFilename(m_heightfieldSource, "png", false);
        })->setWidth(30);
    } heightfieldPane->endRow();

    heightfieldPane->addNumberBox("Y Scale", &m_heightfieldYScale, "m", GuiTheme::LOG_SLIDER, 0.001f, 100.0f) -> setUnitsSize(30); 
    heightfieldPane->addNumberBox("XZ Scale", &m_heightfieldXZScale, "m/px", GuiTheme::LOG_SLIDER, 0.001f, 100.0f) -> setUnitsSize(30); 

    heightfieldPane->addButton("Generate", [this](){
        shared_ptr<G3D::Image> image;
        try {
            drawMessage("Generating Your HeightField. Please Stand By.");
            image = Image::fromFile(m_heightfieldSource);
            App::generateHeightField(image, m_heightfieldYScale, m_heightfieldXZScale);
            G3D::ArticulatedModel::clearCache();
            loadScene(developerWindow->sceneEditorWindow->selectedSceneName());
        } catch(...){
            msgBox("Unable to Load the image.", m_heightfieldSource);
        }
    });
    heightfieldPane->pack();

    GuiPane* cylinderPane = debugPane->addPane("Cylinder/Glass Generator");
    
    cylinderPane->setNewChildSize(240);
   
    cylinderPane->addNumberBox("Radius", &m_cylRadius, "m", GuiTheme::LOG_SLIDER, 0.001f, 100.0f) -> setUnitsSize(30); 
    cylinderPane->addNumberBox("Height", &m_cylHeight, "m", GuiTheme::LOG_SLIDER, 0.001f, 100.0f) -> setUnitsSize(30); 
    cylinderPane->addNumberBox("Slices", &m_cylSlices, "#", GuiTheme::LOG_SLIDER, 1, 500) -> setUnitsSize(30);

    cylinderPane->beginRow();{
        cylinderPane->addButton("Generate Cylinder", [this](){
            try {
                drawMessage("Generating Your Cylinder. Please Stand By.");
                App::makeCylinder(m_cylRadius, m_cylHeight, m_cylSlices);
                G3D::ArticulatedModel::clearCache();
                loadScene(developerWindow->sceneEditorWindow->selectedSceneName());
            } catch(...){
                msgBox("Error While Generating Cylinder", m_heightfieldSource);
            }
        });

        cylinderPane->addButton("Generate Glass", [this](){
            try {
                drawMessage("Generating Your Glass. Please Stand By.");
                App::makeGlass(m_cylSlices);
                G3D::ArticulatedModel::clearCache();
                loadScene(developerWindow->sceneEditorWindow->selectedSceneName());
            } catch(...){
                msgBox("Error While Generating Cylinder", m_heightfieldSource);
            }
        });
    } cylinderPane->endRow();
    cylinderPane->addLabel("NOTE: Generating a Glass Ignores the Above Selected Height and Radius.");
    cylinderPane->pack();

    GuiPane* infoPane = debugPane->addPane("Info", GuiTheme::ORNATE_PANE_STYLE);
    
    // Example of how to add debugging controls
    infoPane->addLabel("Click the Button Below to Exit.");
    infoPane->addButton("Exit", [this]() { m_endProgram = true; });
    infoPane->pack();

    debugWindow->pack();
    debugWindow->setRect(Rect2D::xywh(0, 0, (float)window()->width(), debugWindow->rect().height()));
    
}


// This default implementation is a direct copy of GApp::onGraphics3D to make it easy
// for you to modify. If you aren't changing the hardware rendering strategy, you can
// delete this override entirely.
void App::onGraphics3D(RenderDevice* rd, Array<shared_ptr<Surface> >& allSurfaces) {
    if (!scene()) {
        if ((submitToDisplayMode() == SubmitToDisplayMode::MAXIMIZE_THROUGHPUT) && (!rd->swapBuffersAutomatically())) {
            swapBuffers();
        }
        rd->clear();
        rd->pushState(); {
            rd->setProjectionAndCameraMatrix(activeCamera()->projection(), activeCamera()->frame());
            drawDebugShapes();
        } rd->popState();
        return;
    }

    GBuffer::Specification gbufferSpec = m_gbufferSpecification;
    extendGBufferSpecification(gbufferSpec);
    m_gbuffer->setSpecification(gbufferSpec);
    m_gbuffer->resize(m_framebuffer->width(), m_framebuffer->height());
    m_gbuffer->prepare(rd, activeCamera(), 0, -(float)previousSimTimeStep(), m_settings.hdrFramebuffer.depthGuardBandThickness, m_settings.hdrFramebuffer.colorGuardBandThickness);

    m_renderer->render(rd, m_framebuffer, scene()->lightingEnvironment().ambientOcclusionSettings.enabled ? m_depthPeelFramebuffer : shared_ptr<Framebuffer>(),
        scene()->lightingEnvironment(), m_gbuffer, allSurfaces);

    // Debug visualizations and post-process effects
    rd->pushState(m_framebuffer); {
        // Call to make the App show the output of debugDraw(...)
        rd->setProjectionAndCameraMatrix(activeCamera()->projection(), activeCamera()->frame());
        drawDebugShapes();
        const shared_ptr<Entity>& selectedEntity = (notNull(developerWindow) && notNull(developerWindow->sceneEditorWindow)) ? developerWindow->sceneEditorWindow->selectedEntity() : shared_ptr<Entity>();
        scene()->visualize(rd, selectedEntity, allSurfaces, sceneVisualizationSettings(), activeCamera());

        // Post-process special effects
        m_depthOfField->apply(rd, m_framebuffer->texture(0), m_framebuffer->texture(Framebuffer::DEPTH), activeCamera(), m_settings.hdrFramebuffer.depthGuardBandThickness - m_settings.hdrFramebuffer.colorGuardBandThickness);

        m_motionBlur->apply(rd, m_framebuffer->texture(0), m_gbuffer->texture(GBuffer::Field::SS_EXPRESSIVE_MOTION),
            m_framebuffer->texture(Framebuffer::DEPTH), activeCamera(),
            m_settings.hdrFramebuffer.depthGuardBandThickness - m_settings.hdrFramebuffer.colorGuardBandThickness);
    } rd->popState();

    // We're about to render to the actual back buffer, so swap the buffers now.
    // This call also allows the screenshot and video recording to capture the
    // previous frame just before it is displayed.
    if (submitToDisplayMode() == SubmitToDisplayMode::MAXIMIZE_THROUGHPUT) {
        swapBuffers();
    }

    // Clear the entire screen (needed even though we'll render over it, since
    // AFR uses clear() to detect that the buffer is not re-used.)
    rd->clear();

    // Perform gamma correction, bloom, and SSAA, and write to the native window frame buffer
    m_film->exposeAndRender(rd, activeCamera()->filmSettings(), m_framebuffer->texture(0), settings().hdrFramebuffer.colorGuardBandThickness.x + settings().hdrFramebuffer.depthGuardBandThickness.x, settings().hdrFramebuffer.depthGuardBandThickness.x);
}


void App::onAI() {
    GApp::onAI();
    // Add non-simulation game logic and AI code here
}


void App::onNetwork() {
    GApp::onNetwork();
    // Poll net messages here
}


void App::onSimulation(RealTime rdt, SimTime sdt, SimTime idt) {
    GApp::onSimulation(rdt, sdt, idt);

    // Example GUI dynamic layout code.  Resize the debugWindow to fill
    // the screen horizontally.
    debugWindow->setRect(Rect2D::xywh(0, 0, (float)window()->width(), debugWindow->rect().height()));
}


bool App::onEvent(const GEvent& event) {
    // Handle super-class events
    if (GApp::onEvent(event)) { return true; }

    // If you need to track individual UI events, manage them here.
    // Return true if you want to prevent other parts of the system
    // from observing this specific event.
    //
    // For example,
    // if ((event.type == GEventType::GUI_ACTION) && (event.gui.control == m_button)) { ... return true; }
    // if ((event.type == GEventType::KEY_DOWN) && (event.key.keysym.sym == GKey::TAB)) { ... return true; }
    // if ((event.type == GEventType::KEY_DOWN) && (event.key.keysym.sym == 'p')) { ... return true; }

    return false;
}


void App::onUserInput(UserInput* ui) {
    GApp::onUserInput(ui);
    (void)ui;
    // Add key handling here based on the keys currently held or
    // ones that changed in the last frame.
}


void App::onPose(Array<shared_ptr<Surface> >& surface, Array<shared_ptr<Surface2D> >& surface2D) {
    GApp::onPose(surface, surface2D);

    // Append any models to the arrays that you want to later be rendered by onGraphics()
}


void App::onGraphics2D(RenderDevice* rd, Array<shared_ptr<Surface2D> >& posed2D) {
    // Render 2D objects like Widgets.  These do not receive tone mapping or gamma correction.
    Surface2D::sortAndRender(rd, posed2D);
}


void App::onCleanup() {
    // Called after the application loop ends.  Place a majority of cleanup code
    // here instead of in the constructor so that exceptions can be caught.
}

