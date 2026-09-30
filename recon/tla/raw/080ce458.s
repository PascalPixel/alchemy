.syntax unified
	.thumb
	.global Func_080ce458
	.thumb_func
Func_080ce458:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #12
	str r1, [sp, #8]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	adds r6, r2, #0
	ldr r5, [r3, #16]
	mov r9, r0
	bl Func_080cdf5c
	bl ObjectTable_Get
	ldrh r0, [r0, #6]
	movs r3, #1
	negs r3, r3
	str r0, [sp, #4]
	cmp r6, r3
	beq .L_080ce564
	ldr r0, [r5]
	cmp r0, r3
	beq .L_080ce564
	movs r2, #255
	mov r11, r2
	mov r10, r11
	mov r3, r10
	movs r4, #255
	lsls r4, r4, #8
	ands r3, r6
	ands r4, r6
	mov r10, r3
.L_080ce4a2:
	movs r3, #4
	ldrsh r6, [r5, r3]
	movs r3, #240
	lsls r3, r3, #8
	ldrh r2, [r5, #4]
	ands r6, r3
	ldr r3, .L_080ce4dc
	lsls r0, r0, #16
	ands r3, r2
	lsls r3, r3, #16
	asrs r3, r3, #16
	mov r7, r11
	lsrs r0, r0, #24
	mov r8, r3
	ands r7, r2
	str r4, [sp, #0]
	bl BattleAction_Get
	ldr r3, [r5]
	movs r2, #15
	ands r3, r2
	ldrb r1, [r0, #6]
	ldr r4, [sp, #0]
	cmp r3, #5
	bne .L_080ce556
	ldr r2, [sp, #8]
	cmp r1, r2
	bne .L_080ce556
	b .L_080ce4e0
.L_080ce4dc:
	.4byte 0x00000800
.L_080ce4e0:
	movs r3, #6
	ldrsh r0, [r5, r3]
	bl GameFlag_IsConditionActive
	ldr r4, [sp, #0]
	cmp r0, #0
	beq .L_080ce556
	mov r2, r8
	cmp r2, #0
	beq .L_080ce50c
	ldr r2, [sp, #4]
	subs r3, r6, r2
	lsls r3, r3, #16
	asrs r3, r3, #16
	cmp r3, #0
	bge .L_080ce502
	negs r3, r3
.L_080ce502:
	movs r2, #184
	lsls r2, r2, #5
	adds r2, #255
	cmp r3, r2
	bgt .L_080ce556
.L_080ce50c:
	movs r3, #240
	lsls r3, r3, #24
	adds r3, #5
	cmp r9, r3
	beq .L_080ce522
	ldr r0, [r5]
	adds r3, #10
	ands r3, r0
	cmp r3, r9
	bne .L_080ce556
	b .L_080ce524
.L_080ce522:
	ldr r0, [r5]
.L_080ce524:
	movs r3, #128
	ands r3, r0
	cmp r3, #0
	beq .L_080ce538
	movs r2, #128
	lsls r2, r2, #2
	cmp r4, r2
	bne .L_080ce556
.L_080ce534:
	adds r0, r5, #0
	b .L_080ce566
.L_080ce538:
	movs r3, #16
	ands r3, r0
	cmp r3, #0
	beq .L_080ce54e
	movs r3, #128
	lsls r3, r3, #1
	cmp r4, r3
	bne .L_080ce556
	cmp r10, r7
	bne .L_080ce556
	b .L_080ce534
.L_080ce54e:
	cmp r4, #0
	bne .L_080ce556
	cmp r10, r7
	beq .L_080ce534
.L_080ce556:
	adds r5, #12
	ldr r3, [r5]
	movs r2, #1
	negs r2, r2
	adds r0, r3, #0
	cmp r3, r2
	bne .L_080ce4a2
.L_080ce564:
	movs r0, #0
.L_080ce566:
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
