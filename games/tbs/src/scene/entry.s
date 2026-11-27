@ The entry table of a scene overlay: six stub at the start of its
@ code, which the main image calls through to reach the scene. Each scene
@ defines these six functions, and its overlay links this table against them.
	.syntax unified
	.thumb
	.balign 4
	.include "games/tbs/src/system/stub.inc"
	unnamed_stub scInit
	unnamed_stub scGetEntrances
	unnamed_stub scGetExits
	unnamed_stub scGetActorPlacements
	unnamed_stub scGetCues
	unnamed_stub scGetExitRegions
