.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08014878, 0x08014878
	.set sub_08016ca4, 0x08016ca4
	.set sub_08016ce4, 0x08016ce4
	.set sub_08016cfc, 0x08016cfc
	.set sub_08016d18, 0x08016d18
	.set sub_080afdbc, 0x080afdbc
	.set sub_080afdd8, 0x080afdd8
	.set sub_080afe1c, 0x080afe1c
	.set sub_080b0ab8, 0x080b0ab8
	.set sub_080b0b78, 0x080b0b78
	.set sub_080b0e04, 0x080b0e04
	.set sub_080b1004, 0x080b1004
	.global Overlay_080ae834
Overlay_080ae834:
.L_080ae834:
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	adds	r6, r1, #0
	lsls	r5, r6, #2
	adds	r5, r5, r6
	mov	r8, r2
	lsls	r5, r5, #2
	mov	sl, r0
	add	r5, r8
	bl	sub_080b0b78
	adds	r5, #48
	mov	r0, sl
	adds	r1, r6, #0
	mov	r2, r8
	bl	sub_080b0e04
	adds	r0, r5, #0
	bl	sub_08016cfc
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r0, #128
	lsls	r0, r0, #4
	sub	sp, #12
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080ae886
	b.n	.L_080aea18
.L_080ae886:
	bl	sub_080afdbc
	mov	fp, r0
	cmp	r0, #0
	ble.n	.L_080ae8aa
	ldr	r3, [pc, #404]
	movs	r0, #134
	lsls	r0, r0, #2
	add	r2, sp, #4
	adds	r1, r3, r0
	mov	r5, fp
.L_080ae89c:
	ldrb	r3, [r1, #0]
	subs	r5, #1
	strb	r3, [r2, #0]
	adds	r1, #1
	adds	r2, #1
	cmp	r5, #0
	bne.n	.L_080ae89c
.L_080ae8aa:
	mov	r1, fp
	cmp	r1, #0
	ble.n	.L_080ae8c6
	add	r6, sp, #4
	mov	r5, fp
.L_080ae8b4:
	ldrb	r0, [r6, #0]
	subs	r5, #1
	lsls	r0, r0, #24
	asrs	r0, r0, #24
	adds	r6, #1
	bl	sub_080afe1c
	cmp	r5, #0
	bne.n	.L_080ae8b4
.L_080ae8c6:
	movs	r5, #0
.L_080ae8c8:
	adds	r0, r5, #0
	adds	r5, #1
	bl	sub_080afdd8
	cmp	r5, #3
	ble.n	.L_080ae8c8
	movs	r0, #0
	movs	r1, #0
	movs	r2, #0
	bl	.L_080ae834
	movs	r2, #0
	movs	r0, #3
	movs	r1, #1
	bl	.L_080ae834
	movs	r3, #1
	mov	r6, sp
	movs	r2, #0
	strb	r3, [r6, #0]
	strb	r3, [r6, #1]
	strb	r2, [r6, #2]
	strb	r2, [r6, #3]
	movs	r2, #16
	mov	r9, r2
	mov	sl, r6
.L_080ae8fc:
	bl	sub_08014878
	lsls	r0, r0, #2
	lsrs	r5, r0, #16
	bl	sub_08014878
	lsls	r3, r0, #3
	subs	r3, r3, r0
	lsls	r0, r5, #2
	lsrs	r3, r3, #16
	adds	r0, r0, r5
	mov	r8, r3
	lsls	r0, r0, #2
	add	r0, r8
	adds	r0, #48
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080ae99e
	mov	r0, sl
	ldrb	r3, [r0, r5]
	movs	r1, #144
	lsls	r1, r1, #20
	adds	r3, #1
	strb	r3, [r0, r5]
	mov	ip, r1
	movs	r7, #0
	adds	r1, r6, #0
	adds	r0, r6, #0
	adds	r2, r6, #3
.L_080ae938:
	ldrb	r3, [r0, #0]
	adds	r0, #1
	lsls	r3, r3, #24
	asrs	r4, r3, #24
	cmp	ip, r3
	ble.n	.L_080ae94a
	ldrb	r3, [r1, #0]
	lsls	r3, r3, #24
	mov	ip, r3
.L_080ae94a:
	asrs	r3, r7, #24
	cmp	r3, r4
	bge.n	.L_080ae954
	ldrb	r3, [r1, #0]
	lsls	r7, r3, #24
.L_080ae954:
	adds	r1, #1
	cmp	r1, r2
	ble.n	.L_080ae938
	mov	r2, sl
	ldrb	r3, [r2, r5]
	mov	r0, ip
	subs	r3, #1
	strb	r3, [r2, r5]
	asrs	r3, r7, #24
	asrs	r2, r0, #24
	subs	r3, r3, r2
	cmp	r3, #1
	bgt.n	.L_080ae99e
	bl	sub_08014878
	movs	r3, #100
	adds	r2, r0, #0
	muls	r2, r3
	lsls	r3, r5, #3
	ldr	r1, [pc, #176]
	subs	r3, r3, r5
	add	r3, r8
	lsls	r3, r3, #1
	ldrsh	r3, [r1, r3]
	lsrs	r2, r2, #16
	cmp	r2, r3
	bcs.n	.L_080ae99e
	mov	r1, r8
	adds	r0, r5, #0
	bl	sub_080b0ab8
	ldrb	r3, [r6, r5]
	movs	r1, #1
	adds	r3, #1
	strb	r3, [r6, r5]
	negs	r1, r1
	add	r9, r1
.L_080ae99e:
	mov	r2, r9
	cmp	r2, #0
	bne.n	.L_080ae8fc
	bl	sub_080b1004
	movs	r5, #0
	mov	sl, r5
.L_080ae9ac:
	movs	r3, #0
	mov	r8, r3
	lsls	r3, r5, #2
	adds	r3, r3, r5
	lsls	r3, r3, #2
	movs	r7, #128
	adds	r6, r3, #0
	lsls	r7, r7, #4
	adds	r6, #48
	add	r7, sl
.L_080ae9c0:
	adds	r0, r6, #0
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080ae9d0
	adds	r0, r7, #0
	bl	sub_08016cfc
.L_080ae9d0:
	adds	r0, r6, #0
	bl	sub_08016d18
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r6, #1
	adds	r7, #1
	cmp	r1, #6
	ble.n	.L_080ae9c0
	movs	r2, #7
	adds	r5, #1
	add	sl, r2
	cmp	r5, #3
	ble.n	.L_080ae9ac
	movs	r5, #0
.L_080ae9f0:
	adds	r0, r5, #0
	adds	r5, #1
	bl	sub_080afe1c
	cmp	r5, #3
	ble.n	.L_080ae9f0
	mov	r3, fp
	cmp	r3, #0
	ble.n	.L_080aea18
	add	r6, sp, #4
	mov	r5, fp
.L_080aea06:
	ldrb	r0, [r6, #0]
	subs	r5, #1
	lsls	r0, r0, #24
	asrs	r0, r0, #24
	adds	r6, #1
	bl	sub_080afdd8
	cmp	r5, #0
	bne.n	.L_080aea06
.L_080aea18:
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x02000240
	.2byte 0x1290
	.2byte 0x080b
	push	{r5, lr}
	bl	sub_08016ca4
	adds	r5, r0, #0
	movs	r2, #56
	ldrsh	r0, [r5, r2]
	movs	r3, #52
	ldrsh	r1, [r5, r3]
	lsls	r0, r0, #14
	bl	sub_08002054
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bgt.n	.L_080aea56
	movs	r3, #0
	cmp	r0, #0
	blt.n	.L_080aea56
	adds	r3, r0, #0
.L_080aea56:
	strh	r3, [r5, #20]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_080aea6a
	movs	r2, #56
	ldrsh	r3, [r5, r2]
	cmp	r3, #0
	beq.n	.L_080aea6a
	movs	r3, #1
	strh	r3, [r5, #20]
.L_080aea6a:
	movs	r3, #58
	ldrsh	r0, [r5, r3]
	movs	r2, #54
	ldrsh	r1, [r5, r2]
	lsls	r0, r0, #14
	bl	sub_08002054
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bgt.n	.L_080aea88
	movs	r3, #0
	cmp	r0, #0
	blt.n	.L_080aea88
	adds	r3, r0, #0
.L_080aea88:
	strh	r3, [r5, #22]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L_080aea9c
	movs	r2, #58
	ldrsh	r3, [r5, r2]
	cmp	r3, #0
	beq.n	.L_080aea9c
	movs	r3, #1
	strh	r3, [r5, #22]
.L_080aea9c:
	pop	{r5, pc}
