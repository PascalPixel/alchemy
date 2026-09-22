.syntax unified
	.thumb
	.set sub_0801314c, 0x0801314c
	.set sub_080140d8, 0x080140d8
	.set sub_080142d4, 0x080142d4
	.set sub_080145a8, 0x080145a8
	.set sub_08014694, 0x08014694
	.set sub_0801475c, 0x0801475c
	.set sub_08014cc0, 0x08014cc0
	.set sub_08014d00, 0x08014d00
	.set sub_080cad84, 0x080cad84
	.set sub_080dc390, 0x080dc390
	.set sub_080eaf98, 0x080eaf98
	.set sub_080eb01c, 0x080eb01c
	.global Overlay_080d1a18
Overlay_080d1a18:
	push	{r5, r6, r7, lr}
	adds	r6, r0, #0
	movs	r1, #28
	movs	r0, #144
	sub	sp, #4
	bl	sub_08014d00
	movs	r1, #128
	adds	r7, r0, #0
	lsls	r1, r1, #3
	movs	r0, #56
	bl	sub_08014cc0
	ldr	r3, [pc, #132]
	mov	r5, sp
	str	r3, [r5, #0]
	movs	r2, #133
	movs	r3, #128
	adds	r4, r0, #0
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, r5, #0
	adds	r1, r4, #0
	adds	r2, #128
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r1, #128
	adds	r2, r4, #0
	lsls	r1, r1, #2
	movs	r0, #94
	bl	sub_080142d4
	movs	r0, #56
	bl	sub_0801314c
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #84]
	bl	sub_080145a8
	movs	r2, #252
	movs	r3, #128
	lsls	r2, r2, #6
	lsls	r3, r3, #19
	adds	r2, #158
	adds	r3, #80
	strh	r2, [r3, #0]
	movs	r2, #16
	adds	r3, #2
	strh	r2, [r3, #0]
	movs	r2, #31
	adds	r3, #2
	strh	r2, [r3, #0]
	movs	r3, #0
	str	r3, [r5, #0]
	movs	r2, #133
	movs	r3, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, r5, #0
	adds	r1, r7, #0
	adds	r2, #7
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	cmp	r6, #0
	bne.n	.L_080d1ab0
	ldr	r3, [pc, #28]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl	sub_080cad84
	adds	r6, r0, #0
.L_080d1ab0:
	str	r6, [r7, #24]
	add	sp, #4
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x11111111
	.4byte 0x080d1841
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r0, [pc, #8]
	bl	sub_08014694
	pop	{pc}
	movs	r0, r0
	.2byte 0x1841
	.2byte 0x080d
	push	{lr}
	ldr	r0, [pc, #8]
	bl	sub_0801475c
	pop	{pc}
	movs	r0, r0
	.2byte 0x1841
	.2byte 0x080d
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r2, r3, #0
	ldr	r1, [r3, #32]
	adds	r2, #144
	ldr	r2, [r2, #0]
	adds	r3, r1, #0
	adds	r3, #228
	mov	sl, r2
	movs	r2, #2
	ldrsh	r0, [r3, r2]
	sub	sp, #8
	mov	fp, r0
	movs	r2, #6
	ldrsh	r0, [r3, r2]
	mov	r3, sl
	str	r0, [sp, #4]
	ldr	r2, [r3, #24]
	cmp	r2, #0
	bne.n	.L_080d1b1c
	b.n	.L_080d1cb2
.L_080d1b1c:
	movs	r0, #10
	ldrsh	r6, [r2, r0]
	ldr	r3, [r2, #16]
	ldr	r2, [r2, #12]
	movs	r0, #212
	subs	r3, r3, r2
	asrs	r5, r3, #16
	ldr	r3, [pc, #184]
	movs	r2, #189
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldrh	r3, [r3, #0]
	movs	r2, #8
	lsrs	r3, r3, #5
	lsls	r0, r0, #1
	adds	r2, r2, r6
	mov	r9, r3
	mov	r8, r2
	adds	r3, r1, r0
	ldr	r4, [r3, #0]
	mov	r3, r8
	cmp	r2, #0
	bge.n	.L_080d1b4e
	adds	r3, r6, #0
	adds	r3, #23
.L_080d1b4e:
	asrs	r3, r3, #4
	lsls	r3, r3, #2
	adds	r2, r4, r3
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L_080d1b5c
	adds	r3, #15
.L_080d1b5c:
	asrs	r3, r3, #4
	ldr	r0, [pc, #136]
	lsls	r1, r3, #9
	adds	r7, r6, #0
	adds	r3, r2, r1
	subs	r7, #8
	adds	r2, r3, r0
	mov	ip, r0
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_080d1b74
	adds	r3, r6, #7
.L_080d1b74:
	asrs	r3, r3, #4
	ldr	r0, [pc, #112]
	lsls	r3, r3, #2
	adds	r3, r4, r3
	adds	r3, r3, r1
	ldrb	r2, [r2, #3]
	adds	r1, r3, r0
	movs	r0, #1
	adds	r3, r0, #0
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080d1b96
	ldrb	r2, [r1, #3]
	adds	r3, r0, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080d1c02
.L_080d1b96:
	movs	r3, #128
	mov	r0, sl
	lsls	r3, r3, #23
	str	r3, [r0, #4]
	movs	r3, #128
	lsls	r3, r3, #3
	str	r3, [r0, #8]
	ldr	r3, [pc, #52]
	ldrh	r1, [r0, #8]
	mov	r2, r9
	ands	r2, r3
	mov	r3, ip
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r0, #8]
	mov	r3, fp
	subs	r2, r6, r3
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #248
	adds	r2, r2, r3
	ldr	r3, [pc, #28]
	ldrh	r1, [r0, #6]
	ands	r2, r3
	ldr	r3, [pc, #36]
	str	r4, [sp, #0]
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r0, #6]
	movs	r2, #240
	adds	r3, r5, #0
	ands	r3, r2
	ldr	r2, [sp, #4]
	movs	r1, #1
	b.n	.L_080d1bf0
	.4byte 0x000003ff
	.4byte 0x000001ff
	.4byte 0x020036e0
	.4byte 0xfffffc00
	.2byte 0xfe00
	.2byte 0xffff
.L_080d1bf0:
	subs	r3, r3, r2
	adds	r3, #224
	strb	r3, [r0, #4]
	movs	r3, #12
	adds	r3, r3, r0
	mov	sl, r3
	bl	sub_080140d8
	ldr	r4, [sp, #0]
.L_080d1c02:
	mov	r3, r8
	cmp	r3, #0
	bge.n	.L_080d1c0c
	adds	r3, r6, #0
	adds	r3, #23
.L_080d1c0c:
	asrs	r3, r3, #4
	lsls	r3, r3, #2
	adds	r2, r4, r3
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L_080d1c1a
	adds	r3, #15
.L_080d1c1a:
	asrs	r3, r3, #4
	ldr	r0, [pc, #128]
	lsls	r1, r3, #9
	adds	r3, r2, r1
	adds	r2, r3, r0
	adds	r3, r7, #0
	mov	ip, r0
	cmp	r3, #0
	bge.n	.L_080d1c2e
	adds	r3, r6, #7
.L_080d1c2e:
	asrs	r3, r3, #4
	ldr	r0, [pc, #108]
	lsls	r3, r3, #2
	adds	r3, r4, r3
	adds	r3, r3, r1
	ldrb	r2, [r2, #3]
	adds	r1, r3, r0
	movs	r0, #1
	adds	r3, r0, #0
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080d1c50
	ldrb	r2, [r1, #3]
	adds	r3, r0, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080d1cb2
.L_080d1c50:
	movs	r3, #128
	mov	r0, sl
	lsls	r3, r3, #23
	str	r3, [r0, #4]
	movs	r3, #128
	lsls	r3, r3, #3
	str	r3, [r0, #8]
	ldr	r3, [pc, #56]
	mov	r2, r9
	ands	r2, r3
	mov	r9, r2
	ldr	r3, [pc, #60]
	ldrh	r2, [r0, #8]
	ldrh	r1, [r0, #6]
	ands	r3, r2
	mov	r2, r9
	orrs	r3, r2
	strh	r3, [r0, #8]
	mov	r3, fp
	subs	r2, r6, r3
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #248
	adds	r2, r2, r3
	ldr	r3, [pc, #24]
	ands	r2, r3
	mov	r3, ip
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r0, #6]
	ldr	r2, [sp, #4]
	movs	r3, #240
	ands	r5, r3
	subs	r3, r5, r2
	b.n	.L_080d1ca8
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.2byte 0xfc00
	.2byte 0xffff
.L_080d1ca8:
	adds	r3, #240
	strb	r3, [r0, #4]
	movs	r1, #1
	bl	sub_080140d8
.L_080d1cb2:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	push	{r5, r6, lr}
	adds	r5, r0, #0
	movs	r1, #28
	movs	r0, #144
	sub	sp, #4
	bl	sub_08014d00
	movs	r1, #128
	adds	r6, r0, #0
	lsls	r1, r1, #3
	movs	r0, #56
	bl	sub_08014cc0
	movs	r3, #0
	adds	r4, r0, #0
	mov	r0, sp
	str	r3, [r0, #0]
	movs	r2, #133
	movs	r3, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r1, r4, #0
	adds	r2, #128
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r2, r4, #0
	movs	r0, #94
	bl	sub_080142d4
	movs	r1, #144
	ldr	r0, [pc, #40]
	lsls	r1, r1, #3
	bl	sub_080145a8
	movs	r0, #56
	bl	sub_0801314c
	cmp	r5, #0
	bne.n	.L_080d1d24
	ldr	r3, [pc, #24]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl	sub_080cad84
	adds	r5, r0, #0
.L_080d1d24:
	str	r5, [r6, #24]
	add	sp, #4
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x080d1ae5
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r0, [pc, #8]
	bl	sub_08014694
	pop	{pc}
	movs	r0, r0
	.2byte 0x1ae5
	.2byte 0x080d
	push	{lr}
	ldr	r0, [pc, #8]
	bl	sub_0801475c
	pop	{pc}
	movs	r0, r0
	.2byte 0x1ae5
	.2byte 0x080d
	push	{r5, lr}
	movs	r1, #128
	adds	r5, r0, #0
	lsls	r1, r1, #3
	movs	r0, #56
	sub	sp, #4
	bl	sub_08014cc0
	adds	r4, r0, #0
	cmp	r5, #0
	bne.n	.L_080d1d78
	movs	r3, #128
	movs	r2, #133
	mov	r0, sp
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	str	r5, [r0, #0]
	b.n	.L_080d1d86
.L_080d1d78:
	ldr	r3, [pc, #44]
	mov	r0, sp
	str	r3, [r0, #0]
	movs	r2, #133
	movs	r3, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #24
.L_080d1d86:
	adds	r3, #212
	adds	r1, r4, #0
	adds	r2, #128
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r2, r4, #0
	movs	r0, #94
	bl	sub_080142d4
	movs	r0, #56
	bl	sub_0801314c
	add	sp, #4
	pop	{r5, pc}
	movs	r0, r0
	.2byte 0x0101
	.2byte 0x0101
	push	{r5, lr}
	adds	r5, r0, #0
	ldr	r0, [pc, #56]
	movs	r4, #189
	lsls	r4, r4, #1
	adds	r0, r0, r4
	ldrh	r0, [r0, #0]
	sub	sp, #4
	lsrs	r0, r0, #5
	str	r0, [sp, #0]
	adds	r0, r5, #0
	bl	sub_080eaf98
	movs	r3, #0
	strh	r3, [r5, #30]
	ldrb	r3, [r5, #9]
	movs	r2, #13
	negs	r2, r2
	ldrb	r1, [r5, #5]
	ands	r2, r3
	movs	r3, #4
	orrs	r2, r3
	movs	r3, #33
	negs	r3, r3
	ands	r3, r1
	strb	r3, [r5, #5]
	movs	r3, #15
	ands	r2, r3
	strb	r2, [r5, #9]
	add	sp, #4
	pop	{r5, pc}
	movs	r0, r0
	.2byte 0x36e0
	.2byte 0x0200
	push	{r5, r6, lr}
	sub	sp, #12
	mov	r5, sp
	adds	r6, r0, #0
	adds	r0, r5, #0
	str	r1, [r5, #0]
	str	r2, [r5, #4]
	str	r3, [r5, #8]
	bl	sub_080dc390
	ldr	r3, [r5, #0]
	adds	r0, r6, #0
	str	r3, [r6, #12]
	ldr	r3, [r5, #8]
	str	r3, [r6, #16]
	bl	sub_080eb01c
	add	sp, #12
	pop	{r5, r6, pc}
