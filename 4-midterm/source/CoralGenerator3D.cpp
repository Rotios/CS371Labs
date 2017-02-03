#include "CoralGenerator3D.h"
#include "App.h"

String CoralGenerator3D::makeTubes(Array<float>& radii, Array<float>& heights, int slices) {
    String tube = String("OFF\n");
    tube += format("%d %d 1\n", heights.size()*slices, (heights.size() - 1)*slices);
    for (int i = 0; i < heights.size(); ++i) {
        for (int j = 0; j < slices; ++j) {
            tube += format(STR(%f %f %f\n), radii[i] * (-sin(((2 * pif()*j) / slices))), heights[i], radii[i] * (cos((2 * pif()*j) / slices)));
        }
    }
    for (int i = 0; i < heights.size() - 1; ++i) {
        for (int j = 0; j < slices; ++j) {
            tube += format(STR(4 % d %d %d %d\n), i*slices + slices + j, i*slices + slices + (j + 1) % slices, i*slices + (j + 1) % slices, i*slices + j);
        }
    }
    return tube;
}

String CoralGenerator3D::makeTube(int depth, float& radius, float height, int slices, const CoordinateFrame& coordFrame) {
    String tube = String("");
    tube += format("%d %d 1\n", slices, slices);
        for (int j = 0; j < slices; ++j) {
            Point3 x(radius * (-sin(((2 * pif()*j) / slices))), height, radius * (cos((2 * pif()*j) / slices)));
            coordFrame.pointToWorldSpace(x);
            tube += format(STR(%f %f %f\n), x.x, x.y, x.z);
        }

        for (int j = 0; j < slices; ++j) {
            tube += format(STR(4 % d %d %d %d\n), depth*slices+slices + j, depth*slices+slices + (j + 1) % slices, depth*slices+ (j + 1) % slices, depth*slices + j);
        }

    return tube;
}

void CoralGenerator3D::make3DCoral(String& coral, int depth, float radius, float height, int slices, CoordinateFrame& oldLeft, CoordinateFrame& oldRight){
    Matrix3 rot = oldLeft.rotation * Matrix3::fromEulerAnglesXYZ(10.0f, 10.0f, 10.0f);
    CoordinateFrame newRight(rot, oldRight.translation + Vector3(0,height,0));
    if (depth == 2) { return; }
    
    make3DCoral(coral, ++depth, depth/100, 4*height/7, 8, oldLeft, oldRight);
}

String CoralGenerator3D::write3DCoral(int depth, float startRadius, float startHeight){
    String coral("OFF\n");
    coral += format("%d %d 1\n", 8, 8);
    CoordinateFrame original(Matrix3(1,1,1,1,1,1,1,1,1), Point3(0,0,0));
    coral += makeTube(depth, startRadius, startHeight, 8, original);
    make3DCoral(coral, depth, startRadius, startHeight, 8, original, original);
    return coral;
}
