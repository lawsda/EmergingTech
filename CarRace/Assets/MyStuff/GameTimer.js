#pragma strict

var startTime : float;
var elapsedTime : float;
var fastestLap : float;

var theRaceIsOn : boolean = false;

function Start () {
	startTime = 0; 
	elapsedTime = 0;  
	fastestLap = float.MaxValue;
}

function OnTriggerEnter(other : Collider){
	//begin race
	if(!theRaceIsOn){
		if(other.transform.tag.Equals("Player")){
			startTime = Time.time;
			theRaceIsOn = true;
		}
	}
	//end race 
	else{
		if(other.transform.tag.Equals("Player")){
			//end race
			theRaceIsOn = false;
			other.gameObject.GetComponent(DrivingControls).enabled = false;
			//new lap
			//startTime = Time.time;
			
			this.gameObject.GetComponent(VictoryScreen).enabled = true;

			if(elapsedTime <= fastestLap)
			fastestLap = elapsedTime;
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
    
    //Fastest Lap display
    /*
    GUI.Box(new Rect(0, 20, 100, 40), "");
    var lapLabel: String = String.Format("Fastest Lap: ");
    var lapTime: String;
    if(fastestLap == float.MaxValue){
    	lapTime = String.Format("");
    }
    else{
    	min = fastestLap / 60;
    	sec = fastestLap % 60;
    	msec = (fastestLap * 1000) % 1000;
    	lapTime = String.Format("{0:00}:{1:00}:{2:000}", min, sec, msec);
    }
    GUI.Label(new Rect(5, 20, 90, 20), lapLabel);
    GUI.Label(new Rect(10, 40, 90, 20), lapTime);
    */
}






