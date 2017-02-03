#pragma once
#include <G3D/G3DAll.h>
#include "Edges.h"

/** \brief Application framework. */
class CoralGenerator {
protected:

    /** Called by the generate functions.  Applies the rules of a given L-System to a given set of symbols.  Writes edges of lines to be drawn to create coral in the edgeArray passed.  */
    void applyRules(int depth, const Point2int32& location, float cumulativeAngle, float drawLength, float minAngle, float maxAngle, float thick, float thickMult, float drawMult, const String& symbolBuffer, const Table<String, Array<String>>& rules, Array<shared_ptr<Edges>>& edgeBuffer, Array<float>& thickBuffer);
    
     /** Called by writeCoral.  Specifies the L-System for finger coral and passes it to applyRules.*/
    void generateFingerCoral(int depth, Point2int32& location, float cumulativeAngle, float drawLength, float minAngle, float maxAngle, float thick, Array<shared_ptr<Edges>>&  edgeBuffer, Array<float>& thickBuffer);
    
     /** Called by writeCoral.  Specifies the L-System for thin coral and passes it to applyRules.*/
    void generateThinCoral(int depth, Point2int32& location, float cumulativeAngle, float drawLength, float minAngle, float maxAngle, float thick, Array<shared_ptr<Edges>>&  edgeBuffer, Array<float>& thickBuffer);
    
     /** Called by writeCoral.  Specifies the L-System for crazy coral and passes it to applyRules.*/
    void generateCrazyCoral(int depth, Point2int32& location, float cumulativeAngle, float drawLength, float minAngle, float maxAngle, float thick, Array<shared_ptr<Edges>>&  edgeBuffer, Array<float>& thickBuffer);
    
     /** Called by writeCoral.  Specifies the L-System for flat coral and passes it to applyRules.*/
    void generateFlatCoral(int depth, Point2int32& location, float cumulativeAngle, float drawLength, float minAngle, float maxAngle, float thick, Array<shared_ptr<Edges>>& edgeBuffer, Array<float>& thickBuffer);
    
     /** Called by writeCoral.  Specifies the L-System for branch coral and passes it to applyRules.*/
    void generateBranchCoral(int depth, Point2int32& location, float cumulativeAngle, float drawLength, float minAngle, float maxAngle, float thick, Array<shared_ptr<Edges>>&  edgeBuffer, Array<float>& thickBuffer);
    
     /** Called by writeCoral.  Specifies the L-System for tall coral and passes it to applyRules.*/
    void generateTallCoral(int depth, Point2int32& location, float cumulativeAngle, float drawLength, float minAngle, float maxAngle, float thick, Array<shared_ptr<Edges>>&  edgeBuffer, Array<float>& thickBuffer);

public:

     /** Called by App.cpp.  Writes the color and bump map of a given coral to the Images passed. */
    void writeCoral(String coralType, Color3 coralColor, shared_ptr<Image>& color, shared_ptr<Image>& bump, float initialThickness, float minAngle, float maxAngle);
    
     /** Empty Constructor for CoralGenerator */
    CoralGenerator();
    ~CoralGenerator();

};

