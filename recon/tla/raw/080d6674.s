.syntax unified
	.thumb
	.global Func_080d6674
	.thumb_func
Func_080d6674:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #32]
	ldr r3, [r3, #116]
	mov r10, r0
	mov r8, r3
	mov r3, r10
	adds r3, #228
	ldr r1, [r3]
	sub sp, #4
	str r1, [sp, #0]
	mov r6, r8
	ldr r3, [r3, #4]
	movs r2, #0
	adds r6, #8
	mov r11, r3
	mov r9, r2
.L_080d66a4:
	ldrh r3, [r6, #28]
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r3, r3, r1
	adds r2, r1, #0
	ands r2, r3
	strh r3, [r6, #28]
	cmp r2, r1
	bne .L_080d66ba
	b .L_080d67c4
.L_080d66ba:
	movs r0, #179
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080d66cc
	ldrh r3, [r6, #28]
	adds r3, #1
	strh r3, [r6, #28]
.L_080d66cc:
	ldrh r3, [r6, #28]
	ldr r2, .L_080d6774
	lsls r3, r3, #2
	adds r1, r3, r2
	ldr r0, [sp, #0]
	ldr r3, [r6, #12]
	subs r2, r3, r0
	cmp r2, #0
	bge .L_080d66e6
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	adds r2, r2, r3
.L_080d66e6:
	movs r0, #0
	ldrsh r3, [r1, r0]
	asrs r2, r2, #16
	adds r7, r2, r3
	ldr r2, [r6, #16]
	ldr r3, [r6, #20]
	adds r1, #2
	subs r3, r3, r2
	mov r2, r11
	subs r3, r3, r2
	cmp r3, #0
	bge .L_080d6706
	movs r0, #255
	lsls r0, r0, #8
	adds r0, #255
	adds r3, r3, r0
.L_080d6706:
	movs r0, #0
	ldrsh r2, [r1, r0]
	asrs r3, r3, #16
	adds r4, r3, r2
	adds r3, r7, #0
	adds r3, #16
	cmp r3, #255
	bhi .L_080d678a
	movs r1, #32
	negs r1, r1
	cmp r4, r1
	blt .L_080d678a
	cmp r4, #159
	bgt .L_080d678a
	ldrb r2, [r6, #9]
	movs r0, #13
	negs r0, r0
	adds r3, r0, #0
	ands r2, r3
	movs r3, #4
	orrs r2, r3
	ldr r3, .L_080d6764
	strb r2, [r6, #9]
	ands r7, r3
	ldr r2, .L_080d6768
	ldrh r3, [r6, #6]
	strb r4, [r6, #4]
	ands r3, r2
	orrs r3, r7
	mov r2, r8
	strh r3, [r6, #6]
	ldr r1, [r2, #4]
	ldr r3, .L_080d676c
	ldr r2, .L_080d6770
	ands r1, r3
	ldrh r3, [r6, #8]
	adds r0, r6, #0
	ands r3, r2
	ldrb r2, [r6, #5]
	orrs r3, r1
	movs r1, #63
	strh r3, [r6, #8]
	adds r3, r1, #0
	ands r3, r2
	movs r2, #64
	orrs r3, r2
	b .L_080d6778
.L_080d6764:
	.4byte 0x000001ff
.L_080d6768:
	.4byte 0xfffffe00
.L_080d676c:
	.4byte 0x000003ff
.L_080d6770:
	.4byte 0xfffffc00
.L_080d6774:
	.4byte Data_080f0b50
.L_080d6778:
	strb r3, [r6, #5]
	ldrb r3, [r6, #7]
	ands r1, r3
	movs r3, #128
	orrs r1, r3
	strb r1, [r6, #7]
	movs r1, #240
	bl Func_080140d8
.L_080d678a:
	ldrh r3, [r6, #28]
	cmp r3, #0
	bne .L_080d67c4
	mov r3, r10
	ldr r5, [r3]
	bl Random16
	ldr r3, [r5]
	lsls r0, r0, #8
	adds r3, r3, r0
	ldr r0, .L_080d67e0
	adds r7, r3, r0
	bl Random16
	ldr r3, [r5, #8]
	ldr r1, .L_080d67e4
	lsls r0, r0, #8
	adds r3, r3, r0
	adds r4, r3, r1
	str r7, [r6, #12]
	str r4, [r6, #20]
	movs r0, #0
	adds r1, r7, #0
	adds r2, r4, #0
	bl Func_080201c0
	movs r3, #12
	str r0, [r6, #16]
	strh r3, [r6, #28]
.L_080d67c4:
	movs r2, #1
	add r9, r2
	mov r3, r9
	adds r6, #32
	cmp r3, #63
	bhi .L_080d67d2
	b .L_080d66a4
.L_080d67d2:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080d67e0:
	.4byte 0xffc00000
.L_080d67e4:
	.4byte 0xff800000
