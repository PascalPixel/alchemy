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
	.set sub_08014274, 0x08014274
	.set sub_080143e0, 0x080143e0
	.set sub_080145a8, 0x080145a8
	.set sub_08014644, 0x08014644
	.set sub_08014878, 0x08014878
	.set sub_08014c4c, 0x08014c4c
	.set sub_08014dac, 0x08014dac
	.set sub_08014de4, 0x08014de4
	.set sub_08014e38, 0x08014e38
	.set sub_08014ea8, 0x08014ea8
	.set sub_08015024, 0x08015024
	.set sub_08015068, 0x08015068
	.set sub_080150e4, 0x080150e4
	.set sub_08015160, 0x08015160
	.set sub_080151e4, 0x080151e4
	.set sub_0801521c, 0x0801521c
	.set sub_0801587c, 0x0801587c
	.set sub_08020010, 0x08020010
	.set sub_08020048, 0x08020048
	.set sub_08118010, 0x08118010
	.set sub_08118040, 0x08118040
	.set sub_08118088, 0x08118088
	.set sub_081180e8, 0x081180e8
	.set sub_08138058, 0x08138058
	.set sub_08138086, 0x08138086
	.set sub_0813ba50, 0x0813ba50
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143a88, 0x08143a88
	.set sub_08143b20, 0x08143b20
	.set sub_08143bb8, 0x08143bb8
	.set sub_08144aac, 0x08144aac
	.set sub_0814cc4c, 0x0814cc4c
	.set sub_0814cca8, 0x0814cca8
	.set sub_0814cd48, 0x0814cd48
	.set sub_08152404, 0x08152404
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_0815b290, 0x0815b290
	.set sub_0815b3b0, 0x0815b3b0
	.set sub_0815b434, 0x0815b434
	.set sub_0815e20c, 0x0815e20c
	.set sub_0815e21c, 0x0815e21c
	.set sub_0815f000, 0x0815f000
	.set sub_0815f0a0, 0x0815f0a0
	.set sub_08179e6c, 0x08179e6c
	.set sub_08180600, 0x08180600
	.set sub_08180d8a, 0x08180d8a
	.set sub_08180e38, 0x08180e38
	.set sub_08181846, 0x08181846
	.set sub_081818a6, 0x081818a6
	.set sub_08181ed4, 0x08181ed4
	.set sub_0818caa8, 0x0818caa8
	.set sub_081963ec, 0x081963ec
	.set sub_08196958, 0x08196958
	.set sub_081969f8, 0x081969f8
	.set sub_08196a7c, 0x08196a7c
	.set sub_081c0010, 0x081c0010
	.global Func_0817e698
	.thumb_func
Func_0817e698:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #32
	str	r0, [sp, #8]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #92]
	movs	r0, #0
	mov	sl, r3
	mov	r9, r1
	bl	sub_081435e0
	movs	r1, #142
	lsls	r1, r1, #7
	ldr	r0, [pc, #272]
	add	r1, sl
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r0, #224
	lsls	r0, r0, #3
	movs	r1, #128
	add	r0, sl
	ldr	r3, [pc, #256]
	lsls	r1, r1, #7
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2500
	movs	r7, #0
	mov	r6, sl
.L_0817e6e0:
	movs	r0, #162
	adds	r3, r7, r5
	lsls	r0, r0, #7
	add	r3, sl
	adds	r0, #142
	adds	r2, r3, r0
	movs	r3, #157
	lsls	r3, r3, #4
	adds	r3, #255
	lsls	r1, r5, #6
	adds	r0, r6, r3
	add	r1, sl
	adds	r3, #33
	movs	r4, #0
	adds	r1, r1, r3
.L_0817e6fe:
	ldrb	r3, [r2, #0]
	adds	r4, #1
	strb	r3, [r0, #0]
	adds	r0, #1
	ldrb	r3, [r2, #0]
	adds	r2, #1
	strb	r3, [r1, #0]
	subs	r1, #1
	cmp	r4, #17
	bne.n	.L_0817e6fe
	adds	r5, #1
	adds	r7, #16
	adds	r6, #64
	cmp	r5, #49
	bne.n	.L_0817e6e0
	movs	r5, #0
	movs	r7, #0
	mov	r6, sl
.L_0817e722:
	movs	r2, #200
	lsls	r2, r2, #5
	adds	r2, #77
	lsls	r1, r5, #6
	adds	r0, r6, r2
	mov	r2, sl
	adds	r3, r1, r2
	movs	r2, #200
	lsls	r2, r2, #5
	adds	r2, #114
	adds	r1, r3, r2
	adds	r3, r7, #0
	movs	r2, #168
	add	r3, sl
	lsls	r2, r2, #7
	mov	ip, r3
	adds	r2, #207
	movs	r4, #0
	add	r2, ip
.L_0817e748:
	ldrb	r3, [r2, #0]
	adds	r4, #1
	strb	r3, [r0, #0]
	adds	r0, #1
	ldrb	r3, [r2, #0]
	adds	r2, #1
	strb	r3, [r1, #0]
	subs	r1, #1
	cmp	r4, #19
	bne.n	.L_0817e748
	adds	r5, #1
	adds	r7, #19
	adds	r6, #64
	cmp	r5, #55
	bne.n	.L_0817e722
	movs	r5, #0
	movs	r7, #0
	movs	r6, #0
.L_0817e76c:
	movs	r2, #156
	mov	r0, sl
	lsls	r2, r2, #6
	adds	r3, r6, r0
	adds	r2, #12
	adds	r0, r3, r2
	adds	r2, #39
	adds	r1, r3, r2
	adds	r3, r7, #0
	movs	r2, #176
	add	r3, sl
	lsls	r2, r2, #7
	mov	ip, r3
	adds	r2, #228
	movs	r4, #0
	add	r2, ip
.L_0817e78c:
	ldrb	r3, [r2, #0]
	adds	r4, #1
	strb	r3, [r0, #0]
	adds	r0, #1
	ldrb	r3, [r2, #0]
	adds	r2, #1
	strb	r3, [r1, #0]
	subs	r1, #1
	cmp	r4, #20
	bne.n	.L_0817e78c
	adds	r5, #1
	adds	r7, #20
	adds	r6, #64
	cmp	r5, #64
	bne.n	.L_0817e76c
	mov	r0, r9
	cmp	r0, #3
	bne.n	.L_0817e7b4
	ldr	r0, [pc, #36]
	b.n	.L_0817e7bc
.L_0817e7b4:
	mov	r2, r9
	cmp	r2, #2
	bne.n	.L_0817e7e4
	ldr	r0, [pc, #32]
.L_0817e7bc:
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #24]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xe013
	.4byte 0x0000013c
	.4byte 0x03000258
	.4byte 0x00000148
	.4byte 0x00000129
	.2byte 0x0730
	.2byte 0x0300
.L_0817e7e4:
	ldr	r0, [pc, #100]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #96]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9a02
	movs	r3, #36
	ldrsh	r0, [r2, r3]
	mov	r3, sp
	adds	r3, #20
	adds	r1, r3, #0
	str	r3, [sp, #4]
	bl	sub_0815e21c
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r0, [sp, #4]
	movs	r1, #128
	ldr	r2, [r0, #0]
	movs	r3, #64
	subs	r3, r3, r2
	lsls	r1, r1, #19
	lsls	r3, r3, #8
	adds	r1, #40
	str	r3, [r1, #0]
	movs	r0, #104
	movs	r1, #19
	bl	sub_081963ec
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, sl
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, sl
	movs	r3, #50
	movs	r1, #200
	b.n	.L_0817e854
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x00000150
	.2byte 0x0730
	.2byte 0x0300
.L_0817e854:
	str	r3, [r2, #0]
	ldr	r0, [pc, #416]
	lsls	r1, r1, #4
	bl	sub_080145a8
	mov	r2, r9
	cmp	r2, #2
	bne.n	.L_0817e870
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, sl
	movs	r3, #0
	b.n	.L_0817e884
.L_0817e870:
	mov	r3, r9
	cmp	r3, #1
	beq.n	.L_0817e87a
	cmp	r3, #3
	bne.n	.L_0817e88e
.L_0817e87a:
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, sl
	movs	r3, #8
.L_0817e884:
	str	r3, [r2, #0]
	movs	r0, #212
	bl	sub_081c0010
	b.n	.L_0817e89a
.L_0817e88e:
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, sl
	movs	r3, #32
	str	r3, [r2, #0]
.L_0817e89a:
	mov	r2, r9
	movs	r0, #0
	lsls	r2, r2, #2
	mov	r8, r0
	mov	fp, r2
.L_0817e8a4:
	mov	r3, r8
	cmp	r3, #0
	bne.n	.L_0817e8de
	mov	r0, r9
	cmp	r0, #2
	bne.n	.L_0817e8c8
	ldr	r3, [sp, #8]
	movs	r1, #7
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	movs	r3, #32
	movs	r2, #1
	str	r3, [sp, #0]
	negs	r2, r2
	movs	r3, #0
	bl	sub_0814cd48
	b.n	.L_0817e8de
.L_0817e8c8:
	ldr	r3, [sp, #8]
	movs	r1, #10
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	movs	r3, #32
	movs	r2, #1
	str	r3, [sp, #0]
	negs	r2, r2
	movs	r3, #0
	bl	sub_0814cd48
.L_0817e8de:
	mov	r0, r8
	cmp	r0, #24
	bne.n	.L_0817e8ea
	movs	r0, #0
	bl	sub_081180e8
.L_0817e8ea:
	mov	r2, r8
	cmp	r2, #8
	bne.n	.L_0817e8fc
	mov	r3, r9
	cmp	r3, #0
	bne.n	.L_0817e8fc
	movs	r0, #126
	bl	sub_081c0010
.L_0817e8fc:
	mov	r0, r8
	cmp	r0, #31
	bgt.n	.L_0817e99e
	mov	r3, r8
	cmp	r0, #0
	bge.n	.L_0817e90a
	adds	r3, #3
.L_0817e90a:
	asrs	r7, r3, #2
	cmp	r7, #2
	ble.n	.L_0817e916
	movs	r3, #1
	ands	r3, r7
	adds	r7, r3, #1
.L_0817e916:
	mov	r2, r8
	cmp	r2, #27
	bgt.n	.L_0817e99e
	movs	r0, #32
	bl	sub_08014dac
	adds	r6, r0, #0
	movs	r0, #1
	bl	sub_081969f8
	ldr	r2, [pc, #208]
	ldr	r3, [sp, #12]
	adds	r5, r0, #0
	ands	r3, r2
	ldr	r2, [pc, #204]
	movs	r0, #6
	orrs	r3, r0
	ands	r3, r2
	movs	r2, #192
	lsls	r2, r2, #3
	orrs	r3, r2
	str	r3, [sp, #12]
	movs	r2, #224
	lsls	r3, r7, #12
	lsls	r2, r2, #3
	add	r3, sl
	adds	r3, r3, r2
	add	r2, sp, #12
	str	r3, [r2, #4]
	ldr	r3, [pc, #176]
	str	r0, [r5, #0]
	str	r3, [r5, #8]
	ldr	r3, [pc, #176]
	mov	r0, fp
	ldr	r3, [r3, r0]
	str	r2, [r5, #16]
	str	r3, [r5, #20]
	str	r6, [r5, #12]
	bl	sub_08014de4
	ldr	r2, [sp, #4]
	movs	r0, #0
	ldr	r1, [r2, #4]
	movs	r2, #0
	subs	r1, #58
	lsls	r1, r1, #16
	bl	sub_08015160
	ldr	r3, [pc, #148]
	mov	r2, fp
	ldr	r0, [r3, r2]
	lsls	r0, r0, #1
	bl	sub_0801521c
	adds	r1, r6, #0
	movs	r2, #4
	ldr	r0, [pc, #136]
	bl	sub_08196958
	adds	r0, r5, #0
	bl	sub_08196a7c
	adds	r0, r5, #0
	bl	sub_08013164
	adds	r0, r6, #0
	bl	sub_08013164
.L_0817e99e:
	mov	r3, r9
	cmp	r3, #0
	bne.n	.L_0817e9ae
	movs	r0, #4
	movs	r1, #4
	bl	sub_08158ce0
	b.n	.L_0817e9b6
.L_0817e9ae:
	movs	r0, #16
	movs	r1, #16
	bl	sub_08158ce0
.L_0817e9b6:
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	add	r2, sl
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #1
	add	r8, r0
	mov	r2, r8
	cmp	r2, #48
	beq.n	.L_0817e9d8
	b.n	.L_0817e8a4
.L_0817e9d8:
	ldr	r0, [pc, #28]
	bl	sub_08014644
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x08143001
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x08199340
	.4byte 0x081994e0
	.4byte 0x081994d0
	.2byte 0x91f0
	.2byte 0x0819
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #144
	lsls	r3, r3, #1
	mov	lr, r0
	adds	r6, r1, #0
	mov	ip, r2
	movs	r7, #0
	mov	r8, r3
	movs	r5, #0
.L_0817ea2a:
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r0, r3, #1
	mov	r3, lr
	movs	r1, #0
	adds	r4, r5, r3
.L_0817ea36:
	lsrs	r3, r1, #31
	adds	r3, r1, r3
	ldrb	r2, [r4, #0]
	asrs	r3, r3, #1
	adds	r3, r0, r3
	adds	r1, #1
	adds	r4, #1
	strb	r2, [r6, r3]
	cmp	r1, #40
	bne.n	.L_0817ea36
	adds	r7, #1
	add	r5, ip
	cmp	r7, r8
	bne.n	.L_0817ea2a
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
