.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Resource_FarCall00C
Resource_FarCall00C:
	.global Func_08138000
	.thumb_func
Func_08138000:
	overlay_veneer Func_08196404
	.global Func_08138008
	.thumb_func
Func_08138008:
	overlay_veneer BattleFx_DispatchByIdRange
	.global BattleFx_RunSceneEffectFar
	.thumb_func
BattleFx_RunSceneEffectFar:
	overlay_veneer BattleFx_RunSceneEffect
	.global Func_08138018
	.thumb_func
Func_08138018:
	overlay_veneer BattleFx_RunTableEffect
	.global Func_08138020
	.thumb_func
Func_08138020:
	overlay_veneer Func_0813e3d0
	.global Func_08138028
	.thumb_func
Func_08138028:
	overlay_veneer Func_0813eb70
	.global Func_08138030
	.thumb_func
Func_08138030:
	overlay_veneer Func_0813f518
	.global Func_08138038
	.thumb_func
Func_08138038:
	overlay_veneer Func_0813fa88
	.global Func_08138040
	.thumb_func
Func_08138040:
	overlay_veneer BattleFx_EndElementStreaks
	.global Func_08138048
	.thumb_func
Func_08138048:
	overlay_veneer BattleFx_LoadBlitter
	.global Func_08138050
	.thumb_func
Func_08138050:
	overlay_veneer Func_08142944
