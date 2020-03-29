SaveMesh plug-in

TODO: plug-in description

TODO: link to GmshDefines.h

Term explanation:

- Nodes / points -- triplets of coordinates of all vertices of polygons of the mesh.

- Elements -- polygons of the mesh, referencing to the vertex points through their tags -- integer order numbers

- Entity -- group of nodes or elements, stored in the mesh file as a separate chunk.
  Entities correspond to the vectors of points and triangles / quads / polygons, obtained through getters of TriangleMesh or PolygonMesh classes
  TODO: link to the AutoDesk Fusion 360 API Reference Manual

First in the mesh file are stored all point entities from all mesh classes, in the second part -- all element entities.
Look here for details of the .msh file structure:
TODO: link to the GMSH .msh file format description


Building of the plug-in

TODO: build description, link to the MS Visual Studio Community download


Testing procedure:

- start Fusion 360
- open the file "1.71_1.78_1.85 GHz_Single Element.step"
- generate the mesh with 20 mm grid:
    - MESH tab
    - "BRep to Mesh" (the second button on the toolbar)
    - click the cube
    - "Maximum edge length" -- enter slider value "20 mm" using the keyboard
    - OK
- start the plug-in SaveMesh:
    - TOOLS tab
    - ADD-INS (the second buton on the toolbar)
    - Add-Ins tab
    - SaveMesh
    - Run
- after ~6 sec close the Fusion
     
SaveMesh_polygon.msh and SaveMesh_triangle.msh should be renewed, but unchanged


TODO: triple test description, SaveMesh_polygon_3x.msh and SaveMesh_triangle_3x.msh
