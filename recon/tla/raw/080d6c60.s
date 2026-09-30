.syntax unified
	.thumb
	.global Func_080d6c60
	.thumb_func
Func_080d6c60:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #116]
	ldr r3, [r3, #32]
	sub sp, #12
	movs r1, #0
	mov r11, r0
	str r3, [sp, #8]
	str r1, [sp, #4]
	str r1, [sp, #0]
	mov r7, r11
	adds r7, #8
.L_080d6c86:
	ldrh r3, [r7, #28]
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	adds r1, r3, r2
	adds r3, r1, #0
	ands r3, r2
	strh r1, [r7, #28]
	cmp r3, r2
	bne .L_080d6c9c
	b .L_080d6dea
.L_080d6c9c:
	ldr r3, [sp, #8]
	movs r0, #179
	adds r3, #228
	ldr r6, [r3]
	ldr r3, [r3, #4]
	lsls r0, r0, #1
	mov r10, r3
	lsls r3, r1, #16
	asrs r3, r3, #16
	mov r9, r3
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080d6cc4
	ldrh r3, [r7, #28]
	adds r3, #1
	strh r3, [r7, #28]
	ldr r3, [r7, #24]
	subs r3, #1
	str r3, [r7, #24]
.L_080d6cc4:
	bl Random16
	adds r5, r0, #0
	bl Random16
	ldr r1, [r7, #12]
	movs r3, #1
	ands r0, r3
	ands r3, r5
	adds r3, r3, r0
	subs r1, r1, r6
	lsrs r3, r3, #1
	asrs r1, r1, #16
	adds r1, r1, r3
	subs r2, r1, #1
	ldr r3, [r7, #16]
	mov r8, r2
	ldr r2, [r7, #20]
	mov r0, r9
	subs r2, r2, r3
	mov r3, r10
	subs r2, r2, r3
	lsls r3, r0, #16
	asrs r2, r2, #16
	lsrs r3, r3, #16
	adds r1, #15
	subs r4, r2, r3
	cmp r1, #255
	bls .L_080d6d00
	b .L_080d6de6
.L_080d6d00:
	movs r1, #32
	negs r1, r1
	cmp r4, r1
	blt .L_080d6de6
	cmp r4, #159
	bgt .L_080d6de6
	ldrh r3, [r7, #28]
	cmp r3, #59
	bhi .L_080d6d32
	mov r2, r11
	ldr r3, [r2, #4]
	movs r0, #192
	ldrh r2, [r7, #8]
	ldr r1, .L_080d6d58
	lsls r0, r0, #2
	adds r3, #16
	adds r0, #255
	ands r3, r0
	ands r2, r1
	orrs r2, r3
	ldr r3, [r7, #24]
	strh r2, [r7, #8]
	adds r3, #3
	str r3, [r7, #24]
	b .L_080d6d72
.L_080d6d32:
	cmp r3, #89
	bhi .L_080d6d5c
	mov r2, r11
	ldr r3, [r2, #4]
	movs r0, #192
	ldrh r2, [r7, #8]
	ldr r1, .L_080d6d58
	lsls r0, r0, #2
	adds r3, #8
	adds r0, #255
	ands r3, r0
	ands r2, r1
	orrs r2, r3
	ldr r3, [r7, #24]
	strh r2, [r7, #8]
	adds r3, #1
	str r3, [r7, #24]
	b .L_080d6d72
	.2byte 0x0000
.L_080d6d58:
	.4byte 0xfffffc00
.L_080d6d5c:
	mov r3, r11
	ldr r2, [r3, #4]
	movs r0, #192
	ldrh r3, [r7, #8]
	ldr r1, .L_080d6ddc
	lsls r0, r0, #2
	adds r0, #255
	ands r2, r0
	ands r3, r1
	orrs r3, r2
	strh r3, [r7, #8]
.L_080d6d72:
	ldr r3, .L_080d6de0
	movs r2, #1
	ldr r3, [r3]
	lsrs r3, r3, #3
	ands r3, r2
	cmp r3, #0
	beq .L_080d6d98
	ldrh r2, [r7, #8]
	movs r0, #192
	lsls r3, r2, #22
	ldr r1, .L_080d6ddc
	lsrs r3, r3, #22
	lsls r0, r0, #2
	adds r3, #4
	adds r0, #255
	ands r3, r0
	ands r2, r1
	orrs r2, r3
	strh r2, [r7, #8]
.L_080d6d98:
	ldr r3, .L_080d6dd4
	mov r2, r8
	ands r2, r3
	mov r8, r2
	ldrh r3, [r7, #6]
	ldr r2, .L_080d6dd8
	mov r0, r8
	ands r3, r2
	orrs r3, r0
	strh r3, [r7, #6]
	ldr r3, [r7, #24]
	ldrb r1, [r7, #5]
	asrs r3, r3, #2
	subs r3, r4, r3
	movs r2, #63
	strb r3, [r7, #4]
	adds r3, r2, #0
	ands r3, r1
	strb r3, [r7, #5]
	ldrb r3, [r7, #7]
	adds r0, r7, #0
	ands r2, r3
	movs r3, #64
	orrs r2, r3
	strb r2, [r7, #7]
	movs r1, #240
	bl Func_080140d8
	b .L_080d6de4
	.2byte 0x0000
.L_080d6dd4:
	.4byte 0x000001ff
.L_080d6dd8:
	.4byte 0xfffffe00
.L_080d6ddc:
	.4byte 0xfffffc00
.L_080d6de0:
	.4byte Data_0300122c
.L_080d6de4:
	b .L_080d6dea
.L_080d6de6:
	movs r3, #0
	strh r3, [r7, #28]
.L_080d6dea:
	ldr r1, [sp, #4]
	cmp r1, #7
	bhi .L_080d6e38
	ldrh r2, [r7, #28]
	mov r10, r2
	cmp r2, #0
	bne .L_080d6e38
	ldr r3, [sp, #8]
	ldr r5, .L_080d6e54
	ldr r6, [r3]
	bl Random16
	ldr r3, [r6]
	lsls r0, r0, #8
	adds r3, r3, r0
	adds r3, r3, r5
	mov r8, r3
	bl Random16
	ldr r3, [r6, #8]
	lsls r0, r0, #8
	adds r3, r3, r0
	adds r4, r3, r5
	mov r0, r8
	str r0, [r7, #12]
	mov r1, r8
	adds r2, r4, #0
	str r4, [r7, #20]
	movs r0, #0
	bl Func_080201c0
	movs r3, #120
	mov r1, r10
	str r0, [r7, #16]
	strh r3, [r7, #28]
	str r1, [r7, #24]
	ldr r2, [sp, #4]
	adds r2, #1
	str r2, [sp, #4]
.L_080d6e38:
	ldr r3, [sp, #0]
	adds r7, #32
	adds r3, #1
	str r3, [sp, #0]
	cmp r3, #63
	bhi .L_080d6e46
	b .L_080d6c86
.L_080d6e46:
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080d6e54:
	.4byte 0xff800000
