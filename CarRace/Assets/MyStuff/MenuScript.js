#pragma strict

var showMenu = true;
var gameStarted = false;
var player1 : GameObject;
var player2 : GameObject;
var multiMenu = false;
var connectionIP = "127.0.0.1";
var connectionPort = 25001;

function OnGUI(){

	if(!gameStarted){
		Camera.main.GetComponent(SmoothFollow).enabled = false;
	}

	if(showMenu){
		GUI.BeginGroup(Rect(Screen.width/2 - 100, 10, 200, 160));
		
		GUI.Box(Rect(0, 0, 200, 160), "Main Menu");

		if(gameStarted){
			if( GUI.Button(new Rect(50, 30, 100, 30), "Restart")){
				Application.LoadLevel("CarRace_v1.0");
			}
		}
		else{
			if( GUI.Button(new Rect(50, 30, 100, 30), "Start Game")){
				player1 = GameObject.FindGameObjectWithTag("Player");
				Camera.main.GetComponent(SmoothFollow).enabled = true;
				player1.gameObject.GetComponent(DrivingControls).enabled = true;
				player1.gameObject.GetComponent(DrivingControls).singlePlayer = true;
				showMenu = false;
				gameStarted = true;
			}
		
		
			if(GUI.Button(Rect(50, 70, 100, 30), "Find Opponent")){
				showMenu = false;
				multiMenu = true;
			}
		}
		
		if(GUI.Button(Rect(50, 110, 100, 30), "Quit")){
			Application.Quit();
		}

		GUI.EndGroup();
	}
	else if(multiMenu){
		GUI.BeginGroup(Rect(Screen.width/2 - 100, 10, 200, 160));
		
		GUI.Box(Rect(0, 0, 200, 160), "Find Opponent");

			if( GUI.Button(new Rect(50, 30, 100, 30), "Start Game")){
				player1 = GameObject.FindGameObjectWithTag("Player");
				Camera.main.GetComponent(SmoothFollow).enabled = true;
				if(player1 != null)
					player1.gameObject.GetComponent(DrivingControls).enabled = true;
				showMenu = false;
				multiMenu = false;
				gameStarted = true;
			}
		
		
		if(GUI.Button(Rect(50, 70, 100, 30), "Host Race")){
			Network.InitializeServer(32, connectionPort, false);
			
		}
		
		if(GUI.Button(Rect(50, 110, 100, 30), "Find Race")){
			Network.Connect(connectionIP, connectionPort);
			
		}

		GUI.EndGroup();
	}
	else{
		if(GUI.Button(Rect(Screen.width - 80, 0, 80, 20), "Show Menu")){
			showMenu = true;
		}
	}
}