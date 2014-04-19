#pragma strict

var cubePrefab : Transform;
var myTransform : Transform;
 
function OnServerInitialized()
{
    SpawnPlayer();
}

function OnConnectedToServer()
{
    SpawnPlayer();
}

function SpawnPlayer()
{
	myTransform = Network.Instantiate(cubePrefab, transform.position, transform.rotation, 0);
}

