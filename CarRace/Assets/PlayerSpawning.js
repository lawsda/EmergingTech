#pragma strict

var playerPrefab : Transform;
var playerPrefab1 : Transform;
var playerScripts : Array = new Array();

function OnServerInitialized(){
	SpawnPlayer(Network.player);
}

function OnPlayerConnected(player : NetworkPlayer){
	SpawnPlayer(player);
}

function SpawnPlayer(player : NetworkPlayer){
	var tempPlayerString = player.ToString();
	var playerNumber = parseInt(tempPlayerString);	
	
	var play = playerPrefab;
	if(playerNumber > 0 && playerNumber < 3)
		play = playerPrefab1;	
	
	var newPlayerTransform = Network.Instantiate(play, transform.position, transform.rotation, playerNumber);
	playerScripts.Add(newPlayerTransform.GetComponent("CubeMove"));
	
	var theNetworkView = newPlayerTransform.networkView;
	theNetworkView.RPC("SetPlayer", RPCMode.AllBuffered, player);
	
}