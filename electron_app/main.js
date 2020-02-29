const { app, BrowserWindow } = require('electron');
const Gpio = require('onoff').Gpio;

app.on('ready', function() {
    var mainWindow = new BrowserWindow({
        show: false,
    });
    mainWindow.maximize();
    mainWindow.loadFile('index.html');
    mainWindow.show();
   
    
	const led = new Gpio(17, 'out');
	const button = new Gpio(4, 'in', 'both');
	 
	button.watch((err, value) => {
	  if (err) {
		throw err;
	  }
	 
	  led.writeSync(value);
	  //status.innerHTML(value);
	  //document.getElementById('status').innerHTML(value);
	  console.log("Button Status: " + value);
	});
	 
	process.on('SIGINT', _ => {
	  led.unexport();
	  button.unexport();
	});
	
});
