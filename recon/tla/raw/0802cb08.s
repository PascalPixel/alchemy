.syntax unified
	.thumb
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_0801475c, 0x0801475c
	.set sub_0801587c, 0x0801587c
	.set sub_0802c4d8, 0x0802c4d8
	.global Func_0802cb08
	.thumb_func
Func_0802cb08:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #32]
	ldr	r2, [pc, #60]
	ldr	r3, [pc, #64]
	movs	r0, #1
	str	r3, [r2, #0]
	movs	r3, #130
	lsls	r3, r3, #1
	adds	r2, r1, r3
	movs	r3, #0
	strh	r3, [r2, #0]
	movs	r3, #131
	lsls	r3, r3, #1
	adds	r2, r1, r3
	movs	r3, #159
	strh	r3, [r2, #0]
	bl	sub_08013560
	ldr	r0, [pc, #36]
	bl	sub_08013300
	ldr	r1, [pc, #36]
	bl	sub_0801587c
	bl	sub_0802c4d8
	ldr	r0, [pc, #28]
	bl	sub_0801475c
	movs	r0, #1
	bl	sub_08013560
	pop	{pc}
	movs	r0, r0
	.4byte 0x030011f8
	.4byte 0x0802c98d
	.4byte 0x00000198
	.4byte 0x02010000
	.2byte 0xcb65
	.2byte 0x0802
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	ldr	r2, [pc, #240]
	mov	r8, r3
	movs	r3, #255
	lsls	r3, r3, #8
	ldr	r7, [pc, #236]
	movs	r1, #0
	movs	r6, #132
	mov	r5, r8
	adds	r3, #255
	mov	sl, r1
	lsls	r6, r6, #24
	mov	ip, r2
	adds	r5, #24
	mov	lr, r3
.L_0802cb8e:
	ldr	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_0802cc50
	ldrh	r3, [r5, #10]
	cmp	r3, #0
	bne.n	.L_0802cc50
.L_0802cb9a:
	ldrh	r2, [r5, #8]
	adds	r3, r2, #0
	cmp	r3, #0
	bne.n	.L_0802cc4a
	ldr	r4, [r5, #4]
	ldrh	r0, [r4, #0]
	adds	r4, #2
	cmp	r0, lr
	bne.n	.L_0802cbb2
	ldr	r3, [r5, #0]
	str	r3, [r5, #4]
	b.n	.L_0802cb9a
.L_0802cbb2:
	movs	r3, #255
	lsls	r3, r3, #8
	movs	r1, #254
	ands	r3, r0
	lsls	r1, r1, #8
	cmp	r3, r1
	bne.n	.L_0802cbd2
	movs	r2, #255
	ands	r2, r0
	cmp	r2, #255
	beq.n	.L_0802cc50
	ldr	r3, [r5, #0]
	lsls	r2, r2, #2
	adds	r3, r3, r2
	str	r3, [r5, #4]
	b.n	.L_0802cb9a
.L_0802cbd2:
	ldrh	r2, [r4, #0]
	adds	r4, #2
	ldrh	r3, [r4, #2]
	ldrh	r1, [r4, #0]
	strh	r3, [r5, #8]
	mov	r4, r8
	ldrb	r3, [r4, #22]
	cmp	r3, #0
	bne.n	.L_0802cc12
	movs	r3, #192
	lsls	r3, r3, #3
	cmp	r0, r3
	bcc.n	.L_0802cc00
	ldr	r4, [pc, #124]
	movs	r3, #128
	lsls	r2, r2, #3
	lsls	r0, r0, #5
	lsls	r1, r1, #5
	lsls	r3, r3, #19
	adds	r3, #212
	adds	r0, r0, r4
	add	r1, ip
	b.n	.L_0802cc3c
.L_0802cc00:
	movs	r3, #128
	lsls	r2, r2, #3
	lsls	r0, r0, #5
	lsls	r1, r1, #5
	lsls	r3, r3, #19
	adds	r3, #212
	add	r0, ip
	add	r1, ip
	b.n	.L_0802cc3c
.L_0802cc12:
	movs	r3, #128
	lsls	r3, r3, #2
	cmp	r0, r3
	bcc.n	.L_0802cc2c
	ldr	r4, [pc, #84]
	movs	r3, #128
	lsls	r2, r2, #4
	lsls	r0, r0, #6
	lsls	r1, r1, #6
	lsls	r3, r3, #19
	adds	r3, #212
	adds	r0, r0, r4
	b.n	.L_0802cc3a
.L_0802cc2c:
	movs	r3, #128
	lsls	r2, r2, #4
	lsls	r0, r0, #6
	lsls	r1, r1, #6
	lsls	r3, r3, #19
	adds	r3, #212
	adds	r0, r0, r7
.L_0802cc3a:
	adds	r1, r1, r7
.L_0802cc3c:
	orrs	r2, r6
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [r5, #4]
	adds	r3, #8
	str	r3, [r5, #4]
	b.n	.L_0802cb9a
.L_0802cc4a:
	mov	r1, lr
	adds	r3, r2, r1
	strh	r3, [r5, #8]
.L_0802cc50:
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	adds	r5, #12
	cmp	r3, #15
	bls.n	.L_0802cb8e
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x06004000
	.4byte 0x06008000
	.4byte 0x0201c000
	.4byte 0x02020000
