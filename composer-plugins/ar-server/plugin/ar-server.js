module.exports = function(app, angularApp, bg, templatePath) {
    const os = require('os');
    const path = require('path');
    const fs = require('fs');
    const url = require('url');
    const http = require('http');

    app.arServer = app.arServer || {};

    class ARServer {
        constructor() {
            this._isStarted = false;
            this._port = 8080;
            this._sceneName = "ExampleScene.vitscnj";
            this._tempBasePath = path.join(app.paths.temp, "arServer");
            this._tempScenePath = path.join(app.paths.temp, path.join("arServer", this._sceneName));

            this._server = http.createServer();
        }

        get isStarted() { return this._isStarted; }
        get port() { return this._port; }
        set port(p) { this._port = p; }
        get sceneName() { return this._sceneName; }
        get tempBasePath(){ return this._tempBasePath; }
        get tempScenePath() { return this._tempScenePath; }

        get server() { return this._server; }

        networkData() {
            const networkInterfaces = os.networkInterfaces();
            let result = {};
            for (let key in networkInterfaces) {
                let interfaces = [];

                networkInterfaces[key].forEach((iface) => {
                    if (iface.family == "IPv6" || iface.internal) {
                        return;
                    }
                    interfaces.push(iface.address);
                });

                if (interfaces.length) {
                    result[key] = result[key] || interfaces;
                }
            }
            return result;
        }

        start() {
            return new Promise((resolve, reject) => {
                // Borrar archivos del directorio temporal
                fs.readdirSync(this.tempBasePath).forEach((resource) => {
                    fs.unlink(path.join(this.tempBasePath,resource));
                });

                // Guardar la escena en directorio temporal
                app.render.Scene.Get().selectionManager.removeGizmos();
                bg.base.Writer.Write(this.tempScenePath, app.render.Scene.Get().sceneRoot)
                    .then(() => {
                        // La escena se ha guardado
                        
                        this.server.on('request', (req, res) => {
                            let q = url.parse(req.url,true);

                            let basePath = this.tempBasePath;
                            let filePath = path.join(basePath, q.pathname);

                            let fileName = "";
                            let sceneName = this.sceneName;

                            if(!filePath.includes(".")){
                                fileName = sceneName;
                                filePath += fileName;
                            }
                            else{
                                fileName = path.basename(filePath);
                            }

                            let encoding = fileName == sceneName ? "utf8" : "";
                            fileName = decodeURIComponent(fileName);
                            filePath = decodeURIComponent(filePath);

                            try{
                                fs.readFile(filePath, encoding, function(err, data){
                                    if(err){
                                        console.log("Error, file nor found. Filename: " + fileName +"\n", true);
    
                                        res.writeHead(404, {'Content-Type': 'text/plain'})
                                        return res.end("404 Not Found")
                                    }
                                    else{
                                        console.log("Requested filename: " + fileName + "\n");
    
                                        res.statusCode = 200;
                                        res.setHeader('Content-disposition', 'attachment; filename=' + fileName);
                                        res.setHeader('Content-Type', 'text/plain');
    
                                        if(fileName == sceneName){
                                            let sceneData = JSON.parse(data);
                                            sceneData.resources = [];
                                            
                                            fs.readdirSync(basePath).forEach((resource) => {
                                                if(resource != sceneName) {
                                                    console.log(`Adding external resource: ${ resource }`);
                                                    sceneData.resources.push(resource);
                                                }
                                            });
    
                                            data = JSON.stringify(sceneData, "", "\t");
                                            console.log("Done");
                                        }
                                    }

                                    res.end(data);
                                });

                            }catch(err){
                                console.error(err,true);
                            }
                            

                        }).listen(this.port);

                        return Promise.resolve();
                    })

                .then(() => {
                    // El servidor está en marcha
                    this._isStarted = true;
                    resolve(this.isStarted);
                })

                .catch((err) => {
                    reject(err);
                });
            });
        }

        stop() {
            return new Promise((resolve, reject) => {
                // TODO: Implementar esto
                this.server.close();

                this._isStarted = false;
                resolve(this.isStarted);
            });
        }
    }

    app.arServer.server = new ARServer();

    app.arServer.showServerWindow = function() {
        app.ui.DialogView.Show({
                templateUrl: `${ templatePath }/ar-server.html`,
                title: "AR Server",
                showClose: false,
                type: 'modal',
                onAccept: () => { return true; }
            })
            .then(() => {
                return app.arServer.server.stop();
            })
            .then((status) => {
                console.log("AR Server is stopped");
            })
            .catch((err) => { console.log(err); });
    }

    angularApp.directive("arServerUi", function() {
        return {
            restrict: "E",
            templateUrl: `${ templatePath }/ar-server-ui.html`,
            scope: {},
            controller: ['$scope', function($scope) {
                $scope.networkData = app.arServer.server.networkData();
                $scope.isStarted = false;
                $scope.port = app.arServer.server.port;

                $scope.startServer = function() {
                    app.arServer.server.start()
                        .then((status) => {
                            $scope.isStarted = status;
                            console.log(`The AR server is started and listening at port ${ $scope.port }`);
                            $scope.$apply();
                        })
                        .catch((err) => {
                            console.error(err.message, true);
                        });
                }

                $scope.stopServer = function() {
                    app.arServer.server.stop()
                        .then((status) => {
                            $scope.isStarted = status;
                            console.log("The AR server is stopped");
                            $scope.$apply();
                        })
                        .catch((err) => {
                            console.error(err.message, true);
                        });
                }
            }]
        };
    })
}