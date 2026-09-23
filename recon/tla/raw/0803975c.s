.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08014274, 0x08014274
	.set sub_0803911c, 0x0803911c
	.set sub_080391c4, 0x080391c4
	.set sub_0803939c, 0x0803939c
	.set sub_080393fc, 0x080393fc
	.set sub_0803972c, 0x0803972c
	.set sub_08039754, 0x08039754
	.set sub_0803a084, 0x0803a084
	.set sub_0803b794, 0x0803b794
	.set sub_0803bde4, 0x0803bde4
	.set sub_0803cbf8, 0x0803cbf8
	.set sub_0803cd5c, 0x0803cd5c
	.set sub_0803cdb4, 0x0803cdb4
	.set sub_081c0010, 0x081c0010
	.global Func_0803975c
	.thumb_func
Func_0803975c:
.L_0803975c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	lsls	r3, r0, #1
	ldr	r6, [pc, #100]
	adds	r3, r3, r0
	lsls	r1, r3, #1
	lsls	r3, r3, #3
	mov	r8, r3
	adds	r5, r3, r6
	ldr	r3, [pc, #92]
	sub	sp, #8
	str	r3, [sp, #0]
	movs	r3, #32
	movs	r2, #24
	movs	r4, #132
	subs	r3, r3, r1
	subs	r2, r2, r1
	lsls	r4, r4, #24
	lsls	r3, r3, #2
	mov	r9, r2
	mov	fp, r4
	mov	sl, r3
	movs	r7, #29
.L_08039794:
	movs	r3, #128
	mov	r2, r9
	mov	r4, fp
	lsls	r3, r3, #19
	adds	r3, #212
	adds	r0, r5, #0
	adds	r1, r6, #0
	orrs	r2, r4
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r0, [sp, #0]
	ldr	r3, [pc, #48]
	add	r0, sl
	mov	r1, r8
	movs	r2, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9b00
	subs	r7, #1
	adds	r3, #128
	adds	r6, #128
	adds	r5, #128
	str	r3, [sp, #0]
	cmp	r7, #0
	bge.n	.L_08039794
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x06002520
	.4byte 0x06002500
	.2byte 0x0260
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #60]
	movs	r2, #215
	lsls	r2, r2, #3
	adds	r5, r3, r2
	movs	r3, #1
	movs	r7, #0
	mov	r8, r3
.L_080397f8:
	ldr	r2, [r5, #0]
	cmp	r2, #0
	beq.n	.L_08039856
	ldr	r3, [r2, #24]
	cmp	r3, #0
	bne.n	.L_08039856
	ldrh	r3, [r2, #22]
	cmp	r3, #0
	bne.n	.L_0803980e
	str	r3, [r5, #0]
	b.n	.L_08039856
.L_0803980e:
	ldrh	r6, [r2, #18]
	cmp	r6, #0
	beq.n	.L_0803981c
	adds	r0, r5, #0
	bl	sub_0803cbf8
	b.n	.L_08039856
.L_0803981c:
	adds	r0, r5, #0
	bl	.L_08039864
	cmp	r0, #8
	beq.n	.L_0803982c
	cmp	r0, #9
	beq.n	.L_08039832
	b.n	.L_08039856
.L_0803982c:
	ldr	r3, [r5, #0]
	mov	r2, r8
	b.n	.L_08039854
.L_08039832:
	ldr	r0, [r5, #0]
	movs	r1, #2
	ldrh	r3, [r0, #22]
	ands	r1, r3
	lsls	r1, r1, #16
	lsrs	r1, r1, #16
	bl	sub_0803939c
	ldr	r3, [r5, #0]
	mov	r2, r8
	strh	r6, [r5, #4]
	strh	r6, [r5, #6]
	strh	r6, [r5, #18]
	strh	r6, [r5, #20]
	strh	r6, [r5, #22]
	strh	r6, [r5, #24]
	strh	r6, [r5, #26]
.L_08039854:
	strh	r2, [r3, #20]
.L_08039856:
	adds	r7, #1
	adds	r5, #40
	cmp	r7, #3
	bne.n	.L_080397f8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
.L_08039864:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #60]
	ldr	r1, [pc, #808]
	mov	r8, r3
	ldr	r3, [r1, #0]
	ldr	r3, [pc, #808]
	adds	r6, r0, #0
	movs	r0, #139
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldrb	r3, [r3, #0]
	ldr	r2, [pc, #800]
	sub	sp, #52
	ldrb	r2, [r2, r3]
	str	r2, [sp, #32]
	mov	r2, r8
	ldrb	r3, [r2, #5]
	cmp	r3, #0
	beq.n	.L_080398b4
	ldr	r3, [pc, #788]
	ldrh	r3, [r3, #0]
	adds	r2, r3, #0
	cmp	r2, #0
	bge.n	.L_080398a6
	movs	r2, #0
.L_080398a6:
	cmp	r2, #3
	ble.n	.L_080398ac
	movs	r2, #3
.L_080398ac:
	lsls	r3, r2, #2
	adds	r3, r3, r2
	adds	r3, #3
	str	r3, [sp, #32]
.L_080398b4:
	ldrh	r3, [r6, #28]
	cmp	r3, #0
	beq.n	.L_080398ca
	movs	r0, #1
	bl	.L_0803975c
	ldrh	r3, [r6, #28]
	movs	r0, #0
	subs	r3, #1
	strh	r3, [r6, #28]
	b.n	.L_08039ec2
.L_080398ca:
	ldr	r3, [r1, #0]
	cmp	r3, #0
	bne.n	.L_080398e4
	ldrh	r2, [r6, #34]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_080398e4
	movs	r4, #255
	lsls	r4, r4, #8
	adds	r4, #255
	adds	r3, r2, r4
	strh	r3, [r6, #34]
	b.n	.L_08039ec0
.L_080398e4:
	ldrh	r3, [r6, #32]
	movs	r7, #0
	cmp	r3, #0
	bne.n	.L_080398fa
	ldrh	r3, [r6, #18]
	movs	r0, #244
	lsls	r3, r3, #1
	lsls	r0, r0, #4
	adds	r3, r3, r0
	mov	r1, r8
	ldrh	r7, [r1, r3]
.L_080398fa:
	cmp	r7, #30
	bls.n	.L_08039906
	cmp	r7, #176
	beq.n	.L_08039904
	b.n	.L_08039d00
.L_08039904:
	b.n	.L_08039cf0
.L_08039906:
	ldr	r2, [pc, #684]
	lsls	r3, r7, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	ldr	r4, [sp, #912]
	lsrs	r3, r0, #32
	ldr	r1, [sp, #864]
	lsrs	r3, r0, #32
	ldr	r3, [sp, #752]
	lsrs	r3, r0, #32
	ldr	r1, [sp, #560]
	lsrs	r3, r0, #32
	ldr	r4, [sp, #88]
	lsrs	r3, r0, #32
	ldr	r3, [sp, #952]
	lsrs	r3, r0, #32
	ldr	r3, [sp, #992]
	lsrs	r3, r0, #32
	ldr	r4, [sp, #656]
	lsrs	r3, r0, #32
	ldr	r4, [sp, #184]
	lsrs	r3, r0, #32
	ldr	r4, [sp, #384]
	lsrs	r3, r0, #32
	ldr	r4, [sp, #544]
	lsrs	r3, r0, #32
	ldr	r4, [sp, #960]
	lsrs	r3, r0, #32
	ldr	r4, [sp, #960]
	lsrs	r3, r0, #32
	ldr	r4, [sp, #960]
	lsrs	r3, r0, #32
	ldr	r4, [sp, #960]
	lsrs	r3, r0, #32
	ldr	r4, [sp, #752]
	lsrs	r3, r0, #32
	ldr	r4, [sp, #960]
	lsrs	r3, r0, #32
	ldr	r4, [sp, #960]
	lsrs	r3, r0, #32
	ldr	r4, [sp, #960]
	lsrs	r3, r0, #32
	ldr	r4, [sp, #960]
	lsrs	r3, r0, #32
	ldr	r4, [sp, #960]
	lsrs	r3, r0, #32
	ldr	r4, [sp, #960]
	lsrs	r3, r0, #32
	ldr	r4, [sp, #960]
	lsrs	r3, r0, #32
	ldr	r4, [sp, #960]
	lsrs	r3, r0, #32
	ldr	r4, [sp, #960]
	lsrs	r3, r0, #32
	ldr	r4, [sp, #960]
	lsrs	r3, r0, #32
	ldr	r4, [sp, #960]
	lsrs	r3, r0, #32
	ldr	r4, [sp, #960]
	lsrs	r3, r0, #32
	ldr	r4, [sp, #960]
	lsrs	r3, r0, #32
	ldr	r4, [sp, #960]
	lsrs	r3, r0, #32
	ldr	r4, [sp, #912]
	lsrs	r3, r0, #32
	ldrh	r3, [r6, #30]
	strh	r3, [r6, #4]
	ldr	r3, [r6, #0]
	ldrh	r2, [r3, #22]
	movs	r3, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080399be
	movs	r3, #192
	ldrh	r2, [r6, #6]
	lsls	r3, r3, #4
	adds	r3, #255
	cmp	r2, r3
	bls.n	.L_080399b4
	adds	r0, r6, #0
	bl	sub_08039754
	movs	r2, #1
	str	r2, [sp, #32]
	b.n	.L_08039cf0
.L_080399b4:
	movs	r4, #208
	lsls	r4, r4, #4
	adds	r3, r2, r4
	strh	r3, [r6, #6]
	b.n	.L_08039cf0
.L_080399be:
	ldrh	r3, [r6, #6]
	movs	r0, #240
	ldrh	r2, [r6, #16]
	lsls	r0, r0, #4
	adds	r3, r3, r0
	strh	r3, [r6, #6]
	adds	r3, r2, #0
	cmp	r3, #2
	bls.n	.L_080399d2
	b.n	.L_08039cf0
.L_080399d2:
	adds	r3, r2, #1
	strh	r3, [r6, #16]
	b.n	.L_08039cf0
	ldrh	r1, [r6, #20]
	cmp	r1, #0
	bne.n	.L_080399fa
	ldr	r3, [pc, #456]
	movs	r4, #139
	lsls	r4, r4, #2
	adds	r3, r3, r4
	ldr	r2, [pc, #464]
	ldrb	r3, [r3, #0]
	mov	r0, r8
	ldrb	r3, [r2, r3]
	strh	r3, [r6, #20]
	ldrb	r3, [r0, #4]
	cmp	r3, #0
	beq.n	.L_080399fa
	ldr	r3, [pc, #428]
	str	r1, [r3, #28]
.L_080399fa:
	adds	r0, r6, #0
	bl	sub_0803cdb4
	cmp	r0, #0
	bne.n	.L_08039a40
	ldr	r0, [r6, #0]
	ldrh	r3, [r0, #8]
	cmp	r3, #0
	bne.n	.L_08039a0e
	b.n	.L_08039cf0
.L_08039a0e:
	ldrh	r3, [r0, #10]
	cmp	r3, #0
	bne.n	.L_08039a16
	b.n	.L_08039cf0
.L_08039a16:
	movs	r7, #152
	lsls	r7, r7, #5
	adds	r7, #136
	add	r7, r8
	ldrb	r3, [r7, #0]
	cmp	r3, #0
	beq.n	.L_08039a26
	b.n	.L_08039cf0
.L_08039a26:
	ldrh	r2, [r0, #8]
	ldrh	r3, [r0, #10]
	lsls	r2, r2, #2
	lsls	r3, r3, #3
	movs	r5, #1
	subs	r2, #8
	subs	r3, #16
	movs	r1, #1
	str	r5, [sp, #0]
	bl	sub_0803bde4
	strb	r5, [r7, #0]
	b.n	.L_08039cf0
.L_08039a40:
	ldr	r5, [r6, #0]
	ldrh	r4, [r6, #18]
	movs	r1, #12
	ldrsh	r3, [r5, r1]
	str	r4, [sp, #12]
	str	r3, [sp, #48]
	movs	r2, #14
	ldrsh	r3, [r5, r2]
	movs	r2, #152
	str	r3, [sp, #44]
	ldrh	r3, [r5, #8]
	lsls	r2, r2, #5
	str	r3, [sp, #28]
	adds	r2, #136
	ldrh	r0, [r5, #10]
	movs	r3, #0
	add	r2, r8
	str	r0, [sp, #24]
	strb	r3, [r2, #0]
	adds	r0, r5, #0
	bl	sub_080393fc
	ldrh	r3, [r6, #36]
	ldr	r4, [sp, #12]
	cmp	r3, #0
	bne.n	.L_08039a90
	ldrh	r2, [r5, #8]
	ldrh	r3, [r5, #10]
	orrs	r3, r2
	cmp	r3, #0
	beq.n	.L_08039a90
	movs	r1, #12
	ldrsh	r0, [r5, r1]
	movs	r2, #14
	ldrsh	r1, [r5, r2]
	ldrh	r3, [r5, #10]
	ldrh	r2, [r5, #8]
	bl	sub_0803911c
	ldr	r4, [sp, #12]
.L_08039a90:
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r4, #1
	ands	r4, r3
	movs	r0, #244
	lsls	r3, r4, #1
	lsls	r0, r0, #4
	adds	r3, r3, r0
	mov	r1, r8
	ldrh	r3, [r1, r3]
	cmp	r3, #0
	beq.n	.L_08039b76
	ldrh	r2, [r5, #8]
	ldrh	r3, [r5, #10]
	orrs	r3, r2
	cmp	r3, #0
	beq.n	.L_08039b76
	ldrh	r7, [r6, #36]
	cmp	r7, #0
	beq.n	.L_08039acc
	movs	r2, #12
	ldrsh	r0, [r5, r2]
	movs	r3, #14
	ldrsh	r1, [r5, r3]
	ldrh	r2, [r5, #8]
	ldrh	r3, [r5, #10]
	bl	sub_0803911c
	b.n	.L_08039b66
.L_08039acc:
	add	r0, sp, #48
	mov	r1, sp
	mov	r2, sp
	mov	fp, r0
	adds	r1, #44
	adds	r2, #40
	movs	r3, #36
	movs	r0, #8
	add	r3, sp
	str	r1, [sp, #20]
	str	r2, [sp, #16]
	adds	r0, r0, r6
	str	r3, [sp, #0]
	str	r0, [sp, #4]
	mov	r1, fp
	mov	r9, r3
	mov	sl, r0
	ldr	r3, [sp, #16]
	adds	r0, r4, #0
	ldr	r2, [sp, #20]
	str	r4, [sp, #12]
	str	r7, [sp, #8]
	bl	sub_0803b794
	ldrh	r1, [r5, #22]
	movs	r3, #128
	ands	r3, r1
	ldr	r4, [sp, #12]
	cmp	r3, #0
	beq.n	.L_08039b20
	ldr	r2, [sp, #36]
	ldr	r3, [sp, #24]
	cmp	r3, r2
	beq.n	.L_08039b18
	subs	r2, r2, r3
	ldr	r3, [sp, #44]
	subs	r3, r3, r2
	str	r3, [sp, #44]
.L_08039b18:
	ldr	r3, [sp, #44]
	cmp	r3, #0
	bge.n	.L_08039b20
	str	r7, [sp, #44]
.L_08039b20:
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r3, r1
	cmp	r3, #0
	bne.n	.L_08039b56
	ldr	r3, [sp, #40]
	ldr	r0, [sp, #28]
	subs	r3, r0, r3
	cmp	r3, #0
	bge.n	.L_08039b36
	adds	r3, #3
.L_08039b36:
	ldr	r2, [sp, #48]
	asrs	r3, r3, #2
	adds	r2, r2, r3
	mov	r1, r9
	movs	r3, #2
	str	r2, [sp, #48]
	mov	r2, sl
	str	r1, [sp, #0]
	str	r2, [sp, #4]
	str	r3, [sp, #8]
	adds	r0, r4, #0
	mov	r1, fp
	ldr	r2, [sp, #20]
	ldr	r3, [sp, #16]
	bl	sub_0803b794
.L_08039b56:
	ldr	r3, [sp, #48]
	strh	r3, [r5, #12]
	ldr	r3, [sp, #44]
	strh	r3, [r5, #14]
	ldr	r3, [sp, #40]
	strh	r3, [r5, #8]
	ldr	r3, [sp, #36]
	strh	r3, [r5, #10]
.L_08039b66:
	movs	r3, #12
	ldrsh	r0, [r5, r3]
	movs	r4, #14
	ldrsh	r1, [r5, r4]
	ldrh	r2, [r5, #8]
	ldrh	r3, [r5, #10]
	bl	sub_0803a084
.L_08039b76:
	ldrh	r3, [r6, #30]
	movs	r5, #152
	lsls	r5, r5, #5
	strh	r3, [r6, #4]
	adds	r5, #70
	movs	r3, #15
	movs	r2, #0
	strh	r3, [r6, #22]
	adds	r0, r6, #0
	movs	r3, #10
	add	r5, r8
	strh	r3, [r6, #26]
	strh	r2, [r6, #6]
	strh	r2, [r6, #16]
	strh	r2, [r6, #24]
	bl	sub_0803972c
	ldrh	r0, [r5, #0]
	bl	sub_08014274
	movs	r3, #99
	strh	r3, [r5, #0]
	b.n	.L_08039cf0
	.4byte 0x03001150
	.4byte 0x02000240
	.4byte 0x080aa0e2
	.4byte 0x030011d8
	.4byte 0x08039910
	.2byte 0xa0df
	.2byte 0x080a
	ldrh	r1, [r6, #20]
	cmp	r1, #0
	bne.n	.L_08039bde
	ldr	r3, [pc, #144]
	movs	r0, #139
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r2, [pc, #140]
	ldrb	r3, [r3, #0]
	ldrb	r3, [r2, r3]
	mov	r2, r8
	strh	r3, [r6, #20]
	ldrb	r3, [r2, #4]
	cmp	r3, #0
	beq.n	.L_08039bde
	ldr	r3, [pc, #128]
	str	r1, [r3, #28]
.L_08039bde:
	adds	r0, r6, #0
	bl	sub_0803cdb4
	cmp	r0, #0
	bne.n	.L_08039bea
	b.n	.L_08039cf0
.L_08039bea:
	movs	r0, #9
	b.n	.L_08039ec2
	ldrh	r3, [r6, #20]
	cmp	r3, #0
	bne.n	.L_08039c02
	movs	r3, #20
	b.n	.L_08039c00
	ldrh	r3, [r6, #20]
	cmp	r3, #0
	bne.n	.L_08039c02
	movs	r3, #120
.L_08039c00:
	strh	r3, [r6, #20]
.L_08039c02:
	movs	r2, #152
	lsls	r2, r2, #5
	adds	r2, #134
	add	r2, r8
	movs	r3, #0
	strh	r3, [r2, #0]
	adds	r0, r6, #0
	bl	sub_0803cd5c
	b.n	.L_08039cf0
	ldrh	r3, [r6, #20]
	cmp	r3, #0
	bne.n	.L_08039c20
	movs	r3, #60
	strh	r3, [r6, #20]
.L_08039c20:
	movs	r2, #152
	lsls	r2, r2, #5
	adds	r2, #134
	add	r2, r8
	movs	r3, #0
	strh	r3, [r2, #0]
	b.n	.L_08039cf0
	ldrh	r3, [r6, #18]
	ldr	r2, [pc, #28]
	adds	r3, #1
	ands	r3, r2
	strh	r3, [r6, #18]
	movs	r4, #244
	ldrh	r3, [r6, #18]
	lsls	r4, r4, #4
	lsls	r3, r3, #1
	mov	r0, r8
	adds	r3, r3, r4
	ldrh	r3, [r0, r3]
	adds	r0, r6, #0
	strh	r3, [r6, #22]
	bl	sub_0803972c
	b.n	.L_08039cf0
	.4byte 0x000001ff
	.4byte 0x02000240
	.4byte 0x080aa0df
	.2byte 0x1150
	.2byte 0x0300
	ldrh	r3, [r6, #18]
	ldr	r2, [pc, #32]
	adds	r3, #1
	ands	r3, r2
	strh	r3, [r6, #18]
	movs	r1, #244
	ldrh	r3, [r6, #18]
	lsls	r1, r1, #4
	lsls	r3, r3, #1
	adds	r3, r3, r1
	mov	r2, r8
	ldrh	r3, [r2, r3]
	adds	r0, r6, #0
	strh	r3, [r6, #24]
	bl	sub_0803972c
	b.n	.L_08039cf0
	movs	r0, r0
	.2byte 0x01ff
	.2byte 0x0000
	ldrh	r3, [r6, #18]
	ldr	r2, [pc, #44]
	adds	r3, #1
	ands	r3, r2
	strh	r3, [r6, #18]
	movs	r4, #244
	ldrh	r3, [r6, #18]
	lsls	r4, r4, #4
	lsls	r3, r3, #1
	mov	r0, r8
	adds	r3, r3, r4
	ldrh	r3, [r0, r3]
	adds	r0, r6, #0
	b.n	.L_08039cb0
	movs	r3, #0
	movs	r2, #15
	strh	r3, [r6, #24]
	adds	r0, r6, #0
	movs	r3, #10
	strh	r2, [r6, #22]
.L_08039cb0:
	strh	r3, [r6, #26]
	bl	sub_0803972c
	b.n	.L_08039cf0
	.2byte 0x01ff
	.2byte 0x0000
	ldrh	r3, [r6, #18]
	ldr	r0, [pc, #44]
	adds	r3, #1
	ands	r3, r0
	strh	r3, [r6, #18]
	movs	r4, #244
	ldrh	r2, [r6, #18]
	lsls	r4, r4, #4
	lsls	r3, r2, #1
	adds	r3, r3, r4
	mov	r4, r8
	ldrh	r3, [r4, r3]
	ldr	r1, [r6, #0]
	adds	r2, #1
	strh	r3, [r1, #18]
	ands	r2, r0
	movs	r3, #11
	strh	r3, [r6, #20]
	strh	r2, [r6, #18]
	b.n	.L_08039cf0
	movs	r3, #1
	strh	r3, [r6, #32]
	movs	r0, #8
	b.n	.L_08039ec2
	.2byte 0x01ff
	.2byte 0x0000
.L_08039cf0:
	mov	r0, r8
	ldrb	r3, [r0, #5]
	cmp	r3, #0
	beq.n	.L_08039cfa
	b.n	.L_08039e8c
.L_08039cfa:
	movs	r1, #1
	str	r1, [sp, #32]
	b.n	.L_08039e8c
.L_08039d00:
	ldrh	r3, [r6, #4]
	adds	r2, r3, #0
	adds	r2, #128
	cmp	r2, #0
	bge.n	.L_08039d10
	movs	r4, #128
	adds	r4, #255
	adds	r2, r3, r4
.L_08039d10:
	ldrh	r3, [r6, #6]
	asrs	r5, r2, #8
	adds	r0, r3, #0
	adds	r0, #128
	cmp	r0, #0
	bge.n	.L_08039d22
	movs	r1, #128
	adds	r1, #255
	adds	r0, r3, r1
.L_08039d22:
	ldr	r3, [pc, #64]
	movs	r4, #139
	lsls	r4, r4, #2
	adds	r3, r3, r4
	ldrb	r3, [r3, #0]
	ldr	r2, [pc, #56]
	asrs	r0, r0, #8
	mov	ip, r0
	mov	r0, r8
	ldrb	r2, [r2, r3]
	ldrb	r3, [r0, #4]
	mov	sl, r2
	ldrh	r2, [r6, #18]
	cmp	r3, #0
	beq.n	.L_08039d42
	adds	r5, #8
.L_08039d42:
	adds	r3, r2, #1
	movs	r2, #128
	lsls	r2, r2, #1
	adds	r2, #255
	ands	r3, r2
	movs	r1, #244
	lsls	r3, r3, #1
	lsls	r1, r1, #4
	adds	r3, r3, r1
	mov	r4, r8
	ldrh	r0, [r4, r3]
	cmp	r0, #222
	bne.n	.L_08039d6c
	movs	r3, #128
	lsls	r3, r3, #7
	b.n	.L_08039d74
	movs	r0, r0
	.4byte 0x02000240
	.2byte 0xa0e5
	.2byte 0x080a
.L_08039d6c:
	cmp	r0, #223
	bne.n	.L_08039d7e
	movs	r3, #128
	lsls	r3, r3, #8
.L_08039d74:
	orrs	r7, r3
	ldrh	r3, [r6, #18]
	adds	r3, #1
	ands	r3, r2
	strh	r3, [r6, #18]
.L_08039d7e:
	ldr	r4, [r6, #0]
	movs	r3, #8
	ldrh	r2, [r4, #22]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08039dc8
	cmp	r7, #32
	bls.n	.L_08039dc8
	cmp	r0, #32
	bls.n	.L_08039dc8
	ldr	r1, [pc, #48]
	adds	r3, r7, #0
	adds	r2, r0, #0
	subs	r3, #32
	subs	r2, #32
	lsls	r3, r3, #5
	lsls	r2, r2, #5
	ldrh	r3, [r1, r3]
	ldrh	r2, [r1, r2]
	movs	r1, #240
	adds	r3, r3, r2
	lsls	r3, r3, #16
	lsls	r1, r1, #12
	cmp	r3, r1
	bhi.n	.L_08039dc8
	lsls	r3, r0, #8
	orrs	r7, r3
	ldrh	r3, [r6, #18]
	ldr	r2, [pc, #8]
	adds	r3, #1
	ands	r3, r2
	strh	r3, [r6, #18]
	b.n	.L_08039dc8
	.4byte 0x000001ff
	.2byte 0xa4e0
	.2byte 0x0805
.L_08039dc8:
	movs	r3, #152
	lsls	r3, r3, #5
	adds	r3, #140
	add	r3, r8
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_08039df4
	ldrh	r3, [r6, #38]
	cmp	r3, #0
	beq.n	.L_08039df4
	ldrh	r0, [r6, #6]
	movs	r1, #160
	adds	r0, #128
	lsls	r1, r1, #1
	str	r4, [sp, #12]
	bl	sub_08002054
	ldr	r4, [sp, #12]
	subs	r0, #3
	mov	ip, r0
.L_08039df4:
	movs	r3, #0
	str	r3, [sp, #0]
	adds	r0, r4, #0
	adds	r2, r5, #0
	mov	r3, ip
	adds	r1, r7, #0
	bl	sub_0803bde4
	ldr	r3, [pc, #76]
	adds	r4, r0, #0
	movs	r0, #139
	lsls	r0, r0, #2
	adds	r3, r3, r0
	ldr	r2, [pc, #72]
	ldrb	r3, [r3, #0]
	ldrb	r3, [r2, r3]
	strh	r3, [r6, #34]
	cmp	r4, #0
	bne.n	.L_08039e1c
	b.n	.L_08039bea
.L_08039e1c:
	movs	r1, #152
	lsls	r1, r1, #5
	adds	r1, #132
	add	r1, r8
	ldrh	r3, [r1, #0]
	cmp	r3, #0
	beq.n	.L_08039e66
	movs	r5, #152
	lsls	r5, r5, #5
	adds	r5, #134
	add	r5, r8
	ldrh	r2, [r5, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	bne.n	.L_08039e5c
	cmp	r7, #32
	beq.n	.L_08039e66
	ldrh	r0, [r1, #0]
	movs	r3, #3
	ands	r3, r7
	adds	r0, r0, r3
	str	r4, [sp, #12]
	bl	sub_081c0010
	mov	r1, sl
	strh	r1, [r5, #0]
	ldr	r4, [sp, #12]
	b.n	.L_08039e66
	.4byte 0x02000240
	.2byte 0xa0dc
	.2byte 0x080a
.L_08039e5c:
	movs	r0, #255
	lsls	r0, r0, #8
	adds	r0, #255
	adds	r3, r2, r0
	strh	r3, [r5, #0]
.L_08039e66:
	lsls	r0, r4, #8
	cmp	r7, #32
	bne.n	.L_08039e76
	ldrh	r3, [r6, #16]
	lsls	r3, r3, #1
	adds	r3, #8
	ldrh	r3, [r6, r3]
	adds	r0, r0, r3
.L_08039e76:
	ldrh	r3, [r6, #4]
	adds	r3, r3, r0
	strh	r3, [r6, #4]
	cmp	r7, #32
	bne.n	.L_08039e8c
	mov	r1, r8
	ldrb	r3, [r1, #5]
	cmp	r3, #0
	bne.n	.L_08039e8c
	movs	r2, #1
	str	r2, [sp, #32]
.L_08039e8c:
	ldrh	r2, [r6, #20]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_08039ea4
	movs	r4, #255
	lsls	r4, r4, #8
	adds	r4, #255
	adds	r3, r2, r4
	strh	r3, [r6, #20]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_08039eae
.L_08039ea4:
	ldrh	r3, [r6, #18]
	ldr	r2, [pc, #20]
	adds	r3, #1
	ands	r3, r2
	strh	r3, [r6, #18]
.L_08039eae:
	ldr	r0, [sp, #32]
	subs	r0, #1
	str	r0, [sp, #32]
	cmp	r0, #0
	beq.n	.L_08039eba
	b.n	.L_080398e4
.L_08039eba:
	b.n	.L_08039ec0
	.2byte 0x01ff
	.2byte 0x0000
.L_08039ec0:
	movs	r0, #0
.L_08039ec2:
	add	sp, #52
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r7, [r3, #60]
	movs	r1, #161
	lsls	r1, r1, #3
	movs	r2, #0
	adds	r5, r7, r1
	mov	r8, r2
.L_08039ee6:
	ldrh	r6, [r5, #22]
	cmp	r6, #0
	beq.n	.L_08039f14
	movs	r4, #24
	ldrsh	r3, [r5, r4]
	cmp	r3, #0
	beq.n	.L_08039f04
	adds	r0, r5, #0
	movs	r1, #0
	bl	.L_08039f94
	ldrh	r3, [r5, #24]
	subs	r3, #1
	strh	r3, [r5, #24]
	b.n	.L_08039f82
.L_08039f04:
	movs	r1, #26
	ldrsh	r3, [r5, r1]
	cmp	r3, #0
	beq.n	.L_08039f82
	adds	r0, r5, #0
	bl	sub_080391c4
	b.n	.L_08039f82
.L_08039f14:
	movs	r3, #26
	ldrsh	r2, [r5, r3]
	cmp	r2, #0
	beq.n	.L_08039f82
	movs	r4, #24
	ldrsh	r3, [r5, r4]
	cmp	r3, r2
	beq.n	.L_08039f4a
	movs	r1, #28
	ldrsh	r0, [r5, r1]
	movs	r2, #30
	ldrsh	r1, [r5, r2]
	movs	r3, #32
	ldrsh	r2, [r5, r3]
	movs	r4, #34
	ldrsh	r3, [r5, r4]
	bl	sub_0803911c
	adds	r0, r5, #0
	movs	r1, #1
	bl	.L_08039f94
	ldrh	r3, [r5, #24]
	adds	r3, #1
	strh	r3, [r5, #24]
	movs	r3, #1
	b.n	.L_08039f80
.L_08039f4a:
	movs	r1, #28
	ldrsh	r0, [r5, r1]
	movs	r2, #30
	ldrsh	r1, [r5, r2]
	movs	r3, #32
	ldrsh	r2, [r5, r3]
	movs	r4, #34
	ldrsh	r3, [r5, r4]
	bl	sub_0803911c
	movs	r3, #1
	str	r6, [r5, #0]
	str	r6, [r5, #4]
	strh	r6, [r5, #8]
	strh	r6, [r5, #10]
	strh	r6, [r5, #12]
	strh	r6, [r5, #14]
	strh	r6, [r5, #16]
	strh	r6, [r5, #18]
	strh	r6, [r5, #20]
	strh	r6, [r5, #22]
	strh	r6, [r5, #24]
	strh	r6, [r5, #26]
	strh	r6, [r5, #28]
	strh	r6, [r5, #30]
	strh	r6, [r5, #32]
	strh	r6, [r5, #34]
.L_08039f80:
	strb	r3, [r7, #3]
.L_08039f82:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	adds	r5, #36
	cmp	r2, #12
	bne.n	.L_08039ee6
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
.L_08039f94:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #16
	str	r1, [sp, #0]
	adds	r7, r0, #0
	movs	r2, #24
	ldrsh	r6, [r7, r2]
	movs	r3, #26
	ldrsh	r0, [r7, r3]
	ldrh	r3, [r7, #8]
	add	r5, sp, #4
	adds	r1, r3, #0
	muls	r1, r6
	ldr	r3, [pc, #148]
	subs	r2, r0, r6
	lsls	r1, r1, #16
	lsls	r0, r0, #17
	mov	r8, r3
	str	r1, [r5, #0]
	str	r0, [r5, #4]
	mov	sl, r2
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x220c
	ldrsh	r3, [r7, r2]
	str	r0, [r5, #8]
	asrs	r0, r0, #16
	adds	r0, r0, r3
	ldrh	r3, [r7, #8]
	mov	r9, r0
	mov	r1, sl
	muls	r1, r3
	lsls	r1, r1, #16
	str	r1, [r5, #0]
	ldr	r0, [r5, #4]
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x897b
	str	r0, [r5, #8]
	asrs	r0, r0, #15
	adds	r1, r3, #0
	muls	r1, r6
	mov	fp, r0
	movs	r3, #26
	ldrsh	r0, [r7, r3]
	lsls	r1, r1, #16
	lsls	r0, r0, #17
	str	r1, [r5, #0]
	str	r0, [r5, #4]
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x220e
	ldrsh	r3, [r7, r2]
	str	r0, [r5, #8]
	asrs	r0, r0, #16
	adds	r6, r0, r3
	ldrh	r3, [r7, #10]
	ldr	r0, [r5, #4]
	mov	r1, sl
	muls	r1, r3
	lsls	r1, r1, #16
	str	r1, [r5, #0]
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x60a8
	asrs	r5, r0, #15
	adds	r3, r5, #0
	mov	r0, r9
	adds	r1, r6, #0
	mov	r2, fp
	bl	sub_0803a084
	ldr	r3, [sp, #0]
	cmp	r3, #0
	beq.n	.L_0803a040
	mov	r2, r9
	mov	r3, fp
	strh	r2, [r7, #28]
	strh	r6, [r7, #30]
	strh	r3, [r7, #32]
	strh	r5, [r7, #34]
.L_0803a040:
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x0230
	.2byte 0x0300
	push	{r5, lr}
	adds	r4, r2, #0
	sub	sp, #4
	adds	r5, r0, #0
	cmp	r4, #0
	ble.n	.L_0803a07c
	mov	r0, sp
	movs	r2, #129
	movs	r3, #128
	adds	r0, #2
	lsls	r2, r2, #24
	lsls	r3, r3, #19
	strh	r1, [r0, #0]
	adds	r3, #212
	adds	r1, r5, #0
	orrs	r2, r4
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	lsls	r3, r4, #1
	adds	r5, r5, r3
.L_0803a07c:
	adds	r0, r5, #0
	add	sp, #4
	pop	{r5, pc}
	.2byte 0x0000
