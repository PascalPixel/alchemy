.syntax unified
	.thumb
	.global Func_080e150c
	.thumb_func
Func_080e150c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #248
	ldr r3, [r3]
	sub sp, #28
	str r3, [sp, #12]
	adds r3, #168
	ldr r1, [r3]
	movs r2, #0
	ldr r6, [sp, #12]
	str r1, [sp, #8]
	str r2, [sp, #4]
	mov r10, r3
	add r7, sp, #16
	adds r6, #160
.L_080e1538:
	movs r3, #0
	ldrsh r0, [r6, r3]
	lsls r0, r0, #10
	bl Trig_Sin
	movs r1, #0
	ldrsh r3, [r6, r1]
	mov r8, r0
	cmp r3, #0
	blt .L_080e15dc
	cmp r3, #31
	bgt .L_080e15dc
	ldr r2, [sp, #8]
	adds r0, r7, #0
	ldr r3, [r2, #8]
	str r3, [r7]
	ldr r1, [sp, #8]
	movs r3, #0
	ldrsh r2, [r6, r3]
	ldr r3, [r1, #12]
	lsls r2, r2, #16
	adds r3, r3, r2
	str r3, [r7, #4]
	ldr r3, [r1, #16]
	str r3, [r7, #8]
	bl Func_080dc390
	ldr r3, [r7, #8]
	ldr r1, [sp, #4]
	mov r9, r3
	lsls r3, r1, #2
	ldr r2, [r7]
	adds r3, r3, r1
	ldr r1, [sp, #12]
	lsls r3, r3, #4
	mov r11, r2
	adds r5, r3, r1
	movs r2, #0
.L_080e1584:
	mov r3, r11
	str r3, [r5, #12]
	mov r1, r9
	mov r3, r8
	str r1, [r5, #16]
	str r3, [r5, #20]
	str r3, [r5, #24]
	cmp r2, #0
	bne .L_080e15a6
	mov r1, r10
	ldr r0, [r1]
	str r2, [sp, #0]
	bl Func_080db9cc
	subs r0, #1
	strh r0, [r5, #30]
	b .L_080e15c2
.L_080e15a6:
	mov r3, r10
	ldr r0, [r3]
	str r2, [sp, #0]
	bl Func_080db9cc
	ldr r3, [r5, #16]
	ldr r1, .L_080e15f8
	adds r0, #1
	adds r3, r3, r1
	str r3, [r5, #16]
	ldr r3, [r5, #24]
	strh r0, [r5, #30]
	negs r3, r3
	str r3, [r5, #24]
.L_080e15c2:
	ldr r2, [sp, #0]
	adds r0, r5, #0
	str r2, [sp, #0]
	bl Func_080eb01c
	ldr r2, [sp, #0]
	adds r5, #40
	adds r2, #1
	cmp r2, #1
	ble .L_080e1584
	ldrh r3, [r6]
	adds r3, #1
	strh r3, [r6]
.L_080e15dc:
	ldr r2, [sp, #4]
	adds r6, #2
	adds r2, #1
	str r2, [sp, #4]
	cmp r2, #1
	ble .L_080e1538
	add sp, #28
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080e15f8:
	.4byte 0xffff0000
