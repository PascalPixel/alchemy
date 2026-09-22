.syntax unified
	.thumb
	.set sub_08002dd8, 0x08002dd8
	.set sub_08003dec, 0x08003dec
	.set sub_08003fa4, 0x08003fa4
	.set sub_08004080, 0x08004080
	.set sub_080041d8, 0x080041d8
	.set sub_08004458, 0x08004458
	.set sub_080048f4, 0x080048f4
	.set sub_080053e8, 0x080053e8
	.set sub_080091a8, 0x080091a8
	.set sub_080770c0, 0x080770c0
	.set sub_08091200, 0x08091200
	.set sub_08091ff0, 0x08091ff0
	.set sub_080f9010, 0x080f9010
	.global Overlay_08094820
Overlay_08094820:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #236]
	ldr	r0, [r3, #0]
	ldr	r3, [r3, #84]
	mov	sl, r0
	mov	r8, r3
	mov	r3, sl
	adds	r3, #228
	ldr	r1, [r3, #0]
	sub	sp, #12
	str	r1, [sp, #8]
	ldr	r3, [r3, #4]
	mov	r7, r8
	str	r3, [sp, #4]
	movs	r2, #0
	movs	r3, #63
	adds	r7, #8
	mov	r9, r2
	mov	fp, r3
.L_08094852:
	ldrh	r3, [r7, #28]
	ldr	r1, [pc, #200]
	adds	r3, r3, r1
	adds	r2, r1, #0
	ands	r2, r3
	strh	r3, [r7, #28]
	cmp	r2, r1
	bne.n	.L_08094864
	b.n	.L_08094984
.L_08094864:
	movs	r0, #179
	lsls	r0, r0, #1
	bl	sub_080770c0
	cmp	r0, #0
	beq.n	.L_08094876
	ldrh	r3, [r7, #28]
	adds	r3, #1
	strh	r3, [r7, #28]
.L_08094876:
	ldrh	r2, [r7, #28]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	ldr	r2, [pc, #164]
	lsls	r3, r3, #1
	adds	r4, r3, r2
	ldr	r5, [sp, #8]
	ldr	r3, [r7, #12]
	subs	r2, r3, r5
	cmp	r2, #0
	bge.n	.L_08094890
	ldr	r0, [pc, #144]
	adds	r2, r2, r0
.L_08094890:
	movs	r1, #0
	ldrsh	r3, [r4, r1]
	asrs	r2, r2, #16
	adds	r1, r2, r3
	ldr	r2, [r7, #16]
	ldr	r3, [r7, #20]
	subs	r3, r3, r2
	ldr	r2, [sp, #4]
	subs	r3, r3, r2
	adds	r4, #2
	cmp	r3, #0
	bge.n	.L_080948ac
	ldr	r5, [pc, #116]
	adds	r3, r3, r5
.L_080948ac:
	movs	r0, #0
	ldrsh	r2, [r4, r0]
	asrs	r3, r3, #16
	adds	r0, r3, r2
	adds	r3, r1, #0
	adds	r3, #16
	adds	r4, #2
	cmp	r3, #255
	bhi.n	.L_08094946
	movs	r2, #32
	negs	r2, r2
	cmp	r0, r2
	blt.n	.L_08094946
	cmp	r0, #159
	bgt.n	.L_08094946
	movs	r5, #13
	ldrb	r2, [r7, #9]
	negs	r5, r5
	adds	r3, r5, #0
	ands	r2, r3
	movs	r3, #4
	orrs	r2, r3
	ldr	r3, [pc, #48]
	strb	r2, [r7, #9]
	ands	r1, r3
	ldr	r2, [pc, #48]
	ldrh	r3, [r7, #6]
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r7, #6]
	strb	r0, [r7, #4]
	mov	r0, r8
	ldrh	r3, [r4, #0]
	ldr	r1, [r0, #4]
	adds	r1, r1, r3
	ldr	r3, [pc, #32]
	ldr	r2, [pc, #32]
	ands	r1, r3
	ldrh	r3, [r7, #8]
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r7, #8]
	adds	r4, #2
	ldrb	r2, [r4, #0]
	ldrb	r1, [r7, #5]
	mov	r3, fp
	b.n	.L_08094928
	movs	r0, r0
	.4byte 0x000001ff
	.4byte 0xfffffe00
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.4byte 0x03001e70
	.4byte 0x0000ffff
	.2byte 0xef84
	.2byte 0x0809
.L_08094928:
	lsls	r2, r2, #6
	ands	r3, r1
	orrs	r3, r2
	strb	r3, [r7, #5]
	ldrb	r1, [r7, #7]
	ldrb	r2, [r4, #2]
	mov	r3, fp
	ands	r3, r1
	lsls	r2, r2, #6
	orrs	r3, r2
	strb	r3, [r7, #7]
	adds	r0, r7, #0
	movs	r1, #240
	bl	sub_08003dec
.L_08094946:
	ldrh	r3, [r7, #28]
	cmp	r3, #0
	bne.n	.L_08094984
	mov	r1, sl
	ldr	r6, [r1, #0]
	bl	sub_08004458
	ldr	r3, [r6, #0]
	lsls	r0, r0, #8
	ldr	r5, [pc, #72]
	adds	r3, r3, r0
	adds	r1, r3, r5
	str	r1, [sp, #0]
	bl	sub_08004458
	ldr	r3, [r6, #8]
	lsls	r0, r0, #8
	adds	r3, r3, r0
	ldr	r1, [sp, #0]
	adds	r0, r3, r5
	str	r1, [r7, #12]
	str	r0, [r7, #20]
	asrs	r2, r0, #16
	asrs	r1, r1, #16
	movs	r0, #0
	bl	sub_080091a8
	movs	r3, #16
	lsls	r0, r0, #16
	str	r0, [r7, #16]
	strh	r3, [r7, #28]
.L_08094984:
	movs	r2, #1
	add	r9, r2
	mov	r3, r9
	adds	r7, #32
	cmp	r3, #31
	bhi.n	.L_08094992
	b.n	.L_08094852
.L_08094992:
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.2byte 0x0000
	.2byte 0xff80
	.2byte 0xb5e0
	ldr	r3, [pc, #252]
	movs	r1, #252
	ldr	r6, [r3, #0]
	lsls	r1, r1, #5
	adds	r5, r6, r1
	ldr	r7, [r3, #8]
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #0
	blt.n	.L_08094aa2
	movs	r0, #179
	lsls	r0, r0, #1
	bl	sub_080770c0
	cmp	r0, #0
	beq.n	.L_080949ce
	movs	r3, #128
	strh	r3, [r5, #0]
.L_080949ce:
	ldrh	r3, [r5, #0]
	subs	r2, r3, #1
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	strh	r2, [r5, #0]
	cmp	r3, #11
	bhi.n	.L_08094aa2
	ldr	r2, [pc, #204]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x08094a14
	.4byte 0x08094a86
	.4byte 0x08094aa2
	.4byte 0x08094aa2
	.4byte 0x08094aa2
	.4byte 0x08094a62
	.4byte 0x08094a86
	.4byte 0x08094aa2
	.4byte 0x08094aa2
	.4byte 0x08094aa2
	.4byte 0x08094a62
	.2byte 0x4a86
	.2byte 0x0809
	ldr	r1, [pc, #152]
	adds	r3, r6, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L_08094a62
	bl	sub_08004458
	adds	r5, r0, #0
	bl	sub_08004458
	movs	r2, #100
	muls	r2, r0
	lsls	r3, r5, #1
	adds	r3, r3, r5
	lsls	r3, r3, #3
	adds	r3, r3, r5
	lsls	r3, r3, #4
	lsrs	r2, r2, #16
	lsrs	r3, r3, #16
	movs	r1, #252
	subs	r3, r3, r2
	lsls	r1, r1, #5
	adds	r2, r6, r1
	adds	r3, #150
	strh	r3, [r2, #0]
	ldr	r2, [pc, #104]
	adds	r3, r6, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	beq.n	.L_08094a5c
	movs	r0, #172
	bl	sub_080f9010
	b.n	.L_08094a62
.L_08094a5c:
	movs	r0, #171
	bl	sub_080f9010
.L_08094a62:
	adds	r0, r6, #0
	movs	r1, #1
	bl	sub_08091200
	movs	r2, #168
	lsls	r2, r2, #5
	adds	r0, r6, r2
	movs	r2, #196
	lsls	r2, r2, #5
	adds	r1, r7, r2
	ldr	r3, [pc, #64]
	ldr	r2, [pc, #64]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #64]
	adds	r2, r7, r3
	movs	r3, #12
	b.n	.L_08094a98
	movs	r3, #168
	lsls	r3, r3, #4
	adds	r0, r6, r3
	movs	r1, #1
	bl	sub_08091200
	ldr	r1, [pc, #44]
	movs	r3, #1
	adds	r2, r7, r1
.L_08094a98:
	strb	r3, [r2, #0]
	ldr	r2, [pc, #40]
	movs	r1, #0
	adds	r3, r7, r2
	strb	r1, [r3, #0]
.L_08094aa2:
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001ec8
	.4byte 0x080949e4
	.4byte 0x00001f82
	.4byte 0x00001f84
	.4byte 0x040000d4
	.4byte 0x840002a0
	.4byte 0x00002a01
	.2byte 0x2a02
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r1, #130
	lsls	r1, r1, #3
	movs	r0, #29
	sub	sp, #8
	bl	sub_080048f4
	ldr	r3, [pc, #180]
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #0]
	adds	r5, r0, #0
	movs	r0, #170
	mov	r8, r3
	bl	sub_08091ff0
	movs	r6, #0
	adds	r7, r5, #0
	add	r0, sp, #4
	adds	r7, #8
	str	r6, [r0, #0]
	ldr	r3, [pc, #156]
	adds	r1, r5, #0
	ldr	r2, [pc, #156]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r1, #128
	lsls	r1, r1, #3
	movs	r0, #14
	bl	sub_080048f4
	adds	r6, r0, #0
	adds	r1, r6, #0
	ldr	r0, [pc, #140]
	bl	sub_080053e8
	bl	sub_08004080
	movs	r1, #192
	str	r0, [r5, #0]
	lsls	r1, r1, #2
	adds	r2, r6, #0
	bl	sub_08003fa4
	str	r0, [r5, #4]
	movs	r0, #14
	bl	sub_08002dd8
	movs	r5, #0
.L_08094b2c:
	movs	r4, #0
	adds	r6, r7, #0
	stmia	r6!, {r4}
	ldr	r3, [pc, #108]
	stmia	r6!, {r3}
	movs	r3, #212
	lsls	r3, r3, #8
	str	r3, [r6, #0]
	mov	r3, r8
	ldr	r1, [r3, #0]
	ldr	r2, [r3, #8]
	movs	r0, #0
	str	r1, [r7, #12]
	str	r2, [r7, #20]
	asrs	r1, r1, #16
	asrs	r2, r2, #16
	str	r4, [sp, #0]
	bl	sub_080091a8
	ldr	r2, [pc, #56]
	adds	r3, r5, #0
	ands	r3, r2
	lsls	r0, r0, #16
	adds	r3, #1
	adds	r5, #1
	str	r0, [r7, #16]
	strh	r3, [r7, #28]
	ldr	r4, [sp, #0]
	adds	r7, #32
	cmp	r5, #31
	bls.n	.L_08094b2c
	ldr	r3, [pc, #56]
	movs	r2, #252
	lsls	r2, r2, #6
	strh	r2, [r3, #0]
	ldr	r2, [pc, #52]
	adds	r3, #2
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r4, [r3, #0]
	ldr	r0, [pc, #44]
	movs	r1, #200
	lsls	r1, r1, #4
	bl	sub_080041d8
	add	sp, #8
	b.n	.L_08094bb0
	movs	r0, r0
	.4byte 0x0000000f
	.4byte 0x03001e70
	.4byte 0x040000d4
	.4byte 0x85000104
	.4byte 0x0809ff58
	.4byte 0x40000400
	.4byte 0x04000050
	.4byte 0x00001008
	.2byte 0x4821
	.2byte 0x0809
.L_08094bb0:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
