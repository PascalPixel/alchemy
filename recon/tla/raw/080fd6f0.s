.syntax unified
	.thumb
	.global Func_080fd6f0
	.thumb_func
Func_080fd6f0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	mov r8, r1
	mov r10, r2
	ldr r2, .L_080fd730
	mov r3, r8
	sub sp, #8
	adds r4, r0, #0
	movs r1, #0
	adds r3, #62
	mov r12, r8
.L_080fd70c:
	strh r2, [r3]
	subs r3, #2
	cmp r3, r12
	bge .L_080fd70c
	mov r2, r10
	cmp r2, #1
	bne .L_080fd764
	lsls r3, r1, #1
	mov r2, r8
	adds r5, r3, r2
	movs r3, #252
	lsls r3, r3, #6
	adds r3, #255
	movs r7, #0
	movs r6, #88
	mov r8, r3
	b .L_080fd734
	.2byte 0x0000
.L_080fd730:
	.4byte 0x00000000
.L_080fd734:
	ldrh r2, [r6, r4]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_080fd75a
	mov r0, r8
	ands r0, r2
	str r1, [sp, #4]
	str r4, [sp, #0]
	bl BattleAction_Get
	ldrb r3, [r0, #6]
	ldr r1, [sp, #4]
	ldr r4, [sp, #0]
	cmp r3, #0
	beq .L_080fd75a
	ldrh r3, [r4, r6]
	adds r1, #1
	strh r3, [r5]
	adds r5, #2
.L_080fd75a:
	adds r7, #1
	adds r6, #4
	cmp r7, #31
	ble .L_080fd734
	b .L_080fd7fe
.L_080fd764:
	lsls r3, r1, #1
	mov r2, r8
	adds r5, r3, r2
	movs r3, #252
	lsls r3, r3, #6
	adds r3, #255
	movs r6, #88
	mov r9, r3
	movs r7, #31
.L_080fd776:
	ldrh r2, [r6, r4]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_080fd7a6
	mov r0, r9
	ands r0, r2
	str r1, [sp, #4]
	str r4, [sp, #0]
	bl BattleAction_Get
	ldrb r3, [r0, #6]
	ldr r1, [sp, #4]
	ldr r4, [sp, #0]
	cmp r3, #0
	bne .L_080fd79e
	ldrb r2, [r0, #1]
	movs r3, #64
	ands r3, r2
	cmp r3, #0
	beq .L_080fd7a6
.L_080fd79e:
	ldrh r3, [r6, r4]
	adds r1, #1
	strh r3, [r5]
	adds r5, #2
.L_080fd7a6:
	subs r7, #1
	adds r6, #4
	cmp r7, #0
	bge .L_080fd776
	mov r2, r10
	cmp r2, #2
	beq .L_080fd7fe
	lsls r3, r1, #1
	mov r2, r8
	adds r5, r3, r2
	movs r3, #252
	lsls r3, r3, #6
	adds r3, #255
	movs r6, #88
	mov r8, r3
	movs r7, #31
.L_080fd7c6:
	ldrh r2, [r6, r4]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_080fd7f6
	mov r0, r8
	ands r0, r2
	str r1, [sp, #4]
	str r4, [sp, #0]
	bl BattleAction_Get
	ldrb r3, [r0, #6]
	ldr r1, [sp, #4]
	ldr r4, [sp, #0]
	cmp r3, #0
	bne .L_080fd7f6
	ldrb r2, [r0, #1]
	movs r3, #64
	ands r3, r2
	cmp r3, #0
	bne .L_080fd7f6
	ldrh r3, [r6, r4]
	adds r1, #1
	strh r3, [r5]
	adds r5, #2
.L_080fd7f6:
	subs r7, #1
	adds r6, #4
	cmp r7, #0
	bge .L_080fd7c6
.L_080fd7fe:
	adds r0, r1, #0
	add sp, #8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
