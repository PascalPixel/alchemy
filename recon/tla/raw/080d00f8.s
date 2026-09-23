.syntax unified
	.thumb
	.set sub_080145a8, 0x080145a8
	.set sub_08014d00, 0x08014d00
	.global Func_080d00f8
	.thumb_func
Func_080d00f8:
	push	{r5, r6, lr}
	movs	r1, #168
	adds	r6, r0, #0
	lsls	r1, r1, #3
	movs	r0, #124
	sub	sp, #4
	bl	sub_08014d00
	movs	r3, #128
	adds	r4, r0, #0
	movs	r5, #0
	mov	r0, sp
	lsls	r3, r3, #19
	str	r5, [r0, #0]
	adds	r3, #212
	adds	r1, r4, #0
	ldr	r2, [pc, #88]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r1, #165
	movs	r2, #160
	lsls	r1, r1, #3
	lsls	r2, r2, #3
	adds	r3, r4, r1
	adds	r2, #42
	strh	r6, [r3, #0]
	adds	r3, r4, r2
	strh	r5, [r3, #0]
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #52
	adds	r2, r4, r3
	movs	r3, #249
	lsls	r3, r3, #6
	adds	r3, #255
	strh	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #74
	ldrh	r2, [r3, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	ands	r3, r2
	ldr	r2, [pc, #32]
	adds	r1, #14
	adds	r4, r4, r1
	orrs	r3, r2
	strh	r3, [r4, #0]
	subs	r1, #182
	ldr	r0, [pc, #28]
	bl	sub_080145a8
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #118
	ldr	r0, [pc, #20]
	bl	sub_080145a8
	add	sp, #4
	b.n	.L_080d0180
	.4byte 0x00000001
	.4byte 0x85000150
	.4byte 0x080cf78d
	.2byte 0xf6fd
	.2byte 0x080c
.L_080d0180:
	pop	{r5, r6, pc}
	.align 2, 0
