@ Far-call stub table at 0x08038390: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_08038390
	.thumb_func
Func_08038390:
	overlay_veneer Func_0804da3c
	.global Func_08038398
	.thumb_func
Func_08038398:
	overlay_veneer Func_08044460
	.global UiText_MeasureResourceEntriesFar
	.thumb_func
UiText_MeasureResourceEntriesFar:
	overlay_veneer UiText_MeasureResourceEntries
	.global Func_080383a8
	.thumb_func
Func_080383a8:
	overlay_veneer Func_08046134
	.global Ability_LoadGlyphFar
	.thumb_func
Ability_LoadGlyphFar:
	overlay_veneer Ability_LoadGlyph
	.global Func_080383b8
	.thumb_func
Func_080383b8:
	overlay_veneer Func_080453d0
	.global Func_080383c0
	.thumb_func
Func_080383c0:
	overlay_veneer Func_08041c0c
	.global Menu_AnimateSelectionToEntryFar
	.thumb_func
Menu_AnimateSelectionToEntryFar:
	overlay_veneer Menu_AnimateSelectionToEntry
