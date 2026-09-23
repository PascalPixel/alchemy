.syntax unified
	.thumb
	.global Func_080d0954
	.thumb_func
Func_080d0954:
	push	{lr}
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #6
	ldrh	r3, [r3, #0]
	adds	r4, r3, #0
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	mov	ip, r3
.L_080d0968:
	movs	r0, #134
	lsls	r0, r0, #1
	add	r0, ip
	ldrh	r3, [r0, #0]
	cmp	r3, #1
	beq.n	.L_080d09cc
	cmp	r3, #1
	bgt.n	.L_080d097e
	cmp	r3, #0
	beq.n	.L_080d0a1a
	b.n	.L_080d0a24
.L_080d097e:
	cmp	r3, #2
	beq.n	.L_080d09a6
	cmp	r3, #3
	bne.n	.L_080d0a24
	movs	r3, #132
	lsls	r3, r3, #1
	add	r3, ip
	ldrh	r3, [r3, #0]
	cmp	r4, r3
	bcc.n	.L_080d0a24
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r2, [r1, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #248
	ands	r3, r2
	ldr	r2, [pc, #36]
	orrs	r3, r2
	b.n	.L_080d09c0
.L_080d09a6:
	movs	r3, #133
	lsls	r3, r3, #1
	add	r3, ip
	ldrh	r3, [r3, #0]
	cmp	r4, r3
	bcc.n	.L_080d0a24
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r2, [r1, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #248
	ands	r3, r2
.L_080d09c0:
	strh	r3, [r1, #0]
	movs	r3, #9
	strh	r3, [r0, #0]
	b.n	.L_080d0a24
	.2byte 0x0002
	.2byte 0x0000
.L_080d09cc:
	movs	r3, #132
	lsls	r3, r3, #1
	add	r3, ip
	ldrh	r3, [r3, #0]
	cmp	r4, r3
	bcc.n	.L_080d09f8
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r2, [r1, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #248
	ands	r3, r2
	ldr	r2, [pc, #12]
	orrs	r3, r2
	strh	r3, [r1, #0]
	ldrh	r3, [r0, #0]
	adds	r3, #1
	strh	r3, [r0, #0]
	b.n	.L_080d0968
	.2byte 0x0002
	.2byte 0x0000
.L_080d09f8:
	movs	r3, #133
	lsls	r3, r3, #1
	add	r3, ip
	ldrh	r3, [r3, #0]
	cmp	r4, r3
	bcc.n	.L_080d0a24
	movs	r1, #128
	lsls	r1, r1, #19
	ldrh	r2, [r1, #0]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #248
	ands	r3, r2
	strh	r3, [r1, #0]
	movs	r3, #3
	strh	r3, [r0, #0]
	b.n	.L_080d0968
.L_080d0a1a:
	cmp	r4, #158
	bhi.n	.L_080d0a24
	movs	r3, #1
	strh	r3, [r0, #0]
	b.n	.L_080d0968
.L_080d0a24:
	pop	{pc}
	.align 2, 0
