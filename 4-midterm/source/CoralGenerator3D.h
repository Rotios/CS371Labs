#pragma once
#include <G3D/G3DAll.h>

class CoralGenerator3D {
public:
    String makeTubes(Array<float>& radii, Array<float>& heights, int slices);
    String makeTube(int depth, float& radius, float height, int slices, const CoordinateFrame& coordFrame);
    String write3DCoral(int depth, float startRadius, float startHeight);
    void make3DCoral(String& coral, int depth, float radius, float height, int slices, CoordinateFrame& oldLeft, CoordinateFrame& oldRight);
};