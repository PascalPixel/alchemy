	.section .rodata
	.balign 4
@ カメラ追従
	.global Object_CameraFollowScript
Object_CameraFollowScript:
	.4byte 11, 0, 1, 12, 65535, 0, 16
	.global gObject_IdleScript
gObject_IdleScript:
	.4byte 16
@ 場面の配置 {絵, 条件, 動作, x, y, z, 向き, 話す向き, フラグ}
	.global gField_DefaultPlacements
gField_DefaultPlacements:
	.2byte 0, -1
	.4byte gObject_IdleScript, 300 << 16, 0, 300 << 16
	.2byte 0x5bb0
	.byte 0, 0
	.2byte -1, 0
	.4byte 0, 0, 0, 0
	.2byte 0x0000
	.byte 0, 0
@ 画面切り替えの形 {first, second, {数, 値, x0, x1, y0, y1} x 3}
	.global gDisplayTransition_Shapes
gDisplayTransition_Shapes:
	.2byte 1, 32639
	.2byte 160, 65535
	.byte 0, 0, 0, 0
	.2byte 0, 0
	.byte 0, 0, 0, 0
	.2byte 0, 0
	.byte 0, 0, 0, 0
	.2byte 1, 32639
	.2byte 80, 0
	.byte 120, 120, 134, 120
	.2byte 80, 65535
	.byte 0, 0, 0, 0
	.2byte 0, 0
	.byte 0, 0, 0, 0
	.2byte 1, 32639
	.2byte 80, 0
	.byte 120, 120, 150, 120
	.2byte 80, 65535
	.byte 0, 0, 0, 0
	.2byte 0, 0
	.byte 0, 0, 0, 0
	.2byte 1, 32639
	.2byte 80, 0
	.byte 120, 120, 167, 120
	.2byte 80, 65535
	.byte 0, 0, 0, 0
	.2byte 0, 0
	.byte 0, 0, 0, 0
	.2byte 1, 32639
	.2byte 80, 0
	.byte 120, 120, 188, 120
	.2byte 80, 65535
	.byte 0, 0, 0, 0
	.2byte 0, 0
	.byte 0, 0, 0, 0
	.2byte 1, 32639
	.2byte 80, 0
	.byte 120, 120, 217, 120
	.2byte 80, 65535
	.byte 0, 0, 0, 0
	.2byte 0, 0
	.byte 0, 0, 0, 0
	.2byte 1, 32639
	.2byte 36, 30960
	.byte 0, 0, 0, 0
	.2byte 44, 0
	.byte 120, 120, 240, 120
	.2byte 80, 65535
	.byte 0, 0, 0, 0
	.2byte 1, 32639
	.2byte 59, 30960
	.byte 0, 0, 0, 0
	.2byte 21, 0
	.byte 120, 120, 240, 120
	.2byte 80, 65535
	.byte 0, 0, 0, 0
	.2byte 1, 32639
	.2byte 80, 30960
	.byte 0, 0, 0, 0
	.2byte 80, 65535
	.byte 0, 0, 0, 0
	.2byte 0, 0
	.byte 0, 0, 0, 0
	.2byte 1, 32639
	.2byte 80, 30960
	.byte 0, 0, 0, 0
	.2byte 21, 0
	.byte 120, 240, 240, 240
	.2byte 59, 65535
	.byte 0, 0, 0, 0
	.2byte 1, 32639
	.2byte 80, 30960
	.byte 0, 0, 0, 0
	.2byte 44, 0
	.byte 120, 240, 240, 240
	.2byte 36, 65535
	.byte 0, 0, 0, 0
	.2byte 1, 32639
	.2byte 80, 30960
	.byte 0, 0, 0, 0
	.2byte 80, 0
	.byte 120, 217, 240, 240
	.2byte 0, 0
	.byte 0, 0, 0, 0
	.2byte 1, 32639
	.2byte 80, 30960
	.byte 0, 0, 0, 0
	.2byte 80, 0
	.byte 120, 188, 240, 240
	.2byte 0, 0
	.byte 0, 0, 0, 0
	.2byte 1, 32639
	.2byte 80, 30960
	.byte 0, 0, 0, 0
	.2byte 80, 0
	.byte 120, 167, 240, 240
	.2byte 0, 0
	.byte 0, 0, 0, 0
	.2byte 1, 32639
	.2byte 80, 30960
	.byte 0, 0, 0, 0
	.2byte 80, 0
	.byte 120, 150, 240, 240
	.2byte 0, 0
	.byte 0, 0, 0, 0
	.2byte 1, 32639
	.2byte 80, 30960
	.byte 0, 0, 0, 0
	.2byte 80, 0
	.byte 120, 134, 240, 240
	.2byte 0, 0
	.byte 0, 0, 0, 0
	.2byte 1, 32639
	.2byte 160, 30960
	.byte 0, 0, 0, 0
	.2byte 0, 0
	.byte 0, 0, 0, 0
	.2byte 0, 0
	.byte 0, 0, 0, 0
	.2byte 127, 257
	.2byte 80, 120
	.byte 0, 0, 0, 0
	.2byte 80, 0
	.byte 0, 0, 120, 106
	.2byte 0, 0
	.byte 0, 0, 0, 0
	.2byte 127, 257
	.2byte 80, 120
	.byte 0, 0, 0, 0
	.2byte 80, 0
	.byte 0, 0, 120, 90
	.2byte 0, 0
	.byte 0, 0, 0, 0
	.2byte 127, 257
	.2byte 80, 120
	.byte 0, 0, 0, 0
	.2byte 80, 0
	.byte 0, 0, 120, 73
	.2byte 0, 0
	.byte 0, 0, 0, 0
	.2byte 127, 257
	.2byte 80, 120
	.byte 0, 0, 0, 0
	.2byte 80, 0
	.byte 0, 0, 120, 52
	.2byte 0, 0
	.byte 0, 0, 0, 0
	.2byte 127, 257
	.2byte 80, 120
	.byte 0, 0, 0, 0
	.2byte 80, 0
	.byte 0, 0, 120, 23
	.2byte 0, 0
	.byte 0, 0, 0, 0
	.2byte 127, 257
	.2byte 80, 120
	.byte 0, 0, 0, 0
	.2byte 44, 0
	.byte 0, 0, 120, 0
	.2byte 36, 65535
	.byte 0, 0, 0, 0
	.2byte 127, 257
	.2byte 80, 120
	.byte 0, 0, 0, 0
	.2byte 21, 0
	.byte 0, 0, 120, 0
	.2byte 59, 65535
	.byte 0, 0, 0, 0
	.2byte 127, 257
	.2byte 80, 120
	.byte 0, 0, 0, 0
	.2byte 80, 65535
	.byte 0, 0, 0, 0
	.2byte 0, 0
	.byte 0, 0, 0, 0
	.2byte 127, 257
	.2byte 59, 120
	.byte 0, 0, 0, 0
	.2byte 21, 0
	.byte 0, 120, 120, 120
	.2byte 80, 65535
	.byte 0, 0, 0, 0
	.2byte 127, 257
	.2byte 36, 120
	.byte 0, 0, 0, 0
	.2byte 44, 0
	.byte 0, 120, 120, 120
	.2byte 80, 65535
	.byte 0, 0, 0, 0
	.2byte 127, 257
	.2byte 80, 0
	.byte 23, 120, 120, 120
	.2byte 80, 65535
	.byte 0, 0, 0, 0
	.2byte 0, 0
	.byte 0, 0, 0, 0
	.2byte 127, 257
	.2byte 80, 0
	.byte 52, 120, 120, 120
	.2byte 80, 65535
	.byte 0, 0, 0, 0
	.2byte 0, 0
	.byte 0, 0, 0, 0
	.2byte 127, 257
	.2byte 80, 0
	.byte 73, 120, 120, 120
	.2byte 80, 65535
	.byte 0, 0, 0, 0
	.2byte 0, 0
	.byte 0, 0, 0, 0
	.2byte 127, 257
	.2byte 80, 0
	.byte 90, 120, 120, 120
	.2byte 80, 65535
	.byte 0, 0, 0, 0
	.2byte 0, 0
	.byte 0, 0, 0, 0
	.2byte 127, 257
	.2byte 80, 0
	.byte 106, 120, 120, 120
	.2byte 80, 65535
	.byte 0, 0, 0, 0
	.2byte 0, 0
	.byte 0, 0, 0, 0
	.global DisplayTransition_ShapesTail
DisplayTransition_ShapesTail:
	.byte 0, 2, 4, 2
	.byte 0, 2, 4, 2
	.byte 0, 2, 4, 2
	.global gObjectMotion_LaunchScript
gObjectMotion_LaunchScript:
	.4byte 1, 0, 1, 28, 1, 34, ObjectMotion_StepAngle, 16
	.global gBattleFx_BurstParticleScriptA
gBattleFx_BurstParticleScriptA:
	.4byte 0, 1, 12, 38, 0, 27
	.global gBattleFx_BurstParticleScriptB
gBattleFx_BurstParticleScriptB:
	.4byte 0, 1, 12, 18, 0, 27
	.global gObjectMotion_StepAngleScript
gObjectMotion_StepAngleScript:
	.4byte 34, ObjectMotion_StepAngle, 16
	.global gUi_RenderResultValues
gUi_RenderResultValues:
	.byte 1, 3, 10, 0
	.global gBattleFx_LinkedObjectScript
gBattleFx_LinkedObjectScript:
	.4byte 22, 9, 0xffff8000, 22, 10, 16384, 0, 2
	.4byte 22, 9, 16384, 22, 10, 0xffffe000, 0, 2
	.4byte 22, 9, 8192, 22, 10, 0xfffff000, 0, 2
	.4byte 22, 9, 8192, 22, 10, 0xfffff000, 0, 2
	.4byte 22, 9, 4096, 22, 10, 0xfffff800, 0, 2
	.4byte 22, 9, 0xfffff000, 22, 10, 2048, 0, 2
	.4byte 22, 9, 0xfffff800, 22, 10, 1024, 0, 2
	.4byte 22, 9, 2048, 22, 10, 0xfffffc00, 0, 2
	.4byte 0, 30, 27
	.global gObjectMotion_WaitScript
gObjectMotion_WaitScript:
	.4byte 0, 40, 27
	.global gObjectMotion_ActionKind2Script
gObjectMotion_ActionKind2Script:
	.4byte 21, 15, 32768, 21, 16, 3276, 21, 30
	.4byte 131, 0x80010000, 28, 2, 9, 262144, 524288, 1048576
	.4byte 1, 28, 1, 0, 20, 28, 2, 9
	.4byte 262144, 524288, 1048576, 1, 28, 1, 0, 20
	.4byte 28, 2, 9, 262144, 524288, 1048576, 1, 28
	.4byte 1, 0, 60, 12, 65535, 0xc0010000, 16
	.global gObjectMotion_ActionKind1Script
gObjectMotion_ActionKind1Script:
	.4byte 16
	.global gObjectMotion_ResetActionScript
gObjectMotion_ResetActionScript:
	.4byte 34, Object_ClearChildAndCollisionFlags, 16
	.global gObjectMotion_ActionKind3Script
gObjectMotion_ActionKind3Script:
	.4byte 21, 15, 32768, 21, 16, 3276, 21, 30
	.4byte 131, 0x80010000, 28, 2, 9, 131072, 131072, 524288
	.4byte 1, 28, 1, 0, 20, 28, 2, 9
	.4byte 131072, 131072, 524288, 1, 28, 1, 0, 20
	.4byte 28, 2, 9, 131072, 131072, 524288, 1, 28
	.4byte 1, 0, 60, 12, 65535, 0xc0010000, 16
	.global gObjectMotion_ActionKind4Script
gObjectMotion_ActionKind4Script:
	.4byte 21, 15, 26214, 21, 16, 13107, 28, 2
	.4byte 0x80010000, 9, 131072, 131072, 393216, 1, 0, 4
	.4byte 12, 65535, 0xc0010000
	.global gObjectMotion_MoveTowardTargetScript
gObjectMotion_MoveTowardTargetScript:
	.4byte 34, ObjectMotion_MoveTowardTarget, 12, 65535, 0
	.global gObjectMotion_TurnTowardLinkedScript
gObjectMotion_TurnTowardLinkedScript:
	.4byte 34, ObjectMotion_TurnTowardLinkedTarget, 12, 65535, 0
	.global gObjectMotion_LinkedActionScript
gObjectMotion_LinkedActionScript:
	.4byte 5, 0, 1, 12, 65535, 0
@ 地面の粒, 光の粒, きらめき
	.global gFieldFx_GroundParticleTiles
gFieldFx_GroundParticleTiles:
	.incbin "games/common/src/field/common/effect/ground.4bpp16x16.plz"
	.global gFieldFx_MoteTiles
gFieldFx_MoteTiles:
	.incbin "games/tbs/src/object/mote.4bpp16x16.plz"
	.global gFieldFx_SparkleTiles
gFieldFx_SparkleTiles:
	.incbin "games/common/src/field/common/effect/sparkle.4bpp16x16.plz"
	.balign 4, 0
	.global gBattleFx_TargetColors
gBattleFx_TargetColors:
	.4byte 2113668, 2101764, 2101392, 2114052
	.4byte 2113680, 2101776, 2105608, 2114064
	.global gBattleFx_UntargetedObjectScript
gBattleFx_UntargetedObjectScript:
	.4byte 27
	.global gEffectScripts
gEffectScripts:
	.4byte gBattleFx_ShortEffectScript, gBattleFx_MediumEffectScript, gBattleFx_LongEffectScript
@ {タイル << 16 | フラグ, 地名}
	.global gWorldMap_PlaceMarkers
gWorldMap_PlaceMarkers:
	.4byte 0x10183, 3
	.4byte 0x10185, 20
	.4byte 0x10199, 75
	.4byte 0x101b3, 167
	.4byte 0x101a2, 104
	.4byte 0x101a4, 107
	.4byte 0x101a6, 112
	.4byte 0x10191, 50
	.4byte 0x1018b, 36
	.4byte 0x10197, 72
	.4byte 0x1019b, 88
	.4byte 0x10194, 60
	.4byte 0x10188, 30
	.4byte 0x101a0, 99
	.4byte 0x101b7, 178
	.4byte 0x101a8, 135
	.4byte 0x20193, 53
	.4byte 0x20184, 11
	.4byte 0x201b9, 183
	.4byte 0x1018a, 35
	.4byte 0x201b6, 174
	.4byte 0x0, 0
	.4byte 0x0, 0
	.4byte 0x0, 0
	.4byte 0x0, 0
	.4byte 0x0, 0
	.4byte 0x0, 0
	.4byte 0x0, 0
	.4byte 0x0, 0
	.4byte 0x0, 0
	.4byte 0x0, 0
	.4byte 0x0, 0
	.4byte 0x0, 0
	.4byte 0x0, 0
	.4byte 0x0, 0
	.4byte 0x0, 0
	.4byte 0x0, 0
	.4byte 0x0, 0
	.4byte 0x0, 0
	.4byte 0x1018c, 39
	.4byte 0x101ac, 152
	.4byte -1, 0
