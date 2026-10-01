@ Far-call stub table at 0x080c8828: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080c8828
	.thumb_func
Func_080c8828:
	overlay_veneer Func_080e4244
	.global Func_080c8830
	.thumb_func
Func_080c8830:
	overlay_veneer Func_080dbdc8
	.global Func_080c8838
	.thumb_func
Func_080c8838:
	overlay_veneer Func_080dbde8
	.global Func_080c8840
	.thumb_func
Func_080c8840:
	overlay_veneer Func_080dbdd4
	.global Func_080c8848
	.thumb_func
Func_080c8848:
	overlay_veneer Func_080dbdf4
	.global Func_080c8850
	.thumb_func
Func_080c8850:
	overlay_veneer Func_080dbe08
	.global Func_080c8858
	.thumb_func
Func_080c8858:
	overlay_veneer Func_080dbe44
	.global Func_080c8860
	.thumb_func
Func_080c8860:
	overlay_veneer Func_080d3b28
	.global Func_080c8868
	.thumb_func
Func_080c8868:
	overlay_veneer Func_080da908
	.global Func_080c8870
	.thumb_func
Func_080c8870:
	overlay_veneer Func_080da938
	.global Func_080c8878
	.thumb_func
Func_080c8878:
	overlay_veneer Func_080d4ab4
	.global Func_080c8880
	.thumb_func
Func_080c8880:
	overlay_veneer Func_080e3698
	.global Func_080c8888
	.thumb_func
Func_080c8888:
	overlay_veneer Func_080e37e0
	.global Func_080c8890
	.thumb_func
Func_080c8890:
	overlay_veneer Func_080e70f8
	.global Func_080c8898
	.thumb_func
Func_080c8898:
	overlay_veneer Func_080d2cc4
	.global Func_080c88a0
	.thumb_func
Func_080c88a0:
	overlay_veneer Func_080d1d54
	.global Func_080c88a8
	.thumb_func
Func_080c88a8:
	overlay_veneer Func_080d1dac
	.global Func_080c88b0
	.thumb_func
Func_080c88b0:
	overlay_veneer Func_080d1df0
	.global Func_080c88b8
	.thumb_func
Func_080c88b8:
	ldr r4, [pc, #0]
	bx r4
	.ifdef TLA_EDITION_ES
	.global BattleFx_DecodeFrameTranslationBase
BattleFx_DecodeFrameTranslationBase:
	.endif
	.ifdef TLA_EDITION_IT
	.global BattleFx_DecodeFrameTranslationBase
BattleFx_DecodeFrameTranslationBase:
	.endif
	.4byte Func_080d1e18
	.global ObjectGroup_ApplyRandomChildValuesFar
	.thumb_func
ObjectGroup_ApplyRandomChildValuesFar:
	overlay_veneer ObjectGroup_ApplyRandomChildValues
