/**
  \file Edges.h
 */
 /*  John Freeman
     Jose Rivas-Garcia
     Julia Goldman
     Matheus de Carvalho Souza
 */

#pragma once
#include <G3D/G3DAll.h>

class Edges{ 
public: 
    Point2int32 p0; 
    Point2int32 p1; 
    int depth; 

    Edges(); 
    ~Edges();
};