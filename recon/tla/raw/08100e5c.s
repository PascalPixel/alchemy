.syntax unified
	.thumb
	.set sub_0800206c, 0x0800206c
	.set sub_0801314c, 0x0801314c
	.set sub_08013164, 0x08013164
	.set sub_08013560, 0x08013560
	.set sub_080143ac, 0x080143ac
	.set sub_08014cc0, 0x08014cc0
	.set sub_08014dac, 0x08014dac
	.set sub_08016ce4, 0x08016ce4
	.set sub_08038010, 0x08038010
	.set sub_080380c8, 0x080380c8
	.set sub_08038268, 0x08038268
	.set sub_08038290, 0x08038290
	.set sub_080383b8, 0x080383b8
	.set sub_080383c0, 0x080383c0
	.set sub_080383e8, 0x080383e8
	.set sub_080383f0, 0x080383f0
	.set sub_080383f8, 0x080383f8
	.set sub_080ad008, 0x080ad008
	.set sub_080ad100, 0x080ad100
	.set sub_080ad158, 0x080ad158
	.set sub_080ad160, 0x080ad160
	.set sub_080ad168, 0x080ad168
	.set sub_080ad170, 0x080ad170
	.set sub_080ad178, 0x080ad178
	.set sub_080c85d0, 0x080c85d0
	.set sub_080f80a8, 0x080f80a8
	.set sub_080f80c4, 0x080f80c4
	.set sub_080f80e0, 0x080f80e0
	.set sub_080f811c, 0x080f811c
	.set sub_080f8840, 0x080f8840
	.set sub_080f9108, 0x080f9108
	.set sub_080fa368, 0x080fa368
	.set sub_080fa478, 0x080fa478
	.set sub_08100e34, 0x08100e34
	.set sub_081019a4, 0x081019a4
	.set sub_08101d5c, 0x08101d5c
	.set sub_08102008, 0x08102008
	.set sub_081039fc, 0x081039fc
	.set sub_08104540, 0x08104540
	.set sub_08104a58, 0x08104a58
	.set sub_08104aa4, 0x08104aa4
	.set sub_081051a8, 0x081051a8
	.set sub_0810526c, 0x0810526c
	.set sub_08105300, 0x08105300
	.set sub_08108088, 0x08108088
	.set sub_081c0010, 0x081c0010
	.global Func_08100e5c
	.thumb_func
Func_08100e5c:
	push	{lr}
	movs	r0, #118
	adds	r0, #255
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_08100e6e
	ldr	r0, [pc, #8]
	b.n	.L_08100e70
.L_08100e6e:
	ldr	r0, [pc, #8]
.L_08100e70:
	pop	{pc}
	movs	r0, r0
	.4byte 0x00003007
	.2byte 0x2fd2
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r1, #192
	lsls	r1, r1, #4
	adds	r1, #236
	movs	r0, #220
	sub	sp, #4
	bl	sub_08014cc0
	ldr	r3, [pc, #156]
	movs	r2, #139
	lsls	r2, r2, #2
	adds	r3, r3, r2
	ldrb	r2, [r3, #0]
	movs	r1, #0
	mov	r9, r2
	movs	r2, #2
	strb	r2, [r3, #0]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #24]
	movs	r2, #1
	mov	sl, r2
	mov	r2, sl
	strh	r2, [r3, #4]
	adds	r7, r0, #0
	movs	r2, #30
	movs	r3, #20
	movs	r0, #0
	bl	sub_080383e8
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #0
	bl	sub_080f80e0
	movs	r0, #132
	lsls	r0, r0, #6
	adds	r0, #48
	bl	sub_08014dac
	movs	r2, #192
	lsls	r2, r2, #1
	adds	r3, r7, r2
	str	r0, [r3, #0]
	movs	r6, #132
	movs	r3, #132
	lsls	r3, r3, #6
	lsls	r6, r6, #6
	mov	r8, r0
	adds	r3, #40
	adds	r6, #44
	movs	r5, #0
	add	r3, r8
	add	r6, r8
	movs	r0, #183
	str	r5, [r3, #0]
	lsls	r0, r0, #1
	str	r5, [r6, #0]
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_08100f3c
	movs	r0, #112
	adds	r0, #255
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_08100f22
	movs	r0, #114
	adds	r0, #255
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_08100f1e
	mov	r3, sl
	b.n	.L_08100f3a
.L_08100f1e:
	movs	r3, #14
	b.n	.L_08100f3a
.L_08100f22:
	movs	r0, #114
	adds	r0, #255
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_08100f38
	movs	r3, #27
	b.n	.L_08100f3a
	movs	r0, r0
	.2byte 0x0240
	.2byte 0x0200
.L_08100f38:
	movs	r3, #28
.L_08100f3a:
	str	r3, [r6, #0]
.L_08100f3c:
	bl	sub_080f80c4
	movs	r0, #1
	bl	sub_080383c0
	ldr	r0, [pc, #140]
	bl	sub_080383f8
	movs	r2, #129
	lsls	r2, r2, #2
	adds	r0, r7, r2
	bl	sub_080ad100
	movs	r2, #139
	lsls	r2, r2, #1
	adds	r2, #255
	adds	r3, r7, r2
	strb	r0, [r3, #0]
	bl	sub_08104a58
	movs	r1, #3
	movs	r2, #0
	movs	r3, #7
	movs	r0, #0
	bl	sub_080fa368
	movs	r0, #0
	bl	sub_08100e34
	movs	r0, #14
	bl	sub_080f9108
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #17
	movs	r3, #5
	movs	r0, #13
	bl	sub_08038010
	adds	r3, r7, #0
	adds	r3, #240
	str	r0, [r3, #0]
	ldr	r2, [pc, #60]
	movs	r3, #182
	lsls	r3, r3, #1
	adds	r1, r7, r3
	movs	r3, #255
	strh	r3, [r1, #0]
	strb	r2, [r7, #28]
	strb	r2, [r7, #29]
	movs	r2, #180
	lsls	r2, r2, #1
	movs	r5, #0
	adds	r3, r7, r2
	adds	r2, #2
	strh	r5, [r3, #0]
	adds	r3, r7, r2
	strh	r5, [r3, #0]
	bl	sub_081051a8
	bl	.L_0810106c
	bl	sub_0810526c
	bl	sub_08104aa4
	movs	r0, #1
	bl	sub_08013560
	bl	sub_080fa478
	movs	r1, #0
	b.n	.L_08100fd8
	.4byte 0x00000000
	.2byte 0x2500
	.2byte 0x0600
.L_08100fd8:
	movs	r2, #30
	movs	r0, #0
	movs	r3, #20
	bl	sub_080383e8
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #24]
	strh	r5, [r3, #4]
	bl	sub_08038290
	movs	r0, #0
	bl	sub_080383c0
	mov	r1, r8
	movs	r2, #128
	ldr	r5, [pc, #100]
	adds	r1, #168
	lsls	r2, r2, #6
	ldr	r0, [pc, #100]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2180
	lsls	r1, r1, #6
	movs	r0, #160
	adds	r1, #168
	lsls	r0, r0, #19
	add	r1, r8
	movs	r2, #128
	adds	r0, #128
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2001
	bl	sub_08013560
	bl	sub_080f80a8
	movs	r1, #0
	movs	r0, #0
	movs	r2, #30
	movs	r3, #20
	bl	sub_080383f0
	movs	r2, #192
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldr	r0, [r3, #0]
	bl	sub_08013164
	movs	r0, #220
	bl	sub_0801314c
	bl	sub_080c85d0
	ldr	r3, [pc, #32]
	movs	r2, #139
	lsls	r2, r2, #2
	adds	r3, r3, r2
	mov	r2, r9
	movs	r0, #1
	strb	r2, [r3, #0]
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x03000730
	.4byte 0x06004000
	.2byte 0x0240
	.2byte 0x0200
.L_0810106c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r7, [r3, #0]
	movs	r1, #0
	movs	r4, #0
	mov	r8, r1
	ldr	r1, [r7, #20]
	mov	sl, r4
	movs	r3, #13
	mov	r2, sl
	sub	sp, #8
	strb	r3, [r1, #5]
	strh	r2, [r1, #12]
	str	r4, [sp, #0]
	bl	.L_08101638
	movs	r0, #1
	bl	sub_08013560
	ldr	r4, [sp, #0]
	movs	r5, #2
.L_081010a0:
	cmp	r5, #15
	bls.n	.L_081010a6
	b.n	.L_08101598
.L_081010a6:
	ldr	r2, [pc, #772]
	lsls	r3, r5, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x081010f0
	.4byte 0x08101598
	.4byte 0x08101102
	.4byte 0x081011b6
	.4byte 0x08101598
	.4byte 0x08101514
	.4byte 0x081013b0
	.4byte 0x081012be
	.4byte 0x08101598
	.4byte 0x08101416
	.4byte 0x08101142
	.4byte 0x08101544
	.4byte 0x081013e2
	.4byte 0x081012f0
	.4byte 0x08101448
	.2byte 0x118a
	.2byte 0x0810
	cmp	r4, #0
	blt.n	.L_081010f6
	b.n	.L_08101510
.L_081010f6:
	movs	r3, #1
	negs	r3, r3
	movs	r1, #1
	mov	sl, r3
	mov	r8, r1
	b.n	.L_08101510
	movs	r0, #0
	bl	sub_08100e34
	movs	r0, #1
	movs	r1, #0
	movs	r2, #200
	bl	sub_08105300
	movs	r0, #0
	bl	sub_08102008
	adds	r4, r0, #0
	movs	r5, #15
	cmp	r4, #10
	bne.n	.L_08101122
	b.n	.L_0810159c
.L_08101122:
	movs	r5, #0
	cmp	r4, #0
	bge.n	.L_0810112a
	b.n	.L_0810159c
.L_0810112a:
	movs	r3, #28
	ldrsb	r3, [r7, r3]
	movs	r1, #181
	lsls	r1, r1, #1
	adds	r2, r7, r1
	strh	r3, [r2, #0]
	movs	r5, #10
	cmp	r4, #7
	bne.n	.L_0810113e
	b.n	.L_0810159c
.L_0810113e:
	movs	r5, #3
	b.n	.L_0810159c
	movs	r3, #28
	ldrsb	r3, [r7, r3]
	movs	r2, #129
	lsls	r2, r2, #2
	lsls	r3, r3, #1
	adds	r3, r3, r2
	ldrh	r2, [r7, r3]
	movs	r1, #128
	str	r2, [r7, #8]
	ldrh	r2, [r7, r3]
	lsls	r1, r1, #2
	adds	r1, #22
	adds	r3, r7, r1
	strb	r2, [r3, #0]
	movs	r2, #188
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldr	r1, [r3, #0]
	movs	r2, #13
	strb	r2, [r1, #5]
	movs	r1, #190
	lsls	r1, r1, #1
	adds	r3, r7, r1
	ldr	r1, [r3, #0]
	strb	r2, [r1, #5]
	bl	sub_08104540
	movs	r2, #2
	adds	r4, r0, #0
	negs	r2, r2
	cmp	r4, r2
	beq.n	.L_08101184
	b.n	.L_08101510
.L_08101184:
	movs	r3, #1
	mov	r8, r3
	b.n	.L_08101510
	movs	r1, #188
	lsls	r1, r1, #1
	adds	r3, r7, r1
	ldr	r1, [r3, #0]
	movs	r2, #13
	strb	r2, [r1, #5]
	movs	r1, #190
	lsls	r1, r1, #1
	adds	r3, r7, r1
	ldr	r1, [r3, #0]
	strb	r2, [r1, #5]
	bl	sub_08101d5c
	movs	r2, #2
	adds	r4, r0, #0
	negs	r2, r2
	cmp	r4, r2
	beq.n	.L_081011b0
	b.n	.L_08101510
.L_081011b0:
	movs	r3, #1
	mov	r8, r3
	b.n	.L_08101510
	movs	r0, #8
	negs	r0, r0
	bl	sub_08100e34
	movs	r3, #28
	ldrsb	r3, [r7, r3]
	movs	r1, #129
	lsls	r1, r1, #2
	lsls	r3, r3, #1
	adds	r3, r3, r1
	ldrh	r2, [r7, r3]
	adds	r1, #18
	str	r2, [r7, #8]
	ldrh	r2, [r7, r3]
	adds	r3, r7, r1
	strb	r2, [r3, #0]
	movs	r0, #0
	movs	r3, #28
	ldrsb	r3, [r7, r3]
	movs	r2, #54
	lsls	r1, r3, #3
	subs	r1, r1, r3
	lsls	r1, r1, #3
	adds	r1, #48
	bl	sub_08105300
	movs	r0, #1
	bl	sub_08102008
	movs	r2, #139
	lsls	r2, r2, #1
	adds	r2, #255
	adds	r3, r7, r2
	ldrb	r3, [r3, #0]
	movs	r1, #0
	adds	r4, r0, #0
	cmp	r1, r3
	bge.n	.L_08101216
	adds	r0, r7, r2
	subs	r2, #237
.L_08101206:
	ldrh	r3, [r2, r7]
	adds	r1, #1
	adds	r3, #8
	strh	r3, [r2, r7]
	adds	r2, #2
	ldrb	r3, [r0, #0]
	cmp	r1, r3
	blt.n	.L_08101206
.L_08101216:
	movs	r3, #2
	negs	r3, r3
	cmp	r4, r3
	bne.n	.L_08101222
	movs	r1, #1
	mov	r8, r1
.L_08101222:
	cmp	r4, #0
	bge.n	.L_08101228
	b.n	.L_08101510
.L_08101228:
	subs	r3, r4, #3
	cmp	r3, #1
	bls.n	.L_08101236
	cmp	r4, #8
	beq.n	.L_08101236
	cmp	r4, #9
	bne.n	.L_0810124e
.L_08101236:
	movs	r3, #29
	ldrsb	r3, [r7, r3]
	movs	r2, #129
	lsls	r2, r2, #2
	lsls	r3, r3, #1
	adds	r3, r3, r2
	movs	r1, #140
	ldrh	r2, [r7, r3]
	lsls	r1, r1, #1
	adds	r1, #255
	adds	r3, r7, r1
	strb	r2, [r3, #0]
.L_0810124e:
	cmp	r4, #0
	bge.n	.L_08101254
	b.n	.L_08101510
.L_08101254:
	cmp	r4, #1
	bne.n	.L_0810125c
	movs	r5, #5
	b.n	.L_0810159c
.L_0810125c:
	cmp	r4, #2
	bne.n	.L_08101264
	movs	r5, #6
	b.n	.L_0810159c
.L_08101264:
	cmp	r4, #3
	bne.n	.L_08101276
	movs	r3, #135
	lsls	r3, r3, #2
	adds	r2, r7, r3
	movs	r3, #2
	strh	r3, [r2, #0]
	movs	r5, #7
	b.n	.L_0810159c
.L_08101276:
	cmp	r4, #4
	bne.n	.L_08101288
	movs	r1, #135
	lsls	r1, r1, #2
	adds	r2, r7, r1
	movs	r3, #2
	strh	r3, [r2, #0]
	movs	r5, #9
	b.n	.L_0810159c
.L_08101288:
	cmp	r4, #5
	bne.n	.L_08101290
	movs	r5, #11
	b.n	.L_0810159c
.L_08101290:
	cmp	r4, #6
	bne.n	.L_08101298
	movs	r5, #12
	b.n	.L_0810159c
.L_08101298:
	cmp	r4, #8
	bne.n	.L_081012aa
	movs	r3, #135
	lsls	r3, r3, #2
	adds	r2, r7, r3
	movs	r3, #2
	strh	r3, [r2, #0]
	movs	r5, #13
	b.n	.L_0810159c
.L_081012aa:
	cmp	r4, #9
	beq.n	.L_081012b0
	b.n	.L_0810159c
.L_081012b0:
	movs	r1, #135
	lsls	r1, r1, #2
	adds	r2, r7, r1
	movs	r3, #2
	strh	r3, [r2, #0]
	movs	r5, #14
	b.n	.L_0810159c
	movs	r2, #188
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldr	r1, [r3, #0]
	movs	r2, #13
	strb	r2, [r1, #5]
	movs	r1, #190
	lsls	r1, r1, #1
	adds	r3, r7, r1
	ldr	r1, [r3, #0]
	movs	r0, #1
	strb	r2, [r1, #5]
	bl	sub_081039fc
	movs	r2, #2
	adds	r4, r0, #0
	negs	r2, r2
	cmp	r4, r2
	bne.n	.L_081012e8
	movs	r3, #1
	mov	r8, r3
.L_081012e8:
	movs	r5, #3
	cmp	r4, #0
	bge.n	.L_081012f0
	b.n	.L_0810159c
.L_081012f0:
	movs	r0, #126
	bl	sub_081c0010
	movs	r2, #128
	movs	r1, #128
	lsls	r2, r2, #2
	adds	r2, #90
	lsls	r1, r1, #2
	adds	r3, r7, r2
	adds	r1, #22
	subs	r2, #2
	adds	r6, r7, r1
	ldrb	r1, [r3, #0]
	adds	r3, r7, r2
	ldrb	r2, [r3, #0]
	movs	r3, #140
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r5, r7, r3
	ldrb	r3, [r5, #0]
	ldrb	r0, [r6, #0]
	bl	sub_080ad178
	adds	r4, r0, #0
	ldrb	r0, [r6, #0]
	str	r4, [sp, #0]
	bl	sub_080ad008
	ldrb	r0, [r5, #0]
	bl	sub_080ad008
	ldr	r2, [r7, #20]
	movs	r3, #13
	strb	r3, [r2, #5]
	ldr	r0, [r7, #52]
	bl	sub_08038268
	movs	r1, #192
	lsls	r1, r1, #1
	adds	r3, r7, r1
	ldr	r0, [r3, #0]
	bl	sub_081019a4
	movs	r2, #181
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldrh	r0, [r3, #0]
	movs	r1, #10
	bl	sub_0800206c
	movs	r1, #182
	movs	r3, #0
	lsls	r1, r1, #1
	mov	ip, r3
	adds	r3, r7, r1
	ldrb	r6, [r3, #0]
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	movs	r1, #0
	adds	r5, r0, #0
	adds	r5, #160
	ldr	r4, [sp, #0]
	b.n	.L_08101370
.L_0810136e:
	adds	r1, #1
.L_08101370:
	movs	r2, #192
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldr	r2, [r3, #0]
	ldrsb	r3, [r2, r5]
	cmp	r1, r3
	bge.n	.L_08101390
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #1
	adds	r3, r3, r1
	lsls	r3, r3, #1
	ldrb	r3, [r2, r3]
	cmp	r6, r3
	bne.n	.L_0810136e
	mov	ip, r1
.L_08101390:
	mov	r1, ip
	lsls	r3, r1, #2
	add	r3, ip
	movs	r1, #180
	lsls	r1, r1, #1
	lsls	r3, r3, #1
	adds	r3, r0, r3
	adds	r2, r7, r1
	strh	r3, [r2, #0]
	ldr	r2, [r7, #20]
	movs	r3, #1
	strb	r3, [r2, #5]
	movs	r5, #0
	b.n	.L_0810159c
	.2byte 0x10b0
	.2byte 0x0810
	movs	r2, #188
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldr	r1, [r3, #0]
	movs	r2, #13
	strb	r2, [r1, #5]
	movs	r1, #190
	lsls	r1, r1, #1
	adds	r3, r7, r1
	ldr	r1, [r3, #0]
	movs	r0, #2
	strb	r2, [r1, #5]
	bl	sub_081039fc
	movs	r2, #2
	adds	r4, r0, #0
	negs	r2, r2
	cmp	r4, r2
	bne.n	.L_081013da
	movs	r3, #1
	mov	r8, r3
.L_081013da:
	movs	r5, #3
	cmp	r4, #0
	bge.n	.L_081013e2
	b.n	.L_0810159c
.L_081013e2:
	movs	r0, #175
	bl	sub_081c0010
	movs	r2, #128
	movs	r1, #128
	lsls	r2, r2, #2
	adds	r2, #90
	lsls	r1, r1, #2
	adds	r3, r7, r2
	adds	r1, #22
	subs	r2, #2
	adds	r6, r7, r1
	adds	r5, r7, r2
	ldrb	r1, [r3, #0]
	ldrb	r2, [r5, #0]
	ldrb	r0, [r6, #0]
	str	r3, [sp, #4]
	bl	sub_080ad160
	ldr	r3, [sp, #4]
	ldrb	r2, [r5, #0]
	ldrb	r1, [r3, #0]
	ldrb	r0, [r6, #0]
	bl	sub_080ad170
	b.n	.L_08101576
	movs	r1, #188
	lsls	r1, r1, #1
	adds	r3, r7, r1
	ldr	r1, [r3, #0]
	movs	r2, #13
	strb	r2, [r1, #5]
	movs	r1, #190
	lsls	r1, r1, #1
	adds	r3, r7, r1
	ldr	r1, [r3, #0]
	movs	r0, #0
	strb	r2, [r1, #5]
	bl	sub_081039fc
	movs	r2, #2
	adds	r4, r0, #0
	negs	r2, r2
	cmp	r4, r2
	bne.n	.L_08101440
	movs	r3, #1
	mov	r8, r3
.L_08101440:
	movs	r5, #3
	cmp	r4, #0
	bge.n	.L_08101448
	b.n	.L_0810159c
.L_08101448:
	movs	r0, #126
	bl	sub_081c0010
	movs	r2, #128
	movs	r1, #128
	lsls	r2, r2, #2
	adds	r2, #90
	lsls	r1, r1, #2
	adds	r3, r7, r2
	adds	r1, #22
	subs	r2, #2
	adds	r6, r7, r1
	ldrb	r1, [r3, #0]
	adds	r3, r7, r2
	ldrb	r2, [r3, #0]
	movs	r3, #140
	lsls	r3, r3, #1
	adds	r3, #255
	adds	r5, r7, r3
	ldrb	r3, [r5, #0]
	ldrb	r0, [r6, #0]
	bl	sub_080ad178
	movs	r1, #174
	lsls	r1, r1, #1
	movs	r2, #173
	adds	r1, #255
	lsls	r2, r2, #1
	adds	r3, r7, r1
	adds	r2, #255
	ldrb	r1, [r3, #0]
	adds	r3, r7, r2
	ldrb	r2, [r3, #0]
	ldrb	r0, [r5, #0]
	ldrb	r3, [r6, #0]
	bl	sub_080ad178
	adds	r4, r0, #0
	ldrb	r0, [r6, #0]
	str	r4, [sp, #0]
	bl	sub_080ad008
	ldrb	r0, [r5, #0]
	bl	sub_080ad008
	movs	r1, #192
	lsls	r1, r1, #1
	adds	r3, r7, r1
	ldr	r0, [r3, #0]
	bl	sub_081019a4
	movs	r2, #181
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldrh	r0, [r3, #0]
	movs	r1, #10
	bl	sub_0800206c
	movs	r1, #182
	movs	r3, #0
	lsls	r1, r1, #1
	mov	ip, r3
	adds	r3, r7, r1
	ldrb	r6, [r3, #0]
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	movs	r1, #0
	adds	r5, r0, #0
	adds	r5, #160
	ldr	r4, [sp, #0]
	b.n	.L_081014d8
.L_081014d6:
	adds	r1, #1
.L_081014d8:
	movs	r2, #192
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldr	r2, [r3, #0]
	ldrsb	r3, [r2, r5]
	cmp	r1, r3
	bge.n	.L_081014f8
	lsls	r3, r0, #2
	adds	r3, r3, r0
	lsls	r3, r3, #1
	adds	r3, r3, r1
	lsls	r3, r3, #1
	ldrb	r3, [r2, r3]
	cmp	r6, r3
	bne.n	.L_081014d6
	mov	ip, r1
.L_081014f8:
	mov	r1, ip
	lsls	r3, r1, #2
	add	r3, ip
	movs	r1, #180
	lsls	r1, r1, #1
	lsls	r3, r3, #1
	adds	r3, r0, r3
	adds	r2, r7, r1
	strh	r3, [r2, #0]
	ldr	r2, [r7, #20]
	movs	r3, #1
	strb	r3, [r2, #5]
.L_08101510:
	movs	r5, #2
	b.n	.L_0810159c
	movs	r2, #188
	lsls	r2, r2, #1
	adds	r3, r7, r2
	ldr	r1, [r3, #0]
	movs	r2, #13
	strb	r2, [r1, #5]
	movs	r1, #190
	lsls	r1, r1, #1
	adds	r3, r7, r1
	ldr	r1, [r3, #0]
	movs	r0, #3
	strb	r2, [r1, #5]
	bl	sub_081039fc
	movs	r2, #2
	adds	r4, r0, #0
	negs	r2, r2
	cmp	r4, r2
	bne.n	.L_0810153e
	movs	r3, #1
	mov	r8, r3
.L_0810153e:
	movs	r5, #3
	cmp	r4, #0
	blt.n	.L_0810159c
	movs	r0, #139
	bl	sub_081c0010
	movs	r2, #128
	movs	r1, #128
	lsls	r2, r2, #2
	adds	r2, #90
	lsls	r1, r1, #2
	adds	r3, r7, r2
	adds	r1, #22
	subs	r2, #2
	adds	r6, r7, r1
	adds	r5, r7, r2
	ldrb	r1, [r3, #0]
	ldrb	r2, [r5, #0]
	ldrb	r0, [r6, #0]
	str	r3, [sp, #4]
	bl	sub_080ad158
	ldr	r3, [sp, #4]
	ldrb	r2, [r5, #0]
	ldrb	r1, [r3, #0]
	ldrb	r0, [r6, #0]
	bl	sub_080ad168
.L_08101576:
	adds	r4, r0, #0
	ldrb	r0, [r6, #0]
	str	r4, [sp, #0]
	bl	sub_080ad008
	ldr	r2, [r7, #20]
	movs	r3, #13
	strb	r3, [r2, #5]
	ldr	r0, [r7, #52]
	bl	sub_08038268
	ldr	r2, [r7, #20]
	movs	r3, #1
	strb	r3, [r2, #5]
	movs	r5, #2
	ldr	r4, [sp, #0]
	b.n	.L_0810159c
.L_08101598:
	movs	r3, #1
	mov	r8, r3
.L_0810159c:
	mov	r1, r8
	cmp	r1, #0
	bne.n	.L_081015a4
	b.n	.L_081010a0
.L_081015a4:
	mov	r0, sl
	add	sp, #8
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
.L_081015b0:
	push	{r5, r6, r7, lr}
	movs	r1, #0
	adds	r5, r0, #0
	movs	r3, #15
	mov	ip, r1
	movs	r7, #31
.L_081015bc:
	lsls	r3, r3, #4
	movs	r6, #0
	mov	lr, r3
.L_081015c2:
	mov	r2, lr
	adds	r3, r2, r6
	movs	r1, #160
	lsls	r1, r1, #19
	lsls	r0, r3, #1
	adds	r3, r0, r1
	ldrh	r3, [r3, #0]
	adds	r1, r7, #0
	lsrs	r4, r3, #10
	ands	r4, r7
	lsrs	r2, r3, #5
	ands	r2, r7
	ands	r1, r3
	adds	r4, r4, r5
	adds	r2, r2, r5
	adds	r1, r1, r5
	cmp	r4, #31
	ble.n	.L_081015e8
	movs	r4, #31
.L_081015e8:
	cmp	r2, #31
	ble.n	.L_081015ee
	movs	r2, #31
.L_081015ee:
	cmp	r1, #31
	ble.n	.L_081015f4
	movs	r1, #31
.L_081015f4:
	cmp	r4, #0
	bge.n	.L_081015fa
	movs	r4, #0
.L_081015fa:
	cmp	r2, #0
	bge.n	.L_08101600
	movs	r2, #0
.L_08101600:
	cmp	r1, #0
	bge.n	.L_08101606
	movs	r1, #0
.L_08101606:
	lsls	r2, r2, #5
	lsls	r3, r4, #10
	orrs	r3, r2
	orrs	r3, r1
	ldr	r1, [pc, #36]
	adds	r6, #1
	adds	r2, r0, r1
	strh	r3, [r2, #0]
	cmp	r6, #15
	ble.n	.L_081015c2
	mov	r2, ip
	movs	r3, #5
	cmp	r2, #0
	beq.n	.L_08101628
	movs	r5, #12
	movs	r3, #7
	negs	r5, r5
.L_08101628:
	movs	r1, #1
	add	ip, r1
	mov	r2, ip
	cmp	r2, #2
	ble.n	.L_081015bc
	pop	{r5, r6, r7, pc}
	.2byte 0xffe0
	.2byte 0x04ff
.L_08101638:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	sub	sp, #12
	mov	sl, r3
	movs	r3, #192
	lsls	r3, r3, #1
	add	r3, sl
	ldr	r3, [r3, #0]
	movs	r1, #0
	mov	r8, r3
	mov	r3, sl
	adds	r3, #52
	str	r3, [sp, #8]
	movs	r3, #15
	str	r3, [sp, #0]
	movs	r3, #2
	str	r3, [sp, #4]
	ldr	r0, [sp, #8]
	movs	r2, #5
	movs	r3, #30
	bl	sub_080f811c
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [pc, #324]
	mov	r0, r8
	movs	r2, #128
	mov	r9, r3
	ldr	r1, [pc, #320]
	lsls	r2, r2, #6
	adds	r0, #168
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x2080
	lsls	r0, r0, #6
	movs	r1, #160
	adds	r0, #168
	lsls	r1, r1, #19
	add	r0, r8
	adds	r1, #128
	movs	r2, #128
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x2180
	ldr	r7, [pc, #292]
	lsls	r1, r1, #6
	ldr	r2, [pc, #292]
	ldr	r0, [pc, #284]
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x20a0
	lsls	r0, r0, #19
	movs	r1, #128
	ldr	r2, [pc, #284]
	adds	r0, #128
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x4846
	bl	sub_080383b8
	ldr	r1, [pc, #276]
	movs	r2, #32
	ldr	r0, [pc, #276]
	mov	lr, r9
	.2byte 0xf800
	.2byte 0xf713
	.2byte 0xf98f
	.2byte 0x2380
	movs	r1, #160
	movs	r2, #128
	lsls	r3, r3, #19
	lsls	r1, r1, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r1, #160
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r6, [pc, #248]
	movs	r2, #160
	ldrh	r3, [r6, #0]
	lsls	r2, r2, #19
	adds	r2, #188
	strh	r3, [r2, #0]
	movs	r3, #128
	movs	r2, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #232]
	adds	r1, #64
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #8
	bl	.L_081015b0
	ldrh	r3, [r6, #0]
	movs	r2, #160
	lsls	r2, r2, #19
	adds	r2, #232
	strh	r3, [r2, #0]
	subs	r2, #32
	ldrh	r3, [r6, #0]
	mov	r0, r8
	strh	r3, [r2, #0]
	bl	sub_081019a4
	mov	r3, sl
	ldr	r6, [r3, #16]
	adds	r0, r6, #0
	bl	sub_08038268
	movs	r1, #8
	negs	r1, r1
	movs	r2, #11
	adds	r0, r6, #0
	bl	sub_080f8840
	movs	r3, #0
	mov	fp, r3
	movs	r3, #13
	mov	r8, r3
	adds	r7, r0, #0
	mov	r3, r8
	strb	r3, [r7, #5]
	mov	r3, sl
	str	r7, [r3, #20]
	bl	sub_080143ac
	adds	r5, r0, #0
	cmp	r5, #95
	bgt.n	.L_081017b2
	ldr	r2, [pc, #148]
	movs	r1, #128
	bl	sub_08108088
	movs	r3, #128
	lsls	r3, r3, #23
	mov	r9, r3
	mov	r1, r9
	mov	r3, fp
	adds	r2, r6, #0
	adds	r0, r5, #0
	str	r3, [sp, #0]
	bl	sub_080380c8
	mov	r3, r8
	adds	r7, r0, #0
	strb	r3, [r7, #5]
	movs	r3, #188
	lsls	r3, r3, #1
	add	r3, sl
	str	r7, [r3, #0]
	ldrb	r1, [r7, #23]
	ldr	r2, [pc, #60]
	lsls	r3, r1, #26
	lsrs	r3, r3, #27
	orrs	r2, r3
	movs	r3, #63
	negs	r3, r3
	lsls	r2, r2, #1
	ands	r3, r1
	orrs	r3, r2
	strb	r3, [r7, #23]
	adds	r0, r5, #0
	mov	r3, fp
	mov	r1, r9
	adds	r2, r6, #0
	str	r3, [sp, #0]
	bl	sub_080380c8
	mov	r3, r8
	adds	r7, r0, #0
	strb	r3, [r7, #5]
	movs	r3, #190
	lsls	r3, r3, #1
	add	r3, sl
	str	r7, [r3, #0]
.L_081017b2:
	ldr	r3, [sp, #8]
	ldr	r0, [pc, #56]
	ldr	r6, [r3, #0]
	movs	r3, #188
	lsls	r3, r3, #1
	add	r3, sl
	b.n	.L_081017f4
	.4byte 0x00000008
	.4byte 0x03000730
	.4byte 0x06004000
	.4byte 0x03000260
	.4byte 0x33333333
	.4byte 0x55555555
	.4byte 0x06005000
	.4byte 0x081059b4
	.4byte 0x060052c0
	.4byte 0x050001e8
	.4byte 0x050001e0
	.4byte 0x000001fb
	.2byte 0xfe00
	.2byte 0xffff
.L_081017f4:
	ldr	r7, [r3, #0]
	ldrh	r1, [r6, #12]
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	mov	ip, r3
	lsls	r1, r1, #3
	mov	r3, ip
	adds	r2, r1, #0
	ands	r2, r3
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	ldrh	r4, [r7, #22]
	mov	lr, r3
	mov	r3, lr
	ands	r2, r3
	adds	r3, r0, #0
	ands	r3, r4
	orrs	r3, r2
	ldrh	r2, [r6, #14]
	strh	r3, [r7, #22]
	lsls	r2, r2, #3
	adds	r2, #4
	mov	r3, ip
	adds	r4, r2, #0
	ands	r4, r3
	strh	r1, [r7, #6]
	strh	r2, [r7, #8]
	strb	r4, [r7, #20]
	movs	r3, #190
	lsls	r3, r3, #1
	add	r3, sl
	ldr	r7, [r3, #0]
	adds	r1, #224
	mov	r3, ip
	strh	r1, [r7, #6]
	ands	r1, r3
	mov	r3, lr
	ands	r1, r3
	ldrh	r3, [r7, #22]
	strh	r2, [r7, #8]
	ands	r0, r3
	orrs	r0, r1
	strh	r0, [r7, #22]
	strb	r4, [r7, #20]
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.align 2, 0
