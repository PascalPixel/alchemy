.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08016ca4, 0x08016ca4
	.set sub_08016ce4, 0x08016ce4
	.set sub_08016cfc, 0x08016cfc
	.set sub_080ca3f4, 0x080ca3f4
	.global Overlay_080c9694
Overlay_080c9694:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r3, [pc, #208]
	mov	r8, r0
	movs	r0, #240
	lsls	r0, r0, #1
	adds	r3, r3, r0
	movs	r1, #0
	ldrsh	r7, [r3, r1]
	ldr	r3, [pc, #196]
	ldr	r0, [r3, #20]
	mov	lr, r0
	.2byte 0xf800
	.2byte 0x21ba
	lsls	r1, r1, #2
	movs	r2, #0
	adds	r1, #255
	adds	r6, r0, #0
	mov	r9, r2
	cmp	r8, r1
	beq.n	.L_080c9766
	b.n	.L_080c972c
.L_080c96c6:
	cmp	r5, r7
	bne.n	.L_080c972c
	mov	sl, r6
	b.n	.L_080c96f0
.L_080c96ce:
	cmp	r2, #255
	beq.n	.L_080c96d6
	cmp	r2, r8
	bne.n	.L_080c96f0
.L_080c96d6:
	cmp	r0, #0
	beq.n	.L_080c96ea
	movs	r1, #1
	negs	r1, r1
	cmp	r0, r1
	beq.n	.L_080c96ea
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080c96f0
.L_080c96ea:
	adds	r1, r5, #0
	mov	r9, r7
	b.n	.L_080c9748
.L_080c96f0:
	ldmia	r6!, {r3}
	movs	r2, #255
	lsls	r2, r2, #12
	ands	r2, r3
	movs	r5, #240
	lsrs	r7, r2, #12
	movs	r0, #128
	movs	r2, #255
	lsls	r5, r5, #4
	lsls	r2, r2, #20
	lsls	r0, r0, #21
	adds	r5, #255
	ands	r2, r3
	ands	r0, r3
	ands	r5, r3
	lsrs	r2, r2, #20
	cmp	r0, #0
	beq.n	.L_080c9716
	ldmia	r6!, {r0}
.L_080c9716:
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	cmp	r5, r3
	beq.n	.L_080c9724
	cmp	r2, #0
	bne.n	.L_080c96ce
.L_080c9724:
	mov	r2, sl
	ldr	r1, [r2, #0]
	ands	r1, r3
	b.n	.L_080c9748
.L_080c972c:
	ldmia	r6!, {r5}
	ldr	r3, [pc, #72]
	ands	r3, r5
	cmp	r3, #0
	bne.n	.L_080c972c
	movs	r3, #240
	lsls	r3, r3, #4
	adds	r3, #255
	ands	r5, r3
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	cmp	r5, r3
	bne.n	.L_080c96c6
.L_080c9748:
	movs	r0, #186
	lsls	r0, r0, #2
	adds	r0, #255
	cmp	r1, r0
	beq.n	.L_080c9766
	ldr	r2, [pc, #28]
	movs	r0, #240
	lsls	r0, r0, #1
	adds	r3, r2, r0
	strh	r1, [r3, #0]
	movs	r1, #241
	lsls	r1, r1, #1
	adds	r3, r2, r1
	mov	r2, r9
	strh	r2, [r3, #0]
.L_080c9766:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x02008000
	.2byte 0xf000
	.2byte 0xffff
	push	{r5, r6, lr}
	ldr	r1, [pc, #412]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r2, #86
	movs	r4, #1
	adds	r3, r1, r2
	negs	r4, r4
	strh	r0, [r3, #0]
	cmp	r0, r4
	beq.n	.L_080c9794
	b.n	.L_080c98a2
.L_080c9794:
	movs	r4, #133
	lsls	r4, r4, #2
	adds	r3, r1, r4
	ldr	r0, [r3, #0]
	bl	sub_08016ca4
	adds	r6, r0, #0
	movs	r5, #56
	ldrsh	r3, [r6, r5]
	cmp	r3, #0
	bne.n	.L_080c9810
	movs	r5, #1
	strh	r5, [r6, #56]
	lsls	r5, r5, #14
	movs	r0, #52
	ldrsh	r1, [r6, r0]
	adds	r0, r5, #0
	bl	sub_08002054
	movs	r1, #128
	lsls	r1, r1, #7
	cmp	r0, r1
	bgt.n	.L_080c97ca
	movs	r5, #0
	cmp	r0, #0
	blt.n	.L_080c97ca
	adds	r5, r0, #0
.L_080c97ca:
	lsls	r3, r5, #16
	strh	r5, [r6, #20]
	cmp	r3, #0
	bne.n	.L_080c97de
	movs	r2, #56
	ldrsh	r3, [r6, r2]
	cmp	r3, #0
	beq.n	.L_080c97de
	movs	r3, #1
	strh	r3, [r6, #20]
.L_080c97de:
	movs	r3, #58
	ldrsh	r0, [r6, r3]
	movs	r4, #54
	ldrsh	r1, [r6, r4]
	lsls	r0, r0, #14
	bl	sub_08002054
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bgt.n	.L_080c97fc
	movs	r3, #0
	cmp	r0, #0
	blt.n	.L_080c97fc
	adds	r3, r0, #0
.L_080c97fc:
	strh	r3, [r6, #22]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_080c9810
	movs	r5, #58
	ldrsh	r3, [r6, r5]
	cmp	r3, #0
	beq.n	.L_080c9810
	movs	r3, #1
	strh	r3, [r6, #22]
.L_080c9810:
	ldr	r1, [pc, #264]
	movs	r0, #249
	lsls	r0, r0, #1
	adds	r3, r1, r0
	movs	r5, #250
	lsls	r5, r5, #1
	movs	r4, #0
	ldrsh	r2, [r3, r4]
	adds	r3, r1, r5
	movs	r5, #1
	negs	r5, r5
	movs	r4, #0
	ldrsh	r0, [r3, r4]
	cmp	r2, r5
	bne.n	.L_080c983a
	cmp	r0, r2
	beq.n	.L_080c9868
	movs	r5, #244
	lsls	r5, r5, #1
	adds	r3, r1, r5
	ldrh	r2, [r3, #0]
.L_080c983a:
	movs	r4, #240
	lsls	r4, r4, #1
	adds	r3, r1, r4
	strh	r2, [r3, #0]
	movs	r5, #1
	negs	r5, r5
	cmp	r0, r5
	beq.n	.L_080c9856
	ldr	r3, [pc, #208]
	movs	r1, #241
	lsls	r1, r1, #1
	adds	r3, r3, r1
	strh	r0, [r3, #0]
	b.n	.L_080c9886
.L_080c9856:
	ldr	r2, [pc, #196]
	movs	r4, #245
	lsls	r4, r4, #1
	adds	r3, r2, r4
	ldrh	r3, [r3, #0]
	movs	r5, #241
	lsls	r5, r5, #1
	adds	r2, r2, r5
	b.n	.L_080c9884
.L_080c9868:
	movs	r0, #242
	lsls	r0, r0, #1
	adds	r3, r1, r0
	ldrh	r2, [r3, #0]
	movs	r4, #240
	lsls	r4, r4, #1
	adds	r3, r1, r4
	strh	r2, [r3, #0]
	movs	r5, #243
	lsls	r5, r5, #1
	adds	r3, r1, r5
	ldrh	r3, [r3, #0]
	subs	r0, #2
	adds	r2, r1, r0
.L_080c9884:
	strh	r3, [r2, #0]
.L_080c9886:
	ldr	r2, [pc, #148]
	movs	r1, #240
	lsls	r1, r1, #1
	movs	r5, #241
	adds	r3, r2, r1
	lsls	r5, r5, #1
	movs	r4, #0
	ldrsh	r0, [r3, r4]
	adds	r3, r2, r5
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	bl	sub_080ca3f4
	b.n	.L_080c9918
.L_080c98a2:
	movs	r5, #247
	lsls	r5, r5, #1
	adds	r3, r1, r5
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	adds	r5, #2
	adds	r3, r1, r5
	movs	r5, #0
	ldrsh	r0, [r3, r5]
	cmp	r2, r4
	bne.n	.L_080c98c4
	cmp	r0, r2
	beq.n	.L_080c98f4
	movs	r5, #244
	lsls	r5, r5, #1
	adds	r3, r1, r5
	ldrh	r2, [r3, #0]
.L_080c98c4:
	movs	r4, #240
	lsls	r4, r4, #1
	adds	r3, r1, r4
	strh	r2, [r3, #0]
	movs	r5, #1
	negs	r5, r5
	cmp	r0, r5
	beq.n	.L_080c98e0
	ldr	r3, [pc, #68]
	movs	r1, #241
	lsls	r1, r1, #1
	adds	r3, r3, r1
	strh	r0, [r3, #0]
	b.n	.L_080c9918
.L_080c98e0:
	ldr	r2, [pc, #56]
	movs	r4, #245
	lsls	r4, r4, #1
	adds	r3, r2, r4
	ldrh	r3, [r3, #0]
	movs	r5, #241
	lsls	r5, r5, #1
	adds	r2, r2, r5
	strh	r3, [r2, #0]
	b.n	.L_080c9918
.L_080c98f4:
	movs	r0, #244
	lsls	r0, r0, #1
	adds	r3, r1, r0
	ldrh	r2, [r3, #0]
	movs	r4, #240
	lsls	r4, r4, #1
	adds	r3, r1, r4
	strh	r2, [r3, #0]
	movs	r5, #245
	lsls	r5, r5, #1
	adds	r3, r1, r5
	ldrh	r3, [r3, #0]
	subs	r0, #6
	adds	r2, r1, r0
	strh	r3, [r2, #0]
	subs	r0, #217
	bl	sub_08016cfc
.L_080c9918:
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x02000240
