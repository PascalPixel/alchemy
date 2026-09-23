.syntax unified
	.thumb
	.set sub_08002064, 0x08002064
	.set sub_0801314c, 0x0801314c
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014d00, 0x08014d00
	.set sub_08016ca4, 0x08016ca4
	.set sub_08039260, 0x08039260
	.set sub_0803939c, 0x0803939c
	.set sub_08041c54, 0x08041c54
	.set sub_08042010, 0x08042010
	.set sub_08042188, 0x08042188
	.set sub_08042314, 0x08042314
	.set sub_08044460, 0x08044460
	.set sub_080ad2e8, 0x080ad2e8
	.set sub_08108030, 0x08108030
	.set sub_08108040, 0x08108040
	.set sub_08108048, 0x08108048
	.set sub_081c0010, 0x081c0010
	.global Func_08041204
	.thumb_func
Func_08041204:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #208
	ldr	r0, [r3, #0]
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #164
	adds	r0, r0, r3
	bl	sub_08108030
	pop	{pc}
.L_0804121c:
	push	{lr}
	movs	r1, #197
	lsls	r1, r1, #3
	movs	r0, #208
	sub	sp, #4
	bl	sub_08014d00
	movs	r3, #0
	adds	r1, r0, #0
	mov	r0, sp
	str	r3, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r2, [pc, #16]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #12]
	bl	sub_080145a8
	add	sp, #4
	pop	{pc}
	.4byte 0x8500018a
	.2byte 0x1205
	.2byte 0x0804
.L_08041254:
	push	{lr}
	ldr	r0, [pc, #12]
	bl	sub_08014644
	movs	r0, #208
	bl	sub_0801314c
	pop	{pc}
	.2byte 0x1205
	.2byte 0x0804
.L_08041268:
	push	{lr}
	adds	r4, r0, #0
	cmp	r4, #0
	beq.n	.L_0804127a
	ldr	r0, [pc, #16]
	adds	r0, r4, r0
	bl	sub_08042010
	b.n	.L_08041280
.L_0804127a:
	ldr	r0, [pc, #12]
	bl	sub_08042188
.L_08041280:
	pop	{pc}
	movs	r0, r0
	.4byte 0x000005a7
	.2byte 0xeb50
	.2byte 0x0805
.L_0804128c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #208
	ldr	r3, [r3, #0]
	sub	sp, #4
	mov	sl, r3
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r0, #0
	movs	r3, #19
	movs	r1, #0
	movs	r2, #30
	bl	sub_08039260
	movs	r3, #7
	adds	r6, r0, #0
	movs	r5, #2
	mov	r8, r3
.L_080412b8:
	adds	r2, r5, #0
	movs	r3, #29
	adds	r0, r6, #0
	movs	r1, #0
	str	r5, [sp, #0]
	bl	sub_08041c54
	movs	r3, #1
	negs	r3, r3
	add	r8, r3
	mov	r3, r8
	adds	r5, #2
	cmp	r3, #0
	bge.n	.L_080412b8
	ldr	r5, [pc, #116]
	movs	r3, #0
	movs	r7, #0
	mov	r8, r3
.L_080412dc:
	movs	r3, #0
	ldrsh	r0, [r5, r3]
	adds	r1, r6, #0
	adds	r3, r7, #0
	movs	r2, #8
	bl	.L_08041268
	movs	r3, #18
	ldrsh	r0, [r5, r3]
	adds	r1, r6, #0
	adds	r3, r7, #0
	movs	r2, #64
	bl	.L_08041268
	movs	r3, #36
	ldrsh	r0, [r5, r3]
	adds	r1, r6, #0
	adds	r3, r7, #0
	movs	r2, #120
	bl	.L_08041268
	movs	r3, #54
	ldrsh	r0, [r5, r3]
	adds	r1, r6, #0
	adds	r3, r7, #0
	movs	r2, #176
	bl	.L_08041268
	movs	r3, #1
	add	r8, r3
	mov	r3, r8
	adds	r7, #16
	adds	r5, #2
	cmp	r3, #8
	ble.n	.L_080412dc
	bl	sub_08044460
	movs	r1, #128
	movs	r3, #0
	lsls	r1, r1, #23
	adds	r2, r6, #0
	str	r3, [sp, #0]
	bl	sub_08042314
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #164
	add	r3, sl
	str	r0, [r3, #0]
	add	sp, #4
	adds	r0, r6, #0
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0xa170
	.2byte 0x080a
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r0, #0
	mov	r8, r0
	bl	.L_0804121c
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #208
	ldr	r3, [r3, #0]
	movs	r0, #1
	mov	r9, r3
	bl	sub_08013560
	bl	.L_0804128c
	movs	r6, #1
	negs	r6, r6
	movs	r7, #0
	mov	sl, r0
.L_0804137e:
	cmp	r6, #0
	beq.n	.L_080413e0
	adds	r0, r7, #4
	movs	r1, #4
	bl	sub_08002064
	adds	r7, r0, #0
	mov	r0, r8
	movs	r1, #9
	adds	r0, #9
	bl	sub_08002064
	mov	r3, sl
	movs	r1, #12
	ldrsh	r2, [r3, r1]
	lsls	r3, r7, #3
	subs	r3, r3, r7
	adds	r3, r3, r2
	lsls	r3, r3, #3
	mov	r2, sl
	mov	r8, r0
	subs	r1, r3, #4
	movs	r0, #14
	ldrsh	r3, [r2, r0]
	mov	r0, r8
	lsls	r2, r0, #4
	lsls	r3, r3, #3
	adds	r3, r3, r2
	adds	r2, r3, #0
	movs	r3, #1
	negs	r3, r3
	adds	r2, #12
	cmp	r6, r3
	bne.n	.L_080413d0
	movs	r0, #160
	lsls	r0, r0, #3
	adds	r0, #164
	add	r0, r9
	bl	sub_08108048
	b.n	.L_080413de
.L_080413d0:
	movs	r0, #160
	lsls	r0, r0, #3
	adds	r0, #164
	add	r0, r9
	movs	r3, #3
	bl	sub_08108040
.L_080413de:
	movs	r6, #0
.L_080413e0:
	movs	r0, #1
	bl	sub_08013560
	ldr	r5, [pc, #428]
	movs	r2, #2
	ldr	r3, [r5, #4]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080413f4
	b.n	.L_08041500
.L_080413f4:
	ldr	r3, [r5, #4]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080414f6
	ldr	r3, [r5, #4]
	movs	r2, #128
	lsls	r2, r2, #2
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080414ec
	ldr	r3, [r5, #4]
	movs	r2, #128
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080414e2
	ldr	r3, [r5, #12]
	movs	r2, #32
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0804142a
	movs	r0, #111
	subs	r7, #1
	movs	r6, #1
	bl	sub_081c0010
.L_0804142a:
	ldr	r3, [r5, #12]
	movs	r2, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0804143e
	movs	r0, #111
	adds	r7, #1
	movs	r6, #1
	bl	sub_081c0010
.L_0804143e:
	ldr	r3, [r5, #12]
	movs	r2, #64
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08041456
	movs	r0, #1
	negs	r0, r0
	add	r8, r0
	movs	r0, #111
	movs	r6, #1
	bl	sub_081c0010
.L_08041456:
	ldr	r3, [r5, #12]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0804137e
	movs	r1, #1
	movs	r0, #111
	add	r8, r1
	movs	r6, #1
	bl	sub_081c0010
	b.n	.L_0804137e
.L_0804146e:
	mov	r0, sl
	movs	r1, #2
	bl	sub_0803939c
	bl	.L_08041254
	movs	r0, #1
	bl	sub_08013560
	movs	r2, #1
	negs	r2, r2
	mov	r9, r2
	cmp	r6, r9
	beq.n	.L_08041586
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	ldr	r2, [pc, #260]
	mov	sl, r3
	lsls	r3, r7, #3
	adds	r3, r3, r7
	add	r3, r8
	lsls	r3, r3, #1
	ldrsh	r5, [r2, r3]
	adds	r0, r5, #0
	bl	sub_080ad2e8
	cmp	r0, r9
	bne.n	.L_080414b2
	ldr	r3, [pc, #240]
	movs	r1, #133
	lsls	r1, r1, #2
	adds	r3, r3, r1
	ldr	r0, [r3, #0]
.L_080414b2:
	cmp	r5, #0
	bne.n	.L_0804150c
	cmp	r6, #0
	bne.n	.L_080414c2
	movs	r3, #179
	lsls	r3, r3, #1
	add	r3, sl
	strh	r6, [r3, #0]
.L_080414c2:
	cmp	r6, #1
	bne.n	.L_080414d0
	ldr	r3, [pc, #212]
	movs	r2, #144
	lsls	r2, r2, #2
	adds	r3, r3, r2
	strh	r5, [r3, #0]
.L_080414d0:
	cmp	r6, #2
	bne.n	.L_08041542
	ldr	r3, [pc, #196]
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #66
	adds	r3, r3, r0
	strh	r5, [r3, #0]
	b.n	.L_08041542
.L_080414e2:
	movs	r0, #112
	bl	sub_081c0010
	movs	r6, #2
	b.n	.L_0804146e
.L_080414ec:
	movs	r0, #112
	bl	sub_081c0010
	movs	r6, #1
	b.n	.L_0804146e
.L_080414f6:
	movs	r0, #112
	bl	sub_081c0010
	movs	r6, #0
	b.n	.L_0804146e
.L_08041500:
	movs	r0, #113
	movs	r6, #1
	bl	sub_081c0010
	negs	r6, r6
	b.n	.L_0804146e
.L_0804150c:
	movs	r3, #252
	lsls	r3, r3, #6
	adds	r3, #255
	lsls	r0, r0, #10
	ands	r5, r3
	orrs	r0, r5
	cmp	r6, #0
	bne.n	.L_08041524
	movs	r3, #179
	lsls	r3, r3, #1
	add	r3, sl
	strh	r0, [r3, #0]
.L_08041524:
	cmp	r6, #1
	bne.n	.L_08041532
	ldr	r3, [pc, #112]
	movs	r1, #144
	lsls	r1, r1, #2
	adds	r3, r3, r1
	strh	r0, [r3, #0]
.L_08041532:
	cmp	r6, #2
	bne.n	.L_08041542
	ldr	r3, [pc, #100]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #66
	adds	r3, r3, r2
	strh	r0, [r3, #0]
.L_08041542:
	movs	r0, #4
	bl	sub_08016ca4
	movs	r5, #250
	lsls	r5, r5, #1
	strh	r5, [r0, #58]
	movs	r0, #5
	bl	sub_08016ca4
	strh	r5, [r0, #58]
	movs	r0, #6
	bl	sub_08016ca4
	strh	r5, [r0, #58]
	movs	r0, #7
	bl	sub_08016ca4
	strh	r5, [r0, #58]
	movs	r0, #4
	bl	sub_08016ca4
	strh	r5, [r0, #54]
	movs	r0, #5
	bl	sub_08016ca4
	strh	r5, [r0, #54]
	movs	r0, #6
	bl	sub_08016ca4
	strh	r5, [r0, #54]
	movs	r0, #7
	bl	sub_08016ca4
	strh	r5, [r0, #54]
.L_08041586:
	adds	r0, r6, #0
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x03001150
	.4byte 0x080aa170
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #132
	str	r1, [sp, #0]
	adds	r7, r3, #0
	movs	r3, #192
	mov	fp, r0
	lsls	r3, r3, #18
	ldr	r0, [pc, #228]
	mov	r9, r2
	ldr	r6, [r3, #60]
	bl	sub_08013300
	mov	sl, r0
	mov	r0, fp
	ldrb	r5, [r0, #0]
	movs	r1, #4
	lsls	r2, r5, #5
	add	r1, sp
	mov	r8, r2
	movs	r2, #192
	mov	ip, r1
	lsls	r2, r2, #19
	mov	lr, ip
	add	r2, r8
	movs	r1, #0
.L_080415de:
	ldrb	r4, [r2, #0]
	movs	r0, #15
	adds	r3, r4, #0
	ands	r3, r0
	mov	r0, ip
	strb	r3, [r0, #0]
	lsrs	r3, r4, #4
	strb	r3, [r0, #1]
	adds	r1, #1
	movs	r3, #2
	adds	r2, #1
	add	ip, r3
	cmp	r1, #31
	bls.n	.L_080415de
	mov	r4, r9
	lsls	r3, r4, #5
	mov	r0, sl
	adds	r2, r0, r3
	movs	r3, #15
	mov	ip, lr
	movs	r1, #0
	mov	sl, r3
.L_0804160a:
	ldrb	r0, [r2, #0]
	mov	r4, sl
	adds	r3, r0, #0
	ands	r3, r4
	ldrb	r3, [r7, r3]
	adds	r2, #1
	cmp	r3, #0
	beq.n	.L_0804161e
	mov	r4, ip
	strb	r3, [r4, #0]
.L_0804161e:
	movs	r3, #1
	add	ip, r3
	lsrs	r3, r0, #4
	ldrb	r3, [r7, r3]
	cmp	r3, #0
	beq.n	.L_0804162e
	mov	r4, ip
	strb	r3, [r4, #0]
.L_0804162e:
	movs	r0, #1
	adds	r1, #1
	add	ip, r0
	cmp	r1, #31
	bls.n	.L_0804160a
	mov	ip, lr
	movs	r1, #0
	mov	r0, ip
.L_0804163e:
	ldrb	r3, [r0, #1]
	ldrb	r2, [r0, #0]
	lsls	r3, r3, #4
	orrs	r2, r3
	movs	r4, #1
	mov	r3, ip
	adds	r1, #1
	adds	r0, #2
	strb	r2, [r3, #0]
	add	ip, r4
	cmp	r1, #31
	bls.n	.L_0804163e
	lsls	r3, r5, #24
	cmp	r3, #0
	blt.n	.L_080416a4
	movs	r1, #224
	lsls	r1, r1, #4
	movs	r4, #0
	movs	r0, #127
	adds	r1, #56
.L_08041666:
	ldrh	r3, [r6, #0]
	adds	r2, r3, #1
	ands	r2, r0
	lsls	r3, r3, #24
	strh	r2, [r6, #0]
	lsrs	r5, r3, #24
	adds	r2, r5, r1
	ldrb	r3, [r6, r2]
	cmp	r3, #0
	beq.n	.L_08041680
	adds	r4, #1
	cmp	r4, #127
	bls.n	.L_08041666
.L_08041680:
	movs	r3, #1
	strb	r3, [r6, r2]
	movs	r3, #128
	ldr	r2, [pc, #20]
	orrs	r5, r3
	adds	r3, r5, #0
	orrs	r3, r2
	mov	r0, fp
	strh	r3, [r0, #0]
	ldr	r1, [sp, #0]
	lsls	r5, r5, #5
	strh	r3, [r1, #0]
	mov	r8, r5
	b.n	.L_080416a4
	.4byte 0x0000f000
	.2byte 0x0013
	.2byte 0x0000
.L_080416a4:
	movs	r3, #128
	movs	r1, #192
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r1, r1, #19
	lsls	r2, r2, #24
	adds	r3, #212
	add	r0, sp, #4
	add	r1, r8
	adds	r2, #8
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	add	sp, #132
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
