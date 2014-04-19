﻿#pragma strict

var player2Prefab : Transform;
var player1Prefab : Transform;
var playerScripts : Array = new Array();

function OnServerInitialized(){
	SpawnPlayer(Network.player);
}

function OnPlayerConnected(player : NetworkPlayer){
var tempPlayerString = player.ToString();
	var playerNumber = parseInt(tempPlayerString);	

	if(playerNumber < 2)
		SpawnPlayer(player);
}

function OnNetworkInstantiate(info : NetworkMessageInfo){
	if(networkView.isMine){
		Camera.main.GetComponent(SmoothFollow).target = transform;
	}
}

function SpawnPlayer(player : NetworkPlayer){
	var tempPlayerString = player.ToString();
	var playerNumber = parseInt(tempPlayerString);	
	
	var playerPrefab = player1Prefab;
	if(playerNumber == 1)
		playerPrefab = player2Prefab;
	 
	var startPos = new Vector3(155, 1.4, -110);
	if(playerNumber == 1)
		startPos = new Vector3(165, 1.4, -110);

		var newPlayerTransform = Network.Instantiate(playerPrefab, startPos, transform.rotation, playerNumber);
		playerScripts.Add(newPlayerTransform.GetComponent("DrivingControls"));
	//	Camera.main.GetComponent(SmoothFollow).target = newPlayerTransform;
		
		var theNetworkView = newPlayerTransform.networkView;
		theNetworkView.RPC("SetPlayer", RPCMode.AllBuffered, player);
}