app.addSource(() => {
    new(class ARServerCommandHandler extends app.CommandHandler {
        getMessages() {
            return [
                "startARServer"
            ];
        }

        execute(message, params) {
            if (message == 'startARServer') {
                app.arServer.showServerWindow();
            }
        }
    });
})