/** \file App.cpp */
#include "App.h"
#include "Rasterizer.h"
#include "CoralGenerator.h"
#include "CoralGenerator3D.h"
#include "CoralSceneGenerator.h"

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
    settings.window.caption = argv[0];

    // Set enable to catch more OpenGL errors
    // settings.window.debugContext     = true;

    // Some common resolutions:
    // settings.window.width            =  854; settings.window.height       = 480;
    // settings.window.width            = 1024; settings.window.height       = 768;
    settings.window.width = 1280; settings.window.height = 720;
    //settings.window.width             = 1920; settings.window.height       = 1080;
    // settings.window.width            = OSWindow::primaryDisplayWindowSize().x; settings.window.height = OSWindow::primaryDisplayWindowSize().y;
    settings.window.fullScreen = false;
    settings.window.resizable = !settings.window.fullScreen;
    settings.window.framed = !settings.window.fullScreen;

    // Set to true for a significant performance boost if your app can't render at 60fps, or if
    // you *want* to render faster than the display.
    settings.window.asynchronous = false;

    settings.hdrFramebuffer.depthGuardBandThickness = Vector2int16(64, 64);
    settings.hdrFramebuffer.colorGuardBandThickness = Vector2int16(0, 0);
    settings.dataDir = FileSystem::currentDirectory();
    settings.screenshotDirectory = "../journal/";

    settings.renderer.deferredShading = true;
    settings.renderer.orderIndependentTransparency = false;

    return App(settings).run();
}


App::App(const GApp::Settings& settings) : GApp(settings) {
    edgeBuffer = Array<Array<Point2int32>>();
    thickBuffer = Array<float>();
}


String App::makeCoralString(shared_ptr<Image>& color, shared_ptr<Image>& bump, shared_ptr<CoralGenerator>& coralG, float x, float z, String id) {

    coralG->writeCoral("branch", Color3(0, fmod(.1*x, 1), fmod(0.1*z, 1)), color, bump, 16.0f, 23.0f, 30.0f);
    color->save("../data-files/" + id + "-lambertian.png");
    bump->save("../data-files/" + id + "-bump.png");

    String coral = String("\ncoral_");
    coral += id
        + "= VisibleEntity {"
        + "\n        model = \"starModel\";"
        + "\n        frame = CFrame::fromXYZYPRDegrees("
        + (String)std::to_string(x) + ", 0," + (String)std::to_string(z) + ", 0, 0, 0);"
        + "\n    articulatedModelPose = UniversalMaterial::Specification { "
        + "\n                lambertian = \"" + id + "-lambertian.png\";\n                bump = \"" + id + "-bump.png\"\n            };\n        };\n\n";
    return coral;
}

void App::generateCoralScene(String name) {
    TextOutput text = TextOutput("glassTank.Scene.Any");
    CoralSceneGenerator sceneGenerator;
    String scene("");
    String type("");

    sceneGenerator.writeScene(scene, type, name, 5, 500);
    text.writeSymbols(scene);
    text.commit(true);
}

void App::createScene(String sceneName) {
    TextOutput output = TextOutput("scene\\" + sceneName + ".off");

    //Create the models for the scene
    CoralGenerator3D gen;
    String modelString(gen.write3DCoral(0, .3f, 1.0f));
    output.writeSymbols(modelString);

    // End the File
    output.commit(true);
}

// Called before the application loop begins.  Load data here and
// not in the constructor so that common exceptions will be
// automatically caught.
void App::onInit() {

    GApp::onInit();
    setFrameDuration(1.0f / 120.0f);

    // Call setScene(shared_ptr<Scene>()) or setScene(MyScene::create()) to replace
    // the default scene here.

    showRenderingStats = false;

    makeGUI();
    // For higher-quality screenshots:
    // developerWindow->videoRecordDialog->setScreenShotFormat("PNG");
    // developerWindow->videoRecordDialog->setCaptureGui(false);
    developerWindow->cameraControlWindow->moveTo(Point2(developerWindow->cameraControlWindow->rect().x0(), 0));
    shared_ptr<Image> color;
    shared_ptr<Image> bump;
    color = Image::create(1280, 1280, ImageFormat::RGBA8());
    bump = Image::create(1280, 1280, ImageFormat::RGBA8());
    shared_ptr<Image> color_q1;
    shared_ptr<Image> bump_q1;
    color_q1 = Image::create(640, 640, ImageFormat::RGBA8());
    bump_q1 = Image::create(640, 640, ImageFormat::RGBA8());

    shared_ptr<Image> color_q2;
    shared_ptr<Image> bump_q2;
    color_q2 = Image::create(640, 640, ImageFormat::RGBA8());
    bump_q2 = Image::create(640, 640, ImageFormat::RGBA8());

    shared_ptr<Image> color_q3;
    shared_ptr<Image> bump_q3;
    color_q3 = Image::create(640, 640, ImageFormat::RGBA8());
    bump_q3 = Image::create(640, 640, ImageFormat::RGBA8());

    shared_ptr<Image> color_q4;
    shared_ptr<Image> bump_q4;
    color_q4 = Image::create(640, 640, ImageFormat::RGBA8());
    bump_q4 = Image::create(640, 640, ImageFormat::RGBA8());
    shared_ptr<CoralGenerator> coralG(new CoralGenerator());
    shared_ptr<Rasterizer> painter(new Rasterizer());



    for (int i(0); i < 5; i++) {
        // Generate 4 quadrants
        // crazy, finger, thin, branch, flat
        int rand = Random::threadCommon().integer(0,4);
        
        String type;
        switch (rand){
        case(0): 
            type = "crazy";
            break;
        case(1):
            type = "finger";
            break;
        case(2):
            type = "thin";
            break;
        case(3):
            type = "branch";
            break;
        case(4):
            type = "flat";
        }
        coralG->writeCoral(type, Color3(1, .412, .706), color_q1, bump_q1, 1.0f, 15.0f, 25.0f);
        coralG->writeCoral(type, Color3(1, .412, .706), color_q2, bump_q2, 1.0f, 15.0f, 25.0f);
        coralG->writeCoral(type, Color3(1, .412, .706), color_q3, bump_q3, 1.0f, 15.0f, 25.0f);
        coralG->writeCoral(type, Color3(1, .412, .706), color_q4, bump_q4, 1.0f, 15.0f, 25.0f);

        // Merge them into the big image 
        painter->merge(color_q1, color_q2, color_q3, color_q4, color);
        painter->merge(bump_q1, bump_q2, bump_q3, bump_q4, bump);

        show(color);
        show(bump);

        String save = (String) "Models/Coral/Coral" + (String) std::to_string(i);
        color->save(save + ".png");
        bump->save(save + "_Bump.png");
    }
    generateCoralScene("Hello");
    //createScene("corall");

    loadScene(
        //"G3D Sponza"
        "Test Coral" // Load something simple
        //developerWindow->sceneEditorWindow->selectedSceneName()  // Load the first scene encountered 
    );
}


void App::makeGUI() {
    // Initialize the developer HUD
    createDeveloperHUD();

    debugWindow->setVisible(true);
    developerWindow->videoRecordDialog->setEnabled(true);

    GuiPane* infoPane = debugPane->addPane("Info", GuiTheme::ORNATE_PANE_STYLE);

    // Example of how to add debugging controls
    infoPane->addLabel("You can add GUI controls");
    infoPane->addLabel("in App::onInit().");
    infoPane->addButton("Exit", [this]() { m_endProgram = true; });
    infoPane->pack();

    // More examples of debugging GUI controls:
    // debugPane->addCheckBox("Use explicit checking", &explicitCheck);
    // debugPane->addTextBox("Name", &myName);
    // debugPane->addNumberBox("height", &height, "m", GuiTheme::LINEAR_SLIDER, 1.0f, 2.5f);
    // button = debugPane->addButton("Run Simulator");
    // debugPane->addButton("Generate Heightfield", [this](){ generateHeightfield(); });
    // debugPane->addButton("Generate Heightfield", [this](){ makeHeightfield(imageName, scale, "model/heightfield.off"); });

    debugWindow->pack();
    debugWindow->setRect(Rect2D::xywh(0, 0, (float)window()->width(), debugWindow->rect().height()));

    shared_ptr<G3D::Image> image;
    shared_ptr<G3D::Image> map;
    try {
        int width = 600;
        int height = 500;
        image = Image::create(width, height, ImageFormat::RGB32F());
        map = Image::create(width, height, ImageFormat::RGB32F());
        image->setAll(Color4(1, 1, 1, 0));
        map->setAll(Color4(0, 0, 0, 0));

        shared_ptr<Rasterizer> painter(new Rasterizer());
        /*

                //painter->drawGradiantBackground(Color3(0, 1, 0), Color3(0, 0, 1), height, width, image);
                // painter->drawThickLine(Point2int32(0, 0), Point2int32(599, 499), Color3(1, 0, 0), 5, image);
                painter->drawLine(Point2int32(300, 0), Point2int32(300, 499), Color3(1, 0, 0), image);
                painter->drawLine(Point2int32(0, 250), Point2int32(599, 250), Color3(1, 0, 0), image);
                // Flat diagonals
                painter->drawLine(Point2int32(0, 0), Point2int32(599, 499), Color3(1, 0, 0), image); // Negative
                painter->drawLine(Point2int32(0, 499), Point2int32(599, 0), Color3(1, 0, 0), image); //Positive
                //Steep diagonals
                painter->drawLine(Point2int32(0, 0), Point2int32(100, 499), Color3(1, 0, 0), image); // Negative
                painter->drawLine(Point2int32(100, 499), Point2int32(200, 0), Color3(1, 0, 0), image); // Positive

                painter->drawThickLine(Point2int32(300, 0), Point2int32(300, 499), Color3(1, 0, 0),5, image, map);
                painter->drawThickLine(Point2int32(0, 250), Point2int32(599, 250), Color3(1, 0, 0),5, image, map);
                // Flat diagonals
                painter->drawThickLine(Point2int32(0, 0), Point2int32(599, 499), Color3(1, 0, 0),5, image, map); // Negative
                painter->drawThickLine(Point2int32(0, 499), Point2int32(599, 0), Color3(1, 0, 0),5, image, map); //Positive
                //Steep diagonals
                painter->drawThickLine(Point2int32(0, 0), Point2int32(100, 499), Color3(1, 0, 0),5, image, map); // Negative
                painter->drawThickLine(Point2int32(100, 499), Point2int32(200, 0), Color3(1, 0, 0),5, image, map); // Positive
                */
                //Really Flat diagonals
        painter->drawThickLine(Point2int32(0, 250), Point2int32(599, 300), Color3(1, 0, 0), 5, image, map);
        /*
        //Really Steep diagonals
        painter->drawThickLine(Point2int32(400,0), Point2int32(410, 499), Color3(1,0,0), 5, image, map);

       // drawMyGraph(image);
        //drawClock(image);
        //drawCantorDust(20, 350, 220, 5, image);

        */
        /*
                painter->drawThickLine(Point2int32(300, 0), Point2int32(300, 499), Color3(1, 0, 0), 5, image, map);
                painter->drawThickLine(Point2int32(0, 250), Point2int32(599, 250), Color3(1, 0, 0), 5, image, map);

                painter->drawThickLine(Point2int32(150, 0), Point2int32(450, 499), Color3(1, 0, 0), 5, image, map);
                painter->drawThickLine(Point2int32(450, 0), Point2int32(150, 499), Color3(1, 0, 0), 5, image, map);

                painter->drawThickLine(Point2int32(0, 125), Point2int32(599, 375), Color3(1, 0, 0), 5, image, map);
                painter->drawThickLine(Point2int32(0, 375), Point2int32(599, 125), Color3(1, 0, 0), 5, image, map);


                painter->drawThickLine(Point2int32(0, 0), Point2int32(599, 499), Color3(1, 0, 0), 5, image, map);
                painter->drawThickLine(Point2int32(0, 499), Point2int32(599, 0), Color3(1, 0, 0), 5, image, map);

                painter->drawThickLine(Point2int32(225, 0), Point2int32(375, 499), Color3(1, 0, 0), 5, image, map);
                painter->drawThickLine(Point2int32(375, 0), Point2int32(225, 499), Color3(1, 0, 0), 5, image, map);

        */
        painter->drawThickLine(Point2int32(300, 50), Point2int32(300, 449), Color3(1, 0, 0), 5, image, map);
        painter->drawThickLine(Point2int32(50, 250), Point2int32(549, 250), Color3(1, 0, 0), 5, image, map);

        painter->drawThickLine(Point2int32(150, 50), Point2int32(450, 449), Color3(1, 0, 0), 5, image, map);
        painter->drawThickLine(Point2int32(450, 50), Point2int32(150, 449), Color3(1, 0, 0), 5, image, map);

        painter->drawThickLine(Point2int32(50, 125), Point2int32(549, 375), Color3(1, 0, 0), 5, image, map);
        painter->drawThickLine(Point2int32(50, 375), Point2int32(549, 125), Color3(1, 0, 0), 5, image, map);


        painter->drawThickLine(Point2int32(50, 50), Point2int32(549, 449), Color3(1, 0, 0), 5, image, map);
        painter->drawThickLine(Point2int32(50, 449), Point2int32(549, 50), Color3(1, 0, 0), 5, image, map);

        painter->drawThickLine(Point2int32(225, 50), Point2int32(375, 449), Color3(1, 0, 0), 5, image, map);
        painter->drawThickLine(Point2int32(375, 50), Point2int32(225, 449), Color3(1, 0, 0), 5, image, map);


        show(image);
        show(map);
        image->convert(ImageFormat::RGB8());
        map->convert(ImageFormat::RGB8());
        image->save("../data-files/tubes.png");
        map->save("../data-files/tubesMap.png");
    }
    catch (...) {
        msgBox("Unable to load the image.");
    }
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

    if ((event.type == GEventType::KEY_DOWN) && (event.key.keysym.sym == 'p')) {
        shared_ptr<DefaultRenderer> r = dynamic_pointer_cast<DefaultRenderer>(m_renderer);
        r->setDeferredShading(!r->deferredShading());
        return true;
    }

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
