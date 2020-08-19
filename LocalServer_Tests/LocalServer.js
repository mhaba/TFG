const http = require('http'),
    fs = require('fs'),
    path = require('path')
    url = require('url')

const config = require("./config.json");

const port = config.port;

//  Yo must change basePath to the location that contains the scene to download.
let basePath = config.basePath;

if (!path.isAbsolute(basePath)) {
    basePath = path.join(__dirname, basePath);
}


var sceneExtension = '.vitscnj'
var filename

doWork()

console.log("Server running at http://192.168.0.18:8080 (http://localhost:8080)\n")


function doWork(){
    const server = http.createServer((req, res) => {
        var q = url.parse(req.url, true)
        var filepath = basePath + q.pathname
        filepath = path.normalize(filepath)

        if(!filepath.includes(".")){
            filename = "ExampleScene.vitscnj"
            filepath += filename
        }
        else{
            filename = path.basename(filepath)
        }

        doReadFile(filename, filepath, res)

    }).listen(port)
}

function doReadFile(filename, filepath, res){
    fs.readFile(filepath,"utf8", function(err, data){
        if(err){
            console.log("hola",err)
            console.log("Error, file not found. filename: " + filename + "\n")
            console.log("-------------------------------------------- \n")

            res.writeHead(404, {'Content-Type': 'text/plain'})
            return res.end("404 Not Found")
        }
        else{
            console.log("Requested filename: " + filename + "\n")
            res.statusCode = 200
            res.setHeader('Content-disposition', 'attachment; filename='+filename)
            res.setHeader('Content-Type', 'text/plain')

            console.log("------------------- Done ------------------- ")
            console.log("-------------------------------------------- \n")
        }
        doEnd(data, res)
    })
}

function doEnd(data, res){
    try{
        res.end(data)
    }catch(err){
        console.error(err)
    }
}

function extension(extension){
    var extName = path.extname(extension)
    return extName === sceneExtension
}