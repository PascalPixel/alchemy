	.section .rodata
@ Word-aligned, as an array of structures is: the two bytes before it are
@ the alignment's fill.
	.balign 4
@ 243 rows searched by ID by BattleEffect_FindDefinition, followed by one more row.
	.global gData_080f0308
gData_080f0308:
	.incbin "games/tla/src/field/object/variant_motion.table"
@ Three 128-byte actions for Actor_StartRepeatedMotion, plus a final word.
	.global gActor_VariantScripts
gActor_VariantScripts:
	.4byte 23, 9, -16384, 23
	.4byte 10, 8192, 0, 2
	.4byte 23, 9, 8192, 23
	.4byte 10, -4096, 0, 2
	.4byte 23, 9, 4096, 23
	.4byte 10, -2048, 0, 2
	.4byte 23, 9, 4096, 23
	.4byte 10, -2048, 0, 4
	.4byte 23, 9, -16384, 23
	.4byte 10, 8192, 0, 2
	.4byte 23, 9, 8192, 23
	.4byte 10, -4096, 0, 2
	.4byte 23, 9, 4096, 23
	.4byte 10, -2048, 0, 2
	.4byte 23, 9, 4096, 23
	.4byte 10, -2048, 0, 4
	.4byte 23, 9, -16384, 23
	.4byte 10, 8192, 0, 2
	.4byte 23, 9, 8192, 23
	.4byte 10, -4096, 0, 2
	.4byte 23, 9, 4096, 23
	.4byte 10, -2048, 0, 2
	.4byte 23, 9, 4096, 23
	.4byte 10, -2048, 0, 4
	.4byte 17
	.global Data_080f085c
Data_080f085c:
	.4byte 0, 1, 13, 38, 0, 38
	.global Data_080f0874
Data_080f0874:
	.4byte 0, 1, 13, 18, 0, 38
	.global gActor_BlinkChildValues
gActor_BlinkChildValues:
	.byte 7, 0, 0, 0
