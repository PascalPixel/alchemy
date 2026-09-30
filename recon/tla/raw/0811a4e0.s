.syntax unified
	.thumb
	.global Func_0811a4e0
	.thumb_func
Func_0811a4e0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	sub sp, #4
	ldr r7, [r3, #36]
	mov r11, r0
	bl Owner_GetState
	movs r5, #165
	mov r9, r0
	lsls r5, r5, #1
	add r5, r9
	ldrh r0, [r5]
	bl Summon_IsEntryFlagged
	movs r1, #0
	mov r8, r0
	ldrh r0, [r5]
	mov r10, r1
	bl Func_081280bc
	mov r4, r10
	adds r6, r0, #0
.L_0811a51a:
	movs r3, #42
	adds r3, #255
	add r3, r9
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_0811a5e0
	movs r2, #4
	ldrsh r3, [r7, r2]
	movs r5, #0
	cmp r3, #0
	bne .L_0811a53e
	mov r3, r8
	cmp r3, #0
	bne .L_0811a560
	movs r1, #6
	ldrsh r3, [r7, r1]
	cmp r3, #0
	beq .L_0811a560
.L_0811a53e:
	adds r5, #1
	cmp r5, #5
	bgt .L_0811a560
	lsls r2, r5, #1
	adds r3, r2, #4
	ldrsh r3, [r7, r3]
	cmp r3, #0
	bne .L_0811a53e
	mov r3, r8
	cmp r3, #0
	bne .L_0811a560
	cmp r5, #4
	bgt .L_0811a53e
	adds r3, r2, #6
	ldrsh r3, [r7, r3]
	cmp r3, #0
	bne .L_0811a53e
.L_0811a560:
	cmp r5, #6
	beq .L_0811a5e6
	movs r3, #165
	lsls r3, r3, #1
	add r3, r9
	ldrh r0, [r3]
	str r4, [sp, #0]
	bl Func_081280d8
	ldr r2, .L_0811a5f8
	ldr r4, [sp, #0]
	lsls r1, r5, #14
	adds r3, r0, #0
	adds r1, r1, r2
	adds r0, r5, #0
	adds r2, r6, r4
	bl Func_080202b8
	ldr r4, [sp, #0]
	cmp r0, #0
	bne .L_0811a58e
	movs r0, #0
	b .L_0811a5e8
.L_0811a58e:
	cmp r4, #0
	bne .L_0811a598
	lsls r3, r5, #12
	orrs r3, r6
	mov r10, r3
.L_0811a598:
	lsls r0, r5, #1
	adds r3, r0, #4
	mov r1, r11
	mov r2, r8
	strh r1, [r7, r3]
	cmp r2, #0
	bne .L_0811a5aa
	adds r3, r0, #6
	strh r1, [r7, r3]
.L_0811a5aa:
	movs r2, #153
	lsls r2, r2, #1
	adds r2, #255
	cmp r6, r2
	beq .L_0811a5e0
	movs r3, #188
	lsls r3, r3, #1
	adds r3, #255
	cmp r6, r3
	beq .L_0811a5e0
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #126
	cmp r6, r1
	beq .L_0811a5e0
	adds r2, #24
	cmp r6, r2
	beq .L_0811a5e0
	subs r3, #52
	cmp r6, r3
	beq .L_0811a5e0
	subs r1, #57
	cmp r6, r1
	beq .L_0811a5e0
	subs r2, #2
	cmp r6, r2
	bne .L_0811a5e6
.L_0811a5e0:
	adds r4, #1
	cmp r4, #1
	ble .L_0811a51a
.L_0811a5e6:
	mov r0, r10
.L_0811a5e8:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0811a5f8:
	.4byte Data_02018000
