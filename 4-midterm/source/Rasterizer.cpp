/** \file Rasterizer.cpp */
 /*  John Freeman
     Jose Rivas-Garcia
     Julia Goldman
     Matheus de Carvalho Souza
 */

#include "Rasterizer.h"

Rasterizer::Rasterizer() {};
Rasterizer::~Rasterizer() {};

void Rasterizer::fitToBounds(Point2& c0, Point2& c1, int r, int width, int height) const {
    // Adjust c0 so it's at least r from the image bounds
    c0.x = max<float>(c0.x, 0.0f + r);
    c0.x = min<float>(c0.x, width - r);
    c0.y = max<float>(c0.y, 0.0f + r);
    c0.y = min<float>(c0.y, height - r);

    // Adjust c1 so it's at least r from the image bounds
    c1.x = max<float>(c1.x, 0.0f + r);
    c1.x = min<float>(c1.x, width - r);
    c1.y = max<float>(c1.y, 0.0f + r);
    c1.y = min<float>(c1.y, height - r);
};


void Rasterizer::mergeQuadrants(const shared_ptr<Image>& q1, const shared_ptr<Image>& q2, const shared_ptr<Image>& q3, const shared_ptr<Image>& q4, shared_ptr<Image>& image) const {
    int width(image->width());
    int height(image->height());

    Thread::runConcurrently(Point2int32(0, 0), Point2int32(width, height), [&](Point2int32 pixel) {
        Color4 c(0, 0, 0, 0);
        int x(pixel.x % (width / 2));
        int y(pixel.y % (height / 2));

        switch (findQuadrant(pixel.x, pixel.y, width, height)) {
        case 1: q1->get(Point2int32(x, y), c);
            break;
        case 2: q2->get(Point2int32(x, y), c);
            break;
        case 3: q3->get(Point2int32(x, y), c);
            break;
        case 4: q4->get(Point2int32(x, y), c);
            break;
        }
        image->set(pixel.x, pixel.y, c);
    });
};

int Rasterizer::findQuadrant(int x, int y, int width, int height) const {
    int a(x < (width / 2) ? 1 : 2);
    int b(a + 2);
    return y < (height / 2) ? a : b;
};

void Rasterizer::drawThickLine(const Point2int32& point1, const Point2int32& point2, const Color4& c, int thickness, shared_ptr<Image>& image, shared_ptr<Image>& map) const {
    drawThickLine(point1, point2, c, thickness, image, map, true);
}

void Rasterizer::drawThickLine(const Point2int32& point1, const Point2int32& point2, const Color4& c, int r, shared_ptr<Image>& image, shared_ptr<Image>& map, bool branchEnd) const {
    Point2 c0(point1.x, point1.y);
    Point2 c1(point2.x, point2.y);

    fitToBounds(c0, c1, r, image->width(), image->height());

    LineSegment2D centerLine(LineSegment2D::fromTwoPoints(c0, c1));

    // Bounding box coordinates
    int x0(min<float>(c0.x, c1.x) - r);
    int x1(max<float>(c0.x, c1.x) + r);
    int y0(min<float>(c0.y, c1.y) - r);
    int y1(max<float>(c0.y, c1.y) + r);

    const Color3 bump(Color3::white());

    // gradient divisor
    const float div(1.5f*float(image->height()));

    Thread::runConcurrently(Point2int32(x0, y0), Point2int32(x1, y1), [&](Point2int32 pixel) {
        int x(pixel.x);
        int y(pixel.y);

        Point2 P(x, y);
        float dist(fabs(centerLine.distance(P)));

        if (dist < float(r) + 0.1f) {
            float alpha(float(y) / div);
            Color3 curCol(c.rgb().lerp(Color3::black(), alpha));
            image->set(x, y, Color4(curCol, 1.0f));


            Color3 curBump(bump.lerp(Color3::black(), fabs(dist / float(r))));

            // If we're at the end of a line segment, but not the end or start of coral
            if (((x < x0 + r || x > x1 - r || y < y0 + r || y > y1 - r)) && !branchEnd) {
                // Smooth out the shading
                Color3 prevBump;
                map->get(pixel, prevBump);
                curBump = curBump.max(prevBump);
            }
            map->set(x, y, Color4(curBump, 1.0f));
        }
    });

};

void Rasterizer::drawGradiantBackground(const Color4& c0, const Color4& c1, int height, int width, shared_ptr<Image>& image) const {
    Thread::runConcurrently(Point2int32(0, 0), Point2int32(width, height), [&](Point2int32 pixel) {
        float alpha(float(pixel.y) / (1.5f*height));
        image->set(pixel.x, pixel.y, c0.lerp(c1, alpha));
    });
};