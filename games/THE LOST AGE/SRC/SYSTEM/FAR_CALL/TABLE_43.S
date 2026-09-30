@ Far-call stub table at 0x08118010: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_08118010
	.thumb_func
Func_08118010:
	overlay_veneer BattleActor_SpawnObjectsForList
	.global Func_08118018
	.thumb_func
Func_08118018:
	overlay_veneer Func_08126944
	.global Func_08118020
	.thumb_func
Func_08118020:
	overlay_veneer Func_08126978
