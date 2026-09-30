.syntax unified
	.thumb
	.balign 4
	.global Func_0811d748
	.thumb_func
Func_0811d748:
	push	{r5, r6, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r4, r0, #0
	ldr	r1, [r3, #36]
	cmp	r4, #7
	bhi.n	.L_0811d772
	movs	r5, #128
	movs	r0, #0
	lsls	r5, r5, #1
	movs	r2, #88
.L_0811d75e:
	ldrsh	r3, [r2, r1]
	cmp	r3, #255
	beq.n	.L_0811d782
	cmp	r3, #254
	beq.n	.L_0811d76c
	cmp	r3, r4
	beq.n	.L_0811d790
.L_0811d76c:
	adds	r2, #2
	adds	r0, #1
	b.n	.L_0811d75e
.L_0811d772:
	movs	r5, #192
	movs	r0, #0
	adds	r1, #2
	lsls	r5, r5, #1
	movs	r2, #100
.L_0811d77c:
	ldrsh	r3, [r2, r1]
	cmp	r3, #255
	bne.n	.L_0811d788
.L_0811d782:
	movs	r0, #1
	negs	r0, r0
	b.n	.L_0811d79a
.L_0811d788:
	cmp	r3, #254
	beq.n	.L_0811d794
	cmp	r3, r4
	bne.n	.L_0811d794
.L_0811d790:
	orrs	r0, r5
	b.n	.L_0811d79a
.L_0811d794:
	adds	r2, #2
	adds	r0, #1
	b.n	.L_0811d77c
.L_0811d79a:
	pop	{r5, r6, pc}
	push	{lr}
	adds	r3, r0, #0
	movs	r0, #0
	cmp	r3, #7
	bhi.n	.L_0811d7de
	ldr	r2, [pc, #56]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x0811d7d0
	.4byte 0x0811d7d0
	.4byte 0x0811d7d8
	.4byte 0x0811d7d8
	.4byte 0x0811d7d0
	.4byte 0x0811d7d0
	.4byte 0x0811d7d8
	.2byte 0xd7d0
	.2byte 0x0811
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #1
	b.n	.L_0811d7de
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #4
.L_0811d7de:
	pop	{pc}
	.2byte 0xd7b0
	.2byte 0x0811
	bx	lr
	.2byte 0x0000
