@ Import veneers after the VINASU_CHOJO overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — VINASU_CHOJO overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	overlay_veneer IwramSignedDivide
	overlay_veneer Func_080000c0
	overlay_veneer Func_080000f8
	overlay_veneer Func_080003c8
	.global Engine_GameFlagSet
	.thumb_func
Engine_GameFlagSet:
	overlay_veneer Func_080003d0
	overlay_veneer Func_080003d8
	overlay_veneer Object_SetMode
	overlay_veneer Object_SetCallback
	overlay_veneer Func_080200c0
	overlay_veneer Object_SetPosition
	overlay_veneer Object_CommitPosition
	.global Engine_MapCopyCellsTo
	.thumb_func
Engine_MapCopyCellsTo:
	overlay_veneer Func_08020178
	.global Engine_MapCopyCellAttributes
	.thumb_func
Engine_MapCopyCellAttributes:
	overlay_veneer Func_080201e8
	overlay_veneer Func_08020210
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	overlay_veneer Func_08020220
	.global Engine_MessageShowCentered
	.thumb_func
Engine_MessageShowCentered:
	overlay_veneer Func_08038040
	overlay_veneer Func_080ad110
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
	.global Engine_ActorGet
	.thumb_func
Engine_ActorGet:
	overlay_veneer Func_080c8088
	overlay_veneer Func_080c8098
	overlay_veneer Func_080c80a0
	overlay_veneer Func_080c80a8
	overlay_veneer Func_080c80c0
	overlay_veneer Func_080c80d0
	overlay_veneer Func_080c80d8
	overlay_veneer Func_080c80f0
	overlay_veneer Func_080c80f8
	.global Engine_ActorSetAnimation
	.thumb_func
Engine_ActorSetAnimation:
	overlay_veneer Func_080c8118
	overlay_veneer Func_080c8128
	overlay_veneer Func_080c8140
	overlay_veneer Func_080c8148
	overlay_veneer Func_080c8160
	overlay_veneer Func_080c8170
	overlay_veneer Func_080c8180
	overlay_veneer Func_080c81a0
	overlay_veneer Func_080c81d0
	overlay_veneer Func_080c8210
	overlay_veneer Func_080c8228
	overlay_veneer Func_080c8238
	overlay_veneer Func_080c8240
	overlay_veneer Func_080c8278
	overlay_veneer Func_080c83a8
	overlay_veneer Func_080c83b0
	overlay_veneer Func_080c83b8
	overlay_veneer Func_080c8498
	overlay_veneer Func_080c84a0
	overlay_veneer Func_080c84a8
	overlay_veneer Func_080c84b0
	overlay_veneer Func_080c84b8
	overlay_veneer Func_080c84c0
	.global Engine_EventPrepareSpeakers
	.thumb_func
Engine_EventPrepareSpeakers:
	overlay_veneer Func_080c84e0
	overlay_veneer Func_080c85f8
	overlay_veneer Func_080c8600
	overlay_veneer Func_080c8690
	.global Engine_PartyGetLeaderActor
	.thumb_func
Engine_PartyGetLeaderActor:
	overlay_veneer Func_080c8778
	overlay_veneer Func_080c8798
	overlay_veneer Func_080c87a0
	.global Engine_AudioPlayCue
	.thumb_func
Engine_AudioPlayCue:
	overlay_veneer Audio_PlayCue
