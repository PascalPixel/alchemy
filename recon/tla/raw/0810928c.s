.syntax unified
	.thumb
	.global Func_0810928c
	.thumb_func
Func_0810928c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #20
	str r2, [sp, #16]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	mov r9, r1
	mov r10, r3
	mov r3, r9
	adds r6, r0, #0
	cmp r3, #0
	bge .L_081092b4
	adds r3, #3
.L_081092b4:
	asrs r3, r3, #2
	str r3, [sp, #12]
	lsls r3, r3, #2
	mov r11, r3
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #4
	add r3, r10
	movs r5, #0
	ldrsb r5, [r3, r5]
	cmp r6, #0
	beq .L_08109396
	adds r0, r6, #0
	bl RenderOutput_PrepareForRedrawFar
	mov r1, r11
	cmp r1, #0
	beq .L_081092fe
	movs r3, #128
	lsls r3, r3, #3
	adds r3, #238
	add r3, r10
	ldrh r0, [r3]
	movs r3, #12
	negs r3, r3
	movs r1, #128
	str r3, [sp, #0]
	adds r2, r6, #0
	movs r3, #88
	lsls r1, r1, #23
	bl RenderOutput_CreateFar
	movs r2, #0
	movs r3, #17
	strb r2, [r0, #4]
	strb r3, [r0, #5]
	strh r2, [r0, #12]
.L_081092fe:
	mov r3, r11
	adds r3, #4
	cmp r3, r5
	bge .L_08109326
	movs r3, #158
	lsls r3, r3, #3
	add r3, r10
	movs r1, #128
	ldrh r0, [r3]
	movs r5, #0
	movs r3, #88
	lsls r1, r1, #23
	adds r2, r6, #0
	str r5, [sp, #0]
	bl RenderOutput_CreateFar
	movs r3, #15
	strb r5, [r0, #4]
	strb r3, [r0, #5]
	strh r5, [r0, #12]
.L_08109326:
	movs r2, #0
	mov r8, r2
	mov r1, r10
	ldr r2, [sp, #12]
	adds r1, #248
	mov r3, r10
	adds r3, #2
	str r1, [sp, #4]
	movs r1, #153
	str r3, [sp, #8]
	movs r7, #156
	lsls r3, r2, #3
	lsls r1, r1, #3
	lsls r7, r7, #1
	adds r6, r3, r1
.L_08109344:
	ldr r2, [sp, #8]
	mov r3, r11
	ldrsh r5, [r2, r6]
	ldr r1, [sp, #4]
	add r3, r8
	ldmia r1!, {r0}
	adds r2, r1, #0
	str r2, [sp, #4]
	cmp r0, #0
	beq .L_08109388
	cmp r3, r9
	bne .L_08109364
	movs r1, #30
	bl Animation_ApplyChildArgumentFar
	b .L_0810936a
.L_08109364:
	movs r1, #1
	bl Animation_ApplyChildArgumentFar
.L_0810936a:
	movs r3, #128
	lsls r3, r3, #9
	mov r2, r10
	str r3, [r7, r2]
	adds r0, r5, #0
	ldr r1, [sp, #16]
	bl Djinn_IsActiveFar + 0x18
	cmp r0, #0
	bne .L_08109388
	movs r3, #204
	lsls r3, r3, #8
	adds r3, #204
	mov r1, r10
	str r3, [r7, r1]
.L_08109388:
	movs r2, #1
	add r8, r2
	mov r3, r8
	adds r7, #4
	adds r6, #2
	cmp r3, #3
	ble .L_08109344
.L_08109396:
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
