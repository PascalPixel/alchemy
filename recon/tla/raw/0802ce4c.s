.syntax unified
	.thumb
	.set sub_080145a8, 0x080145a8
	.set sub_08014694, 0x08014694
	.set sub_0801475c, 0x0801475c
	.set sub_08014d00, 0x08014d00
	.global Func_0802ce4c
	.thumb_func
Func_0802ce4c:
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	sub	sp, #4
	adds	r4, r3, #0
	movs	r2, #133
	movs	r3, #128
	adds	r5, r0, #0
	movs	r6, #0
	mov	r0, sp
	adds	r4, #216
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	str	r6, [r0, #0]
	adds	r3, #212
	adds	r1, r4, #0
	adds	r2, #3
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldrh	r3, [r5, #0]
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	beq.n	.L_0802ce8a
	str	r5, [r4, #0]
	str	r5, [r4, #4]
	strh	r6, [r4, #8]
	strh	r6, [r4, #10]
	movs	r6, #1
.L_0802ce8a:
	cmp	r6, #0
	beq.n	.L_0802ce98
	movs	r1, #144
	ldr	r0, [pc, #8]
	lsls	r1, r1, #3
	bl	sub_080145a8
.L_0802ce98:
	add	sp, #4
	pop	{r5, r6, pc}
	.2byte 0xcd95
	.2byte 0x0802
	push	{lr}
	ldr	r0, [pc, #8]
	bl	sub_08014694
	pop	{pc}
	movs	r0, r0
	.2byte 0xcd95
	.2byte 0x0802
	push	{lr}
	ldr	r0, [pc, #8]
	bl	sub_0801475c
	pop	{pc}
	movs	r0, r0
	.2byte 0xcd95
	.2byte 0x0802
	push	{r5, r6, lr}
	movs	r1, #180
	movs	r0, #112
	bl	sub_08014d00
	adds	r5, r0, #0
	adds	r1, r5, #0
	movs	r0, #0
	movs	r4, #0
.L_0802ced2:
	str	r4, [r1, #0]
	strh	r4, [r1, #4]
	strh	r4, [r1, #6]
	strh	r4, [r1, #8]
	strh	r4, [r1, #10]
	movs	r3, #0
.L_0802cede:
	lsrs	r2, r3, #16
	lsls	r3, r2, #1
	movs	r6, #128
	adds	r2, #1
	adds	r3, #12
	lsls	r2, r2, #16
	lsls	r6, r6, #13
	strh	r4, [r1, r3]
	adds	r3, r2, #0
	cmp	r2, r6
	bne.n	.L_0802cede
	movs	r2, #128
	lsls	r2, r2, #9
	movs	r6, #128
	adds	r3, r0, r2
	lsls	r6, r6, #11
	adds	r1, #44
	adds	r0, r3, #0
	cmp	r3, r6
	bne.n	.L_0802ced2
	adds	r2, r5, #0
	adds	r2, #176
	movs	r3, #0
	strh	r3, [r2, #0]
	pop	{r5, r6, pc}
