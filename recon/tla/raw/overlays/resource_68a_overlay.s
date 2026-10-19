.syntax unified
	.thumb
	.section .text.x02008468,"ax",%progbits
	.balign 4
	.global Func_02000468
	.thumb_func
Func_02000468:
	push {r5, r6, lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #109
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008528
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #108
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008528
	movs r0, #12
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #109
	bl GameFlag_SetBit
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #110
	bl GameFlag_ClearBit
	movs r0, #13
	bl Func_02000080
	movs r1, #135
	movs r2, #184
	movs r0, #12
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_0200195c
	movs r3, #176
	lsls r3, r3, #13
	str r3, [r5, #12]
	movs r0, #12
	movs r1, #11
	movs r2, #0
	bl ObjectMotion_Launch
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r5, #28]
	str r3, [r5, #24]
	movs r0, #12
	ldr r1, .L_0200852c
	ldr r2, .L_02008530
	bl ObjectMotion_SetSpeedParameters
	movs r1, #156
	movs r0, #12
	lsls r1, r1, #1
	movs r2, #184
	bl ObjectMotion_ResetAndSetPositionInMode2
	movs r6, #23
.L_020084e6:
	ldrh r3, [r5, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	strh r3, [r5, #6]
	movs r2, #255
	ldr r3, [r5, #24]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bgt .L_02008504
	movs r2, #160
	lsls r2, r2, #4
	adds r3, r3, r2
	str r3, [r5, #24]
.L_02008504:
	ldr r3, [r5, #24]
	movs r0, #1
	str r3, [r5, #28]
	subs r6, #1
	bl WaitFrames
	cmp r6, #0
	bge .L_020084e6
	movs r3, #192
	lsls r3, r3, #6
	strh r3, [r5, #6]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #28]
	str r3, [r5, #24]
	movs r0, #12
	bl ObjectMotion_CommitCurrentPositionAndActivate
.L_02008528:
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200852c:
	.4byte 0x00023333
.L_02008530:
	.4byte 0x00011999
