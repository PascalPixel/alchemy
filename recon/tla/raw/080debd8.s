.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_0801489c, 0x0801489c
	.set sub_08020090, 0x08020090
	.set sub_080200a8, 0x080200a8
	.set sub_080200c8, 0x080200c8
	.set sub_08020148, 0x08020148
	.set sub_08020150, 0x08020150
	.set sub_08020210, 0x08020210
	.set sub_08020218, 0x08020218
	.set sub_08020278, 0x08020278
	.set sub_08020298, 0x08020298
	.set sub_08020330, 0x08020330
	.set sub_080ce31c, 0x080ce31c
	.set sub_080ce458, 0x080ce458
	.set sub_080ceafc, 0x080ceafc
	.set sub_080db974, 0x080db974
	.set sub_080dc10c, 0x080dc10c
	.set sub_080dc294, 0x080dc294
	.set sub_080dc384, 0x080dc384
	.set sub_080dcf54, 0x080dcf54
	.set sub_080dfb0c, 0x080dfb0c
	.set sub_081c0010, 0x081c0010
	.global Func_080debd8
	.thumb_func
Func_080debd8:
.L_080debd8:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r3, [r3, #0]
	movs	r0, #154
	ldr	r5, [r3, #16]
	movs	r7, #1
	ldr	r6, [r5, #80]
	ldr	r1, [r6, #40]
	mov	sl, r1
	bl	sub_081c0010
	ldr	r0, [pc, #136]
	bl	sub_08014644
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_08020090
	movs	r3, #0
	str	r3, [r5, #108]
	movs	r2, #7
	movs	r5, #0
	mov	r9, r5
	mov	r8, r2
.L_080dec14:
	mov	r1, sl
	mov	r3, r8
	strb	r3, [r1, #5]
	movs	r3, #2
	strb	r3, [r6, #26]
	movs	r0, #2
	strb	r7, [r6, #25]
	bl	sub_08013560
	mov	r2, r9
	strb	r7, [r6, #25]
	strb	r2, [r6, #26]
	movs	r0, #2
	adds	r5, #1
	bl	sub_08013560
	cmp	r5, #4
	bls.n	.L_080dec14
	movs	r5, #0
	movs	r3, #7
	mov	r8, r5
	movs	r7, #1
	mov	r9, r3
.L_080dec42:
	mov	r1, r9
	mov	r2, sl
	mov	r3, r8
	strb	r1, [r2, #5]
	strb	r3, [r6, #26]
	movs	r0, #2
	strb	r7, [r6, #25]
	bl	sub_08013560
	mov	r1, r8
	mov	r2, sl
	strb	r1, [r2, #5]
	strb	r7, [r6, #25]
	movs	r0, #2
	adds	r5, #1
	bl	sub_08013560
	cmp	r5, #4
	bls.n	.L_080dec42
	movs	r3, #1
	strb	r3, [r6, #26]
	movs	r1, #183
	ldr	r3, [pc, #24]
	lsls	r1, r1, #1
	adds	r1, #255
	movs	r2, #0
	adds	r3, r3, r1
	strb	r2, [r3, #0]
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0x080deba5
	.2byte 0x0240
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #32]
	movs	r2, #183
	lsls	r2, r2, #1
	adds	r2, #255
	adds	r3, r3, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080decac
	movs	r1, #144
	ldr	r0, [pc, #12]
	lsls	r1, r1, #3
	bl	sub_080145a8
.L_080decac:
	pop	{pc}
	movs	r0, r0
	.4byte 0x02000240
	.2byte 0xeba5
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
	ldr	r2, [r3, #108]
	sub	sp, #4
	str	r2, [sp, #0]
	adds	r3, #224
	ldr	r3, [r3, #0]
	movs	r0, #130
	ldr	r5, [r3, #16]
	ldr	r7, [r5, #80]
	ldr	r3, [r7, #40]
	mov	sl, r3
	bl	sub_081c0010
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_08020090
	movs	r3, #0
	str	r3, [r5, #108]
	movs	r2, #1
	movs	r5, #0
	movs	r3, #7
	mov	fp, r5
	mov	r8, r2
	mov	r9, r3
.L_080decfa:
	mov	r3, sl
	mov	r2, r9
	strb	r2, [r3, #5]
	movs	r6, #2
	mov	r2, r8
	strb	r2, [r7, #25]
	movs	r0, #2
	strb	r6, [r7, #26]
	bl	sub_08013560
	mov	r3, r8
	mov	r2, fp
	strb	r3, [r7, #25]
	strb	r2, [r7, #26]
	movs	r0, #2
	adds	r5, #1
	bl	sub_08013560
	cmp	r5, #9
	bls.n	.L_080decfa
	ldr	r5, [pc, #88]
	movs	r3, #0
	mov	r8, r3
	mov	r2, r8
	mov	r3, sl
	strb	r6, [r7, #26]
	movs	r1, #144
	movs	r6, #1
	strb	r2, [r3, #5]
	lsls	r1, r1, #3
	adds	r0, r5, #0
	strb	r6, [r7, #25]
	bl	sub_080145a8
	ldr	r3, [pc, #64]
	movs	r2, #183
	lsls	r2, r2, #1
	adds	r2, #255
	adds	r3, r3, r2
	strb	r6, [r3, #0]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x9b00
	movs	r2, #179
	lsls	r2, r2, #1
	adds	r5, r3, r2
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	movs	r2, #128
	lsls	r2, r2, #6
	adds	r2, #146
	cmp	r3, r2
	bne.n	.L_080ded6c
	bl	.L_080debd8
	mov	r3, r8
	strh	r3, [r5, #0]
.L_080ded6c:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x080deba5
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r6, r0, #0
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #20]
	cmp	r2, r3
	bgt.n	.L_080deda2
	adds	r2, r6, #0
	adds	r2, #94
	movs	r3, #2
	movs	r5, #0
	strh	r3, [r2, #0]
	ldr	r1, [pc, #8]
	bl	sub_080200a8
	str	r5, [r6, #108]
.L_080deda2:
	pop	{r5, r6, pc}
	.2byte 0x0e54
	.2byte 0x080f
	push	{r5, r6, lr}
	adds	r6, r0, #0
	ldr	r2, [pc, #40]
	ldr	r3, [r6, #12]
	adds	r3, r3, r2
	str	r3, [r6, #12]
	bl	sub_08014878
	adds	r5, r0, #0
	bl	sub_08014878
	ldr	r3, [r6, #8]
	subs	r5, r5, r0
	adds	r3, r3, r5
	str	r3, [r6, #8]
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #20]
	cmp	r2, r3
	bgt.n	.L_080dedd6
	ldr	r1, [pc, #12]
	adds	r0, r6, #0
	bl	sub_080200a8
.L_080dedd6:
	pop	{r5, r6, pc}
	.4byte 0xffffb334
	.2byte 0x0e54
	.2byte 0x080f
	push	{lr}
	bl	.L_080dede8
	pop	{pc}
.L_080dede8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r3, [r3, #0]
	sub	sp, #52
	ldr	r0, [r3, #20]
	ldr	r5, [r3, #16]
	mov	sl, r0
	movs	r0, #150
	movs	r1, #0
	mov	r8, r3
	lsls	r0, r0, #1
	movs	r2, #0
	movs	r3, #0
	str	r1, [sp, #12]
	bl	sub_080dc10c
	adds	r7, r0, #0
	movs	r6, #0
	cmp	r7, #0
	bne.n	.L_080dee22
	b.n	.L_080df156
.L_080dee22:
	bl	sub_080dc294
	movs	r0, #138
	bl	sub_081c0010
	mov	r2, r8
	ldr	r3, [r2, #20]
	cmp	r3, #0
	bne.n	.L_080dee48
	ldr	r3, [r5, #8]
	movs	r0, #128
	str	r3, [r2, #4]
	ldrh	r1, [r2, #0]
	ldr	r3, [r5, #16]
	lsls	r0, r0, #13
	str	r3, [r2, #12]
	adds	r2, #4
	bl	sub_0801489c
.L_080dee48:
	mov	r3, sp
	adds	r3, #28
	str	r3, [sp, #8]
	ldr	r4, [sp, #8]
	ldr	r3, [r5, #8]
	movs	r0, #128
	str	r3, [r4, #0]
	lsls	r0, r0, #13
	ldr	r3, [r5, #12]
	mov	r1, sp
	adds	r3, r3, r0
	str	r3, [r4, #4]
	adds	r1, #16
	ldr	r3, [r5, #16]
	mov	r2, r8
	str	r3, [r4, #8]
	str	r1, [sp, #4]
	movs	r4, #128
	ldr	r3, [r2, #4]
	lsls	r4, r4, #14
	str	r3, [r1, #0]
	mov	r0, r8
	ldr	r2, [r2, #8]
	adds	r3, r2, r4
	str	r3, [r1, #4]
	ldr	r3, [r0, #12]
	str	r3, [r1, #8]
	mov	r3, r8
	adds	r3, #52
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080dee96
	movs	r1, #160
	lsls	r1, r1, #15
	adds	r3, r2, r1
	ldr	r2, [sp, #4]
	str	r3, [r2, #4]
.L_080dee96:
	mov	r0, sl
	bl	sub_08020330
	movs	r3, #190
	lsls	r3, r3, #1
	cmp	r0, r3
	bne.n	.L_080deeb4
	ldr	r4, [sp, #4]
	movs	r0, #128
	ldr	r3, [r4, #4]
	lsls	r0, r0, #14
	adds	r3, r3, r0
	movs	r1, #1
	str	r3, [r4, #4]
	str	r1, [sp, #12]
.L_080deeb4:
	ldr	r2, [sp, #8]
	ldr	r3, [sp, #4]
	mov	r9, r2
	mov	sl, r3
.L_080deebc:
	mov	r4, sl
	mov	r0, r9
	ldr	r3, [r4, #0]
	ldr	r5, [r0, #0]
	movs	r1, #10
	subs	r3, r3, r5
	adds	r0, r6, #0
	muls	r0, r3
	bl	sub_08002054
	adds	r5, r5, r0
	str	r5, [r7, #8]
	mov	r2, r9
	mov	r1, sl
	ldr	r5, [r2, #4]
	ldr	r3, [r1, #4]
	movs	r1, #10
	subs	r3, r3, r5
	adds	r0, r6, #0
	muls	r0, r3
	bl	sub_08002054
	adds	r5, r5, r0
	str	r5, [r7, #12]
	mov	r4, sl
	mov	r0, r9
	ldr	r3, [r4, #8]
	ldr	r5, [r0, #8]
	movs	r1, #10
	subs	r3, r3, r5
	adds	r0, r6, #0
	muls	r0, r3
	bl	sub_08002054
	movs	r3, #192
	lsls	r3, r3, #8
	adds	r5, r5, r0
	movs	r1, #10
	adds	r0, r6, #0
	muls	r0, r3
	str	r5, [r7, #16]
	bl	sub_08002054
	movs	r1, #128
	lsls	r1, r1, #7
	adds	r0, r0, r1
	str	r0, [r7, #24]
	str	r0, [r7, #28]
	adds	r6, #1
	movs	r0, #1
	bl	sub_08013560
	cmp	r6, #11
	blt.n	.L_080deebc
	movs	r0, #10
	bl	sub_08013560
	mov	r3, r8
	adds	r3, #65
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_080df038
	mov	r3, r8
	adds	r3, #32
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	movs	r2, #10
	mov	fp, r2
	cmp	r3, #0
	bne.n	.L_080def52
	movs	r3, #24
	mov	fp, r3
.L_080def52:
	movs	r4, #0
	mov	sl, r4
	cmp	sl, fp
	bge.n	.L_080df030
	mov	r0, fp
	subs	r0, #1
	str	r0, [sp, #0]
	add	r6, sp, #40
	mov	r9, r6
.L_080def64:
	ldr	r3, [r7, #8]
	mov	r1, r9
	str	r3, [r1, #0]
	ldr	r3, [r7, #12]
	str	r3, [r1, #4]
	ldr	r3, [r7, #16]
	str	r3, [r1, #8]
	bl	sub_08014878
	movs	r2, #192
	lsls	r5, r0, #2
	lsls	r2, r2, #10
	adds	r5, r5, r0
	adds	r5, r5, r2
	bl	sub_08014878
	mov	r2, r9
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_0801489c
	ldr	r3, [sp, #0]
	cmp	sl, r3
	bne.n	.L_080defa6
	movs	r0, #20
	bl	sub_08013560
	ldr	r3, [r7, #8]
	str	r3, [r6, #0]
	ldr	r3, [r7, #12]
	str	r3, [r6, #4]
	ldr	r3, [r7, #16]
	str	r3, [r6, #8]
.L_080defa6:
	movs	r0, #46
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #4]
	ldr	r3, [r6, #8]
	adds	r0, #255
	bl	sub_080dc10c
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_080deff0
	mov	r4, r8
	ldr	r2, [r4, #20]
	cmp	r2, #0
	beq.n	.L_080defd6
	ldr	r3, [r2, #12]
	str	r3, [r5, #20]
	ldr	r0, [sp, #12]
	cmp	r0, #1
	bne.n	.L_080defde
	ldr	r3, [r2, #12]
	movs	r1, #128
	lsls	r1, r1, #14
	adds	r3, r3, r1
	b.n	.L_080defdc
.L_080defd6:
	ldr	r3, [r6, #4]
	ldr	r2, [pc, #392]
	adds	r3, r3, r2
.L_080defdc:
	str	r3, [r5, #20]
.L_080defde:
	ldr	r3, [pc, #392]
	adds	r2, r5, #0
	str	r3, [r5, #108]
	adds	r2, #85
	movs	r3, #2
	strb	r3, [r2, #0]
	subs	r2, #50
	movs	r3, #5
	strb	r3, [r2, #0]
.L_080deff0:
	movs	r0, #132
	bl	sub_081c0010
	movs	r0, #6
	bl	sub_08013560
	mov	r3, sl
	cmp	r3, #12
	bne.n	.L_080df028
	movs	r0, #5
	bl	sub_080ce31c
	adds	r2, r0, #0
	movs	r0, #160
	lsls	r0, r0, #23
	adds	r0, #5
	movs	r1, #5
	bl	sub_080ce458
	cmp	r0, #0
	beq.n	.L_080df028
	mov	r2, r8
	movs	r4, #24
	ldrsh	r1, [r2, r4]
	movs	r3, #26
	ldrsh	r2, [r2, r3]
	bl	sub_080ceafc
.L_080df028:
	movs	r4, #1
	add	sl, r4
	cmp	sl, fp
	blt.n	.L_080def64
.L_080df030:
	movs	r0, #10
	bl	sub_08013560
	b.n	.L_080df0d8
.L_080df038:
	mov	r3, r8
	adds	r3, #32
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	movs	r0, #10
	mov	fp, r0
	cmp	r3, #0
	bne.n	.L_080df04e
	movs	r1, #30
	mov	fp, r1
.L_080df04e:
	mov	r2, fp
	cmp	r2, #0
	beq.n	.L_080df0d2
	add	r6, sp, #40
	mov	sl, fp
.L_080df058:
	ldr	r3, [r7, #8]
	str	r3, [r6, #0]
	ldr	r3, [r7, #12]
	str	r3, [r6, #4]
	ldr	r3, [r7, #16]
	str	r3, [r6, #8]
	bl	sub_08014878
	movs	r3, #192
	lsls	r5, r0, #2
	lsls	r3, r3, #10
	adds	r5, r5, r0
	adds	r5, r5, r3
	bl	sub_08014878
	adds	r2, r6, #0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_0801489c
	movs	r0, #168
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #4]
	ldr	r3, [r6, #8]
	lsls	r0, r0, #2
	bl	sub_080dc10c
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_080df0c0
	ldr	r3, [pc, #212]
	adds	r2, r5, #0
	str	r3, [r5, #108]
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r4, #13
	ldr	r1, [r5, #80]
	negs	r4, r4
	ldrb	r2, [r1, #9]
	adds	r3, r4, #0
	ands	r2, r3
	movs	r3, #8
	orrs	r2, r3
	strb	r2, [r1, #9]
	movs	r1, #8
	bl	sub_08020090
	adds	r0, r5, #0
	movs	r1, #7
	bl	sub_08020278
.L_080df0c0:
	movs	r0, #6
	bl	sub_08013560
	movs	r0, #1
	negs	r0, r0
	add	sl, r0
	mov	r1, sl
	cmp	r1, #0
	bne.n	.L_080df058
.L_080df0d2:
	movs	r0, #70
	bl	sub_08013560
.L_080df0d8:
	ldr	r2, [sp, #4]
	ldr	r3, [sp, #8]
	movs	r6, #0
	mov	sl, r2
	mov	r8, r3
.L_080df0e2:
	mov	r4, r8
	mov	r0, sl
	ldr	r3, [r4, #0]
	ldr	r5, [r0, #0]
	movs	r1, #10
	subs	r3, r3, r5
	adds	r0, r6, #0
	muls	r0, r3
	bl	sub_08002054
	adds	r5, r5, r0
	str	r5, [r7, #8]
	mov	r2, sl
	mov	r1, r8
	ldr	r5, [r2, #4]
	ldr	r3, [r1, #4]
	movs	r1, #10
	subs	r3, r3, r5
	adds	r0, r6, #0
	muls	r0, r3
	bl	sub_08002054
	adds	r5, r5, r0
	str	r5, [r7, #12]
	mov	r4, r8
	mov	r0, sl
	ldr	r3, [r4, #8]
	ldr	r5, [r0, #8]
	movs	r1, #10
	subs	r3, r3, r5
	adds	r0, r6, #0
	muls	r0, r3
	bl	sub_08002054
	ldr	r3, [pc, #72]
	adds	r5, r5, r0
	movs	r1, #10
	adds	r0, r6, #0
	muls	r0, r3
	str	r5, [r7, #16]
	bl	sub_08002054
	movs	r1, #128
	lsls	r1, r1, #9
	adds	r0, r0, r1
	str	r0, [r7, #24]
	str	r0, [r7, #28]
	adds	r6, #1
	movs	r0, #1
	bl	sub_08013560
	cmp	r6, #11
	blt.n	.L_080df0e2
	adds	r0, r7, #0
	bl	sub_080200c8
	bl	sub_080dc384
.L_080df156:
	add	sp, #52
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xffe00000
	.4byte 0x080ded85
	.4byte 0x080deda9
	.2byte 0x4000
	.2byte 0xffff
	.2byte 0xb560
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r3, [r3, #0]
	sub	sp, #12
	ldr	r5, [r3, #20]
	mov	r6, sp
	ldr	r3, [r5, #8]
	str	r3, [r6, #0]
	bl	sub_08014878
	ldr	r3, [r5, #12]
	lsls	r0, r0, #4
	movs	r2, #192
	lsls	r2, r2, #13
	subs	r3, r3, r0
	adds	r3, r3, r2
	str	r3, [r6, #4]
	ldr	r3, [r5, #16]
	str	r3, [r6, #8]
	bl	sub_08014878
	lsls	r5, r0, #1
	adds	r5, r5, r0
	bl	sub_08014878
	lsls	r5, r5, #4
	adds	r1, r0, #0
	adds	r2, r6, #0
	adds	r0, r5, #0
	bl	sub_0801489c
	movs	r0, #209
	lsls	r0, r0, #1
	adds	r0, #255
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #4]
	ldr	r3, [r6, #8]
	bl	sub_080dc10c
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_080df1f2
	adds	r2, r5, #0
	adds	r2, #85
	movs	r3, #2
	strb	r3, [r2, #0]
	movs	r3, #200
	lsls	r3, r3, #5
	adds	r3, #153
	str	r3, [r5, #72]
	movs	r1, #0
	bl	sub_08020090
	adds	r2, r5, #0
	adds	r2, #94
	movs	r3, #12
	strh	r3, [r2, #0]
	ldr	r1, [pc, #12]
	adds	r0, r5, #0
	bl	sub_080200a8
.L_080df1f2:
	add	sp, #12
	pop	{r5, r6, pc}
	movs	r0, r0
	.2byte 0x0e54
	.2byte 0x080f
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r3, [r3, #0]
	sub	sp, #128
	str	r3, [sp, #32]
	ldr	r0, [r3, #16]
	str	r0, [sp, #28]
	ldr	r7, [r3, #20]
	cmp	r7, #0
	bne.n	.L_080df222
	b.n	.L_080df6de
.L_080df222:
	bl	sub_080dc294
	ldr	r1, [sp, #28]
	str	r7, [r1, #104]
	ldr	r0, [sp, #28]
	ldr	r1, [pc, #772]
	bl	sub_080200a8
	ldr	r3, [sp, #32]
	movs	r2, #36
	ldr	r0, [r3, #4]
	add	r2, sp
	str	r0, [r2, #0]
	mov	sl, r2
	ldr	r1, [r3, #8]
	movs	r2, #128
	lsls	r2, r2, #13
	adds	r1, r1, r2
	mov	r3, sl
	str	r1, [r3, #4]
	ldr	r3, [sp, #32]
	ldr	r2, [r3, #12]
	mov	r3, sl
	str	r2, [r3, #8]
	movs	r3, #128
	lsls	r3, r3, #14
	adds	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #8
	bl	.L_080df820
	mov	r1, sl
	adds	r6, r0, #0
	ldr	r2, [pc, #720]
	ldr	r0, [r1, #0]
	mov	r3, sl
	adds	r0, r0, r2
	ldr	r1, [r1, #4]
	ldr	r2, [r3, #8]
	movs	r3, #0
	str	r6, [sp, #20]
	bl	.L_080df820
	adds	r5, r0, #0
	ldr	r0, [sp, #20]
	str	r5, [sp, #24]
	cmp	r0, #0
	beq.n	.L_080df286
	cmp	r5, #0
	bne.n	.L_080df28c
.L_080df286:
	bl	sub_080dc384
	b.n	.L_080df6de
.L_080df28c:
	movs	r0, #15
	bl	sub_08013560
	ldr	r1, [r7, #8]
	mov	r2, sl
	str	r1, [r2, #0]
	ldr	r2, [r7, #12]
	movs	r3, #128
	lsls	r3, r3, #13
	adds	r2, r2, r3
	mov	r0, sl
	str	r2, [r0, #4]
	ldr	r3, [r7, #16]
	str	r3, [r0, #8]
	movs	r0, #128
	lsls	r0, r0, #13
	adds	r1, r1, r0
	adds	r0, r6, #0
	bl	sub_08020148
	mov	r2, sl
	ldr	r1, [r2, #0]
	ldr	r3, [pc, #640]
	mov	r0, sl
	adds	r1, r1, r3
	ldr	r2, [r2, #4]
	ldr	r3, [r0, #8]
	adds	r0, r5, #0
	bl	sub_08020148
	adds	r0, r6, #0
	bl	sub_08020150
	adds	r0, r5, #0
	bl	sub_08020150
	mov	r1, sl
	ldr	r3, [r1, #0]
	movs	r2, #128
	lsls	r2, r2, #13
	adds	r3, r3, r2
	movs	r2, #0
	str	r2, [r6, #36]
	str	r3, [r6, #8]
	ldr	r0, [pc, #596]
	ldr	r3, [r1, #0]
	movs	r1, #144
	adds	r3, r3, r0
	str	r3, [r5, #8]
	ldr	r3, [pc, #592]
	str	r2, [r5, #36]
	str	r3, [r7, #108]
	lsls	r1, r1, #3
	ldr	r0, [pc, #588]
	bl	sub_080145a8
	movs	r0, #130
	bl	sub_081c0010
	adds	r1, r7, #0
	adds	r1, #85
	movs	r3, #4
	str	r1, [sp, #16]
	adds	r0, r7, #0
	strb	r3, [r1, #0]
	movs	r1, #0
	bl	sub_08020218
	ldr	r2, [sp, #20]
	cmp	r2, #0
	beq.n	.L_080df35c
	ldr	r3, [sp, #24]
	cmp	r3, #0
	beq.n	.L_080df35c
	ldr	r3, [r7, #12]
	ldr	r2, [r7, #20]
	movs	r0, #192
	subs	r3, r3, r2
	lsls	r0, r0, #13
	cmp	r3, r0
	bgt.n	.L_080df35c
	movs	r1, #192
	lsls	r1, r1, #7
.L_080df332:
	ldr	r3, [r6, #12]
	movs	r0, #1
	adds	r3, r3, r1
	str	r3, [r6, #12]
	str	r1, [sp, #0]
	ldr	r3, [r5, #12]
	adds	r3, r3, r1
	str	r3, [r5, #12]
	ldr	r3, [r7, #12]
	adds	r3, r3, r1
	str	r3, [r7, #12]
	bl	sub_08013560
	ldr	r2, [r7, #20]
	ldr	r3, [r7, #12]
	ldr	r1, [sp, #0]
	subs	r3, r3, r2
	movs	r2, #192
	lsls	r2, r2, #13
	cmp	r3, r2
	ble.n	.L_080df332
.L_080df35c:
	ldr	r3, [sp, #20]
	ldr	r0, [sp, #24]
	movs	r1, #128
	movs	r2, #128
	lsls	r1, r1, #11
	lsls	r2, r2, #8
	str	r1, [r3, #48]
	str	r2, [r3, #52]
	movs	r3, #204
	str	r2, [r0, #52]
	lsls	r3, r3, #7
	movs	r2, #204
	str	r1, [r0, #48]
	adds	r3, #102
	lsls	r2, r2, #6
	adds	r1, r7, #0
	str	r3, [r7, #48]
	adds	r1, #90
	movs	r3, #0
	adds	r2, #51
	str	r2, [r7, #52]
	strb	r3, [r1, #0]
	adds	r3, r7, #0
	adds	r3, #34
	str	r3, [sp, #12]
	ldr	r1, [sp, #12]
	movs	r3, #2
	strb	r3, [r1, #0]
	ldr	r3, [pc, #432]
	movs	r1, #128
	lsls	r1, r1, #13
	mov	fp, r3
	mov	r5, sl
	mov	r9, r1
	str	r0, [sp, #8]
	b.n	.L_080df550
.L_080df3a4:
	mov	r2, fp
	ldr	r0, [r2, #0]
	bl	sub_080dcf54
	movs	r3, #255
	lsls	r0, r0, #16
	lsls	r3, r3, #8
	lsrs	r6, r0, #16
	adds	r3, #255
	cmp	r6, r3
	bne.n	.L_080df3f2
	ldr	r1, [r7, #8]
	ldr	r0, [sp, #20]
	str	r1, [r5, #0]
	ldr	r2, [r7, #12]
	add	r1, r9
	add	r2, r9
	str	r2, [r5, #4]
	ldr	r3, [r7, #16]
	str	r3, [r5, #8]
	bl	sub_08020148
	ldr	r1, [r5, #0]
	ldr	r0, [pc, #360]
	ldr	r2, [r5, #4]
	adds	r1, r1, r0
	ldr	r3, [r5, #8]
	ldr	r0, [sp, #24]
	bl	sub_08020148
	ldr	r0, [sp, #20]
	movs	r1, #1
	bl	sub_08020090
	ldr	r0, [sp, #24]
	movs	r1, #1
	bl	sub_08020090
	b.n	.L_080df550
.L_080df3f2:
	ldr	r3, [r7, #8]
	movs	r0, #128
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	lsls	r0, r0, #10
	add	r3, r9
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	adds	r1, r6, #0
	str	r3, [r5, #8]
	adds	r2, r5, #0
	bl	sub_0801489c
	ldr	r1, [r5, #0]
	ldr	r0, [sp, #20]
	add	r1, r9
	ldr	r2, [r5, #4]
	ldr	r3, [r5, #8]
	bl	sub_08020148
	ldr	r1, [r5, #0]
	ldr	r2, [pc, #284]
	ldr	r0, [sp, #24]
	adds	r1, r1, r2
	ldr	r3, [r5, #8]
	ldr	r2, [r5, #4]
	bl	sub_08020148
	ldr	r0, [sp, #20]
	bl	sub_08020150
	ldr	r0, [sp, #24]
	bl	sub_08020150
	ldr	r3, [r7, #8]
	mov	r0, r9
	str	r3, [r5, #0]
	ldr	r3, [r7, #20]
	adds	r1, r6, #0
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	adds	r2, r5, #0
	str	r3, [r5, #8]
	bl	sub_0801489c
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_08020298
	mov	r8, r0
	cmp	r0, #0
	bne.n	.L_080df478
	ldr	r3, [r7, #20]
	movs	r0, #128
	lsls	r0, r0, #13
	adds	r3, r3, r0
	str	r3, [r7, #20]
	mov	r1, sl
	adds	r0, r7, #0
	bl	sub_08020210
	ldr	r3, [r7, #20]
	ldr	r1, [pc, #204]
	adds	r3, r3, r1
	str	r3, [r7, #20]
	cmp	r0, #0
	ble.n	.L_080df49c
.L_080df478:
	ldr	r0, [sp, #20]
	movs	r1, #4
	bl	sub_08020090
	ldr	r0, [sp, #24]
	movs	r1, #4
	bl	sub_08020090
	ldr	r3, [pc, #192]
	movs	r2, #15
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080df550
	movs	r0, #114
	bl	sub_081c0010
	b.n	.L_080df550
.L_080df49c:
	movs	r0, #175
	bl	sub_081c0010
	ldr	r0, [sp, #20]
	mov	r2, sl
	movs	r1, #4
	ldr	r5, [r2, #0]
	ldr	r6, [r2, #8]
	bl	sub_08020090
	ldr	r0, [sp, #24]
	movs	r1, #4
	bl	sub_08020090
	movs	r0, #15
	bl	sub_08013560
	movs	r1, #204
	adds	r3, r7, #0
	lsls	r1, r1, #6
	adds	r1, #51
	adds	r3, #91
	mov	r0, r8
	strb	r0, [r3, #0]
	str	r1, [r7, #48]
	str	r1, [r7, #52]
	mov	r2, sl
	mov	r0, sl
	ldr	r1, [r2, #0]
	ldr	r3, [r0, #8]
	ldr	r2, [r2, #4]
	adds	r0, r7, #0
	bl	sub_08020148
	ldr	r3, [sp, #20]
	movs	r1, #204
	lsls	r1, r1, #6
	adds	r1, #51
	str	r1, [r3, #48]
	str	r1, [r3, #52]
	ldr	r2, [sp, #8]
	mov	r3, sl
	str	r1, [r2, #48]
	str	r1, [r2, #52]
	movs	r0, #128
	ldr	r1, [r3, #0]
	lsls	r0, r0, #13
	adds	r1, r1, r0
	ldr	r2, [r3, #4]
	ldr	r0, [sp, #20]
	ldr	r3, [r3, #8]
	bl	sub_08020148
	mov	r2, sl
	ldr	r1, [r2, #0]
	ldr	r3, [pc, #48]
	mov	r0, sl
	adds	r1, r1, r3
	ldr	r2, [r2, #4]
	ldr	r3, [r0, #8]
	ldr	r0, [sp, #8]
	bl	sub_08020148
	adds	r0, r7, #0
	bl	sub_08020150
	mov	r1, r8
	str	r5, [r7, #8]
	str	r6, [r7, #16]
	str	r1, [r7, #36]
	str	r1, [r7, #44]
	movs	r0, #10
	bl	sub_08013560
	b.n	.L_080df568
	movs	r0, r0
	.4byte 0x080f0e60
	.4byte 0xffe00000
	.4byte 0xfff00000
	.4byte 0x080db91d
	.4byte 0x080df175
	.4byte 0x03001150
	.2byte 0x122c
	.2byte 0x0300
.L_080df550:
	movs	r0, #1
	bl	sub_08013560
	mov	r3, fp
	ldr	r2, [r3, #4]
	movs	r3, #129
	lsls	r3, r3, #2
	adds	r3, #255
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L_080df568
	b.n	.L_080df3a4
.L_080df568:
	ldr	r0, [sp, #20]
	movs	r1, #4
	bl	sub_08020090
	ldr	r0, [sp, #24]
	movs	r1, #4
	bl	sub_08020090
	ldr	r0, [pc, #368]
	bl	sub_08014644
	movs	r0, #135
	bl	sub_081c0010
	movs	r0, #15
	bl	sub_08013560
	movs	r0, #135
	bl	sub_081c0010
	movs	r0, #15
	bl	sub_08013560
	ldr	r3, [r7, #8]
	mov	r0, sl
	str	r3, [r0, #0]
	ldr	r3, [r7, #12]
	movs	r1, #128
	lsls	r1, r1, #13
	adds	r3, r3, r1
	str	r3, [r0, #4]
	ldr	r3, [r7, #16]
	mov	r2, sp
	adds	r2, #48
	str	r3, [r0, #8]
	movs	r3, #128
	str	r2, [sp, #4]
	lsls	r3, r3, #10
	movs	r0, #19
	mov	r9, r3
	mov	r8, r0
.L_080df5ba:
	mov	r0, sl
	ldr	r3, [r0, #8]
	movs	r0, #209
	mov	r2, sl
	lsls	r0, r0, #1
	ldr	r1, [r2, #0]
	adds	r0, #255
	ldr	r2, [r2, #4]
	bl	sub_080dc10c
	ldr	r2, [sp, #4]
	adds	r6, r0, #0
	stmia	r2!, {r6}
	adds	r1, r2, #0
	str	r1, [sp, #4]
	cmp	r6, #0
	beq.n	.L_080df614
	ldr	r1, [pc, #272]
	bl	sub_080200a8
	bl	sub_08014878
	mov	r3, r9
	adds	r2, r6, #0
	adds	r2, #85
	str	r3, [r6, #52]
	add	r0, r9
	movs	r3, #0
	str	r0, [r6, #48]
	strb	r3, [r2, #0]
	bl	sub_08014878
	lsls	r5, r0, #1
	adds	r5, r5, r0
	movs	r0, #128
	lsls	r0, r0, #12
	lsls	r5, r5, #3
	adds	r5, r5, r0
	bl	sub_08014878
	adds	r1, r5, #0
	adds	r2, r0, #0
	adds	r0, r6, #0
	bl	sub_080db974
.L_080df614:
	movs	r1, #1
	negs	r1, r1
	add	r8, r1
	mov	r2, r8
	cmp	r2, #0
	bge.n	.L_080df5ba
	movs	r0, #131
	bl	sub_081c0010
	ldr	r0, [sp, #20]
	bl	sub_080200c8
	ldr	r0, [sp, #24]
	bl	sub_080200c8
	ldr	r3, [sp, #32]
	adds	r0, r7, #0
	adds	r3, #64
	ldrb	r1, [r3, #0]
	bl	sub_08020278
	ldr	r3, [sp, #32]
	adds	r0, r7, #0
	ldr	r1, [r3, #60]
	bl	sub_080200a8
	ldr	r0, [sp, #32]
	movs	r2, #0
	ldr	r3, [r0, #56]
	str	r3, [r7, #108]
	ldr	r1, [sp, #16]
	movs	r3, #3
	strb	r3, [r1, #0]
	movs	r3, #160
	lsls	r3, r3, #12
	str	r3, [r7, #40]
	movs	r3, #204
	lsls	r3, r3, #6
	adds	r3, #51
	str	r3, [r7, #68]
	ldr	r3, [sp, #12]
	movs	r1, #0
	strb	r2, [r3, #0]
	ldr	r0, [sp, #28]
	str	r2, [r0, #108]
	ldr	r0, [sp, #28]
	bl	sub_08020278
	ldr	r3, [sp, #32]
	adds	r3, #52
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_080df6da
	ldr	r3, [r7, #40]
	movs	r1, #0
	mov	r8, r1
	cmp	r3, #0
	blt.n	.L_080df6a2
.L_080df68c:
	movs	r0, #1
	bl	sub_08013560
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	cmp	r3, #89
	bgt.n	.L_080df6a2
	ldr	r3, [r7, #40]
	cmp	r3, #0
	bge.n	.L_080df68c
.L_080df6a2:
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [r7, #40]
	movs	r0, #0
	mov	r8, r0
	cmp	r3, #0
	bge.n	.L_080df6c8
.L_080df6b2:
	movs	r0, #1
	bl	sub_08013560
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	cmp	r2, #89
	bgt.n	.L_080df6c8
	ldr	r3, [r7, #40]
	cmp	r3, #0
	blt.n	.L_080df6b2
.L_080df6c8:
	adds	r0, r7, #0
	bl	sub_080dfb0c
	bl	sub_080dc384
	movs	r0, #30
	bl	sub_08013560
	b.n	.L_080df6de
.L_080df6da:
	bl	sub_080dc384
.L_080df6de:
	add	sp, #128
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x080df175
	.2byte 0x0e78
	.2byte 0x080f
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #224
	ldr	r3, [r3, #0]
	sub	sp, #20
	mov	sl, r3
	bl	sub_080dc294
	mov	r3, sl
	ldr	r0, [r3, #4]
	add	r5, sp, #8
	str	r0, [r5, #0]
	ldr	r1, [r3, #8]
	movs	r3, #128
	lsls	r3, r3, #13
	adds	r1, r1, r3
	str	r1, [r5, #4]
	mov	r3, sl
	ldr	r2, [r3, #12]
	movs	r3, #128
	lsls	r3, r3, #14
	adds	r0, r0, r3
	movs	r3, #128
	str	r2, [r5, #8]
	lsls	r3, r3, #8
	bl	.L_080df820
	ldr	r3, [pc, #220]
	str	r0, [sp, #0]
	ldr	r0, [r5, #0]
	ldr	r1, [r5, #4]
	adds	r0, r0, r3
	ldr	r2, [r5, #8]
	movs	r3, #0
	bl	.L_080df820
	str	r0, [sp, #4]
	movs	r0, #15
	mov	fp, sp
	bl	sub_08013560
	movs	r0, #1
	mov	r7, fp
	mov	r8, r0
.L_080df75a:
	ldmia	r7!, {r6}
	cmp	r6, #0
	beq.n	.L_080df76c
	movs	r1, #192
	ldrh	r2, [r6, #6]
	adds	r0, r6, #0
	lsls	r1, r1, #13
	bl	sub_080db974
.L_080df76c:
	movs	r3, #1
	negs	r3, r3
	add	r8, r3
	mov	r0, r8
	cmp	r0, #0
	bge.n	.L_080df75a
	ldr	r0, [sp, #0]
	bl	sub_08020150
	movs	r0, #134
	bl	sub_081c0010
	movs	r0, #128
	movs	r3, #23
	lsls	r0, r0, #10
	adds	r7, r5, #0
	mov	r8, r3
	mov	r9, r0
.L_080df790:
	mov	r3, sl
	ldr	r1, [r3, #4]
	movs	r0, #128
	str	r1, [r7, #0]
	lsls	r0, r0, #13
	ldr	r2, [r3, #8]
	adds	r2, r2, r0
	str	r2, [r7, #4]
	movs	r0, #209
	ldr	r3, [r3, #12]
	lsls	r0, r0, #1
	str	r3, [r7, #8]
	adds	r0, #255
	bl	sub_080dc10c
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_080df7ec
	ldr	r1, [pc, #100]
	bl	sub_080200a8
	bl	sub_08014878
	mov	r3, r9
	adds	r2, r6, #0
	adds	r2, #85
	str	r3, [r6, #52]
	add	r0, r9
	movs	r3, #0
	str	r0, [r6, #48]
	strb	r3, [r2, #0]
	bl	sub_08014878
	lsls	r5, r0, #1
	adds	r5, r5, r0
	movs	r0, #128
	lsls	r0, r0, #12
	lsls	r5, r5, #3
	adds	r5, r5, r0
	bl	sub_08014878
	adds	r1, r5, #0
	adds	r2, r0, #0
	adds	r0, r6, #0
	bl	sub_080db974
.L_080df7ec:
	movs	r3, #1
	negs	r3, r3
	add	r8, r3
	mov	r0, r8
	cmp	r0, #0
	bge.n	.L_080df790
	ldr	r0, [sp, #0]
	bl	sub_080200c8
	mov	r3, fp
	ldr	r0, [r3, #4]
	bl	sub_080200c8
	bl	sub_080dc384
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xffe00000
	.2byte 0x0e78
	.2byte 0x080f
.L_080df820:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	movs	r0, #138
	adds	r6, r1, #0
	mov	r8, r2
	adds	r7, r3, #0
	bl	sub_081c0010
	movs	r0, #139
	adds	r1, r5, #0
	lsls	r0, r0, #1
	adds	r2, r6, #0
	mov	r3, r8
	bl	sub_080dc10c
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_080df89e
	movs	r3, #128
	lsls	r3, r3, #7
	str	r3, [r5, #28]
	str	r3, [r5, #24]
	movs	r3, #192
	lsls	r3, r3, #10
	adds	r2, r5, #0
	str	r3, [r5, #48]
	str	r3, [r5, #52]
	adds	r2, #90
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r1, #1
	bl	sub_08020090
	ldr	r3, [r5, #24]
	movs	r2, #128
	lsls	r2, r2, #9
	cmp	r3, r2
	bge.n	.L_080df89c
	ldr	r6, [pc, #36]
.L_080df872:
	movs	r2, #128
	lsls	r2, r2, #4
	adds	r3, r3, r2
	str	r3, [r5, #28]
	str	r3, [r5, #24]
	ldrh	r3, [r5, #6]
	movs	r0, #1
	adds	r3, r3, r6
	strh	r3, [r5, #6]
	bl	sub_08013560
	movs	r2, #255
	ldr	r3, [r5, #24]
	lsls	r2, r2, #8
	adds	r2, #255
	cmp	r3, r2
	ble.n	.L_080df872
	b.n	.L_080df89c
	movs	r0, r0
	.2byte 0x2000
	.2byte 0x0000
.L_080df89c:
	strh	r7, [r5, #6]
.L_080df89e:
	adds	r0, r5, #0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.align 2, 0
