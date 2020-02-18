//
//Copyright (c) 2016, Skedans Systems, Inc.
//All rights reserved.
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions are met:
//
//    * Redistributions of source code must retain the above copyright notice,
//      this list of conditions and the following disclaimer.
//    * Redistributions in binary form must reproduce the above copyright
//      notice, this list of conditions and the following disclaimer in the
//      documentation and/or other materials provided with the distribution.
//
//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
//AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
//LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
//CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
//SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
//INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
//ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
//POSSIBILITY OF SUCH DAMAGE.
//
// var easyrtc = require('open-easyrtc');

var selfEasyrtcid = "";
// var channelIsActive = {}; // tracks which channels are active


function connect() {
    easyrtc.setVideoDims(1280,720);
    //turn this off for production
    easyrtc.enableDebug(true);
    // easyrtc.setDataChannelOpenListener(openListener);
    // easyrtc.setDataChannelCloseListener(closeListener);
    // easyrtc.setPeerListener(addToConversation);
    easyrtc.setRoomOccupantListener(convertListToButtons);
    easyrtc.easyApp("easyrtc.videoChatHd", "selfVideo", ["callerVideo"], loginSuccess, loginFailure);
}


// function addToConversation(who, msgType, content) {
//     // Escape html special characters, then add linefeeds.
//     content = content.replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;');
//     content = content.replace(/\n/g, '<br />');
//     document.getElementById('conversation').innerHTML +=
//             "<b>" + who + ":</b>&nbsp;" + content + "<br />";
// }


// function openListener(otherParty) {
//     channelIsActive[otherParty] = true;
//     updateButtonState(otherParty);
// }


// function closeListener(otherParty) {
//     channelIsActive[otherParty] = false;
//     updateButtonState(otherParty);
// }


function clearConnectList() {
    var otherClientDiv = document.getElementById('otherClients');
    while (otherClientDiv.hasChildNodes()) {
        otherClientDiv.removeChild(otherClientDiv.lastChild);
    }
}


// function updateButtonState(otherEasyrtcid) {
//     var isConnected = channelIsActive[otherEasyrtcid];
//     if(document.getElementById('connect_' + otherEasyrtcid)) {
//         document.getElementById('connect_' + otherEasyrtcid).disabled = isConnected;
//     }
//     if( document.getElementById('send_' + otherEasyrtcid)) {
//         document.getElementById('send_' + otherEasyrtcid).disabled = !isConnected;
//     }
// }


// function sendStuffP2P(otherEasyrtcid) {
//     var text = document.getElementById('sendMessageText').value;
//     if (text.replace(/\s/g, "").length === 0) { // Don't send just whitespace
//         return;
//     }
//     if (easyrtc.getConnectStatus(otherEasyrtcid) === easyrtc.IS_CONNECTED) {
//         easyrtc.sendDataP2P(otherEasyrtcid, 'msg', text);
//     }
//     else {
//         easyrtc.showError("NOT-CONNECTED", "not connected to " + easyrtc.idToName(otherEasyrtcid) + " yet.");
//     }

//     addToConversation("Me", "msgtype", text);
//     document.getElementById('sendMessageText').value = "";
// }


function convertListToButtons (roomName, data, isPrimary) {
    clearConnectList();
    var otherClientDiv = document.getElementById('otherClients');
    for(var easyrtcid in data) {
        var button = document.createElement('button');
        button.onclick = function(easyrtcid) {
            return function() {
                performCall(easyrtcid);
            };
        }(easyrtcid);

        var label = document.createTextNode(easyrtc.idToName(easyrtcid));
        button.appendChild(label);
        button.className = "callbutton";
        otherClientDiv.appendChild(button);
    }
}

// function convertListToButtons(roomName, occupantList, isPrimary) {
//     connectList = occupantList;

//     var otherClientDiv = document.getElementById('otherClients');
//     while (otherClientDiv.hasChildNodes()) {
//         otherClientDiv.removeChild(otherClientDiv.lastChild);
//     }

//     var label, button;
//     for (var easyrtcid in connectList) {
//         var rowGroup = document.createElement("span");
//         var rowLabel = document.createTextNode(easyrtc.idToName(easyrtcid));
//         rowGroup.appendChild(rowLabel);

//         button = document.createElement('button');
//         button.id = "connect_" + easyrtcid;
//         button.onclick = function(easyrtcid) {
//             return function() {
//                 startCall(easyrtcid);
//             };
//         }(easyrtcid);
//         label = document.createTextNode("Connect");
//         button.appendChild(label);
//         rowGroup.appendChild(button);

//         button = document.createElement('button');
//         button.id = "send_" + easyrtcid;
//         button.onclick = function(easyrtcid) {
//             return function() {
//                 sendStuffP2P(easyrtcid);
//             };
//         }(easyrtcid);
//         label = document.createTextNode("Send Message");
//         button.appendChild(label);
//         rowGroup.appendChild(button);
//         otherClientDiv.appendChild(rowGroup);
//         updateButtonState(easyrtcid);
//     }
//     if (!otherClientDiv.hasChildNodes()) {
//         otherClientDiv.innerHTML = "<em>Nobody else logged in to talk to...</em>";
//     }
// }


function performCall(otherEasyrtcid) {
    easyrtc.hangupAll();
    var acceptedCB = function(accepted, caller) {
        if( !accepted ) {
            easyrtc.showError("CALL-REJECTED", "Sorry, your call to " + easyrtc.idToName(caller) + " was rejected");
        }
    };
    var successCB = function() {};
    var failureCB = function() {};
    easyrtc.call(otherEasyrtcid, successCB, failureCB, acceptedCB);
}


function loginSuccess(easyrtcid) {
    selfEasyrtcid = easyrtcid;
    document.getElementById("iam").innerHTML = "I am " + easyrtc.cleanId(easyrtcid);
}


function loginFailure(errorCode, message) {
    easyrtc.showError(errorCode, message);
}


// Sets calls so they are automatically accepted (this is default behaviour)
easyrtc.setAcceptChecker(function(caller, cb) {
    cb(true);
} );

function sendUp() {
    document.getElementById("arrows_in").value = "Up";
}

function sendDown() {
    document.getElementById("arrows_in").value = "Down";
}

function sendLeft() {
    document.getElementById("arrows_in").value = "Left";
}

function sendRight() {
    document.getElementById("arrows_in").value = "Right";
}