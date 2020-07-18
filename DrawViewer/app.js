

var express = require('express'),
    path = require('path'),
    app = express(),
    multer    =   require( 'multer' ),
    process =  require('child_process');
    var fs = require('fs');

//start server communication
app.listen(5000,function(){
console.log("Listening to 5000 port");    
});

//multer storage name fix from online
var storage = multer.diskStorage({
    destination: function (req, file, cb) {
      cb(null, './uploads')
    },
    filename: function (req, file, cb) {
        cb(null, Date.now()+file.originalname);        
    }
  })  
var upload = multer({ storage: storage });



//upload action
app.post( '/upload', upload.single( '.dxf' ), function( req, res, next ) {
    let success = false;
    if(req.file.originalname.endsWith(".dxf")||req.file.originalname.endsWith(".dwg"))
    {
        console.log(req.file);
        
try{

            let chil = process.execFileSync('dxf2json.exe', [req.file.path]);
            success = true;
        }
        catch(e)
        {
            success = false;
        }
        //return res.status( 200 ).send( req.file );
    }
    else
    {
        return  res.status( 422 ).send("Incorrect file type");
    }

    if(success)
       {        
       return res.status( 200 ).send("Successful click View");
       }
    else
        return  res.status( 422 ).send("Failure please try again");
    
});


//Node file access
app.get('/index.html',function(req,res){
    res.sendFile(path.join(__dirname + '/index.html')); 
 });
 app.get('/', function(req, res) {
     res.sendFile(path.join(__dirname + '/drag_and_drop.html')); 
 });
 
 app.get('/dropzone/dropzone.css', function(req, res) {
     res.sendFile(path.join(__dirname + '/dropzone/dropzone.css')); 
 });
 
 app.get('/dropzone/dropzone.js', function(req, res) {
     res.sendFile(path.join(__dirname + '/dropzone/dropzone.js')); 
 });
 
 app.get('/js/viewer.js',function(req,res){
     res.sendFile(path.join(__dirname + '/js/viewer.js')); 
 });
 
 app.get('/js/threeJS/three.js',function(req,res){
     res.sendFile(path.join(__dirname + '/js/threeJS/three.js')); 
 });
 
 app.get('/js/threeJS/EllipseCurve.js',function(req,res){
     res.sendFile(path.join(__dirname + '/js/threeJS/EllipseCurve.js')); 
 });
 
 app.get('/js/threeJS/Curve.js',function(req,res){
     res.sendFile(path.join(__dirname + '/js/threeJS/Curve.js')); 
 });
 
 app.get('/js/threeJS/Vector2.js',function(req,res){
     res.sendFile(path.join(__dirname + '/js/threeJS/Vector2.js')); 
 });
 
 app.get('/sampleInputs/out.json',function(req,res){
     res.sendFile(path.join(__dirname + '/sampleInputs/out.json')); 
 });
 
 app.get('/out.json',function(req,res){
    res.sendFile(path.join(__dirname + '/out.json')); 
});
 
 app.get('/js/threeJS/TrackballControls.js',function(req,res){
     res.sendFile(path.join(__dirname + '/js/threeJS/TrackballControls.js')); 
 });
 