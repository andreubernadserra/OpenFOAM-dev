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
#include "treeDataFace.H"
#include "dimensionedScalar.H"

using namespace Foam;

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

int main(int argc, char *argv[])
{
    argList::addNote("Test find sphere method");

    #include "setRootCase.H"
    #include "createTimeNoFunctionObjects.H"
    #include "createNamedMesh.H"

    const scalar radius = 2.5e-2;
    const point centre = {5e-2, 5e-2, 5e-3};
    Info<< "Sphere with radius " << radius << " and centre " << centre << "\n";


    Info<< "Testing the treeDataCell class\n";
    volScalarField overlapCells
    (
        IOobject
        (
            "overlapCells",
            runTime.name(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensionedScalar("overlaps", dimless, 0)
    );

    const indexedOctree<treeDataCell>& cellTree = mesh.cellTree();
    const labelList cells = cellTree.findSphere(centre, sqr(radius));

    forAll(cells, i)
    {
        overlapCells[cells[i]] ++;
    }

    
    Info<< "Testing the treeDataFace class\n";
    surfaceScalarField overlapFaces
    (
        IOobject
        (
            "overlapFaces",
            runTime.name(),
            mesh,
            IOobject::NO_READ,
            IOobject::NO_WRITE
        ),
        mesh,
        dimensionedScalar("overlaps", dimless, 0)
    );

    labelList internalFaces(mesh.nInternalFaces());
    forAll(internalFaces, facei)
    {
        internalFaces[facei] = facei;
    }

    indexedOctree<treeDataFace> faceTree(
        treeDataFace(
            false, 
            mesh,
            internalFaces
        ),
        treeBoundBox(mesh.bounds()).extend(1e-4),
        8,
        0.5,
        1.0
    );

    const labelList faces = faceTree.findSphere(centre, sqr(radius));

    forAll(faces, i)
    {
        overlapFaces[faces[i]] ++;
    }


    runTime ++;
    overlapCells.write();
    overlapFaces.write();

    Info<< "End\n" << endl;

    return 0;
}


// ************************************************************************* //
