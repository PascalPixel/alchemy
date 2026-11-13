	.section .rodata
	.global gField_ExamineTriggerMessageOffsets
gField_ExamineTriggerMessageOffsets:
	.byte 2, 3, 13, 8, 30, 31
@ 8-byte rows searched by ID by Field_GetPsynergyTargetRule; ID -1 terminates.
	.global gData_080eff28
gData_080eff28:
	.incbin "games/tla/src/field/event_commands/descent.table"
@ Actions passed to Actor_StartScriptFar (lists of words).
	.global gFieldEffect_ParticleEmitterScript
gFieldEffect_ParticleEmitterScript:
	.4byte 22, 9, 32768, 22
	.4byte 10, 32768, 0, 2
	.4byte 22, 9, 49152, 22
	.4byte 10, 49152, 0, 2
	.4byte 22, 9, 57344, 22
	.4byte 10, 57344, 0, 2
	.4byte 22, 9, 61440, 22
	.4byte 10, 61440, 0, 2
	.4byte 22, 9, 65536, 22
	.4byte 10, 65536, 17
	.global gFieldEffect_RisingPulseScript
gFieldEffect_RisingPulseScript:
	.4byte 0, 10, 22, 9
	.4byte 90112, 22, 10, 90112
	.4byte 0, 2, 22, 9
	.4byte 98304, 22, 10, 98304
	.4byte 0, 2, 22, 9
	.4byte 73728, 22, 10, 73728
	.4byte 0, 2, 22, 9
	.4byte 65536, 22, 10, 65536
	.4byte 0, 2, 22, 9
	.4byte 90112, 22, 10, 90112
	.4byte 0, 2, 22, 9
	.4byte 98304, 22, 10, 98304
	.4byte 0, 2, 22, 9
	.4byte 73728, 22, 10, 73728
	.4byte 0, 2, 22, 9
	.4byte 65536, 22, 10, 65536
	.4byte 0, 2, 0, 50
	.4byte 13, 65535, 0, 17
	.global gBattleEffect_ParticleScript
gBattleEffect_ParticleScript:
	.4byte 23, 6, -16384, 0
	.4byte 1, 13, 60, 0
	.4byte 38
