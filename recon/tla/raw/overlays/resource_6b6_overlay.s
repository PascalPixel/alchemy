.syntax unified
	.thumb
	.section .text.x02008054,"ax",%progbits
	.global Func_02000054
	.thumb_func
Func_02000054:
	push {lr}
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_020000bc
	movs r0, #48
	bl GameFlag_ClearBit
	pop {pc}
	.section .text.x02008068,"ax",%progbits
	.global Func_02000068
	.thumb_func
Func_02000068:
	push {lr}
	movs r0, #9
	movs r1, #1
	movs r2, #0
	bl Func_020000bc
	movs r0, #68
	bl GameFlag_ClearBit
	pop {pc}
	.section .text.x0200807c,"ax",%progbits
	.global Func_0200007c
	.thumb_func
Func_0200007c:
	push {lr}
	movs r0, #10
	movs r1, #2
	movs r2, #0
	bl Func_020000bc
	movs r0, #88
	bl GameFlag_ClearBit
	pop {pc}
	.section .text.x02008090,"ax",%progbits
	.global Func_02000090
	.thumb_func
Func_02000090:
	push {lr}
	movs r0, #11
	movs r1, #3
	movs r2, #0
	bl Func_020000bc
	movs r0, #108
	bl GameFlag_ClearBit
	pop {pc}
	.section .text.x020080ac,"ax",%progbits
	.global Func_020000ac
	.thumb_func
Func_020000ac:
	movs r0, #0
	bx lr
	.section .rodata.x020080c4,"a",%progbits
	.global gSceneEntrances
gSceneEntrances:
	.4byte 0xffff0000
	.4byte 0x000000c0
	.4byte 0x800000c0
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSceneExits
gSceneExits:
	.4byte 0x00000139
	.4byte 0x0000013a
	.4byte 0x0000013b
	.4byte 0x0000013c
	.4byte 0x000001ff
	.global gScenePlacements
gScenePlacements:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSceneEvents
gSceneEvents:
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte Func_02000054
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte Func_02000068
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte Func_0200007c
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte Func_02000090
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x02000200
	.4byte 0x03600400
	.4byte 0xffffffff
	.4byte 0xffffffff
