@ Import veneers after the OVERLAY_6AC overlay's code: fixed 8-byte veneers
@ through which the overlay calls main-image code, each loading its target
@ into r4 and branching, which the calling convention permits.
@ credit: reconstructed_veneer — OVERLAY_6AC overlay import veneers
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.balign 4
	.global Engine_MathDivide
	.thumb_func
Engine_MathDivide:
	overlay_veneer IwramSignedDivide
	.global Engine_MathRemainder
	.thumb_func
Engine_MathRemainder:
	overlay_veneer IwramSignedRemainder
	.global WaitFrames
	.thumb_func
WaitFrames:
	overlay_veneer WaitFramesFar
	.global Func_020030b8
	.thumb_func
Func_020030b8:
	overlay_veneer Func_080000d0
	.global Scheduler_RemoveCallbackFar
	.thumb_func
Scheduler_RemoveCallbackFar:
	overlay_veneer Func_080000d8
	.global Random16Far
	.thumb_func
Random16Far:
	overlay_veneer Func_080000f8
	.global Math_Sine
	.thumb_func
Math_Sine:
	overlay_veneer Math_SineFar
	.global Math_Cosine
	.thumb_func
Math_Cosine:
	overlay_veneer Math_CosineFar
	.global Vector_AddPolarOffsetFar
	.thumb_func
Vector_AddPolarOffsetFar:
	overlay_veneer Func_08000128
	.global Func_020030e8
	.thumb_func
Func_020030e8:
	overlay_veneer Func_080001b8
	.global Owner_GetState
	.thumb_func
Owner_GetState:
	overlay_veneer Owner_GetStateFar
	.global Func_020030f8
	.thumb_func
Func_020030f8:
	overlay_veneer Func_080003c8
	.global Func_02003100
	.thumb_func
Func_02003100:
	overlay_veneer Func_080003d0
	.global Func_02003108
	.thumb_func
Func_02003108:
	overlay_veneer Func_080003d8
	.global Func_02003110
	.thumb_func
Func_02003110:
	overlay_veneer Func_08020070
	.global Func_02003118
	.thumb_func
Func_02003118:
	overlay_veneer Object_SetMode
	.global Func_02003120
	.thumb_func
Func_02003120:
	overlay_veneer Object_SetCallback
	.global Func_02003128
	.thumb_func
Func_02003128:
	overlay_veneer Func_080200c0
	.global Func_02003130
	.thumb_func
Func_02003130:
	overlay_veneer Func_080200c8
	.global Object_SetPositionAndResetMotion
	.thumb_func
Object_SetPositionAndResetMotion:
	overlay_veneer Object_SetPositionAndResetMotionFar
	.global Func_02003140
	.thumb_func
Func_02003140:
	overlay_veneer Object_SetPosition
	.global Func_02003148
	.thumb_func
Func_02003148:
	overlay_veneer Func_08020178
	.global Func_02003150
	.thumb_func
Func_02003150:
	overlay_veneer Func_08020198
	.global Func_02003158
	.thumb_func
Func_02003158:
	overlay_veneer Func_080201b0
	.global Map_GetTerrainHeight
	.thumb_func
Map_GetTerrainHeight:
	overlay_veneer Map_GetTerrainHeightFar
	.global Func_02003168
	.thumb_func
Func_02003168:
	overlay_veneer Func_080201d8
	.global Func_02003170
	.thumb_func
Func_02003170:
	overlay_veneer Func_080201e8
	.global Func_02003178
	.thumb_func
Func_02003178:
	overlay_veneer Func_080201f0
	.global Func_02003180
	.thumb_func
Func_02003180:
	overlay_veneer Func_08020210
	.global ObjectDispatch_SetSingleChildField26
	.thumb_func
ObjectDispatch_SetSingleChildField26:
	overlay_veneer ObjectDispatch_SetSingleChildField26Far
	.global Animation_SetStateFlags
	.thumb_func
Animation_SetStateFlags:
	overlay_veneer Animation_SetStateFlagsFar
	.global Func_02003198
	.thumb_func
Func_02003198:
	overlay_veneer Func_08020228
	.global Animation_ApplyChildValues
	.thumb_func
Animation_ApplyChildValues:
	overlay_veneer Animation_ApplyChildValuesFar
	.global Func_020031a8
	.thumb_func
Func_020031a8:
	overlay_veneer Func_08020290
	.global Func_020031b0
	.thumb_func
Func_020031b0:
	overlay_veneer Func_08020348
	.global Func_020031b8
	.thumb_func
Func_020031b8:
	overlay_veneer Func_08020390
	.global Func_020031c0
	.thumb_func
Func_020031c0:
	overlay_veneer Func_08038210
	.global PartyInventory_FindOwner
	.thumb_func
PartyInventory_FindOwner:
	overlay_veneer PartyInventory_FindOwnerFar
	.global Party_CountActiveOwners
	.thumb_func
Party_CountActiveOwners:
	overlay_veneer Party_CountActiveOwnersFar
	.global Battle_WaitMode0
	.thumb_func
Battle_WaitMode0:
	overlay_veneer Battle_WaitMode0Far
	.global Func_020031e0
	.thumb_func
Func_020031e0:
	overlay_veneer Func_080c8018
	.global Func_020031e8
	.thumb_func
Func_020031e8:
	overlay_veneer Func_080c8020
	.global Object_GetById
	.thumb_func
Object_GetById:
	overlay_veneer Object_GetByIdFar
	.global Func_020031f8
	.thumb_func
Func_020031f8:
	overlay_veneer Func_080c80f8
	.global Func_02003200
	.thumb_func
Func_02003200:
	overlay_veneer Func_080c8150
	.global Func_02003208
	.thumb_func
Func_02003208:
	overlay_veneer Func_080c8170
	.global Func_02003210
	.thumb_func
Func_02003210:
	overlay_veneer Func_080c8218
	.global Func_02003218
	.thumb_func
Func_02003218:
	overlay_veneer Func_080c8230
	.global Func_02003220
	.thumb_func
Func_02003220:
	overlay_veneer Func_080c8238
	.global Func_02003228
	.thumb_func
Func_02003228:
	overlay_veneer Func_080c8240
	.global Func_02003230
	.thumb_func
Func_02003230:
	overlay_veneer Func_080c8248
	.global Func_02003238
	.thumb_func
Func_02003238:
	overlay_veneer Func_080c8278
	.global Func_02003240
	.thumb_func
Func_02003240:
	overlay_veneer Func_080c8350
	.global Func_02003248
	.thumb_func
Func_02003248:
	overlay_veneer Func_080c8378
	.global Func_02003250
	.thumb_func
Func_02003250:
	overlay_veneer Func_080c8380
	.global Func_02003258
	.thumb_func
Func_02003258:
	overlay_veneer Func_080c8390
	.global Event_SetStatus1c6
	.thumb_func
Event_SetStatus1c6:
	overlay_veneer Event_SetStatus1c6Far
	.global Event_WaitValue1c8Frames
	.thumb_func
Event_WaitValue1c8Frames:
	overlay_veneer Event_WaitValue1c8FramesFar
	.global Func_02003270
	.thumb_func
Func_02003270:
	overlay_veneer Func_080c83c0
	.global Func_02003278
	.thumb_func
Func_02003278:
	overlay_veneer Func_080c8480
	.global Func_02003280
	.thumb_func
Func_02003280:
	overlay_veneer Func_080c84e0
	.global Func_02003288
	.thumb_func
Func_02003288:
	overlay_veneer Func_080c8548
	.global Func_02003290
	.thumb_func
Func_02003290:
	overlay_veneer Func_080c8570
	.global Func_02003298
	.thumb_func
Func_02003298:
	overlay_veneer Func_080c8778
	.global Func_020032a0
	.thumb_func
Func_020032a0:
	overlay_veneer Func_080c88d8
	.global Func_020032a8
	.thumb_func
Func_020032a8:
	overlay_veneer Func_080c88e8
	.global Func_020032b0
	.thumb_func
Func_020032b0:
	overlay_veneer Func_080c8918
	.global Func_020032b8
	.thumb_func
Func_020032b8:
	overlay_veneer Audio_PlayCue
	.global Func_020032c0
	.thumb_func
Func_020032c0:
	overlay_veneer Func_081c0078
