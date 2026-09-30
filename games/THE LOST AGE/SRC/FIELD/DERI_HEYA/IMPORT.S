@ Import veneers after the DERI_HEYA overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — DERI_HEYA overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer Owner_GetStateFar
	overlay_veneer Func_080003c8
	overlay_veneer Func_080003d0
	overlay_veneer Func_080003d8
	overlay_veneer Object_ResetMotion
	overlay_veneer Func_080201a0
	overlay_veneer Func_080201e0
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	overlay_veneer Func_08020300
	overlay_veneer Func_08038040
	overlay_veneer Func_08038120
	overlay_veneer Func_08038140
	overlay_veneer Owner_RecalculateStatsFar
	overlay_veneer Item_Get
	overlay_veneer Inventory_GetQuantityFar
	overlay_veneer PartyInventory_AddFar
	overlay_veneer Inventory_DiscardFar
	overlay_veneer Func_080ad288
	overlay_veneer Inventory_CountItemFar
	overlay_veneer Func_080ad2f0
	overlay_veneer Battle_WaitMode0Far
	.global Engine_EventBegin
	.thumb_func
Engine_EventBegin:
	overlay_veneer Func_080c8018
	.global Engine_EventEnd
	.thumb_func
Engine_EventEnd:
	overlay_veneer Func_080c8020
	overlay_veneer Func_080c8070
	overlay_veneer Object_GetByIdFar
	overlay_veneer ObjectMotion_SetPositionAndResetFar
	overlay_veneer Func_080c80f8
	overlay_veneer Object_SetModeByIdFar
	overlay_veneer ObjectMotion_LaunchFar
	overlay_veneer Func_080c8158
	.global Engine_EventSetMessage
	.thumb_func
Engine_EventSetMessage:
	overlay_veneer Func_080c8180
	overlay_veneer Func_080c8188
	.global Engine_EventShowMessage
	.thumb_func
Engine_EventShowMessage:
	overlay_veneer Func_080c81a0
	overlay_veneer Func_080c81d0
	.global Engine_ActorSetSpritePriority
	.thumb_func
Engine_ActorSetSpritePriority:
	overlay_veneer Func_080c8200
	overlay_veneer Func_080c8210
	overlay_veneer Func_080c82e0
	overlay_veneer Func_080c83d8
	overlay_veneer Func_080c8480
	.global Engine_EventPrepareSpeakers
	.thumb_func
Engine_EventPrepareSpeakers:
	overlay_veneer Func_080c84e0
	overlay_veneer Func_080c8558
	overlay_veneer Func_080c86f8
	overlay_veneer Func_080c8708
	overlay_veneer Func_080c8778
	overlay_veneer Func_08108008
	overlay_veneer Func_08108010
	overlay_veneer Func_08108018
	overlay_veneer Func_08108068
	overlay_veneer Audio_PlayCue
