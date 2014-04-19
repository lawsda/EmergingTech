#pragma strict

var speed : float;
var moveDir : Vector3;
var myPosition: Vector3;
var receivedPosition: Vector3;


function Awake(){
	if(!networkView.isMine)
		enabled = false;
}

function Start () {
}

function Update () {
	if (networkView.isMine)
	{
	    moveDir = new Vector3(Input.GetAxis("Horizontal"), 0, Input.GetAxis("Vertical"));
	    speed = 5;
	    transform.Translate(speed * moveDir * Time.deltaTime);
	}
}

function OnSerializeNetworkView(stream : BitStream, info : NetworkMessageInfo)
{
    if (stream.isWriting)
	{
		myPosition = transform.position;
	    stream.Serialize(myPosition);
	}
	else
	{
		receivedPosition = Vector3.zero;
		stream.Serialize(receivedPosition);
		transform.position = receivedPosition;
	}
}