.syntax unified
	.thumb
	.global Func_0802cd94
	.thumb_func
Func_0802cd94:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	mov r12, r3
	mov r4, r12
	adds r4, #216
	ldr r3, [r4]
	cmp r3, #0
	beq .L_0802ce48
	ldrh r3, [r4, #10]
	cmp r3, #0
	bne .L_0802ce48
.L_0802cdae:
	ldrh r2, [r4, #8]
	adds r3, r2, #0
	cmp r3, #0
	bne .L_0802ce3e
	ldr r0, [r4, #4]
	movs r2, #255
	ldrh r1, [r0]
	lsls r2, r2, #8
	adds r2, #255
	adds r0, #2
	cmp r1, r2
	bne .L_0802cdcc
	ldr r3, [r4]
	str r3, [r4, #4]
	b .L_0802cdae
.L_0802cdcc:
	movs r3, #255
	lsls r3, r3, #8
	movs r2, #254
	ands r3, r1
	lsls r2, r2, #8
	cmp r3, r2
	bne .L_0802cdec
	movs r2, #255
	ands r2, r1
	cmp r2, #255
	beq .L_0802ce48
	ldr r3, [r4]
	lsls r2, r2, #2
	adds r3, r3, r2
	str r3, [r4, #4]
	b .L_0802cdae
.L_0802cdec:
	movs r3, #240
	lsls r3, r3, #8
	movs r2, #192
	ands r3, r1
	lsls r2, r2, #6
	cmp r3, r2
	bne .L_0802ce12
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #80
	strh r1, [r3]
	movs r3, #8
	adds r3, #255
	add r3, r12
	strb r1, [r3]
	ldr r3, [r4, #4]
	adds r3, #2
	str r3, [r4, #4]
	b .L_0802cdae
.L_0802ce12:
	movs r3, #8
	adds r3, #255
	add r3, r12
	ldrb r2, [r3]
	movs r3, #192
	ands r3, r2
	cmp r3, #64
	bne .L_0802ce2a
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #82
	b .L_0802ce30
.L_0802ce2a:
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #84
.L_0802ce30:
	strh r1, [r3]
	ldrh r3, [r0]
	strh r3, [r4, #8]
	ldr r3, [r4, #4]
	adds r3, #4
	str r3, [r4, #4]
	b .L_0802cdae
.L_0802ce3e:
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r3, r2, r1
	strh r3, [r4, #8]
.L_0802ce48:
	pop {pc}
	.2byte 0x0000
