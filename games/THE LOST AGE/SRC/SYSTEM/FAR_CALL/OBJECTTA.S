@ Far-call stub table at 0x080c83a8: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080c83a8
	.thumb_func
Func_080c83a8:
	overlay_veneer Func_080d2a3c
	.global Func_080c83b0
	.thumb_func
Func_080c83b0:
	overlay_veneer Func_080d2a64
	.global Func_080c83b8
	.thumb_func
Func_080c83b8:
	overlay_veneer Func_080d2a8c
	.global Func_080c83c0
	.thumb_func
Func_080c83c0:
	overlay_veneer Func_080d6408
	overlay_veneer Func_080d64b8
	overlay_veneer Func_080cdc40
	.global Func_080c83d8
	.thumb_func
Func_080c83d8:
	overlay_veneer Func_080cf3b4
	.global Func_080c83e0
	.thumb_func
Func_080c83e0:
	overlay_veneer Func_080cf424
	overlay_veneer Func_080ceffc
	overlay_veneer Func_080cefb4
	overlay_veneer Func_080cef68
	overlay_veneer Func_080cefd0
	.global Func_080c8408
	.thumb_func
Func_080c8408:
	overlay_veneer Func_080cf00c
	overlay_veneer Func_080cf004
	overlay_veneer Func_080cf050
	.global Func_080c8420
	.thumb_func
Func_080c8420:
	overlay_veneer Func_080cee40
	.global Func_080c8428
	.thumb_func
Func_080c8428:
	overlay_veneer Func_080ceec8
	.global Func_080c8430
	.thumb_func
Func_080c8430:
	overlay_veneer Func_080cb82c
	.global Func_080c8438
	.thumb_func
Func_080c8438:
	overlay_veneer Func_080cb8a4
	overlay_veneer Func_080caa4c
	overlay_veneer Func_080ca6e8
	overlay_veneer Func_080caa2c
	.global ObjectTable_GetFar
	.thumb_func
ObjectTable_GetFar:
	overlay_veneer ObjectTable_Get
	overlay_veneer Func_080ca9cc
	overlay_veneer Func_080d1a18
	overlay_veneer Func_080d1ac4
	overlay_veneer Func_080d1ad4
	.global Func_080c8480
	.thumb_func
Func_080c8480:
	overlay_veneer Func_080d1cc0
	overlay_veneer Func_080d1d34
	overlay_veneer Func_080d1d44
	.global Func_080c8498
	.thumb_func
Func_080c8498:
	overlay_veneer Field_DispatchTypeHandler
	.global Func_080c84a0
	.thumb_func
Func_080c84a0:
	overlay_veneer Func_080dc410
	.global Func_080c84a8
	.thumb_func
Func_080c84a8:
	overlay_veneer Func_080dc62c
	.global Func_080c84b0
	.thumb_func
Func_080c84b0:
	overlay_veneer Func_080dc7cc
	.global Func_080c84b8
	.thumb_func
Func_080c84b8:
	overlay_veneer Func_080dc7e8
	.global Func_080c84c0
	.thumb_func
Func_080c84c0:
	overlay_veneer Func_080dc6d8
	overlay_veneer BattleEffect_InitializeSharedScene
	.global BattleFx_PrepareBufferInterpolationFar
	.thumb_func
BattleFx_PrepareBufferInterpolationFar:
	overlay_veneer BattleFx_PrepareBufferInterpolation
