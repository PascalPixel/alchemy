@ Far-call stub table at 0x081c0040: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_081c0040
	.thumb_func
Func_081c0040:
	overlay_veneer Func_081c0fac
	.global Audio_Check
	.thumb_func
Audio_Check:
	overlay_veneer Func_081c0fd0
	.global Func_081c0050
	.thumb_func
Func_081c0050:
	overlay_veneer Func_081c1014
	.global Func_081c0058
	.thumb_func
Func_081c0058:
	overlay_veneer Func_081c0fc0
	.global Func_081c0060
	.thumb_func
Func_081c0060:
	overlay_veneer Func_081c0fc8
	.global AudioCommand_UpdateToggleMaskFar
	.thumb_func
AudioCommand_UpdateToggleMaskFar:
	overlay_veneer AudioCommand_UpdateToggleMask
