.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
	.set sub_08002096, 0x08002096
	.set sub_08013164, 0x08013164
	.set sub_08013560, 0x08013560
	.set sub_080138a8, 0x080138a8
	.set sub_080145a8, 0x080145a8
	.set sub_08014d78, 0x08014d78
	.set sub_08014dac, 0x08014dac
	.set sub_08016ca4, 0x08016ca4
	.set sub_08038018, 0x08038018
	.set sub_08038038, 0x08038038
	.set sub_08038048, 0x08038048
	.set sub_08038060, 0x08038060
	.set sub_08038068, 0x08038068
	.set sub_08038070, 0x08038070
	.set sub_08038080, 0x08038080
	.set sub_08038090, 0x08038090
	.set sub_080380a8, 0x080380a8
	.set sub_080380b0, 0x080380b0
	.set sub_080380b8, 0x080380b8
	.set sub_080380d8, 0x080380d8
	.set sub_08038120, 0x08038120
	.set sub_08038260, 0x08038260
	.set sub_08038268, 0x08038268
	.set sub_08038270, 0x08038270
	.set sub_080ad008, 0x080ad008
	.set sub_080ad078, 0x080ad078
	.set sub_080ad100, 0x080ad100
	.set sub_080ad150, 0x080ad150
	.set sub_080ad158, 0x080ad158
	.set sub_080ad160, 0x080ad160
	.set sub_080f811c, 0x080f811c
	.set sub_080f815c, 0x080f815c
	.set sub_080f8a44, 0x080f8a44
	.set sub_080f8ab4, 0x080f8ab4
	.set sub_08100e28, 0x08100e28
	.set sub_08101860, 0x08101860
	.set sub_08101a54, 0x08101a54
	.set sub_08103168, 0x08103168
	.set sub_081043c2, 0x081043c2
	.set sub_081047fc, 0x081047fc
	.set sub_0810498c, 0x0810498c
	.set sub_08104b18, 0x08104b18
	.set sub_08104ba8, 0x08104ba8
	.set sub_08104e38, 0x08104e38
	.set sub_08105300, 0x08105300
	.set sub_08105350, 0x08105350
	.set sub_081c0010, 0x081c0010
	.global Overlay_08103218
Overlay_08103218:
.L_08103218:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #168
	mov	r9, r0
	adds	r0, r3, #0
	ldr	r5, [sp, #200]
	ldr	r6, [sp, #204]
	str	r1, [sp, #60]
	str	r2, [sp, #56]
	str	r3, [sp, #52]
	bl	sub_08016ca4
	movs	r3, #192
	str	r0, [sp, #48]
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r4, [r3, #0]
	movs	r1, #150
	lsls	r1, r1, #2
	adds	r0, r4, #2
	adds	r3, r5, r1
	ldrb	r2, [r0, r3]
	lsls	r5, r5, #1
	str	r2, [sp, #40]
	mov	lr, r0
	ldrb	r3, [r4, r3]
	movs	r2, #128
	str	r3, [sp, #36]
	movs	r3, #182
	lsls	r3, r3, #1
	mov	ip, r3
	add	r5, ip
	ldrh	r0, [r4, r5]
	lsls	r2, r2, #8
	adds	r3, r2, #0
	ands	r3, r0
	lsls	r3, r3, #16
	lsrs	r3, r3, #16
	str	r3, [sp, #32]
	adds	r1, r6, r1
	mov	r0, lr
	ldrb	r0, [r0, r1]
	lsls	r6, r6, #1
	str	r0, [sp, #28]
	add	r6, ip
	ldrb	r1, [r4, r1]
	movs	r5, #166
	str	r1, [sp, #24]
	lsls	r5, r5, #1
	ldrh	r3, [r4, r6]
	adds	r0, r5, #0
	ands	r2, r3
	lsls	r2, r2, #16
	lsrs	r2, r2, #16
	str	r2, [sp, #20]
	bl	sub_08014d78
	ldr	r3, [pc, #916]
	ldr	r1, [sp, #48]
	str	r0, [sp, #44]
	adds	r2, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9935
	cmp	r1, #0
	beq.n	.L_081032a8
	b.n	.L_081033ca
.L_081032a8:
	ldr	r2, [sp, #208]
	cmp	r2, #3
	bne.n	.L_08103336
	ldr	r1, [sp, #48]
	ldr	r2, [sp, #60]
	movs	r3, #52
	ldrsh	r0, [r1, r3]
	ldr	r3, [sp, #56]
	lsls	r7, r2, #3
	lsls	r3, r3, #3
	mov	fp, r3
	adds	r5, r7, #0
	movs	r1, #56
	adds	r5, #72
	add	r1, fp
	str	r1, [sp, #0]
	mov	sl, r1
	mov	r2, r9
	adds	r3, r5, #0
	movs	r1, #4
	bl	sub_080380b0
	ldr	r3, [sp, #48]
	movs	r1, #64
	add	r1, fp
	movs	r2, #54
	ldrsh	r0, [r3, r2]
	mov	r8, r1
	str	r1, [sp, #0]
	mov	r2, r9
	adds	r3, r5, #0
	movs	r1, #4
	bl	sub_080380b0
	ldr	r3, [sp, #48]
	subs	r5, #40
	mov	r1, sl
	movs	r2, #56
	ldrsh	r0, [r3, r2]
	str	r1, [sp, #0]
	mov	r2, r9
	adds	r3, r5, #0
	movs	r1, #4
	bl	sub_080380b0
	ldr	r3, [sp, #48]
	mov	r1, r8
	movs	r2, #58
	ldrsh	r0, [r3, r2]
	str	r1, [sp, #0]
	adds	r3, r5, #0
	movs	r1, #4
	mov	r2, r9
	bl	sub_080380b0
	ldr	r5, [pc, #792]
	adds	r6, r7, #0
	adds	r6, #64
	adds	r0, r5, #0
	mov	r1, r9
	adds	r2, r6, #0
	mov	r3, sl
	bl	sub_08038090
	adds	r0, r5, #0
	mov	r1, r9
	adds	r2, r6, #0
	mov	r3, r8
	bl	sub_08038090
	b.n	.L_08103370
.L_08103336:
	ldr	r3, [sp, #48]
	ldr	r1, [sp, #60]
	movs	r2, #56
	ldrsh	r0, [r3, r2]
	ldr	r2, [sp, #56]
	lsls	r7, r1, #3
	lsls	r2, r2, #3
	mov	fp, r2
	adds	r5, r7, #0
	mov	r3, fp
	adds	r5, #32
	adds	r3, #56
	str	r3, [sp, #0]
	movs	r1, #4
	adds	r3, r5, #0
	mov	r2, r9
	bl	sub_080380b0
	ldr	r1, [sp, #48]
	mov	r2, r9
	movs	r3, #58
	ldrsh	r0, [r1, r3]
	mov	r3, fp
	adds	r3, #64
	str	r3, [sp, #0]
	movs	r1, #4
	adds	r3, r5, #0
	bl	sub_080380b0
.L_08103370:
	ldr	r2, [sp, #48]
	adds	r5, r7, #0
	mov	r3, fp
	adds	r5, #48
	adds	r3, #72
	ldrh	r0, [r2, #60]
	movs	r1, #3
	str	r3, [sp, #0]
	mov	r2, r9
	adds	r3, r5, #0
	bl	sub_080380b0
	ldr	r3, [sp, #48]
	movs	r1, #3
	ldrh	r0, [r3, #62]
	mov	r3, fp
	adds	r3, #80
	str	r3, [sp, #0]
	mov	r2, r9
	adds	r3, r5, #0
	bl	sub_080380b0
	ldr	r3, [sp, #48]
	movs	r1, #3
	adds	r3, #64
	ldrh	r0, [r3, #0]
	mov	r3, fp
	adds	r3, #88
	str	r3, [sp, #0]
	mov	r2, r9
	adds	r3, r5, #0
	bl	sub_080380b0
	ldr	r3, [sp, #48]
	mov	r2, fp
	adds	r3, #66
	ldrb	r0, [r3, #0]
	adds	r2, #96
	adds	r3, r7, #0
	str	r2, [sp, #0]
	adds	r3, #56
	movs	r1, #2
	mov	r2, r9
	bl	sub_080380b0
.L_081033ca:
	ldr	r0, [sp, #208]
	cmp	r0, #1
	beq.n	.L_08103402
	cmp	r0, #1
	bgt.n	.L_081033da
	cmp	r0, #0
	beq.n	.L_081033e6
	b.n	.L_0810346e
.L_081033da:
	ldr	r1, [sp, #208]
	cmp	r1, #2
	beq.n	.L_08103416
	cmp	r1, #4
	beq.n	.L_0810344e
	b.n	.L_0810346e
.L_081033e6:
	ldr	r2, [sp, #24]
	movs	r5, #31
	ands	r5, r2
	ldr	r1, [sp, #28]
	adds	r2, r5, #0
	ldr	r0, [sp, #52]
	bl	sub_080ad150
	adds	r2, r5, #0
	ldr	r0, [sp, #52]
	ldr	r1, [sp, #28]
	bl	sub_080ad158
	b.n	.L_0810346e
.L_08103402:
	ldr	r0, [sp, #36]
	movs	r3, #31
	ands	r0, r3
	str	r0, [sp, #36]
	ldr	r1, [sp, #40]
	ldr	r0, [sp, #52]
	ldr	r2, [sp, #36]
	bl	sub_080ad160
	b.n	.L_0810346e
.L_08103416:
	ldr	r1, [sp, #32]
	cmp	r1, #0
	beq.n	.L_0810342c
	ldr	r2, [sp, #36]
	movs	r3, #31
	ands	r2, r3
	ldr	r0, [sp, #52]
	ldr	r1, [sp, #40]
	str	r2, [sp, #36]
	bl	sub_080ad160
.L_0810342c:
	ldr	r3, [sp, #24]
	movs	r5, #31
	ands	r5, r3
	ldr	r0, [sp, #52]
	ldr	r1, [sp, #28]
	adds	r2, r5, #0
	bl	sub_080ad150
	ldr	r0, [sp, #20]
	cmp	r0, #0
	beq.n	.L_0810346e
	ldr	r0, [sp, #52]
	ldr	r1, [sp, #28]
	adds	r2, r5, #0
	bl	sub_080ad158
	b.n	.L_0810346e
.L_0810344e:
	ldr	r1, [sp, #24]
	movs	r5, #31
	ands	r5, r1
	adds	r2, r5, #0
	ldr	r0, [sp, #52]
	ldr	r1, [sp, #28]
	bl	sub_080ad150
	ldr	r2, [sp, #20]
	cmp	r2, #0
	beq.n	.L_0810346e
	ldr	r0, [sp, #52]
	ldr	r1, [sp, #28]
	adds	r2, r5, #0
	bl	sub_080ad158
.L_0810346e:
	ldr	r0, [sp, #52]
	bl	sub_080ad008
	ldr	r0, [sp, #52]
	bl	sub_08016ca4
	ldr	r3, [sp, #212]
	str	r0, [sp, #48]
	cmp	r3, #0
	bne.n	.L_08103530
	ldr	r0, [sp, #60]
	ldr	r1, [sp, #56]
	lsls	r7, r0, #3
	lsls	r1, r1, #3
	mov	r8, r1
	adds	r6, r7, #0
	adds	r6, #40
	mov	r5, r8
	ldr	r0, [sp, #48]
	adds	r5, #16
	mov	r1, r9
	adds	r2, r6, #0
	mov	r3, r8
	bl	sub_08038090
	adds	r3, r5, #0
	ldr	r0, [pc, #400]
	mov	r1, r9
	adds	r2, r6, #0
	bl	sub_08038090
	ldr	r2, [sp, #48]
	adds	r3, r7, #0
	ldrb	r0, [r2, #15]
	adds	r3, #88
	movs	r1, #2
	mov	r2, r9
	str	r5, [sp, #0]
	bl	sub_080380b0
	ldr	r5, [pc, #376]
	mov	r3, r8
	adds	r0, r5, #0
	adds	r3, #56
	mov	r1, r9
	adds	r2, r7, #0
	bl	sub_08038080
	mov	r3, r8
	adds	r0, r5, #1
	adds	r3, #64
	mov	r1, r9
	adds	r2, r7, #0
	bl	sub_08038080
	mov	r3, r8
	adds	r0, r5, #2
	adds	r3, #72
	mov	r1, r9
	adds	r2, r7, #0
	bl	sub_08038080
	mov	r3, r8
	adds	r0, r5, #3
	adds	r3, #80
	mov	r1, r9
	adds	r2, r7, #0
	bl	sub_08038080
	mov	r3, r8
	adds	r0, r5, #4
	adds	r3, #88
	mov	r1, r9
	adds	r2, r7, #0
	bl	sub_08038080
	adds	r5, #5
	mov	r3, r8
	adds	r3, #96
	adds	r0, r5, #0
	mov	r1, r9
	adds	r2, r7, #0
	bl	sub_08038080
	ldr	r0, [sp, #44]
	movs	r1, #42
	adds	r1, #255
	adds	r3, r0, r1
	ldrb	r0, [r3, #0]
	ldr	r3, [pc, #280]
	mov	r1, r9
	adds	r0, r0, r3
	mov	r3, r8
	adds	r3, #32
	adds	r2, r7, #0
	bl	sub_08038080
.L_08103530:
	ldr	r2, [sp, #212]
	cmp	r2, #0
	beq.n	.L_08103538
	b.n	.L_081037f6
.L_08103538:
	ldr	r0, [sp, #44]
	movs	r3, #42
	ldr	r1, [sp, #48]
	adds	r3, #255
	adds	r6, r0, r3
	adds	r5, r1, r3
	ldrb	r2, [r6, #0]
	ldrb	r3, [r5, #0]
	mov	ip, r2
	cmp	ip, r3
	beq.n	.L_08103580
	ldr	r2, [sp, #60]
	ldr	r1, [sp, #56]
	ldr	r3, [pc, #232]
	ldrb	r0, [r5, #0]
	lsls	r7, r2, #3
	adds	r0, r0, r3
	lsls	r3, r1, #3
	adds	r3, #48
	mov	r1, r9
	adds	r2, r7, #0
	bl	sub_08038080
	ldr	r3, [sp, #212]
	ldr	r2, [sp, #60]
	movs	r1, #242
	lsls	r1, r1, #8
	adds	r2, #2
	str	r3, [sp, #0]
	adds	r1, #150
	movs	r3, #5
	mov	r0, r9
	bl	sub_08038270
	ldrb	r2, [r6, #0]
	ldrb	r3, [r5, #0]
.L_08103580:
	ldr	r0, [sp, #60]
	mov	ip, r2
	mov	r8, r0
	cmp	ip, r3
	beq.n	.L_0810358e
	adds	r0, #5
	mov	r8, r0
.L_0810358e:
	ldr	r7, [sp, #56]
	movs	r6, #0
	adds	r7, #5
	mov	sl, r6
.L_08103596:
	asrs	r6, r6, #24
	movs	r2, #160
	lsls	r5, r6, #1
	lsls	r2, r2, #7
	adds	r2, #1
	add	r5, r8
	mov	r3, sl
	adds	r1, r6, r2
	str	r3, [sp, #0]
	adds	r2, r5, #0
	mov	r0, r9
	adds	r3, r7, #0
	bl	sub_08038270
	movs	r0, #142
	ldr	r2, [sp, #48]
	lsls	r0, r0, #1
	adds	r3, r6, r0
	ldrb	r1, [r2, r3]
	movs	r3, #240
	lsls	r3, r3, #8
	adds	r3, #48
	mov	r0, sl
	adds	r5, #1
	adds	r1, r1, r3
	str	r0, [sp, #0]
	adds	r2, r5, #0
	mov	r0, r9
	adds	r3, r7, #0
	bl	sub_08038270
	adds	r6, #1
	movs	r1, #192
	lsls	r6, r6, #24
	lsls	r1, r1, #18
	cmp	r6, r1
	ble.n	.L_08103596
	ldr	r3, [sp, #48]
	movs	r2, #56
	ldrsh	r0, [r3, r2]
	ldr	r2, [sp, #44]
	movs	r1, #56
	ldrsh	r3, [r2, r1]
	cmp	r0, r3
	beq.n	.L_0810364e
	ldr	r3, [sp, #60]
	ldr	r1, [sp, #56]
	lsls	r7, r3, #3
	lsls	r2, r1, #3
	adds	r5, r2, #0
	adds	r3, r7, #0
	adds	r3, #72
	movs	r1, #4
	mov	r2, r9
	adds	r5, #56
	str	r5, [sp, #0]
	bl	sub_080380b0
	ldr	r0, [sp, #48]
	movs	r3, #56
	ldrsh	r2, [r0, r3]
	ldr	r0, [sp, #44]
	movs	r1, #56
	ldrsh	r3, [r0, r1]
	cmp	r2, r3
	ble.n	.L_08103640
	adds	r1, r7, #0
	adds	r1, #62
	mov	r0, r9
	adds	r2, r5, #0
	movs	r3, #0
	bl	sub_08104ba8
	b.n	.L_0810364e
	movs	r0, r0
	.4byte 0x03000730
	.4byte 0x081059d8
	.4byte 0x081059d4
	.4byte 0x00000d0e
	.2byte 0x0b63
	.2byte 0x0000
.L_08103640:
	adds	r1, r7, #0
	adds	r1, #62
	mov	r0, r9
	adds	r2, r5, #0
	movs	r3, #1
	bl	sub_08104ba8
.L_0810364e:
	ldr	r2, [sp, #48]
	movs	r1, #58
	ldrsh	r0, [r2, r1]
	ldr	r2, [sp, #44]
	movs	r1, #58
	ldrsh	r3, [r2, r1]
	cmp	r0, r3
	beq.n	.L_081036a6
	ldr	r3, [sp, #60]
	ldr	r1, [sp, #56]
	lsls	r7, r3, #3
	lsls	r2, r1, #3
	adds	r5, r2, #0
	adds	r3, r7, #0
	adds	r3, #72
	movs	r1, #4
	mov	r2, r9
	adds	r5, #64
	str	r5, [sp, #0]
	bl	sub_080380b0
	ldr	r0, [sp, #48]
	movs	r3, #58
	ldrsh	r2, [r0, r3]
	ldr	r0, [sp, #44]
	movs	r1, #58
	ldrsh	r3, [r0, r1]
	cmp	r2, r3
	ble.n	.L_08103698
	adds	r1, r7, #0
	adds	r1, #62
	mov	r0, r9
	adds	r2, r5, #0
	movs	r3, #0
	bl	sub_08104ba8
	b.n	.L_081036a6
.L_08103698:
	adds	r1, r7, #0
	adds	r1, #62
	mov	r0, r9
	adds	r2, r5, #0
	movs	r3, #1
	bl	sub_08104ba8
.L_081036a6:
	ldr	r1, [sp, #48]
	ldr	r0, [sp, #44]
	ldrh	r2, [r1, #60]
	ldrh	r3, [r0, #60]
	cmp	r2, r3
	beq.n	.L_081036f8
	ldr	r1, [sp, #60]
	adds	r0, r2, #0
	lsls	r7, r1, #3
	ldr	r1, [sp, #56]
	adds	r3, r7, #0
	lsls	r2, r1, #3
	adds	r5, r2, #0
	adds	r3, #72
	mov	r2, r9
	adds	r5, #72
	movs	r1, #4
	str	r5, [sp, #0]
	bl	sub_080380b0
	ldr	r3, [sp, #48]
	ldr	r0, [sp, #44]
	ldrh	r2, [r3, #60]
	ldrh	r3, [r0, #60]
	cmp	r2, r3
	bls.n	.L_081036ea
	adds	r1, r7, #0
	adds	r1, #70
	mov	r0, r9
	adds	r2, r5, #0
	movs	r3, #0
	bl	sub_08104ba8
	b.n	.L_081036f8
.L_081036ea:
	adds	r1, r7, #0
	adds	r1, #70
	mov	r0, r9
	adds	r2, r5, #0
	movs	r3, #1
	bl	sub_08104ba8
.L_081036f8:
	ldr	r1, [sp, #48]
	ldr	r0, [sp, #44]
	ldrh	r2, [r1, #62]
	ldrh	r3, [r0, #62]
	cmp	r2, r3
	beq.n	.L_0810374a
	ldr	r1, [sp, #60]
	adds	r0, r2, #0
	lsls	r7, r1, #3
	ldr	r1, [sp, #56]
	adds	r3, r7, #0
	lsls	r2, r1, #3
	adds	r5, r2, #0
	adds	r3, #72
	mov	r2, r9
	adds	r5, #80
	movs	r1, #4
	str	r5, [sp, #0]
	bl	sub_080380b0
	ldr	r3, [sp, #48]
	ldr	r0, [sp, #44]
	ldrh	r2, [r3, #62]
	ldrh	r3, [r0, #62]
	cmp	r2, r3
	bls.n	.L_0810373c
	adds	r1, r7, #0
	adds	r1, #70
	mov	r0, r9
	adds	r2, r5, #0
	movs	r3, #0
	bl	sub_08104ba8
	b.n	.L_0810374a
.L_0810373c:
	adds	r1, r7, #0
	adds	r1, #70
	mov	r0, r9
	adds	r2, r5, #0
	movs	r3, #1
	bl	sub_08104ba8
.L_0810374a:
	ldr	r5, [sp, #48]
	ldr	r1, [sp, #44]
	adds	r5, #64
	adds	r1, #64
	ldrh	r2, [r5, #0]
	ldrh	r3, [r1, #0]
	mov	r8, r1
	cmp	r2, r3
	beq.n	.L_081037a0
	adds	r0, r2, #0
	ldr	r1, [sp, #56]
	ldr	r2, [sp, #60]
	lsls	r7, r2, #3
	lsls	r2, r1, #3
	adds	r6, r2, #0
	adds	r3, r7, #0
	adds	r3, #72
	mov	r2, r9
	adds	r6, #88
	movs	r1, #4
	str	r6, [sp, #0]
	bl	sub_080380b0
	mov	r0, r8
	ldrh	r2, [r5, #0]
	ldrh	r3, [r0, #0]
	cmp	r2, r3
	bls.n	.L_08103792
	adds	r1, r7, #0
	adds	r1, #70
	mov	r0, r9
	adds	r2, r6, #0
	movs	r3, #0
	bl	sub_08104ba8
	b.n	.L_081037a0
.L_08103792:
	adds	r1, r7, #0
	adds	r1, #70
	mov	r0, r9
	adds	r2, r6, #0
	movs	r3, #1
	bl	sub_08104ba8
.L_081037a0:
	ldr	r5, [sp, #48]
	ldr	r1, [sp, #44]
	adds	r5, #66
	adds	r1, #66
	ldrb	r2, [r5, #0]
	ldrb	r3, [r1, #0]
	mov	r8, r1
	cmp	r2, r3
	beq.n	.L_081037f6
	adds	r0, r2, #0
	ldr	r1, [sp, #56]
	ldr	r2, [sp, #60]
	lsls	r7, r2, #3
	lsls	r2, r1, #3
	adds	r6, r2, #0
	adds	r3, r7, #0
	adds	r3, #88
	mov	r2, r9
	adds	r6, #96
	movs	r1, #2
	str	r6, [sp, #0]
	bl	sub_080380b0
	mov	r0, r8
	ldrb	r2, [r5, #0]
	ldrb	r3, [r0, #0]
	cmp	r2, r3
	bls.n	.L_081037e8
	adds	r1, r7, #0
	adds	r1, #70
	mov	r0, r9
	adds	r2, r6, #0
	movs	r3, #0
	bl	sub_08104ba8
	b.n	.L_081037f6
.L_081037e8:
	adds	r1, r7, #0
	adds	r1, #70
	mov	r0, r9
	adds	r2, r6, #0
	movs	r3, #1
	bl	sub_08104ba8
.L_081037f6:
	ldr	r1, [sp, #212]
	cmp	r1, #0
	bgt.n	.L_081037fe
	b.n	.L_081039a8
.L_081037fe:
	ldr	r3, [sp, #208]
	movs	r2, #3
	eors	r2, r3
	negs	r3, r2
	orrs	r3, r2
	lsrs	r3, r3, #31
	mov	fp, r3
	mov	r0, fp
	movs	r3, #6
	subs	r0, r3, r0
	adds	r3, r1, #0
	mov	fp, r0
	subs	r3, #1
	mov	r1, fp
	muls	r1, r3
	ldr	r0, [sp, #44]
	mov	sl, r1
	ldr	r1, [sp, #48]
	add	r2, sp, #64
	add	r5, sp, #72
	adds	r1, #88
	add	r3, sp, #68
	str	r2, [sp, #0]
	adds	r0, #88
	adds	r2, r5, #0
	bl	sub_08101860
	lsls	r0, r0, #24
	str	r0, [sp, #12]
	asrs	r3, r0, #24
	ldr	r0, [sp, #60]
	movs	r2, #0
	movs	r1, #0
	lsls	r7, r0, #3
	cmp	sl, r3
	bge.n	.L_0810391a
	cmp	r1, fp
	bge.n	.L_08103916
	str	r1, [sp, #8]
	mov	r1, sl
	lsls	r3, r1, #1
	str	r7, [sp, #16]
	adds	r3, r3, r5
	mov	r8, r3
.L_08103856:
	ldr	r3, [sp, #56]
	lsls	r6, r2, #24
	mov	r0, r8
	asrs	r2, r6, #23
	adds	r2, r3, r2
	movs	r1, #252
	ldrh	r3, [r0, #0]
	lsls	r1, r1, #6
	adds	r1, #255
	lsls	r2, r2, #3
	ands	r3, r1
	adds	r2, #4
	mov	r0, r9
	adds	r1, r7, #0
	bl	sub_08104b18
	mov	r3, r8
	ldrh	r2, [r3, #0]
	ldr	r3, [pc, #32]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0810388a
	movs	r0, #4
	bl	sub_080380b8
	b.n	.L_081038aa
.L_0810388a:
	ldr	r3, [pc, #20]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081038a4
	movs	r0, #2
	bl	sub_080380b8
	b.n	.L_081038aa
	movs	r0, r0
	.4byte 0x00008000
	.2byte 0x4000
	.2byte 0x0000
.L_081038a4:
	movs	r0, #15
	bl	sub_080380b8
.L_081038aa:
	mov	r0, r8
	ldr	r1, [sp, #56]
	ldrh	r3, [r0, #0]
	movs	r0, #252
	asrs	r6, r6, #24
	lsls	r0, r0, #6
	lsls	r5, r6, #1
	adds	r0, #255
	ldr	r2, [sp, #16]
	ands	r0, r3
	adds	r5, r1, r5
	ldr	r3, [pc, #288]
	lsls	r5, r5, #3
	adds	r5, #8
	adds	r0, r0, r3
	mov	r1, r9
	adds	r3, r5, #0
	adds	r2, #16
	bl	sub_08038080
	mov	r2, r8
	ldrh	r0, [r2, #0]
	bl	sub_080ad078
	ldr	r3, [sp, #16]
	ldrb	r0, [r0, #9]
	movs	r1, #2
	mov	r2, r9
	adds	r3, #88
	str	r5, [sp, #0]
	bl	sub_080380a8
	ldr	r0, [sp, #8]
	movs	r1, #128
	lsls	r1, r1, #17
	adds	r3, r0, r1
	movs	r0, #1
	add	sl, r0
	ldr	r0, [sp, #12]
	adds	r6, #1
	lsrs	r1, r3, #24
	movs	r3, #2
	lsls	r6, r6, #24
	add	r8, r3
	asrs	r3, r0, #24
	lsrs	r2, r6, #24
	cmp	sl, r3
	bge.n	.L_0810391a
	lsls	r3, r1, #24
	str	r3, [sp, #8]
	asrs	r3, r3, #24
	cmp	r3, fp
	blt.n	.L_08103856
	b.n	.L_0810391a
.L_08103916:
	ldr	r1, [sp, #60]
	lsls	r7, r1, #3
.L_0810391a:
	movs	r0, #15
	bl	sub_080380b8
	ldr	r3, [sp, #56]
	adds	r2, r7, #0
	lsls	r6, r3, #3
	ldr	r0, [pc, #192]
	adds	r2, #88
	mov	r1, r9
	adds	r3, r6, #0
	bl	sub_08038080
	ldr	r0, [sp, #208]
	cmp	r0, #3
	beq.n	.L_0810399e
	ldr	r3, [sp, #68]
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_08103956
	movs	r0, #4
	bl	sub_080380b8
	adds	r3, r6, #0
	ldr	r0, [pc, #160]
	adds	r3, #88
	mov	r1, r9
	adds	r2, r7, #0
	bl	sub_08038080
	movs	r5, #1
.L_08103956:
	ldr	r3, [sp, #64]
	cmp	r3, #0
	beq.n	.L_08103976
	movs	r0, #2
	bl	sub_080380b8
	ldr	r1, [sp, #56]
	ldr	r0, [pc, #136]
	adds	r3, r1, r5
	lsls	r3, r3, #3
	adds	r3, #88
	mov	r1, r9
	adds	r2, r7, #0
	bl	sub_08038080
	adds	r5, #1
.L_08103976:
	cmp	r5, #0
	bne.n	.L_08103988
	adds	r3, r6, #0
	ldr	r0, [pc, #116]
	adds	r3, #88
	mov	r1, r9
	adds	r2, r7, #0
	bl	sub_08038080
.L_08103988:
	movs	r0, #15
	bl	sub_080380b8
	movs	r3, #11
	str	r3, [sp, #0]
	mov	r0, r9
	movs	r1, #0
	movs	r2, #11
	movs	r3, #13
	bl	sub_08038070
.L_0810399e:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #60]
	movs	r3, #1
	strb	r3, [r2, #3]
.L_081039a8:
	ldr	r2, [sp, #212]
	cmp	r2, #0
	bne.n	.L_081039be
	str	r2, [sp, #0]
	str	r2, [sp, #4]
	ldr	r0, [sp, #52]
	movs	r1, #0
	ldr	r2, [sp, #216]
	mov	r3, r9
	bl	sub_080380d8
.L_081039be:
	movs	r2, #166
	ldr	r1, [sp, #44]
	ldr	r3, [pc, #52]
	ldr	r0, [sp, #48]
	lsls	r2, r2, #1
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x980b
	bl	sub_08013164
	movs	r0, #1
	add	sp, #168
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x000005a7
	.4byte 0x0000101c
	.4byte 0x000010d3
	.4byte 0x000010d4
	.4byte 0x000010d9
	.2byte 0x0730
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #168
	str	r0, [sp, #104]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	movs	r0, #0
	mov	sl, r3
	str	r0, [sp, #100]
	mov	r0, sl
	adds	r0, #240
	movs	r1, #1
	movs	r2, #0
	movs	r3, #2
	str	r3, [sp, #88]
	str	r0, [sp, #80]
	str	r1, [sp, #96]
	str	r2, [sp, #92]
	mov	r3, sl
	ldr	r1, [r0, #0]
	mov	r0, sp
	str	r1, [sp, #84]
	add	r5, sp, #140
	ldr	r2, [r3, #20]
	adds	r0, #132
	movs	r3, #13
	movs	r7, #0
	strb	r3, [r2, #5]
	str	r7, [sp, #132]
	str	r0, [sp, #24]
	str	r7, [r0, #4]
	adds	r0, r5, #0
	bl	sub_080ad100
	mov	r1, sl
	movs	r3, #28
	ldrsb	r3, [r1, r3]
	lsls	r3, r3, #1
	ldrh	r6, [r5, r3]
	movs	r3, #29
	ldrsb	r3, [r1, r3]
	adds	r0, r6, #0
	lsls	r3, r3, #1
	ldrh	r5, [r5, r3]
	adds	r1, r5, #0
	bl	sub_08104e38
	movs	r3, #151
	ldr	r1, [pc, #8]
	lsls	r3, r3, #1
	movs	r2, #3
	add	r3, sl
	b.n	.L_08103a78
	.2byte 0x00c8
	.2byte 0x0000
.L_08103a78:
	subs	r2, #1
	strh	r1, [r3, #0]
	subs	r3, #2
	cmp	r2, #0
	bge.n	.L_08103a78
	movs	r3, #140
	lsls	r3, r3, #1
	add	r3, sl
	movs	r2, #16
	strh	r2, [r3, #0]
	movs	r3, #148
	lsls	r3, r3, #1
	add	r3, sl
	movs	r1, #32
	strh	r1, [r3, #0]
	cmp	r6, r5
	beq.n	.L_08103aaa
	movs	r2, #141
	lsls	r2, r2, #1
	movs	r3, #120
	add	r2, sl
	strh	r3, [r2, #0]
	adds	r3, #178
	add	r3, sl
	strh	r1, [r3, #0]
.L_08103aaa:
	mov	r2, sl
	ldr	r0, [r2, #52]
	bl	sub_08038268
	movs	r0, #1
	bl	sub_08013560
	movs	r1, #1
	ldr	r0, [sp, #80]
	bl	sub_080f815c
	movs	r3, #5
	str	r3, [sp, #0]
	movs	r3, #2
	str	r3, [sp, #4]
	ldr	r0, [sp, #80]
	movs	r3, #30
	movs	r1, #0
	movs	r2, #0
	bl	sub_080f811c
	mov	r3, sp
	movs	r7, #1
	adds	r3, #124
	str	r7, [sp, #124]
	str	r3, [sp, #28]
	str	r7, [r3, #4]
	ldr	r0, [sp, #104]
	cmp	r0, #1
	bls.n	.L_08103ae8
	b.n	.L_08103c5c
.L_08103ae8:
	movs	r0, #96
	bl	sub_08014dac
	movs	r5, #166
	lsls	r5, r5, #1
	str	r0, [sp, #76]
	adds	r0, r5, #0
	bl	sub_08014dac
	movs	r1, #0
	movs	r3, #128
	str	r1, [sp, #72]
	str	r1, [sp, #68]
	lsls	r3, r3, #2
	adds	r3, #22
	add	r3, sl
	ldrb	r6, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #90
	add	r3, sl
	ldrb	r3, [r3, #0]
	movs	r2, #31
	mov	fp, r3
	movs	r3, #150
	lsls	r3, r3, #2
	add	r3, sl
	ldrb	r3, [r3, #0]
	mov	r8, r2
	mov	r9, r0
	mov	r0, r8
	ands	r0, r3
	movs	r3, #182
	lsls	r3, r3, #1
	add	r3, sl
	ldrh	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #8
	ands	r3, r2
	lsls	r3, r3, #16
	lsrs	r3, r3, #16
	mov	r8, r0
	adds	r0, r6, #0
	str	r3, [sp, #64]
	bl	sub_08016ca4
	adds	r7, r0, #0
	ldr	r3, [pc, #780]
	adds	r1, r7, #0
	adds	r2, r5, #0
	mov	r0, r9
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4659
	adds	r0, r6, #0
	mov	r2, r8
	bl	sub_080ad160
	ldr	r1, [sp, #104]
	cmp	r1, #0
	bne.n	.L_08103ba6
	movs	r3, #174
	lsls	r3, r3, #1
	adds	r3, #255
	add	r3, sl
	ldrb	r3, [r3, #0]
	movs	r2, #31
	str	r3, [sp, #72]
	movs	r3, #173
	lsls	r3, r3, #1
	adds	r3, #255
	add	r3, sl
	ldrb	r3, [r3, #0]
	movs	r5, #128
	ands	r2, r3
	str	r2, [sp, #68]
	movs	r3, #183
	lsls	r3, r3, #1
	add	r3, sl
	ldrh	r3, [r3, #0]
	lsls	r5, r5, #8
	ands	r5, r3
	lsls	r5, r5, #16
	lsrs	r5, r5, #16
	adds	r0, r6, #0
	ldr	r1, [sp, #72]
	bl	sub_080ad150
	cmp	r5, #0
	beq.n	.L_08103ba6
	adds	r0, r6, #0
	ldr	r1, [sp, #72]
	ldr	r2, [sp, #68]
	bl	sub_080ad158
.L_08103ba6:
	adds	r0, r6, #0
	bl	sub_080ad008
	mov	r0, sp
	mov	r2, sp
	adds	r0, #120
	mov	r3, r9
	str	r0, [sp, #56]
	adds	r3, #88
	adds	r2, #116
	adds	r1, r7, #0
	str	r3, [sp, #60]
	str	r2, [sp, #52]
	str	r2, [sp, #0]
	adds	r1, #88
	ldr	r2, [sp, #76]
	adds	r0, r3, #0
	ldr	r3, [sp, #56]
	bl	sub_08101860
	movs	r2, #166
	str	r0, [sp, #124]
	mov	r1, r9
	lsls	r2, r2, #1
	ldr	r5, [pc, #636]
	adds	r0, r7, #0
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x238c
	lsls	r3, r3, #1
	adds	r3, #255
	add	r3, sl
	ldrb	r6, [r3, #0]
	adds	r0, r6, #0
	bl	sub_08016ca4
	movs	r2, #166
	adds	r7, r0, #0
	adds	r1, r7, #0
	mov	r0, r9
	lsls	r2, r2, #1
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x9b1a
	cmp	r3, #0
	bne.n	.L_08103c0c
	adds	r0, r6, #0
	ldr	r1, [sp, #72]
	ldr	r2, [sp, #68]
	bl	sub_080ad160
.L_08103c0c:
	adds	r0, r6, #0
	mov	r1, fp
	mov	r2, r8
	bl	sub_080ad150
	ldr	r0, [sp, #64]
	cmp	r0, #0
	beq.n	.L_08103c26
	adds	r0, r6, #0
	mov	r1, fp
	mov	r2, r8
	bl	sub_080ad158
.L_08103c26:
	adds	r0, r6, #0
	bl	sub_080ad008
	ldr	r2, [sp, #52]
	adds	r1, r7, #0
	str	r2, [sp, #0]
	ldr	r3, [sp, #56]
	ldr	r2, [sp, #76]
	ldr	r0, [sp, #60]
	adds	r1, #88
	bl	sub_08101860
	ldr	r3, [sp, #28]
	movs	r2, #166
	str	r0, [r3, #4]
	mov	r1, r9
	lsls	r2, r2, #1
	adds	r0, r7, #0
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4648
	bl	sub_08013164
	ldr	r0, [sp, #76]
	bl	sub_08013164
	b.n	.L_08103d1a
.L_08103c5c:
	ldr	r3, [sp, #104]
	subs	r3, #2
	cmp	r3, #1
	bhi.n	.L_08103d1a
	movs	r0, #96
	bl	sub_08014dac
	str	r0, [sp, #48]
	movs	r0, #166
	lsls	r0, r0, #1
	bl	sub_08014dac
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #22
	add	r3, sl
	ldrb	r6, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #90
	add	r3, sl
	ldrb	r3, [r3, #0]
	mov	r9, r0
	mov	fp, r3
	movs	r3, #150
	lsls	r3, r3, #2
	add	r3, sl
	ldrb	r3, [r3, #0]
	movs	r0, #31
	ldr	r2, [sp, #104]
	mov	r8, r0
	mov	r1, r8
	ands	r1, r3
	movs	r3, #3
	eors	r3, r2
	negs	r5, r3
	adds	r0, r6, #0
	mov	r8, r1
	orrs	r5, r3
	bl	sub_08016ca4
	lsrs	r5, r5, #31
	subs	r5, r7, r5
	movs	r2, #166
	adds	r7, r0, #0
	adds	r1, r7, #0
	lsls	r2, r2, #1
	mov	r0, r9
	ldr	r3, [pc, #404]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c30
	mov	r1, fp
	mov	r2, r8
	bl	sub_080ad160
	cmp	r5, #0
	beq.n	.L_08103cda
	adds	r0, r6, #0
	mov	r1, fp
	mov	r2, r8
	bl	sub_080ad158
.L_08103cda:
	adds	r0, r6, #0
	bl	sub_080ad008
	mov	r0, r9
	add	r2, sp, #108
	adds	r1, r7, #0
	add	r3, sp, #112
	str	r2, [sp, #0]
	adds	r1, #88
	ldr	r2, [sp, #48]
	adds	r0, #88
	bl	sub_08101860
	movs	r2, #166
	str	r0, [sp, #124]
	mov	r1, r9
	lsls	r2, r2, #1
	ldr	r3, [pc, #340]
	adds	r0, r7, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9807
	ldr	r3, [sp, #124]
	movs	r1, #0
	str	r3, [r0, #4]
	mov	r0, r9
	str	r1, [sp, #88]
	bl	sub_08013164
	ldr	r0, [sp, #48]
	bl	sub_08013164
.L_08103d1a:
	ldr	r0, [sp, #124]
	movs	r1, #5
	subs	r0, #1
	bl	sub_08002054
	adds	r0, #1
	str	r0, [sp, #124]
	cmp	r0, #0
	bne.n	.L_08103d30
	movs	r3, #1
	str	r3, [sp, #124]
.L_08103d30:
	ldr	r2, [sp, #28]
	movs	r1, #5
	ldr	r0, [r2, #4]
	subs	r0, #1
	bl	sub_08002054
	ldr	r3, [sp, #28]
	adds	r0, #1
	str	r0, [r3, #4]
	cmp	r0, #0
	bne.n	.L_08103d4c
	ldr	r0, [sp, #28]
	movs	r3, #1
	str	r3, [r0, #4]
.L_08103d4c:
	mov	r1, sl
	adds	r1, #40
	str	r1, [sp, #44]
	movs	r6, #15
	movs	r5, #2
	adds	r0, r1, #0
	movs	r2, #5
	movs	r1, #0
	movs	r3, #15
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl	sub_080f811c
	mov	r2, sl
	adds	r2, #56
	str	r2, [sp, #40]
	movs	r3, #15
	adds	r0, r2, #0
	movs	r1, #15
	movs	r2, #5
	str	r6, [sp, #0]
	str	r5, [sp, #4]
	bl	sub_080f811c
	ldr	r0, [sp, #84]
	bl	sub_08038260
	ldr	r3, [sp, #104]
	cmp	r3, #2
	bne.n	.L_08103d8c
	ldr	r0, [pc, #204]
	b.n	.L_08103d94
.L_08103d8c:
	ldr	r0, [sp, #104]
	cmp	r0, #3
	bne.n	.L_08103dac
	ldr	r0, [pc, #200]
.L_08103d94:
	ldr	r1, [sp, #84]
	movs	r2, #96
	movs	r3, #0
	bl	sub_08038080
	ldr	r0, [pc, #192]
	ldr	r1, [sp, #84]
	movs	r2, #96
	movs	r3, #16
	bl	sub_08038080
	b.n	.L_08103de4
.L_08103dac:
	ldr	r1, [sp, #104]
	cmp	r1, #0
	bne.n	.L_08103dcc
	ldr	r0, [pc, #176]
	ldr	r1, [sp, #84]
	movs	r2, #128
	movs	r3, #0
	bl	sub_08038080
	ldr	r0, [pc, #160]
	ldr	r1, [sp, #84]
	movs	r2, #128
	movs	r3, #8
	bl	sub_08038080
	b.n	.L_08103de4
.L_08103dcc:
	ldr	r0, [pc, #152]
	ldr	r1, [sp, #84]
	movs	r2, #128
	movs	r3, #0
	bl	sub_08038080
	ldr	r0, [pc, #132]
	ldr	r1, [sp, #84]
	movs	r2, #128
	movs	r3, #8
	bl	sub_08038080
.L_08103de4:
	movs	r3, #182
	lsls	r3, r3, #1
	add	r3, sl
	ldrh	r6, [r3, #0]
	movs	r3, #183
	lsls	r3, r3, #1
	add	r3, sl
	ldrh	r7, [r3, #0]
	ldr	r3, [sp, #104]
	movs	r2, #0
	mov	r8, r2
	cmp	r3, #1
	bne.n	.L_08103e02
	movs	r0, #1
	mov	r8, r0
.L_08103e02:
	movs	r1, #128
	lsls	r1, r1, #8
	adds	r5, r6, #0
	ands	r5, r1
	mov	r9, r1
	cmp	r5, #0
	bne.n	.L_08103e16
	movs	r0, #2
	bl	sub_080380b8
.L_08103e16:
	mov	r3, r8
	lsls	r2, r3, #3
	ldr	r0, [sp, #84]
	movs	r1, #40
	adds	r3, r6, #0
	bl	sub_08101a54
	ldr	r0, [sp, #104]
	cmp	r0, #0
	bne.n	.L_08103e6c
	adds	r3, r7, #0
	mov	r1, r9
	ands	r3, r1
	cmp	r3, #0
	bne.n	.L_08103e3a
	movs	r0, #2
	bl	sub_080380b8
.L_08103e3a:
	ldr	r0, [sp, #84]
	movs	r1, #40
	movs	r2, #16
	adds	r3, r7, #0
	bl	sub_08101a54
	b.n	.L_08103e9e
.L_08103e48:
	movs	r7, #1
	b.n	.L_08104490
.L_08103e4c:
	movs	r0, #113
	movs	r7, #2
	b.n	.L_081043d8
	movs	r0, r0
	.4byte 0x03000730
	.4byte 0x000010d6
	.4byte 0x000010d5
	.4byte 0x000010f2
	.4byte 0x000010d8
	.2byte 0x10d7
	.2byte 0x0000
.L_08103e6c:
	ldr	r3, [sp, #104]
	subs	r3, #2
	cmp	r3, #1
	bhi.n	.L_08103e9e
	cmp	r5, #0
	beq.n	.L_08103e7e
	movs	r0, #2
	bl	sub_080380b8
.L_08103e7e:
	ldr	r0, [sp, #84]
	movs	r1, #40
	movs	r2, #16
	adds	r3, r6, #0
	bl	sub_08101a54
	movs	r1, #242
	movs	r3, #0
	lsls	r1, r1, #8
	str	r3, [sp, #0]
	adds	r1, #150
	ldr	r0, [sp, #84]
	movs	r2, #7
	movs	r3, #1
	bl	sub_08038270
.L_08103e9e:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #60]
	mov	fp, r3
	movs	r3, #192
	lsls	r3, r3, #1
	add	r3, sl
	ldr	r6, [r3, #0]
	ldr	r3, [pc, #784]
	ldr	r2, [r3, #4]
	str	r2, [sp, #36]
	ldr	r3, [r3, #12]
	str	r3, [sp, #32]
	movs	r3, #132
	lsls	r3, r3, #6
	adds	r3, #44
	adds	r5, r6, r3
	ldr	r3, [r5, #0]
	cmp	r3, #0
	bne.n	.L_08103ec8
	b.n	.L_081040bc
.L_08103ec8:
	movs	r0, #128
	lsls	r0, r0, #2
	bl	sub_08014dac
	movs	r1, #132
	mov	r9, r0
	movs	r0, #0
	str	r0, [sp, #36]
	str	r0, [sp, #32]
	lsls	r1, r1, #6
	adds	r1, #40
	adds	r2, r6, r1
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
	ldr	r3, [r5, #0]
	subs	r3, #5
	cmp	r3, #18
	bls.n	.L_08103ef0
	b.n	.L_081040b6
.L_08103ef0:
	ldr	r2, [pc, #720]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x08103f44
	.4byte 0x081040b6
	.4byte 0x081040b6
	.4byte 0x081040b6
	.4byte 0x081040b6
	.4byte 0x08103f6e
	.4byte 0x0810400e
	.4byte 0x08103f44
	.4byte 0x081040b6
	.4byte 0x081040b6
	.4byte 0x081040b6
	.4byte 0x081040b6
	.4byte 0x081040b6
	.4byte 0x081040b6
	.4byte 0x081040b6
	.4byte 0x081040b6
	.4byte 0x081040b6
	.4byte 0x081040b6
	.2byte 0x3f44
	.2byte 0x0810
	movs	r3, #132
	lsls	r3, r3, #6
	adds	r3, #40
	adds	r2, r6, r3
	ldr	r3, [r2, #0]
	cmp	r3, #100
	beq.n	.L_08103f54
	b.n	.L_081040b6
.L_08103f54:
	movs	r0, #1
	movs	r3, #0
	movs	r1, #132
	str	r0, [sp, #32]
	str	r0, [sp, #36]
	lsls	r1, r1, #6
	str	r3, [r2, #0]
	adds	r1, #44
	adds	r2, r6, r1
	ldr	r3, [r2, #0]
	adds	r3, #1
	str	r3, [r2, #0]
	b.n	.L_081040b6
	movs	r2, #132
	lsls	r2, r2, #6
	adds	r2, #40
	adds	r3, r6, r2
	ldr	r3, [r3, #0]
	cmp	r3, #60
	beq.n	.L_08103f7e
	b.n	.L_081040b6
.L_08103f7e:
	movs	r3, #8
	add	r3, fp
	mov	r8, r3
	movs	r2, #128
	mov	r1, r8
	ldr	r3, [pc, #572]
	lsls	r2, r2, #2
	mov	r0, r9
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xf7fc
	.2byte 0xff63
	.2byte 0x2108
	movs	r2, #0
	movs	r3, #1
	adds	r0, #3
	bl	sub_08038038
	adds	r7, r0, #0
	mov	r0, sl
	ldr	r3, [r0, #20]
	movs	r5, #1
	strb	r5, [r3, #5]
	movs	r0, #2
	movs	r1, #96
	bl	sub_080f8ab4
	ldr	r3, [pc, #532]
	movs	r1, #139
	lsls	r1, r1, #2
	adds	r3, r3, r1
	strb	r5, [r3, #0]
	b.n	.L_08103fc6
.L_08103fc0:
	movs	r0, #1
	bl	sub_08013560
.L_08103fc6:
	bl	sub_08038048
	cmp	r0, #0
	beq.n	.L_08103fc0
	adds	r0, r7, #0
	movs	r1, #1
	bl	sub_08038018
	movs	r2, #128
	mov	r1, r9
	ldr	r3, [pc, #492]
	lsls	r2, r2, #2
	mov	r0, r8
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xf734
	.2byte 0xf954
	.2byte 0x2184
	lsls	r1, r1, #6
	movs	r3, #1
	mov	r0, fp
	adds	r1, #40
	strb	r3, [r0, #3]
	movs	r2, #0
	str	r3, [sp, #96]
	adds	r3, r6, r1
	str	r2, [r3, #0]
	movs	r3, #132
	lsls	r3, r3, #6
	adds	r3, #44
	adds	r2, r6, r3
	movs	r3, #11
	str	r3, [r2, #0]
	mov	r0, sl
	ldr	r2, [r0, #20]
	b.n	.L_081040b2
	movs	r1, #132
	lsls	r1, r1, #6
	adds	r1, #40
	adds	r3, r6, r1
	ldr	r3, [r3, #0]
	cmp	r3, #60
	bne.n	.L_081040b6
	movs	r2, #8
	add	r2, fp
	mov	r8, r2
	movs	r2, #128
	mov	r1, r8
	ldr	r3, [pc, #416]
	lsls	r2, r2, #2
	mov	r0, r9
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xf7fc
	.2byte 0xff14
	.2byte 0x2108
	movs	r2, #0
	movs	r3, #1
	adds	r0, #2
	bl	sub_08038038
	adds	r7, r0, #0
	mov	r0, sl
	ldr	r3, [r0, #20]
	movs	r5, #1
	strb	r5, [r3, #5]
	movs	r0, #106
	movs	r1, #56
	bl	sub_080f8ab4
	ldr	r3, [pc, #376]
	movs	r1, #139
	lsls	r1, r1, #2
	adds	r3, r3, r1
	strb	r5, [r3, #0]
	b.n	.L_08104064
.L_0810405e:
	movs	r0, #1
	bl	sub_08013560
.L_08104064:
	bl	sub_08038048
	cmp	r0, #0
	beq.n	.L_0810405e
	adds	r0, r7, #0
	movs	r1, #1
	bl	sub_08038018
	movs	r2, #128
	mov	r1, r9
	ldr	r3, [pc, #332]
	lsls	r2, r2, #2
	mov	r0, r8
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xf734
	.2byte 0xf905
	.2byte 0x465a
	movs	r3, #1
	strb	r3, [r2, #3]
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #132
	lsls	r0, r0, #6
	movs	r1, #132
	movs	r3, #1
	adds	r0, #40
	lsls	r1, r1, #6
	str	r3, [sp, #96]
	movs	r5, #0
	adds	r3, r6, r0
	adds	r1, #44
	str	r5, [r3, #0]
	adds	r2, r6, r1
	movs	r3, #12
	str	r3, [r2, #0]
	mov	r3, sl
	ldr	r2, [r3, #20]
.L_081040b2:
	movs	r3, #13
	strb	r3, [r2, #5]
.L_081040b6:
	mov	r0, r9
	bl	sub_08013164
.L_081040bc:
	ldr	r0, [sp, #96]
	cmp	r0, #0
	bne.n	.L_081040c4
	b.n	.L_0810422e
.L_081040c4:
	ldr	r2, [sp, #104]
	movs	r1, #1
	mov	r8, r1
	cmp	r2, #1
	bhi.n	.L_0810411c
	movs	r3, #24
	str	r3, [sp, #0]
	ldr	r0, [sp, #84]
	movs	r3, #224
	movs	r1, #128
	movs	r2, #16
	bl	sub_08038068
	ldr	r3, [sp, #88]
	cmp	r3, #1
	bne.n	.L_081040f2
	ldr	r0, [pc, #232]
	ldr	r1, [sp, #84]
	movs	r2, #128
	movs	r3, #16
	bl	sub_08038080
	b.n	.L_0810411c
.L_081040f2:
	ldr	r0, [sp, #88]
	movs	r2, #2
	eors	r2, r0
	negs	r3, r2
	orrs	r3, r2
	movs	r2, #133
	lsls	r2, r2, #2
	lsrs	r3, r3, #31
	adds	r3, r3, r2
	mov	r2, sl
	adds	r2, #2
	ldrb	r0, [r2, r3]
	movs	r1, #1
	bl	sub_08038120
	ldr	r0, [pc, #192]
	ldr	r1, [sp, #84]
	movs	r2, #128
	movs	r3, #16
	bl	sub_08038080
.L_0810411c:
	movs	r3, #1
	mov	r1, fp
	strb	r3, [r1, #6]
	mov	r2, sl
	ldr	r0, [r2, #40]
	bl	sub_08038060
	mov	r3, sl
	ldr	r0, [r3, #56]
	bl	sub_08038060
	ldr	r0, [sp, #96]
	lsrs	r3, r0, #1
	cmp	r3, #0
	beq.n	.L_08104140
	movs	r0, #1
	bl	sub_08013560
.L_08104140:
	ldr	r1, [sp, #104]
	cmp	r1, #3
	bne.n	.L_0810414e
	movs	r2, #0
	movs	r7, #0
	mov	r8, r2
	b.n	.L_0810415e
.L_0810414e:
	ldr	r3, [sp, #104]
	cmp	r3, #2
	bne.n	.L_0810415c
	movs	r0, #0
	movs	r7, #1
	mov	r8, r0
	b.n	.L_0810415e
.L_0810415c:
	movs	r7, #2
.L_0810415e:
	ldr	r1, [sp, #88]
	cmp	r1, #0
	beq.n	.L_081041d8
	cmp	r1, #1
	beq.n	.L_08104172
	ldr	r0, [sp, #104]
	movs	r1, #0
	bl	sub_08103168
	b.n	.L_08104228
.L_08104172:
	ldr	r2, [sp, #104]
	cmp	r2, #1
	bne.n	.L_0810417a
	movs	r7, #4
.L_0810417a:
	movs	r6, #140
	ldr	r1, [sp, #88]
	lsls	r6, r6, #1
	adds	r6, #255
	mov	r3, sl
	add	r6, sl
	ldr	r0, [r3, #40]
	movs	r5, #0
	ldrb	r3, [r6, #0]
	movs	r2, #0
	str	r1, [sp, #0]
	movs	r1, #0
	str	r5, [sp, #4]
	str	r7, [sp, #8]
	str	r5, [sp, #12]
	str	r5, [sp, #16]
	bl	.L_08103218
	ldr	r1, [sp, #88]
	mov	r2, sl
	ldr	r0, [r2, #56]
	ldrb	r3, [r6, #0]
	str	r1, [sp, #0]
	str	r5, [sp, #4]
	ldr	r1, [sp, #24]
	str	r7, [sp, #8]
	ldr	r2, [r1, #4]
	movs	r1, #0
	adds	r2, #1
	str	r2, [sp, #12]
	movs	r2, #0
	str	r5, [sp, #16]
	bl	.L_08103218
	b.n	.L_08104228
	.4byte 0x03001150
	.4byte 0x08103ef8
	.4byte 0x03000730
	.4byte 0x02000240
	.4byte 0x000010d2
	.2byte 0x10d1
	.2byte 0x0000
.L_081041d8:
	ldr	r2, [sp, #104]
	cmp	r2, #1
	bne.n	.L_081041e4
	movs	r3, #0
	movs	r7, #1
	mov	r8, r3
.L_081041e4:
	ldr	r2, [sp, #88]
	movs	r5, #151
	mov	r1, sl
	lsls	r5, r5, #2
	ldr	r0, [r1, #40]
	add	r5, sl
	mov	r1, r8
	ldrb	r3, [r5, #0]
	movs	r6, #1
	str	r2, [sp, #0]
	str	r1, [sp, #4]
	str	r2, [sp, #12]
	movs	r1, #0
	movs	r2, #0
	str	r7, [sp, #8]
	str	r6, [sp, #16]
	bl	.L_08103218
	mov	r2, sl
	ldr	r0, [r2, #56]
	mov	r2, r8
	ldrb	r3, [r5, #0]
	str	r2, [sp, #4]
	ldr	r2, [sp, #132]
	ldr	r1, [sp, #88]
	adds	r2, #1
	str	r1, [sp, #0]
	str	r2, [sp, #12]
	movs	r1, #0
	movs	r2, #0
	str	r7, [sp, #8]
	str	r6, [sp, #16]
	bl	.L_08103218
.L_08104228:
	movs	r3, #0
	mov	r0, fp
	strb	r3, [r0, #6]
.L_0810422e:
	ldr	r1, [sp, #88]
	cmp	r1, #1
	bgt.n	.L_081042d2
	ldr	r0, [sp, #28]
	lsls	r4, r1, #2
	mov	r2, sl
	adds	r3, r4, r0
	ldr	r7, [r2, #56]
	ldr	r2, [r3, #0]
	cmp	r2, #1
	ble.n	.L_081042d2
	movs	r5, #0
	cmp	r5, r2
	bge.n	.L_0810428c
	adds	r6, r3, #0
.L_0810424c:
	movs	r2, #240
	lsls	r2, r2, #8
	adds	r2, #49
	adds	r1, r5, r2
	cmp	r5, #8
	ble.n	.L_0810425e
	movs	r1, #240
	lsls	r1, r1, #8
	adds	r1, #63
.L_0810425e:
	ldr	r0, [sp, #24]
	ldr	r3, [r4, r0]
	cmp	r5, r3
	bne.n	.L_0810426a
	ldr	r2, [pc, #692]
	adds	r1, r1, r2
.L_0810426a:
	ldr	r3, [r6, #0]
	ldrh	r2, [r7, #8]
	adds	r0, r7, #0
	subs	r2, r2, r3
	adds	r2, r2, r5
	movs	r3, #0
	str	r3, [sp, #0]
	subs	r2, #2
	subs	r3, #1
	str	r4, [sp, #20]
	bl	sub_08038270
	ldr	r3, [r6, #0]
	adds	r5, #1
	ldr	r4, [sp, #20]
	cmp	r5, r3
	blt.n	.L_0810424c
.L_0810428c:
	ldr	r0, [sp, #28]
	ldrh	r2, [r7, #8]
	ldr	r3, [r4, r0]
	movs	r1, #241
	movs	r6, #1
	negs	r6, r6
	subs	r2, r2, r3
	lsls	r1, r1, #8
	movs	r5, #0
	adds	r0, r7, #0
	adds	r3, r6, #0
	adds	r1, #40
	subs	r2, #3
	str	r5, [sp, #0]
	bl	sub_08038270
	ldrh	r2, [r7, #8]
	movs	r1, #241
	lsls	r1, r1, #8
	adds	r1, #41
	subs	r2, #2
	adds	r0, r7, #0
	adds	r3, r6, #0
	str	r5, [sp, #0]
	bl	sub_08038270
	ldrh	r3, [r7, #14]
	movs	r2, #2
	lsls	r3, r3, #16
	asrs	r3, r3, #18
	mov	r1, fp
	lsls	r2, r3
	ldrb	r3, [r1, #3]
	orrs	r2, r3
	strb	r2, [r1, #3]
.L_081042d2:
	ldr	r2, [sp, #92]
	movs	r1, #60
	adds	r2, #1
	adds	r0, r2, #0
	str	r2, [sp, #92]
	bl	sub_08002064
	subs	r6, r0, #5
	cmp	r6, #0
	bge.n	.L_081042e8
	movs	r6, #0
.L_081042e8:
	cmp	r6, #29
	ble.n	.L_081042ee
	movs	r6, #29
.L_081042ee:
	ldr	r5, [pc, #560]
	movs	r0, #0
	adds	r1, r5, #0
	bl	sub_08105350
	movs	r0, #1
	adds	r1, r5, #0
	bl	sub_08105350
	ldr	r3, [sp, #104]
	cmp	r3, #1
	bhi.n	.L_08104368
	movs	r1, #30
	adds	r0, r6, #0
	bl	sub_08002064
	adds	r3, r0, #0
	lsls	r0, r3, #4
	adds	r0, r0, r3
	lsls	r0, r0, #4
	adds	r0, r0, r3
	lsls	r0, r0, #2
	bl	sub_08002096
	ldr	r3, [pc, #516]
	adds	r1, r0, #0
	movs	r0, #16
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2306
	negs	r5, r0
	negs	r3, r3
	cmp	r5, r3
	bge.n	.L_08104334
	adds	r5, r3, #0
.L_08104334:
	cmp	r5, #12
	ble.n	.L_0810433a
	movs	r5, #12
.L_0810433a:
	adds	r0, r6, #0
	movs	r1, #35
	bl	sub_08002064
	lsls	r6, r0, #1
	adds	r1, r6, #0
	adds	r2, r5, #0
	movs	r0, #0
	adds	r1, #34
	adds	r2, #20
	bl	sub_08105300
	ldr	r0, [sp, #104]
	cmp	r0, #0
	bne.n	.L_08104372
	movs	r1, #99
	movs	r2, #36
	subs	r1, r1, r6
	subs	r2, r2, r5
	movs	r0, #1
	bl	sub_08105300
	b.n	.L_08104372
.L_08104368:
	movs	r0, #0
	movs	r1, #32
	movs	r2, #30
	bl	sub_08105300
.L_08104372:
	ldr	r1, [sp, #96]
	cmp	r1, #0
	beq.n	.L_08104386
	movs	r2, #0
	ldr	r0, [sp, #100]
	movs	r1, #2
	str	r2, [sp, #96]
	bl	sub_08100e28
	str	r0, [sp, #100]
.L_08104386:
	ldr	r3, [sp, #100]
	movs	r1, #16
	lsls	r0, r3, #3
	subs	r0, r0, r3
	lsls	r0, r0, #3
	adds	r0, #80
	bl	sub_080f8a44
	ldr	r0, [sp, #92]
	movs	r3, #3
	ands	r3, r0
	cmp	r3, #0
	bne.n	sub_081043c2
	movs	r3, #4
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_081043b6
	ldr	r1, [pc, #380]
	ldr	r3, [pc, #384]
	ldr	r0, [pc, #384]
	movs	r2, #32
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xe005
.L_081043b6:
	ldr	r3, [pc, #380]
	ldr	r0, [pc, #372]
	movs	r1, #32
	ldr	r2, [pc, #376]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9909
	movs	r3, #1
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_081043e0
	ldr	r2, [sp, #100]
	cmp	r2, #0
	bne.n	.L_081043d4
	b.n	.L_08103e48
.L_081043d4:
	movs	r0, #113
	movs	r7, #1
.L_081043d8:
	bl	sub_081c0010
	negs	r7, r7
	b.n	.L_08104490
.L_081043e0:
	ldr	r0, [sp, #36]
	movs	r3, #8
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_081043ec
	b.n	.L_08103e4c
.L_081043ec:
	movs	r3, #2
	ands	r3, r0
	cmp	r3, #0
	bne.n	.L_081043d4
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_0810441c
	ldr	r1, [sp, #104]
	cmp	r1, #1
	bhi.n	.L_08104488
	ldr	r0, [sp, #88]
	movs	r1, #3
	adds	r0, #1
	bl	sub_08002064
	movs	r2, #2
	str	r0, [sp, #88]
	movs	r0, #111
	str	r2, [sp, #96]
	bl	sub_081c0010
	b.n	.L_08104488
.L_0810441c:
	ldr	r0, [sp, #32]
	movs	r3, #32
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_08104452
	ldr	r1, [sp, #88]
	cmp	r1, #1
	bgt.n	.L_08104488
	ldr	r2, [sp, #24]
	lsls	r3, r1, #2
	adds	r5, r3, r2
	ldr	r0, [r5, #0]
	subs	r0, #1
	str	r0, [r5, #0]
	ldr	r2, [sp, #28]
	ldr	r1, [r3, r2]
	bl	sub_08100e28
	str	r0, [r5, #0]
	movs	r0, #111
	bl	sub_081c0010
	bl	sub_080138a8
	movs	r3, #1
	str	r3, [sp, #96]
	b.n	.L_08104488
.L_08104452:
	ldr	r0, [sp, #32]
	movs	r3, #16
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_08104488
	ldr	r1, [sp, #88]
	cmp	r1, #1
	bgt.n	.L_08104488
	movs	r0, #111
	bl	sub_081c0010
	bl	sub_080138a8
	ldr	r2, [sp, #88]
	ldr	r0, [sp, #24]
	lsls	r3, r2, #2
	adds	r5, r3, r0
	ldr	r0, [r5, #0]
	movs	r1, #1
	adds	r0, #1
	str	r0, [r5, #0]
	ldr	r2, [sp, #28]
	str	r1, [sp, #96]
	ldr	r1, [r3, r2]
	bl	sub_08100e28
	str	r0, [r5, #0]
.L_08104488:
	movs	r0, #1
	bl	sub_08013560
	b.n	.L_08103e9e
.L_08104490:
	movs	r0, #0
	movs	r1, #0
	bl	sub_08105350
	movs	r0, #1
	movs	r1, #0
	bl	sub_08105350
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #148]
	bl	sub_080145a8
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r2, [r5, #60]
	movs	r3, #1
	strb	r3, [r2, #6]
	ldr	r0, [sp, #80]
	movs	r1, #1
	bl	sub_080f815c
	movs	r0, #1
	bl	sub_08013560
	movs	r3, #5
	str	r3, [sp, #0]
	movs	r3, #2
	str	r3, [sp, #4]
	ldr	r0, [sp, #80]
	movs	r2, #0
	movs	r3, #17
	movs	r1, #13
	bl	sub_080f811c
	movs	r1, #1
	ldr	r0, [sp, #44]
	bl	sub_080f815c
	movs	r1, #1
	ldr	r0, [sp, #40]
	bl	sub_080f815c
	mov	r3, sl
	ldr	r0, [r3, #52]
	bl	sub_08038260
	mov	r1, sl
	ldr	r0, [r1, #44]
	bl	sub_08038260
	mov	r2, sl
	ldr	r0, [r2, #16]
	bl	sub_08038260
	ldr	r3, [r5, #60]
	movs	r6, #0
	strb	r6, [r3, #6]
	movs	r0, #1
	bl	sub_08013560
	adds	r0, r7, #0
	add	sp, #168
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0xfffff000
	.4byte 0xffff4000
	.4byte 0x0300021c
	.4byte 0x081059b4
	.4byte 0x03000730
	.4byte 0x060052c0
	.4byte 0x03000260
	.4byte 0x44444444
	.2byte 0x4da9
	.2byte 0x0810
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	sub	sp, #88
	movs	r1, #0
	adds	r3, #220
	movs	r0, #1
	ldr	r3, [r3, #0]
	str	r1, [sp, #56]
	str	r0, [sp, #60]
	mov	sl, r3
	ldr	r2, [r3, #20]
	movs	r3, #13
	strb	r3, [r2, #5]
	ldr	r1, [pc, #20]
	mov	r2, sp
	movs	r3, #151
	adds	r2, #80
	movs	r7, #0
	lsls	r3, r3, #1
	str	r2, [sp, #24]
	str	r7, [sp, #80]
	add	r3, sl
	str	r7, [r2, #4]
	movs	r2, #3
	b.n	.L_08104584
	.2byte 0x00c8
	.2byte 0x0000
.L_08104584:
	subs	r2, #1
	strh	r1, [r3, #0]
	subs	r3, #2
	cmp	r2, #0
	bge.n	.L_08104584
	mov	r3, sl
	ldr	r0, [r3, #52]
	bl	sub_08038268
	movs	r0, #1
	bl	sub_08013560
	mov	r0, sp
	adds	r0, #72
	movs	r7, #1
	str	r0, [sp, #28]
	str	r7, [sp, #72]
	str	r7, [r0, #4]
	movs	r0, #96
	bl	sub_08014dac
	adds	r5, r0, #0
	movs	r0, #166
	lsls	r0, r0, #1
	bl	sub_08014dac
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #22
	add	r3, sl
	adds	r6, r0, #0
	ldrb	r0, [r3, #0]
	bl	sub_08016ca4
	adds	r1, r0, #0
	add	r2, sp, #64
	adds	r1, #88
	str	r2, [sp, #0]
	add	r3, sp, #68
	adds	r2, r5, #0
	adds	r0, r1, #0
	bl	sub_08101860
	ldr	r1, [sp, #28]
	str	r0, [sp, #72]
	str	r0, [r1, #4]
	adds	r0, r6, #0
	bl	sub_08013164
	adds	r0, r5, #0
	bl	sub_08013164
	ldr	r0, [sp, #72]
	movs	r1, #6
	subs	r0, #1
	bl	sub_08002054
	adds	r0, #1
	str	r0, [sp, #72]
	cmp	r0, #0
	bne.n	.L_08104600
	str	r7, [sp, #72]
.L_08104600:
	ldr	r2, [sp, #28]
	movs	r1, #6
	ldr	r0, [r2, #4]
	subs	r0, #1
	bl	sub_08002054
	ldr	r3, [sp, #28]
	adds	r0, #1
	str	r0, [r3, #4]
	cmp	r0, #0
	bne.n	.L_08104618
	str	r7, [r3, #4]
.L_08104618:
	mov	r0, sl
	movs	r5, #2
	adds	r0, #40
	movs	r6, #15
	movs	r1, #0
	movs	r2, #5
	movs	r3, #15
	str	r5, [sp, #4]
	str	r0, [sp, #52]
	str	r6, [sp, #0]
	bl	sub_080f811c
	mov	r1, sl
	adds	r1, #56
	str	r1, [sp, #48]
	movs	r3, #15
	adds	r0, r1, #0
	movs	r2, #5
	movs	r1, #15
	str	r5, [sp, #4]
	str	r6, [sp, #0]
	bl	sub_080f811c
	mov	r2, sl
	adds	r2, #240
	str	r2, [sp, #44]
	ldr	r0, [r2, #0]
	bl	sub_08038260
	mov	r3, sl
	ldr	r0, [r3, #16]
	bl	sub_08038260
	ldr	r5, [pc, #680]
	mov	r0, sl
	ldr	r1, [r0, #16]
	movs	r2, #0
	adds	r0, r5, #0
	movs	r3, #0
	bl	sub_08038080
	adds	r5, #2
	mov	r2, sl
	ldr	r1, [r2, #16]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #16
	bl	sub_08038080
	movs	r2, #151
	mov	r1, sp
	lsls	r2, r2, #2
	ldr	r0, [sp, #24]
	adds	r1, #72
	add	r2, sl
	str	r1, [sp, #20]
	str	r2, [sp, #32]
	movs	r3, #0
	mov	r9, r3
	mov	fp, r0
.L_08104690:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #60]
	ldr	r1, [sp, #60]
	mov	r8, r3
	ldr	r3, [pc, #620]
	ldr	r0, [r3, #4]
	str	r0, [sp, #40]
	ldr	r3, [r3, #12]
	str	r3, [sp, #36]
	cmp	r1, #0
	beq.n	.L_08104708
	mov	r3, r8
	movs	r2, #1
	strb	r2, [r3, #6]
	mov	r1, sl
	ldr	r0, [r1, #40]
	bl	sub_08038060
	mov	r2, sl
	ldr	r0, [r2, #56]
	bl	sub_08038060
	ldr	r1, [sp, #32]
	mov	r2, r9
	mov	r3, sl
	ldr	r0, [r3, #40]
	ldrb	r3, [r1, #0]
	str	r2, [sp, #0]
	str	r2, [sp, #4]
	str	r2, [sp, #12]
	movs	r1, #3
	movs	r2, #1
	str	r1, [sp, #8]
	str	r2, [sp, #16]
	movs	r1, #0
	movs	r2, #0
	bl	.L_08103218
	ldr	r1, [sp, #32]
	mov	r2, r9
	mov	r3, sl
	ldr	r0, [r3, #56]
	ldrb	r3, [r1, #0]
	str	r2, [sp, #0]
	str	r2, [sp, #4]
	ldr	r2, [sp, #80]
	movs	r1, #3
	adds	r2, #1
	str	r2, [sp, #12]
	movs	r2, #1
	str	r1, [sp, #8]
	str	r2, [sp, #16]
	movs	r1, #0
	movs	r2, #0
	bl	.L_08103218
	mov	r3, r9
	mov	r0, r8
	strb	r3, [r0, #6]
.L_08104708:
	ldr	r2, [sp, #28]
	movs	r1, #0
	ldr	r3, [r1, r2]
	cmp	r3, #1
	ble.n	.L_081047aa
	mov	r0, sl
	movs	r5, #0
	ldr	r6, [r0, #56]
	cmp	r5, r3
	bge.n	.L_08104760
	adds	r7, r2, #0
	adds	r7, #0
.L_08104720:
	movs	r2, #240
	lsls	r2, r2, #8
	adds	r2, #49
	adds	r1, r5, r2
	cmp	r5, #8
	ble.n	.L_08104732
	movs	r1, #240
	lsls	r1, r1, #8
	adds	r1, #63
.L_08104732:
	ldr	r2, [sp, #24]
	movs	r0, #0
	ldr	r3, [r0, r2]
	cmp	r5, r3
	bne.n	.L_08104740
	ldr	r3, [pc, #460]
	adds	r1, r1, r3
.L_08104740:
	ldr	r3, [r7, #0]
	ldrh	r2, [r6, #8]
	mov	r0, r9
	subs	r2, r2, r3
	adds	r2, r2, r5
	movs	r3, #1
	str	r0, [sp, #0]
	negs	r3, r3
	subs	r2, #2
	adds	r0, r6, #0
	bl	sub_08038270
	ldr	r3, [r7, #0]
	adds	r5, #1
	cmp	r5, r3
	blt.n	.L_08104720
.L_08104760:
	ldr	r0, [sp, #28]
	movs	r1, #0
	ldr	r3, [r1, r0]
	ldrh	r2, [r6, #8]
	mov	r1, r9
	str	r1, [sp, #0]
	movs	r5, #1
	movs	r1, #241
	negs	r5, r5
	subs	r2, r2, r3
	lsls	r1, r1, #8
	adds	r0, r6, #0
	adds	r3, r5, #0
	subs	r2, #3
	adds	r1, #40
	bl	sub_08038270
	ldrh	r2, [r6, #8]
	movs	r1, #241
	mov	r3, r9
	lsls	r1, r1, #8
	str	r3, [sp, #0]
	subs	r2, #2
	adds	r0, r6, #0
	adds	r1, #41
	adds	r3, r5, #0
	bl	sub_08038270
	ldrh	r3, [r6, #14]
	movs	r2, #2
	lsls	r3, r3, #16
	asrs	r3, r3, #18
	mov	r0, r8
	lsls	r2, r3
	ldrb	r3, [r0, #3]
	orrs	r2, r3
	strb	r2, [r0, #3]
.L_081047aa:
	ldr	r1, [sp, #56]
	adds	r1, #1
	str	r1, [sp, #56]
	adds	r0, r1, #0
	movs	r1, #60
	bl	sub_08002064
	subs	r0, #5
	movs	r2, #200
	movs	r0, #0
	movs	r1, #32
	bl	sub_08105300
	ldr	r2, [sp, #60]
	cmp	r2, #0
	beq.n	.L_081047ce
	movs	r3, #0
	str	r3, [sp, #60]
.L_081047ce:
	ldr	r3, [sp, #56]
	movs	r0, #3
	ands	r3, r0
	cmp	r3, #0
	bne.n	sub_081047fc
	ldr	r1, [sp, #56]
	movs	r3, #4
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_081047f0
	ldr	r3, [pc, #300]
	ldr	r0, [pc, #300]
	ldr	r1, [pc, #304]
	movs	r2, #32
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xe005
.L_081047f0:
	ldr	r3, [pc, #296]
	ldr	r0, [pc, #288]
	movs	r1, #32
	ldr	r2, [pc, #296]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9a0a
	movs	r3, #8
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0810480c
	movs	r0, #113
	movs	r7, #2
	b.n	.L_0810481e
.L_0810480c:
	movs	r3, #129
	ldr	r0, [sp, #40]
	lsls	r3, r3, #2
	adds	r3, #255
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_08104826
	movs	r0, #113
	movs	r7, #1
.L_0810481e:
	bl	sub_081c0010
	negs	r7, r7
	b.n	.L_08104888
.L_08104826:
	ldr	r1, [sp, #36]
	movs	r3, #32
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_08104854
	mov	r2, fp
	ldr	r0, [r2, #0]
	subs	r0, #1
	str	r0, [r2, #0]
	ldr	r3, [sp, #20]
	ldr	r1, [r3, #0]
	bl	sub_08100e28
	mov	r1, fp
	str	r0, [r1, #0]
	movs	r0, #111
	bl	sub_081c0010
	bl	sub_080138a8
	movs	r2, #1
	str	r2, [sp, #60]
	b.n	.L_08104880
.L_08104854:
	ldr	r0, [sp, #36]
	movs	r3, #16
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L_08104880
	mov	r1, fp
	ldr	r0, [r1, #0]
	adds	r0, #1
	str	r0, [r1, #0]
	ldr	r2, [sp, #20]
	ldr	r1, [r2, #0]
	bl	sub_08100e28
	mov	r3, fp
	str	r0, [r3, #0]
	movs	r0, #111
	bl	sub_081c0010
	bl	sub_080138a8
	movs	r0, #1
	str	r0, [sp, #60]
.L_08104880:
	movs	r0, #1
	bl	sub_08013560
	b.n	.L_08104690
.L_08104888:
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #148]
	bl	sub_080145a8
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r2, [r5, #60]
	movs	r3, #1
	strb	r3, [r2, #6]
	ldr	r0, [sp, #44]
	movs	r1, #1
	bl	sub_080f815c
	movs	r0, #1
	bl	sub_08013560
	movs	r3, #5
	str	r3, [sp, #0]
	movs	r3, #2
	str	r3, [sp, #4]
	ldr	r0, [sp, #44]
	movs	r3, #17
	movs	r2, #0
	movs	r1, #13
	bl	sub_080f811c
	movs	r1, #1
	ldr	r0, [sp, #52]
	bl	sub_080f815c
	movs	r1, #1
	ldr	r0, [sp, #48]
	bl	sub_080f815c
	mov	r1, sl
	ldr	r0, [r1, #52]
	bl	sub_08038260
	mov	r2, sl
	ldr	r0, [r2, #44]
	bl	sub_08038260
	mov	r3, sl
	ldr	r0, [r3, #16]
	bl	sub_08038260
	ldr	r3, [r5, #60]
	movs	r6, #0
	strb	r6, [r3, #6]
	movs	r0, #1
	bl	sub_08013560
	adds	r0, r7, #0
	add	sp, #88
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x000010db
	.4byte 0x03001150
	.4byte 0xfffff000
	.4byte 0x03000730
	.4byte 0x060052c0
	.4byte 0x081059b4
	.4byte 0x03000260
	.4byte 0x44444444
	.2byte 0x4da9
	.2byte 0x0810
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	movs	r2, #0
	mov	r8, r3
	movs	r3, #139
	lsls	r3, r3, #1
	adds	r3, #255
	add	r3, r8
	ldrb	r3, [r3, #0]
	sub	sp, #4
	adds	r7, r1, #0
	movs	r6, #0
	cmp	r2, r3
	bge.n	.L_08104980
	adds	r5, r0, #0
.L_08104950:
	movs	r3, #0
	strb	r3, [r5, #0]
	cmp	r6, r7
	beq.n	.L_0810496e
	adds	r0, r7, #0
	adds	r1, r6, #0
	str	r2, [sp, #0]
	bl	sub_0810498c
	ldr	r2, [sp, #0]
	cmp	r0, #0
	bne.n	.L_0810496e
	movs	r3, #1
	strb	r3, [r5, #0]
	adds	r2, #1
.L_0810496e:
	movs	r3, #139
	lsls	r3, r3, #1
	adds	r3, #255
	add	r3, r8
	ldrb	r3, [r3, #0]
	adds	r6, #1
	adds	r5, #1
	cmp	r6, r3
	blt.n	.L_08104950
.L_08104980:
	adds	r0, r2, #0
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
