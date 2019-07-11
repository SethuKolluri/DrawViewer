// ------------------------------------------------
// BASIC SETUP
// ------------------------------------------------
var scene, camera, renderer, controls;



// Create an empty scene
scene = new THREE.Scene();

// Create a basic perspective camera
camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 1, 1000);
camera.position.z = 10;

// Create a renderer with Antialiasing
renderer = new THREE.WebGLRenderer({ antialias: true });

// Configure renderer clear color
renderer.setClearColor("#ffffff");

// Configure renderer size
renderer.setSize(window.innerWidth, window.innerHeight);

// Append Renderer to DOM
document.body.appendChild(renderer.domElement);

//controls = new THREE.OrbitControls( camera, renderer.domElement );

//track ball controls code
controls = new THREE.TrackballControls(camera, renderer.domElement);
controls.rotateSpeed = 3;
controls.zoomSpeed = 3;
controls.panSpeed = 3;
controls.noZoom = false;
controls.noPan = false;
controls.staticMoving = true;
controls.dynamicDampingFactor = 0.3;
controls.addEventListener('change', render);


jQuery.getJSON("..\\sampleInputs\\sample1.json", function (jsonData) {

    jsonData.curves.forEach(curve => {
        if (curve.type == "straight") {
            let geomData = curve.geometry;

            let material = new THREE.LineBasicMaterial({
                color: 0xff0000
            });

            let geometry = new THREE.Geometry();
            geometry.vertices.push(
                new THREE.Vector3(geomData.end_point[0], geomData.end_point[1], geomData.end_point[2]),
                new THREE.Vector3(geomData.start_point[0], geomData.start_point[1], geomData.start_point[2])
            );

            let line = new THREE.Line(geometry, material);
            scene.add(line);
        }
        else if (curve.type == "circle") {
            let geomData = curve.geometry;

            let material = new THREE.LineBasicMaterial({
                color: 0xff0000
            });
            let curveGeom = new THREE.EllipseCurve(
                geomData.center[0], geomData.center[1],            // ax, aY
                geomData.radius, geomData.radius,           // xRadius, yRadius
                geomData.start_param, geomData.end_param,  // aStartAngle, aEndAngle
                false,            // aClockwise
                0                 // aRotation
            );

            let points = curveGeom.getPoints(50);
            let geometry = new THREE.BufferGeometry().setFromPoints(points);

            // Create the final object to add to the scene
            let ellipse = new THREE.Line(geometry, material);
            scene.add(ellipse);
        }
    });
    // controls.update();

    camera.position.x = 90;
    camera.position.y = 0;
    camera.position.z = 70;
    camera.rotation =0;
    renderer.render(scene, camera);

});

// Render Loop
var animate = function () {
  requestAnimationFrame( animate );

  controls.update(); 
};

function render() {
    renderer.render( scene, camera );
}

animate();