.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
	.set sub_08002064, 0x08002064
	.set sub_0800206c, 0x0800206c
	.set sub_08002090, 0x08002090
	.set sub_08002096, 0x08002096
	.set sub_0801314c, 0x0801314c
	.set sub_08013164, 0x08013164
	.set sub_08013300, 0x08013300
	.set sub_08013560, 0x08013560
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_08014dac, 0x08014dac
	.set sub_08014de4, 0x08014de4
	.set sub_080150e4, 0x080150e4
	.set sub_08015160, 0x08015160
	.set sub_080151e4, 0x080151e4
	.set sub_08020010, 0x08020010
	.set sub_08020048, 0x08020048
	.set sub_08118088, 0x08118088
	.set sub_081180e8, 0x081180e8
	.set sub_08138086, 0x08138086
	.set sub_0813ba50, 0x0813ba50
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_08144aac, 0x08144aac
	.set sub_0814cc4c, 0x0814cc4c
	.set sub_0814cd48, 0x0814cd48
	.set sub_08152404, 0x08152404
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_0815b434, 0x0815b434
	.set sub_0815e21c, 0x0815e21c
	.set sub_0815f0a0, 0x0815f0a0
	.set sub_0816442c, 0x0816442c
	.set sub_0816467c, 0x0816467c
	.set sub_0816fd58, 0x0816fd58
	.set sub_081706fa, 0x081706fa
	.set sub_08170720, 0x08170720
	.set sub_08170806, 0x08170806
	.set sub_08170ad0, 0x08170ad0
	.set sub_08170b7c, 0x08170b7c
	.set sub_0818caa8, 0x0818caa8
	.set sub_081963ec, 0x081963ec
	.set sub_08196404, 0x08196404
	.set sub_08196958, 0x08196958
	.set sub_081969f8, 0x081969f8
	.set sub_08196a7c, 0x08196a7c
	.set sub_081c0010, 0x081c0010
	.global Overlay_0816fca8
Overlay_0816fca8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #96
	str	r0, [sp, #60]
	str	r1, [sp, #56]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #96]
	ldr	r0, [r3, #92]
	str	r1, [sp, #52]
	mov	fp, r0
	ldr	r3, [r3, #100]
	movs	r0, #1
	str	r3, [sp, #36]
	bl	sub_081435e0
	ldr	r2, [sp, #56]
	cmp	r2, #0
	bne.n	.L_0816fce4
	movs	r2, #128
	ldr	r3, [pc, #4]
	b.n	.L_0816fce8
	movs	r0, r0
	.2byte 0x1010
	.2byte 0x0000
.L_0816fce4:
	movs	r2, #128
	ldr	r3, [pc, #60]
.L_0816fce8:
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	movs	r1, #19
	movs	r0, #104
	bl	sub_081963ec
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	movs	r1, #240
	lsls	r1, r1, #4
	str	r3, [sp, #40]
	ldr	r0, [pc, #36]
	movs	r3, #0
	add	r1, fp
	movs	r2, #1
	str	r3, [sp, #32]
	bl	sub_08157cf4
	ldr	r0, [pc, #24]
	ldr	r1, [sp, #36]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r0, [pc, #16]
	ldr	r1, [pc, #20]
	movs	r2, #1
	b.n	.L_0816fd38
	.4byte 0x00000810
	.4byte 0x00000192
	.4byte 0x00000134
	.4byte 0x00000188
	.2byte 0x0000
	.2byte 0x0201
.L_0816fd38:
	movs	r3, #1
	bl	sub_08157cf4
	ldr	r7, [sp, #56]
	cmp	r7, #1
	bne.n	sub_0816fd58
	ldr	r0, [pc, #684]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #680]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x20d8
	lsls	r0, r0, #5
	adds	r0, #86
	movs	r1, #128
	lsls	r1, r1, #7
	add	r0, fp
	ldr	r3, [pc, #660]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x21d8
	lsls	r1, r1, #5
	movs	r0, #0
	adds	r1, #90
	mov	r9, r0
	movs	r7, #0
	mov	ip, r1
.L_0816fd78:
	mov	r2, r9
	lsls	r3, r2, #12
	mov	r0, fp
	movs	r6, #0
	lsls	r5, r7, #6
	adds	r4, r3, r0
.L_0816fd84:
	ldr	r3, [pc, #632]
	mov	r2, ip
	adds	r1, r4, r2
	movs	r0, #0
	adds	r2, r5, r3
.L_0816fd8e:
	ldrb	r3, [r2, #0]
	adds	r0, #1
	strb	r3, [r1, #0]
	adds	r2, #1
	adds	r1, #1
	cmp	r0, #24
	bne.n	.L_0816fd8e
	adds	r6, #1
	adds	r5, #24
	adds	r4, #32
	cmp	r6, #120
	bne.n	.L_0816fd84
	movs	r0, #1
	add	r9, r0
	mov	r1, r9
	adds	r7, #45
	cmp	r1, #4
	bne.n	.L_0816fd78
	ldr	r2, [sp, #56]
	cmp	r2, #0
	bne.n	.L_0816fdce
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, fp
	movs	r3, #50
	b.n	.L_0816fde2
.L_0816fdce:
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, fp
	movs	r3, #0
.L_0816fde2:
	str	r3, [r2, #0]
	movs	r1, #200
	ldr	r0, [pc, #540]
	lsls	r1, r1, #4
	bl	sub_080145a8
	ldr	r3, [sp, #56]
	cmp	r3, #0
	bne.n	.L_0816fdfa
	movs	r7, #24
	str	r7, [sp, #28]
	b.n	.L_0816fdfe
.L_0816fdfa:
	movs	r0, #40
	str	r0, [sp, #28]
.L_0816fdfe:
	movs	r1, #0
	str	r1, [sp, #48]
.L_0816fe02:
	ldr	r2, [sp, #48]
	cmp	r2, #0
	bne.n	.L_0816fef4
	ldr	r3, [sp, #60]
	add	r5, sp, #72
	ldr	r0, [r3, #8]
	adds	r1, r5, #0
	bl	sub_0815e21c
	ldr	r1, [sp, #60]
	add	r6, sp, #84
	movs	r7, #36
	ldrsh	r0, [r1, r7]
	adds	r1, r6, #0
	bl	sub_0815e21c
	ldr	r2, [sp, #56]
	cmp	r2, #0
	bne.n	.L_0816fe44
	ldr	r2, [r5, #0]
	mov	r3, fp
	lsls	r2, r2, #16
	str	r2, [r3, #0]
	mov	r7, fp
	ldr	r3, [r5, #4]
	subs	r3, #32
	lsls	r3, r3, #16
	str	r3, [r7, #4]
	ldr	r1, [sp, #28]
	ldr	r0, [r6, #0]
	lsls	r0, r0, #16
	subs	r0, r0, r2
	b.n	.L_0816fe78
.L_0816fe44:
	ldr	r0, [sp, #60]
	ldr	r3, [r0, #4]
	cmp	r3, #0
	bne.n	.L_0816fe58
	ldr	r3, [r5, #0]
	mov	r1, fp
	subs	r3, #32
	lsls	r3, r3, #16
	str	r3, [r1, #0]
	b.n	.L_0816fe62
.L_0816fe58:
	ldr	r3, [r5, #0]
	mov	r2, fp
	adds	r3, #32
	lsls	r3, r3, #16
	str	r3, [r2, #0]
.L_0816fe62:
	ldr	r3, [r5, #4]
	mov	r7, fp
	subs	r3, #80
	lsls	r3, r3, #16
	str	r3, [r7, #4]
	ldr	r3, [r7, #0]
	ldr	r0, [r6, #0]
	ldr	r1, [sp, #28]
	subs	r0, #32
	lsls	r0, r0, #16
	subs	r0, r0, r3
.L_0816fe78:
	bl	sub_08002054
	str	r0, [r7, #12]
	ldr	r0, [r6, #4]
	mov	r1, fp
	ldr	r3, [r1, #4]
	subs	r0, #32
	lsls	r0, r0, #16
	subs	r0, r0, r3
	ldr	r1, [sp, #28]
	bl	sub_08002054
	mov	r2, fp
	str	r0, [r2, #16]
	movs	r0, #221
	bl	sub_081c0010
	mov	r5, fp
	movs	r3, #0
	mov	r9, r3
	adds	r5, #28
.L_0816fea2:
	bl	sub_08014878
	str	r0, [r5, #8]
	bl	sub_08014878
	movs	r3, #255
	ands	r3, r0
	adds	r3, #128
	str	r3, [r5, #20]
	bl	sub_08014878
	movs	r3, #1
	ands	r0, r3
	cmp	r0, #0
	beq.n	.L_0816fec6
	ldr	r3, [r5, #20]
	negs	r3, r3
	str	r3, [r5, #20]
.L_0816fec6:
	bl	sub_08014878
	movs	r7, #1
	movs	r3, #3
	add	r9, r7
	ands	r3, r0
	mov	r0, r9
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r0, #63
	bne.n	.L_0816fea2
	ldr	r3, [pc, #296]
	movs	r1, #0
	movs	r2, #128
	mov	r9, r1
	lsls	r2, r2, #2
	subs	r1, #1
.L_0816fee8:
	movs	r7, #1
	add	r9, r7
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r9, r2
	bne.n	.L_0816fee8
.L_0816fef4:
	ldr	r0, [sp, #48]
	ldr	r1, [sp, #28]
	cmp	r0, r1
	bne.n	.L_0816ffb0
	ldr	r7, [pc, #268]
	movs	r2, #0
	mov	r8, r2
.L_0816ff02:
	mov	r0, fp
	ldr	r3, [r0, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r7, #0]
	ldr	r3, [r0, #4]
	str	r3, [r7, #4]
	bl	sub_08014878
	adds	r6, r0, #0
	bl	sub_08014878
	movs	r1, #200
	lsls	r1, r1, #1
	bl	sub_0800206c
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r5, #32
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #7
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #16]
	bl	sub_08014878
	movs	r3, #7
	movs	r1, #1
	ands	r3, r0
	add	r8, r1
	adds	r3, #16
	mov	r2, r8
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r2, #128
	bne.n	.L_0816ff02
	movs	r1, #240
	ldr	r0, [sp, #52]
	ldr	r3, [pc, #172]
	lsls	r1, r1, #6
	ldr	r2, [pc, #172]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x22ee
	lsls	r2, r2, #7
	adds	r2, #168
	movs	r3, #16
	add	r2, fp
	str	r3, [r2, #0]
	ldr	r7, [sp, #60]
	movs	r1, #4
	movs	r3, #36
	ldrsh	r0, [r7, r3]
	bl	sub_08118088
	movs	r3, #8
	movs	r1, #36
	ldrsh	r0, [r7, r1]
	movs	r2, #5
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r3, #0
	bl	sub_0814cd48
	ldr	r2, [sp, #56]
	cmp	r2, #0
	bne.n	.L_0816ffa2
	movs	r0, #145
	bl	sub_081180e8
	b.n	.L_0816ffb0
.L_0816ffa2:
	movs	r0, #1
	negs	r0, r0
	bl	sub_081180e8
	movs	r0, #145
	bl	sub_081c0010
.L_0816ffb0:
	ldr	r3, [sp, #48]
	ldr	r7, [sp, #28]
	cmp	r3, r7
	ble.n	.L_0816ffba
	b.n	.L_08170124
.L_0816ffba:
	mov	r0, fp
	ldr	r3, [r0, #24]
	adds	r3, #1
	str	r3, [r0, #24]
	ldr	r3, [r0, #0]
	asrs	r3, r3, #17
	str	r3, [sp, #24]
	ldr	r3, [sp, #56]
	movs	r2, #6
	ldrsh	r1, [r0, r2]
	str	r1, [sp, #20]
	cmp	r3, #0
	bne.n	.L_0816ffe0
	ldr	r7, [sp, #48]
	lsls	r3, r7, #1
	adds	r3, #16
	str	r3, [sp, #32]
	movs	r3, #48
	b.n	.L_0816ffea
.L_0816ffe0:
	ldr	r0, [sp, #48]
	lsls	r3, r0, #1
	adds	r3, #16
	str	r3, [sp, #32]
	movs	r3, #64
.L_0816ffea:
	ldr	r1, [sp, #32]
	cmp	r1, r3
	ble.n	.L_08170018
	str	r3, [sp, #32]
	b.n	.L_0817005c
	.4byte 0x00000165
	.4byte 0x03000730
	.4byte 0x03000258
	.4byte 0x02010000
	.4byte 0x08143001
	.4byte 0x02014018
	.4byte 0x02014000
	.4byte 0x03000260
	.2byte 0x3f3f
	.2byte 0x3f3f
.L_08170018:
	movs	r2, #167
	lsls	r2, r2, #9
	adds	r2, #32
	ldr	r0, [pc, #780]
	ldr	r1, [sp, #32]
	bl	sub_0815b434
	ldr	r2, [sp, #56]
	cmp	r2, #1
	bne.n	.L_0817005c
	ldr	r7, [sp, #32]
	movs	r3, #0
	mov	r9, r3
	adds	r3, r7, #0
	muls	r3, r7
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	cmp	r3, #0
	beq.n	.L_0817005c
	ldr	r1, [pc, #744]
	movs	r0, #64
	mov	ip, r3
.L_08170046:
	ldrb	r2, [r1, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_08170052
	subs	r3, r0, r2
	strb	r3, [r1, #0]
.L_08170052:
	movs	r2, #1
	add	r9, r2
	adds	r1, #1
	cmp	r9, ip
	bne.n	.L_08170046
.L_0817005c:
	ldr	r3, [sp, #32]
	ldr	r0, [pc, #716]
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #20]
	bl	sub_0818caa8
	ldr	r7, [sp, #32]
	movs	r3, #0
	asrs	r7, r7, #31
	str	r7, [sp, #16]
	mov	r9, r3
	movs	r7, #3
.L_08170074:
	ldr	r0, [sp, #16]
	ldr	r1, [sp, #32]
	mov	r4, r9
	ands	r4, r7
	lsrs	r5, r0, #31
	str	r4, [sp, #12]
	adds	r5, r1, r5
	bl	sub_08014878
	adds	r6, r0, #0
	bl	sub_08002096
	asrs	r5, r5, #1
	adds	r2, r5, #0
	muls	r2, r0
	ldr	r3, [sp, #24]
	ldr	r4, [sp, #12]
	ldr	r0, [pc, #664]
	asrs	r2, r2, #16
	mov	r8, r2
	add	r8, r3
	ldrb	r3, [r0, r4]
	mov	r1, r8
	lsrs	r3, r3, #1
	subs	r1, r1, r3
	mov	sl, r0
	adds	r0, r6, #0
	mov	r8, r1
	bl	sub_08002090
	ldr	r6, [pc, #640]
	ldr	r4, [sp, #12]
	muls	r5, r0
	ldr	r2, [sp, #20]
	ldrb	r3, [r6, r4]
	asrs	r5, r5, #16
	adds	r5, r2, r5
	lsrs	r3, r3, #1
	subs	r5, r5, r3
	bl	sub_08014878
	ldr	r3, [pc, #624]
	ands	r0, r7
	ldrb	r2, [r3, r0]
	adds	r3, r7, #0
	orrs	r3, r2
	movs	r2, #1
	str	r2, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r0, #188
	bl	sub_08196404
	ldr	r4, [sp, #12]
	ldr	r2, [pc, #600]
	lsls	r3, r4, #1
	ldrh	r1, [r2, r3]
	movs	r3, #240
	lsls	r3, r3, #4
	mov	r0, sl
	add	r1, fp
	adds	r1, r1, r3
	ldrb	r3, [r0, r4]
	movs	r2, #192
	str	r3, [sp, #0]
	lsls	r2, r2, #18
	ldrb	r3, [r6, r4]
	adds	r2, #188
	str	r3, [sp, #4]
	ldr	r0, [sp, #52]
	ldr	r4, [r2, #0]
	adds	r3, r5, #0
	mov	r2, r8
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x20bc
	bl	sub_0801314c
	movs	r3, #1
	add	r9, r3
	mov	r0, r9
	cmp	r0, #4
	bne.n	.L_08170074
	mov	r0, fp
	movs	r1, #64
	movs	r2, #0
	bl	sub_08138086
.L_08170124:
	ldr	r1, [sp, #56]
	cmp	r1, #0
	bne.n	.L_081701c2
	ldr	r2, [sp, #48]
	ldr	r3, [sp, #28]
	cmp	r2, r3
	blt.n	.L_081701cc
	ldr	r6, [pc, #524]
	movs	r7, #0
	mov	r9, r7
.L_08170138:
	ldr	r3, [r6, #24]
	cmp	r3, #0
	blt.n	.L_081701b4
	asrs	r3, r3, #3
	adds	r5, r3, #2
	ldr	r0, [pc, #512]
	lsls	r1, r5, #1
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	str	r1, [sp, #8]
	asrs	r3, r3, #1
	movs	r4, #0
	mov	sl, r0
	mov	r8, r3
.L_08170154:
	ldr	r3, [sp, #8]
	mov	r2, sl
	subs	r3, #2
	ldrh	r1, [r2, r3]
	movs	r7, #2
	ldrsh	r2, [r6, r7]
	ldr	r3, [sp, #36]
	mov	r0, r8
	adds	r1, r3, r1
	subs	r2, r2, r0
	movs	r7, #6
	ldrsh	r3, [r6, r7]
	ldr	r0, [sp, #8]
	str	r4, [sp, #12]
	str	r0, [sp, #4]
	subs	r3, r3, r5
	str	r5, [sp, #0]
	ldr	r0, [sp, #52]
	ldr	r7, [sp, #40]
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x2280
	adds	r0, r6, #0
	movs	r1, #62
	lsls	r2, r2, #2
	bl	sub_08138086
	ldr	r4, [sp, #12]
	adds	r4, #1
	cmp	r4, #8
	bne.n	.L_08170154
	movs	r0, #6
	ldrsh	r3, [r6, r0]
	cmp	r3, #111
	ble.n	.L_081701ae
	ldr	r2, [r6, #16]
	negs	r2, r2
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #4
	cmp	r3, #0
	bge.n	.L_081701aa
	adds	r3, #63
.L_081701aa:
	asrs	r3, r3, #6
	str	r3, [r6, #16]
.L_081701ae:
	ldr	r3, [r6, #24]
	subs	r3, #1
	str	r3, [r6, #24]
.L_081701b4:
	movs	r1, #1
	movs	r2, #128
	add	r9, r1
	lsls	r2, r2, #2
	adds	r6, #28
	cmp	r9, r2
	bne.n	.L_08170138
.L_081701c2:
	ldr	r3, [sp, #48]
	ldr	r7, [sp, #28]
	cmp	r3, r7
	ble.n	.L_081701cc
	b.n	.L_081702e2
.L_081701cc:
	movs	r0, #32
	bl	sub_08014dac
	adds	r7, r0, #0
	movs	r0, #1
	bl	sub_081969f8
	ldr	r2, [pc, #364]
	ldr	r3, [sp, #64]
	adds	r6, r0, #0
	ands	r3, r2
	movs	r2, #5
	orrs	r3, r2
	ldr	r2, [pc, #356]
	ldr	r0, [sp, #56]
	ands	r3, r2
	movs	r2, #224
	lsls	r2, r2, #3
	orrs	r3, r2
	str	r3, [sp, #64]
	cmp	r0, #0
	bne.n	.L_081701fc
	movs	r3, #6
	b.n	.L_081701fe
.L_081701fc:
	movs	r3, #9
.L_081701fe:
	str	r3, [r6, #0]
	movs	r3, #6
	str	r3, [r6, #0]
	ldr	r3, [pc, #328]
	add	r1, sp, #64
	str	r1, [r6, #16]
	str	r3, [r6, #8]
	str	r7, [r6, #12]
	movs	r2, #0
	mov	r5, fp
	mov	r8, r1
	mov	r9, r2
	adds	r5, #28
.L_08170218:
	ldr	r3, [sp, #48]
	cmp	r3, r9
	blt.n	.L_081702ca
	ldr	r0, [sp, #48]
	mov	r3, r9
	adds	r3, #8
	cmp	r0, r3
	bge.n	.L_081702ca
	movs	r2, #0
	cmp	r0, r9
	bne.n	.L_08170232
	subs	r2, #16
	b.n	.L_08170246
.L_08170232:
	ldr	r1, [sp, #48]
	mov	r3, r9
	adds	r3, #4
	cmp	r1, r3
	blt.n	.L_08170246
	mov	r2, r9
	subs	r3, r1, r2
	lsls	r3, r3, #4
	subs	r3, #64
	negs	r2, r3
.L_08170246:
	ldr	r3, [r5, #24]
	movs	r0, #216
	lsls	r3, r3, #12
	lsls	r0, r0, #5
	adds	r0, #86
	add	r3, fp
	adds	r3, r3, r0
	mov	r1, r8
	str	r3, [r1, #4]
	str	r2, [r6, #20]
	bl	sub_08014de4
	mov	r2, fp
	ldr	r0, [r2, #0]
	ldr	r1, [r2, #4]
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	ldr	r3, [pc, #232]
	asrs	r0, r0, #1
	adds	r0, r0, r3
	adds	r1, r1, r3
	movs	r2, #0
	bl	sub_08015160
	ldr	r0, [r5, #8]
	bl	sub_080150e4
	ldr	r3, [r5, #8]
	ldr	r2, [r5, #20]
	adds	r3, r3, r2
	str	r3, [r5, #8]
	ldr	r3, [sp, #56]
	cmp	r3, #0
	bne.n	.L_081702a4
	movs	r2, #128
	lsls	r2, r2, #9
	movs	r0, #128
	adds	r1, r2, #0
	lsls	r0, r0, #11
	bl	sub_080151e4
	ldr	r0, [pc, #188]
	adds	r1, r7, #0
	movs	r2, #4
	bl	sub_08196958
	b.n	.L_081702c4
.L_081702a4:
	ldr	r0, [sp, #32]
	movs	r2, #128
	lsls	r1, r0, #2
	adds	r1, r1, r0
	lsls	r1, r1, #3
	subs	r1, r1, r0
	lsls	r1, r1, #3
	lsls	r0, r0, #11
	lsls	r2, r2, #9
	bl	sub_080151e4
	ldr	r0, [pc, #160]
	adds	r1, r7, #0
	movs	r2, #4
	bl	sub_08196958
.L_081702c4:
	adds	r0, r6, #0
	bl	sub_08196a7c
.L_081702ca:
	movs	r1, #1
	add	r9, r1
	mov	r2, r9
	adds	r5, #28
	cmp	r2, #40
	bne.n	.L_08170218
	adds	r0, r6, #0
	bl	sub_08013164
	adds	r0, r7, #0
	bl	sub_08013164
.L_081702e2:
	movs	r0, #4
	movs	r1, #4
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	movs	r3, #1
	add	r2, fp
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [sp, #48]
	adds	r3, #1
	str	r3, [sp, #48]
	cmp	r3, #50
	beq.n	.L_0817030c
	b.n	.L_0816fe02
.L_0817030c:
	ldr	r0, [pc, #80]
	bl	sub_08014644
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #96
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x02010000
	.4byte 0x08197492
	.4byte 0x08197498
	.4byte 0x08198bee
	.4byte 0x08197486
	.4byte 0x02014000
	.4byte 0x08197410
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x08199268
	.4byte 0xffc00000
	.4byte 0x081991b0
	.4byte 0x081991c0
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #124
	str	r0, [sp, #88]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #96]
	movs	r6, #252
	str	r0, [sp, #84]
	lsls	r6, r6, #6
	ldr	r1, [r3, #92]
	ldr	r5, [pc, #80]
	str	r1, [sp, #80]
	ldr	r3, [r3, #100]
	str	r3, [sp, #72]
	bl	sub_0813ba50
	movs	r0, #0
	bl	sub_081435e0
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	mov	r2, sp
	adds	r2, #100
	adds	r1, r2, #0
	movs	r0, #0
	str	r2, [sp, #68]
	bl	sub_08144aac
	ldr	r3, [sp, #80]
	ldr	r0, [pc, #40]
	adds	r1, r3, r6
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r7, [sp, #80]
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r1, r7, r2
	ldr	r0, [pc, #24]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	ldr	r0, [pc, #20]
	ldr	r1, [sp, #72]
	b.n	.L_081703e8
	.4byte 0x00001010
	.4byte 0x02014000
	.4byte 0x0000013e
	.4byte 0x000000b7
	.2byte 0x0134
	.2byte 0x0000
.L_081703e8:
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	adds	r1, r5, #0
	ldr	r0, [pc, #600]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	movs	r6, #184
	movs	r3, #192
	lsls	r3, r3, #4
	lsls	r6, r6, #6
	adds	r3, #86
	adds	r6, #22
	adds	r1, r5, r3
	ldr	r0, [pc, #580]
	movs	r2, #1
	movs	r3, #0
	adds	r5, r5, r6
	bl	sub_08157cf4
	ldr	r0, [pc, #572]
	adds	r1, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r0, #239
	movs	r1, #238
	lsls	r0, r0, #7
	lsls	r1, r1, #7
	adds	r2, r7, r0
	movs	r3, #2
	adds	r1, #132
	str	r3, [r2, #0]
	adds	r2, r7, r1
	movs	r3, #75
	movs	r1, #200
	str	r3, [r2, #0]
	ldr	r0, [pc, #540]
	lsls	r1, r1, #4
	bl	sub_080145a8
	movs	r2, #0
	adds	r3, r7, #0
	mov	r9, r2
	adds	r3, #24
	subs	r2, #1
.L_0817044c:
	movs	r6, #1
	add	r9, r6
	mov	r7, r9
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r7, #64
	bne.n	.L_0817044c
	ldr	r1, [sp, #80]
	movs	r2, #168
	movs	r0, #0
	lsls	r2, r2, #2
	mov	r9, r0
	adds	r5, r1, r2
.L_08170466:
	bl	sub_08014878
	movs	r3, #127
	ands	r3, r0
	str	r3, [r5, #0]
	bl	sub_08014878
	movs	r3, #7
	ands	r3, r0
	adds	r3, #56
	str	r3, [r5, #4]
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	negs	r3, r3
	str	r3, [r5, #24]
	movs	r3, #1
	add	r9, r3
	mov	r6, r9
	adds	r5, #28
	cmp	r6, #16
	bne.n	.L_08170466
	ldr	r5, [sp, #80]
	movs	r7, #0
	mov	r9, r7
.L_0817049a:
	bl	sub_08014878
	movs	r3, #31
	ands	r3, r0
	adds	r3, #28
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	bl	sub_08014878
	movs	r1, #48
	bl	sub_0800206c
	adds	r0, #56
	lsls	r0, r0, #16
	str	r0, [r5, #4]
	bl	sub_08014878
	movs	r3, #255
	ands	r3, r0
	subs	r3, #128
	lsls	r3, r3, #12
	str	r3, [r5, #12]
	bl	sub_08014878
	movs	r2, #127
	ands	r2, r0
	movs	r3, #16
	subs	r3, r3, r2
	movs	r1, #1
	lsls	r3, r3, #10
	mov	r0, r9
	add	r9, r1
	str	r3, [r5, #16]
	mov	r2, r9
	mvns	r3, r0
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r2, #32
	bne.n	.L_0817049a
	movs	r3, #0
	mov	r9, r3
	ldr	r3, [pc, #364]
	movs	r2, #128
	subs	r1, #2
	lsls	r2, r2, #2
.L_081704f4:
	movs	r6, #1
	add	r9, r6
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r9, r2
	bne.n	.L_081704f4
	ldr	r0, [pc, #348]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	movs	r2, #128
	ldr	r3, [pc, #340]
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x20f0
	ldr	r7, [sp, #80]
	lsls	r0, r0, #7
	adds	r0, #240
	adds	r3, r7, r0
	ldr	r0, [r3, #0]
	bl	sub_0814cc4c
	movs	r0, #1
	bl	sub_08013560
	movs	r1, #0
	str	r1, [sp, #76]
.L_0817052e:
	ldr	r3, [pc, #312]
	movs	r2, #3
	ldr	r3, [r3, #12]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08170548
	ldr	r2, [sp, #76]
	cmp	r2, #10
	ble.n	.L_08170548
	cmp	r2, #107
	bgt.n	.L_08170554
	movs	r3, #107
	str	r3, [sp, #76]
.L_08170548:
	ldr	r6, [sp, #76]
	cmp	r6, #107
	bne.n	.L_08170554
	movs	r0, #212
	bl	sub_081c0010
.L_08170554:
	ldr	r7, [sp, #76]
	cmp	r7, #148
	bne.n	.L_08170560
	movs	r0, #134
	bl	sub_081180e8
.L_08170560:
	ldr	r0, [sp, #76]
	cmp	r0, #0
	bne.n	.L_0817058a
	movs	r1, #128
	lsls	r1, r1, #14
	str	r1, [sp, #60]
	movs	r2, #144
	ldr	r6, [pc, #252]
	movs	r1, #201
	lsls	r2, r2, #15
	movs	r3, #144
	lsls	r1, r1, #1
	str	r2, [sp, #64]
	lsls	r3, r3, #12
	movs	r0, #2
	adds	r1, #255
	movs	r2, #2
	str	r3, [sp, #52]
	str	r6, [sp, #56]
	bl	sub_08152404
.L_0817058a:
	ldr	r7, [sp, #76]
	cmp	r7, #108
	bne.n	.L_081705bc
	ldr	r0, [sp, #80]
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #220
	adds	r3, r0, r1
	ldr	r0, [r3, #0]
	bl	sub_08020048
	movs	r6, #238
	ldr	r2, [sp, #80]
	lsls	r6, r6, #7
	adds	r6, #224
	adds	r3, r2, r6
	ldr	r0, [r3, #0]
	bl	sub_08020048
	movs	r1, #164
	movs	r0, #11
	lsls	r1, r1, #2
	movs	r2, #2
	bl	sub_08152404
.L_081705bc:
	ldr	r7, [sp, #76]
	cmp	r7, #107
	bgt.n	.L_081705c4
	b.n	.L_081706e4
.L_081705c4:
	ldr	r0, [sp, #60]
	ldr	r3, [sp, #64]
	ldr	r2, [sp, #52]
	ldr	r7, [sp, #52]
	ldr	r6, [sp, #56]
	adds	r1, r0, r2
	adds	r2, r3, r6
	adds	r0, r7, #0
	lsls	r3, r7, #4
	subs	r3, r3, r0
	lsls	r3, r3, #2
	subs	r3, r3, r0
	str	r1, [sp, #60]
	str	r2, [sp, #64]
	cmp	r3, #0
	bge.n	.L_081705e6
	adds	r3, #63
.L_081705e6:
	ldr	r6, [sp, #56]
	asrs	r3, r3, #6
	str	r3, [sp, #52]
	lsls	r3, r6, #4
	subs	r3, r3, r6
	lsls	r3, r3, #2
	subs	r3, r3, r6
	cmp	r3, #0
	bge.n	.L_081705fa
	adds	r3, #63
.L_081705fa:
	asrs	r3, r3, #6
	movs	r0, #4
	str	r3, [sp, #56]
	bl	sub_0816442c
	ldr	r7, [sp, #76]
	cmp	r7, #108
	bne.n	.L_08170624
	ldr	r3, [pc, #80]
	movs	r0, #0
	movs	r1, #1
	movs	r2, #128
	mov	r9, r0
	negs	r1, r1
	lsls	r2, r2, #2
.L_08170618:
	movs	r6, #1
	add	r9, r6
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r9, r2
	bne.n	.L_08170618
.L_08170624:
	ldr	r7, [sp, #76]
	subs	r7, #108
	cmp	r7, #23
	bhi.n	.L_081706e4
	lsrs	r3, r7, #31
	adds	r3, r7, r3
	asrs	r3, r3, #1
	movs	r2, #10
	subs	r2, r2, r3
	mov	r8, r2
	cmp	r2, #0
	ble.n	.L_081706e4
	ldr	r1, [sp, #60]
	movs	r0, #0
	lsrs	r3, r1, #31
	adds	r3, r3, r1
	asrs	r3, r3, #1
	mov	r9, r0
	mov	sl, r3
	b.n	.L_081706d8
	.4byte 0x00000192
	.4byte 0x00000188
	.4byte 0x0000017e
	.4byte 0x08143001
	.4byte 0x02010018
	.4byte 0x00000150
	.4byte 0x03000730
	.4byte 0x03001150
	.2byte 0x0000
	.2byte 0xfffe
.L_08170670:
	.2byte 0x2280
	lsls	r3, r7, #5
	lsls	r2, r2, #1
	add	r3, r9
	adds	r2, #255
	ands	r3, r2
	lsls	r6, r3, #3
	ldr	r2, [pc, #812]
	subs	r6, r6, r3
	lsls	r6, r6, #2
	adds	r6, r6, r2
	bl	sub_08014878
	adds	r2, r0, #0
	str	r2, [sp, #16]
	bl	sub_08014878
	ldr	r2, [sp, #16]
	movs	r5, #31
	ands	r5, r0
	adds	r0, r2, #0
	bl	sub_08002096
	adds	r5, #16
	adds	r3, r5, #0
	muls	r3, r0
	ldr	r2, [sp, #16]
	movs	r0, #128
	lsls	r0, r0, #13
	add	r3, sl
	adds	r3, r3, r0
	str	r3, [r6, #0]
	adds	r0, r2, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	ldr	r1, [sp, #64]
	ldr	r2, [pc, #752]
	adds	r3, r3, r1
	adds	r3, r3, r2
	str	r3, [r6, #4]
	movs	r3, #0
	str	r3, [r6, #12]
	str	r3, [r6, #16]
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	str	r3, [r6, #24]
	movs	r3, #1
	add	r9, r3
.L_081706d8:
	mov	r0, r8
	movs	r1, #5
	bl	sub_08002054
	cmp	r9, r0
	bne.n	.L_08170670
.L_081706e4:
	ldr	r3, [sp, #76]
	subs	r3, #64
	cmp	r3, #3
	bhi.n	sub_081706fa
	movs	r1, #128
	ldr	r3, [pc, #708]
	ldr	r0, [sp, #84]
	lsls	r1, r1, #7
	ldr	r2, [pc, #704]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9e13
	cmp	r6, #103
	ble.n	.L_08170706
	ldr	r0, [pc, #696]
	bl	sub_0815f0a0
.L_08170706:
	ldr	r7, [sp, #76]
	cmp	r7, #64
	bne.n	sub_08170720
	ldr	r0, [pc, #688]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #684]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9813
	cmp	r0, #63
	bgt.n	.L_08170728
	b.n	.L_081708d6
.L_08170728:
	cmp	r0, #111
	bgt.n	sub_08170806
	cmp	r0, #0
	bge.n	.L_08170732
	adds	r0, #3
.L_08170732:
	movs	r1, #3
	asrs	r0, r0, #2
	bl	sub_08002064
	ldr	r2, [sp, #76]
	adds	r1, r0, #0
	lsls	r0, r2, #11
	str	r1, [sp, #20]
	bl	sub_08002096
	ldr	r6, [sp, #76]
	movs	r3, #108
	eors	r3, r6
	ldr	r1, [sp, #20]
	negs	r2, r3
	lsls	r0, r0, #3
	asrs	r0, r0, #16
	orrs	r2, r3
	adds	r7, r0, #0
	lsrs	r2, r2, #31
	ldr	r0, [sp, #68]
	movs	r6, #1
	subs	r6, r6, r2
	lsls	r5, r1, #1
	adds	r5, r5, r1
	lsls	r6, r6, #2
	ldr	r1, [sp, #80]
	adds	r6, r6, r0
	movs	r3, #96
	movs	r0, #64
	movs	r2, #224
	str	r3, [sp, #0]
	str	r0, [sp, #4]
	lsls	r2, r2, #3
	lsls	r5, r5, #11
	mov	r9, r2
	adds	r5, r1, r5
	subs	r7, #16
	add	r5, r9
	ldr	r0, [sp, #84]
	mov	r8, r3
	adds	r1, r5, #0
	adds	r2, r7, #0
	ldr	r4, [r6, #0]
	movs	r3, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2234
	mov	r1, r8
	str	r1, [sp, #0]
	str	r2, [sp, #4]
	ldr	r0, [sp, #84]
	ldr	r4, [r6, #0]
	adds	r1, r5, #0
	mov	sl, r2
	movs	r3, #64
	adds	r2, r7, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9b13
	movs	r1, #3
	lsrs	r0, r3, #31
	adds	r0, r3, r0
	asrs	r0, r0, #1
	bl	sub_08002064
	ldr	r7, [sp, #76]
	movs	r1, #128
	adds	r3, r0, #0
	lsls	r1, r1, #8
	lsls	r0, r7, #12
	adds	r0, r0, r1
	str	r3, [sp, #12]
	bl	sub_08002096
	ldr	r3, [sp, #12]
	ldr	r2, [sp, #80]
	lsls	r0, r0, #3
	lsls	r5, r3, #1
	asrs	r7, r0, #16
	adds	r5, r5, r3
	movs	r0, #64
	mov	r3, r8
	str	r3, [sp, #0]
	str	r0, [sp, #4]
	lsls	r5, r5, #11
	adds	r5, r2, r5
	add	r5, r9
	ldr	r4, [r6, #0]
	adds	r1, r5, #0
	adds	r2, r7, #0
	movs	r3, #0
	ldr	r0, [sp, #84]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4641
	mov	r2, sl
	str	r1, [sp, #0]
	str	r2, [sp, #4]
	ldr	r0, [sp, #84]
	ldr	r4, [r6, #0]
	adds	r1, r5, #0
	adds	r2, r7, #0
	movs	r3, #64
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9d14
	movs	r3, #0
	mov	r9, r3
.L_0817080c:
	ldr	r3, [r5, #24]
	cmp	r3, #15
	bhi.n	.L_0817085a
	adds	r1, r3, #0
	cmp	r3, #0
	bge.n	.L_0817081a
	adds	r1, r3, #3
.L_0817081a:
	ldr	r7, [sp, #80]
	asrs	r1, r1, #2
	lsls	r1, r1, #11
	movs	r0, #158
	adds	r1, r7, r1
	lsls	r0, r0, #7
	adds	r1, r1, r0
	mov	r6, r9
	movs	r0, #32
	movs	r4, #1
	movs	r7, #64
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	ands	r4, r6
	movs	r6, #6
	ldrsh	r3, [r5, r6]
	str	r0, [sp, #0]
	str	r7, [sp, #4]
	ldr	r0, [sp, #68]
	lsls	r4, r4, #2
	subs	r3, #32
	ldr	r4, [r4, r0]
	subs	r2, #16
	ldr	r0, [sp, #84]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c28
	movs	r1, #62
	ldr	r2, [pc, #372]
	bl	sub_08138086
	ldr	r3, [r5, #24]
.L_0817085a:
	adds	r3, #1
	str	r3, [r5, #24]
	ldr	r1, [sp, #76]
	cmp	r1, #108
	bne.n	.L_0817087a
	ldr	r3, [r5, #12]
	movs	r2, #128
	lsls	r2, r2, #10
	adds	r3, r3, r2
	str	r3, [r5, #12]
	ldr	r3, [r5, #16]
	cmp	r3, #0
	bge.n	.L_08170876
	adds	r3, #3
.L_08170876:
	asrs	r3, r3, #2
	str	r3, [r5, #16]
.L_0817087a:
	ldr	r3, [sp, #76]
	cmp	r3, #107
	bgt.n	.L_081708ca
	ldr	r3, [r5, #24]
	cmp	r3, #16
	bne.n	.L_0817088a
	movs	r3, #0
	str	r3, [r5, #24]
.L_0817088a:
	cmp	r3, #0
	bne.n	.L_081708ca
	bl	sub_08014878
	movs	r3, #31
	ands	r3, r0
	adds	r3, #28
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	bl	sub_08014878
	movs	r1, #48
	bl	sub_0800206c
	adds	r0, #56
	lsls	r0, r0, #16
	str	r0, [r5, #4]
	bl	sub_08014878
	movs	r3, #255
	ands	r3, r0
	subs	r3, #128
	lsls	r3, r3, #12
	str	r3, [r5, #12]
	bl	sub_08014878
	movs	r2, #127
	ands	r2, r0
	movs	r3, #16
	subs	r3, r3, r2
	lsls	r3, r3, #10
	str	r3, [r5, #16]
.L_081708ca:
	movs	r6, #1
	add	r9, r6
	mov	r7, r9
	adds	r5, #28
	cmp	r7, #24
	bne.n	.L_0817080c
.L_081708d6:
	ldr	r0, [sp, #76]
	cmp	r0, #64
	bne.n	.L_081708e2
	movs	r0, #163
	bl	sub_081c0010
.L_081708e2:
	movs	r1, #0
	mov	r9, r1
	movs	r5, #75
.L_081708e8:
	ldr	r2, [sp, #76]
	cmp	r2, r5
	bne.n	.L_081708f4
	movs	r0, #134
	bl	sub_081c0010
.L_081708f4:
	movs	r3, #1
	add	r9, r3
	mov	r6, r9
	adds	r5, #4
	cmp	r6, #8
	bne.n	.L_081708e8
	ldr	r7, [sp, #76]
	cmp	r7, #10
	bne.n	.L_0817093e
	ldr	r0, [sp, #80]
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #168
	adds	r3, r0, r1
	str	r6, [r3, #0]
	ldr	r6, [sp, #88]
	movs	r2, #0
	ldr	r3, [r6, #20]
	mov	r9, r2
	cmp	r3, #0
	beq.n	.L_0817093e
	movs	r6, #16
	movs	r5, #36
.L_08170922:
	ldr	r7, [sp, #88]
	mov	r3, r9
	ldrsh	r0, [r5, r7]
	movs	r2, #5
	movs	r1, #7
	str	r6, [sp, #0]
	bl	sub_0814cd48
	ldr	r3, [r7, #20]
	movs	r2, #1
	add	r9, r2
	adds	r5, #2
	cmp	r9, r3
	bne.n	.L_08170922
.L_0817093e:
	ldr	r3, [sp, #76]
	cmp	r3, #107
	ble.n	.L_08170a02
	ldr	r5, [pc, #100]
	movs	r6, #0
	mov	r9, r6
.L_0817094a:
	ldr	r3, [r5, #24]
	cmp	r3, #35
	bhi.n	.L_081709f4
	lsrs	r0, r3, #31
	adds	r0, r3, r0
	movs	r1, #9
	asrs	r0, r0, #1
	bl	sub_08002064
	ldr	r2, [pc, #108]
	lsls	r3, r0, #1
	ldrh	r1, [r2, r3]
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	ldr	r3, [pc, #104]
	ldr	r7, [pc, #104]
	ldrb	r4, [r3, r0]
	movs	r6, #6
	ldrsh	r3, [r5, r6]
	lsrs	r0, r4, #1
	subs	r3, r3, r0
	adds	r1, r1, r7
	subs	r2, r2, r0
	str	r4, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [sp, #84]
	ldr	r4, [sp, #100]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c28
	movs	r1, #58
	movs	r2, #0
	bl	sub_08138086
	mov	r7, r9
	movs	r3, #1
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_081709dc
	ldr	r3, [r5, #12]
	ldr	r0, [pc, #60]
	movs	r1, #128
	adds	r3, r3, r0
	str	r3, [r5, #12]
	ldr	r3, [r5, #16]
	lsls	r1, r1, #5
	adds	r3, r3, r1
	b.n	.L_081709ec
	movs	r0, r0
	.4byte 0x02010000
	.4byte 0xfff80000
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.4byte 0x00000167
	.4byte 0x00000148
	.4byte 0x03000730
	.4byte 0xffffc000
	.4byte 0x0819744c
	.4byte 0x0819745e
	.4byte 0x02016e16
	.2byte 0xf800
	.2byte 0xffff
.L_081709dc:
	ldr	r3, [r5, #12]
	ldr	r2, [pc, #872]
	movs	r6, #128
	adds	r3, r3, r2
	str	r3, [r5, #12]
	ldr	r3, [r5, #16]
	lsls	r6, r6, #4
	adds	r3, r3, r6
.L_081709ec:
	str	r3, [r5, #16]
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
.L_081709f4:
	movs	r7, #1
	movs	r0, #128
	add	r9, r7
	lsls	r0, r0, #2
	adds	r5, #28
	cmp	r9, r0
	bne.n	.L_0817094a
.L_08170a02:
	ldr	r1, [sp, #76]
	cmp	r1, #63
	ble.n	.L_08170a0a
	b.n	.L_08170cee
.L_08170a0a:
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #238
	movs	r6, #192
	lsls	r0, r0, #7
	movs	r2, #0
	movs	r3, #12
	lsls	r6, r6, #13
	movs	r7, #10
	adds	r0, #220
	movs	r1, #24
	str	r3, [sp, #44]
	str	r6, [sp, #40]
	str	r7, [sp, #36]
	str	r0, [sp, #32]
	str	r1, [sp, #28]
	str	r2, [sp, #24]
	mov	r9, r2
.L_08170a30:
	mov	r2, r9
	lsls	r2, r2, #4
	mov	r8, r2
	ldr	r6, [sp, #76]
	mov	r3, r8
	adds	r3, #5
	cmp	r6, r3
	bne.n	.L_08170a46
	movs	r0, #212
	bl	sub_081c0010
.L_08170a46:
	ldr	r7, [sp, #76]
	mov	r3, r8
	adds	r3, #9
	cmp	r7, r3
	bne.n	.L_08170a72
	ldr	r0, [sp, #80]
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #168
	adds	r3, r0, r1
	movs	r2, #8
	str	r2, [r3, #0]
	movs	r1, #240
	ldr	r3, [pc, #744]
	ldr	r0, [sp, #84]
	lsls	r1, r1, #6
	ldr	r2, [pc, #744]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2090
	bl	sub_081c0010
.L_08170a72:
	ldr	r2, [sp, #76]
	cmp	r2, r8
	bge.n	.L_08170a7a
	b.n	.L_08170c1c
.L_08170a7a:
	mov	r3, r8
	subs	r5, r2, r3
	lsls	r3, r5, #4
	adds	r2, r3, #0
	ldr	r3, [pc, #720]
	subs	r2, #16
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	str	r3, [sp, #92]
	str	r4, [sp, #96]
	cmp	r2, #131
	ble.n	.L_08170a94
	movs	r2, #132
.L_08170a94:
	add	r1, sp, #108
	movs	r3, #0
	str	r3, [r1, #12]
	str	r3, [r1, #4]
	ldr	r6, [sp, #28]
	lsls	r3, r6, #17
	str	r3, [r1, #0]
	lsls	r3, r2, #16
	str	r3, [r1, #8]
	ldr	r2, [sp, #80]
	ldr	r7, [sp, #32]
	movs	r3, #0
	ldr	r0, [r7, r2]
	add	r2, sp, #92
	bl	sub_08020010
	cmp	r5, #9
	bne.n	sub_08170ad0
	ldr	r6, [sp, #80]
	movs	r1, #128
	ldr	r3, [r7, r6]
	ldr	r7, [pc, #664]
	ldrh	r0, [r3, #8]
	lsls	r1, r1, #2
	lsls	r0, r0, #22
	lsrs	r0, r0, #17
	adds	r0, r0, r7
	ldr	r3, [pc, #656]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9809
	ldr	r1, [sp, #76]
	str	r0, [sp, #48]
	cmp	r1, r0
	bne.n	.L_08170b3a
	ldr	r3, [sp, #40]
	ldr	r6, [sp, #24]
	ldr	r0, [pc, #640]
	movs	r2, #0
	mov	fp, r2
	mov	sl, r3
	adds	r7, r6, r0
.L_08170ae8:
	bl	sub_08014878
	movs	r3, #254
	lsls	r3, r3, #7
	adds	r3, #255
	adds	r6, r0, #0
	ands	r6, r3
	bl	sub_08014878
	movs	r3, #255
	adds	r5, r0, #0
	ands	r5, r3
	movs	r3, #224
	mov	r1, sl
	lsls	r3, r3, #15
	str	r1, [r7, #0]
	str	r3, [r7, #4]
	adds	r0, r6, #0
	bl	sub_08002096
	negs	r0, r0
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #5
	str	r3, [r7, #16]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #5
	str	r3, [r7, #12]
	movs	r3, #0
	str	r3, [r7, #24]
	movs	r2, #1
	movs	r3, #128
	add	fp, r2
	lsls	r3, r3, #1
	adds	r7, #28
	cmp	fp, r3
	bne.n	.L_08170ae8
.L_08170b3a:
	ldr	r6, [sp, #76]
	ldr	r7, [sp, #48]
	cmp	r6, r7
	blt.n	.L_08170c1c
	mov	r3, r8
	adds	r3, #18
	cmp	r6, r3
	bge.n	sub_08170b7c
	adds	r2, r6, #0
	add	r2, r9
	cmp	r2, #0
	bge.n	.L_08170b54
	adds	r2, #3
.L_08170b54:
	movs	r3, #3
	asrs	r2, r2, #2
	ands	r2, r3
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r1, r3, #4
	ldr	r0, [pc, #512]
	subs	r1, r1, r3
	movs	r3, #24
	lsls	r1, r1, #6
	str	r3, [sp, #0]
	movs	r3, #112
	adds	r1, r1, r0
	str	r3, [sp, #4]
	ldr	r4, [sp, #100]
	ldr	r0, [sp, #84]
	ldr	r2, [sp, #44]
	movs	r3, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9913
	ldr	r2, [sp, #48]
	cmp	r1, r2
	blt.n	.L_08170c1c
	movs	r3, #0
	mov	fp, r3
	movs	r7, #3
.L_08170b8a:
	mov	r4, fp
	ands	r4, r7
	str	r4, [sp, #8]
	bl	sub_08014878
	movs	r5, #7
	ldr	r4, [sp, #8]
	ands	r5, r0
	ldr	r0, [pc, #460]
	ldr	r6, [sp, #28]
	ldrb	r3, [r0, r4]
	adds	r5, r6, r5
	lsrs	r3, r3, #1
	subs	r5, r5, r3
	mov	sl, r0
	bl	sub_08014878
	ldr	r1, [pc, #444]
	ldr	r4, [sp, #8]
	movs	r6, #63
	ldrb	r3, [r1, r4]
	ands	r6, r0
	lsrs	r3, r3, #1
	mov	r8, r1
	subs	r6, r6, r3
	bl	sub_08014878
	ldr	r3, [pc, #428]
	ands	r0, r7
	ldrb	r2, [r3, r0]
	ldr	r0, [sp, #88]
	adds	r3, r7, #0
	orrs	r3, r2
	ldr	r1, [pc, #420]
	ldr	r2, [r0, #24]
	movs	r0, #188
	ldrb	r2, [r1, r2]
	movs	r1, #7
	str	r2, [sp, #0]
	movs	r2, #7
	bl	sub_08196404
	ldr	r4, [sp, #8]
	ldr	r2, [pc, #404]
	lsls	r3, r4, #1
	mov	r0, sl
	ldrh	r1, [r2, r3]
	ldrb	r3, [r0, r4]
	ldr	r2, [pc, #400]
	str	r3, [sp, #0]
	adds	r1, r1, r2
	mov	r2, r8
	ldrb	r3, [r2, r4]
	adds	r6, #40
	str	r3, [sp, #4]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #188
	subs	r5, #4
	ldr	r4, [r3, #0]
	ldr	r0, [sp, #84]
	adds	r3, r6, #0
	adds	r2, r5, #0
	movs	r6, #1
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x44b3
	movs	r0, #188
	bl	sub_0801314c
	mov	r0, fp
	cmp	r0, #2
	bne.n	.L_08170b8a
.L_08170c1c:
	ldr	r1, [sp, #44]
	ldr	r2, [sp, #40]
	movs	r3, #128
	adds	r1, #32
	lsls	r3, r3, #14
	ldr	r6, [sp, #36]
	adds	r2, r2, r3
	ldr	r7, [sp, #32]
	ldr	r0, [sp, #28]
	str	r1, [sp, #44]
	ldr	r1, [sp, #24]
	str	r2, [sp, #40]
	movs	r3, #1
	movs	r2, #224
	adds	r6, #16
	lsls	r2, r2, #5
	add	r9, r3
	str	r6, [sp, #36]
	adds	r7, #4
	adds	r0, #32
	adds	r1, r1, r2
	mov	r6, r9
	str	r7, [sp, #32]
	str	r0, [sp, #28]
	str	r1, [sp, #24]
	cmp	r6, #2
	beq.n	.L_08170c54
	b.n	.L_08170a30
.L_08170c54:
	movs	r0, #188
	movs	r1, #3
	bl	sub_081963ec
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #188
	ldr	r3, [r3, #0]
	ldr	r7, [sp, #68]
	str	r3, [r7, #4]
	ldr	r0, [sp, #76]
	cmp	r0, #69
	bgt.n	.L_08170cee
	ldr	r5, [pc, #240]
	movs	r1, #0
	mov	r9, r1
.L_08170c74:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	blt.n	.L_08170ce0
	mov	r0, r9
	movs	r1, #3
	bl	sub_08002064
	ldr	r3, [r5, #16]
	adds	r4, r0, #1
	movs	r6, #4
	cmp	r3, #0
	bgt.n	.L_08170c8e
	movs	r6, #0
.L_08170c8e:
	ldr	r2, [pc, #240]
	lsls	r0, r4, #1
	subs	r3, r0, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #72]
	adds	r1, r2, r1
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	lsrs	r3, r4, #31
	adds	r3, r4, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	movs	r7, #6
	ldrsh	r3, [r5, r7]
	str	r0, [sp, #4]
	str	r4, [sp, #0]
	ldr	r0, [sp, #68]
	subs	r3, r3, r4
	ldr	r4, [r6, r0]
	ldr	r0, [sp, #84]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2280
	adds	r0, r5, #0
	movs	r1, #62
	lsls	r2, r2, #7
	bl	sub_08138086
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
	ldr	r3, [r5, #16]
	cmp	r3, #0
	ble.n	.L_08170ce0
	movs	r1, #6
	ldrsh	r3, [r5, r1]
	cmp	r3, #104
	ble.n	.L_08170ce0
	movs	r3, #1
	negs	r3, r3
	str	r3, [r5, #24]
.L_08170ce0:
	movs	r2, #1
	movs	r3, #128
	add	r9, r2
	lsls	r3, r3, #2
	adds	r5, #28
	cmp	r9, r3
	bne.n	.L_08170c74
.L_08170cee:
	movs	r0, #8
	movs	r1, #8
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r7, #240
	ldr	r6, [sp, #80]
	lsls	r7, r7, #7
	adds	r7, #232
	adds	r2, r6, r7
	movs	r3, #1
	movs	r0, #1
	str	r3, [r2, #0]
	bl	sub_08013560
	ldr	r0, [sp, #76]
	adds	r0, #1
	str	r0, [sp, #76]
	cmp	r0, #170
	beq.n	.L_08170d1c
	bl	.L_0817052e
.L_08170d1c:
	ldr	r0, [pc, #100]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	ldr	r1, [sp, #60]
	ldr	r2, [sp, #64]
	movs	r0, #4
	bl	sub_0816467c
	movs	r2, #238
	lsls	r2, r2, #7
	movs	r1, #0
	adds	r2, #220
	mov	r9, r1
	adds	r5, r6, r2
	b.n	.L_08170d88
	movs	r0, r0
	.4byte 0xfffff000
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.4byte 0x08196e94
	.4byte 0x06010e00
	.4byte 0x03000258
	.4byte 0x02010000
	.4byte 0x02014c56
	.4byte 0x08197492
	.4byte 0x08197498
	.4byte 0x08198bf4
	.4byte 0x08198bf8
	.4byte 0x08197486
	.4byte 0x02014000
	.4byte 0x08197410
	.2byte 0x3001
	.2byte 0x0814
.L_08170d88:
	ldmia	r5!, {r0}
	bl	sub_08020048
	movs	r3, #1
	add	r9, r3
	mov	r6, r9
	cmp	r6, #11
	bne.n	.L_08170d88
	bl	sub_08143bb8
	add	sp, #124
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
