/*
 * *
 * Test concave primitivs 
 *
 *
 *----------------------------------------------------------------------------
 */

/

#include <UT/UT_DSOVersion.h>
#include <SYS/SYS_Math.h>
#include <GU/GU_Detail.h>
#include <GEO/GEO_AttributeHandle.h>
#include <PRM/PRM_Include.h>
#include <OP/OP_Director.h>
#include <OP/OP_Operator.h>
#include <OP/OP_OperatorTable.h>
#include <GU/GU_PrimPoly.h>
#include "SOP_ConcTest.h"

using namespace RYM_Namespace;

void
newSopOperator(OP_OperatorTable *table)
{
     table->addOperator(new OP_Operator("rym_concave_test",
                                        "Rym CPP concave test ",
                                         SOP_ConcTest::myConstructor,
                                         SOP_ConcTest::myTemplateList,
                                         1,
                                         1,
                                         0));
}

PRM_Template
SOP_ConcTest::myTemplateList[] = {
    PRM_Template(),
};


OP_Node *
SOP_ConcTest::myConstructor(OP_Network *net, const char *name, OP_Operator *op)
{
    return new SOP_ConcTest(net, name, op);
}

SOP_ConcTest::SOP_ConcTest(OP_Network *net, const char *name, OP_Operator *op)
        : SOP_Node(net, name, op)
{
}

SOP_ConcTest::~SOP_ConcTest()
{
}

bool isConcave(std::set<GEO_Primitive*> pols)
{
    GEO_Primitive *p, *pp;
    float a,b,c,d, test;
    UT_Vector3 n;
    int plus=0,minus=0;
    UT_Vector3 p2;
    //GEO_Point point;
    for(std::set<GEO_Primitive*>::const_iterator it = pols.begin();it != pols.end(); it++)
    {
        p=(*it);
        n = p->computeNormal();
        a=n.x();
        b=n.y();
        c=n.z();
        p2 = p->getVertex(0).getPt()->getPos3();
        d=-(a*p2[0]+b*p2[1]+c*p2[2]);
        plus=0;
        minus=0;
        for(std::set<GEO_Primitive*>::const_iterator it2 = pols.begin();it2 != pols.end(); it2++)
        {   
            pp=(*it2);
            for (int i=0; i<3; i++)
            {
                p2 = pp->getVertex(i).getPt()->getPos3();
                test=a*p2[0]+b*p2[1]+c*p2[2]+d;
                if (abs(test)>0.01)
                {
                    if (test>0)
                        ++plus;
                    else
                        ++minus;
                }
                if (plus*minus != 0)
                    return true;
            }
        }
    }

return false;
}

OP_ERROR
SOP_ConcTest::cookMySop(OP_Context &context)
{
    // Before we do anything, we must lock our inputs.  Before returning,
    // we have to make sure that the inputs get unlocked.
    if (lockInputs(context) >= UT_ERROR_ABORT)
        return error();

    GEO_PrimList prims = gdp->primitives();
    GEO_Primitive *p;
    std::set<int> set_n;
    std::set<GEO_Primitive *> pols;
    int val=0
    GEO_AttributeHandle h = gdp->findPrimitiveAttribute("piece");
    GEO_AttributeHandle h2 = gdp->findPrimitiveAttribute("isConcave");
    for (int i=0; i<prims.entries(); i++)
    {              
        h.setElement(prims[i]);
        set_n.insert(h.getI());
    }
    int curr_n;
    for(std::set<int>::const_iterator it = set_n.begin();it != set_n.end(); it++)
    {
        curr_n=(*it);
        pols.clear();
        for (int i=0; i<prims.entries(); i++)
        {              
            h.setElement(prims[i]);
            if (h.getI()==curr_n)
                pols.insert(prims[i]);
                i=i+0;
        }
        if (isConcave(pols)==true)
            val=1;
        else
            val=0
        for(std::set<GEO_Primitive*>::const_iterator it2 = pols.begin();it2 != pols.end(); it2++)
        {
            p=(*it2);
            h2.setElement(p);
            h2.setI(val);
        }
    }

    unlockInputs();
    return error();