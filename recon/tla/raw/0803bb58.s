.syntax unified
	.thumb
	.global Func_0803bb58
	.thumb_func
Func_0803bb58:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #108
	str r2, [sp, #20]
	str r1, [sp, #24]
	mov r8, r3
	movs r3, #192
	lsls r3, r3, #18
	add r6, sp, #44
	ldr r1, [r3, #60]
	movs r3, #15
	str r3, [r6]
	str r3, [r6, #4]
	str r3, [r6, #8]
	str r3, [r6, #12]
	str r3, [r6, #16]
	str r3, [r6, #20]
	str r3, [r6, #24]
	str r3, [r6, #28]
	str r3, [r6, #32]
	str r3, [r6, #36]
	str r3, [r6, #40]
	str r3, [r6, #44]
	str r3, [r6, #48]
	str r3, [r6, #52]
	str r3, [r6, #56]
	str r3, [r6, #60]
	ldr r3, .L_0803bd9c
	movs r2, #0
	mov r12, r3
	mov r3, sp
	adds r3, #28
	mov r9, r2
	str r2, [sp, #16]
	str r3, [sp, #12]
	movs r2, #36
	movs r4, #0
	add r2, sp
	adds r5, r0, #0
	movs r7, #0
	movs r0, #0
	mov r11, r2
	mov r10, r4
.L_0803bbb8:
	movs r2, #244
	lsls r3, r5, #1
	lsls r2, r2, #4
	adds r3, r3, r2
	ldrh r2, [r1, r3]
	movs r3, #128
	lsls r3, r3, #1
	adds r5, #1
	adds r3, #255
	ands r5, r3
	cmp r2, #31
	bls .L_0803bbfc
	cmp r2, #176
	beq .L_0803bbfc
	cmp r2, #32
	bne .L_0803bbde
	adds r7, #5
	adds r0, #1
	b .L_0803bbb8
.L_0803bbde:
	ldr r3, .L_0803bda0
	subs r2, #32
	lsls r2, r2, #5
	ldrh r2, [r3, r2]
	movs r3, #240
	lsls r3, r3, #4
	adds r3, #60
	ldrh r3, [r3, r1]
	cmp r3, #1
	beq .L_0803bbf6
	cmp r3, #5
	bne .L_0803bbf8
.L_0803bbf6:
	adds r2, #1
.L_0803bbf8:
	adds r7, r7, r2
	b .L_0803bbb8
.L_0803bbfc:
	cmp r2, #28
	bhi .L_0803bbb8
	lsls r3, r2, #2
	mov r2, r12
	ldr r3, [r3, r2]
	mov pc, r3
.L_0803bc08:
	.4byte .L_0803bcb2
	.4byte .L_0803bccc
	.4byte .L_0803bbb8
	.4byte .L_0803bc7c
	.4byte .L_0803bbb8
	.4byte .L_0803bbb8
	.4byte .L_0803bbb8
	.4byte .L_0803bbb8
	.4byte .L_0803bd04
	.4byte .L_0803bd10
	.4byte .L_0803bd04
	.4byte .L_0803bbb8
	.4byte .L_0803bbb8
	.4byte .L_0803bbb8
	.4byte .L_0803bcfa
	.4byte .L_0803bd04
	.4byte .L_0803bbb8
	.4byte .L_0803bd04
	.4byte .L_0803bbb8
	.4byte .L_0803bbb8
	.4byte .L_0803bbb8
	.4byte .L_0803bbb8
	.4byte .L_0803bbb8
	.4byte .L_0803bbb8
	.4byte .L_0803bbb8
	.4byte .L_0803bbb8
	.4byte .L_0803bbb8
	.4byte .L_0803bbb8
	.4byte .L_0803bcfa
.L_0803bc7c:
	mov r3, r11
	mov r2, r10
	adds r0, #1
	strh r0, [r3, r2]
	ldr r3, [sp, #12]
	strh r7, [r3, r2]
	cmp r4, #0
	bne .L_0803bc92
	cmp r9, r7
	bcs .L_0803bc92
	mov r9, r7
.L_0803bc92:
	ldr r3, [sp, #16]
	cmp r3, #2
	bhi .L_0803bca0
	adds r3, #1
	str r3, [sp, #16]
	lsls r3, r3, #1
	mov r10, r3
.L_0803bca0:
	lsls r2, r4, #2
	ldr r3, [r6, r2]
	movs r0, #0
	adds r3, #15
	str r3, [r6, r2]
	ldr r2, .L_0803bd9c
	movs r7, #0
	mov r12, r2
	b .L_0803bbb8
.L_0803bcb2:
	mov r3, r11
	mov r2, r10
	adds r0, #1
	strh r0, [r3, r2]
	ldr r3, [sp, #12]
	strh r7, [r3, r2]
	cmp r4, #0
	bne .L_0803bcc8
	cmp r9, r7
	bcs .L_0803bcc8
	mov r9, r7
.L_0803bcc8:
	adds r4, #1
	b .L_0803bd34
.L_0803bccc:
	mov r3, r11
	mov r2, r10
	adds r0, #1
	strh r0, [r3, r2]
	ldr r3, [sp, #12]
	strh r7, [r3, r2]
	cmp r4, #0
	bne .L_0803bce2
	cmp r9, r7
	bcs .L_0803bce2
	mov r9, r7
.L_0803bce2:
	adds r4, #1
	str r0, [sp, #8]
	str r1, [sp, #4]
	str r4, [sp, #0]
	bl UiWork_ResetCounters
	ldr r3, .L_0803bd9c
	ldr r0, [sp, #8]
	mov r12, r3
	ldr r1, [sp, #4]
	ldr r4, [sp, #0]
	b .L_0803bbb8
.L_0803bcfa:
	movs r3, #128
	lsls r3, r3, #1
	adds r5, #1
	adds r3, #255
	ands r5, r3
.L_0803bd04:
	movs r3, #128
	lsls r3, r3, #1
	adds r5, #1
	adds r3, #255
	ands r5, r3
	b .L_0803bbb8
.L_0803bd10:
	movs r2, #244
	lsls r3, r5, #1
	lsls r2, r2, #4
	adds r3, r3, r2
	ldrh r2, [r1, r3]
	movs r3, #240
	lsls r3, r3, #4
	adds r3, #60
	adds r3, r3, r1
	strh r2, [r3]
	movs r3, #128
	ldr r2, .L_0803bd9c
	lsls r3, r3, #1
	adds r5, #1
	adds r3, #255
	ands r5, r3
	mov r12, r2
	b .L_0803bbb8
.L_0803bd34:
	ldrb r3, [r1, #4]
	cmp r3, #0
	beq .L_0803bd3e
	movs r3, #2
	add r9, r3
.L_0803bd3e:
	movs r0, #0
	cmp r0, r4
	bcs .L_0803bd6a
	adds r1, r6, #0
	adds r5, r1, #0
.L_0803bd48:
	cmp r0, #0
	bne .L_0803bd54
	ldr r3, [r5]
	ldr r2, [sp, #20]
	str r3, [r2]
	b .L_0803bd62
.L_0803bd54:
	ldr r2, [sp, #20]
	ldr r3, [r2]
	ldr r2, [r1]
	cmp r3, r2
	bcs .L_0803bd62
	ldr r3, [sp, #20]
	str r2, [r3]
.L_0803bd62:
	adds r0, #1
	adds r1, #4
	cmp r0, r4
	bcc .L_0803bd48
.L_0803bd6a:
	ldr r3, [sp, #24]
	mov r2, r9
	str r2, [r3]
	mov r3, r9
	adds r3, #19
	lsrs r3, r3, #3
	lsls r3, r3, #3
	subs r3, #16
	mov r2, r8
	mov r9, r3
	cmp r2, #0
	beq .L_0803bdd2
	movs r6, #0
	movs r5, #0
.L_0803bd86:
	mov r2, r11
	ldrh r3, [r5, r2]
	cmp r3, #1
	bhi .L_0803bda4
	ldr r3, .L_0803bd98
	mov r2, r8
	strh r3, [r2]
	b .L_0803bdc4
	.2byte 0x0000
.L_0803bd98:
	.4byte 0x00000000
.L_0803bd9c:
	.4byte .L_0803bc08
.L_0803bda0:
	.4byte UiText_Glyphs
.L_0803bda4:
	ldr r2, [sp, #12]
	ldrh r3, [r5, r2]
	mov r2, r9
	subs r0, r2, r3
	subs r0, #4
	cmp r0, #0
	bge .L_0803bdb4
	movs r0, #0
.L_0803bdb4:
	mov r3, r11
	ldrh r1, [r5, r3]
	lsls r0, r0, #8
	subs r1, #1
	bl Math_Div
	mov r2, r8
	strh r0, [r2]
.L_0803bdc4:
	movs r3, #2
	add r8, r3
	ldr r2, [sp, #16]
	adds r6, #1
	adds r5, #2
	cmp r6, r2
	bls .L_0803bd86
.L_0803bdd2:
	bl UiWork_ResetCounters
	add sp, #108
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
