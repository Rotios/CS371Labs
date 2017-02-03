/**
  \file Drawung.h
 */
 /*  John Freeman
     Jose Rivas-Garcia
     Julia Goldman
     Matheus de Carvalho Souza
 */

#pragma once
#include <G3D/G3DAll.h>

 /** \brief Application framework. */
class Drawing {
protected:
    bool inBounds(int x, int y, const shared_ptr<Image>& image) const;

    void drawLine(const Point2int32& point1, const Point2int32& point2, int offset, const Color4& c, shared_ptr<Image>& image) const;

    void drawHLine(int x0, int x1, int y, const Color4& c, shared_ptr<Image>& image) const;

    void drawVLine(int x, int y0, int y1, const Color4& c, shared_ptr<Image>& image) const;

    void drawFlatLine(int x0, int x1, int y, float m, const Color4& c, shared_ptr<Image>& image) const;

    void drawSteepLine(int x, int y0, int y1, float m, const Color4& c, shared_ptr<Image>& image) const;

    void drawSteepLine(const Point2int32& point1, const Point2int32& point2, int offset, const Color4& c, shared_ptr<Image>& image) const;

    void drawFlatLine(const Point2int32& point1, const Point2int32& point2, int offset, const Color4& c, shared_ptr<Image>& image) const;

public:
    void drawLine(const Point2int32& point1, const Point2int32& point2, const Color4& c, shared_ptr<Image>& image) const;

    void drawAxes(int rad, int rng, int xOff, int yOff, shared_ptr<Image>& image, shared_ptr<Image>& map) const;

    void drawVarGraph(bool isClock, int width, int height, int xOff, int yOff, int sign, shared_ptr<Image>& image) const;

    void drawMyGraph(shared_ptr<Image>& image) const;

    void drawClock(shared_ptr<Image>& image) const;

    void drawCantorDust(float xSt, float xEn, float y, int level, shared_ptr<Image>& image) const;

    void drawThickLine(const Point2int32& point1, const Point2int32& point2, const Color4& c, int thickness, shared_ptr<Image>& image, shared_ptr<Image>& map) const;

    void drawGradiantBackground(const Color4& c1, const Color4& c2, int height, int width, shared_ptr<Image>& image) const;

    Drawing();
    ~Drawing();

};

