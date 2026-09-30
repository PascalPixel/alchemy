.syntax unified
	.thumb
	.global Func_080d46a4
	.thumb_func
Func_080d46a4:
	push {r5, r6, r7, lr}
	adds r7, r1, #0
	movs r3, #192
	movs r1, #213
	lsls r3, r3, #18
	lsls r1, r1, #4
	adds r6, r0, #0
	movs r0, #108
	ldr r5, [r3, #32]
	bl Runtime_AllocateBlock
	movs r1, #197
	lsls r1, r1, #1
	adds r3, r0, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #3
	bne .L_080d470a
	movs r1, #128
	ldr r3, .L_080d470c
	lsls r1, r1, #9
	adds r0, r6, #0
	mov lr, r3
	.2byte 0xf800
	movs r2, #144
	movs r3, #144
	lsls r2, r2, #4
	lsls r3, r3, #4
	adds r2, #100
	adds r3, #104
	adds r1, r5, r2
	adds r2, r5, r3
	ldr r3, [r2]
	str r3, [r1]
	movs r1, #144
	lsls r1, r1, #4
	adds r1, #108
	adds r3, r5, r1
	adds r1, #2
	str r0, [r2]
	strh r7, [r3]
	adds r3, r5, r1
	movs r1, #128
	movs r2, #0
	lsls r1, r1, #3
	strh r2, [r3]
	ldr r0, .L_080d4710
	adds r1, #148
	bl Scheduler_AddOrUpdateCallback
.L_080d470a:
	pop {r5, r6, r7, pc}
.L_080d470c:
	.4byte IwramRatioMulQ14
.L_080d4710:
	.4byte Func_080d45d8
