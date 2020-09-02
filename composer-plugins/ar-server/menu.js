module.exports = {
    getMenu: function() {
        return {
            label: "AR",
            submenu: [{
                label: "Start AR server",
                click: (item, fw) => { fw.webContents.send('triggerMenu', { msg: 'startARServer' }) }
            }]
        }
    }
}