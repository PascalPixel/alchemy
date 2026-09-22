.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_08013560, 0x08013560
	.global Overlay_08013f80
Overlay_08013f80:
	push	{r5, lr}
	adds	r5, r3, #0
	ldr	r3, [pc, #28]
	adds	r4, r2, #0
	strb	r0, [r3, #0]
	ldr	r3, [pc, #20]
	ldr	r2, [pc, #24]
	ands	r1, r3
	strh	r1, [r2, #0]
	cmp	r4, #16
	bls.n	.L_08013fb4
	ldr	r2, [pc, #20]
	ldr	r3, [pc, #20]
	ldrb	r2, [r2, #0]
	strb	r2, [r3, #0]
	b.n	.L_08013fb8
	.4byte 0x0000003f
	.4byte 0x030011dc
	.4byte 0x030011f4
	.4byte 0x030011b0
	.2byte 0x113c
	.2byte 0x0300
.L_08013fb4:
	ldr	r3, [pc, #20]
	strb	r4, [r3, #0]
.L_08013fb8:
	ldr	r3, [pc, #20]
	ldr	r2, [pc, #24]
	strb	r5, [r3, #0]
	ldr	r3, [sp, #8]
	ldr	r1, [pc, #20]
	strb	r3, [r2, #0]
	ldrb	r3, [r2, #0]
	strb	r3, [r1, #0]
	pop	{r5, pc}
	movs	r0, r0
	.4byte 0x0300113c
	.4byte 0x030011b0
	.4byte 0x03001178
	.2byte 0x110c
	.2byte 0x0300
	push	{r5, lr}
	ldr	r2, [pc, #24]
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_08013ff4
	adds	r5, r2, #0
.L_08013fe8:
	movs	r0, #1
	bl	sub_08013560
	ldrb	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L_08013fe8
.L_08013ff4:
	pop	{r5, pc}
	movs	r0, r0
	.2byte 0x1178
	.2byte 0x0300
	push	{lr}
	ldr	r2, [pc, #16]
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r1, #32
	ldr	r3, [pc, #12]
	ldr	r0, [pc, #12]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xbd00
	.4byte 0x030011fc
	.4byte 0x03000258
	.2byte 0x1190
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r2, #0
	ldrsh	r1, [r0, r2]
	ldrh	r6, [r0, #4]
	mov	r8, r1
	movs	r1, #2
	ldrsh	r3, [r0, r1]
	ldr	r1, [pc, #152]
	mov	sl, r3
	ldrb	r2, [r1, #0]
	movs	r0, #31
	mov	r9, r2
	cmp	r2, #31
	bhi.n	.L_080140c0
	lsls	r3, r2, #3
	ldr	r2, [pc, #140]
	adds	r7, r3, r2
	mov	r3, r9
	adds	r3, #1
	strb	r3, [r1, #0]
	cmp	r8, sl
	beq.n	.L_08014058
	mov	r3, r8
	mov	r1, sl
	cmn	r3, r1
	bne.n	.L_08014080
.L_08014058:
	cmp	r6, #0
	bne.n	.L_08014080
	movs	r0, #128
	ldr	r3, [pc, #116]
	mov	r1, sl
	lsls	r0, r0, #9
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4642
	mov	r1, sl
	adds	r3, r0, #0
	cmn	r2, r1
	bne.n	.L_08014074
	negs	r3, r0
.L_08014074:
	lsls	r3, r3, #16
	lsrs	r3, r3, #16
	str	r3, [r7, #0]
	lsls	r3, r0, #16
	str	r3, [r7, #4]
	b.n	.L_080140be
.L_08014080:
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl	sub_08002090
	mov	r1, r8
	adds	r6, r0, #0
	bl	sub_08002054
	mov	r1, r8
	strh	r0, [r7, #0]
	adds	r0, r5, #0
	bl	sub_08002054
	adds	r7, #2
	negs	r5, r5
	strh	r0, [r7, #0]
	mov	r1, sl
	adds	r0, r5, #0
	bl	sub_08002054
	adds	r7, #2
	strh	r0, [r7, #0]
	mov	r1, sl
	adds	r0, r6, #0
	bl	sub_08002054
	adds	r7, #2
	strh	r0, [r7, #0]
.L_080140be:
	mov	r0, r9
.L_080140c0:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x030011fc
	.4byte 0x020054d0
	.2byte 0x0528
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	cmp	r1, #239
	bls.n	.L_080140ea
	cmp	r1, #0
	bge.n	.L_080140e8
	movs	r1, #0
	b.n	.L_080140ea
.L_080140e8:
	movs	r1, #239
.L_080140ea:
	ldr	r3, [pc, #52]
	lsrs	r2, r1, #3
	adds	r6, r2, r3
	ldrb	r2, [r6, #0]
	movs	r0, #7
	ands	r0, r1
	adds	r4, r2, #0
	lsrs	r4, r0
	movs	r3, #1
	ands	r4, r3
	ldr	r7, [pc, #36]
	cmp	r4, #0
	beq.n	.L_08014110
	lsls	r3, r1, #2
	adds	r3, r3, r7
	ldr	r2, [r3, #0]
	str	r5, [r3, #0]
	str	r2, [r5, #0]
	b.n	.L_0801411c
.L_08014110:
	lsls	r3, r0
	orrs	r2, r3
	lsls	r3, r1, #2
	strb	r2, [r6, #0]
	str	r5, [r3, r7]
	str	r4, [r5, #0]
.L_0801411c:
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x03001190
	.2byte 0x3010
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r5, r0, #0
	cmp	r1, #255
	ble.n	.L_08014132
	movs	r1, #255
.L_08014132:
	cmp	r1, #0
	bge.n	.L_08014138
	movs	r1, #0
.L_08014138:
	ldr	r3, [pc, #48]
	lsrs	r2, r1, #3
	adds	r6, r2, r3
	ldrb	r4, [r6, #0]
	ldr	r3, [pc, #44]
	movs	r0, #7
	lsls	r2, r1, #2
	ands	r0, r1
	adds	r1, r4, #0
	adds	r2, r2, r3
	lsrs	r1, r0
	movs	r3, #1
	ands	r1, r3
	cmp	r1, #0
	beq.n	.L_0801415e
	ldr	r3, [r2, #0]
	str	r5, [r2, #0]
	str	r3, [r5, #0]
	b.n	.L_08014168
.L_0801415e:
	lsls	r3, r0
	orrs	r4, r3
	strb	r4, [r6, #0]
	str	r5, [r2, #0]
	str	r1, [r5, #0]
.L_08014168:
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x03001190
	.2byte 0x3010
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	lsrs	r1, r1, #6
	cmp	r5, #95
	bls.n	.L_08014184
	movs	r0, #1
	negs	r0, r0
	b.n	.L_080141e8
.L_08014184:
	ldr	r7, [pc, #100]
	ldr	r2, [pc, #104]
	movs	r4, #0
	mov	lr, r2
	adds	r6, r7, #0
.L_0801418e:
	movs	r3, #128
	movs	r0, #1
	lsls	r3, r3, #2
	negs	r0, r0
	cmp	r4, r3
	bge.n	.L_080141e8
	ldrb	r3, [r6, r4]
	cmp	r3, #255
	bne.n	.L_080141d8
	movs	r2, #128
	movs	r0, #1
	adds	r3, r4, r1
	lsls	r2, r2, #2
	negs	r0, r0
	cmp	r3, r2
	bhi.n	.L_080141e8
	adds	r0, r4, #0
	cmp	r0, r3
	bcs.n	.L_080141c6
	mov	ip, r3
	adds	r2, r0, r6
.L_080141b8:
	ldrb	r3, [r2, #0]
	adds	r2, #1
	cmp	r3, #255
	bne.n	.L_080141d8
	adds	r4, #1
	cmp	r4, ip
	bcc.n	.L_080141b8
.L_080141c6:
	movs	r2, #0
	cmp	r2, r1
	bcs.n	.L_080141e6
.L_080141cc:
	adds	r3, r0, r2
	adds	r2, #1
	strb	r5, [r7, r3]
	cmp	r2, r1
	bcc.n	.L_080141cc
	b.n	.L_080141e6
.L_080141d8:
	ldrb	r3, [r7, r4]
	mov	r2, lr
	lsls	r3, r3, #2
	ldrh	r3, [r2, r3]
	lsrs	r3, r3, #6
	adds	r4, r4, r3
	b.n	.L_0801418e
.L_080141e6:
	lsls	r0, r0, #6
.L_080141e8:
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x02003410
	.4byte 0x020036e0
