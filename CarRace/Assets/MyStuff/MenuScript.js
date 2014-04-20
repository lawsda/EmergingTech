#pragma strict

var showMenu = true;
var gameStarted = false;
var clientRdy = false;
var player1 : GameObject = null;
var player2 : GameObject = null;
var connectionIP = "127.0.0.1";
var connectionPort = 25001;

function OnGUI(){	

	if(showMenu){
		if(Network.peerType == NetworkPeerType.Disconnected){
			GUI.BeginGroup(Rect(Screen.width/2 - 100, 10, 200, 130));
			
			GUI.Box(Rect(0, 0, 200, 160), "Find Opponent");		
			if(GUI.Button(Rect(50, 30, 100, 20), "Host Race")){
				Network.InitializeServer(32, connectionPort, false);
			}
			if(GUI.Button(Rect(50, 60, 100, 20), "Find Race")){
				Network.Connect(connectionIP, connectionPort);
				if(Network.peerType == NetworkPeerType.Client)
					showMenu = false;
			}
			if(GUI.Button(Rect(50, 90, 100, 20), "Quit")){
				Application.Quit();
			}

			GUI.EndGroup();
		}
		else if(Network.peerType == NetworkPeerType.Client){
			
			GUI.BeginGroup(Rect(Screen.width/2 - 100, 10, 200, 130));
			
			GUI.Box(Rect(0, 0, 200, 160), "Waiting on Server...");		
			if(gameStarted){
				if( GUI.Button(new Rect(50, 30, 100, 20), "Restart")){
					Application.LoadLevel("CarRace_v1.0");
				}
			}
			if(GUI.Button(Rect(50, 60, 100, 20), "Disconnect")){
				Network.Disconnect(200);
				Network.Destroy(player2.GetComponent(NetworkView).gameObject);
			}
			if(GUI.Button(Rect(50, 90, 100, 20), "Quit")){
				Application.Quit();
			}

			GUI.EndGroup();
		}
		else if(Network.peerType == NetworkPeerType.Server){
			GUI.BeginGroup(Rect(Screen.width/2 - 100, 10, 200, 130));
			
			var waitingMsg = "Waiting for Client...";
			if(player2 != null)
				waitingMsg = "Ready to race";
			
			GUI.Box(Rect(0, 0, 200, 160), waitingMsg);		
			if(gameStarted){
				if( GUI.Button(new Rect(50, 30, 100, 20), "Restart")){
					Application.LoadLevel("CarRace_v1.0");
				}
			}
			else {
				if( GUI.Button(new Rect(50, 30, 100, 20), "Start Game")){
					Camera.main.GetComponent(SmoothFollow).enabled = true;
					if(player1 != null){
						player1.GetComponent(DrivingControls).enabled = true;
					}
					if(player2 != null){
						player2.GetComponent(DrivingControls).enabled = true;
					}
					showMenu = false;
					gameStarted = true;
					
				}
			}
			if(GUI.Button(Rect(50, 60, 100, 20), "Disconnect")){
				Network.Disconnect(200);
				Network.Destroy(player1.GetComponent(NetworkView).gameObject);
			}
			if(GUI.Button(Rect(50, 90, 100, 20), "Quit")){
				Application.Quit();
			}

			GUI.EndGroup();
		}
		
		if(GUI.Button(Rect(Screen.width - 80, 0, 80, 20), "Hide Menu")){
			showMenu = false;
		}
	}
	else{
		if(GUI.Button(Rect(Screen.width - 80, 0, 80, 20), "Show Menu")){
			showMenu = true;
		}
	}
}