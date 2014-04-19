#pragma strict

var theOwner : NetworkPlayer;
var lastClientHInput : float = 0f;
var lastClientVInput : float = 0f;
var serverCurrentHInput : float = 0f;
var serverCurrentVInput : float = 0f;

function Awake(){
	if(Network.isClient)
		enabled = false;
}

@RPC
function SetPlayer(player : NetworkPlayer){
	theOwner = player;
	if(player == Network.player)
		enabled = true;
}

function Update()
{
	if(theOwner != null && Network.player == theOwner)
	{
		var HInput = Input.GetAxis("Horizontal");
		var VInput = Input.GetAxis("Vertical");
		
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
			var moveDirection = new Vector3(serverCurrentHInput, 0, serverCurrentVInput);
			var speed = 5;
			transform.Translate(speed * moveDirection * Time.deltaTime);
		}	
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



