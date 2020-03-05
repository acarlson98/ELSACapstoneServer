const { app, BrowserWindow } = require('electron');
const Gpio = require('onoff').Gpio;

app.on('ready', function() {
    var mainWindow = new BrowserWindow({
		show: false,
		webPreferences: {
			nodeIntegration: true,
		}
    });
    mainWindow.maximize();
    mainWindow.loadFile('index.html');
    mainWindow.show();
   
	console.log(mainWindow.webContents);
	
	const led = new Gpio(17, 'out');
	const button = new Gpio(4, 'in', 'both');
	 
	button.watch((err, value) => {
	  if (err) {
		throw err;
	  }
	 
	  led.writeSync(value);
	  console.log("Button Status: " + value);
	//   mainWindow.webContents.insertText(value);
	  mainWindow.webContents.send('button-status', value);
	});
	 
	process.on('SIGINT', _ => {
	  led.unexport();
	  button.unexport();
	});
	
});
