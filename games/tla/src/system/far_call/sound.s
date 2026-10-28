.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global m4aSoundVSyncOffFar
	.thumb_func
m4aSoundVSyncOffFar:
	overlay_veneer AudioEngine_SuspendDirectSoundWrapper
	.global Func_081c0088
	.thumb_func
Func_081c0088:
	overlay_veneer Sound_UpdateBgmRamps
	.global SongFlag_TestFar
	.thumb_func
SongFlag_TestFar:
	overlay_veneer SongFlag_Test
	.global SongFlag_SetFar
	.thumb_func
SongFlag_SetFar:
	overlay_veneer SongFlag_Set
	.global SongFlag_ClearFar
	.thumb_func
SongFlag_ClearFar:
	overlay_veneer SongFlag_Clear
	.global Func_081c00a8
	.thumb_func
Func_081c00a8:
	overlay_veneer Func_081c0c18
