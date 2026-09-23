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
	.set sub_080156e8, 0x080156e8
	.set sub_0801587c, 0x0801587c
	.set sub_08020010, 0x08020010
	.set sub_08020048, 0x08020048
	.set sub_08020090, 0x08020090
	.set sub_08020098, 0x08020098
	.set sub_08118010, 0x08118010
	.set sub_08118040, 0x08118040
	.set sub_08118088, 0x08118088
	.set sub_08118098, 0x08118098
	.set sub_081180e8, 0x081180e8
	.set sub_081180f0, 0x081180f0
	.set sub_08138058, 0x08138058
	.set sub_08138086, 0x08138086
	.set sub_0813ba50, 0x0813ba50
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143a88, 0x08143a88
	.set sub_08143bb8, 0x08143bb8
	.set sub_08144aac, 0x08144aac
	.set sub_081496c8, 0x081496c8
	.set sub_0814cd48, 0x0814cd48
	.set sub_0815585c, 0x0815585c
	.set sub_08157530, 0x08157530
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_08158d68, 0x08158d68
	.set sub_0815b290, 0x0815b290
	.set sub_0815b3b0, 0x0815b3b0
	.set sub_0815b410, 0x0815b410
	.set sub_0815b434, 0x0815b434
	.set sub_0815e1ec, 0x0815e1ec
	.set sub_0815e1fc, 0x0815e1fc
	.set sub_0815e20c, 0x0815e20c
	.set sub_0815e21c, 0x0815e21c
	.set sub_0815e22c, 0x0815e22c
	.set sub_0815f000, 0x0815f000
	.set sub_0815f0a0, 0x0815f0a0
	.set sub_08163c2c, 0x08163c2c
	.set sub_08164b2c, 0x08164b2c
	.set sub_0816ae40, 0x0816ae40
	.set sub_08174094, 0x08174094
	.set sub_08174248, 0x08174248
	.set sub_081744f6, 0x081744f6
	.set sub_081745d2, 0x081745d2
	.set sub_081747f2, 0x081747f2
	.set sub_08175508, 0x08175508
	.set sub_08175b22, 0x08175b22
	.set sub_08175b76, 0x08175b76
	.set sub_08176642, 0x08176642
	.set sub_08176664, 0x08176664
	.set sub_08176c8c, 0x08176c8c
	.set sub_081771e0, 0x081771e0
	.set sub_081773b6, 0x081773b6
	.set sub_081776e4, 0x081776e4
	.set sub_081776f8, 0x081776f8
	.set sub_081784a6, 0x081784a6
	.set sub_081791d4, 0x081791d4
	.set sub_08179292, 0x08179292
	.set sub_081796bc, 0x081796bc
	.set sub_0817970e, 0x0817970e
	.set sub_08179a10, 0x08179a10
	.set sub_0818caa8, 0x0818caa8
	.set sub_081963ec, 0x081963ec
	.set sub_08196958, 0x08196958
	.set sub_081969f8, 0x081969f8
	.set sub_08196a7c, 0x08196a7c
	.set sub_081c0010, 0x081c0010
	.global Func_08173aac
	.thumb_func
Func_08173aac:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #272
	str	r0, [sp, #56]
	str	r1, [sp, #52]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #92]
	str	r0, [sp, #48]
	movs	r0, #0
	ldr	r1, [r3, #96]
	str	r1, [sp, #44]
	ldr	r2, [r3, #100]
	str	r2, [sp, #28]
	ldr	r3, [r3, #48]
	str	r3, [sp, #24]
	bl	sub_081435e0
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [sp, #52]
	cmp	r3, #0
	bne.n	.L_08173b48
	ldr	r4, [sp, #56]
	movs	r3, #1
	ldr	r1, [r4, #4]
	add	r2, sp, #260
	eors	r1, r3
	lsls	r1, r1, #4
	movs	r3, #35
	adds	r0, r4, #0
	orrs	r1, r3
	add	r3, sp, #248
	bl	sub_0815585c
	movs	r6, #216
	ldr	r5, [sp, #48]
	lsls	r6, r6, #7
	adds	r6, #192
	movs	r7, #224
	ldr	r0, [pc, #8]
	adds	r1, r5, r6
	movs	r2, #1
	b.n	.L_08173b1c
	.4byte 0x00001010
	.2byte 0x0153
	.2byte 0x0000
.L_08173b1c:
	movs	r3, #1
	lsls	r7, r7, #3
	bl	sub_08157cf4
	ldr	r0, [pc, #28]
	movs	r2, #0
	movs	r3, #0
	adds	r1, r5, r7
	bl	sub_08157cf4
	movs	r2, #128
	ldr	r3, [pc, #12]
	lsls	r2, r2, #19
	adds	r2, #12
	movs	r0, #84
	strh	r3, [r2, #0]
	str	r0, [sp, #32]
	b.n	sub_08174094
	.4byte 0x00000785
	.2byte 0x00d8
	.2byte 0x0000
.L_08173b48:
	ldr	r1, [sp, #52]
	cmp	r1, #1
	bne.n	.L_08173ba6
	ldr	r2, [sp, #56]
	ldr	r3, [sp, #52]
	ldr	r1, [r2, #4]
	ldr	r0, [sp, #56]
	eors	r1, r3
	lsls	r1, r1, #4
	movs	r3, #33
	add	r2, sp, #260
	orrs	r1, r3
	add	r3, sp, #248
	bl	sub_0815585c
	ldr	r4, [sp, #48]
	movs	r5, #224
	lsls	r5, r5, #3
	adds	r1, r4, r5
	ldr	r0, [pc, #524]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	movs	r7, #216
	ldr	r6, [sp, #48]
	lsls	r7, r7, #7
	adds	r7, #192
	adds	r1, r6, r7
	movs	r2, #1
	movs	r3, #1
	ldr	r0, [pc, #504]
	bl	sub_08157cf4
	ldr	r0, [pc, #500]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #496]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2030
	str	r0, [sp, #32]
	b.n	sub_08174094
.L_08173ba6:
	ldr	r1, [sp, #52]
	cmp	r1, #4
	bne.n	.L_08173c38
	ldr	r2, [sp, #56]
	movs	r3, #1
	ldr	r1, [r2, #4]
	ldr	r0, [sp, #56]
	eors	r1, r3
	movs	r3, #144
	lsls	r3, r3, #1
	lsls	r1, r1, #4
	add	r2, sp, #260
	orrs	r1, r3
	add	r3, sp, #248
	bl	sub_0815585c
	movs	r4, #216
	ldr	r3, [sp, #48]
	lsls	r4, r4, #7
	adds	r4, #192
	adds	r1, r3, r4
	ldr	r0, [pc, #440]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r6, [sp, #48]
	movs	r5, #184
	lsls	r5, r5, #5
	adds	r1, r6, r5
	ldr	r0, [pc, #428]
	movs	r2, #1
	movs	r3, #0
	movs	r7, #224
	bl	sub_08157cf4
	lsls	r7, r7, #3
	movs	r1, #128
	adds	r0, r6, r7
	ldr	r3, [pc, #412]
	lsls	r1, r1, #5
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x20a0
	lsls	r0, r0, #4
	ldr	r4, [sp, #48]
	adds	r0, #12
	movs	r6, #0
	movs	r7, #0
	mov	ip, r0
.L_08173c0a:
	mov	r2, ip
	adds	r1, r4, r2
	ldr	r2, [sp, #48]
	adds	r3, r7, r6
	lsls	r3, r3, #3
	adds	r3, r3, r2
	movs	r0, #0
	adds	r2, r3, r5
.L_08173c1a:
	ldrb	r3, [r2, #0]
	adds	r0, #1
	strb	r3, [r1, #0]
	adds	r2, #1
	adds	r1, #1
	cmp	r0, #40
	bne.n	.L_08173c1a
	adds	r6, #1
	adds	r7, #4
	adds	r4, #64
	cmp	r6, #40
	bne.n	.L_08173c0a
	movs	r3, #42
	str	r3, [sp, #32]
	b.n	sub_08174094
.L_08173c38:
	ldr	r4, [sp, #52]
	cmp	r4, #6
	bne.n	.L_08173c86
	ldr	r5, [sp, #56]
	movs	r3, #1
	ldr	r1, [r5, #4]
	add	r2, sp, #260
	eors	r1, r3
	lsls	r1, r1, #4
	movs	r3, #131
	orrs	r1, r3
	adds	r0, r5, #0
	add	r3, sp, #248
	bl	sub_0815585c
	movs	r7, #216
	ldr	r6, [sp, #48]
	lsls	r7, r7, #7
	adds	r7, #192
	ldr	r0, [pc, #312]
	adds	r1, r6, r7
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r2, #224
	lsls	r2, r2, #3
	ldr	r0, [pc, #300]
	adds	r1, r6, r2
	movs	r3, #0
	movs	r2, #0
	bl	sub_08157cf4
	movs	r3, #80
	movs	r0, #84
	str	r3, [sp, #32]
	bl	sub_081c0010
	b.n	sub_08174094
.L_08173c86:
	ldr	r4, [sp, #52]
	cmp	r4, #11
	bne.n	.L_08173cb8
	ldr	r5, [sp, #48]
	movs	r6, #224
	lsls	r6, r6, #3
	adds	r1, r5, r6
	movs	r2, #0
	movs	r3, #0
	ldr	r0, [pc, #256]
	bl	sub_08157cf4
	ldr	r0, [pc, #256]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #220]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2740
	str	r7, [sp, #32]
	b.n	sub_08174094
.L_08173cb8:
	ldr	r0, [sp, #52]
	cmp	r0, #14
	bne.n	.L_08173d04
	ldr	r2, [sp, #48]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #212]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r0, [pc, #172]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #172]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x25ee
	ldr	r4, [sp, #48]
	lsls	r5, r5, #7
	movs	r6, #238
	adds	r5, #180
	lsls	r6, r6, #7
	adds	r2, r4, r5
	movs	r3, #24
	adds	r6, #184
	str	r3, [r2, #0]
	movs	r7, #64
	adds	r2, r4, r6
	movs	r3, #0
	str	r3, [r2, #0]
	str	r7, [sp, #32]
	b.n	sub_08174094
.L_08173d04:
	ldr	r0, [sp, #52]
	cmp	r0, #13
	bne.n	.L_08173d22
	ldr	r2, [sp, #48]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #144]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r4, #40
	str	r4, [sp, #32]
	b.n	sub_08174094
.L_08173d22:
	ldr	r5, [sp, #52]
	cmp	r5, #12
	beq.n	.L_08173d2e
	ldr	r5, [sp, #52]
	cmp	r5, #15
	bne.n	.L_08173db4
.L_08173d2e:
	ldr	r0, [pc, #120]
	ldr	r1, [pc, #120]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r6, [sp, #48]
	movs	r7, #142
	lsls	r7, r7, #7
	adds	r5, r6, r7
	ldr	r0, [pc, #108]
	adds	r1, r5, #0
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r0, #224
	lsls	r0, r0, #3
	adds	r1, r6, r0
	movs	r2, #64
	movs	r3, #64
	adds	r0, r5, #0
	bl	sub_0816ae40
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #180
	adds	r2, r6, r1
	movs	r3, #24
	str	r3, [r2, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #184
	adds	r2, r6, r3
	movs	r4, #48
	movs	r3, #0
	str	r3, [r2, #0]
	str	r4, [sp, #32]
	b.n	sub_08174094
	.4byte 0x000000d4
	.4byte 0x00000155
	.4byte 0x00000148
	.4byte 0x03000730
	.4byte 0x00000157
	.4byte 0x00000156
	.4byte 0x03000258
	.4byte 0x00000153
	.4byte 0x000000d7
	.4byte 0x00000151
	.4byte 0x000000f4
	.4byte 0x000000c2
	.4byte 0x02010000
	.2byte 0x00d6
	.2byte 0x0000
.L_08173db4:
	ldr	r5, [sp, #52]
	cmp	r5, #5
	bne.n	.L_08173e46
	ldr	r6, [sp, #56]
	movs	r3, #226
	ldr	r1, [r6, #4]
	add	r2, sp, #260
	lsls	r1, r1, #4
	orrs	r1, r3
	adds	r0, r6, #0
	add	r3, sp, #248
	bl	sub_0815585c
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #12
	strh	r3, [r2, #0]
	ldr	r7, [sp, #48]
	movs	r2, #216
	lsls	r2, r2, #7
	adds	r2, #192
	adds	r1, r7, r2
	ldr	r0, [pc, #44]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r7, r3
	ldr	r0, [pc, #32]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r4, #142
	lsls	r4, r4, #7
	adds	r1, r7, r4
	ldr	r0, [pc, #20]
	movs	r2, #0
	movs	r3, #0
	b.n	.L_08173e1c
	movs	r0, r0
	.4byte 0x00000785
	.4byte 0x00000151
	.4byte 0x000000c1
	.2byte 0x00c9
	.2byte 0x0000
.L_08173e1c:
	bl	sub_08157cf4
	ldr	r1, [pc, #280]
	movs	r2, #0
	movs	r3, #0
	ldr	r0, [pc, #280]
	bl	sub_08157cf4
	ldr	r0, [pc, #276]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #272]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2552
	str	r5, [sp, #32]
	b.n	sub_08174094
.L_08173e46:
	ldr	r6, [sp, #52]
	cmp	r6, #10
	bne.n	.L_08173ecc
	movs	r2, #1
	ldr	r0, [pc, #252]
	ldr	r1, [pc, #232]
	movs	r3, #1
	bl	sub_08157cf4
	ldr	r7, [sp, #48]
	movs	r1, #224
	lsls	r1, r1, #3
	adds	r0, r7, r1
	movs	r1, #128
	ldr	r3, [pc, #236]
	lsls	r1, r1, #7
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x23e0
	lsls	r3, r3, #3
	movs	r2, #0
	adds	r3, #4
	mov	r8, r2
	movs	r7, #0
	mov	ip, r3
.L_08173e78:
	ldr	r0, [sp, #48]
	mov	r4, r8
	lsls	r3, r4, #12
	movs	r6, #0
	lsls	r5, r7, #6
	adds	r4, r3, r0
.L_08173e84:
	ldr	r3, [pc, #180]
	mov	r2, ip
	adds	r1, r4, r2
	movs	r0, #0
	adds	r2, r5, r3
.L_08173e8e:
	ldrb	r3, [r2, #0]
	adds	r0, #1
	strb	r3, [r1, #0]
	adds	r2, #1
	adds	r1, #1
	cmp	r0, #24
	bne.n	.L_08173e8e
	adds	r6, #1
	adds	r5, #24
	adds	r4, #32
	cmp	r6, #120
	bne.n	.L_08173e84
	movs	r4, #1
	add	r8, r4
	mov	r5, r8
	adds	r7, #45
	cmp	r5, #4
	bne.n	.L_08173e78
	ldr	r0, [pc, #160]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #136]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x263c
	str	r6, [sp, #32]
	b.n	sub_08174094
.L_08173ecc:
	ldr	r7, [sp, #52]
	cmp	r7, #7
	bne.n	.L_08173efe
	ldr	r2, [sp, #48]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #124]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r0, [pc, #116]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #88]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x244c
	str	r4, [sp, #32]
	b.n	sub_08174094
.L_08173efe:
	ldr	r5, [sp, #52]
	cmp	r5, #8
	bne.n	.L_08173f64
	ldr	r6, [sp, #48]
	movs	r7, #142
	lsls	r7, r7, #7
	adds	r5, r6, r7
	ldr	r0, [pc, #80]
	adds	r1, r5, #0
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r0, #224
	lsls	r0, r0, #3
	adds	r1, r6, r0
	movs	r2, #64
	movs	r3, #64
	adds	r0, r5, #0
	bl	sub_0816ae40
	movs	r2, #128
	ldr	r3, [pc, #12]
	lsls	r2, r2, #19
	adds	r2, #12
	movs	r1, #16
	strh	r3, [r2, #0]
	str	r1, [sp, #32]
	b.n	sub_08174094
	.4byte 0x00000785
	.4byte 0x02010000
	.4byte 0x000000c2
	.4byte 0x00000148
	.4byte 0x03000730
	.4byte 0x00000188
	.4byte 0x03000258
	.4byte 0x00000149
	.4byte 0x000000b4
	.4byte 0x00000155
	.2byte 0x00d6
	.2byte 0x0000
.L_08173f64:
	ldr	r2, [sp, #52]
	cmp	r2, #9
	bne.n	.L_08174018
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #12
	strh	r3, [r2, #0]
	ldr	r3, [sp, #48]
	movs	r4, #222
	lsls	r4, r4, #6
	adds	r1, r3, r4
	ldr	r0, [pc, #44]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r6, [sp, #48]
	movs	r7, #224
	lsls	r7, r7, #3
	adds	r5, r6, r7
	ldr	r0, [pc, #32]
	adds	r1, r5, #0
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	movs	r0, #0
	movs	r1, #216
	mov	r8, r0
	lsls	r1, r1, #5
	movs	r0, #63
	b.n	.L_08173fb4
	movs	r0, r0
	.4byte 0x00000785
	.4byte 0x000000c2
	.2byte 0x0161
	.2byte 0x0000
.L_08173fb4:
	ldrb	r2, [r5, #0]
	lsls	r3, r2, #1
	strb	r3, [r5, #0]
	movs	r3, #252
	lsls	r2, r2, #25
	lsls	r3, r3, #22
	cmp	r2, r3
	bls.n	.L_08173fc6
	strb	r0, [r5, #0]
.L_08173fc6:
	movs	r4, #1
	add	r8, r4
	adds	r5, #1
	cmp	r8, r1
	bne.n	.L_08173fb4
	ldr	r5, [sp, #56]
	ldr	r3, [r5, #24]
	cmp	r3, #2
	bne.n	.L_08173fee
	ldr	r0, [pc, #528]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #524]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xe009
.L_08173fee:
	ldr	r0, [pc, #516]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #500]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9e0e
	movs	r2, #10
	ldr	r0, [r6, #8]
	movs	r7, #36
	ldrsh	r1, [r6, r7]
	movs	r3, #80
	bl	sub_08157530
	movs	r0, #41
	str	r0, [sp, #32]
	b.n	sub_08174094
.L_08174018:
	ldr	r1, [sp, #52]
	cmp	r1, #2
	bne.n	.L_0817403c
	ldr	r2, [sp, #56]
	movs	r3, #1
	ldr	r1, [r2, #4]
	ldr	r0, [sp, #56]
	eors	r1, r3
	ldr	r3, [sp, #52]
	lsls	r1, r1, #4
	orrs	r1, r3
	add	r2, sp, #260
	add	r3, sp, #248
	bl	sub_0815585c
	movs	r4, #76
	str	r4, [sp, #32]
	b.n	.L_0817405a
.L_0817403c:
	ldr	r5, [sp, #48]
	movs	r6, #238
	lsls	r6, r6, #7
	movs	r7, #238
	adds	r6, #180
	lsls	r7, r7, #7
	adds	r2, r5, r6
	movs	r3, #24
	adds	r7, #184
	str	r3, [r2, #0]
	movs	r0, #70
	adds	r2, r5, r7
	movs	r3, #0
	str	r3, [r2, #0]
	str	r0, [sp, #32]
.L_0817405a:
	ldr	r2, [sp, #48]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r1, r2, r3
	ldr	r0, [pc, #404]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r5, #216
	ldr	r4, [sp, #48]
	lsls	r5, r5, #7
	adds	r5, #192
	adds	r1, r4, r5
	movs	r2, #1
	movs	r3, #1
	ldr	r0, [pc, #384]
	bl	sub_08157cf4
	ldr	r0, [pc, #360]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #356]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x485a
	ldr	r1, [sp, #28]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r6, [sp, #48]
	movs	r7, #239
	movs	r0, #238
	lsls	r7, r7, #7
	lsls	r0, r0, #7
	adds	r2, r6, r7
	movs	r3, #2
	adds	r0, #132
	str	r3, [r2, #0]
	adds	r2, r6, r0
	movs	r3, #50
	str	r3, [r2, #0]
	ldr	r1, [sp, #52]
	cmp	r1, #0
	beq.n	.L_081740c2
	cmp	r1, #6
	bne.n	.L_081740cc
.L_081740c2:
	movs	r1, #200
	ldr	r0, [pc, #316]
	lsls	r1, r1, #4
	bl	sub_080145a8
.L_081740cc:
	movs	r1, #200
	ldr	r0, [pc, #312]
	lsls	r1, r1, #4
	bl	sub_080145a8
	ldr	r2, [sp, #52]
	cmp	r2, #2
	beq.n	.L_081740e4
	cmp	r2, #7
	beq.n	.L_081740e4
	cmp	r2, #4
	bne.n	.L_081740ee
.L_081740e4:
	ldr	r4, [sp, #56]
	ldr	r3, [r4, #4]
	cmp	r3, #0
	bne.n	.L_0817410a
	b.n	.L_08174100
.L_081740ee:
	ldr	r5, [sp, #52]
	cmp	r5, #3
	beq.n	.L_081740f8
	cmp	r5, #5
	bne.n	.L_08174114
.L_081740f8:
	ldr	r6, [sp, #56]
	ldr	r3, [r6, #4]
	cmp	r3, #1
	bne.n	.L_0817410a
.L_08174100:
	movs	r0, #104
	movs	r1, #7
	bl	sub_081963ec
	b.n	.L_0817412e
.L_0817410a:
	movs	r0, #104
	movs	r1, #3
	bl	sub_081963ec
	b.n	.L_0817412e
.L_08174114:
	ldr	r7, [sp, #56]
	ldr	r3, [r7, #4]
	cmp	r3, #0
	bne.n	.L_08174126
	movs	r0, #104
	movs	r1, #23
	bl	sub_081963ec
	b.n	.L_0817412e
.L_08174126:
	movs	r0, #104
	movs	r1, #19
	bl	sub_081963ec
.L_0817412e:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	ldr	r1, [sp, #32]
	movs	r0, #0
	str	r3, [sp, #36]
	mov	fp, r0
	cmp	r1, #0
	bne.n	.L_08174144
	.2byte 0xf001
	.2byte 0xfef5
.L_08174144:
	mov	r2, sp
	adds	r2, #60
	str	r2, [sp, #8]
.L_0817414a:
	ldr	r3, [sp, #52]
	cmp	r3, #0
	bne.n	.L_08174174
	ldr	r4, [sp, #48]
	movs	r6, #225
	lsls	r6, r6, #7
	movs	r7, #0
	adds	r5, r4, r6
	mov	r8, r7
	movs	r6, #31
.L_0817415e:
	bl	sub_08014878
	ands	r0, r6
	lsls	r0, r0, #4
	stmia	r5!, {r0}
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	cmp	r1, #160
	bne.n	.L_0817415e
	b.n	sub_08174248
.L_08174174:
	ldr	r2, [sp, #52]
	cmp	r2, #6
	bne.n	.L_081741b2
	ldr	r3, [sp, #48]
	movs	r4, #225
	movs	r5, #0
	lsls	r4, r4, #7
	mov	r7, fp
	mov	r8, r5
	adds	r6, r3, r4
	lsls	r5, r7, #11
.L_0817418a:
	adds	r0, r5, #0
	bl	sub_08002096
	lsls	r2, r0, #1
	adds	r2, r2, r0
	movs	r3, #192
	lsls	r2, r2, #1
	lsls	r3, r3, #11
	movs	r1, #1
	subs	r3, r3, r2
	movs	r0, #128
	add	r8, r1
	asrs	r3, r3, #10
	lsls	r0, r0, #4
	mov	r2, r8
	stmia	r6!, {r3}
	adds	r5, r5, r0
	cmp	r2, #160
	bne.n	.L_0817418a
	b.n	sub_08174248
.L_081741b2:
	ldr	r3, [sp, #52]
	cmp	r3, #1
	bne.n	sub_08174248
	mov	r4, fp
	cmp	r4, #31
	ble.n	sub_08174248
	movs	r0, #160
	ldr	r4, [pc, #36]
	lsls	r0, r0, #19
	movs	r5, #0
	adds	r0, #2
	mov	r8, r5
.L_081741ca:
	ldrh	r3, [r0, #0]
	movs	r1, #31
	ands	r1, r3
	lsls	r3, r3, #16
	lsrs	r2, r3, #21
	mov	r6, fp
	lsrs	r3, r3, #26
	ands	r2, r4
	ands	r3, r4
	cmp	r6, #40
	ble.n	.L_0817420c
	adds	r1, #4
	adds	r2, #4
	adds	r3, #4
	b.n	.L_08174212
	.4byte 0x0000001f
	.4byte 0x00000148
	.4byte 0x03000730
	.4byte 0x00000184
	.4byte 0x000000b4
	.4byte 0x00000151
	.4byte 0x00000134
	.4byte 0x08152475
	.2byte 0x3001
	.2byte 0x0814
.L_0817420c:
	adds	r1, #2
	adds	r2, #2
	adds	r3, #2
.L_08174212:
	cmp	r1, #31
	ble.n	.L_08174218
	movs	r1, #31
.L_08174218:
	cmp	r2, #31
	ble.n	.L_0817421e
	movs	r2, #31
.L_0817421e:
	cmp	r3, #31
	ble.n	.L_08174224
	movs	r3, #31
.L_08174224:
	lsls	r3, r3, #10
	lsls	r2, r2, #5
	movs	r7, #1
	orrs	r3, r2
	add	r8, r7
	orrs	r3, r1
	mov	r1, r8
	strh	r3, [r0, #0]
	adds	r0, #2
	cmp	r1, #63
	bne.n	.L_081741ca
	movs	r1, #128
	ldr	r3, [pc, #576]
	ldr	r0, [sp, #44]
	lsls	r1, r1, #7
	ldr	r2, [pc, #576]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9a0d
	cmp	r2, #3
	beq.n	.L_08174250
	b.n	.L_081743f6
.L_08174250:
	ldr	r3, [sp, #56]
	add	r6, sp, #224
	ldr	r0, [r3, #8]
	adds	r1, r6, #0
	bl	sub_0815e20c
	movs	r7, #236
	ldr	r5, [sp, #56]
	add	r7, sp
	mov	sl, r7
	movs	r4, #36
	ldrsh	r0, [r5, r4]
	mov	r1, sl
	bl	sub_0815e20c
	ldr	r3, [r6, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r6, #0]
	ldr	r3, [r7, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r0, r3, #1
	mov	r3, fp
	subs	r3, #32
	str	r0, [r7, #0]
	cmp	r3, #8
	bhi.n	.L_081742fe
	mov	r1, fp
	subs	r1, #31
	cmp	r1, #8
	ble.n	.L_08174294
	movs	r1, #8
.L_08174294:
	ldr	r5, [pc, #496]
	ldr	r2, [r6, #0]
	ldrsb	r3, [r5, r1]
	subs	r0, r0, r2
	muls	r3, r0
	cmp	r3, #0
	bge.n	.L_081742a4
	adds	r3, #15
.L_081742a4:
	asrs	r3, r3, #4
	adds	r4, r2, r3
	subs	r3, r1, #1
	ldrsb	r3, [r5, r3]
	muls	r3, r0
	cmp	r3, #0
	bge.n	.L_081742b4
	adds	r3, #15
.L_081742b4:
	asrs	r3, r3, #4
	adds	r7, r2, r3
	movs	r0, #0
	subs	r4, r4, r7
	mov	r8, r0
	mov	r9, r4
	movs	r5, #0
.L_081742c2:
	adds	r0, r5, #0
	movs	r1, #3
	bl	sub_08002054
	ldr	r3, [r6, #4]
	movs	r4, #216
	subs	r3, #20
	mov	ip, r3
	ldr	r3, [sp, #48]
	adds	r2, r0, #0
	movs	r1, #20
	lsls	r4, r4, #7
	str	r1, [sp, #0]
	adds	r4, #192
	movs	r1, #40
	adds	r2, r7, r2
	str	r1, [sp, #4]
	subs	r2, #10
	adds	r1, r3, r4
	ldr	r0, [sp, #44]
	mov	r3, ip
	ldr	r4, [sp, #36]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2001
	add	r8, r0
	mov	r1, r8
	add	r5, r9
	cmp	r1, #4
	bne.n	.L_081742c2
.L_081742fe:
	mov	r2, fp
	cmp	r2, #37
	bgt.n	.L_08174306
	b.n	sub_081747f2
.L_08174306:
	cmp	r2, #38
	bne.n	.L_0817439c
	ldr	r7, [sp, #48]
	movs	r3, #0
	mov	r8, r3
.L_08174310:
	mov	r4, sl
	ldr	r3, [r4, #0]
	movs	r5, #128
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	lsls	r5, r5, #1
	ldr	r3, [r4, #4]
	adds	r5, #255
	lsls	r3, r3, #16
	str	r3, [r7, #4]
	bl	sub_08014878
	adds	r6, r0, #0
	bl	sub_08014878
	ands	r5, r0
	movs	r0, #128
	lsls	r0, r0, #1
	adds	r5, r5, r0
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #8
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #7
	str	r3, [r7, #16]
	bl	sub_08014878
	movs	r3, #15
	movs	r1, #1
	ands	r3, r0
	add	r8, r1
	adds	r3, #16
	mov	r2, r8
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r2, #63
	bne.n	.L_08174310
	movs	r0, #134
	bl	sub_081180e8
	ldr	r4, [sp, #56]
	movs	r1, #4
	movs	r3, #36
	ldrsh	r0, [r4, r3]
	bl	sub_08118088
	movs	r6, #238
	ldr	r5, [sp, #48]
	lsls	r6, r6, #7
	adds	r6, #168
	movs	r2, #8
	adds	r3, r5, r6
	str	r2, [r3, #0]
	ldr	r1, [sp, #56]
	movs	r3, #0
	movs	r7, #36
	ldrsh	r0, [r1, r7]
	str	r2, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	bl	sub_0814cd48
.L_0817439c:
	ldr	r6, [pc, #236]
	ldr	r5, [sp, #48]
	movs	r2, #0
	mov	r8, r2
.L_081743a4:
	ldr	r0, [r5, #24]
	cmp	r0, #0
	blt.n	.L_081743e8
	asrs	r0, r0, #3
	adds	r0, #2
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r6, r3]
	ldr	r3, [sp, #28]
	movs	r7, #2
	ldrsh	r2, [r5, r7]
	adds	r1, r3, r1
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	movs	r7, #6
	ldrsh	r3, [r5, r7]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #44]
	ldr	r4, [sp, #36]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2280
	adds	r0, r5, #0
	movs	r1, #60
	lsls	r2, r2, #3
	bl	sub_08138086
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L_081743e8:
	movs	r7, #1
	add	r8, r7
	mov	r0, r8
	adds	r5, #28
	cmp	r0, #63
	bne.n	.L_081743a4
	b.n	sub_081747f2
.L_081743f6:
	ldr	r1, [sp, #52]
	cmp	r1, #6
	bne.n	.L_08174472
	mov	r2, fp
	lsls	r6, r2, #11
	adds	r0, r6, #0
	bl	sub_08002096
	add	r5, sp, #260
	ldr	r3, [r5, #0]
	lsls	r2, r0, #2
	adds	r2, r2, r0
	lsrs	r1, r3, #31
	adds	r3, r3, r1
	lsls	r2, r2, #2
	asrs	r3, r3, #1
	asrs	r2, r2, #16
	adds	r2, r2, r3
	adds	r0, r6, #0
	adds	r7, r2, #0
	bl	sub_08002090
	ldr	r3, [r5, #4]
	lsls	r0, r0, #2
	asrs	r0, r0, #16
	adds	r0, r0, r3
	adds	r4, r0, #0
	mov	r3, fp
	subs	r7, #10
	subs	r4, #24
	cmp	r3, #44
	ble.n	.L_0817443e
	lsls	r0, r3, #1
	subs	r3, r4, r0
	adds	r4, r3, #0
	adds	r4, #88
.L_0817443e:
	ldr	r5, [sp, #48]
	movs	r6, #216
	movs	r3, #20
	lsls	r6, r6, #7
	str	r3, [sp, #0]
	adds	r6, #192
	movs	r3, #40
	adds	r2, r7, #0
	str	r3, [sp, #4]
	adds	r1, r5, r6
	adds	r3, r4, #0
	ldr	r0, [sp, #44]
	ldr	r7, [sp, #36]
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x21aa
	movs	r2, #170
	lsls	r1, r1, #8
	lsls	r2, r2, #7
	mov	r0, fp
	adds	r1, #171
	adds	r2, #85
	movs	r3, #0
	bl	sub_081496c8
	b.n	sub_081747f2
.L_08174472:
	ldr	r0, [sp, #52]
	cmp	r0, #2
	beq.n	.L_08174490
	cmp	r0, #7
	beq.n	.L_0817447e
	b.n	.L_081745e8
.L_0817447e:
	b.n	sub_081744f6
	.4byte 0x03000260
	.4byte 0x01010101
	.4byte 0x08198c28
	.2byte 0x7410
	.2byte 0x0819
.L_08174490:
	add	r1, sp, #260
	ldr	r3, [r1, #0]
	mov	r5, fp
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	adds	r7, r3, #0
	ldr	r3, [r1, #4]
	subs	r7, #10
	adds	r6, r3, #0
	subs	r6, #20
	cmp	r5, #12
	ble.n	.L_081744ac
	movs	r5, #12
.L_081744ac:
	mov	r1, fp
	lsls	r1, r1, #9
	mov	r9, r1
	mov	r0, r9
	bl	sub_08002096
	adds	r3, r5, #0
	muls	r3, r0
	lsls	r3, r3, #1
	asrs	r3, r3, #16
	mov	r0, r9
	subs	r7, r7, r3
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #16
	subs	r3, r3, r5
	mov	r2, fp
	subs	r6, r6, r3
	cmp	r2, #61
	bgt.n	sub_081744f6
	movs	r3, #20
	str	r3, [sp, #0]
	movs	r3, #40
	str	r3, [sp, #4]
	movs	r4, #216
	ldr	r3, [sp, #48]
	lsls	r4, r4, #7
	adds	r4, #192
	adds	r1, r3, r4
	ldr	r0, [sp, #44]
	adds	r2, r7, #0
	adds	r3, r6, #0
	ldr	r5, [sp, #36]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x465e
	cmp	r6, #0
	bne.n	.L_08174502
	movs	r0, #142
	bl	sub_081c0010
.L_08174502:
	mov	r7, fp
	cmp	r7, #62
	bne.n	.L_0817450e
	movs	r0, #139
	bl	sub_081c0010
.L_0817450e:
	mov	r0, fp
	cmp	r0, #0
	bne.n	.L_08174542
	ldr	r5, [sp, #48]
	movs	r1, #0
	mov	r8, r1
.L_0817451a:
	bl	sub_08014878
	str	r0, [r5, #8]
	bl	sub_08014878
	movs	r3, #63
	ands	r3, r0
	adds	r3, #64
	str	r3, [r5, #20]
	bl	sub_08014878
	mov	r3, r8
	movs	r2, #1
	adds	r3, #32
	add	r8, r2
	str	r3, [r5, #24]
	mov	r3, r8
	adds	r5, #28
	cmp	r3, #63
	bne.n	.L_0817451a
.L_08174542:
	ldr	r5, [sp, #56]
	movs	r6, #0
	movs	r4, #36
	ldrsh	r0, [r5, r4]
	add	r5, sp, #212
	adds	r1, r5, #0
	bl	sub_0815e20c
	mov	r8, r6
	ldr	r6, [sp, #48]
	mov	r9, r5
.L_08174558:
	ldr	r3, [r6, #24]
	mov	sl, r6
	cmp	r3, #31
	bhi.n	.L_081745d4
	adds	r2, r3, #0
	cmp	r3, #0
	bge.n	.L_08174568
	adds	r2, r3, #7
.L_08174568:
	asrs	r3, r2, #3
	ldr	r0, [r6, #8]
	adds	r7, r3, #2
	bl	sub_08002096
	ldr	r3, [r6, #20]
	adds	r1, r3, #0
	muls	r1, r0
	mov	r0, r9
	lsrs	r3, r1, #31
	adds	r1, r1, r3
	ldr	r3, [r0, #0]
	asrs	r1, r1, #17
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	ldr	r0, [r6, #8]
	adds	r5, r1, r3
	bl	sub_08002090
	ldr	r1, [r6, #20]
	mov	r4, r9
	adds	r3, r1, #0
	muls	r3, r0
	ldr	r2, [r4, #4]
	asrs	r3, r3, #16
	adds	r4, r3, r2
	ldr	r3, [r6, #8]
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r3, r3, r0
	subs	r1, #4
	str	r3, [r6, #8]
	str	r1, [r6, #20]
	cmp	r1, #0
	ble.n	sub_081745d2
	ldr	r2, [pc, #460]
	lsls	r0, r7, #1
	subs	r3, r0, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #28]
	subs	r3, r4, r7
	adds	r1, r2, r1
	lsrs	r2, r7, #31
	adds	r2, r7, r2
	asrs	r2, r2, #1
	str	r0, [sp, #4]
	subs	r2, r5, r2
	str	r7, [sp, #0]
	ldr	r0, [sp, #44]
	ldr	r4, [sp, #36]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x69b3
.L_081745d4:
	movs	r7, #1
	add	r8, r7
	subs	r3, #1
	mov	r5, sl
	mov	r0, r8
	str	r3, [r5, #24]
	adds	r6, #28
	cmp	r0, #48
	bne.n	.L_08174558
	b.n	sub_081747f2
.L_081745e8:
	ldr	r1, [sp, #52]
	cmp	r1, #5
	beq.n	.L_081745f0
	b.n	.L_08174784
.L_081745f0:
	ldr	r2, [sp, #48]
	movs	r3, #192
	lsls	r3, r3, #3
	adds	r3, #228
	mov	r4, fp
	adds	r7, r2, r3
	cmp	r4, #0
	bne.n	.L_0817460e
	add	r2, sp, #248
	ldr	r3, [r2, #0]
	str	r3, [r7, #12]
	ldr	r3, [r2, #4]
	str	r3, [r7, #16]
	ldr	r3, [r2, #8]
	str	r3, [r7, #20]
.L_0817460e:
	mov	r5, fp
	cmp	r5, #47
	bgt.n	.L_0817465a
	ldr	r6, [sp, #56]
	ldr	r0, [r6, #8]
	bl	sub_08118098
	ldr	r6, [r0, #0]
	add	r5, sp, #248
	ldr	r3, [r5, #0]
	ldr	r0, [r6, #8]
	movs	r1, #40
	subs	r0, r0, r3
	bl	sub_08002054
	ldr	r3, [r7, #12]
	movs	r1, #40
	adds	r3, r3, r0
	str	r3, [r7, #12]
	movs	r0, #240
	ldr	r3, [r5, #4]
	lsls	r0, r0, #14
	subs	r0, r0, r3
	bl	sub_08002054
	ldr	r3, [r7, #16]
	movs	r1, #40
	adds	r3, r3, r0
	str	r3, [r7, #16]
	ldr	r3, [r5, #8]
	ldr	r0, [r6, #16]
	subs	r0, r0, r3
	bl	sub_08002054
	ldr	r3, [r7, #20]
	adds	r3, r3, r0
	str	r3, [r7, #20]
	b.n	.L_081746da
.L_0817465a:
	ldr	r2, [sp, #56]
	mov	r5, fp
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	bl	sub_08118098
	ldr	r3, [sp, #48]
	movs	r4, #217
	lsls	r4, r4, #3
	ldr	r0, [r0, #0]
	adds	r1, r3, r4
	cmp	r5, #48
	bne.n	.L_081746aa
	movs	r3, #0
	str	r3, [r1, #0]
	str	r3, [r1, #4]
	str	r3, [r1, #8]
	ldr	r3, [r7, #12]
	ldr	r2, [r0, #8]
	subs	r2, r2, r3
	cmp	r2, #0
	bge.n	.L_08174688
	adds	r2, #31
.L_08174688:
	asrs	r3, r2, #5
	str	r3, [r1, #12]
	ldr	r3, [r7, #16]
	negs	r3, r3
	cmp	r3, #0
	bge.n	.L_08174696
	adds	r3, #31
.L_08174696:
	asrs	r3, r3, #5
	str	r3, [r1, #16]
	ldr	r3, [r7, #20]
	ldr	r2, [r0, #16]
	subs	r0, r2, r3
	cmp	r0, #0
	bge.n	.L_081746a6
	adds	r0, #31
.L_081746a6:
	asrs	r3, r0, #5
	str	r3, [r1, #20]
.L_081746aa:
	ldr	r2, [r1, #0]
	ldr	r3, [r7, #12]
	adds	r3, r3, r2
	str	r3, [r7, #12]
	ldr	r3, [r7, #16]
	ldr	r2, [r1, #4]
	adds	r3, r3, r2
	str	r3, [r7, #16]
	ldr	r3, [r7, #20]
	ldr	r2, [r1, #8]
	adds	r3, r3, r2
	str	r3, [r7, #20]
	ldr	r3, [r1, #0]
	ldr	r2, [r1, #12]
	adds	r3, r3, r2
	str	r3, [r1, #0]
	ldr	r2, [r1, #16]
	ldr	r3, [r1, #4]
	adds	r3, r3, r2
	str	r3, [r1, #4]
	ldr	r2, [r1, #20]
	ldr	r3, [r1, #8]
	adds	r3, r3, r2
	str	r3, [r1, #8]
.L_081746da:
	bl	sub_08014de4
	ldr	r0, [sp, #24]
	adds	r1, r0, #0
	adds	r1, #12
	bl	sub_080156e8
	adds	r0, r7, #0
	add	r5, sp, #200
	adds	r1, r5, #0
	adds	r0, #12
	bl	sub_0815e1ec
	ldr	r2, [r5, #0]
	movs	r1, #20
	asrs	r2, r2, #1
	str	r2, [r5, #0]
	str	r2, [r7, #0]
	ldr	r4, [sp, #36]
	ldr	r3, [r5, #4]
	subs	r2, #10
	str	r3, [r7, #4]
	ldr	r6, [sp, #48]
	movs	r7, #216
	lsls	r7, r7, #7
	str	r1, [sp, #0]
	adds	r7, #192
	movs	r1, #40
	str	r1, [sp, #4]
	subs	r3, #20
	adds	r1, r6, r7
	ldr	r0, [sp, #44]
	mov	r6, fp
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2e00
	bne.n	.L_0817472a
	movs	r0, #140
	bl	sub_081c0010
.L_0817472a:
	mov	r7, fp
	cmp	r7, #55
	bne.n	.L_08174776
	ldr	r0, [sp, #48]
	movs	r1, #192
	ldr	r3, [r5, #0]
	lsls	r1, r1, #3
	adds	r1, #172
	adds	r2, r0, r1
	str	r3, [r2, #0]
	movs	r0, #145
	ldr	r3, [r5, #4]
	movs	r7, #238
	str	r3, [r2, #4]
	bl	sub_081180e8
	ldr	r3, [sp, #56]
	movs	r1, #4
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	bl	sub_08118088
	ldr	r5, [sp, #56]
	movs	r3, #24
	movs	r4, #36
	ldrsh	r0, [r5, r4]
	movs	r2, #5
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r3, #0
	bl	sub_0814cd48
	ldr	r6, [sp, #48]
	lsls	r7, r7, #7
	adds	r7, #168
	adds	r2, r6, r7
	movs	r3, #16
	str	r3, [r2, #0]
.L_08174776:
	movs	r0, #8
	movs	r1, #8
	bl	sub_08158ce0
	b.n	sub_081747f2
	.2byte 0x7410
	.2byte 0x0819
.L_08174784:
	ldr	r0, [sp, #52]
	cmp	r0, #0
	bne.n	sub_081747f2
	mov	r1, fp
	lsls	r6, r1, #11
	adds	r0, r6, #0
	bl	sub_08002096
	add	r5, sp, #260
	ldr	r3, [r5, #0]
	lsls	r2, r0, #2
	adds	r2, r2, r0
	lsrs	r1, r3, #31
	adds	r3, r3, r1
	lsls	r2, r2, #2
	asrs	r3, r3, #1
	asrs	r2, r2, #16
	adds	r2, r2, r3
	adds	r0, r6, #0
	adds	r7, r2, #0
	bl	sub_08002090
	ldr	r3, [r5, #4]
	lsls	r0, r0, #2
	asrs	r0, r0, #16
	adds	r0, r0, r3
	adds	r4, r0, #0
	mov	r2, fp
	subs	r7, #10
	subs	r4, #24
	cmp	r2, #48
	ble.n	.L_081747cc
	lsls	r0, r2, #1
	subs	r3, r4, r0
	adds	r4, r3, #0
	adds	r4, #96
.L_081747cc:
	movs	r3, #40
	negs	r3, r3
	cmp	r4, r3
	ble.n	sub_081747f2
	ldr	r5, [sp, #48]
	movs	r6, #216
	movs	r3, #20
	lsls	r6, r6, #7
	str	r3, [sp, #0]
	adds	r6, #192
	movs	r3, #40
	str	r3, [sp, #4]
	adds	r2, r7, #0
	ldr	r0, [sp, #44]
	adds	r1, r5, r6
	adds	r3, r4, #0
	ldr	r7, [sp, #36]
	mov	lr, r7
	.2byte 0xf800
	.2byte 0xf6a0
	.2byte 0xfaf7
	.2byte 0x4ac4
	movs	r3, #104
	movs	r0, #128
	str	r3, [r2, #16]
	lsls	r0, r0, #3
	bl	sub_08014dac
	str	r0, [sp, #20]
	movs	r0, #1
	bl	sub_081969f8
	ldr	r1, [sp, #8]
	mov	sl, r0
	str	r1, [r0, #16]
	ldr	r2, [sp, #20]
	str	r2, [r0, #12]
	ldr	r3, [sp, #52]
	cmp	r3, #0
	beq.n	.L_0817481e
	b.n	.L_0817495c
.L_0817481e:
	mov	r4, fp
	cmp	r4, #0
	bne.n	.L_08174870
	movs	r5, #0
	mov	r8, r5
	ldr	r5, [sp, #48]
	movs	r6, #0
.L_0817482c:
	mov	r7, r8
	lsls	r3, r7, #15
	str	r3, [r5, #0]
	movs	r3, #224
	lsls	r3, r3, #14
	str	r3, [r5, #4]
	bl	sub_08014878
	movs	r3, #128
	lsls	r3, r3, #1
	adds	r3, #255
	ands	r3, r0
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r3, r3, r0
	str	r3, [r5, #12]
	bl	sub_08014878
	movs	r3, #127
	movs	r1, #153
	ands	r3, r0
	lsls	r1, r1, #2
	adds	r3, r3, r1
	negs	r3, r3
	movs	r2, #1
	lsls	r3, r3, #7
	add	r8, r2
	str	r3, [r5, #16]
	mov	r3, r8
	str	r6, [r5, #24]
	subs	r6, #8
	adds	r5, #28
	cmp	r3, #16
	bne.n	.L_0817482c
.L_08174870:
	mov	r4, fp
	cmp	r4, #4
	bne.n	.L_0817487c
	movs	r0, #147
	bl	sub_081c0010
.L_0817487c:
	mov	r5, fp
	cmp	r5, #48
	bne.n	.L_0817488a
	movs	r0, #1
	negs	r0, r0
	bl	sub_081180e8
.L_0817488a:
	ldr	r6, [sp, #8]
	mov	r7, sp
	movs	r3, #5
	adds	r7, #60
	strb	r3, [r6, #0]
	str	r7, [sp, #8]
	strb	r3, [r7, #1]
	ldr	r3, [pc, #624]
	mov	r1, sl
	movs	r0, #7
	str	r0, [r1, #0]
	str	r3, [r1, #8]
	ldr	r5, [sp, #48]
	movs	r2, #0
	mov	r9, r7
	mov	r8, r2
.L_081748aa:
	ldr	r2, [r5, #24]
	adds	r7, r5, #0
	cmp	r2, #0
	blt.n	.L_08174948
	movs	r6, #234
	lsls	r6, r6, #7
	movs	r3, #16
	adds	r6, #48
	negs	r3, r3
	cmp	r2, #6
	bgt.n	.L_081748c4
	lsls	r3, r2, #3
	subs	r3, #64
.L_081748c4:
	cmp	r2, #31
	ble.n	.L_081748d6
	movs	r3, #32
	subs	r3, r3, r2
	movs	r4, #64
	lsls	r3, r3, #2
	negs	r4, r4
	cmp	r3, r4
	blt.n	.L_0817494c
.L_081748d6:
	mov	r1, r8
	mov	r0, sl
	str	r3, [r0, #20]
	lsrs	r3, r1, #31
	add	r3, r8
	asrs	r3, r3, #1
	ldr	r2, [sp, #48]
	lsls	r3, r3, #1
	subs	r3, r1, r3
	lsls	r3, r3, #10
	movs	r4, #224
	lsls	r4, r4, #3
	adds	r3, r2, r3
	adds	r3, r3, r4
	mov	r0, r9
	str	r3, [r0, #4]
	bl	sub_08014de4
	ldr	r3, [r5, #4]
	ldr	r2, [r5, #16]
	ldr	r0, [r5, #0]
	adds	r3, r3, r2
	str	r3, [r5, #4]
	ldr	r3, [r5, #12]
	adds	r0, r0, r3
	str	r0, [r5, #0]
	bl	sub_08002096
	ldr	r3, [sp, #260]
	lsls	r0, r0, #4
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	subs	r3, #64
	lsls	r3, r3, #16
	adds	r0, r0, r3
	ldr	r1, [r5, #4]
	movs	r2, #0
	bl	sub_08015160
	asrs	r2, r6, #1
	adds	r0, r2, #0
	adds	r1, r6, #0
	bl	sub_080151e4
	ldr	r0, [pc, #476]
	bl	sub_08015024
	movs	r2, #4
	ldr	r0, [pc, #472]
	ldr	r1, [sp, #20]
	bl	sub_08196958
	mov	r0, sl
	bl	sub_08196a7c
	ldr	r2, [r5, #24]
.L_08174948:
	adds	r3, r2, #1
	str	r3, [r7, #24]
.L_0817494c:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	adds	r5, #28
	cmp	r2, #6
	bne.n	.L_081748aa
	.2byte 0xf001
	.2byte 0xfac0
.L_0817495c:
	ldr	r3, [sp, #52]
	cmp	r3, #6
	bne.n	.L_08174a4e
	ldr	r4, [sp, #8]
	mov	r5, sp
	adds	r5, #60
	movs	r3, #5
	strb	r3, [r4, #0]
	str	r5, [sp, #8]
	strb	r3, [r5, #1]
	ldr	r6, [sp, #52]
	ldr	r3, [pc, #408]
	mov	r7, sl
	str	r6, [r7, #0]
	str	r5, [r7, #16]
	str	r3, [r7, #8]
	ldr	r0, [sp, #20]
	mov	r9, r5
	str	r0, [r7, #12]
	movs	r7, #0
.L_08174984:
	lsls	r3, r7, #5
	cmp	fp, r3
	blt.n	.L_08174a44
	mov	r1, fp
	subs	r2, r1, r3
	movs	r4, #128
	lsls	r3, r2, #10
	lsls	r4, r4, #5
	adds	r6, r3, r4
	movs	r3, #0
	cmp	r2, #63
	ble.n	.L_081749aa
	movs	r3, #64
	subs	r3, r3, r2
	movs	r5, #64
	lsls	r3, r3, #2
	negs	r5, r5
	cmp	r3, r5
	blt.n	.L_08174a44
.L_081749aa:
	mov	r0, sl
	str	r3, [r0, #20]
	asrs	r5, r6, #1
	bl	sub_08014de4
	adds	r1, r6, #0
	adds	r2, r6, #0
	adds	r0, r5, #0
	bl	sub_080151e4
	mov	r1, fp
	movs	r2, #192
	lsls	r0, r1, #8
	lsls	r2, r2, #8
	adds	r0, r0, r2
	bl	sub_080150e4
	ldr	r4, [pc, #328]
	mov	r3, fp
	lsls	r0, r3, #6
	adds	r0, r0, r4
	bl	sub_08015024
	movs	r3, #178
	movs	r6, #0
	lsls	r3, r3, #1
	mov	r8, r6
	mov	r6, fp
	muls	r6, r3
.L_081749e4:
	mov	r0, r8
	movs	r1, #3
	bl	sub_08002064
	ldr	r1, [sp, #48]
	lsls	r0, r0, #10
	movs	r2, #224
	adds	r0, r1, r0
	lsls	r2, r2, #3
	adds	r0, r0, r2
	mov	r3, r9
	str	r0, [r3, #4]
	bl	sub_08014e38
	movs	r3, #168
	lsls	r3, r3, #5
	adds	r3, #85
	mov	r0, r8
	muls	r0, r3
	adds	r0, r6, r0
	bl	sub_080150e4
	movs	r1, #232
	movs	r0, #0
	lsls	r1, r1, #15
	movs	r2, #0
	bl	sub_08015160
	adds	r0, r5, #0
	adds	r1, r5, #0
	adds	r2, r5, #0
	bl	sub_080151e4
	ldr	r0, [pc, #236]
	ldr	r1, [sp, #20]
	movs	r2, #4
	bl	sub_08196958
	bl	sub_08014ea8
	mov	r0, sl
	bl	sub_08196a7c
	movs	r4, #1
	add	r8, r4
	mov	r0, r8
	cmp	r0, #12
	bne.n	.L_081749e4
.L_08174a44:
	adds	r7, #1
	cmp	r7, #1
	bne.n	.L_08174984
	.2byte 0xf001
	.2byte 0xfa47
.L_08174a4e:
	ldr	r1, [sp, #52]
	cmp	r1, #1
	.2byte 0xd167
	movs	r3, #0
	mov	r2, sl
	str	r3, [r2, #20]
	ldr	r3, [sp, #8]
	movs	r2, #6
	strb	r2, [r3, #0]
	ldr	r3, [pc, #184]
	movs	r5, #61
	mov	r6, sl
	add	r4, sp, #60
	add	r5, sp
	str	r4, [sp, #8]
	strb	r2, [r5, #0]
	str	r3, [r6, #8]
	str	r2, [r6, #0]
	ldr	r7, [sp, #48]
	movs	r0, #224
	ldr	r2, [sp, #56]
	lsls	r0, r0, #3
	adds	r3, r7, r0
	str	r3, [sp, #64]
	ldr	r3, [r2, #20]
	movs	r1, #0
	mov	r8, r1
	cmp	r3, #0
	bne.n	.L_08174a8c
	.2byte 0xf001
	.2byte 0xfa28
.L_08174a8c:
	mov	r3, fp
	lsls	r3, r3, #9
	mov	r9, r3
	movs	r3, #160
	lsls	r3, r3, #4
	adds	r3, #36
	mov	r4, fp
	muls	r4, r3
	adds	r3, r4, #0
	add	r6, sp, #188
	movs	r7, #36
	asrs	r5, r3, #1
.L_08174aa4:
	ldr	r1, [sp, #56]
	ldrsh	r0, [r7, r1]
	adds	r1, r6, #0
	bl	sub_0815e20c
	bl	sub_08014de4
	ldr	r0, [r6, #0]
	ldr	r1, [r6, #4]
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	subs	r0, #64
	subs	r1, #48
	lsls	r1, r1, #16
	movs	r2, #0
	lsls	r0, r0, #16
	bl	sub_08015160
	movs	r0, #128
	lsls	r0, r0, #7
	bl	sub_08015024
	ldr	r0, [pc, #76]
	adds	r7, #2
	add	r0, r9
	bl	sub_08015068
	adds	r0, r5, #0
	adds	r1, r5, #0
	adds	r2, r5, #0
	bl	sub_080151e4
	ldr	r0, [pc, #44]
	ldr	r1, [sp, #20]
	movs	r2, #4
	bl	sub_08196958
	mov	r0, sl
	bl	sub_08196a7c
	ldr	r4, [sp, #56]
	movs	r3, #1
	add	r8, r3
	ldr	r3, [r4, #20]
	cmp	r8, r3
	bne.n	.L_08174aa4
	.2byte 0xf001
	.2byte 0xf9eb
	movs	r0, r0
	asrs	r0, r4, #7
	lsls	r0, r0, #12
	str	r2, [sp, #272]
	lsrs	r1, r3, #32
	.2byte 0xc000
	.2byte 0xffff
	.2byte 0x9210
	lsrs	r1, r3, #32
	.2byte 0xb890
	.2byte 0xffff
	.2byte 0x9340
	lsrs	r1, r3, #32
	bl	sub_08175b22
.L_08174b24:
	ldr	r5, [sp, #52]
	cmp	r5, #5
	beq.n	.L_08174b2c
	b.n	.L_08174d26
.L_08174b2c:
	mov	r6, fp
	cmp	r6, #47
	ble.n	.L_08174b34
	b.n	.L_08174c58
.L_08174b34:
	movs	r3, #16
	mov	r7, sl
	negs	r3, r3
	str	r3, [r7, #20]
	ldr	r0, [sp, #8]
	movs	r2, #61
	add	r1, sp, #60
	movs	r3, #2
	add	r2, sp
	strb	r5, [r0, #0]
	str	r1, [sp, #8]
	strb	r3, [r2, #0]
	ldr	r4, [sp, #48]
	movs	r5, #142
	lsls	r5, r5, #7
	adds	r3, r4, r5
	str	r3, [sp, #64]
	ldr	r3, [pc, #896]
	movs	r6, #7
	str	r1, [r7, #16]
	str	r6, [r7, #0]
	str	r3, [r7, #8]
	ldr	r0, [sp, #20]
	mov	r1, fp
	str	r0, [r7, #12]
	cmp	r1, #0
	bne.n	.L_08174bae
	ldr	r5, [sp, #48]
	movs	r2, #0
	mov	r8, r2
	movs	r6, #0
.L_08174b72:
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
	beq.n	.L_08174b9e
	ldr	r3, [r5, #4]
	negs	r3, r3
	str	r3, [r5, #4]
.L_08174b9e:
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	str	r6, [r5, #24]
	subs	r6, #2
	adds	r5, #28
	cmp	r4, #16
	bne.n	.L_08174b72
.L_08174bae:
	movs	r5, #0
	ldr	r6, [sp, #48]
	movs	r0, #192
	mov	r8, r5
	lsls	r0, r0, #3
	movs	r1, #128
	ldr	r5, [sp, #48]
	adds	r0, #228
	lsls	r1, r1, #9
	mov	r2, fp
	adds	r7, r6, r0
	mov	r9, r1
	lsls	r6, r2, #1
.L_08174bc8:
	ldr	r3, [r5, #24]
	ldr	r2, [r5, #4]
	adds	r3, #1
	str	r3, [r5, #24]
	ldr	r3, [r5, #0]
	lsls	r2, r2, #1
	adds	r3, r3, r2
	str	r3, [r5, #0]
	movs	r2, #31
	adds	r3, r6, #0
	ands	r3, r2
	mov	r4, sl
	strb	r3, [r4, #25]
	ldr	r3, [r5, #24]
	cmp	r3, #31
	bhi.n	.L_08174c4a
	bl	sub_08014de4
	ldr	r0, [r7, #0]
	ldr	r1, [pc, #748]
	lsls	r0, r0, #16
	adds	r0, r0, r1
	ldr	r1, [r7, #4]
	ldr	r2, [pc, #740]
	lsls	r1, r1, #16
	adds	r1, r1, r2
	movs	r2, #0
	bl	sub_08015160
	movs	r0, #128
	mov	r1, r9
	mov	r2, r9
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
	adds	r3, r0, #0
	lsls	r0, r3, #1
	adds	r0, r0, r3
	movs	r3, #192
	lsls	r3, r3, #8
	movs	r1, #128
	adds	r0, r0, r3
	lsls	r1, r1, #10
	mov	r2, r9
	bl	sub_080151e4
	ldr	r0, [pc, #676]
	ldr	r1, [sp, #20]
	movs	r2, #4
	bl	sub_08196958
	mov	r0, sl
	bl	sub_08196a7c
.L_08174c4a:
	movs	r4, #1
	add	r8, r4
	mov	r0, r8
	adds	r6, #7
	adds	r5, #28
	cmp	r0, #8
	bne.n	.L_08174bc8
.L_08174c58:
	ldr	r2, [sp, #56]
	add	r5, sp, #176
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	adds	r1, r5, #0
	bl	sub_0815e21c
	ldr	r4, [sp, #8]
	movs	r7, #61
	movs	r3, #7
	add	r6, sp, #60
	add	r7, sp
	strb	r3, [r4, #0]
	str	r6, [sp, #8]
	strb	r3, [r7, #0]
	ldr	r3, [pc, #620]
	mov	r2, sl
	movs	r1, #7
	str	r1, [r2, #0]
	str	r6, [r2, #16]
	str	r3, [r2, #8]
	ldr	r0, [pc, #612]
	ldr	r3, [sp, #20]
	str	r0, [sp, #64]
	str	r3, [r2, #12]
	movs	r4, #0
	movs	r7, #128
	mov	r8, r4
	lsls	r7, r7, #8
.L_08174c92:
	mov	r6, r8
	lsls	r3, r6, #2
	mov	r0, fp
	subs	r3, r0, r3
	adds	r2, r3, #0
	subs	r2, #55
	cmp	r2, #0
	blt.n	.L_08174d18
	lsls	r3, r2, #1
	adds	r3, r3, r2
	movs	r1, #128
	lsls	r3, r3, #12
	lsls	r1, r1, #7
	adds	r6, r3, r1
	movs	r3, #0
	cmp	r2, #7
	ble.n	.L_08174cba
	movs	r3, #8
	subs	r3, r3, r2
	lsls	r3, r3, #3
.L_08174cba:
	movs	r4, #64
	mov	r2, sl
	negs	r4, r4
	str	r3, [r2, #20]
	cmp	r3, r4
	ble.n	.L_08174d18
	bl	sub_08014de4
	ldr	r0, [r5, #0]
	ldr	r1, [r5, #4]
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	subs	r0, #64
	subs	r1, #64
	lsls	r0, r0, #16
	lsls	r1, r1, #16
	movs	r2, #0
	bl	sub_08015160
	movs	r1, #128
	adds	r0, r7, #0
	lsls	r1, r1, #9
	adds	r2, r7, #0
	bl	sub_080151e4
	mov	r1, r8
	movs	r2, #128
	lsls	r0, r1, #14
	lsls	r2, r2, #6
	adds	r0, r0, r2
	bl	sub_080150e4
	ldr	r0, [pc, #492]
	bl	sub_08015024
	adds	r0, r6, #0
	bl	sub_0801521c
	ldr	r0, [pc, #484]
	ldr	r1, [sp, #20]
	movs	r2, #4
	bl	sub_08196958
	mov	r0, sl
	bl	sub_08196a7c
.L_08174d18:
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	cmp	r4, #3
	bne.n	.L_08174c92
	bl	.L_08175edc
.L_08174d26:
	ldr	r5, [sp, #52]
	cmp	r5, #4
	beq.n	.L_08174d2e
	b.n	.L_08174e30
.L_08174d2e:
	mov	r6, fp
	subs	r6, #16
	cmp	r6, #64
	bhi.n	.L_08174e00
	cmp	r6, #23
	bgt.n	.L_08174d48
	ldr	r7, [sp, #48]
	movs	r0, #238
	lsls	r0, r0, #7
	adds	r0, #132
	adds	r2, r7, r0
	movs	r3, #75
	b.n	.L_08174d54
.L_08174d48:
	ldr	r1, [sp, #48]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #132
	adds	r2, r1, r3
	movs	r3, #50
.L_08174d54:
	str	r3, [r2, #0]
	ldr	r4, [sp, #8]
	movs	r7, #61
	add	r5, sp, #60
	movs	r3, #6
	add	r7, sp
	strb	r3, [r4, #0]
	str	r5, [sp, #8]
	strb	r3, [r7, #0]
	ldr	r3, [pc, #396]
	mov	r0, sl
	movs	r1, #7
	str	r3, [r0, #8]
	str	r1, [r0, #0]
	ldr	r7, [sp, #56]
	ldr	r2, [sp, #48]
	movs	r4, #224
	movs	r5, #36
	ldrsh	r0, [r7, r5]
	lsls	r4, r4, #3
	add	r5, sp, #176
	adds	r3, r2, r4
	adds	r1, r5, #0
	str	r3, [sp, #64]
	bl	sub_0815e20c
	lsls	r3, r6, #3
	adds	r3, r3, r6
	lsls	r3, r3, #5
	subs	r3, r3, r6
	lsls	r3, r3, #4
	movs	r1, #128
	movs	r0, #0
	negs	r3, r3
	lsls	r1, r1, #9
	mov	r8, r0
	adds	r6, r3, r1
.L_08174d9e:
	cmp	r6, #0
	ble.n	.L_08174df0
	mov	r2, r8
	lsls	r3, r2, #4
	negs	r3, r3
	cmp	r3, #0
	ble.n	.L_08174dae
	movs	r3, #0
.L_08174dae:
	mov	r4, sl
	str	r3, [r4, #20]
	bl	sub_08014de4
	ldr	r0, [r5, #0]
	ldr	r1, [r5, #4]
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	subs	r0, #64
	subs	r1, #48
	lsls	r1, r1, #16
	movs	r2, #0
	lsls	r0, r0, #16
	bl	sub_08015160
	movs	r0, #192
	lsls	r0, r0, #8
	bl	sub_08015024
	adds	r0, r6, #0
	adds	r1, r6, #0
	adds	r2, r6, #0
	bl	sub_080151e4
	ldr	r0, [pc, #268]
	ldr	r1, [sp, #20]
	movs	r2, #4
	bl	sub_08196958
	mov	r0, sl
	bl	sub_08196a7c
.L_08174df0:
	movs	r0, #1
	movs	r7, #128
	add	r8, r0
	lsls	r7, r7, #7
	mov	r1, r8
	adds	r6, r6, r7
	cmp	r1, #2
	bne.n	.L_08174d9e
.L_08174e00:
	mov	r2, fp
	cmp	r2, #14
	bne.n	.L_08174e0c
	movs	r0, #212
	bl	sub_081c0010
.L_08174e0c:
	mov	r3, fp
	cmp	r3, #24
	beq.n	.L_08174e16
	bl	.L_08175edc
.L_08174e16:
	ldr	r5, [sp, #56]
	movs	r3, #12
	movs	r2, #1
	movs	r4, #36
	ldrsh	r0, [r5, r4]
	movs	r1, #7
	str	r3, [sp, #0]
	negs	r2, r2
	movs	r3, #0
	bl	sub_0814cd48
	bl	.L_08175edc
.L_08174e30:
	ldr	r6, [sp, #52]
	cmp	r6, #9
	beq.n	.L_08174e38
	b.n	.L_08175178
.L_08174e38:
	ldr	r7, [sp, #56]
	mov	r1, sp
	ldr	r0, [r7, #8]
	adds	r1, #176
	str	r1, [sp, #16]
	bl	sub_0815e21c
	mov	r2, fp
	cmp	r2, #0
	bne.n	.L_08174f42
	ldr	r3, [sp, #48]
	movs	r4, #192
	movs	r6, #36
	ldrsh	r0, [r7, r6]
	lsls	r4, r4, #3
	add	r6, sp, #164
	adds	r4, #228
	adds	r1, r6, #0
	adds	r5, r3, r4
	bl	sub_0815e20c
	ldr	r3, [r6, #0]
	movs	r7, #0
	str	r3, [r5, #0]
	movs	r2, #1
	ldr	r3, [r6, #4]
	mov	r8, r7
	str	r3, [r5, #4]
	ldr	r3, [sp, #48]
	negs	r2, r2
	adds	r3, #24
.L_08174e76:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r1, #32
	bne.n	.L_08174e76
	ldr	r7, [pc, #112]
	movs	r2, #0
	mov	r8, r2
	mov	r9, r6
.L_08174e8c:
	bl	sub_08014878
	movs	r5, #128
	lsls	r5, r5, #1
	adds	r5, #255
	ands	r5, r0
	bl	sub_08014878
	adds	r6, r0, #0
	bl	sub_08002096
	adds	r5, #64
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	muls	r5, r0
	asrs	r1, r5, #4
	str	r1, [r7, #16]
	ldr	r4, [sp, #56]
	ldr	r3, [r4, #24]
	cmp	r3, #0
	bne.n	.L_08174efc
	ldr	r3, [r7, #12]
	cmp	r3, #0
	bge.n	.L_08174ec8
	adds	r3, #3
.L_08174ec8:
	asrs	r3, r3, #2
	str	r3, [r7, #12]
	adds	r3, r1, #0
	cmp	r1, #0
	bge.n	.L_08174ed4
	adds	r3, r1, #3
.L_08174ed4:
	asrs	r3, r3, #2
	b.n	.L_08174f10
	.4byte 0x08198c34
	.4byte 0xffc00000
	.4byte 0x081991b0
	.4byte 0x08199364
	.4byte 0x02010000
	.4byte 0xfffff000
	.4byte 0x08199210
	.4byte 0x08199340
	.2byte 0x4000
	.2byte 0x0201
.L_08174efc:
	cmp	r3, #1
	bne.n	.L_08174f12
	ldr	r3, [r7, #12]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r7, #12]
	lsrs	r3, r5, #31
	adds	r3, r1, r3
	asrs	r3, r3, #1
.L_08174f10:
	str	r3, [r7, #16]
.L_08174f12:
	mov	r5, r9
	ldr	r3, [r5, #0]
	movs	r6, #1
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	add	r8, r6
	ldr	r3, [r5, #4]
	adds	r3, #16
	lsls	r3, r3, #16
	str	r3, [r7, #4]
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	movs	r0, #128
	adds	r3, #8
	lsls	r0, r0, #1
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r8, r0
	bne.n	.L_08174e8c
.L_08174f42:
	mov	r1, fp
	cmp	r1, #9
	bgt.n	.L_08174fa2
	ldr	r7, [sp, #16]
	ldr	r4, [sp, #48]
	lsls	r0, r1, #1
	lsls	r3, r1, #4
	subs	r3, r3, r0
	lsls	r3, r3, #2
	adds	r5, r3, r4
	ldr	r3, [r7, #0]
	movs	r2, #0
	mov	r8, r2
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	ldr	r3, [r7, #4]
	movs	r6, #255
	adds	r3, #8
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	bl	sub_08014878
	movs	r1, #128
	ands	r0, r6
	lsls	r1, r1, #1
	adds	r0, r0, r1
	lsls	r0, r0, #8
	str	r0, [r5, #12]
	ldr	r2, [sp, #56]
	ldr	r3, [r2, #4]
	cmp	r3, #1
	bne.n	.L_08174f8c
	negs	r3, r0
	str	r3, [r5, #12]
.L_08174f8c:
	bl	sub_08014878
	movs	r3, #128
	ands	r0, r6
	lsls	r3, r3, #1
	adds	r0, r0, r3
	negs	r0, r0
	lsls	r0, r0, #4
	mov	r4, r8
	str	r0, [r5, #16]
	str	r4, [r5, #24]
.L_08174fa2:
	movs	r5, #0
	mov	r8, r5
	ldr	r5, [sp, #48]
.L_08174fa8:
	ldr	r3, [r5, #24]
	cmp	r3, #23
	bhi.n	.L_08174ff2
	cmp	r3, #0
	bge.n	.L_08174fb4
	adds	r3, #3
.L_08174fb4:
	asrs	r3, r3, #2
	ldr	r6, [sp, #48]
	lsls	r1, r3, #3
	movs	r0, #2
	ldrsh	r2, [r5, r0]
	adds	r1, r1, r3
	movs	r4, #6
	ldrsh	r3, [r5, r4]
	movs	r0, #24
	lsls	r1, r1, #7
	movs	r7, #224
	str	r0, [sp, #0]
	adds	r1, r6, r1
	movs	r0, #48
	lsls	r7, r7, #3
	subs	r3, #24
	str	r0, [sp, #4]
	adds	r1, r1, r7
	subs	r2, #12
	ldr	r0, [sp, #44]
	ldr	r6, [sp, #36]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x1c28
	movs	r1, #60
	ldr	r2, [pc, #776]
	bl	sub_08138086
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
.L_08174ff2:
	movs	r7, #1
	add	r8, r7
	mov	r0, r8
	adds	r5, #28
	cmp	r0, #32
	bne.n	.L_08174fa8
	mov	r1, fp
	cmp	r1, #9
	bgt.n	.L_08175006
	b.n	.L_0817512a
.L_08175006:
	ldr	r3, [sp, #56]
	movs	r2, #0
	mov	r8, r2
	ldr	r1, [pc, #740]
	ldr	r2, [r3, #24]
	ldrb	r3, [r1, r2]
	cmp	r3, #0
	beq.n	.L_0817506e
	ldr	r5, [pc, #736]
	ldr	r6, [pc, #736]
.L_0817501a:
	ldr	r0, [r5, #24]
	cmp	r0, #0
	blt.n	.L_08175062
	asrs	r0, r0, #2
	adds	r0, #3
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r6, r3]
	ldr	r7, [sp, #28]
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	adds	r1, r7, r1
	subs	r2, r2, r3
	movs	r7, #6
	ldrsh	r3, [r5, r7]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #44]
	ldr	r4, [sp, #36]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x213e
	movs	r2, #0
	adds	r0, r5, #0
	bl	sub_08138086
	ldr	r3, [r5, #24]
	ldr	r1, [pc, #664]
	subs	r3, #1
	str	r3, [r5, #24]
	ldr	r7, [sp, #56]
	ldr	r2, [r7, #24]
.L_08175062:
	ldrb	r3, [r1, r2]
	movs	r0, #1
	add	r8, r0
	adds	r5, #28
	cmp	r8, r3
	bne.n	.L_0817501a
.L_0817506e:
	mov	r1, fp
	cmp	r1, #9
	ble.n	.L_0817512a
	ldr	r3, [sp, #48]
	movs	r4, #192
	lsls	r4, r4, #3
	adds	r4, #228
	adds	r6, r3, r4
	mov	r3, fp
	subs	r3, #10
	movs	r7, #128
	lsls	r3, r3, #15
	lsls	r7, r7, #7
	adds	r5, r3, r7
	movs	r1, #0
	cmp	r2, #0
	bne.n	.L_081750a2
	adds	r2, r5, #0
	cmp	r5, #0
	bge.n	.L_0817509e
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r0, #3
	adds	r2, r3, r0
.L_0817509e:
	asrs	r5, r2, #2
	b.n	.L_081750ac
.L_081750a2:
	cmp	r2, #1
	bne.n	.L_081750ac
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r5, r3, #1
.L_081750ac:
	mov	r2, fp
	cmp	r2, #15
	ble.n	.L_081750b8
	movs	r3, #16
	subs	r3, r3, r2
	lsls	r1, r3, #3
.L_081750b8:
	movs	r3, #64
	negs	r3, r3
	cmp	r1, r3
	ble.n	.L_0817512a
	mov	r4, sl
	str	r1, [r4, #20]
	ldr	r0, [sp, #8]
	movs	r3, #61
	add	r1, sp, #60
	add	r3, sp
	movs	r2, #7
	movs	r7, #7
	strb	r7, [r0, #0]
	str	r1, [sp, #8]
	strb	r2, [r3, #0]
	ldr	r3, [pc, #552]
	movs	r7, #7
	str	r3, [r4, #8]
	str	r7, [r4, #0]
	ldr	r0, [sp, #48]
	movs	r1, #222
	lsls	r1, r1, #6
	adds	r3, r0, r1
	str	r3, [sp, #64]
	bl	sub_08014de4
	ldr	r0, [r6, #0]
	ldr	r1, [r6, #4]
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	subs	r0, #64
	subs	r1, #48
	lsls	r0, r0, #16
	lsls	r1, r1, #16
	movs	r2, #0
	bl	sub_08015160
	adds	r0, r5, #0
	cmp	r5, #0
	bge.n	.L_0817510c
	adds	r0, r5, #3
.L_0817510c:
	lsrs	r2, r5, #31
	adds	r2, r5, r2
	asrs	r0, r0, #2
	asrs	r2, r2, #1
	adds	r1, r5, #0
	bl	sub_080151e4
	ldr	r0, [pc, #488]
	ldr	r1, [sp, #20]
	movs	r2, #4
	bl	sub_08196958
	mov	r0, sl
	bl	sub_08196a7c
.L_0817512a:
	mov	r2, fp
	cmp	r2, #10
	bne.n	.L_08175166
	movs	r0, #134
	bl	sub_081180e8
	movs	r4, #238
	ldr	r5, [sp, #56]
	ldr	r3, [sp, #48]
	lsls	r4, r4, #7
	adds	r4, #168
	adds	r2, r3, r4
	ldr	r3, [r5, #24]
	movs	r1, #0
	lsls	r3, r3, #1
	adds	r3, #8
	str	r3, [r2, #0]
	movs	r6, #36
	ldrsh	r0, [r5, r6]
	bl	sub_08118088
	movs	r3, #12
	movs	r7, #36
	ldrsh	r0, [r5, r7]
	movs	r1, #7
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r3, #0
	bl	sub_0814cd48
.L_08175166:
	ldr	r0, [sp, #56]
	ldr	r1, [r0, #24]
	lsls	r1, r1, #1
	adds	r1, #8
	adds	r0, r1, #0
	bl	sub_08158ce0
	bl	.L_08175edc
.L_08175178:
	ldr	r1, [sp, #52]
	cmp	r1, #11
	beq.n	.L_08175180
	b.n	.L_08175310
.L_08175180:
	mov	r2, fp
	cmp	r2, #0
	bne.n	.L_08175236
	ldr	r4, [sp, #56]
	add	r5, sp, #152
	movs	r3, #36
	ldrsh	r0, [r4, r3]
	adds	r1, r5, #0
	bl	sub_0815e21c
	ldr	r7, [sp, #48]
	movs	r6, #0
	mov	r8, r6
	mov	r9, r5
.L_0817519c:
	bl	sub_08014878
	movs	r6, #255
	ands	r6, r0
	bl	sub_08014878
	movs	r5, #254
	lsls	r5, r5, #7
	adds	r5, #255
	ands	r5, r0
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r5, r5, r0
	adds	r0, r5, #0
	bl	sub_08002096
	adds	r6, #128
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #7
	str	r3, [r7, #12]
	adds	r0, r5, #0
	bl	sub_08002090
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #16]
	movs	r3, #128
	lsls	r3, r3, #3
	str	r3, [r7, #20]
	mov	r1, r9
	ldr	r3, [r1, #0]
	movs	r5, #1
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	bl	sub_08014878
	mov	r4, r9
	ldr	r3, [r4, #4]
	movs	r2, #31
	ands	r2, r0
	subs	r3, r3, r2
	adds	r3, #16
	lsls	r3, r3, #16
	str	r3, [r7, #4]
	bl	sub_08014878
	add	r8, r5
	mov	r6, r8
	str	r0, [r7, #8]
	adds	r7, #28
	cmp	r6, #32
	bne.n	.L_0817519c
	movs	r0, #212
	bl	sub_081c0010
	movs	r0, #238
	ldr	r7, [sp, #48]
	lsls	r0, r0, #7
	adds	r0, #168
	adds	r3, r7, r0
	movs	r2, #16
	str	r2, [r3, #0]
	ldr	r2, [sp, #56]
	movs	r3, #0
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	movs	r2, #1
	movs	r1, #10
	negs	r2, r2
	str	r6, [sp, #0]
	bl	sub_0814cd48
.L_08175236:
	ldr	r4, [sp, #8]
	mov	r5, sp
	adds	r5, #60
	movs	r3, #5
	strb	r3, [r4, #0]
	mov	r7, sl
	str	r5, [sp, #8]
	strb	r3, [r5, #1]
	movs	r3, #6
	str	r3, [r7, #0]
	ldr	r3, [pc, #188]
	str	r5, [r7, #16]
	str	r3, [r7, #8]
	ldr	r0, [sp, #20]
	adds	r6, r5, #0
	str	r0, [r7, #12]
	ldr	r5, [sp, #48]
	movs	r1, #0
	mov	r8, r1
.L_0817525c:
	mov	r2, r8
	lsls	r3, r2, #1
	cmp	fp, r3
	blt.n	.L_081752e0
	mov	r4, fp
	subs	r2, r4, r3
	movs	r3, #0
	cmp	r2, #23
	ble.n	.L_0817527c
	movs	r3, #24
	subs	r3, r3, r2
	movs	r7, #64
	lsls	r3, r3, #2
	negs	r7, r7
	cmp	r3, r7
	blt.n	.L_081752e0
.L_0817527c:
	mov	r0, sl
	str	r3, [r0, #20]
	movs	r1, #3
	mov	r0, r8
	bl	sub_08002064
	ldr	r1, [sp, #48]
	lsls	r0, r0, #10
	movs	r2, #224
	adds	r0, r1, r0
	lsls	r2, r2, #3
	adds	r0, r0, r2
	str	r0, [r6, #4]
	bl	sub_08014de4
	ldr	r3, [pc, #112]
	ldr	r0, [r5, #0]
	ldr	r1, [r5, #4]
	adds	r0, r0, r3
	adds	r1, r1, r3
	movs	r2, #0
	bl	sub_08015160
	movs	r2, #128
	lsls	r2, r2, #9
	movs	r0, #128
	adds	r1, r2, #0
	lsls	r0, r0, #8
	bl	sub_080151e4
	ldr	r0, [r5, #8]
	bl	sub_080150e4
	movs	r0, #128
	lsls	r0, r0, #8
	bl	sub_0801521c
	movs	r2, #4
	ldr	r1, [sp, #20]
	ldr	r0, [pc, #56]
	bl	sub_08196958
	mov	r0, sl
	bl	sub_08196a7c
	adds	r0, r5, #0
	movs	r1, #60
	movs	r2, #0
	bl	sub_08138058
.L_081752e0:
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	adds	r5, #28
	cmp	r4, #12
	bne.n	.L_0817525c
	b.n	.L_081758ea
	movs	r0, r0
	.4byte 0xfffff000
	.4byte 0x08198c64
	.4byte 0x02014000
	.4byte 0x08197410
	.4byte 0x08199364
	.4byte 0x081991e0
	.4byte 0x08199244
	.2byte 0x0000
	.2byte 0xffc0
.L_08175310:
	.2byte 0x9d0d
	cmp	r5, #14
	beq.n	.L_08175318
	b.n	.L_08175468
.L_08175318:
	mov	r6, fp
	cmp	r6, #0
	bne.n	.L_081753ae
	ldr	r7, [sp, #56]
	add	r5, sp, #140
	ldr	r0, [r7, #8]
	adds	r1, r5, #0
	bl	sub_0815e21c
	ldr	r7, [sp, #48]
	movs	r0, #0
	mov	r8, r0
	mov	r9, r5
.L_08175332:
	bl	sub_08014878
	movs	r6, #255
	ands	r6, r0
	bl	sub_08014878
	movs	r5, #254
	lsls	r5, r5, #7
	adds	r5, #255
	movs	r1, #128
	lsls	r1, r1, #7
	ands	r5, r0
	adds	r5, r5, r1
	adds	r0, r5, #0
	bl	sub_08002096
	adds	r6, #128
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #12]
	adds	r0, r5, #0
	bl	sub_08002090
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #16]
	movs	r3, #128
	lsls	r3, r3, #3
	str	r3, [r7, #20]
	mov	r2, r9
	ldr	r3, [r2, #0]
	movs	r5, #1
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	bl	sub_08014878
	mov	r4, r9
	ldr	r3, [r4, #4]
	movs	r2, #31
	ands	r2, r0
	subs	r3, r3, r2
	subs	r3, #16
	lsls	r3, r3, #16
	str	r3, [r7, #4]
	bl	sub_08014878
	add	r8, r5
	mov	r6, r8
	str	r0, [r7, #8]
	adds	r7, #28
	cmp	r6, #32
	bne.n	.L_08175332
	movs	r0, #182
	lsls	r0, r0, #1
	adds	r0, #255
	bl	sub_081c0010
.L_081753ae:
	ldr	r7, [sp, #8]
	mov	r0, sp
	adds	r0, #60
	movs	r3, #5
	strb	r3, [r7, #0]
	mov	r1, sl
	str	r0, [sp, #8]
	strb	r3, [r0, #1]
	movs	r3, #6
	str	r3, [r1, #0]
	ldr	r3, [pc, #868]
	str	r0, [r1, #16]
	str	r3, [r1, #8]
	ldr	r2, [sp, #20]
	movs	r3, #0
	str	r2, [r1, #12]
	ldr	r5, [sp, #48]
	adds	r6, r0, #0
	mov	r8, r3
.L_081753d4:
	mov	r4, r8
	lsls	r3, r4, #1
	cmp	fp, r3
	blt.n	.L_08175458
	mov	r7, fp
	subs	r2, r7, r3
	movs	r3, #0
	cmp	r2, #23
	ble.n	.L_081753f4
	movs	r3, #24
	subs	r3, r3, r2
	movs	r0, #64
	lsls	r3, r3, #2
	negs	r0, r0
	cmp	r3, r0
	blt.n	.L_08175458
.L_081753f4:
	mov	r1, sl
	str	r3, [r1, #20]
	mov	r0, r8
	movs	r1, #3
	bl	sub_08002064
	ldr	r2, [sp, #48]
	lsls	r0, r0, #10
	movs	r3, #224
	adds	r0, r2, r0
	lsls	r3, r3, #3
	adds	r0, r0, r3
	str	r0, [r6, #4]
	bl	sub_08014de4
	ldr	r3, [pc, #792]
	ldr	r0, [r5, #0]
	ldr	r1, [r5, #4]
	adds	r0, r0, r3
	adds	r1, r1, r3
	movs	r2, #0
	bl	sub_08015160
	movs	r2, #128
	lsls	r2, r2, #9
	movs	r0, #128
	adds	r1, r2, #0
	lsls	r0, r0, #8
	bl	sub_080151e4
	ldr	r0, [r5, #8]
	bl	sub_080150e4
	movs	r0, #128
	lsls	r0, r0, #8
	bl	sub_0801521c
	movs	r2, #4
	ldr	r1, [sp, #20]
	ldr	r0, [pc, #748]
	bl	sub_08196958
	mov	r0, sl
	bl	sub_08196a7c
	adds	r0, r5, #0
	movs	r1, #60
	movs	r2, #0
	bl	sub_08138058
.L_08175458:
	movs	r4, #1
	add	r8, r4
	mov	r7, r8
	adds	r5, #28
	cmp	r7, #12
	bne.n	.L_081753d4
	bl	.L_08175edc
.L_08175468:
	ldr	r0, [sp, #52]
	cmp	r0, #10
	beq.n	.L_08175470
	b.n	.L_081757cc
.L_08175470:
	mov	r1, fp
	cmp	r1, #0
	bne.n	.L_0817549e
	ldr	r2, [sp, #56]
	ldr	r0, [r2, #8]
	bl	sub_08118098
	ldr	r5, [r0, #0]
	movs	r1, #0
	adds	r0, r5, #0
	bl	sub_08020090
	adds	r0, r5, #0
	movs	r1, #2
	bl	sub_08020090
	adds	r0, r5, #0
	movs	r1, #0
	bl	sub_08020098
	movs	r0, #212
	bl	sub_081c0010
.L_0817549e:
	mov	r3, fp
	cmp	r3, #14
	bne.n	.L_081754b4
	ldr	r4, [sp, #56]
	ldr	r0, [r4, #8]
	bl	sub_08118098
	movs	r1, #16
	ldr	r0, [r0, #0]
	bl	sub_08020098
.L_081754b4:
	mov	r5, fp
	cmp	r5, #22
	bne.n	.L_081754c0
	movs	r0, #212
	bl	sub_081c0010
.L_081754c0:
	mov	r6, fp
	cmp	r6, #26
	bne.n	sub_08175508
	movs	r0, #144
	bl	sub_081180e8
	movs	r0, #238
	ldr	r7, [sp, #48]
	lsls	r0, r0, #7
	adds	r0, #168
	adds	r2, r7, r0
	movs	r3, #8
	str	r3, [r2, #0]
	ldr	r2, [sp, #56]
	movs	r3, #12
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r3, #0
	movs	r1, #7
	bl	sub_0814cd48
	ldr	r4, [sp, #56]
	movs	r1, #4
	movs	r3, #36
	ldrsh	r0, [r4, r3]
	bl	sub_08118088
	movs	r1, #128
	ldr	r3, [pc, #564]
	ldr	r0, [sp, #44]
	lsls	r1, r1, #7
	ldr	r2, [pc, #564]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xad20
	mov	r6, fp
	mov	r9, r5
	cmp	r6, #27
	bgt.n	.L_08175514
	b.n	.L_08175616
.L_08175514:
	cmp	r6, #28
	bne.n	.L_08175590
	ldr	r1, [sp, #56]
	movs	r7, #36
	ldrsh	r0, [r1, r7]
	mov	r1, r9
	ldr	r7, [pc, #536]
	bl	sub_0815e20c
	movs	r3, #0
	mov	r8, r3
.L_0817552a:
	bl	sub_08014878
	movs	r5, #128
	lsls	r5, r5, #1
	adds	r5, #255
	ands	r5, r0
	bl	sub_08014878
	adds	r6, r0, #0
	bl	sub_08002096
	adds	r5, #32
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #7
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r1, r5, #0
	muls	r1, r0
	asrs	r1, r1, #6
	str	r1, [r7, #16]
	ldr	r3, [sp, #128]
	lsls	r1, r1, #1
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	ldr	r2, [r7, #12]
	asrs	r3, r3, #1
	lsls	r2, r2, #1
	lsls	r3, r3, #16
	adds	r3, r3, r2
	str	r3, [r7, #0]
	ldr	r3, [sp, #132]
	adds	r3, #16
	lsls	r3, r3, #16
	adds	r3, r3, r1
	str	r3, [r7, #4]
	bl	sub_08014878
	movs	r3, #15
	movs	r4, #1
	ands	r3, r0
	add	r8, r4
	adds	r3, #16
	mov	r5, r8
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r5, #128
	bne.n	.L_0817552a
	b.n	.L_08175594
.L_08175590:
	add	r6, sp, #128
	mov	r9, r6
.L_08175594:
	ldr	r5, [pc, #420]
	movs	r7, #0
	mov	r8, r7
.L_0817559a:
	ldr	r0, [r5, #24]
	cmp	r0, #0
	blt.n	.L_0817560a
	movs	r6, #0
	b.n	.L_081755a6
.L_081755a4:
	ldr	r0, [r5, #24]
.L_081755a6:
	cmp	r0, #0
	bge.n	.L_081755ac
	adds	r0, #7
.L_081755ac:
	asrs	r0, r0, #3
	adds	r0, #1
	ldr	r2, [pc, #396]
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #28]
	adds	r1, r2, r1
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	movs	r7, #6
	ldrsh	r3, [r5, r7]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #44]
	ldr	r4, [sp, #36]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2280
	lsls	r2, r2, #4
	adds	r0, r5, #0
	movs	r1, #63
	bl	sub_08138086
	ldr	r3, [r5, #4]
	movs	r2, #208
	lsls	r2, r2, #15
	cmp	r3, r2
	ble.n	.L_081755fe
	ldr	r3, [r5, #16]
	str	r2, [r5, #4]
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r5, #16]
.L_081755fe:
	adds	r6, #1
	cmp	r6, #4
	bne.n	.L_081755a4
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L_0817560a:
	movs	r6, #1
	add	r8, r6
	mov	r7, r8
	adds	r5, #28
	cmp	r7, #64
	bne.n	.L_0817559a
.L_08175616:
	ldr	r1, [sp, #56]
	mov	r7, sl
	ldr	r0, [r1, #8]
	mov	r1, r9
	bl	sub_0815e20c
	ldr	r2, [sp, #8]
	movs	r3, #5
	strb	r3, [r2, #0]
	add	r3, sp, #60
	str	r3, [sp, #8]
	str	r3, [r7, #16]
	ldr	r3, [pc, #276]
	movs	r5, #61
	movs	r4, #7
	add	r5, sp
	movs	r6, #7
	str	r3, [r7, #8]
	strb	r4, [r5, #0]
	str	r6, [r7, #0]
	ldr	r0, [sp, #20]
	str	r0, [r7, #12]
	bl	sub_08014de4
	movs	r2, #128
	lsls	r2, r2, #9
	movs	r0, #128
	adds	r1, r2, #0
	lsls	r0, r0, #8
	bl	sub_080151e4
	ldr	r1, [sp, #56]
	ldr	r3, [r1, #4]
	cmp	r3, #0
	bne.n	.L_08175672
	mov	r2, r9
	ldr	r0, [r2, #0]
	ldr	r1, [r2, #4]
	subs	r0, #136
	subs	r1, #48
	lsls	r0, r0, #16
	lsls	r1, r1, #16
	movs	r2, #0
	bl	sub_08015160
	b.n	.L_08175686
.L_08175672:
	mov	r3, r9
	ldr	r0, [r3, #0]
	ldr	r1, [r3, #4]
	subs	r0, #108
	subs	r1, #56
	lsls	r0, r0, #16
	lsls	r1, r1, #16
	movs	r2, #0
	bl	sub_08015160
.L_08175686:
	movs	r4, #0
	mov	r8, r4
.L_0817568a:
	bl	sub_08014e38
	bl	sub_08014878
	movs	r3, #3
	ldr	r5, [sp, #48]
	ands	r3, r0
	ldr	r2, [sp, #8]
	lsls	r3, r3, #12
	movs	r6, #224
	adds	r3, r5, r3
	lsls	r6, r6, #3
	adds	r3, r3, r6
	add	r7, sp, #60
	str	r3, [r2, #4]
	adds	r0, r7, #0
	movs	r3, #5
	movs	r1, #7
	str	r7, [sp, #8]
	strb	r3, [r0, #0]
	strb	r1, [r2, #1]
	mov	r2, fp
	cmp	r2, #23
	ble.n	.L_0817574c
	mov	r4, r8
	ldr	r6, [pc, #136]
	lsls	r3, r2, #4
	lsls	r2, r4, #1
	subs	r3, r3, r2
	adds	r5, r3, r6
	cmp	r5, #0
	blt.n	.L_081757be
	cmp	r5, #80
	ble.n	.L_081756d0
	movs	r5, #80
.L_081756d0:
	ldr	r7, [sp, #56]
	ldr	r3, [r7, #4]
	cmp	r3, #0
	bne.n	.L_081756ec
	lsls	r0, r5, #1
	lsls	r1, r5, #3
	adds	r0, r0, r5
	subs	r1, r1, r5
	lsls	r0, r0, #13
	lsls	r1, r1, #11
	movs	r2, #0
	bl	sub_08015160
	b.n	.L_08175700
.L_081756ec:
	lsls	r0, r5, #1
	adds	r0, r0, r5
	lsls	r1, r5, #3
	lsls	r0, r0, #13
	subs	r1, r1, r5
	negs	r0, r0
	lsls	r1, r1, #11
	movs	r2, #0
	bl	sub_08015160
.L_08175700:
	movs	r3, #188
	lsls	r3, r3, #8
	adds	r3, #24
	adds	r0, r5, #0
	muls	r0, r3
	movs	r1, #80
	bl	sub_08002054
	ldr	r2, [sp, #56]
	movs	r1, #192
	ldr	r3, [r2, #4]
	lsls	r1, r1, #7
	adds	r0, r0, r1
	cmp	r3, #0
	bne.n	.L_08175720
	negs	r0, r0
.L_08175720:
	bl	sub_080150e4
	b.n	.L_08175764
	movs	r0, r0
	.4byte 0x08199244
	.4byte 0xffc00000
	.4byte 0x081991e0
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.4byte 0x02014000
	.4byte 0x08197410
	.4byte 0x08199268
	.2byte 0xfea0
	.2byte 0xffff
.L_0817574c:
	mov	r3, r8
	cmp	r3, #0
	bne.n	.L_081757be
	ldr	r4, [sp, #56]
	ldr	r0, [pc, #708]
	ldr	r3, [r4, #4]
	cmp	r3, #0
	beq.n	.L_08175760
	movs	r0, #192
	lsls	r0, r0, #7
.L_08175760:
	bl	sub_080150e4
.L_08175764:
	mov	r5, r8
	negs	r3, r5
	lsls	r3, r3, #3
	mov	r6, sl
	mov	r7, fp
	str	r3, [r6, #20]
	cmp	r7, #23
	bgt.n	.L_08175780
	movs	r3, #160
	lsls	r3, r3, #4
	adds	r3, #170
	mov	r2, fp
	muls	r2, r3
	b.n	.L_08175798
.L_08175780:
	mov	r0, fp
	cmp	r0, #31
	ble.n	.L_08175794
	lsls	r3, r0, #1
	add	r3, fp
	movs	r2, #128
	lsls	r3, r3, #11
	lsls	r2, r2, #11
	subs	r2, r2, r3
	b.n	.L_08175798
.L_08175794:
	movs	r2, #128
	lsls	r2, r2, #9
.L_08175798:
	cmp	r2, #0
	ble.n	.L_081757be
	lsls	r1, r2, #1
	adds	r0, r1, r2
	movs	r2, #128
	lsls	r0, r0, #1
	lsls	r2, r2, #9
	bl	sub_080151e4
	ldr	r0, [pc, #628]
	ldr	r1, [sp, #20]
	movs	r2, #4
	bl	sub_08196958
	mov	r0, sl
	bl	sub_08196a7c
	bl	sub_08014ea8
.L_081757be:
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	cmp	r2, #4
	beq.n	.L_081757ca
	b.n	.L_0817568a
.L_081757ca:
	b.n	.L_081758ea
.L_081757cc:
	ldr	r3, [sp, #52]
	cmp	r3, #8
	beq.n	.L_081757d4
	b.n	.L_081758f4
.L_081757d4:
	ldr	r4, [sp, #48]
	movs	r5, #238
	lsls	r5, r5, #7
	adds	r5, #132
	adds	r1, r4, r5
	movs	r3, #50
	str	r3, [r1, #0]
	ldr	r7, [sp, #8]
	movs	r3, #61
	movs	r6, #7
	add	r2, sp, #60
	add	r3, sp
	strb	r6, [r7, #0]
	str	r2, [sp, #8]
	strb	r6, [r3, #0]
	ldr	r3, [pc, #560]
	mov	r4, sl
	str	r3, [r4, #8]
	movs	r3, #6
	str	r3, [r4, #0]
	ldr	r5, [sp, #48]
	movs	r6, #239
	lsls	r6, r6, #7
	adds	r2, r5, r6
	movs	r0, #0
	movs	r3, #1
	str	r3, [r2, #0]
	str	r0, [r1, #0]
	ldr	r1, [sp, #56]
	movs	r7, #224
	lsls	r7, r7, #3
	adds	r3, r5, r7
	add	r5, sp, #116
	ldr	r0, [r1, #8]
	adds	r1, r5, #0
	str	r3, [sp, #64]
	bl	sub_0815e20c
	mov	r2, fp
	cmp	r2, #15
	bgt.n	.L_08175894
	movs	r3, #0
	mov	r8, r3
	lsls	r6, r2, #11
	adds	r7, r5, #0
.L_0817582e:
	adds	r0, r6, #0
	bl	sub_08002096
	lsls	r3, r0, #2
	adds	r5, r3, r0
	cmp	r5, #0
	ble.n	.L_0817588a
	mov	r4, sl
	movs	r3, #0
	str	r3, [r4, #20]
	bl	sub_08014de4
	ldr	r0, [r7, #0]
	mov	r1, r8
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	lsls	r3, r1, #4
	ldr	r1, [r7, #4]
	asrs	r0, r0, #1
	adds	r0, r0, r3
	subs	r0, #72
	subs	r1, #64
	lsls	r0, r0, #16
	lsls	r1, r1, #16
	movs	r2, #0
	bl	sub_08015160
	lsrs	r2, r5, #31
	adds	r2, r5, r2
	asrs	r2, r2, #1
	adds	r0, r2, #0
	adds	r1, r5, #0
	bl	sub_080151e4
	movs	r0, #128
	lsls	r0, r0, #6
	bl	sub_080150e4
	ldr	r0, [pc, #428]
	ldr	r1, [sp, #20]
	movs	r2, #4
	bl	sub_08196958
	mov	r0, sl
	bl	sub_08196a7c
.L_0817588a:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	cmp	r3, #2
	bne.n	.L_0817582e
.L_08175894:
	mov	r4, fp
	cmp	r4, #8
	bne.n	.L_081758ea
	movs	r0, #212
	bl	sub_081c0010
	movs	r6, #238
	ldr	r5, [sp, #48]
	lsls	r6, r6, #7
	adds	r6, #168
	adds	r3, r5, r6
	mov	r7, fp
	str	r7, [r3, #0]
	movs	r1, #128
	ldr	r3, [pc, #376]
	lsls	r1, r1, #7
	ldr	r0, [sp, #44]
	ldr	r2, [pc, #376]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x990e
	movs	r0, #0
	ldr	r3, [r1, #20]
	mov	r8, r0
	cmp	r3, #0
	beq.n	.L_081758ea
	movs	r6, #7
	movs	r5, #36
.L_081758cc:
	ldr	r2, [sp, #56]
	movs	r1, #7
	ldrsh	r0, [r5, r2]
	mov	r3, r8
	movs	r2, #5
	str	r6, [sp, #0]
	bl	sub_0814cd48
	ldr	r7, [sp, #56]
	movs	r4, #1
	ldr	r3, [r7, #20]
	add	r8, r4
	adds	r5, #2
	cmp	r8, r3
	bne.n	.L_081758cc
.L_081758ea:
	movs	r0, #8
	movs	r1, #8
	bl	sub_08158ce0
	b.n	.L_08175edc
.L_081758f4:
	ldr	r0, [sp, #52]
	cmp	r0, #13
	beq.n	.L_081758fc
	b.n	.L_08175a3c
.L_081758fc:
	mov	r1, fp
	cmp	r1, #0
	bne.n	.L_08175908
	movs	r0, #104
	bl	sub_081c0010
.L_08175908:
	ldr	r3, [sp, #56]
	mov	r4, sp
	adds	r4, #104
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	adds	r1, r4, #0
	mov	r5, fp
	str	r4, [sp, #12]
	movs	r6, #16
	bl	sub_0815e21c
	lsls	r3, r5, #2
	subs	r3, #64
	negs	r6, r6
	cmp	r3, r6
	blt.n	.L_0817592c
	movs	r3, #16
	negs	r3, r3
.L_0817592c:
	mov	r7, sl
	str	r3, [r7, #20]
	ldr	r2, [sp, #8]
	movs	r3, #6
	str	r2, [r7, #16]
	ldr	r0, [sp, #20]
	str	r3, [r7, #0]
	ldr	r3, [pc, #248]
	str	r0, [r7, #12]
	str	r3, [r7, #8]
	ldr	r1, [sp, #48]
	movs	r4, #224
	lsls	r4, r4, #3
	adds	r3, r1, r4
	str	r3, [r2, #4]
	add	r5, sp, #60
	movs	r6, #7
	movs	r3, #4
	mov	r7, fp
	str	r5, [sp, #8]
	strb	r6, [r5, #0]
	strb	r3, [r2, #1]
	cmp	r7, #26
	bne.n	.L_0817596e
	movs	r0, #238
	lsls	r0, r0, #7
	adds	r0, #168
	adds	r2, r1, r0
	movs	r3, #8
	str	r3, [r2, #0]
	movs	r0, #134
	bl	sub_081180e8
.L_0817596e:
	movs	r2, #128
	movs	r1, #0
	lsls	r2, r2, #9
	mov	r3, fp
	mov	r8, r1
	mov	r9, r2
	lsls	r7, r3, #9
	mov	r6, fp
.L_0817597e:
	mov	r4, r8
	lsls	r3, r4, #1
	movs	r5, #128
	adds	r3, #16
	lsls	r5, r5, #10
	cmp	fp, r3
	blt.n	.L_08175994
	mov	r0, fp
	subs	r3, r0, r3
	lsls	r3, r3, #13
	subs	r5, r5, r3
.L_08175994:
	movs	r1, #128
	lsls	r1, r1, #8
	cmp	r5, r1
	ble.n	.L_08175a06
	movs	r2, #7
	adds	r3, r6, #0
	ands	r3, r2
	mov	r4, sl
	lsls	r3, r3, #4
	strb	r3, [r4, #24]
	bl	sub_08014de4
	ldr	r1, [sp, #12]
	movs	r2, #0
	ldr	r0, [r1, #0]
	movs	r1, #128
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	subs	r0, #60
	lsls	r0, r0, #16
	lsls	r1, r1, #12
	bl	sub_08015160
	adds	r1, r5, #0
	asrs	r0, r5, #1
	mov	r2, r9
	bl	sub_080151e4
	mov	r2, r8
	movs	r3, #250
	lsls	r3, r3, #4
	lsls	r0, r2, #13
	adds	r0, r0, r3
	bl	sub_08015024
	mov	r4, fp
	lsls	r0, r4, #9
	bl	sub_08015068
	adds	r0, r7, #0
	bl	sub_080150e4
	movs	r1, #128
	mov	r0, r9
	lsls	r1, r1, #8
	mov	r2, r9
	bl	sub_080151e4
	ldr	r0, [pc, #64]
	ldr	r1, [sp, #20]
	movs	r2, #32
	bl	sub_08196958
	mov	r0, sl
	bl	sub_08196a7c
.L_08175a06:
	movs	r0, #1
	movs	r5, #128
	add	r8, r0
	lsls	r5, r5, #6
	mov	r1, r8
	adds	r7, r7, r5
	adds	r6, #5
	cmp	r1, #8
	bne.n	.L_0817597e
	b.n	.L_08175edc
	movs	r0, r0
	.4byte 0xffffa000
	.4byte 0x081991d0
	.4byte 0x08199364
	.4byte 0x081991e0
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.4byte 0x08198ec4
	.2byte 0x8cac
	.2byte 0x0819
.L_08175a3c:
	ldr	r2, [sp, #52]
	cmp	r2, #12
	beq.n	.L_08175a44
	b.n	.L_08175c42
.L_08175a44:
	ldr	r4, [sp, #8]
	mov	r6, sp
	adds	r6, #60
	movs	r3, #7
	strb	r3, [r4, #0]
	str	r6, [sp, #8]
	strb	r3, [r6, #1]
	ldr	r3, [pc, #840]
	ldr	r7, [pc, #840]
	mov	r0, sl
	str	r3, [r0, #8]
	movs	r3, #6
	str	r3, [r0, #0]
	str	r7, [r6, #4]
	ldr	r1, [sp, #56]
	add	r2, sp, #92
	mov	r9, r2
	ldr	r0, [r1, #8]
	mov	r1, r9
	bl	sub_0815e20c
	bl	sub_08014de4
	mov	r3, r9
	ldr	r0, [r3, #0]
	mov	r4, r9
	lsrs	r3, r0, #31
	ldr	r1, [r4, #4]
	adds	r0, r0, r3
	asrs	r0, r0, #1
	subs	r0, #64
	subs	r1, #48
	lsls	r0, r0, #16
	lsls	r1, r1, #16
	movs	r2, #0
	mov	r6, fp
	movs	r5, #0
	bl	sub_08015160
	cmp	r6, #15
	bgt.n	.L_08175abe
	lsls	r3, r6, #1
	add	r3, fp
	movs	r1, #192
	lsls	r3, r3, #12
	lsls	r1, r1, #10
	subs	r1, r1, r3
	asrs	r2, r1, #1
	mov	r7, sl
	adds	r0, r2, #0
	str	r5, [r7, #20]
	bl	sub_080151e4
	ldr	r0, [pc, #756]
	ldr	r1, [sp, #20]
	movs	r2, #4
	bl	sub_08196958
	mov	r0, sl
	bl	sub_08196a7c
.L_08175abe:
	bl	sub_08014de4
	mov	r1, r9
	ldr	r0, [r1, #0]
	ldr	r1, [r1, #4]
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	subs	r0, #64
	subs	r1, #48
	lsls	r0, r0, #16
	lsls	r1, r1, #16
	movs	r2, #0
	bl	sub_08015160
	mov	r3, fp
	subs	r3, #12
	cmp	r3, #31
	bhi.n	.L_08175b2a
	ldr	r3, [pc, #704]
	mov	r2, fp
	lsls	r0, r2, #10
	adds	r0, r0, r3
	bl	sub_08002096
	lsls	r1, r0, #1
	adds	r1, r1, r0
	ldr	r4, [sp, #48]
	ldr	r6, [sp, #8]
	lsrs	r2, r1, #31
	movs	r5, #224
	adds	r2, r1, r2
	lsls	r5, r5, #3
	adds	r3, r4, r5
	asrs	r2, r2, #1
	adds	r0, r2, #0
	str	r3, [r6, #4]
	bl	sub_080151e4
	mov	r7, fp
	movs	r1, #128
	lsls	r1, r1, #6
	lsls	r0, r7, #9
	adds	r0, r0, r1
	bl	sub_080150e4
	ldr	r0, [pc, #648]
	ldr	r1, [sp, #20]
	movs	r2, #4
	bl	sub_08196958
	mov	r0, sl
	bl	sub_08196a7c
.L_08175b2a:
	mov	r2, fp
	cmp	r2, #0
	bne.n	sub_08175b76
	ldr	r7, [pc, #632]
	ldr	r5, [sp, #48]
	movs	r3, #0
	movs	r6, #192
	mov	r8, r3
	lsls	r6, r6, #2
.L_08175b3c:
	bl	sub_08014878
	str	r6, [r5, #0]
	adds	r0, r7, #0
	movs	r1, #48
	bl	sub_08002054
	str	r0, [r5, #12]
	bl	sub_08014878
	movs	r4, #1
	add	r8, r4
	str	r0, [r5, #4]
	mov	r0, r8
	subs	r7, #24
	adds	r6, #24
	adds	r5, #28
	cmp	r0, #64
	bne.n	.L_08175b3c
	ldr	r0, [pc, #588]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #580]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4659
	cmp	r1, #15
	bgt.n	.L_08175be2
	ldr	r6, [sp, #48]
	movs	r2, #0
	mov	r8, r2
.L_08175b82:
	movs	r7, #0
.L_08175b84:
	ldr	r0, [r6, #4]
	bl	sub_08002096
	ldr	r3, [r6, #0]
	mov	r4, r9
	adds	r5, r3, #0
	muls	r5, r0
	ldr	r3, [r4, #0]
	asrs	r5, r5, #21
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	ldr	r0, [r6, #4]
	adds	r5, r5, r3
	bl	sub_08002090
	ldr	r3, [r6, #0]
	ldr	r2, [pc, #528]
	muls	r3, r0
	ldrh	r1, [r2, #6]
	ldr	r0, [sp, #28]
	subs	r5, #2
	movs	r2, #4
	movs	r4, #8
	asrs	r3, r3, #20
	adds	r1, r0, r1
	adds	r3, #56
	str	r2, [sp, #0]
	str	r4, [sp, #4]
	adds	r2, r5, #0
	ldr	r0, [sp, #44]
	ldr	r5, [sp, #36]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x6832
	ldr	r3, [r6, #12]
	adds	r7, #1
	adds	r2, r2, r3
	str	r2, [r6, #0]
	cmp	r7, #3
	bne.n	.L_08175b84
	movs	r7, #1
	add	r8, r7
	mov	r0, r8
	adds	r6, #28
	cmp	r0, #64
	bne.n	.L_08175b82
.L_08175be2:
	movs	r1, #0
	mov	r8, r1
.L_08175be6:
	mov	r2, fp
	cmp	r2, #7
	ble.n	.L_08175c0c
	cmp	r2, #15
	bgt.n	.L_08175bf8
	ldr	r0, [pc, #456]
	bl	sub_0815f0a0
	b.n	.L_08175c0c
.L_08175bf8:
	mov	r3, fp
	cmp	r3, #23
	bgt.n	.L_08175c06
	ldr	r0, [pc, #448]
	bl	sub_0815f0a0
	b.n	.L_08175c0c
.L_08175c06:
	ldr	r0, [pc, #444]
	bl	sub_0815f0a0
.L_08175c0c:
	movs	r4, #1
	add	r8, r4
	mov	r5, r8
	cmp	r5, #3
	bne.n	.L_08175be6
	mov	r6, fp
	cmp	r6, #16
	bne.n	.L_08175c38
	movs	r0, #212
	bl	sub_081c0010
	ldr	r1, [sp, #56]
	movs	r3, #30
	movs	r2, #1
	movs	r7, #36
	ldrsh	r0, [r1, r7]
	negs	r2, r2
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r3, #0
	bl	sub_0814cd48
.L_08175c38:
	mov	r2, fp
	cmp	r2, #28
	beq.n	.L_08175c40
	b.n	.L_08175edc
.L_08175c40:
	b.n	.L_08175d90
.L_08175c42:
	ldr	r3, [sp, #52]
	cmp	r3, #15
	beq.n	.L_08175c4a
	b.n	.L_08175dd4
.L_08175c4a:
	ldr	r5, [sp, #8]
	ldr	r3, [pc, #332]
	mov	r6, sp
	ldr	r7, [pc, #332]
	movs	r4, #7
	adds	r6, #60
	mov	r0, sl
	strb	r4, [r5, #0]
	str	r6, [sp, #8]
	str	r3, [r0, #8]
	movs	r3, #6
	strb	r4, [r6, #1]
	str	r7, [r6, #4]
	str	r3, [r0, #0]
	ldr	r1, [sp, #56]
	add	r2, sp, #80
	mov	r9, r2
	ldr	r0, [r1, #8]
	mov	r1, r9
	bl	sub_0815e20c
	mov	r4, fp
	movs	r3, #0
	mov	r8, r3
	lsls	r7, r4, #3
	lsls	r6, r4, #14
	mov	r5, fp
.L_08175c80:
	bl	sub_08014de4
	ldr	r0, [sp, #80]
	ldr	r1, [sp, #84]
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	subs	r0, #64
	subs	r1, #48
	lsls	r0, r0, #16
	lsls	r1, r1, #16
	movs	r2, #0
	bl	sub_08015160
	cmp	r5, #15
	bhi.n	.L_08175ce4
	movs	r3, #128
	lsls	r3, r3, #11
	subs	r1, r3, r6
	movs	r3, #0
	cmp	r5, #7
	bgt.n	.L_08175cb0
	adds	r3, r7, #0
	subs	r3, #64
.L_08175cb0:
	lsrs	r2, r1, #31
	adds	r2, r1, r2
	asrs	r2, r2, #1
	mov	r0, sl
	str	r3, [r0, #20]
	adds	r0, r2, #0
	bl	sub_080151e4
	mov	r1, r8
	movs	r2, #128
	lsls	r0, r1, #14
	lsls	r2, r2, #6
	adds	r0, r0, r2
	bl	sub_080150e4
	ldr	r0, [pc, #248]
	bl	sub_08015024
	ldr	r0, [pc, #244]
	ldr	r1, [sp, #20]
	movs	r2, #4
	bl	sub_08196958
	mov	r0, sl
	bl	sub_08196a7c
.L_08175ce4:
	ldr	r3, [pc, #232]
	movs	r4, #1
	add	r8, r4
	mov	r0, r8
	subs	r7, #64
	adds	r6, r6, r3
	subs	r5, #8
	cmp	r0, #2
	bne.n	.L_08175c80
	bl	sub_08014de4
	mov	r1, r9
	ldr	r0, [r1, #0]
	ldr	r1, [r1, #4]
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	subs	r0, #64
	subs	r1, #48
	lsls	r0, r0, #16
	lsls	r1, r1, #16
	movs	r2, #0
	bl	sub_08015160
	mov	r3, fp
	subs	r3, #12
	cmp	r3, #31
	bhi.n	.L_08175d5c
	ldr	r3, [pc, #136]
	mov	r2, fp
	lsls	r0, r2, #10
	adds	r0, r0, r3
	bl	sub_08002096
	ldr	r4, [sp, #48]
	adds	r1, r0, #0
	lsls	r1, r1, #1
	movs	r5, #224
	asrs	r2, r1, #1
	lsls	r5, r5, #3
	adds	r3, r4, r5
	adds	r0, r2, #0
	mov	r6, fp
	movs	r7, #128
	str	r3, [sp, #64]
	lsls	r7, r7, #6
	bl	sub_080151e4
	lsls	r0, r6, #9
	adds	r0, r0, r7
	bl	sub_080150e4
	ldr	r0, [pc, #84]
	ldr	r1, [sp, #20]
	movs	r2, #4
	bl	sub_08196958
	mov	r0, sl
	bl	sub_08196a7c
.L_08175d5c:
	mov	r0, fp
	cmp	r0, #0
	bne.n	.L_08175d66
	bl	sub_0815b410
.L_08175d66:
	mov	r1, fp
	cmp	r1, #16
	bne.n	.L_08175d88
	movs	r0, #212
	bl	sub_081c0010
	ldr	r3, [sp, #56]
	movs	r1, #7
	movs	r2, #36
	ldrsh	r0, [r3, r2]
	movs	r3, #30
	movs	r2, #1
	str	r3, [sp, #0]
	negs	r2, r2
	movs	r3, #0
	bl	sub_0814cd48
.L_08175d88:
	mov	r4, fp
	cmp	r4, #28
	beq.n	.L_08175d90
	b.n	.L_08175edc
.L_08175d90:
	movs	r0, #1
	negs	r0, r0
	bl	sub_081180e8
	b.n	.L_08175edc
	movs	r0, r0
	.4byte 0x08199364
	.4byte 0x02010000
	.4byte 0x081991e0
	.4byte 0xffffd000
	.4byte 0xfffffd00
	.4byte 0x00000184
	.4byte 0x03000730
	.4byte 0x08197410
	.4byte 0x00000154
	.4byte 0x00000150
	.4byte 0x00000152
	.4byte 0xfffff000
	.4byte 0x08199210
	.2byte 0x0000
	.2byte 0xfffe
.L_08175dd4:
	.2byte 0x4655
	movs	r3, #0
	str	r3, [r5, #20]
	ldr	r6, [sp, #8]
	movs	r0, #61
	movs	r3, #6
	add	r7, sp, #60
	add	r0, sp
	strb	r3, [r6, #0]
	str	r7, [sp, #8]
	strb	r3, [r0, #0]
	ldr	r3, [pc, #368]
	movs	r1, #7
	str	r3, [r5, #8]
	str	r1, [r5, #0]
	ldr	r2, [sp, #48]
	movs	r4, #224
	ldr	r5, [sp, #52]
	lsls	r4, r4, #3
	adds	r3, r2, r4
	str	r3, [sp, #64]
	cmp	r5, #2
	beq.n	.L_08175e06
	cmp	r5, #7
	bne.n	.L_08175e22
.L_08175e06:
	mov	r6, fp
	cmp	r6, #31
	ble.n	.L_08175e28
	mov	r7, fp
	cmp	r7, #59
	ble.n	.L_08175e4e
	movs	r3, #176
	lsls	r3, r3, #4
	adds	r3, #184
	mov	r2, fp
	muls	r2, r3
	ldr	r3, [pc, #320]
	subs	r6, r3, r2
	b.n	.L_08175e54
.L_08175e22:
	mov	r0, fp
	cmp	r0, #31
	bgt.n	.L_08175e34
.L_08175e28:
	movs	r3, #160
	lsls	r3, r3, #3
	adds	r3, #220
	mov	r6, fp
	muls	r6, r3
	b.n	.L_08175e54
.L_08175e34:
	mov	r1, fp
	cmp	r1, #39
	ble.n	.L_08175e4e
	movs	r3, #176
	lsls	r3, r3, #4
	adds	r3, #184
	mov	r2, fp
	muls	r2, r3
	movs	r3, #164
	lsls	r3, r3, #10
	adds	r3, #64
	subs	r6, r3, r2
	b.n	.L_08175e54
.L_08175e4e:
	movs	r6, #187
	lsls	r6, r6, #8
	adds	r6, #128
.L_08175e54:
	cmp	r6, #0
	ble.n	.L_08175edc
	ldr	r2, [sp, #52]
	cmp	r2, #2
	beq.n	.L_08175e62
	cmp	r2, #7
	bne.n	.L_08175e72
.L_08175e62:
	ldr	r4, [sp, #56]
	add	r5, sp, #68
	movs	r3, #36
	ldrsh	r0, [r4, r3]
	adds	r1, r5, #0
	bl	sub_0815e20c
	b.n	.L_08175e7e
.L_08175e72:
	ldr	r5, [sp, #56]
	ldr	r0, [r5, #8]
	add	r5, sp, #68
	adds	r1, r5, #0
	bl	sub_0815e20c
.L_08175e7e:
	bl	sub_08014de4
	ldr	r0, [r5, #0]
	ldr	r1, [r5, #4]
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	subs	r0, #64
	subs	r1, #48
	movs	r2, #0
	lsls	r1, r1, #16
	lsls	r0, r0, #16
	bl	sub_08015160
	movs	r0, #128
	lsls	r0, r0, #7
	bl	sub_08015024
	adds	r0, r6, #0
	movs	r1, #3
	bl	sub_08002054
	adds	r1, r6, #0
	adds	r2, r6, #0
	bl	sub_080151e4
	ldr	r6, [sp, #52]
	cmp	r6, #2
	bne.n	.L_08175ec4
	mov	r7, fp
	negs	r0, r7
	lsls	r0, r0, #10
	bl	sub_08015068
	b.n	.L_08175ecc
.L_08175ec4:
	mov	r1, fp
	lsls	r0, r1, #10
	bl	sub_08015068
.L_08175ecc:
	ldr	r0, [pc, #148]
	ldr	r1, [sp, #20]
	movs	r2, #4
	bl	sub_08196958
	mov	r0, sl
	bl	sub_08196a7c
.L_08175edc:
	mov	r0, sl
	bl	sub_08013164
	ldr	r0, [sp, #20]
	bl	sub_08013164
	ldr	r2, [pc, #124]
	movs	r3, #120
	str	r3, [r2, #12]
	str	r3, [r2, #16]
	bl	sub_081434f8
	ldr	r2, [sp, #52]
	cmp	r2, #1
	bne.n	.L_08175f0c
	ldr	r3, [sp, #48]
	movs	r4, #240
	lsls	r4, r4, #7
	adds	r4, #228
	adds	r2, r3, r4
.L_08175f04:
	ldr	r3, [r2, #0]
	ldr	r3, [r2, #0]
	cmp	r3, #1
	bls.n	.L_08175f04
.L_08175f0c:
	ldr	r5, [sp, #48]
	movs	r6, #240
	lsls	r6, r6, #7
	adds	r6, #232
	adds	r2, r5, r6
	movs	r3, #1
	movs	r0, #1
	str	r3, [r2, #0]
	bl	sub_08013560
	ldr	r0, [sp, #32]
	movs	r7, #1
	add	fp, r7
	cmp	fp, r0
	beq.n	.L_08175f2e
	bl	.L_0817414a
.L_08175f2e:
	movs	r0, #104
	bl	sub_0801314c
	ldr	r1, [sp, #52]
	cmp	r1, #0
	beq.n	.L_08175f3e
	cmp	r1, #6
	bne.n	.L_08175f44
.L_08175f3e:
	ldr	r0, [pc, #44]
	bl	sub_08014644
.L_08175f44:
	ldr	r0, [pc, #40]
	bl	sub_08014644
	bl	sub_08143bb8
	add	sp, #272
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x08199340
	.4byte 0x00037aa0
	.4byte 0x08199210
	.4byte 0x030011e0
	.4byte 0x08152475
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #324
	str	r0, [sp, #100]
	movs	r2, #192
	lsls	r2, r2, #18
	ldr	r0, [r2, #96]
	adds	r3, r2, #0
	str	r0, [sp, #96]
	adds	r3, #176
	ldr	r1, [r2, #92]
	movs	r0, #0
	str	r1, [sp, #92]
	ldr	r6, [pc, #100]
	ldr	r3, [r3, #0]
	mov	fp, r6
	str	r3, [sp, #88]
	ldr	r3, [pc, #96]
	movs	r6, #1
	ldrh	r4, [r3, #4]
	mov	sl, r3
	str	r4, [sp, #76]
	ldr	r5, [r2, #100]
	str	r5, [sp, #72]
	movs	r5, #240
	ldr	r2, [r2, #36]
	str	r2, [sp, #68]
	bl	sub_081435e0
	bl	sub_0813ba50
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #12
	strh	r3, [r2, #0]
	ldr	r2, [pc, #52]
	movs	r3, #160
	lsls	r3, r3, #19
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	ldr	r7, [sp, #92]
	movs	r0, #239
	lsls	r0, r0, #7
	adds	r3, r7, r0
	movs	r1, #0
	str	r1, [r3, #0]
	mov	r8, r1
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #32]
	bl	sub_080145a8
	movs	r0, #1
	movs	r1, #0
	bl	sub_08163c2c
	ldr	r3, [pc, #24]
	mov	r4, sl
	b.n	.L_08176010
	.4byte 0x00000784
	.4byte 0x00000000
	.4byte 0x02010000
	.4byte 0x03001120
	.4byte 0x08143001
	.2byte 0x11e0
	.2byte 0x0300
.L_08176010:
	str	r5, [r3, #16]
	ldr	r2, [sp, #88]
	mov	r3, r8
	add	r0, sp, #264
	strh	r3, [r4, #4]
	movs	r3, #255
	movs	r1, #0
	str	r6, [r2, #16]
	strh	r3, [r0, #0]
	bl	sub_08118010
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #72
	strh	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #64
	strh	r5, [r3, #0]
	movs	r0, #1
	bl	sub_08013560
	bl	sub_08014c4c
	movs	r7, #237
	ldr	r5, [sp, #68]
	lsls	r7, r7, #3
	adds	r7, #255
	adds	r3, r5, r7
	strb	r6, [r3, #0]
	ldr	r1, [pc, #24]
	movs	r0, #1
	movs	r2, #0
	bl	sub_08118040
	movs	r2, #20
	negs	r2, r2
	adds	r1, r2, #0
	movs	r0, #0
	b.n	.L_0817606c
	movs	r0, r0
	.4byte 0x00002737
	.2byte 0x0075
	.2byte 0x0000
.L_0817606c:
	bl	sub_08164b2c
	movs	r0, #1
	movs	r1, #1
	bl	sub_08163c2c
	ldr	r3, [pc, #52]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	subs	r2, #2
	strh	r3, [r2, #0]
	ldr	r3, [pc, #44]
	subs	r2, #48
	strh	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	mov	r1, sp
	adds	r3, #40
	mov	r0, r8
	str	r0, [r3, #0]
	adds	r1, #176
	str	r1, [sp, #64]
	bl	sub_08144aac
	ldr	r0, [pc, #24]
	bl	sub_08013300
	movs	r2, #160
	adds	r7, r0, #0
	b.n	.L_081760c0
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x00003f44
	.4byte 0x00000080
	.2byte 0x009c
	.2byte 0x0000
.L_081760c0:
	adds	r1, r7, #0
	ldr	r3, [pc, #540]
	lsls	r2, r2, #1
	ldr	r0, [pc, #540]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9b17
	movs	r2, #160
	movs	r4, #224
	lsls	r2, r2, #1
	lsls	r4, r4, #3
	adds	r7, r7, r2
	adds	r5, r3, r4
	adds	r1, r5, #0
	adds	r0, r7, #0
	bl	sub_0801587c
	movs	r6, #0
	movs	r1, #128
	mov	r9, r6
	lsls	r1, r1, #5
.L_081760ea:
	ldrb	r2, [r5, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_081760f6
	adds	r3, #32
	strb	r3, [r5, #0]
.L_081760f6:
	movs	r7, #1
	add	r9, r7
	adds	r5, #1
	cmp	r9, r1
	bne.n	.L_081760ea
	ldr	r2, [pc, #484]
	movs	r1, #64
	movs	r3, #0
	movs	r0, #64
	bl	sub_0815b290
	movs	r2, #238
	ldr	r1, [sp, #92]
	ldrb	r3, [r0, #9]
	lsls	r2, r2, #7
	adds	r2, #220
	adds	r1, r1, r2
	movs	r2, #12
	orrs	r3, r2
	strb	r3, [r0, #9]
	ldr	r3, [pc, #460]
	str	r0, [r1, #0]
	mov	sl, r3
	ldrb	r3, [r0, #16]
	ldr	r5, [sp, #92]
	lsls	r3, r3, #2
	add	r3, sl
	ldr	r4, [pc, #448]
	ldrh	r0, [r3, #2]
	movs	r7, #224
	lsls	r7, r7, #3
	adds	r6, r5, r7
	movs	r2, #128
	adds	r0, r0, r4
	ldr	r5, [pc, #420]
	mov	r8, r1
	lsls	r2, r2, #5
	adds	r1, r6, #0
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4640
	ldr	r2, [r0, #0]
	movs	r3, #13
	ldrb	r1, [r2, #5]
	negs	r3, r3
	ands	r3, r1
	strb	r3, [r2, #5]
	movs	r3, #32
	strb	r3, [r2, #23]
	ldr	r0, [pc, #408]
	bl	sub_08013300
	adds	r7, r0, #0
	adds	r1, r7, #0
	movs	r2, #32
	adds	r7, #32
	ldr	r0, [pc, #400]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x1c38
	adds	r1, r6, #0
	bl	sub_0801587c
	movs	r2, #146
	lsls	r2, r2, #1
	add	r2, sp
	str	r2, [sp, #24]
	ldr	r7, [pc, #380]
	movs	r1, #0
	mov	r8, r5
	mov	r9, r1
	adds	r5, r2, #0
.L_08176186:
	movs	r3, #240
	movs	r1, #16
	movs	r2, #0
	lsls	r3, r3, #8
	movs	r0, #16
	bl	sub_0815b290
	movs	r4, #13
	ldrb	r2, [r0, #9]
	negs	r4, r4
	adds	r3, r4, #0
	ands	r2, r3
	movs	r3, #4
	orrs	r2, r3
	ldrb	r3, [r0, #16]
	str	r0, [r7, #0]
	lsls	r3, r3, #2
	add	r3, sl
	strb	r2, [r0, #9]
	ldr	r1, [pc, #320]
	ldrh	r0, [r3, #2]
	movs	r2, #32
	adds	r0, r0, r1
	adds	r1, r6, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0xcf08
	movs	r2, #1
	ldrh	r3, [r3, #8]
	add	r9, r2
	lsls	r3, r3, #22
	lsrs	r3, r3, #22
	strh	r3, [r5, #0]
	mov	r3, r9
	adds	r5, #2
	adds	r6, #32
	cmp	r3, #16
	bne.n	.L_08176186
	ldr	r6, [pc, #268]
	ldr	r5, [pc, #296]
	movs	r4, #0
	mov	r9, r4
.L_081761da:
	movs	r3, #240
	movs	r1, #8
	movs	r2, #0
	lsls	r3, r3, #8
	movs	r0, #8
	bl	sub_0815b3b0
	mov	r7, fp
	stmia	r5!, {r0}
	ldr	r1, [r7, #0]
	movs	r2, #24
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x2001
	add	r9, r0
	mov	r1, r9
	cmp	r1, #32
	bne.n	.L_081761da
	ldr	r5, [pc, #260]
	ldr	r1, [sp, #72]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r2, [sp, #92]
	movs	r6, #224
	lsls	r6, r6, #3
	adds	r0, r5, #0
	adds	r1, r2, r6
	movs	r3, #0
	movs	r2, #0
	bl	sub_08157cf4
	ldr	r4, [pc, #232]
	movs	r3, #0
	str	r3, [sp, #84]
	movs	r5, #32
	mov	ip, r4
	movs	r7, #6
	mov	lr, r5
	movs	r4, #0
	movs	r0, #0
.L_08176230:
	ldr	r3, [sp, #72]
	movs	r1, #0
	lsls	r2, r4, #1
	mov	r9, r1
	adds	r2, r2, r3
	adds	r1, r0, #0
.L_0817623c:
	mov	r5, ip
	ldrh	r3, [r5, r7]
	ldr	r5, [sp, #92]
	add	r3, r9
	adds	r3, r3, r6
	ldrb	r3, [r5, r3]
	cmp	r3, #0
	beq.n	.L_08176254
	subs	r3, r3, r1
	cmp	r3, #0
	bgt.n	.L_08176254
	movs	r3, #1
.L_08176254:
	strb	r3, [r2, #0]
	movs	r3, #1
	add	r9, r3
	adds	r2, #1
	cmp	r9, lr
	bne.n	.L_0817623c
	ldr	r5, [sp, #84]
	adds	r4, #16
	adds	r5, #1
	adds	r0, #7
	str	r5, [sp, #84]
	cmp	r5, #10
	bne.n	.L_08176230
	ldr	r6, [sp, #92]
	movs	r7, #224
	lsls	r7, r7, #3
	adds	r1, r6, r7
	ldr	r0, [pc, #148]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r2, #184
	lsls	r2, r2, #5
	adds	r1, r6, r2
	movs	r3, #1
	movs	r2, #1
	ldr	r0, [pc, #132]
	bl	sub_08157cf4
	ldr	r0, [pc, #128]
	bl	sub_08013300
	adds	r7, r0, #0
	movs	r0, #160
	movs	r2, #128
	ldr	r5, [pc, #64]
	lsls	r0, r0, #19
	adds	r1, r7, #0
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4b0d
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #12
	strh	r3, [r2, #0]
	movs	r3, #128
	adds	r2, #28
	lsls	r3, r3, #4
	str	r3, [r2, #0]
	movs	r3, #0
	str	r3, [sp, #60]
	str	r3, [sp, #56]
	str	r3, [sp, #80]
	ldr	r3, [pc, #84]
	movs	r2, #3
	ldr	r3, [r3, #12]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_081762d0
	bl	.L_08177932
.L_081762d0:
	ldr	r5, [pc, #72]
	movs	r4, #0
	str	r4, [sp, #20]
	str	r5, [sp, #16]
	b.n	.L_08176320
	movs	r0, r0
	.4byte 0x00000784
	.4byte 0x03000730
	.4byte 0x05000240
	.4byte 0xc0002000
	.4byte 0x020036e0
	.4byte 0x06010000
	.4byte 0x0000009e
	.4byte 0x050003e0
	.4byte 0x02010000
	.4byte 0x02010040
	.4byte 0x00000134
	.4byte 0x08197410
	.4byte 0x000000b4
	.4byte 0x000000b6
	.4byte 0x000000bd
	.4byte 0x03001150
	.2byte 0x3e18
	.2byte 0x0201
.L_08176320:
	ldr	r0, [sp, #80]
	subs	r0, #32
	cmp	r0, #21
	bhi.n	.L_08176336
	ldr	r2, [sp, #80]
	movs	r1, #20
	negs	r0, r0
	subs	r2, #52
	negs	r1, r1
	bl	sub_08164b2c
.L_08176336:
	ldr	r6, [sp, #80]
	cmp	r6, #55
	bne.n	.L_0817634a
	movs	r1, #10
	movs	r2, #20
	movs	r0, #0
	negs	r1, r1
	negs	r2, r2
	bl	sub_08164b2c
.L_0817634a:
	ldr	r7, [sp, #80]
	cmp	r7, #4
	bne.n	.L_08176356
	movs	r0, #107
	bl	sub_081c0010
.L_08176356:
	ldr	r0, [sp, #80]
	cmp	r0, #55
	bne.n	.L_08176362
	movs	r0, #208
	bl	sub_081c0010
.L_08176362:
	ldr	r1, [sp, #80]
	cmp	r1, #78
	bne.n	.L_0817636e
	movs	r0, #219
	bl	sub_081c0010
.L_0817636e:
	ldr	r2, [sp, #80]
	cmp	r2, #127
	bne.n	.L_0817637a
	movs	r0, #212
	bl	sub_081c0010
.L_0817637a:
	ldr	r3, [sp, #80]
	cmp	r3, #131
	bne.n	.L_08176386
	movs	r0, #149
	bl	sub_081c0010
.L_08176386:
	ldr	r4, [sp, #80]
	cmp	r4, #206
	bne.n	.L_08176392
	movs	r0, #142
	bl	sub_081c0010
.L_08176392:
	ldr	r5, [sp, #80]
	cmp	r5, #244
	bne.n	.L_0817639e
	movs	r0, #212
	bl	sub_081c0010
.L_0817639e:
	ldr	r6, [sp, #80]
	movs	r7, #151
	lsls	r7, r7, #1
	cmp	r6, r7
	bne.n	.L_081763ae
	movs	r0, #212
	bl	sub_081c0010
.L_081763ae:
	ldr	r0, [sp, #80]
	movs	r1, #154
	lsls	r1, r1, #1
	cmp	r0, r1
	bne.n	.L_081763be
	movs	r0, #212
	bl	sub_081c0010
.L_081763be:
	ldr	r2, [sp, #80]
	movs	r3, #156
	lsls	r3, r3, #1
	cmp	r2, r3
	bne.n	.L_081763ce
	movs	r0, #212
	bl	sub_081c0010
.L_081763ce:
	ldr	r4, [sp, #80]
	movs	r5, #175
	lsls	r5, r5, #1
	cmp	r4, r5
	bne.n	.L_081763de
	movs	r0, #104
	bl	sub_081c0010
.L_081763de:
	ldr	r6, [sp, #80]
	cmp	r6, #0
	bne.n	.L_08176456
	ldr	r7, [sp, #92]
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
	str	r3, [r2, #0]
	movs	r2, #128
	ldr	r3, [pc, #40]
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	movs	r2, #0
	mov	r9, r2
	movs	r5, #240
	ldr	r4, [pc, #28]
	ldr	r0, [pc, #32]
	ldr	r1, [pc, #32]
	ldr	r2, [pc, #36]
	lsls	r5, r5, #16
	adds	r5, #160
.L_08176418:
	str	r5, [r2, #0]
	ldr	r3, [r0, #0]
	adds	r0, #28
	cmp	r3, #0
	bge.n	.L_0817643c
	adds	r3, #15
	b.n	.L_0817643c
	movs	r0, r0
	.4byte 0x00001010
	.4byte 0x0000f000
	.4byte 0x02014218
	.4byte 0x02014004
	.2byte 0x4000
	.2byte 0x0201
.L_0817643c:
	ldr	r6, [sp, #24]
	asrs	r3, r3, #4
	lsls	r3, r3, #1
	ldrh	r3, [r6, r3]
	movs	r7, #1
	orrs	r3, r4
	add	r9, r7
	str	r3, [r1, #0]
	mov	r3, r9
	adds	r1, #8
	adds	r2, #8
	cmp	r3, #64
	bne.n	.L_08176418
.L_08176456:
	ldr	r4, [sp, #80]
	subs	r4, #132
	mov	fp, r4
	cmp	r4, #107
	bls.n	.L_08176462
	b.n	.L_081765b2
.L_08176462:
	ldr	r3, [pc, #244]
	movs	r5, #168
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	add	r5, sp
	str	r3, [sp, #168]
	str	r4, [sp, #172]
	movs	r3, #160
	lsls	r3, r3, #9
	str	r3, [r5, #4]
	ldr	r6, [sp, #80]
	mov	r8, r5
	str	r3, [sp, #168]
	cmp	r6, #132
	bne.n	.L_081764d4
	ldr	r5, [pc, #216]
	movs	r7, #0
	mov	r9, r7
	movs	r6, #255
.L_08176488:
	bl	sub_08014878
	movs	r1, #160
	bl	sub_0800206c
	adds	r0, #40
	lsls	r0, r0, #16
	str	r0, [r5, #0]
	bl	sub_08014878
	movs	r3, #63
	ands	r3, r0
	adds	r3, #32
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	bl	sub_08014878
	ands	r0, r6
	str	r0, [r5, #24]
	bl	sub_08014878
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #9
	str	r0, [r5, #12]
	bl	sub_08014878
	ands	r0, r6
	negs	r0, r0
	subs	r0, #128
	lsls	r0, r0, #8
	str	r0, [r5, #16]
	movs	r0, #1
	add	r9, r0
	mov	r1, r9
	adds	r5, #28
	cmp	r1, #128
	bne.n	.L_08176488
.L_081764d4:
	ldr	r7, [pc, #136]
	ldr	r6, [pc, #140]
	movs	r2, #0
	movs	r4, #128
	mov	r9, r2
	add	r5, sp, #248
	lsls	r4, r4, #9
.L_081764e2:
	mov	r3, r9
	cmp	r3, #10
	bne.n	.L_081764ee
	mov	r0, r8
	str	r4, [r0, #4]
	str	r4, [sp, #168]
.L_081764ee:
	movs	r3, #0
	str	r3, [r5, #12]
	movs	r3, #255
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	movs	r1, #128
	ldr	r3, [r6, #0]
	lsls	r1, r1, #14
	str	r3, [r5, #0]
	ldr	r0, [r7, #0]
	ldr	r3, [r6, #4]
	mov	r2, r8
	adds	r3, r3, r1
	str	r3, [r5, #8]
	adds	r1, r5, #0
	movs	r3, #0
	str	r4, [sp, #8]
	bl	sub_08020010
	ldr	r3, [r6, #24]
	ldr	r0, [r7, #0]
	ldr	r4, [sp, #8]
	cmp	r3, #0
	bge.n	.L_08176520
	adds	r3, #15
.L_08176520:
	ldr	r2, [sp, #24]
	asrs	r3, r3, #4
	lsls	r3, r3, #1
	ldrh	r1, [r2, r3]
	ldr	r3, [pc, #36]
	ldr	r2, [pc, #40]
	ands	r1, r3
	ldrh	r3, [r0, #8]
	adds	r7, #4
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r0, #8]
	movs	r3, #1
	add	r9, r3
	mov	r0, r9
	adds	r6, #28
	cmp	r0, #48
	bne.n	.L_081764e2
	movs	r1, #0
	mov	r9, r1
	ldr	r4, [pc, #28]
	ldr	r1, [pc, #16]
	b.n	.L_0817656c
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.4byte 0x08196ea8
	.4byte 0x02014200
	.4byte 0x02010000
	.4byte 0x02014900
	.2byte 0x0000
	.2byte 0xfff0
.L_0817656c:
	.2byte 0x6848
	cmp	r0, r4
	ble.n	.L_0817658a
	ldr	r2, [r1, #0]
	cmp	r2, r4
	ble.n	.L_0817658a
	ldr	r3, [pc, #120]
	cmp	r2, r3
	bgt.n	.L_0817658a
	ldr	r3, [r1, #12]
	adds	r3, r2, r3
	str	r3, [r1, #0]
	ldr	r3, [r1, #16]
	adds	r3, r0, r3
	str	r3, [r1, #4]
.L_0817658a:
	mov	r2, r9
	cmp	r2, #0
	bge.n	.L_08176592
	adds	r2, #15
.L_08176592:
	ldr	r3, [r1, #24]
	asrs	r2, r2, #4
	adds	r2, r3, r2
	adds	r3, r2, #2
	str	r3, [r1, #24]
	cmp	r3, #255
	ble.n	.L_081765a6
	adds	r3, r2, #0
	subs	r3, #254
	str	r3, [r1, #24]
.L_081765a6:
	movs	r5, #1
	add	r9, r5
	mov	r6, r9
	adds	r1, #28
	cmp	r6, #128
	bne.n	.L_0817656c
.L_081765b2:
	ldr	r7, [sp, #80]
	subs	r7, #56
	str	r7, [sp, #52]
	cmp	r7, #75
	bhi.n	.L_08176614
	ldr	r3, [pc, #56]
	lsls	r0, r7, #7
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	str	r3, [sp, #160]
	str	r4, [sp, #164]
	bl	sub_08002096
	movs	r1, #128
	lsls	r1, r1, #7
	lsls	r0, r0, #1
	adds	r0, r0, r1
	movs	r3, #0
	add	r1, sp, #232
	add	r2, sp, #160
	str	r0, [r2, #4]
	str	r0, [sp, #160]
	str	r3, [r1, #12]
	movs	r3, #255
	lsls	r3, r3, #16
	str	r3, [r1, #4]
	movs	r3, #228
	lsls	r3, r3, #15
	str	r3, [r1, #0]
	cmp	r7, #0
	blt.n	.L_081765fc
	movs	r3, #156
	b.n	.L_081765fe
	.4byte 0x00ffffff
	.2byte 0x6eb0
	.2byte 0x0819
.L_081765fc:
	movs	r3, #200
.L_081765fe:
	lsls	r3, r3, #15
	str	r3, [r1, #8]
	ldr	r4, [sp, #92]
	movs	r5, #238
	lsls	r5, r5, #7
	adds	r5, #220
	adds	r3, r4, r5
	ldr	r0, [r3, #0]
	movs	r3, #0
	bl	sub_08020010
.L_08176614:
	ldr	r6, [sp, #80]
	cmp	r6, #131
	bne.n	sub_08176642
	movs	r0, #1
	ldr	r1, [pc, #124]
	movs	r2, #0
	bl	sub_08118040
	movs	r0, #160
	lsls	r0, r0, #19
	movs	r1, #128
	lsls	r1, r1, #1
	ldr	r2, [pc, #112]
	adds	r0, #192
	ldr	r7, [pc, #112]
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x21f0
	ldr	r0, [sp, #96]
	lsls	r1, r1, #6
	ldr	r2, [pc, #104]
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x4658
	cmp	r0, #14
	bhi.n	sub_08176664
	ldr	r1, [sp, #80]
	ldr	r0, [sp, #96]
	lsls	r3, r1, #8
	adds	r3, r3, r1
	lsls	r2, r3, #16
	adds	r3, r3, r2
	ldr	r2, [pc, #84]
	lsls	r3, r3, #2
	movs	r1, #240
	subs	r2, r2, r3
	lsls	r1, r1, #6
	ldr	r3, [pc, #68]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9c14
	movs	r3, #3
	ands	r3, r4
	cmp	r3, #0
	beq.n	.L_08176670
	b.n	.L_08176794
.L_08176670:
	mov	r5, fp
	cmp	r5, #107
	bls.n	.L_08176678
	b.n	.L_08176816
.L_08176678:
	ldr	r0, [pc, #32]
	bl	sub_08013300
	movs	r6, #160
	mov	lr, r0
	ldr	r0, [pc, #20]
	lsls	r6, r6, #19
	adds	r6, #192
	movs	r7, #0
	movs	r1, #31
	mov	r8, r6
	mov	r9, r7
	mov	ip, r0
	mov	sl, r1
	b.n	.L_081766b0
	movs	r0, r0
	.4byte 0x0000001f
	.4byte 0x00000075
	.4byte 0x7fff7fff
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.2byte 0x514f
	.2byte 0x5151
.L_081766b0:
	mov	r3, r8
	ldrh	r2, [r3, #0]
	ldr	r5, [sp, #80]
	lsls	r3, r2, #16
	mov	r4, ip
	lsrs	r0, r3, #26
	lsrs	r6, r3, #21
	mov	r7, sl
	ands	r0, r4
	ands	r6, r4
	ands	r7, r2
	cmp	r5, #167
	bgt.n	.L_081766e0
	mov	r1, lr
	ldrh	r2, [r1, #0]
	lsls	r3, r2, #16
	lsrs	r5, r3, #26
	ands	r5, r4
	lsrs	r4, r3, #21
	mov	r3, ip
	ands	r4, r3
	mov	r3, sl
	ands	r3, r2
	b.n	.L_08176700
.L_081766e0:
	mov	r4, lr
	ldrh	r1, [r4, #0]
	mov	r5, ip
	lsls	r2, r1, #16
	lsrs	r3, r2, #26
	ands	r3, r5
	adds	r5, r3, #0
	lsrs	r2, r2, #21
	mov	r3, ip
	ands	r2, r3
	mov	r3, sl
	adds	r4, r2, #0
	ands	r3, r1
	subs	r5, #16
	subs	r4, #16
	adds	r3, #8
.L_08176700:
	cmp	r5, #0
	bge.n	.L_08176706
	movs	r5, #0
.L_08176706:
	cmp	r4, #0
	bge.n	.L_0817670c
	movs	r4, #0
.L_0817670c:
	cmp	r3, #31
	ble.n	.L_08176712
	movs	r3, #31
.L_08176712:
	cmp	r0, r5
	ble.n	.L_08176718
	subs	r0, #1
.L_08176718:
	cmp	r6, r4
	ble.n	.L_0817671e
	subs	r6, #1
.L_0817671e:
	cmp	r7, r3
	ble.n	.L_08176724
	subs	r7, #1
.L_08176724:
	lsls	r2, r6, #5
	lsls	r3, r0, #10
	movs	r5, #1
	orrs	r3, r2
	add	r9, r5
	orrs	r3, r7
	mov	r4, r8
	movs	r6, #2
	mov	r7, r9
	strh	r3, [r4, #0]
	add	lr, r6
	add	r8, r6
	cmp	r7, #128
	bne.n	.L_081766b0
	movs	r0, #0
	mov	r9, r0
	ldr	r0, [pc, #112]
	movs	r4, #31
.L_08176748:
	mov	r3, fp
	cmp	r3, #0
	bge.n	.L_08176752
	ldr	r3, [sp, #80]
	subs	r3, #129
.L_08176752:
	asrs	r3, r3, #2
	subs	r1, r4, r3
	adds	r2, r1, #0
	adds	r3, r1, #0
	cmp	r1, #31
	ble.n	.L_08176760
	movs	r1, #31
.L_08176760:
	cmp	r1, #11
	bgt.n	.L_08176766
	movs	r1, #12
.L_08176766:
	cmp	r2, #31
	ble.n	.L_0817676c
	movs	r2, #31
.L_0817676c:
	cmp	r2, #3
	bgt.n	.L_08176772
	movs	r2, #4
.L_08176772:
	cmp	r3, #31
	ble.n	.L_08176778
	movs	r3, #31
.L_08176778:
	cmp	r3, #3
	bgt.n	.L_0817677e
	movs	r3, #4
.L_0817677e:
	lsls	r2, r2, #5
	lsls	r3, r3, #10
	orrs	r3, r2
	orrs	r3, r1
	movs	r1, #1
	add	r9, r1
	mov	r2, r9
	strh	r3, [r0, #0]
	adds	r0, #2
	cmp	r2, #15
	bne.n	.L_08176748
.L_08176794:
	mov	r3, fp
	cmp	r3, #107
	bhi.n	.L_08176816
	ldr	r0, [pc, #32]
	bl	sub_08013300
	ldr	r6, [pc, #16]
	ldr	r4, [pc, #28]
	movs	r5, #0
	movs	r7, #31
	mov	lr, r4
	mov	r9, r5
	mov	ip, r6
	mov	r8, r7
	b.n	.L_081767c4
	movs	r0, r0
	.4byte 0x0000001f
	.4byte 0x050003c2
	.4byte 0x0000009b
	.2byte 0x0200
	.2byte 0x0500
.L_081767c4:
	mov	r1, lr
	ldrh	r2, [r1, #0]
	ldrh	r1, [r0, #0]
	lsls	r3, r2, #16
	lsrs	r7, r3, #26
	lsrs	r5, r3, #21
	mov	r6, r8
	lsls	r3, r1, #16
	mov	r4, ip
	ands	r6, r2
	lsrs	r2, r3, #26
	lsrs	r3, r3, #21
	ands	r7, r4
	ands	r5, r4
	ands	r2, r4
	ands	r3, r4
	mov	r4, r8
	ands	r4, r1
	cmp	r7, r2
	ble.n	.L_081767ee
	subs	r7, #1
.L_081767ee:
	cmp	r5, r3
	ble.n	.L_081767f4
	subs	r5, #1
.L_081767f4:
	cmp	r6, r4
	ble.n	.L_081767fa
	subs	r6, #1
.L_081767fa:
	lsls	r3, r7, #10
	lsls	r2, r5, #5
	orrs	r3, r2
	orrs	r3, r6
	movs	r6, #1
	add	r9, r6
	mov	r5, lr
	movs	r7, #2
	mov	r1, r9
	strh	r3, [r5, #0]
	adds	r0, #2
	add	lr, r7
	cmp	r1, #192
	bne.n	.L_081767c4
.L_08176816:
	ldr	r2, [sp, #80]
	movs	r1, #0
	subs	r2, #214
	cmp	r2, #31
	bhi.n	.L_0817683c
	adds	r3, r2, #0
	cmp	r3, #0
	bge.n	.L_0817682a
	ldr	r3, [sp, #80]
	subs	r3, #211
.L_0817682a:
	asrs	r2, r3, #2
	lsrs	r3, r3, #31
	adds	r3, r2, r3
	asrs	r3, r3, #1
	str	r3, [sp, #48]
	movs	r3, #0
	str	r3, [sp, #44]
	str	r2, [sp, #40]
	movs	r1, #1
.L_0817683c:
	ldr	r2, [sp, #80]
	subs	r2, #251
	cmp	r2, #31
	bhi.n	.L_0817686c
	lsrs	r3, r2, #31
	adds	r0, r2, r3
	asrs	r1, r0, #1
	adds	r2, r1, #0
	cmp	r1, #0
	bge.n	.L_08176852
	adds	r2, r1, #3
.L_08176852:
	asrs	r2, r2, #2
	movs	r3, #4
	subs	r3, r3, r2
	str	r3, [sp, #48]
	lsrs	r3, r0, #31
	adds	r3, r1, r3
	asrs	r3, r3, #1
	movs	r2, #8
	negs	r4, r3
	subs	r2, r2, r3
	str	r4, [sp, #44]
	str	r2, [sp, #40]
	movs	r1, #1
.L_0817686c:
	ldr	r5, [sp, #80]
	ldr	r6, [pc, #108]
	adds	r3, r5, r6
	cmp	r3, #63
	bhi.n	.L_0817689c
	cmp	r3, #0
	bge.n	.L_0817687e
	ldr	r7, [pc, #100]
	adds	r3, r5, r7
.L_0817687e:
	asrs	r0, r3, #2
	lsrs	r3, r3, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r3, #8
	str	r3, [sp, #44]
	str	r0, [sp, #48]
	adds	r3, r0, #0
	cmp	r0, #0
	bge.n	.L_08176894
	adds	r3, #7
.L_08176894:
	asrs	r3, r3, #3
	subs	r3, #4
	str	r3, [sp, #40]
	movs	r1, #1
.L_0817689c:
	cmp	r1, #1
	bne.n	.L_0817691c
	ldr	r0, [pc, #64]
	bl	sub_08013300
	ldr	r5, [pc, #48]
	ldr	r4, [pc, #60]
	movs	r1, #0
	mov	r9, r1
.L_081768ae:
	ldrh	r3, [r0, #0]
	ldr	r6, [sp, #48]
	movs	r2, #31
	ands	r2, r3
	adds	r1, r2, r6
	lsls	r3, r3, #16
	ldr	r7, [sp, #44]
	ldr	r6, [sp, #40]
	lsrs	r2, r3, #21
	lsrs	r3, r3, #26
	ands	r2, r5
	ands	r3, r5
	adds	r2, r2, r7
	adds	r3, r3, r6
	cmp	r1, #31
	ble.n	.L_081768d0
	movs	r1, #31
.L_081768d0:
	cmp	r2, #31
	ble.n	.L_081768ec
	movs	r2, #31
	b.n	.L_081768ec
	.4byte 0x0000001f
	.4byte 0xfffffea4
	.4byte 0xfffffea7
	.4byte 0x0000009b
	.2byte 0x0200
	.2byte 0x0500
.L_081768ec:
	cmp	r3, #31
	ble.n	.L_081768f2
	movs	r3, #31
.L_081768f2:
	cmp	r1, #0
	bge.n	.L_081768f8
	movs	r1, #0
.L_081768f8:
	cmp	r2, #0
	bge.n	.L_081768fe
	movs	r2, #0
.L_081768fe:
	cmp	r3, #0
	bge.n	.L_08176904
	movs	r3, #0
.L_08176904:
	lsls	r3, r3, #10
	lsls	r2, r2, #5
	movs	r7, #1
	orrs	r3, r2
	add	r9, r7
	orrs	r3, r1
	mov	r1, r9
	strh	r3, [r4, #0]
	adds	r0, #2
	adds	r4, #2
	cmp	r1, #192
	bne.n	.L_081768ae
.L_0817691c:
	movs	r2, #236
	lsls	r2, r2, #1
	adds	r2, #255
	cmp	fp, r2
	bls.n	.L_0817692a
	bl	.L_0817749a
.L_0817692a:
	ldr	r3, [sp, #80]
	cmp	r3, #132
	beq.n	.L_08176932
	b.n	.L_08176ac0
.L_08176932:
	ldr	r4, [sp, #92]
	movs	r6, #238
	lsls	r6, r6, #7
	adds	r6, #220
	adds	r5, r4, r6
	ldr	r0, [r5, #0]
	bl	sub_08020048
	ldr	r0, [pc, #584]
	bl	sub_08013300
	movs	r1, #192
	ldr	r2, [pc, #580]
	adds	r7, r0, #0
	lsls	r1, r1, #1
	ldr	r3, [pc, #576]
	ldr	r0, [pc, #580]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x24c0
	lsls	r4, r4, #1
	adds	r7, r7, r4
	ldr	r6, [sp, #92]
	adds	r0, r7, #0
	movs	r7, #172
	lsls	r7, r7, #6
	adds	r1, r6, r7
	bl	sub_0801587c
	ldr	r1, [pc, #556]
	ldr	r6, [sp, #92]
	movs	r2, #172
	ldr	r7, [pc, #556]
	movs	r0, #0
	lsls	r2, r2, #6
	mov	r9, r0
	mov	sl, r1
	adds	r6, r6, r2
.L_0817697e:
	movs	r1, #32
	ldr	r2, [pc, #544]
	movs	r3, #0
	movs	r0, #32
	bl	sub_0815b290
	movs	r4, #12
	ldrb	r3, [r0, #9]
	mov	r8, r4
	mov	r1, r8
	orrs	r3, r1
	strb	r3, [r0, #9]
	ldrb	r3, [r0, #16]
	stmia	r5!, {r0}
	lsls	r3, r3, #2
	add	r3, sl
	ldrh	r0, [r3, #2]
	ldr	r2, [pc, #516]
	adds	r1, r6, #0
	adds	r0, r0, r2
	movs	r2, #128
	lsls	r2, r2, #3
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x2401
	movs	r3, #128
	add	r9, r4
	lsls	r3, r3, #3
	mov	r0, r9
	adds	r6, r6, r3
	cmp	r0, #12
	bne.n	.L_0817697e
	ldr	r0, [pc, #492]
	bl	sub_08013300
	adds	r7, r0, #0
	ldr	r3, [pc, #472]
	adds	r1, r7, #0
	movs	r2, #32
	ldr	r0, [pc, #480]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4c70
	ldr	r2, [pc, #440]
	movs	r1, #32
	ldr	r0, [pc, #468]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9e17
	movs	r0, #172
	lsls	r0, r0, #6
	adds	r5, r6, r0
	adds	r7, #32
	adds	r1, r5, #0
	adds	r0, r7, #0
	bl	sub_0801587c
	movs	r2, #192
	movs	r3, #224
	movs	r1, #64
	lsls	r2, r2, #24
	lsls	r3, r3, #8
	movs	r0, #64
	bl	sub_0815b290
	movs	r1, #240
	lsls	r1, r1, #7
	adds	r1, #12
	adds	r3, r6, r1
	str	r0, [r3, #0]
	ldrb	r3, [r0, #9]
	mov	r2, r8
	orrs	r3, r2
	strb	r3, [r0, #9]
	ldrb	r3, [r0, #16]
	ldr	r6, [pc, #388]
	lsls	r3, r3, #2
	adds	r3, r3, r6
	ldrh	r0, [r3, #2]
	ldr	r3, [pc, #392]
	movs	r2, #128
	adds	r1, r5, #0
	ldr	r4, [pc, #380]
	adds	r0, r0, r3
	lsls	r2, r2, #4
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2140
	ldr	r2, [pc, #388]
	movs	r3, #0
	movs	r0, #64
	bl	sub_0815b290
	movs	r1, #240
	ldr	r7, [sp, #92]
	lsls	r1, r1, #7
	ldrb	r2, [r0, #9]
	adds	r1, #16
	movs	r4, #13
	negs	r4, r4
	adds	r3, r7, r1
	str	r0, [r3, #0]
	adds	r3, r4, #0
	ands	r2, r3
	movs	r3, #8
	orrs	r2, r3
	movs	r3, #32
	strb	r3, [r0, #23]
	ldrb	r3, [r0, #16]
	strb	r2, [r0, #9]
	lsls	r3, r3, #2
	adds	r3, r3, r6
	ldrh	r0, [r3, #2]
	ldr	r6, [pc, #324]
	movs	r1, #128
	lsls	r1, r1, #5
	movs	r2, #0
	adds	r0, r0, r6
	ldr	r7, [pc, #296]
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x4851
	bl	sub_08013300
	adds	r7, r0, #0
	movs	r0, #160
	adds	r1, r7, #0
	movs	r2, #128
	ldr	r3, [pc, #288]
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1c29
	movs	r3, #0
	ldr	r0, [pc, #304]
	movs	r2, #0
	bl	sub_08157cf4
	movs	r2, #192
	lsls	r2, r2, #2
	adds	r1, r5, #0
	ldr	r0, [sp, #72]
	adds	r2, #2
	ldr	r4, [pc, #256]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9d17
	movs	r6, #184
	lsls	r6, r6, #5
	ldr	r0, [pc, #276]
	adds	r1, r5, r6
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r7, #144
	movs	r0, #224
	lsls	r7, r7, #15
	lsls	r0, r0, #14
	str	r7, [sp, #60]
	str	r0, [sp, #56]
.L_08176ac0:
	ldr	r1, [sp, #80]
	cmp	r1, #243
	bne.n	.L_08176b46
	ldr	r0, [pc, #252]
	bl	sub_08013300
	adds	r7, r0, #0
	movs	r1, #64
	movs	r2, #0
	ldr	r3, [pc, #192]
	ldr	r0, [pc, #240]
	adds	r7, #64
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x493b
	adds	r0, r7, #0
	bl	sub_0801587c
	ldr	r1, [pc, #228]
	movs	r4, #0
	movs	r0, #128
	mov	r9, r4
	lsls	r0, r0, #5
.L_08176aee:
	ldrb	r2, [r1, #0]
	adds	r3, r2, #0
	cmp	r3, #0
	beq.n	.L_08176afa
	adds	r3, #192
	strb	r3, [r1, #0]
.L_08176afa:
	movs	r5, #1
	add	r9, r5
	adds	r1, #1
	cmp	r9, r0
	bne.n	.L_08176aee
	ldr	r3, [pc, #200]
	movs	r6, #0
	mov	r9, r6
.L_08176b0a:
	movs	r0, #1
	mov	r7, r9
	add	r9, r0
	mov	r1, r9
	strb	r7, [r3, #0]
	adds	r3, #1
	cmp	r1, #32
	bne.n	.L_08176b0a
	ldr	r5, [pc, #180]
	movs	r2, #0
	movs	r6, #128
	mov	r9, r2
	movs	r7, #31
	lsls	r6, r6, #5
.L_08176b26:
	bl	sub_08014878
	ldr	r3, [pc, #160]
	ands	r0, r7
	adds	r0, r0, r3
	adds	r0, r0, r6
	ldrb	r2, [r5, #0]
	ldrb	r3, [r0, #0]
	movs	r4, #1
	add	r9, r4
	strb	r3, [r5, #0]
	strb	r2, [r0, #0]
	mov	r0, r9
	adds	r5, #1
	cmp	r0, #32
	bne.n	.L_08176b26
.L_08176b46:
	add	r5, sp, #216
	movs	r3, #0
	str	r3, [r5, #12]
	movs	r3, #255
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	ldr	r1, [sp, #80]
	cmp	r1, #159
	bgt.n	.L_08176b6c
	ldr	r2, [sp, #60]
	ldr	r4, [sp, #56]
	ldr	r6, [pc, #116]
	movs	r3, #128
	lsls	r3, r3, #6
	adds	r2, r2, r3
	adds	r4, r4, r6
	str	r2, [sp, #60]
	str	r4, [sp, #56]
	b.n	.L_08176be2
.L_08176b6c:
	ldr	r7, [sp, #80]
	movs	r0, #76
	adds	r0, #255
	cmp	r7, r0
	bgt.n	.L_08176bd8
	ldr	r1, [sp, #60]
	ldr	r3, [sp, #56]
	movs	r2, #128
	movs	r4, #128
	lsls	r2, r2, #6
	lsls	r4, r4, #5
	adds	r1, r1, r2
	adds	r3, r3, r4
	str	r1, [sp, #60]
	str	r3, [sp, #56]
	b.n	.L_08176be2
	.4byte 0x0000009b
	.4byte 0x7fff7fff
	.4byte 0x03000260
	.4byte 0x05000200
	.4byte 0x020036e0
	.4byte 0x03000730
	.4byte 0x80002000
	.4byte 0x06010000
	.4byte 0x0000009f
	.4byte 0x050003c0
	.4byte 0xc0002000
	.4byte 0x000000b8
	.4byte 0x00000137
	.4byte 0x000000be
	.4byte 0x0000009d
	.4byte 0x05000380
	.4byte 0x02014000
	.4byte 0x02015000
	.2byte 0xc000
	.2byte 0xffff
.L_08176bd8:
	.2byte 0x9e0f
	movs	r7, #128
	lsls	r7, r7, #6
	adds	r6, r6, r7
	str	r6, [sp, #60]
.L_08176be2:
	movs	r3, #160
	add	r7, sp, #152
	lsls	r3, r3, #9
	str	r3, [r7, #4]
	ldr	r0, [sp, #56]
	movs	r1, #48
	str	r3, [sp, #152]
	bl	sub_08002054
	movs	r1, #128
	lsls	r1, r1, #7
	adds	r0, r0, r1
	str	r0, [r7, #4]
	ldr	r2, [sp, #60]
	movs	r4, #128
	lsls	r4, r4, #15
	adds	r3, r2, r4
	str	r0, [sp, #152]
	str	r3, [r5, #0]
	movs	r3, #144
	lsls	r3, r3, #16
	str	r3, [r5, #8]
	ldr	r6, [sp, #92]
	movs	r0, #240
	lsls	r0, r0, #7
	adds	r0, #12
	adds	r3, r6, r0
	ldr	r0, [r3, #0]
	adds	r1, r5, #0
	adds	r2, r7, #0
	movs	r3, #0
	bl	sub_08020010
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r7, #4]
	ldr	r2, [sp, #92]
	str	r3, [sp, #152]
	movs	r3, #238
	lsls	r3, r3, #7
	movs	r1, #0
	adds	r3, #220
	mov	r9, r1
	adds	r6, r2, r3
.L_08176c3a:
	mov	r3, r9
	cmp	r3, #0
	bge.n	.L_08176c42
	adds	r3, #3
.L_08176c42:
	asrs	r3, r3, #2
	ldr	r0, [sp, #60]
	mov	r4, r9
	lsls	r2, r3, #2
	subs	r2, r4, r2
	lsls	r2, r2, #21
	adds	r2, r2, r0
	str	r2, [r5, #0]
	ldr	r1, [sp, #56]
	lsls	r3, r3, #21
	adds	r3, r3, r1
	str	r3, [r5, #8]
	adds	r2, r7, #0
	movs	r3, #0
	ldmia	r6!, {r0}
	adds	r1, r5, #0
	bl	sub_08020010
	movs	r2, #1
	add	r9, r2
	mov	r3, r9
	cmp	r3, #12
	bne.n	.L_08176c3a
	ldr	r4, [sp, #80]
	cmp	r4, #200
	bne.n	sub_08176c8c
	ldr	r0, [pc, #660]
	bl	sub_08013300
	adds	r7, r0, #0
	movs	r0, #160
	lsls	r0, r0, #19
	adds	r1, r7, #0
	movs	r2, #128
	ldr	r5, [pc, #648]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x9d14
	subs	r5, #200
	cmp	r5, #101
	bls.n	.L_08176c96
	b.n	.L_08176e16
.L_08176c96:
	movs	r0, #32
	bl	sub_08014dac
	mov	sl, r0
	movs	r0, #1
	bl	sub_081969f8
	adds	r7, r5, #0
	lsls	r3, r7, #2
	adds	r4, r3, #0
	lsls	r3, r7, #1
	adds	r3, r3, r7
	adds	r6, r0, #0
	lsls	r2, r3, #7
	movs	r0, #195
	ldr	r1, [sp, #80]
	subs	r2, r2, r3
	lsls	r0, r0, #9
	adds	r0, #160
	lsls	r2, r2, #2
	subs	r4, #64
	mov	fp, r0
	subs	r5, r0, r2
	cmp	r1, #253
	ble.n	.L_08176cce
	movs	r3, #254
	subs	r3, r3, r1
	lsls	r4, r3, #3
.L_08176cce:
	movs	r2, #212
	lsls	r2, r2, #6
	adds	r2, #231
	cmp	r5, r2
	bgt.n	.L_08176cde
	movs	r5, #212
	lsls	r5, r5, #6
	adds	r5, #232
.L_08176cde:
	cmp	r4, #0
	ble.n	.L_08176ce4
	movs	r4, #0
.L_08176ce4:
	str	r4, [r6, #20]
	ldr	r3, [sp, #92]
	movs	r4, #239
	lsls	r4, r4, #7
	adds	r2, r3, r4
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r0, [sp, #92]
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #132
	adds	r2, r0, r1
	movs	r3, #75
	str	r3, [r2, #0]
	ldr	r3, [sp, #144]
	ldr	r2, [pc, #528]
	movs	r4, #184
	ands	r3, r2
	movs	r2, #7
	mov	r9, r2
	orrs	r3, r2
	ldr	r2, [pc, #520]
	lsls	r4, r4, #5
	ands	r3, r2
	movs	r2, #224
	lsls	r2, r2, #3
	orrs	r3, r2
	str	r3, [sp, #144]
	adds	r3, r0, r4
	add	r0, sp, #144
	str	r3, [r0, #4]
	ldr	r3, [pc, #504]
	mov	r1, r9
	mov	r2, sl
	str	r1, [r6, #0]
	str	r0, [r6, #16]
	str	r3, [r6, #8]
	str	r2, [r6, #12]
	ldr	r3, [sp, #80]
	movs	r4, #141
	lsls	r4, r4, #1
	mov	r8, r0
	cmp	r3, r4
	bgt.n	.L_08176d80
	bl	sub_08014de4
	lsls	r2, r5, #1
	adds	r0, r2, #0
	adds	r1, r2, #0
	bl	sub_080151e4
	ldr	r1, [pc, #468]
	movs	r2, #0
	ldr	r0, [pc, #468]
	bl	sub_08015160
	movs	r0, #224
	lsls	r0, r0, #8
	bl	sub_08015024
	movs	r0, #128
	lsls	r3, r7, #8
	lsls	r0, r0, #7
	subs	r0, r0, r3
	bl	sub_080150e4
	negs	r0, r7
	lsls	r0, r0, #10
	bl	sub_08015068
	ldr	r0, [pc, #436]
	mov	r1, sl
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r6, #0
	bl	sub_08196a7c
.L_08176d80:
	ldr	r5, [sp, #80]
	cmp	r5, #231
	ble.n	.L_08176e0a
	adds	r7, r5, #0
	subs	r7, #232
	lsls	r2, r7, #2
	lsls	r3, r7, #1
	adds	r2, r2, r7
	adds	r4, r3, #0
	lsls	r3, r2, #4
	subs	r3, r3, r2
	movs	r1, #64
	lsls	r3, r3, #4
	mov	r0, fp
	subs	r4, #64
	negs	r1, r1
	subs	r5, r0, r3
	cmp	r4, r1
	bge.n	.L_08176daa
	movs	r4, #64
	negs	r4, r4
.L_08176daa:
	cmp	r4, #0
	ble.n	.L_08176db0
	movs	r4, #0
.L_08176db0:
	mov	r2, r8
	movs	r3, #6
	strb	r3, [r2, #0]
	strb	r3, [r2, #1]
	str	r4, [r6, #20]
	ldr	r4, [sp, #92]
	movs	r0, #224
	lsls	r0, r0, #3
	adds	r3, r4, r0
	str	r3, [r2, #4]
	ldr	r3, [pc, #356]
	mov	r1, r9
	str	r2, [r6, #16]
	mov	r2, sl
	str	r3, [r6, #8]
	str	r1, [r6, #0]
	str	r2, [r6, #12]
	bl	sub_08014de4
	ldr	r1, [pc, #328]
	movs	r2, #0
	adds	r0, r1, #0
	bl	sub_08015160
	adds	r1, r5, #0
	adds	r2, r5, #0
	adds	r0, r5, #0
	bl	sub_080151e4
	movs	r0, #192
	lsls	r0, r0, #8
	bl	sub_08015024
	negs	r0, r7
	lsls	r0, r0, #10
	bl	sub_08015068
	ldr	r0, [pc, #300]
	mov	r1, sl
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r6, #0
	bl	sub_08196a7c
.L_08176e0a:
	adds	r0, r6, #0
	bl	sub_08013164
	mov	r0, sl
	bl	sub_08013164
.L_08176e16:
	ldr	r3, [sp, #80]
	subs	r3, #244
	cmp	r3, #65
	bls.n	.L_08176e20
	b.n	.L_08176f9a
.L_08176e20:
	ldr	r3, [pc, #268]
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	str	r3, [sp, #136]
	str	r4, [sp, #140]
	ldr	r3, [sp, #80]
	movs	r4, #4
	adds	r4, #255
	cmp	r3, r4
	bgt.n	.L_08176eb4
	ldr	r6, [sp, #16]
	ldr	r0, [sp, #92]
	movs	r1, #240
	movs	r7, #128
	lsls	r1, r1, #7
	lsls	r7, r7, #5
	adds	r1, #16
	movs	r5, #0
	mov	ip, r6
	adds	r3, r0, r1
	adds	r6, r6, r7
	mov	r9, r5
	mov	lr, r6
	ldr	r4, [r3, #0]
	ldr	r6, [pc, #224]
	ldr	r5, [pc, #228]
.L_08176e54:
	mov	r2, lr
	ldrb	r0, [r2, #0]
	mov	r3, r9
	ldr	r7, [pc, #224]
	lsls	r1, r3, #6
	lsls	r0, r0, #1
	adds	r3, r1, r0
	adds	r2, r3, r7
	adds	r7, #1
	adds	r3, r3, r7
	ldrb	r3, [r3, #0]
	ldrb	r2, [r2, #0]
	lsls	r3, r3, #8
	orrs	r2, r3
	ldrb	r3, [r4, #16]
	subs	r7, #1
	lsls	r3, r3, #2
	adds	r3, r3, r6
	ldrh	r3, [r3, #2]
	adds	r3, r1, r3
	adds	r3, r3, r0
	strh	r2, [r3, r5]
	movs	r3, #128
	lsls	r3, r3, #5
	adds	r3, #1
	add	r3, ip
	ldrb	r0, [r3, #0]
	lsls	r0, r0, #1
	adds	r3, r1, r0
	adds	r2, r3, r7
	adds	r7, #1
	adds	r3, r3, r7
	ldrb	r3, [r3, #0]
	ldrb	r2, [r2, #0]
	lsls	r3, r3, #8
	orrs	r2, r3
	ldrb	r3, [r4, #16]
	lsls	r3, r3, #2
	adds	r3, r3, r6
	ldrh	r3, [r3, #2]
	adds	r1, r1, r3
	adds	r1, r1, r0
	movs	r0, #1
	add	r9, r0
	strh	r2, [r1, r5]
	mov	r1, r9
	cmp	r1, #64
	bne.n	.L_08176e54
.L_08176eb4:
	ldr	r3, [sp, #80]
	add	r1, sp, #200
	lsls	r2, r3, #11
	movs	r3, #155
	lsls	r3, r3, #12
	subs	r3, r3, r2
	add	r2, sp, #136
	str	r3, [sp, #136]
	str	r3, [r2, #4]
	movs	r3, #0
	str	r3, [r1, #12]
	movs	r3, #255
	lsls	r3, r3, #16
	str	r3, [r1, #4]
	movs	r3, #184
	lsls	r3, r3, #15
	str	r3, [r1, #0]
	movs	r3, #144
	lsls	r3, r3, #15
	str	r3, [r1, #8]
	ldr	r4, [sp, #92]
	movs	r5, #240
	lsls	r5, r5, #7
	adds	r5, #16
	adds	r3, r4, r5
	ldr	r0, [r3, #0]
	movs	r3, #0
	bl	sub_08020010
	ldr	r0, [pc, #80]
	bl	sub_08013300
	ldr	r6, [pc, #76]
	ldr	r1, [pc, #16]
	movs	r7, #0
	movs	r2, #31
	mov	lr, r6
	mov	r9, r7
	mov	ip, r1
	mov	r8, r2
	b.n	.L_08176f48
	movs	r0, r0
	.4byte 0x0000001f
	.4byte 0x000000bd
	.4byte 0x03000730
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x08199364
	.4byte 0xfff80000
	.4byte 0xfff00000
	.4byte 0x08199210
	.4byte 0x08199340
	.4byte 0x08196eb8
	.4byte 0x020036e0
	.4byte 0x06010000
	.4byte 0x02014000
	.4byte 0x0000009d
	.2byte 0x0380
	.2byte 0x0500
.L_08176f48:
	mov	r3, lr
	ldrh	r2, [r3, #0]
	ldrh	r1, [r0, #0]
	lsls	r3, r2, #16
	lsrs	r7, r3, #26
	lsrs	r5, r3, #21
	mov	r6, r8
	lsls	r3, r1, #16
	mov	r4, ip
	ands	r6, r2
	lsrs	r2, r3, #26
	lsrs	r3, r3, #21
	ands	r7, r4
	ands	r5, r4
	ands	r2, r4
	ands	r3, r4
	mov	r4, r8
	ands	r4, r1
	cmp	r7, r2
	bge.n	.L_08176f72
	adds	r7, #1
.L_08176f72:
	cmp	r5, r3
	bge.n	.L_08176f78
	adds	r5, #1
.L_08176f78:
	cmp	r6, r4
	bge.n	.L_08176f7e
	adds	r6, #1
.L_08176f7e:
	lsls	r3, r7, #10
	lsls	r2, r5, #5
	orrs	r3, r2
	orrs	r3, r6
	movs	r6, #1
	add	r9, r6
	mov	r5, lr
	movs	r7, #2
	mov	r1, r9
	strh	r3, [r5, #0]
	adds	r0, #2
	add	lr, r7
	cmp	r1, #32
	bne.n	.L_08176f48
.L_08176f9a:
	ldr	r2, [sp, #80]
	movs	r3, #46
	adds	r3, #255
	cmp	r2, r3
	bgt.n	.L_08176fa6
	b.n	.L_081771ca
.L_08176fa6:
	movs	r4, #151
	lsls	r4, r4, #1
	cmp	r2, r4
	bne.n	.L_08177008
	movs	r1, #240
	ldr	r0, [sp, #96]
	lsls	r1, r1, #6
	ldr	r2, [pc, #124]
	ldr	r5, [pc, #128]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2140
	ldr	r2, [pc, #124]
	ldr	r0, [pc, #124]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x481f
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	lsls	r0, r0, #19
	movs	r2, #128
	lsls	r2, r2, #1
	ldr	r6, [pc, #112]
	adds	r0, #192
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x481b
	bl	sub_08013300
	adds	r7, r0, #0
	movs	r0, #160
	adds	r1, r7, #0
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x9f17
	movs	r2, #184
	lsls	r2, r2, #5
	adds	r1, r7, r2
	ldr	r0, [pc, #84]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	bl	sub_0815b410
.L_08177008:
	ldr	r3, [sp, #80]
	movs	r4, #48
	adds	r4, #255
	cmp	r3, r4
	ble.n	.L_081770c6
	movs	r5, #160
	ldr	r7, [pc, #24]
	ldr	r0, [pc, #44]
	lsls	r5, r5, #19
	bl	sub_08013300
	adds	r5, #192
	movs	r6, #0
	movs	r1, #31
	mov	lr, r5
	mov	r9, r6
	mov	ip, r7
	mov	r8, r1
	b.n	.L_08177054
	movs	r0, r0
	.4byte 0x0000001f
	.4byte 0x3f3f3f3f
	.4byte 0x03000260
	.4byte 0x7fff7fff
	.4byte 0x05000380
	.4byte 0x00000075
	.4byte 0x03000730
	.4byte 0x000000bd
	.2byte 0x00c2
	.2byte 0x0000
.L_08177054:
	mov	r3, lr
	ldrh	r2, [r3, #0]
	mov	r4, ip
	lsls	r3, r2, #16
	lsrs	r6, r3, #26
	ldrh	r1, [r0, #0]
	mov	r5, ip
	ands	r6, r4
	lsrs	r4, r3, #21
	ands	r4, r5
	mov	r5, r8
	ands	r5, r2
	lsls	r2, r1, #16
	lsrs	r3, r2, #26
	mov	r7, ip
	ands	r3, r7
	adds	r7, r3, #0
	lsrs	r2, r2, #21
	mov	r3, ip
	ands	r2, r3
	mov	r3, r8
	ands	r3, r1
	subs	r7, #20
	subs	r2, #20
	adds	r3, #8
	cmp	r7, #0
	bge.n	.L_0817708c
	movs	r7, #0
.L_0817708c:
	cmp	r2, #0
	bge.n	.L_08177092
	movs	r2, #0
.L_08177092:
	cmp	r3, #31
	ble.n	.L_08177098
	movs	r3, #31
.L_08177098:
	cmp	r6, r7
	ble.n	.L_0817709e
	subs	r6, #1
.L_0817709e:
	cmp	r4, r2
	ble.n	.L_081770a4
	subs	r4, #1
.L_081770a4:
	cmp	r5, r3
	ble.n	.L_081770aa
	subs	r5, #1
.L_081770aa:
	lsls	r3, r6, #10
	lsls	r2, r4, #5
	orrs	r3, r2
	orrs	r3, r5
	movs	r5, #1
	add	r9, r5
	mov	r4, lr
	movs	r6, #2
	mov	r7, r9
	strh	r3, [r4, #0]
	adds	r0, #2
	add	lr, r6
	cmp	r7, #128
	bne.n	.L_08177054
.L_081770c6:
	ldr	r0, [sp, #80]
	movs	r1, #46
	adds	r1, #255
	cmp	r0, r1
	ble.n	.L_081771ca
	movs	r0, #32
	bl	sub_08014dac
	mov	fp, r0
	movs	r0, #1
	bl	sub_081969f8
	ldr	r2, [pc, #564]
	ldr	r3, [sp, #128]
	movs	r1, #7
	ands	r3, r2
	ldr	r2, [pc, #560]
	orrs	r3, r1
	ands	r3, r2
	movs	r2, #224
	lsls	r2, r2, #3
	orrs	r3, r2
	ldr	r2, [sp, #92]
	movs	r4, #184
	lsls	r4, r4, #5
	str	r3, [sp, #128]
	adds	r3, r2, r4
	add	r2, sp, #128
	str	r3, [r2, #4]
	ldr	r3, [pc, #536]
	adds	r6, r0, #0
	mov	r5, fp
	str	r1, [r6, #0]
	str	r2, [r6, #16]
	str	r3, [r6, #8]
	str	r5, [r6, #12]
	ldr	r0, [sp, #80]
	ldr	r1, [pc, #524]
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r2, r3, #7
	adds	r3, r3, r2
	ldr	r2, [pc, #520]
	lsls	r3, r3, #4
	adds	r1, r1, r3
	lsls	r3, r0, #2
	adds	r2, r2, r3
	ldr	r3, [pc, #512]
	movs	r7, #0
	mov	r9, r7
	mov	sl, r1
	mov	r8, r2
	adds	r7, r0, r3
.L_08177130:
	cmp	r7, #0
	blt.n	.L_081771a8
	ldr	r3, [pc, #500]
	mov	r4, sl
	mov	r0, r8
	subs	r5, r3, r4
	subs	r0, #16
	cmp	r5, #0
	bge.n	.L_08177144
	movs	r5, #0
.L_08177144:
	cmp	r0, #0
	ble.n	.L_0817714a
	movs	r0, #0
.L_0817714a:
	str	r0, [r6, #20]
	bl	sub_08014de4
	ldr	r1, [pc, #476]
	movs	r2, #0
	adds	r0, r1, #0
	bl	sub_08015160
	lsrs	r2, r5, #31
	adds	r2, r5, r2
	asrs	r2, r2, #1
	adds	r1, r5, #0
	adds	r0, r2, #0
	mov	r5, r9
	bl	sub_080151e4
	cmp	r5, #0
	bne.n	.L_08177178
	movs	r0, #240
	lsls	r0, r0, #8
	bl	sub_080150e4
	b.n	.L_08177190
.L_08177178:
	mov	r0, r9
	cmp	r0, #1
	bne.n	.L_08177188
	movs	r0, #128
	lsls	r0, r0, #6
	bl	sub_080150e4
	b.n	.L_08177190
.L_08177188:
	movs	r0, #224
	lsls	r0, r0, #8
	bl	sub_080150e4
.L_08177190:
	movs	r0, #128
	lsls	r0, r0, #4
	bl	sub_08015024
	ldr	r0, [pc, #408]
	mov	r1, fp
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r6, #0
	bl	sub_08196a7c
.L_081771a8:
	ldr	r1, [pc, #396]
	movs	r3, #1
	movs	r2, #32
	add	r9, r3
	negs	r2, r2
	mov	r4, r9
	add	sl, r1
	add	r8, r2
	subs	r7, #8
	cmp	r4, #3
	bne.n	.L_08177130
	adds	r0, r6, #0
	bl	sub_08013164
	mov	r0, fp
	bl	sub_08013164
.L_081771ca:
	ldr	r5, [sp, #80]
	movs	r6, #165
	lsls	r6, r6, #1
	cmp	r5, r6
	bne.n	sub_081771e0
	ldr	r0, [pc, #356]
	movs	r1, #64
	ldr	r2, [pc, #356]
	ldr	r7, [pc, #360]
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x9814
	ldr	r1, [pc, #356]
	adds	r3, r0, r1
	cmp	r3, #20
	bhi.n	.L_08177254
	ldr	r3, [pc, #352]
	ldr	r2, [sp, #80]
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	movs	r0, #0
	str	r3, [sp, #120]
	str	r4, [sp, #124]
	movs	r3, #173
	lsls	r3, r3, #1
	movs	r4, #0
	cmp	r2, r3
	ble.n	.L_08177214
	ldr	r5, [pc, #332]
	adds	r3, #1
	subs	r4, r3, r2
	adds	r3, r2, r5
	cmp	r3, #0
	bge.n	.L_08177212
	ldr	r6, [pc, #324]
	adds	r3, r2, r6
.L_08177212:
	asrs	r0, r3, #2
.L_08177214:
	ldr	r7, [sp, #80]
	ldr	r1, [pc, #320]
	lsls	r3, r7, #11
	adds	r3, r3, r1
	add	r2, sp, #120
	str	r3, [r2, #4]
	add	r1, sp, #184
	str	r3, [sp, #120]
	movs	r3, #0
	str	r3, [r1, #12]
	movs	r3, #255
	lsls	r3, r3, #16
	str	r3, [r1, #4]
	lsls	r3, r4, #16
	movs	r4, #184
	lsls	r4, r4, #15
	adds	r3, r3, r4
	movs	r5, #144
	str	r3, [r1, #0]
	lsls	r5, r5, #15
	lsls	r3, r0, #16
	adds	r3, r3, r5
	str	r3, [r1, #8]
	ldr	r6, [sp, #92]
	movs	r7, #240
	lsls	r7, r7, #7
	adds	r7, #16
	adds	r3, r6, r7
	ldr	r0, [r3, #0]
	movs	r3, #0
	bl	sub_08020010
.L_08177254:
	ldr	r0, [sp, #80]
	movs	r1, #92
	adds	r1, #255
	cmp	r0, r1
	bne.n	.L_081772ae
	ldr	r3, [sp, #92]
	movs	r4, #239
	lsls	r4, r4, #7
	adds	r2, r3, r4
	movs	r3, #1
	str	r3, [r2, #0]
	ldr	r5, [sp, #92]
	movs	r6, #238
	lsls	r6, r6, #7
	adds	r6, #132
	movs	r2, #0
	adds	r3, r5, r6
	str	r2, [r3, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #80
	movs	r1, #160
	strh	r2, [r3, #0]
	lsls	r1, r1, #19
	adds	r1, #2
	mov	r9, r2
	movs	r0, #31
.L_0817728a:
	mov	r7, r9
	subs	r2, r0, r7
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L_08177296
	movs	r2, #0
.L_08177296:
	cmp	r3, #0
	bge.n	.L_0817729c
	movs	r3, #0
.L_0817729c:
	lsls	r3, r3, #10
	orrs	r3, r2
	movs	r2, #1
	add	r9, r2
	strh	r3, [r1, #0]
	mov	r3, r9
	adds	r1, #2
	cmp	r3, #63
	bne.n	.L_0817728a
.L_081772ae:
	ldr	r4, [sp, #80]
	movs	r5, #173
	lsls	r5, r5, #1
	cmp	r4, r5
	ble.n	.L_08177392
	ldr	r6, [pc, #160]
	movs	r7, #94
	lsls	r3, r4, #2
	adds	r7, #255
	adds	r5, r3, r6
	movs	r6, #54
	cmp	r4, r7
	ble.n	.L_081772de
	ldr	r1, [pc, #148]
	lsls	r0, r4, #10
	adds	r0, r0, r1
	bl	sub_08002096
	adds	r3, r0, #0
	muls	r3, r6
	negs	r3, r3
	asrs	r3, r3, #16
	adds	r6, r3, #0
	adds	r6, #54
.L_081772de:
	ldr	r2, [sp, #80]
	movs	r3, #180
	lsls	r3, r3, #1
	cmp	r2, r3
	bne.n	.L_081772f4
	ldr	r4, [sp, #92]
	movs	r7, #239
	lsls	r7, r7, #7
	adds	r2, r4, r7
	movs	r3, #0
	str	r3, [r2, #0]
.L_081772f4:
	ldr	r0, [sp, #80]
	movs	r1, #116
	adds	r1, #255
	cmp	r0, r1
	bgt.n	.L_08177364
	ldr	r2, [sp, #92]
	movs	r3, #184
	lsls	r3, r3, #5
	adds	r0, r2, r3
	movs	r2, #128
	adds	r1, r5, #0
	lsls	r2, r2, #9
	bl	sub_0815b434
	b.n	.L_08177366
	movs	r0, r0
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x08199364
	.4byte 0xffe37760
	.4byte 0xfffffb48
	.4byte 0xfffffed2
	.4byte 0x000222e0
	.4byte 0xfff80000
	.4byte 0x08199210
	.4byte 0xffff3e80
	.4byte 0x05000380
	.4byte 0x7c1f7c1f
	.4byte 0x03000260
	.4byte 0xfffffeb6
	.4byte 0x08196ec0
	.4byte 0xfffffea5
	.4byte 0xfffffea8
	.4byte 0xfff5b000
	.4byte 0xfffffa9c
	.2byte 0x8800
	.2byte 0xfffa
.L_08177364:
	.2byte 0x2568
.L_08177366:
	movs	r0, #188
	bl	sub_0801314c
	ldr	r4, [sp, #92]
	movs	r7, #184
	lsls	r7, r7, #5
	adds	r0, r4, r7
	adds	r3, r5, #0
	adds	r1, r6, #0
	movs	r2, #58
	bl	sub_0818caa8
	movs	r0, #188
	movs	r1, #3
	bl	sub_081963ec
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #188
	ldr	r3, [r3, #0]
	ldr	r0, [sp, #64]
	str	r3, [r0, #4]
.L_08177392:
	ldr	r2, [sp, #80]
	ldr	r3, [pc, #72]
	movs	r1, #0
	str	r1, [sp, #84]
	adds	r2, r2, r3
	mov	r8, r2
.L_0817739e:
	mov	r4, r8
	cmp	r4, #0
	bne.n	sub_081773b6
	movs	r0, #160
	lsls	r0, r0, #19
	movs	r1, #128
	adds	r0, #192
	lsls	r1, r1, #1
	ldr	r2, [pc, #52]
	ldr	r5, [pc, #52]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4646
	cmp	r6, #159
	bhi.n	.L_08177488
	ldr	r0, [pc, #44]
	bl	sub_08013300
	movs	r7, #160
	ldr	r1, [pc, #20]
	lsls	r7, r7, #19
	mov	ip, r0
	adds	r7, #192
	movs	r0, #0
	movs	r2, #31
	mov	sl, r7
	mov	r9, r0
	mov	lr, r1
	mov	fp, r2
	b.n	.L_081773f0
	movs	r0, r0
	.4byte 0x0000001f
	.4byte 0xfffffe9b
	.4byte 0x7fff7fff
	.4byte 0x03000260
	.2byte 0x0075
	.2byte 0x0000
.L_081773f0:
	mov	r3, sl
	ldrh	r2, [r3, #0]
	mov	r4, lr
	lsls	r3, r2, #16
	lsrs	r0, r3, #26
	lsrs	r6, r3, #21
	mov	r7, fp
	mov	r5, r8
	ands	r0, r4
	ands	r6, r4
	ands	r7, r2
	cmp	r5, #7
	bgt.n	.L_08177420
	mov	r1, ip
	ldrh	r2, [r1, #0]
	mov	r1, fp
	lsls	r3, r2, #16
	lsrs	r5, r3, #26
	ands	r5, r4
	lsrs	r4, r3, #21
	mov	r3, lr
	ands	r4, r3
	ands	r1, r2
	b.n	.L_08177442
.L_08177420:
	mov	r4, ip
	ldrh	r1, [r4, #0]
	mov	r5, lr
	lsls	r2, r1, #16
	lsrs	r3, r2, #26
	ands	r3, r5
	adds	r5, r3, #0
	lsrs	r2, r2, #21
	mov	r3, lr
	ands	r2, r3
	mov	r3, fp
	ands	r3, r1
	adds	r4, r2, #0
	adds	r1, r3, #0
	subs	r5, #16
	subs	r4, #16
	adds	r1, #8
.L_08177442:
	cmp	r5, #0
	bge.n	.L_08177448
	movs	r5, #0
.L_08177448:
	cmp	r4, #0
	bge.n	.L_0817744e
	movs	r4, #0
.L_0817744e:
	cmp	r1, #31
	ble.n	.L_08177454
	movs	r1, #31
.L_08177454:
	adds	r3, r5, #1
	cmp	r0, r3
	ble.n	.L_0817745c
	subs	r0, #2
.L_0817745c:
	adds	r3, r4, #1
	cmp	r6, r3
	ble.n	.L_08177464
	subs	r6, #2
.L_08177464:
	adds	r3, r1, #1
	cmp	r7, r3
	ble.n	.L_0817746c
	subs	r7, #2
.L_0817746c:
	lsls	r2, r6, #5
	lsls	r3, r0, #10
	movs	r5, #1
	orrs	r3, r2
	add	r9, r5
	orrs	r3, r7
	mov	r4, sl
	movs	r6, #2
	mov	r7, r9
	strh	r3, [r4, #0]
	add	ip, r6
	add	sl, r6
	cmp	r7, #128
	bne.n	.L_081773f0
.L_08177488:
	ldr	r1, [sp, #84]
	movs	r0, #19
	negs	r0, r0
	adds	r1, #1
	add	r8, r0
	str	r1, [sp, #84]
	cmp	r1, #2
	beq.n	.L_0817749a
	b.n	.L_0817739e
.L_0817749a:
	ldr	r2, [sp, #52]
	cmp	r2, #23
	bhi.n	.L_0817750c
	ldr	r0, [sp, #20]
	ldr	r7, [sp, #20]
	subs	r0, #114
	movs	r1, #6
	subs	r7, #104
	bl	sub_08002054
	cmp	r0, #0
	bge.n	.L_081774b4
	movs	r0, #0
.L_081774b4:
	movs	r3, #0
	movs	r4, #8
	lsls	r0, r0, #5
	movs	r5, #2
	mov	r9, r3
	mov	fp, r4
	mov	r8, r0
	mov	sl, r5
.L_081774c4:
	mov	r0, r9
	lsls	r6, r0, #8
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r5, r7, #0
	muls	r5, r0
	adds	r0, r6, #0
	bl	sub_08002090
	lsls	r5, r5, #1
	asrs	r5, r5, #16
	adds	r3, r7, #0
	muls	r3, r0
	mov	r1, sl
	adds	r5, #66
	subs	r5, r5, r1
	ldr	r1, [sp, #72]
	movs	r2, #4
	str	r2, [sp, #0]
	mov	r4, fp
	adds	r2, r5, #0
	asrs	r3, r3, #16
	movs	r5, #1
	movs	r6, #128
	str	r4, [sp, #4]
	adds	r3, #68
	ldr	r4, [sp, #176]
	ldr	r0, [sp, #96]
	add	r1, r8
	add	r9, r5
	lsls	r6, r6, #1
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x45b1
	bne.n	.L_081774c4
.L_0817750c:
	ldr	r3, [sp, #80]
	subs	r3, #108
	cmp	r3, #23
	bhi.n	.L_0817757c
	movs	r0, #160
	ldr	r4, [pc, #40]
	movs	r7, #0
	lsls	r0, r0, #19
	mov	r9, r7
	adds	r0, #192
.L_08177520:
	ldrh	r3, [r0, #0]
	movs	r1, #31
	ands	r1, r3
	lsls	r3, r3, #16
	lsrs	r2, r3, #21
	ands	r2, r4
	lsrs	r3, r3, #26
	subs	r2, #1
	ands	r3, r4
	cmp	r2, #0
	bge.n	.L_08177538
	subs	r1, #1
.L_08177538:
	cmp	r1, #31
	ble.n	.L_08177544
	movs	r1, #31
	b.n	.L_0817754a
	.2byte 0x001f
	.2byte 0x0000
.L_08177544:
	cmp	r1, #0
	bge.n	.L_0817754a
	movs	r1, #0
.L_0817754a:
	cmp	r2, #31
	ble.n	.L_08177552
	movs	r2, #31
	b.n	.L_08177558
.L_08177552:
	cmp	r2, #0
	bge.n	.L_08177558
	movs	r2, #0
.L_08177558:
	cmp	r3, #31
	ble.n	.L_08177560
	movs	r3, #31
	b.n	.L_08177566
.L_08177560:
	cmp	r3, #0
	bge.n	.L_08177566
	movs	r3, #0
.L_08177566:
	lsls	r2, r2, #5
	lsls	r3, r3, #10
	orrs	r3, r2
	orrs	r3, r1
	movs	r1, #1
	add	r9, r1
	mov	r2, r9
	strh	r3, [r0, #0]
	adds	r0, #2
	cmp	r2, #128
	bne.n	.L_08177520
.L_0817757c:
	ldr	r3, [sp, #52]
	cmp	r3, #35
	bls.n	.L_08177584
	b.n	.L_081776d0
.L_08177584:
	ldr	r4, [sp, #80]
	cmp	r4, #56
	bne.n	.L_0817764e
	ldr	r5, [sp, #92]
	movs	r6, #239
	lsls	r6, r6, #7
	adds	r2, r5, r6
	movs	r3, #3
	movs	r7, #238
	str	r3, [r2, #0]
	lsls	r7, r7, #7
	ldr	r3, [pc, #256]
	adds	r7, #132
	adds	r2, r5, r7
	str	r3, [r2, #0]
	ldr	r0, [pc, #252]
	bl	sub_08013300
	movs	r2, #172
	adds	r7, r0, #0
	adds	r7, #32
	lsls	r2, r2, #6
	adds	r0, r7, #0
	adds	r1, r5, r2
	bl	sub_0801587c
	movs	r3, #7
	movs	r4, #0
	movs	r7, #0
	mov	sl, r3
	mov	lr, r4
.L_081775c2:
	ldr	r3, [sp, #92]
	movs	r5, #172
	lsls	r5, r5, #6
	add	r3, lr
	adds	r5, #1
	adds	r5, r5, r3
	movs	r6, #0
	mov	r8, lr
	mov	ip, r5
.L_081775d4:
	adds	r3, r7, #0
	cmp	r7, #0
	bge.n	.L_081775dc
	adds	r3, r7, #7
.L_081775dc:
	asrs	r3, r3, #3
	lsls	r3, r3, #5
	adds	r2, r6, #0
	cmp	r6, #0
	bge.n	.L_081775e8
	adds	r2, r6, #7
.L_081775e8:
	asrs	r2, r2, #3
	mov	r0, sl
	adds	r2, r3, r2
	adds	r3, r7, #0
	ands	r3, r0
	lsls	r2, r2, #3
	adds	r2, r2, r3
	ldr	r1, [pc, #172]
	adds	r3, r6, #0
	ands	r3, r0
	lsls	r2, r2, #3
	adds	r2, r2, r3
	adds	r2, r2, r1
	ldrh	r3, [r2, #0]
	lsrs	r5, r3, #8
	lsls	r3, r3, #24
	lsrs	r4, r3, #24
	mov	r3, r8
	adds	r0, r3, r6
	mov	r3, ip
	ldrb	r1, [r3, #0]
	adds	r3, r1, #0
	cmp	r3, #0
	beq.n	.L_0817761e
	adds	r3, #224
	lsls	r3, r3, #24
	lsrs	r5, r3, #24
.L_0817761e:
	movs	r1, #172
	lsls	r1, r1, #6
	adds	r3, r0, r1
	ldr	r0, [sp, #92]
	ldrb	r1, [r0, r3]
	adds	r3, r1, #0
	cmp	r3, #0
	beq.n	.L_08177634
	adds	r3, #224
	lsls	r3, r3, #24
	lsrs	r4, r3, #24
.L_08177634:
	lsls	r3, r5, #8
	orrs	r3, r4
	movs	r1, #2
	adds	r6, #2
	strh	r3, [r2, #0]
	add	ip, r1
	cmp	r6, #184
	bne.n	.L_081775d4
	movs	r2, #184
	adds	r7, #1
	add	lr, r2
	cmp	r7, #80
	bne.n	.L_081775c2
.L_0817764e:
	ldr	r3, [sp, #80]
	cmp	r3, #87
	bgt.n	.L_081776d0
	ldr	r0, [pc, #72]
	bl	sub_08013300
	ldr	r7, [sp, #52]
	ldr	r6, [pc, #72]
	lsls	r7, r7, #9
	mov	r8, r7
	ldr	r7, [pc, #52]
	movs	r4, #0
	adds	r5, r0, #0
	mov	r9, r4
.L_0817766a:
	mov	r0, r8
	bl	sub_08002090
	lsls	r2, r0, #1
	adds	r2, r2, r0
	ldrh	r1, [r5, #0]
	lsls	r2, r2, #2
	asrs	r2, r2, #16
	movs	r3, #31
	adds	r2, #16
	ands	r3, r1
	lsls	r1, r1, #16
	adds	r4, r3, r2
	lsrs	r3, r1, #21
	lsrs	r1, r1, #26
	ands	r3, r7
	ands	r1, r7
	adds	r0, r3, r2
	adds	r1, r1, r2
	cmp	r4, #31
	ble.n	.L_081776ac
	movs	r4, #31
	b.n	.L_081776ac
	.4byte 0x0000001f
	.4byte 0x04040404
	.4byte 0x000000b5
	.4byte 0x0600a900
	.2byte 0x01c0
	.2byte 0x0500
.L_081776ac:
	cmp	r0, #31
	ble.n	.L_081776b2
	movs	r0, #31
.L_081776b2:
	cmp	r1, #31
	ble.n	.L_081776b8
	movs	r1, #31
.L_081776b8:
	lsls	r3, r1, #10
	lsls	r2, r0, #5
	movs	r0, #1
	orrs	r3, r2
	add	r9, r0
	orrs	r3, r4
	mov	r1, r9
	strh	r3, [r6, #0]
	adds	r5, #2
	adds	r6, #2
	cmp	r1, #16
	bne.n	.L_0817766a
.L_081776d0:
	ldr	r2, [sp, #80]
	cmp	r2, #54
	bne.n	sub_081776e4
	movs	r1, #240
	ldr	r0, [sp, #96]
	lsls	r1, r1, #6
	ldr	r2, [pc, #624]
	ldr	r3, [pc, #628]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9c14
	cmp	r4, #55
	bne.n	sub_081776f8
	movs	r1, #240
	ldr	r0, [sp, #96]
	lsls	r1, r1, #6
	ldr	r2, [pc, #612]
	ldr	r5, [pc, #608]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x9e14
	cmp	r6, #131
	ble.n	.L_08177700
	b.n	.L_081778f4
.L_08177700:
	movs	r0, #32
	bl	sub_08014dac
	mov	sl, r0
	movs	r0, #1
	bl	sub_081969f8
	adds	r7, r0, #0
	ldr	r0, [sp, #92]
	movs	r1, #239
	lsls	r1, r1, #7
	adds	r2, r0, r1
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #132
	adds	r2, r0, r3
	movs	r3, #50
	str	r3, [r2, #0]
	ldr	r4, [sp, #20]
	ldr	r0, [sp, #80]
	adds	r2, r4, r6
	lsls	r3, r2, #7
	subs	r3, r3, r2
	adds	r5, r4, #0
	lsls	r4, r3, #2
	ldr	r3, [pc, #548]
	subs	r5, #64
	subs	r6, r3, r4
	cmp	r0, #55
	ble.n	.L_08177744
	ldr	r1, [pc, #540]
	adds	r6, r4, r1
.L_08177744:
	movs	r2, #8
	negs	r2, r2
	cmp	r5, r2
	blt.n	.L_08177750
	movs	r5, #8
	negs	r5, r5
.L_08177750:
	ldr	r3, [sp, #80]
	cmp	r3, #95
	ble.n	.L_0817775e
	ldr	r4, [sp, #20]
	movs	r3, #184
	subs	r5, r3, r4
	str	r5, [r7, #20]
.L_0817775e:
	ldr	r3, [sp, #112]
	ldr	r2, [pc, #512]
	ldr	r0, [sp, #92]
	ands	r3, r2
	movs	r2, #6
	orrs	r3, r2
	ldr	r2, [pc, #508]
	movs	r1, #224
	ands	r3, r2
	movs	r2, #192
	lsls	r2, r2, #3
	orrs	r3, r2
	lsls	r1, r1, #3
	add	r2, sp, #112
	str	r3, [sp, #112]
	adds	r3, r0, r1
	str	r3, [r2, #4]
	movs	r3, #7
	str	r3, [r7, #0]
	ldr	r3, [pc, #484]
	movs	r4, #48
	str	r3, [r7, #8]
	negs	r4, r4
	mov	r3, sl
	mov	r8, r2
	str	r2, [r7, #16]
	str	r3, [r7, #12]
	cmp	r5, r4
	ble.n	.L_081777e4
	str	r5, [r7, #20]
	bl	sub_08014de4
	movs	r0, #128
	lsls	r0, r0, #11
	movs	r1, #0
	movs	r2, #0
	bl	sub_08015160
	adds	r0, r6, #0
	adds	r1, r6, #0
	adds	r2, r6, #0
	bl	sub_080151e4
	movs	r0, #128
	lsls	r0, r0, #6
	bl	sub_08015024
	ldr	r6, [sp, #80]
	cmp	r6, #55
	ble.n	.L_081777ca
	lsls	r0, r6, #10
	bl	sub_08015068
	b.n	.L_081777d4
.L_081777ca:
	ldr	r1, [sp, #80]
	negs	r0, r1
	lsls	r0, r0, #10
	bl	sub_08015068
.L_081777d4:
	ldr	r0, [pc, #408]
	mov	r1, sl
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
.L_081777e4:
	ldr	r2, [sp, #80]
	cmp	r2, #95
	ble.n	.L_081777f8
	ldr	r4, [sp, #20]
	adds	r3, r4, r2
	movs	r2, #142
	lsls	r3, r3, #1
	lsls	r2, r2, #2
	subs	r5, r2, r3
	str	r5, [r7, #20]
.L_081777f8:
	movs	r6, #48
	negs	r6, r6
	cmp	r5, r6
	ble.n	.L_08177838
	bl	sub_08014de4
	movs	r0, #128
	ldr	r1, [pc, #364]
	movs	r2, #0
	lsls	r0, r0, #11
	bl	sub_08015160
	ldr	r0, [pc, #356]
	bl	sub_0801521c
	movs	r0, #128
	lsls	r0, r0, #6
	bl	sub_08015024
	ldr	r5, [sp, #80]
	negs	r0, r5
	lsls	r0, r0, #9
	bl	sub_08015068
	ldr	r0, [pc, #324]
	mov	r1, sl
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
.L_08177838:
	ldr	r0, [sp, #80]
	cmp	r0, #55
	ble.n	.L_081778d6
	ldr	r2, [sp, #20]
	ldr	r1, [sp, #92]
	movs	r4, #184
	lsls	r4, r4, #5
	movs	r0, #32
	adds	r3, r1, r4
	mov	r5, r8
	subs	r2, #208
	negs	r0, r0
	str	r3, [r5, #4]
	cmp	r2, r0
	blt.n	.L_0817785a
	movs	r2, #32
	negs	r2, r2
.L_0817785a:
	str	r2, [r7, #20]
	bl	sub_08014de4
	movs	r0, #128
	movs	r2, #0
	ldr	r1, [pc, #276]
	lsls	r0, r0, #10
	bl	sub_08015160
	movs	r0, #176
	lsls	r0, r0, #7
	adds	r0, #240
	bl	sub_0801521c
	movs	r0, #128
	lsls	r0, r0, #6
	bl	sub_08015024
	ldr	r1, [sp, #80]
	lsls	r0, r1, #10
	bl	sub_08015068
	ldr	r5, [pc, #232]
	mov	r1, sl
	adds	r0, r5, #0
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
	ldr	r4, [sp, #80]
	cmp	r4, r6
	ble.n	.L_081778d6
	bl	sub_08014de4
	movs	r0, #128
	ldr	r1, [pc, #216]
	movs	r2, #0
	lsls	r0, r0, #10
	bl	sub_08015160
	ldr	r0, [pc, #212]
	bl	sub_0801521c
	movs	r0, #128
	lsls	r0, r0, #6
	bl	sub_08015024
	ldr	r6, [sp, #80]
	negs	r0, r6
	lsls	r0, r0, #10
	bl	sub_08015068
	adds	r0, r5, #0
	mov	r1, sl
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
.L_081778d6:
	adds	r0, r7, #0
	bl	sub_08013164
	mov	r0, sl
	bl	sub_08013164
	movs	r0, #240
	ldr	r7, [sp, #92]
	lsls	r0, r0, #7
	adds	r0, #228
	adds	r2, r7, r0
.L_081778ec:
	ldr	r3, [r2, #0]
	ldr	r3, [r2, #0]
	cmp	r3, #1
	bls.n	.L_081778ec
.L_081778f4:
	ldr	r1, [sp, #92]
	movs	r3, #240
	lsls	r3, r3, #7
	adds	r3, #232
	adds	r2, r1, r3
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r4, [sp, #20]
	ldr	r5, [sp, #16]
	ldr	r6, [sp, #80]
	movs	r7, #197
	adds	r4, #2
	adds	r5, #2
	adds	r6, #1
	lsls	r7, r7, #1
	str	r4, [sp, #20]
	str	r5, [sp, #16]
	str	r6, [sp, #80]
	cmp	r6, r7
	beq.n	.L_08177932
	ldr	r3, [pc, #100]
	movs	r2, #3
	ldr	r3, [r3, #12]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08177932
	bl	.L_08176320
.L_08177932:
	movs	r0, #162
	bl	sub_081c0010
	ldr	r0, [sp, #80]
	cmp	r0, #131
	bgt.n	.L_0817798c
	ldr	r1, [sp, #92]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #220
	adds	r3, r1, r2
	ldr	r0, [r3, #0]
	bl	sub_08020048
	b.n	.L_081779aa
	.4byte 0x20202020
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.4byte 0x0001c350
	.4byte 0xffff32a0
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x08199340
	.4byte 0x08199210
	.4byte 0xfff00000
	.4byte 0xffff58f0
	.4byte 0xffe00000
	.4byte 0xffd00000
	.4byte 0xffffce20
	.2byte 0x1150
	.2byte 0x0300
.L_0817798c:
	ldr	r4, [sp, #92]
	movs	r6, #238
	lsls	r6, r6, #7
	movs	r3, #0
	adds	r6, #220
	mov	r9, r3
	adds	r5, r4, r6
.L_0817799a:
	movs	r7, #1
	ldmia	r5!, {r0}
	add	r9, r7
	bl	sub_08020048
	mov	r0, r9
	cmp	r0, #14
	bne.n	.L_0817799a
.L_081779aa:
	ldr	r5, [pc, #96]
	movs	r1, #0
	mov	r9, r1
.L_081779b0:
	ldmia	r5!, {r0}
	bl	sub_08020048
	movs	r2, #1
	add	r9, r2
	mov	r3, r9
	cmp	r3, #48
	bne.n	.L_081779b0
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08014c4c
	ldr	r3, [pc, #52]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r5, #237
	ldr	r4, [sp, #68]
	lsls	r5, r5, #3
	adds	r5, #255
	adds	r2, r4, r5
	movs	r3, #0
	strb	r3, [r2, #0]
	movs	r6, #206
	lsls	r6, r6, #3
	adds	r3, r4, r6
	ldrh	r1, [r3, #0]
	movs	r0, #1
	movs	r2, #24
	bl	sub_08118040
	ldr	r5, [pc, #16]
	movs	r1, #128
	ldr	r0, [sp, #96]
	lsls	r1, r1, #7
	movs	r2, #0
	b.n	.L_08177a14
	.4byte 0x00000141
	.4byte 0x02010000
	.2byte 0x0260
	.2byte 0x0300
.L_08177a14:
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x481d
	movs	r1, #128
	lsls	r1, r1, #7
	movs	r2, #0
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x4a1b
	movs	r3, #120
	str	r3, [r2, #16]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #48]
	movs	r7, #0
	str	r3, [sp, #36]
	str	r7, [sp, #32]
	add	r2, sp, #32
	movs	r1, #54
	ldrsh	r0, [r3, r1]
	movs	r4, #239
	str	r0, [sp, #28]
	lsls	r4, r4, #7
	ldrh	r2, [r2, #0]
	movs	r6, #238
	strh	r2, [r3, #54]
	ldr	r3, [sp, #92]
	lsls	r6, r6, #7
	adds	r1, r3, r4
	movs	r3, #1
	str	r3, [r1, #0]
	ldr	r5, [sp, #92]
	adds	r6, #132
	adds	r2, r5, r6
	movs	r3, #2
	str	r7, [r2, #0]
	str	r3, [r1, #0]
	movs	r3, #50
	str	r3, [r2, #0]
	ldr	r3, [pc, #36]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #40
	str	r7, [r3, #0]
	ldr	r3, [pc, #20]
	subs	r2, #70
	movs	r5, #160
	strh	r3, [r2, #0]
	movs	r7, #1
	lsls	r5, r5, #19
	mov	r9, r7
	adds	r5, #2
	b.n	.L_08177a98
	movs	r0, r0
	.4byte 0x00000410
	.4byte 0x00000786
	.4byte 0x06004000
	.2byte 0x11e0
	.2byte 0x0300
.L_08177a98:
	mov	r1, r9
	lsls	r0, r1, #1
	movs	r1, #3
	bl	sub_08002054
	movs	r3, #31
	mov	r2, r9
	subs	r1, r3, r0
	lsrs	r3, r2, #31
	add	r3, r9
	asrs	r3, r3, #1
	movs	r2, #8
	subs	r2, r2, r3
	movs	r3, #27
	subs	r3, r3, r0
	cmp	r1, #0
	bge.n	.L_08177abc
	movs	r1, #0
.L_08177abc:
	cmp	r2, #0
	bge.n	.L_08177ac2
	movs	r2, #0
.L_08177ac2:
	cmp	r3, #0
	bge.n	.L_08177ac8
	movs	r3, #0
.L_08177ac8:
	lsls	r3, r3, #10
	lsls	r2, r2, #5
	orrs	r3, r2
	orrs	r3, r1
	strh	r3, [r5, #0]
	movs	r3, #1
	add	r9, r3
	mov	r4, r9
	adds	r5, #2
	cmp	r4, #64
	bne.n	.L_08177a98
	ldr	r5, [sp, #100]
	ldr	r2, [pc, #60]
	ldr	r3, [r5, #20]
	adds	r6, r5, #0
	lsls	r3, r3, #1
	adds	r3, #36
	adds	r0, r6, #0
	strh	r2, [r6, r3]
	adds	r0, #36
	movs	r1, #0
	bl	sub_08118010
	ldr	r7, [sp, #92]
	movs	r1, #224
	movs	r5, #128
	lsls	r5, r5, #9
	lsls	r1, r1, #3
	adds	r0, r7, r1
	adds	r2, r5, #0
	movs	r1, #56
	bl	sub_0815b434
	movs	r2, #184
	lsls	r2, r2, #5
	adds	r0, r7, r2
	movs	r1, #16
	adds	r2, r5, #0
	bl	sub_0815b434
	movs	r3, #156
	lsls	r3, r3, #6
	b.n	.L_08177b24
	movs	r0, r0
	.2byte 0x00ff
	.2byte 0x0000
.L_08177b24:
	movs	r1, #128
	adds	r0, r7, r3
	lsls	r1, r1, #5
	ldr	r3, [pc, #808]
	ldr	r2, [pc, #808]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x24dc
	lsls	r4, r4, #6
	adds	r1, r7, r4
	ldr	r0, [pc, #800]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r3, #0
	ldr	r0, [pc, #792]
	ldr	r1, [sp, #72]
	movs	r2, #0
	bl	sub_08157cf4
	movs	r3, #192
	movs	r5, #0
	lsls	r3, r3, #2
	mov	r9, r5
	adds	r3, #2
.L_08177b58:
	movs	r6, #35
	add	r9, r6
	cmp	r9, r3
	bne.n	.L_08177b58
	ldr	r3, [sp, #92]
	movs	r4, #220
	movs	r7, #0
	movs	r1, #128
	lsls	r4, r4, #6
	mov	r9, r7
	movs	r0, #63
	lsls	r1, r1, #5
	adds	r2, r3, r4
.L_08177b72:
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_08177b7a
	strb	r0, [r2, #0]
.L_08177b7a:
	movs	r5, #1
	add	r9, r5
	adds	r2, #1
	cmp	r9, r1
	bne.n	.L_08177b72
	ldr	r1, [pc, #732]
	ldr	r0, [pc, #736]
	ldrh	r3, [r0, #0]
	adds	r4, r3, #0
	strh	r0, [r0, #0]
	ldrh	r2, [r1, #0]
	cmp	r2, #31
	bgt.n	.L_08177bb6
	lsls	r3, r2, #1
	adds	r3, r3, r2
	adds	r2, #1
	lsls	r3, r3, #2
	strh	r2, [r1, #0]
	movs	r2, #238
	adds	r3, r3, r1
	lsls	r2, r2, #7
	adds	r3, #4
	adds	r2, #65
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #19
	stmia	r3!, {r2}
	movs	r2, #128
	lsls	r2, r2, #10
	str	r2, [r3, #0]
.L_08177bb6:
	strh	r4, [r0, #0]
	movs	r6, #0
	ldr	r2, [sp, #92]
	mov	r9, r6
	adds	r2, #24
.L_08177bc0:
	movs	r0, #1
	mov	r7, r9
	add	r9, r0
	negs	r3, r7
	mov	r1, r9
	str	r3, [r2, #0]
	adds	r2, #28
	cmp	r1, #16
	bne.n	.L_08177bc0
	ldr	r3, [pc, #664]
	movs	r2, #0
	mov	r9, r2
	subs	r2, #1
.L_08177bda:
	movs	r4, #1
	add	r9, r4
	mov	r5, r9
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r5, #128
	bne.n	.L_08177bda
	ldr	r7, [sp, #100]
	movs	r6, #0
	ldr	r3, [r7, #20]
	mov	r9, r6
	cmp	r3, #0
	beq.n	.L_08177c24
	ldr	r0, [sp, #92]
	movs	r1, #224
	lsls	r1, r1, #2
	adds	r5, r0, r1
	movs	r6, #36
.L_08177bfe:
	ldr	r2, [sp, #100]
	ldrsh	r0, [r6, r2]
	bl	sub_08118098
	ldr	r2, [r0, #0]
	movs	r4, #1
	ldr	r3, [r2, #8]
	add	r9, r4
	str	r3, [r5, #0]
	adds	r6, #2
	ldr	r3, [r2, #12]
	str	r3, [r5, #4]
	ldr	r3, [r2, #16]
	str	r3, [r5, #8]
	ldr	r7, [sp, #100]
	adds	r5, #28
	ldr	r3, [r7, #20]
	cmp	r9, r3
	bne.n	.L_08177bfe
.L_08177c24:
	movs	r0, #0
	str	r0, [sp, #80]
.L_08177c28:
	ldr	r3, [pc, #580]
	movs	r2, #3
	ldr	r3, [r3, #12]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08177c3e
	ldr	r1, [sp, #80]
	cmp	r1, #119
	bgt.n	.L_08177c3e
	movs	r2, #120
	str	r2, [sp, #80]
.L_08177c3e:
	ldr	r3, [sp, #80]
	cmp	r3, #0
	bne.n	.L_08177cd2
	movs	r0, #162
	bl	sub_081c0010
	movs	r5, #192
	ldr	r4, [sp, #92]
	lsls	r5, r5, #3
	adds	r5, #252
	movs	r3, #160
	adds	r2, r4, r5
	str	r3, [r2, #0]
	ldr	r7, [sp, #100]
	movs	r6, #0
	str	r6, [sp, #32]
	ldr	r3, [r7, #20]
	mov	r9, r6
	cmp	r3, #0
	beq.n	.L_08177c80
	movs	r0, #236
	movs	r1, #1
	lsls	r0, r0, #1
	negs	r1, r1
	adds	r2, r4, r0
.L_08177c70:
	str	r1, [r2, #0]
	ldr	r4, [sp, #100]
	movs	r3, #1
	add	r9, r3
	ldr	r3, [r4, #20]
	adds	r2, #28
	cmp	r9, r3
	bne.n	.L_08177c70
.L_08177c80:
	movs	r5, #0
	mov	r9, r5
	cmp	r3, #0
	beq.n	.L_08177cd2
	movs	r6, #1
	negs	r6, r6
	movs	r7, #254
	mov	sl, r6
	mov	r8, r5
	lsls	r7, r7, #24
	movs	r6, #36
.L_08177c96:
	ldr	r1, [sp, #100]
	ldrsh	r0, [r6, r1]
	bl	sub_08118098
	ldr	r3, [sp, #100]
	mov	r1, sl
	ldr	r5, [r0, #0]
	movs	r2, #5
	ldrsh	r0, [r6, r3]
	str	r1, [sp, #0]
	mov	r3, sl
	movs	r1, #0
	bl	sub_0814cd48
	movs	r3, #160
	mov	r2, r8
	lsls	r3, r3, #13
	str	r7, [r5, #8]
	str	r3, [r5, #12]
	str	r2, [r5, #16]
	str	r2, [r5, #72]
	ldr	r3, [pc, #432]
	ldr	r5, [sp, #100]
	adds	r7, r7, r3
	ldr	r3, [r5, #20]
	movs	r4, #1
	add	r9, r4
	adds	r6, #2
	cmp	r9, r3
	bne.n	.L_08177c96
.L_08177cd2:
	ldr	r6, [sp, #80]
	cmp	r6, #120
	bne.n	.L_08177d3c
	ldr	r7, [sp, #92]
	movs	r0, #239
	movs	r1, #238
	lsls	r0, r0, #7
	lsls	r1, r1, #7
	adds	r2, r7, r0
	movs	r3, #1
	adds	r1, #132
	str	r3, [r2, #0]
	adds	r3, r7, r1
	movs	r2, #0
	str	r2, [r3, #0]
	ldr	r3, [sp, #68]
	movs	r4, #160
	lsls	r4, r4, #3
	movs	r1, #160
	movs	r2, #153
	adds	r4, #108
	lsls	r1, r1, #19
	lsls	r2, r2, #8
	adds	r0, r3, r4
	adds	r1, #192
	movs	r3, #128
	adds	r2, #160
	bl	sub_081180f0
	ldr	r6, [sp, #100]
	movs	r5, #0
	ldr	r3, [r6, #20]
	mov	r9, r5
	cmp	r3, #0
	beq.n	.L_08177d3c
	movs	r6, #36
.L_08177d1a:
	ldr	r7, [sp, #100]
	ldrsh	r0, [r6, r7]
	bl	sub_08118098
	ldr	r2, [r0, #0]
	movs	r3, #248
	lsls	r3, r3, #15
	str	r5, [r2, #8]
	str	r3, [r2, #12]
	str	r5, [r2, #16]
	str	r5, [r2, #72]
	ldr	r3, [r7, #20]
	movs	r2, #1
	add	r9, r2
	adds	r6, #2
	cmp	r9, r3
	bne.n	.L_08177d1a
.L_08177d3c:
	ldr	r3, [sp, #80]
	cmp	r3, #160
	bne.n	.L_08177d48
	movs	r0, #134
	bl	sub_081180e8
.L_08177d48:
	ldr	r4, [sp, #80]
	cmp	r4, #139
	bgt.n	.L_08177d50
	b.n	.L_08177ffc
.L_08177d50:
	cmp	r4, #140
	beq.n	.L_08177d56
	b.n	.L_08177f16
.L_08177d56:
	movs	r6, #240
	ldr	r7, [pc, #284]
	movs	r5, #0
	lsls	r6, r6, #14
	mov	r9, r5
	mov	r8, r6
.L_08177d62:
	bl	sub_08014878
	movs	r5, #128
	lsls	r5, r5, #1
	adds	r5, #255
	ands	r5, r0
	bl	sub_08014878
	adds	r6, r0, #0
	mov	r0, r8
	str	r0, [r7, #0]
	str	r0, [r7, #4]
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r5, #32
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #5
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	movs	r2, #1
	asrs	r3, r3, #5
	add	r9, r2
	str	r3, [r7, #16]
	movs	r1, #0
	mov	r3, r9
	str	r1, [r7, #24]
	adds	r7, #28
	cmp	r3, #32
	bne.n	.L_08177d62
	ldr	r7, [pc, #208]
	mov	r9, r1
.L_08177dac:
	bl	sub_08014878
	movs	r5, #255
	ands	r5, r0
	bl	sub_08014878
	movs	r3, #240
	lsls	r3, r3, #14
	str	r3, [r7, #0]
	str	r3, [r7, #4]
	adds	r6, r0, #0
	bl	sub_08002096
	adds	r5, #32
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #16]
	bl	sub_08014878
	movs	r3, #15
	movs	r4, #1
	ands	r3, r0
	add	r9, r4
	adds	r3, #16
	mov	r5, r9
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r5, #128
	bne.n	.L_08177dac
	ldr	r1, [pc, #136]
	movs	r2, #1
	movs	r3, #0
	ldr	r0, [pc, #132]
	bl	sub_08157cf4
	movs	r0, #144
	bl	sub_081c0010
	movs	r1, #240
	ldr	r3, [pc, #72]
	lsls	r1, r1, #6
	ldr	r2, [pc, #72]
	ldr	r0, [sp, #96]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4b0e
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #12
	strh	r3, [r2, #0]
	ldr	r6, [sp, #92]
	movs	r7, #239
	movs	r0, #238
	lsls	r7, r7, #7
	lsls	r0, r0, #7
	adds	r2, r6, r7
	movs	r3, #2
	adds	r0, #132
	str	r3, [r2, #0]
	adds	r2, r6, r0
	movs	r3, #50
	str	r3, [r2, #0]
	ldr	r2, [sp, #100]
	movs	r1, #0
	ldr	r3, [r2, #20]
	mov	r9, r1
	cmp	r3, #0
	beq.n	.L_08177eb0
	movs	r3, #226
	lsls	r3, r3, #2
	adds	r5, r6, r3
	movs	r6, #36
	b.n	.L_08177e88
	movs	r0, r0
	.4byte 0x00000784
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.4byte 0x000000c3
	.4byte 0x00000134
	.4byte 0x020038e0
	.4byte 0x04000208
	.4byte 0x02014218
	.4byte 0x03001150
	.4byte 0xff800000
	.4byte 0x02014c80
	.4byte 0x02015000
	.4byte 0x02010000
	.2byte 0x013e
	.2byte 0x0000
.L_08177e88:
	ldr	r4, [sp, #100]
	ldrsh	r0, [r6, r4]
	bl	sub_08118098
	ldr	r2, [r0, #0]
	movs	r3, #128
	lsls	r3, r3, #14
	movs	r0, #0
	str	r0, [r2, #8]
	str	r3, [r2, #12]
	movs	r1, #1
	ldr	r3, [r5, #0]
	add	r9, r1
	str	r3, [r2, #16]
	ldr	r2, [sp, #100]
	adds	r5, #28
	ldr	r3, [r2, #20]
	adds	r6, #2
	cmp	r9, r3
	bne.n	.L_08177e88
.L_08177eb0:
	movs	r4, #0
	mov	r9, r4
	cmp	r3, #0
	beq.n	.L_08177f08
	movs	r6, #36
.L_08177eba:
	bl	sub_08014878
	adds	r5, r0, #0
	bl	sub_08014878
	movs	r3, #31
	ldr	r1, [sp, #100]
	ands	r3, r0
	adds	r3, #120
	movs	r7, #128
	ldrsh	r0, [r6, r1]
	lsls	r7, r7, #10
	str	r3, [sp, #4]
	lsls	r5, r5, #1
	movs	r2, #128
	movs	r3, #128
	adds	r5, r5, r7
	movs	r1, #1
	lsls	r2, r2, #11
	lsls	r3, r3, #12
	str	r5, [sp, #0]
	bl	sub_0815f000
	ldr	r3, [sp, #100]
	movs	r1, #7
	ldrsh	r0, [r6, r3]
	movs	r3, #32
	str	r3, [sp, #0]
	movs	r2, #5
	mov	r3, r9
	bl	sub_0814cd48
	ldr	r7, [sp, #100]
	movs	r5, #1
	ldr	r3, [r7, #20]
	add	r9, r5
	adds	r6, #2
	cmp	r9, r3
	bne.n	.L_08177eba
.L_08177f08:
	ldr	r0, [sp, #92]
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #168
	adds	r2, r0, r1
	movs	r3, #3
	str	r3, [r2, #0]
.L_08177f16:
	movs	r0, #104
	movs	r1, #31
	bl	sub_081963ec
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	ldr	r5, [pc, #316]
	str	r3, [sp, #176]
	movs	r2, #0
	mov	r9, r2
.L_08177f2c:
	ldr	r3, [r5, #24]
	cmp	r3, #23
	bhi.n	.L_08177f6a
	adds	r1, r3, #0
	cmp	r3, #0
	bge.n	.L_08177f3a
	adds	r1, r3, #3
.L_08177f3a:
	ldr	r3, [pc, #300]
	asrs	r1, r1, #2
	lsls	r1, r1, #11
	movs	r4, #2
	ldrsh	r2, [r5, r4]
	adds	r1, r1, r3
	movs	r6, #6
	ldrsh	r3, [r5, r6]
	movs	r0, #32
	str	r0, [sp, #0]
	movs	r0, #64
	subs	r3, #32
	str	r0, [sp, #4]
	subs	r2, #16
	ldr	r4, [sp, #176]
	ldr	r0, [sp, #96]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c28
	movs	r1, #60
	ldr	r2, [pc, #264]
	bl	sub_08138086
	ldr	r3, [r5, #24]
.L_08177f6a:
	movs	r7, #1
	add	r9, r7
	adds	r3, #1
	mov	r0, r9
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r0, #16
	bne.n	.L_08177f2c
	ldr	r6, [pc, #244]
	movs	r1, #0
	mov	r9, r1
.L_08177f80:
	ldr	r3, [r6, #24]
	cmp	r3, #0
	blt.n	.L_08177fea
	asrs	r3, r3, #3
	adds	r5, r3, #2
	ldr	r3, [pc, #232]
	movs	r2, #0
	mov	fp, r3
	lsrs	r3, r5, #31
	adds	r3, r5, r3
	asrs	r3, r3, #1
	mov	r8, r2
	lsls	r7, r5, #1
	mov	sl, r3
.L_08177f9c:
	subs	r3, r7, #2
	mov	r4, fp
	ldrh	r1, [r4, r3]
	ldr	r0, [sp, #72]
	movs	r3, #2
	ldrsh	r2, [r6, r3]
	adds	r1, r0, r1
	movs	r0, #6
	ldrsh	r3, [r6, r0]
	mov	r4, sl
	subs	r2, r2, r4
	subs	r3, r3, r5
	str	r5, [sp, #0]
	str	r7, [sp, #4]
	ldr	r4, [sp, #176]
	ldr	r0, [sp, #96]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x213f
	adds	r0, r6, #0
	movs	r2, #0
	bl	sub_08138086
	ldr	r3, [r6, #4]
	movs	r1, #224
	lsls	r1, r1, #15
	cmp	r3, r1
	ble.n	.L_08177fda
	ldr	r3, [r6, #16]
	negs	r3, r3
	str	r3, [r6, #16]
.L_08177fda:
	movs	r2, #1
	add	r8, r2
	mov	r3, r8
	cmp	r3, #4
	bne.n	.L_08177f9c
	ldr	r3, [r6, #24]
	subs	r3, #1
	str	r3, [r6, #24]
.L_08177fea:
	movs	r4, #1
	add	r9, r4
	mov	r5, r9
	adds	r6, #28
	cmp	r5, #128
	bne.n	.L_08177f80
	movs	r0, #104
	bl	sub_0801314c
.L_08177ffc:
	ldr	r6, [sp, #80]
	cmp	r6, #119
	ble.n	.L_08178004
	b.n	.L_0817810e
.L_08178004:
	movs	r7, #0
	mov	r9, r7
.L_08178008:
	ldr	r0, [pc, #108]
	mov	r1, r9
	ldrb	r2, [r0, r1]
	ldr	r3, [sp, #80]
	cmp	r3, r2
	bne.n	.L_0817802c
	movs	r0, #160
	lsls	r0, r0, #19
	movs	r1, #128
	ldr	r2, [pc, #96]
	ldr	r3, [pc, #96]
	adds	r0, #192
	lsls	r1, r1, #1
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4c14
	mov	r5, r9
	ldrb	r2, [r4, r5]
.L_0817802c:
	ldr	r6, [sp, #80]
	adds	r3, r2, #1
	cmp	r6, r3
	blt.n	.L_08178102
	adds	r3, #15
	cmp	r6, r3
	bge.n	.L_08178102
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	movs	r7, #160
	movs	r0, #0
	lsls	r7, r7, #3
	ldr	r1, [pc, #24]
	adds	r7, #108
	str	r0, [sp, #84]
	adds	r7, r7, r3
	movs	r2, #31
	movs	r3, #10
	mov	fp, r7
	mov	lr, r1
	mov	sl, r2
	mov	r8, r3
	mov	ip, r0
	b.n	.L_08178084
	movs	r0, r0
	.4byte 0x0000001f
	.4byte 0x02014c80
	.4byte 0x02010000
	.4byte 0xffffe000
	.4byte 0x02015000
	.4byte 0x08197410
	.4byte 0x08198c67
	.4byte 0x7fff7fff
	.2byte 0x0260
	.2byte 0x0300
.L_08178084:
	mov	r4, ip
	mov	r5, fp
	ldrh	r2, [r4, r5]
	mov	r3, sl
	ands	r3, r2
	mov	r7, r8
	lsls	r2, r2, #16
	mov	r0, lr
	subs	r6, r3, r7
	lsrs	r3, r2, #21
	lsrs	r2, r2, #26
	ands	r2, r0
	subs	r5, r2, r7
	movs	r7, #160
	lsls	r7, r7, #19
	adds	r7, #192
	ands	r3, r0
	add	r7, ip
	adds	r4, r3, #0
	ldrh	r3, [r7, #0]
	mov	r0, sl
	ands	r0, r3
	lsls	r3, r3, #16
	lsrs	r2, r3, #21
	mov	r1, lr
	ands	r2, r1
	lsrs	r1, r3, #26
	mov	r3, lr
	subs	r4, #20
	ands	r1, r3
	cmp	r6, #0
	bge.n	.L_081780c6
	movs	r6, #0
.L_081780c6:
	cmp	r4, #0
	bge.n	.L_081780cc
	movs	r4, #0
.L_081780cc:
	cmp	r5, #0
	bge.n	.L_081780d2
	movs	r5, #0
.L_081780d2:
	subs	r3, r0, #1
	cmp	r6, r3
	bge.n	.L_081780da
	subs	r0, #2
.L_081780da:
	subs	r3, r2, #1
	cmp	r4, r3
	bge.n	.L_081780e2
	subs	r2, #2
.L_081780e2:
	subs	r3, r1, #1
	cmp	r5, r3
	bge.n	.L_081780ea
	subs	r1, #2
.L_081780ea:
	lsls	r3, r1, #10
	lsls	r2, r2, #5
	orrs	r3, r2
	orrs	r3, r0
	strh	r3, [r7, #0]
	ldr	r5, [sp, #84]
	movs	r4, #2
	adds	r5, #1
	add	ip, r4
	str	r5, [sp, #84]
	cmp	r5, #128
	bne.n	.L_08178084
.L_08178102:
	movs	r6, #1
	add	r9, r6
	mov	r7, r9
	cmp	r7, #3
	beq.n	.L_0817810e
	b.n	.L_08178008
.L_0817810e:
	ldr	r0, [sp, #80]
	cmp	r0, #120
	ble.n	.L_0817817c
	cmp	r0, #121
	bne.n	.L_0817812e
	add	r1, sp, #76
	ldr	r3, [pc, #388]
	ldrh	r1, [r1, #0]
	movs	r2, #0
	strh	r1, [r3, #4]
	ldr	r3, [sp, #88]
	add	r4, sp, #28
	str	r2, [r3, #16]
	ldrh	r4, [r4, #0]
	ldr	r5, [sp, #36]
	strh	r4, [r5, #54]
.L_0817812e:
	ldr	r5, [sp, #80]
	cmp	r5, #147
	ble.n	.L_08178136
	b.n	.L_081785f6
.L_08178136:
	adds	r0, r5, #0
	subs	r0, #120
	movs	r1, #6
	bl	sub_08002054
	movs	r7, #192
	ldr	r6, [sp, #92]
	lsls	r7, r7, #3
	adds	r7, #252
	adds	r2, r6, r7
	ldr	r6, [r2, #0]
	movs	r3, #8
	subs	r3, r3, r0
	subs	r3, r6, r3
	str	r3, [r2, #0]
	cmp	r6, #0
	bgt.n	.L_0817815a
	b.n	.L_081785f6
.L_0817815a:
	ldr	r0, [sp, #92]
	movs	r1, #224
	lsls	r1, r1, #3
	adds	r5, r0, r1
	movs	r2, #128
	adds	r0, r5, #0
	adds	r1, r6, #0
	lsls	r2, r2, #9
	bl	sub_0815b434
	adds	r0, r5, #0
	movs	r1, #60
	movs	r2, #60
	adds	r3, r6, #0
	bl	sub_0818caa8
	b.n	.L_081785f6
.L_0817817c:
	ldr	r3, [sp, #80]
	subs	r3, #4
	cmp	r3, #21
	bhi.n	.L_0817818a
	ldr	r2, [sp, #32]
	adds	r2, #4
	str	r2, [sp, #32]
.L_0817818a:
	ldr	r3, [sp, #80]
	cmp	r3, #107
	ble.n	.L_0817819e
	ldr	r4, [sp, #32]
	subs	r4, #8
	str	r4, [sp, #32]
	cmp	r4, #0
	bge.n	.L_0817819e
	movs	r5, #0
	str	r5, [sp, #32]
.L_0817819e:
	ldr	r7, [sp, #100]
	movs	r6, #0
	ldr	r3, [r7, #20]
	mov	r9, r6
	cmp	r3, #0
	bne.n	.L_081781ac
	b.n	.L_08178320
.L_081781ac:
	ldr	r0, [sp, #92]
	movs	r1, #224
	lsls	r1, r1, #1
	adds	r2, r7, #0
	str	r6, [sp, #12]
	adds	r0, r0, r1
	adds	r2, #36
	mov	r8, r0
	mov	fp, r2
.L_081781be:
	mov	r4, fp
	movs	r3, #0
	ldrsh	r0, [r4, r3]
	bl	sub_08118098
	ldr	r0, [r0, #0]
	ldr	r5, [pc, #216]
	ldr	r3, [r0, #12]
	mov	sl, r0
	movs	r1, #0
	cmp	r3, r5
	bgt.n	.L_081782ac
	mov	r6, r8
	ldr	r3, [r6, #24]
	movs	r7, #1
	negs	r7, r7
	cmp	r3, r7
	bne.n	.L_081781f0
	ldr	r3, [r0, #8]
	movs	r0, #160
	lsls	r0, r0, #12
	adds	r3, r3, r0
	mov	r2, sl
	str	r3, [r2, #8]
	b.n	.L_081781fc
.L_081781f0:
	mov	r4, sl
	ldr	r3, [r4, #8]
	movs	r5, #128
	lsls	r5, r5, #10
	adds	r3, r3, r5
	str	r3, [r4, #8]
.L_081781fc:
	mov	r6, r8
	ldr	r2, [r6, #24]
	movs	r7, #1
	negs	r7, r7
	cmp	r2, r7
	bne.n	.L_081782b0
	mov	r0, sl
	ldr	r3, [r0, #8]
	movs	r4, #192
	lsls	r4, r4, #14
	cmp	r3, r4
	ble.n	.L_08178216
	movs	r1, #1
.L_08178216:
	cmp	r1, #1
	bne.n	.L_081782b0
	movs	r5, #0
	mov	r6, r8
	str	r5, [r6, #24]
	mov	r1, fp
	movs	r7, #0
	ldrsh	r0, [r1, r7]
	mov	r1, r8
	bl	sub_0815e20c
	movs	r0, #103
	bl	sub_081c0010
	ldr	r3, [pc, #116]
	ldr	r2, [sp, #12]
	str	r5, [sp, #84]
	adds	r7, r2, r3
.L_0817823a:
	bl	sub_08014878
	movs	r3, #254
	lsls	r3, r3, #7
	adds	r3, #255
	adds	r6, r0, #0
	ands	r6, r3
	bl	sub_08014878
	movs	r5, #255
	ands	r5, r0
	mov	r0, r8
	ldr	r3, [r0, #0]
	movs	r4, #128
	lsls	r3, r3, #15
	str	r3, [r7, #0]
	lsls	r4, r4, #1
	ldr	r3, [r0, #4]
	adds	r0, r6, #0
	lsls	r3, r3, #16
	str	r3, [r7, #4]
	adds	r5, r5, r4
	bl	sub_08002096
	adds	r3, r5, #0
	muls	r3, r0
	negs	r3, r3
	asrs	r3, r3, #7
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #16]
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	adds	r3, #16
	str	r3, [r7, #24]
	ldr	r1, [sp, #84]
	adds	r7, #28
	adds	r1, #1
	str	r1, [sp, #84]
	cmp	r1, #16
	bne.n	.L_0817823a
	mov	r3, r8
	ldr	r2, [r3, #24]
	b.n	.L_081782b0
	.4byte 0x03001120
	.4byte 0x0063ffff
	.2byte 0x4200
	.2byte 0x0201
.L_081782ac:
	mov	r4, r8
	ldr	r2, [r4, #24]
.L_081782b0:
	cmp	r2, #2
	beq.n	.L_081782b8
	cmp	r2, #7
	bne.n	.L_081782d0
.L_081782b8:
	movs	r3, #32
	mov	r6, fp
	movs	r5, #0
	ldrsh	r0, [r6, r5]
	str	r3, [sp, #0]
	movs	r2, #5
	movs	r1, #7
	mov	r3, r9
	mov	r7, r8
	bl	sub_0814cd48
	ldr	r2, [r7, #24]
.L_081782d0:
	cmp	r2, #6
	bne.n	.L_081782ee
	mov	r2, fp
	movs	r1, #0
	ldrsh	r0, [r2, r1]
	movs	r3, #32
	movs	r2, #1
	str	r3, [sp, #0]
	negs	r2, r2
	mov	r3, r9
	movs	r1, #15
	bl	sub_0814cd48
	mov	r3, r8
	ldr	r2, [r3, #24]
.L_081782ee:
	cmp	r2, #8
	bne.n	.L_08178300
	movs	r3, #148
	mov	r5, sl
	movs	r4, #0
	lsls	r3, r3, #16
	str	r4, [r5, #8]
	str	r3, [r5, #12]
	str	r4, [r5, #16]
.L_08178300:
	ldr	r0, [sp, #12]
	movs	r1, #224
	lsls	r1, r1, #1
	adds	r0, r0, r1
	str	r0, [sp, #12]
	ldr	r4, [sp, #100]
	movs	r2, #1
	ldr	r3, [r4, #20]
	movs	r6, #2
	movs	r7, #28
	add	r9, r2
	add	fp, r6
	add	r8, r7
	cmp	r9, r3
	beq.n	.L_08178320
	b.n	.L_081781be
.L_08178320:
	ldr	r5, [sp, #80]
	cmp	r5, #0
	bge.n	.L_08178328
	b.n	.L_08178524
.L_08178328:
	ldr	r2, [pc, #812]
	movs	r6, #255
	ldrh	r3, [r2, #4]
	lsls	r6, r6, #8
	adds	r6, #244
	adds	r3, r3, r6
	strh	r3, [r2, #4]
	movs	r1, #27
	movs	r0, #104
	bl	sub_081963ec
	movs	r7, #0
	ldr	r2, [sp, #92]
	mov	r9, r7
	movs	r0, #26
	movs	r3, #168
	ldr	r7, [sp, #32]
	negs	r0, r0
	movs	r1, #0
	lsls	r3, r3, #3
	movs	r4, #28
	lsls	r6, r5, #10
	mov	sl, r0
	mov	r8, r1
	adds	r5, r2, r3
	mov	fp, r4
.L_0817835c:
	adds	r0, r6, #0
	bl	sub_08002096
	lsls	r0, r0, #2
	asrs	r0, r0, #16
	adds	r3, r7, r0
	mov	r1, r9
	mov	r0, r8
	str	r3, [r5, #0]
	str	r0, [r5, #4]
	cmp	r1, #3
	bne.n	.L_0817838a
	subs	r3, #8
	mov	r2, sl
	movs	r0, #104
	str	r3, [r5, #0]
	str	r2, [r5, #4]
	bl	sub_0801314c
	movs	r0, #104
	movs	r1, #19
	bl	sub_081963ec
.L_0817838a:
	ldr	r2, [r5, #0]
	mov	r0, fp
	movs	r1, #56
	ldr	r4, [r5, #4]
	mov	r3, fp
	str	r0, [sp, #0]
	str	r1, [sp, #4]
	subs	r3, r2, r3
	movs	r2, #192
	mov	ip, r3
	lsls	r2, r2, #18
	ldr	r3, [sp, #92]
	mov	lr, r4
	ldr	r4, [r2, #104]
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r1, r3, r2
	ldr	r0, [sp, #96]
	mov	r3, lr
	mov	r2, ip
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2001
	movs	r4, #128
	add	r9, r0
	movs	r3, #30
	lsls	r4, r4, #6
	mov	r1, r9
	add	sl, r3
	add	r8, r3
	adds	r7, #8
	adds	r6, r6, r4
	adds	r5, #28
	cmp	r1, #4
	bne.n	.L_0817835c
	movs	r0, #104
	bl	sub_0801314c
	ldr	r5, [sp, #92]
	movs	r2, #0
	movs	r6, #160
	mov	r9, r2
	lsls	r6, r6, #15
.L_081783e0:
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
	cmp	r3, #4
	bne.n	.L_081783ee
	movs	r3, #0
	str	r3, [r5, #24]
.L_081783ee:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	bne.n	.L_08178428
	bl	sub_08014878
	movs	r1, #24
	bl	sub_0800206c
	ldr	r4, [sp, #32]
	mov	r7, r9
	lsls	r3, r7, #1
	adds	r0, r0, r4
	subs	r0, r0, r3
	lsls	r0, r0, #16
	str	r0, [r5, #0]
	str	r6, [r5, #4]
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r3, r0
	ldr	r0, [pc, #576]
	negs	r3, r3
	adds	r3, r3, r0
	str	r3, [r5, #12]
	ldr	r3, [pc, #572]
	str	r3, [r5, #16]
	ldr	r3, [r5, #24]
.L_08178428:
	cmp	r3, #0
	blt.n	.L_08178452
	ldr	r4, [sp, #92]
	movs	r7, #184
	lsls	r7, r7, #5
	movs	r2, #2
	ldrsh	r1, [r5, r2]
	adds	r0, r4, r7
	movs	r3, #6
	ldrsh	r2, [r5, r3]
	movs	r3, #16
	bl	sub_0818caa8
	ldr	r3, [r5, #0]
	ldr	r2, [r5, #12]
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r2, [r5, #16]
	ldr	r3, [r5, #4]
	adds	r3, r3, r2
	str	r3, [r5, #4]
.L_08178452:
	movs	r1, #1
	movs	r0, #192
	add	r9, r1
	lsls	r0, r0, #10
	mov	r2, r9
	adds	r6, r6, r0
	adds	r5, #28
	cmp	r2, #12
	bne.n	.L_081783e0
	movs	r0, #104
	movs	r1, #31
	bl	sub_081963ec
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	ldr	r4, [sp, #92]
	movs	r7, #168
	str	r3, [sp, #176]
	lsls	r7, r7, #3
	movs	r3, #0
	mov	r9, r3
	movs	r6, #0
	adds	r5, r4, r7
.L_08178482:
	ldr	r2, [r5, #0]
	movs	r3, #120
	subs	r3, r3, r2
	adds	r5, #28
	cmp	r3, #0
	ble.n	sub_081784a6
	str	r3, [sp, #0]
	movs	r3, #30
	str	r3, [sp, #4]
	ldr	r3, [sp, #92]
	movs	r7, #156
	lsls	r7, r7, #6
	adds	r1, r3, r7
	ldr	r4, [sp, #176]
	ldr	r0, [sp, #96]
	adds	r3, r6, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2001
	add	r9, r0
	mov	r1, r9
	adds	r6, #30
	cmp	r1, #4
	bne.n	.L_08178482
	movs	r0, #104
	bl	sub_0801314c
	movs	r0, #104
	movs	r1, #47
	bl	sub_081963ec
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	ldr	r6, [pc, #412]
	str	r3, [sp, #176]
	ldr	r5, [pc, #412]
	movs	r2, #0
	mov	r9, r2
.L_081784d0:
	ldr	r0, [r5, #24]
	cmp	r0, #0
	blt.n	.L_08178512
	asrs	r0, r0, #3
	adds	r0, #2
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r6, r3]
	ldr	r3, [sp, #72]
	movs	r7, #2
	ldrsh	r2, [r5, r7]
	adds	r1, r3, r1
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	movs	r7, #6
	ldrsh	r3, [r5, r7]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #96]
	ldr	r4, [sp, #176]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c28
	movs	r1, #60
	movs	r2, #0
	bl	sub_08138086
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L_08178512:
	movs	r0, #1
	add	r9, r0
	mov	r1, r9
	adds	r5, #28
	cmp	r1, #96
	bne.n	.L_081784d0
	movs	r0, #104
	bl	sub_0801314c
.L_08178524:
	movs	r0, #32
	bl	sub_08014dac
	mov	r8, r0
	movs	r0, #1
	bl	sub_081969f8
	ldr	r2, [pc, #312]
	ldr	r3, [sp, #104]
	movs	r4, #220
	ands	r3, r2
	movs	r2, #6
	orrs	r3, r2
	ldr	r2, [pc, #304]
	lsls	r4, r4, #6
	ands	r3, r2
	movs	r2, #192
	lsls	r2, r2, #3
	orrs	r3, r2
	ldr	r2, [sp, #92]
	str	r3, [sp, #104]
	adds	r3, r2, r4
	add	r2, sp, #104
	adds	r7, r0, #0
	str	r3, [r2, #4]
	movs	r3, #9
	str	r3, [r7, #0]
	ldr	r3, [pc, #280]
	mov	r5, r8
	str	r3, [r7, #8]
	str	r2, [r7, #16]
	str	r5, [r7, #12]
	ldr	r0, [sp, #100]
	movs	r6, #0
	ldr	r3, [r0, #20]
	mov	r9, r6
	cmp	r3, #0
	beq.n	.L_081785f6
	ldr	r1, [sp, #92]
	movs	r2, #224
	lsls	r2, r2, #1
	adds	r6, r1, r2
.L_08178578:
	ldr	r1, [r6, #24]
	cmp	r1, #17
	bhi.n	.L_081785dc
	lsls	r3, r1, #1
	adds	r3, r3, r1
	lsls	r5, r3, #7
	adds	r2, r3, #0
	subs	r5, r5, r3
	movs	r3, #128
	lsls	r3, r3, #4
	lsls	r5, r5, #3
	adds	r5, r5, r3
	subs	r2, #64
	adds	r3, r1, #1
	str	r3, [r6, #24]
	str	r2, [r7, #20]
	bl	sub_08014de4
	ldr	r0, [r6, #0]
	ldr	r1, [r6, #4]
	lsrs	r3, r0, #31
	adds	r0, r0, r3
	asrs	r0, r0, #1
	subs	r0, #56
	subs	r1, #56
	movs	r2, #0
	lsls	r0, r0, #16
	lsls	r1, r1, #16
	bl	sub_08015160
	movs	r1, #3
	adds	r0, r5, #0
	bl	sub_08002054
	adds	r1, r5, #0
	adds	r2, r5, #0
	bl	sub_080151e4
	movs	r0, #192
	lsls	r0, r0, #8
	bl	sub_08015024
	ldr	r0, [pc, #168]
	mov	r1, r8
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r7, #0
	bl	sub_08196a7c
.L_081785dc:
	adds	r0, r7, #0
	bl	sub_08013164
	mov	r0, r8
	bl	sub_08013164
	ldr	r5, [sp, #100]
	movs	r4, #1
	ldr	r3, [r5, #20]
	add	r9, r4
	adds	r6, #28
	cmp	r9, r3
	bne.n	.L_08178578
.L_081785f6:
	movs	r0, #8
	bl	sub_08158d68
	bl	sub_081434f8
	movs	r7, #240
	ldr	r6, [sp, #92]
	lsls	r7, r7, #7
	adds	r7, #232
	adds	r2, r6, r7
	movs	r3, #1
	movs	r0, #1
	str	r3, [r2, #0]
	bl	sub_08013560
	ldr	r0, [sp, #80]
	adds	r0, #1
	str	r0, [sp, #80]
	cmp	r0, #180
	beq.n	.L_08178622
	bl	.L_08177c28
.L_08178622:
	ldr	r2, [sp, #100]
	movs	r1, #0
	ldr	r0, [r2, #20]
	mov	r9, r1
	cmp	r0, #0
	beq.n	.L_08178638
	mov	ip, r0
.L_08178630:
	movs	r3, #1
	add	r9, r3
	cmp	r9, ip
	bne.n	.L_08178630
.L_08178638:
	movs	r0, #188
	bl	sub_0801314c
	ldr	r0, [pc, #60]
	bl	sub_08014644
	bl	sub_08143bb8
	add	sp, #324
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x03001120
	.4byte 0xffffc000
	.4byte 0xffff0000
	.4byte 0x08197410
	.4byte 0x02014200
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x08199340
	.4byte 0x08199210
	.2byte 0x3001
	.2byte 0x0814
.L_08178680:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	mov	r9, r0
	mov	ip, r1
	mov	lr, r3
	movs	r7, #0
	cmp	r3, #0
	beq.n	.L_081786f2
	subs	r5, r2, #1
	movs	r2, #3
	movs	r3, #1
	mov	r8, r2
	mov	r2, ip
	mov	sl, r3
	movs	r6, #2
	movs	r0, #1
	subs	r2, #1
	movs	r1, #0
	mov	r4, r9
.L_081786b0:
	lsls	r3, r7, #1
	mov	fp, r3
	mov	r3, sl
	strb	r3, [r4, #5]
	strb	r3, [r4, #9]
	strb	r3, [r4, #21]
	mov	r3, fp
	strb	r3, [r4, #0]
	mov	r3, r8
	strb	r3, [r4, #13]
	adds	r7, #1
	movs	r3, #2
	strb	r1, [r4, #4]
	strb	r1, [r4, #6]
	strb	r2, [r4, #8]
	strb	r1, [r4, #16]
	strb	r2, [r4, #18]
	strb	r2, [r4, #20]
	strb	r0, [r4, #1]
	strb	r6, [r4, #2]
	strb	r0, [r4, #12]
	strb	r6, [r4, #14]
	strb	r5, [r4, #7]
	strb	r5, [r4, #17]
	strb	r5, [r4, #19]
	add	r8, r3
	adds	r6, #2
	adds	r0, #2
	add	r2, ip
	add	r1, ip
	adds	r4, #24
	cmp	r7, lr
	bne.n	.L_081786b0
.L_081786f2:
	mov	r2, lr
	lsls	r3, r2, #1
	add	r3, lr
	lsls	r3, r3, #3
	add	r3, r9
	movs	r2, #0
	strb	r2, [r3, #0]
	strb	r2, [r3, #1]
	strb	r2, [r3, #2]
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
	sub	sp, #164
	str	r0, [sp, #84]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r0, [r3, #92]
	str	r0, [sp, #80]
	movs	r0, #1
	ldr	r1, [r3, #96]
	str	r1, [sp, #76]
	ldr	r2, [r3, #48]
	str	r2, [sp, #60]
	ldr	r3, [r3, #100]
	str	r3, [sp, #56]
	bl	sub_081435e0
	ldr	r4, [sp, #84]
	ldr	r3, [r4, #4]
	cmp	r3, #0
	bne.n	.L_0817874e
	movs	r0, #104
	movs	r1, #19
	bl	sub_081963ec
	b.n	.L_08178756
.L_0817874e:
	movs	r0, #104
	movs	r1, #23
	bl	sub_081963ec
.L_08178756:
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	movs	r2, #128
	str	r3, [sp, #68]
	ldr	r3, [pc, #56]
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r7, [sp, #80]
	movs	r0, #239
	movs	r1, #238
	lsls	r0, r0, #7
	lsls	r1, r1, #7
	adds	r2, r7, r0
	movs	r3, #2
	adds	r1, #132
	str	r3, [r2, #0]
	adds	r2, r7, r1
	movs	r3, #50
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #24]
	bl	sub_080145a8
	movs	r3, #0
	movs	r4, #0
	str	r3, [sp, #104]
	str	r4, [sp, #108]
	str	r3, [sp, #96]
	str	r4, [sp, #100]
	ldr	r2, [sp, #84]
	ldr	r0, [r2, #8]
	b.n	.L_081787a4
	.4byte 0x00001010
	.2byte 0x3001
	.2byte 0x0814
.L_081787a4:
	bl	sub_08118098
	ldr	r0, [r0, #0]
	movs	r1, #48
	str	r0, [sp, #52]
	bl	sub_08020098
	movs	r1, #2
	ldr	r0, [sp, #52]
	bl	sub_08020090
	ldr	r3, [sp, #84]
	mov	r4, sp
	adds	r4, #136
	ldr	r0, [r3, #8]
	adds	r1, r4, #0
	str	r4, [sp, #48]
	bl	sub_0815e21c
	ldr	r1, [sp, #84]
	mov	r2, sp
	adds	r2, #124
	movs	r7, #36
	ldrsh	r0, [r1, r7]
	adds	r1, r2, #0
	str	r2, [sp, #44]
	bl	sub_0815e21c
	ldr	r3, [sp, #80]
	movs	r4, #224
	lsls	r4, r4, #3
	adds	r1, r3, r4
	ldr	r0, [pc, #116]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	ldr	r0, [pc, #112]
	ldr	r1, [sp, #56]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r7, [sp, #80]
	movs	r2, #142
	lsls	r2, r2, #7
	adds	r1, r7, r2
	movs	r3, #1
	movs	r2, #1
	ldr	r0, [pc, #92]
	bl	sub_08157cf4
	ldr	r0, [pc, #88]
	bl	sub_08013300
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #84]
	movs	r2, #128
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4b0d
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r0, [pc, #68]
	movs	r2, #32
	movs	r3, #32
	movs	r1, #4
	bl	.L_08178680
	movs	r3, #0
	str	r3, [sp, #64]
	ldr	r4, [sp, #84]
	movs	r7, #60
	ldr	r2, [r4, #24]
	negs	r7, r7
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #1
	cmp	r3, r7
	bne.n	.L_0817884e
	b.n	.L_08178ea4
.L_0817884e:
	ldr	r0, [sp, #60]
	adds	r0, #12
	str	r0, [sp, #32]
	b.n	.L_08178874
	movs	r0, r0
	.4byte 0x00000080
	.4byte 0x000000f4
	.4byte 0x00000134
	.4byte 0x000000e8
	.4byte 0x00000130
	.4byte 0x03000730
	.2byte 0x2000
	.2byte 0x0201
.L_08178874:
	bl	sub_08014de4
	ldr	r1, [sp, #32]
	ldr	r0, [sp, #60]
	bl	sub_080156e8
	ldr	r1, [sp, #64]
	cmp	r1, #0
	bne.n	.L_081788a8
	add	r3, sp, #148
	str	r1, [r3, #0]
	str	r1, [r3, #4]
	str	r1, [r3, #8]
	str	r1, [r3, #12]
	ldr	r3, [sp, #80]
	movs	r2, #0
	mov	sl, r2
	adds	r3, #24
	subs	r2, #1
.L_0817889a:
	movs	r4, #1
	add	sl, r4
	mov	r7, sl
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r7, #64
	bne.n	.L_0817889a
.L_081788a8:
	ldr	r2, [sp, #84]
	movs	r3, #1
	ldr	r1, [r2, #24]
	movs	r0, #0
	negs	r3, r3
	mov	sl, r0
	cmp	r1, r3
	bne.n	.L_081788ba
	b.n	.L_081789ec
.L_081788ba:
	movs	r4, #28
	str	r4, [sp, #20]
	mov	fp, r0
.L_081788c0:
	ldr	r7, [sp, #64]
	ldr	r0, [sp, #20]
	cmp	r7, r0
	beq.n	.L_081788ca
	b.n	.L_081789d4
.L_081788ca:
	ldr	r2, [sp, #84]
	add	r5, sp, #112
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	adds	r1, r5, #0
	bl	sub_0815e21c
	ldr	r7, [sp, #80]
	movs	r3, #0
	mov	r9, r3
	mov	r8, r5
	add	r7, fp
.L_081788e2:
	mov	r4, r8
	ldr	r3, [r4, #0]
	movs	r5, #255
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	ldr	r3, [r4, #4]
	subs	r3, #16
	lsls	r3, r3, #16
	str	r3, [r7, #4]
	bl	sub_08014878
	adds	r6, r0, #0
	bl	sub_08014878
	ands	r5, r0
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r5, #127
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #16]
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	movs	r0, #1
	add	r9, r0
	adds	r3, #16
	mov	r1, r9
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r1, #32
	bne.n	.L_081788e2
	movs	r1, #128
	ldr	r3, [pc, #80]
	ldr	r2, [pc, #80]
	ldr	r0, [sp, #76]
	lsls	r1, r1, #7
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x24ee
	ldr	r3, [sp, #80]
	lsls	r4, r4, #7
	adds	r4, #168
	adds	r2, r3, r4
	movs	r3, #8
	str	r3, [r2, #0]
	ldr	r7, [sp, #64]
	cmp	r7, #28
	bne.n	.L_08178998
	ldr	r0, [sp, #84]
	ldr	r3, [r0, #24]
	cmp	r3, #0
	bne.n	.L_0817896e
	movs	r0, #144
	bl	sub_081180e8
	b.n	.L_08178974
.L_0817896e:
	movs	r0, #144
	bl	sub_081c0010
.L_08178974:
	ldr	r2, [sp, #84]
	movs	r3, #128
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	movs	r2, #150
	str	r2, [sp, #4]
	movs	r2, #128
	lsls	r3, r3, #11
	movs	r1, #1
	lsls	r2, r2, #10
	str	r3, [sp, #0]
	bl	sub_0815f000
	b.n	.L_081789bc
	.4byte 0x03000260
	.2byte 0x3f3f
	.2byte 0x3f3f
.L_08178998:
	movs	r0, #144
	bl	sub_081180e8
	ldr	r4, [sp, #84]
	movs	r2, #128
	movs	r3, #36
	ldrsh	r0, [r4, r3]
	movs	r3, #224
	lsls	r3, r3, #11
	str	r3, [sp, #0]
	movs	r3, #200
	str	r3, [sp, #4]
	movs	r3, #128
	movs	r1, #1
	lsls	r2, r2, #10
	lsls	r3, r3, #11
	bl	sub_0815f000
.L_081789bc:
	ldr	r1, [sp, #84]
	movs	r3, #8
	movs	r7, #36
	ldrsh	r0, [r1, r7]
	movs	r2, #5
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r3, #0
	bl	sub_0814cd48
	ldr	r2, [sp, #84]
	ldr	r1, [r2, #24]
.L_081789d4:
	ldr	r3, [sp, #20]
	movs	r4, #224
	adds	r3, #10
	movs	r7, #1
	str	r3, [sp, #20]
	lsls	r4, r4, #2
	add	sl, r7
	adds	r3, r1, #1
	add	fp, r4
	cmp	sl, r3
	beq.n	.L_081789ec
	b.n	.L_081788c0
.L_081789ec:
	ldr	r6, [pc, #852]
	ldr	r5, [sp, #80]
	movs	r0, #0
	mov	sl, r0
.L_081789f4:
	ldr	r0, [r5, #24]
	cmp	r0, #0
	blt.n	.L_08178a38
	asrs	r0, r0, #3
	adds	r0, #2
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r6, r3]
	ldr	r2, [sp, #56]
	adds	r1, r2, r1
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	movs	r7, #6
	ldrsh	r3, [r5, r7]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #76]
	ldr	r4, [sp, #68]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2280
	adds	r0, r5, #0
	movs	r1, #60
	lsls	r2, r2, #5
	bl	sub_08138086
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
.L_08178a38:
	movs	r7, #1
	add	sl, r7
	mov	r0, sl
	adds	r5, #28
	cmp	r0, #64
	bne.n	.L_081789f4
	ldr	r1, [sp, #64]
	cmp	r1, #15
	bgt.n	.L_08178a4c
	b.n	.L_08178e6c
.L_08178a4c:
	movs	r3, #44
	adds	r4, r1, #0
	adds	r1, r4, #0
	muls	r1, r3
	ldr	r7, [pc, #752]
	add	r2, sp, #148
	adds	r3, r1, r7
	mov	fp, r2
	str	r3, [r2, #0]
	movs	r2, #192
	lsls	r2, r2, #2
	adds	r2, #182
	cmp	r3, r2
	ble.n	.L_08178a6c
	mov	r0, fp
	str	r2, [r0, #0]
.L_08178a6c:
	ldr	r3, [sp, #64]
	cmp	r3, #31
	ble.n	.L_08178a80
	ldr	r4, [pc, #728]
	mov	r7, fp
	adds	r3, r1, r4
	str	r3, [r7, #4]
	cmp	r3, r2
	ble.n	.L_08178a80
	str	r2, [r7, #4]
.L_08178a80:
	ldr	r0, [sp, #64]
	cmp	r0, #29
	ble.n	.L_08178aa0
	movs	r3, #44
	muls	r3, r0
	ldr	r1, [pc, #708]
	mov	r2, fp
	adds	r3, r3, r1
	str	r3, [r2, #8]
	movs	r2, #192
	lsls	r2, r2, #2
	adds	r2, #182
	cmp	r3, r2
	ble.n	.L_08178aa0
	mov	r3, fp
	str	r2, [r3, #8]
.L_08178aa0:
	ldr	r4, [sp, #64]
	cmp	r4, #45
	ble.n	.L_08178abe
	movs	r3, #44
	muls	r3, r4
	ldr	r7, [pc, #680]
	movs	r2, #192
	lsls	r2, r2, #2
	adds	r3, r3, r7
	mov	r0, fp
	adds	r2, #182
	str	r3, [r0, #12]
	cmp	r3, r2
	ble.n	.L_08178abe
	str	r2, [r0, #12]
.L_08178abe:
	movs	r1, #0
	str	r1, [sp, #24]
	mov	r9, r1
.L_08178ac4:
	ldr	r2, [sp, #48]
	ldr	r3, [r2, #0]
	cmp	r3, #0
	bge.n	.L_08178ace
	adds	r3, #3
.L_08178ace:
	ldr	r4, [sp, #44]
	asrs	r0, r3, #2
	ldr	r3, [r4, #0]
	cmp	r3, #0
	bge.n	.L_08178ada
	adds	r3, #3
.L_08178ada:
	asrs	r3, r3, #2
	mov	r7, fp
	ldr	r2, [r7, #4]
	subs	r4, r0, r3
	ldr	r3, [r7, #0]
	subs	r3, r3, r2
	mov	r1, r9
	muls	r1, r3
	lsls	r3, r2, #5
	adds	r3, r3, r2
	adds	r1, r1, r3
	mov	r8, r1
	cmp	r4, #0
	bge.n	.L_08178af8
	negs	r4, r4
.L_08178af8:
	adds	r0, r4, #0
	movs	r1, #3
	str	r4, [sp, #8]
	bl	sub_08002054
	ldr	r1, [sp, #24]
	str	r0, [sp, #40]
	adds	r6, r1, #0
	adds	r6, #16
	ldr	r4, [sp, #8]
	cmp	r6, #32
	ble.n	.L_08178b12
	movs	r6, #32
.L_08178b12:
	movs	r0, #162
	lsls	r0, r0, #7
	adds	r0, #32
	adds	r3, r0, #0
	muls	r3, r6
	cmp	r3, #0
	bge.n	.L_08178b28
	movs	r2, #255
	lsls	r2, r2, #8
	adds	r2, #255
	adds	r3, r3, r2
.L_08178b28:
	asrs	r6, r3, #16
	lsrs	r3, r3, #31
	adds	r3, r6, r3
	ldr	r0, [sp, #40]
	asrs	r3, r3, #1
	negs	r3, r3
	movs	r7, #0
	mov	sl, r7
	adds	r2, r0, r3
	adds	r7, r4, r3
	lsls	r3, r1, #2
	ldr	r1, [pc, #536]
	adds	r5, r3, r1
.L_08178b42:
	mov	r0, r8
	str	r2, [sp, #12]
	str	r4, [sp, #8]
	bl	sub_08002090
	adds	r3, r7, #0
	muls	r3, r0
	ldr	r4, [sp, #8]
	asrs	r3, r3, #16
	subs	r3, r3, r4
	strb	r3, [r5, #0]
	mov	r0, r8
	bl	sub_08002096
	ldr	r2, [sp, #12]
	adds	r7, r7, r6
	adds	r3, r2, #0
	muls	r3, r0
	movs	r0, #1
	asrs	r3, r3, #16
	negs	r3, r3
	add	sl, r0
	strb	r3, [r5, #1]
	mov	r1, sl
	movs	r3, #0
	strb	r3, [r5, #2]
	adds	r2, r2, r6
	adds	r5, #4
	ldr	r4, [sp, #8]
	cmp	r1, #2
	bne.n	.L_08178b42
	mov	r2, r9
	cmp	r2, #32
	bne.n	.L_08178baa
	mov	r0, r8
	bl	sub_08002090
	ldr	r4, [sp, #8]
	adds	r3, r4, #0
	muls	r3, r0
	asrs	r3, r3, #16
	subs	r3, r3, r4
	mov	r0, r8
	str	r3, [sp, #104]
	bl	sub_08002096
	ldr	r4, [sp, #40]
	adds	r3, r4, #0
	muls	r3, r0
	asrs	r3, r3, #16
	negs	r3, r3
	str	r3, [sp, #96]
.L_08178baa:
	ldr	r7, [sp, #24]
	movs	r0, #1
	add	r9, r0
	adds	r7, #2
	mov	r1, r9
	str	r7, [sp, #24]
	cmp	r1, #33
	bne.n	.L_08178ac4
	movs	r2, #0
	str	r2, [sp, #16]
	mov	r9, r2
.L_08178bc0:
	ldr	r4, [sp, #48]
	ldr	r3, [r4, #0]
	cmp	r3, #0
	bge.n	.L_08178bca
	adds	r3, #3
.L_08178bca:
	ldr	r7, [sp, #44]
	asrs	r0, r3, #2
	ldr	r3, [r7, #0]
	cmp	r3, #0
	bge.n	.L_08178bd6
	adds	r3, #3
.L_08178bd6:
	asrs	r3, r3, #2
	subs	r4, r0, r3
	mov	r0, fp
	ldr	r2, [r0, #12]
	ldr	r3, [r0, #8]
	subs	r3, r3, r2
	mov	r1, r9
	muls	r1, r3
	lsls	r3, r2, #5
	adds	r3, r3, r2
	adds	r1, r1, r3
	mov	r8, r1
	cmp	r4, #0
	bge.n	.L_08178bf4
	negs	r4, r4
.L_08178bf4:
	lsls	r0, r4, #5
	movs	r1, #36
	str	r4, [sp, #8]
	bl	sub_08002054
	mov	r1, r9
	lsls	r6, r1, #2
	str	r0, [sp, #36]
	ldr	r4, [sp, #8]
	cmp	r6, #24
	ble.n	.L_08178c0c
	movs	r6, #24
.L_08178c0c:
	movs	r2, #162
	lsls	r2, r2, #7
	adds	r2, #32
	adds	r3, r2, #0
	muls	r3, r6
	cmp	r3, #0
	bge.n	.L_08178c22
	movs	r7, #255
	lsls	r7, r7, #8
	adds	r7, #255
	adds	r3, r3, r7
.L_08178c22:
	asrs	r6, r3, #16
	lsrs	r3, r3, #31
	adds	r3, r6, r3
	ldr	r1, [sp, #36]
	asrs	r3, r3, #1
	negs	r3, r3
	movs	r0, #0
	mov	sl, r0
	adds	r2, r1, r3
	adds	r7, r4, r3
	ldr	r0, [pc, #292]
	ldr	r3, [sp, #16]
	adds	r5, r3, r0
.L_08178c3c:
	mov	r0, r8
	str	r2, [sp, #12]
	str	r4, [sp, #8]
	bl	sub_08002090
	adds	r3, r7, #0
	muls	r3, r0
	ldr	r4, [sp, #8]
	asrs	r3, r3, #16
	subs	r3, r3, r4
	strb	r3, [r5, #0]
	mov	r0, r8
	bl	sub_08002096
	ldr	r2, [sp, #12]
	movs	r1, #0
	adds	r3, r2, #0
	muls	r3, r0
	asrs	r3, r3, #16
	strb	r3, [r5, #1]
	movs	r3, #1
	add	sl, r3
	mov	r0, sl
	strb	r1, [r5, #2]
	adds	r2, r2, r6
	adds	r7, r7, r6
	adds	r5, #4
	ldr	r4, [sp, #8]
	cmp	r0, #2
	bne.n	.L_08178c3c
	mov	r1, r9
	cmp	r1, #32
	bne.n	.L_08178ca0
	mov	r0, r8
	bl	sub_08002090
	ldr	r4, [sp, #8]
	adds	r3, r4, #0
	muls	r3, r0
	asrs	r3, r3, #16
	subs	r3, r3, r4
	mov	r0, r8
	str	r3, [sp, #108]
	bl	sub_08002096
	ldr	r2, [sp, #36]
	adds	r3, r2, #0
	muls	r3, r0
	asrs	r3, r3, #16
	str	r3, [sp, #100]
.L_08178ca0:
	ldr	r3, [sp, #16]
	movs	r4, #1
	add	r9, r4
	adds	r3, #8
	mov	r7, r9
	str	r3, [sp, #16]
	cmp	r7, #33
	bne.n	.L_08178bc0
	movs	r0, #1
	bl	sub_081969f8
	adds	r6, r0, #0
	ldr	r0, [pc, #164]
	movs	r2, #0
	movs	r3, #6
	add	r7, sp, #88
	str	r3, [r6, #0]
	strb	r2, [r6, #25]
	str	r7, [r6, #16]
	str	r2, [r6, #20]
	str	r0, [r6, #12]
	mov	sl, r2
	ldr	r2, [sp, #84]
	subs	r3, #7
	ldr	r1, [r2, #24]
	cmp	r1, r3
	bne.n	.L_08178cd8
	b.n	.L_08178e66
.L_08178cd8:
	ldr	r0, [pc, #124]
	ldr	r4, [sp, #48]
	str	r0, [sp, #28]
	movs	r2, #0
	mov	r8, r4
	mov	r9, r2
.L_08178ce4:
	mov	r3, r9
	adds	r3, #4
	mov	r4, fp
	mov	r0, r9
	ldr	r2, [r4, r3]
	ldr	r3, [r4, r0]
	cmp	r2, r3
	blt.n	.L_08178cf6
	b.n	.L_08178e4c
.L_08178cf6:
	movs	r1, #6
	strb	r1, [r7, #0]
	movs	r3, #5
	add	r7, sp, #88
	strb	r3, [r7, #1]
	ldr	r2, [sp, #80]
	ldr	r0, [pc, #96]
	movs	r4, #224
	lsls	r4, r4, #3
	adds	r3, r2, r4
	str	r3, [r7, #4]
	str	r0, [r6, #8]
	ldr	r1, [sp, #64]
	movs	r3, #127
	lsls	r2, r1, #3
	bics	r3, r2
	strb	r3, [r6, #24]
	bl	sub_08014de4
	ldr	r2, [sp, #84]
	ldr	r3, [r2, #4]
	cmp	r3, #0
	bne.n	.L_08178d6c
	mov	r3, r8
	ldr	r0, [r3, #0]
	mov	r4, r8
	lsrs	r3, r0, #31
	ldr	r1, [r4, #4]
	ldr	r2, [pc, #56]
	adds	r0, r0, r3
	asrs	r0, r0, #1
	subs	r0, #68
	lsls	r1, r1, #16
	adds	r1, r1, r2
	lsls	r0, r0, #16
	movs	r2, #0
	bl	sub_08015160
	b.n	.L_08178d8a
	.4byte 0x08197410
	.4byte 0xfffffd40
	.4byte 0xfffffa80
	.4byte 0xfffffad8
	.4byte 0xfffff818
	.4byte 0x02010000
	.4byte 0x02010108
	.4byte 0x02011000
	.4byte 0x02012000
	.2byte 0x0000
	.2byte 0xffa0
.L_08178d6c:
	.2byte 0x4643
	ldr	r0, [r3, #0]
	mov	r4, r8
	lsrs	r3, r0, #31
	ldr	r1, [r4, #4]
	ldr	r2, [pc, #340]
	adds	r0, r0, r3
	asrs	r0, r0, #1
	subs	r0, #52
	lsls	r1, r1, #16
	adds	r1, r1, r2
	lsls	r0, r0, #16
	movs	r2, #0
	bl	sub_08015160
.L_08178d8a:
	movs	r5, #128
	lsls	r5, r5, #8
	movs	r1, #128
	adds	r0, r5, #0
	lsls	r1, r1, #9
	adds	r2, r5, #0
	bl	sub_080151e4
	ldr	r4, [sp, #84]
	ldr	r3, [r4, #4]
	cmp	r3, #1
	bne.n	.L_08178da8
	adds	r0, r5, #0
	bl	sub_08015068
.L_08178da8:
	movs	r3, #220
	lsls	r3, r3, #6
	adds	r3, #112
	mov	r0, sl
	muls	r0, r3
	ldr	r1, [pc, #284]
	adds	r0, r0, r1
	bl	sub_080150e4
	movs	r0, #128
	lsls	r0, r0, #11
	bl	sub_0801521c
	movs	r2, #66
	ldr	r0, [sp, #28]
	ldr	r1, [pc, #268]
	bl	sub_08196958
	adds	r0, r6, #0
	bl	sub_08196a7c
	mov	r2, sl
	ldr	r4, [sp, #64]
	lsls	r3, r2, #4
	adds	r3, #40
	cmp	r4, r3
	bge.n	.L_08178e48
	add	r0, sp, #88
	adds	r3, r0, #0
	movs	r7, #6
	strb	r7, [r0, #0]
	adds	r7, r3, #0
	ldr	r3, [pc, #236]
	movs	r2, #0
	movs	r1, #6
	strb	r2, [r6, #24]
	strb	r1, [r7, #1]
	str	r3, [r6, #8]
	ldr	r2, [sp, #80]
	movs	r4, #142
	lsls	r4, r4, #7
	adds	r3, r2, r4
	mov	r0, sl
	lsls	r2, r0, #2
	str	r3, [r7, #4]
	add	r3, sp, #104
	ldr	r0, [r2, r3]
	add	r3, sp, #96
	ldr	r1, [r2, r3]
	lsls	r0, r0, #16
	lsls	r1, r1, #16
	movs	r2, #0
	bl	sub_08015160
	mov	r1, r9
	mov	r2, fp
	ldr	r3, [r1, r2]
	movs	r0, #192
	lsls	r0, r0, #6
	lsls	r3, r3, #5
	adds	r0, #96
	subs	r0, r0, r3
	bl	sub_080150e4
	movs	r0, #162
	lsls	r0, r0, #7
	adds	r0, #32
	bl	sub_0801521c
	ldr	r1, [pc, #160]
	ldr	r0, [pc, #164]
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r6, #0
	bl	sub_08196a7c
	ldr	r3, [sp, #84]
	ldr	r1, [r3, #24]
	b.n	.L_08178e4c
.L_08178e48:
	ldr	r4, [sp, #84]
	ldr	r1, [r4, #24]
.L_08178e4c:
	ldr	r0, [sp, #28]
	movs	r2, #132
	movs	r3, #8
	lsls	r2, r2, #1
	movs	r4, #1
	adds	r0, r0, r2
	add	r9, r3
	add	sl, r4
	adds	r3, r1, #1
	str	r0, [sp, #28]
	cmp	sl, r3
	beq.n	.L_08178e66
	b.n	.L_08178ce4
.L_08178e66:
	adds	r0, r6, #0
	bl	sub_08013164
.L_08178e6c:
	bl	sub_081434f8
	movs	r1, #2
	movs	r0, #2
	bl	sub_08158ce0
	movs	r0, #240
	ldr	r7, [sp, #80]
	lsls	r0, r0, #7
	adds	r0, #232
	adds	r2, r7, r0
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r1, [sp, #64]
	ldr	r3, [sp, #84]
	adds	r1, #1
	str	r1, [sp, #64]
	ldr	r2, [r3, #24]
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r3, r3, #1
	adds	r3, #60
	cmp	r1, r3
	beq.n	.L_08178ea4
	b.n	.L_08178874
.L_08178ea4:
	ldr	r0, [sp, #52]
	movs	r1, #16
	bl	sub_08020098
	movs	r0, #104
	bl	sub_0801314c
	ldr	r0, [pc, #44]
	bl	sub_08014644
	bl	sub_08143bb8
	add	sp, #164
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0xffa00000
	.4byte 0xfffff000
	.4byte 0x02011000
	.4byte 0x08199340
	.4byte 0x081991e0
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #36
	str	r0, [sp, #12]
	movs	r5, #192
	lsls	r5, r5, #18
	ldr	r0, [r5, #92]
	ldr	r1, [r5, #96]
	mov	r8, r0
	movs	r0, #128
	lsls	r0, r0, #6
	adds	r0, #1
	mov	fp, r1
	ldr	r6, [pc, #68]
	bl	sub_08143a88
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r0, [pc, #56]
	movs	r2, #1
	movs	r3, #1
	adds	r1, r6, #0
	bl	sub_08157cf4
	movs	r1, #35
	movs	r0, #104
	bl	sub_081963ec
	ldr	r3, [r5, #104]
	movs	r1, #39
	movs	r0, #188
	str	r3, [sp, #16]
	bl	sub_081963ec
	adds	r5, #188
	ldr	r3, [r5, #0]
	add	r2, sp, #16
	str	r3, [r2, #4]
	mov	sl, r2
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, r8
	b.n	.L_08178f58
	movs	r0, r0
	.4byte 0x00000100
	.4byte 0x02010000
	.2byte 0x00dc
	.2byte 0x0000
.L_08178f58:
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	movs	r3, #0
	add	r2, r8
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #84]
	bl	sub_080145a8
	ldr	r1, [sp, #12]
	mov	r2, sp
	adds	r2, #24
	movs	r3, #36
	ldrsh	r0, [r1, r3]
	adds	r1, r2, #0
	str	r2, [sp, #8]
	bl	sub_0815e20c
	movs	r3, #0
	mov	r9, r3
.L_08178f88:
	mov	r0, r9
	cmp	r0, #0
	bne.n	.L_08178fea
	ldr	r1, [sp, #12]
	ldr	r3, [r1, #4]
	cmp	r3, #0
	bne.n	.L_08178fa4
	ldr	r2, [sp, #8]
	mov	r0, r8
	ldr	r3, [r2, #0]
	adds	r3, #40
	lsls	r3, r3, #16
	str	r3, [r0, #0]
	b.n	.L_08178fb0
.L_08178fa4:
	ldr	r1, [sp, #8]
	mov	r2, r8
	ldr	r3, [r1, #0]
	subs	r3, #168
	lsls	r3, r3, #16
	str	r3, [r2, #0]
.L_08178fb0:
	ldr	r3, [pc, #20]
	mov	r0, r8
	str	r3, [r0, #4]
	ldr	r1, [sp, #12]
	ldr	r3, [r1, #4]
	cmp	r3, #0
	bne.n	.L_08178fd0
	ldr	r3, [pc, #12]
	str	r3, [r0, #12]
	b.n	.L_08178fd8
	.4byte 0x08143001
	.4byte 0xff9c0000
	.2byte 0x0000
	.2byte 0xfffa
.L_08178fd0:
	.2byte 0x23c0
	lsls	r3, r3, #11
	mov	r2, r8
	str	r3, [r2, #12]
.L_08178fd8:
	movs	r3, #128
	lsls	r3, r3, #12
	mov	r0, r8
	str	r3, [r0, #16]
	movs	r1, #128
	ldr	r3, [pc, #52]
	lsls	r1, r1, #19
	adds	r1, #82
	strh	r3, [r1, #0]
.L_08178fea:
	mov	r2, r9
	cmp	r2, #31
	bgt.n	.L_08178ffa
	mov	r0, r8
	movs	r1, #60
	movs	r2, #0
	bl	sub_08138086
.L_08178ffa:
	mov	r3, r9
	cmp	r3, #32
	bgt.n	.L_08179028
	lsrs	r3, r3, #31
	add	r3, r9
	asrs	r1, r3, #1
	ldr	r2, [pc, #20]
	lsrs	r3, r3, #31
	adds	r3, r1, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	movs	r0, #128
	lsls	r2, r2, #8
	b.n	.L_08179020
	movs	r0, r0
	.4byte 0x00001000
	.2byte 0x0010
	.2byte 0x0000
.L_08179020:
	lsls	r0, r0, #19
	orrs	r2, r1
	adds	r0, #82
	strh	r2, [r0, #0]
.L_08179028:
	mov	r1, r9
	cmp	r1, #91
	ble.n	.L_08179060
	mov	r2, r9
	movs	r1, #108
	subs	r1, r1, r2
	lsrs	r2, r1, #31
	ldr	r3, [pc, #32]
	adds	r2, r1, r2
	asrs	r2, r2, #1
	subs	r3, r3, r2
	movs	r0, #128
	lsls	r3, r3, #8
	lsls	r0, r0, #19
	orrs	r3, r1
	adds	r0, #82
	strh	r3, [r0, #0]
	movs	r1, #60
	mov	r0, r8
	ldr	r2, [pc, #12]
	bl	sub_08138086
	b.n	.L_08179060
	movs	r0, r0
	.4byte 0x00000010
	.2byte 0x8000
	.2byte 0xffff
.L_08179060:
	.2byte 0x4649
	lsls	r6, r1, #8
	adds	r0, r6, #0
	bl	sub_08002096
	mov	r3, r8
	movs	r2, #2
	ldrsh	r5, [r3, r2]
	lsls	r0, r0, #3
	asrs	r0, r0, #16
	adds	r5, r5, r0
	adds	r0, r6, #0
	bl	sub_08002090
	mov	r2, r8
	movs	r1, #6
	ldrsh	r3, [r2, r1]
	lsls	r0, r0, #3
	asrs	r0, r0, #16
	adds	r7, r3, r0
	movs	r3, #128
	lsls	r3, r3, #19
	negs	r5, r5
	adds	r3, #40
	lsls	r5, r5, #8
	str	r5, [r3, #0]
	ldr	r3, [sp, #12]
	ldr	r1, [r3, #4]
	cmp	r1, #0
	bne.n	.L_081790fe
	movs	r2, #96
	str	r2, [sp, #0]
	movs	r2, #83
	str	r2, [sp, #4]
	mov	r0, sl
	ldr	r4, [r0, #0]
	ldr	r1, [pc, #688]
	adds	r3, r7, #2
	mov	r0, fp
	movs	r2, #25
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4649
	cmp	r1, #47
	bhi.n	.L_081790dc
	ldr	r2, [sp, #12]
	mov	r0, sl
	ldr	r1, [r2, #4]
	movs	r2, #48
	str	r2, [sp, #0]
	movs	r2, #51
	str	r2, [sp, #4]
	lsls	r1, r1, #2
	adds	r3, r7, #0
	ldr	r4, [r1, r0]
	adds	r3, #22
	mov	r0, fp
	ldr	r1, [pc, #652]
	movs	r2, #31
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe041
.L_081790dc:
	ldr	r2, [sp, #12]
	mov	r0, sl
	ldr	r1, [r2, #4]
	movs	r2, #42
	str	r2, [sp, #0]
	movs	r2, #35
	str	r2, [sp, #4]
	lsls	r1, r1, #2
	adds	r3, r7, #0
	ldr	r4, [r1, r0]
	adds	r3, #16
	mov	r0, fp
	ldr	r1, [pc, #620]
	movs	r2, #64
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe030
.L_081790fe:
	movs	r2, #96
	str	r2, [sp, #0]
	movs	r2, #83
	str	r2, [sp, #4]
	lsls	r1, r1, #2
	mov	r2, sl
	ldr	r4, [r1, r2]
	adds	r3, r7, #2
	mov	r0, fp
	ldr	r1, [pc, #584]
	movs	r2, #7
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x464b
	cmp	r3, #47
	bhi.n	.L_08179140
	ldr	r0, [sp, #12]
	movs	r2, #48
	ldr	r1, [r0, #4]
	str	r2, [sp, #0]
	movs	r2, #51
	str	r2, [sp, #4]
	lsls	r1, r1, #2
	mov	r2, sl
	adds	r3, r7, #0
	ldr	r4, [r1, r2]
	adds	r3, #22
	mov	r0, fp
	ldr	r1, [pc, #552]
	movs	r2, #49
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe00f
.L_08179140:
	ldr	r3, [sp, #12]
	movs	r2, #42
	ldr	r1, [r3, #4]
	str	r2, [sp, #0]
	movs	r2, #35
	str	r2, [sp, #4]
	lsls	r1, r1, #2
	mov	r0, sl
	adds	r3, r7, #0
	ldr	r4, [r1, r0]
	adds	r3, #16
	mov	r0, fp
	ldr	r1, [pc, #520]
	movs	r2, #22
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4648
	subs	r0, #28
	cmp	r0, #11
	bhi.n	sub_081791d4
	movs	r1, #3
	bl	sub_08002054
	ldr	r1, [sp, #12]
	adds	r6, r0, #0
	ldr	r5, [r1, #4]
	cmp	r5, #0
	bne.n	.L_081791a6
	ldr	r2, [pc, #492]
	lsls	r3, r6, #1
	ldrh	r1, [r2, r3]
	ldr	r3, [pc, #492]
	ldr	r2, [pc, #492]
	ldrb	r4, [r3, r6]
	adds	r1, r1, r2
	lsrs	r3, r4, #1
	movs	r2, #46
	subs	r2, r2, r3
	ldr	r3, [pc, #484]
	ldrb	r0, [r3, r6]
	str	r4, [sp, #0]
	str	r0, [sp, #4]
	lsrs	r3, r0, #1
	subs	r3, r7, r3
	mov	r0, sl
	ldr	r4, [r0, #0]
	adds	r3, #61
	mov	r0, fp
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe016
.L_081791a6:
	ldr	r2, [pc, #448]
	lsls	r3, r6, #1
	ldrh	r1, [r2, r3]
	ldr	r3, [pc, #444]
	ldr	r2, [pc, #448]
	ldrb	r0, [r3, r6]
	ldr	r3, [pc, #448]
	adds	r1, r1, r2
	ldrb	r4, [r3, r6]
	str	r0, [sp, #0]
	str	r4, [sp, #4]
	lsrs	r2, r0, #1
	lsrs	r3, r4, #1
	subs	r2, r2, r0
	lsls	r5, r5, #2
	mov	r0, sl
	subs	r3, r7, r3
	ldr	r4, [r5, r0]
	adds	r2, #82
	adds	r3, #61
	mov	r0, fp
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4649
	cmp	r1, #39
	ble.n	sub_08179292
	cmp	r1, #47
	bgt.n	.L_08179220
	ldr	r2, [sp, #12]
	ldr	r1, [r2, #4]
	cmp	r1, #0
	bne.n	.L_08179202
	movs	r2, #32
	str	r2, [sp, #0]
	movs	r2, #29
	str	r2, [sp, #4]
	mov	r0, sl
	adds	r3, r7, #0
	ldr	r4, [r0, #0]
	adds	r3, #46
	mov	r0, fp
	ldr	r1, [pc, #380]
	movs	r2, #30
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe047
.L_08179202:
	movs	r2, #32
	str	r2, [sp, #0]
	movs	r2, #29
	str	r2, [sp, #4]
	lsls	r1, r1, #2
	mov	r2, sl
	adds	r3, r7, #0
	ldr	r4, [r1, r2]
	adds	r3, #46
	mov	r0, fp
	ldr	r1, [pc, #352]
	movs	r2, #66
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe038
.L_08179220:
	mov	r0, r9
	subs	r0, #56
	movs	r1, #6
	bl	sub_08002054
	adds	r4, r0, #0
	cmp	r4, #0
	bge.n	.L_08179232
	movs	r4, #0
.L_08179232:
	cmp	r4, #7
	bgt.n	sub_08179292
	ldr	r3, [sp, #12]
	ldr	r0, [r3, #4]
	cmp	r0, #0
	bne.n	.L_08179268
	ldr	r2, [pc, #316]
	lsls	r3, r4, #1
	ldrh	r1, [r2, r3]
	ldr	r3, [pc, #312]
	movs	r2, #32
	ldrb	r3, [r3, r4]
	str	r2, [sp, #0]
	ldr	r2, [pc, #308]
	ldr	r0, [pc, #296]
	ldrb	r2, [r2, r4]
	adds	r3, r7, r3
	str	r2, [sp, #4]
	mov	r2, sl
	adds	r1, r1, r0
	ldr	r4, [r2, #0]
	subs	r3, #8
	mov	r0, fp
	movs	r2, #63
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe014
.L_08179268:
	ldr	r2, [pc, #272]
	lsls	r3, r4, #1
	ldrh	r1, [r2, r3]
	ldr	r3, [pc, #264]
	movs	r2, #32
	adds	r1, r1, r3
	ldr	r3, [pc, #264]
	lsls	r0, r0, #2
	ldrb	r3, [r3, r4]
	str	r2, [sp, #0]
	ldr	r2, [pc, #260]
	adds	r3, r7, r3
	ldrb	r2, [r2, r4]
	subs	r3, #8
	str	r2, [sp, #4]
	mov	r2, sl
	ldr	r4, [r0, r2]
	mov	r0, fp
	movs	r2, #33
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x464b
	cmp	r3, #80
	bne.n	.L_081792be
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, r8
	movs	r3, #6
	str	r3, [r2, #0]
	movs	r0, #134
	bl	sub_081180e8
	ldr	r2, [sp, #12]
	movs	r3, #16
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	movs	r3, #0
	bl	sub_0814cd48
.L_081792be:
	mov	r3, r9
	cmp	r3, #30
	bne.n	.L_081792f8
	movs	r1, #128
	lsls	r1, r1, #7
	ldr	r3, [pc, #188]
	ldr	r2, [pc, #192]
	mov	r0, fp
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x22ee
	lsls	r2, r2, #7
	adds	r2, #168
	add	r2, r8
	movs	r3, #3
	str	r3, [r2, #0]
	movs	r0, #212
	bl	sub_081c0010
	ldr	r2, [sp, #12]
	movs	r3, #16
	movs	r1, #36
	ldrsh	r0, [r2, r1]
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	movs	r3, #0
	bl	sub_0814cd48
.L_081792f8:
	movs	r0, #4
	movs	r1, #4
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	movs	r3, #1
	add	r2, r8
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r3, #1
	add	r9, r3
	mov	r0, r9
	cmp	r0, #108
	beq.n	.L_08179322
	b.n	.L_08178f88
.L_08179322:
	ldr	r0, [pc, #108]
	bl	sub_08014644
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #36]
	movs	r1, #206
	lsls	r1, r1, #3
	adds	r3, r3, r1
	ldrh	r1, [r3, #0]
	movs	r2, #24
	movs	r0, #1
	bl	sub_08118040
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x02010000
	.4byte 0x020124de
	.4byte 0x02011f20
	.4byte 0x081993d8
	.4byte 0x081993d0
	.4byte 0x02012e6e
	.4byte 0x081993d4
	.4byte 0x02014e9d
	.4byte 0x081993f0
	.4byte 0x081993e8
	.4byte 0x081993e0
	.4byte 0x03000260
	.4byte 0x3f3f3f3f
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r5, #192
	lsls	r5, r5, #18
	mov	fp, r0
	ldr	r0, [r5, #92]
	sub	sp, #172
	str	r0, [sp, #64]
	movs	r0, #1
	ldr	r1, [r5, #96]
	mov	r6, fp
	str	r1, [sp, #60]
	ldr	r2, [r5, #100]
	str	r2, [sp, #44]
	bl	sub_081435e0
	ldr	r3, [pc, #52]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	movs	r1, #3
	movs	r0, #104
	bl	sub_081963ec
	ldr	r3, [r5, #104]
	movs	r0, #188
	movs	r1, #7
	str	r3, [sp, #48]
	bl	sub_081963ec
	adds	r5, #188
	ldr	r5, [r5, #0]
	ldr	r2, [r6, #20]
	movs	r4, #0
	str	r5, [sp, #52]
	mov	r8, r4
	cmp	r2, #0
	beq.n	.L_08179414
	add	r7, sp, #76
	adds	r6, r7, #0
	movs	r5, #36
	b.n	.L_081793f8
	.2byte 0x0a10
	.2byte 0x0000
.L_081793f8:
	mov	r1, fp
	ldrsh	r0, [r5, r1]
	adds	r1, r6, #0
	bl	sub_0815e20c
	mov	r4, fp
	ldr	r2, [r4, #20]
	movs	r3, #1
	add	r8, r3
	adds	r6, #12
	adds	r5, #2
	cmp	r8, r2
	bne.n	.L_081793f8
	b.n	.L_08179416
.L_08179414:
	add	r7, sp, #76
.L_08179416:
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #2
	subs	r3, #12
	ldr	r1, [r7, #0]
	ldr	r3, [r7, r3]
	ldr	r0, [pc, #928]
	subs	r3, r3, r1
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	adds	r1, r1, r3
	str	r1, [sp, #40]
	ldr	r3, [r7, #4]
	ldr	r1, [pc, #916]
	subs	r3, #48
	str	r3, [sp, #36]
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	ldr	r6, [sp, #64]
	movs	r7, #224
	lsls	r7, r7, #3
	adds	r1, r6, r7
	ldr	r0, [pc, #892]
	movs	r2, #64
	movs	r3, #64
	bl	sub_0816ae40
	ldr	r0, [pc, #888]
	ldr	r1, [sp, #44]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r2, #142
	lsls	r2, r2, #7
	adds	r1, r6, r2
	ldr	r0, [pc, #872]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r0, [pc, #868]
	ldr	r1, [pc, #852]
	movs	r2, #1
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r0, [pc, #860]
	ldr	r1, [pc, #860]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r3, #239
	movs	r4, #238
	lsls	r3, r3, #7
	lsls	r4, r4, #7
	adds	r2, r6, r3
	adds	r4, #132
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r1, #200
	adds	r2, r6, r4
	movs	r3, #50
	str	r3, [r2, #0]
	ldr	r0, [pc, #832]
	lsls	r1, r1, #4
	bl	sub_080145a8
	ldr	r0, [sp, #40]
	mov	r7, sp
	movs	r6, #0
	adds	r7, #68
	lsls	r0, r0, #16
	str	r6, [sp, #56]
	str	r7, [sp, #16]
	str	r0, [sp, #12]
.L_081794b6:
	ldr	r1, [sp, #56]
	cmp	r1, #0
	bne.n	.L_081794f4
	ldr	r3, [sp, #64]
	movs	r2, #0
	mov	r8, r2
	adds	r3, #24
	movs	r2, #24
.L_081794c6:
	movs	r4, #1
	add	r8, r4
	mov	r6, r8
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r6, #64
	bne.n	.L_081794c6
	ldr	r3, [pc, #780]
	movs	r7, #0
	movs	r1, #1
	movs	r2, #128
	mov	r8, r7
	negs	r1, r1
	lsls	r2, r2, #1
.L_081794e2:
	movs	r0, #1
	add	r8, r0
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r8, r2
	bne.n	.L_081794e2
	movs	r0, #140
	bl	sub_081c0010
.L_081794f4:
	ldr	r3, [sp, #56]
	subs	r3, #40
	cmp	r3, #15
	bhi.n	.L_08179502
	ldr	r0, [pc, #744]
	bl	sub_0815f0a0
.L_08179502:
	ldr	r1, [sp, #56]
	cmp	r1, #56
	bne.n	.L_0817950e
	movs	r0, #144
	bl	sub_081c0010
.L_0817950e:
	ldr	r2, [sp, #56]
	cmp	r2, #55
	bgt.n	.L_08179516
	b.n	.L_0817979c
.L_08179516:
	ldr	r4, [sp, #36]
	ldr	r6, [pc, #720]
	ldr	r7, [pc, #724]
	lsls	r3, r2, #3
	adds	r3, r4, r3
	adds	r6, r6, r3
	adds	r3, r3, r7
	mov	sl, r6
	cmp	r3, #7
	bls.n	.L_0817952c
	b.n	.L_08179680
.L_0817952c:
	ldr	r1, [sp, #40]
	ldr	r2, [sp, #40]
	asrs	r1, r1, #31
	lsrs	r3, r1, #31
	adds	r3, r2, r3
	ldr	r6, [sp, #64]
	movs	r0, #0
	asrs	r3, r3, #1
	mov	r8, r0
	mov	r9, r1
	lsls	r7, r3, #16
.L_08179542:
	mov	r4, r8
	negs	r3, r4
	cmp	r3, #0
	bge.n	.L_0817954c
	adds	r3, #3
.L_0817954c:
	asrs	r3, r3, #2
	adds	r3, #2
	str	r3, [r6, #24]
	str	r7, [r6, #0]
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	add	r3, sl
	adds	r3, #16
	lsls	r3, r3, #16
	str	r3, [r6, #4]
	bl	sub_08014878
	mov	r1, fp
	ldr	r3, [r1, #24]
	ldr	r2, [pc, #644]
	lsls	r3, r3, #2
	adds	r3, #1
	ldrb	r5, [r2, r3]
	adds	r1, r5, #0
	bl	sub_0800206c
	lsrs	r5, r5, #1
	subs	r0, r0, r5
	lsls	r0, r0, #12
	str	r0, [r6, #12]
	bl	sub_08014878
	movs	r2, #255
	ands	r2, r0
	movs	r3, #192
	subs	r3, r3, r2
	lsls	r3, r3, #10
	str	r3, [r6, #16]
	movs	r3, #1
	add	r8, r3
	mov	r4, r8
	adds	r6, #28
	cmp	r4, #32
	bne.n	.L_08179542
	mov	r7, fp
	ldr	r3, [r7, #24]
	ldr	r0, [pc, #592]
	lsls	r3, r3, #2
	adds	r3, #3
	ldrb	r3, [r0, r3]
	movs	r6, #0
	mov	r8, r6
	cmp	r3, #0
	beq.n	.L_08179634
	ldr	r2, [sp, #40]
	mov	r1, r9
	lsrs	r3, r1, #31
	adds	r3, r2, r3
	asrs	r3, r3, #1
	lsls	r3, r3, #16
	ldr	r7, [pc, #568]
	str	r3, [sp, #32]
	movs	r3, #15
	mov	r9, r3
.L_081795c6:
	ldr	r4, [sp, #32]
	mov	r6, r9
	str	r4, [r7, #0]
	bl	sub_08014878
	ands	r0, r6
	add	r0, sl
	adds	r0, #24
	lsls	r0, r0, #16
	str	r0, [r7, #4]
	bl	sub_08014878
	movs	r6, #254
	lsls	r6, r6, #7
	adds	r6, #255
	ands	r6, r0
	movs	r0, #128
	lsls	r0, r0, #7
	adds	r6, r6, r0
	bl	sub_08014878
	movs	r5, #255
	ands	r5, r0
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r5, #127
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #5
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #5
	str	r3, [r7, #16]
	bl	sub_08014878
	mov	r1, r9
	ands	r0, r1
	adds	r0, #16
	str	r0, [r7, #24]
	mov	r4, fp
	ldr	r3, [r4, #24]
	ldr	r6, [pc, #464]
	lsls	r3, r3, #2
	adds	r3, #3
	ldrb	r3, [r6, r3]
	movs	r2, #1
	add	r8, r2
	adds	r7, #28
	cmp	r8, r3
	bne.n	.L_081795c6
.L_08179634:
	movs	r0, #145
	bl	sub_081180e8
	mov	r0, fp
	ldr	r3, [r0, #20]
	movs	r7, #0
	mov	r8, r7
	cmp	r3, #0
	beq.n	.L_08179672
	movs	r5, #36
.L_08179648:
	mov	r1, fp
	ldrsh	r0, [r5, r1]
	movs	r1, #0
	bl	sub_08118088
	mov	r3, fp
	ldrsh	r0, [r5, r3]
	movs	r3, #16
	str	r3, [sp, #0]
	movs	r1, #7
	mov	r3, r8
	movs	r2, #5
	mov	r7, fp
	bl	sub_0814cd48
	ldr	r3, [r7, #20]
	movs	r6, #1
	add	r8, r6
	adds	r5, #2
	cmp	r8, r3
	bne.n	.L_08179648
.L_08179672:
	ldr	r0, [sp, #64]
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #168
	adds	r2, r0, r1
	movs	r3, #16
	str	r3, [r2, #0]
.L_08179680:
	mov	r2, sl
	cmp	r2, #52
	bgt.n	sub_081796bc
	ldr	r3, [sp, #40]
	movs	r4, #24
	lsrs	r5, r3, #31
	adds	r5, r3, r5
	asrs	r5, r5, #1
	movs	r6, #64
	adds	r2, r5, #0
	str	r4, [sp, #0]
	ldr	r1, [pc, #304]
	subs	r2, #24
	mov	r3, sl
	str	r4, [sp, #8]
	str	r6, [sp, #4]
	ldr	r0, [sp, #60]
	ldr	r7, [sp, #48]
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x9c02
	str	r6, [sp, #4]
	str	r4, [sp, #0]
	ldr	r0, [sp, #60]
	ldr	r1, [pc, #276]
	adds	r2, r5, #0
	mov	r3, sl
	ldr	r4, [sp, #52]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x465f
	movs	r6, #0
	ldr	r2, [r7, #24]
	mov	r8, r6
	ldr	r6, [pc, #300]
	lsls	r3, r2, #2
	ldrb	r3, [r6, r3]
	cmp	r3, #0
	beq.n	.L_08179730
	ldr	r5, [sp, #64]
	adds	r7, r6, #0
.L_081796d2:
	ldr	r3, [r5, #24]
	cmp	r3, #23
	bhi.n	.L_0817971a
	cmp	r3, #1
	ble.n	sub_0817970e
	adds	r1, r3, #0
	cmp	r3, #0
	bge.n	.L_081796e4
	adds	r1, r3, #3
.L_081796e4:
	ldr	r0, [sp, #64]
	asrs	r1, r1, #2
	lsls	r1, r1, #11
	movs	r2, #142
	adds	r1, r0, r1
	lsls	r2, r2, #7
	adds	r1, r1, r2
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	movs	r4, #6
	ldrsh	r3, [r5, r4]
	movs	r0, #32
	str	r0, [sp, #0]
	movs	r0, #64
	str	r0, [sp, #4]
	subs	r2, #16
	subs	r3, #32
	ldr	r0, [sp, #60]
	ldr	r4, [sp, #48]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c28
	movs	r1, #60
	ldr	r2, [pc, #232]
	bl	sub_08138086
	ldr	r3, [r5, #24]
.L_0817971a:
	adds	r3, #1
	str	r3, [r5, #24]
	mov	r1, fp
	ldr	r2, [r1, #24]
	movs	r0, #1
	lsls	r3, r2, #2
	ldrb	r3, [r7, r3]
	add	r8, r0
	adds	r5, #28
	cmp	r8, r3
	bne.n	.L_081796d2
.L_08179730:
	movs	r3, #0
	mov	r8, r3
	lsls	r3, r2, #2
	adds	r3, #3
	ldrb	r3, [r6, r3]
	cmp	r3, #0
	beq.n	.L_0817979c
	ldr	r5, [pc, #184]
	ldr	r7, [pc, #188]
.L_08179742:
	ldr	r0, [r5, #24]
	cmp	r0, #0
	blt.n	.L_0817978c
	asrs	r0, r0, #3
	adds	r0, #1
	lsls	r4, r0, #1
	subs	r3, r4, #2
	ldrh	r1, [r7, r3]
	ldr	r6, [sp, #44]
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	adds	r1, r6, r1
	subs	r2, r2, r3
	movs	r6, #6
	ldrsh	r3, [r5, r6]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #60]
	ldr	r4, [sp, #48]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2280
	adds	r0, r5, #0
	lsls	r2, r2, #5
	movs	r1, #60
	bl	sub_08138086
	ldr	r3, [r5, #24]
	mov	r0, fp
	subs	r3, #1
	str	r3, [r5, #24]
	ldr	r6, [pc, #104]
	ldr	r2, [r0, #24]
.L_0817978c:
	lsls	r3, r2, #2
	adds	r3, #3
	ldrb	r3, [r6, r3]
	movs	r1, #1
	add	r8, r1
	adds	r5, #28
	cmp	r8, r3
	bne.n	.L_08179742
.L_0817979c:
	movs	r0, #32
	bl	sub_08014dac
	str	r0, [sp, #28]
	movs	r0, #1
	bl	sub_081969f8
	movs	r2, #0
	str	r2, [sp, #24]
	movs	r2, #7
	str	r2, [r0, #0]
	ldr	r1, [sp, #16]
	ldr	r3, [pc, #76]
	str	r1, [r0, #16]
	str	r3, [r0, #8]
	ldr	r3, [sp, #28]
	movs	r6, #68
	str	r3, [r0, #12]
	ldr	r4, [sp, #24]
	b.n	.L_08179808
	.4byte 0x000000ed
	.4byte 0x02010000
	.4byte 0x00000134
	.4byte 0x0000013e
	.4byte 0x0000017f
	.4byte 0x000000c2
	.4byte 0x02011000
	.4byte 0x08143001
	.4byte 0x02015018
	.4byte 0x00000148
	.4byte 0xfffffe20
	.4byte 0xfffffdec
	.4byte 0x08199400
	.4byte 0x02015000
	.4byte 0xfffff000
	.4byte 0x08197410
	.2byte 0x9364
	.2byte 0x0819
.L_08179808:
	add	r6, sp
	str	r4, [r0, #20]
	str	r6, [sp, #16]
	strb	r2, [r6, #0]
	strb	r2, [r1, #1]
	ldr	r3, [sp, #36]
	movs	r7, #0
	adds	r3, #8
	lsls	r3, r3, #16
	str	r3, [sp, #20]
	mov	sl, r0
	mov	r9, r6
	mov	r8, r7
.L_08179822:
	mov	r0, fp
	ldr	r3, [r0, #24]
	ldr	r1, [pc, #340]
	lsls	r3, r3, #2
	adds	r3, #2
	ldrb	r3, [r1, r3]
	lsls	r6, r3, #10
	lsls	r7, r3, #5
	bl	sub_08014de4
	movs	r2, #128
	lsls	r2, r2, #8
	movs	r1, #128
	adds	r0, r2, #0
	lsls	r1, r1, #9
	bl	sub_080151e4
	mov	r2, r8
	cmp	r2, #0
	bne.n	.L_08179860
	ldr	r3, [sp, #36]
	ldr	r2, [pc, #304]
	lsls	r1, r3, #16
	ldr	r4, [sp, #12]
	ldr	r3, [pc, #304]
	adds	r0, r4, r2
	adds	r1, r1, r3
	movs	r2, #0
	bl	sub_08015160
	b.n	.L_08179872
.L_08179860:
	ldr	r1, [pc, #284]
	ldr	r2, [sp, #20]
	ldr	r4, [sp, #12]
	ldr	r3, [pc, #284]
	adds	r0, r4, r1
	adds	r1, r2, r3
	movs	r2, #0
	bl	sub_08015160
.L_08179872:
	movs	r0, #250
	lsls	r0, r0, #3
	bl	sub_08015024
	ldr	r4, [sp, #56]
	cmp	r4, #47
	bgt.n	.L_0817988a
	movs	r0, #48
	subs	r0, r0, r4
	lsls	r0, r0, #9
	bl	sub_08015024
.L_0817988a:
	mov	r0, r8
	cmp	r0, #0
	bne.n	.L_081798c8
	ldr	r3, [sp, #56]
	subs	r3, #16
	adds	r5, r7, #0
	muls	r5, r3
	cmp	r5, r6
	ble.n	.L_0817989e
	adds	r5, r6, #0
.L_0817989e:
	ldr	r1, [sp, #56]
	cmp	r1, #60
	ble.n	.L_081798ac
	lsls	r3, r1, #1
	subs	r3, #120
	muls	r3, r7
	subs	r5, r6, r3
.L_081798ac:
	ldr	r2, [sp, #64]
	movs	r4, #224
	lsls	r4, r4, #3
	adds	r3, r2, r4
	mov	r6, r9
	str	r3, [r6, #4]
	ldr	r7, [sp, #56]
	negs	r3, r7
	lsls	r0, r3, #1
	adds	r0, r0, r3
	lsls	r0, r0, #8
	bl	sub_08015068
	b.n	.L_081798f0
.L_081798c8:
	ldr	r0, [sp, #56]
	lsls	r3, r0, #2
	subs	r3, #224
	adds	r5, r7, #0
	muls	r5, r3
	cmp	r5, #0
	blt.n	.L_08179914
	movs	r2, #128
	lsls	r3, r0, #3
	lsls	r2, r2, #2
	subs	r2, r2, r3
	mov	r3, sl
	str	r2, [sp, #24]
	str	r2, [r3, #20]
	ldr	r3, [pc, #160]
	mov	r4, r9
	movs	r1, #128
	str	r3, [r4, #4]
	lsls	r1, r1, #7
	adds	r5, r5, r1
.L_081798f0:
	cmp	r5, #0
	ble.n	.L_08179914
	ldr	r6, [sp, #24]
	movs	r7, #64
	negs	r7, r7
	cmp	r6, r7
	ble.n	.L_08179914
	adds	r0, r5, #0
	bl	sub_0801521c
	ldr	r0, [pc, #132]
	ldr	r1, [sp, #28]
	movs	r2, #4
	bl	sub_08196958
	mov	r0, sl
	bl	sub_08196a7c
.L_08179914:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	cmp	r1, #2
	bne.n	.L_08179822
	mov	r0, sl
	bl	sub_08013164
	ldr	r0, [sp, #28]
	bl	sub_08013164
	movs	r0, #8
	movs	r1, #8
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r4, #240
	ldr	r3, [sp, #64]
	lsls	r4, r4, #7
	adds	r4, #232
	adds	r2, r3, r4
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r6, [sp, #56]
	adds	r6, #1
	str	r6, [sp, #56]
	cmp	r6, #96
	beq.n	.L_08179956
	b.n	.L_081794b6
.L_08179956:
	ldr	r0, [pc, #56]
	bl	sub_08014644
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #172
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x08199400
	.4byte 0xff800000
	.4byte 0xffc00000
	.4byte 0x02011000
	.4byte 0x08199210
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
	ldr	r1, [r3, #96]
	sub	sp, #120
	mov	fp, r0
	ldr	r0, [r3, #92]
	str	r1, [sp, #60]
	mov	r8, r0
	ldr	r3, [r3, #100]
	movs	r0, #1
	str	r3, [sp, #52]
	bl	sub_081435e0
	ldr	r3, [pc, #52]
	movs	r2, #128
	lsls	r2, r2, #19
	movs	r1, #224
	adds	r2, #82
	lsls	r1, r1, #3
	strh	r3, [r2, #0]
	ldr	r0, [pc, #44]
	add	r1, r8
	movs	r2, #1
	movs	r3, #1
	bl	sub_08157cf4
	movs	r2, #0
	movs	r3, #0
	ldr	r0, [pc, #32]
	ldr	r1, [sp, #52]
	bl	sub_08157cf4
	mov	r2, fp
	ldr	r3, [r2, #24]
	cmp	r3, #2
	bne.n	sub_08179a10
	ldr	r0, [pc, #20]
	bl	sub_08013300
	adds	r1, r0, #0
	b.n	.L_08179a04
	.4byte 0x00001010
	.4byte 0x000000eb
	.4byte 0x00000134
	.2byte 0x0148
	.2byte 0x0000
.L_08179a04:
	movs	r0, #160
	ldr	r3, [pc, #856]
	lsls	r0, r0, #19
	movs	r2, #128
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x466b
	adds	r3, #64
	adds	r1, r3, #0
	movs	r0, #0
	str	r3, [sp, #48]
	bl	sub_08144aac
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, r8
	movs	r3, #2
	str	r3, [r2, #0]
	movs	r2, #238
	lsls	r2, r2, #7
	adds	r2, #132
	add	r2, r8
	movs	r3, #50
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #808]
	bl	sub_080145a8
	movs	r7, #0
	str	r7, [sp, #44]
	ldr	r3, [pc, #804]
	movs	r0, #0
	movs	r1, #1
	movs	r2, #192
	mov	r9, r0
	negs	r1, r1
	lsls	r2, r2, #2
.L_08179a50:
	movs	r7, #1
	add	r9, r7
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r9, r2
	bne.n	.L_08179a50
	mov	r1, sp
	movs	r0, #0
	adds	r1, #108
	str	r0, [sp, #56]
	str	r1, [sp, #36]
	str	r0, [sp, #16]
.L_08179a68:
	ldr	r2, [sp, #56]
	cmp	r2, #16
	bne.n	.L_08179a74
	movs	r0, #104
	bl	sub_081c0010
.L_08179a74:
	ldr	r3, [sp, #56]
	cmp	r3, #0
	bne.n	.L_08179a7e
	movs	r7, #0
	str	r7, [sp, #44]
.L_08179a7e:
	mov	r1, fp
	ldr	r0, [r1, #8]
	ldr	r1, [sp, #36]
	bl	sub_0815e21c
	ldr	r2, [sp, #56]
	cmp	r2, #7
	ble.n	.L_08179aa2
	mov	r3, fp
	ldr	r1, [r3, #24]
	ldr	r4, [pc, #728]
	lsls	r3, r1, #3
	subs	r3, r3, r1
	ldrb	r3, [r4, r3]
	ldr	r7, [sp, #44]
	adds	r7, r7, r3
	str	r7, [sp, #44]
	b.n	.L_08179aa8
.L_08179aa2:
	mov	r0, fp
	ldr	r1, [r0, #24]
	ldr	r4, [pc, #708]
.L_08179aa8:
	lsls	r3, r1, #3
	subs	r3, r3, r1
	adds	r3, #1
	ldrb	r2, [r4, r3]
	ldr	r1, [sp, #44]
	cmp	r1, r2
	ble.n	.L_08179ab8
	str	r2, [sp, #44]
.L_08179ab8:
	ldr	r2, [sp, #56]
	cmp	r2, #63
	bgt.n	.L_08179ad6
	ldr	r7, [sp, #36]
	mov	r0, r8
	ldr	r3, [r7, #0]
	lsls	r3, r3, #16
	str	r3, [r0, #0]
	ldr	r3, [r7, #4]
	ldr	r1, [sp, #44]
	subs	r3, r3, r1
	subs	r3, #32
	lsls	r3, r3, #16
	str	r3, [r0, #4]
	b.n	.L_08179b6a
.L_08179ad6:
	ldr	r2, [sp, #56]
	cmp	r2, #64
	bne.n	.L_08179b28
	add	r5, sp, #96
	adds	r0, r5, #0
	bl	sub_0815e22c
	ldr	r3, [sp, #36]
	mov	r7, r8
	ldr	r2, [r3, #0]
	lsls	r2, r2, #16
	str	r2, [r7, #0]
	ldr	r0, [sp, #36]
	ldr	r1, [sp, #44]
	ldr	r3, [r0, #4]
	subs	r3, r3, r1
	subs	r3, #32
	lsls	r0, r3, #16
	movs	r3, #0
	str	r0, [r7, #4]
	str	r3, [r7, #12]
	str	r3, [r7, #16]
	ldr	r3, [r5, #0]
	lsls	r3, r3, #16
	subs	r3, r3, r2
	cmp	r3, #0
	bge.n	.L_08179b0e
	adds	r3, #63
.L_08179b0e:
	asrs	r3, r3, #6
	mov	r2, r8
	str	r3, [r2, #8]
	ldr	r3, [r5, #4]
	subs	r3, #32
	lsls	r3, r3, #16
	subs	r0, r3, r0
	cmp	r0, #0
	bge.n	.L_08179b22
	adds	r0, #63
.L_08179b22:
	asrs	r3, r0, #6
	mov	r7, r8
	str	r3, [r7, #20]
.L_08179b28:
	mov	r0, r8
	ldr	r2, [r0, #12]
	ldr	r3, [r0, #0]
	ldr	r1, [r0, #16]
	adds	r3, r3, r2
	str	r3, [r0, #0]
	ldr	r3, [r0, #4]
	adds	r3, r3, r1
	str	r3, [r0, #4]
	ldr	r3, [r0, #8]
	adds	r2, r2, r3
	ldr	r3, [r0, #20]
	str	r2, [r0, #12]
	adds	r1, r1, r3
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #4
	str	r1, [r0, #16]
	cmp	r3, #0
	bge.n	.L_08179b52
	adds	r3, #63
.L_08179b52:
	asrs	r3, r3, #6
	mov	r2, r8
	str	r3, [r2, #12]
	lsls	r3, r1, #1
	adds	r3, r3, r1
	lsls	r1, r3, #4
	cmp	r1, #0
	bge.n	.L_08179b64
	adds	r1, #63
.L_08179b64:
	asrs	r3, r1, #6
	mov	r7, r8
	str	r3, [r7, #16]
.L_08179b6a:
	mov	r1, fp
	ldr	r3, [r1, #24]
	ldr	r7, [pc, #508]
	lsls	r2, r3, #3
	subs	r2, r2, r3
	adds	r2, #2
	ldrb	r3, [r7, r2]
	movs	r0, #0
	mov	r9, r0
	cmp	r3, #0
	beq.n	.L_08179c64
	ldr	r0, [sp, #56]
	ldr	r1, [sp, #56]
	ldr	r2, [sp, #44]
	asrs	r0, r0, #31
	lsls	r1, r1, #2
	str	r0, [sp, #28]
	str	r1, [sp, #32]
	lsls	r2, r2, #16
	mov	sl, r2
	add	r5, sp, #84
.L_08179b94:
	ldr	r0, [sp, #28]
	ldr	r1, [sp, #56]
	lsrs	r3, r0, #31
	adds	r3, r1, r3
	mov	r0, r9
	lsls	r2, r0, #2
	asrs	r3, r3, #1
	adds	r6, r3, r2
	movs	r3, #3
	ands	r6, r3
	bl	sub_08014de4
	mov	r1, r8
	ldr	r3, [pc, #448]
	ldr	r0, [r1, #0]
	ldr	r2, [pc, #448]
	ldr	r1, [r1, #4]
	adds	r0, r0, r2
	adds	r1, r1, r3
	movs	r2, #0
	bl	sub_08015160
	ldr	r0, [sp, #16]
	bl	sub_080150e4
	ldr	r1, [sp, #16]
	movs	r2, #250
	lsls	r2, r2, #1
	adds	r0, r1, r2
	bl	sub_08015024
	mov	r3, r9
	cmp	r3, #7
	ble.n	.L_08179be0
	movs	r0, #128
	lsls	r0, r0, #7
	bl	sub_08015024
.L_08179be0:
	mov	r0, fp
	ldr	r2, [r0, #24]
	ldr	r1, [sp, #32]
	lsls	r3, r2, #3
	subs	r3, r3, r2
	adds	r3, #3
	ldrb	r3, [r7, r3]
	mov	r0, r9
	muls	r0, r3
	adds	r0, r1, r0
	lsls	r0, r0, #8
	bl	sub_08015068
	movs	r3, #0
	mov	r2, sl
	str	r3, [r5, #0]
	str	r3, [r5, #4]
	str	r2, [r5, #8]
	ldr	r3, [pc, #368]
	adds	r1, r5, #0
	adds	r0, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x682a
	asrs	r2, r2, #17
	adds	r3, r2, #0
	adds	r3, #64
	str	r3, [r5, #0]
	movs	r0, #6
	ldrsh	r3, [r5, r0]
	adds	r2, #56
	adds	r1, r3, #0
	adds	r1, #64
	str	r1, [r5, #4]
	mov	r1, fp
	ldr	r0, [r1, #24]
	adds	r3, #16
	lsls	r1, r0, #3
	subs	r1, r1, r0
	adds	r1, #6
	ldrb	r4, [r7, r1]
	movs	r0, #224
	lsls	r1, r6, #10
	lsls	r0, r0, #3
	add	r1, r8
	adds	r1, r1, r0
	movs	r0, #16
	str	r0, [sp, #0]
	movs	r0, #64
	str	r0, [sp, #4]
	ldr	r0, [sp, #48]
	lsls	r4, r4, #2
	ldr	r4, [r4, r0]
	ldr	r0, [sp, #60]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x465b
	ldr	r2, [r3, #24]
	movs	r1, #1
	lsls	r3, r2, #3
	subs	r3, r3, r2
	adds	r3, #2
	ldrb	r3, [r7, r3]
	add	r9, r1
	cmp	r9, r3
	bne.n	.L_08179b94
.L_08179c64:
	ldr	r7, [sp, #56]
	cmp	r7, #88
	beq.n	.L_08179c6c
	b.n	.L_08179d98
.L_08179c6c:
	movs	r0, #134
	bl	sub_081180e8
	mov	r1, fp
	ldr	r3, [r1, #20]
	movs	r0, #0
	mov	r9, r0
	cmp	r3, #0
	bne.n	.L_08179c80
	b.n	.L_08179d80
.L_08179c80:
	mov	r2, sp
	adds	r2, #72
	movs	r3, #36
	str	r2, [sp, #24]
	str	r3, [sp, #20]
	str	r0, [sp, #12]
.L_08179c8c:
	ldr	r7, [sp, #20]
	mov	r2, fp
	ldrsh	r0, [r7, r2]
	ldr	r1, [sp, #24]
	bl	sub_0815e1fc
	mov	r1, fp
	ldrsh	r0, [r7, r1]
	movs	r1, #4
	bl	sub_08118088
	mov	r3, fp
	ldrsh	r0, [r7, r3]
	movs	r3, #16
	str	r3, [sp, #0]
	movs	r1, #7
	mov	r3, r9
	movs	r2, #5
	bl	sub_0814cd48
	movs	r7, #0
	str	r7, [sp, #40]
	mov	r0, fp
	ldr	r1, [r0, #24]
	ldr	r4, [pc, #172]
	lsls	r3, r1, #3
	subs	r3, r3, r1
	adds	r3, #4
	ldrb	r3, [r4, r3]
	cmp	r3, #0
	beq.n	.L_08179d40
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #12]
	ldr	r3, [pc, #172]
	mov	sl, r1
	adds	r7, r2, r3
.L_08179cd4:
	mov	r0, sl
	ldr	r3, [r0, #0]
	str	r4, [sp, #8]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	movs	r5, #255
	ldr	r3, [r0, #4]
	lsls	r3, r3, #16
	str	r3, [r7, #4]
	bl	sub_08014878
	adds	r6, r0, #0
	bl	sub_08014878
	ands	r5, r0
	adds	r0, r6, #0
	bl	sub_08002096
	adds	r5, #127
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #12]
	adds	r0, r6, #0
	bl	sub_08002090
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #16]
	bl	sub_08014878
	movs	r3, #15
	ands	r3, r0
	adds	r3, #24
	str	r3, [r7, #24]
	ldr	r1, [sp, #40]
	mov	r2, fp
	adds	r1, #1
	str	r1, [sp, #40]
	ldr	r0, [pc, #64]
	ldr	r1, [r2, #24]
	ldr	r2, [sp, #40]
	lsls	r3, r1, #3
	subs	r3, r3, r1
	adds	r3, #4
	ldrb	r3, [r0, r3]
	adds	r7, #28
	ldr	r4, [sp, #8]
	cmp	r2, r3
	bne.n	.L_08179cd4
.L_08179d40:
	ldr	r3, [sp, #20]
	ldr	r7, [sp, #12]
	movs	r0, #224
	lsls	r0, r0, #4
	adds	r3, #2
	adds	r7, r7, r0
	str	r7, [sp, #12]
	str	r3, [sp, #20]
	mov	r7, fp
	ldr	r3, [r7, #20]
	movs	r2, #1
	add	r9, r2
	cmp	r9, r3
	bne.n	.L_08179c8c
	b.n	.L_08179d86
	movs	r0, r0
	.4byte 0x03000730
	.4byte 0x08143001
	.4byte 0x02010018
	.4byte 0x0819940c
	.4byte 0xffc00000
	.4byte 0xff800000
	.4byte 0x03000354
	.2byte 0x0000
	.2byte 0x0201
.L_08179d80:
	mov	r0, fp
	ldr	r1, [r0, #24]
	ldr	r4, [pc, #208]
.L_08179d86:
	lsls	r2, r1, #3
	subs	r2, r2, r1
	movs	r3, #238
	adds	r2, #5
	lsls	r3, r3, #7
	ldrb	r2, [r4, r2]
	adds	r3, #168
	add	r3, r8
	str	r2, [r3, #0]
.L_08179d98:
	ldr	r2, [pc, #192]
	ldr	r6, [pc, #196]
	movs	r1, #0
	mov	r9, r1
	mov	sl, r2
.L_08179da2:
	ldr	r0, [r6, #24]
	cmp	r0, #0
	blt.n	.L_08179df0
	asrs	r0, r0, #3
	adds	r0, #1
	lsls	r5, r0, #1
	mov	r3, r9
	movs	r4, #1
	ands	r4, r3
	mov	r7, sl
	subs	r3, r5, #2
	ldrh	r1, [r7, r3]
	ldr	r2, [sp, #52]
	lsls	r4, r4, #2
	adds	r1, r2, r1
	movs	r3, #2
	ldrsh	r2, [r6, r3]
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	movs	r7, #6
	ldrsh	r3, [r6, r7]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r5, [sp, #4]
	ldr	r0, [sp, #48]
	ldr	r4, [r4, r0]
	ldr	r0, [sp, #60]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x1c30
	movs	r1, #60
	ldr	r2, [pc, #124]
	bl	sub_08138086
	ldr	r3, [r6, #24]
	subs	r3, #1
	str	r3, [r6, #24]
.L_08179df0:
	movs	r1, #1
	movs	r2, #192
	add	r9, r1
	lsls	r2, r2, #2
	adds	r6, #28
	cmp	r9, r2
	bne.n	.L_08179da2
	movs	r0, #8
	movs	r1, #8
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	movs	r3, #1
	add	r2, r8
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [sp, #16]
	ldr	r0, [sp, #56]
	movs	r7, #128
	lsls	r7, r7, #1
	adds	r3, r3, r7
	adds	r0, #1
	str	r3, [sp, #16]
	str	r0, [sp, #56]
	cmp	r0, #122
	beq.n	.L_08179e32
	b.n	.L_08179a68
.L_08179e32:
	movs	r0, #188
	bl	sub_0801314c
	movs	r0, #104
	bl	sub_0801314c
	ldr	r0, [pc, #40]
	bl	sub_08014644
	bl	sub_08143bb8
	add	sp, #120
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0819940c
	.4byte 0x08197410
	.4byte 0x02010000
	.4byte 0xffffc000
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #96]
	movs	r7, #7
	mov	r8, r3
	bl	sub_0815b410
	ldr	r3, [pc, #44]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #12
	strh	r3, [r2, #0]
	ldr	r3, [pc, #40]
	movs	r2, #160
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	ldr	r3, [pc, #36]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r3, [pc, #28]
	adds	r2, #50
	strh	r3, [r2, #0]
	ldr	r3, [pc, #28]
	subs	r2, #2
	strh	r3, [r2, #0]
	movs	r3, #255
	movs	r1, #0
	mov	ip, r3
	b.n	.L_08179ec4
	.4byte 0x00000784
	.4byte 0x00000000
	.4byte 0x0000004e
	.4byte 0x00001010
	.2byte 0x3f44
	.2byte 0x0000
.L_08179ec4:
	adds	r5, r1, #0
	movs	r4, #0
	lsls	r6, r1, #1
	ands	r5, r7
.L_08179ecc:
	lsls	r0, r1, #1
	cmp	r1, #31
	ble.n	.L_08179edc
	mov	r3, ip
	subs	r0, r3, r6
	cmp	r1, #95
	bgt.n	.L_08179edc
	movs	r0, #63
.L_08179edc:
	adds	r2, r4, #0
	cmp	r4, #0
	bge.n	.L_08179ee4
	adds	r2, r4, #7
.L_08179ee4:
	asrs	r2, r2, #3
	adds	r3, r1, #0
	cmp	r1, #0
	bge.n	.L_08179eee
	adds	r3, r1, #7
.L_08179eee:
	asrs	r3, r3, #3
	lsls	r2, r2, #4
	adds	r2, r2, r3
	adds	r3, r4, #0
	ands	r3, r7
	lsls	r2, r2, #3
	adds	r2, r2, r3
	lsls	r2, r2, #3
	adds	r2, r2, r5
	mov	r3, r8
	adds	r4, #1
	strb	r0, [r3, r2]
	cmp	r4, #128
	bne.n	.L_08179ecc
	adds	r1, #1
	cmp	r1, #128
	bne.n	.L_08179ec4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
