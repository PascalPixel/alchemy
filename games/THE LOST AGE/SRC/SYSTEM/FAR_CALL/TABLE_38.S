@ Far-call stub table at 0x080f8000: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Resource_FarCall007
Resource_FarCall007:
	.global Func_080f8000
	.thumb_func
Func_080f8000:
	overlay_veneer Func_080f94a4
	.global Menu_OpenConfirmPromptFar
	.thumb_func
Menu_OpenConfirmPromptFar:
	overlay_veneer Func_080fcab8
	.global Func_080f8010
	.thumb_func
Func_080f8010:
	overlay_veneer Func_080fe274
	.global Func_080f8018
	.thumb_func
Func_080f8018:
	overlay_veneer Func_080fe184
	.global UiIcon_CreateWithResourceVariantFar
	.thumb_func
UiIcon_CreateWithResourceVariantFar:
	overlay_veneer UiIcon_CreateWithResourceVariant
