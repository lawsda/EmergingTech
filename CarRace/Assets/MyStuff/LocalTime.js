#pragma strict

var now : System.DateTime;
var hour : int;
var dawnSkybox : Material;
var daySkybox : Material;
var nightSkybox : Material;

var player1 : GameObject;
var player2 : GameObject;

function Start(){
	now = System.DateTime.Now;	
	hour = now.Hour;
}

function timeChanges(timeOfDay : String){
	if(timeOfDay.Equals("Night")){
		Camera.main.GetComponent(Skybox).material = nightSkybox;
		this.gameObject.GetComponent(Light).intensity = .1;
		if(player1 != null){
			player1.GetComponent(Light).intensity = 8;
		}
		if(player2 != null){
			player2.GetComponent(Light).intensity = 8;
		}
	}
	if(timeOfDay.Equals("Dawn/Dusk")){
		Camera.main.GetComponent(Skybox).material = dawnSkybox;
		this.gameObject.GetComponent(Light).intensity = .25;
		if(player1 != null){
			player1.GetComponent(Light).intensity = 4;
		}
		if(player2 != null){
			player2.GetComponent(Light).intensity = 4;
		}
	}
	if(timeOfDay.Equals("Day")){
		Camera.main.GetComponent(Skybox).material = daySkybox;
		this.gameObject.GetComponent(Light).intensity = .4;
		if(player1 != null){
			player1.GetComponent(Light).intensity = 0;
		}
		if(player2 != null){
			player2.GetComponent(Light).intensity = 0;
		}
	}
}

function Update () {
	player1 = GameObject.FindGameObjectWithTag("Player1");
	player2 = GameObject.FindGameObjectWithTag("Player2");

	now = System.DateTime.Now;
	print (hour);
	
	if(hour < 6){
		timeChanges("Night");
	}
	else if(hour < 9){
		timeChanges("Dawn/Dusk");
	}
	else if(hour < 18){
		timeChanges("Day");
	}
	else if(hour < 21){
		timeChanges("Dawn/Dusk");
	}
	else{
		timeChanges("Night");
	}
}


//gui buttons for testing time changes
// /*
function OnGUI()
{
	if(GUI.Button(Rect(Screen.width - 20, Screen.height - 20, 20, 20), "+")){
			if(hour < 23)
				hour++;
			else
				hour = 0;
	}
	if(GUI.Button(Rect(Screen.width - 60, Screen.height - 20, 20, 20), "-")){
			if(hour > 0)
				hour--;
			else
				hour = 23;
	}
	if(GUI.Button(Rect(Screen.width - 40, Screen.height - 20, 20, 20), "=")){
			hour = now.Hour;
	}
}
// */

