.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08016ca4, 0x08016ca4
	.set sub_08016ce4, 0x08016ce4
	.set sub_08038040, 0x08038040
	.set sub_08038120, 0x08038120
	.set sub_08038140, 0x08038140
	.set sub_080ad008, 0x080ad008
	.set sub_080ad010, 0x080ad010
	.set sub_080ad018, 0x080ad018
	.set sub_080ad020, 0x080ad020
	.set sub_080ad028, 0x080ad028
	.set sub_080ad088, 0x080ad088
	.set sub_080ad0f0, 0x080ad0f0
	.set sub_080ad110, 0x080ad110
	.set sub_080ad1e8, 0x080ad1e8
	.set sub_080ad278, 0x080ad278
	.set sub_080ad288, 0x080ad288
	.set sub_080ad2a0, 0x080ad2a0
	.set sub_080ad2f0, 0x080ad2f0
	.set sub_080d295c, 0x080d295c
	.set sub_08108068, 0x08108068
	.set sub_081c0010, 0x081c0010
	.global Func_080d2414
	.thumb_func
Func_080d2414:
.L_080d2414:
	push	{r5, r6, lr}
	movs	r6, #192
	lsrs	r5, r0, #10
	movs	r3, #15
	lsls	r6, r6, #2
	adds	r6, #255
	ands	r5, r3
	ands	r6, r0
	cmp	r5, #7
	ble.n	.L_080d242e
	movs	r0, #1
	negs	r0, r0
	b.n	.L_080d2452
.L_080d242e:
	adds	r0, r5, #0
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080d243e
	movs	r0, #2
	negs	r0, r0
	b.n	.L_080d2452
.L_080d243e:
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl	sub_080ad088
	cmp	r0, #0
	bne.n	.L_080d2450
	movs	r0, #3
	negs	r0, r0
	b.n	.L_080d2452
.L_080d2450:
	movs	r0, #0
.L_080d2452:
	pop	{r5, r6, pc}
.L_080d2454:
	push	{r5, r6, lr}
	ldr	r6, [pc, #44]
	movs	r3, #144
	lsls	r3, r3, #2
	adds	r5, r6, r3
	ldrh	r0, [r5, #0]
	bl	.L_080d2414
	cmp	r0, #0
	beq.n	.L_080d246c
	movs	r3, #0
	strh	r3, [r5, #0]
.L_080d246c:
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #66
	adds	r5, r6, r3
	ldrh	r0, [r5, #0]
	bl	.L_080d2414
	cmp	r0, #0
	beq.n	.L_080d2482
	movs	r3, #0
	strh	r3, [r5, #0]
.L_080d2482:
	pop	{r5, r6, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r5, r0, #0
	bl	sub_080ad110
	bl	.L_080d2454
	adds	r0, r5, #0
	bl	sub_08016ca4
	adds	r6, r0, #0
	ldrh	r1, [r6, #52]
	ldrh	r3, [r6, #54]
	strh	r1, [r6, #56]
	strh	r3, [r6, #58]
	lsls	r1, r1, #16
	asrs	r1, r1, #16
	lsls	r0, r1, #14
	bl	sub_08002054
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bgt.n	.L_080d24c2
	movs	r3, #0
	cmp	r0, #0
	blt.n	.L_080d24c2
	adds	r3, r0, #0
.L_080d24c2:
	strh	r3, [r6, #20]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_080d24d6
	movs	r2, #56
	ldrsh	r3, [r6, r2]
	cmp	r3, #0
	beq.n	.L_080d24d6
	movs	r3, #1
	strh	r3, [r6, #20]
.L_080d24d6:
	movs	r3, #58
	ldrsh	r0, [r6, r3]
	movs	r2, #54
	ldrsh	r1, [r6, r2]
	lsls	r0, r0, #14
	bl	sub_08002054
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bgt.n	.L_080d24f4
	movs	r3, #0
	cmp	r0, #0
	blt.n	.L_080d24f4
	adds	r3, r0, #0
.L_080d24f4:
	strh	r3, [r6, #22]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_080d2508
	movs	r2, #58
	ldrsh	r3, [r6, r2]
	cmp	r3, #0
	beq.n	.L_080d2508
	movs	r3, #1
	strh	r3, [r6, #22]
.L_080d2508:
	movs	r3, #50
	adds	r3, #255
	adds	r2, r6, r3
	movs	r3, #0
	strb	r3, [r2, #0]
	mov	r8, r3
	bl	sub_080ad0f0
	cmp	r8, r0
	bge.n	.L_080d2542
	ldr	r3, [pc, #164]
	movs	r2, #134
	lsls	r2, r2, #2
	adds	r7, r3, r2
	adds	r5, r0, #0
.L_080d2526:
	ldrb	r0, [r7, #0]
	bl	sub_08016ca4
	adds	r6, r0, #0
	movs	r2, #56
	ldrsh	r3, [r6, r2]
	adds	r7, #1
	cmp	r3, #0
	beq.n	.L_080d253c
	movs	r3, #1
	add	r8, r3
.L_080d253c:
	subs	r5, #1
	cmp	r5, #0
	bne.n	.L_080d2526
.L_080d2542:
	mov	r2, r8
	cmp	r2, #0
	bne.n	.L_080d25be
	ldr	r3, [pc, #120]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl	sub_08016ca4
	movs	r5, #1
	adds	r6, r0, #0
	strh	r5, [r6, #56]
	lsls	r5, r5, #14
	movs	r3, #52
	ldrsh	r1, [r6, r3]
	adds	r0, r5, #0
	bl	sub_08002054
	movs	r2, #128
	lsls	r2, r2, #7
	cmp	r0, r2
	bgt.n	.L_080d2578
	movs	r5, #0
	cmp	r0, #0
	blt.n	.L_080d2578
	adds	r5, r0, #0
.L_080d2578:
	lsls	r3, r5, #16
	strh	r5, [r6, #20]
	cmp	r3, #0
	bne.n	.L_080d258c
	movs	r2, #56
	ldrsh	r3, [r6, r2]
	cmp	r3, #0
	beq.n	.L_080d258c
	movs	r3, #1
	strh	r3, [r6, #20]
.L_080d258c:
	movs	r3, #58
	ldrsh	r0, [r6, r3]
	movs	r2, #54
	ldrsh	r1, [r6, r2]
	lsls	r0, r0, #14
	bl	sub_08002054
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bgt.n	.L_080d25aa
	movs	r3, #0
	cmp	r0, #0
	blt.n	.L_080d25aa
	adds	r3, r0, #0
.L_080d25aa:
	strh	r3, [r6, #22]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_080d25be
	movs	r2, #58
	ldrsh	r3, [r6, r2]
	cmp	r3, #0
	beq.n	.L_080d25be
	movs	r3, #1
	strh	r3, [r6, #22]
.L_080d25be:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, lr}
	adds	r6, r0, #0
	bl	sub_080ad0f0
	adds	r0, r6, #0
	bl	sub_080ad2f0
	cmp	r0, #0
	bge.n	.L_080d2604
	adds	r0, r6, #0
	movs	r1, #2
	bl	sub_08038120
	ldr	r5, [pc, #36]
	movs	r1, #1
	adds	r0, r5, #0
	bl	sub_08038040
	adds	r5, #1
	adds	r0, r6, #0
	movs	r1, #2
	bl	sub_08038120
	adds	r0, r5, #0
	movs	r1, #1
	bl	sub_08038040
	movs	r0, #1
	negs	r0, r0
	b.n	.L_080d2606
.L_080d2604:
	movs	r0, #0
.L_080d2606:
	pop	{r5, r6, pc}
	.2byte 0x0e26
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r7, #226
	mov	fp, r3
	lsls	r7, r7, #1
	add	r7, fp
	movs	r3, #0
	ldrsh	r2, [r7, r3]
	sub	sp, #12
	str	r2, [sp, #0]
	adds	r6, r0, #0
	bl	sub_080ad028
	movs	r2, #1
	mov	r8, r0
	negs	r2, r2
	cmp	r8, r2
	beq.n	.L_080d2642
	b.n	.L_080d2782
.L_080d2642:
	adds	r0, r6, #0
	movs	r1, #2
	bl	sub_08038120
	ldr	r0, [pc, #396]
	movs	r1, #1
	bl	sub_08038040
	ldr	r0, [pc, #392]
	movs	r1, #1
	bl	sub_08038040
	movs	r3, #8
	movs	r2, #4
	add	r3, sp
	add	r2, sp
	mov	r9, r3
	mov	sl, r2
.L_080d2666:
	ldr	r7, [pc, #376]
	movs	r1, #1
	adds	r0, r7, #0
	bl	sub_08038040
	mov	r0, r9
	mov	r1, sl
	bl	sub_08108068
	movs	r3, #1
	adds	r5, r0, #0
	negs	r3, r3
	cmp	r5, r3
	bne.n	.L_080d26d8
	adds	r0, r6, #0
	bl	sub_080ad010
	ldrb	r2, [r0, #3]
	movs	r3, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080d269e
	adds	r0, r6, #0
	movs	r1, #2
	bl	sub_08038120
	adds	r0, r7, #4
	b.n	.L_080d2706
.L_080d269e:
	adds	r0, r6, #0
	movs	r1, #2
	bl	sub_08038120
	adds	r0, r7, #1
	movs	r1, #5
	bl	sub_08038040
	movs	r0, #1
	bl	sub_080d295c
	adds	r5, r0, #0
	bl	sub_08038140
	cmp	r5, #0
	bne.n	.L_080d2666
	movs	r1, #1
	adds	r0, r6, #0
	bl	sub_080ad1e8
	adds	r0, r6, #0
	movs	r1, #2
	bl	sub_08038120
	adds	r0, r7, #2
	movs	r1, #1
	bl	sub_08038040
	b.n	.L_080d2772
.L_080d26d8:
	ldr	r0, [sp, #8]
	bl	sub_08016ca4
	ldr	r1, [sp, #4]
	ldr	r0, [sp, #8]
	bl	sub_080ad018
	adds	r1, r6, #0
	adds	r5, r0, #0
	ldr	r0, [sp, #8]
	bl	sub_080ad2a0
	cmp	r0, #29
	ble.n	.L_080d270e
	ldr	r0, [sp, #8]
	movs	r1, #1
	bl	sub_08038120
	adds	r0, r6, #0
	movs	r1, #2
	bl	sub_08038120
	adds	r0, r7, #7
.L_080d2706:
	movs	r1, #1
	bl	sub_08038040
	b.n	.L_080d2666
.L_080d270e:
	cmp	r5, #0
	ble.n	.L_080d2720
.L_080d2712:
	ldr	r0, [sp, #8]
	ldr	r1, [sp, #4]
	subs	r5, #1
	bl	sub_080ad278
	cmp	r5, #0
	bne.n	.L_080d2712
.L_080d2720:
	ldr	r0, [sp, #8]
	bl	sub_080ad288
	ldr	r0, [sp, #8]
	bl	sub_080ad008
	adds	r0, r6, #0
	bl	sub_080ad028
	mov	r8, r0
	movs	r0, #83
	bl	sub_081c0010
	ldr	r3, [pc, #168]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r3, [r3, #0]
	cmp	r8, r3
	bne.n	.L_080d275a
	adds	r0, r6, #0
	movs	r1, #2
	bl	sub_08038120
	ldr	r0, [pc, #132]
	movs	r1, #3
	bl	sub_08038040
	b.n	.L_080d2772
.L_080d275a:
	adds	r0, r6, #0
	movs	r1, #2
	bl	sub_08038120
	mov	r0, r8
	movs	r1, #1
	bl	sub_08038120
	ldr	r0, [pc, #124]
	movs	r1, #3
	bl	sub_08038040
.L_080d2772:
	mov	r2, sp
	movs	r3, #226
	ldrh	r2, [r2, #0]
	lsls	r3, r3, #1
	add	r3, fp
	strh	r2, [r3, #0]
	mov	r0, r8
	b.n	.L_080d27c8
.L_080d2782:
	movs	r0, #83
	bl	sub_081c0010
	adds	r0, r6, #0
	movs	r1, #2
	bl	sub_08038120
	ldr	r5, [pc, #68]
	movs	r1, #3
	adds	r0, r5, #0
	bl	sub_08038040
	ldr	r3, [pc, #72]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r3, [r3, #0]
	cmp	r8, r3
	beq.n	.L_080d27c0
	adds	r0, r6, #0
	movs	r1, #2
	bl	sub_08038120
	mov	r0, r8
	movs	r1, #1
	bl	sub_08038120
	adds	r0, r5, #1
	movs	r1, #1
	bl	sub_08038040
.L_080d27c0:
	mov	r3, sp
	ldrh	r3, [r3, #0]
	mov	r0, r8
	strh	r3, [r7, #0]
.L_080d27c8:
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x00000e11
	.4byte 0x00000e20
	.4byte 0x00000e21
	.4byte 0x02000240
	.2byte 0x0e12
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r2, #0
	adds	r1, r0, #0
	adds	r0, r5, #0
	bl	sub_080ad020
	cmp	r0, #0
	blt.n	.L_080d2800
	adds	r0, r5, #0
	b.n	.L_080d2804
.L_080d2800:
	movs	r0, #1
	negs	r0, r0
.L_080d2804:
	pop	{r5, pc}
	movs	r0, r0
	bx	lr
	movs	r0, r0
	bx	lr
	.2byte 0x0000
