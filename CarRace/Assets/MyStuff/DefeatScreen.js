#pragma strict

function OnGUI(){

	Camera.main.gameObject.GetComponent(MenuScript).enabled = false;

	GUI.BeginGroup(Rect(Screen.width/2 - 100, Screen.height/2 - 60, 200, 120));
		
		GUI.Box(Rect(0, 0, 200, 160), "Defeat!");

			if( GUI.Button(new Rect(50, 30, 100, 30), "Restart")){
				Network.Disconnect(200);
				Application.LoadLevel("CarRace_v1.0");
			}
		
		if(GUI.Button(Rect(50, 70, 100, 30), "Quit")){
			Application.Quit();
		}

	GUI.EndGroup();
}