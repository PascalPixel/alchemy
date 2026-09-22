.syntax unified
	.thumb
	.set sub_08016ca4, 0x08016ca4
	.set sub_08016ce4, 0x08016ce4
	.set sub_08016cfc, 0x08016cfc
	.set sub_08016d18, 0x08016d18
	.set sub_08038040, 0x08038040
	.set sub_08038120, 0x08038120
	.set sub_08038140, 0x08038140
	.set sub_080ad010, 0x080ad010
	.set sub_080ad050, 0x080ad050
	.set sub_080ad078, 0x080ad078
	.set sub_080ad0f0, 0x080ad0f0
	.set sub_080cad84, 0x080cad84
	.set sub_080cb09c, 0x080cb09c
	.set sub_080cb144, 0x080cb144
	.set sub_080ccd48, 0x080ccd48
	.set sub_080cd91c, 0x080cd91c
	.set sub_080cdac0, 0x080cdac0
	.set sub_080cdf5c, 0x080cdf5c
	.set sub_080cdfb0, 0x080cdfb0
	.set sub_080ce1e8, 0x080ce1e8
	.set sub_080d22a8, 0x080d22a8
	.set sub_080d2350, 0x080d2350
	.set sub_080d295c, 0x080d295c
	.set sub_080d3be8, 0x080d3be8
	.set sub_080d407c, 0x080d407c
	.set sub_080db4b8, 0x080db4b8
	.set sub_080dc410, 0x080dc410
	.set sub_080dc7e8, 0x080dc7e8
	.global Overlay_080ce0ac
Overlay_080ce0ac:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r3, #255
	movs	r2, #1
	sub	sp, #8
	mov	fp, r3
	negs	r2, r2
	str	r2, [sp, #0]
	mov	r2, fp
	ands	r2, r0
	movs	r3, #15
	asrs	r0, r0, #10
	ands	r0, r3
	mov	fp, r2
	mov	sl, r0
	str	r1, [sp, #4]
	bl	sub_080ad0f0
	mov	r2, sl
	movs	r7, #0
	mov	r8, r0
	cmp	r2, #15
	bne.n	.L_080ce136
	movs	r3, #0
	mov	sl, r3
	movs	r6, #0
	cmp	sl, r8
	bge.n	.L_080ce15a
	ldr	r3, [pc, #536]
	movs	r0, #128
	lsls	r0, r0, #1
	movs	r2, #134
	adds	r0, #255
	lsls	r2, r2, #2
	mov	r9, r0
	adds	r5, r3, r2
.L_080ce102:
	ldrb	r0, [r5, #0]
	bl	sub_08016ca4
	movs	r4, #0
	adds	r0, #216
	movs	r1, #14
.L_080ce10e:
	ldrh	r2, [r0, #0]
	mov	r3, r9
	ands	r3, r2
	adds	r0, #2
	cmp	r3, fp
	bne.n	.L_080ce11c
	adds	r4, #1
.L_080ce11c:
	subs	r1, #1
	cmp	r1, #0
	bge.n	.L_080ce10e
	cmp	r7, r4
	bge.n	.L_080ce12c
	ldrb	r3, [r5, #0]
	adds	r7, r4, #0
	mov	sl, r3
.L_080ce12c:
	adds	r6, #1
	adds	r5, #1
	cmp	r6, r8
	blt.n	.L_080ce102
	b.n	.L_080ce15a
.L_080ce136:
	mov	r0, sl
	bl	sub_08016ca4
	movs	r4, #128
	lsls	r4, r4, #1
	adds	r4, #255
	adds	r0, #216
	movs	r1, #14
.L_080ce146:
	ldrh	r2, [r0, #0]
	adds	r3, r4, #0
	ands	r3, r2
	adds	r0, #2
	cmp	r3, fp
	bne.n	.L_080ce154
	adds	r7, #1
.L_080ce154:
	subs	r1, #1
	cmp	r1, #0
	bge.n	.L_080ce146
.L_080ce15a:
	cmp	r7, #0
	bne.n	.L_080ce16c
	ldr	r0, [pc, #432]
	movs	r1, #1
	bl	sub_08038040
	movs	r0, #1
	negs	r0, r0
	b.n	.L_080ce2fe
.L_080ce16c:
	mov	r0, fp
	bl	sub_080cdfb0
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_080ce1ee
	ldr	r3, [r6, #8]
	cmp	r3, #0
	beq.n	.L_080ce1ee
	movs	r0, #68
	adds	r0, #255
	bl	sub_08016d18
	movs	r0, #161
	lsls	r0, r0, #1
	bl	sub_08016d18
	ldrh	r2, [r6, #4]
	movs	r3, #128
	lsls	r3, r3, #3
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080ce1b2
	mov	r0, sl
	movs	r1, #1
	bl	sub_08038120
	mov	r0, fp
	movs	r1, #2
	bl	sub_08038120
	ldr	r0, [pc, #360]
	movs	r1, #1
	bl	sub_08038040
.L_080ce1b2:
	ldr	r3, [r6, #8]
	movs	r0, #128
	lsls	r0, r0, #9
	cmp	r3, r0
	bge.n	.L_080ce1de
	bl	sub_080cdf5c
	bl	sub_080cd91c
	adds	r5, r0, #0
	bl	sub_080d22a8
	ldr	r0, [r6, #8]
	bl	sub_080d3be8
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_080d407c
	bl	sub_080d2350
	b.n	sub_080ce1e8
.L_080ce1de:
	mov	r0, fp
	mov	r1, sl
	ldr	r2, [sp, #4]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2200
	str	r2, [sp, #0]
	b.n	.L_080ce2d4
.L_080ce1ee:
	movs	r0, #68
	movs	r7, #161
	adds	r0, #255
	lsls	r7, r7, #1
	bl	sub_08016d18
	adds	r0, r7, #0
	bl	sub_08016cfc
	mov	r0, fp
	bl	sub_080ad010
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	ldrh	r5, [r0, #40]
	mov	r8, r3
	cmp	r5, #0
	beq.n	.L_080ce2d4
	movs	r0, #70
	adds	r0, #255
	bl	sub_08016cfc
	adds	r0, r7, #0
	bl	sub_08016d18
	cmp	r5, #149
	bne.n	.L_080ce282
	movs	r0, #162
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080ce282
	mov	r0, fp
	movs	r1, #2
	bl	sub_08038120
	movs	r1, #13
	ldr	r0, [pc, #216]
	bl	sub_08038040
	movs	r0, #1
	bl	sub_080d295c
	adds	r6, r0, #0
	bl	sub_08038140
	movs	r0, #0
	cmp	r6, #0
	bne.n	.L_080ce2fe
	ldr	r1, [pc, #180]
	movs	r0, #152
	lsls	r0, r0, #2
	adds	r3, r1, r0
	ldrh	r2, [r3, #0]
	subs	r0, #128
	adds	r3, r1, r0
	strh	r2, [r3, #0]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #98
	adds	r3, r1, r2
	ldrh	r3, [r3, #0]
	adds	r0, #2
	adds	r2, r1, r0
	strh	r3, [r2, #0]
	movs	r2, #172
	movs	r3, #186
	lsls	r2, r2, #1
	lsls	r3, r3, #2
	add	r2, r8
	adds	r3, #255
	strh	r3, [r2, #0]
.L_080ce282:
	mov	r0, sl
	movs	r1, #1
	bl	sub_08038120
	movs	r6, #192
	mov	r0, fp
	movs	r1, #2
	bl	sub_08038120
	lsls	r6, r6, #4
	ldr	r0, [pc, #124]
	movs	r1, #1
	bl	sub_08038040
	adds	r6, #182
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_080dc410
	add	r6, r8
	movs	r2, #0
	movs	r5, #1
	mov	r8, r2
	strb	r5, [r6, #0]
	bl	sub_080db4b8
	mov	r3, r8
	strb	r3, [r6, #0]
	bl	sub_080dc7e8
	mov	r0, fp
	bl	sub_080ad010
	ldrb	r3, [r0, #12]
	ands	r5, r3
	cmp	r5, #0
	beq.n	.L_080ce2d4
	movs	r0, #68
	adds	r0, #255
	bl	sub_08016cfc
.L_080ce2d4:
	movs	r0, #161
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080ce2e8
	ldr	r0, [pc, #44]
	movs	r1, #1
	bl	sub_08038040
.L_080ce2e8:
	movs	r0, #68
	adds	r0, #255
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080ce2fc
	mov	r0, sl
	ldr	r1, [sp, #4]
	bl	sub_080ad050
.L_080ce2fc:
	ldr	r0, [sp, #0]
.L_080ce2fe:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x00000dc3
	.4byte 0x00000d94
	.2byte 0x0dc0
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
	sub	sp, #20
	ldr	r5, [r3, #16]
	mov	r9, r0
	bl	sub_080cdf5c
	bl	sub_080cad84
	ldrh	r0, [r0, #6]
	str	r0, [sp, #16]
	bl	sub_080cb09c
	str	r0, [sp, #12]
	bl	sub_080cb144
	movs	r2, #156
	str	r0, [sp, #8]
	lsls	r2, r2, #6
	movs	r0, #1
	negs	r0, r0
	adds	r2, #15
	str	r2, [sp, #4]
	str	r0, [sp, #0]
	mov	fp, r0
	ldr	r1, [r5, #0]
	mov	sl, r0
	cmp	r1, r0
	beq.n	.L_080ce434
.L_080ce366:
	movs	r3, #4
	ldrsh	r6, [r5, r3]
	movs	r3, #240
	lsls	r3, r3, #8
	ldrh	r2, [r5, #4]
	ands	r6, r3
	ldr	r3, [pc, #40]
	lsls	r0, r1, #16
	ands	r3, r2
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	movs	r7, #255
	lsrs	r0, r0, #24
	mov	r8, r3
	ands	r7, r2
	bl	sub_080ad078
	ldr	r3, [r5, #0]
	movs	r2, #15
	ands	r3, r2
	ldrb	r1, [r0, #6]
	cmp	r3, #5
	bne.n	.L_080ce426
	cmp	r1, r9
	bne.n	.L_080ce426
	b.n	.L_080ce3a0
	movs	r0, r0
	.2byte 0x0800
	.2byte 0x0000
.L_080ce3a0:
	movs	r2, #6
	ldrsh	r0, [r5, r2]
	bl	sub_080ccd48
	cmp	r0, #0
	beq.n	.L_080ce426
	mov	r3, r8
	cmp	r3, #0
	beq.n	.L_080ce3ca
	ldr	r2, [sp, #16]
	subs	r3, r6, r2
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	cmp	r3, #0
	bge.n	.L_080ce3c0
	negs	r3, r3
.L_080ce3c0:
	movs	r2, #184
	lsls	r2, r2, #5
	adds	r2, #255
	cmp	r3, r2
	bgt.n	.L_080ce426
.L_080ce3ca:
	ldr	r2, [r5, #0]
	movs	r3, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ce3dc
	movs	r3, #128
	lsls	r3, r3, #2
	str	r3, [sp, #0]
	b.n	.L_080ce426
.L_080ce3dc:
	movs	r3, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080ce410
	bl	sub_080cdf5c
	adds	r2, r7, #0
	adds	r1, r0, #0
	mov	r0, r9
	bl	sub_080cdac0
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	beq.n	.L_080ce426
	ldr	r3, [sp, #4]
	cmp	r3, r0
	ble.n	.L_080ce426
	movs	r2, #128
	lsls	r2, r2, #1
	mov	sl, r2
	mov	r3, sl
	orrs	r3, r7
	mov	sl, r3
	str	r0, [sp, #4]
	b.n	.L_080ce426
.L_080ce410:
	mov	r2, r9
	cmp	r2, #30
	bne.n	.L_080ce420
	ldr	r3, [sp, #8]
	cmp	r7, r3
	bne.n	.L_080ce426
.L_080ce41c:
	mov	fp, r7
	b.n	.L_080ce434
.L_080ce420:
	ldr	r2, [sp, #12]
	cmp	r7, r2
	beq.n	.L_080ce41c
.L_080ce426:
	adds	r5, #12
	ldr	r3, [r5, #0]
	movs	r2, #1
	negs	r2, r2
	adds	r1, r3, #0
	cmp	r3, r2
	bne.n	.L_080ce366
.L_080ce434:
	movs	r3, #1
	negs	r3, r3
	cmp	fp, r3
	beq.n	.L_080ce440
	mov	r0, fp
	b.n	.L_080ce448
.L_080ce440:
	mov	r0, sl
	cmp	sl, fp
	bne.n	.L_080ce448
	ldr	r0, [sp, #0]
.L_080ce448:
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
