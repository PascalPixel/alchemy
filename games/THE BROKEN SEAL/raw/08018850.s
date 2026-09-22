.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_08002df0, 0x08002df0
	.set sub_08002f40, 0x08002f40
	.set sub_08004080, 0x08004080
	.set sub_08004938, 0x08004938
	.set sub_080072fc, 0x080072fc
	.set sub_08015e8c, 0x08015e8c
	.set sub_08016584, 0x08016584
	.set sub_080178b0, 0x080178b0
	.global Overlay_08018850
Overlay_08018850:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #32
	str	r1, [sp, #12]
	str	r2, [sp, #8]
	adds	r6, r3, #0
	ldr	r3, [pc, #392]
	ldr	r4, [r3, #0]
	ldr	r3, [pc, #392]
	movs	r2, #0
	mov	sl, r2
	mov	lr, r3
	movs	r2, #24
	movs	r3, #16
	movs	r1, #15
	movs	r5, #0
	add	r2, sp
	add	r3, sp
	mov	fp, r1
	movs	r7, #0
	movs	r1, #0
	mov	r8, r2
	mov	ip, r5
	mov	r9, r3
.L_0801888a:
	movs	r2, #235
	lsls	r3, r0, #1
	lsls	r2, r2, #4
	adds	r3, r3, r2
	ldrh	r2, [r4, r3]
	ldr	r3, [pc, #352]
	adds	r0, #1
	ands	r0, r3
	cmp	r2, #31
	bls.n	.L_080188c6
	cmp	r2, #32
	bne.n	.L_080188a8
	adds	r1, #5
	adds	r5, #1
	b.n	.L_0801888a
.L_080188a8:
	ldr	r3, [pc, #336]
	subs	r2, #32
	lsls	r2, r2, #5
	ldrh	r2, [r3, r2]
	ldr	r3, [pc, #332]
	adds	r3, r4, r3
	ldrh	r3, [r3, #0]
	str	r3, [sp, #0]
	cmp	r3, #1
	beq.n	.L_080188c0
	cmp	r3, #5
	bne.n	.L_080188c2
.L_080188c0:
	adds	r2, #1
.L_080188c2:
	adds	r1, r1, r2
	b.n	.L_0801888a
.L_080188c6:
	cmp	r2, #28
	bhi.n	.L_0801888a
	lsls	r3, r2, #2
	mov	r2, lr
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	ldrh	r2, [r4, #12]
	lsrs	r1, r0, #32
	ldrh	r2, [r4, #12]
	lsrs	r1, r0, #32
	ldrh	r2, [r1, #4]
	lsrs	r1, r0, #32
	ldrh	r0, [r1, #10]
	lsrs	r1, r0, #32
	ldrh	r2, [r1, #4]
	lsrs	r1, r0, #32
	ldrh	r2, [r1, #4]
	lsrs	r1, r0, #32
	ldrh	r2, [r1, #4]
	lsrs	r1, r0, #32
	ldrh	r2, [r1, #4]
	lsrs	r1, r0, #32
	ldrh	r6, [r7, #10]
	lsrs	r1, r0, #32
	ldrh	r6, [r0, #12]
	lsrs	r1, r0, #32
	ldrh	r6, [r7, #10]
	lsrs	r1, r0, #32
	ldrh	r2, [r1, #4]
	lsrs	r1, r0, #32
	ldrh	r2, [r1, #4]
	lsrs	r1, r0, #32
	ldrh	r2, [r1, #4]
	lsrs	r1, r0, #32
	ldrh	r0, [r7, #10]
	lsrs	r1, r0, #32
	ldrh	r6, [r7, #10]
	lsrs	r1, r0, #32
	ldrh	r2, [r1, #4]
	lsrs	r1, r0, #32
	ldrh	r6, [r7, #10]
	lsrs	r1, r0, #32
	ldrh	r2, [r1, #4]
	lsrs	r1, r0, #32
	ldrh	r2, [r1, #4]
	lsrs	r1, r0, #32
	ldrh	r2, [r1, #4]
	lsrs	r1, r0, #32
	ldrh	r2, [r1, #4]
	lsrs	r1, r0, #32
	ldrh	r2, [r1, #4]
	lsrs	r1, r0, #32
	ldrh	r2, [r1, #4]
	lsrs	r1, r0, #32
	ldrh	r2, [r1, #4]
	lsrs	r1, r0, #32
	ldrh	r2, [r1, #4]
	lsrs	r1, r0, #32
	ldrh	r2, [r1, #4]
	lsrs	r1, r0, #32
	ldrh	r2, [r1, #4]
	lsrs	r1, r0, #32
	ldrh	r0, [r7, #10]
	lsrs	r1, r0, #32
	mov	r3, r8
	mov	r2, ip
	adds	r5, #1
	strh	r5, [r3, r2]
	mov	r3, r9
	strh	r1, [r3, r2]
	cmp	r7, r1
	bcs.n	.L_0801895a
	adds	r7, r1, #0
.L_0801895a:
	mov	r3, sl
	cmp	r3, #2
	bhi.n	.L_0801896a
	movs	r1, #1
	add	sl, r1
	mov	r2, sl
	lsls	r2, r2, #1
	mov	ip, r2
.L_0801896a:
	ldr	r2, [pc, #136]
	movs	r3, #15
	movs	r5, #0
	movs	r1, #0
	add	fp, r3
	mov	lr, r2
	b.n	.L_0801888a
	ldr	r3, [pc, #124]
	adds	r0, #1
	ands	r0, r3
	ldr	r3, [pc, #120]
	adds	r0, #1
	ands	r0, r3
	b.n	.L_0801888a
	movs	r2, #235
	lsls	r3, r0, #1
	lsls	r2, r2, #4
	adds	r3, r3, r2
	ldrh	r2, [r4, r3]
	ldr	r3, [pc, #108]
	adds	r3, r3, r4
	strh	r2, [r3, #0]
	ldr	r3, [pc, #96]
	ldr	r2, [pc, #88]
	adds	r0, #1
	ands	r0, r3
	mov	lr, r2
	b.n	.L_0801888a
	mov	r3, r8
	mov	r2, ip
	adds	r5, #1
	strh	r5, [r3, r2]
	mov	r3, r9
	strh	r1, [r3, r2]
	cmp	r7, r1
	bcs.n	.L_080189b4
	adds	r7, r1, #0
.L_080189b4:
	ldr	r1, [pc, #76]
	adds	r3, r4, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080189c0
	adds	r7, #2
.L_080189c0:
	ldr	r2, [sp, #12]
	str	r7, [r2, #0]
	ldr	r1, [sp, #8]
	mov	r3, fp
	str	r3, [r1, #0]
	adds	r3, r7, #0
	adds	r3, #19
	lsrs	r7, r3, #3
	lsls	r3, r7, #3
	adds	r7, r3, #0
	subs	r7, #16
	cmp	r6, #0
	beq.n	.L_08018a3e
	movs	r2, #0
	movs	r5, #0
.L_080189de:
	mov	r1, r8
	ldrh	r3, [r5, r1]
	cmp	r3, #1
	bhi.n	.L_08018a08
	ldr	r3, [pc, #4]
	strh	r3, [r6, #0]
	b.n	.L_08018a34
	.4byte 0x00000000
	.4byte 0x03001e8c
	.4byte 0x080188d4
	.4byte 0x000001ff
	.4byte 0x08032224
	.4byte 0x00000eac
	.2byte 0x0ea4
	.2byte 0x0000
.L_08018a08:
	mov	r1, r9
	ldrh	r3, [r5, r1]
	subs	r0, r7, r3
	subs	r0, #4
	cmp	r0, #0
	bge.n	.L_08018a16
	movs	r0, #0
.L_08018a16:
	mov	r3, r8
	ldrh	r1, [r5, r3]
	lsls	r0, r0, #8
	subs	r1, #1
	str	r2, [sp, #4]
	bl	sub_080022ec
	movs	r1, #192
	lsls	r1, r1, #4
	ldr	r2, [sp, #4]
	cmp	r0, r1
	bls.n	.L_08018a32
	movs	r0, #128
	lsls	r0, r0, #2
.L_08018a32:
	strh	r0, [r6, #0]
.L_08018a34:
	adds	r6, #2
	adds	r2, #1
	adds	r5, #2
	cmp	r2, sl
	bls.n	.L_080189de
.L_08018a3e:
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #92
	str	r1, [sp, #8]
	mov	r8, r3
	ldr	r3, [pc, #492]
	add	r1, sp, #28
	ldr	r5, [r3, #0]
	movs	r3, #15
	str	r3, [r1, #0]
	str	r3, [r1, #4]
	str	r3, [r1, #8]
	str	r3, [r1, #12]
	str	r3, [r1, #16]
	str	r3, [r1, #20]
	str	r3, [r1, #24]
	str	r3, [r1, #28]
	str	r3, [r1, #32]
	str	r3, [r1, #36]
	str	r3, [r1, #40]
	str	r3, [r1, #44]
	str	r3, [r1, #48]
	str	r3, [r1, #52]
	str	r3, [r1, #56]
	str	r3, [r1, #60]
	add	r3, sp, #20
	mov	lr, r2
	mov	r9, r3
	movs	r2, #0
	add	r3, sp, #12
	str	r2, [sp, #4]
	mov	sl, r2
	movs	r4, #0
	movs	r6, #0
	movs	r7, #0
	mov	ip, r2
	mov	fp, r3
.L_08018aa4:
	movs	r2, #235
	lsls	r3, r0, #1
	lsls	r2, r2, #4
	adds	r3, r3, r2
	ldrh	r2, [r5, r3]
	ldr	r3, [pc, #424]
	adds	r0, #1
	ands	r0, r3
	cmp	r2, #31
	bls.n	.L_08018ae0
	cmp	r2, #32
	bne.n	.L_08018ac2
	adds	r4, #5
	adds	r6, #1
	b.n	.L_08018aa4
.L_08018ac2:
	ldr	r3, [pc, #408]
	subs	r2, #32
	lsls	r2, r2, #5
	ldrh	r2, [r3, r2]
	ldr	r3, [pc, #404]
	adds	r3, r5, r3
	ldrh	r3, [r3, #0]
	str	r3, [sp, #0]
	cmp	r3, #1
	beq.n	.L_08018ada
	cmp	r3, #5
	bne.n	.L_08018adc
.L_08018ada:
	adds	r2, #1
.L_08018adc:
	adds	r4, r4, r2
	b.n	.L_08018aa4
.L_08018ae0:
	cmp	r2, #28
	bhi.n	.L_08018aa4
	lsls	r3, r2, #2
	ldr	r2, [pc, #380]
	ldr	r3, [r3, r2]
	mov	pc, r3
	ldrh	r2, [r2, #28]
	lsrs	r1, r0, #32
	ldrh	r4, [r5, #28]
	lsrs	r1, r0, #32
	ldrh	r4, [r4, #20]
	lsrs	r1, r0, #32
	ldrh	r0, [r4, #26]
	lsrs	r1, r0, #32
	ldrh	r4, [r4, #20]
	lsrs	r1, r0, #32
	ldrh	r4, [r4, #20]
	lsrs	r1, r0, #32
	ldrh	r4, [r4, #20]
	lsrs	r1, r0, #32
	ldrh	r4, [r4, #20]
	lsrs	r1, r0, #32
	ldrh	r6, [r3, #30]
	lsrs	r1, r0, #32
	ldrh	r6, [r1, #30]
	lsrs	r1, r0, #32
	ldrh	r6, [r3, #30]
	lsrs	r1, r0, #32
	ldrh	r4, [r4, #20]
	lsrs	r1, r0, #32
	ldrh	r4, [r4, #20]
	lsrs	r1, r0, #32
	ldrh	r4, [r4, #20]
	lsrs	r1, r0, #32
	ldrh	r6, [r0, #30]
	lsrs	r1, r0, #32
	ldrh	r6, [r3, #30]
	lsrs	r1, r0, #32
	ldrh	r4, [r4, #20]
	lsrs	r1, r0, #32
	ldrh	r6, [r3, #30]
	lsrs	r1, r0, #32
	ldrh	r4, [r4, #20]
	lsrs	r1, r0, #32
	ldrh	r4, [r4, #20]
	lsrs	r1, r0, #32
	ldrh	r4, [r4, #20]
	lsrs	r1, r0, #32
	ldrh	r4, [r4, #20]
	lsrs	r1, r0, #32
	ldrh	r4, [r4, #20]
	lsrs	r1, r0, #32
	ldrh	r4, [r4, #20]
	lsrs	r1, r0, #32
	ldrh	r4, [r4, #20]
	lsrs	r1, r0, #32
	ldrh	r4, [r4, #20]
	lsrs	r1, r0, #32
	ldrh	r4, [r4, #20]
	lsrs	r1, r0, #32
	ldrh	r4, [r4, #20]
	lsrs	r1, r0, #32
	ldrh	r6, [r0, #30]
	lsrs	r1, r0, #32
	mov	r3, r9
	mov	r2, ip
	adds	r6, #1
	strh	r6, [r3, r2]
	mov	r3, fp
	strh	r4, [r3, r2]
	cmp	r7, #0
	bne.n	.L_08018b76
	cmp	sl, r4
	bcs.n	.L_08018b76
	mov	sl, r4
.L_08018b76:
	ldr	r3, [sp, #4]
	cmp	r3, #2
	bhi.n	.L_08018b84
	adds	r3, #1
	str	r3, [sp, #4]
	lsls	r3, r3, #1
	mov	ip, r3
.L_08018b84:
	lsls	r2, r7, #2
	ldr	r3, [r1, r2]
	adds	r3, #15
	movs	r6, #0
	movs	r4, #0
	str	r3, [r1, r2]
	b.n	.L_08018aa4
	mov	r2, r9
	mov	r3, ip
	adds	r6, #1
	strh	r6, [r2, r3]
	mov	r2, fp
	strh	r4, [r2, r3]
	cmp	r7, #0
	bne.n	.L_08018ba8
	cmp	sl, r4
	bcs.n	.L_08018ba8
	mov	sl, r4
.L_08018ba8:
	adds	r7, #1
	b.n	.L_08018be6
	mov	r2, r9
	mov	r3, ip
	adds	r6, #1
	strh	r6, [r2, r3]
	mov	r2, fp
	strh	r4, [r2, r3]
	cmp	r7, #0
	bne.n	.L_08018bc2
	cmp	sl, r4
	bcs.n	.L_08018bc2
	mov	sl, r4
.L_08018bc2:
	adds	r7, #1
	b.n	.L_08018aa4
	ldr	r3, [pc, #144]
	adds	r0, #1
	ands	r0, r3
	b.n	.L_08018bde
	movs	r2, #235
	lsls	r3, r0, #1
	lsls	r2, r2, #4
	adds	r3, r3, r2
	ldrh	r2, [r5, r3]
	ldr	r3, [pc, #132]
	adds	r3, r5, r3
	strh	r2, [r3, #0]
.L_08018bde:
	ldr	r3, [pc, #120]
	adds	r0, #1
	ands	r0, r3
	b.n	.L_08018aa4
.L_08018be6:
	ldr	r2, [pc, #128]
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08018bf4
	movs	r3, #2
	add	sl, r3
.L_08018bf4:
	movs	r4, #0
	cmp	r4, r7
	bcs.n	.L_08018c20
	adds	r0, r1, #0
	adds	r5, r0, #0
.L_08018bfe:
	cmp	r4, #0
	bne.n	.L_08018c0a
	ldr	r3, [r5, #0]
	mov	r2, lr
	str	r3, [r2, #0]
	b.n	.L_08018c18
.L_08018c0a:
	mov	r2, lr
	ldr	r3, [r2, #0]
	ldr	r2, [r0, #0]
	cmp	r3, r2
	bcs.n	.L_08018c18
	mov	r3, lr
	str	r2, [r3, #0]
.L_08018c18:
	adds	r4, #1
	adds	r0, #4
	cmp	r4, r7
	bcc.n	.L_08018bfe
.L_08018c20:
	ldr	r3, [sp, #8]
	mov	r2, sl
	str	r2, [r3, #0]
	mov	r3, sl
	adds	r3, #19
	lsrs	r3, r3, #3
	lsls	r3, r3, #3
	subs	r3, #16
	mov	r2, r8
	mov	sl, r3
	cmp	r2, #0
	beq.n	.L_08018c9a
	movs	r6, #0
	movs	r5, #0
.L_08018c3c:
	mov	r2, r9
	ldrh	r3, [r5, r2]
	cmp	r3, #1
	bhi.n	.L_08018c6c
	ldr	r3, [pc, #8]
	mov	r2, r8
	strh	r3, [r2, #0]
	movs	r3, #2
	b.n	.L_08018c8e
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x03001e8c
	.4byte 0x000001ff
	.4byte 0x08032224
	.4byte 0x00000eac
	.4byte 0x08018aec
	.2byte 0x0ea4
	.2byte 0x0000
.L_08018c6c:
	mov	r2, fp
	ldrh	r3, [r5, r2]
	mov	r2, sl
	subs	r0, r2, r3
	subs	r0, #4
	cmp	r0, #0
	bge.n	.L_08018c7c
	movs	r0, #0
.L_08018c7c:
	mov	r3, r9
	ldrh	r1, [r5, r3]
	lsls	r0, r0, #8
	subs	r1, #1
	bl	sub_080022ec
	mov	r2, r8
	movs	r3, #2
	strh	r0, [r2, #0]
.L_08018c8e:
	add	r8, r3
	ldr	r2, [sp, #4]
	adds	r6, #1
	adds	r5, #2
	cmp	r6, r2
	bls.n	.L_08018c3c
.L_08018c9a:
	add	sp, #92
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #144
	str	r2, [sp, #12]
	str	r3, [sp, #8]
	adds	r7, r1, #0
	ldr	r1, [pc, #352]
	ldr	r2, [pc, #352]
	ldr	r6, [r1, #0]
	adds	r3, r6, r2
	ldrh	r3, [r3, #0]
	ldr	r2, [pc, #348]
	str	r3, [sp, #4]
	adds	r3, r6, r2
	ldrh	r3, [r3, #0]
	mov	fp, r3
	ldr	r3, [sp, #176]
	mov	r8, r0
	cmp	r3, #1
	beq.n	.L_08018d50
	ldrh	r2, [r0, #22]
	movs	r3, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08018d50
	ldr	r3, [r1, #88]
	ldr	r3, [r3, #0]
	cmp	r3, r8
	bne.n	.L_08018d06
	ldr	r0, [pc, #316]
	bl	sub_08002f40
	ldr	r0, [pc, #316]
	bl	sub_08002f40
	movs	r1, #3
	mov	r9, r1
	cmp	r7, #32
	bne.n	.L_08018d06
	b.n	.L_08018ecc
.L_08018d06:
	ldr	r0, [pc, #300]
	bl	sub_08002f40
	movs	r2, #4
	mov	sl, r0
	mov	r9, r2
	cmp	r7, #32
	bne.n	.L_08018d18
	b.n	.L_08018ecc
.L_08018d18:
	ldr	r5, [pc, #284]
	adds	r0, r5, #0
	bl	sub_08004938
	movs	r2, #132
	adds	r6, r0, #0
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	ldr	r3, [pc, #272]
	ldr	r0, [pc, #276]
	adds	r1, r6, #0
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r3, sl
	str	r3, [sp, #0]
	adds	r1, r7, #0
	ldr	r2, [sp, #12]
	ldr	r3, [sp, #8]
	mov	r0, r8
	bl	sub_080072fc
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl	sub_08002df0
	adds	r0, r5, #0
	b.n	.L_08018ee8
.L_08018d50:
	movs	r1, #5
	mov	r9, r1
	cmp	r7, #32
	bne.n	.L_08018d5a
	b.n	.L_08018ecc
.L_08018d5a:
	bl	sub_08015e8c
	adds	r5, r0, #0
	movs	r0, #0
	cmp	r5, #0
	bne.n	.L_08018d68
	b.n	.L_08018ee8
.L_08018d68:
	ldr	r2, [pc, #216]
	subs	r3, r5, r6
	adds	r3, r3, r2
	ldr	r2, [pc, #216]
	adds	r1, r3, #0
	muls	r1, r2
	movs	r3, #1
	movs	r2, #0
	strb	r3, [r5, #5]
	strb	r2, [r5, #4]
	ldr	r3, [sp, #176]
	mov	sl, r1
	cmp	r3, #1
	bne.n	.L_08018d8e
	movs	r1, #1
	movs	r3, #2
	mov	r9, r1
	strb	r3, [r5, #5]
	b.n	.L_08018dd6
.L_08018d8e:
	ldr	r1, [pc, #188]
	adds	r3, r6, r1
	ldrh	r3, [r3, #0]
	cmp	r3, #3
	beq.n	.L_08018dac
	cmp	r3, #3
	bgt.n	.L_08018da2
	cmp	r3, #2
	beq.n	.L_08018dc0
	b.n	.L_08018dc6
.L_08018da2:
	cmp	r3, #4
	beq.n	.L_08018db2
	cmp	r3, #5
	beq.n	.L_08018dbc
	b.n	.L_08018dc6
.L_08018dac:
	movs	r3, #5
	strb	r3, [r5, #5]
	b.n	.L_08018dc6
.L_08018db2:
	movs	r3, #6
	strb	r3, [r5, #5]
	movs	r3, #8
	strh	r3, [r5, #12]
	b.n	.L_08018dc6
.L_08018dbc:
	movs	r3, #7
	b.n	.L_08018dc2
.L_08018dc0:
	movs	r3, #4
.L_08018dc2:
	strb	r3, [r5, #5]
	strh	r2, [r5, #12]
.L_08018dc6:
	add	r1, sp, #16
	adds	r0, r7, #0
	bl	sub_080178b0
	cmp	r0, #0
	bne.n	.L_08018dd4
	movs	r0, #1
.L_08018dd4:
	mov	r9, r0
.L_08018dd6:
	ldrb	r3, [r5, #5]
	cmp	r3, #2
	bne.n	.L_08018e5c
	ldr	r2, [pc, #112]
	adds	r6, r6, r2
	ldrh	r3, [r6, #0]
	adds	r7, r5, #0
	adds	r7, #16
	cmp	r3, #99
	bne.n	.L_08018df0
	bl	sub_08004080
	strh	r0, [r6, #0]
.L_08018df0:
	mov	r3, r8
	ldrh	r2, [r3, #12]
	ldr	r1, [pc, #92]
	ldrh	r3, [r3, #8]
	adds	r3, r3, r1
	adds	r2, r2, r3
	lsls	r2, r2, #3
	ldr	r3, [pc, #32]
	adds	r2, #4
	ands	r2, r3
	ldrh	r1, [r7, #6]
	ldr	r3, [pc, #80]
	ands	r3, r1
	orrs	r3, r2
	mov	r2, r8
	strh	r3, [r7, #6]
	ldrb	r3, [r2, #14]
	ldrb	r2, [r2, #10]
	adds	r2, #254
	adds	r3, r3, r2
	lsls	r3, r3, #3
	subs	r3, #1
	strb	r3, [r7, #4]
	b.n	.L_08018eac
	.4byte 0x000001ff
	.4byte 0x03001e8c
	.4byte 0x000012b0
	.4byte 0x00000ea8
	.4byte 0x00000014
	.4byte 0x00000013
	.4byte 0x00000318
	.4byte 0x040000d4
	.4byte 0x080155d0
	.4byte 0xfffff968
	.4byte 0xb6db6db7
	.4byte 0x00000eac
	.4byte 0x000012b6
	.4byte 0x0000fffe
	.2byte 0xfe00
	.2byte 0xffff
.L_08018e5c:
	ldr	r3, [pc, #116]
	adds	r4, r6, r3
	ldrh	r1, [r4, #0]
	ldr	r2, [pc, #116]
	add	r1, sl
	lsls	r1, r1, #5
	adds	r1, r1, r2
	ldr	r3, [pc, #112]
	add	r0, sp, #16
	ldr	r2, [pc, #112]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r2, [sp, #8]
	mov	r1, fp
	lsrs	r3, r1, #1
	mov	r1, r8
	adds	r3, r2, r3
	ldrh	r2, [r1, #14]
	lsls	r2, r2, #3
	adds	r3, r3, r2
	ldr	r2, [pc, #92]
	adds	r3, r3, r2
	strh	r3, [r5, #20]
	ldr	r3, [sp, #4]
	ldr	r1, [sp, #12]
	lsrs	r2, r3, #1
	adds	r2, r1, r2
	mov	r1, r8
	ldrh	r3, [r1, #12]
	lsls	r3, r3, #3
	adds	r2, r2, r3
	ldr	r3, [pc, #52]
	adds	r2, #2
	orrs	r2, r3
	strh	r2, [r5, #22]
	ldrh	r3, [r4, #0]
	add	r3, sl
	adds	r7, r5, #0
	strh	r3, [r5, #24]
	adds	r7, #16
.L_08018eac:
	movs	r3, #254
	strb	r3, [r5, #15]
	ldrh	r3, [r7, #6]
	lsls	r3, r3, #23
	lsrs	r3, r3, #23
	strh	r3, [r5, #6]
	ldrb	r3, [r7, #4]
	movs	r2, #0
	strh	r3, [r5, #8]
	mov	r3, sl
	strb	r3, [r5, #14]
	str	r2, [r5, #0]
	mov	r0, r8
	adds	r1, r5, #0
	bl	sub_08016584
.L_08018ecc:
	mov	r0, r9
	b.n	.L_08018ee8
	.4byte 0x00004000
	.4byte 0x000012b8
	.4byte 0x06010000
	.4byte 0x040000d4
	.4byte 0x84000020
	.2byte 0xfffe
	.2byte 0x0000
.L_08018ee8:
	add	sp, #144
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
