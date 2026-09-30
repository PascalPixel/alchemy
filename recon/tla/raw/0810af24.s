.syntax unified
	.thumb
	.global Func_0810af24
	.thumb_func
Func_0810af24:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	sub sp, #20
	mov r8, r3
	movs r3, #129
	lsls r3, r3, #3
	adds r3, #255
	add r3, r8
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	mov r9, r1
	str r3, [sp, #16]
	mov r3, r9
	adds r6, r0, #0
	cmp r3, #0
	bge .L_0810af5a
	adds r3, #3
.L_0810af5a:
	asrs r3, r3, #2
	str r3, [sp, #12]
	lsls r3, r3, #2
	mov r11, r3
	movs r3, #160
	lsls r3, r3, #3
	adds r3, #4
	add r3, r8
	movs r5, #0
	ldrsb r5, [r3, r5]
	cmp r6, #0
	beq .L_0810b03c
	adds r0, r6, #0
	bl RenderOutput_PrepareForRedrawFar
	mov r1, r11
	cmp r1, #0
	beq .L_0810afa4
	movs r3, #128
	lsls r3, r3, #3
	adds r3, #238
	add r3, r8
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
.L_0810afa4:
	mov r3, r11
	adds r3, #4
	cmp r3, r5
	bge .L_0810afcc
	movs r3, #158
	lsls r3, r3, #3
	add r3, r8
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
.L_0810afcc:
	movs r2, #0
	mov r10, r2
	mov r1, r8
	ldr r2, [sp, #12]
	adds r1, #248
	mov r3, r8
	adds r3, #2
	str r1, [sp, #4]
	movs r1, #153
	str r3, [sp, #8]
	movs r7, #156
	lsls r3, r2, #3
	lsls r1, r1, #3
	lsls r7, r7, #1
	adds r6, r3, r1
.L_0810afea:
	ldr r2, [sp, #8]
	mov r3, r11
	ldrsh r5, [r2, r6]
	ldr r1, [sp, #4]
	add r3, r10
	ldmia r1!, {r0}
	adds r2, r1, #0
	str r2, [sp, #4]
	cmp r0, #0
	beq .L_0810b02e
	cmp r3, r9
	bne .L_0810b00a
	movs r1, #30
	bl Animation_ApplyChildArgumentFar
	b .L_0810b010
.L_0810b00a:
	movs r1, #1
	bl Animation_ApplyChildArgumentFar
.L_0810b010:
	movs r3, #128
	lsls r3, r3, #9
	mov r2, r8
	str r3, [r7, r2]
	adds r0, r5, #0
	ldr r1, [sp, #16]
	bl Shop_CanServe
	cmp r0, #0
	bne .L_0810b02e
	movs r3, #179
	lsls r3, r3, #8
	adds r3, #51
	mov r1, r8
	str r3, [r7, r1]
.L_0810b02e:
	movs r2, #1
	add r10, r2
	mov r3, r10
	adds r7, #4
	adds r6, #2
	cmp r3, #3
	ble .L_0810afea
.L_0810b03c:
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
