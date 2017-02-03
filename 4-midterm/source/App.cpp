/** \file App.cpp */
#include "App.h"
#include "Rasterizer.h"
#include "CoralGenerator.h"
#include "CoralGenerator3D.h"
#include "CoralSceneGenerator.h"
#include "PathTracer.h"

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

void App::generateCoralScene() {
    TextOutput text = TextOutput("Scene/glassTank.Scene.Any");
    CoralSceneGenerator sceneGenerator;
    String scene("");
    String name("GlassTank");
    sceneGenerator.writeTankScene(scene, name, 7, 250);
    text.writeSymbols(scene);
    text.commit(true);

    TextOutput text2 = TextOutput("Scene/sea.Scene.Any");
    scene = "";
    name = "Sea Scene";
    sceneGenerator.writeSeaScene(scene, name, 7, 250);
    text2.writeSymbols(scene);
    text2.commit(true);
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

    // Generate 4 quadrants
    coralG->writeCoral("crazy", Color3(1, .412, .706), color_q1, bump_q1, 4.0f, 10.0f, 30.0f);
    coralG->writeCoral("crazy", Color3(1, .412, .706), color_q2, bump_q2, 4.0f, 10.0f, 30.0f);
    coralG->writeCoral("crazy", Color3(1, .412, .706), color_q3, bump_q3, 4.0f, 10.0f, 30.0f);
    coralG->writeCoral("crazy", Color3(1, .412, .706), color_q4, bump_q4, 4.0f, 10.0f, 30.0f);

    // Merge them into the big image 
    painter->mergeQuadrants(color_q1, color_q2, color_q3, color_q4, color);
    painter->mergeQuadrants(bump_q1, bump_q2, bump_q3, bump_q4, bump);

    color->save("../data-files/models/Coral/Coral1.png");
    bump->save("../data-files/models/Coral/Coral1_Bump.png");


    coralG->writeCoral("finger", Color3(.1, .5, 1.0), color_q1, bump_q1, 16.0f, 20.0f, 30.0f);
    coralG->writeCoral("finger", Color3(.1, .5, 1.0), color_q2, bump_q2, 16.0f, 20.0f, 30.0f);
    coralG->writeCoral("finger", Color3(.1, .5, 1.0), color_q3, bump_q3, 16.0f, 20.0f, 30.0f);
    coralG->writeCoral("finger", Color3(.1, .5, 1.0), color_q4, bump_q4, 16.0f, 20.0f, 30.0f);

    painter->mergeQuadrants(color_q1, color_q2, color_q3, color_q4, color);
    painter->mergeQuadrants(bump_q1, bump_q2, bump_q3, bump_q4, bump);

    color->save("../data-files/models/Coral/Coral2.png");
    bump->save("../data-files/models/Coral/Coral2_Bump.png");

    coralG->writeCoral("thin", Color3(.1, .9, .3), color_q1, bump_q1, 8.0f, 28.0f, 30.0f);
    coralG->writeCoral("thin", Color3(.1, .9, .3), color_q2, bump_q2, 8.0f, 28.0f, 30.0f);
    coralG->writeCoral("thin", Color3(.1, .9, .3), color_q3, bump_q3, 8.0f, 28.0f, 30.0f);
    coralG->writeCoral("thin", Color3(.1, .9, .3), color_q4, bump_q4, 8.0f, 28.0f, 30.0f);

    painter->mergeQuadrants(color_q1, color_q2, color_q3, color_q4, color);
    painter->mergeQuadrants(bump_q1, bump_q2, bump_q3, bump_q4, bump);

    color->save("../data-files/models/Coral/Coral3.png");
    bump->save("../data-files/models/Coral/Coral3_Bump.png");

    coralG->writeCoral("branch", Color3(1, 0, .0), color_q1, bump_q1, 6.0f, 0.0f, 20.0f);
    coralG->writeCoral("branch", Color3(1, 0, .0), color_q2, bump_q2, 6.0f, 0.0f, 20.0f);
    coralG->writeCoral("branch", Color3(1, 0, .0), color_q3, bump_q3, 6.0f, 0.0f, 20.0f);
    coralG->writeCoral("branch", Color3(1, 0, .0), color_q4, bump_q4, 6.0f, 0.0f, 20.0f);

    painter->mergeQuadrants(color_q1, color_q2, color_q3, color_q4, color);
    painter->mergeQuadrants(bump_q1, bump_q2, bump_q3, bump_q4, bump);

    color->save("../data-files/models/Coral/Coral4.png");
    bump->save("../data-files/models/Coral/Coral4_Bump.png");

    // Generate 4 quadrants
    coralG->writeCoral("flat", Color3(0, 1, 1), color_q1, bump_q1, 8.0f, 15.0f, 18.0f);
    coralG->writeCoral("flat", Color3(0, 1, 1), color_q2, bump_q2, 8.0f, 15.0f, 18.0f);
    coralG->writeCoral("flat", Color3(0, 1, 1), color_q3, bump_q3, 8.0f, 15.0f, 18.0f);
    coralG->writeCoral("flat", Color3(0, 1, 1), color_q4, bump_q4, 8.0f, 15.0f, 18.0f);

    // Merge them into the big image 
    painter->mergeQuadrants(color_q1, color_q2, color_q3, color_q4, color);
    painter->mergeQuadrants(bump_q1, bump_q2, bump_q3, bump_q4, bump);

    color->save("../data-files/models/Coral/Coral5.png");
    bump->save("../data-files/models/Coral/Coral5_Bump.png");

    // Generate 4 quadrants
    coralG->writeCoral("branch", Color3(1, 1, 0), color_q1, bump_q1, 8.0f, 10.0f, 10.0f);
    coralG->writeCoral("branch", Color3(1, 1, 0), color_q2, bump_q2, 8.0f, 10.0f, 10.0f);
    coralG->writeCoral("branch", Color3(1, 1, 0), color_q3, bump_q3, 8.0f, 10.0f, 10.0f);
    coralG->writeCoral("branch", Color3(1, 1, 0), color_q4, bump_q4, 8.0f, 10.0f, 10.0f);

    // Merge them into the big image 
    painter->mergeQuadrants(color_q1, color_q2, color_q3, color_q4, color);
    painter->mergeQuadrants(bump_q1, bump_q2, bump_q3, bump_q4, bump);

    color->save("../data-files/models/Coral/Coral6.png");
    bump->save("../data-files/models/Coral/Coral6_Bump.png");

    coralG->writeCoral("tall", Color3(1, .412, .706), color_q1, bump_q1, 4.0f, 10.0f, 30.0f);
    coralG->writeCoral("tall", Color3(1, .412, .706), color_q2, bump_q2, 4.0f, 10.0f, 30.0f);
    coralG->writeCoral("tall", Color3(1, .412, .706), color_q3, bump_q3, 4.0f, 10.0f, 30.0f);
    coralG->writeCoral("tall", Color3(1, .412, .706), color_q4, bump_q4, 4.0f, 10.0f, 25.0f);

    painter->mergeQuadrants(color_q1, color_q2, color_q3, color_q4, color);
    painter->mergeQuadrants(bump_q1, bump_q2, bump_q3, bump_q4, bump);

    color->save("../data-files/models/Coral/Coral7.png");
    bump->save("../data-files/models/Coral/Coral7_Bump.png");

    generateCoralScene();
    createScene("corall");

    loadScene(
        "Test Coral" // Load something simple
    );
}


void App::makeGUI() {
    // Initialize the developer HUD
    createDeveloperHUD();

    debugWindow->setVisible(true);
    developerWindow->videoRecordDialog->setEnabled(true);
    debugPane->setNewChildSize(500, -1, 300);

    GuiPane* rayTracePane = debugPane->addPane("Ray Trace");
 
    rayTracePane->setNewChildSize(240);
    GuiText temp("1x1");
    Array<GuiText> resolutionMenu(temp);
    temp = "320x200";
    resolutionMenu.append(temp);
    temp = "640x400";
    resolutionMenu.append(temp);
    temp = "1280x720";
    resolutionMenu.append(temp);
    GuiDropDownList* list(rayTracePane->addDropDownList("Resolution", resolutionMenu));

    rayTracePane->addNumberBox("Scatter Events", &m_maxScatter, "", GuiTheme::LOG_SLIDER, 1, 2048) -> setUnitsSize(200);
    rayTracePane->addNumberBox("Paths", &m_pathsPPx, " /pixels", GuiTheme::LOG_SLIDER, 1, 2048) -> setUnitsSize(200);
 

    

    rayTracePane->addButton("Render", [this, list, rayTracePane](){
        drawMessage("Path Tracer is loading");
        shared_ptr<G3D::Image> image;
        try{
            const int width = int(window()->width()  * 0.5f);
            const int height = int(window()->height() * 0.5f);

            if(!list->selectedIndex()) image = Image::create(1, 1, ImageFormat::RGB32F());
            else if (list->selectedIndex() == 1) image = Image::create(320,200, ImageFormat::RGB32F());
            else if (list->selectedIndex() == 2) image = Image::create(640,420, ImageFormat::RGB32F());
            else image = Image::create(1280,720, ImageFormat::RGB32F());
            PathTracer tracer = PathTracer(scene(), m_maxScatter, m_pathsPPx);
            Stopwatch watch("watch");
            watch.tick();
            tracer.pathTrace(scene(), activeCamera(), image);
            watch.tock();
            const shared_ptr<Texture>& src = Texture::fromImage("Source", image);
            if (m_result) {
                m_result->resize(width, height);
            }

            m_film->exposeAndRender(renderDevice, activeCamera()->filmSettings(), src, settings().hdrFramebuffer.colorGuardBandThickness.x, settings().hdrFramebuffer.depthGuardBandThickness.x, m_result);
            debugPrintf(String(std::to_string(watch.smoothElapsedTime()) + " seconds").c_str());
            show(m_result, String(std::to_string(watch.smoothElapsedTime()) + " seconds + Numcores = " + std::to_string(G3D::System::numCores())));
            show(image, "image");
            ArticulatedModel::clearCache();
            
        }catch(...){
            msgBox("Unable to load the image.");
        }
    });


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
