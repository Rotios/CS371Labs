/**
  \file Rasterizer.h
 */
 /*  John Freeman
     Jose Rivas-Garcia
     Julia Goldman
     Matheus de Carvalho Souza
     Uses code from Julia's CS136 Rasterization Project
     and Jose's CS136 Rasterization Project.
 */

#pragma once
#include <G3D/G3DAll.h>

 /** \brief Application framework. */
class Rasterizer {
protected:
    /** Called by drawThickLine()
        Sets c0 and c1 within width by height box*/
    void fitToBounds(Point2& c0, Point2& c1, int r, int w, int h) const;

    /** Called by merge**/
    int findQuadrant(int x, int y, int width, int height) const;

public:
    /** Calls fitToBounds()
        writes to image*/
    void drawThickLine(const Point2int32& point1, const Point2int32& point2, const Color4& c, int thickness, shared_ptr<Image>& image, shared_ptr<Image>& map) const;

    void drawThickLine(const Point2int32& point1, const Point2int32& point2, const Color4& c, int thickness, shared_ptr<Image>& image, shared_ptr<Image>& map, bool branchEnd) const;

    /** writes to image */
    void drawGradiantBackground(const Color4& c1, const Color4& c2, int height, int width, shared_ptr<Image>& image) const;
  
    /** Calls findQuadrants()
        writes to image*/
    void mergeQuadrants(const shared_ptr<Image>& q1, const shared_ptr<Image>& q2, const shared_ptr<Image>& q3, const shared_ptr<Image>& q4, shared_ptr<Image>& image) const; 
    
    Rasterizer();
    ~Rasterizer();

};

