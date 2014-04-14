#pragma strict
 
var turned = false;
var turnedRight = false;
var turnedLeft = false;


function Start () {

}

function Update () {

	var turnDir = Input.GetAxis("Horizontal");
	
	if(!turned && turnDir > 0){
		transform.Rotate(0,10,0);
		turned = true;
		turnedRight = true;
	}
	else if(!turned && turnDir < 0){
		transform.Rotate(0,-10,0);
		turned = true;
		turnedLeft = true;
	}
	else if(turned && turnDir == 0){
		if(turnedRight){
			transform.Rotate(0,-10,0);
			turnedRight = false;
		}
		else if(turnedLeft){
			transform.Rotate(0,10,0);
			turnedLeft = false;
		}
		turned = false;
	}	
}


