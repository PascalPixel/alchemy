@ Far-call stub table at 0x080c8770: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080c8770
	.thumb_func
Func_080c8770:
	overlay_veneer Func_080dbd48
	.global Func_080c8778
	.thumb_func
Func_080c8778:
	overlay_veneer Func_080cdf5c
	.global Func_080c8780
	.thumb_func
Func_080c8780:
	overlay_veneer Func_080db444
	overlay_veneer Func_080daecc
	overlay_veneer Func_080db0b0
	.global Func_080c8798
	.thumb_func
Func_080c8798:
	overlay_veneer Func_080d5d70
	.global Func_080c87a0
	.thumb_func
Func_080c87a0:
	overlay_veneer Func_080eb960
	.global Func_080c87a8
	.thumb_func
Func_080c87a8:
	overlay_veneer Func_080d3928
	.global Func_080c87b0
	.thumb_func
Func_080c87b0:
	overlay_veneer Func_080d3940
	.global Func_080c87b8
	.thumb_func
Func_080c87b8:
	overlay_veneer Func_080d3a10
	.global Func_080c87c0
	.thumb_func
Func_080c87c0:
	overlay_veneer Func_080eaf98
	.global Func_080c87c8
	.thumb_func
Func_080c87c8:
	overlay_veneer Func_080eb01c
	.global Func_080c87d0
	.thumb_func
Func_080c87d0:
	overlay_veneer Func_080eb298
	overlay_veneer Func_080eb2c8
	.global Func_080c87e0
	.thumb_func
Func_080c87e0:
	overlay_veneer Func_080eb2d0
	.global Func_080c87e8
	.thumb_func
Func_080c87e8:
	overlay_veneer Func_080db9c0
	.global Func_080c87f0
	.thumb_func
Func_080c87f0:
	overlay_veneer Func_080db9cc
	.global Func_080c87f8
	.thumb_func
Func_080c87f8:
	overlay_veneer Func_080dc1b0
	.global Field_BeginPaletteTransitionFar
	.thumb_func
Field_BeginPaletteTransitionFar:
	overlay_veneer Field_BeginPaletteTransition
	.global Func_080c8808
	.thumb_func
Func_080c8808:
	overlay_veneer Func_080e15fc
	.global Func_080c8810
	.thumb_func
Func_080c8810:
	overlay_veneer Func_080e1650
	.global Func_080c8818
	.thumb_func
Func_080c8818:
	overlay_veneer Func_080e137c
	.global Func_080c8820
	.thumb_func
Func_080c8820:
	overlay_veneer Func_080e1420
