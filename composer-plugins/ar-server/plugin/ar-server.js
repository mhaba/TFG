module.exports = function(app, angularApp, bg, templatePath) {
    const os = require('os');
    const path = require('path');

    app.arServer = app.arServer || {};

    class ARServer {
        constructor() {
            this._isStarted = false;
            this._port = 8080;
            this._sceneName = "ExampleScene.vitscnj";
            this._tempScenePath = path.join(app.paths.temp, path.join("arServer", this._sceneName));
        }

        get isStarted() { return this._isStarted; }
        get port() { return this._port; }
        set port(p) { this._port = p; }
        get sceneName() { return this._sceneName; }
        get tempScenePath() { return this._tempScenePath; }

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
                // Guardar la escena en directorio temporal
                app.render.Scene.Get().selectionManager.removeGizmos();
                bg.base.Writer.Write(this.tempScenePath, app.render.Scene.Get().sceneRoot)
                    .then(() => {
                        // La escena se ha guardado

                        // TODO: implementar esto
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