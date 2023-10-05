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

    // Different spheres for covering different cases:
    //   - Cell fully overlapped by the sphere
    //   - One point overlapped by the sphere
    //   - One face intersected by the sphere
    //   - One within a cell not intersecting anything
    const scalar rBig = 2e-2;
    const scalar rSmall = 2.5e-3;
    const point cCell = {5e-2, 5e-2, 5e-3};
    const point cPoint = {0, 2e-2, 5e-3};
    const point cFace = {9e-2, 9e-2, 5e-3};
    const point cNone = {8.3e-2, 1e-2, 5e-3};
    

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
    labelList cells = cellTree.findSphere(cCell, sqr(rBig));
    cells.append(cellTree.findSphere(cPoint, sqr(rSmall)));
    cells.append(cellTree.findSphere(cFace, sqr(rSmall)));
    cells.append(cellTree.findSphere(cNone, sqr(rSmall)));

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

    labelList faces = faceTree.findSphere(cCell, sqr(rBig));

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
