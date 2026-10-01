@ Far-call stub table at 0x080382a0: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global BattleLayout_HighlightPartyPanelsFar
	.thumb_func
BattleLayout_HighlightPartyPanelsFar:
	overlay_veneer Func_080461c8
	.global Func_080382a8
	.thumb_func
Func_080382a8:
	overlay_veneer Func_0803d5c4
	.global Func_080382b0
	.thumb_func
Func_080382b0:
	overlay_veneer Func_0803d9bc
