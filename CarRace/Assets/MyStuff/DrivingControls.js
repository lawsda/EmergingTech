#pragma strict

var forwardSpeed : float = 30000;
var turnSpeed : float = 20;

function Start () {
}

function Update () {

	//keyboard input controls
	var forwardAmount = Input.GetAxis("Vertical") * forwardSpeed;
	var turnAmount = Input.GetAxis("Horizontal") * turnSpeed;
	
	//accelerometer controls
	//overrides keyboard input
	if(Input.acceleration.y > 0)
		forwardAmount = Mathf.Ceil(Input.acceleration.y) * forwardSpeed;
	else if(Input.acceleration.y < 0)
		forwardAmount = Mathf.Floor(Input.acceleration.y) * forwardSpeed;
		
	if(Input.acceleration.x != 0)
		turnAmount = Input.acceleration.x * turnSpeed;

	//cut speed by half when backing up
	if(forwardAmount < 0)
		forwardAmount *= .5;
	
	transform.Rotate(0,turnAmount,0);
	rigidbody.AddRelativeForce(0,0,forwardAmount);
	
}


