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
	.set sub_08015024, 0x08015024
	.set sub_08015068, 0x08015068
	.set sub_080150e4, 0x080150e4
	.set sub_08015160, 0x08015160
	.set sub_080151e4, 0x080151e4
	.set sub_0801521c, 0x0801521c
	.set sub_0801587c, 0x0801587c
	.set sub_08020010, 0x08020010
	.set sub_08020048, 0x08020048
	.set sub_08118040, 0x08118040
	.set sub_08118088, 0x08118088
	.set sub_081180e8, 0x081180e8
	.set sub_08138086, 0x08138086
	.set sub_0813ba50, 0x0813ba50
	.set sub_081434f8, 0x081434f8
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_08143eb4, 0x08143eb4
	.set sub_0814cc4c, 0x0814cc4c
	.set sub_0814cca8, 0x0814cca8
	.set sub_0814cd48, 0x0814cd48
	.set sub_08157cf4, 0x08157cf4
	.set sub_08158ce0, 0x08158ce0
	.set sub_0815b290, 0x0815b290
	.set sub_0815b3b0, 0x0815b3b0
	.set sub_0815b410, 0x0815b410
	.set sub_0815b434, 0x0815b434
	.set sub_0815e1ec, 0x0815e1ec
	.set sub_0815f0a0, 0x0815f0a0
	.set sub_08179e6c, 0x08179e6c
	.set sub_0817a516, 0x0817a516
	.set sub_0817a536, 0x0817a536
	.set sub_0818caa8, 0x0818caa8
	.set sub_081963ec, 0x081963ec
	.set sub_08196958, 0x08196958
	.set sub_081969f8, 0x081969f8
	.set sub_08196a7c, 0x08196a7c
	.set sub_081c0010, 0x081c0010
	.global Func_08179f18
	.thumb_func
Func_08179f18:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #292
	str	r0, [sp, #180]
	movs	r2, #192
	lsls	r2, r2, #18
	ldr	r0, [r2, #96]
	ldr	r3, [pc, #436]
	str	r0, [sp, #176]
	movs	r0, #0
	ldr	r1, [r2, #92]
	movs	r5, #239
	str	r1, [sp, #172]
	lsls	r5, r5, #7
	ldrh	r3, [r3, #4]
	movs	r6, #80
	str	r3, [sp, #156]
	adds	r3, r2, #0
	adds	r3, #176
	ldr	r3, [r3, #0]
	negs	r6, r6
	str	r3, [sp, #152]
	ldr	r3, [pc, #412]
	ldr	r2, [r2, #100]
	mov	r9, r3
	str	r2, [sp, #148]
	bl	sub_081435e0
	bl	sub_0813ba50
	bl	sub_08179e6c
	ldr	r4, [sp, #172]
	movs	r3, #0
	adds	r2, r4, r5
	movs	r1, #200
	str	r3, [r2, #0]
	ldr	r0, [pc, #384]
	lsls	r1, r1, #4
	bl	sub_080145a8
	movs	r0, #0
	str	r6, [sp, #144]
	str	r0, [sp, #160]
.L_08179f7a:
	ldr	r1, [sp, #160]
	cmp	r1, #27
	beq.n	.L_08179f82
	b.n	.L_0817a1a4
.L_08179f82:
	ldr	r2, [pc, #368]
	movs	r3, #240
	str	r3, [r2, #16]
	ldr	r2, [sp, #172]
	movs	r4, #240
	lsls	r4, r4, #7
	adds	r4, #240
	adds	r3, r2, r4
	ldr	r0, [r3, #0]
	bl	sub_0814cc4c
	movs	r0, #1
	ldr	r1, [pc, #348]
	movs	r2, #0
	bl	sub_08118040
	movs	r6, #238
	ldr	r5, [sp, #172]
	lsls	r6, r6, #7
	movs	r0, #238
	adds	r6, #144
	lsls	r0, r0, #7
	adds	r3, r5, r6
	movs	r1, #0
	adds	r0, #148
	str	r1, [r3, #0]
	movs	r2, #1
	adds	r3, r5, r0
	str	r2, [r3, #0]
	movs	r3, #238
	lsls	r3, r3, #7
	adds	r3, #152
	movs	r4, #238
	adds	r2, r5, r3
	lsls	r4, r4, #7
	movs	r3, #3
	adds	r4, #156
	negs	r3, r3
	str	r3, [r2, #0]
	adds	r3, r5, r4
	str	r1, [r3, #0]
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #288]
	bl	sub_080145a8
	ldr	r6, [sp, #152]
	movs	r5, #1
	str	r5, [r6, #16]
	ldr	r0, [pc, #280]
	bl	sub_08013300
	adds	r6, r0, #0
	ldr	r5, [pc, #276]
	adds	r1, r6, #0
	movs	r2, #32
	adds	r6, #32
	ldr	r0, [pc, #272]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x1c30
	ldr	r1, [pc, #268]
	bl	sub_0801587c
	ldr	r1, [pc, #268]
	mov	r8, r5
	movs	r7, #240
	ldr	r5, [pc, #256]
	movs	r0, #0
	lsls	r7, r7, #7
	mov	sl, r0
	mov	fp, r1
	mov	r6, r9
	adds	r7, #12
.L_0817a016:
	movs	r2, #128
	movs	r3, #240
	movs	r1, #16
	lsls	r2, r2, #23
	lsls	r3, r3, #8
	movs	r0, #16
	bl	sub_0815b290
	ldr	r2, [sp, #172]
	ldrb	r3, [r0, #9]
	movs	r4, #13
	negs	r4, r4
	str	r0, [r7, r2]
	adds	r2, r4, #0
	ands	r3, r2
	movs	r2, #8
	orrs	r3, r2
	strb	r3, [r0, #9]
	ldrb	r3, [r0, #16]
	ldr	r1, [pc, #212]
	lsls	r3, r3, #2
	add	r3, fp
	ldrh	r0, [r3, #2]
	movs	r2, #128
	adds	r0, r0, r1
	adds	r1, r5, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x9a2b
	adds	r5, #128
	ldr	r3, [r7, r2]
	adds	r7, #4
	ldrh	r3, [r3, #8]
	lsls	r3, r3, #22
	lsrs	r3, r3, #22
	strh	r3, [r6, #0]
	movs	r3, #1
	add	sl, r3
	mov	r4, sl
	adds	r6, #2
	cmp	r4, #15
	bne.n	.L_0817a016
	movs	r0, #240
	lsls	r0, r0, #7
	ldr	r7, [pc, #148]
	movs	r5, #0
	adds	r0, #72
	mov	sl, r5
	adds	r6, r2, r0
.L_0817a078:
	movs	r2, #128
	movs	r3, #240
	lsls	r3, r3, #8
	movs	r1, #16
	lsls	r2, r2, #23
	movs	r0, #16
	bl	sub_0815b3b0
	stmia	r6!, {r0}
	ldr	r1, [sp, #172]
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #12
	adds	r5, r1, r2
	ldr	r1, [r5, #0]
	movs	r2, #24
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x2301
	add	sl, r3
	mov	r4, sl
	cmp	r4, #17
	bne.n	.L_0817a078
	movs	r6, #0
	mov	sl, r6
	ldr	r7, [pc, #52]
	ldr	r6, [pc, #104]
.L_0817a0ae:
	bl	sub_08014878
	movs	r1, #15
	bl	sub_0800206c
	lsls	r0, r0, #8
	strh	r0, [r6, #0]
	ldmia	r5!, {r0}
	ldrh	r3, [r6, #0]
	mov	r2, r9
	lsrs	r3, r3, #8
	lsls	r3, r3, #1
	ldrh	r1, [r2, r3]
	ldrh	r3, [r0, #8]
	ldr	r2, [pc, #24]
	ands	r1, r7
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r0, #8]
	movs	r3, #1
	add	sl, r3
	mov	r4, sl
	adds	r6, #2
	b.n	.L_0817a11c
	movs	r0, r0
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.4byte 0x03001120
	.4byte 0x02014000
	.4byte 0x08143001
	.4byte 0x030011e0
	.4byte 0x00000045
	.4byte 0x0813baed
	.4byte 0x0000009a
	.4byte 0x03000730
	.4byte 0x050003e0
	.4byte 0x02010000
	.4byte 0x020036e0
	.4byte 0x06010000
	.2byte 0x401e
	.2byte 0x0201
.L_0817a11c:
	cmp	r4, #32
	bne.n	.L_0817a0ae
	ldr	r0, [pc, #368]
	bl	sub_08013300
	movs	r5, #240
	adds	r6, r0, #0
	lsls	r5, r5, #1
	adds	r6, r6, r5
	adds	r0, r6, #0
	ldr	r1, [pc, #356]
	bl	sub_0801587c
	movs	r6, #0
	ldr	r0, [pc, #352]
	ldr	r1, [pc, #356]
	ldr	r2, [sp, #172]
	movs	r3, #238
	mov	sl, r6
	lsls	r3, r3, #7
	movs	r4, #13
	ldr	r6, [pc, #336]
	adds	r3, #220
	negs	r4, r4
	mov	fp, r0
	mov	r8, r1
	adds	r5, r2, r3
	adds	r7, r4, #0
.L_0817a154:
	movs	r1, #32
	ldr	r2, [pc, #332]
	movs	r3, #0
	movs	r0, #32
	bl	sub_0815b290
	ldrb	r3, [r0, #9]
	movs	r2, #8
	ands	r3, r7
	orrs	r3, r2
	strb	r3, [r0, #9]
	ldrb	r3, [r0, #16]
	stmia	r5!, {r0}
	lsls	r3, r3, #2
	add	r3, fp
	ldrh	r0, [r3, #2]
	ldr	r1, [pc, #304]
	movs	r2, #128
	adds	r0, r0, r1
	lsls	r2, r2, #3
	adds	r1, r6, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x2301
	movs	r2, #128
	add	sl, r3
	lsls	r2, r2, #3
	mov	r4, sl
	adds	r6, r6, r2
	cmp	r4, #12
	bne.n	.L_0817a154
	ldr	r5, [sp, #172]
	movs	r6, #224
	lsls	r6, r6, #3
	ldr	r0, [pc, #272]
	adds	r1, r5, r6
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
.L_0817a1a4:
	ldr	r0, [sp, #144]
	movs	r3, #128
	adds	r0, #4
	lsls	r3, r3, #19
	lsls	r2, r0, #8
	adds	r3, #40
	str	r0, [sp, #144]
	str	r2, [r3, #0]
	ldr	r1, [sp, #172]
	movs	r2, #240
	lsls	r2, r2, #7
	adds	r2, #232
	adds	r3, r1, r2
	movs	r4, #1
	str	r4, [r3, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r5, [sp, #160]
	adds	r5, #1
	str	r5, [sp, #160]
	cmp	r5, #52
	beq.n	.L_0817a1d4
	b.n	.L_08179f7a
.L_0817a1d4:
	ldr	r5, [pc, #216]
	ldr	r1, [pc, #192]
	adds	r0, r5, #0
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	adds	r0, r5, #0
	ldr	r1, [sp, #148]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r0, [pc, #196]
	movs	r6, #0
	movs	r1, #63
	mov	sl, r6
	mov	ip, r0
	movs	r7, #4
	movs	r5, #0
	movs	r4, #0
	mov	lr, r1
.L_0817a200:
	mov	r2, sl
	ldr	r6, [sp, #148]
	adds	r3, r4, r2
	lsls	r3, r3, #1
	movs	r2, #128
	adds	r3, r3, r6
	lsls	r2, r2, #2
	mov	r8, r3
	adds	r2, #78
	movs	r1, #0
	adds	r0, r5, #0
	add	r2, r8
.L_0817a218:
	mov	r6, ip
	ldrh	r3, [r6, r7]
	ldr	r6, [pc, #120]
	adds	r3, r3, r1
	adds	r3, r3, r6
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	ble.n	.L_0817a232
	mov	r6, lr
	subs	r3, r6, r0
	cmp	r3, #0
	bgt.n	.L_0817a232
	movs	r3, #1
.L_0817a232:
	adds	r1, #1
	strb	r3, [r2, #0]
	adds	r2, #1
	cmp	r1, #18
	bne.n	.L_0817a218
	movs	r0, #1
	add	sl, r0
	mov	r1, sl
	adds	r5, #7
	adds	r4, #8
	cmp	r1, #10
	bne.n	.L_0817a200
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	adds	r2, #32
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	adds	r2, #50
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	subs	r2, #2
	strh	r3, [r2, #0]
	subs	r2, #40
	movs	r3, #0
	str	r3, [r2, #0]
	movs	r5, #128
	movs	r2, #128
	movs	r3, #44
	lsls	r5, r5, #2
	str	r2, [sp, #140]
	str	r3, [sp, #136]
	movs	r4, #0
	str	r5, [sp, #128]
	mov	r0, r9
	movs	r5, #128
	mov	r1, r9
	b.n	.L_0817a2b8
	movs	r0, r0
	.4byte 0x00007741
	.4byte 0x00000100
	.4byte 0x0000100e
	.4byte 0x00003f44
	.4byte 0x00000098
	.4byte 0x02010000
	.4byte 0x020036e0
	.4byte 0x03000730
	.4byte 0x80002000
	.4byte 0x06010000
	.4byte 0x000000c2
	.4byte 0x00000134
	.2byte 0x7410
	.2byte 0x0819
.L_0817a2b8:
	str	r4, [sp, #132]
	str	r4, [sp, #124]
	str	r4, [sp, #120]
	mov	sl, r4
	lsls	r5, r5, #1
	movs	r4, #22
	adds	r0, #224
	adds	r1, #96
.L_0817a2c8:
	mov	r3, sl
	cmp	r3, #0
	bge.n	.L_0817a2d0
	adds	r3, #7
.L_0817a2d0:
	asrs	r3, r3, #3
	lsls	r2, r3, #3
	mov	r6, sl
	subs	r2, r6, r2
	lsls	r2, r2, #5
	muls	r3, r4
	adds	r2, r2, r5
	stmia	r1!, {r2}
	movs	r2, #1
	adds	r3, #128
	add	sl, r2
	stmia	r0!, {r3}
	mov	r3, sl
	cmp	r3, #32
	bne.n	.L_0817a2c8
	ldr	r4, [sp, #172]
	movs	r5, #239
	movs	r6, #238
	lsls	r5, r5, #7
	lsls	r6, r6, #7
	adds	r2, r4, r5
	movs	r3, #2
	adds	r6, #132
	str	r3, [r2, #0]
	adds	r2, r4, r6
	movs	r3, #50
	str	r3, [r2, #0]
	movs	r1, #19
	movs	r0, #104
	bl	sub_081963ec
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #104]
	movs	r0, #0
	str	r3, [sp, #164]
	str	r0, [sp, #160]
	ldr	r3, [pc, #116]
	movs	r1, #138
	ldr	r3, [r3, #12]
	lsls	r1, r1, #1
	mov	r2, sp
	mov	r3, sp
	add	r1, sp
	adds	r2, #192
	adds	r3, #184
	str	r1, [sp, #48]
	str	r2, [sp, #52]
	str	r3, [sp, #56]
.L_0817a332:
	ldr	r4, [sp, #160]
	cmp	r4, #0
	bne.n	.L_0817a3b2
	movs	r1, #128
	ldr	r5, [pc, #88]
	lsls	r1, r1, #7
	ldr	r0, [pc, #88]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2180
	ldr	r0, [sp, #176]
	lsls	r1, r1, #7
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x2398
	ldr	r2, [pc, #52]
	movs	r5, #0
	lsls	r3, r3, #2
	mov	sl, r5
	add	r3, r9
.L_0817a35a:
	movs	r6, #1
	add	sl, r6
	mov	r0, sl
	strh	r2, [r3, #0]
	adds	r3, #2
	cmp	r0, #240
	bne.n	.L_0817a35a
	movs	r2, #240
	ldr	r3, [pc, #48]
	ldr	r1, [pc, #48]
	lsls	r2, r2, #1
	ldr	r0, [pc, #48]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4b05
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	bl	sub_0815b410
	b.n	.L_0817a3a8
	movs	r0, r0
	.4byte 0x00007fff
	.4byte 0x00000100
	.4byte 0x03001150
	.4byte 0x03000258
	.4byte 0x06004000
	.4byte 0x03000730
	.4byte 0x02014260
	.2byte 0x0200
	.2byte 0x0500
.L_0817a3a8:
	movs	r2, #128
	ldr	r3, [pc, #40]
	lsls	r2, r2, #19
	adds	r2, #40
	str	r3, [r2, #0]
.L_0817a3b2:
	ldr	r1, [sp, #160]
	movs	r2, #30
	adds	r2, #255
	cmp	r1, r2
	ble.n	.L_0817a3c4
	ldr	r0, [pc, #24]
	bl	sub_0815f0a0
	b.n	.L_0817a3ec
.L_0817a3c4:
	ldr	r3, [sp, #160]
	cmp	r3, #199
	ble.n	.L_0817a3e0
	ldr	r0, [pc, #16]
	bl	sub_0815f0a0
	b.n	.L_0817a3ec
	movs	r0, r0
	.4byte 0xffffc400
	.4byte 0x00000166
	.2byte 0x0167
	.2byte 0x0000
.L_0817a3e0:
	ldr	r4, [sp, #160]
	cmp	r4, #180
	ble.n	.L_0817a3ec
	ldr	r0, [pc, #76]
	bl	sub_0815f0a0
.L_0817a3ec:
	ldr	r5, [sp, #160]
	cmp	r5, #108
	bne.n	.L_0817a44c
	ldr	r3, [pc, #60]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	movs	r3, #0
	adds	r2, #8
	str	r3, [r2, #0]
	movs	r5, #192
	movs	r2, #128
	lsls	r5, r5, #8
	ldr	r0, [pc, #44]
	movs	r1, #8
	lsls	r2, r2, #9
	bl	sub_0815b434
	ldr	r0, [pc, #40]
	movs	r1, #16
	adds	r2, r5, #0
	bl	sub_0815b434
	ldr	r0, [pc, #32]
	movs	r1, #32
	adds	r2, r5, #0
	bl	sub_0815b434
	ldr	r0, [pc, #28]
	movs	r1, #48
	adds	r2, r5, #0
	b.n	.L_0817a448
	movs	r0, r0
	.4byte 0x00000080
	.4byte 0x00000150
	.4byte 0x02010000
	.4byte 0x02010020
	.4byte 0x020100a0
	.2byte 0x02a0
	.2byte 0x0201
.L_0817a448:
	bl	sub_0815b434
.L_0817a44c:
	ldr	r3, [sp, #160]
	subs	r3, #223
	cmp	r3, #29
	bhi.n	.L_0817a474
	bl	sub_08014878
	movs	r3, #3
	ands	r0, r3
	ldr	r3, [pc, #64]
	ldr	r2, [pc, #68]
	ldrb	r3, [r3, r0]
	lsls	r0, r0, #1
	ldrh	r0, [r2, r0]
	ldr	r6, [pc, #64]
	lsls	r3, r3, #1
	adds	r0, r0, r6
	movs	r1, #44
	movs	r2, #78
	bl	sub_0818caa8
.L_0817a474:
	ldr	r0, [sp, #160]
	subs	r0, #112
	mov	r8, r0
	cmp	r0, #31
	bhi.n	sub_0817a516
	ldr	r0, [pc, #44]
	bl	sub_08013300
	ldr	r2, [pc, #20]
	movs	r4, #152
	movs	r1, #0
	movs	r3, #31
	lsls	r4, r4, #2
	adds	r7, r0, #0
	mov	sl, r1
	mov	lr, r2
	mov	fp, r3
	mov	ip, r4
	b.n	.L_0817a4b0
	movs	r0, r0
	.4byte 0x0000001f
	.4byte 0x08199421
	.4byte 0x08199426
	.4byte 0x02010000
	.2byte 0x0098
	.2byte 0x0000
.L_0817a4b0:
	mov	r5, ip
	mov	r6, r9
	ldrh	r2, [r5, r6]
	mov	r1, lr
	lsls	r3, r2, #16
	lsrs	r0, r3, #26
	lsrs	r5, r3, #21
	ands	r0, r1
	ands	r5, r1
	ldrh	r1, [r7, #0]
	mov	r6, fp
	lsls	r3, r1, #16
	mov	r4, lr
	ands	r6, r2
	lsrs	r2, r3, #26
	lsrs	r3, r3, #21
	ands	r2, r4
	ands	r3, r4
	mov	r4, fp
	ands	r4, r1
	cmp	r0, r2
	ble.n	.L_0817a4de
	subs	r0, #1
.L_0817a4de:
	cmp	r5, r3
	ble.n	.L_0817a4e4
	subs	r5, #1
.L_0817a4e4:
	cmp	r6, r4
	ble.n	.L_0817a4ea
	subs	r6, #1
.L_0817a4ea:
	lsls	r2, r5, #5
	lsls	r3, r0, #10
	movs	r1, #1
	orrs	r3, r2
	add	sl, r1
	orrs	r3, r6
	mov	r5, ip
	mov	r6, r9
	movs	r0, #2
	mov	r2, sl
	strh	r3, [r5, r6]
	add	ip, r0
	adds	r7, #2
	cmp	r2, #240
	bne.n	.L_0817a4b0
	movs	r2, #240
	ldr	r3, [pc, #824]
	ldr	r0, [pc, #824]
	ldr	r1, [pc, #828]
	lsls	r2, r2, #1
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4643
	cmp	r3, #7
	bhi.n	sub_0817a536
	lsls	r3, r3, #8
	add	r3, r8
	lsls	r2, r3, #16
	adds	r3, r3, r2
	ldr	r2, [pc, #808]
	lsls	r3, r3, #3
	movs	r1, #128
	subs	r2, r2, r3
	ldr	r0, [sp, #176]
	ldr	r3, [pc, #804]
	lsls	r1, r1, #7
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4bc8
	ldr	r5, [sp, #48]
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	movs	r7, #0
	str	r3, [sp, #192]
	str	r4, [sp, #196]
	movs	r3, #255
	movs	r4, #0
	lsls	r3, r3, #16
	str	r4, [r5, #12]
	str	r3, [r5, #4]
	ldr	r6, [sp, #160]
	mov	r8, r4
	cmp	r6, #119
	bgt.n	.L_0817a582
	movs	r5, #205
	lsls	r5, r5, #8
	adds	r0, r5, #0
	bl	sub_08002096
	lsls	r3, r0, #1
	adds	r3, r3, r0
	movs	r0, #152
	lsls	r3, r3, #3
	lsls	r0, r0, #16
	adds	r0, r3, r0
	str	r0, [sp, #140]
	adds	r0, r5, #0
	bl	sub_08002090
	lsls	r3, r0, #2
	adds	r3, r3, r0
	movs	r1, #176
	lsls	r3, r3, #2
	lsls	r1, r1, #14
	adds	r1, r3, r1
	b.n	.L_0817a674
.L_0817a582:
	ldr	r2, [sp, #160]
	cmp	r2, #179
	bgt.n	.L_0817a5bc
	lsls	r5, r2, #1
	ldr	r3, [pc, #720]
	adds	r5, r5, r2
	lsls	r5, r5, #8
	adds	r5, r5, r3
	adds	r0, r5, #0
	bl	sub_08002096
	lsls	r3, r0, #1
	adds	r3, r3, r0
	movs	r4, #152
	lsls	r3, r3, #3
	lsls	r4, r4, #16
	adds	r4, r3, r4
	adds	r0, r5, #0
	str	r4, [sp, #140]
	bl	sub_08002090
	lsls	r3, r0, #2
	adds	r3, r3, r0
	movs	r5, #176
	lsls	r3, r3, #2
	lsls	r5, r5, #14
	adds	r5, r3, r5
	str	r5, [sp, #136]
	b.n	.L_0817a676
.L_0817a5bc:
	ldr	r6, [sp, #160]
	cmp	r6, #221
	bgt.n	.L_0817a5d6
	ldr	r0, [sp, #140]
	ldr	r2, [sp, #136]
	ldr	r1, [pc, #664]
	movs	r3, #128
	lsls	r3, r3, #8
	adds	r0, r0, r1
	adds	r2, r2, r3
	str	r0, [sp, #140]
	str	r2, [sp, #136]
	b.n	.L_0817a676
.L_0817a5d6:
	ldr	r4, [sp, #160]
	cmp	r4, #251
	bgt.n	.L_0817a65e
	adds	r0, r4, #0
	movs	r1, #6
	bl	sub_08002064
	cmp	r0, #1
	bne.n	.L_0817a5ea
	movs	r7, #1
.L_0817a5ea:
	cmp	r0, #2
	bne.n	.L_0817a5f0
	movs	r7, #1
.L_0817a5f0:
	cmp	r0, #3
	bne.n	.L_0817a5fc
	movs	r5, #1
	negs	r5, r5
	movs	r7, #2
	mov	r8, r5
.L_0817a5fc:
	cmp	r0, #4
	bne.n	.L_0817a608
	movs	r6, #1
	negs	r6, r6
	movs	r7, #2
	mov	r8, r6
.L_0817a608:
	cmp	r0, #5
	bne.n	.L_0817a614
	movs	r1, #1
	negs	r1, r1
	movs	r7, #2
	mov	r8, r1
.L_0817a614:
	cmp	r0, #0
	bne.n	.L_0817a61e
	movs	r2, #0
	movs	r7, #0
	mov	r8, r2
.L_0817a61e:
	ldr	r4, [sp, #160]
	movs	r3, #3
	ands	r3, r4
	cmp	r3, #0
	bne.n	.L_0817a62e
	ldr	r2, [pc, #568]
	movs	r3, #30
	strh	r3, [r2, #6]
.L_0817a62e:
	ldr	r3, [sp, #160]
	cmp	r3, #0
	bge.n	.L_0817a636
	adds	r3, #3
.L_0817a636:
	ldr	r5, [sp, #160]
	asrs	r3, r3, #2
	lsls	r3, r3, #2
	subs	r1, r5, r3
	cmp	r1, #1
	bne.n	.L_0817a648
	ldr	r2, [pc, #544]
	movs	r3, #30
	strh	r3, [r2, #6]
.L_0817a648:
	cmp	r1, #2
	bne.n	.L_0817a652
	ldr	r2, [pc, #532]
	movs	r3, #34
	strh	r3, [r2, #6]
.L_0817a652:
	cmp	r1, #3
	bne.n	.L_0817a676
	ldr	r2, [pc, #524]
	movs	r3, #34
	strh	r3, [r2, #6]
	b.n	.L_0817a676
.L_0817a65e:
	ldr	r2, [pc, #516]
	movs	r3, #32
	strh	r3, [r2, #6]
	ldr	r6, [sp, #140]
	ldr	r1, [sp, #136]
	ldr	r2, [pc, #508]
	movs	r0, #128
	lsls	r0, r0, #12
	adds	r6, r6, r0
	adds	r1, r1, r2
	str	r6, [sp, #140]
.L_0817a674:
	str	r1, [sp, #136]
.L_0817a676:
	ldr	r3, [sp, #160]
	cmp	r3, #111
	ble.n	.L_0817a6ca
	ldr	r0, [sp, #172]
	movs	r1, #238
	lsls	r1, r1, #7
	ldr	r5, [sp, #48]
	movs	r4, #0
	adds	r1, #220
	mov	sl, r4
	adds	r6, r0, r1
.L_0817a68c:
	mov	r0, sl
	movs	r1, #3
	bl	sub_08002064
	ldr	r2, [sp, #140]
	lsls	r0, r0, #5
	adds	r0, r0, r7
	lsls	r0, r0, #16
	adds	r0, r2, r0
	str	r0, [r5, #0]
	movs	r1, #3
	mov	r0, sl
	bl	sub_08002054
	ldr	r3, [sp, #136]
	lsls	r0, r0, #5
	add	r0, r8
	lsls	r0, r0, #16
	adds	r0, r3, r0
	str	r0, [r5, #8]
	adds	r1, r5, #0
	ldmia	r6!, {r0}
	ldr	r2, [sp, #52]
	movs	r3, #0
	bl	sub_08020010
	movs	r4, #1
	add	sl, r4
	mov	r0, sl
	cmp	r0, #12
	bne.n	.L_0817a68c
.L_0817a6ca:
	movs	r1, #0
	movs	r2, #128
	movs	r4, #240
	movs	r6, #176
	lsls	r2, r2, #17
	mov	sl, r1
	movs	r3, #28
	lsls	r4, r4, #1
	movs	r5, #224
	lsls	r6, r6, #1
	movs	r0, #96
	str	r1, [sp, #20]
	movs	r1, #211
	mov	fp, r2
	str	r3, [sp, #40]
	movs	r2, #224
	str	r4, [sp, #36]
	str	r5, [sp, #32]
	str	r6, [sp, #28]
	str	r0, [sp, #24]
	str	r1, [sp, #16]
	mov	r7, r9
	add	r2, r9
	adds	r7, #96
	mov	r8, r2
.L_0817a6fc:
	mov	r3, sl
	cmp	r3, #0
	bne.n	.L_0817a778
	ldr	r4, [sp, #160]
	cmp	r4, #0
	bne.n	.L_0817a71a
	movs	r3, #240
	movs	r6, #192
	mov	r5, r9
	lsls	r3, r3, #16
	lsls	r6, r6, #14
	movs	r0, #224
	str	r3, [r5, #96]
	str	r6, [r5, r0]
	b.n	.L_0817a76a
.L_0817a71a:
	ldr	r1, [sp, #160]
	cmp	r1, #47
	bgt.n	.L_0817a73a
	mov	r4, r9
	movs	r3, #96
	ldr	r2, [r4, r3]
	ldr	r5, [pc, #324]
	movs	r6, #224
	adds	r2, r2, r5
	str	r2, [r4, r3]
	movs	r0, #192
	ldr	r3, [r4, r6]
	lsls	r0, r0, #9
	adds	r3, r3, r0
	str	r3, [r4, r6]
	b.n	.L_0817a76a
.L_0817a73a:
	ldr	r1, [sp, #160]
	cmp	r1, #79
	bgt.n	.L_0817a756
	mov	r4, r9
	movs	r2, #96
	ldr	r3, [r4, r2]
	ldr	r1, [pc, #296]
	movs	r5, #224
	adds	r3, r3, r1
	str	r3, [r4, r2]
	ldr	r3, [r4, r5]
	adds	r3, r3, r1
	str	r3, [r4, r5]
	b.n	.L_0817a76a
.L_0817a756:
	ldr	r6, [sp, #160]
	cmp	r6, #111
	bgt.n	.L_0817a76a
	movs	r3, #96
	mov	r0, r9
	ldr	r2, [r0, r3]
	movs	r1, #192
	lsls	r1, r1, #10
	adds	r2, r2, r1
	str	r2, [r0, r3]
.L_0817a76a:
	ldr	r3, [sp, #160]
	cmp	r3, #112
	beq.n	.L_0817a772
	b.n	.L_0817ac5a
.L_0817a772:
	mov	r4, fp
	str	r4, [r7, #0]
	b.n	.L_0817ac5a
.L_0817a778:
	mov	r5, sl
	cmp	r5, #1
	bne.n	.L_0817a810
	ldr	r6, [sp, #160]
	cmp	r6, #27
	bgt.n	.L_0817a792
	movs	r2, #192
	mov	r1, r9
	mov	r0, fp
	lsls	r2, r2, #14
	movs	r3, #228
	str	r0, [r1, #100]
	b.n	.L_0817a800
.L_0817a792:
	ldr	r4, [sp, #160]
	cmp	r4, #28
	bne.n	.L_0817a7aa
	movs	r3, #128
	lsls	r3, r3, #13
	mov	r5, r9
	str	r3, [r5, #100]
	movs	r3, #128
	lsls	r3, r3, #14
	movs	r6, #228
	str	r3, [r5, r6]
	b.n	.L_0817a802
.L_0817a7aa:
	ldr	r0, [sp, #160]
	cmp	r0, #62
	bgt.n	.L_0817a7c8
	mov	r1, r9
	movs	r3, #100
	ldr	r2, [r1, r3]
	movs	r4, #235
	lsls	r4, r4, #9
	adds	r4, #216
	adds	r2, r2, r4
	str	r2, [r1, r3]
	movs	r5, #228
	ldr	r3, [r1, r5]
	ldr	r6, [pc, #172]
	b.n	.L_0817a7e8
.L_0817a7c8:
	ldr	r0, [sp, #160]
	cmp	r0, #79
	bgt.n	.L_0817a7ee
	mov	r1, r9
	movs	r3, #100
	ldr	r2, [r1, r3]
	movs	r4, #206
	lsls	r4, r4, #9
	adds	r4, #64
	adds	r2, r2, r4
	str	r2, [r1, r3]
	movs	r5, #228
	ldr	r3, [r1, r5]
	movs	r6, #206
	lsls	r6, r6, #7
	adds	r6, #16
.L_0817a7e8:
	adds	r3, r3, r6
	str	r3, [r1, r5]
	b.n	.L_0817a802
.L_0817a7ee:
	ldr	r0, [sp, #160]
	cmp	r0, #141
	bgt.n	.L_0817a802
	movs	r3, #100
	mov	r1, r9
	ldr	r2, [r1, r3]
	movs	r4, #192
	lsls	r4, r4, #10
	adds	r2, r2, r4
.L_0817a800:
	str	r2, [r1, r3]
.L_0817a802:
	ldr	r6, [sp, #160]
	cmp	r6, #112
	beq.n	.L_0817a80a
	b.n	.L_0817ac5a
.L_0817a80a:
	mov	r0, fp
	str	r0, [r7, #0]
	b.n	.L_0817ac5a
.L_0817a810:
	mov	r1, sl
	cmp	r1, #2
	bne.n	.L_0817a8de
	ldr	r2, [sp, #160]
	cmp	r2, #49
	bgt.n	.L_0817a82c
	movs	r5, #192
	mov	r4, r9
	mov	r3, fp
	lsls	r5, r5, #14
	movs	r6, #232
	str	r3, [r4, #104]
	str	r5, [r4, r6]
	b.n	.L_0817a8d0
.L_0817a82c:
	ldr	r0, [sp, #160]
	cmp	r0, #50
	bne.n	.L_0817a878
	movs	r3, #0
	mov	r1, r9
	str	r3, [r1, #104]
	movs	r3, #248
	lsls	r3, r3, #15
	movs	r2, #232
	str	r3, [r1, r2]
	b.n	.L_0817a8d0
	movs	r0, r0
	.4byte 0x03000730
	.4byte 0x05000200
	.4byte 0x02014260
	.4byte 0x3f3f3f3f
	.4byte 0x03000260
	.4byte 0x08196ec8
	.4byte 0xffff6800
	.4byte 0xfffee000
	.4byte 0x03001120
	.4byte 0xfffa0000
	.4byte 0xfffd0000
	.4byte 0xffff0000
	.2byte 0xfde8
	.2byte 0x0001
.L_0817a878:
	ldr	r3, [sp, #160]
	cmp	r3, #62
	bgt.n	.L_0817a894
	mov	r4, r9
	movs	r3, #104
	ldr	r2, [r4, r3]
	movs	r5, #128
	lsls	r5, r5, #11
	adds	r2, r2, r5
	str	r2, [r4, r3]
	movs	r6, #232
	ldr	r3, [r4, r6]
	ldr	r0, [pc, #840]
	b.n	.L_0817a8cc
.L_0817a894:
	ldr	r1, [sp, #160]
	cmp	r1, #93
	bgt.n	.L_0817a8b2
	mov	r4, r9
	movs	r3, #104
	ldr	r2, [r4, r3]
	movs	r5, #192
	lsls	r5, r5, #10
	adds	r2, r2, r5
	str	r2, [r4, r3]
	movs	r6, #232
	ldr	r3, [r4, r6]
	movs	r0, #192
	lsls	r0, r0, #8
	b.n	.L_0817a8cc
.L_0817a8b2:
	ldr	r1, [sp, #160]
	cmp	r1, #125
	bgt.n	.L_0817a8d0
	mov	r4, r9
	movs	r3, #104
	ldr	r2, [r4, r3]
	movs	r5, #192
	lsls	r5, r5, #10
	adds	r2, r2, r5
	str	r2, [r4, r3]
	movs	r6, #232
	ldr	r3, [r4, r6]
	ldr	r0, [pc, #788]
.L_0817a8cc:
	adds	r3, r3, r0
	str	r3, [r4, r6]
.L_0817a8d0:
	ldr	r1, [sp, #160]
	cmp	r1, #112
	beq.n	.L_0817a8d8
	b.n	.L_0817ac5a
.L_0817a8d8:
	mov	r2, fp
	str	r2, [r7, #0]
	b.n	.L_0817ac5a
.L_0817a8de:
	mov	r3, sl
	cmp	r3, #3
	bne.n	.L_0817a94c
	ldr	r4, [sp, #160]
	cmp	r4, #49
	bgt.n	.L_0817a8fa
	movs	r0, #192
	mov	r6, r9
	mov	r5, fp
	movs	r3, #236
	lsls	r0, r0, #14
	str	r5, [r6, #108]
	str	r0, [r6, r3]
	b.n	.L_0817a9b8
.L_0817a8fa:
	ldr	r1, [sp, #160]
	cmp	r1, #50
	bne.n	.L_0817a910
	movs	r3, #0
	mov	r2, r9
	str	r3, [r2, #108]
	movs	r3, #216
	movs	r2, #236
	lsls	r3, r3, #15
	mov	r4, r9
	b.n	.L_0817a9a0
.L_0817a910:
	ldr	r5, [sp, #160]
	cmp	r5, #103
	bgt.n	.L_0817a930
	mov	r6, r9
	movs	r2, #108
	ldr	r3, [r6, r2]
	movs	r0, #192
	lsls	r0, r0, #10
	adds	r3, r3, r0
	str	r3, [r6, r2]
	movs	r2, #236
	ldr	r3, [r6, r2]
	ldr	r1, [pc, #688]
	adds	r3, r3, r1
	str	r3, [r6, r2]
	b.n	.L_0817a9b8
.L_0817a930:
	ldr	r2, [sp, #160]
	cmp	r2, #135
	bgt.n	.L_0817a9b8
	mov	r4, r9
	movs	r2, #108
	ldr	r3, [r4, r2]
	movs	r1, #128
	lsls	r1, r1, #9
	adds	r3, r3, r1
	str	r3, [r4, r2]
	movs	r2, #236
	ldr	r3, [r4, r2]
	adds	r3, r3, r1
	b.n	.L_0817a9a0
.L_0817a94c:
	mov	r0, sl
	cmp	r0, #4
	bne.n	.L_0817a9c6
	ldr	r1, [sp, #160]
	cmp	r1, #59
	bgt.n	.L_0817a96a
	mov	r3, r9
	mov	r2, fp
	movs	r5, #192
	str	r2, [r3, #112]
	lsls	r5, r5, #14
	movs	r3, #240
	mov	r4, r9
	str	r5, [r4, r3]
	b.n	.L_0817a9b8
.L_0817a96a:
	ldr	r6, [sp, #160]
	cmp	r6, #60
	bne.n	.L_0817a982
	movs	r3, #128
	lsls	r3, r3, #13
	mov	r0, r9
	str	r3, [r0, #112]
	movs	r3, #128
	movs	r2, #240
	lsls	r3, r3, #14
	str	r3, [r0, r2]
	b.n	.L_0817a9b8
.L_0817a982:
	ldr	r1, [sp, #160]
	cmp	r1, #82
	bgt.n	.L_0817a9a4
	mov	r4, r9
	movs	r2, #112
	ldr	r3, [r4, r2]
	movs	r5, #128
	lsls	r5, r5, #11
	adds	r3, r3, r5
	str	r3, [r4, r2]
	movs	r2, #240
	ldr	r3, [r4, r2]
	movs	r6, #192
	lsls	r6, r6, #10
	adds	r3, r3, r6
.L_0817a9a0:
	str	r3, [r4, r2]
	b.n	.L_0817a9b8
.L_0817a9a4:
	ldr	r0, [sp, #160]
	cmp	r0, #135
	bgt.n	.L_0817a9b8
	movs	r2, #112
	mov	r1, r9
	ldr	r3, [r1, r2]
	movs	r4, #128
	lsls	r4, r4, #11
	adds	r3, r3, r4
	str	r3, [r1, r2]
.L_0817a9b8:
	ldr	r5, [sp, #160]
	cmp	r5, #112
	beq.n	.L_0817a9c0
	b.n	.L_0817ac5a
.L_0817a9c0:
	mov	r6, fp
	str	r6, [r7, #0]
	b.n	.L_0817ac5a
.L_0817a9c6:
	mov	r0, sl
	cmp	r0, #5
	bne.n	.L_0817aa2a
	ldr	r1, [sp, #160]
	cmp	r1, #85
	bgt.n	.L_0817a9e4
	mov	r3, r9
	mov	r2, fp
	movs	r5, #192
	str	r2, [r3, #116]
	lsls	r5, r5, #14
	movs	r3, #244
	mov	r4, r9
	str	r5, [r4, r3]
	b.n	.L_0817aa1c
.L_0817a9e4:
	ldr	r6, [sp, #160]
	cmp	r6, #86
	bne.n	.L_0817a9fc
	movs	r3, #144
	lsls	r3, r3, #16
	mov	r0, r9
	str	r3, [r0, #116]
	movs	r3, #128
	movs	r2, #244
	lsls	r3, r3, #14
	str	r3, [r0, r2]
	b.n	.L_0817aa1c
.L_0817a9fc:
	ldr	r1, [sp, #160]
	cmp	r1, #135
	bgt.n	.L_0817aa1c
	mov	r4, r9
	movs	r2, #116
	ldr	r3, [r4, r2]
	movs	r5, #128
	lsls	r5, r5, #9
	adds	r3, r3, r5
	str	r3, [r4, r2]
	movs	r2, #244
	ldr	r3, [r4, r2]
	movs	r6, #192
	lsls	r6, r6, #10
	adds	r3, r3, r6
	str	r3, [r4, r2]
.L_0817aa1c:
	ldr	r0, [sp, #160]
	cmp	r0, #112
	beq.n	.L_0817aa24
	b.n	.L_0817ac5a
.L_0817aa24:
	mov	r1, fp
	str	r1, [r7, #0]
	b.n	.L_0817ac5a
.L_0817aa2a:
	mov	r2, sl
	cmp	r2, #15
	bgt.n	.L_0817aafc
	ldr	r3, [sp, #160]
	cmp	r3, #0
	bne.n	.L_0817aa44
	movs	r3, #248
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	ldr	r4, [sp, #160]
	mov	r5, r8
	str	r4, [r5, #0]
	b.n	.L_0817ac5a
.L_0817aa44:
	ldr	r6, [sp, #160]
	ldr	r0, [sp, #16]
	cmp	r6, r0
	bne.n	.L_0817aa98
	bl	sub_08014878
	movs	r1, #7
	ands	r0, r1
	movs	r2, #184
	lsls	r2, r2, #15
	lsls	r0, r0, #16
	adds	r0, r0, r2
	str	r0, [r7, #0]
	bl	sub_08014878
	movs	r3, #7
	ands	r0, r3
	movs	r4, #208
	lsls	r4, r4, #15
	lsls	r0, r0, #16
	adds	r0, r0, r4
	mov	r5, r8
	str	r0, [r5, #0]
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	ands	r0, r3
	lsls	r3, r0, #1
	ldr	r6, [pc, #352]
	adds	r3, r3, r0
	ldr	r0, [sp, #28]
	lsls	r3, r3, #2
	adds	r3, r3, r6
	mov	r1, r9
	str	r3, [r0, r1]
	ldr	r2, [sp, #36]
	movs	r3, #128
	lsls	r3, r3, #11
	str	r3, [r2, r1]
	b.n	.L_0817ac5a
.L_0817aa98:
	ldr	r3, [sp, #160]
	ldr	r4, [sp, #16]
	cmp	r3, r4
	bge.n	.L_0817aaa2
	b.n	.L_0817ac5a
.L_0817aaa2:
	ldr	r2, [r7, #0]
	ldr	r5, [pc, #320]
	cmp	r2, r5
	blt.n	.L_0817aacc
	mov	r6, r8
	ldr	r3, [r6, #0]
	movs	r0, #168
	lsls	r0, r0, #16
	cmp	r3, r0
	bgt.n	.L_0817aacc
	ldr	r1, [sp, #28]
	mov	r4, r9
	ldr	r3, [r1, r4]
	adds	r3, r2, r3
	str	r3, [r7, #0]
	ldr	r5, [sp, #36]
	ldr	r2, [r6, #0]
	ldr	r3, [r5, r4]
	adds	r2, r2, r3
	str	r2, [r6, #0]
	b.n	.L_0817ac5a
.L_0817aacc:
	ldr	r6, [sp, #160]
	cmp	r6, #251
	ble.n	.L_0817aad4
	b.n	.L_0817ac5a
.L_0817aad4:
	bl	sub_08014878
	movs	r1, #7
	ands	r0, r1
	movs	r2, #184
	lsls	r2, r2, #15
	lsls	r0, r0, #16
	adds	r0, r0, r2
	str	r0, [r7, #0]
	bl	sub_08014878
	movs	r3, #7
	ands	r0, r3
	movs	r4, #208
	lsls	r0, r0, #16
	lsls	r4, r4, #15
	adds	r0, r0, r4
	mov	r5, r8
	str	r0, [r5, #0]
	b.n	.L_0817ac5a
.L_0817aafc:
	mov	r6, sl
	cmp	r6, #31
	ble.n	.L_0817ab04
	b.n	.L_0817ac5a
.L_0817ab04:
	ldr	r0, [sp, #160]
	cmp	r0, #0
	bne.n	.L_0817ab16
	movs	r3, #248
	lsls	r3, r3, #16
	mov	r1, r8
	str	r3, [r7, #0]
	str	r0, [r1, #0]
	b.n	.L_0817ac5a
.L_0817ab16:
	ldr	r2, [sp, #160]
	cmp	r2, #112
	bne.n	.L_0817ab58
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r5, r0, #0
	ands	r5, r3
	bl	sub_08014878
	movs	r3, #255
	ands	r3, r0
	movs	r4, #128
	lsls	r3, r3, #1
	lsls	r4, r4, #1
	adds	r6, r3, r4
	bl	sub_08014878
	movs	r1, #7
	ands	r0, r1
	movs	r2, #168
	lsls	r2, r2, #16
	lsls	r0, r0, #16
	adds	r0, r0, r2
	str	r0, [r7, #0]
	bl	sub_08014878
	movs	r3, #7
	ands	r0, r3
	movs	r4, #184
	b.n	.L_0817ab98
.L_0817ab58:
	ldr	r5, [sp, #160]
	cmp	r5, #252
	bne.n	.L_0817abec
	bl	sub_08014878
	movs	r3, #255
	lsls	r3, r3, #8
	adds	r3, #255
	adds	r5, r0, #0
	ands	r5, r3
	bl	sub_08014878
	movs	r3, #255
	ands	r3, r0
	movs	r0, #128
	lsls	r3, r3, #1
	lsls	r0, r0, #1
	adds	r6, r3, r0
	bl	sub_08014878
	movs	r1, #7
	ands	r0, r1
	movs	r2, #184
	lsls	r2, r2, #15
	lsls	r0, r0, #16
	adds	r0, r0, r2
	str	r0, [r7, #0]
	bl	sub_08014878
	movs	r3, #7
	ands	r0, r3
	movs	r4, #208
.L_0817ab98:
	lsls	r4, r4, #15
	lsls	r0, r0, #16
	adds	r0, r0, r4
	mov	r1, r8
	str	r0, [r1, #0]
	adds	r0, r5, #0
	bl	sub_08002096
	adds	r3, r6, #0
	muls	r3, r0
	cmp	r3, #0
	bge.n	.L_0817abb2
	adds	r3, #255
.L_0817abb2:
	ldr	r4, [sp, #28]
	asrs	r3, r3, #8
	mov	r2, r9
	str	r3, [r2, r4]
	adds	r0, r5, #0
	bl	sub_08002090
	ldr	r5, [sp, #20]
	adds	r3, r6, #0
	muls	r3, r0
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r2, r5, r1
	cmp	r3, #0
	bge.n	.L_0817abd2
	adds	r3, #255
.L_0817abd2:
	asrs	r3, r3, #8
	mov	r4, r9
	str	r3, [r4, r2]
	b.n	.L_0817ac5a
	movs	r0, r0
	.4byte 0xfffec000
	.4byte 0xffff0000
	.4byte 0xfff40000
	.2byte 0x0000
	.2byte 0xfff0
.L_0817abec:
	.2byte 0x9d28
	cmp	r5, #111
	ble.n	.L_0817ac5a
	ldr	r6, [sp, #28]
	mov	r0, r9
	ldr	r3, [r6, r0]
	ldr	r2, [r7, #0]
	mov	r1, r8
	adds	r2, r2, r3
	str	r2, [r7, #0]
	ldr	r4, [sp, #36]
	ldr	r2, [r1, #0]
	ldr	r3, [r4, r0]
	adds	r2, r2, r3
	str	r2, [r1, #0]
	ldr	r2, [r6, r0]
	lsls	r3, r2, #6
	subs	r3, r3, r2
	cmp	r3, #0
	bge.n	.L_0817ac16
	adds	r3, #63
.L_0817ac16:
	ldr	r6, [sp, #28]
	asrs	r3, r3, #6
	mov	r5, r9
	str	r3, [r5, r6]
	ldr	r0, [sp, #36]
	ldr	r2, [r5, r0]
	lsls	r3, r2, #6
	subs	r3, r3, r2
	cmp	r3, #0
	bge.n	.L_0817ac2c
	adds	r3, #63
.L_0817ac2c:
	asrs	r2, r3, #6
	ldr	r3, [sp, #36]
	mov	r1, r9
	str	r2, [r1, r3]
	ldr	r4, [sp, #28]
	movs	r5, #255
	ldr	r3, [r1, r4]
	lsls	r5, r5, #8
	ldr	r1, [pc, #56]
	adds	r5, #255
	adds	r3, r3, r5
	cmp	r3, r1
	bhi.n	.L_0817ac5a
	adds	r3, r2, r5
	cmp	r3, r1
	bhi.n	.L_0817ac5a
	ldr	r1, [sp, #24]
	mov	r6, r9
	mov	r0, fp
	str	r0, [r6, r1]
	ldr	r2, [sp, #32]
	movs	r3, #0
	str	r3, [r6, r2]
.L_0817ac5a:
	mov	r3, sl
	cmp	r3, #5
	bgt.n	.L_0817ac80
	ldr	r5, [sp, #40]
	ldr	r4, [pc, #24]
	movs	r1, #240
	ldrh	r0, [r4, r5]
	lsls	r1, r1, #4
	adds	r0, #32
	bl	sub_08002064
	ldr	r6, [pc, #8]
	adds	r1, r5, #0
	strh	r0, [r6, r1]
	b.n	.L_0817ac96
	.4byte 0x0001fffe
	.2byte 0x4002
	.2byte 0x0201
.L_0817ac80:
	ldr	r3, [sp, #40]
	ldr	r2, [pc, #112]
	movs	r1, #240
	ldrh	r0, [r2, r3]
	lsls	r1, r1, #4
	adds	r0, #128
	bl	sub_08002064
	ldr	r4, [pc, #96]
	ldr	r5, [sp, #40]
	strh	r0, [r4, r5]
.L_0817ac96:
	ldr	r6, [sp, #20]
	movs	r0, #240
	ldr	r4, [sp, #40]
	ldr	r1, [sp, #172]
	ldr	r2, [pc, #84]
	lsls	r0, r0, #7
	adds	r0, #12
	adds	r3, r6, r0
	ldr	r0, [r1, r3]
	ldrh	r3, [r2, r4]
	mov	r5, r9
	lsrs	r3, r3, #8
	lsls	r3, r3, #1
	ldrh	r1, [r5, r3]
	ldr	r3, [pc, #56]
	ldr	r2, [pc, #56]
	ands	r1, r3
	ldrh	r3, [r0, #8]
	adds	r4, #2
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r0, #8]
	ldr	r6, [sp, #36]
	ldr	r1, [sp, #32]
	adds	r6, #4
	ldr	r2, [sp, #28]
	ldr	r3, [sp, #24]
	ldr	r5, [sp, #16]
	str	r4, [sp, #40]
	ldr	r4, [sp, #20]
	str	r6, [sp, #36]
	movs	r6, #1
	movs	r0, #4
	add	sl, r6
	add	r8, r0
	adds	r1, #4
	adds	r2, #4
	adds	r3, #4
	adds	r4, #4
	adds	r5, #2
	mov	r0, sl
	str	r1, [sp, #32]
	b.n	.L_0817acf8
	.4byte 0x000003ff
	.4byte 0xfffffc00
	.2byte 0x4002
	.2byte 0x0201
.L_0817acf8:
	str	r2, [sp, #28]
	adds	r7, #4
	str	r3, [sp, #24]
	str	r4, [sp, #20]
	str	r5, [sp, #16]
	cmp	r0, #32
	beq.n	.L_0817ad08
	b.n	.L_0817a6fc
.L_0817ad08:
	ldr	r3, [sp, #172]
	movs	r6, #240
	ldr	r2, [pc, #752]
	lsls	r6, r6, #7
	adds	r6, #12
	ldr	r5, [sp, #48]
	movs	r1, #0
	adds	r4, r3, r6
	mov	r7, r9
	mov	r6, r9
	mov	sl, r1
	mov	r8, r2
	adds	r7, #224
	adds	r6, #96
.L_0817ad24:
	ldmia	r6!, {r3}
	ldmia	r4!, {r0}
	add	r3, r8
	str	r3, [r5, #0]
	ldmia	r7!, {r3}
	adds	r1, r5, #0
	add	r3, r8
	str	r3, [r5, #8]
	ldr	r2, [sp, #52]
	movs	r3, #0
	str	r4, [sp, #12]
	bl	sub_08020010
	movs	r0, #1
	add	sl, r0
	mov	r1, sl
	ldr	r4, [sp, #12]
	cmp	r1, #32
	bne.n	.L_0817ad24
	ldr	r2, [sp, #160]
	cmp	r2, #114
	bne.n	.L_0817ad56
	movs	r0, #212
	bl	sub_081c0010
.L_0817ad56:
	movs	r0, #128
	lsls	r0, r0, #2
	bl	sub_08014dac
	mov	r8, r0
	movs	r0, #1
	bl	sub_081969f8
	ldr	r2, [pc, #668]
	ldr	r3, [sp, #184]
	movs	r1, #7
	ands	r3, r2
	ldr	r2, [pc, #664]
	orrs	r3, r1
	ands	r3, r2
	movs	r2, #224
	lsls	r2, r2, #3
	orrs	r3, r2
	str	r3, [sp, #184]
	ldr	r3, [sp, #172]
	ldr	r4, [sp, #56]
	adds	r2, r3, r2
	ldr	r3, [pc, #648]
	adds	r6, r0, #0
	mov	r5, r8
	str	r1, [r6, #0]
	str	r2, [r4, #4]
	movs	r1, #128
	str	r4, [r6, #16]
	str	r3, [r6, #8]
	str	r5, [r6, #12]
	movs	r0, #0
	lsls	r1, r1, #8
	mov	sl, r0
	mov	fp, r1
.L_0817ad9c:
	ldr	r3, [pc, #624]
	mov	r2, sl
	lsls	r5, r2, #1
	ldrh	r3, [r3, r5]
	ldr	r4, [sp, #160]
	subs	r1, r4, r3
	cmp	r1, #0
	blt.n	.L_0817ae1a
	lsls	r7, r1, #13
	movs	r3, #0
	cmp	r1, #7
	ble.n	.L_0817adbe
	movs	r2, #8
	subs	r2, r2, r1
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #1
.L_0817adbe:
	movs	r0, #64
	negs	r0, r0
	str	r3, [r6, #20]
	cmp	r3, r0
	ble.n	.L_0817ae1a
	bl	sub_08014de4
	ldr	r3, [pc, #580]
	mov	r1, sl
	ldrsb	r0, [r3, r1]
	ldr	r3, [pc, #580]
	lsls	r0, r0, #16
	ldrsb	r1, [r3, r1]
	movs	r2, #0
	lsls	r1, r1, #16
	bl	sub_08015160
	movs	r1, #128
	mov	r2, fp
	lsls	r1, r1, #9
	mov	r0, fp
	bl	sub_080151e4
	ldr	r3, [pc, #556]
	ldrsh	r0, [r3, r5]
	bl	sub_080150e4
	ldr	r3, [pc, #552]
	ldrsh	r0, [r3, r5]
	bl	sub_08015024
	adds	r0, r7, #0
	bl	sub_0801521c
	ldr	r5, [sp, #160]
	lsls	r0, r5, #11
	bl	sub_08015068
	ldr	r0, [pc, #536]
	mov	r1, r8
	movs	r2, #4
	bl	sub_08196958
	adds	r0, r6, #0
	bl	sub_08196a7c
.L_0817ae1a:
	movs	r0, #1
	add	sl, r0
	mov	r1, sl
	cmp	r1, #4
	bne.n	.L_0817ad9c
	adds	r0, r6, #0
	bl	sub_08013164
	mov	r0, r8
	bl	sub_08013164
	bl	sub_08014de4
	movs	r0, #148
	lsls	r0, r0, #8
	bl	sub_08015024
	movs	r0, #208
	lsls	r0, r0, #7
	bl	sub_08015068
	ldr	r2, [sp, #160]
	cmp	r2, #156
	bne.n	.L_0817ae54
	movs	r3, #128
	lsls	r3, r3, #2
	movs	r4, #0
	str	r3, [sp, #128]
	str	r4, [sp, #132]
.L_0817ae54:
	ldr	r0, [sp, #132]
	bl	sub_080150e4
	ldr	r5, [sp, #160]
	cmp	r5, #156
	bne.n	.L_0817ae66
	movs	r0, #142
	bl	sub_081c0010
.L_0817ae66:
	ldr	r6, [sp, #160]
	cmp	r6, #223
	bne.n	.L_0817ae72
	movs	r0, #104
	bl	sub_081c0010
.L_0817ae72:
	ldr	r0, [sp, #160]
	cmp	r0, #252
	bne.n	.L_0817ae7e
	movs	r0, #212
	bl	sub_081c0010
.L_0817ae7e:
	ldr	r2, [sp, #160]
	subs	r2, #156
	cmp	r2, #74
	bls.n	.L_0817ae88
	b.n	.L_0817afe2
.L_0817ae88:
	lsrs	r3, r2, #31
	adds	r3, r2, r3
	ldr	r1, [sp, #160]
	asrs	r3, r3, #1
	movs	r2, #80
	subs	r2, r2, r3
	str	r2, [sp, #116]
	cmp	r1, #156
	bne.n	.L_0817aea0
	movs	r2, #0
	str	r2, [sp, #124]
	str	r2, [sp, #120]
.L_0817aea0:
	ldr	r3, [sp, #160]
	cmp	r3, #194
	bgt.n	.L_0817aeb2
	ldr	r4, [sp, #120]
	ldr	r5, [sp, #124]
	adds	r4, #32
	adds	r5, #3
	str	r4, [sp, #120]
	str	r5, [sp, #124]
.L_0817aeb2:
	ldr	r6, [sp, #128]
	ldr	r0, [sp, #132]
	adds	r6, #16
	adds	r0, r0, r6
	str	r0, [sp, #132]
	movs	r1, #3
	movs	r0, #188
	str	r6, [sp, #128]
	bl	sub_081963ec
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #188
	ldr	r3, [r3, #0]
	mov	r2, sp
	str	r3, [sp, #168]
	adds	r2, #252
	movs	r3, #132
	lsls	r3, r3, #1
	str	r2, [sp, #104]
	movs	r1, #0
	add	r3, sp
	mov	sl, r1
	mov	fp, r3
.L_0817aee2:
	mov	r3, sl
	cmp	r3, #0
	bge.n	.L_0817aeea
	adds	r3, #3
.L_0817aeea:
	ldr	r0, [sp, #120]
	asrs	r3, r3, #2
	lsls	r4, r3, #13
	mov	r5, sl
	lsls	r3, r3, #2
	subs	r3, r5, r3
	ldr	r1, [sp, #124]
	adds	r6, r0, #0
	muls	r6, r3
	adds	r0, r1, #0
	muls	r0, r3
	movs	r1, #30
	str	r4, [sp, #112]
	str	r6, [sp, #108]
	bl	sub_08002054
	ldr	r2, [sp, #116]
	ldr	r5, [sp, #160]
	adds	r7, r2, r0
	ldr	r0, [pc, #276]
	movs	r3, #7
	movs	r4, #3
	mov	r8, r3
	mov	ip, r3
	lsls	r3, r5, #2
	mov	lr, r4
	adds	r1, r5, #0
	movs	r6, #0
	adds	r4, r3, r0
	movs	r5, #208
.L_0817af26:
	ldr	r2, [sp, #160]
	cmp	r2, r5
	blt.n	.L_0817af60
	mov	r3, sl
	cmp	r3, #0
	bge.n	.L_0817af34
	adds	r3, #3
.L_0817af34:
	asrs	r3, r3, #2
	mov	r0, sl
	lsls	r3, r3, #2
	subs	r3, r0, r3
	mov	r0, lr
	subs	r2, r0, r6
	cmp	r3, r2
	bne.n	.L_0817af60
	ldr	r2, [sp, #116]
	adds	r0, r1, #0
	subs	r7, r2, r4
	subs	r0, #208
	cmp	r7, #0
	bge.n	.L_0817af52
	movs	r7, #0
.L_0817af52:
	mov	r3, ip
	subs	r3, r3, r0
	mov	r8, r3
	cmp	r3, #2
	bgt.n	.L_0817af60
	movs	r0, #3
	mov	r8, r0
.L_0817af60:
	adds	r6, #1
	subs	r4, #16
	subs	r1, #4
	adds	r5, #4
	cmp	r6, #4
	bne.n	.L_0817af26
	ldr	r1, [sp, #112]
	ldr	r2, [sp, #108]
	movs	r6, #0
	adds	r5, r1, r2
	adds	r0, r5, #0
	bl	sub_08002096
	adds	r3, r7, #0
	muls	r3, r0
	ldr	r4, [sp, #104]
	asrs	r3, r3, #16
	str	r3, [r4, #0]
	adds	r0, r5, #0
	bl	sub_08002090
	adds	r3, r7, #0
	muls	r3, r0
	ldr	r5, [sp, #104]
	asrs	r3, r3, #16
	str	r3, [r5, #4]
	str	r6, [r5, #8]
	ldr	r0, [sp, #104]
	mov	r1, fp
	bl	sub_0815e1ec
	mov	r6, fp
	ldr	r2, [r6, #0]
	ldr	r0, [pc, #136]
	mov	r1, r8
	lsls	r4, r1, #1
	asrs	r2, r2, #1
	str	r2, [r6, #0]
	subs	r1, r4, #2
	ldrh	r1, [r0, r1]
	ldr	r5, [sp, #148]
	ldr	r3, [r6, #4]
	mov	r6, r8
	lsrs	r0, r6, #31
	adds	r1, r5, r1
	add	r0, r8
	movs	r5, #1
	asrs	r0, r0, #1
	adds	r2, #48
	adds	r3, #92
	add	sl, r5
	subs	r2, r2, r0
	subs	r3, r3, r6
	str	r6, [sp, #0]
	str	r4, [sp, #4]
	ldr	r0, [sp, #176]
	ldr	r4, [sp, #168]
	mov	r6, sl
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2e20
	bne.n	.L_0817aee2
	movs	r0, #188
	bl	sub_0801314c
.L_0817afe2:
	ldr	r3, [sp, #160]
	subs	r3, #186
	cmp	r3, #79
	bls.n	.L_0817afec
	b.n	.L_0817b26c
.L_0817afec:
	ldr	r0, [sp, #160]
	str	r3, [sp, #100]
	cmp	r0, #247
	ble.n	.L_0817b030
	movs	r3, #133
	lsls	r2, r0, #1
	lsls	r3, r3, #2
	subs	r3, r3, r2
	str	r3, [sp, #100]
	b.n	.L_0817b03a
	.4byte 0xfff80000
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x08199364
	.4byte 0x0819942e
	.4byte 0x08199446
	.4byte 0x0819944a
	.4byte 0x08199436
	.4byte 0x0819943e
	.4byte 0x08199210
	.4byte 0xfffffcc0
	.2byte 0x7410
	.2byte 0x0819
.L_0817b030:
	ldr	r1, [sp, #100]
	cmp	r1, #36
	ble.n	.L_0817b03a
	movs	r2, #36
	str	r2, [sp, #100]
.L_0817b03a:
	ldr	r4, [sp, #100]
	mov	r0, sp
	movs	r3, #64
	adds	r7, r4, #0
	adds	r0, #248
	subs	r3, r3, r4
	str	r3, [sp, #96]
	str	r0, [sp, #80]
	strb	r7, [r0, #0]
	strb	r7, [r0, #1]
	ldr	r1, [sp, #160]
	lsrs	r3, r7, #31
	adds	r3, r7, r3
	movs	r2, #18
	asrs	r3, r3, #1
	subs	r6, r2, r3
	lsls	r0, r1, #10
	movs	r3, #36
	subs	r5, r3, r7
	bl	sub_080150e4
	ldr	r2, [sp, #160]
	cmp	r2, #221
	bgt.n	.L_0817b070
	lsls	r0, r5, #10
	bl	sub_08015024
.L_0817b070:
	cmp	r6, #0
	bge.n	.L_0817b076
	movs	r6, #0
.L_0817b076:
	cmp	r5, #0
	bge.n	.L_0817b07c
	movs	r5, #0
.L_0817b07c:
	adds	r6, #48
	str	r6, [sp, #92]
	movs	r3, #92
	ldr	r6, [sp, #164]
	subs	r3, r3, r5
	str	r3, [sp, #88]
	movs	r3, #88
	subs	r3, r3, r5
	movs	r4, #224
	movs	r5, #236
	str	r6, [sp, #76]
	str	r3, [sp, #84]
	add	r4, sp
	movs	r3, #0
	add	r5, sp
	mov	sl, r3
	mov	fp, r4
	mov	r8, r5
.L_0817b0a0:
	mov	r0, sl
	lsls	r5, r0, #9
	adds	r0, r5, #0
	bl	sub_08002096
	ldr	r1, [sp, #96]
	mov	r2, fp
	adds	r3, r1, #0
	muls	r3, r0
	asrs	r3, r3, #16
	str	r3, [r2, #0]
	adds	r0, r5, #0
	bl	sub_08002090
	ldr	r4, [sp, #96]
	mov	r5, fp
	adds	r3, r4, #0
	muls	r3, r0
	asrs	r3, r3, #16
	str	r3, [r5, #4]
	movs	r3, #0
	str	r3, [r5, #8]
	mov	r1, r8
	mov	r0, fp
	bl	sub_0815e1ec
	mov	r6, r8
	ldr	r2, [r6, #0]
	asrs	r2, r2, #1
	str	r2, [r6, #0]
	ldr	r0, [sp, #92]
	ldr	r3, [sp, #88]
	adds	r0, r2, r0
	str	r0, [sp, #8]
	ldr	r1, [r6, #4]
	adds	r7, r1, r3
	lsls	r3, r2, #1
	adds	r3, r3, r2
	cmp	r3, #0
	bge.n	.L_0817b0f2
	adds	r3, #3
.L_0817b0f2:
	ldr	r4, [sp, #92]
	asrs	r3, r3, #2
	adds	r6, r3, r4
	lsls	r3, r1, #1
	adds	r3, r3, r1
	cmp	r3, #0
	bge.n	.L_0817b102
	adds	r3, #3
.L_0817b102:
	ldr	r0, [sp, #84]
	ldr	r2, [sp, #8]
	asrs	r3, r3, #2
	adds	r5, r3, r0
	subs	r1, r7, #1
	movs	r3, #1
	mov	ip, r1
	str	r3, [sp, #0]
	movs	r4, #2
	add	r3, sp, #248
	str	r4, [sp, #4]
	adds	r1, r3, #0
	ldr	r4, [sp, #76]
	mov	r3, ip
	subs	r2, #1
	ldr	r0, [sp, #176]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2001
	add	r4, sp, #248
	movs	r1, #2
	subs	r3, r5, #1
	str	r0, [sp, #0]
	str	r1, [sp, #4]
	subs	r2, r6, #1
	adds	r1, r4, #0
	ldr	r0, [sp, #176]
	ldr	r4, [sp, #76]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x231f
	mov	r0, sl
	ands	r3, r0
	cmp	r3, #0
	bne.n	.L_0817b168
	ldr	r1, [sp, #100]
	ldr	r0, [sp, #8]
	str	r1, [sp, #0]
	adds	r2, r6, #0
	adds	r1, r7, #0
	adds	r3, r5, #0
	bl	sub_08143eb4
	ldr	r2, [sp, #100]
	adds	r1, r7, #1
	str	r2, [sp, #0]
	adds	r3, r5, #1
	ldr	r0, [sp, #8]
	adds	r2, r6, #0
	bl	sub_08143eb4
.L_0817b168:
	movs	r3, #1
	add	sl, r3
	mov	r4, sl
	cmp	r4, #128
	bne.n	.L_0817b0a0
	ldr	r3, [sp, #160]
	subs	r3, #228
	cmp	r3, #24
	bhi.n	.L_0817b26c
	ldr	r5, [sp, #160]
	movs	r3, #128
	lsls	r3, r3, #1
	subs	r7, r3, r5
	ldr	r6, [sp, #80]
	ldr	r3, [sp, #164]
	mov	r1, sp
	subs	r5, #204
	adds	r1, #200
	str	r5, [sp, #96]
	movs	r2, #212
	strb	r7, [r6, #0]
	strb	r7, [r6, #1]
	str	r1, [sp, #72]
	str	r6, [sp, #68]
	str	r3, [sp, #64]
	movs	r0, #0
	add	r2, sp
	mov	sl, r0
	mov	fp, r2
.L_0817b1a2:
	mov	r4, sl
	lsls	r5, r4, #9
	adds	r0, r5, #0
	bl	sub_08002096
	ldr	r6, [sp, #96]
	adds	r3, r6, #0
	muls	r3, r0
	ldr	r0, [sp, #72]
	asrs	r3, r3, #16
	str	r3, [r0, #0]
	adds	r0, r5, #0
	bl	sub_08002090
	adds	r3, r6, #0
	muls	r3, r0
	ldr	r1, [sp, #72]
	asrs	r3, r3, #16
	str	r3, [r1, #4]
	movs	r3, #0
	str	r3, [r1, #8]
	ldr	r0, [sp, #72]
	mov	r1, fp
	bl	sub_0815e1ec
	mov	r3, fp
	ldr	r2, [r3, #0]
	ldr	r1, [r3, #4]
	asrs	r2, r2, #1
	str	r2, [r3, #0]
	ldr	r4, [sp, #92]
	ldr	r5, [sp, #88]
	lsls	r3, r2, #1
	adds	r4, r4, r2
	adds	r5, r1, r5
	adds	r3, r3, r2
	mov	r8, r4
	str	r5, [sp, #8]
	cmp	r3, #0
	bge.n	.L_0817b1f4
	adds	r3, #3
.L_0817b1f4:
	ldr	r0, [sp, #92]
	asrs	r3, r3, #2
	adds	r6, r3, r0
	lsls	r3, r1, #1
	adds	r3, r3, r1
	cmp	r3, #0
	bge.n	.L_0817b204
	adds	r3, #3
.L_0817b204:
	ldr	r1, [sp, #84]
	asrs	r3, r3, #2
	adds	r5, r3, r1
	ldr	r3, [sp, #8]
	movs	r4, #1
	movs	r0, #2
	mov	r2, r8
	str	r4, [sp, #0]
	str	r0, [sp, #4]
	ldr	r1, [sp, #68]
	ldr	r4, [sp, #64]
	subs	r2, #1
	subs	r3, #1
	ldr	r0, [sp, #176]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2001
	movs	r1, #2
	subs	r3, r5, #1
	str	r0, [sp, #0]
	str	r1, [sp, #4]
	subs	r2, r6, #1
	ldr	r0, [sp, #176]
	ldr	r1, [sp, #68]
	ldr	r4, [sp, #64]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x231f
	mov	r0, sl
	ands	r3, r0
	cmp	r3, #0
	bne.n	.L_0817b262
	mov	r0, r8
	ldr	r1, [sp, #8]
	adds	r2, r6, #0
	adds	r3, r5, #0
	str	r7, [sp, #0]
	bl	sub_08143eb4
	ldr	r1, [sp, #8]
	adds	r3, r5, #1
	adds	r1, #1
	mov	r0, r8
	adds	r2, r6, #0
	str	r7, [sp, #0]
	bl	sub_08143eb4
.L_0817b262:
	movs	r1, #1
	add	sl, r1
	mov	r2, sl
	cmp	r2, #128
	bne.n	.L_0817b1a2
.L_0817b26c:
	ldr	r3, [sp, #172]
	movs	r4, #240
	lsls	r4, r4, #7
	adds	r4, #232
	adds	r2, r3, r4
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r5, [sp, #160]
	movs	r6, #150
	adds	r5, #1
	lsls	r6, r6, #1
	str	r5, [sp, #160]
	cmp	r5, r6
	beq.n	.L_0817b2a6
	ldr	r3, [pc, #232]
	movs	r2, #3
	ldr	r3, [r3, #12]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_0817b29e
	bl	.L_0817a332
.L_0817b29e:
	cmp	r5, #16
	bgt.n	.L_0817b2a6
	bl	.L_0817a332
.L_0817b2a6:
	ldr	r0, [pc, #212]
	bl	sub_08014644
	add	r0, sp, #156
	ldr	r3, [pc, #208]
	ldrh	r0, [r0, #0]
	movs	r2, #0
	strh	r0, [r3, #4]
	ldr	r1, [sp, #152]
	str	r2, [r1, #16]
	bl	sub_0814cca8
	movs	r4, #238
	ldr	r3, [sp, #172]
	lsls	r4, r4, #7
	movs	r2, #0
	adds	r4, #220
	mov	sl, r2
	adds	r5, r3, r4
.L_0817b2cc:
	movs	r6, #1
	ldmia	r5!, {r0}
	add	sl, r6
	bl	sub_08020048
	mov	r0, sl
	cmp	r0, #44
	bne.n	.L_0817b2cc
	bl	sub_08014c4c
	movs	r3, #128
	movs	r1, #160
	movs	r2, #128
	lsls	r3, r3, #19
	lsls	r1, r1, #19
	lsls	r2, r2, #24
	adds	r3, #212
	ldr	r0, [pc, #148]
	adds	r1, #160
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [pc, #140]
	movs	r2, #160
	ldrh	r3, [r3, #0]
	lsls	r2, r2, #19
	adds	r2, #188
	strh	r3, [r2, #0]
	ldr	r1, [sp, #172]
	movs	r3, #239
	lsls	r3, r3, #7
	adds	r2, r1, r3
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r4, [sp, #172]
	movs	r5, #238
	lsls	r5, r5, #7
	adds	r5, #132
	adds	r1, r4, r5
	movs	r3, #50
	str	r3, [r1, #0]
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	movs	r2, #0
	adds	r3, #40
	str	r2, [r1, #0]
	str	r2, [r3, #0]
	adds	r3, #40
	strh	r2, [r3, #0]
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #12
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	adds	r2, #70
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	subs	r2, #50
	strh	r3, [r2, #0]
	ldr	r0, [pc, #64]
	bl	sub_08013300
	adds	r6, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #60]
	adds	r1, r6, #0
	movs	r2, #32
	lsls	r0, r0, #19
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x982b
	movs	r2, #224
	adds	r6, #32
	lsls	r2, r2, #3
	adds	r1, r0, r2
	adds	r0, r6, #0
	b.n	.L_0817b394
	.4byte 0x00000784
	.4byte 0x00001010
	.4byte 0x00000080
	.4byte 0x03001150
	.4byte 0x0813baed
	.4byte 0x03001120
	.4byte 0x05000200
	.4byte 0x050001e8
	.4byte 0x0000009a
	.2byte 0x0730
	.2byte 0x0300
.L_0817b394:
	bl	sub_0801587c
	ldr	r3, [sp, #172]
	movs	r5, #0
	movs	r4, #184
	lsls	r4, r4, #5
	str	r5, [sp, #60]
	str	r5, [sp, #44]
	adds	r3, r3, r4
	movs	r6, #15
	mov	r9, r3
	mov	sl, r5
	mov	fp, r6
.L_0817b3ae:
	ldr	r1, [sp, #44]
	movs	r0, #0
	mov	lr, r0
	mov	r8, r1
.L_0817b3b6:
	movs	r2, #0
	mov	ip, r2
.L_0817b3ba:
	mov	r3, r8
	add	r3, lr
	lsls	r3, r3, #4
	add	r3, ip
	movs	r7, #0
	lsls	r5, r3, #3
.L_0817b3c6:
	mov	r3, r9
	adds	r0, r5, r3
	ldr	r3, [sp, #172]
	movs	r1, #224
	add	r3, sl
	lsls	r1, r1, #3
	movs	r6, #0
	adds	r4, r3, r1
.L_0817b3d6:
	ldrb	r1, [r4, #0]
	movs	r3, #15
	lsrs	r2, r1, #4
	ands	r2, r3
	mov	r3, fp
	ands	r3, r1
	adds	r6, #1
	movs	r1, #1
	strb	r3, [r0, #0]
	strb	r2, [r0, #1]
	adds	r4, #1
	add	sl, r1
	adds	r0, #2
	cmp	r6, #4
	bne.n	.L_0817b3d6
	adds	r7, #1
	adds	r5, #16
	cmp	r7, #8
	bne.n	.L_0817b3c6
	add	ip, r1
	mov	r2, ip
	cmp	r2, #2
	bne.n	.L_0817b3ba
	add	lr, r1
	mov	r3, lr
	cmp	r3, #2
	bne.n	.L_0817b3b6
	ldr	r4, [sp, #44]
	ldr	r5, [sp, #60]
	adds	r4, #2
	adds	r5, #1
	str	r4, [sp, #44]
	str	r5, [sp, #60]
	cmp	r5, #15
	bne.n	.L_0817b3ae
	movs	r6, #0
	str	r6, [sp, #160]
.L_0817b420:
	ldr	r0, [sp, #160]
	cmp	r0, #0
	bne.n	.L_0817b4a2
	ldr	r5, [pc, #416]
	movs	r1, #0
	mov	sl, r1
	movs	r7, #63
	movs	r6, #255
.L_0817b430:
	bl	sub_08014878
	ands	r0, r7
	adds	r0, #96
	lsls	r0, r0, #16
	str	r0, [r5, #0]
	bl	sub_08014878
	ands	r0, r7
	negs	r0, r0
	lsls	r0, r0, #16
	str	r0, [r5, #4]
	bl	sub_08014878
	ands	r0, r6
	adds	r0, #128
	negs	r0, r0
	lsls	r0, r0, #7
	str	r0, [r5, #12]
	bl	sub_08014878
	ands	r0, r6
	adds	r0, #128
	lsls	r0, r0, #9
	str	r0, [r5, #16]
	bl	sub_08014878
	movs	r3, #31
	movs	r2, #1
	ands	r3, r0
	add	sl, r2
	str	r3, [r5, #24]
	mov	r3, sl
	adds	r5, #28
	cmp	r3, #128
	bne.n	.L_0817b430
	ldr	r5, [sp, #180]
	movs	r4, #0
	ldr	r3, [r5, #20]
	mov	sl, r4
	cmp	r3, #0
	beq.n	.L_0817b4a2
	ldr	r5, [sp, #172]
	movs	r6, #15
	adds	r5, #24
.L_0817b48a:
	bl	sub_08014878
	ands	r0, r6
	adds	r0, #1
	str	r0, [r5, #0]
	ldr	r1, [sp, #180]
	movs	r0, #1
	ldr	r3, [r1, #20]
	add	sl, r0
	adds	r5, #28
	cmp	sl, r3
	bne.n	.L_0817b48a
.L_0817b4a2:
	ldr	r5, [pc, #292]
	movs	r2, #0
	mov	sl, r2
.L_0817b4a8:
	ldr	r3, [sp, #160]
	cmp	r3, sl
	ble.n	.L_0817b4fc
	ldr	r0, [r5, #24]
	cmp	r0, #0
	bge.n	.L_0817b4b6
	adds	r0, #3
.L_0817b4b6:
	movs	r1, #15
	asrs	r0, r0, #2
	bl	sub_08002064
	ldr	r4, [sp, #172]
	adds	r1, r0, #0
	movs	r0, #2
	ldrsh	r2, [r5, r0]
	lsls	r1, r1, #8
	movs	r6, #184
	adds	r1, r4, r1
	movs	r0, #16
	lsls	r6, r6, #5
	movs	r4, #6
	ldrsh	r3, [r5, r4]
	adds	r1, r1, r6
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	subs	r2, #4
	ldr	r0, [sp, #176]
	ldr	r6, [sp, #164]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x1c28
	movs	r1, #64
	movs	r2, #0
	bl	sub_08138086
	ldr	r3, [r5, #12]
	ldr	r0, [pc, #216]
	adds	r3, r3, r0
	str	r3, [r5, #12]
	ldr	r3, [r5, #24]
	adds	r3, #1
	str	r3, [r5, #24]
.L_0817b4fc:
	movs	r1, #1
	add	sl, r1
	mov	r2, sl
	adds	r5, #28
	cmp	r2, #64
	bne.n	.L_0817b4a8
	ldr	r3, [sp, #160]
	cmp	r3, #56
	bne.n	.L_0817b514
	movs	r0, #134
	bl	sub_081180e8
.L_0817b514:
	ldr	r3, [sp, #160]
	subs	r3, #57
	cmp	r3, #50
	bhi.n	.L_0817b57c
	ldr	r5, [sp, #180]
	movs	r4, #0
	ldr	r3, [r5, #20]
	mov	sl, r4
	cmp	r3, #0
	beq.n	.L_0817b57c
	ldr	r5, [sp, #172]
	movs	r6, #36
.L_0817b52c:
	ldr	r3, [r5, #24]
	subs	r3, #1
	str	r3, [r5, #24]
	cmp	r3, #0
	bne.n	.L_0817b56c
	movs	r3, #15
	str	r3, [r5, #24]
	ldr	r0, [sp, #172]
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #168
	adds	r2, r0, r1
	movs	r3, #4
	str	r3, [r2, #0]
	movs	r0, #133
	bl	sub_081c0010
	ldr	r2, [sp, #180]
	movs	r1, #0
	ldrsh	r0, [r6, r2]
	bl	sub_08118088
	ldr	r4, [sp, #180]
	movs	r3, #8
	movs	r2, #1
	ldrsh	r0, [r6, r4]
	negs	r2, r2
	str	r3, [sp, #0]
	movs	r1, #7
	mov	r3, sl
	bl	sub_0814cd48
.L_0817b56c:
	ldr	r4, [sp, #180]
	movs	r2, #1
	ldr	r3, [r4, #20]
	add	sl, r2
	adds	r6, #2
	adds	r5, #28
	cmp	sl, r3
	bne.n	.L_0817b52c
.L_0817b57c:
	movs	r0, #2
	movs	r1, #2
	bl	sub_08158ce0
	bl	sub_081434f8
	movs	r6, #240
	ldr	r5, [sp, #172]
	lsls	r6, r6, #7
	adds	r6, #232
	adds	r2, r5, r6
	movs	r3, #1
	movs	r0, #1
	str	r3, [r2, #0]
	bl	sub_08013560
	ldr	r0, [sp, #160]
	adds	r0, #1
	str	r0, [sp, #160]
	cmp	r0, #128
	beq.n	.L_0817b5a8
	b.n	.L_0817b420
.L_0817b5a8:
	ldr	r0, [pc, #36]
	bl	sub_08014644
	movs	r0, #104
	bl	sub_0801314c
	bl	sub_08143bb8
	add	sp, #292
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x02010000
	.4byte 0xfffff000
	.4byte 0x08143001
