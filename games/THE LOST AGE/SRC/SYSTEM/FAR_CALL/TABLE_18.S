@ Far-call stub table at 0x08038328: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_08038328
	.thumb_func
Func_08038328:
	overlay_veneer Func_08044a58
	.global Party_ShowJoinedMessageFar
	.thumb_func
Party_ShowJoinedMessageFar:
	overlay_veneer Func_08044b98
	.global Party_ShowPairJoinedMessageFar
	.thumb_func
Party_ShowPairJoinedMessageFar:
	overlay_veneer Func_08044c80
	.global Func_08038340
	.thumb_func
Func_08038340:
	overlay_veneer Func_08044dc8
	.global Func_08038348
	.thumb_func
Func_08038348:
	overlay_veneer Func_0803f9c0
	overlay_veneer Func_080400e8
