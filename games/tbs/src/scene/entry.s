@ The entry table of a scene overlay: six far-call stubs at the start of its
@ code, which the main image calls through to reach the scene. Each scene
@ defines these six routines, and its overlay links this table against them.
@ A scene whose routines are named otherwise has its own table in entry/.
	.syntax unified
	.thumb
	.balign 4
	.include "games/tbs/src/gamepak/far_call/veneer.s"
	overlay_veneer Scene_Initialize
	overlay_veneer Scene_GetEntrances
	overlay_veneer Scene_GetExits
	overlay_veneer Scene_GetActorPlacements
	overlay_veneer Scene_GetTriggers
	overlay_veneer Scene_GetExitRegions
