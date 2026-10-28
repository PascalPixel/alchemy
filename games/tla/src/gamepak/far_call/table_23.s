.syntax unified
	.thumb
	.include "games/tla/src/gamepak/far_call/veneer.s"
	.global Unit_GetExperienceForLevelFar
	.thumb_func
Unit_GetExperienceForLevelFar:
	overlay_veneer Unit_GetExperienceForLevel
	.global Func_080ad208
	.thumb_func
Func_080ad208:
	overlay_veneer Func_080ae16c
	.global Func_080ad210
	.thumb_func
Func_080ad210:
	overlay_veneer Func_080ae220
	.global Party_SetLeaderJennaFar
	.thumb_func
Party_SetLeaderJennaFar:
	overlay_veneer Party_SetLeaderJenna
	.global Party_SetLeaderFelixFar
	.thumb_func
Party_SetLeaderFelixFar:
	overlay_veneer Party_SetLeaderFelix
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
