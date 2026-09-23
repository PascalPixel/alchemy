.syntax unified
	.thumb
	.set sub_0800205c, 0x0800205c
	.set sub_0800206c, 0x0800206c
	.set sub_08013164, 0x08013164
	.set sub_08013560, 0x08013560
	.set sub_08014dac, 0x08014dac
	.set sub_0801587c, 0x0801587c
	.set sub_08015ca4, 0x08015ca4
	.set sub_08015e8c, 0x08015e8c
	.set sub_08016150, 0x08016150
	.set sub_08039260, 0x08039260
	.set sub_0803939c, 0x0803939c
	.set sub_0803ae14, 0x0803ae14
	.set sub_08041fa4, 0x08041fa4
	.set sub_080c8698, 0x080c8698
	.set sub_081c0010, 0x081c0010
	.set sub_081c0040, 0x081c0040
	.global Func_08042e28
	.thumb_func
Func_08042e28:
	push	{r5, r6, r7, lr}
	sub	sp, #4
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r2, #30
	movs	r3, #20
	movs	r1, #0
	adds	r5, r0, #0
	movs	r0, #0
	bl	sub_08039260
	adds	r6, r0, #0
	movs	r0, #128
	lsls	r0, r0, #1
	bl	sub_08014dac
	adds	r7, r0, #0
	adds	r1, r7, #0
	ldr	r0, [pc, #132]
	bl	sub_0801587c
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, r7, #0
	ldr	r1, [pc, #120]
	adds	r2, #64
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	cmp	r5, #1
	beq.n	.L_08042e7c
	cmp	r5, #1
	bcc.n	.L_08042e74
	cmp	r5, #2
	beq.n	.L_08042e8a
	b.n	.L_08042e96
.L_08042e74:
	ldr	r0, [pc, #100]
	adds	r1, r6, #0
	movs	r2, #76
	b.n	.L_08042e82
.L_08042e7c:
	ldr	r0, [pc, #96]
	adds	r1, r6, #0
	movs	r2, #92
.L_08042e82:
	movs	r3, #40
	bl	sub_08041fa4
	b.n	.L_08042e96
.L_08042e8a:
	ldr	r0, [pc, #88]
	adds	r1, r6, #0
	movs	r2, #84
	movs	r3, #40
	bl	sub_08041fa4
.L_08042e96:
	ldr	r5, [pc, #80]
	adds	r1, r6, #0
	adds	r0, r5, #0
	movs	r2, #40
	movs	r3, #88
	adds	r5, #1
	bl	sub_08041fa4
	movs	r2, #40
	movs	r3, #104
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl	sub_08041fa4
	movs	r1, #30
	movs	r0, #0
	bl	sub_081c0040
	movs	r0, #149
	lsls	r0, r0, #1
	bl	sub_081c0010
	movs	r0, #10
	bl	sub_08013560
	adds	r0, r7, #0
	bl	sub_08013164
	adds	r0, r6, #0
	add	sp, #4
	pop	{r5, r6, r7, pc}
	.4byte 0x0805f585
	.4byte 0x06001000
	.4byte 0x0000001d
	.4byte 0x0000001e
	.4byte 0x0000001f
	.2byte 0x0020
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r0, #0
	movs	r0, #10
	bl	sub_08013560
	bl	sub_080c8698
	movs	r0, #128
	lsls	r0, r0, #1
	movs	r1, #30
	bl	sub_081c0040
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_0803939c
	adds	r0, r5, #0
	pop	{r5, pc}
	push	{r5, r6, lr}
	mov	r6, fp
	mov	r5, sl
	push	{r5, r6}
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6}
	sub	sp, #8
	adds	r6, r0, #0
	mov	fp, r1
	movs	r1, #4
	ldr	r5, [pc, #188]
	add	r1, sp
	ldrh	r3, [r5, #0]
	mov	sl, r1
	str	r3, [r1, #0]
	strh	r5, [r5, #0]
	mov	r9, sp
	ldr	r2, [pc, #176]
	ldrh	r3, [r2, #0]
	mov	r8, r2
	str	r3, [sp, #0]
	movs	r3, #0
	mov	r1, r8
	strh	r3, [r1, #0]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #176
	movs	r4, #197
	ldrh	r1, [r2, #10]
	lsls	r4, r4, #8
	adds	r4, #255
	adds	r3, r4, #0
	ands	r3, r1
	strh	r3, [r2, #10]
	movs	r0, #254
	ldrh	r1, [r2, #10]
	lsls	r0, r0, #7
	adds	r0, #255
	adds	r3, r0, #0
	ands	r3, r1
	strh	r3, [r2, #10]
	ldrh	r3, [r2, #10]
	adds	r2, #12
	ldrh	r1, [r2, #10]
	adds	r3, r4, #0
	ands	r3, r1
	strh	r3, [r2, #10]
	adds	r3, r0, #0
	ldrh	r1, [r2, #10]
	ands	r3, r1
	strh	r3, [r2, #10]
	ldrh	r3, [r2, #10]
	adds	r2, #12
	ldrh	r1, [r2, #10]
	adds	r3, r4, #0
	ands	r3, r1
	strh	r3, [r2, #10]
	adds	r3, r0, #0
	ldrh	r1, [r2, #10]
	ands	r3, r1
	strh	r3, [r2, #10]
	ldrh	r3, [r2, #10]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldrh	r2, [r3, #10]
	ands	r4, r2
	strh	r4, [r3, #10]
	ldrh	r2, [r3, #10]
	ands	r0, r2
	strh	r0, [r3, #10]
	ldrh	r3, [r3, #10]
	mov	r2, sl
	ldr	r3, [r2, #0]
	strh	r3, [r5, #0]
	ldr	r0, [pc, #64]
	bl	sub_08016150
	mov	r1, fp
	adds	r0, r6, #0
	bl	sub_08015ca4
	adds	r6, r0, #0
	movs	r0, #0
	bl	sub_08016150
	ldrh	r2, [r5, #0]
	mov	r3, sl
	str	r2, [r3, #0]
	strh	r5, [r5, #0]
	mov	r1, r9
	ldr	r3, [r1, #0]
	mov	r1, r8
	strh	r3, [r1, #0]
	strh	r2, [r5, #0]
	adds	r0, r6, #0
	add	sp, #8
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r3}
	mov	fp, r3
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x04000208
	.4byte 0x04000200
	.2byte 0x2dc5
	.2byte 0x0804
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6}
	mov	r6, r8
	push	{r6}
	sub	sp, #8
	adds	r6, r0, #0
	movs	r1, #4
	ldr	r5, [pc, #180]
	add	r1, sp
	ldrh	r3, [r5, #0]
	mov	sl, r1
	str	r3, [r1, #0]
	strh	r5, [r5, #0]
	mov	r9, sp
	ldr	r2, [pc, #168]
	ldrh	r3, [r2, #0]
	mov	r8, r2
	str	r3, [sp, #0]
	movs	r3, #0
	mov	r1, r8
	strh	r3, [r1, #0]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #176
	movs	r4, #197
	ldrh	r1, [r2, #10]
	lsls	r4, r4, #8
	adds	r4, #255
	adds	r3, r4, #0
	ands	r3, r1
	strh	r3, [r2, #10]
	movs	r0, #254
	ldrh	r1, [r2, #10]
	lsls	r0, r0, #7
	adds	r0, #255
	adds	r3, r0, #0
	ands	r3, r1
	strh	r3, [r2, #10]
	ldrh	r3, [r2, #10]
	adds	r2, #12
	ldrh	r1, [r2, #10]
	adds	r3, r4, #0
	ands	r3, r1
	strh	r3, [r2, #10]
	adds	r3, r0, #0
	ldrh	r1, [r2, #10]
	ands	r3, r1
	strh	r3, [r2, #10]
	ldrh	r3, [r2, #10]
	adds	r2, #12
	ldrh	r1, [r2, #10]
	adds	r3, r4, #0
	ands	r3, r1
	strh	r3, [r2, #10]
	adds	r3, r0, #0
	ldrh	r1, [r2, #10]
	ands	r3, r1
	strh	r3, [r2, #10]
	ldrh	r3, [r2, #10]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldrh	r2, [r3, #10]
	ands	r4, r2
	strh	r4, [r3, #10]
	ldrh	r2, [r3, #10]
	ands	r0, r2
	strh	r0, [r3, #10]
	ldrh	r3, [r3, #10]
	mov	r2, sl
	ldr	r3, [r2, #0]
	strh	r3, [r5, #0]
	ldr	r0, [pc, #56]
	bl	sub_08016150
	adds	r0, r6, #0
	bl	sub_08015e8c
	adds	r6, r0, #0
	movs	r0, #0
	bl	sub_08016150
	ldrh	r2, [r5, #0]
	mov	r3, sl
	str	r2, [r3, #0]
	strh	r5, [r5, #0]
	mov	r1, r9
	ldr	r3, [r1, #0]
	mov	r1, r8
	strh	r3, [r1, #0]
	strh	r2, [r5, #0]
	adds	r0, r6, #0
	add	sp, #8
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, pc}
	.4byte 0x04000208
	.4byte 0x04000200
	.2byte 0x2dc5
	.2byte 0x0804
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r7, r1, #0
	movs	r1, #60
	sub	sp, #16
	adds	r5, r0, #0
	bl	sub_0800206c
	movs	r1, #60
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	sub_0800205c
	movs	r3, #100
	adds	r5, r0, #0
	movs	r1, #60
	adds	r0, r6, #0
	muls	r0, r3
	bl	sub_0800205c
	movs	r1, #60
	mov	fp, r0
	adds	r0, r5, #0
	bl	sub_0800206c
	movs	r1, #60
	mov	r9, r0
	adds	r0, r5, #0
	bl	sub_0800205c
	movs	r1, #60
	adds	r5, r0, #0
	bl	sub_0800206c
	movs	r1, #60
	adds	r6, r0, #0
	adds	r0, r5, #0
	bl	sub_0800205c
	mov	sl, sp
	adds	r1, r0, #0
	movs	r2, #3
	mov	r0, sl
	bl	sub_0803ae14
	ldrb	r3, [r0, #0]
	adds	r0, #1
	strb	r3, [r7, #0]
	adds	r5, r7, #1
	ldrb	r3, [r0, #0]
	adds	r6, #100
	strb	r3, [r5, #0]
	adds	r5, #1
	ldrb	r3, [r0, #1]
	adds	r1, r6, #0
	strb	r3, [r5, #0]
	movs	r3, #58
	adds	r5, #1
	strb	r3, [r5, #0]
	mov	r0, sl
	movs	r2, #2
	mov	r8, r3
	bl	sub_0803ae14
	ldrb	r3, [r0, #0]
	adds	r5, #1
	strb	r3, [r5, #0]
	adds	r5, #1
	ldrb	r3, [r0, #1]
	movs	r2, #2
	strb	r3, [r5, #0]
	mov	r3, r8
	adds	r5, #1
	strb	r3, [r5, #0]
	movs	r3, #100
	add	r9, r3
	mov	r1, r9
	mov	r0, sl
	bl	sub_0803ae14
	ldrb	r3, [r0, #0]
	adds	r5, #1
	strb	r3, [r5, #0]
	adds	r5, #1
	ldrb	r3, [r0, #1]
	movs	r2, #2
	strb	r3, [r5, #0]
	mov	r3, r8
	adds	r5, #1
	strb	r3, [r5, #0]
	movs	r3, #100
	add	fp, r3
	mov	r0, sl
	mov	r1, fp
	bl	sub_0803ae14
	ldrb	r3, [r0, #0]
	adds	r5, #1
	strb	r3, [r5, #0]
	adds	r5, #1
	ldrb	r3, [r0, #1]
	add	sp, #16
	strb	r3, [r5, #0]
	movs	r3, #0
	adds	r0, r7, #0
	strb	r3, [r5, #1]
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.align 2, 0
