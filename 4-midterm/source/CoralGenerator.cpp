#include "CoralGenerator.h"
#include "App.h"
#include "Rasterizer.h"
#include "Edges.h"

CoralGenerator::CoralGenerator() {};
CoralGenerator::~CoralGenerator() {};


void CoralGenerator::writeCoral(String coralType, Color3 coralColor, shared_ptr<Image> &color, shared_ptr<Image> &bump, float initialThickness, float minAngle, float maxAngle) {
    shared_ptr<Rasterizer> painter(new Rasterizer());

    Array<shared_ptr<Edges>> edgeBuffer;

    Array<float> thickBuffer = Array<float>();
    
    int maxDepth;

    int width(color->width());
    int height(color->height());
    int w4(width/4); 
    if (coralType == "thin") {
        maxDepth = 4;
        generateThinCoral(maxDepth, Point2int32(width/2, height - (2 * initialThickness)), -90, 100, minAngle, maxAngle, initialThickness, edgeBuffer, thickBuffer);

    }
    else if (coralType == "finger") {
        maxDepth = 4;
        generateFingerCoral(maxDepth, Point2int32(width/2, height - (2 * initialThickness)), -90, 100, minAngle, maxAngle, initialThickness, edgeBuffer, thickBuffer);
    }
    else if (coralType == "flat") {
        maxDepth = 7;
        generateFlatCoral(maxDepth, Point2int32(width/2, height - (2 * initialThickness)), -90, 100, minAngle, maxAngle, initialThickness, edgeBuffer, thickBuffer);
    }
    else if (coralType == "branch") {
        maxDepth = 6;
        generateBranchCoral(maxDepth, Point2int32(width/2, height - (2 * initialThickness)), -90, 20, minAngle, maxAngle, initialThickness, edgeBuffer, thickBuffer);
    }
    else if (coralType == "tall") {
        maxDepth = 6;
        generateTallCoral(maxDepth, Point2int32(width/2, height - (2 * initialThickness)), -90, 100, minAngle, maxAngle, initialThickness, edgeBuffer, thickBuffer);
    }
    else {
        maxDepth = 4;
        generateCrazyCoral(maxDepth, Point2int32(width/2, height - (2 * initialThickness)), -90, 100, minAngle, maxAngle, initialThickness, edgeBuffer, thickBuffer);
    }

    try {
        painter->drawGradiantBackground(Color4(coralColor, 0.0), Color4(Color3::black(), 0.0), height, width, color);
        bump->setAll(Color3::black());

        for (int i(0); i < edgeBuffer.size(); ++i) {

            Point2int32 s(edgeBuffer[i]->p0);
            Point2int32 f(edgeBuffer[i]->p1);
            int curDepth(maxDepth - edgeBuffer[i]->depth);
            painter->drawThickLine(s, f, coralColor, thickBuffer[i], color, bump, curDepth == maxDepth);
        }

    }
    catch (...) {
        msgBox("Unable to load the image.");
    }
}


void CoralGenerator::generateThinCoral(int depth, Point2int32& location, float cumulativeAngle, float drawLength, float minAngle, float maxAngle, float thick, Array<shared_ptr<Edges>>& edgeBuffer, Array<float>& thickBuffer) {

    Table<String, Array<String>> rules;

    Array<String> SBuffer("-[FXFXF]+[FXFF]");
    rules.set("S", SBuffer);

    Array<String> XBuffer("-[FFXF]+[FXF][FXFF]F");
    rules.set("X", XBuffer);

    rules.set("F", Array<String>("FF"));

    applyRules(depth, location, cumulativeAngle, drawLength, minAngle, maxAngle, thick, 0.65f, 0.8f, "S", rules, edgeBuffer, thickBuffer);
}

void CoralGenerator::generateFlatCoral(int depth, Point2int32& location, float cumulativeAngle, float drawLength, float minAngle, float maxAngle, float thick, Array<shared_ptr<Edges>>& edgeBuffer, Array<float>& thickBuffer) {

    Table<String, Array<String>> rules;

    Array<String> SBuffer("Y--[FY]+[YFY]");
    rules.set("S", SBuffer);

    Array<String> YBuffer("++[XFX]", "--[XFX]");
    rules.set("Y", YBuffer);

    Array<String> XBuffer("+[FXF]");
    rules.set("X", XBuffer);

    rules.set("F", Array<String>(""));

    applyRules(depth, location, cumulativeAngle, drawLength, minAngle, maxAngle, thick, 0.8f, 0.75f, "S", rules, edgeBuffer, thickBuffer);
}

void CoralGenerator::generateBranchCoral(int depth, Point2int32& location, float cumulativeAngle, float drawLength, float minAngle, float maxAngle, float thick, Array<shared_ptr<Edges>>& edgeBuffer, Array<float>& thickBuffer) {

    Table<String, Array<String>> rules;

    Array<String> XBuffer("+[FFFFFX]-[FFFFFX]");
    rules.set("X", XBuffer);

    Array<String> YBuffer("+[FFFXFFX]");
    rules.set("Y", YBuffer);

    Array<String> ZBuffer("-[FFFXFFX]");
    rules.set("Z", ZBuffer);

    rules.set("F", Array<String>(""));

    applyRules(depth, location, cumulativeAngle, drawLength, minAngle, maxAngle, thick, 0.8f, 0.8f, "FX+++[YF]---[ZF]", rules, edgeBuffer, thickBuffer);
}

void CoralGenerator::generateTallCoral(int depth, Point2int32& location, float cumulativeAngle, float drawLength, float minAngle, float maxAngle, float thick, Array<shared_ptr<Edges>>& edgeBuffer, Array<float>& thickBuffer) {

    Table<String, Array<String>> rules;

    Array<String> XBuffer("+[FFFFFW]-[FFFFFW]");
    rules.set("X", XBuffer);

    Array<String> YBuffer("+[FFXFFFX]");
    rules.set("Y", YBuffer);

    Array<String> ZBuffer("-[FFFXFFX]");
    rules.set("Z", ZBuffer);

    Array<String> WBuffer("-[FF]", "+[FF]", "F");
    rules.set("W", WBuffer);

    rules.set("F", Array<String>(""));

    applyRules(depth, location, cumulativeAngle, drawLength, minAngle, maxAngle, thick, 0.5f, 0.5f, "X+[FFFFFW]-[FFFFFW]++[+[FFXFFFX]]--[-[FFFXFFX]]+++[+[FFXFFFX]]---[-[FFFXFFX]]++[FFFFFW]--[FFFFFW]+++[FFFFFW]---[FFFFFW]", rules, edgeBuffer, thickBuffer);
}

void CoralGenerator::generateFingerCoral(int depth, Point2int32& location, float cumulativeAngle, float drawLength, float minAngle, float maxAngle, float thick, Array<shared_ptr<Edges>>& edgeBuffer, Array<float>& thickBuffer) {
    Table<String, Array<String>> rules;

    Array<String> SBuffer("-[FX]--[FX]+[FX]++[FX]+++[FX]---[FX]");
    rules.set("S", SBuffer);

    Array<String> XBuffer;
    for (int i = 0; i < 8; ++i) {
        XBuffer.append("-[FX]--[FX]+[FX]++[FX]");
    }
    XBuffer.append("-[FX]+[FX]");
    rules.set("X", XBuffer);

    rules.set("F", Array<String>(""));

    applyRules(depth, location, cumulativeAngle, drawLength, minAngle, maxAngle, thick, 0.75f, 0.85f, "S", rules, edgeBuffer, thickBuffer);
}

void CoralGenerator::generateCrazyCoral(int depth, Point2int32& location, float cumulativeAngle, float drawLength, float minAngle, float maxAngle, float thick, Array<shared_ptr<Edges>>& edgeBuffer, Array<float>& thickBuffer) {
    Table<String, Array<String>> rules;

    Array<String> XBuffer;

    for (int i = 0; i < 6; ++i) {
        XBuffer.append("-[FX]-[-[FX]]+[FX]+[+[FX]]+[+[+[FX]]]-[-[-[FX]]]");
    }
    XBuffer.append("-[FX]-[-[FX]]+[FX]+[+[FX]]");
    XBuffer.append("-[FX]-[-[FX]]+[FX]+[+[FX]]");
    XBuffer.append("-[FX]+[FX]");
    XBuffer.append("-[FX]+[FX]");
    rules.set("S", XBuffer);
    rules.set("X", XBuffer);

    rules.set("F", Array<String>(""));


    applyRules(depth, location, cumulativeAngle, drawLength, minAngle, maxAngle, thick, 1.0f, 0.8f, "S", rules, edgeBuffer, thickBuffer);
}

void CoralGenerator::applyRules(int depth, const Point2int32& location, float cumulativeAngle, float drawLength, float minAngle, float maxAngle, float thick, const float thickMult, const float drawMult, const String& symbolBuffer, const Table<String, Array<String>>& rules, /*Array<Array<Point2int32>>&*/Array<shared_ptr<Edges>>& edgeBuffer, Array<float>& thickBuffer) {

    if (depth == 0) { return; }

    int isBracket = 0;
    Array<float> angles;
    Array<Point2int32> positions;

    angles.resize(20);
    positions.resize(20);

    angles.setAll(cumulativeAngle);
    positions.setAll(location);

    for (int i(0); i < symbolBuffer.length(); ++i) {

        if (symbolBuffer.substr(i, 1) == "-") {
            angles[isBracket + 1] = angles[isBracket + 1] - G3D::Random::threadCommon().uniform(minAngle, maxAngle);
        }
        else if (symbolBuffer.substr(i, 1) == "+") {
            angles[isBracket + 1] = angles[isBracket + 1] + G3D::Random::threadCommon().uniform(minAngle, maxAngle);
        }
        else if (symbolBuffer.substr(i, 1) == "[") {
            ++isBracket;
            positions[isBracket] = positions[isBracket - 1];
        }
        else if (symbolBuffer.substr(i, 1) == "]") {
            angles[isBracket] = cumulativeAngle;
            --isBracket;
        }
        else if (symbolBuffer.substr(i, 1) == "F") {
            float randLen = drawLength * G3D::Random::threadCommon().uniform(0.7f, 1.0);
            float randAng = angles[isBracket] * G3D::Random::threadCommon().uniform(0.75f, 1.25f);

            float radians = (randAng / 180.0f) * pif();
            int x = lround(cos(radians) * randLen) + positions[isBracket].x;
            int y = lround(sin(radians) * randLen) + positions[isBracket].y;

            Point2int32 point(x, y);
            shared_ptr<Edges> edge(new Edges); 
            edge->p0 = positions[isBracket];
            edge->p1 = point; 
            edge->depth = depth;
            edgeBuffer.append(edge);
            thickBuffer.append(thick);

            positions[isBracket] = point;

            Array<String> apply = rules["F"];
            int count = G3D::Random::threadCommon().uniform(0, apply.size() - 1);

            applyRules(depth - 1, positions[isBracket], angles[isBracket], drawLength * drawMult, minAngle, maxAngle, thick, thickMult, drawMult, apply[count], rules, edgeBuffer, thickBuffer);
        }
        else if (symbolBuffer.substr(i, 1) != "\n") {

            String str = symbolBuffer.substr(i, 1);
            Array<String> apply = rules[str];
            int count = G3D::Random::threadCommon().uniform(0, apply.size() - 1);

            applyRules(depth - 1, positions[isBracket], angles[isBracket], drawLength * drawMult, minAngle, maxAngle, thick * thickMult, thickMult, drawMult, apply[count], rules, edgeBuffer, thickBuffer);
        }
    }
}