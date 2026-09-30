.syntax unified
	.thumb
	.global Func_080e8b44
	.thumb_func
Func_080e8b44:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r10, r2
	movs r2, #192
	lsls r2, r2, #18
	adds r7, r1, #0
	ldr r1, [r2, #108]
	mov r11, r0
	movs r0, #230
	lsls r0, r0, #1
	adds r3, r1, r0
	ldr r3, [r3]
	ldr r6, [r2, #92]
	mov r9, r3
	ldr r3, [r2, #32]
	adds r3, #252
	ldr r3, [r3]
	cmp r3, #0
	beq .L_080e8b76
	b .L_080e8c8a
.L_080e8b76:
	movs r2, #197
	lsls r2, r2, #1
	adds r3, r1, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #3
	bne .L_080e8b88
	b .L_080e8c8a
.L_080e8b88:
	movs r3, #1
	mov r0, r11
	ands r3, r0
	cmp r3, #0
	beq .L_080e8c20
	movs r1, #28
	ldrsh r3, [r6, r1]
	movs r2, #1
	negs r2, r2
	cmp r3, r2
	bne .L_080e8bc8
	lsls r5, r7, #12
	adds r0, r5, #0
	bl Trig_Sin
	cmp r0, #0
	blt .L_080e8bb2
	adds r0, r5, #0
	bl Trig_Sin
	b .L_080e8bba
.L_080e8bb2:
	adds r0, r5, #0
	bl Trig_Sin
	negs r0, r0
.L_080e8bba:
	ldr r3, .L_080e8c98
	mov r1, r10
	mov lr, r3
	.2byte 0xf800
	ldr r3, [r6]
	adds r3, r3, r0
	b .L_080e8c22
.L_080e8bc8:
	cmp r3, #1
	bne .L_080e8bf6
	lsls r5, r7, #12
	adds r0, r5, #0
	bl Trig_Sin
	cmp r0, #0
	blt .L_080e8be2
	adds r0, r5, #0
	bl Trig_Sin
	negs r0, r0
	b .L_080e8be8
.L_080e8be2:
	adds r0, r5, #0
	bl Trig_Sin
.L_080e8be8:
	ldr r3, .L_080e8c98
	mov r1, r10
	mov lr, r3
	.2byte 0xf800
	ldr r3, [r6]
	adds r3, r3, r0
	b .L_080e8c22
.L_080e8bf6:
	lsls r5, r7, #12
	adds r0, r5, #0
	bl Trig_Sin
	cmp r0, #0
	blt .L_080e8c0a
	adds r0, r5, #0
	bl Trig_Sin
	b .L_080e8c12
.L_080e8c0a:
	adds r0, r5, #0
	bl Trig_Sin
	negs r0, r0
.L_080e8c12:
	ldr r3, .L_080e8c98
	mov r1, r10
	mov lr, r3
	.2byte 0xf800
	ldr r3, [r6]
	adds r3, r3, r0
	b .L_080e8c22
.L_080e8c20:
	ldr r3, [r6]
.L_080e8c22:
	mov r8, r3
	movs r3, #2
	mov r0, r11
	ands r3, r0
	cmp r3, #0
	beq .L_080e8c7c
	movs r1, #30
	ldrsh r3, [r6, r1]
	movs r2, #1
	negs r2, r2
	cmp r3, r2
	beq .L_080e8c52
	cmp r3, #1
	bne .L_080e8c52
	lsls r5, r7, #12
	adds r0, r5, #0
	bl Trig_Sin
	cmp r0, #0
	bge .L_080e8c66
	adds r0, r5, #0
	bl Trig_Sin
	b .L_080e8c6e
.L_080e8c52:
	lsls r5, r7, #12
	adds r0, r5, #0
	bl Trig_Sin
	cmp r0, #0
	blt .L_080e8c66
	adds r0, r5, #0
	bl Trig_Sin
	b .L_080e8c6e
.L_080e8c66:
	adds r0, r5, #0
	bl Trig_Sin
	negs r0, r0
.L_080e8c6e:
	ldr r3, .L_080e8c98
	mov r1, r10
	mov lr, r3
	.2byte 0xf800
	ldr r3, [r6, #8]
	adds r2, r3, r0
	b .L_080e8c7e
.L_080e8c7c:
	ldr r2, [r6, #8]
.L_080e8c7e:
	mov r0, r9
	mov r3, r8
	str r3, [r0, #8]
	ldr r3, [r6, #4]
	str r2, [r0, #16]
	str r3, [r0, #12]
.L_080e8c8a:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080e8c98:
	.4byte IwramMulQ16
