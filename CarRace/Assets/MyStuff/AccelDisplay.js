#pragma strict

var vertAccel : float;
var horiAccel : float;

function Start () {

}

function Update () {

vertAccel = Input.acceleration.x;
horiAccel = Input.acceleration.y;

}

function OnGUI(){
	
	var accelText : String = String.Format("Accel: x={0}, y={1}", vertAccel, horiAccel);
	GUI.Label(new Rect(0, Screen.height-20, 200, Screen.height), accelText);  
}