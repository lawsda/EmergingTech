#pragma strict

var forwardSpeed : float = 30000;
var turnSpeed : float = 2.5;

function Start () {

}

function Update () {

	var forwardAmount = Input.GetAxis("Vertical") * forwardSpeed;
	var turnAmount = Input.GetAxis("Horizontal") * turnSpeed;
	
	transform.Rotate(0,turnAmount,0);
	rigidbody.AddRelativeForce(0,0,forwardAmount);
	
}