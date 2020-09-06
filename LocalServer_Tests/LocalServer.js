const http = require('http'),
    fs = require('fs'),
    path = require('path'),
    url = require('url'),
    os = require('os');

let networkInterfaces = os.networkInterfaces();

const config = require("./config.json");
const port = config.port;
const defaultSceneName = config.defaultSceneName;
let basePath = config.basePath;

if (!path.isAbsolute(basePath)) {
    basePath = path.join(__dirname, basePath);
}


var sceneExtension = '.vitscnj';
var filename;


console.log(`Server running and listening at:\n`);
console.log("Interfaces:\n");
for (let key in networkInterfaces) {
    console.log(`  ${key}:`);
    networkInterfaces[key].forEach((interface) => {
        console.log(`     ${interface.address}:${port}`);
    });
}

const server = http.createServer((req, res) => {
    var q = url.parse(req.url, true);
    var filepath = basePath + q.pathname;
    filepath = path.normalize(filepath);

    if (!filepath.includes(".")) {
        filename = defaultSceneName;
        filepath += filename;
    }
    else {
        filename = path.basename(filepath);
    }

    doReadFile(filename, filepath, res);

}).listen(port);


function doReadFile(filename, filepath, res) {
    filename = decodeURIComponent(filename);
    filepath = decodeURIComponent(filepath);

    let encoding = filename == defaultSceneName ? "utf8" : "";
    fs.readFile(filepath, encoding, function (err, data) {
        if (err) {
            console.log(`Error, file not found. filename: ${filename} \n`);

            res.writeHead(404, { 'Content-Type': 'text/plain' });
            return res.end("404 Not Found");
        }
        else {
            console.log(`Requested filename: ${filename} \n`);
            res.statusCode = 200;
            res.setHeader('Content-disposition', 'attachment; filename=' + filename);
            res.setHeader('Content-Type', 'text/plain');

            if (filename == defaultSceneName) {
                let sceneData = JSON.parse(data);
                sceneData.resources = [];
                fs.readdirSync(basePath).forEach((resource) => {
                    if (resource != defaultSceneName) {
                        console.log(`Adding external resource: ${resource}`);
                        sceneData.resources.push(resource);
                    }
                });

                data = JSON.stringify(sceneData, "", "\t");
            } 
            console.log("------------------- Done ------------------- ");
            console.log("-------------------------------------------- \n");
        }
        doEnd(data, res);
    });
}

function doEnd(data, res) {
    try {
        res.end(data);
    } catch (err) {
        console.error(err);
    }
}