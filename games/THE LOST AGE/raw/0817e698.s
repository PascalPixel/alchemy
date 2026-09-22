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
	.global Overlay_0817e698
Overlay_0817e698:
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
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #268
	str	r0, [sp, #148]
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r0, [r5, #96]
	adds	r3, r5, #0
	str	r0, [sp, #144]
	adds	r3, #176
	ldr	r2, [r5, #100]
	ldr	r1, [r5, #92]
	str	r2, [sp, #136]
	movs	r0, #0
	ldr	r3, [r3, #0]
	mov	fp, r1
	str	r3, [sp, #132]
	ldr	r7, [pc, #132]
	ldr	r3, [r5, #36]
	str	r3, [sp, #128]
	bl	sub_081435e0
	bl	sub_0813ba50
	bl	sub_08179e6c
	movs	r1, #3
	movs	r0, #104
	bl	sub_081963ec
	ldr	r3, [r5, #104]
	movs	r2, #239
	lsls	r2, r2, #7
	str	r3, [sp, #176]
	add	r2, fp
	movs	r3, #0
	movs	r1, #200
	str	r3, [r2, #0]
	ldr	r0, [pc, #96]
	lsls	r1, r1, #4
	bl	sub_080145a8
	movs	r4, #0
	str	r4, [sp, #124]
	str	r4, [sp, #112]
	str	r7, [sp, #108]
	str	r4, [sp, #104]
	ldr	r3, [pc, #80]
	movs	r0, #80
	ldrh	r3, [r3, #4]
	negs	r0, r0
	str	r3, [sp, #100]
	str	r0, [sp, #96]
	str	r4, [sp, #140]
.L_0817eace:
	ldr	r1, [sp, #140]
	cmp	r1, #27
	beq.n	.L_0817ead6
	b.n	.L_0817eec0
.L_0817ead6:
	ldr	r2, [sp, #128]
	ldr	r5, [pc, #44]
	ldr	r0, [r2, #84]
	bl	sub_08014274
	ldr	r2, [pc, #52]
	movs	r3, #240
	add	r0, sp, #240
	str	r3, [r2, #16]
	movs	r3, #255
	strh	r3, [r0, #0]
	movs	r1, #0
	bl	sub_08118010
	movs	r0, #1
	ldr	r1, [pc, #36]
	movs	r2, #0
	bl	sub_08118040
	movs	r4, #160
	lsls	r4, r4, #19
	movs	r3, #0
	adds	r4, #192
	mov	r8, r3
	b.n	.L_0817eb20
	.4byte 0x0000001f
	.4byte 0xffe30000
	.4byte 0x08143001
	.4byte 0x03001120
	.4byte 0x030011e0
	.2byte 0x0045
	.2byte 0x0000
.L_0817eb20:
	ldrh	r3, [r4, #0]
	movs	r1, #31
	ands	r1, r3
	lsls	r3, r3, #16
	lsrs	r2, r3, #21
	ands	r2, r5
	lsrs	r3, r3, #26
	ands	r3, r5
	subs	r0, r2, #4
	subs	r3, #4
	subs	r1, #4
	cmp	r0, #0
	bge.n	.L_0817eb3c
	movs	r0, #0
.L_0817eb3c:
	cmp	r1, #0
	bge.n	.L_0817eb42
	movs	r1, #0
.L_0817eb42:
	cmp	r3, #0
	bge.n	.L_0817eb48
	movs	r3, #0
.L_0817eb48:
	cmp	r0, #31
	ble.n	.L_0817eb4e
	movs	r0, #31
.L_0817eb4e:
	cmp	r1, #31
	ble.n	.L_0817eb54
	movs	r1, #31
.L_0817eb54:
	cmp	r3, #31
	ble.n	.L_0817eb5a
	movs	r3, #31
.L_0817eb5a:
	lsls	r3, r3, #10
	lsls	r2, r1, #5
	movs	r7, #1
	orrs	r3, r2
	add	r8, r7
	orrs	r3, r0
	mov	r0, r8
	strh	r3, [r4, #0]
	adds	r4, #2
	cmp	r0, #128
	bne.n	.L_0817eb20
	movs	r3, #238
	lsls	r3, r3, #7
	movs	r2, #238
	adds	r3, #144
	lsls	r2, r2, #7
	movs	r1, #0
	add	r3, fp
	adds	r2, #148
	str	r1, [r3, #0]
	add	r2, fp
	movs	r3, #5
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #152
	add	r2, fp
	subs	r3, #6
	str	r3, [r2, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #156
	add	r3, fp
	str	r1, [r3, #0]
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #736]
	bl	sub_080145a8
	ldr	r1, [sp, #132]
	movs	r3, #1
	str	r3, [r1, #16]
	movs	r1, #224
	lsls	r1, r1, #3
	ldr	r0, [pc, #724]
	movs	r2, #0
	add	r1, fp
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r6, [pc, #716]
	movs	r2, #0
	mov	sl, r2
	movs	r5, #6
	movs	r4, #32
	movs	r0, #0
.L_0817ebca:
	ldr	r7, [sp, #136]
	movs	r3, #0
	mov	r8, r3
	lsls	r3, r0, #1
	adds	r2, r3, r7
	mov	r3, sl
	lsls	r1, r3, #2
.L_0817ebd8:
	ldrh	r3, [r6, r5]
	movs	r7, #224
	lsls	r7, r7, #3
	add	r3, r8
	adds	r3, r3, r7
	mov	r7, fp
	ldrb	r3, [r7, r3]
	cmp	r3, #0
	beq.n	.L_0817ebf4
	subs	r3, r3, r1
	subs	r3, #40
	cmp	r3, #0
	bgt.n	.L_0817ebf4
	movs	r3, #1
.L_0817ebf4:
	strb	r3, [r2, #0]
	movs	r3, #1
	add	r8, r3
	adds	r2, #1
	cmp	r8, r4
	bne.n	.L_0817ebd8
	add	sl, r3
	mov	r7, sl
	adds	r0, #16
	cmp	r7, #10
	bne.n	.L_0817ebca
	ldr	r0, [pc, #644]
	bl	sub_08013300
	adds	r5, r0, #0
	adds	r1, r5, #0
	movs	r2, #32
	ldr	r3, [pc, #636]
	ldr	r0, [pc, #636]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x21e0
	adds	r5, #32
	lsls	r1, r1, #3
	movs	r6, #238
	adds	r0, r5, #0
	add	r1, fp
	lsls	r6, r6, #7
	bl	sub_0801587c
	adds	r6, #220
	movs	r4, #0
	mov	sl, r4
	mov	r9, r4
	add	r6, fp
	movs	r7, #0
.L_0817ec3c:
	mov	r1, sl
	lsls	r3, r1, #12
	movs	r2, #224
	movs	r0, #0
	add	r3, fp
	lsls	r2, r2, #3
	mov	r8, r0
	adds	r5, r3, r2
.L_0817ec4c:
	movs	r2, #128
	movs	r3, #240
	movs	r1, #32
	lsls	r2, r2, #24
	lsls	r3, r3, #8
	movs	r0, #32
	bl	sub_0815b290
	mov	r4, r8
	movs	r1, #238
	adds	r3, r7, r4
	lsls	r1, r1, #7
	adds	r1, #220
	lsls	r3, r3, #2
	adds	r3, r3, r1
	mov	r2, fp
	str	r0, [r2, r3]
	movs	r4, #13
	ldrb	r2, [r0, #9]
	negs	r4, r4
	adds	r3, r4, #0
	ands	r2, r3
	movs	r3, #4
	orrs	r2, r3
	ldrb	r3, [r0, #16]
	strb	r2, [r0, #9]
	ldr	r0, [pc, #536]
	lsls	r3, r3, #2
	adds	r3, r3, r0
	ldrh	r0, [r3, #2]
	ldr	r1, [pc, #532]
	movs	r2, #128
	adds	r0, r0, r1
	lsls	r2, r2, #2
	adds	r1, r5, #0
	ldr	r3, [pc, #512]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2001
	movs	r4, #128
	add	r8, r0
	lsls	r4, r4, #2
	mov	r1, r8
	adds	r5, r5, r4
	cmp	r1, #8
	bne.n	.L_0817ec4c
	movs	r2, #128
	movs	r3, #240
	movs	r1, #32
	lsls	r2, r2, #24
	lsls	r3, r3, #8
	movs	r0, #32
	bl	sub_0815b3b0
	ldr	r1, [r6, #0]
	movs	r2, #24
	ldr	r3, [pc, #468]
	str	r0, [r6, #32]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x464a
	add	r2, sl
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #2
	movs	r0, #238
	add	r3, fp
	lsls	r2, r2, #2
	lsls	r0, r0, #7
	movs	r4, #0
	adds	r1, r3, #0
	add	r2, fp
	adds	r0, #220
	mov	r8, r4
	adds	r1, #24
	adds	r2, r2, r0
.L_0817ece4:
	ldmia	r2!, {r3}
	ldrh	r3, [r3, #8]
	lsls	r3, r3, #22
	lsrs	r3, r3, #22
	str	r3, [r1, #0]
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	adds	r1, #28
	cmp	r4, #8
	bne.n	.L_0817ece4
	add	sl, r3
	movs	r0, #8
	mov	r1, sl
	add	r9, r0
	adds	r6, #36
	adds	r7, #9
	cmp	r1, #2
	bne.n	.L_0817ec3c
	ldr	r0, [pc, #408]
	bl	sub_08013300
	movs	r2, #224
	adds	r5, r0, #0
	adds	r1, r5, #0
	lsls	r2, r2, #1
	ldr	r3, [pc, #376]
	ldr	r0, [pc, #396]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x24e0
	lsls	r4, r4, #1
	adds	r5, r5, r4
	adds	r0, r5, #0
	ldr	r1, [pc, #384]
	bl	sub_0801587c
	movs	r5, #240
	ldr	r0, [pc, #360]
	ldr	r1, [pc, #352]
	lsls	r5, r5, #7
	ldr	r6, [pc, #372]
	movs	r7, #0
	adds	r5, #36
	mov	r8, r7
	mov	r9, r0
	mov	sl, r1
	add	r5, fp
.L_0817ed44:
	movs	r1, #32
	ldr	r2, [pc, #360]
	movs	r3, #0
	movs	r0, #32
	bl	sub_0815b290
	ldrb	r3, [r0, #9]
	movs	r7, #12
	orrs	r3, r7
	strb	r3, [r0, #9]
	ldrb	r3, [r0, #16]
	stmia	r5!, {r0}
	lsls	r3, r3, #2
	add	r3, r9
	ldrh	r0, [r3, #2]
	ldr	r2, [pc, #316]
	adds	r1, r6, #0
	adds	r0, r0, r2
	movs	r2, #128
	lsls	r2, r2, #3
	mov	lr, sl
	.2byte 0xf800
	.2byte 0x2401
	movs	r3, #128
	add	r8, r4
	lsls	r3, r3, #3
	mov	r0, r8
	adds	r6, r6, r3
	cmp	r0, #21
	bne.n	.L_0817ed44
	movs	r1, #64
	ldr	r2, [pc, #304]
	movs	r3, #0
	movs	r0, #32
	bl	sub_0815b290
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #120
	mov	r1, fp
	str	r0, [r1, r3]
	ldrb	r3, [r0, #9]
	ldr	r2, [pc, #256]
	orrs	r3, r7
	strb	r3, [r0, #9]
	ldrb	r3, [r0, #16]
	ldr	r7, [pc, #264]
	lsls	r3, r3, #2
	adds	r3, r3, r2
	ldrh	r0, [r3, #2]
	ldr	r3, [pc, #244]
	mov	r4, r8
	lsls	r1, r4, #10
	movs	r2, #128
	adds	r1, r1, r7
	lsls	r2, r2, #4
	adds	r0, r0, r3
	ldr	r3, [pc, #220]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x483e
	bl	sub_08013300
	adds	r5, r0, #0
	ldr	r4, [pc, #204]
	adds	r1, r5, #0
	movs	r2, #32
	adds	r5, #32
	ldr	r0, [pc, #236]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c28
	ldr	r1, [pc, #212]
	bl	sub_0801587c
	movs	r2, #128
	movs	r3, #224
	movs	r1, #8
	lsls	r2, r2, #7
	lsls	r3, r3, #8
	movs	r0, #16
	bl	sub_0815b290
	movs	r7, #13
	ldrb	r2, [r0, #9]
	negs	r7, r7
	adds	r3, r7, #0
	movs	r5, #241
	ands	r2, r3
	lsls	r5, r5, #7
	movs	r3, #4
	orrs	r2, r3
	add	r5, fp
	ldrb	r3, [r0, #16]
	strb	r2, [r0, #9]
	str	r0, [r5, #0]
	ldr	r0, [pc, #148]
	lsls	r3, r3, #2
	adds	r3, r3, r0
	ldrh	r0, [r3, #2]
	ldr	r1, [pc, #144]
	movs	r2, #128
	adds	r0, r0, r1
	lsls	r2, r2, #1
	ldr	r1, [pc, #148]
	ldr	r3, [pc, #124]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x26f0
	ldr	r7, [pc, #116]
	movs	r4, #0
	lsls	r6, r6, #7
	mov	r8, r4
	adds	r6, #132
.L_0817ee28:
	movs	r2, #128
	movs	r3, #240
	lsls	r3, r3, #8
	movs	r1, #8
	lsls	r2, r2, #7
	movs	r0, #16
	bl	sub_0815b3b0
	mov	r1, fp
	str	r0, [r6, r1]
	movs	r2, #24
	ldr	r1, [r5, #0]
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x465a
	ldr	r3, [r5, #0]
	ldr	r0, [r6, r2]
	ldrh	r2, [r3, #8]
	ldr	r1, [pc, #48]
	ldrh	r3, [r0, #8]
	lsls	r2, r2, #22
	lsrs	r2, r2, #22
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r0, #8]
	mov	r3, r8
	cmp	r3, #6
	ble.n	.L_0817ee70
	ldrb	r3, [r0, #9]
	movs	r4, #13
	negs	r4, r4
	adds	r2, r4, #0
	ands	r3, r2
	movs	r2, #8
	orrs	r3, r2
	strb	r3, [r0, #9]
.L_0817ee70:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r6, #4
	cmp	r1, #15
	bne.n	.L_0817ee28
	b.n	.L_0817eec0
	movs	r0, r0
	.4byte 0xfffffc00
	.4byte 0x0813baed
	.4byte 0x00000134
	.4byte 0x08197410
	.4byte 0x000000a0
	.4byte 0x03000730
	.4byte 0x050003e0
	.4byte 0x020036e0
	.4byte 0x06010000
	.4byte 0x000000a1
	.4byte 0x05000200
	.4byte 0x02010000
	.4byte 0x80002000
	.4byte 0xc000a000
	.4byte 0x000000a2
	.2byte 0x03c0
	.2byte 0x0500
.L_0817eec0:
	ldr	r2, [sp, #140]
	cmp	r2, #27
	bgt.n	.L_0817eec8
	b.n	.L_0817f020
.L_0817eec8:
	ldr	r3, [pc, #176]
	add	r6, sp, #224
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	ldr	r7, [pc, #172]
	str	r3, [sp, #184]
	str	r4, [sp, #188]
	movs	r3, #0
	str	r3, [r6, #12]
	movs	r3, #255
	lsls	r3, r3, #16
	str	r3, [r6, #4]
	ldr	r3, [sp, #112]
	movs	r4, #128
	lsls	r4, r4, #8
	adds	r4, r3, r4
	str	r4, [sp, #116]
	str	r4, [sp, #112]
	cmp	r4, r7
	ble.n	.L_0817ef16
	ldr	r2, [sp, #104]
	adds	r2, #7
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_0817eefe
	ldr	r3, [sp, #104]
	adds	r3, #14
.L_0817eefe:
	asrs	r3, r3, #3
	str	r3, [sp, #104]
	lsls	r3, r3, #3
	subs	r2, r2, r3
	ldr	r0, [sp, #116]
	ldr	r3, [pc, #120]
	ldr	r1, [sp, #112]
	adds	r0, r0, r3
	adds	r1, r1, r3
	str	r2, [sp, #104]
	str	r0, [sp, #116]
	str	r1, [sp, #112]
.L_0817ef16:
	mov	r3, sp
	movs	r4, #238
	movs	r0, #206
	adds	r3, #184
	lsls	r4, r4, #7
	lsls	r0, r0, #15
	adds	r4, #252
	str	r3, [sp, #36]
	str	r6, [sp, #92]
	str	r0, [sp, #12]
	movs	r2, #0
	add	r4, fp
	mov	sl, r2
	movs	r7, #0
	mov	r9, r4
.L_0817ef34:
	ldr	r2, [sp, #12]
	ldr	r5, [sp, #104]
	movs	r0, #238
	lsls	r3, r7, #2
	lsls	r0, r0, #7
	str	r2, [sp, #88]
	movs	r1, #0
	add	r3, fp
	adds	r0, #220
	mov	r8, r1
	adds	r5, #64
	adds	r4, r3, r0
.L_0817ef4c:
	ldr	r2, [sp, #116]
	mov	r1, r8
	ldr	r0, [sp, #92]
	lsls	r3, r1, #21
	adds	r3, r3, r2
	str	r3, [r0, #0]
	ldr	r1, [sp, #88]
	mov	r2, r8
	str	r1, [r0, #8]
	cmp	r2, #8
	bne.n	.L_0817ef88
	mov	r3, r9
	ldr	r0, [r3, #0]
	ldr	r3, [sp, #104]
	adds	r3, #71
	adds	r2, r3, #0
	cmp	r3, #0
	bge.n	.L_0817ef74
	ldr	r2, [sp, #104]
	adds	r2, #78
.L_0817ef74:
	asrs	r2, r2, #3
	lsls	r2, r2, #3
	subs	r2, r3, r2
	b.n	.L_0817efae
	.4byte 0x08196ed0
	.4byte 0x001fffff
	.2byte 0x0000
	.2byte 0xffe0
.L_0817ef88:
	.2byte 0x4640
	movs	r1, #238
	adds	r3, r7, r0
	lsls	r1, r1, #7
	lsls	r3, r3, #2
	adds	r1, #220
	adds	r3, r3, r1
	mov	r2, fp
	ldr	r0, [r2, r3]
	ldr	r3, [sp, #104]
	adds	r2, r5, #0
	add	r3, r8
	cmp	r5, #0
	bge.n	.L_0817efa8
	adds	r2, r3, #0
	adds	r2, #71
.L_0817efa8:
	asrs	r2, r2, #3
	lsls	r2, r2, #3
	subs	r2, r5, r2
.L_0817efae:
	adds	r2, r7, r2
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, #24
	mov	r2, fp
	ldr	r1, [r2, r3]
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r3, #255
	ands	r1, r3
	ldr	r2, [pc, #24]
	ldrh	r3, [r0, #8]
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r0, #8]
	ldr	r3, [r6, #0]
	ldr	r0, [pc, #16]
	cmp	r3, r0
	ble.n	.L_0817efec
	ldr	r1, [pc, #16]
	adds	r3, r3, r1
	str	r3, [r6, #0]
	b.n	.L_0817efec
	movs	r0, r0
	.4byte 0xfffffc00
	.4byte 0x00ffffff
	.2byte 0x0000
	.2byte 0xfee0
.L_0817efec:
	.2byte 0xcc01
	ldr	r2, [sp, #36]
	movs	r3, #0
	adds	r1, r6, #0
	str	r4, [sp, #8]
	bl	sub_08020010
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	adds	r5, #1
	ldr	r4, [sp, #8]
	cmp	r3, #9
	bne.n	.L_0817ef4c
	ldr	r0, [sp, #12]
	movs	r1, #128
	lsls	r1, r1, #14
	add	sl, r2
	movs	r4, #36
	adds	r0, r0, r1
	mov	r2, sl
	adds	r7, #9
	add	r9, r4
	str	r0, [sp, #12]
	cmp	r2, #2
	bne.n	.L_0817ef34
.L_0817f020:
	ldr	r3, [sp, #96]
	movs	r6, #128
	adds	r3, #4
	lsls	r6, r6, #19
	str	r3, [sp, #96]
	adds	r6, #40
	lsls	r3, r3, #8
	str	r3, [r6, #0]
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #232
	add	r3, fp
	movs	r7, #1
	str	r7, [r3, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r4, [sp, #140]
	adds	r4, #1
	str	r4, [sp, #140]
	cmp	r4, #52
	beq.n	.L_0817f04e
	b.n	.L_0817eace
.L_0817f04e:
	movs	r0, #104
	bl	sub_0801314c
	movs	r1, #240
	ldr	r0, [sp, #144]
	ldr	r5, [pc, #84]
	lsls	r1, r1, #6
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2180
	lsls	r1, r1, #7
	ldr	r0, [pc, #76]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4b0f
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #12
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	mov	r0, sp
	adds	r2, #20
	movs	r5, #0
	adds	r0, #176
	strh	r3, [r2, #0]
	str	r5, [r6, #0]
	str	r0, [sp, #84]
	ldr	r1, [sp, #84]
	movs	r0, #0
	bl	sub_08144aac
	movs	r3, #238
	lsls	r3, r3, #7
	movs	r2, #238
	adds	r3, #144
	lsls	r2, r2, #7
	add	r3, fp
	adds	r2, #148
	str	r5, [r3, #0]
	add	r2, fp
	movs	r3, #5
	str	r3, [r2, #0]
	movs	r2, #238
	b.n	.L_0817f0b8
	movs	r0, r0
	.4byte 0x00000786
	.4byte 0x00000080
	.4byte 0x03000258
	.2byte 0x4000
	.2byte 0x0600
.L_0817f0b8:
	lsls	r2, r2, #7
	adds	r2, #152
	add	r2, fp
	subs	r3, #6
	str	r3, [r2, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #156
	add	r3, fp
	str	r5, [r3, #0]
	ldr	r1, [sp, #132]
	ldr	r0, [pc, #160]
	str	r7, [r1, #16]
	movs	r1, #224
	lsls	r1, r1, #3
	add	r1, fp
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r1, #220
	lsls	r1, r1, #4
	ldr	r0, [pc, #140]
	add	r1, fp
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	movs	r1, #247
	lsls	r1, r1, #6
	ldr	r0, [pc, #128]
	add	r1, fp
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r1, [pc, #120]
	movs	r2, #0
	movs	r3, #0
	ldr	r0, [pc, #120]
	bl	sub_08157cf4
	ldr	r0, [pc, #116]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #112]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x22ef
	lsls	r2, r2, #7
	add	r2, fp
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, fp
	movs	r3, #50
	str	r3, [r2, #0]
	ldr	r3, [pc, #44]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [pc, #36]
	subs	r2, #2
	strh	r3, [r2, #0]
	ldr	r3, [pc, #68]
	movs	r2, #0
	str	r3, [sp, #80]
	mov	r8, r2
	ldrh	r3, [r3, #0]
	ldr	r2, [pc, #24]
	lsrs	r3, r3, #5
	ands	r3, r2
	cmp	r3, #31
	beq.n	.L_0817f1ac
	ldr	r0, [sp, #80]
	ldr	r1, [pc, #12]
	adds	r2, r0, #0
	b.n	.L_0817f190
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x00003f44
	.4byte 0x0000001f
	.4byte 0x000000ba
	.4byte 0x0000013e
	.4byte 0x000000bb
	.4byte 0x02010000
	.4byte 0x000000c2
	.4byte 0x00000161
	.4byte 0x03000730
	.2byte 0x0200
	.2byte 0x0500
.L_0817f190:
	movs	r4, #1
	add	r8, r4
	mov	r7, r8
	cmp	r7, #224
	beq.n	.L_0817f1ac
	lsls	r3, r7, #1
	adds	r3, r3, r0
	str	r3, [sp, #80]
	adds	r2, #2
	ldrh	r3, [r2, #0]
	lsrs	r3, r3, #5
	ands	r3, r1
	cmp	r3, #31
	bne.n	.L_0817f190
.L_0817f1ac:
	ldr	r3, [pc, #144]
	movs	r0, #0
	movs	r1, #1
	movs	r2, #128
	mov	r8, r0
	negs	r1, r1
	lsls	r2, r2, #2
.L_0817f1ba:
	movs	r4, #1
	add	r8, r4
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r8, r2
	bne.n	.L_0817f1ba
	movs	r7, #0
	str	r7, [sp, #140]
	ldr	r1, [pc, #120]
	movs	r2, #2
	ldr	r3, [r1, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0817f1da
	bl	.L_081806f0
.L_0817f1da:
	ldr	r3, [r1, #12]
	movs	r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_0817f1e8
	bl	.L_081806f0
.L_0817f1e8:
	mov	r0, sp
	mov	r1, sp
	adds	r0, #208
	adds	r1, #168
	str	r0, [sp, #28]
	str	r1, [sp, #40]
.L_0817f1f4:
	ldr	r2, [sp, #140]
	cmp	r2, #0
	beq.n	.L_0817f1fc
	b.n	.L_0817f390
.L_0817f1fc:
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r4, [sp, #80]
	ldr	r3, [pc, #48]
	ldr	r7, [pc, #60]
	strh	r3, [r4, #0]
	movs	r0, #0
	movs	r3, #240
	str	r7, [sp, #108]
	str	r0, [sp, #124]
	lsls	r3, r3, #7
	adds	r3, #120
	add	r3, fp
	ldr	r2, [r3, #0]
	movs	r3, #192
	lsls	r3, r3, #8
	strh	r3, [r2, #18]
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #116
	add	r3, fp
	add	r1, sp, #140
	ldr	r3, [r3, #0]
	ldrh	r1, [r1, #0]
	movs	r5, #224
	b.n	.L_0817f24c
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x0000190b
	.4byte 0x02014018
	.4byte 0x03001150
	.2byte 0x0000
	.2byte 0xffe3
.L_0817f24c:
	.2byte 0x8259
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #88
	add	r3, fp
	ldr	r3, [r3, #0]
	adds	r2, r1, #0
	strh	r2, [r3, #18]
	ldr	r4, [sp, #140]
	movs	r3, #140
	lsls	r3, r3, #2
	add	r3, fp
	str	r4, [r3, #0]
	movs	r3, #141
	lsls	r3, r3, #2
	add	r3, fp
	str	r4, [r3, #0]
	movs	r2, #143
	ldr	r3, [pc, #344]
	lsls	r2, r2, #2
	add	r2, fp
	str	r3, [r2, #0]
	movs	r3, #144
	lsls	r3, r3, #2
	add	r3, fp
	str	r4, [r3, #0]
	movs	r3, #146
	lsls	r3, r3, #2
	add	r3, fp
	str	r4, [r3, #0]
	movs	r3, #161
	lsls	r3, r3, #2
	add	r3, fp
	str	r4, [r3, #0]
	movs	r3, #162
	lsls	r3, r3, #2
	add	r3, fp
	str	r4, [r3, #0]
	movs	r2, #164
	ldr	r3, [pc, #308]
	lsls	r2, r2, #2
	add	r2, fp
	str	r3, [r2, #0]
	movs	r3, #165
	lsls	r3, r3, #2
	add	r3, fp
	str	r4, [r3, #0]
	movs	r3, #167
	lsls	r3, r3, #2
	add	r3, fp
	str	r4, [r3, #0]
	movs	r3, #147
	lsls	r3, r3, #2
	add	r3, fp
	str	r4, [r3, #0]
	movs	r3, #148
	lsls	r3, r3, #2
	add	r3, fp
	str	r4, [r3, #0]
	movs	r2, #150
	ldr	r3, [pc, #268]
	lsls	r2, r2, #2
	add	r2, fp
	str	r3, [r2, #0]
	movs	r3, #151
	lsls	r3, r3, #2
	add	r3, fp
	str	r4, [r3, #0]
	movs	r3, #153
	lsls	r3, r3, #2
	add	r3, fp
	movs	r2, #154
	str	r4, [r3, #0]
	lsls	r2, r2, #2
	movs	r3, #160
	lsls	r3, r3, #16
	add	r2, fp
	str	r3, [r2, #0]
	movs	r3, #155
	lsls	r3, r3, #2
	add	r3, fp
	str	r4, [r3, #0]
	movs	r3, #157
	lsls	r3, r3, #2
	add	r3, fp
	str	r4, [r3, #0]
	movs	r3, #158
	lsls	r3, r3, #2
	add	r3, fp
	str	r4, [r3, #0]
	movs	r3, #160
	lsls	r3, r3, #2
	add	r3, fp
	str	r4, [r3, #0]
	lsls	r5, r5, #2
	mov	r8, r0
	movs	r6, #0
	add	r5, fp
.L_0817f310:
	mov	r0, r8
	movs	r1, #3
	bl	sub_08002054
	movs	r7, #176
	lsls	r0, r0, #18
	lsls	r7, r7, #16
	adds	r0, r0, r7
	str	r0, [r5, #0]
	movs	r1, #3
	mov	r0, r8
	bl	sub_08002064
	lsls	r3, r0, #1
	adds	r3, r3, r0
	movs	r1, #1
	movs	r0, #148
	lsls	r3, r3, #17
	lsls	r0, r0, #15
	add	r8, r1
	adds	r3, r3, r0
	mov	r2, r8
	str	r3, [r5, #4]
	str	r6, [r5, #12]
	str	r6, [r5, #16]
	str	r6, [r5, #24]
	adds	r5, #28
	cmp	r2, #8
	bne.n	.L_0817f310
	movs	r5, #140
	movs	r3, #0
	lsls	r5, r5, #3
	mov	r8, r3
	movs	r6, #0
	add	r5, fp
.L_0817f356:
	mov	r0, r8
	movs	r1, #3
	bl	sub_08002054
	movs	r4, #224
	lsls	r4, r4, #15
	lsls	r0, r0, #18
	adds	r0, r0, r4
	str	r0, [r5, #0]
	movs	r1, #3
	mov	r0, r8
	bl	sub_08002064
	lsls	r3, r0, #1
	adds	r3, r3, r0
	movs	r7, #164
	movs	r0, #1
	lsls	r3, r3, #17
	lsls	r7, r7, #15
	add	r8, r0
	adds	r3, r3, r7
	mov	r1, r8
	str	r3, [r5, #4]
	str	r6, [r5, #12]
	str	r6, [r5, #16]
	str	r6, [r5, #24]
	adds	r5, #28
	cmp	r1, #8
	bne.n	.L_0817f356
.L_0817f390:
	ldr	r4, [sp, #140]
	ldr	r7, [pc, #68]
	ldr	r2, [sp, #124]
	adds	r3, r4, r7
	str	r2, [sp, #120]
	cmp	r3, #15
	bhi.n	.L_0817f3e8
	ldr	r0, [sp, #80]
	ldr	r2, [pc, #36]
	ldrh	r3, [r0, #0]
	movs	r0, #31
	ands	r0, r3
	lsls	r3, r3, #16
	lsrs	r1, r3, #21
	lsrs	r3, r3, #26
	ands	r1, r2
	ands	r3, r2
	cmp	r0, #30
	bgt.n	.L_0817f3b8
	adds	r0, #1
.L_0817f3b8:
	cmp	r1, #19
	bgt.n	.L_0817f3be
	adds	r1, #1
.L_0817f3be:
	cmp	r3, #5
	bgt.n	.L_0817f3dc
	adds	r3, #1
	b.n	.L_0817f3dc
	movs	r0, r0
	.4byte 0x0000001f
	.4byte 0xfffa0000
	.4byte 0xfff40000
	.4byte 0xffff0000
	.2byte 0xfde2
	.2byte 0xffff
.L_0817f3dc:
	.2byte 0x014a
	lsls	r3, r3, #10
	ldr	r1, [sp, #80]
	orrs	r3, r2
	orrs	r3, r0
	strh	r3, [r1, #0]
.L_0817f3e8:
	movs	r3, #151
	ldr	r2, [sp, #140]
	lsls	r3, r3, #1
	adds	r3, #255
	cmp	r2, r3
	ble.n	.L_0817f42c
	ldr	r4, [sp, #80]
	ldr	r2, [pc, #36]
	ldrh	r3, [r4, #0]
	movs	r0, #31
	ands	r0, r3
	lsls	r3, r3, #16
	lsrs	r1, r3, #21
	lsrs	r3, r3, #26
	ands	r1, r2
	ands	r3, r2
	cmp	r0, #20
	ble.n	.L_0817f40e
	subs	r0, #1
.L_0817f40e:
	cmp	r1, #0
	ble.n	.L_0817f414
	subs	r1, #1
.L_0817f414:
	cmp	r3, #0
	ble.n	.L_0817f420
	subs	r3, #1
	b.n	.L_0817f420
	.2byte 0x001f
	.2byte 0x0000
.L_0817f420:
	lsls	r3, r3, #10
	lsls	r2, r1, #5
	ldr	r7, [sp, #80]
	orrs	r3, r2
	orrs	r3, r0
	strh	r3, [r7, #0]
.L_0817f42c:
	ldr	r0, [sp, #140]
	movs	r1, #140
	lsls	r1, r1, #1
	cmp	r0, r1
	bne.n	.L_0817f454
	ldr	r0, [pc, #112]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #104]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2292
	lsls	r2, r2, #2
	add	r2, fp
	movs	r3, #1
	str	r3, [r2, #0]
.L_0817f454:
	ldr	r2, [sp, #140]
	movs	r3, #144
	lsls	r3, r3, #1
	cmp	r2, r3
	bne.n	.L_0817f468
	movs	r2, #167
	lsls	r2, r2, #2
	add	r2, fp
	movs	r3, #1
	str	r3, [r2, #0]
.L_0817f468:
	ldr	r4, [sp, #140]
	movs	r7, #24
	adds	r7, #255
	cmp	r4, r7
	bne.n	.L_0817f488
	ldr	r1, [pc, #44]
	movs	r0, #128
	movs	r2, #128
	ldr	r3, [pc, #40]
	lsls	r0, r0, #19
	lsls	r2, r2, #19
	adds	r0, #32
	adds	r2, #12
	strh	r1, [r0, #0]
	strh	r3, [r2, #0]
	strh	r1, [r0, #0]
.L_0817f488:
	ldr	r0, [sp, #140]
	movs	r1, #253
	lsls	r1, r1, #1
	cmp	r0, r1
	bne.n	.L_0817f4b0
	movs	r2, #153
	lsls	r2, r2, #2
	add	r2, fp
	movs	r3, #1
	str	r3, [r2, #0]
	b.n	.L_0817f4b0
	movs	r0, r0
	.4byte 0x00000080
	.4byte 0x00000784
	.4byte 0x00000161
	.2byte 0x0730
	.2byte 0x0300
.L_0817f4b0:
	ldr	r2, [sp, #140]
	movs	r3, #234
	adds	r3, #255
	cmp	r2, r3
	bne.n	.L_0817f4d6
	ldr	r3, [pc, #52]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	subs	r2, #20
	strh	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #152
	add	r2, fp
	movs	r3, #0
	str	r3, [r2, #0]
.L_0817f4d6:
	ldr	r4, [sp, #140]
	movs	r7, #132
	lsls	r7, r7, #2
	cmp	r4, r7
	bne.n	.L_0817f500
	ldr	r3, [pc, #12]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r3, [pc, #12]
	b.n	.L_0817f4fc
	movs	r0, r0
	.4byte 0x00000080
	.4byte 0x00000784
	.2byte 0x0785
	.2byte 0x0000
.L_0817f4fc:
	subs	r2, #20
	strh	r3, [r2, #0]
.L_0817f500:
	ldr	r0, [sp, #140]
	cmp	r0, #0
	bne.n	.L_0817f55a
	movs	r1, #239
	movs	r2, #238
	lsls	r1, r1, #7
	lsls	r2, r2, #7
	add	r1, fp
	movs	r3, #1
	adds	r2, #132
	str	r3, [r1, #0]
	add	r2, fp
	movs	r3, #2
	str	r0, [r2, #0]
	str	r3, [r1, #0]
	movs	r3, #50
	str	r3, [r2, #0]
	ldr	r7, [pc, #376]
	ldr	r6, [pc, #376]
	ldr	r5, [pc, #380]
	movs	r1, #0
	mov	r8, r1
.L_0817f52c:
	ldrb	r3, [r6, #0]
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	ldrb	r3, [r6, #1]
	adds	r6, #2
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	ldr	r3, [r7, #0]
	str	r3, [r5, #12]
	ldr	r3, [r7, #4]
	adds	r7, #8
	str	r3, [r5, #16]
	bl	sub_08014878
	movs	r3, #127
	movs	r2, #1
	ands	r3, r0
	add	r8, r2
	str	r3, [r5, #24]
	mov	r3, r8
	adds	r5, #28
	cmp	r3, #10
	bne.n	.L_0817f52c
.L_0817f55a:
	ldr	r5, [pc, #328]
	movs	r4, #0
	mov	r8, r4
.L_0817f560:
	ldr	r3, [pc, #324]
	mov	r7, r8
	ldrb	r3, [r3, r7]
	ldr	r0, [sp, #140]
	cmp	r0, r3
	blt.n	.L_0817f5b6
	ldr	r0, [r5, #24]
	ldr	r6, [pc, #316]
	cmp	r0, #0
	bge.n	.L_0817f576
	adds	r0, #3
.L_0817f576:
	movs	r1, #12
	asrs	r0, r0, #2
	bl	sub_08002064
	ldrb	r1, [r6, r0]
	movs	r2, #247
	lsls	r1, r1, #8
	movs	r0, #16
	lsls	r2, r2, #6
	add	r1, fp
	adds	r1, r1, r2
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	movs	r4, #6
	ldrsh	r3, [r5, r4]
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	ldr	r4, [sp, #176]
	ldr	r0, [sp, #144]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x682b
	ldr	r2, [r5, #12]
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r2, [r5, #16]
	ldr	r3, [r5, #4]
	adds	r3, r3, r2
	str	r3, [r5, #4]
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
.L_0817f5b6:
	movs	r7, #1
	add	r8, r7
	mov	r0, r8
	adds	r5, #28
	cmp	r0, #10
	bne.n	.L_0817f560
	ldr	r1, [sp, #140]
	movs	r2, #218
	adds	r2, #255
	cmp	r1, r2
	bgt.n	.L_0817f5d6
	ldr	r3, [sp, #112]
	movs	r4, #128
	lsls	r4, r4, #8
	adds	r3, r3, r4
	str	r3, [sp, #112]
.L_0817f5d6:
	ldr	r7, [sp, #112]
	ldr	r0, [sp, #124]
	movs	r1, #128
	adds	r7, r7, r0
	lsls	r1, r1, #14
	str	r7, [sp, #116]
	cmp	r7, r1
	blt.n	.L_0817f60c
	ldr	r2, [sp, #104]
	adds	r2, #7
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_0817f5f4
	ldr	r3, [sp, #104]
	adds	r3, #14
.L_0817f5f4:
	asrs	r3, r3, #3
	str	r3, [sp, #104]
	lsls	r3, r3, #3
	subs	r2, r2, r3
	ldr	r4, [sp, #112]
	ldr	r3, [pc, #176]
	str	r2, [sp, #104]
	ldr	r2, [sp, #116]
	adds	r4, r4, r3
	adds	r2, r2, r3
	str	r2, [sp, #116]
	str	r4, [sp, #112]
.L_0817f60c:
	ldr	r7, [sp, #108]
	cmp	r7, #0
	bge.n	.L_0817f61c
	movs	r1, #128
	adds	r0, r7, #0
	lsls	r1, r1, #6
	adds	r0, r0, r1
	str	r0, [sp, #108]
.L_0817f61c:
	ldr	r3, [pc, #148]
	movs	r2, #0
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	str	r2, [sp, #68]
	str	r3, [sp, #168]
	str	r4, [sp, #172]
	movs	r4, #141
	ldr	r3, [sp, #140]
	lsls	r4, r4, #1
	adds	r4, #255
	str	r2, [sp, #64]
	cmp	r3, r4
	ble.n	.L_0817f6cc
	ldr	r5, [pc, #124]
	adds	r0, r5, #0
	bl	sub_08002090
	lsls	r0, r0, #2
	str	r0, [sp, #76]
	adds	r0, r5, #0
	bl	sub_08002096
	lsls	r0, r0, #2
	str	r0, [sp, #72]
	ldr	r7, [sp, #140]
	ldr	r0, [pc, #104]
	adds	r3, r7, r0
	cmp	r3, #15
	bhi.n	.L_0817f674
	ldr	r1, [pc, #100]
	lsls	r5, r7, #11
	adds	r5, r5, r1
	adds	r0, r5, #0
	bl	sub_08002096
	lsls	r3, r0, #1
	adds	r3, r3, r0
	adds	r0, r5, #0
	str	r3, [sp, #68]
	bl	sub_08002096
	lsls	r0, r0, #1
	str	r0, [sp, #64]
.L_0817f674:
	ldr	r2, [sp, #140]
	ldr	r4, [pc, #76]
	adds	r3, r2, r4
	cmp	r3, #15
	bhi.n	.L_0817f6e4
	ldr	r7, [pc, #72]
	lsls	r5, r2, #11
	adds	r5, r5, r7
	adds	r0, r5, #0
	bl	sub_08002096
	lsls	r0, r0, #1
	str	r0, [sp, #68]
	adds	r0, r5, #0
	bl	sub_08002096
	lsls	r3, r0, #1
	adds	r3, r3, r0
	str	r3, [sp, #64]
	b.n	.L_0817f6e4
	.4byte 0x08199528
	.4byte 0x08199514
	.4byte 0x020176e8
	.4byte 0x08199578
	.4byte 0x08199508
	.4byte 0xffe00000
	.4byte 0x08196ed8
	.4byte 0x00021a00
	.4byte 0xfffffde2
	.4byte 0xffef1000
	.4byte 0xfffffdd0
	.2byte 0x8000
	.2byte 0xffee
.L_0817f6cc:
	.2byte 0x9823
	lsls	r5, r0, #8
	adds	r0, r5, #0
	bl	sub_08002090
	lsls	r0, r0, #2
	str	r0, [sp, #76]
	adds	r0, r5, #0
	bl	sub_08002096
	lsls	r0, r0, #2
	str	r0, [sp, #72]
.L_0817f6e4:
	ldr	r1, [sp, #72]
	cmp	r1, #0
	ble.n	.L_0817f6ee
	negs	r1, r1
	str	r1, [sp, #72]
.L_0817f6ee:
	ldr	r2, [sp, #108]
	ldr	r4, [sp, #72]
	ldr	r7, [sp, #28]
	lsls	r3, r2, #1
	subs	r4, r4, r3
	movs	r6, #238
	movs	r3, #0
	str	r4, [sp, #72]
	lsls	r6, r6, #7
	str	r3, [r7, #12]
	ldr	r5, [pc, #904]
	movs	r3, #255
	lsls	r3, r3, #16
	movs	r0, #0
	adds	r6, #168
	str	r3, [r7, #4]
	mov	r8, r0
	add	r6, fp
	movs	r7, #23
.L_0817f714:
	ldrh	r3, [r5, #0]
	ldr	r1, [sp, #140]
	adds	r5, #2
	cmp	r1, r3
	bne.n	.L_0817f726
	str	r7, [r6, #0]
	movs	r0, #144
	bl	sub_081c0010
.L_0817f726:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	cmp	r3, #5
	bne.n	.L_0817f714
	ldr	r4, [sp, #140]
	movs	r7, #255
	lsls	r7, r7, #1
	cmp	r4, r7
	bne.n	.L_0817f740
	movs	r0, #186
	bl	sub_081c0010
.L_0817f740:
	ldr	r0, [sp, #140]
	movs	r1, #142
	lsls	r1, r1, #2
	cmp	r0, r1
	bne.n	.L_0817f75c
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, fp
	movs	r3, #10
	str	r3, [r2, #0]
	movs	r0, #145
	bl	sub_081c0010
.L_0817f75c:
	movs	r3, #128
	ldr	r2, [sp, #140]
	lsls	r3, r3, #2
	adds	r3, #66
	cmp	r2, r3
	bne.n	.L_0817f76e
	movs	r0, #163
	bl	sub_081c0010
.L_0817f76e:
	movs	r5, #224
	movs	r7, #146
	movs	r4, #0
	lsls	r5, r5, #2
	lsls	r7, r7, #2
	mov	r8, r4
	movs	r6, #0
	add	r5, fp
	add	r7, fp
.L_0817f780:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	beq.n	.L_0817f7f0
	ldr	r2, [r5, #0]
	ldr	r0, [pc, #772]
	adds	r3, #1
	str	r3, [r5, #24]
	cmp	r2, r0
	ble.n	.L_0817f7b4
	ldr	r3, [r5, #12]
	movs	r1, #255
	adds	r3, r2, r3
	str	r3, [r5, #0]
	ldr	r2, [r5, #16]
	ldr	r3, [r5, #4]
	lsls	r1, r1, #8
	ldr	r4, [pc, #752]
	adds	r3, r3, r2
	adds	r1, #255
	str	r3, [r5, #4]
	adds	r3, r2, r1
	cmp	r3, r4
	bhi.n	.L_0817f7b4
	ldr	r3, [r5, #20]
	adds	r3, r2, r3
	str	r3, [r5, #16]
.L_0817f7b4:
	ldr	r3, [r5, #12]
	ldr	r0, [pc, #736]
	cmp	r3, r0
	ble.n	.L_0817f7c2
	ldr	r1, [pc, #732]
	adds	r3, r3, r1
	str	r3, [r5, #12]
.L_0817f7c2:
	ldr	r3, [r5, #24]
	mov	r2, r8
	lsls	r1, r2, #5
	movs	r2, #31
	ands	r3, r2
	adds	r1, r1, r3
	lsls	r2, r1, #3
	ldr	r3, [pc, #716]
	subs	r2, r2, r1
	lsls	r2, r2, #2
	adds	r2, r2, r3
	str	r6, [r2, #24]
	ldr	r4, [pc, #712]
	ldr	r3, [r5, #0]
	lsrs	r1, r3, #31
	adds	r3, r3, r1
	asrs	r3, r3, #1
	str	r3, [r2, #0]
	ldr	r3, [r5, #4]
	str	r6, [r2, #12]
	adds	r3, r3, r4
	str	r3, [r2, #4]
	str	r6, [r2, #16]
.L_0817f7f0:
	movs	r3, #7
	mov	r0, r8
	ands	r3, r0
	lsls	r2, r3, #1
	adds	r2, r2, r3
	ldr	r3, [r7, #0]
	lsls	r2, r2, #1
	adds	r2, #10
	cmp	r3, r2
	bne.n	.L_0817f854
	movs	r0, #138
	bl	sub_081c0010
	movs	r3, #1
	str	r3, [r5, #24]
	ldr	r1, [sp, #76]
	ldr	r3, [r5, #0]
	ldr	r2, [pc, #660]
	adds	r3, r3, r1
	str	r3, [r5, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #168
	add	r3, fp
	ldr	r3, [r3, #0]
	ldrsb	r3, [r2, r3]
	ldr	r2, [sp, #72]
	lsls	r3, r3, #16
	subs	r3, r2, r3
	ldr	r2, [r5, #4]
	adds	r2, r2, r3
	str	r2, [r5, #4]
	bl	sub_08014878
	movs	r3, #255
	ldr	r4, [pc, #612]
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r0
	adds	r3, r3, r4
	str	r3, [r5, #16]
	bl	sub_08014878
	movs	r3, #252
	lsls	r3, r3, #6
	adds	r3, #255
	ands	r3, r0
	ldr	r0, [pc, #604]
	adds	r3, r3, r0
	str	r3, [r5, #20]
.L_0817f854:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	adds	r5, #28
	cmp	r2, #16
	bne.n	.L_0817f780
	ldr	r3, [sp, #140]
	movs	r4, #234
	adds	r4, #255
	cmp	r3, r4
	bgt.n	.L_0817f8aa
	movs	r7, #8
	movs	r6, #240
	mov	r8, r7
	lsls	r6, r6, #7
	movs	r5, #140
	ldr	r7, [sp, #28]
	adds	r6, #156
	lsls	r5, r5, #3
	add	r6, fp
	add	r5, fp
.L_0817f87e:
	ldr	r3, [r5, #0]
	ldr	r0, [sp, #124]
	adds	r3, r3, r0
	str	r3, [r7, #0]
	ldr	r3, [r5, #4]
	str	r3, [r7, #8]
	ldr	r3, [r5, #24]
	cmp	r3, #0
	beq.n	.L_0817f89c
	ldr	r0, [r6, #0]
	ldr	r1, [sp, #28]
	ldr	r2, [sp, #40]
	movs	r3, #0
	bl	sub_08020010
.L_0817f89c:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	adds	r6, #4
	adds	r5, #28
	cmp	r2, #16
	bne.n	.L_0817f87e
.L_0817f8aa:
	movs	r4, #128
	ldr	r3, [sp, #140]
	lsls	r4, r4, #2
	adds	r4, #66
	cmp	r3, r4
	bne.n	.L_0817f8ce
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #148
	add	r2, fp
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #144
	add	r2, fp
	movs	r3, #0
	str	r3, [r2, #0]
.L_0817f8ce:
	ldr	r7, [sp, #140]
	ldr	r0, [pc, #476]
	adds	r3, r7, r0
	cmp	r3, #15
	bhi.n	.L_0817f8ee
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #152
	add	r2, fp
	cmp	r3, #0
	bge.n	.L_0817f8e8
	ldr	r1, [pc, #460]
	adds	r3, r7, r1
.L_0817f8e8:
	asrs	r3, r3, #3
	mvns	r3, r3
	str	r3, [r2, #0]
.L_0817f8ee:
	ldr	r4, [sp, #124]
	ldr	r7, [sp, #76]
	ldr	r0, [sp, #68]
	movs	r6, #240
	subs	r3, r4, r7
	movs	r1, #240
	lsls	r6, r6, #7
	ldr	r5, [sp, #28]
	ldr	r4, [pc, #424]
	movs	r2, #0
	adds	r3, r3, r0
	lsls	r1, r1, #15
	adds	r6, #92
	mov	r8, r2
	adds	r7, r3, r1
	add	r6, fp
.L_0817f90e:
	mov	r2, r8
	lsls	r3, r2, #5
	movs	r2, #238
	str	r7, [r5, #0]
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, fp
	ldr	r2, [r2, #0]
	ldr	r0, [sp, #72]
	ldrsb	r2, [r4, r2]
	ldr	r1, [sp, #64]
	subs	r3, r3, r2
	lsls	r3, r3, #16
	adds	r3, r3, r0
	movs	r2, #200
	adds	r3, r3, r1
	lsls	r2, r2, #15
	adds	r3, r3, r2
	str	r3, [r5, #8]
	ldmia	r6!, {r0}
	movs	r3, #0
	adds	r1, r5, #0
	ldr	r2, [sp, #40]
	str	r4, [sp, #8]
	bl	sub_08020010
	movs	r3, #1
	add	r8, r3
	mov	r0, r8
	ldr	r4, [sp, #8]
	cmp	r0, #2
	bne.n	.L_0817f90e
	ldr	r1, [sp, #140]
	movs	r2, #158
	lsls	r2, r2, #1
	cmp	r1, r2
	bgt.n	.L_0817f9f8
	movs	r0, #161
	lsls	r0, r0, #2
	add	r0, fp
	ldr	r3, [r0, #24]
	cmp	r3, #0
	beq.n	.L_0817f9b2
	adds	r3, #1
	ldr	r1, [r0, #12]
	str	r3, [r0, #24]
	ldr	r3, [r0, #0]
	ldr	r2, [r0, #16]
	adds	r3, r3, r1
	str	r3, [r0, #0]
	ldr	r3, [r0, #4]
	adds	r3, r3, r2
	str	r3, [r0, #4]
	movs	r3, #128
	lsls	r3, r3, #6
	adds	r2, r2, r3
	lsls	r3, r1, #1
	adds	r3, r3, r1
	lsls	r3, r3, #4
	str	r2, [r0, #16]
	cmp	r3, #0
	bge.n	.L_0817f98c
	adds	r3, #63
.L_0817f98c:
	asrs	r3, r3, #6
	str	r3, [r0, #12]
	lsls	r3, r2, #6
	subs	r2, r3, r2
	cmp	r2, #0
	bge.n	.L_0817f99a
	adds	r2, #63
.L_0817f99a:
	asrs	r3, r2, #6
	str	r3, [r0, #16]
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #116
	add	r3, fp
	ldr	r2, [r3, #0]
	movs	r4, #128
	ldrh	r3, [r2, #18]
	lsls	r4, r4, #2
	adds	r3, r3, r4
	strh	r3, [r2, #18]
.L_0817f9b2:
	ldr	r3, [r0, #0]
	ldr	r7, [sp, #76]
	ldr	r1, [sp, #124]
	subs	r3, r3, r7
	ldr	r4, [sp, #28]
	movs	r2, #240
	adds	r3, r3, r1
	lsls	r2, r2, #15
	adds	r3, r3, r2
	str	r3, [r4, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #168
	add	r3, fp
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #212]
	ldr	r7, [sp, #72]
	ldrsb	r2, [r2, r3]
	movs	r3, #99
	subs	r3, r3, r2
	ldr	r2, [r0, #4]
	lsls	r3, r3, #16
	adds	r3, r3, r7
	adds	r3, r3, r2
	str	r3, [r4, #8]
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #116
	add	r3, fp
	ldr	r0, [r3, #0]
	ldr	r1, [sp, #28]
	ldr	r2, [sp, #40]
	movs	r3, #0
	bl	sub_08020010
.L_0817f9f8:
	ldr	r0, [sp, #124]
	ldr	r1, [sp, #68]
	ldr	r4, [sp, #28]
	movs	r2, #142
	adds	r6, r0, r1
	lsls	r2, r2, #16
	adds	r3, r6, r2
	movs	r0, #238
	str	r3, [r4, #0]
	lsls	r0, r0, #7
	ldr	r7, [pc, #152]
	adds	r0, #168
	add	r0, fp
	ldr	r3, [r0, #0]
	mov	sl, r7
	mov	r1, sl
	ldrsb	r3, [r1, r3]
	ldr	r2, [sp, #72]
	movs	r5, #72
	ldr	r4, [sp, #64]
	subs	r3, r5, r3
	ldr	r7, [sp, #28]
	lsls	r3, r3, #16
	adds	r3, r3, r2
	adds	r3, r3, r4
	str	r3, [r7, #8]
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #36
	add	r3, fp
	ldr	r1, [sp, #28]
	ldr	r2, [sp, #40]
	mov	r8, r0
	ldr	r0, [r3, #0]
	movs	r3, #0
	bl	sub_08020010
	movs	r0, #174
	lsls	r0, r0, #16
	adds	r3, r6, r0
	str	r3, [r7, #0]
	mov	r1, r8
	ldr	r3, [r1, #0]
	mov	r2, sl
	ldrsb	r3, [r2, r3]
	ldr	r4, [sp, #64]
	subs	r5, r5, r3
	ldr	r3, [sp, #72]
	lsls	r5, r5, #16
	adds	r5, r5, r3
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r5, r5, r4
	adds	r3, #40
	str	r5, [r7, #8]
	add	r3, fp
	ldr	r0, [r3, #0]
	ldr	r1, [sp, #28]
	ldr	r2, [sp, #40]
	movs	r3, #0
	bl	sub_08020010
	movs	r7, #128
	ldr	r0, [sp, #28]
	lsls	r7, r7, #16
	adds	r6, r6, r7
	str	r6, [r0, #0]
	mov	r1, r8
	ldr	r3, [r1, #0]
	mov	r4, sl
	ldrsb	r2, [r4, r3]
	ldr	r7, [sp, #72]
	b.n	.L_0817fab8
	movs	r0, r0
	.4byte 0x08199582
	.4byte 0xfff00000
	.4byte 0x0001fffe
	.4byte 0xfff80000
	.4byte 0xffff8000
	.4byte 0x02014000
	.4byte 0xffec0000
	.4byte 0x081994f0
	.4byte 0xffffe000
	.4byte 0xfffffdbe
	.2byte 0xfdc5
	.2byte 0xffff
.L_0817fab8:
	.2byte 0x2368
	ldr	r0, [sp, #64]
	subs	r3, r3, r2
	ldr	r1, [sp, #28]
	lsls	r3, r3, #16
	adds	r3, r3, r7
	adds	r3, r3, r0
	str	r3, [r1, #8]
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #44
	add	r3, fp
	ldr	r0, [r3, #0]
	ldr	r2, [sp, #40]
	movs	r3, #0
	ldr	r1, [sp, #28]
	bl	sub_08020010
	movs	r3, #128
	ldr	r2, [sp, #140]
	movs	r7, #154
	lsls	r3, r3, #2
	lsls	r7, r7, #2
	adds	r3, #90
	add	r7, fp
	cmp	r2, r3
	ble.n	.L_0817faf4
	ldr	r0, [pc, #636]
	bl	sub_0815f0a0
.L_0817faf4:
	movs	r0, #128
	ldr	r4, [sp, #140]
	lsls	r0, r0, #2
	adds	r0, #58
	cmp	r4, r0
	bne.n	.L_0817fba0
	movs	r3, #152
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	mov	r1, r8
	ldr	r3, [r1, #0]
	mov	r4, sl
	ldrsb	r2, [r4, r3]
	ldr	r0, [sp, #72]
	movs	r3, #92
	subs	r3, r3, r2
	lsls	r3, r3, #16
	adds	r3, r3, r0
	str	r3, [r7, #4]
	movs	r3, #1
	str	r3, [r7, #24]
	ldr	r3, [pc, #592]
	ldr	r5, [pc, #592]
	str	r3, [r7, #12]
	ldr	r3, [pc, #592]
	movs	r1, #0
	str	r3, [r7, #16]
	mov	r8, r1
	movs	r6, #31
.L_0817fb2e:
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	adds	r3, #56
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	bl	sub_08014878
	ands	r0, r6
	adds	r0, #64
	lsls	r0, r0, #16
	str	r0, [r5, #4]
	bl	sub_08014878
	ands	r0, r6
	adds	r0, #8
	negs	r0, r0
	lsls	r0, r0, #12
	str	r0, [r5, #12]
	bl	sub_08014878
	ands	r0, r6
	adds	r0, #8
	movs	r2, #1
	movs	r3, #0
	negs	r0, r0
	add	r8, r2
	str	r3, [r5, #24]
	lsls	r0, r0, #12
	mov	r3, r8
	str	r0, [r5, #16]
	adds	r5, #28
	cmp	r3, #16
	bne.n	.L_0817fb2e
	ldr	r3, [pc, #516]
	movs	r4, #0
	mov	r8, r4
	subs	r2, #2
.L_0817fb7c:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r1, #32
	bne.n	.L_0817fb7c
	ldr	r3, [pc, #500]
	movs	r2, #0
	mov	r8, r2
	subs	r2, #1
.L_0817fb92:
	movs	r4, #1
	add	r8, r4
	mov	r0, r8
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r0, #32
	bne.n	.L_0817fb92
.L_0817fba0:
	ldr	r1, [r7, #24]
	cmp	r1, #0
	bne.n	.L_0817fba8
	b.n	.L_0817fe98
.L_0817fba8:
	cmp	r1, #9
	ble.n	.L_0817fc1e
	movs	r3, #31
	ands	r3, r1
	lsls	r5, r3, #3
	ldr	r1, [pc, #464]
	movs	r6, #240
	subs	r5, r5, r3
	lsls	r6, r6, #7
	lsls	r5, r5, #2
	adds	r6, #120
	adds	r5, r5, r1
	add	r6, fp
	movs	r3, #0
	str	r3, [r5, #24]
	ldr	r3, [r6, #0]
	ldrh	r0, [r3, #18]
	bl	sub_08002096
	lsls	r0, r0, #1
	negs	r0, r0
	ldr	r3, [r6, #0]
	str	r0, [r5, #12]
	ldrh	r0, [r3, #18]
	bl	sub_08002090
	lsls	r0, r0, #1
	ldr	r3, [r6, #0]
	str	r0, [r5, #16]
	ldrh	r0, [r3, #18]
	bl	sub_08002096
	ldr	r2, [r7, #0]
	ldr	r3, [sp, #124]
	adds	r2, r2, r3
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	lsls	r3, r0, #3
	subs	r3, r3, r0
	lsls	r3, r3, #2
	subs	r3, r3, r0
	asrs	r3, r3, #1
	asrs	r2, r2, #1
	subs	r2, r2, r3
	str	r2, [r5, #0]
	ldr	r3, [r6, #0]
	ldrh	r0, [r3, #18]
	bl	sub_08002090
	lsls	r3, r0, #3
	ldr	r2, [r7, #4]
	subs	r3, r3, r0
	lsls	r3, r3, #2
	ldr	r4, [pc, #372]
	subs	r3, r3, r0
	adds	r2, r2, r3
	adds	r2, r2, r4
	str	r2, [r5, #4]
	ldr	r1, [r7, #24]
.L_0817fc1e:
	adds	r3, r1, #0
	subs	r3, #50
	cmp	r3, #9
	bls.n	.L_0817fc2c
	subs	r3, #63
	cmp	r3, #86
	bhi.n	.L_0817fc70
.L_0817fc2c:
	movs	r2, #3
	ands	r2, r1
	cmp	r2, #0
	bne.n	.L_0817fc70
	movs	r6, #31
	adds	r3, r6, #0
	ands	r3, r1
	lsls	r5, r3, #3
	ldr	r0, [pc, #332]
	subs	r5, r5, r3
	lsls	r5, r5, #2
	adds	r5, r5, r0
	str	r2, [r5, #24]
	bl	sub_08014878
	ands	r0, r6
	adds	r0, #16
	lsls	r0, r0, #12
	str	r0, [r5, #12]
	bl	sub_08014878
	movs	r3, #16
	negs	r3, r3
	negs	r0, r0
	orrs	r0, r3
	movs	r3, #176
	lsls	r3, r3, #15
	str	r3, [r5, #0]
	movs	r3, #128
	lsls	r0, r0, #14
	lsls	r3, r3, #16
	str	r0, [r5, #16]
	str	r3, [r5, #4]
	ldr	r1, [r7, #24]
.L_0817fc70:
	ldr	r2, [sp, #124]
	movs	r3, #154
	str	r2, [sp, #120]
	lsls	r3, r3, #2
	add	r3, fp
	ldr	r2, [r3, #24]
	cmp	r2, #229
	ble.n	.L_0817fc8c
	ldr	r3, [r3, #0]
	subs	r2, #230
	lsls	r2, r2, #17
	ldr	r4, [sp, #124]
	adds	r3, r3, r2
	b.n	.L_0817fc94
.L_0817fc8c:
	cmp	r2, #0
	ble.n	.L_0817fcb2
	ldr	r3, [r3, #0]
	ldr	r4, [sp, #124]
.L_0817fc94:
	negs	r3, r3
	movs	r0, #152
	subs	r2, r3, r4
	lsls	r0, r0, #16
	adds	r3, r2, r0
	cmp	r3, #0
	bge.n	.L_0817fcaa
	movs	r4, #152
	lsls	r4, r4, #16
	adds	r4, #3
	adds	r3, r2, r4
.L_0817fcaa:
	ldr	r0, [sp, #124]
	asrs	r3, r3, #2
	adds	r0, r0, r3
	str	r0, [sp, #124]
.L_0817fcb2:
	cmp	r1, #1
	ble.n	.L_0817fcc8
	ldr	r3, [r7, #0]
	ldr	r2, [r7, #12]
	ldr	r1, [r7, #24]
	adds	r3, r3, r2
	str	r3, [r7, #0]
	ldr	r2, [r7, #16]
	ldr	r3, [r7, #4]
	adds	r3, r3, r2
	str	r3, [r7, #4]
.L_0817fcc8:
	cmp	r1, #7
	ble.n	.L_0817fcf6
	ldr	r2, [r7, #12]
	lsls	r3, r2, #6
	subs	r3, r3, r2
	cmp	r3, #0
	bge.n	.L_0817fcd8
	adds	r3, #63
.L_0817fcd8:
	ldr	r2, [r7, #16]
	asrs	r3, r3, #6
	str	r3, [r7, #12]
	lsls	r3, r2, #6
	subs	r3, r3, r2
	cmp	r3, #0
	bge.n	.L_0817fce8
	adds	r3, #63
.L_0817fce8:
	movs	r1, #128
	lsls	r1, r1, #4
	adds	r1, #200
	asrs	r3, r3, #6
	adds	r3, r3, r1
	str	r3, [r7, #16]
	ldr	r1, [r7, #24]
.L_0817fcf6:
	cmp	r1, #79
	bgt.n	.L_0817fd06
	ldr	r3, [r7, #16]
	movs	r2, #128
	lsls	r2, r2, #2
	adds	r3, r3, r2
	str	r3, [r7, #16]
	ldr	r1, [r7, #24]
.L_0817fd06:
	cmp	r1, #29
	ble.n	.L_0817fd14
	ldr	r3, [r7, #12]
	ldr	r4, [pc, #128]
	ldr	r1, [r7, #24]
	adds	r3, r3, r4
	str	r3, [r7, #12]
.L_0817fd14:
	adds	r3, r1, #0
	subs	r3, #40
	cmp	r3, #27
	bhi.n	.L_0817fd26
	ldr	r3, [r7, #16]
	ldr	r0, [pc, #116]
	ldr	r1, [r7, #24]
	adds	r3, r3, r0
	str	r3, [r7, #16]
.L_0817fd26:
	adds	r3, r1, #0
	subs	r3, #50
	cmp	r3, #9
	bhi.n	.L_0817fd4a
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #120
	add	r3, fp
	ldr	r2, [r3, #0]
	lsls	r3, r1, #7
	ldr	r1, [pc, #92]
	adds	r3, r3, r1
	strh	r3, [r2, #18]
	ldr	r3, [r7, #16]
	ldr	r2, [pc, #80]
	ldr	r1, [r7, #24]
	adds	r3, r3, r2
	str	r3, [r7, #16]
.L_0817fd4a:
	adds	r3, r1, #0
	subs	r3, #108
	cmp	r3, #31
	bhi.n	.L_0817fdbe
	subs	r3, #12
	cmp	r3, #2
	bhi.n	.L_0817fda0
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #120
	add	r3, fp
	ldr	r2, [r3, #0]
	ldr	r4, [pc, #56]
	ldrh	r3, [r2, #18]
	adds	r3, r3, r4
	b.n	.L_0817fdb2
	movs	r0, r0
	.4byte 0x00000148
	.4byte 0xfffc0000
	.4byte 0x02016220
	.4byte 0xffffb000
	.4byte 0x02016bd8
	.4byte 0x02016f58
	.4byte 0x02016bc0
	.4byte 0xfff40000
	.4byte 0x02016f40
	.4byte 0xffffc000
	.4byte 0xfffff000
	.4byte 0xffffa700
	.2byte 0xfe80
	.2byte 0xffff
.L_0817fda0:
	.2byte 0x23f0
	lsls	r3, r3, #7
	adds	r3, #120
	add	r3, fp
	ldr	r2, [r3, #0]
	movs	r0, #128
	ldrh	r3, [r2, #18]
	lsls	r0, r0, #1
	adds	r3, r3, r0
.L_0817fdb2:
	strh	r3, [r2, #18]
	ldr	r3, [r7, #16]
	ldr	r1, [pc, #288]
	adds	r3, r3, r1
	str	r3, [r7, #16]
	ldr	r1, [r7, #24]
.L_0817fdbe:
	adds	r3, r1, #0
	subs	r3, #140
	cmp	r3, #19
	bhi.n	.L_0817fdd0
	ldr	r3, [r7, #16]
	ldr	r2, [pc, #272]
	ldr	r1, [r7, #24]
	adds	r3, r3, r2
	str	r3, [r7, #16]
.L_0817fdd0:
	cmp	r1, #159
	ble.n	.L_0817fddc
	ldr	r3, [r7, #16]
	ldr	r4, [pc, #264]
	adds	r3, r3, r4
	str	r3, [r7, #16]
.L_0817fddc:
	ldr	r5, [pc, #260]
	movs	r0, #0
	mov	r8, r0
.L_0817fde2:
	ldr	r3, [r5, #24]
	cmp	r3, #5
	bhi.n	.L_0817fe24
	lsls	r1, r3, #3
	adds	r1, r1, r3
	lsls	r1, r1, #5
	movs	r2, #224
	lsls	r2, r2, #3
	movs	r0, #12
	add	r1, fp
	adds	r1, r1, r2
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	movs	r4, #6
	ldrsh	r3, [r5, r4]
	str	r0, [sp, #0]
	movs	r0, #24
	str	r0, [sp, #4]
	ldr	r0, [sp, #84]
	subs	r3, #12
	ldr	r4, [r0, #4]
	subs	r2, #2
	ldr	r0, [sp, #144]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c28
	movs	r1, #64
	movs	r2, #0
	bl	sub_08138086
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
.L_0817fe24:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	adds	r5, #28
	cmp	r2, #32
	bne.n	.L_0817fde2
	ldr	r5, [pc, #180]
	movs	r3, #0
	mov	r8, r3
.L_0817fe36:
	ldr	r1, [r5, #24]
	cmp	r1, #23
	bhi.n	.L_0817fe86
	cmp	r1, #0
	bge.n	.L_0817fe42
	adds	r1, #3
.L_0817fe42:
	asrs	r1, r1, #2
	lsls	r1, r1, #11
	movs	r4, #220
	movs	r0, #2
	ldrsh	r2, [r5, r0]
	lsls	r4, r4, #4
	movs	r0, #32
	add	r1, fp
	adds	r1, r1, r4
	movs	r4, #6
	ldrsh	r3, [r5, r4]
	str	r0, [sp, #0]
	movs	r0, #64
	str	r0, [sp, #4]
	ldr	r0, [sp, #84]
	subs	r2, #16
	ldr	r4, [r0, #4]
	subs	r3, #32
	ldr	r0, [sp, #144]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x68eb
	movs	r1, #128
	lsls	r1, r1, #5
	adds	r3, r3, r1
	str	r3, [r5, #12]
	adds	r0, r5, #0
	movs	r1, #64
	movs	r2, #0
	bl	sub_08138086
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
.L_0817fe86:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	adds	r5, #28
	cmp	r3, #32
	bne.n	.L_0817fe36
	ldr	r3, [r7, #24]
	adds	r3, #1
	str	r3, [r7, #24]
.L_0817fe98:
	ldr	r3, [pc, #80]
	ldr	r4, [sp, #40]
	str	r3, [sp, #168]
	str	r3, [r4, #4]
	ldr	r3, [r7, #24]
	cmp	r3, #0
	bne.n	.L_0817fef4
	ldr	r7, [sp, #124]
	ldr	r0, [sp, #68]
	ldr	r2, [sp, #28]
	movs	r1, #152
	adds	r3, r7, r0
	lsls	r1, r1, #16
	adds	r3, r3, r1
	str	r3, [r2, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #168
	add	r3, fp
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #44]
	ldr	r4, [sp, #72]
	ldrsb	r2, [r2, r3]
	movs	r3, #92
	ldr	r7, [sp, #64]
	subs	r3, r3, r2
	lsls	r3, r3, #16
	ldr	r0, [sp, #28]
	adds	r3, r3, r4
	adds	r3, r3, r7
	str	r3, [r0, #8]
	b.n	.L_0817ff02
	.4byte 0xfffff000
	.4byte 0xfffff800
	.4byte 0xffffe000
	.4byte 0x02016bc0
	.4byte 0x02016f40
	.4byte 0x000103ff
	.2byte 0x94f0
	.2byte 0x0819
.L_0817fef4:
	ldr	r3, [r7, #0]
	ldr	r1, [sp, #124]
	ldr	r2, [sp, #28]
	adds	r3, r3, r1
	str	r3, [r2, #0]
	ldr	r3, [r7, #4]
	str	r3, [r2, #8]
.L_0817ff02:
	movs	r5, #240
	lsls	r5, r5, #7
	adds	r5, #120
	add	r5, fp
	ldr	r2, [sp, #40]
	movs	r3, #0
	ldr	r0, [r5, #0]
	ldr	r1, [sp, #28]
	bl	sub_08020010
	ldr	r2, [r5, #0]
	movs	r3, #32
	movs	r6, #0
	strb	r6, [r2, #22]
	strb	r3, [r2, #23]
	ldr	r3, [sp, #140]
	movs	r4, #142
	lsls	r4, r4, #2
	cmp	r3, r4
	blt.n	.L_0817ffcc
	movs	r0, #128
	lsls	r0, r0, #2
	bl	sub_08014dac
	adds	r7, r0, #0
	movs	r0, #1
	bl	sub_081969f8
	ldr	r2, [pc, #936]
	ldr	r3, [sp, #160]
	movs	r1, #7
	ands	r3, r2
	ldr	r2, [pc, #932]
	orrs	r3, r1
	ands	r3, r2
	movs	r2, #224
	adds	r5, r0, #0
	lsls	r2, r2, #3
	ldr	r0, [pc, #924]
	orrs	r3, r2
	str	r3, [sp, #160]
	add	r3, sp, #160
	str	r0, [r3, #4]
	str	r3, [r5, #16]
	ldr	r3, [pc, #916]
	str	r6, [r5, #4]
	str	r3, [r5, #8]
	str	r1, [r5, #0]
	str	r7, [r5, #12]
	ldr	r2, [pc, #908]
	ldr	r1, [sp, #140]
	movs	r3, #96
	adds	r6, r1, r2
	lsls	r2, r6, #4
	subs	r3, r3, r2
	cmp	r3, #0
	ble.n	.L_0817ff76
	movs	r3, #0
.L_0817ff76:
	movs	r4, #63
	negs	r4, r4
	cmp	r3, r4
	blt.n	.L_0817ffc0
	str	r3, [r5, #20]
	bl	sub_08014de4
	ldr	r1, [sp, #124]
	movs	r2, #128
	lsls	r2, r2, #10
	adds	r0, r1, r2
	movs	r1, #128
	lsls	r1, r1, #12
	movs	r2, #0
	bl	sub_08015160
	movs	r0, #134
	lsls	r0, r0, #7
	bl	sub_080150e4
	ldr	r0, [pc, #856]
	bl	sub_08015024
	movs	r3, #131
	lsls	r3, r3, #7
	lsls	r0, r6, #14
	adds	r0, r0, r3
	bl	sub_0801521c
	ldr	r0, [pc, #840]
	adds	r1, r7, #0
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r5, #0
	bl	sub_08196a7c
.L_0817ffc0:
	adds	r0, r5, #0
	bl	sub_08013164
	adds	r0, r7, #0
	bl	sub_08013164
.L_0817ffcc:
	ldr	r4, [sp, #140]
	movs	r7, #142
	lsls	r7, r7, #2
	cmp	r4, r7
	bne.n	.L_0817ffec
	movs	r2, #210
	lsls	r2, r2, #2
	add	r2, fp
	movs	r3, #0
	str	r3, [r2, #0]
	movs	r2, #213
	lsls	r2, r2, #2
	movs	r3, #128
	add	r2, fp
	lsls	r3, r3, #12
	str	r3, [r2, #0]
.L_0817ffec:
	movs	r1, #210
	lsls	r1, r1, #2
	add	r1, fp
	str	r1, [sp, #60]
	movs	r0, #0
	mov	sl, r0
.L_0817fff8:
	ldr	r2, [pc, #772]
	mov	r4, sl
	lsls	r3, r4, #1
	ldrh	r2, [r2, r3]
	ldr	r7, [sp, #140]
	cmp	r7, r2
	blt.n	.L_081800a0
	adds	r3, r2, #0
	adds	r3, #26
	cmp	r7, r3
	bge.n	.L_081800a0
	ldr	r0, [sp, #60]
	ldr	r4, [sp, #140]
	ldr	r1, [r0, #0]
	subs	r3, r4, r2
	movs	r0, #213
	lsrs	r2, r3, #31
	lsls	r0, r0, #2
	adds	r3, r3, r2
	add	r0, fp
	asrs	r4, r3, #1
	ldr	r3, [r0, #0]
	ldr	r2, [sp, #60]
	asrs	r7, r1, #16
	adds	r1, r1, r3
	str	r1, [r2, #0]
	ldr	r2, [r0, #0]
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #4
	cmp	r3, #0
	bge.n	.L_0818003a
	adds	r3, #63
.L_0818003a:
	asrs	r3, r3, #6
	str	r3, [r0, #0]
	cmp	r4, #5
	bgt.n	.L_081800a0
	movs	r3, #0
	mov	r8, r3
	lsls	r3, r4, #3
	adds	r3, r3, r4
	lsls	r3, r3, #5
	add	r3, fp
	mov	r9, r3
.L_08180050:
	mov	r4, r8
	lsls	r6, r4, #11
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r5, r7, #0
	muls	r5, r0
	lsrs	r3, r5, #31
	adds	r5, r5, r3
	ldr	r3, [pc, #672]
	mov	r0, sl
	ldrsb	r3, [r3, r0]
	asrs	r5, r5, #17
	adds	r0, r6, #0
	adds	r5, r5, r3
	bl	sub_08002090
	adds	r3, r7, #0
	muls	r3, r0
	movs	r2, #12
	lsls	r3, r3, #1
	movs	r1, #224
	str	r2, [sp, #0]
	asrs	r3, r3, #16
	movs	r2, #24
	subs	r5, #6
	lsls	r1, r1, #3
	str	r2, [sp, #4]
	add	r1, r9
	adds	r2, r5, #0
	adds	r3, #60
	ldr	r4, [sp, #176]
	ldr	r0, [sp, #144]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2101
	add	r8, r1
	mov	r2, r8
	cmp	r2, #32
	bne.n	.L_08180050
.L_081800a0:
	movs	r3, #1
	add	sl, r3
	mov	r4, sl
	cmp	r4, #1
	bne.n	.L_0817fff8
	ldr	r7, [sp, #40]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [sp, #168]
	str	r3, [r7, #4]
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #48
	add	r3, fp
	ldr	r0, [r3, #0]
	movs	r4, #13
	ldrb	r2, [r0, #9]
	negs	r4, r4
	adds	r3, r4, #0
	ands	r3, r2
	movs	r2, #240
	lsls	r2, r2, #7
	movs	r1, #8
	adds	r2, #52
	orrs	r3, r1
	add	r2, fp
	strb	r3, [r0, #9]
	str	r2, [sp, #56]
	adds	r3, r4, #0
	ldr	r1, [r2, #0]
	movs	r7, #8
	ldrb	r2, [r1, #9]
	movs	r6, #104
	ands	r3, r2
	orrs	r3, r7
	strb	r3, [r1, #9]
	movs	r1, #240
	lsls	r1, r1, #7
	adds	r1, #56
	add	r1, fp
	str	r1, [sp, #52]
	adds	r3, r4, #0
	ldr	r1, [r1, #0]
	str	r4, [sp, #8]
	ldrb	r2, [r1, #9]
	movs	r5, #136
	ands	r3, r2
	movs	r2, #8
	orrs	r3, r2
	strb	r3, [r1, #9]
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #60
	add	r3, fp
	str	r3, [sp, #48]
	ldr	r1, [r3, #0]
	adds	r3, r4, #0
	ldrb	r2, [r1, #9]
	ands	r3, r2
	orrs	r3, r7
	strb	r3, [r1, #9]
	ldr	r2, [sp, #68]
	ldr	r1, [sp, #120]
	ldr	r3, [sp, #28]
	adds	r1, r1, r2
	movs	r7, #160
	mov	r8, r1
	lsls	r7, r7, #16
	add	r7, r8
	movs	r2, #238
	str	r7, [r3, #0]
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, fp
	ldr	r1, [pc, #464]
	ldr	r3, [r2, #0]
	mov	r9, r1
	ldrsb	r3, [r1, r3]
	ldr	r1, [sp, #72]
	subs	r3, r6, r3
	mov	sl, r2
	lsls	r3, r3, #16
	ldr	r2, [sp, #64]
	adds	r3, r3, r1
	ldr	r1, [sp, #28]
	adds	r3, r3, r2
	str	r3, [r1, #8]
	ldr	r1, [sp, #28]
	ldr	r2, [sp, #40]
	movs	r3, #0
	bl	sub_08020010
	movs	r2, #128
	ldr	r3, [sp, #28]
	lsls	r2, r2, #16
	add	r2, r8
	str	r2, [sp, #44]
	str	r2, [r3, #0]
	mov	r0, sl
	ldr	r3, [r0, #0]
	mov	r1, r9
	ldrsb	r3, [r1, r3]
	ldr	r2, [sp, #72]
	ldr	r0, [sp, #64]
	subs	r3, r5, r3
	ldr	r1, [sp, #28]
	lsls	r3, r3, #16
	adds	r3, r3, r2
	adds	r3, r3, r0
	str	r3, [r1, #8]
	ldr	r2, [sp, #56]
	ldr	r1, [sp, #28]
	ldr	r0, [r2, #0]
	movs	r3, #0
	ldr	r2, [sp, #40]
	bl	sub_08020010
	ldr	r3, [sp, #28]
	mov	r0, r9
	str	r7, [r3, #0]
	mov	r7, sl
	ldr	r3, [r7, #0]
	ldr	r1, [sp, #72]
	ldrsb	r3, [r0, r3]
	ldr	r2, [sp, #64]
	subs	r3, r5, r3
	ldr	r7, [sp, #28]
	lsls	r3, r3, #16
	adds	r3, r3, r1
	adds	r3, r3, r2
	str	r3, [r7, #8]
	ldr	r1, [sp, #52]
	ldr	r2, [sp, #40]
	ldr	r0, [r1, #0]
	movs	r3, #0
	ldr	r1, [sp, #28]
	bl	sub_08020010
	movs	r2, #192
	lsls	r2, r2, #16
	add	r8, r2
	mov	r3, r8
	str	r3, [r7, #0]
	mov	r7, sl
	ldr	r3, [r7, #0]
	mov	r0, r9
	ldrsb	r3, [r0, r3]
	ldr	r1, [sp, #72]
	ldr	r2, [sp, #64]
	subs	r5, r5, r3
	lsls	r5, r5, #16
	ldr	r3, [sp, #28]
	adds	r5, r5, r1
	adds	r5, r5, r2
	str	r5, [r3, #8]
	ldr	r7, [sp, #48]
	ldr	r1, [sp, #28]
	ldr	r0, [r7, #0]
	ldr	r2, [sp, #40]
	movs	r3, #0
	bl	sub_08020010
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #64
	add	r3, fp
	ldr	r0, [r3, #0]
	ldr	r4, [sp, #8]
	ldrb	r3, [r0, #9]
	movs	r1, #8
	ands	r4, r3
	orrs	r4, r1
	strb	r4, [r0, #9]
	ldr	r2, [sp, #44]
	ldr	r3, [sp, #28]
	mov	r4, sl
	str	r2, [r3, #0]
	mov	r7, r9
	ldr	r3, [r4, #0]
	ldr	r1, [sp, #72]
	ldrsb	r3, [r7, r3]
	ldr	r2, [sp, #64]
	subs	r6, r6, r3
	lsls	r6, r6, #16
	ldr	r3, [sp, #28]
	adds	r6, r6, r1
	adds	r6, r6, r2
	str	r6, [r3, #8]
	ldr	r1, [sp, #28]
	ldr	r2, [sp, #40]
	movs	r3, #0
	bl	sub_08020010
	movs	r6, #240
	lsls	r6, r6, #7
	ldr	r5, [sp, #28]
	ldr	r7, [pc, #224]
	movs	r4, #0
	adds	r6, #68
	mov	r8, r4
	add	r6, fp
	mov	r4, r9
.L_08180234:
	ldrb	r3, [r7, #0]
	ldr	r0, [sp, #76]
	ldr	r1, [sp, #120]
	ldr	r2, [sp, #68]
	lsls	r3, r3, #16
	adds	r3, r3, r0
	adds	r3, r3, r1
	movs	r0, #176
	adds	r3, r3, r2
	lsls	r0, r0, #16
	adds	r3, r3, r0
	movs	r2, #238
	str	r3, [r5, #0]
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, fp
	ldr	r2, [r2, #0]
	ldrb	r3, [r7, #1]
	ldrsb	r2, [r4, r2]
	ldr	r1, [sp, #72]
	subs	r3, r3, r2
	ldr	r2, [sp, #64]
	lsls	r3, r3, #16
	adds	r3, r3, r1
	movs	r0, #168
	adds	r3, r3, r2
	lsls	r0, r0, #15
	adds	r3, r3, r0
	str	r3, [r5, #8]
	ldmia	r6!, {r0}
	movs	r1, #13
	ldrb	r3, [r0, #9]
	negs	r1, r1
	adds	r2, r1, #0
	ands	r3, r2
	movs	r2, #8
	orrs	r3, r2
	strb	r3, [r0, #9]
	ldr	r2, [sp, #40]
	movs	r3, #0
	adds	r1, r5, #0
	str	r4, [sp, #8]
	bl	sub_08020010
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	adds	r7, #2
	ldr	r4, [sp, #8]
	cmp	r3, #5
	bne.n	.L_08180234
	ldr	r4, [sp, #140]
	movs	r7, #234
	adds	r7, #255
	cmp	r4, r7
	bgt.n	.L_08180310
	movs	r6, #241
	movs	r5, #224
	ldr	r7, [sp, #28]
	movs	r0, #0
	lsls	r6, r6, #7
	lsls	r5, r5, #2
	mov	r8, r0
	add	r6, fp
	add	r5, fp
.L_081802b6:
	ldr	r3, [r5, #0]
	ldr	r1, [sp, #124]
	adds	r3, r3, r1
	str	r3, [r7, #0]
	ldr	r3, [r5, #4]
	str	r3, [r7, #8]
	ldr	r3, [r5, #24]
	cmp	r3, #0
	beq.n	.L_081802d4
	ldr	r0, [r6, #0]
	ldr	r1, [sp, #28]
	ldr	r2, [sp, #40]
	movs	r3, #0
	bl	sub_08020010
.L_081802d4:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	adds	r6, #4
	adds	r5, #28
	cmp	r3, #8
	bne.n	.L_081802b6
	b.n	.L_08180310
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x02010000
	.4byte 0x08199364
	.4byte 0xfffffdc8
	.4byte 0xfffff800
	.4byte 0x08199210
	.4byte 0x0819958c
	.4byte 0x08199590
	.4byte 0x081994f0
	.2byte 0x9592
	.2byte 0x0819
.L_08180310:
	ldr	r4, [sp, #140]
	movs	r7, #135
	lsls	r7, r7, #2
	cmp	r4, r7
	bge.n	.L_081803d8
	movs	r5, #147
	lsls	r5, r5, #2
	add	r5, fp
	ldr	r3, [r5, #24]
	cmp	r3, #1
	bne.n	.L_0818035c
	ldr	r1, [r5, #12]
	ldr	r3, [r5, #0]
	ldr	r2, [r5, #16]
	adds	r3, r3, r1
	str	r3, [r5, #0]
	ldr	r3, [r5, #4]
	movs	r0, #128
	adds	r3, r3, r2
	str	r3, [r5, #4]
	lsls	r3, r1, #4
	lsls	r0, r0, #6
	subs	r3, r3, r1
	adds	r2, r2, r0
	lsls	r3, r3, #2
	str	r2, [r5, #16]
	cmp	r3, #0
	bge.n	.L_0818034a
	adds	r3, #63
.L_0818034a:
	asrs	r3, r3, #6
	str	r3, [r5, #12]
	lsls	r3, r2, #6
	subs	r2, r3, r2
	cmp	r2, #0
	bge.n	.L_08180358
	adds	r2, #63
.L_08180358:
	asrs	r3, r2, #6
	str	r3, [r5, #16]
.L_0818035c:
	movs	r7, #240
	lsls	r7, r7, #7
	ldr	r6, [sp, #28]
	ldr	r4, [pc, #420]
	movs	r1, #0
	adds	r7, #100
	mov	r8, r1
	add	r7, fp
.L_0818036c:
	mov	r2, r8
	movs	r3, #1
	ands	r3, r2
	ldr	r2, [r5, #0]
	ldr	r0, [sp, #124]
	lsls	r3, r3, #21
	adds	r3, r3, r2
	movs	r1, #128
	adds	r3, r3, r0
	lsls	r1, r1, #16
	adds	r3, r3, r1
	mov	r2, r8
	str	r3, [r6, #0]
	lsrs	r3, r2, #31
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, fp
	ldr	r2, [r2, #0]
	add	r3, r8
	ldrsb	r2, [r4, r2]
	asrs	r3, r3, #1
	ldr	r0, [sp, #72]
	lsls	r3, r3, #5
	subs	r3, r3, r2
	ldr	r2, [r5, #4]
	lsls	r3, r3, #16
	adds	r3, r3, r0
	movs	r1, #208
	adds	r3, r3, r2
	lsls	r1, r1, #15
	adds	r3, r3, r1
	str	r3, [r6, #8]
	ldmia	r7!, {r0}
	movs	r1, #13
	ldrb	r3, [r0, #9]
	negs	r1, r1
	adds	r2, r1, #0
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r0, #9]
	ldr	r2, [sp, #40]
	movs	r3, #0
	adds	r1, r6, #0
	str	r4, [sp, #8]
	bl	sub_08020010
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	ldr	r4, [sp, #8]
	cmp	r3, #4
	bne.n	.L_0818036c
.L_081803d8:
	ldr	r4, [sp, #140]
	movs	r7, #158
	lsls	r7, r7, #1
	cmp	r4, r7
	bgt.n	.L_08180490
	movs	r4, #140
	lsls	r4, r4, #2
	add	r4, fp
	ldr	r3, [r4, #24]
	cmp	r3, #0
	beq.n	.L_0818043c
	adds	r3, #1
	ldr	r1, [r4, #12]
	str	r3, [r4, #24]
	ldr	r3, [r4, #0]
	ldr	r2, [r4, #16]
	adds	r3, r3, r1
	str	r3, [r4, #0]
	ldr	r3, [r4, #4]
	movs	r0, #128
	adds	r3, r3, r2
	str	r3, [r4, #4]
	lsls	r3, r1, #1
	lsls	r0, r0, #6
	adds	r3, r3, r1
	adds	r2, r2, r0
	lsls	r3, r3, #4
	str	r2, [r4, #16]
	cmp	r3, #0
	bge.n	.L_08180416
	adds	r3, #63
.L_08180416:
	asrs	r3, r3, #6
	str	r3, [r4, #12]
	lsls	r3, r2, #6
	subs	r2, r3, r2
	cmp	r2, #0
	bge.n	.L_08180424
	adds	r2, #63
.L_08180424:
	asrs	r3, r2, #6
	str	r3, [r4, #16]
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #88
	add	r3, fp
	ldr	r2, [r3, #0]
	movs	r1, #128
	ldrh	r3, [r2, #18]
	lsls	r1, r1, #1
	adds	r3, r3, r1
	strh	r3, [r2, #18]
.L_0818043c:
	ldr	r2, [sp, #76]
	ldr	r3, [r4, #0]
	ldr	r7, [sp, #124]
	ldr	r1, [sp, #28]
	adds	r3, r2, r3
	movs	r0, #184
	lsls	r0, r0, #16
	adds	r3, r3, r7
	adds	r3, r3, r0
	str	r3, [r1, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #168
	add	r3, fp
	ldr	r3, [r3, #0]
	ldr	r2, [pc, #172]
	ldrsb	r2, [r2, r3]
	movs	r3, #92
	subs	r3, r3, r2
	ldr	r2, [sp, #72]
	lsls	r3, r3, #16
	adds	r3, r3, r2
	ldr	r2, [r4, #4]
	adds	r3, r3, r2
	str	r3, [r1, #8]
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #88
	add	r3, fp
	ldr	r0, [r3, #0]
	movs	r3, #13
	ldrb	r2, [r0, #9]
	negs	r3, r3
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r0, #9]
	ldr	r1, [sp, #28]
	ldr	r2, [sp, #40]
	movs	r3, #0
	bl	sub_08020010
.L_08180490:
	ldr	r4, [sp, #28]
	movs	r3, #0
	mov	r9, r4
	movs	r4, #238
	lsls	r4, r4, #7
	adds	r4, #252
	mov	sl, r3
	movs	r6, #0
	add	r4, fp
.L_081804a2:
	mov	r0, sl
	lsls	r0, r0, #5
	ldr	r5, [sp, #104]
	movs	r1, #238
	lsls	r3, r6, #2
	lsls	r1, r1, #7
	str	r0, [sp, #24]
	movs	r7, #0
	add	r3, fp
	adds	r1, #220
	mov	r8, r7
	adds	r5, #64
	adds	r7, r3, r1
.L_081804bc:
	ldr	r0, [sp, #116]
	mov	r2, r8
	lsls	r3, r2, #21
	adds	r3, r3, r0
	mov	r1, r9
	str	r3, [r1, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #168
	add	r3, fp
	ldr	r2, [pc, #52]
	ldr	r3, [r3, #0]
	ldr	r0, [sp, #108]
	ldrsb	r3, [r2, r3]
	ldr	r2, [sp, #24]
	movs	r1, #132
	subs	r3, r2, r3
	lsls	r3, r3, #16
	adds	r3, r3, r0
	lsls	r1, r1, #16
	adds	r3, r3, r1
	mov	r2, r9
	str	r3, [r2, #8]
	mov	r3, r8
	cmp	r3, #8
	bne.n	.L_0818050c
	ldr	r3, [sp, #104]
	ldr	r1, [r4, #0]
	adds	r3, #71
	adds	r2, r3, #0
	cmp	r3, #0
	bge.n	.L_08180500
	ldr	r2, [sp, #104]
	adds	r2, #78
.L_08180500:
	asrs	r2, r2, #3
	lsls	r2, r2, #3
	subs	r2, r3, r2
	b.n	.L_08180532
	.2byte 0x94f0
	.2byte 0x0819
.L_0818050c:
	mov	r1, r8
	movs	r2, #238
	adds	r3, r6, r1
	lsls	r2, r2, #7
	adds	r2, #220
	lsls	r3, r3, #2
	adds	r3, r3, r2
	mov	r0, fp
	ldr	r1, [r0, r3]
	ldr	r3, [sp, #104]
	adds	r2, r5, #0
	add	r3, r8
	cmp	r5, #0
	bge.n	.L_0818052c
	adds	r2, r3, #0
	adds	r2, #71
.L_0818052c:
	asrs	r2, r2, #3
	lsls	r2, r2, #3
	subs	r2, r5, r2
.L_08180532:
	adds	r2, r6, r2
	lsls	r3, r2, #3
	subs	r3, r3, r2
	lsls	r3, r3, #2
	adds	r3, #24
	mov	r0, fp
	ldr	r2, [r0, r3]
	movs	r3, #192
	lsls	r3, r3, #2
	adds	r3, #255
	ands	r2, r3
	ldr	r0, [pc, #628]
	ldrh	r3, [r1, #8]
	ands	r3, r0
	orrs	r3, r2
	strh	r3, [r1, #8]
	ldr	r1, [sp, #28]
	ldr	r2, [pc, #620]
	ldr	r3, [r1, #0]
	cmp	r3, r2
	ble.n	.L_08180562
	ldr	r0, [pc, #616]
	adds	r3, r3, r0
	str	r3, [r1, #0]
.L_08180562:
	ldr	r1, [sp, #28]
	ldr	r2, [sp, #40]
	ldmia	r7!, {r0}
	movs	r3, #0
	str	r4, [sp, #8]
	bl	sub_08020010
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	adds	r5, #1
	ldr	r4, [sp, #8]
	cmp	r2, #9
	bne.n	.L_081804bc
	add	sl, r1
	mov	r3, sl
	adds	r6, #9
	adds	r4, #36
	cmp	r3, #2
	bne.n	.L_081804a2
	ldr	r0, [pc, #576]
	movs	r4, #0
	mov	sl, r4
	movs	r7, #0
	mov	r9, r0
.L_08180594:
	ldr	r3, [pc, #568]
	ldr	r2, [sp, #140]
	ldr	r4, [pc, #568]
	movs	r1, #0
	adds	r6, r2, r3
	lsls	r3, r7, #1
	mov	r8, r1
	adds	r5, r3, r4
.L_081805a4:
	mov	r0, r8
	movs	r1, #245
	ldr	r4, [sp, #140]
	lsls	r2, r0, #1
	lsls	r1, r1, #1
	adds	r3, r2, r1
	cmp	r4, r3
	blt.n	sub_08180600
	movs	r0, #251
	lsls	r0, r0, #1
	adds	r3, r2, r0
	cmp	r4, r3
	bge.n	sub_08180600
	ldr	r4, [sp, #72]
	ldrb	r3, [r5, #1]
	asrs	r1, r4, #16
	adds	r3, r3, r1
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #168
	add	r1, fp
	ldr	r1, [r1, #0]
	mov	r4, r9
	ldrsb	r1, [r4, r1]
	lsrs	r0, r6, #31
	adds	r0, r6, r0
	asrs	r0, r0, #1
	subs	r3, r3, r1
	lsls	r1, r0, #3
	adds	r1, r1, r0
	lsls	r1, r1, #5
	movs	r0, #224
	ldrb	r2, [r5, #0]
	lsls	r0, r0, #3
	add	r1, fp
	adds	r1, r1, r0
	movs	r0, #12
	str	r0, [sp, #0]
	movs	r0, #24
	str	r0, [sp, #4]
	subs	r2, #6
	subs	r3, #12
	ldr	r4, [sp, #176]
	ldr	r0, [sp, #144]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2101
	add	r8, r1
	mov	r2, r8
	subs	r6, #2
	adds	r5, #2
	cmp	r2, #6
	bne.n	.L_081805a4
	add	sl, r1
	mov	r3, sl
	adds	r7, #6
	cmp	r3, #4
	bne.n	.L_08180594
	movs	r4, #0
	mov	r8, r4
	movs	r6, #0
.L_0818061e:
	ldr	r0, [pc, #440]
	movs	r7, #0
	mov	sl, r7
	adds	r5, r6, r0
.L_08180626:
	ldr	r3, [r5, #24]
	cmp	r3, #5
	bhi.n	.L_08180672
	lsls	r1, r3, #3
	adds	r1, r1, r3
	lsls	r1, r1, #5
	movs	r2, #224
	lsls	r2, r2, #3
	add	r1, fp
	adds	r1, r1, r2
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	movs	r4, #6
	ldrsh	r3, [r5, r4]
	movs	r0, #12
	str	r0, [sp, #0]
	movs	r0, #24
	subs	r2, #2
	subs	r3, #12
	str	r0, [sp, #4]
	ldr	r4, [sp, #176]
	ldr	r0, [sp, #144]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x682a
	ldr	r3, [r5, #12]
	ldr	r7, [pc, #384]
	adds	r2, r2, r3
	str	r2, [r5, #0]
	ldr	r3, [r5, #4]
	ldr	r2, [r5, #16]
	adds	r3, r3, r2
	str	r3, [r5, #4]
	ldr	r3, [r5, #24]
	adds	r2, r2, r7
	adds	r3, #1
	str	r2, [r5, #16]
	str	r3, [r5, #24]
.L_08180672:
	movs	r0, #1
	add	sl, r0
	mov	r1, sl
	adds	r5, #28
	cmp	r1, #32
	bne.n	.L_08180626
	movs	r2, #224
	add	r8, r0
	lsls	r2, r2, #2
	mov	r3, r8
	adds	r6, r6, r2
	cmp	r3, #16
	bne.n	.L_0818061e
	movs	r4, #238
	lsls	r4, r4, #7
	adds	r4, #168
	add	r4, fp
	ldr	r3, [r4, #0]
	ldr	r2, [pc, #308]
	ldr	r0, [pc, #324]
	ldrsb	r1, [r2, r3]
	ldr	r2, [pc, #324]
	adds	r3, r1, #0
	adds	r3, #32
	strh	r3, [r0, #6]
	movs	r3, #120
	subs	r3, r3, r1
	str	r3, [r2, #16]
	ldr	r3, [r4, #0]
	cmp	r3, #0
	ble.n	.L_081806b4
	subs	r3, #1
	str	r3, [r4, #0]
.L_081806b4:
	bl	sub_081434f8
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #232
	add	r3, fp
	movs	r5, #1
	str	r5, [r3, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r4, [sp, #140]
	movs	r7, #200
	adds	r4, #1
	lsls	r7, r7, #2
	str	r4, [sp, #140]
	cmp	r4, r7
	beq.n	.L_081806f0
	ldr	r1, [pc, #268]
	movs	r2, #2
	ldr	r3, [r1, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_081806f0
	ldr	r3, [r1, #12]
	ands	r3, r5
	cmp	r3, #0
	bne.n	.L_081806f0
	bl	.L_0817f1f4
.L_081806f0:
	ldr	r0, [pc, #248]
	bl	sub_08014644
	add	r0, sp, #100
	ldr	r3, [pc, #228]
	ldrh	r0, [r0, #0]
	movs	r2, #0
	strh	r0, [r3, #4]
	ldr	r1, [sp, #132]
	movs	r5, #238
	str	r2, [r1, #16]
	lsls	r5, r5, #7
	bl	sub_0814cca8
	adds	r5, #220
	movs	r2, #0
	mov	r8, r2
	add	r5, fp
.L_08180714:
	ldmia	r5!, {r0}
	bl	sub_08020048
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	cmp	r4, #57
	bne.n	.L_08180714
	movs	r0, #128
	bl	sub_080143e0
	ldr	r7, [sp, #128]
	str	r0, [r7, #84]
	bl	sub_08014c4c
	movs	r3, #128
	movs	r1, #160
	movs	r2, #128
	lsls	r3, r3, #19
	lsls	r1, r1, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #172]
	adds	r1, #160
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #168]
	movs	r2, #160
	ldrh	r3, [r3, #0]
	lsls	r2, r2, #19
	adds	r2, #188
	strh	r3, [r2, #0]
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
	str	r3, [r2, #0]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #40
	movs	r3, #0
	str	r3, [r2, #0]
	ldr	r3, [pc, #52]
	adds	r2, #40
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	subs	r2, #48
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	subs	r2, #20
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	adds	r2, #70
	strh	r3, [r2, #0]
	ldr	r0, [sp, #148]
	ldr	r2, [pc, #40]
	ldr	r3, [r0, #20]
	adds	r1, r0, #0
	lsls	r3, r3, #1
	adds	r3, #36
	strh	r2, [r1, r3]
	adds	r0, #36
	movs	r1, #0
	bl	sub_08118010
	movs	r1, #204
	lsls	r1, r1, #1
	movs	r2, #1
	b.n	.L_081807f8
	.4byte 0x00003f44
	.4byte 0x00000080
	.4byte 0x00000784
	.4byte 0x00001010
	.4byte 0x000000ff
	.4byte 0xfffffc00
	.4byte 0x00ffffff
	.4byte 0xfee00000
	.4byte 0x081994f0
	.4byte 0xfffffe16
	.4byte 0x0819959c
	.4byte 0x02014000
	.4byte 0xffffc000
	.4byte 0x03001120
	.4byte 0x030011e0
	.4byte 0x03001150
	.4byte 0x0813baed
	.4byte 0x05000200
	.2byte 0x01e8
	.2byte 0x0500
.L_081807f8:
	adds	r1, #255
	movs	r0, #1
	bl	sub_08152404
	movs	r5, #238
	ldr	r6, [pc, #616]
	movs	r2, #0
	lsls	r5, r5, #7
	mov	r8, r2
	adds	r5, #224
.L_0818080c:
	movs	r1, #16
	ldr	r2, [pc, #608]
	movs	r3, #0
	movs	r0, #32
	bl	sub_0815b3b0
	mov	r3, fp
	str	r0, [r5, r3]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #220
	add	r3, fp
	ldr	r1, [r3, #0]
	movs	r2, #24
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x465c
	ldr	r2, [r5, r4]
	movs	r7, #1
	movs	r3, #32
	add	r8, r7
	strb	r3, [r2, #22]
	mov	r0, r8
	movs	r3, #8
	strb	r3, [r2, #23]
	adds	r5, #4
	cmp	r0, #15
	bne.n	.L_0818080c
	movs	r1, #224
	lsls	r1, r1, #3
	ldr	r0, [pc, #552]
	add	r1, fp
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	movs	r1, #220
	lsls	r1, r1, #6
	add	r1, fp
	movs	r2, #0
	movs	r3, #0
	ldr	r0, [pc, #536]
	bl	sub_08157cf4
	ldr	r0, [pc, #532]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	movs	r2, #128
	ldr	r3, [pc, #504]
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x466a
	movs	r1, #0
	adds	r2, #192
	str	r1, [sp, #140]
	str	r2, [sp, #32]
.L_08180882:
	ldr	r3, [sp, #140]
	cmp	r3, #0
	bne.n	.L_08180934
	movs	r6, #238
	lsls	r6, r6, #7
	movs	r4, #0
	adds	r6, #220
	movs	r0, #0
	mov	r8, r4
	movs	r7, #0
	add	r6, fp
	mov	sl, r0
	mov	r5, fp
.L_0818089c:
	str	r7, [r5, #24]
	bl	sub_08014878
	str	r7, [r5, #20]
	mov	r2, sl
	ldr	r3, [r5, #20]
	lsls	r0, r3, #2
	adds	r0, r0, r3
	ldr	r3, [pc, #464]
	lsls	r0, r0, #3
	subs	r0, r2, r0
	adds	r0, r0, r3
	str	r0, [r5, #8]
	bl	sub_08002090
	negs	r0, r0
	lsls	r0, r0, #3
	str	r0, [r5, #12]
	ldr	r0, [r5, #8]
	bl	sub_08002096
	negs	r0, r0
	lsls	r0, r0, #3
	str	r0, [r5, #16]
	bl	sub_08014878
	movs	r1, #80
	bl	sub_0800206c
	ldr	r2, [r5, #12]
	adds	r0, #80
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #3
	lsls	r0, r0, #16
	subs	r0, r0, r3
	str	r0, [r5, #0]
	bl	sub_08014878
	ldr	r2, [r5, #16]
	movs	r1, #7
	ands	r1, r0
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r1, #96
	lsls	r3, r3, #3
	lsls	r1, r1, #16
	subs	r1, r1, r3
	ldmia	r6!, {r2}
	ldr	r3, [r5, #8]
	ldr	r4, [pc, #384]
	movs	r0, #1
	add	r8, r0
	str	r1, [r5, #4]
	strh	r3, [r2, #18]
	mov	r1, r8
	movs	r3, #8
	strb	r7, [r2, #22]
	strb	r3, [r2, #23]
	add	sl, r4
	adds	r5, #28
	cmp	r1, #16
	bne.n	.L_0818089c
	movs	r2, #0
	ldr	r3, [pc, #360]
	mov	r8, r2
	movs	r1, #1
	movs	r2, #128
	negs	r1, r1
	lsls	r2, r2, #2
.L_08180928:
	movs	r4, #1
	add	r8, r4
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r8, r2
	bne.n	.L_08180928
.L_08180934:
	ldr	r3, [pc, #340]
	ldr	r7, [sp, #32]
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	movs	r1, #238
	str	r3, [sp, #152]
	str	r4, [sp, #156]
	movs	r3, #0
	str	r3, [r7, #12]
	movs	r3, #255
	movs	r0, #0
	lsls	r3, r3, #16
	lsls	r1, r1, #7
	str	r3, [r7, #4]
	adds	r1, #220
	str	r0, [sp, #20]
	str	r0, [sp, #16]
	add	r1, fp
	mov	r8, r0
	mov	r4, fp
	mov	r9, r1
.L_0818095e:
	ldr	r3, [pc, #304]
	ldr	r2, [sp, #20]
	ldr	r7, [sp, #140]
	ldrh	r3, [r3, r2]
	cmp	r7, r3
	bge.n	.L_0818096c
	b.n	.L_08180b38
.L_0818096c:
	ldr	r3, [r4, #24]
	cmp	r3, #0
	beq.n	.L_08180974
	b.n	.L_08180b38
.L_08180974:
	ldr	r3, [r4, #0]
	ldr	r0, [sp, #32]
	ldr	r1, [pc, #280]
	str	r3, [r0, #0]
	ldr	r3, [r4, #4]
	str	r3, [r0, #8]
	subs	r3, #1
	cmp	r3, r1
	bhi.n	.L_08180998
	mov	r2, r9
	ldr	r0, [r2, #0]
	ldr	r1, [sp, #32]
	add	r2, sp, #152
	movs	r3, #0
	str	r4, [sp, #8]
	bl	sub_08020010
	ldr	r4, [sp, #8]
.L_08180998:
	ldr	r3, [r4, #4]
	ldr	r7, [pc, #252]
	cmp	r3, r7
	bgt.n	.L_081809a2
	b.n	.L_08180aa0
.L_081809a2:
	movs	r3, #1
	str	r3, [r4, #24]
	ldr	r1, [sp, #16]
	ldr	r2, [pc, #240]
	movs	r0, #0
	mov	sl, r0
	adds	r7, r1, r2
.L_081809b0:
	str	r4, [sp, #8]
	bl	sub_08014878
	movs	r6, #31
	ands	r6, r0
	bl	sub_08014878
	movs	r5, #254
	ldr	r4, [sp, #8]
	lsls	r5, r5, #7
	adds	r5, #255
	movs	r3, #128
	ands	r5, r0
	lsls	r3, r3, #7
	adds	r5, r5, r3
	ldr	r3, [r4, #0]
	adds	r0, r5, #0
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r7, #0]
	adds	r6, #32
	ldr	r3, [r4, #4]
	str	r3, [r7, #4]
	bl	sub_08002096
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #4
	str	r3, [r7, #12]
	adds	r0, r5, #0
	bl	sub_08002090
	adds	r3, r6, #0
	muls	r3, r0
	movs	r0, #1
	asrs	r3, r3, #4
	add	sl, r0
	str	r3, [r7, #16]
	mov	r1, sl
	movs	r3, #0
	str	r3, [r7, #24]
	ldr	r4, [sp, #8]
	adds	r7, #28
	cmp	r1, #2
	bne.n	.L_081809b0
	movs	r0, #144
	bl	sub_081c0010
	ldr	r7, [sp, #148]
	movs	r2, #0
	ldr	r3, [r7, #20]
	mov	sl, r2
	ldr	r4, [sp, #8]
	cmp	r3, #0
	beq.n	.L_08180ae2
	movs	r5, #36
.L_08180a22:
	ldr	r1, [sp, #148]
	movs	r3, #128
	lsls	r3, r3, #10
	ldrsh	r0, [r5, r1]
	str	r3, [sp, #0]
	movs	r3, #100
	str	r3, [sp, #4]
	movs	r1, #1
	movs	r2, #0
	movs	r3, #0
	str	r4, [sp, #8]
	bl	sub_0815f000
	ldr	r3, [sp, #148]
	movs	r1, #7
	ldrsh	r0, [r5, r3]
	movs	r3, #7
	str	r3, [sp, #0]
	movs	r2, #5
	mov	r3, sl
	bl	sub_0814cd48
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	movs	r3, #4
	add	r2, fp
	str	r3, [r2, #0]
	ldr	r1, [sp, #148]
	movs	r0, #1
	ldr	r3, [r1, #20]
	add	sl, r0
	adds	r5, #2
	ldr	r4, [sp, #8]
	cmp	sl, r3
	bne.n	.L_08180a22
	b.n	.L_08180ae2
	.4byte 0x03000730
	.4byte 0x80006000
	.4byte 0x0000013e
	.4byte 0x000000ba
	.4byte 0x00000148
	.4byte 0xfffff800
	.4byte 0xffffff00
	.4byte 0x02014018
	.4byte 0x08196ee0
	.4byte 0x081995cc
	.4byte 0x007ffffe
	.4byte 0x006fffff
	.2byte 0x4000
	.2byte 0x0201
.L_08180aa0:
	ldr	r3, [pc, #192]
	ldr	r2, [sp, #20]
	ldr	r7, [sp, #140]
	ldrh	r3, [r3, r2]
	subs	r1, r7, r3
	adds	r3, r1, #0
	cmp	r1, #0
	bge.n	.L_08180ab2
	adds	r3, #15
.L_08180ab2:
	asrs	r2, r3, #4
	lsls	r3, r2, #4
	mov	r0, r8
	subs	r2, r1, r3
	lsls	r3, r0, #4
	adds	r3, r3, r2
	lsls	r2, r3, #3
	subs	r2, r2, r3
	ldr	r1, [pc, #164]
	ldr	r3, [r4, #0]
	lsls	r2, r2, #2
	adds	r2, r2, r1
	lsrs	r1, r3, #31
	adds	r3, r3, r1
	asrs	r3, r3, #1
	str	r3, [r2, #0]
	ldr	r7, [pc, #152]
	ldr	r3, [r4, #4]
	adds	r3, r3, r7
	str	r3, [r2, #4]
	movs	r3, #0
	str	r3, [r2, #12]
	str	r3, [r2, #16]
	str	r3, [r2, #24]
.L_08180ae2:
	mov	r0, r9
	ldr	r3, [r4, #8]
	ldr	r2, [r0, #0]
	movs	r1, #64
	adds	r0, r4, #0
	strh	r3, [r2, #18]
	movs	r2, #0
	str	r4, [sp, #8]
	bl	sub_08138058
	mov	r1, r9
	ldr	r3, [r1, #0]
	mov	r7, fp
	ldrh	r0, [r3, #18]
	bl	sub_08002090
	ldr	r4, [sp, #8]
	negs	r0, r0
	mov	r2, r9
	lsls	r0, r0, #3
	ldr	r3, [r2, #0]
	str	r0, [r4, #12]
	ldrh	r0, [r3, #18]
	bl	sub_08002096
	movs	r1, #238
	ldr	r4, [sp, #8]
	negs	r0, r0
	lsls	r1, r1, #7
	lsls	r0, r0, #3
	mov	r3, r9
	adds	r1, #220
	str	r0, [r4, #16]
	ldr	r0, [r3, #0]
	ldr	r3, [r7, r1]
	ldr	r1, [pc, #52]
	ldrh	r2, [r3, #8]
	ldrh	r3, [r0, #8]
	lsls	r2, r2, #22
	lsrs	r2, r2, #22
	ands	r3, r1
	orrs	r3, r2
	strh	r3, [r0, #8]
.L_08180b38:
	ldr	r3, [sp, #20]
	ldr	r7, [sp, #16]
	movs	r0, #1
	add	r8, r0
	movs	r2, #4
	adds	r3, #2
	adds	r7, #224
	mov	r1, r8
	add	r9, r2
	str	r3, [sp, #20]
	adds	r4, #28
	str	r7, [sp, #16]
	cmp	r1, #16
	beq.n	.L_08180b56
	b.n	.L_0818095e
.L_08180b56:
	ldr	r5, [pc, #16]
	movs	r2, #0
	mov	r8, r2
	b.n	.L_08180b70
	movs	r0, r0
	.4byte 0xfffffc00
	.4byte 0x081995cc
	.4byte 0x02015c00
	.2byte 0x0000
	.2byte 0xfff0
.L_08180b70:
	.2byte 0x69ab
	cmp	r3, #7
	bhi.n	.L_08180bae
	subs	r2, r3, #2
	cmp	r2, #0
	blt.n	.L_08180baa
	lsls	r1, r2, #3
	adds	r1, r1, r2
	lsls	r1, r1, #5
	movs	r3, #220
	lsls	r3, r3, #6
	movs	r0, #12
	add	r1, fp
	movs	r4, #2
	ldrsh	r2, [r5, r4]
	adds	r1, r1, r3
	movs	r7, #6
	ldrsh	r3, [r5, r7]
	str	r0, [sp, #0]
	movs	r0, #24
	str	r0, [sp, #4]
	ldr	r0, [sp, #84]
	subs	r3, #12
	ldr	r4, [r0, #4]
	subs	r2, #6
	ldr	r0, [sp, #144]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x69ab
.L_08180baa:
	adds	r3, #1
	str	r3, [r5, #24]
.L_08180bae:
	movs	r1, #1
	movs	r2, #128
	add	r8, r1
	lsls	r2, r2, #1
	adds	r5, #28
	cmp	r8, r2
	bne.n	.L_08180b70
	ldr	r5, [pc, #200]
	movs	r3, #0
	mov	r8, r3
.L_08180bc2:
	ldr	r0, [r5, #24]
	cmp	r0, #17
	bhi.n	.L_08180c10
	movs	r1, #3
	bl	sub_08002054
	adds	r1, r0, #0
	lsls	r1, r1, #11
	movs	r0, #224
	lsls	r0, r0, #3
	add	r1, fp
	adds	r1, r1, r0
	mov	r7, r8
	movs	r0, #32
	movs	r4, #1
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	ands	r4, r7
	movs	r7, #6
	ldrsh	r3, [r5, r7]
	str	r0, [sp, #0]
	movs	r0, #64
	str	r0, [sp, #4]
	ldr	r0, [sp, #84]
	lsls	r4, r4, #2
	subs	r3, #48
	ldr	r4, [r4, r0]
	subs	r2, #16
	ldr	r0, [sp, #144]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c28
	movs	r1, #56
	ldr	r2, [pc, #132]
	bl	sub_08138086
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
.L_08180c10:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	adds	r5, #28
	cmp	r2, #128
	bne.n	.L_08180bc2
	movs	r0, #8
	movs	r1, #8
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
	ldr	r3, [sp, #140]
	adds	r3, #1
	str	r3, [sp, #140]
	cmp	r3, #109
	beq.n	.L_08180c46
	b.n	.L_08180882
.L_08180c46:
	movs	r5, #238
	lsls	r5, r5, #7
	movs	r4, #0
	adds	r5, #220
	mov	r8, r4
	add	r5, fp
.L_08180c52:
	movs	r7, #1
	ldmia	r5!, {r0}
	add	r8, r7
	bl	sub_08020048
	mov	r0, r8
	cmp	r0, #16
	bne.n	.L_08180c52
	ldr	r0, [pc, #44]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #268
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x02014000
	.4byte 0xffffe000
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #68
	str	r0, [sp, #32]
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r0, [r5, #96]
	str	r0, [sp, #28]
	ldr	r2, [r5, #92]
	mov	sl, r2
	bl	sub_0813ba50
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #68
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	adds	r2, #4
	strh	r3, [r2, #0]
	movs	r0, #0
	bl	sub_081435e0
	ldr	r3, [pc, #40]
	movs	r2, #128
	lsls	r2, r2, #19
	movs	r1, #128
	adds	r2, #82
	lsls	r1, r1, #2
	strh	r3, [r2, #0]
	adds	r1, #150
	movs	r2, #3
	movs	r0, #1
	bl	sub_08152404
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #240
	add	r3, sl
	ldr	r0, [r3, #0]
	b.n	.L_08180cfc
	.4byte 0x00001080
	.4byte 0x00002737
	.2byte 0x1010
	.2byte 0x0000
.L_08180cfc:
	bl	sub_0814cc4c
	movs	r1, #3
	movs	r0, #104
	bl	sub_081963ec
	ldr	r5, [r5, #104]
	movs	r1, #224
	lsls	r1, r1, #3
	ldr	r0, [pc, #656]
	add	r1, sl
	movs	r2, #1
	movs	r3, #0
	str	r5, [sp, #20]
	bl	sub_08157cf4
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
	str	r3, [r2, #0]
	ldr	r0, [pc, #620]
	lsls	r1, r1, #4
	bl	sub_080145a8
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #180
	add	r2, sl
	movs	r3, #24
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #184
	movs	r3, #0
	add	r2, sl
	str	r3, [r2, #0]
	mov	fp, r3
	ldr	r3, [pc, #592]
	str	r3, [sp, #8]
.L_08180d5a:
	mov	r4, fp
	cmp	r4, #0
	bne.n	sub_08180d8a
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
	str	r3, [r2, #0]
	ldr	r0, [pc, #564]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #556]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4658
	cmp	r0, #56
	bne.n	.L_08180d96
	movs	r0, #140
	bl	sub_081c0010
.L_08180d96:
	movs	r6, #0
.L_08180d98:
	ldr	r3, [pc, #536]
	ldrb	r3, [r3, r6]
	cmp	fp, r3
	bne.n	.L_08180dfa
	ldr	r4, [sp, #32]
	movs	r2, #0
	ldr	r3, [r4, #20]
	mov	r8, r2
	cmp	r3, #0
	beq.n	.L_08180de8
	movs	r7, #128
	lsls	r7, r7, #10
	movs	r5, #36
.L_08180db2:
	ldr	r2, [sp, #32]
	movs	r1, #1
	ldrsh	r0, [r5, r2]
	movs	r3, #120
	str	r3, [sp, #4]
	movs	r3, #128
	adds	r2, r7, #0
	lsls	r3, r3, #12
	str	r7, [sp, #0]
	bl	sub_0815f000
	ldr	r4, [sp, #32]
	movs	r3, #7
	ldrsh	r0, [r5, r4]
	movs	r1, #7
	str	r3, [sp, #0]
	movs	r2, #5
	mov	r3, r8
	bl	sub_0814cd48
	ldr	r4, [sp, #32]
	movs	r3, #1
	add	r8, r3
	ldr	r3, [r4, #20]
	adds	r5, #2
	cmp	r8, r3
	bne.n	.L_08180db2
.L_08180de8:
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, sl
	movs	r3, #4
	str	r3, [r2, #0]
	movs	r0, #145
	bl	sub_081c0010
.L_08180dfa:
	adds	r6, #1
	cmp	r6, #7
	bne.n	.L_08180d98
	mov	r0, fp
	cmp	r0, #79
	bgt.n	.L_08180e08
	b.n	.L_08180f20
.L_08180e08:
	cmp	r0, #80
	bne.n	sub_08180e38
	ldr	r3, [pc, #424]
	movs	r2, #1
	movs	r6, #0
	negs	r2, r2
.L_08180e14:
	adds	r6, #1
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r6, #64
	bne.n	.L_08180e14
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #168
	add	r3, sl
	movs	r2, #8
	str	r2, [r3, #0]
	movs	r1, #240
	ldr	r3, [pc, #396]
	ldr	r0, [sp, #28]
	lsls	r1, r1, #6
	ldr	r2, [pc, #396]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x465b
	subs	r3, #80
	cmp	r3, #7
	bhi.n	.L_08180ed0
	bl	sub_08014878
	movs	r3, #63
	ands	r3, r0
	adds	r3, #28
	lsls	r3, r3, #16
	str	r3, [sp, #16]
	bl	sub_08014878
	movs	r3, #31
	ands	r3, r0
	ldr	r4, [sp, #8]
	adds	r3, #80
	lsls	r3, r3, #16
	str	r3, [sp, #12]
	movs	r3, #15
	mov	r9, r3
	ldr	r0, [pc, #352]
	lsls	r3, r4, #3
	subs	r3, r3, r4
	movs	r2, #0
	lsls	r3, r3, #2
	mov	r8, r2
	adds	r7, r3, r0
.L_08180e70:
	bl	sub_08014878
	movs	r5, #31
	ands	r5, r0
	bl	sub_08014878
	adds	r6, r0, #0
	bl	sub_08014878
	mov	r2, r9
	ldr	r3, [sp, #16]
	ands	r0, r2
	subs	r0, #8
	lsls	r0, r0, #16
	adds	r0, r3, r0
	str	r0, [r7, #0]
	bl	sub_08014878
	mov	r4, r9
	ands	r0, r4
	ldr	r2, [sp, #12]
	subs	r0, #8
	lsls	r0, r0, #16
	adds	r0, r2, r0
	str	r0, [r7, #4]
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r5, #32
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #3
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #3
	str	r3, [r7, #16]
	movs	r3, #0
	str	r3, [r7, #24]
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	adds	r7, #28
	cmp	r4, #8
	bne.n	.L_08180e70
.L_08180ed0:
	ldr	r5, [pc, #240]
	movs	r6, #0
.L_08180ed4:
	ldr	r0, [r5, #24]
	cmp	r0, #17
	bhi.n	.L_08180f18
	movs	r1, #3
	bl	sub_08002054
	adds	r1, r0, #0
	lsls	r1, r1, #11
	movs	r0, #224
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	lsls	r0, r0, #3
	movs	r4, #6
	ldrsh	r3, [r5, r4]
	add	r1, sl
	adds	r1, r1, r0
	movs	r0, #32
	str	r0, [sp, #0]
	movs	r0, #64
	subs	r3, #48
	str	r0, [sp, #4]
	subs	r2, #16
	ldr	r0, [sp, #28]
	ldr	r4, [sp, #20]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c28
	movs	r1, #62
	ldr	r2, [pc, #184]
	bl	sub_08138086
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
.L_08180f18:
	adds	r6, #1
	adds	r5, #28
	cmp	r6, #64
	bne.n	.L_08180ed4
.L_08180f20:
	mov	r0, fp
	cmp	r0, #91
	bgt.n	.L_08180ff4
	ldr	r3, [pc, #164]
	movs	r1, #0
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	str	r3, [sp, #44]
	str	r4, [sp, #48]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #220
	add	r3, sl
	ldr	r2, [r3, #0]
	movs	r3, #32
	strb	r3, [r2, #23]
	movs	r3, #136
	lsls	r3, r3, #8
	strh	r3, [r2, #18]
	movs	r3, #255
	add	r4, sp, #52
	lsls	r3, r3, #16
	strb	r1, [r2, #22]
	str	r1, [r4, #12]
	str	r3, [r4, #4]
	cmp	r0, #63
	bgt.n	.L_08180f72
	mov	r3, fp
	cmp	r0, #0
	bge.n	.L_08180f5e
	adds	r3, #3
.L_08180f5e:
	asrs	r3, r3, #2
	movs	r2, #152
	subs	r2, r2, r3
	lsls	r2, r2, #16
	ldr	r0, [pc, #104]
	str	r2, [r4, #0]
	mov	r2, fp
	lsls	r3, r2, #17
	adds	r3, r3, r0
	b.n	.L_08180fde
.L_08180f72:
	mov	r1, fp
	subs	r1, #64
	cmp	r1, #63
	bhi.n	.L_08180fd4
	adds	r2, r1, #0
	cmp	r1, #0
	bge.n	.L_08180f84
	mov	r2, fp
	subs	r2, #49
.L_08180f84:
	asrs	r2, r2, #4
	movs	r3, #136
	subs	r3, r3, r2
	lsls	r3, r3, #16
	str	r3, [r4, #0]
	lsrs	r3, r1, #31
	adds	r3, r1, r3
	asrs	r3, r3, #1
	movs	r2, #128
	lsls	r3, r3, #16
	lsls	r2, r2, #15
	adds	r3, r3, r2
	b.n	.L_08180fde
	movs	r0, r0
	.4byte 0x0000013e
	.4byte 0x08143001
	.4byte 0xfffffd80
	.4byte 0x00000148
	.4byte 0x03000730
	.4byte 0x081995ec
	.4byte 0x02014018
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.4byte 0x02014000
	.4byte 0xfffff800
	.4byte 0x08196ee8
	.2byte 0x0000
	.2byte 0xffc0
.L_08180fd4:
	.2byte 0x2388
	lsls	r3, r3, #16
	str	r3, [r4, #0]
	movs	r3, #128
	lsls	r3, r3, #15
.L_08180fde:
	str	r3, [r4, #8]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #220
	add	r3, sl
	ldr	r0, [r3, #0]
	adds	r1, r4, #0
	add	r2, sp, #44
	movs	r3, #0
	bl	sub_08020010
.L_08180ff4:
	mov	r3, fp
	subs	r3, #56
	cmp	r3, #35
	bls.n	.L_08180ffe
	b.n	.L_08181158
.L_08180ffe:
	mov	r4, fp
	lsls	r3, r4, #1
	movs	r6, #128
	subs	r3, #104
	lsls	r6, r6, #15
	movs	r5, #128
	cmp	r3, #64
	bgt.n	.L_0818101c
	lsls	r5, r3, #1
	movs	r2, #128
	ldr	r0, [pc, #416]
	adds	r1, r5, #0
	lsls	r2, r2, #9
	bl	sub_0815b434
.L_0818101c:
	movs	r3, #160
	lsls	r3, r3, #15
	asrs	r2, r3, #16
	ldr	r0, [pc, #400]
	asrs	r1, r6, #16
	adds	r3, r5, #0
	bl	sub_0818caa8
	mov	r4, fp
	cmp	r4, #56
	bne.n	.L_08181080
	movs	r6, #0
	movs	r7, #0
	mov	r5, sl
.L_08181038:
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r0
	str	r3, [r5, #0]
	bl	sub_08014878
	movs	r3, #255
	ands	r3, r0
	adds	r3, #128
	str	r3, [r5, #4]
	bl	sub_08014878
	movs	r3, #1
	ands	r0, r3
	cmp	r0, #0
	beq.n	.L_08181064
	ldr	r3, [r5, #4]
	negs	r3, r3
	str	r3, [r5, #4]
.L_08181064:
	adds	r6, #1
	str	r7, [r5, #24]
	subs	r7, #4
	adds	r5, #28
	cmp	r6, #16
	bne.n	.L_08181038
	movs	r1, #142
	lsls	r1, r1, #7
	ldr	r0, [pc, #320]
	add	r1, sl
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
.L_08181080:
	movs	r0, #32
	bl	sub_08014dac
	mov	r9, r0
	movs	r0, #1
	bl	sub_081969f8
	movs	r3, #0
	adds	r7, r0, #0
	ldr	r2, [pc, #296]
	str	r3, [r7, #20]
	ldr	r3, [sp, #36]
	mov	r0, r9
	ands	r3, r2
	movs	r2, #5
	orrs	r3, r2
	ldr	r2, [pc, #284]
	str	r0, [r7, #12]
	ands	r3, r2
	movs	r2, #128
	lsls	r2, r2, #2
	orrs	r3, r2
	str	r3, [sp, #36]
	movs	r3, #142
	lsls	r3, r3, #7
	add	r2, sp, #36
	add	r3, sl
	str	r3, [r2, #4]
	movs	r3, #7
	str	r3, [r7, #0]
	ldr	r3, [pc, #260]
	str	r2, [r7, #16]
	str	r3, [r7, #8]
	movs	r2, #128
	lsls	r2, r2, #9
	movs	r6, #0
	mov	r8, r2
	mov	r5, sl
.L_081810cc:
	ldr	r2, [r5, #4]
	ldr	r1, [r5, #24]
	ldr	r3, [r5, #0]
	lsls	r2, r2, #1
	adds	r1, #2
	adds	r3, r3, r2
	str	r1, [r5, #24]
	str	r3, [r5, #0]
	cmp	r1, #47
	bhi.n	.L_08181144
	bl	sub_08014de4
	movs	r1, #3
	ands	r1, r6
	ldr	r4, [pc, #220]
	lsls	r0, r1, #17
	movs	r3, #132
	adds	r1, #2
	movs	r2, #160
	lsls	r3, r3, #15
	lsls	r2, r2, #15
	lsls	r1, r1, #16
	adds	r0, r0, r3
	subs	r1, r2, r1
	adds	r0, r0, r4
	adds	r1, r1, r4
	movs	r2, #0
	bl	sub_08015160
	movs	r0, #128
	mov	r1, r8
	mov	r2, r8
	lsls	r0, r0, #8
	bl	sub_080151e4
	ldr	r0, [r5, #0]
	bl	sub_080150e4
	ldr	r2, [r5, #24]
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #170
	adds	r0, r2, #0
	muls	r0, r3
	bl	sub_08002096
	movs	r1, #128
	add	r0, r8
	lsls	r1, r1, #10
	mov	r2, r8
	bl	sub_080151e4
	ldr	r0, [pc, #148]
	mov	r1, r9
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
.L_08181144:
	adds	r6, #1
	adds	r5, #28
	cmp	r6, #6
	bne.n	.L_081810cc
	adds	r0, r7, #0
	bl	sub_08013164
	mov	r0, r9
	bl	sub_08013164
.L_08181158:
	movs	r0, #8
	movs	r1, #8
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	movs	r3, #1
	add	r2, sl
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [sp, #8]
	movs	r4, #1
	add	fp, r4
	adds	r3, #8
	mov	r0, fp
	str	r3, [sp, #8]
	cmp	r0, #108
	beq.n	.L_08181188
	b.n	.L_08180d5a
.L_08181188:
	ldr	r0, [pc, #68]
	bl	sub_08014644
	movs	r0, #104
	bl	sub_0801314c
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #220
	add	r3, sl
	ldr	r0, [r3, #0]
	bl	sub_08020048
	bl	sub_08143bb8
	add	sp, #68
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02010000
	.4byte 0x000000c9
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x08199220
	.4byte 0xffc00000
	.4byte 0x081991b0
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	mov	sl, r0
	ldr	r0, [r3, #92]
	sub	sp, #68
	str	r0, [sp, #52]
	movs	r0, #0
	ldr	r3, [r3, #96]
	movs	r5, #7
	mov	r8, r3
	bl	sub_08143a88
	ldr	r3, [pc, #28]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r3, [pc, #20]
	adds	r2, #50
	strh	r3, [r2, #0]
	movs	r1, #0
	mov	fp, r1
	movs	r0, #0
.L_08181210:
	movs	r1, #0
	subs	r4, r0, #3
	b.n	.L_08181220
	movs	r0, r0
	.4byte 0x00000100
	.2byte 0x0410
	.2byte 0x0000
.L_08181220:
	mov	r2, sl
	ldr	r3, [r2, #4]
	cmp	r3, #0
	bne.n	.L_08181238
	cmp	r1, #7
	bgt.n	.L_0818123c
	cmp	r4, #3
	bhi.n	.L_08181256
	adds	r3, r0, #0
	adds	r2, r1, #0
	adds	r3, #9
	b.n	.L_0818125a
.L_08181238:
	cmp	r1, #7
	bgt.n	.L_08181248
.L_0818123c:
	cmp	r0, #11
	bgt.n	.L_08181256
	adds	r2, r1, #0
	ands	r2, r5
	adds	r3, r0, #0
	b.n	.L_0818125a
.L_08181248:
	cmp	r4, #3
	bhi.n	.L_08181256
	adds	r2, r1, #0
	adds	r3, r0, #0
	ands	r2, r5
	adds	r3, #9
	b.n	.L_0818125a
.L_08181256:
	movs	r2, #0
	movs	r3, #0
.L_0818125a:
	lsls	r3, r3, #3
	adds	r3, r3, r2
	lsls	r3, r3, #1
	adds	r2, r3, #1
	lsls	r2, r2, #8
	orrs	r2, r3
	ldr	r3, [pc, #104]
	lsls	r2, r2, #16
	add	r3, fp
	asrs	r2, r2, #16
	strh	r2, [r3, #0]
	adds	r1, #1
	movs	r3, #2
	add	fp, r3
	cmp	r1, #16
	bne.n	.L_08181220
	adds	r0, #1
	cmp	r0, #16
	bne.n	.L_08181210
	ldr	r3, [pc, #80]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #44
	str	r3, [r2, #0]
	ldr	r1, [pc, #52]
	movs	r3, #128
	ldr	r2, [pc, #52]
	lsls	r3, r3, #19
	adds	r3, #64
	strh	r1, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	subs	r3, #2
	strh	r1, [r3, #0]
	adds	r3, #4
	strh	r2, [r3, #0]
	ldr	r3, [pc, #36]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #72
	strh	r3, [r2, #0]
	ldr	r3, [pc, #28]
	adds	r2, #2
	strh	r3, [r2, #0]
	ldr	r4, [sp, #52]
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r1, r4, r2
	movs	r3, #1
	b.n	.L_081812d8
	movs	r0, r0
	.4byte 0x000000f0
	.4byte 0x00002888
	.4byte 0x00003737
	.4byte 0x00002033
	.4byte 0x06003800
	.2byte 0xd800
	.2byte 0xffff
.L_081812d8:
	.2byte 0x2201
	ldr	r0, [pc, #200]
	bl	sub_08157cf4
	movs	r1, #35
	movs	r0, #104
	bl	sub_081963ec
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r3, [r5, #104]
	movs	r1, #39
	movs	r0, #188
	str	r3, [sp, #40]
	bl	sub_081963ec
	ldr	r4, [sp, #52]
	movs	r0, #239
	movs	r1, #238
	lsls	r0, r0, #7
	adds	r5, #188
	lsls	r1, r1, #7
	adds	r2, r4, r0
	ldr	r5, [r5, #0]
	movs	r3, #1
	adds	r1, #132
	str	r3, [r2, #0]
	adds	r2, r4, r1
	movs	r3, #0
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #140]
	str	r5, [sp, #44]
	bl	sub_080145a8
	movs	r2, #172
	lsls	r2, r2, #15
	ldr	r1, [sp, #52]
	str	r2, [sp, #36]
	movs	r2, #146
	movs	r3, #128
	mov	r0, sp
	lsls	r2, r2, #7
	lsls	r3, r3, #13
	movs	r4, #0
	adds	r0, #56
	adds	r2, r1, r2
	str	r3, [sp, #32]
	str	r4, [sp, #48]
	str	r0, [sp, #12]
	str	r2, [sp, #20]
.L_08181340:
	movs	r3, #192
	lsls	r3, r3, #18
	mov	r4, sl
	ldr	r5, [r3, #48]
	ldr	r1, [sp, #12]
	movs	r3, #36
	ldrsh	r0, [r4, r3]
	bl	sub_0815e20c
	ldr	r0, [sp, #48]
	cmp	r0, #31
	bgt.n	.L_0818136c
	mov	r1, sl
	ldr	r3, [r1, #4]
	cmp	r3, #0
	bne.n	.L_08181366
	ldrh	r3, [r5, #54]
	subs	r3, #128
	b.n	.L_0818136a
.L_08181366:
	ldrh	r3, [r5, #54]
	adds	r3, #128
.L_0818136a:
	strh	r3, [r5, #54]
.L_0818136c:
	ldr	r2, [sp, #48]
	cmp	r2, #108
	blt.n	.L_08181384
	movs	r4, #124
	ldr	r1, [pc, #40]
	movs	r3, #128
	subs	r2, r4, r2
	lsls	r3, r3, #19
	subs	r2, #1
	adds	r3, #82
	orrs	r2, r1
	strh	r2, [r3, #0]
.L_08181384:
	ldr	r0, [sp, #48]
	cmp	r0, #0
	bne.n	.L_08181422
	mov	r1, sl
	ldr	r6, [r1, #4]
	cmp	r6, #0
	bne.n	.L_081813ac
	movs	r2, #172
	movs	r3, #128
	lsls	r2, r2, #15
	lsls	r3, r3, #13
	str	r2, [sp, #36]
	str	r3, [sp, #32]
	b.n	.L_081813b4
	.4byte 0x00001000
	.4byte 0x000000b0
	.2byte 0x3001
	.2byte 0x0814
.L_081813ac:
	ldr	r4, [pc, #188]
	ldr	r0, [pc, #192]
	str	r4, [sp, #36]
	str	r0, [sp, #32]
.L_081813b4:
	movs	r1, #0
	mov	fp, r1
	b.n	.L_081813be
.L_081813ba:
	mov	r2, sl
	ldr	r6, [r2, #4]
.L_081813be:
	mov	r4, fp
	lsls	r3, r4, #3
	ldr	r0, [sp, #52]
	subs	r3, r3, r4
	lsls	r3, r3, #2
	adds	r5, r0, r3
	cmp	r6, #0
	bne.n	.L_081813da
	movs	r3, #160
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	movs	r3, #192
	lsls	r3, r3, #12
	b.n	.L_081813e2
.L_081813da:
	movs	r3, #128
	lsls	r3, r3, #14
	str	r3, [r5, #0]
	ldr	r3, [pc, #144]
.L_081813e2:
	str	r3, [r5, #12]
	movs	r3, #192
	lsls	r3, r3, #12
	str	r3, [r5, #4]
	mov	r3, sl
	ldr	r2, [r3, #24]
	ldr	r1, [pc, #136]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	add	r3, fp
	ldrb	r3, [r1, r3]
	cmp	r3, #0
	beq.n	.L_081813fe
	movs	r3, #1
.L_081813fe:
	str	r3, [r5, #8]
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	ldr	r4, [pc, #112]
	adds	r3, #255
	ands	r3, r0
	movs	r0, #1
	adds	r3, r3, r4
	add	fp, r0
	str	r3, [r5, #16]
	mov	r1, fp
	movs	r3, #0
	str	r3, [r5, #20]
	str	r3, [r5, #24]
	cmp	r1, #7
	bne.n	.L_081813ba
.L_08181422:
	ldr	r2, [sp, #48]
	cmp	r2, #24
	bgt.n	.L_08181480
	ldr	r4, [sp, #36]
	movs	r3, #128
	asrs	r0, r4, #16
	lsls	r3, r3, #19
	negs	r2, r0
	adds	r3, #40
	lsls	r2, r2, #8
	str	r2, [r3, #0]
	mov	r1, sl
	ldr	r3, [r1, #4]
	cmp	r3, #0
	bne.n	.L_0818145a
	ldr	r3, [pc, #36]
	adds	r2, r0, #0
	adds	r2, #120
	lsls	r2, r2, #8
	orrs	r2, r3
	movs	r1, #128
	movs	r3, #128
	lsls	r1, r1, #19
	lsls	r3, r3, #19
	adds	r1, #64
	adds	r3, #66
	strh	r2, [r1, #0]
	b.n	.L_0818149c
.L_0818145a:
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r2, r0, #0
	adds	r3, #64
	adds	r2, #240
	b.n	.L_08181498
	movs	r0, r0
	.4byte 0x000000f0
	.4byte 0xffaa0000
	.4byte 0xfff00000
	.4byte 0xfff40000
	.4byte 0x081995f3
	.2byte 0x8000
	.2byte 0xffff
.L_08181480:
	.2byte 0x9c09
	movs	r2, #128
	asrs	r3, r4, #16
	lsls	r2, r2, #19
	negs	r3, r3
	adds	r2, #40
	lsls	r3, r3, #8
	str	r3, [r2, #0]
	ldr	r2, [pc, #32]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #64
.L_08181498:
	strh	r2, [r3, #0]
	adds	r3, #2
.L_0818149c:
	strh	r2, [r3, #0]
	ldr	r1, [sp, #32]
	ldr	r0, [sp, #36]
	movs	r3, #52
	muls	r3, r1
	subs	r0, r0, r1
	str	r0, [sp, #36]
	cmp	r3, #0
	bge.n	.L_081814b8
	adds	r3, #63
	b.n	.L_081814b8
	movs	r0, r0
	.2byte 0x00f0
	.2byte 0x0000
.L_081814b8:
	asrs	r3, r3, #6
	str	r3, [sp, #32]
	ldr	r3, [sp, #48]
	movs	r2, #0
	str	r2, [sp, #28]
	cmp	r3, #105
	ble.n	.L_081814cc
	movs	r4, #1
	str	r4, [sp, #28]
	b.n	.L_081814e2
.L_081814cc:
	ldr	r0, [sp, #48]
	cmp	r0, #21
	ble.n	.L_081814e2
	subs	r0, #22
	movs	r1, #12
	bl	sub_08002064
	cmp	r0, #3
	ble.n	.L_081814e2
	movs	r1, #1
	str	r1, [sp, #28]
.L_081814e2:
	mov	r2, sl
	ldr	r3, [r2, #4]
	cmp	r3, #0
	bne.n	.L_08181520
	ldr	r3, [sp, #28]
	cmp	r3, #0
	bne.n	.L_08181502
	ldr	r4, [sp, #52]
	movs	r3, #88
	movs	r2, #224
	lsls	r2, r2, #3
	str	r3, [sp, #0]
	movs	r3, #96
	adds	r1, r4, r2
	str	r3, [sp, #4]
	b.n	.L_08181512
.L_08181502:
	movs	r3, #88
	ldr	r2, [sp, #52]
	str	r3, [sp, #0]
	movs	r3, #96
	str	r3, [sp, #4]
	movs	r3, #160
	lsls	r3, r3, #6
	adds	r1, r2, r3
.L_08181512:
	mov	r0, r8
	movs	r2, #32
	movs	r3, #0
	ldr	r4, [sp, #40]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe01f
.L_08181520:
	ldr	r0, [sp, #28]
	cmp	r0, #0
	bne.n	.L_08181544
	movs	r3, #88
	ldr	r2, [sp, #52]
	str	r3, [sp, #0]
	movs	r3, #96
	str	r3, [sp, #4]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	mov	r0, r8
	movs	r2, #8
	movs	r3, #0
	ldr	r4, [sp, #44]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe00d
.L_08181544:
	movs	r3, #88
	ldr	r2, [sp, #52]
	str	r3, [sp, #0]
	movs	r3, #96
	str	r3, [sp, #4]
	movs	r3, #160
	lsls	r3, r3, #6
	adds	r1, r2, r3
	mov	r0, r8
	movs	r2, #8
	movs	r3, #0
	ldr	r4, [sp, #44]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9b0a
	ldr	r4, [sp, #44]
	ldr	r1, [sp, #52]
	movs	r2, #146
	movs	r0, #0
	lsls	r2, r2, #7
	adds	r2, #204
	str	r3, [sp, #24]
	str	r4, [sp, #16]
	str	r0, [sp, #8]
	ldr	r5, [sp, #52]
	adds	r1, r1, r2
	mov	fp, r0
	mov	r9, r1
.L_0818157c:
	ldr	r3, [sp, #8]
	ldr	r0, [sp, #48]
	adds	r3, #22
	cmp	r0, r3
	bge.n	.L_08181594
	cmp	r0, #21
	ble.n	.L_0818158c
	b.n	sub_08181846
.L_0818158c:
	mov	r1, fp
	cmp	r1, #0
	beq.n	.L_08181594
	b.n	sub_08181846
.L_08181594:
	ldr	r3, [sp, #8]
	ldr	r2, [sp, #48]
	adds	r3, #26
	cmp	r2, r3
	blt.n	.L_081815b6
	ldr	r4, [r5, #24]
	cmp	r4, #0
	bne.n	.L_081815b8
	ldr	r3, [r5, #0]
	ldr	r2, [r5, #12]
	subs	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r2, [r5, #16]
	ldr	r3, [r5, #4]
	adds	r3, r3, r2
	str	r3, [r5, #4]
	b.n	.L_081815b8
.L_081815b6:
	ldr	r4, [r5, #24]
.L_081815b8:
	cmp	r4, #4
	ble.n	.L_081815be
	b.n	sub_08181846
.L_081815be:
	cmp	r4, #0
	ble.n	.L_081815c8
	adds	r3, r4, #1
	str	r3, [r5, #24]
	adds	r4, r3, #0
.L_081815c8:
	ldr	r3, [r5, #8]
	cmp	r3, #1
	bne.n	.L_081816b4
	cmp	r4, #0
	bne.n	.L_081816ba
	mov	r3, sl
	ldr	r6, [r3, #4]
	movs	r7, #0
	cmp	r6, #0
	bne.n	.L_081815f2
	ldr	r1, [sp, #12]
	ldr	r0, [r5, #0]
	ldr	r2, [r1, #0]
	asrs	r3, r0, #16
	lsrs	r1, r2, #31
	adds	r2, r2, r1
	asrs	r2, r2, #1
	adds	r2, #16
	cmp	r3, r2
	bge.n	.L_08181608
	b.n	.L_08181606
.L_081815f2:
	ldr	r1, [sp, #12]
	ldr	r0, [r5, #0]
	ldr	r2, [r1, #0]
	asrs	r3, r0, #16
	lsrs	r1, r2, #31
	adds	r2, r2, r1
	asrs	r2, r2, #1
	adds	r2, #48
	cmp	r3, r2
	ble.n	.L_08181608
.L_08181606:
	movs	r7, #1
.L_08181608:
	cmp	r7, #1
	bne.n	.L_081816c0
	ldr	r3, [sp, #52]
	movs	r4, #238
	lsls	r4, r4, #7
	adds	r4, #168
	adds	r2, r3, r4
	mov	r0, fp
	movs	r3, #4
	str	r3, [r2, #0]
	cmp	r0, #6
	bne.n	.L_08181634
	mov	r2, sl
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	movs	r1, #4
	bl	sub_08118088
	movs	r0, #134
	bl	sub_081180e8
	b.n	.L_0818165c
.L_08181634:
	mov	r4, sl
	mov	r1, fp
	movs	r3, #36
	ldrsh	r0, [r4, r3]
	lsls	r3, r1, #2
	add	r3, fp
	lsls	r3, r3, #1
	adds	r3, #110
	str	r3, [sp, #4]
	movs	r2, #128
	movs	r3, #128
	lsls	r2, r2, #10
	movs	r1, #1
	lsls	r3, r3, #12
	str	r2, [sp, #0]
	bl	sub_0815f000
	movs	r0, #133
	bl	sub_081c0010
.L_0818165c:
	mov	r3, sl
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	movs	r3, #4
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r3, #0
	movs	r2, #5
	bl	sub_0814cd48
	movs	r3, #1
	str	r3, [r5, #24]
	mov	r4, sl
	ldr	r3, [r4, #4]
	cmp	r3, #0
	bne.n	.L_08181696
	ldr	r0, [sp, #12]
	ldr	r3, [r0, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	movs	r1, #2
	ldrsh	r2, [r5, r1]
	asrs	r3, r3, #1
	subs	r3, r3, r2
	str	r3, [r5, #0]
	adds	r0, r3, #0
	ldr	r6, [r4, #4]
	movs	r4, #1
	b.n	.L_081816c0
.L_08181696:
	ldr	r2, [sp, #12]
	mov	r0, sl
	ldr	r3, [r2, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	movs	r4, #2
	ldrsh	r2, [r5, r4]
	asrs	r3, r3, #1
	subs	r3, r3, r2
	adds	r3, #128
	str	r3, [r5, #0]
	movs	r4, #1
	ldr	r6, [r0, #4]
	adds	r0, r3, #0
	b.n	.L_081816c0
.L_081816b4:
	mov	r1, sl
	ldr	r6, [r1, #4]
	b.n	.L_081816be
.L_081816ba:
	mov	r2, sl
	ldr	r6, [r2, #4]
.L_081816be:
	ldr	r0, [r5, #0]
.L_081816c0:
	cmp	r6, #0
	bne.n	.L_08181784
	cmp	r4, #0
	bne.n	.L_08181738
	movs	r4, #6
	ldrsh	r3, [r5, r4]
	asrs	r2, r0, #16
	movs	r1, #12
	movs	r0, #51
	str	r0, [sp, #0]
	str	r1, [sp, #4]
	ldr	r4, [sp, #24]
	mov	r1, r9
	subs	r2, #111
	adds	r3, #24
	mov	r0, r8
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2002
	ldrsh	r2, [r5, r0]
	movs	r1, #6
	ldrsh	r3, [r5, r1]
	movs	r4, #51
	movs	r0, #12
	str	r4, [sp, #0]
	str	r0, [sp, #4]
	mov	r1, r9
	ldr	r4, [sp, #24]
	adds	r2, #17
	adds	r3, #96
	mov	r0, r8
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2002
	ldrsh	r2, [r5, r0]
	movs	r1, #6
	ldrsh	r3, [r5, r1]
	movs	r4, #17
	movs	r0, #12
	ldr	r1, [sp, #20]
	subs	r2, #128
	adds	r3, #24
	str	r4, [sp, #0]
	str	r0, [sp, #4]
	ldr	r4, [sp, #24]
	mov	r0, r8
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2106
	ldrsh	r3, [r5, r1]
	movs	r0, #2
	ldrsh	r2, [r5, r0]
	movs	r4, #17
	movs	r0, #12
	str	r4, [sp, #0]
	str	r0, [sp, #4]
	adds	r3, #96
	mov	r0, r8
	ldr	r1, [sp, #20]
	b.n	.L_0818177c
.L_08181738:
	ldr	r2, [sp, #56]
	movs	r1, #51
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	asrs	r2, r2, #1
	adds	r2, r2, r0
	movs	r0, #6
	ldrsh	r3, [r5, r0]
	movs	r4, #12
	subs	r2, #111
	adds	r3, #24
	str	r1, [sp, #0]
	str	r4, [sp, #4]
	mov	r1, r9
	ldr	r4, [sp, #24]
	mov	r0, r8
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9a0e
	movs	r1, #51
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [r5, #0]
	asrs	r2, r2, #1
	adds	r2, r2, r3
	movs	r0, #6
	ldrsh	r3, [r5, r0]
	movs	r4, #12
	str	r1, [sp, #0]
	str	r4, [sp, #4]
	adds	r2, #17
	adds	r3, #96
	mov	r0, r8
	mov	r1, r9
.L_0818177c:
	ldr	r4, [sp, #24]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe060
.L_08181784:
	cmp	r4, #0
	bne.n	.L_081817fe
	asrs	r2, r0, #16
	movs	r0, #6
	ldrsh	r3, [r5, r0]
	movs	r1, #51
	movs	r4, #12
	str	r1, [sp, #0]
	str	r4, [sp, #4]
	mov	r1, r9
	ldr	r4, [sp, #16]
	adds	r3, #24
	mov	r0, r8
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2002
	ldrsh	r2, [r5, r0]
	movs	r1, #6
	ldrsh	r3, [r5, r1]
	movs	r4, #51
	movs	r0, #12
	str	r4, [sp, #0]
	str	r0, [sp, #4]
	mov	r1, r9
	ldr	r4, [sp, #16]
	subs	r2, #128
	adds	r3, #96
	mov	r0, r8
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2002
	ldrsh	r2, [r5, r0]
	movs	r1, #6
	ldrsh	r3, [r5, r1]
	movs	r4, #17
	movs	r0, #12
	ldr	r1, [sp, #20]
	adds	r2, #51
	adds	r3, #24
	str	r4, [sp, #0]
	str	r0, [sp, #4]
	ldr	r4, [sp, #16]
	mov	r0, r8
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2002
	ldrsh	r2, [r5, r0]
	movs	r1, #6
	ldrsh	r3, [r5, r1]
	movs	r4, #17
	movs	r0, #12
	str	r4, [sp, #0]
	str	r0, [sp, #4]
	subs	r2, #77
	adds	r3, #96
	mov	r0, r8
	ldr	r1, [sp, #20]
	ldr	r4, [sp, #16]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe023
.L_081817fe:
	ldr	r2, [sp, #56]
	movs	r1, #51
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	asrs	r2, r2, #1
	adds	r2, r2, r0
	movs	r0, #6
	ldrsh	r3, [r5, r0]
	movs	r4, #12
	adds	r3, #24
	str	r1, [sp, #0]
	str	r4, [sp, #4]
	mov	r1, r9
	ldr	r4, [sp, #16]
	mov	r0, r8
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9a0e
	movs	r1, #51
	lsrs	r3, r2, #31
	adds	r2, r2, r3
	ldr	r3, [r5, #0]
	asrs	r2, r2, #1
	adds	r2, r2, r3
	movs	r0, #6
	ldrsh	r3, [r5, r0]
	movs	r4, #12
	str	r1, [sp, #0]
	str	r4, [sp, #4]
	subs	r2, #128
	adds	r3, #96
	mov	r0, r8
	mov	r1, r9
	ldr	r4, [sp, #16]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9802
	movs	r1, #1
	add	fp, r1
	adds	r0, #12
	mov	r2, fp
	str	r0, [sp, #8]
	adds	r5, #28
	cmp	r2, #7
	beq.n	.L_0818185a
	b.n	.L_0818157c
.L_0818185a:
	ldr	r3, [sp, #28]
	cmp	r3, #0
	bne.n	sub_081818a6
	mov	r4, sl
	ldr	r3, [r4, #4]
	cmp	r3, #0
	bne.n	.L_08181888
	movs	r3, #14
	str	r3, [sp, #0]
	movs	r3, #13
	ldr	r2, [sp, #52]
	str	r3, [sp, #4]
	movs	r3, #152
	lsls	r3, r3, #7
	adds	r3, #48
	adds	r1, r2, r3
	mov	r0, r8
	movs	r2, #88
	movs	r3, #29
	ldr	r4, [sp, #40]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe00e
.L_08181888:
	movs	r3, #14
	str	r3, [sp, #0]
	movs	r3, #13
	ldr	r2, [sp, #52]
	str	r3, [sp, #4]
	movs	r3, #152
	lsls	r3, r3, #7
	adds	r3, #48
	adds	r1, r2, r3
	mov	r0, r8
	movs	r2, #26
	movs	r3, #29
	ldr	r4, [sp, #44]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xf7c1
	.2byte 0xfe27
	.2byte 0x2001
	movs	r1, #4
	bl	sub_08158ce0
	movs	r1, #240
	ldr	r0, [sp, #52]
	lsls	r1, r1, #7
	adds	r1, #232
	adds	r2, r0, r1
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r2, [sp, #48]
	adds	r2, #1
	str	r2, [sp, #48]
	cmp	r2, #124
	beq.n	.L_081818d2
	b.n	.L_08181340
.L_081818d2:
	ldr	r2, [pc, #40]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #68
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	ldr	r0, [pc, #28]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #68
	b.n	.L_08181904
	movs	r0, r0
	.4byte 0x00001088
	.2byte 0x3001
	.2byte 0x0814
.L_08181904:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #92]
	ldr	r3, [r3, #96]
	sub	sp, #64
	str	r3, [sp, #40]
	mov	sl, r0
	ldr	r3, [r0, #4]
	mov	r8, r1
	cmp	r3, #0
	bne.n	.L_08181940
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #1
	bl	sub_08143a88
	b.n	.L_0818194a
.L_08181940:
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #1
	bl	sub_08143b20
.L_0818194a:
	ldr	r3, [pc, #44]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r3, [pc, #36]
	adds	r2, #50
	strh	r3, [r2, #0]
	ldr	r0, [pc, #36]
	movs	r2, #1
	movs	r3, #1
	ldr	r1, [pc, #32]
	bl	sub_08157cf4
	mov	r2, sl
	ldr	r3, [r2, #4]
	cmp	r3, #0
	bne.n	.L_08181988
	movs	r0, #104
	movs	r1, #35
	bl	sub_081963ec
	b.n	.L_08181990
	.4byte 0x00000100
	.4byte 0x00000610
	.4byte 0x000000f5
	.2byte 0x0000
	.2byte 0x0201
.L_08181988:
	movs	r0, #104
	movs	r1, #39
	bl	sub_081963ec
.L_08181990:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	movs	r2, #239
	lsls	r2, r2, #7
	str	r3, [sp, #32]
	add	r2, r8
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, r8
	movs	r3, #0
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #520]
	bl	sub_080145a8
	movs	r3, #0
	str	r3, [sp, #28]
	str	r3, [sp, #24]
	str	r3, [sp, #20]
	mov	r2, sp
	adds	r2, #52
	mov	r1, sl
	movs	r4, #36
	ldrsh	r0, [r1, r4]
	adds	r1, r2, #0
	str	r2, [sp, #16]
	bl	sub_0815e20c
	movs	r3, #0
	mov	r9, r3
.L_081819d6:
	movs	r4, #0
	mov	r1, r9
	str	r4, [sp, #12]
	cmp	r1, #0
	bne.n	.L_08181a40
	movs	r0, #206
	bl	sub_081c0010
	mov	r2, sl
	ldr	r3, [r2, #4]
	cmp	r3, #0
	bne.n	.L_08181a00
	movs	r3, #128
	mov	r4, r8
	lsls	r3, r3, #14
	str	r3, [r4, #0]
	ldr	r3, [pc, #456]
	str	r3, [r4, #4]
	ldr	r3, [pc, #456]
	str	r3, [r4, #8]
	b.n	.L_08181a0e
.L_08181a00:
	ldr	r3, [pc, #452]
	mov	r1, r8
	str	r3, [r1, #0]
	ldr	r3, [pc, #440]
	str	r3, [r1, #4]
	ldr	r3, [pc, #440]
	str	r3, [r1, #8]
.L_08181a0e:
	movs	r3, #0
	mov	r2, r8
	str	r3, [r2, #20]
	str	r3, [r2, #24]
	movs	r3, #192
	lsls	r3, r3, #13
	str	r3, [sp, #24]
	mov	r4, sl
	ldr	r3, [r4, #4]
	cmp	r3, #0
	bne.n	.L_08181a30
	ldr	r1, [sp, #16]
	movs	r2, #240
	ldr	r3, [r1, #0]
	lsls	r2, r2, #12
	adds	r3, #176
	b.n	.L_08181a3a
.L_08181a30:
	ldr	r4, [sp, #16]
	ldr	r1, [pc, #408]
	ldr	r3, [r4, #0]
	ldr	r2, [pc, #408]
	adds	r3, r3, r1
.L_08181a3a:
	lsls	r3, r3, #16
	str	r3, [sp, #28]
	str	r2, [sp, #20]
.L_08181a40:
	mov	r3, r9
	cmp	r3, #61
	ble.n	.L_08181a6a
	mov	r4, sl
	ldr	r3, [r4, #4]
	cmp	r3, #0
	bne.n	.L_08181a5a
	ldr	r1, [sp, #28]
	movs	r2, #128
	lsls	r2, r2, #10
	adds	r1, r1, r2
	str	r1, [sp, #28]
	b.n	.L_08181a62
.L_08181a5a:
	ldr	r3, [sp, #28]
	ldr	r4, [pc, #372]
	adds	r3, r3, r4
	str	r3, [sp, #28]
.L_08181a62:
	ldr	r1, [sp, #24]
	ldr	r2, [pc, #368]
	adds	r1, r1, r2
	str	r1, [sp, #24]
.L_08181a6a:
	ldr	r4, [sp, #20]
	ldr	r3, [sp, #28]
	subs	r3, r3, r4
	str	r3, [sp, #28]
	lsls	r3, r4, #4
	subs	r3, r3, r4
	lsls	r3, r3, #2
	cmp	r3, #0
	bge.n	.L_08181a7e
	adds	r3, #63
.L_08181a7e:
	ldr	r1, [sp, #28]
	asrs	r3, r3, #6
	asrs	r2, r1, #16
	str	r3, [sp, #20]
	cmp	r2, #120
	ble.n	.L_08181a92
	subs	r2, #120
	str	r2, [sp, #12]
	movs	r2, #120
	b.n	.L_08181a9a
.L_08181a92:
	cmp	r2, #0
	bge.n	.L_08181a9a
	str	r2, [sp, #12]
	movs	r2, #0
.L_08181a9a:
	movs	r3, #128
	lsls	r3, r3, #19
	negs	r2, r2
	adds	r3, #40
	lsls	r2, r2, #8
	str	r2, [r3, #0]
	mov	r3, r9
	subs	r3, #24
	cmp	r3, #39
	bhi.n	.L_08181ad6
	mov	r2, sl
	ldr	r3, [r2, #4]
	movs	r2, #64
	cmp	r3, #0
	beq.n	.L_08181aba
	movs	r2, #5
.L_08181aba:
	ldr	r3, [sp, #12]
	ldr	r4, [sp, #24]
	movs	r1, #59
	str	r1, [sp, #0]
	movs	r1, #89
	adds	r2, r2, r3
	str	r1, [sp, #4]
	asrs	r3, r4, #16
	ldr	r0, [sp, #40]
	ldr	r1, [pc, #268]
	ldr	r4, [sp, #32]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe01f
.L_08181ad6:
	mov	r1, sl
	ldr	r3, [r1, #4]
	cmp	r3, #0
	bne.n	.L_08181aee
	mov	r2, r9
	lsls	r3, r2, #1
	adds	r2, r3, #0
	adds	r2, #16
	cmp	r2, #64
	ble.n	.L_08181afc
	movs	r2, #64
	b.n	.L_08181afc
.L_08181aee:
	mov	r3, r9
	lsls	r2, r3, #1
	movs	r3, #57
	subs	r2, r3, r2
	cmp	r2, #8
	bgt.n	.L_08181afc
	movs	r2, #9
.L_08181afc:
	ldr	r1, [sp, #24]
	ldr	r4, [sp, #12]
	asrs	r3, r1, #16
	movs	r1, #55
	str	r1, [sp, #0]
	movs	r1, #88
	adds	r2, r2, r4
	str	r1, [sp, #4]
	ldr	r0, [sp, #40]
	ldr	r1, [pc, #208]
	ldr	r4, [sp, #32]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2600
	movs	r7, #0
	mov	r5, r8
.L_08181b1c:
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
	cmp	r3, #0
	ble.n	.L_08181c04
	cmp	r3, #1
	bne.n	.L_08181b2e
	str	r7, [r5, #12]
	str	r7, [r5, #16]
.L_08181b2e:
	ldr	r2, [r5, #24]
	cmp	r2, #24
	bne.n	.L_08181b3e
	ldr	r3, [pc, #172]
	str	r3, [r5, #12]
	movs	r3, #128
	lsls	r3, r3, #12
	str	r3, [r5, #16]
.L_08181b3e:
	cmp	r2, #33
	bne.n	.L_08181b78
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r5, #12]
	ldr	r3, [pc, #156]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	str	r3, [r5, #16]
	add	r2, r8
	movs	r3, #4
	str	r3, [r2, #0]
	movs	r1, #0
	movs	r0, #133
	str	r1, [sp, #20]
	bl	sub_081c0010
	mov	r3, sl
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r1, #7
	movs	r3, #0
	bl	sub_0814cd48
	ldr	r2, [r5, #24]
.L_08181b78:
	cmp	r2, #35
	bne.n	.L_08181b80
	str	r7, [r5, #12]
	str	r7, [r5, #16]
.L_08181b80:
	mov	r4, r9
	cmp	r4, #63
	ble.n	.L_08181ba6
	ldr	r1, [pc, #100]
	lsls	r3, r4, #7
	adds	r2, r3, r1
	ldr	r4, [pc, #96]
	ldr	r3, [r5, #16]
	movs	r1, #128
	adds	r3, r3, r4
	lsls	r1, r1, #3
	str	r3, [r5, #16]
	cmp	r2, r1
	ble.n	.L_08181ba0
	movs	r2, #128
	lsls	r2, r2, #3
.L_08181ba0:
	ldr	r3, [r5, #8]
	subs	r3, r3, r2
	str	r3, [r5, #8]
.L_08181ba6:
	ldr	r3, [r5, #24]
	cmp	r3, #8
	ble.n	.L_08181c04
	mov	r2, sl
	ldr	r3, [r2, #4]
	cmp	r3, #0
	bne.n	.L_08181bf4
	ldr	r3, [r5, #0]
	ldr	r2, [r5, #12]
	adds	r3, r3, r2
	b.n	.L_08181bfa
	.4byte 0x08143001
	.4byte 0xffc00000
	.4byte 0xffffe3d0
	.4byte 0xffe00000
	.4byte 0xfffffed0
	.4byte 0xfff10000
	.4byte 0xfffe0000
	.4byte 0xfffb5556
	.4byte 0x020112e8
	.4byte 0x02010000
	.4byte 0xfffc0000
	.4byte 0xfff80000
	.4byte 0xffffe000
	.2byte 0xc000
	.2byte 0xffff
.L_08181bf4:
	.2byte 0x682b
	ldr	r2, [r5, #12]
	subs	r3, r3, r2
.L_08181bfa:
	str	r3, [r5, #0]
	ldr	r3, [r5, #4]
	ldr	r2, [r5, #16]
	adds	r3, r3, r2
	str	r3, [r5, #4]
.L_08181c04:
	adds	r6, #1
	adds	r5, #28
	cmp	r6, #1
	bne.n	.L_08181b1c
	movs	r0, #32
	bl	sub_08014dac
	mov	fp, r0
	movs	r0, #1
	bl	sub_081969f8
	mov	r3, r9
	movs	r4, #4
	subs	r3, #50
	negs	r4, r4
	adds	r7, r0, #0
	movs	r5, #0
	str	r3, [sp, #8]
	cmp	r3, r4
	bne.n	.L_08181c32
	movs	r0, #212
	bl	sub_081c0010
.L_08181c32:
	ldr	r1, [sp, #8]
	cmp	r1, #0
	bne.n	.L_08181c78
	movs	r0, #134
	bl	sub_081180e8
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, r8
	movs	r3, #12
	str	r3, [r2, #0]
	mov	r3, sl
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	movs	r3, #0
	bl	sub_0814cd48
	movs	r1, #128
	lsls	r1, r1, #7
	ldr	r3, [pc, #528]
	ldr	r0, [sp, #40]
	ldr	r2, [pc, #528]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4651
	movs	r4, #36
	ldrsh	r0, [r1, r4]
	movs	r1, #4
	bl	sub_08118088
.L_08181c78:
	mov	r3, r8
	ldr	r2, [r3, #24]
	mov	r3, r9
	subs	r3, #44
	cmp	r3, #13
	bhi.n	.L_08181c8c
	mov	r4, r8
	ldr	r3, [r4, #20]
	adds	r3, #1
	str	r3, [r4, #20]
.L_08181c8c:
	cmp	r2, #0
	bge.n	.L_08181c92
	b.n	.L_08181e20
.L_08181c92:
	movs	r6, #177
	lsls	r6, r6, #8
	mov	r1, r9
	adds	r6, #224
	cmp	r1, #63
	ble.n	.L_08181ca8
	lsls	r3, r1, #9
	movs	r2, #128
	subs	r3, r6, r3
	lsls	r2, r2, #8
	adds	r6, r3, r2
.L_08181ca8:
	cmp	r6, #0
	ble.n	.L_08181d6e
	movs	r3, #9
	str	r5, [r7, #20]
	str	r3, [r7, #0]
	add	r5, sp, #44
	mov	r3, fp
	str	r3, [r7, #12]
	str	r5, [r7, #16]
	bl	sub_08014de4
	mov	r1, r8
	ldr	r4, [sp, #12]
	ldr	r0, [r1, #0]
	lsls	r3, r4, #16
	adds	r0, r0, r3
	movs	r2, #0
	ldr	r1, [r1, #4]
	bl	sub_08015160
	mov	r2, sl
	ldr	r3, [r2, #4]
	cmp	r3, #1
	bne.n	.L_08181ce0
	movs	r0, #128
	lsls	r0, r0, #8
	bl	sub_08015068
.L_08181ce0:
	mov	r3, r8
	ldr	r0, [r3, #8]
	bl	sub_080150e4
	adds	r0, r6, #0
	bl	sub_0801521c
	movs	r3, #5
	strb	r3, [r5, #1]
	ldr	r3, [pc, #392]
	movs	r4, #7
	str	r3, [r5, #4]
	ldr	r3, [pc, #388]
	strb	r4, [r5, #0]
	str	r3, [r7, #8]
	bl	sub_08014e38
	mov	r1, r8
	ldr	r0, [r1, #20]
	lsls	r0, r0, #11
	bl	sub_08002096
	ldr	r2, [pc, #372]
	asrs	r0, r0, #3
	adds	r0, r0, r2
	bl	sub_080150e4
	mov	r1, fp
	movs	r2, #4
	ldr	r0, [pc, #364]
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
	bl	sub_08014ea8
	ldr	r3, [pc, #352]
	mov	r4, r8
	str	r3, [r7, #8]
	movs	r3, #7
	strb	r3, [r5, #0]
	movs	r3, #6
	ldr	r0, [r4, #20]
	strb	r3, [r5, #1]
	ldr	r3, [pc, #340]
	lsls	r0, r0, #11
	str	r3, [r5, #4]
	bl	sub_08002096
	adds	r3, r0, #0
	movs	r0, #128
	asrs	r3, r3, #3
	lsls	r0, r0, #6
	subs	r0, r0, r3
	bl	sub_080150e4
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r0, r2, #0
	ldr	r1, [pc, #312]
	bl	sub_080151e4
	ldr	r0, [pc, #296]
	mov	r1, fp
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
.L_08181d6e:
	ldr	r1, [sp, #8]
	cmp	r1, #7
	bhi.n	.L_08181e20
	mov	r2, r9
	lsls	r3, r2, #13
	ldr	r2, [pc, #284]
	subs	r6, r2, r3
	bl	sub_08014de4
	mov	r4, sl
	ldr	r3, [r4, #4]
	cmp	r3, #0
	bne.n	.L_08181d94
	ldr	r0, [pc, #272]
	movs	r1, #0
	movs	r2, #0
	bl	sub_08015160
	b.n	.L_08181da0
.L_08181d94:
	movs	r0, #128
	lsls	r0, r0, #13
	movs	r1, #0
	movs	r2, #0
	bl	sub_08015160
.L_08181da0:
	adds	r1, r6, #0
	adds	r0, r6, #0
	adds	r2, r6, #0
	bl	sub_080151e4
	mov	r1, sl
	ldr	r3, [r1, #4]
	cmp	r3, #1
	bne.n	.L_08181dba
	movs	r0, #128
	lsls	r0, r0, #8
	bl	sub_08015068
.L_08181dba:
	movs	r3, #6
	str	r3, [r7, #0]
	ldr	r3, [pc, #224]
	add	r2, sp, #44
	str	r3, [r7, #8]
	movs	r3, #5
	strb	r3, [r2, #0]
	strb	r3, [r2, #1]
	ldr	r3, [pc, #216]
	movs	r5, #172
	str	r3, [r2, #4]
	lsls	r5, r5, #7
	movs	r6, #0
	adds	r5, #80
.L_08181dd6:
	bl	sub_08014e38
	adds	r0, r5, #0
	bl	sub_080150e4
	movs	r1, #1
	ands	r1, r6
	movs	r2, #192
	lsls	r2, r2, #8
	lsls	r1, r1, #15
	adds	r1, r1, r2
	movs	r0, #128
	movs	r2, #128
	lsls	r2, r2, #8
	lsls	r0, r0, #7
	bl	sub_080151e4
	movs	r0, #128
	lsls	r0, r0, #9
	bl	sub_0801521c
	mov	r1, fp
	movs	r2, #4
	ldr	r0, [pc, #160]
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
	bl	sub_08014ea8
	movs	r3, #128
	lsls	r3, r3, #7
	adds	r6, #1
	adds	r5, r5, r3
	cmp	r6, #4
	bne.n	.L_08181dd6
.L_08181e20:
	adds	r0, r7, #0
	bl	sub_08013164
	mov	r0, fp
	bl	sub_08013164
	movs	r1, #8
	movs	r0, #8
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	add	r2, r8
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r4, #1
	add	r9, r4
	mov	r1, r9
	cmp	r1, #90
	beq.n	.L_08181e56
	b.n	.L_081819d6
.L_08181e56:
	ldr	r0, [pc, #84]
	bl	sub_08014644
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #64
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.4byte 0x0201476b
	.4byte 0x0819962c
	.4byte 0xffffe000
	.4byte 0x08199650
	.4byte 0x08199608
	.4byte 0x0201276b
	.4byte 0x00015f90
	.4byte 0x00078e20
	.4byte 0xfff00000
	.4byte 0x08199244
	.4byte 0x0201576b
	.4byte 0x081991f0
	.2byte 0x3001
	.2byte 0x0814
	push	{lr}
	movs	r1, #0
	bl	sub_08181ed4
	pop	{pc}
