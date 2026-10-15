	.section .rodata
@ Word-aligned, as an array of structures is: the two bytes before it are
@ the alignment's fill.
	.balign 4
@ Func_080d1e60 が番号で探す 243 行と、その後の 1 行
	.global gData_080f0308
gData_080f0308:
	.incbin "games/tla/src/field/common/object/variant_motion.table"
@ ObjectMotion_SetVariantCallback の動作 3 本 (各 128 バイト) と終わりの 1 語
	.global gObject_VariantMotionScripts
gObject_VariantMotionScripts:
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
	.global gObjectGroup_BlinkChildValues
gObjectGroup_BlinkChildValues:
	.byte 7, 0, 0, 0
