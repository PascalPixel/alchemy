.syntax unified
	.thumb
	.global Func_080d9f1c
	.thumb_func
Func_080d9f1c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #40
	str r1, [sp, #32]
	str r2, [sp, #28]
	str r3, [sp, #24]
	str r0, [sp, #36]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #160
	ldr r3, [r3]
	ldr r2, [sp, #72]
	str r3, [sp, #20]
	movs r3, #0
	str r3, [sp, #16]
	ldr r3, [sp, #32]
	ldr r1, [sp, #76]
	subs r2, r2, r3
	str r2, [sp, #12]
	asrs r2, r2, #8
	mov r9, r2
	ldr r2, [sp, #28]
	ldr r3, [sp, #24]
	ldr r0, [sp, #80]
	subs r1, r1, r2
	str r1, [sp, #8]
	subs r0, r0, r3
	asrs r1, r1, #8
	mov r8, r1
	adds r1, r0, #0
	asrs r1, r1, #8
	str r0, [sp, #4]
	mov r10, r1
	ldr r6, .L_080da058
	mov r1, r9
	mov r0, r9
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
	ldr r3, .L_080da05c
	adds r0, r5, #0
	mov lr, r3
	.2byte 0xf800
	movs r1, #6
	bl __divsi3
	ldr r2, [sp, #36]
	ldr r3, [sp, #36]
	lsls r2, r2, #3
	ldr r1, [sp, #20]
	subs r5, r2, r3
	lsls r5, r5, #2
	adds r3, r1, r5
	adds r3, #36
	mov r1, r9
	adds r6, r0, #1
	mov r0, r10
	str r2, [sp, #0]
	mov r8, r3
	bl ArcTan2
	ldr r2, [sp, #20]
	movs r3, #0
	lsls r0, r0, #16
	adds r5, #16
	lsrs r0, r0, #16
	movs r7, #0
	mov r11, r3
	mov r9, r3
	mov r10, r3
	str r0, [r2, r5]
	b .L_080da01c
.L_080d9fcc:
	ldr r3, [sp, #36]
	adds r1, r6, #0
	adds r3, #1
	strb r3, [r5, #17]
	mov r0, r10
	bl __divsi3
	ldr r1, [sp, #32]
	adds r0, r1, r0
	str r0, [r5, #4]
	adds r1, r6, #0
	mov r0, r9
	bl __divsi3
	ldr r2, [sp, #28]
	adds r1, r6, #0
	adds r0, r2, r0
	str r0, [r5, #8]
	mov r0, r11
	bl __divsi3
	ldr r3, [sp, #24]
	adds r0, r3, r0
	str r0, [r5, #12]
	cmp r7, r6
	bne .L_080da004
	movs r3, #2
	b .L_080da006
.L_080da004:
	movs r3, #1
.L_080da006:
	strb r3, [r5, #16]
	mov r1, r8
	str r5, [r1]
	ldr r2, [sp, #4]
	ldr r3, [sp, #8]
	ldr r1, [sp, #12]
	mov r8, r5
	add r11, r2
	add r9, r3
	add r10, r1
	adds r7, #1
.L_080da01c:
	cmp r7, r6
	bgt .L_080da030
	bl Func_080d9e74
	adds r5, r0, #0
	cmp r5, #0
	bne .L_080d9fcc
	movs r2, #1
	negs r2, r2
	str r2, [sp, #16]
.L_080da030:
	mov r1, r8
	movs r3, #0
	str r3, [r1]
	ldr r2, [sp, #0]
	ldr r1, [sp, #36]
	subs r3, r2, r1
	ldr r1, [sp, #20]
	lsls r3, r3, #2
	adds r3, #32
	movs r2, #1
	str r2, [r1, r3]
	ldr r0, [sp, #16]
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080da058:
	.4byte IwramMulQ16
.L_080da05c:
	.4byte IwramFillWords + 0x74
