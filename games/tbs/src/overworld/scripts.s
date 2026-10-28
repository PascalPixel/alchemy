	.section .rodata
	.balign 4
@ The camera follows its target.
	.global Actor_CameraFollowScript
Actor_CameraFollowScript:
	.4byte 11, 0, 1, 12, 65535, 0, 16
	.global gField_LeaderScript
gField_LeaderScript:
	.4byte 16
@ The party leader's actor placement table (struct ActorPlacement): {sprite,
@ condition, behavior, x, y, z, facing, talk facing, flags}
	.global gField_DefaultActorPlacements
gField_DefaultActorPlacements:
	.2byte 0, -1
	.4byte gField_LeaderScript, 300 << 16, 0, 300 << 16
	.2byte 0x5bb0
	.byte 0, 0
	.2byte -1, 0
	.4byte 0, 0, 0, 0
	.2byte 0x0000
	.byte 0, 0
@ Screen transition shapes: {first, second, {count, value, x0, x1, y0, y1} x 3}
	.global gScreenTransition_Shapes
gScreenTransition_Shapes:
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
	.global ScreenTransition_ShapesTail
ScreenTransition_ShapesTail:
	.byte 0, 2, 4, 2
	.byte 0, 2, 4, 2
	.byte 0, 2, 4, 2
	.global gActor_LaunchScript
gActor_LaunchScript:
	.4byte 1, 0, 1, 28, 1, 34, Actor_StepAngle, 16
	.global gFieldEffect_BurstParticleScriptA
gFieldEffect_BurstParticleScriptA:
	.4byte 0, 1, 12, 38, 0, 27
	.global gFieldEffect_BurstParticleScriptB
gFieldEffect_BurstParticleScriptB:
	.4byte 0, 1, 12, 18, 0, 27
	.global gActor_StepAngleScript
gActor_StepAngleScript:
	.4byte 34, Actor_StepAngle, 16
	.global gMessageSpeed_VoiceRates
gMessageSpeed_VoiceRates:
	.byte 1, 3, 10, 0
	.global gFieldEffect_LinkedActorScript
gFieldEffect_LinkedActorScript:
	.4byte 22, 9, 0xffff8000, 22, 10, 16384, 0, 2
	.4byte 22, 9, 16384, 22, 10, 0xffffe000, 0, 2
	.4byte 22, 9, 8192, 22, 10, 0xfffff000, 0, 2
	.4byte 22, 9, 8192, 22, 10, 0xfffff000, 0, 2
	.4byte 22, 9, 4096, 22, 10, 0xfffff800, 0, 2
	.4byte 22, 9, 0xfffff000, 22, 10, 2048, 0, 2
	.4byte 22, 9, 0xfffff800, 22, 10, 1024, 0, 2
	.4byte 22, 9, 2048, 22, 10, 0xfffffc00, 0, 2
	.4byte 0, 30, 27
	.global gActor_WaitScript
gActor_WaitScript:
	.4byte 0, 40, 27
	.global gMovementType_WanderScript
gMovementType_WanderScript:
	.4byte 21, 15, 32768, 21, 16, 3276, 21, 30
	.4byte 131, 0x80010000, 28, 2, 9, 262144, 524288, 1048576
	.4byte 1, 28, 1, 0, 20, 28, 2, 9
	.4byte 262144, 524288, 1048576, 1, 28, 1, 0, 20
	.4byte 28, 2, 9, 262144, 524288, 1048576, 1, 28
	.4byte 1, 0, 60, 12, 65535, 0xc0010000, 16
	.global gMovementType_StandScript
gMovementType_StandScript:
	.4byte 16
	.global gMovementType_ResetActionScript
gMovementType_ResetActionScript:
	.4byte 34, Actor_ClearChildAndCollisionFlags, 16
	.global gMovementType_WanderNearScript
gMovementType_WanderNearScript:
	.4byte 21, 15, 32768, 21, 16, 3276, 21, 30
	.4byte 131, 0x80010000, 28, 2, 9, 131072, 131072, 524288
	.4byte 1, 28, 1, 0, 20, 28, 2, 9
	.4byte 131072, 131072, 524288, 1, 28, 1, 0, 20
	.4byte 28, 2, 9, 131072, 131072, 524288, 1, 28
	.4byte 1, 0, 60, 12, 65535, 0xc0010000, 16
	.global gMovementType_WanderCloseScript
gMovementType_WanderCloseScript:
	.4byte 21, 15, 26214, 21, 16, 13107, 28, 2
	.4byte 0x80010000, 9, 131072, 131072, 393216, 1, 0, 4
	.4byte 12, 65535, 0xc0010000
	.global gMovementType_FollowLinkedScript
gMovementType_FollowLinkedScript:
	.4byte 34, Actor_MoveTowardTarget, 12, 65535, 0
	.global gMovementType_FaceLeaderScript
gMovementType_FaceLeaderScript:
	.4byte 34, Actor_TurnTowardLinkedActor, 12, 65535, 0
	.global gActor_LinkedScript
gActor_LinkedScript:
	.4byte 5, 0, 1, 12, 65535, 0
@ Ground particles, motes of light and sparkles
	.global gFieldEffect_GroundParticleTiles
gFieldEffect_GroundParticleTiles:
	.incbin "games/tbs/src/field/shared/effect/ground.4bpp16x16.plz"
	.global gFieldEffect_MoteTiles
gFieldEffect_MoteTiles:
	.incbin "games/tbs/src/overworld/mote.4bpp16x16.plz"
	.global gFieldEffect_SparkleTiles
gFieldEffect_SparkleTiles:
	.incbin "games/tbs/src/field/shared/effect/sparkle.4bpp16x16.plz"
	.balign 4, 0
	.global gFieldEffect_TargetColors
gFieldEffect_TargetColors:
	.4byte 2113668, 2101764, 2101392, 2114052
	.4byte 2113680, 2101776, 2105608, 2114064
	.global gFieldEffect_UntargetedActorScript
gFieldEffect_UntargetedActorScript:
	.4byte 27
	.global gFieldEffect_DurationScripts
gFieldEffect_DurationScripts:
	.4byte gFieldEffect_ShortEffectScript, gFieldEffect_MediumEffectScript, gFieldEffect_LongEffectScript
@ The world picture's markers (WorldPicture_Show): a row for each World Map
@ actor from slot 11 (WORLD_MAP_FIRST_AREA_ACTOR), whose marker
@ is drawn where that actor stands, so a place's spot on the picture is
@ its actor's on the World Map. The flag is the place's
@ FLAG_AREA_VISITED. A row of zeros is an actor with no marker, and
@ -1 ends the table.
@ {tile << 16 | flag, area name}
	.global gWorldPicture_AreaMarkers
gWorldPicture_AreaMarkers:
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
