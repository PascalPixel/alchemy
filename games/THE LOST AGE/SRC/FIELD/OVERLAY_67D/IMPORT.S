@ Import veneers after the OVERLAY_67D overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_67D overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Func_02002194
	.thumb_func
Func_02002194:
	overlay_veneer Func_080003c8
	.global Func_0200219c
	.thumb_func
Func_0200219c:
	overlay_veneer Func_080003d0
	.global Func_020021a4
	.thumb_func
Func_020021a4:
	overlay_veneer Func_080003d8
	.global PartyInventory_FindOwner
	.thumb_func
PartyInventory_FindOwner:
	overlay_veneer PartyInventory_FindOwnerFar
	.global PartyInventory_Remove
	.thumb_func
PartyInventory_Remove:
	overlay_veneer PartyInventory_RemoveFar
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_020021c4
	.thumb_func
Func_020021c4:
	overlay_veneer Func_080c8018
	.global Func_020021cc
	.thumb_func
Func_020021cc:
	overlay_veneer Func_080c8020
	.global Inventory_PromptAndSetObjectMode
	.thumb_func
Inventory_PromptAndSetObjectMode:
	overlay_veneer Inventory_PromptAndSetObjectModeFar
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global ObjectMotion_SetSpeedParameters
	.thumb_func
ObjectMotion_SetSpeedParameters:
	overlay_veneer ObjectMotion_SetSpeedParametersFar
	.global ObjectMotion_ResetAndSetPosition
	.thumb_func
ObjectMotion_ResetAndSetPosition:
	overlay_veneer ObjectMotion_ResetAndSetPositionFar
	.global ObjectMotion_ResetAndSetPositionInMode2
	.thumb_func
ObjectMotion_ResetAndSetPositionInMode2:
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2Far
	.global ObjectMotion_SetPositionAndReset
	.thumb_func
ObjectMotion_SetPositionAndReset:
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	.global ObjectMotion_OffsetPositionAndResetMotion
	.thumb_func
ObjectMotion_OffsetPositionAndResetMotion:
	overlay_veneer ObjectMotion_OffsetPositionAndResetMotionFar
	.global ObjectMotion_CommitCurrentPositionAndActivate
	.thumb_func
ObjectMotion_CommitCurrentPositionAndActivate:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivateFar
	.global Func_02002214
	.thumb_func
Func_02002214:
	overlay_veneer Func_080c80f8
	.global Object_SetModeById
	.thumb_func
Object_SetModeById:
	overlay_veneer Object_SetModeByIdFar
	.global ObjectMotion_Launch
	.thumb_func
ObjectMotion_Launch:
	overlay_veneer ObjectMotion_LaunchFar
	.global ObjectMotion_SetVariantCallback
	.thumb_func
ObjectMotion_SetVariantCallback:
	overlay_veneer ObjectMotion_SetVariantCallbackFar
	.global Func_02002234
	.thumb_func
Func_02002234:
	overlay_veneer Func_080c8158
	.global Object_LinkPair
	.thumb_func
Object_LinkPair:
	overlay_veneer Object_LinkPairFar
	.global Func_02002244
	.thumb_func
Func_02002244:
	overlay_veneer Func_080c8180
	.global Func_0200224c
	.thumb_func
Func_0200224c:
	overlay_veneer Func_080c8188
	.global Func_02002254
	.thumb_func
Func_02002254:
	overlay_veneer Func_080c81a0
	.global Func_0200225c
	.thumb_func
Func_0200225c:
	overlay_veneer Func_080c81d0
	.global Func_02002264
	.thumb_func
Func_02002264:
	overlay_veneer Func_080c8200
	.global Func_0200226c
	.thumb_func
Func_0200226c:
	overlay_veneer Func_080c8210
	.global Func_02002274
	.thumb_func
Func_02002274:
	overlay_veneer Func_080c8218
	.global Func_0200227c
	.thumb_func
Func_0200227c:
	overlay_veneer Func_080c8228
	.global Func_02002284
	.thumb_func
Func_02002284:
	overlay_veneer Func_080c8238
	.global Func_0200228c
	.thumb_func
Func_0200228c:
	overlay_veneer Func_080c8498
	.global Func_02002294
	.thumb_func
Func_02002294:
	overlay_veneer Func_080c84a0
	.global Func_0200229c
	.thumb_func
Func_0200229c:
	overlay_veneer Func_080c84a8
	.global Func_020022a4
	.thumb_func
Func_020022a4:
	overlay_veneer Func_080c84b0
	.global Func_020022ac
	.thumb_func
Func_020022ac:
	overlay_veneer Func_080c84b8
	.global Func_020022b4
	.thumb_func
Func_020022b4:
	overlay_veneer Func_080c84c0
	.global Func_020022bc
	.thumb_func
Func_020022bc:
	overlay_veneer Func_080c84e0
	.global Func_020022c4
	.thumb_func
Func_020022c4:
	overlay_veneer Func_080c8580
	.global Func_020022cc
	.thumb_func
Func_020022cc:
	overlay_veneer Func_080c85e8
	.global ObjectMotion_OffsetPositionAndReset
	.thumb_func
ObjectMotion_OffsetPositionAndReset:
	overlay_veneer ObjectMotion_OffsetPositionAndResetFar
	.global Func_020022dc
	.thumb_func
Func_020022dc:
	overlay_veneer Func_080c8778
	.global Func_020022e4
	.thumb_func
Func_020022e4:
	overlay_veneer Func_080c8808
	.global Func_020022ec
	.thumb_func
Func_020022ec:
	overlay_veneer Func_080c8810
	.global Func_020022f4
	.thumb_func
Func_020022f4:
	overlay_veneer Func_080c8818
	.global Func_020022fc
	.thumb_func
Func_020022fc:
	overlay_veneer Func_080c8820
	.global Func_02002304
	.thumb_func
Func_02002304:
	overlay_veneer Func_08108008
	.global Func_0200230c
	.thumb_func
Func_0200230c:
	overlay_veneer Func_08108010
	.global Func_02002314
	.thumb_func
Func_02002314:
	overlay_veneer Func_08108018
	.global Func_0200231c
	.thumb_func
Func_0200231c:
	overlay_veneer Audio_PlayCue
