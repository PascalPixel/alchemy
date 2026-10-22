.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Func_080c8770
	.thumb_func
Func_080c8770:
	overlay_veneer Func_080dbd48
	.global EventRuntime_GetControlledOwnerFar
	.thumb_func
EventRuntime_GetControlledOwnerFar:
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
.ifdef TLA_EDITION_ES
	overlay_veneer Func_080eaf98
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Func_080eaf98
.else
	overlay_veneer Func_080d3940
.endif
.endif
	.global Func_080c87b8
	.thumb_func
Func_080c87b8:
	overlay_veneer Func_080d3a10
	.global Func_080c87c0
	.thumb_func
Func_080c87c0:
.ifdef TLA_EDITION_ES
	overlay_veneer EffectSprite_DrawAtWorldPosition
.else
.ifdef TLA_EDITION_IT
	overlay_veneer EffectSprite_DrawAtWorldPosition
.else
	overlay_veneer Func_080eaf98
.endif
.endif
	.global Func_080c87c8
	.thumb_func
Func_080c87c8:
.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_StepVector2
.else
.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_StepVector2
.else
	overlay_veneer Func_080eb01c
.endif
.endif
	.global Func_080c87d0
	.thumb_func
Func_080c87d0:
.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_StepVector3
.else
.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_StepVector3
.else
	overlay_veneer EffectSprite_DrawAtWorldPosition
.endif
.endif
	.global BattleEffect_StepVector2Far
	.thumb_func
BattleEffect_StepVector2Far:
.ifdef TLA_EDITION_ES
	overlay_veneer Func_080eb2c8
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Func_080eb2c8
.else
	overlay_veneer BattleEffect_StepVector2
.endif
.endif
	.global BattleEffect_StepVector3Far
	.thumb_func
BattleEffect_StepVector3Far:
.ifdef TLA_EDITION_ES
	overlay_veneer Func_080eb2d0
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Func_080eb2d0
.else
	overlay_veneer BattleEffect_StepVector3
.endif
.endif
	.global Func_080c87e8
	.thumb_func
Func_080c87e8:
	overlay_veneer BattleEffect_GetObjectPriority
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
	overlay_veneer BattleEffect_GetObjectDepth
	.endif
	.endif
	.global Func_080c87f8
	.thumb_func
Func_080c87f8:
.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_StartArcEffect
.else
.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_StartArcEffect
.else
	overlay_veneer Field_BeginColorTransition
.endif
.endif
	.ifdef TLA_EDITION_ES
	.global Field_EndActorSpriteEffectFar
	.thumb_func
Field_EndActorSpriteEffectFar:
	overlay_veneer BattleEffect_EndArcEffect
	.else
	.ifdef TLA_EDITION_IT
	.global Field_EndActorSpriteEffectFar
	.thumb_func
Field_EndActorSpriteEffectFar:
	overlay_veneer BattleEffect_EndArcEffect
	.else
	.global Field_BeginPaletteTransitionFar
	.thumb_func
Field_BeginPaletteTransitionFar:
	overlay_veneer Field_BeginPaletteTransition
	.endif
	.endif
	.global BattleEffect_StartArcEffectFar
	.thumb_func
BattleEffect_StartArcEffectFar:
.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_WaitForArcGroups
.else
.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_WaitForArcGroups
.else
	overlay_veneer BattleEffect_StartArcEffect
.endif
.endif
	.global BattleEffect_EndArcEffectFar
	.thumb_func
BattleEffect_EndArcEffectFar:
.ifdef TLA_EDITION_ES
	overlay_veneer BattleEffect_SetObjectTickMode
.else
.ifdef TLA_EDITION_IT
	overlay_veneer BattleEffect_SetObjectTickMode
.else
	overlay_veneer BattleEffect_EndArcEffect
.endif
.endif
	.global BattleEffect_WaitForArcGroupsFar
	.thumb_func
BattleEffect_WaitForArcGroupsFar:
.ifdef TLA_EDITION_ES
	overlay_veneer Func_080e137c
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Func_080e137c
.else
	overlay_veneer BattleEffect_WaitForArcGroups
.endif
.endif
	.global BattleEffect_SetObjectTickModeFar
	.thumb_func
BattleEffect_SetObjectTickModeFar:
.ifdef TLA_EDITION_ES
	overlay_veneer Func_080e1420
.else
.ifdef TLA_EDITION_IT
	overlay_veneer Func_080e1420
.else
	overlay_veneer BattleEffect_SetObjectTickMode
.endif
.endif
