.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08013560, 0x08013560
	.set sub_080148e8, 0x080148e8
	.set sub_08016ce4, 0x08016ce4
	.set sub_08016cfc, 0x08016cfc
	.set sub_08016d18, 0x08016d18
	.set sub_08020090, 0x08020090
	.set sub_08020310, 0x08020310
	.set sub_08038040, 0x08038040
	.set sub_080cad84, 0x080cad84
	.set sub_080ccd48, 0x080ccd48
	.set sub_080ccd78, 0x080ccd78
	.set sub_080cd7c0, 0x080cd7c0
	.set sub_080cd7f2, 0x080cd7f2
	.set sub_080cd8b0, 0x080cd8b0
	.set sub_080cdc74, 0x080cdc74
	.set sub_080cdea8, 0x080cdea8
	.set sub_080cf17c, 0x080cf17c
	.set sub_080d2260, 0x080d2260
	.set sub_080d2d84, 0x080d2d84
	.set sub_080d3460, 0x080d3460
	.set sub_080d4d08, 0x080d4d08
	.set sub_080d50f8, 0x080d50f8
	.set sub_080d53b8, 0x080d53b8
	.set sub_081c0010, 0x081c0010
	.global Func_080cd680
	.thumb_func
Func_080cd680:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #380]
	movs	r3, #192
	movs	r1, #133
	lsls	r3, r3, #18
	lsls	r1, r1, #2
	ldr	r6, [r3, #108]
	adds	r3, r2, r1
	adds	r1, #88
	adds	r7, r0, #0
	ldr	r0, [r3, #0]
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	mov	r8, r3
	cmp	r3, #0
	beq.n	.L_080cd6aa
	b.n	.L_080cd7fc
.L_080cd6aa:
	movs	r3, #32
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_080cd6da
	movs	r2, #194
	lsls	r2, r2, #1
	adds	r5, r6, r2
	movs	r1, #0
	ldrsh	r3, [r5, r1]
	cmp	r3, #12
	ble.n	.L_080cd6da
	adds	r2, #2
	adds	r3, r6, r2
	ldrh	r2, [r3, #0]
	movs	r3, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080cd6da
	movs	r1, #6
	movs	r2, #0
	bl	sub_080d3460
	mov	r3, r8
	strh	r3, [r5, #0]
.L_080cd6da:
	movs	r3, #128
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_080cd6fa
	movs	r1, #194
	lsls	r1, r1, #1
	adds	r5, r6, r1
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	cmp	r3, #12
	ble.n	.L_080cd6fa
	bl	sub_080d4d08
	cmp	r0, #0
	bne.n	.L_080cd6fa
	strh	r0, [r5, #0]
.L_080cd6fa:
	movs	r3, #64
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_080cd75c
	movs	r1, #194
	lsls	r1, r1, #1
	adds	r3, r6, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #12
	ble.n	.L_080cd75c
	adds	r1, #2
	adds	r3, r6, r1
	ldrh	r3, [r3, #0]
	movs	r2, #240
	movs	r0, #1
	ands	r2, r3
	negs	r0, r0
	cmp	r2, #128
	bne.n	.L_080cd728
	bl	sub_080d50f8
	b.n	.L_080cd730
.L_080cd728:
	cmp	r2, #64
	bne.n	.L_080cd730
	bl	sub_080d53b8
.L_080cd730:
	cmp	r0, #0
	bne.n	.L_080cd75c
	movs	r2, #194
	lsls	r2, r2, #1
	movs	r1, #173
	adds	r3, r6, r2
	lsls	r1, r1, #1
	strh	r0, [r3, #0]
	subs	r2, #38
	adds	r3, r6, r1
	strh	r0, [r3, #0]
	adds	r1, #2
	adds	r3, r6, r2
	strh	r0, [r3, #0]
	adds	r2, #6
	adds	r3, r6, r1
	strh	r0, [r3, #0]
	adds	r1, #10
	adds	r3, r6, r2
	strh	r0, [r3, #0]
	adds	r3, r6, r1
	strh	r0, [r3, #0]
.L_080cd75c:
	movs	r3, #2
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_080cd7fc
	movs	r2, #194
	lsls	r2, r2, #1
	adds	r3, r6, r2
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #12
	ble.n	.L_080cd7fc
	bl	sub_080cdc74
	cmp	r0, #0
	beq.n	.L_080cd7fc
	ldr	r3, [pc, #136]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl	.L_080cd91c
	movs	r3, #1
	negs	r3, r3
	adds	r5, r0, #0
	mov	r8, r3
	cmp	r5, r8
	beq.n	.L_080cd7fc
	movs	r0, #8
	adds	r1, r5, #0
	bl	sub_080ccd78
	cmp	r0, #0
	beq.n	sub_080cd7c0
	ldr	r3, [r0, #8]
	cmp	r3, #0
	beq.n	sub_080cd7c0
	movs	r1, #128
	lsls	r1, r1, #9
	cmp	r3, r1
	bge.n	.L_080cd7b8
	mov	r0, r8
	adds	r1, r3, #0
	bl	sub_080cdea8
	b.n	sub_080cd7c0
.L_080cd7b8:
	adds	r0, r7, #0
	adds	r1, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xf000
	.2byte 0xfade
	.2byte 0x2009
	adds	r1, r7, #0
	bl	sub_080ccd78
	cmp	r0, #0
	beq.n	sub_080cd7f2
	ldr	r3, [r0, #8]
	cmp	r3, #0
	beq.n	sub_080cd7f2
	movs	r2, #128
	lsls	r2, r2, #9
	cmp	r3, r2
	bge.n	.L_080cd7ea
	movs	r0, #1
	negs	r0, r0
	adds	r1, r3, #0
	bl	sub_080cdea8
	b.n	sub_080cd7f2
.L_080cd7ea:
	adds	r0, r7, #0
	adds	r1, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x23c2
	lsls	r3, r3, #1
	adds	r2, r6, r3
	movs	r3, #0
	strh	r3, [r2, #0]
.L_080cd7fc:
	movs	r0, #0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	adds	r6, r5, #0
	subs	r6, #242
	cmp	r6, #5
	bhi.n	.L_080cd832
	bl	sub_080d2260
	ldr	r3, [pc, #224]
	ldr	r0, [pc, #228]
	ldrb	r5, [r3, r6]
	movs	r1, #1
	adds	r0, r5, r0
	bl	sub_08038040
	ldr	r0, [pc, #220]
	movs	r1, #1
	adds	r0, r5, r0
	bl	sub_08038040
	b.n	.L_080cd8f8
.L_080cd832:
	adds	r1, r5, #0
	movs	r0, #3
	bl	sub_080ccd78
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L_080cd8e0
	ldr	r3, [r5, #0]
	movs	r2, #6
	ldrsh	r7, [r5, r2]
	asrs	r6, r3, #4
	ldrh	r2, [r5, #4]
	movs	r3, #31
	ands	r6, r3
	movs	r3, #128
	lsls	r3, r3, #3
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080cd874
	cmp	r6, #0
	beq.n	.L_080cd874
	bl	sub_080d2260
	ldr	r0, [pc, #156]
	movs	r1, #1
	adds	r0, r6, r0
	bl	sub_08038040
	movs	r0, #161
	lsls	r0, r0, #1
	bl	sub_08016cfc
	b.n	.L_080cd87c
.L_080cd874:
	movs	r0, #161
	lsls	r0, r0, #1
	bl	sub_08016d18
.L_080cd87c:
	ldr	r2, [r5, #8]
	movs	r3, #240
	lsls	r3, r3, #20
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080cd896
	ldr	r3, [pc, #124]
	ands	r3, r2
	movs	r2, #128
	lsls	r2, r2, #15
	cmp	r3, r2
	bne.n	.L_080cd8f0
	b.n	.L_080cd8c8
.L_080cd896:
	adds	r0, r7, #0
	bl	sub_080ccd48
	cmp	r0, #0
	beq.n	sub_080cd8b0
	ldr	r3, [pc, #104]
	movs	r2, #133
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	ldr	r3, [r5, #8]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x20a1
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080cd8f0
	ldr	r0, [pc, #68]
	movs	r1, #1
	adds	r0, r6, r0
	bl	sub_08038040
	b.n	.L_080cd8f0
.L_080cd8c8:
	adds	r0, r7, #0
	bl	sub_080ccd48
	cmp	r0, #0
	beq.n	.L_080cd8d6
	ldrh	r0, [r5, #8]
	b.n	.L_080cd8d8
.L_080cd8d6:
	ldr	r0, [pc, #56]
.L_080cd8d8:
	movs	r1, #1
	bl	sub_08038040
	b.n	.L_080cd8f0
.L_080cd8e0:
	ldr	r0, [pc, #48]
	movs	r1, #1
	bl	sub_08038040
	ldr	r0, [pc, #44]
	movs	r1, #1
	bl	sub_08038040
.L_080cd8f0:
	movs	r0, #161
	lsls	r0, r0, #1
	bl	sub_08016d18
.L_080cd8f8:
	movs	r0, #0
	pop	{r5, r6, r7, pc}
	.4byte 0x080eff22
	.4byte 0x00000dc4
	.4byte 0x00000def
	.4byte 0xfff00000
	.4byte 0x02000240
	.4byte 0x00000e1f
	.4byte 0x00000dc9
	.2byte 0x0df4
	.2byte 0x0000
.L_080cd91c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #1
	sub	sp, #4
	negs	r1, r1
	movs	r2, #32
	mov	r9, r0
	str	r1, [sp, #0]
	mov	fp, r2
	bl	sub_080cad84
	adds	r7, r0, #0
	cmp	r7, #0
	beq.n	.L_080cda3a
	movs	r3, #0
	mov	sl, r3
.L_080cd946:
	cmp	sl, r9
	beq.n	.L_080cda30
	mov	r0, sl
	bl	sub_080cad84
	adds	r6, r0, #0
	cmp	r6, #0
	beq.n	.L_080cda30
	movs	r1, #89
	adds	r1, r1, r6
	ldrb	r2, [r1, #0]
	movs	r3, #8
	ands	r3, r2
	mov	r8, r1
	cmp	r3, #0
	bne.n	.L_080cda30
	ldr	r4, [r6, #12]
	ldr	r1, [r7, #12]
	subs	r3, r4, r1
	cmp	r3, #0
	blt.n	.L_080cd978
	ldr	r2, [pc, #216]
	cmp	r3, r2
	ble.n	.L_080cd980
	b.n	.L_080cda30
.L_080cd978:
	ldr	r2, [pc, #208]
	subs	r3, r1, r4
	cmp	r3, r2
	bgt.n	.L_080cda30
.L_080cd980:
	ldr	r2, [r6, #8]
	ldr	r3, [r7, #8]
	subs	r0, r2, r3
	cmp	r0, #0
	bge.n	.L_080cd992
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r0, r0, r3
.L_080cd992:
	subs	r2, r4, r1
	asrs	r0, r0, #16
	cmp	r2, #0
	bge.n	.L_080cd9a2
	movs	r1, #255
	lsls	r1, r1, #8
	adds	r1, #255
	adds	r2, r2, r1
.L_080cd9a2:
	asrs	r1, r2, #16
	ldr	r3, [r7, #16]
	ldr	r2, [r6, #16]
	subs	r2, r2, r3
	cmp	r2, #0
	bge.n	.L_080cd9b6
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r2, r2, r3
.L_080cd9b6:
	asrs	r3, r2, #16
	adds	r2, r0, #0
	muls	r2, r0
	adds	r0, r2, #0
	adds	r2, r1, #0
	muls	r2, r1
	adds	r1, r3, #0
	muls	r1, r3
	adds	r0, r0, r2
	adds	r3, r1, #0
	adds	r0, r0, r3
	ldr	r3, [pc, #128]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4652
	adds	r5, r0, #0
	cmp	r2, #63
	ble.n	.L_080cd9dc
	lsls	r5, r5, #1
.L_080cd9dc:
	mov	r3, r8
	ldrb	r2, [r3, #0]
	movs	r3, #4
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080cd9f6
	lsls	r0, r5, #2
	adds	r0, r0, r5
	lsls	r0, r0, #1
	movs	r1, #13
	bl	sub_08002054
	adds	r5, r0, #0
.L_080cd9f6:
	cmp	r5, fp
	bge.n	.L_080cda30
	ldr	r3, [r7, #16]
	ldr	r0, [r6, #16]
	ldr	r1, [r6, #8]
	subs	r0, r0, r3
	ldr	r3, [r7, #8]
	subs	r1, r1, r3
	bl	sub_080148e8
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	cmp	r5, #11
	ble.n	.L_080cda2a
	ldrh	r3, [r7, #6]
	ldr	r1, [pc, #60]
	subs	r3, r0, r3
	lsls	r3, r3, #16
	asrs	r0, r3, #16
	cmp	r0, r1
	blt.n	.L_080cda30
	movs	r2, #188
	lsls	r2, r2, #6
	adds	r2, #255
	cmp	r0, r2
	bgt.n	.L_080cda30
.L_080cda2a:
	mov	r3, sl
	str	r3, [sp, #0]
	mov	fp, r5
.L_080cda30:
	movs	r1, #1
	add	sl, r1
	mov	r2, sl
	cmp	r2, #80
	ble.n	.L_080cd946
.L_080cda3a:
	ldr	r0, [sp, #0]
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x002fffff
	.4byte 0x030002d4
	.2byte 0xd001
	.2byte 0xffff
.L_080cda58:
	.2byte 0xb500
	adds	r2, r0, #0
	ldr	r0, [pc, #32]
	movs	r1, #0
	ldrsh	r3, [r0, r1]
	movs	r1, #1
	negs	r1, r1
	cmp	r3, r1
	beq.n	.L_080cda7e
	cmp	r2, r3
	beq.n	.L_080cda7e
	mov	ip, r1
.L_080cda70:
	adds	r0, #8
	movs	r1, #0
	ldrsh	r3, [r0, r1]
	cmp	r3, ip
	beq.n	.L_080cda7e
	cmp	r2, r3
	bne.n	.L_080cda70
.L_080cda7e:
	pop	{pc}
	.2byte 0xff28
	.2byte 0x080e
	push	{r5, r6, r7, lr}
	adds	r6, r1, #0
	adds	r5, r2, #0
	bl	.L_080cda58
	adds	r3, r6, #0
	adds	r3, #34
	ldrb	r2, [r3, #0]
	adds	r7, r0, #0
	ldr	r1, [r5, #8]
	ldr	r0, [r5, #0]
	ldr	r3, [r6, #20]
	bl	sub_08020310
	adds	r2, r0, #0
	cmp	r0, #0
	bge.n	.L_080cdaa8
	negs	r2, r0
.L_080cdaa8:
	movs	r1, #4
	ldrsh	r3, [r7, r1]
	lsls	r3, r3, #16
	cmp	r2, r3
	bgt.n	.L_080cdaba
	cmp	r0, #0
	bge.n	.L_080cdabe
	negs	r0, r0
	b.n	.L_080cdabe
.L_080cdaba:
	movs	r0, #1
	negs	r0, r0
.L_080cdabe:
	pop	{r5, r6, r7, pc}
.L_080cdac0:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	mov	sl, r0
	adds	r0, r1, #0
	adds	r5, r2, #0
	bl	sub_080cad84
	adds	r7, r0, #0
	adds	r0, r5, #0
	bl	sub_080cad84
	adds	r6, r0, #0
	cmp	r7, #0
	beq.n	.L_080cdbe0
	cmp	r6, #0
	beq.n	.L_080cdbe0
	movs	r1, #89
	adds	r1, r1, r6
	ldrb	r2, [r1, #0]
	movs	r3, #8
	ands	r3, r2
	mov	r9, r1
	cmp	r3, #0
	bne.n	.L_080cdbe0
	mov	r0, sl
	bl	.L_080cda58
	movs	r2, #4
	ldrsh	r3, [r0, r2]
	mov	r8, r0
	lsls	r1, r3, #16
	mov	r3, sl
	cmp	r3, #18
	bne.n	.L_080cdb22
	ldr	r0, [r6, #20]
	ldr	r3, [r7, #20]
	subs	r2, r0, r3
	cmp	r2, #0
	blt.n	.L_080cdb1a
.L_080cdb14:
	cmp	r2, r1
	bgt.n	.L_080cdbe0
	b.n	.L_080cdb32
.L_080cdb1a:
	subs	r3, r3, r0
	cmp	r3, r1
	ble.n	.L_080cdb32
	b.n	.L_080cdbe0
.L_080cdb22:
	ldr	r0, [r6, #12]
	ldr	r3, [r7, #12]
	subs	r2, r0, r3
	cmp	r2, #0
	bge.n	.L_080cdb14
	subs	r3, r3, r0
	cmp	r3, r1
	bgt.n	.L_080cdbe0
.L_080cdb32:
	mov	r2, r8
	movs	r1, #2
	ldrsh	r3, [r2, r1]
	adds	r5, r3, #1
	mov	r3, r9
	ldrb	r2, [r3, #0]
	movs	r3, #16
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080cdb50
	lsls	r3, r5, #1
	adds	r3, r3, r5
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r5, r3, #1
.L_080cdb50:
	ldr	r2, [r6, #8]
	ldr	r3, [r7, #8]
	subs	r0, r2, r3
	cmp	r0, #0
	bge.n	.L_080cdb62
	movs	r1, #255
	lsls	r1, r1, #8
	adds	r1, #255
	adds	r0, r0, r1
.L_080cdb62:
	ldr	r2, [r6, #16]
	ldr	r3, [r7, #16]
	asrs	r0, r0, #16
	subs	r2, r2, r3
	cmp	r2, #0
	bge.n	.L_080cdb76
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r2, r2, r3
.L_080cdb76:
	asrs	r3, r2, #16
	adds	r1, r0, #0
	muls	r1, r0
	adds	r2, r3, #0
	muls	r2, r3
	adds	r0, r1, #0
	adds	r3, r2, #0
	adds	r0, r0, r3
	ldr	r3, [pc, #108]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4682
	cmp	sl, r5
	bgt.n	.L_080cdbe0
	mov	r3, sl
	cmp	r3, #11
	ble.n	.L_080cdbe6
	mov	r1, r8
	movs	r2, #6
	ldrsh	r1, [r1, r2]
	mov	r8, r1
	cmp	r3, #19
	ble.n	.L_080cdbae
	lsls	r0, r1, #1
	movs	r1, #3
	bl	sub_08002054
	mov	r8, r0
.L_080cdbae:
	ldrh	r5, [r7, #6]
	movs	r2, #128
	lsls	r2, r2, #6
	movs	r3, #192
	adds	r5, r5, r2
	lsls	r3, r3, #8
	ands	r5, r3
	ldr	r0, [r6, #16]
	ldr	r3, [r7, #16]
	ldr	r1, [r6, #8]
	subs	r0, r0, r3
	ldr	r3, [r7, #8]
	subs	r1, r1, r3
	bl	sub_080148e8
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	subs	r0, r0, r5
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	cmp	r0, #0
	bge.n	.L_080cdbdc
	negs	r0, r0
.L_080cdbdc:
	cmp	r0, r8
	blt.n	.L_080cdbe6
.L_080cdbe0:
	movs	r0, #1
	negs	r0, r0
	b.n	.L_080cdbe8
.L_080cdbe6:
	mov	r0, sl
.L_080cdbe8:
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x02d4
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r7, #156
	movs	r3, #1
	lsls	r7, r7, #6
	negs	r3, r3
	adds	r6, r0, #0
	mov	sl, r1
	adds	r7, #15
	mov	r8, r3
	movs	r5, #0
.L_080cdc12:
	cmp	r5, r6
	beq.n	.L_080cdc30
	mov	r0, sl
	adds	r1, r6, #0
	adds	r2, r5, #0
	bl	.L_080cdac0
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	beq.n	.L_080cdc30
	cmp	r7, r0
	ble.n	.L_080cdc30
	mov	r8, r5
	adds	r7, r0, #0
.L_080cdc30:
	adds	r5, #1
	cmp	r5, #80
	ble.n	.L_080cdc12
	mov	r0, r8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	adds	r7, r0, #0
	adds	r0, r1, #0
	adds	r6, r2, #0
	bl	sub_080d2d84
	adds	r5, r0, #0
	movs	r0, #0
	cmp	r5, #0
	beq.n	.L_080cdc70
	movs	r0, #124
	bl	sub_081c0010
	movs	r1, #4
	adds	r0, r5, #0
	bl	sub_08020090
	movs	r0, #12
	bl	sub_08013560
	adds	r0, r7, #0
	adds	r1, r6, #0
	bl	sub_080cf17c
.L_080cdc70:
	pop	{r5, r6, r7, pc}
	.align 2, 0
