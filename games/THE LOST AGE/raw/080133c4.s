.syntax unified
	.thumb
	.set sub_080005e8, 0x080005e8
	.set sub_08000630, 0x08000630
	.set sub_080019f4, 0x080019f4
	.set sub_0801314c, 0x0801314c
	.set sub_0801352e, 0x0801352e
	.set sub_08013a36, 0x08013a36
	.set sub_08013af6, 0x08013af6
	.set sub_08013d84, 0x08013d84
	.set sub_08013de4, 0x08013de4
	.set sub_08013ffc, 0x08013ffc
	.set sub_080147d8, 0x080147d8
	.set sub_08014840, 0x08014840
	.set sub_08014cc0, 0x08014cc0
	.set sub_08016430, 0x08016430
	.set sub_080164e8, 0x080164e8
	.set sub_08016990, 0x08016990
	.set sub_08016dfc, 0x08016dfc
	.set sub_08016e04, 0x08016e04
	.set sub_081c0080, 0x081c0080
	.set sub_081c0088, 0x081c0088
	.global Overlay_080133c4
Overlay_080133c4:
	push	{r5, lr}
	ldr	r5, [pc, #76]
	movs	r4, #0
	strh	r4, [r5, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #68]
	ldr	r1, [pc, #68]
	ldr	r2, [pc, #72]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #68]
	ldr	r2, [pc, #72]
	ldr	r0, [pc, #72]
	str	r3, [r2, #0]
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r1, [pc, #64]
	adds	r2, #14
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	subs	r3, #208
	strh	r4, [r3, #0]
	movs	r2, #195
	ldr	r3, [pc, #52]
	lsls	r2, r2, #8
	adds	r2, #255
	strh	r2, [r3, #0]
	movs	r2, #192
	lsls	r2, r2, #6
	adds	r2, #1
	adds	r3, #206
	strh	r2, [r3, #0]
	movs	r3, #1
	strh	r3, [r5, #0]
	pop	{r5, pc}
	.4byte 0x04000208
	.4byte 0x080006b8
	.4byte 0x03000100
	.4byte 0x84000400
	.4byte 0x03000100
	.4byte 0x03007ffc
	.4byte 0x080178b4
	.4byte 0x030001e4
	.2byte 0x0132
	.2byte 0x0400
	push	{r5, r6, lr}
	adds	r5, r1, #0
	adds	r1, r2, #0
	cmp	r0, #13
	bhi.n	.L_0801349c
	ldr	r3, [pc, #92]
	ldrh	r2, [r3, #0]
	adds	r6, r2, #0
	strh	r3, [r3, #0]
	movs	r2, #1
	ldr	r4, [pc, #84]
	lsls	r2, r0
	ldrh	r3, [r4, #0]
	bics	r3, r2
	cmp	r1, #0
	beq.n	.L_0801345a
	orrs	r3, r2
.L_0801345a:
	strh	r3, [r4, #0]
	cmp	r0, #2
	bhi.n	.L_08013484
	movs	r4, #8
	lsls	r4, r0
	mvns	r2, r4
	cmp	r0, #2
	bne.n	.L_08013472
	lsls	r3, r5, #8
	orrs	r4, r3
	movs	r3, #255
	ands	r2, r3
.L_08013472:
	movs	r5, #128
	lsls	r5, r5, #19
	adds	r5, #4
	ldrh	r3, [r5, #0]
	ands	r3, r2
	cmp	r1, #0
	beq.n	.L_08013482
	orrs	r3, r4
.L_08013482:
	strh	r3, [r5, #0]
.L_08013484:
	cmp	r1, #0
	beq.n	.L_08013490
	ldr	r2, [pc, #28]
	lsls	r3, r0, #2
	str	r1, [r2, r3]
	b.n	.L_08013498
.L_08013490:
	ldr	r1, [pc, #20]
	ldr	r3, [pc, #24]
	lsls	r2, r0, #2
	str	r3, [r1, r2]
.L_08013498:
	ldr	r3, [pc, #4]
	strh	r6, [r3, #0]
.L_0801349c:
	pop	{r5, r6, pc}
	movs	r0, r0
	.4byte 0x04000208
	.4byte 0x04000200
	.4byte 0x030001e4
	.2byte 0x33c1
	.2byte 0x0801
.L_080134b0:
	push	{lr}
	ldr	r3, [pc, #148]
	ldrb	r3, [r3, #2]
	cmp	r3, #0
	beq.n	.L_080134d8
	ldr	r3, [pc, #144]
	movs	r2, #255
	ldrh	r1, [r3, #0]
	lsls	r2, r2, #8
	adds	r2, #254
	ands	r2, r1
	strh	r2, [r3, #0]
	adds	r0, r3, #0
	movs	r1, #1
.L_080134cc:
	ldrh	r2, [r0, #0]
	adds	r3, r1, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080134cc
	b.n	.L_08013544
.L_080134d8:
	ldr	r3, [pc, #116]
	ldrh	r2, [r3, #0]
	adds	r4, r2, #0
	strh	r3, [r3, #0]
	ldr	r1, [pc, #104]
	movs	r3, #255
	ldrh	r2, [r1, #0]
	lsls	r3, r3, #8
	adds	r3, #254
	ands	r3, r2
	strh	r3, [r1, #0]
	ldr	r3, [pc, #100]
	ldr	r1, [pc, #84]
	ldr	r3, [r3, #0]
	ldrb	r2, [r1, #0]
	strb	r3, [r1, #0]
	ldrb	r3, [r1, #3]
	movs	r3, #1
	strb	r3, [r1, #3]
	ldr	r3, [pc, #88]
	ldr	r0, [r3, #0]
	ldrb	r2, [r0, #4]
	adds	r3, r2, #0
	cmp	r3, #1
	bhi.n	.L_0801350e
	ldrb	r3, [r0, #11]
	b.n	.L_08013512
.L_0801350e:
	adds	r3, r2, #0
	adds	r3, #255
.L_08013512:
	ldrb	r2, [r1, #1]
	strb	r3, [r1, #1]
	ldr	r3, [pc, #56]
	strh	r4, [r3, #0]
	bl	sub_081c0088
	bl	sub_08000630
	cmp	r0, #0
	bne.n	sub_0801352e
	ldr	r3, [pc, #52]
	movs	r0, #8
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4b06
	ldr	r0, [pc, #24]
	ldrb	r2, [r3, #3]
	movs	r2, #0
	strb	r2, [r3, #3]
	movs	r1, #1
.L_0801353a:
	ldrh	r2, [r0, #0]
	adds	r3, r1, #0
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0801353a
.L_08013544:
	pop	{pc}
	movs	r0, r0
	.4byte 0x03001138
	.4byte 0x0300121c
	.4byte 0x04000208
	.4byte 0x03001100
	.4byte 0x03007ff0
	.2byte 0x0d40
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #8
	movs	r1, #0
	str	r0, [sp, #4]
	str	r1, [sp, #0]
	cmp	r1, r0
	bcc.n	.L_0801357c
	b.n	.L_08013880
.L_0801357c:
	ldr	r2, [pc, #224]
	ldr	r3, [pc, #228]
	mov	fp, r2
	mov	sl, r1
	mov	r9, r3
.L_08013586:
	ldr	r5, [pc, #224]
	movs	r1, #1
	movs	r0, #144
	strb	r1, [r5, #0]
	lsls	r0, r0, #3
	bl	sub_080147d8
	movs	r3, #0
	strb	r3, [r5, #0]
	ldr	r3, [pc, #208]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080135b0
	movs	r1, #128
	lsls	r1, r1, #3
	movs	r0, #80
	bl	sub_08014cc0
	bl	sub_080019f4
	b.n	.L_080135ba
.L_080135b0:
	ldr	r2, [pc, #188]
	strh	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #1
	strh	r3, [r2, #2]
.L_080135ba:
	ldr	r3, [pc, #184]
	movs	r2, #1
	strb	r2, [r3, #0]
	ldr	r3, [pc, #180]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08013606
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #6
	ldrh	r1, [r3, #0]
	cmp	r1, #159
	bls.n	.L_080135d8
	subs	r1, #160
	b.n	.L_080135da
.L_080135d8:
	adds	r1, #68
.L_080135da:
	ldr	r3, [pc, #160]
	ldr	r0, [pc, #160]
	ldrh	r3, [r3, #0]
	ldr	r2, [r0, #0]
	subs	r3, #1
	lsls	r3, r3, #8
	adds	r1, r1, r3
	cmp	r2, #0
	bne.n	.L_080135f2
	ldr	r3, [pc, #148]
	str	r2, [r3, #0]
	b.n	.L_080135f6
.L_080135f2:
	subs	r3, r2, #1
	str	r3, [r0, #0]
.L_080135f6:
	ldr	r2, [pc, #140]
	ldr	r3, [r2, #0]
	cmp	r3, r1
	bcs.n	.L_08013606
	str	r1, [r2, #0]
	ldr	r2, [pc, #124]
	movs	r3, #30
	str	r3, [r2, #0]
.L_08013606:
	ldr	r3, [pc, #128]
	ldrb	r3, [r3, #0]
	adds	r2, r3, #0
	cmp	r2, #0
	bne.n	.L_08013654
	ldr	r3, [pc, #120]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08013642
	mov	r1, fp
	ldr	r3, [r1, #0]
	cmp	r3, #0
	beq.n	.L_08013626
	mov	r3, r9
	strh	r2, [r3, #0]
	b.n	.L_08013642
.L_08013626:
	mov	r1, r9
	ldrh	r3, [r1, #0]
	mov	r2, r9
	adds	r3, #1
	strh	r3, [r2, #0]
	movs	r3, #168
	ldrh	r2, [r2, #0]
	lsls	r3, r3, #6
	adds	r3, #48
	cmp	r2, r3
	bls.n	.L_08013642
	ldr	r3, [pc, #80]
	movs	r1, #1
	strb	r1, [r3, #0]
.L_08013642:
	mov	r2, fp
	ldr	r3, [r2, #0]
	movs	r1, #193
	lsls	r1, r1, #2
	cmp	r3, r1
	bne.n	.L_08013654
	ldr	r3, [pc, #64]
	movs	r2, #1
	strb	r2, [r3, #0]
.L_08013654:
	ldr	r3, [pc, #60]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080136fa
	ldr	r5, [pc, #56]
	b.n	.L_080136d2
	.4byte 0x03001150
	.4byte 0x03001218
	.4byte 0x03001108
	.4byte 0x0300120c
	.4byte 0x03001248
	.4byte 0x03001230
	.4byte 0x0300123c
	.4byte 0x030011d4
	.4byte 0x03001140
	.4byte 0x03001184
	.4byte 0x03001180
	.4byte 0x03001200
	.4byte 0x030011d0
	.4byte 0x03001238
	.2byte 0x1214
	.2byte 0x0300
.L_0801369c:
	mov	r1, fp
	ldr	r3, [r1, #0]
	cmp	r3, #12
	bne.n	.L_080136fa
	movs	r2, #1
	strb	r2, [r5, #0]
.L_080136a8:
	bl	.L_080134b0
	bl	.L_080138b4
	ldr	r2, [pc, #280]
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_080136d2
	movs	r3, #0
	strb	r3, [r2, #0]
	ldr	r2, [pc, #272]
	ldr	r3, [pc, #276]
	str	r3, [r2, #0]
	bl	sub_081c0080
	movs	r4, #128
	lsls	r4, r4, #20
	ldr	r3, [pc, #268]
	mov	r1, sl
	strh	r1, [r3, #0]
	bx	r4
.L_080136d2:
	ldrb	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_0801369c
	ldr	r0, [pc, #256]
	movs	r2, #7
	ldr	r3, [r0, #12]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_080136fa
	ldr	r1, [r0, #0]
	movs	r3, #240
	ands	r1, r3
	cmp	r1, #0
	bne.n	.L_080136fa
	ldr	r3, [r0, #12]
	movs	r2, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_080136a8
	strb	r1, [r5, #0]
.L_080136fa:
	ldr	r2, [pc, #228]
	ldr	r1, [pc, #228]
	ldrh	r3, [r2, #0]
	strh	r3, [r1, #0]
	mov	r3, sl
	strh	r3, [r2, #0]
	bl	.L_080134b0
	movs	r0, #80
	bl	sub_0801314c
	bl	sub_08013ffc
	ldr	r2, [pc, #208]
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
	ldr	r2, [pc, #204]
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
	bl	.L_080138b4
	ldr	r3, [pc, #196]
	ldrh	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_08013740
	bl	sub_08016430
	ldr	r2, [pc, #188]
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_08013740
	movs	r3, #1
	strb	r3, [r2, #8]
.L_08013740:
	ldr	r1, [pc, #180]
	ldrb	r3, [r1, #0]
	cmp	r3, #0
	bne.n	.L_0801374a
	b.n	.L_08013850
.L_0801374a:
	ldr	r3, [pc, #176]
	ldrb	r3, [r3, #0]
	adds	r2, r3, #0
	cmp	r2, #0
	beq.n	.L_08013756
	b.n	.L_08013850
.L_08013756:
	movs	r4, #128
	lsls	r4, r4, #19
	ldrh	r3, [r4, #0]
	movs	r0, #160
	lsls	r3, r3, #16
	lsls	r0, r0, #19
	asrs	r7, r3, #16
	ldrh	r3, [r0, #0]
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	mov	r8, r3
	ldrb	r3, [r1, #0]
	cmp	r3, #1
	bne.n	.L_0801384a
	strh	r2, [r4, #0]
	movs	r3, #254
	lsls	r3, r3, #7
	adds	r3, #255
	strh	r3, [r0, #0]
	movs	r5, #9
.L_0801377e:
	subs	r5, #1
	bl	.L_080134b0
	cmp	r5, #0
	bge.n	.L_0801377e
	mov	r1, fp
	ldr	r3, [r1, #0]
	cmp	r3, #0
	beq.n	.L_0801379c
	ldr	r5, [pc, #72]
.L_08013792:
	bl	.L_080134b0
	ldr	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L_08013792
.L_0801379c:
	ldr	r6, [pc, #96]
	ldr	r3, [pc, #40]
	strh	r3, [r6, #0]
	movs	r3, #195
	ldr	r5, [pc, #92]
	lsls	r3, r3, #8
	adds	r3, #4
	strh	r3, [r5, #0]
	bl	sub_08016dfc
	svc	3
	bl	sub_08016e04
	movs	r3, #192
	lsls	r3, r3, #8
	adds	r3, #15
	strh	r3, [r5, #0]
	mov	r2, sl
	strh	r2, [r6, #0]
	movs	r5, #9
	b.n	.L_08013808
	movs	r0, r0
	.4byte 0x00000001
	.4byte 0x030011c0
	.4byte 0x03007800
	.4byte 0x19670704
	.4byte 0x04000208
	.4byte 0x03001150
	.4byte 0x030011d4
	.4byte 0x030011d8
	.4byte 0x0300122c
	.4byte 0x0300117c
	.4byte 0x030011b8
	.4byte 0x02005360
	.4byte 0x030011d0
	.4byte 0x03001180
	.4byte 0x02003000
	.2byte 0x0132
	.2byte 0x0400
.L_08013808:
	subs	r5, #1
	bl	.L_080134b0
	cmp	r5, #0
	bge.n	.L_08013808
	mov	r1, fp
	ldr	r3, [r1, #0]
	cmp	r3, #0
	beq.n	.L_08013826
	ldr	r5, [pc, #116]
.L_0801381c:
	bl	.L_080134b0
	ldr	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L_0801381c
.L_08013826:
	lsls	r3, r7, #16
	movs	r2, #128
	lsrs	r3, r3, #16
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	mov	r2, r8
	lsls	r3, r2, #16
	movs	r2, #160
	lsrs	r3, r3, #16
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	ldr	r2, [pc, #84]
	movs	r3, #0
	strb	r3, [r2, #0]
	mov	r1, r9
	mov	r3, sl
	strh	r3, [r1, #0]
	b.n	.L_08013850
.L_0801384a:
	ldrb	r3, [r1, #0]
	adds	r3, #255
	strb	r3, [r1, #0]
.L_08013850:
	ldr	r2, [pc, #68]
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_08013872
	movs	r3, #0
	strb	r3, [r2, #0]
	ldr	r2, [pc, #60]
	ldr	r3, [pc, #64]
	str	r3, [r2, #0]
	bl	sub_081c0080
	movs	r4, #128
	lsls	r4, r4, #20
	ldr	r3, [pc, #56]
	mov	r2, sl
	strh	r2, [r3, #0]
	bx	r4
.L_08013872:
	ldr	r3, [sp, #0]
	ldr	r1, [sp, #4]
	adds	r3, #1
	str	r3, [sp, #0]
	cmp	r3, r1
	bcs.n	.L_08013880
	b.n	.L_08013586
.L_08013880:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x03001150
	.4byte 0x030011d0
	.4byte 0x030011c0
	.4byte 0x03007800
	.4byte 0x19670704
	.2byte 0x0208
	.2byte 0x0400
	ldr	r2, [pc, #4]
	movs	r3, #19
	str	r3, [r2, #32]
	bx	lr
	.2byte 0x1150
	.2byte 0x0300
.L_080138b4:
	push	{r5, lr}
	ldr	r2, [pc, #224]
	movs	r5, #0
	ldr	r4, [r2, #0]
	ldr	r3, [r2, #32]
	cmp	r3, #0
	bgt.n	.L_080138d6
	adds	r1, r4, #0
	str	r4, [r2, #12]
	cmp	r3, #0
	bne.n	.L_080138d0
	movs	r3, #6
	str	r3, [r2, #32]
	b.n	.L_080138da
.L_080138d0:
	movs	r3, #19
	str	r3, [r2, #32]
	b.n	.L_080138da
.L_080138d6:
	movs	r1, #0
	str	r1, [r2, #12]
.L_080138da:
	cmp	r1, #0
	beq.n	.L_08013984
	movs	r3, #64
	ands	r3, r1
	movs	r2, #0
	cmp	r3, #0
	beq.n	.L_080138ea
	movs	r2, #1
.L_080138ea:
	movs	r3, #128
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080138f4
	adds	r2, #1
.L_080138f4:
	movs	r3, #32
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_080138fe
	adds	r2, #1
.L_080138fe:
	movs	r3, #16
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_08013908
	adds	r2, #1
.L_08013908:
	ldr	r0, [pc, #140]
	str	r1, [r0, #16]
	cmp	r2, #1
	beq.n	.L_08013930
	cmp	r2, #1
	bcc.n	.L_0801392a
	cmp	r2, #2
	beq.n	.L_08013938
	cmp	r2, #3
	beq.n	.L_08013958
	movs	r3, #48
	str	r3, [r0, #20]
	movs	r2, #255
	ldr	r3, [r0, #16]
	lsls	r2, r2, #8
	adds	r2, #15
	b.n	.L_0801397e
.L_0801392a:
	movs	r3, #48
	str	r3, [r0, #20]
	b.n	.L_08013988
.L_08013930:
	movs	r3, #240
	ands	r1, r3
	str	r1, [r0, #20]
	b.n	.L_08013988
.L_08013938:
	ldr	r3, [r0, #20]
	ldr	r2, [r0, #16]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08013946
	movs	r3, #48
	str	r3, [r0, #20]
.L_08013946:
	ldr	r3, [r0, #20]
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	eors	r3, r2
	ldr	r2, [r0, #16]
	ands	r2, r3
	str	r2, [r0, #16]
	b.n	.L_08013988
.L_08013958:
	ldr	r3, [r0, #20]
	movs	r2, #48
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08013964
	movs	r5, #48
.L_08013964:
	ldr	r3, [r0, #20]
	movs	r2, #192
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08013970
	movs	r5, #192
.L_08013970:
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	eors	r2, r5
	ands	r1, r2
	str	r1, [r0, #20]
	ldr	r3, [r0, #16]
.L_0801397e:
	ands	r3, r2
	str	r3, [r0, #16]
	b.n	.L_08013988
.L_08013984:
	ldr	r3, [pc, #16]
	str	r1, [r3, #16]
.L_08013988:
	ldr	r3, [pc, #12]
	adds	r2, r4, #0
	ldr	r1, [r3, #24]
	bics	r2, r1
	str	r2, [r3, #4]
	str	r4, [r3, #24]
	pop	{r5, pc}
	movs	r0, r0
	.2byte 0x1150
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #176
	ldrh	r1, [r2, #10]
	movs	r3, #197
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r1
	strh	r3, [r2, #10]
	movs	r3, #254
	ldrh	r1, [r2, #10]
	lsls	r3, r3, #7
	adds	r3, #255
	ands	r3, r1
	strh	r3, [r2, #10]
	ldrh	r3, [r2, #10]
	ldr	r3, [pc, #168]
	ldrh	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080139d6
	ldr	r0, [pc, #164]
	ldr	r1, [pc, #164]
	ldr	r5, [pc, #168]
	bl	sub_080164e8
	strh	r0, [r5, #0]
	bl	sub_08016990
.L_080139d6:
	bl	sub_080005e8
	ldr	r3, [pc, #156]
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_080139e6
	bl	sub_08013de4
.L_080139e6:
	ldr	r2, [pc, #148]
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_08013a26
	movs	r3, #0
	strb	r3, [r2, #0]
	ldr	r2, [pc, #140]
	ldrh	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_08013a16
	movs	r1, #192
	lsls	r1, r1, #18
	adds	r2, r3, #0
	movs	r4, #132
	movs	r3, #128
	ldr	r0, [r1, #80]
	lsls	r4, r4, #24
	lsls	r3, r3, #19
	movs	r1, #224
	adds	r3, #212
	lsls	r1, r1, #19
	adds	r2, r2, r4
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
.L_08013a16:
	movs	r1, #128
	lsls	r1, r1, #19
	ldr	r0, [pc, #104]
	adds	r1, #16
	ldmia	r0!, {r2, r3, r4, r5}
	stmia	r1!, {r2, r3, r4, r5}
	bl	sub_08013d84
.L_08013a26:
	ldr	r1, [pc, #96]
	ldr	r2, [r1, #0]
	cmp	r2, #0
	beq.n	sub_08013a36
	movs	r3, #0
	str	r3, [r1, #0]
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x20c8
	lsls	r0, r0, #4
	bl	sub_08014840
	ldr	r3, [pc, #76]
	ldr	r1, [pc, #76]
	ldrh	r2, [r3, #0]
	ldr	r3, [pc, #28]
	adds	r0, r3, #0
	ldr	r3, [r1, #0]
	eors	r0, r2
	adds	r2, r0, #0
	bics	r2, r3
	str	r2, [r1, #4]
	ldr	r3, [r1, #28]
	orrs	r3, r2
	str	r3, [r1, #28]
	str	r0, [r1, #0]
	cmp	r0, #0
	bne.n	.L_08013a94
	movs	r3, #19
	b.n	.L_08013aaa
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0x030011b8
	.4byte 0x02003a70
	.4byte 0x02003870
	.4byte 0x0300124c
	.4byte 0x03001178
	.4byte 0x03001230
	.4byte 0x03001248
	.4byte 0x03001120
	.4byte 0x030011f8
	.4byte 0x04000130
	.2byte 0x1150
	.2byte 0x0300
.L_08013a94:
	ldr	r3, [r1, #8]
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	eors	r3, r2
	ldr	r2, [r1, #0]
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L_08013ab0
	movs	r3, #1
	negs	r3, r3
.L_08013aaa:
	str	r3, [r1, #32]
	str	r0, [r1, #12]
	b.n	.L_08013abc
.L_08013ab0:
	ldr	r3, [r1, #32]
	cmp	r3, #0
	ble.n	.L_08013abc
	ldr	r3, [r1, #32]
	subs	r3, #1
	str	r3, [r1, #32]
.L_08013abc:
	ldr	r3, [pc, #84]
	ldr	r7, [pc, #88]
	str	r0, [r3, #8]
	ldr	r6, [pc, #88]
	ldr	r5, [r7, #0]
	ldrb	r3, [r6, #0]
	mov	ip, r3
	lsls	r3, r5, #24
	lsrs	r3, r3, #24
	cmp	ip, r3
	beq.n	.L_08013afa
	ldrb	r3, [r6, #3]
	cmp	r3, #0
	bne.n	.L_08013afa
	ldr	r3, [pc, #68]
	ldrb	r2, [r6, #1]
	ldr	r3, [r3, #0]
	ldrb	r3, [r3, #4]
	strb	r3, [r6, #1]
	bl	sub_081c0088
	bl	sub_08000630
	cmp	r0, #0
	bne.n	sub_08013af6
	ldr	r3, [pc, #52]
	movs	r0, #8
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x7833
	strb	r5, [r6, #0]
.L_08013afa:
	adds	r3, r5, #1
	str	r3, [r7, #0]
	ldr	r2, [pc, #40]
	ldrh	r3, [r2, #0]
	adds	r3, #1
	strh	r3, [r2, #0]
	ldr	r2, [pc, #36]
	ldr	r3, [pc, #4]
	strh	r3, [r2, #0]
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x00000001
	.4byte 0x03001150
	.4byte 0x03001100
	.4byte 0x03001138
	.4byte 0x03007ff0
	.4byte 0x03000d40
	.4byte 0x030011d4
	.2byte 0x121c
	.2byte 0x0300
	push	{lr}
	movs	r0, #0
	pop	{pc}
