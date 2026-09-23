.syntax unified
	.thumb
	.set sub_08015b24, 0x08015b24
	.global Func_08015f0c
	.thumb_func
Func_08015f0c:
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #204
	ldr	r3, [r3, #0]
	adds	r6, r0, #0
	adds	r1, r3, #0
	movs	r0, #0
	movs	r5, #15
	movs	r4, #0
	adds	r1, #30
	adds	r2, r3, #0
.L_08015f24:
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_08015f3a
	ldrb	r3, [r2, #15]
	cmp	r6, r3
	bne.n	.L_08015f3a
	ldrh	r3, [r1, #0]
	cmp	r0, r3
	bcs.n	.L_08015f3a
	adds	r0, r3, #0
	adds	r5, r4, #0
.L_08015f3a:
	adds	r4, #3
	adds	r1, #6
	adds	r2, #3
	cmp	r4, #14
	bls.n	.L_08015f24
	adds	r0, r5, #0
	pop	{r5, r6, pc}
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	sub	sp, #20
	adds	r3, #204
	adds	r6, r0, #0
	ldr	r5, [r3, #0]
	mov	r0, sp
	movs	r3, #0
	str	r3, [r0, #0]
	movs	r2, #133
	movs	r3, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	add	r1, sp, #4
	adds	r2, #4
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r1, #128
	lsls	r1, r1, #19
	movs	r2, #128
	adds	r1, #212
	lsls	r2, r2, #24
.L_08015f78:
	ldr	r3, [r1, #8]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08015f78
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #108]
	add	r1, sp, #4
	adds	r2, #2
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r1, #128
	lsls	r1, r1, #19
	movs	r3, #128
	adds	r1, #212
	lsls	r3, r3, #24
.L_08015f9e:
	ldr	r2, [r1, #8]
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L_08015f9e
	add	r0, sp, #4
	movs	r3, #16
	strb	r3, [r0, #7]
	strh	r2, [r0, #10]
	movs	r3, #128
	movs	r2, #132
	adds	r1, r5, #0
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r1, #60
	adds	r2, #4
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r1, #128
	lsls	r1, r1, #19
	movs	r2, #128
	adds	r1, #212
	lsls	r2, r2, #24
.L_08015fcc:
	ldr	r3, [r1, #8]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08015fcc
	adds	r0, r6, #0
	bl	sub_08015b24
	cmp	r0, #0
	beq.n	.L_08015fe2
	movs	r0, #1
	b.n	.L_08015ff2
.L_08015fe2:
	movs	r3, #16
	adds	r2, r5, r6
	strb	r0, [r5, r6]
	strb	r3, [r2, #15]
	lsls	r3, r6, #1
	adds	r3, r5, r3
	strh	r0, [r3, #30]
	movs	r0, #0
.L_08015ff2:
	add	sp, #20
	pop	{r5, r6, pc}
	movs	r0, r0
	.2byte 0x7d10
	.2byte 0x0801
	push	{lr}
	adds	r4, r2, #0
	movs	r2, #0
	b.n	.L_0801600a
.L_08016004:
	subs	r4, #1
	adds	r0, #1
	adds	r1, #1
.L_0801600a:
	cmp	r4, #0
	beq.n	.L_08016018
	ldrb	r2, [r0, #0]
	ldrb	r3, [r1, #0]
	subs	r2, r2, r3
	cmp	r2, #0
	beq.n	.L_08016004
.L_08016018:
	adds	r0, r2, #0
	pop	{pc}
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #204
	ldr	r3, [r3, #0]
	adds	r5, r0, #0
	adds	r1, r3, #0
	movs	r0, #0
	movs	r4, #0
	adds	r1, #30
	adds	r2, r3, #0
.L_08016032:
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_08016046
	ldrb	r3, [r2, #15]
	cmp	r5, r3
	bne.n	.L_08016046
	ldrh	r3, [r1, #0]
	cmp	r0, r3
	bcs.n	.L_08016046
	adds	r0, r3, #0
.L_08016046:
	adds	r4, #3
	adds	r1, #6
	adds	r2, #3
	cmp	r4, #14
	bls.n	.L_08016032
	pop	{r5, pc}
	.2byte 0x0000
