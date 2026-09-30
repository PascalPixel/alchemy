.syntax unified
	.thumb
	.global Func_0803a1c0
	.thumb_func
Func_0803a1c0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r8, r3
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #60]
	sub sp, #4
	str r3, [sp, #0]
	adds r7, r2, #0
	ldr r2, [sp, #0]
	lsls r3, r1, #6
	adds r3, r2, r3
	lsls r2, r0, #1
	adds r6, r3, r2
	adds r5, r6, #0
	adds r5, #8
	cmp r7, #1
	bls .L_0803a2a0
	mov r3, r8
	cmp r3, #1
	bls .L_0803a2a0
	cmp r7, #30
	bhi .L_0803a2a0
	cmp r3, #30
	bhi .L_0803a2a0
	adds r2, r7, #0
	bl Func_08041abc
	ldr r3, .L_0803a234
	subs r2, r7, #2
	strh r3, [r5]
	adds r5, #2
	adds r0, r5, #0
	ldr r1, .L_0803a23c
	mov r10, r2
	bl Func_0803a054
	ldr r3, .L_0803a238
	adds r5, r0, #0
	strh r3, [r5]
	movs r2, #1
	movs r3, #32
	subs r3, r3, r7
	negs r2, r2
	adds r5, #2
	lsls r3, r3, #1
	movs r6, #1
	add r8, r2
	adds r5, r5, r3
	cmp r6, r8
	bcs .L_0803a26e
	b .L_0803a240
	.2byte 0x0000
.L_0803a234:
	.4byte 0x0000f008
.L_0803a238:
	.4byte 0x0000f00a
.L_0803a23c:
	.4byte 0xf009f009
.L_0803a240:
	mov r9, r3
	movs r3, #240
	lsls r3, r3, #8
	adds r3, #14
	mov r11, r3
.L_0803a24a:
	mov r2, r11
	strh r2, [r5]
	adds r5, #2
	cmp r7, #2
	beq .L_0803a260
	adds r0, r5, #0
	ldr r1, .L_0803a298
	mov r2, r10
	bl Func_0803a054
	adds r5, r0, #0
.L_0803a260:
	ldr r3, .L_0803a28c
	adds r6, #1
	strh r3, [r5]
	adds r5, #2
	add r5, r9
	cmp r6, r8
	bcc .L_0803a24a
.L_0803a26e:
	ldr r3, .L_0803a290
	mov r2, r10
	strh r3, [r5]
	adds r5, #2
	adds r0, r5, #0
	ldr r1, .L_0803a29c
	bl Func_0803a054
	ldr r3, .L_0803a294
	adds r5, r0, #0
	strh r3, [r5]
	ldr r2, [sp, #0]
	movs r3, #1
	strb r3, [r2, #3]
	b .L_0803a2a0
.L_0803a28c:
	.4byte 0x0000f00f
.L_0803a290:
	.4byte 0x0000f00b
.L_0803a294:
	.4byte 0x0000f00d
.L_0803a298:
	.4byte 0xf020f020
.L_0803a29c:
	.4byte 0xf00cf00c
.L_0803a2a0:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
