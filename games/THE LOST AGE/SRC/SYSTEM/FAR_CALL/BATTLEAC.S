@ Far-call stub table at 0x08118118: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_08118118
	.thumb_func
Func_08118118:
	overlay_veneer BattleActor_CommitPlacement
	overlay_veneer Func_0811b724
	overlay_veneer Func_08120060
	overlay_veneer Func_08123574
	overlay_veneer Func_0811a0b0
	overlay_veneer Func_0811a39c
	overlay_veneer Func_0811b75c
	overlay_veneer Func_08119734
	overlay_veneer Func_08118f6c
