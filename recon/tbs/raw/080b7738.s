.syntax unified
	.thumb
	.global Func_080b7738
	.thumb_func
Func_080b7738:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #44
	add r1, sp, #16
	mov r10, r1
	movs r0, #3
	bl BattleParty_ListActorIds
	movs r7, #0
	mov r2, r10
	ldrh r3, [r2, r7]
	cmp r3, #255
	beq .L_080b77b2
	movs r2, #0
.L_080b775e:
	mov r3, r10
	ldrh r0, [r3, r2]
	bl GetBattleObjectSlot
	adds r6, r0, #0
	cmp r6, #0
	beq .L_080b77a0
	ldr r5, [r6]
	bl BattleStatusIcon_Cycle
	ldr r3, [r6, #36]
	cmp r3, #0
	beq .L_080b77a0
	adds r0, r5, #0
	movs r1, #0
	bl GetMotionRecord
	cmp r0, #0
	beq .L_080b77a0
	ldr r3, [r5, #12]
	movs r1, #0
	cmp r3, #0
	beq .L_080b778e
	movs r1, #9
.L_080b778e:
	ldr r2, [r6, #36]
	ldrb r3, [r2, #6]
	cmp r3, r1
	beq .L_080b77a0
	strb r1, [r2, #6]
	adds r2, r0, #0
	adds r2, #37
	movs r3, #1
	strb r3, [r2]
.L_080b77a0:
	adds r7, #1
	cmp r7, #13
	bgt .L_080b77b2
	lsls r3, r7, #1
	adds r2, r3, #0
	mov r1, r10
	ldrh r3, [r1, r2]
	cmp r3, #255
	bne .L_080b775e
.L_080b77b2:
	ldr r3, .L_080b78e0
	ldr r3, [r3]
	movs r2, #54
	ldrsh r3, [r3, r2]
	cmp r3, #0
	blt .L_080b77c8
	movs r3, #1
	movs r1, #2
	str r3, [sp, #8]
	str r1, [sp, #12]
	b .L_080b77d0
.L_080b77c8:
	movs r2, #2
	movs r3, #1
	str r2, [sp, #8]
	str r3, [sp, #12]
.L_080b77d0:
	movs r0, #1
	mov r1, r10
	bl BattleParty_ListActorIds
	movs r7, #0
	mov r9, r0
	cmp r7, r9
	bge .L_080b784e
	ldr r1, [sp, #8]
	movs r3, #3
	ands r3, r1
	lsls r2, r3, #2
	str r2, [sp, #4]
	mov r11, r3
	mov r8, r10
.L_080b77ee:
	mov r3, r8
	movs r1, #2
	ldrh r0, [r3]
	add r8, r1
	bl GetBattleObjectSlot
	cmp r0, #0
	beq .L_080b7848
	ldr r5, [r0]
	adds r3, r5, #0
	adds r3, #84
	ldrb r3, [r3]
	movs r2, #15
	ands r2, r3
	cmp r2, #1
	beq .L_080b7814
	cmp r2, #2
	beq .L_080b7826
	b .L_080b7848
.L_080b7814:
	ldr r0, [r5, #80]
	movs r2, #13
	ldrb r3, [r0, #9]
	ldr r1, [sp, #4]
	negs r2, r2
	ands r3, r2
	orrs r3, r1
	strb r3, [r0, #9]
	b .L_080b7848
.L_080b7826:
	mov r2, r11
	movs r6, #13
	ldr r1, [r5, #80]
	negs r6, r6
	lsls r5, r2, #2
	movs r4, #3
.L_080b7832:
	ldmia r1!, {r0}
	cmp r0, #0
	beq .L_080b7842
	ldrb r2, [r0, #9]
	adds r3, r6, #0
	ands r3, r2
	orrs r3, r5
	strb r3, [r0, #9]
.L_080b7842:
	subs r4, #1
	cmp r4, #0
	bge .L_080b7832
.L_080b7848:
	adds r7, #1
	cmp r7, r9
	blt .L_080b77ee
.L_080b784e:
	movs r0, #2
	mov r1, r10
	bl BattleParty_ListActorIds
	movs r7, #0
	mov r9, r0
	cmp r7, r9
	bge .L_080b78cc
	ldr r1, [sp, #12]
	movs r3, #3
	ands r3, r1
	lsls r2, r3, #2
	str r2, [sp, #0]
	mov r11, r3
	mov r8, r10
.L_080b786c:
	mov r3, r8
	movs r1, #2
	ldrh r0, [r3]
	add r8, r1
	bl GetBattleObjectSlot
	cmp r0, #0
	beq .L_080b78c6
	ldr r5, [r0]
	adds r3, r5, #0
	adds r3, #84
	ldrb r3, [r3]
	movs r2, #15
	ands r2, r3
	cmp r2, #1
	beq .L_080b7892
	cmp r2, #2
	beq .L_080b78a4
	b .L_080b78c6
.L_080b7892:
	ldr r0, [r5, #80]
	movs r2, #13
	ldrb r3, [r0, #9]
	ldr r1, [sp, #0]
	negs r2, r2
	ands r3, r2
	orrs r3, r1
	strb r3, [r0, #9]
	b .L_080b78c6
.L_080b78a4:
	mov r2, r11
	movs r6, #13
	ldr r1, [r5, #80]
	negs r6, r6
	lsls r5, r2, #2
	movs r4, #3
.L_080b78b0:
	ldmia r1!, {r0}
	cmp r0, #0
	beq .L_080b78c0
	ldrb r2, [r0, #9]
	adds r3, r6, #0
	ands r3, r2
	orrs r3, r5
	strb r3, [r0, #9]
.L_080b78c0:
	subs r4, #1
	cmp r4, #0
	bge .L_080b78b0
.L_080b78c6:
	adds r7, #1
	cmp r7, r9
	blt .L_080b786c
.L_080b78cc:
	add sp, #44
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_080b78e0:
	.4byte gCameraWork
