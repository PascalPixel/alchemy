.syntax unified
	.thumb
	.global Func_0813f8d4
	.thumb_func
Func_0813f8d4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #92]
	sub sp, #48
	str r3, [sp, #8]
	ldr r2, [sp, #8]
	ldr r1, [r3]
	mov r11, r1
	mov r3, r11
	adds r3, #1
	str r3, [r2]
	mov r3, r11
	cmp r3, #0
	bne .L_0813f936
	ldr r5, .L_0813fa40
	movs r1, #0
	mov r10, r1
.L_0813f904:
	bl Random16
	movs r3, #15
	ands r0, r3
	adds r3, r0, #0
	adds r3, #48
	adds r0, #40
	str r3, [r5]
	str r0, [r5, #4]
	bl Random16
	str r0, [r5, #12]
	bl Random16
	str r0, [r5, #16]
	bl Random16
	movs r2, #1
	movs r3, #128
	add r10, r2
	lsls r3, r3, #1
	str r0, [r5, #20]
	adds r5, #28
	cmp r10, r3
	bne .L_0813f904
.L_0813f936:
	add r3, sp, #36
	movs r2, #0
	ldr r1, .L_0813fa40
	str r2, [r3, #4]
	str r2, [r3, #8]
	mov r10, r2
	mov r9, r3
	mov r8, r1
.L_0813f946:
	mov r3, r10
	cmp r3, #0
	bge .L_0813f94e
	adds r3, #3
.L_0813f94e:
	asrs r3, r3, #2
	cmp r11, r3
	ble .L_0813fa1c
	mov r2, r8
	ldr r3, [r2]
	cmp r3, #0
	ble .L_0813fa1c
	bl Func_08014de4
	mov r3, r8
	ldr r0, [r3, #20]
	bl Func_080150e4
	mov r1, r8
	ldr r0, [r1, #12]
	bl Func_08015024
	mov r2, r8
	ldr r0, [r2, #16]
	bl Func_08015068
	mov r1, r8
	ldr r3, [r1]
	add r4, sp, #24
	mov r2, r9
	str r3, [r2]
	adds r1, r4, #0
	mov r0, r9
	str r4, [sp, #4]
	bl Func_0815e1ec
	ldr r4, [sp, #4]
	mov r1, r8
	ldr r3, [r4]
	add r7, sp, #12
	adds r3, #64
	str r3, [r4]
	ldr r3, [r4, #4]
	mov r2, r9
	adds r3, #80
	str r3, [r4, #4]
	mov r0, r9
	ldr r3, [r1, #4]
	adds r1, r7, #0
	str r3, [r2]
	bl Func_0815e1ec
	ldr r3, [r7]
	mov r1, r8
	adds r3, #64
	str r3, [r7]
	ldr r3, [r7, #4]
	ldr r4, [sp, #4]
	adds r3, #80
	str r3, [r7, #4]
	mov r3, r8
	ldr r2, [r3, #4]
	subs r2, #4
	str r2, [r3, #4]
	ldr r3, [r3]
	subs r3, #4
	str r3, [r1]
	cmp r2, #0
	bge .L_0813f9d2
	movs r3, #0
	str r3, [r1, #4]
.L_0813f9d2:
	mov r2, r8
	ldr r5, [r2, #4]
	ldr r0, [r7]
	negs r5, r5
	lsrs r3, r5, #31
	ldr r2, [r4]
	adds r5, r5, r3
	asrs r5, r5, #1
	adds r6, r5, #0
	ldr r3, [r4, #4]
	ldr r1, [r7, #4]
	subs r0, #1
	subs r2, #1
	adds r6, #48
	str r4, [sp, #4]
	str r6, [sp, #0]
	bl Func_08143eb4
	ldr r4, [sp, #4]
	ldr r1, [r7, #4]
	ldr r3, [r4, #4]
	ldr r2, [r4]
	ldr r0, [r7]
	subs r1, #1
	subs r3, #1
	str r6, [sp, #0]
	bl Func_08143eb4
	ldr r4, [sp, #4]
	ldr r0, [r7]
	ldr r1, [r7, #4]
	ldr r2, [r4]
	ldr r3, [r4, #4]
	adds r5, #56
	str r5, [sp, #0]
	bl Func_08143eb4
.L_0813fa1c:
	movs r1, #1
	add r10, r1
	movs r3, #28
	mov r2, r10
	add r8, r3
	cmp r2, #64
	bne .L_0813f946
	ldr r1, [sp, #8]
	movs r3, #1
	str r3, [r1, #4]
	add sp, #48
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0813fa40:
	.4byte gMapCellBuffer
