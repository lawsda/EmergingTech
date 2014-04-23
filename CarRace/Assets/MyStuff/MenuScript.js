#pragma strict

var showMenu = true;
var gameStarted = false;
var clientRdy = false;
var player1 : GameObject = null;
var player2 : GameObject = null;
var connectionIP = "127.0.0.1";
var connectionPort = 25001;

var finishLine : GameObject;


function Start(){
	finishLine = GameObject.FindGameObjectWithTag("Finish");
}

function OnGUI(){	
  /*
	if(player1 != null && player1.gameObject.networkView.isMine
	&& Camera.main.GetComponent(SmoothFollow).target == null){
		Camera.main.GetComponent(SmoothFollow).target = player1.gameObject.transform;
	}
//*/
	if(showMenu){
		if(Network.peerType == NetworkPeerType.Disconnected){
			GUI.BeginGroup(Rect(Screen.width/2 - 100, 10, 200, 130));
			
				GUI.Box(Rect(0, 0, 200, 160), "Find Opponent");		
				if(GUI.Button(Rect(50, 30, 100, 20), "Host Race")){
					Network.InitializeServer(32, connectionPort, false);
				}
				if(GUI.Button(Rect(50, 60, 100, 20), "Find Race")){
					Network.Connect(connectionIP, connectionPort);
				}
				if(GUI.Button(Rect(50, 90, 100, 20), "Quit")){
					Application.Quit();
				}

			GUI.EndGroup();
		}
		else if(Network.peerType == NetworkPeerType.Connecting){
						showMenu = false;
		}
		else if(Network.peerType == NetworkPeerType.Client){
		
			player2 = GameObject.FindWithTag("Player2");
			
			GUI.BeginGroup(Rect(Screen.width/2 - 100, 10, 200, 130));
			
				GUI.Box(Rect(0, 0, 200, 160), "Waiting on Server...");		
				if(gameStarted){
					if( GUI.Button(new Rect(50, 30, 100, 20), "Restart")){
						Network.Disconnect(200);
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
		
			if(Camera.main.GetComponent(SmoothFollow).target == null)
				Camera.main.GetComponent(SmoothFollow).target = player1.gameObject.transform;
		
			GUI.BeginGroup(Rect(Screen.width/2 - 100, 10, 200, 130));
			
				var waitingMsg = "Waiting for Client...";
				if(player2 != null)
					waitingMsg = "Ready to race";
				
				GUI.Box(Rect(0, 0, 200, 160), waitingMsg);		
				if(gameStarted){
					if( GUI.Button(new Rect(50, 30, 100, 20), "Restart")){
						Network.Disconnect(200);
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
						finishLine.gameObject.GetComponent(GameTimer).RaceIsOn();
					}
				
					if(GUI.Button(Rect(50, 60, 100, 20), "Disconnect")){
						Network.Disconnect(200);
						Network.Destroy(player1.GetComponent(NetworkView).gameObject);
					}
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
	
	 if(Network.peerType == NetworkPeerType.Client){
	 	if(player2 != null){
			Camera.main.GetComponent(SmoothFollow).enabled = true;
			Camera.main.GetComponent(SmoothFollow).target = player2.gameObject.transform;
		}
	 }
}