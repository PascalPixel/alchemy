.syntax unified
	.thumb
	.global Func_080d1ae4
	.thumb_func
Func_080d1ae4:
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
	ldr r1, [r3, #32]
	adds r2, #144
	ldr r2, [r2]
	adds r3, r1, #0
	adds r3, #228
	mov r10, r2
	movs r2, #2
	ldrsh r0, [r3, r2]
	sub sp, #8
	mov r11, r0
	movs r2, #6
	ldrsh r0, [r3, r2]
	mov r3, r10
	str r0, [sp, #4]
	ldr r2, [r3, #24]
	cmp r2, #0
	bne .L_080d1b1c
	b .L_080d1cb2
.L_080d1b1c:
	movs r0, #10
	ldrsh r6, [r2, r0]
	ldr r3, [r2, #16]
	ldr r2, [r2, #12]
	movs r0, #212
	subs r3, r3, r2
	asrs r5, r3, #16
	ldr r3, .L_080d1be4
	movs r2, #189
	lsls r2, r2, #1
	adds r3, r3, r2
	ldrh r3, [r3]
	movs r2, #8
	lsrs r3, r3, #5
	lsls r0, r0, #1
	adds r2, r2, r6
	mov r9, r3
	mov r8, r2
	adds r3, r1, r0
	ldr r4, [r3]
	mov r3, r8
	cmp r2, #0
	bge .L_080d1b4e
	adds r3, r6, #0
	adds r3, #23
.L_080d1b4e:
	asrs r3, r3, #4
	lsls r3, r3, #2
	adds r2, r4, r3
	adds r3, r5, #0
	cmp r5, #0
	bge .L_080d1b5c
	adds r3, #15
.L_080d1b5c:
	asrs r3, r3, #4
	ldr r0, .L_080d1be8
	lsls r1, r3, #9
	adds r7, r6, #0
	adds r3, r2, r1
	subs r7, #8
	adds r2, r3, r0
	mov r12, r0
	adds r3, r7, #0
	cmp r7, #0
	bge .L_080d1b74
	adds r3, r6, #7
.L_080d1b74:
	asrs r3, r3, #4
	ldr r0, .L_080d1be8
	lsls r3, r3, #2
	adds r3, r4, r3
	adds r3, r3, r1
	ldrb r2, [r2, #3]
	adds r1, r3, r0
	movs r0, #1
	adds r3, r0, #0
	ands r3, r2
	cmp r3, #0
	bne .L_080d1b96
	ldrb r2, [r1, #3]
	adds r3, r0, #0
	ands r3, r2
	cmp r3, #0
	beq .L_080d1c02
.L_080d1b96:
	movs r3, #128
	mov r0, r10
	lsls r3, r3, #23
	str r3, [r0, #4]
	movs r3, #128
	lsls r3, r3, #3
	str r3, [r0, #8]
	ldr r3, .L_080d1bdc
	ldrh r1, [r0, #8]
	mov r2, r9
	ands r2, r3
	mov r3, r12
	ands r3, r1
	orrs r3, r2
	strh r3, [r0, #8]
	mov r3, r11
	subs r2, r6, r3
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #248
	adds r2, r2, r3
	ldr r3, .L_080d1be0
	ldrh r1, [r0, #6]
	ands r2, r3
	ldr r3, .L_080d1bec
	str r4, [sp, #0]
	ands r3, r1
	orrs r3, r2
	strh r3, [r0, #6]
	movs r2, #240
	adds r3, r5, #0
	ands r3, r2
	ldr r2, [sp, #4]
	movs r1, #1
	b .L_080d1bf0
.L_080d1bdc:
	.4byte 0x000003ff
.L_080d1be0:
	.4byte 0x000001ff
.L_080d1be4:
	.4byte ResourceTableEntries
.L_080d1be8:
	.4byte 0xfffffc00
.L_080d1bec:
	.4byte 0xfffffe00
.L_080d1bf0:
	subs r3, r3, r2
	adds r3, #224
	strb r3, [r0, #4]
	movs r3, #12
	adds r3, r3, r0
	mov r10, r3
	bl Func_080140d8
	ldr r4, [sp, #0]
.L_080d1c02:
	mov r3, r8
	cmp r3, #0
	bge .L_080d1c0c
	adds r3, r6, #0
	adds r3, #23
.L_080d1c0c:
	asrs r3, r3, #4
	lsls r3, r3, #2
	adds r2, r4, r3
	adds r3, r5, #0
	cmp r5, #0
	bge .L_080d1c1a
	adds r3, #15
.L_080d1c1a:
	asrs r3, r3, #4
	ldr r0, .L_080d1ca0
	lsls r1, r3, #9
	adds r3, r2, r1
	adds r2, r3, r0
	adds r3, r7, #0
	mov r12, r0
	cmp r3, #0
	bge .L_080d1c2e
	adds r3, r6, #7
.L_080d1c2e:
	asrs r3, r3, #4
	ldr r0, .L_080d1ca0
	lsls r3, r3, #2
	adds r3, r4, r3
	adds r3, r3, r1
	ldrb r2, [r2, #3]
	adds r1, r3, r0
	movs r0, #1
	adds r3, r0, #0
	ands r3, r2
	cmp r3, #0
	bne .L_080d1c50
	ldrb r2, [r1, #3]
	adds r3, r0, #0
	ands r3, r2
	cmp r3, #0
	beq .L_080d1cb2
.L_080d1c50:
	movs r3, #128
	mov r0, r10
	lsls r3, r3, #23
	str r3, [r0, #4]
	movs r3, #128
	lsls r3, r3, #3
	str r3, [r0, #8]
	ldr r3, .L_080d1c98
	mov r2, r9
	ands r2, r3
	mov r9, r2
	ldr r3, .L_080d1ca4
	ldrh r2, [r0, #8]
	ldrh r1, [r0, #6]
	ands r3, r2
	mov r2, r9
	orrs r3, r2
	strh r3, [r0, #8]
	mov r3, r11
	subs r2, r6, r3
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #248
	adds r2, r2, r3
	ldr r3, .L_080d1c9c
	ands r2, r3
	mov r3, r12
	ands r3, r1
	orrs r3, r2
	strh r3, [r0, #6]
	ldr r2, [sp, #4]
	movs r3, #240
	ands r5, r3
	subs r3, r5, r2
	b .L_080d1ca8
	.2byte 0x0000
.L_080d1c98:
	.4byte 0x000003ff
.L_080d1c9c:
	.4byte 0x000001ff
.L_080d1ca0:
	.4byte 0xfffffe00
.L_080d1ca4:
	.4byte 0xfffffc00
.L_080d1ca8:
	adds r3, #240
	strb r3, [r0, #4]
	movs r1, #1
	bl Func_080140d8
.L_080d1cb2:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
