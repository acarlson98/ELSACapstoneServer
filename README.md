# ELSACapstoneServer
This repo contains the node.js application stored on the server at https://elsa.unoteam5.com/

This server will function to connect the ELSA device with the remote user's device and allow for peer-to-peer WebRTC communication.
The peer-to-peer connection will then be responsible for sending and receiving camera movement data as well as the media being captured by the camera.

A previous iteration of this code that can be run locally can be found at https://github.com/acarlson98/CapstoneWebRTC

The rest of this readme is from the EasyRTC documentation.

EasyRTC Server Example
======================

This folder contains all the files you'll need to create a simple server with EasyRTC, Express, and Socket.io. You can copy these files where you wish.

Files and Folders:
------------------

 - package.json - Provides project information allowing npm to find and install required modules.
 - server.js - Server code.
 - server_ssl.js - Server code for doing ssl using included keys, for testing purposes.
 - localhost.(key/crt) - self-signed keys for local host.
 - /static/ - Root folder for web server. Put html files here!

 
Installing Required Modules:
----------------------------

 - Type `npm install` in console.
 - This will read the package.json file to find and install the required modules including EasyRTC, Express, and Socket.io.
 - Required modules will go into a new 'node_modules' subfolder


Running the Server:
-------------------

 - Type `npm start` in console.


Viewing the examples:
---------------------

 - In your WebRTC enabled browser, visit your server address including the port. By default port 8080 is used.
 - http://localhost:8080/

Running the Server using docker:
-------------------

 - Type `npm run docker:build` then `npm run docker:start` in console.

Note: Require Docker to be installed.

Running the Server using docker-compose:
-------------------

 - Type `npm run docker-compose:start` in console.

Note: Require docker-compose to be installed.


Manualy build and run container
-------------------

Build:
> docker build . -t easyrtc

Run default:
> docker run -it --name easyrtc -p 8080:8080 easyrtc

Run HTTP server:
> docker run -it --name easyrtc --rm -p 8080:8080 easyrtc run server

Run HTTPS server:
>  docker run -it --name easyrtc --rm -p 8443:8443 easyrtc run server_ssl

Run HTTPS server with custom certs:
>  docker run -it --name easyrtc --rm -p 8443:8443 -v $(pwd)/certs/:/usr/src/app/certs/:ro easyrtc run server_ssl



