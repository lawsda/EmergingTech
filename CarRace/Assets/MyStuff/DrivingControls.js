#pragma strict

var theOwner : NetworkPlayer;
var lastClientHInput : float = 0f;
var lastClientVInput : float = 0f;
var serverCurrentHInput : float = 0f;
var serverCurrentVInput : float = 0f;
var forwardSpeed : float = 30000;
var turnSpeed : float = 20;

var singlePlayer : boolean = false;


function Awake(){
	if(Network.isClient)
		enabled = false;
}

function Update () {
	if(theOwner != null && Network.player == theOwner)
	{
		var VInput = Input.GetAxis("Vertical");
		var HInput = Input.GetAxis("Horizontal");
		if(lastClientHInput != HInput || lastClientVInput != VInput)
		{
			lastClientHInput = HInput;
			lastClientVInput = VInput;
		}
		if(Network.isServer)
		{
			SendMovementInput(HInput, VInput);
		}
		else if(Network.isClient)
		{
			networkView.RPC("SendMovementInput", RPCMode.Server, HInput, VInput);
		}
	}
	
	if(Network.isServer)
	{
		//keyboard input controls
		var forwardAmount = serverCurrentVInput * forwardSpeed;
		var turnAmount = serverCurrentHInput * turnSpeed;
	
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
	else if(singlePlayer){
	
		VInput = Input.GetAxis("Vertical");
		HInput = Input.GetAxis("Horizontal");
		//keyboard input controls
		forwardAmount = VInput * forwardSpeed;
		turnAmount = HInput * turnSpeed;
	
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
}

@RPC
function SetPlayer(player : NetworkPlayer){
	theOwner = player;
	if(player == Network.player)
		enabled = true;
}

@RPC
function SendMovementInput(HInput : float, VInput : float){
	serverCurrentHInput = HInput;
	serverCurrentVInput = VInput;
}

function OnSerializeNetworkView(stream : BitStream, info : NetworkMessageInfo){
	if (stream.isWriting)
    {
        var pos = transform.position;
        stream.Serialize(pos);
    }
    else
    {
        var posReceive = Vector3.zero;
        stream.Serialize(posReceive);
    	transform.position = posReceive;
    }
}




