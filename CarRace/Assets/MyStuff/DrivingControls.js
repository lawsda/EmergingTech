#pragma strict

var forwardSpeed : float = 300;
var turnSpeed : float = 2;

function Start () {

}

function Update () {

	var forwardAmount = Input.GetAxis("Vertical") * forwardSpeed;
	var turnAmount = Input.GetAxis("Horizontal") * turnSpeed;
	
	transform.Rotate(0,turnAmount,0);
	rigidbody.AddRelativeForce(forwardAmount,0,0);
	
}