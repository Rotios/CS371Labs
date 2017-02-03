/** \file Drawing.cpp */
#include "Drawing.h"
#include <stack>

Drawing::Drawing() {};
Drawing::~Drawing() {};

bool Drawing::inBounds(int x, int y, const shared_ptr<Image>& image) const {
    int height(image->height());
    int width(image->width());
    return x >= 0 && y >= 0 && x < width && y < height;
};


void Drawing::drawLine(const Point2int32& point1, const Point2int32& point2, const Color4& c, shared_ptr<Image>& image) const {
    drawLine(point1, point2, 0, c, image);
};

void Drawing::drawLine(const Point2int32& point1, const Point2int32& point2, int offset, const Color4& c, shared_ptr<Image>& image) const {
    int x0(min<int>(point1.x, point2.x));
    int x1(max<int>(point1.x, point2.x));
    int y0(min<int>(point1.y, point2.y));
    int y1(max<int>(point1.y, point2.y));
    if (x0 == x1) {
        drawVLine(x0 + offset, y0, y1, c, image);
    }
    else if (y0 == y1) {
        drawHLine(x0, x1, y0 + offset, c, image);
    }
    else {
        float m = (point2.y - point1.y) / (point2.x - point1.x);
        if (fabs(m) <= 1) {
            drawFlatLine(point1, point2, offset, c, image);
        }
        else {
            drawSteepLine(point1, point2, offset, c, image);
        }
    }
}

void Drawing::drawVLine(int x, int y0, int y1, const Color4&c, shared_ptr<Image>& image) const {
    for (int y = y0; y <= y1; ++y) {
        if (inBounds(x, y, image)) {
            image->set(x, y, c);
        }
    }

}

void Drawing::drawHLine(int x0, int x1, int y, const Color4& c, shared_ptr<Image>& image) const {
    for (int x = x0; x <= x1; ++x) {
        if (inBounds(x, y, image)) {
            image->set(x, y, c);
        }
    }
}

/*void Drawing::drawHLine(Point2int32 point1, Point2int32 point2, Color4 c, shared_ptr<Image>& image) {
    float x0;
    float x1;
    x0 = min<int>(point1.x, point2.x);
    x1 = max<int>(point1.x, point2.x);

    for (int x = x0; x <= x1; ++x) {
        image->set(x, point1.y, c);
    }
}*/

void Drawing::drawFlatLine(int x0, int x1, int y, float m, const Color4& c, shared_ptr<Image>& image) const {
    for (int x = x0; x <= x1; ++x, y += m) {
        if (inBounds(x, int(y + .5f), image)) {
            image->set(x, int(y + .5f), c);
        }
    }
};


void Drawing::drawFlatLine(const Point2int32& point1, const Point2int32& point2, int offset, const Color4& c, shared_ptr<Image>& image) const {
    float y(offset); // Offset y by thick coordinate
    float x0(min<float>(point1.x, point2.x));
    float x1(max<float>(point1.x, point2.x));

    if (point2.x >= point1.x) { // Center y at appropriate 
        y+= point1.y;
    }
    else {
        y += point2.y;
    }

    float m = 1.0f * (point2.y - point1.y) / (point2.x - point1.x);


    for (int x = (int)x0; x <= x1; ++x, y += m) {
        if (inBounds(x, y, image)) {
            image->set(x, y, c);
        }
    }
}


void Drawing::drawSteepLine(int x, int y0, int y1, float m, const Color4& c, shared_ptr<Image>& image) const {
    for (int y = y0; y <= y1; ++y, x += 1 / m) {
        if (inBounds(int(x + .5f), y, image)) {
            image->set(int(x + .5f), y, c);
        }
    }
};

void Drawing::drawSteepLine(const Point2int32& point1, const Point2int32& point2, int offset, const Color4& c, shared_ptr<Image>& image) const {
    float x(offset); // Offset x by thick coordinate
    float y0(min<float>(point1.y, point2.y));
    float y1(max<float>(point1.y, point2.y));

    if (point2.y >= point1.y) { // Center x at appropriate 
        x+= point1.x;
    }
    else {
        x += point2.x;
    }

    // Inverse slope
    float m_i = 1.0f * (point2.x - point1.x) / (point2.y - point1.y);

    for (int y = (int)y0; y <= y1; ++y, x += m_i) {
        if (inBounds(x, y, image)) {
            image->set(x, y, c);
        }
    }
}

void Drawing::drawThickLine(const Point2int32& point1, const Point2int32& point2, const Color4& c, int halfGirth, shared_ptr<Image>& image, shared_ptr<Image>& map) const {
    int x0(min<int>(point1.x, point2.x));
    int x1(max<int>(point1.x, point2.x));
    int y0(min<int>(point1.y, point2.y));
    int y1(max<int>(point1.y, point2.y));

    Color4 shade(0, 0, 0);

    if (x0 == x1) {
        Color4 increment(1.0f/halfGirth, 1.0f/halfGirth, 1.0f/halfGirth, 1.0);
        for (int i(-halfGirth); i <= halfGirth; ++i) {
            drawVLine(x0 + i, y0, y1, c, image);
            drawVLine(x0 + i, y0, y1, shade, map);
            shade -= increment*sign(1.0*i);
        }
    }
    else if (y0 == y1) {
        Color4 increment(1.0f/halfGirth, 1.0f/halfGirth, 1.0f/halfGirth, 1.0);
        for (int i(-halfGirth); i <= halfGirth; ++i) {
            drawHLine(x0, x1, y0 + i, c, image);
            drawHLine(x0, x1, y0 + i, shade, map);
            shade -= increment*sign(1.0*i);
        }
    }
    else {
        float m = float(point2.y - point1.y) / float(point2.x - point1.x);
        Vector2 d(point2-point1);
        if (fabs(m) <= 1) { 

            int t_y = abs(halfGirth/(abs(d.direction().x)));
            Color4 increment(1.0f/t_y, 1.0f/t_y, 1.0f/t_y, 1.0);
            for(int i(-t_y); i < t_y; ++i) { 
                drawFlatLine(point1, point2, i, c, image);
                drawFlatLine(point1, point2, i, shade, map);
                shade -= increment*sign(1.0*i);
            }
        }
        else {
            int t_x = abs(halfGirth /abs(d.direction().y));
            Color4 increment(1.0f/t_x, 1.0f/t_x, 1.0f/t_x, 1.0);
            for(int i(-t_x); i < t_x; ++i) { 
                drawSteepLine(point1, point2, i, c, image);
                drawSteepLine(point1, point2, i, shade, map);
                shade -= increment*sign(1.0*i);
            }
        }
    }
 }



void Drawing::drawGradiantBackground(const Color4& c1, const Color4& c2, int height, int width, shared_ptr<Image>& image) const {
    Color4 current = c1;
    for (int y = 0; y < height; ++y) {
        current = current + (c2 - c1) / (height - 1);
        drawLine(Point2int32(0, y), Point2int32(width - 1, y), current, image);
    }
}

void Drawing::drawAxes(int rad, int rng, int xOff, int yOff, shared_ptr<Image>& image, shared_ptr<Image>& map) const {
    drawThickLine(Point2int32(0 + xOff, 10 * rng + yOff), Point2int32(20 * rad + xOff, 10 * rng + yOff), Color4(1, 0, 0), 5, image, map);
    drawThickLine(Point2int32(10 * rad + xOff, 0 + yOff), Point2int32(10 * rad + xOff, 20 * rng + yOff), Color4(1, 0, 0), 5, image, map);
    for (int x = 0; x <= 2 * rad; ++x) {
        drawThickLine(Point2int32(xOff + x * 10, yOff + rng * 10 - 2), Point2int32(xOff + x * 10, yOff + rng * 10 + 2), Color4(1, 0, 0), 0, image, map);
    }
    for (int y = 0; y <= 2 * rng; ++y) {
        drawThickLine(Point2int32(xOff + rad * 10 - 2, yOff + y * 10), Point2int32(xOff + rad * 10 + 2, yOff + y * 10), Color4(1, 0, 0), 0, image, map);
    }
}

void Drawing::drawVarGraph(bool isClock, int width, int height, int xOff, int yOff, int sign, shared_ptr<Image>& image) const {
    float realX = -1 * width;
    float realY = 0;
    Point2int32 p1 = Point2int32(-1, -1);
    Point2int32 p2 = Point2int32(-1, -1);
    for (int x = xOff; x <= 20 * width + xOff; ++x) {
        if (isClock) {
            realY = sign * sqrt(36 - realX*realX);
        }
        else {
            realY = .13 * realX * realX * realX;
        }
        float pixY = 10 * height;
        if (realY > 0) {
            pixY = 10 * height - 10 * realY;
        }
        if (realY < 0) {
            pixY = 10 * height - 10 * realY;
        }
        pixY += yOff;
        p2 = Point2int32(1.0f * x, pixY);
        if (p1.x != -1) {
            drawLine(p1, p2, Color4(0, 0, 0), image);
        }
        p1 = p2;
        realX += .1;
    }
}

void Drawing::drawMyGraph(shared_ptr<Image>& image) const {
    //drawAxes(4, 10, 5, 5, image);
    drawVarGraph(false, 4, 10, 5, 5, 1, image);
}

void Drawing::drawClock(shared_ptr<Image>& image) const {
    Point2int32 center = Point2int32(210, 65);
    drawVarGraph(true, 6, 6, 150, 5, 1, image);
    drawVarGraph(true, 6, 6, 150, 5, -1, image);
    float rotation = 0;
    int time = 0;
    Point2int32 outer = Point2int32((float)(1.0f*center.x + (50.0f * cos(rotation))), (float)(1.0f*center.y + (50.0f * sin(rotation))));
    drawLine(center, outer, Color4(1, 0, 0), image);
    if (time % 5 == 0) {
        rotation += (3.14159 / 12);
    }
}

void Drawing::drawCantorDust(float xSt, float xEn, float y, int level, shared_ptr<Image>& image) const {
    drawLine(Point2int32(xSt, y), Point2int32(xEn, y), Color4(0, 0, 1), image);
    if (level > 1) {
        drawCantorDust(xSt, (xEn - xSt) / 3 + xSt, y + 10, level - 1, image);
        drawCantorDust(2.0 * (xEn - xSt) / 3 + xSt, xEn, y + 10, level - 1, image);
    }
}