.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global m4aSoundVSyncOffFar
	.thumb_func
m4aSoundVSyncOffFar:
	overlay_veneer Sound_SuspendDirectSound
	.global Func_081c0088
	.thumb_func
Func_081c0088:
	overlay_veneer Sound_UpdateBgmRamps
	.global SoundFlag_TestFar
	.thumb_func
SoundFlag_TestFar:
	overlay_veneer SoundFlag_Test
	.global SoundFlag_SetFar
	.thumb_func
SoundFlag_SetFar:
	overlay_veneer SoundFlag_Set
	.global SoundFlag_ClearFar
	.thumb_func
SoundFlag_ClearFar:
	overlay_veneer SoundFlag_Clear
	.global Func_081c00a8
	.thumb_func
Func_081c00a8:
	overlay_veneer Func_081c0c18
