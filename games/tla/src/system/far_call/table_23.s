.syntax unified
	.thumb
	.include "games/tla/src/system/overlay.inc"
	.global Owner_GetLevelThresholdFar
	.thumb_func
Owner_GetLevelThresholdFar:
	overlay_veneer Owner_GetLevelThreshold
	.global Func_080ad208
	.thumb_func
Func_080ad208:
	overlay_veneer Func_080ae16c
	.global Func_080ad210
	.thumb_func
Func_080ad210:
	overlay_veneer Func_080ae220
	.global Party_SetLeaderJasmineFar
	.thumb_func
Party_SetLeaderJasmineFar:
	overlay_veneer Party_SetLeaderJasmine
	.global Party_SetLeaderGarciaFar
	.thumb_func
Party_SetLeaderGarciaFar:
	overlay_veneer Party_SetLeaderGarcia
	.global Func_080ad228
	.thumb_func
Func_080ad228:
	overlay_veneer Func_080ae118
	.global Func_080ad230
	.thumb_func
Func_080ad230:
	overlay_veneer Func_080ae830
	.global Func_080ad238
	.thumb_func
Func_080ad238:
	overlay_veneer Func_080ae410
	.global Func_080ad240
	.thumb_func
Func_080ad240:
	overlay_veneer Func_080ae6e4
