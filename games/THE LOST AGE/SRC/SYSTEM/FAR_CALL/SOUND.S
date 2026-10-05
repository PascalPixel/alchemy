@ Far-call stubs: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global AudioEngine_SuspendDirectSoundFar
	.thumb_func
AudioEngine_SuspendDirectSoundFar:
	overlay_veneer AudioEngine_SuspendDirectSoundWrapper
	.global Func_081c0088
	.thumb_func
Func_081c0088:
	overlay_veneer Func_081c0e30
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
