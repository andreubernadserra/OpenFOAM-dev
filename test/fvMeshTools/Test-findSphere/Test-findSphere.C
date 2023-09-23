/*---------------------------------------------------------------------------*\

Application
    Test-findSphere

Description
    Testing the use of the find sphere method. 

\*---------------------------------------------------------------------------*/

#include "argList.H"
#include "Time.H"
#include "fvMeshTools.H"
#include "indexedOctree.H"
#include "treeDataCell.H"

using namespace Foam;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

labelList testFindSphere
(
    const indexedOctree<treeDataCell>& cellTree
)
{
    const scalar radiusSqr = 9e-4;
    const point centre = {5e-2, 5e-2, 5e-3};

    return cellTree.findSphere(centre, radiusSqr);
}

int main(int argc, char *argv[])
{
    argList::addNote("Test find sphere method");

    #include "setRootCase.H"
    #include "createTimeNoFunctionObjects.H"
    #include "createNamedMesh.H"

    const labelList cellsWithinSphere = testFindSphere(mesh.cellTree());

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
