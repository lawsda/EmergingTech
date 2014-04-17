#pragma strict

var showMenu = true;
var gameStarted = false;
var player : GameObject;

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
				player = GameObject.FindGameObjectWithTag("Player");
				Camera.main.GetComponent(SmoothFollow).enabled = true;
				if(player != null)
				player.gameObject.GetComponent(DrivingControls).enabled = true;
				showMenu = false;
				gameStarted = true;
			}
		}
		
		if(GUI.Button(Rect(50, 70, 100, 30), "Find Opponent")){

		}
		
		if(GUI.Button(Rect(50, 110, 100, 30), "Quit")){
			Application.Quit();
		}

		GUI.EndGroup();
	}
	else{
		if(GUI.Button(Rect(Screen.width - 80, 0, 80, 20), "Show Menu")){
			showMenu = true;
		}
	}
}