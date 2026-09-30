.syntax unified
	.thumb
	.global Func_080d7dbc
	.thumb_func
Func_080d7dbc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_080d7f78
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	adds r7, r0, #0
	ldr r0, [r3]
	sub sp, #12
	bl Object_GetById
	movs r2, #64
	adds r2, r2, r7
	movs r6, #0
	ldrsb r6, [r2, r6]
	mov r9, r0
	mov r10, r2
	cmp r6, #0
	bne .L_080d7e76
	ldr r2, [r7, #20]
	ldr r3, [r7, #24]
	str r2, [r7, #4]
	str r3, [r7, #8]
	mov r8, sp
	str r2, [sp, #0]
	str r3, [sp, #8]
	bl Random16
	adds r5, r0, #0
	bl Random16
	lsls r1, r5, #1
	lsls r3, r0, #1
	adds r3, r3, r0
	adds r1, r1, r5
	lsls r1, r1, #11
	lsls r3, r3, #11
	lsrs r3, r3, #16
	lsrs r1, r1, #16
	subs r1, r1, r3
	movs r3, #192
	lsls r3, r3, #8
	movs r0, #240
	adds r1, r1, r3
	lsls r0, r0, #15
	mov r2, r8
	bl Vector_AddPolarOffset
	mov r1, r8
	ldr r3, [r1]
	mov r2, r10
	str r3, [r7, #12]
	ldr r3, [r1, #8]
	mov r1, r9
	str r3, [r7, #16]
	movs r3, #160
	lsls r3, r3, #11
	str r3, [r7, #36]
	str r3, [r7, #32]
	adds r3, r7, #0
	adds r3, #66
	strb r6, [r3]
	ldrb r3, [r2]
	adds r3, #1
	strb r3, [r2]
	ldr r3, [r1, #80]
	ldr r0, [r7]
	ldrb r3, [r3, #9]
	ldrb r1, [r0, #9]
	movs r2, #12
	ands r2, r3
	movs r3, #13
	negs r3, r3
	ands r3, r1
	orrs r3, r2
	strb r3, [r0, #9]
	adds r3, r7, #0
	adds r3, #71
	strb r6, [r3]
	strh r6, [r7, #56]
	ldr r3, .L_080d7f7c
	movs r2, #1
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_080d7f6a
	movs r0, #134
	bl Audio_PlayCue
	b .L_080d7f6a
.L_080d7e76:
	cmp r6, #1
	bne .L_080d7e98
	movs r2, #56
	ldrsh r3, [r7, r2]
	cmp r3, #3
	bne .L_080d7f42
	ldr r1, [r7]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	adds r2, r7, #0
	strb r3, [r1, #9]
	adds r2, #71
	movs r3, #4
	strb r3, [r2]
	b .L_080d7f42
.L_080d7e98:
	cmp r6, #2
	bne .L_080d7ed6
	adds r0, r7, #0
	bl BattleFx_HasReachedTarget
	cmp r0, #0
	bne .L_080d7f6a
	ldr r3, [r7, #4]
	ldr r1, [r7]
	str r3, [r7, #20]
	ldr r3, [r7, #8]
	str r3, [r7, #24]
	movs r3, #13
	ldrb r2, [r1, #9]
	negs r3, r3
	ands r3, r2
	adds r2, r7, #0
	adds r2, #71
	strb r3, [r1, #9]
	movs r3, #4
	strb r3, [r2]
	adds r3, r7, #0
	adds r3, #68
	strb r0, [r3]
	mov r2, r10
	ldrb r3, [r2]
	adds r3, #1
	strb r3, [r2]
	movs r3, #40
	strh r3, [r7, #58]
	b .L_080d7f6a
.L_080d7ed6:
	cmp r6, #3
	bne .L_080d7f3e
	movs r1, #1
	mov r8, r1
	adds r3, r7, #0
	adds r3, #68
	mov r2, r8
	strb r2, [r3]
	ldr r3, [r7, #20]
	mov r1, r9
	str r3, [r7, #4]
	ldr r3, [r7, #24]
	mov r5, sp
	str r3, [r7, #8]
	movs r2, #160
	ldr r3, [r1, #8]
	lsls r2, r2, #13
	str r3, [r5]
	adds r0, r5, #0
	ldr r3, [r1, #12]
	adds r3, r3, r2
	str r3, [r5, #4]
	ldr r3, [r1, #16]
	str r3, [r5, #8]
	bl Camera_WorldToScreen
	bl Random16
	adds r1, r0, #0
	movs r0, #128
	adds r2, r5, #0
	lsls r0, r0, #11
	bl Vector_AddPolarOffset
	ldr r3, [r5]
	mov r1, r10
	str r3, [r7, #12]
	mov r2, r8
	ldr r3, [r5, #8]
	str r3, [r7, #16]
	ldrb r3, [r1]
	adds r3, #1
	strb r3, [r1]
	ldr r3, .L_080d7f7c
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_080d7f6a
	movs r0, #145
	bl Audio_PlayCue
	b .L_080d7f6a
.L_080d7f3e:
	cmp r6, #4
	bne .L_080d7f56
.L_080d7f42:
	adds r0, r7, #0
	bl BattleFx_HasReachedTarget
	cmp r0, #0
	bne .L_080d7f6a
	mov r1, r10
	ldrb r3, [r1]
	subs r3, #1
	strb r3, [r1]
	b .L_080d7f6a
.L_080d7f56:
	cmp r6, #5
	bne .L_080d7f6a
	adds r0, r7, #0
	bl BattleFx_HasReachedTarget
	cmp r0, #0
	bne .L_080d7f6a
	adds r0, r7, #0
	bl BattleFx_ClearOwnedSlot
.L_080d7f6a:
	add sp, #12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080d7f78:
	.4byte gPartyState
.L_080d7f7c:
	.4byte gFrameTick
