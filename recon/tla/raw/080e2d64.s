.syntax unified
	.thumb
	.global Func_080e2d64
	.thumb_func
Func_080e2d64:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r2, r3, #0
	ldr r6, [r3, #92]
	adds r2, #224
	movs r1, #216
	ldr r2, [r2]
	lsls r1, r1, #5
	adds r1, #18
	adds r1, r6, r1
	sub sp, #20
	movs r3, #216
	ldr r2, [r2, #16]
	lsls r3, r3, #5
	str r1, [sp, #4]
	adds r3, #16
	adds r3, r3, r6
	mov r10, r2
	mov r9, r3
	movs r2, #0
	ldrsh r7, [r1, r2]
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	beq .L_080e2daa
	cmp r3, #1
	beq .L_080e2e82
	b .L_080e2e8c
.L_080e2daa:
	lsls r2, r7, #8
	mov r11, r2
	mov r0, r11
	bl Trig_Sin
	movs r3, #216
	lsls r3, r3, #5
	adds r3, #24
	adds r3, r6, r3
	lsls r0, r0, #5
	movs r1, #216
	str r3, [sp, #0]
	lsls r1, r1, #5
	str r0, [r3]
	adds r1, #20
	adds r1, r1, r6
	ldr r3, [r1]
	movs r2, #160
	lsls r2, r2, #3
	adds r3, r3, r2
	str r3, [r1]
	mov r8, r1
	movs r3, #216
	movs r1, #216
	lsls r3, r3, #5
	lsls r1, r1, #5
	adds r3, #28
	adds r1, #40
	adds r2, r6, r3
	adds r3, r6, r1
	ldr r3, [r3]
	ldr r2, [r2]
	add r5, sp, #8
	subs r3, r3, r2
	muls r3, r7
	cmp r3, #0
	bge .L_080e2df6
	adds r3, #127
.L_080e2df6:
	asrs r3, r3, #7
	adds r3, r2, r3
	mov r0, r11
	str r3, [r5]
	bl Trig_Sin
	movs r1, #216
	movs r3, #217
	lsls r1, r1, #5
	lsls r3, r3, #5
	adds r1, #44
	adds r2, r6, r3
	adds r3, r6, r1
	ldr r3, [r3]
	ldr r2, [r2]
	subs r3, r3, r2
	muls r3, r7
	cmp r3, #0
	bge .L_080e2e1e
	adds r3, #127
.L_080e2e1e:
	asrs r3, r3, #7
	adds r3, r2, r3
	lsls r2, r0, #6
	adds r3, r3, r2
	str r3, [r5, #4]
	movs r1, #216
	movs r3, #216
	lsls r3, r3, #5
	lsls r1, r1, #5
	adds r3, #36
	adds r1, #48
	adds r2, r6, r3
	adds r3, r6, r1
	ldr r3, [r3]
	ldr r2, [r2]
	subs r3, r3, r2
	muls r3, r7
	cmp r3, #0
	bge .L_080e2e46
	adds r3, #127
.L_080e2e46:
	asrs r3, r3, #7
	adds r3, r2, r3
	str r3, [r5, #8]
	ldr r2, [sp, #0]
	mov r3, r8
	ldr r1, [r3]
	ldr r0, [r2]
	adds r2, r5, #0
	bl Func_0801489c
	ldr r3, [r5]
	mov r1, r10
	str r3, [r1, #8]
	ldr r3, [r5, #4]
	str r3, [r1, #12]
	ldr r3, [r5, #8]
	str r3, [r1, #16]
	cmp r7, #127
	ble .L_080e2e8c
	mov r2, r9
	ldrh r3, [r2]
	mov r1, r9
	adds r3, #1
	strh r3, [r1]
	ldr r2, [sp, #4]
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r2]
	b .L_080e2e8c
.L_080e2e82:
	movs r3, #186
	lsls r3, r3, #2
	adds r3, #255
	mov r1, r9
	strh r3, [r1]
.L_080e2e8c:
	movs r3, #216
	lsls r3, r3, #5
	adds r3, #18
	adds r2, r6, r3
	ldrh r3, [r2]
	movs r1, #216
	adds r3, #1
	strh r3, [r2]
	lsls r1, r1, #5
	adds r1, #12
	adds r3, r6, r1
	mov r1, r10
	ldr r2, [r3]
	ldr r3, [r1, #8]
	add sp, #20
	str r3, [r2, #8]
	ldr r3, [r1, #12]
	str r3, [r2, #12]
	ldr r3, [r1, #16]
	str r3, [r2, #16]
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
