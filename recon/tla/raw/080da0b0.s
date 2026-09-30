.syntax unified
	.thumb
	.global Func_080da0b0
	.thumb_func
Func_080da0b0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #24
	str r2, [sp, #16]
	ldr r2, [sp, #56]
	str r3, [sp, #12]
	str r1, [sp, #20]
	movs r3, #192
	lsls r3, r3, #18
	subs r2, r2, r1
	adds r3, #160
	ldr r3, [r3]
	asrs r1, r2, #8
	str r2, [sp, #8]
	mov r9, r0
	ldr r2, [sp, #16]
	ldr r0, [sp, #60]
	ldr r4, [sp, #64]
	subs r0, r0, r2
	ldr r2, [sp, #12]
	mov r11, r3
	adds r3, r0, #0
	subs r4, r4, r2
	asrs r3, r3, #8
	mov r8, r3
	adds r3, r4, #0
	str r0, [sp, #4]
	str r4, [sp, #0]
	asrs r3, r3, #8
	ldr r6, .L_080da1b0
	adds r0, r1, #0
	mov r10, r3
	mov lr, r6
	.2byte 0xf800
	mov r1, r8
	adds r5, r0, #0
	mov r0, r8
	mov lr, r6
	.2byte 0xf800
	mov r1, r10
	mov r8, r0
	mov r0, r10
	mov lr, r6
	.2byte 0xf800
	add r5, r8
	adds r5, r5, r0
	ldr r3, .L_080da1b4
	adds r0, r5, #0
	mov lr, r3
	.2byte 0xf800
	movs r1, #6
	bl __divsi3
	mov r2, r9
	lsls r3, r2, #3
	subs r3, r3, r2
	lsls r3, r3, #2
	adds r3, #36
	mov r2, r11
	ldr r6, [r2, r3]
	movs r3, #0
	adds r7, r0, #1
	mov r8, r3
	cmp r8, r7
	bgt .L_080da1a0
	mov r11, r3
	mov r9, r3
	mov r10, r3
.L_080da142:
	mov r2, r8
	lsls r0, r2, #15
	adds r1, r7, #0
	bl __divsi3
	bl Trig_Sin
	ldr r3, .L_080da1b0
	ldr r1, [sp, #68]
	mov lr, r3
	.2byte 0xf800
	adds r1, r7, #0
	adds r5, r0, #0
	mov r0, r10
	bl __divsi3
	ldr r3, [sp, #20]
	adds r1, r7, #0
	adds r0, r3, r0
	str r0, [r6, #4]
	mov r0, r9
	bl __divsi3
	ldr r2, [sp, #16]
	adds r1, r7, #0
	adds r0, r2, r0
	adds r0, r0, r5
	str r0, [r6, #8]
	mov r0, r11
	bl __divsi3
	ldr r3, [sp, #12]
	movs r2, #1
	adds r0, r3, r0
	str r0, [r6, #12]
	strb r2, [r6, #16]
	ldr r3, [sp, #0]
	ldr r2, [sp, #4]
	add r11, r3
	ldr r3, [sp, #8]
	add r9, r2
	movs r2, #1
	add r8, r2
	ldr r6, [r6]
	add r10, r3
	cmp r8, r7
	ble .L_080da142
.L_080da1a0:
	movs r0, #0
	add sp, #24
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080da1b0:
	.4byte IwramMulQ16
.L_080da1b4:
	.4byte IwramFillWords + 0x74
