@ Import veneers after the DERI_MURA overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — DERI_MURA overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Engine_GameFlagIsSet
	.thumb_func
Engine_GameFlagIsSet:
	overlay_veneer Func_080003c8
	.global Engine_GameFlagSet
	.thumb_func
Engine_GameFlagSet:
	overlay_veneer Func_080003d0
	overlay_veneer Func_080201e8
	.global Engine_EventWait
	.thumb_func
Engine_EventWait:
	overlay_veneer Func_080c8010
	.global Engine_EventBegin
	.thumb_func
Engine_EventBegin:
	overlay_veneer Func_080c8018
	.global Engine_EventEnd
	.thumb_func
Engine_EventEnd:
	overlay_veneer Func_080c8020
	.global Engine_EventChooseYesNo
	.thumb_func
Engine_EventChooseYesNo:
	overlay_veneer Func_080c8070
	.global Engine_ActorGet
	.thumb_func
Engine_ActorGet:
	overlay_veneer Func_080c8088
	.global Engine_ActorSetSpeed
	.thumb_func
Engine_ActorSetSpeed:
	overlay_veneer Func_080c8098
	overlay_veneer Func_080c80c0
	overlay_veneer Func_080c80c8
	overlay_veneer Func_080c80d8
	.global Engine_ActorCenterAndWalk
	.thumb_func
Engine_ActorCenterAndWalk:
	overlay_veneer Func_080c80e0
	overlay_veneer Func_080c80f0
	overlay_veneer Func_080c80f8
	.global Engine_ActorSetAnimation
	.thumb_func
Engine_ActorSetAnimation:
	overlay_veneer Func_080c8118
	overlay_veneer Func_080c8138
	overlay_veneer Func_080c8140
	overlay_veneer Func_080c8148
	overlay_veneer Func_080c8158
	overlay_veneer Func_080c8160
	.global Engine_EventSetMessage
	.thumb_func
Engine_EventSetMessage:
	overlay_veneer Func_080c8180
	.global Engine_EventOpenMessage
	.thumb_func
Engine_EventOpenMessage:
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
	.global Engine_EventRequestExit
	.thumb_func
Engine_EventRequestExit:
	overlay_veneer Func_080c8278
	overlay_veneer Func_080c82e0
	.global Engine_EventCloseScreen
	.thumb_func
Engine_EventCloseScreen:
	overlay_veneer Func_080c83b0
	.global Engine_EventWaitForScreen
	.thumb_func
Engine_EventWaitForScreen:
	overlay_veneer Func_080c83b8
	overlay_veneer Func_080c8420
	.global Engine_ActorReturnHome
	.thumb_func
Engine_ActorReturnHome:
	overlay_veneer Func_080c8428
	.global Engine_EventPrepareSpeakers
	.thumb_func
Engine_EventPrepareSpeakers:
	overlay_veneer Func_080c84e0
	overlay_veneer Func_080c85e8
	overlay_veneer Func_080c86f8
	overlay_veneer Func_080c8708
	.global Engine_PartyGetLeaderActor
	.thumb_func
Engine_PartyGetLeaderActor:
	overlay_veneer Func_080c8778
	overlay_veneer Func_080c87b0
	.global Engine_ShopOpen
	.thumb_func
Engine_ShopOpen:
	overlay_veneer Func_08108008
	.global Engine_AudioPlayCue
	.thumb_func
Engine_AudioPlayCue:
	overlay_veneer Audio_PlayCue
