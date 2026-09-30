.syntax unified
	.thumb
	.global Func_080d81ec
	.thumb_func
Func_080d81ec:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_080d82d8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r7, r0, #0
	ldr r0, [r3]
	sub sp, #12
	bl Object_GetById
	movs r3, #64
	adds r3, r3, r7
	movs r2, #0
	ldrsb r2, [r3, r2]
	mov r10, r3
	mov r8, r2
	cmp r2, #0
	bne .L_080d829e
	ldr r3, [r0, #8]
	mov r6, sp
	str r3, [r6]
	ldr r3, [r0, #12]
	str r3, [r6, #4]
	ldr r3, [r0, #16]
	str r3, [r6, #8]
	bl Random16
	lsls r5, r0, #2
	adds r5, r5, r0
	movs r3, #160
	lsls r3, r3, #12
	lsls r5, r5, #1
	adds r5, r5, r3
	bl Random16
	adds r2, r6, #0
	adds r1, r0, #0
	adds r0, r5, #0
	bl Vector_AddPolarOffset
	adds r0, r6, #0
	bl Camera_WorldToScreen
	ldr r2, [r6]
	movs r0, #240
	str r2, [r7, #20]
	movs r1, #192
	ldr r3, [r6, #8]
	lsls r0, r0, #15
	str r3, [r7, #24]
	str r2, [r7, #4]
	str r3, [r7, #8]
	lsls r1, r1, #8
	str r2, [r6]
	adds r2, r6, #0
	str r3, [r6, #8]
	bl Vector_AddPolarOffset
	ldr r3, [r6]
	mov r2, r8
	str r3, [r7, #12]
	ldr r3, [r6, #8]
	str r3, [r7, #16]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #36]
	movs r3, #160
	lsls r3, r3, #11
	str r3, [r7, #32]
	adds r3, r7, #0
	adds r3, #66
	strb r2, [r3]
	mov r2, r10
	ldrb r3, [r2]
	adds r3, #1
	strb r3, [r2]
	ldr r3, .L_080d82dc
	movs r2, #1
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_080d82ce
	movs r0, #144
	bl Audio_PlayCue
	b .L_080d82ce
.L_080d829e:
	mov r3, r8
	cmp r3, #1
	bne .L_080d82b8
	adds r0, r7, #0
	bl BattleFx_HasReachedTarget
	cmp r0, #0
	bne .L_080d82ce
	mov r2, r10
	ldrb r3, [r2]
	subs r3, #1
	strb r3, [r2]
	b .L_080d82ce
.L_080d82b8:
	mov r3, r8
	cmp r3, #2
	bne .L_080d82ce
	adds r0, r7, #0
	bl BattleFx_HasReachedTarget
	cmp r0, #0
	bne .L_080d82ce
	adds r0, r7, #0
	bl BattleFx_ClearOwnedSlot
.L_080d82ce:
	add sp, #12
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_080d82d8:
	.4byte gPartyState
.L_080d82dc:
	.4byte gFrameTick
