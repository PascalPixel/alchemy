@ Far-call stub table at 0x080c8088: fixed 8-byte veneers, each loading its target into r4 and
@ branching, which the calling convention permits. No compiler emits them; the
@ shared overlay veneer macro builds every entry from its target.
@ credit: reconstructed_veneer — main-image far-call stub table
.syntax unified
	.thumb
	.include "games/THE LOST AGE/SRC/SYSTEM/OVERLAY.INC"
	.global Func_080c8088
	.thumb_func
Func_080c8088:
	overlay_veneer Object_GetById
	.global Func_080c8090
	.thumb_func
Func_080c8090:
	overlay_veneer Func_080d36a8
	.global Func_080c8098
	.thumb_func
Func_080c8098:
	overlay_veneer ObjectMotion_SetSpeedParameters
	.global Func_080c80a0
	.thumb_func
Func_080c80a0:
	overlay_veneer ObjectMotion_EnableActionAndSetCallback
	.global Func_080c80a8
	.thumb_func
Func_080c80a8:
	overlay_veneer Object_RefreshSelectorById
	.global Func_080c80b0
	.thumb_func
Func_080c80b0:
	overlay_veneer ObjectMotion_EnableActionAndResetMotion
	.global Func_080c80b8
	.thumb_func
Func_080c80b8:
	overlay_veneer Object_SetActionCallbackAndRefreshById
	.global Func_080c80c0
	.thumb_func
Func_080c80c0:
	overlay_veneer ObjectMotion_ResetAndSetPosition
	.global Func_080c80c8
	.thumb_func
Func_080c80c8:
	overlay_veneer ObjectMotion_SetPositionAndCommit
	.global Func_080c80d0
	.thumb_func
Func_080c80d0:
	overlay_veneer ObjectMotion_ResetAndSetPositionInMode2
	.global Func_080c80d8
	.thumb_func
Func_080c80d8:
	overlay_veneer ObjectMotion_SetPositionAndReset
	.global Func_080c80e0
	.thumb_func
Func_080c80e0:
	overlay_veneer ObjectMotion_SnapHeadingAndOffset
	.global Func_080c80e8
	.thumb_func
Func_080c80e8:
	overlay_veneer ObjectMotion_OffsetPositionAndResetMotion
	.global Func_080c80f0
	.thumb_func
Func_080c80f0:
	overlay_veneer ObjectMotion_CommitCurrentPositionAndActivate
	.global Func_080c80f8
	.thumb_func
Func_080c80f8:
	overlay_veneer Func_080d3118
	.global Func_080c8100
	.thumb_func
Func_080c8100:
	overlay_veneer Func_080d3164
	overlay_veneer Func_080d31c0
	.global Func_080c8110
	.thumb_func
Func_080c8110:
	overlay_veneer Func_080d3214
	.global Func_080c8118
	.thumb_func
Func_080c8118:
	overlay_veneer Object_SetModeById
	.global Func_080c8120
	.thumb_func
Func_080c8120:
	overlay_veneer Object_SetActionById
	.global Func_080c8128
	.thumb_func
Func_080c8128:
	overlay_veneer Motion_SetModeAndWaitAnimation
	.global Func_080c8130
	.thumb_func
Func_080c8130:
	overlay_veneer ObjectMotion_WaitForAnimationChange
	.global Func_080c8138
	.thumb_func
Func_080c8138:
	overlay_veneer ObjectMotion_Launch
	.global Func_080c8140
	.thumb_func
Func_080c8140:
	overlay_veneer ObjectMotion_SetVariantCallback
	.global Func_080c8148
	.thumb_func
Func_080c8148:
	overlay_veneer Motion_SetVarCbAndRefresh
	.global Func_080c8150
	.thumb_func
Func_080c8150:
	overlay_veneer Func_080d3460
	.global Func_080c8158
	.thumb_func
Func_080c8158:
	overlay_veneer Func_080d359c
	.global Func_080c8160
	.thumb_func
Func_080c8160:
	overlay_veneer Object_LinkPair
	.global Func_080c8168
	.thumb_func
Func_080c8168:
	overlay_veneer Func_080d36c8
	.global Func_080c8170
	.thumb_func
Func_080c8170:
	overlay_veneer Func_080d3744
	overlay_veneer Func_080d3780
	.global Func_080c8180
	.thumb_func
Func_080c8180:
	overlay_veneer Func_080d3be8
	.global Func_080c8188
	.thumb_func
Func_080c8188:
	overlay_veneer Func_080d3c88
	overlay_veneer Func_080d3fb0
	.global Func_080c8198
	.thumb_func
Func_080c8198:
	overlay_veneer Func_080d406c
	.global Func_080c81a0
	.thumb_func
Func_080c81a0:
	overlay_veneer Func_080d407c
	.global Func_080c81a8
	.thumb_func
Func_080c81a8:
	overlay_veneer Func_080d4084
	overlay_veneer Func_080d40d8
	overlay_veneer Func_080d40dc
	overlay_veneer Func_080d4178
	overlay_veneer Func_080d4180
	.global Func_080c81d0
	.thumb_func
Func_080c81d0:
	overlay_veneer Func_080d3838
	.global Func_080c81d8
	.thumb_func
Func_080c81d8:
	overlay_veneer Func_080d3860
	overlay_veneer Func_080d3814
	overlay_veneer ObjectTable_RunIfActive
	.global Func_080c81f0
	.thumb_func
Func_080c81f0:
	overlay_veneer Func_080d41fc
	overlay_veneer Func_080d4330
	.global Func_080c8200
	.thumb_func
Func_080c8200:
	overlay_veneer Func_080d3888
	.global Func_080c8208
	.thumb_func
Func_080c8208:
	overlay_veneer Func_080d390c
	.global Func_080c8210
	.thumb_func
Func_080c8210:
	overlay_veneer Func_080d47b4
	.global Func_080c8218
	.thumb_func
Func_080c8218:
	overlay_veneer Func_080d489c
	.global Func_080c8220
	.thumb_func
Func_080c8220:
	overlay_veneer Func_080d38d4
	.global Func_080c8228
	.thumb_func
Func_080c8228:
	overlay_veneer Func_080d4384
	.global Func_080c8230
	.thumb_func
Func_080c8230:
	overlay_veneer Func_080d43ec
	.global Func_080c8238
	.thumb_func
Func_080c8238:
	overlay_veneer Func_080d440c
	.global Func_080c8240
	.thumb_func
Func_080c8240:
	overlay_veneer Func_080d4548
	.global Func_080c8248
	.thumb_func
Func_080c8248:
	overlay_veneer Func_080d450c
	.global Func_080c8250
	.thumb_func
Func_080c8250:
	overlay_veneer Func_080d452c
	.global Func_080c8258
	.thumb_func
Func_080c8258:
	overlay_veneer Func_080d4568
	.global Func_080c8260
	.thumb_func
Func_080c8260:
	overlay_veneer Func_080d45b8
