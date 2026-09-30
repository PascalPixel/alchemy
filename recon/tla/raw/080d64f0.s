.syntax unified
	.thumb
	.global Func_080d64f0
	.thumb_func
Func_080d64f0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	ldr r3, [r3, #116]
	mov r10, r1
	mov r8, r3
	mov r3, r10
	adds r3, #228
	ldr r2, [r3]
	sub sp, #12
	str r2, [sp, #8]
	mov r7, r8
	ldr r3, [r3, #4]
	movs r4, #63
	str r3, [sp, #4]
	movs r3, #0
	adds r7, #8
	mov r9, r3
	mov r11, r4
.L_080d6524:
	ldrh r3, [r7, #28]
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r3, r3, r1
	adds r2, r1, #0
	ands r2, r3
	strh r3, [r7, #28]
	cmp r2, r1
	bne .L_080d653a
	b .L_080d6654
.L_080d653a:
	movs r0, #179
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080d654c
	ldrh r3, [r7, #28]
	adds r3, #1
	strh r3, [r7, #28]
.L_080d654c:
	ldrh r2, [r7, #28]
	ldr r5, [sp, #8]
	lsls r3, r2, #2
	adds r3, r3, r2
	ldr r2, .L_080d65f8
	lsls r3, r3, #1
	adds r0, r3, r2
	ldr r3, [r7, #12]
	subs r2, r3, r5
	cmp r2, #0
	bge .L_080d656a
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r2, r2, r1
.L_080d656a:
	movs r4, #0
	ldrsh r3, [r0, r4]
	asrs r2, r2, #16
	adds r1, r2, r3
	ldr r3, [r7, #20]
	ldr r2, [r7, #16]
	ldr r5, [sp, #4]
	subs r3, r3, r2
	subs r3, r3, r5
	adds r0, #2
	cmp r3, #0
	bge .L_080d658a
	movs r2, #255
	lsls r2, r2, #8
	adds r2, #255
	adds r3, r3, r2
.L_080d658a:
	movs r4, #0
	ldrsh r2, [r0, r4]
	asrs r3, r3, #16
	adds r4, r3, r2
	adds r3, r1, #0
	adds r3, #16
	adds r0, #2
	cmp r3, #255
	bhi .L_080d661a
	movs r5, #32
	negs r5, r5
	cmp r4, r5
	blt .L_080d661a
	cmp r4, #159
	bgt .L_080d661a
	ldrb r2, [r7, #9]
	adds r5, #19
	adds r3, r5, #0
	ands r2, r3
	movs r3, #4
	orrs r2, r3
	ldr r3, .L_080d65e8
	strb r2, [r7, #9]
	ands r1, r3
	ldr r2, .L_080d65ec
	ldrh r3, [r7, #6]
	strb r4, [r7, #4]
	ands r3, r2
	orrs r3, r1
	strh r3, [r7, #6]
	mov r2, r8
	ldrh r3, [r0]
	ldr r1, [r2, #4]
	ldr r2, .L_080d65f0
	adds r1, r1, r3
	ldr r3, .L_080d65f4
	adds r0, #2
	ands r1, r3
	ldrh r3, [r7, #8]
	ands r3, r2
	orrs r3, r1
	strh r3, [r7, #8]
	ldrb r1, [r7, #5]
	ldrb r2, [r0]
	mov r3, r11
	b .L_080d65fc
	.2byte 0x0000
.L_080d65e8:
	.4byte 0x000001ff
.L_080d65ec:
	.4byte 0xfffffe00
.L_080d65f0:
	.4byte 0xfffffc00
.L_080d65f4:
	.4byte 0x000003ff
.L_080d65f8:
	.4byte Data_080f0ab0
.L_080d65fc:
	lsls r2, r2, #6
	ands r3, r1
	orrs r3, r2
	strb r3, [r7, #5]
	ldrb r1, [r7, #7]
	ldrb r2, [r0, #2]
	mov r3, r11
	ands r3, r1
	lsls r2, r2, #6
	orrs r3, r2
	strb r3, [r7, #7]
	adds r0, r7, #0
	movs r1, #240
	bl Func_080140d8
.L_080d661a:
	ldrh r3, [r7, #28]
	cmp r3, #0
	bne .L_080d6654
	mov r3, r10
	ldr r6, [r3]
	bl Random16
	ldr r3, [r6]
	ldr r5, .L_080d6670
	lsls r0, r0, #8
	adds r3, r3, r0
	adds r1, r3, r5
	str r1, [sp, #0]
	bl Random16
	ldr r3, [r6, #8]
	lsls r0, r0, #8
	ldr r1, [sp, #0]
	adds r3, r3, r0
	adds r4, r3, r5
	str r1, [r7, #12]
	str r4, [r7, #20]
	movs r0, #0
	adds r2, r4, #0
	bl Func_080201c0
	movs r3, #16
	str r0, [r7, #16]
	strh r3, [r7, #28]
.L_080d6654:
	movs r4, #1
	add r9, r4
	mov r5, r9
	adds r7, #32
	cmp r5, #63
	bhi .L_080d6662
	b .L_080d6524
.L_080d6662:
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080d6670:
	.4byte 0xff800000
