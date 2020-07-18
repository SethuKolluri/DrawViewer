# DrawViewer
To Draw the entites into ThreeJS space

Read DXF/DWG files in C++ project and dump the geometry details into a json output file for drawing them using three.js


Steps to Use:
Start app.js with node
open localhost:5000
Upload https://github.com/SethuKolluri/DrawViewer/blob/master/customLibDxfrw/dxf2json/input.dwg
Press "View Recent" Button


Projects:

DrawViewer/customLibDxfrw/ are made on top of dxf2text sample from libdxfrw library, stable build was available at https://github.com/LibreCAD/libdxfrw
DrawViewer/DrawViewer/ uses node.js for server and three.js for viewing and dropzone.js for upload capabilities

Current Issues:
DrawViewer/customLibDxfrw/dxf2json/ 
	Supports only LINE,ARC,CIRCLE,POLYLINE,LWPOLYLINE if any further support needed , project to be extended
	libdxfrw support for reading files is also little on low side



	