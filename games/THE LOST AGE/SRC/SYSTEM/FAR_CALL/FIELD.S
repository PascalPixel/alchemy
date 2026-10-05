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
.ifdef TLA_EDITION_ES
	overlay_veneer Func_080cdf5c
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Func_080cdf5c
.else
	overlay_veneer EventRuntime_GetControlledOwner
.endif
.endif
	.global Func_080c8780
	.thumb_func
Func_080c8780:
	overlay_veneer Func_080db444
	.global Func_080c8788
	.thumb_func
Func_080c8788:
	overlay_veneer Func_080daecc
	.global Func_080c8790
	.thumb_func
Func_080c8790:
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
.ifdef TLA_EDITION_ES
	overlay_veneer BattleFx_StepVector2
.else
.ifdef TLA_EDITION_IT
	overlay_veneer BattleFx_StepVector2
.else
	overlay_veneer Func_080eb01c
.endif
.endif
	.global Func_080c87d0
	.thumb_func
Func_080c87d0:
.ifdef TLA_EDITION_ES
	overlay_veneer BattleFx_StepVector3
.else
.ifdef TLA_EDITION_IT
	overlay_veneer BattleFx_StepVector3
.else
	overlay_veneer Func_080eb298
.endif
.endif
	.global Func_080c87d8
	.thumb_func
Func_080c87d8:
.ifdef TLA_EDITION_ES
	overlay_veneer Func_080eb2c8
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Func_080eb2c8
.else
	overlay_veneer BattleFx_StepVector2
.endif
.endif
	.global Func_080c87e0
	.thumb_func
Func_080c87e0:
.ifdef TLA_EDITION_ES
	overlay_veneer Func_080eb2d0
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Func_080eb2d0
.else
	overlay_veneer BattleFx_StepVector3
.endif
.endif
	.global Func_080c87e8
	.thumb_func
Func_080c87e8:
	overlay_veneer Func_080db9c0
	.ifdef TLA_EDITION_ES
	.global Field_BeginPaletteTransitionFar
	.thumb_func
Field_BeginPaletteTransitionFar:
	overlay_veneer Field_BeginPaletteTransition
	.else
	.ifdef TLA_EDITION_IT
	.global Field_BeginPaletteTransitionFar
	.thumb_func
Field_BeginPaletteTransitionFar:
	overlay_veneer Field_BeginPaletteTransition
	.else
	.global Func_080c87f0
	.thumb_func
Func_080c87f0:
	overlay_veneer Func_080db9cc
	.endif
	.endif
	.global Func_080c87f8
	.thumb_func
Func_080c87f8:
.ifdef TLA_EDITION_ES
	overlay_veneer BattleFx_StartArcEffect
.else
.ifdef TLA_EDITION_IT
	overlay_veneer BattleFx_StartArcEffect
.else
	overlay_veneer Func_080dc1b0
.endif
.endif
	.ifdef TLA_EDITION_ES
	.global Field_EndActorSpriteEffectFar
	.thumb_func
Field_EndActorSpriteEffectFar:
	overlay_veneer BattleFx_EndArcEffect
	.else
	.ifdef TLA_EDITION_IT
	.global Field_EndActorSpriteEffectFar
	.thumb_func
Field_EndActorSpriteEffectFar:
	overlay_veneer BattleFx_EndArcEffect
	.else
	.global Field_BeginPaletteTransitionFar
	.thumb_func
Field_BeginPaletteTransitionFar:
	overlay_veneer Field_BeginPaletteTransition
	.endif
	.endif
	.global Func_080c8808
	.thumb_func
Func_080c8808:
.ifdef TLA_EDITION_ES
	overlay_veneer BattleFx_WaitForArcGroups
.else
.ifdef TLA_EDITION_IT
	overlay_veneer BattleFx_WaitForArcGroups
.else
	overlay_veneer BattleFx_StartArcEffect
.endif
.endif
	.global Func_080c8810
	.thumb_func
Func_080c8810:
.ifdef TLA_EDITION_ES
	overlay_veneer BattleFx_SetObjectTickMode
.else
.ifdef TLA_EDITION_IT
	overlay_veneer BattleFx_SetObjectTickMode
.else
	overlay_veneer BattleFx_EndArcEffect
.endif
.endif
	.global Func_080c8818
	.thumb_func
Func_080c8818:
.ifdef TLA_EDITION_ES
	overlay_veneer Func_080e137c
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Func_080e137c
.else
	overlay_veneer BattleFx_WaitForArcGroups
.endif
.endif
	.global Func_080c8820
	.thumb_func
Func_080c8820:
.ifdef TLA_EDITION_ES
	overlay_veneer Func_080e1420
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Func_080e1420
.else
	overlay_veneer BattleFx_SetObjectTickMode
.endif
.endif
