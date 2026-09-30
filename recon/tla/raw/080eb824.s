.syntax unified
	.thumb
	.global Func_080eb824
	.thumb_func
Func_080eb824:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	movs r1, #196
	movs r0, #240
	ldr r5, [r3, #60]
	bl Runtime_AllocateHeapBlock
	movs r2, #128
	lsls r2, r2, #5
	mov r8, r2
	adds r7, r0, #0
	mov r1, r8
	movs r0, #96
	bl Runtime_AllocateHeapBlock
	mov r10, r0
	ldr r6, .L_080eb8ac
	movs r1, #196
	adds r0, r7, #0
	movs r2, #0
	mov lr, r6
	.2byte 0xf800
	movs r3, #152
	lsls r3, r3, #5
	adds r3, #72
	adds r5, r5, r3
	ldrh r3, [r5]
	adds r5, r7, #0
	adds r5, #188
	strh r3, [r5]
	ldr r2, .L_080eb8a8
	adds r3, r7, #0
	movs r1, #1
	adds r3, #192
	strb r1, [r3]
	adds r3, #1
	strb r2, [r3]
	adds r2, r7, #0
	adds r2, #168
	movs r3, #16
	str r3, [r2]
	adds r3, r7, #0
	movs r0, #0
	adds r3, #172
	movs r2, #128
	str r0, [r3]
	lsls r2, r2, #9
	adds r3, #4
	str r2, [r3]
	adds r3, #4
	str r2, [r3]
	adds r3, #11
	adds r2, r7, #0
	strb r1, [r3]
	adds r2, #190
	movs r3, #250
	strb r3, [r2]
	mov r1, r8
	mov r0, r10
	movs r2, #0
	b .L_080eb8b0
	.2byte 0x0000
.L_080eb8a8:
	.4byte 0x00000000
.L_080eb8ac:
	.4byte IwramFillWords
.L_080eb8b0:
	mov lr, r6
	.2byte 0xf800
	ldrh r1, [r5]
	ldr r2, .L_080eb920
	movs r3, #128
	lsls r1, r1, #5
	lsls r3, r3, #19
	adds r1, r1, r2
	adds r3, #212
	mov r0, r10
	ldr r2, .L_080eb924
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r1, #128
	lsls r1, r1, #6
	movs r0, #180
	bl Runtime_AllocateBlock
	movs r6, #0
	movs r7, #7
.L_080eb8d8:
	asrs r3, r6, #3
	adds r4, r6, #0
	movs r1, #0
	ands r4, r7
	lsls r5, r3, #3
.L_080eb8e2:
	asrs r3, r1, #3
	adds r3, r5, r3
	lsls r3, r3, #3
	adds r2, r1, #0
	adds r3, r3, r4
	ands r2, r7
	lsls r3, r3, #3
	adds r3, r3, r2
	adds r1, #1
	strh r3, [r0]
	adds r0, #2
	cmp r1, #63
	ble .L_080eb8e2
	adds r6, #1
	cmp r6, #63
	ble .L_080eb8d8
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_080eb928
	bl Scheduler_AddOrUpdateCallback
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #108
	ldr r0, .L_080eb92c
	bl Scheduler_AddOrUpdateCallback
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_080eb920:
	.4byte 0x06010000
.L_080eb924:
	.4byte 0x84000400
.L_080eb928:
	.4byte Func_080eb6a0
.L_080eb92c:
	.4byte Func_080eb594
