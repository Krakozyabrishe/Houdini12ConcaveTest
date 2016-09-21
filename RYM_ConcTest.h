#ifndef __SOP_ConcTest_h__
#define __SOP_ConcTest_h__

#include <SOP/SOP_Node.h>

namespace HDK_Sample {

/// Pure C++ implementation of @ref SOP_HOMWave
/// @see SOP_HOMWave, vex_wave(), @ref HOM/SOP_HOMWave.py
class SOP_ConcTest : public SOP_Node
{
public:
	     SOP_ConcTest(OP_Network *net, const char *name, OP_Operator *op);
    virtual ~SOP_ConcTest();

    static PRM_Template		 myTemplateList[];
    static OP_Node		*myConstructor(OP_Network*, const char *,
							    OP_Operator *);

protected:
    virtual OP_ERROR		 cookMySop(OP_Context &context);
	bool isConcave(std::set<GEO_Primitive*> pols);
private:
};
}	// End of HDK_Sample namespace

#endif