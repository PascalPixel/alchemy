.syntax unified
	.thumb
	.section .text.x02008054,"ax",%progbits
	.global Func_02000054
	.thumb_func
Func_02000054:
	push {lr}
	bl Func_020000b8
	pop {pc}
	.section .text.x02008064,"ax",%progbits
	.global Func_02000064
	.thumb_func
Func_02000064:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #85
	str r2, [r3]
	movs r1, #2
	movs r0, #8
	bl ObjectMotion_SetActionVariant
	movs r0, #8
	bl Object_GetById
	adds r0, #89
	ldrb r2, [r0]
	movs r3, #4
	orrs r3, r2
	strb r3, [r0]
	movs r0, #8
	bl Object_GetById
	adds r0, #89
	ldrb r2, [r0]
	movs r3, #16
	orrs r3, r2
	strb r3, [r0]
	movs r0, #0
	pop {pc}
	.2byte 0x0000
	.section .rodata.x020080c0,"a",%progbits
	.global gSceneEntrances
gSceneEntrances:
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
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
	.4byte 0x000000b8
	.4byte 0x101070b6
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global gScenePlacements
gScenePlacements:
	.4byte 0xffff00ca
	.4byte 0x00000001
	.4byte 0x02d00000
	.4byte 0x00000000
	.4byte 0x00930000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSceneEvents
gSceneEvents:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000c400
	.4byte 0xffff0008
	.4byte Func_02000054
	.4byte 0x00000000
	.4byte 0x197f0008
	.4byte 0x000026b0
	.4byte 0x00008d15
	.4byte 0x197f0008
	.4byte 0x000026b1
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte 0x00002439
	.4byte 0x00008d15
	.4byte 0xffff0008
	.4byte 0x0000243a
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
