.syntax unified
	.thumb
	.set sub_02000736, 0x02000736
	.set sub_02000740, 0x02000740
	.set sub_0200076c, 0x0200076c
	.set sub_02000784, 0x02000784
	.set sub_0200078e, 0x0200078e
	.set sub_020007ac, 0x020007ac
	.set sub_020007c8, 0x020007c8
	.set sub_020007dc, 0x020007dc
	.set sub_02000838, 0x02000838
	.set sub_02000860, 0x02000860
	.set sub_0200086a, 0x0200086a
	.set sub_02000882, 0x02000882
	.set sub_02000884, 0x02000884
	.set sub_02000892, 0x02000892
	.set sub_02000928, 0x02000928
	.set sub_02000940, 0x02000940
	.set sub_0200094e, 0x0200094e
	.set sub_0200096e, 0x0200096e
	.set sub_02000984, 0x02000984
	.set sub_02000994, 0x02000994
	.set sub_02000a5a, 0x02000a5a
	.set sub_02000aa6, 0x02000aa6
	.set sub_02000aac, 0x02000aac
	.set sub_02000aba, 0x02000aba
	.set sub_02000ac8, 0x02000ac8
	.set sub_02000ae8, 0x02000ae8
	.set sub_02000aec, 0x02000aec
	.set sub_02000b04, 0x02000b04
	.set sub_02000b5e, 0x02000b5e
	.set sub_02000b64, 0x02000b64
	.set sub_02000b66, 0x02000b66
	.set sub_02000b6e, 0x02000b6e
	.set sub_02000b88, 0x02000b88
	.set sub_02000b96, 0x02000b96
	.set sub_02000b98, 0x02000b98
	.set sub_02000ba2, 0x02000ba2
	.set sub_02000bb0, 0x02000bb0
	.set sub_02000bce, 0x02000bce
	.set sub_02000bdc, 0x02000bdc
	.set sub_02000bf0, 0x02000bf0
	.set sub_02000c2c, 0x02000c2c
	.set sub_02000c86, 0x02000c86
	.set sub_02000c8c, 0x02000c8c
	.set sub_02000c96, 0x02000c96
	.set sub_02000c9c, 0x02000c9c
	.set sub_02000caa, 0x02000caa
	.set sub_02000cba, 0x02000cba
	.set sub_02000ce8, 0x02000ce8
	.set sub_02000cfc, 0x02000cfc
	.set sub_02000d56, 0x02000d56
	.set sub_02000d64, 0x02000d64
	.set sub_02000d86, 0x02000d86
	.set sub_02000dca, 0x02000dca
	.set sub_02000dd2, 0x02000dd2
	.global Overlay_02000000
Overlay_02000000:
	.4byte 0x47204c00
	.4byte 0x02008415
	.4byte 0x47204c00
	.4byte 0x02008315
	.4byte 0x47204c00
	.4byte 0x02008349
	.4byte 0x47204c00
	.4byte 0x02008351
	.4byte 0x47204c00
	.4byte 0x020083e5
	.4byte 0x47204c00
	.4byte 0x02008345
	.4byte 0x23036d00
	.4byte 0x40197a42
	.4byte 0x425b230d
	.4byte 0x40130089
	.4byte 0x7243430b
	.2byte 0x4770
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r4, r0, #0
	adds	r5, r1, #0
	adds	r6, r2, #0
	adds	r0, r3, #0
	adds	r2, r5, #0
	adds	r1, r4, #0
	adds	r3, r6, #0
	bl	sub_02000736
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_02000096
	ldr	r1, [r5, #80]
	movs	r3, #13
	ldrb	r2, [r1, #9]
	negs	r3, r3
	ands	r3, r2
	adds	r2, r5, #0
	strb	r3, [r1, #9]
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r2, #4
	movs	r3, #8
	strb	r3, [r2, #0]
	movs	r1, #0
	bl	sub_0200076c
	adds	r0, r5, #0
	movs	r1, #14
	bl	sub_020007dc
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_02000784
	adds	r0, r5, #0
	b.n	.L_02000098
.L_02000096:
	movs	r0, #0
.L_02000098:
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	.2byte 0x0000
	push	{r5, r6, lr}
	adds	r4, r0, #0
	adds	r5, r1, #0
	adds	r6, r2, #0
	adds	r0, r3, #0
	adds	r2, r5, #0
	adds	r1, r4, #0
	adds	r3, r6, #0
	bl	sub_0200078e
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_020000fa
	ldr	r1, [r5, #80]
	movs	r3, #13
	ldrb	r2, [r1, #9]
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	adds	r2, r5, #0
	strb	r3, [r1, #9]
	adds	r2, #85
	movs	r3, #0
	strb	r3, [r2, #0]
	adds	r2, #4
	movs	r3, #8
	strb	r3, [r2, #0]
	movs	r1, #0
	bl	sub_020007c8
	adds	r0, r5, #0
	movs	r1, #15
	bl	sub_02000838
	adds	r1, r5, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	movs	r2, #2
	orrs	r3, r2
	strb	r3, [r1, #0]
	adds	r0, r5, #0
	b.n	.L_020000fc
.L_020000fa:
	movs	r0, #0
.L_020000fc:
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	.2byte 0x0000
	.4byte 0x6c426883
	.4byte 0x6083189b
	.4byte 0x68c36c82
	.4byte 0x60c3189b
	.4byte 0x69036cc2
	.4byte 0x6103189b
	.4byte 0x69836b02
	.4byte 0x6183189b
	.4byte 0x69c36b42
	.4byte 0x61c3189b
	.4byte 0x30646d01
	.4byte 0x88028bcb
	.4byte 0x83cb189b
	.2byte 0x4770
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #8
	adds	r6, r1, #0
	ldr	r1, [sp, #48]
	adds	r5, r0, #0
	movs	r0, #0
	mov	r8, r2
	str	r3, [sp, #4]
	mov	sl, r1
	ldr	r7, [sp, #52]
	bl	sub_02000882
	movs	r3, #128
	lsls	r3, r3, #13
	mov	r2, sl
	ands	r3, r2
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L_0200017a
	cmp	r7, #0
	beq.n	.L_0200017a
	movs	r3, #24
	ldrsh	r0, [r7, r3]
	adds	r2, r6, #0
	b.n	.L_0200017e
.L_0200017a:
	adds	r2, r6, #0
	movs	r0, #222
.L_0200017e:
	adds	r1, r5, #0
	mov	r3, r8
	bl	sub_02000860
	adds	r6, r0, #0
	cmp	r6, #0
	bne.n	.L_0200018e
	b.n	.L_020002f6
.L_0200018e:
	ldr	r1, [r6, #80]
	mov	r8, r1
	mov	r1, sl
	movs	r5, #15
	adds	r1, #1
	ands	r1, r5
	adds	r0, r6, #0
	bl	sub_0200086a
	mov	r3, sl
	ldr	r2, [pc, #356]
	ands	r3, r5
	lsls	r3, r3, #2
	ldr	r1, [r2, r3]
	adds	r0, r6, #0
	mov	fp, r3
	bl	sub_02000884
	adds	r3, r6, #0
	movs	r0, #0
	adds	r3, #85
	strb	r0, [r3, #0]
	mov	r3, r8
	adds	r3, #38
	strb	r0, [r3, #0]
	ldr	r3, [pc, #328]
	str	r3, [r6, #108]
	ldr	r3, [sp, #4]
	str	r3, [r6, #68]
	ldr	r3, [sp, #40]
	str	r3, [r6, #72]
	ldr	r3, [sp, #44]
	mov	r1, r9
	str	r3, [r6, #76]
	ldr	r3, [r1, #80]
	ldrb	r3, [r3, #9]
	movs	r2, #12
	ands	r2, r3
	mov	r3, r8
	ldrb	r1, [r3, #9]
	movs	r3, #13
	negs	r3, r3
	mov	r9, r3
	ands	r3, r1
	orrs	r3, r2
	adds	r2, r6, #0
	mov	r1, r8
	adds	r2, #100
	strb	r3, [r1, #9]
	adds	r3, r2, #0
	str	r0, [r6, #48]
	str	r0, [r6, #52]
	str	r2, [sp, #0]
	strh	r0, [r3, #0]
	ldr	r3, [pc, #276]
	mov	r1, sl
	ands	r3, r1
	movs	r5, #3
	cmp	r3, #0
	beq.n	.L_020002f6
	cmp	r7, #0
	beq.n	.L_020002f6
	movs	r3, #128
	lsls	r3, r3, #9
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_0200021c
	ldr	r1, [r7, #4]
	adds	r0, r6, #0
	bl	sub_0200096e
.L_0200021c:
	movs	r3, #128
	lsls	r3, r3, #10
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_02000248
	adds	r1, r6, #0
	adds	r1, #35
	ldrb	r2, [r1, #0]
	movs	r3, #254
	ands	r3, r2
	strb	r3, [r1, #0]
	mov	r3, r8
	ldrb	r2, [r7, #0]
	ldrb	r1, [r3, #9]
	ands	r2, r5
	mov	r3, r9
	ands	r3, r1
	lsls	r2, r2, #2
	orrs	r3, r2
	mov	r1, r8
	strb	r3, [r1, #9]
.L_02000248:
	movs	r2, #128
	lsls	r2, r2, #12
	mov	r3, sl
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_0200025c
	ldr	r3, [r7, #8]
	str	r3, [r6, #24]
	ldr	r3, [r7, #12]
	str	r3, [r6, #28]
.L_0200025c:
	movs	r3, #128
	lsls	r3, r3, #11
	mov	r1, sl
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_020002a6
	ldr	r3, [pc, #156]
	mov	r1, fp
	ldr	r5, [r3, r1]
	cmp	r2, #0
	beq.n	.L_0200028a
	ldr	r0, [r7, #16]
	ldr	r3, [r6, #24]
	ldr	r1, [r5, #12]
	subs	r0, r0, r3
	bl	sub_02000928
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [r6, #28]
	ldr	r1, [r5, #12]
	subs	r0, r0, r3
	b.n	.L_020002a0
.L_0200028a:
	ldr	r0, [r7, #16]
	ldr	r2, [pc, #128]
	ldr	r1, [r5, #12]
	adds	r0, r0, r2
	bl	sub_02000940
	str	r0, [r6, #48]
	ldr	r0, [r7, #20]
	ldr	r3, [pc, #116]
	ldr	r1, [r5, #12]
	adds	r0, r0, r3
.L_020002a0:
	bl	sub_0200094e
	str	r0, [r6, #52]
.L_020002a6:
	movs	r3, #128
	lsls	r3, r3, #14
	mov	r1, sl
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_020002c2
	adds	r0, r6, #0
	movs	r1, #1
	bl	sub_02000984
	ldr	r1, [r7, #28]
	adds	r0, r6, #0
	bl	sub_02000994
.L_020002c2:
	movs	r3, #128
	lsls	r3, r3, #15
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020002d4
	ldrh	r3, [r7, #32]
	mov	r1, r8
	strh	r3, [r1, #30]
.L_020002d4:
	movs	r3, #128
	lsls	r3, r3, #16
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020002e6
	ldrh	r3, [r7, #34]
	ldr	r1, [sp, #0]
	strh	r3, [r1, #0]
.L_020002e6:
	movs	r3, #128
	lsls	r3, r3, #17
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_020002f6
	ldr	r3, [r7, #36]
	str	r3, [r6, #108]
.L_020002f6:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x0200880c
	.4byte 0x02008105
	.2byte 0x0000
	.2byte 0xffff
	.2byte 0xb500
	ldr	r3, [pc, #28]
	movs	r1, #224
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #20]
	cmp	r2, r3
	bne.n	.L_0200032c
	ldr	r0, [pc, #16]
	b.n	.L_0200032e
.L_0200032c:
	ldr	r0, [pc, #16]
.L_0200032e:
	pop	{r1}
	bx	r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000001d
	.4byte 0x020088d8
	.4byte 0x02008818
	.4byte 0x47702000
	.4byte 0x47704800
	.2byte 0x8920
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #28]
	movs	r1, #224
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #20]
	cmp	r2, r3
	bne.n	.L_02000368
	ldr	r0, [pc, #16]
	b.n	.L_0200036a
.L_02000368:
	ldr	r0, [pc, #16]
.L_0200036a:
	pop	{r1}
	bx	r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000001d
	.4byte 0x02008978
	.2byte 0x8948
	.2byte 0x0200
	push	{lr}
	movs	r0, #8
	sub	sp, #8
	bl	sub_02000aac
	ldr	r3, [r0, #8]
	cmp	r3, #0
	bge.n	.L_02000394
	ldr	r2, [pc, #72]
	adds	r3, r3, r2
.L_02000394:
	asrs	r3, r3, #20
	cmp	r3, #24
	bne.n	.L_020003d6
	movs	r0, #8
	bl	sub_02000892
	movs	r0, #8
	bl	sub_02000ac8
	adds	r0, #35
	ldrb	r2, [r0, #0]
	movs	r3, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r2, #17
	movs	r3, #19
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r1, #74
	movs	r2, #9
	movs	r3, #3
	movs	r0, #19
	bl	sub_02000aa6
	movs	r0, #8
	bl	sub_02000aec
	movs	r1, #0
	bl	sub_02000aba
	ldr	r0, [pc, #12]
	bl	sub_02000ae8
.L_020003d6:
	add	sp, #8
	pop	{r0}
	bx	r0
	.4byte 0x000fffff
	.2byte 0x0864
	.2byte 0x0000
	push	{lr}
	ldr	r3, [pc, #28]
	movs	r1, #224
	lsls	r1, r1, #1
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	ldr	r3, [pc, #20]
	cmp	r2, r3
	bne.n	.L_020003fc
	ldr	r0, [pc, #16]
	b.n	.L_020003fe
.L_020003fc:
	ldr	r0, [pc, #16]
.L_020003fe:
	pop	{r1}
	bx	r1
	.2byte 0x0000
	.4byte 0x02000240
	.4byte 0x0000001d
	.4byte 0x020089f0
	.2byte 0x8990
	.2byte 0x0200
	push	{lr}
	ldr	r3, [pc, #152]
	movs	r2, #224
	ldr	r1, [r3, #0]
	movs	r3, #129
	lsls	r3, r3, #2
	lsls	r2, r2, #1
	str	r3, [r1, r2]
	ldr	r1, [pc, #140]
	ldrsh	r2, [r1, r2]
	ldr	r3, [pc, #140]
	sub	sp, #8
	cmp	r2, r3
	bne.n	.L_020004a6
	movs	r2, #225
	lsls	r2, r2, #1
	adds	r3, r1, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #5
	bne.n	.L_02000446
	ldr	r0, [pc, #124]
	bl	sub_02000b5e
	b.n	.L_020004a6
.L_02000446:
	movs	r0, #8
	bl	sub_02000b6e
	adds	r0, #89
	ldrb	r2, [r0, #0]
	movs	r3, #16
	orrs	r3, r2
	strb	r3, [r0, #0]
	ldr	r0, [pc, #104]
	bl	sub_02000b66
	cmp	r0, #0
	beq.n	.L_020004a6
	movs	r1, #173
	movs	r2, #146
	lsls	r2, r2, #17
	lsls	r1, r1, #17
	movs	r0, #8
	bl	sub_02000bb0
	movs	r0, #8
	bl	sub_02000b96
	movs	r1, #0
	bl	sub_02000b64
	movs	r0, #8
	bl	sub_02000ba2
	adds	r0, #35
	ldrb	r3, [r0, #0]
	movs	r2, #2
	orrs	r3, r2
	strb	r3, [r0, #0]
	movs	r1, #2
	movs	r0, #8
	bl	sub_02000bdc
	movs	r3, #19
	movs	r2, #17
	str	r3, [sp, #0]
	str	r2, [sp, #4]
	movs	r0, #19
	movs	r1, #74
	movs	r2, #9
	movs	r3, #3
	bl	sub_02000b88
.L_020004a6:
	movs	r0, #0
	add	sp, #8
	pop	{r1}
	bx	r1
	.2byte 0x0000
	.4byte 0x03001ebc
	.4byte 0x02000240
	.4byte 0x0000001c
	.4byte 0x0000012f
	.4byte 0x00000864
	.4byte 0x49026d02
	.4byte 0x185b8bd3
	.4byte 0x477083d3
	.2byte 0xf800
	.2byte 0xffff
	push	{r5, r6, r7, lr}
	adds	r7, r0, #0
	adds	r6, r1, #0
	movs	r5, #60
.L_020004dc:
	cmp	r5, #0
	beq.n	.L_020004ee
	movs	r0, #1
	bl	sub_02000b98
	ldr	r3, [r7, #12]
	subs	r5, #1
	cmp	r3, r6
	bgt.n	.L_020004dc
.L_020004ee:
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
	sub	sp, #68
	mov	fp, r0
	bl	sub_02000c2c
	adds	r6, r0, #0
	adds	r5, r6, #0
	movs	r3, #0
	adds	r5, #85
	strb	r3, [r5, #0]
	mov	r8, r3
.L_02000516:
	movs	r0, #1
	bl	sub_02000bce
	ldr	r2, [r6, #80]
	ldr	r1, [pc, #376]
	ldrh	r3, [r2, #30]
	adds	r3, r3, r1
	strh	r3, [r2, #30]
	ldr	r3, [r6, #80]
	ldrh	r0, [r3, #30]
	bl	sub_02000bf0
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	ldr	r3, [r6, #8]
	asrs	r0, r0, #1
	subs	r3, r3, r0
	str	r3, [r6, #8]
	movs	r2, #1
	movs	r3, #128
	lsls	r3, r3, #24
	add	r8, r2
	str	r3, [r6, #56]
	mov	r3, r8
	cmp	r3, #17
	bls.n	.L_02000516
	ldr	r3, [pc, #336]
	movs	r1, #192
	movs	r2, #192
	str	r3, [r6, #108]
	mov	r0, fp
	lsls	r1, r1, #10
	lsls	r2, r2, #9
	bl	sub_02000c86
	movs	r1, #188
	movs	r2, #144
	lsls	r1, r1, #1
	mov	r0, fp
	lsls	r2, r2, #1
	bl	sub_02000c9c
	ldr	r3, [pc, #308]
	str	r3, [r6, #72]
	movs	r3, #3
	strb	r3, [r5, #0]
	adds	r3, r6, #0
	adds	r3, #34
	movs	r2, #0
	strb	r2, [r3, #0]
	mov	r0, fp
	bl	sub_02000cba
	movs	r1, #0
	adds	r0, r6, #0
	bl	sub_02000a5a
	movs	r0, #188
	bl	sub_02000ce8
	movs	r0, #160
	lsls	r0, r0, #11
	movs	r2, #128
	adds	r1, r0, #0
	lsls	r2, r2, #9
	bl	sub_02000c96
	movs	r0, #141
	bl	sub_02000cfc
	movs	r0, #1
	movs	r1, #1
	negs	r0, r0
	negs	r1, r1
	ldr	r2, [pc, #248]
	bl	sub_02000caa
	movs	r4, #0
	add	r7, sp, #56
	mov	r8, r4
	mov	sl, r7
	mov	r9, r4
.L_020005ba:
	mov	r1, r8
	lsls	r5, r1, #12
	adds	r0, r5, #0
	bl	sub_02000c86
	mov	r2, sl
	mov	r3, r9
	str	r0, [r2, #0]
	str	r3, [r2, #4]
	adds	r0, r5, #0
	bl	sub_02000c8c
	mov	r4, sl
	ldr	r2, [r4, #0]
	str	r0, [r4, #8]
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_020005e0
	adds	r3, r2, #3
.L_020005e0:
	lsrs	r5, r0, #31
	adds	r5, r0, r5
	asrs	r3, r3, #2
	asrs	r5, r5, #1
	subs	r3, r2, r3
	subs	r5, r0, r5
	str	r3, [r7, #0]
	str	r5, [r7, #8]
	ldr	r4, [r7, #4]
	ldr	r1, [r6, #12]
	ldr	r2, [r6, #16]
	ldr	r0, [r6, #8]
	str	r4, [sp, #0]
	mov	r4, r9
	str	r5, [sp, #4]
	str	r4, [sp, #8]
	str	r4, [sp, #12]
	bl	sub_02000740
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	cmp	r2, #16
	bls.n	.L_020005ba
	movs	r3, #160
	lsls	r3, r3, #11
	movs	r1, #173
	movs	r2, #146
	str	r3, [r6, #40]
	lsls	r2, r2, #1
	mov	r0, fp
	lsls	r1, r1, #1
	bl	sub_02000d56
	mov	r0, fp
	bl	sub_02000d64
	adds	r0, r6, #0
	movs	r1, #0
	bl	sub_02000b04
	mov	r3, r9
	str	r3, [r6, #108]
	ldr	r2, [r6, #80]
	movs	r3, #128
	lsls	r3, r3, #5
	strh	r3, [r2, #30]
	add	r4, sp, #16
	movs	r3, #214
	strh	r3, [r4, #24]
	movs	r3, #128
	lsls	r3, r3, #8
	str	r3, [r4, #8]
	ldr	r3, [pc, #84]
	str	r3, [r4, #12]
	movs	r3, #192
	lsls	r3, r3, #9
	str	r3, [r4, #16]
	ldr	r3, [pc, #80]
	str	r3, [r4, #20]
	mov	r3, r9
	ldr	r2, [r6, #16]
	ldr	r1, [r6, #12]
	ldr	r0, [r6, #8]
	str	r3, [sp, #0]
	str	r3, [sp, #4]
	movs	r3, #224
	lsls	r3, r3, #13
	str	r3, [sp, #8]
	movs	r3, #0
	str	r4, [sp, #12]
	bl	sub_020007ac
	movs	r0, #154
	bl	sub_02000dd2
	mov	r0, fp
	movs	r1, #3
	bl	sub_02000dca
	bl	sub_02000d86
	add	sp, #68
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.4byte 0xffffff00
	.4byte 0x020084c5
	.4byte 0x0000cccc
	.4byte 0x0000e666
	.4byte 0x00013333
	.4byte 0x47204c00
	.4byte 0x03000380
	.4byte 0x47204c00
	.4byte 0x080000c1
	.4byte 0x47204c00
	.4byte 0x08000119
	.4byte 0x47204c00
	.4byte 0x08000121
	.4byte 0x47204c00
	.4byte 0x08009081
	.4byte 0x47204c00
	.4byte 0x08009099
	.4byte 0x47204c00
	.4byte 0x080090c9
	.4byte 0x47204c00
	.4byte 0x080091c1
	.4byte 0x47204c00
	.4byte 0x080091e1
	.4byte 0x47204c00
	.4byte 0x080091e9
	.4byte 0x47204c00
	.4byte 0x080091f1
	.4byte 0x47204c00
	.4byte 0x080091f9
	.4byte 0x47204c00
	.4byte 0x080770c1
	.4byte 0x47204c00
	.4byte 0x080770c9
	.4byte 0x47204c00
	.4byte 0x080770d1
	.4byte 0x47204c00
	.4byte 0x0808a081
	.4byte 0x47204c00
	.4byte 0x0808a091
	.4byte 0x47204c00
	.4byte 0x0808a0b9
	.4byte 0x47204c00
	.4byte 0x0808a0e9
	.4byte 0x47204c00
	.4byte 0x0808a0f1
	.4byte 0x47204c00
	.4byte 0x0808a101
	.4byte 0x47204c00
	.4byte 0x0808a161
	.4byte 0x47204c00
	.4byte 0x080f9011
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000016
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000002c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000007e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.4byte 0x02008764
	.4byte 0x0200879c
	.4byte 0x020087d4
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000178
	.4byte 0xc0000308
	.4byte 0x00580000
	.4byte 0x02980008
	.4byte 0x00000340
	.4byte 0xffff0002
	.4byte 0x000000a8
	.4byte 0x40000228
	.4byte 0x00580000
	.4byte 0x02980008
	.4byte 0x00000340
	.4byte 0xffff0003
	.4byte 0x00000258
	.4byte 0x40000208
	.4byte 0x00580000
	.4byte 0x02980008
	.4byte 0x00000340
	.4byte 0xffff0004
	.4byte 0x00000178
	.4byte 0x400000a8
	.4byte 0x00580000
	.4byte 0x02980008
	.4byte 0x00000340
	.4byte 0xffff0005
	.4byte 0x00000266
	.4byte 0xc0000350
	.4byte 0x01e00000
	.4byte 0x02d0025d
	.4byte 0x00000384
	.4byte 0xffff0063
	.4byte 0x000001a8
	.4byte 0x80000118
	.4byte 0x00580000
	.4byte 0x02980008
	.4byte 0x00000340
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff0001
	.4byte 0x0000009a
	.4byte 0xc000015e
	.4byte 0x00100000
	.4byte 0x02780030
	.4byte 0x00000168
	.4byte 0xffff0002
	.4byte 0x00000228
	.4byte 0xc0000138
	.4byte 0x00100000
	.4byte 0x02780030
	.4byte 0x00000168
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00102019
	.4byte 0x0020101b
	.4byte 0x0030201b
	.4byte 0x0040101d
	.4byte 0x0050b005
	.4byte 0x0000001d
	.4byte 0x0010401c
	.4byte 0x00228002
	.4byte 0x000001ff
	.4byte 0xffff00d4
	.4byte 0x00000001
	.4byte 0x01900000
	.4byte 0x00000000
	.4byte 0x01200000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00000031
	.4byte 0xffff0006
	.4byte 0x00000002
	.4byte 0x00000031
	.4byte 0xffff0007
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000004
	.4byte 0x00000013
	.4byte 0x0f280064
	.4byte 0x001000bf
	.4byte 0x00008c15
	.4byte 0x08640008
	.4byte 0x02008381
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
