#pragma strict

var startTime : float;
var elapsedTime : float;

var theRaceIsOn : boolean = false;
var p1InRace : boolean = false;
var p2InRace : boolean = false;

var player1 : GameObject = null;
var player2 : GameObject = null;

function Start () {
	startTime = 0; 
	elapsedTime = 0;  
}

function RaceIsOn(){
	startTime = Time.time;
	theRaceIsOn = true;
}

function OnTriggerEnter(other : Collider){
	//end race 
	if(p1InRace){
		if(other.transform.tag.Equals("Player1")){
			theRaceIsOn = false;
			player1.GetComponent(DrivingControls).enabled = false;
			if(player2 != null){
				player2.GetComponent(DrivingControls).enabled = false;
			}
			if(Network.peerType == NetworkPeerType.Client){
				this.gameObject.GetComponent(DefeatScreen).enabled = true;
			}
			else if(Network.peerType == NetworkPeerType.Server){
				this.gameObject.GetComponent(VictoryScreen).enabled = true;
			}
		}
	}
	if(p2InRace){
		if(other.transform.tag.Equals("Player2")){
			theRaceIsOn = false;
			player2.GetComponent(DrivingControls).enabled = false;
			if(player1 != null){
				player1.GetComponent(DrivingControls).enabled = false;
			}
			if(Network.peerType == NetworkPeerType.Server){
				this.gameObject.GetComponent(DefeatScreen).enabled = true;
			}
			else if(Network.peerType == NetworkPeerType.Client){
				this.gameObject.GetComponent(VictoryScreen).enabled = true;
			}
		}
	}
	//begin race
	if(!p1InRace){
		if(other.transform.tag.Equals("Player1")){
			p1InRace = true;
			player1 = other.gameObject;
		}
	}
	if(!p2InRace){
		if(other.transform.tag.Equals("Player2")){
			p2InRace = true;
			player2 = other.gameObject;
		}
	}
}

function Update () { 
	if(startTime != 0 && theRaceIsOn){
		elapsedTime = Time.time - startTime;
	}
}

function OnGUI(){
	
	//Timer display
	GUI.Box(new Rect(0,0,100,20), "");
	
	var min : int = elapsedTime / 60;
	var sec : int = elapsedTime % 60;
	var msec : int = elapsedTime * 1000;
	msec = msec % 1000;
	var timerText : String = String.Format("{0:00}:{1:00}:{2:000}", min, sec, msec);
    GUI.Label(new Rect(10, 0, 90, 20), timerText);   
}






