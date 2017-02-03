/**
  \file App.h

  The G3D 10.00 default starter app is configured for OpenGL 4.1 and
  relatively recent GPUs.
 */
#pragma once
#include <G3D/G3DAll.h>

/** \brief Application framework. */
class App : public GApp {
protected:

    float m_cylHeight;
    float m_cylRadius;
    int m_cylSlices;
    float m_heightfieldYScale;
    float m_heightfieldXZScale;
    String m_heightfieldSource;

    /** Called from onInit */
    void makeGUI();
    void createScene(String sceneName);
    String generateStairs();
    String generateModels();
    String generateEntities();
    void makeCylinder(float radius, float height, int slices = 37);
    String generateCylVertices(float radius, float height, int slices);
    String generateCylSides(int offset, char inward, int slices);
    String generateCylLids(int offset, int slices); 
    void makeGlass(int slices);
    String generateDoughnutLid(float firstStart, float Secondstart, int slices);
    void generateHeightField(shared_ptr<G3D::Image> image, float yScale, float xzScale);


public:
    
    App(const GApp::Settings& settings = GApp::Settings());

    virtual void onInit() override;
    virtual void onAI() override;
    virtual void onNetwork() override;
    virtual void onSimulation(RealTime rdt, SimTime sdt, SimTime idt) override;
    virtual void onPose(Array<shared_ptr<Surface> >& posed3D, Array<shared_ptr<Surface2D> >& posed2D) override;

    // You can override onGraphics if you want more control over the rendering loop.
    // virtual void onGraphics(RenderDevice* rd, Array<shared_ptr<Surface> >& surface, Array<shared_ptr<Surface2D> >& surface2D) override;

    virtual void onGraphics3D(RenderDevice* rd, Array<shared_ptr<Surface> >& surface3D) override;
    virtual void onGraphics2D(RenderDevice* rd, Array<shared_ptr<Surface2D> >& surface2D) override;

    virtual bool onEvent(const GEvent& e) override;
    virtual void onUserInput(UserInput* ui) override;
    virtual void onCleanup() override;
};
