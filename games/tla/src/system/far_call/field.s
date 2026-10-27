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
	overlay_veneer EventRuntime_GetControlledOwner
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
	overlay_veneer Func_080eb01c
	.global Func_080c87d0
	.thumb_func
Func_080c87d0:
	overlay_veneer EffectSprite_DrawAtWorldPosition
	.global BattleEffect_StepVector2Far
	.thumb_func
BattleEffect_StepVector2Far:
	overlay_veneer BattleEffect_StepVector2
	.global BattleEffect_StepVector3Far
	.thumb_func
BattleEffect_StepVector3Far:
	overlay_veneer BattleEffect_StepVector3
	.global Func_080c87e8
	.thumb_func
Func_080c87e8:
	overlay_veneer BattleEffect_GetObjectPriority
	.global Func_080c87f0
	.thumb_func
Func_080c87f0:
	overlay_veneer BattleEffect_GetObjectDepth
	.global Func_080c87f8
	.thumb_func
Func_080c87f8:
	overlay_veneer Field_StartColorTransition
	.global Field_StartPaletteTransitionFar
	.thumb_func
Field_StartPaletteTransitionFar:
	overlay_veneer Field_StartPaletteTransition
	.global BattleEffect_StartArcEffectFar
	.thumb_func
BattleEffect_StartArcEffectFar:
	overlay_veneer BattleEffect_StartArcEffect
	.global BattleEffect_EndArcEffectFar
	.thumb_func
BattleEffect_EndArcEffectFar:
	overlay_veneer BattleEffect_EndArcEffect
	.global BattleEffect_WaitForArcGroupsFar
	.thumb_func
BattleEffect_WaitForArcGroupsFar:
	overlay_veneer BattleEffect_WaitForArcGroups
	.global BattleEffect_SetObjectUpdateModeFar
	.thumb_func
BattleEffect_SetObjectUpdateModeFar:
	overlay_veneer BattleEffect_SetObjectUpdateMode
