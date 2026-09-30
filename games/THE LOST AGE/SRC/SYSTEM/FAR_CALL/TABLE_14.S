@ Far-call stub table at 0x080380f8: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080380f8
	.thumb_func
Func_080380f8:
	overlay_veneer Func_0803d3c0
	.global UiWork_FinalizeEntityMatchingLocalizedIdFar
	.thumb_func
UiWork_FinalizeEntityMatchingLocalizedIdFar:
	overlay_veneer Func_0803d450
	.global Func_08038108
	.thumb_func
Func_08038108:
	overlay_veneer Func_0803b880
	overlay_veneer Func_0803b8cc
	.global Func_08038118
	.thumb_func
Func_08038118:
	overlay_veneer Func_0803cca8
	.global Func_08038120
	.thumb_func
Func_08038120:
	overlay_veneer Func_0803ccd0
	overlay_veneer Func_08042690
	.global UiWindow_DrawPartyStatusContentsFar
	.thumb_func
UiWindow_DrawPartyStatusContentsFar:
	overlay_veneer Func_0804297c
	overlay_veneer Func_08042dac
	.global Func_08038140
	.thumb_func
Func_08038140:
	overlay_veneer Func_0803ce1c
	overlay_veneer Func_0803cb1c
	overlay_veneer Func_0803dc1c
	overlay_veneer Func_0803dd24
	overlay_veneer Func_0803dd68
	overlay_veneer Func_0803df00
	overlay_veneer Func_0803df14
	overlay_veneer Func_0803e5a8
	overlay_veneer Func_0803e6d8
	overlay_veneer Func_0803e7ac
	overlay_veneer Func_0803e918
	overlay_veneer Func_0803e998
	overlay_veneer Func_0803f610
	overlay_veneer Func_0803f614
	overlay_veneer Func_0803dd98
	overlay_veneer Func_0803dea8
