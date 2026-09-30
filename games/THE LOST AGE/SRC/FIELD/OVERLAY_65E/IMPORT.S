@ Import veneers after the OVERLAY_65E overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_65E overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Func_020005a4
	.thumb_func
Func_020005a4:
	overlay_veneer Func_080003c8
	.global Func_020005ac
	.thumb_func
Func_020005ac:
	overlay_veneer Func_080003d0
	.global Func_020005b4
	.thumb_func
Func_020005b4:
	overlay_veneer Func_080003d8
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_020005c4
	.thumb_func
Func_020005c4:
	overlay_veneer Func_080c8018
	.global Func_020005cc
	.thumb_func
Func_020005cc:
	overlay_veneer Func_080c8020
	.global Inventory_PromptAndSetObjectMode
	.thumb_func
Inventory_PromptAndSetObjectMode:
	overlay_veneer Inventory_PromptAndSetObjectModeFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global Func_020005e4
	.thumb_func
Func_020005e4:
	overlay_veneer Func_080c80f8
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global Func_020005f4
	.thumb_func
Func_020005f4:
	overlay_veneer Func_080c8158
	.global Func_020005fc
	.thumb_func
Func_020005fc:
	overlay_veneer Func_080c8170
	.global Func_02000604
	.thumb_func
Func_02000604:
	overlay_veneer Func_080c8180
	.global Func_0200060c
	.thumb_func
Func_0200060c:
	overlay_veneer Func_080c8188
	.global Func_02000614
	.thumb_func
Func_02000614:
	overlay_veneer Func_080c81a0
	.global Func_0200061c
	.thumb_func
Func_0200061c:
	overlay_veneer Func_080c8200
	.global Func_02000624
	.thumb_func
Func_02000624:
	overlay_veneer Func_080c8210
	.global Func_0200062c
	.thumb_func
Func_0200062c:
	overlay_veneer Func_080c84e0
	.global Func_02000634
	.thumb_func
Func_02000634:
	overlay_veneer Func_080c8778
	.global Func_0200063c
	.thumb_func
Func_0200063c:
	overlay_veneer Func_08108008
	.global Func_02000644
	.thumb_func
Func_02000644:
	overlay_veneer Func_08108010
	.global Func_0200064c
	.thumb_func
Func_0200064c:
	overlay_veneer Func_08108018
