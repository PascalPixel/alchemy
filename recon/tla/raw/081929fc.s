.syntax unified
	.thumb
	.set sub_08002054, 0x08002054
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
	.set sub_08014ee0, 0x08014ee0
	.set sub_080150e4, 0x080150e4
	.set sub_08015128, 0x08015128
	.set sub_080151ac, 0x080151ac
	.set sub_0801587c, 0x0801587c
	.set sub_08020010, 0x08020010
	.set sub_08020048, 0x08020048
	.set sub_08118040, 0x08118040
	.set sub_0813ba50, 0x0813ba50
	.set sub_081435e0, 0x081435e0
	.set sub_08143bb8, 0x08143bb8
	.set sub_08143eb4, 0x08143eb4
	.set sub_0814cc4c, 0x0814cc4c
	.set sub_0814cd48, 0x0814cd48
	.set sub_08157cf4, 0x08157cf4
	.set sub_0815b290, 0x0815b290
	.set sub_0815b3b0, 0x0815b3b0
	.set sub_0815b410, 0x0815b410
	.set sub_0815b434, 0x0815b434
	.set sub_0815b510, 0x0815b510
	.set sub_08164b2c, 0x08164b2c
	.set sub_0818caa8, 0x0818caa8
	.set sub_08191958, 0x08191958
	.set sub_08191c20, 0x08191c20
	.set sub_08191cc4, 0x08191cc4
	.set sub_08192648, 0x08192648
	.set sub_0819273c, 0x0819273c
	.set sub_08192828, 0x08192828
	.set sub_0819284c, 0x0819284c
	.set sub_08192894, 0x08192894
	.set sub_08192c2a, 0x08192c2a
	.set sub_0819506e, 0x0819506e
	.set sub_081963ec, 0x081963ec
	.set sub_08196958, 0x08196958
	.set sub_081969f8, 0x081969f8
	.set sub_08196a7c, 0x08196a7c
	.set sub_081c0010, 0x081c0010
	.global Func_081929fc
	.thumb_func
Func_081929fc:
.L_081929fc:
	push	{lr}
	mov	ip, r3
	mov	r3, r9
	push	{r3}
	mov	r3, ip
	sub	sp, #4
	mov	r3, sp
	mov	r2, r9
	str	r2, [r3, #0]
	adds	r3, r2, #0
	adds	r4, r3, #0
	subs	r4, #16
	ldr	r3, [r4, #0]
	movs	r0, #32
	ldr	r3, [r3, #24]
	movs	r1, #0
	cmp	r3, #0
	bne.n	.L_08192a24
	movs	r0, #0
	b.n	.L_08192a3c
.L_08192a24:
	adds	r1, #1
	cmp	r1, #32
	beq.n	.L_08192a3c
	lsls	r2, r1, #3
	ldr	r3, [r4, #0]
	subs	r2, r2, r1
	lsls	r2, r2, #2
	adds	r2, #24
	ldr	r3, [r3, r2]
	cmp	r3, #0
	bne.n	.L_08192a24
	adds	r0, r1, #0
.L_08192a3c:
	cmp	r0, #32
	bne.n	.L_08192a44
	movs	r0, #1
	negs	r0, r0
.L_08192a44:
	add	sp, #4
	pop	{r3}
	mov	r9, r3
	pop	{pc}
.L_08192a4c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #64
	mov	r0, r9
	add	r3, sp, #60
	str	r0, [r3, #0]
	adds	r3, r0, #0
	movs	r1, #0
	str	r3, [sp, #36]
	str	r1, [sp, #32]
	str	r1, [sp, #12]
	ldr	r1, [sp, #36]
	subs	r3, #16
	mov	r2, sp
	mov	r9, r3
	adds	r2, #40
	movs	r3, #4
	movs	r5, #24
	movs	r0, #8
	subs	r1, #24
	str	r2, [sp, #28]
	str	r3, [sp, #16]
	str	r5, [sp, #8]
	str	r0, [sp, #20]
	str	r1, [sp, #24]
	add	r7, sp, #48
.L_08192a8a:
	mov	r2, r9
	ldr	r4, [r2, #0]
	ldr	r3, [sp, #8]
	ldr	r1, [r4, r3]
	cmp	r1, #0
	bne.n	.L_08192a98
	b.n	.L_08192c4a
.L_08192a98:
	ldr	r5, [sp, #12]
	movs	r6, #15
	ldr	r3, [r4, r5]
	ldr	r5, [sp, #16]
	asrs	r0, r3, #16
	ldr	r2, [r4, r5]
	mov	lr, r0
	asrs	r0, r2, #16
	mov	ip, r0
	movs	r0, #254
	movs	r5, #0
	lsls	r0, r0, #15
	ands	r6, r1
	mov	sl, r5
	cmp	r3, r0
	bhi.n	.L_08192ac0
	cmp	r2, #0
	blt.n	.L_08192ac0
	cmp	r2, r0
	ble.n	.L_08192acc
.L_08192ac0:
	mov	r0, r9
	ldr	r2, [r0, #0]
	ldr	r1, [sp, #8]
	movs	r3, #0
	str	r3, [r2, r1]
	b.n	.L_08192c4a
.L_08192acc:
	movs	r3, #16
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_08192b06
	ldr	r3, [sp, #36]
	ldr	r5, [sp, #20]
	subs	r3, #12
	ldr	r2, [r3, #0]
	ldr	r3, [r4, r5]
	subs	r2, r2, r3
	cmp	r2, #32
	ble.n	.L_08192af4
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #7
	ldr	r0, [pc, #400]
	adds	r3, r3, r2
	lsls	r3, r3, #1
	adds	r0, r0, r3
	mov	sl, r0
.L_08192af4:
	movs	r1, #192
	lsls	r1, r1, #5
	adds	r1, #16
	cmp	sl, r1
	bne.n	.L_08192b06
	ldr	r2, [sp, #8]
	movs	r3, #0
	str	r3, [r4, r2]
	b.n	.L_08192c4a
.L_08192b06:
	mov	r3, r9
	ldr	r5, [r3, #0]
	ldr	r0, [sp, #8]
	movs	r2, #128
	ldr	r3, [r5, r0]
	lsls	r2, r2, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08192bfc
	movs	r0, #32
	bl	sub_08014dac
	mov	r8, r0
	movs	r0, #1
	bl	sub_081969f8
	movs	r1, #0
	mov	r2, r9
	ldr	r3, [sp, #36]
	mov	sl, r1
	ldr	r5, [sp, #20]
	ldr	r1, [r2, #0]
	subs	r3, #12
	ldr	r2, [r3, #0]
	ldr	r3, [r1, r5]
	adds	r6, r0, #0
	subs	r2, r2, r3
	cmp	r2, #24
	ble.n	.L_08192b48
	ldr	r0, [pc, #316]
	lsls	r3, r2, #11
	adds	r0, r0, r3
	mov	sl, r0
.L_08192b48:
	ldr	r3, [sp, #40]
	ldr	r2, [pc, #312]
	movs	r5, #248
	ands	r3, r2
	movs	r2, #5
	orrs	r3, r2
	ldr	r2, [pc, #304]
	ldr	r0, [sp, #28]
	ands	r3, r2
	movs	r2, #192
	lsls	r2, r2, #3
	orrs	r3, r2
	lsls	r5, r5, #5
	mov	r2, sl
	str	r3, [sp, #40]
	adds	r5, #16
	adds	r3, r1, r2
	adds	r3, r3, r5
	str	r3, [r0, #4]
	bl	sub_08014ee0
	mov	r2, r9
	ldr	r5, [sp, #12]
	ldr	r1, [r2, #0]
	ldr	r2, [pc, #272]
	ldr	r3, [r1, r5]
	movs	r0, #0
	adds	r3, r3, r2
	str	r3, [r7, #0]
	adds	r3, r5, #0
	adds	r3, #4
	ldr	r3, [r1, r3]
	mov	fp, r0
	adds	r3, r3, r2
	str	r3, [r7, #4]
	str	r0, [r7, #8]
	adds	r0, r7, #0
	bl	sub_08015128
	mov	r1, r9
	ldr	r5, [sp, #12]
	ldr	r3, [r1, #0]
	adds	r5, #20
	ldr	r3, [r3, r5]
	mov	r2, fp
	str	r3, [r7, #0]
	str	r3, [r7, #4]
	str	r2, [r7, #8]
	adds	r0, r7, #0
	bl	sub_080151ac
	mov	r3, r9
	ldr	r2, [r3, #0]
	movs	r0, #128
	ldr	r3, [r2, r5]
	lsls	r0, r0, #6
	adds	r3, r3, r0
	str	r3, [r2, r5]
	movs	r3, #4
	str	r3, [r6, #0]
	ldr	r1, [sp, #28]
	ldr	r3, [pc, #204]
	mov	r2, r8
	str	r3, [r6, #8]
	str	r1, [r6, #16]
	str	r2, [r6, #12]
	mov	r1, r8
	movs	r2, #4
	ldr	r0, [pc, #192]
	bl	sub_08196958
	adds	r0, r6, #0
	bl	sub_08196a7c
	adds	r0, r6, #0
	bl	sub_08013164
	mov	r0, r8
	bl	sub_08013164
	movs	r3, #224
	lsls	r3, r3, #6
	cmp	sl, r3
	bne.n	sub_08192c2a
	mov	r5, r9
	ldr	r3, [r5, #0]
	ldr	r0, [sp, #8]
	mov	r1, fp
	str	r1, [r3, r0]
	b.n	sub_08192c2a
.L_08192bfc:
	ldr	r3, [sp, #36]
	ldr	r2, [pc, #152]
	lsls	r4, r6, #1
	subs	r3, #28
	ldr	r0, [r3, #0]
	subs	r3, r4, #2
	ldrh	r1, [r2, r3]
	movs	r2, #224
	add	r1, sl
	adds	r1, r5, r1
	lsls	r2, r2, #3
	mov	r3, lr
	mov	r5, ip
	adds	r1, r1, r2
	lsrs	r2, r6, #1
	subs	r2, r3, r2
	str	r6, [sp, #0]
	subs	r3, r5, r6
	str	r4, [sp, #4]
	ldr	r5, [sp, #24]
	ldr	r4, [r5, #0]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x9a05
	mov	r0, r9
	ldr	r1, [r0, #0]
	ldr	r5, [sp, #12]
	adds	r2, #4
	ldr	r2, [r1, r2]
	ldr	r3, [r1, r5]
	adds	r3, r3, r2
	str	r3, [r1, r5]
	ldr	r2, [sp, #20]
	ldr	r0, [sp, #16]
	adds	r2, #8
	ldr	r3, [r1, r0]
	ldr	r2, [r1, r2]
	adds	r3, r3, r2
	str	r3, [r1, r0]
.L_08192c4a:
	ldr	r1, [sp, #16]
	ldr	r2, [sp, #12]
	ldr	r3, [sp, #8]
	ldr	r5, [sp, #20]
	ldr	r0, [sp, #32]
	adds	r1, #28
	adds	r2, #28
	adds	r3, #28
	adds	r5, #28
	adds	r0, #1
	str	r1, [sp, #16]
	str	r2, [sp, #12]
	str	r3, [sp, #8]
	str	r5, [sp, #20]
	str	r0, [sp, #32]
	cmp	r0, #32
	beq.n	.L_08192c6e
	b.n	.L_08192a8a
.L_08192c6e:
	add	sp, #64
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0xffff9fc0
	.4byte 0xffff4000
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0xffc00000
	.4byte 0x08199f98
	.4byte 0x08199f88
	.2byte 0x7410
	.2byte 0x0819
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r6, r0, #0
	movs	r0, #196
	sub	sp, #420
	lsls	r0, r0, #1
	add	r0, sp
	str	r0, [sp, #176]
	movs	r1, #192
	lsls	r1, r1, #18
	ldr	r3, [r1, #96]
	movs	r2, #202
	lsls	r2, r2, #1
	add	r2, sp
	str	r3, [r0, #0]
	str	r2, [sp, #64]
	adds	r7, r2, #0
	movs	r4, #206
	ldr	r2, [r1, #92]
	lsls	r4, r4, #1
	add	r4, sp
	adds	r3, r7, #0
	str	r2, [r3, #0]
	str	r4, [sp, #180]
	ldr	r3, [pc, #120]
	ldr	r5, [r1, #100]
	movs	r0, #208
	lsls	r0, r0, #1
	add	r0, sp
	str	r3, [r4, #0]
	str	r5, [sp, #204]
	str	r0, [sp, #184]
	ldr	r3, [pc, #108]
	movs	r4, #240
	str	r3, [r0, #0]
	ldr	r3, [r1, #36]
	lsls	r4, r4, #7
	str	r3, [sp, #200]
	ldr	r3, [pc, #100]
	adds	r4, #240
	ldrh	r3, [r3, #4]
	adds	r2, r2, r4
	str	r3, [sp, #196]
	movs	r0, #128
	ldr	r1, [r1, #48]
	lsls	r0, r0, #6
	str	r1, [sp, #192]
	str	r6, [r2, #0]
	bl	sub_081435e0
	bl	sub_0813ba50
	ldr	r2, [pc, #56]
	movs	r3, #160
	lsls	r3, r3, #19
	strh	r2, [r3, #0]
	adds	r3, #2
	strh	r2, [r3, #0]
	ldr	r5, [sp, #64]
	movs	r0, #239
	ldr	r2, [r5, #0]
	lsls	r0, r0, #7
	adds	r1, r2, r0
	movs	r3, #0
	str	r3, [r1, #0]
	movs	r1, #238
	lsls	r1, r1, #7
	adds	r1, #132
	adds	r2, r2, r1
	movs	r3, #75
	movs	r1, #200
	str	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #32]
	bl	sub_080145a8
	ldr	r3, [pc, #12]
	movs	r2, #128
	lsls	r2, r2, #19
	b.n	.L_08192d60
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x00007741
	.4byte 0x02010000
	.4byte 0x02011f40
	.4byte 0x03001120
	.2byte 0x3001
	.2byte 0x0814
.L_08192d60:
	strh	r3, [r2, #0]
	ldr	r3, [pc, #56]
	adds	r2, #12
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	adds	r2, #20
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	adds	r2, #50
	strh	r3, [r2, #0]
	ldr	r3, [pc, #48]
	subs	r2, #2
	strh	r3, [r2, #0]
	movs	r0, #0
	ldr	r2, [pc, #44]
	movs	r1, #4
	bl	sub_08191958
	ldr	r2, [sp, #176]
	movs	r1, #128
	ldr	r0, [r2, #0]
	ldr	r3, [pc, #36]
	lsls	r1, r1, #7
	movs	r2, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2280
	lsls	r2, r2, #19
	b.n	.L_08192db4
	movs	r0, r0
	.4byte 0x00000784
	.4byte 0x00000080
	.4byte 0x00001010
	.4byte 0x00003f44
	.4byte 0x0819253d
	.2byte 0x0260
	.2byte 0x0300
.L_08192db4:
	movs	r3, #128
	adds	r2, #212
	lsls	r3, r3, #24
.L_08192dba:
	ldr	r5, [r2, #8]
	ands	r5, r3
	cmp	r5, #0
	bne.n	.L_08192dba
	ldr	r3, [r7, #0]
	movs	r4, #240
	lsls	r4, r4, #7
	adds	r4, #232
	adds	r3, r3, r4
	movs	r2, #1
	str	r2, [r3, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #40
	str	r5, [r3, #0]
	ldr	r3, [pc, #40]
	adds	r2, #50
	strh	r3, [r2, #0]
	ldr	r3, [pc, #40]
	subs	r2, #2
	strh	r3, [r2, #0]
	ldr	r2, [r7, #0]
	movs	r5, #239
	movs	r0, #238
	lsls	r5, r5, #7
	lsls	r0, r0, #7
	adds	r1, r2, r5
	movs	r3, #2
	adds	r0, #132
	str	r3, [r1, #0]
	adds	r2, r2, r0
	movs	r3, #75
	str	r3, [r2, #0]
	ldr	r1, [sp, #192]
	b.n	.L_08192e1c
	.4byte 0x00000080
	.4byte 0x00001010
	.2byte 0x3f44
	.2byte 0x0000
.L_08192e1c:
	movs	r2, #54
	ldrsh	r1, [r1, r2]
	str	r1, [sp, #188]
	ldr	r3, [r6, #4]
	cmp	r3, #0
	bne.n	.L_08192e32
	ldr	r2, [sp, #192]
	movs	r3, #128
	lsls	r3, r3, #7
	strh	r3, [r2, #54]
	b.n	.L_08192e3a
.L_08192e32:
	ldr	r4, [sp, #192]
	movs	r3, #192
	lsls	r3, r3, #8
	strh	r3, [r4, #54]
.L_08192e3a:
	movs	r1, #19
	movs	r0, #104
	bl	sub_081963ec
	movs	r1, #3
	movs	r0, #188
	bl	sub_081963ec
	movs	r6, #198
	lsls	r6, r6, #1
	add	r6, sp
	adds	r5, r6, #0
	str	r5, [sp, #172]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #104]
	adds	r3, #188
	str	r2, [r6, #0]
	ldr	r5, [pc, #132]
	ldr	r3, [r3, #0]
	adds	r0, r5, #0
	str	r3, [r6, #4]
	ldr	r1, [pc, #128]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	adds	r0, r5, #0
	ldr	r1, [sp, #204]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	movs	r1, #192
	ldr	r2, [sp, #204]
	movs	r0, #0
	lsls	r1, r1, #2
	mov	sl, r0
	adds	r1, #2
.L_08192e88:
	ldrb	r3, [r2, #0]
	lsrs	r3, r3, #1
	strb	r3, [r2, #0]
	movs	r3, #1
	add	sl, r3
	adds	r2, #1
	cmp	sl, r1
	bne.n	.L_08192e88
	ldr	r0, [r7, #0]
	movs	r4, #224
	lsls	r4, r4, #3
	movs	r2, #128
	adds	r0, r0, r4
	lsls	r2, r2, #9
	movs	r1, #32
	bl	sub_0815b434
	bl	sub_0815b410
	movs	r5, #204
	lsls	r5, r5, #1
	add	r5, sp
	movs	r3, #0
	str	r5, [sp, #168]
	movs	r0, #188
	str	r3, [r5, #0]
	bl	sub_0801314c
	bl	sub_08014de4
	ldr	r6, [sp, #184]
	str	r6, [sp, #164]
.L_08192ec8:
	ldr	r3, [pc, #32]
	movs	r2, #3
	ldr	r3, [r3, #12]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08192ed6
	b.n	.L_08192ff4
.L_08192ed6:
	ldr	r0, [sp, #168]
	ldr	r3, [r0, #0]
	cmp	r3, #175
	bgt.n	.L_08192ef0
	movs	r3, #176
	str	r3, [r0, #0]
	b.n	.L_08192ff4
	.4byte 0x00000134
	.4byte 0x02010578
	.2byte 0x1150
	.2byte 0x0300
.L_08192ef0:
	movs	r1, #134
	subs	r3, #177
	lsls	r1, r1, #1
	cmp	r3, r1
	bhi.n	.L_08192fc2
	ldr	r3, [pc, #56]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r2, [sp, #176]
	movs	r1, #128
	ldr	r0, [r2, #0]
	lsls	r1, r1, #7
	ldr	r2, [pc, #40]
	ldr	r3, [pc, #44]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9c10
	movs	r5, #240
	ldr	r3, [r4, #0]
	lsls	r5, r5, #7
	adds	r5, #232
	adds	r3, r3, r5
	movs	r2, #1
	str	r2, [r3, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r6, [sp, #168]
	movs	r0, #193
	ldr	r3, [r6, #0]
	lsls	r0, r0, #1
	b.n	.L_08192f40
	.4byte 0x00001010
	.4byte 0x3f3f3f3f
	.2byte 0x0260
	.2byte 0x0300
.L_08192f40:
	cmp	r3, r0
	bgt.n	.L_08192fb8
	movs	r5, #238
	movs	r1, #0
	lsls	r5, r5, #7
	mov	sl, r1
	adds	r5, #220
.L_08192f4e:
	ldr	r2, [sp, #64]
	ldr	r3, [r2, #0]
	ldr	r0, [r3, r5]
	bl	sub_08020048
	movs	r3, #1
	add	sl, r3
	mov	r4, sl
	adds	r5, #4
	cmp	r4, #16
	bne.n	.L_08192f4e
	ldr	r5, [sp, #64]
	movs	r6, #240
	ldr	r3, [r5, #0]
	lsls	r6, r6, #7
	adds	r6, #240
	ldr	r0, [r3, r6]
	bl	sub_0814cc4c
	bl	sub_08014c4c
	ldr	r3, [r5, #0]
	movs	r0, #0
	ldr	r3, [r3, r6]
	mov	sl, r0
	ldr	r3, [r3, #20]
	cmp	r3, #0
	beq.n	.L_08192fb8
	ldr	r5, [sp, #64]
	movs	r7, #1
	adds	r4, r6, #0
	negs	r7, r7
	movs	r6, #36
.L_08192f90:
	ldr	r3, [r5, #0]
	movs	r2, #5
	ldr	r3, [r3, r4]
	str	r4, [sp, #16]
	ldrsh	r0, [r3, r6]
	movs	r3, #0
	str	r3, [sp, #0]
	adds	r1, r7, #0
	adds	r3, r7, #0
	bl	sub_0814cd48
	ldr	r3, [r5, #0]
	ldr	r4, [sp, #16]
	movs	r2, #1
	ldr	r3, [r3, r4]
	add	sl, r2
	ldr	r3, [r3, #20]
	adds	r6, #2
	cmp	sl, r3
	bne.n	.L_08192f90
.L_08192fb8:
	ldr	r4, [sp, #168]
	movs	r3, #223
	lsls	r3, r3, #1
	str	r3, [r4, #0]
	b.n	.L_08192ff4
.L_08192fc2:
	ldr	r5, [sp, #176]
	movs	r1, #128
	lsls	r1, r1, #7
	ldr	r2, [pc, #32]
	ldr	r0, [r5, #0]
	ldr	r6, [pc, #32]
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x9810
	movs	r1, #240
	ldr	r3, [r0, #0]
	lsls	r1, r1, #7
	adds	r1, #232
	adds	r3, r3, r1
	movs	r2, #1
	str	r2, [r3, #0]
	movs	r0, #1
	bl	sub_08013560
	bl	.L_081956a0
	subs	r7, #63
	subs	r7, #63
	lsls	r0, r4, #9
	lsls	r0, r0, #12
.L_08192ff4:
	ldr	r2, [sp, #168]
	movs	r4, #192
	ldr	r3, [r2, #0]
	lsls	r4, r4, #2
	adds	r4, #82
	cmp	r3, r4
	bne.n	.L_08193006
	bl	.L_081956a0
.L_08193006:
	cmp	r3, #0
	bne.n	.L_08193010
	movs	r0, #206
	bl	sub_081c0010
.L_08193010:
	ldr	r5, [pc, #748]
	adds	r6, r5, #0
	adds	r6, #11
.L_08193016:
	ldr	r0, [sp, #168]
	ldrb	r2, [r5, #0]
	ldr	r3, [r0, #0]
	adds	r5, #1
	cmp	r3, r2
	bne.n	.L_08193028
	movs	r0, #130
	bl	sub_081c0010
.L_08193028:
	cmp	r5, r6
	bne.n	.L_08193016
	ldr	r1, [sp, #168]
	ldr	r3, [r1, #0]
	cmp	r3, #175
	bne.n	.L_0819303a
	movs	r0, #212
	bl	sub_081c0010
.L_0819303a:
	ldr	r5, [pc, #712]
	movs	r2, #0
	mov	sl, r2
.L_08193040:
	ldr	r4, [sp, #168]
	ldrh	r2, [r5, #0]
	ldr	r3, [r4, #0]
	adds	r5, #2
	cmp	r3, r2
	bne.n	.L_08193052
	movs	r0, #209
	bl	sub_081c0010
.L_08193052:
	movs	r6, #1
	add	sl, r6
	mov	r0, sl
	cmp	r0, #5
	bne.n	.L_08193040
	ldr	r5, [pc, #680]
	movs	r1, #0
	mov	sl, r1
.L_08193062:
	ldr	r4, [sp, #168]
	ldrh	r2, [r5, #0]
	ldr	r3, [r4, #0]
	adds	r5, #2
	cmp	r3, r2
	bne.n	.L_08193074
	movs	r0, #212
	bl	sub_081c0010
.L_08193074:
	movs	r6, #1
	add	sl, r6
	mov	r0, sl
	cmp	r0, #6
	bne.n	.L_08193062
	ldr	r1, [sp, #168]
	movs	r2, #114
	ldr	r3, [r1, #0]
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_08193090
	movs	r0, #206
	bl	sub_081c0010
.L_08193090:
	movs	r3, #0
	movs	r5, #161
	mov	sl, r3
	lsls	r5, r5, #2
.L_08193098:
	ldr	r4, [sp, #168]
	ldr	r3, [r4, #0]
	cmp	r3, r5
	bne.n	.L_081930a6
	movs	r0, #104
	bl	sub_081c0010
.L_081930a6:
	movs	r6, #1
	add	sl, r6
	mov	r0, sl
	adds	r5, #12
	cmp	r0, #6
	bne.n	.L_08193098
	ldr	r1, [sp, #168]
	movs	r2, #250
	ldr	r3, [r1, #0]
	lsls	r2, r2, #1
	adds	r2, #255
	cmp	r3, r2
	bne.n	.L_081930c6
	movs	r0, #212
	bl	sub_081c0010
.L_081930c6:
	movs	r5, #252
	movs	r3, #0
	lsls	r5, r5, #1
	mov	sl, r3
	adds	r5, #255
.L_081930d0:
	ldr	r4, [sp, #168]
	ldr	r3, [r4, #0]
	cmp	r3, r5
	bne.n	.L_081930de
	movs	r0, #145
	bl	sub_081c0010
.L_081930de:
	movs	r6, #1
	add	sl, r6
	mov	r0, sl
	adds	r5, #9
	cmp	r0, #6
	bne.n	.L_081930d0
	ldr	r1, [sp, #168]
	movs	r2, #205
	ldr	r3, [r1, #0]
	lsls	r2, r2, #2
	cmp	r3, r2
	bne.n	.L_08193100
	movs	r0, #208
	bl	sub_081c0010
	ldr	r4, [sp, #168]
	ldr	r3, [r4, #0]
.L_08193100:
	cmp	r3, #0
	bne.n	.L_081931a4
	ldr	r5, [pc, #516]
	movs	r3, #240
	lsls	r3, r3, #14
	str	r3, [r5, #0]
	movs	r3, #128
	lsls	r3, r3, #16
	str	r3, [r5, #4]
	movs	r3, #192
	ldr	r0, [pc, #504]
	ldr	r1, [pc, #508]
	lsls	r3, r3, #12
	ldr	r2, [pc, #508]
	str	r3, [r5, #16]
	ldr	r7, [pc, #508]
	movs	r6, #0
	movs	r4, #150
	mov	ip, r0
	mov	lr, r1
	mov	sl, r6
	lsls	r4, r4, #16
	mov	r8, r2
	movs	r0, #0
	movs	r1, #0
.L_08193132:
	movs	r3, #200
	mov	r2, sl
	muls	r2, r3
	ldr	r3, [pc, #484]
	mov	r6, sl
	mov	r5, ip
	adds	r2, r2, r3
	ldrsb	r3, [r5, r6]
	mov	r5, lr
	lsls	r3, r3, #16
	str	r3, [r2, #0]
	ldrb	r3, [r7, r6]
	mov	r6, r8
	lsls	r3, r3, #16
	str	r3, [r2, #4]
	ldrh	r3, [r1, r5]
	str	r3, [r2, #8]
	ldrsh	r3, [r1, r6]
	ldr	r5, [pc, #460]
	str	r3, [r2, #12]
	movs	r3, #0
	str	r3, [r2, #16]
	mov	r3, sl
	adds	r2, r0, r3
	lsls	r3, r2, #2
	adds	r3, r3, r2
	movs	r6, #1
	lsls	r3, r3, #3
	mov	fp, r6
	adds	r3, r3, r5
.L_0819316e:
	movs	r6, #1
	add	fp, r6
	mov	r2, fp
	str	r4, [r3, #0]
	str	r4, [r3, #4]
	adds	r3, #20
	cmp	r2, #10
	bne.n	.L_0819316e
	add	sl, r6
	mov	r3, sl
	adds	r0, #4
	adds	r1, #2
	cmp	r3, #15
	bne.n	.L_08193132
	ldr	r4, [sp, #64]
	movs	r5, #239
	ldr	r2, [r4, #0]
	movs	r6, #238
	lsls	r5, r5, #7
	lsls	r6, r6, #7
	adds	r1, r2, r5
	movs	r3, #2
	adds	r6, #132
	str	r3, [r1, #0]
	adds	r2, r2, r6
	movs	r3, #50
	str	r3, [r2, #0]
.L_081931a4:
	ldr	r0, [sp, #168]
	ldr	r3, [r0, #0]
	cmp	r3, #175
	ble.n	.L_081931ae
	b.n	.L_08193450
.L_081931ae:
	movs	r1, #3
	movs	r0, #188
	bl	sub_081963ec
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #188
	ldr	r3, [r3, #0]
	ldr	r1, [sp, #172]
	movs	r2, #0
	str	r3, [r1, #4]
	str	r2, [sp, #24]
	mov	sl, r2
.L_081931c8:
	ldr	r4, [sp, #168]
	movs	r2, #1
	ldr	r3, [r4, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_081931fc
	ldr	r2, [sp, #24]
	ldr	r6, [pc, #336]
	add	r2, sl
	lsls	r3, r2, #2
	adds	r3, r3, r2
	movs	r5, #9
	lsls	r3, r3, #3
	mov	fp, r5
	adds	r2, r3, r6
.L_081931e6:
	ldr	r3, [r2, #0]
	movs	r0, #1
	str	r3, [r2, #20]
	negs	r0, r0
	ldr	r3, [r2, #4]
	add	fp, r0
	mov	r1, fp
	str	r3, [r2, #24]
	subs	r2, #20
	cmp	r1, #0
	bne.n	.L_081931e6
.L_081931fc:
	ldr	r4, [sp, #168]
	mov	r2, sl
	lsls	r3, r2, #3
	ldr	r2, [r4, #0]
	adds	r3, #24
	cmp	r2, r3
	blt.n	.L_081932b8
	movs	r3, #200
	mov	r5, sl
	muls	r5, r3
	ldr	r6, [pc, #268]
	adds	r3, r5, #0
	adds	r5, r3, r6
	movs	r0, #6
	ldrsh	r3, [r5, r0]
	cmp	r3, #59
	ble.n	.L_081932b8
	ldr	r0, [r5, #8]
	bl	sub_08002096
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	ldr	r2, [r5, #0]
	asrs	r3, r3, #1
	adds	r2, r2, r3
	str	r2, [r5, #0]
	ldr	r0, [r5, #8]
	bl	sub_08002090
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	ldr	r2, [r5, #4]
	ldr	r0, [r5, #12]
	asrs	r3, r3, #1
	subs	r2, r2, r3
	str	r2, [r5, #4]
	movs	r1, #3
	lsls	r0, r0, #1
	bl	sub_08002054
	ldr	r3, [r5, #8]
	movs	r1, #3
	adds	r3, r3, r0
	str	r3, [r5, #8]
	mov	r3, sl
	ands	r3, r1
	adds	r6, r3, #5
	movs	r2, #0
	mov	r3, sl
	mov	fp, r2
	lsls	r2, r3, #2
	add	r2, sl
	ldr	r5, [pc, #188]
	lsls	r3, r2, #2
	ldr	r0, [pc, #172]
	adds	r3, r3, r2
	asrs	r4, r6, #1
	lsls	r3, r3, #3
	mov	r9, r5
	mov	r8, r4
	lsls	r7, r6, #1
	adds	r5, r3, r0
.L_08193280:
	ldr	r1, [sp, #176]
	subs	r3, r7, #2
	mov	r2, r9
	ldr	r0, [r1, #0]
	ldrh	r1, [r2, r3]
	ldr	r3, [sp, #204]
	movs	r4, #2
	ldrsh	r2, [r5, r4]
	adds	r1, r3, r1
	mov	r3, r8
	subs	r2, r2, r3
	movs	r4, #6
	ldrsh	r3, [r5, r4]
	str	r6, [sp, #0]
	subs	r3, r3, r6
	mov	ip, r3
	str	r7, [sp, #4]
	ldr	r3, [sp, #172]
	adds	r5, #20
	ldr	r4, [r3, #4]
	mov	r3, ip
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2401
	add	fp, r4
	mov	r0, fp
	cmp	r0, #10
	bne.n	.L_08193280
.L_081932b8:
	ldr	r1, [sp, #24]
	movs	r2, #1
	add	sl, r2
	adds	r1, #4
	mov	r3, sl
	str	r1, [sp, #24]
	cmp	r3, #15
	beq.n	.L_081932ca
	b.n	.L_081931c8
.L_081932ca:
	ldr	r4, [sp, #168]
	ldr	r2, [r4, #0]
	cmp	r2, #71
	bgt.n	.L_08193366
	ldr	r5, [pc, #56]
	ldr	r6, [pc, #88]
	ldr	r3, [r5, #4]
	ldr	r0, [pc, #88]
	adds	r3, r3, r6
	str	r3, [r5, #4]
	ldr	r3, [r5, #16]
	adds	r3, r3, r0
	str	r3, [r5, #16]
	mov	r3, sl
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_081933ce
	movs	r4, #160
	ldr	r5, [pc, #12]
	lsls	r4, r4, #19
	movs	r1, #0
	adds	r4, #192
	mov	sl, r1
	movs	r6, #31
	b.n	.L_08193338
	.4byte 0x0000001f
	.4byte 0x08199fbc
	.4byte 0x08199fc8
	.4byte 0x08199fd2
	.4byte 0x02012ee0
	.4byte 0x08199fde
	.4byte 0x08199ffc
	.4byte 0x0819a01a
	.4byte 0x08199fed
	.4byte 0x02012ef4
	.4byte 0x02012f08
	.4byte 0x02012f94
	.4byte 0x08197410
	.4byte 0xffff0000
	.2byte 0xf000
	.2byte 0xffff
.L_08193338:
	.2byte 0x8820
	adds	r1, r6, #0
	lsls	r2, r0, #16
	lsrs	r3, r2, #26
	lsrs	r2, r2, #21
	ands	r3, r5
	ands	r2, r5
	adds	r3, #1
	adds	r2, #1
	lsls	r2, r2, #5
	ands	r1, r0
	lsls	r3, r3, #10
	orrs	r3, r2
	adds	r1, #1
	movs	r2, #1
	orrs	r3, r1
	add	sl, r2
	strh	r3, [r4, #0]
	mov	r3, sl
	adds	r4, #2
	cmp	r3, #128
	bne.n	.L_08193338
	b.n	.L_081933ce
.L_08193366:
	ldr	r4, [pc, #76]
	movs	r5, #128
	ldr	r3, [r4, #16]
	lsls	r5, r5, #6
	adds	r3, r3, r5
	str	r3, [r4, #16]
	ldr	r6, [sp, #168]
	movs	r2, #7
	ldr	r3, [r6, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_081933ce
	movs	r5, #160
	ldr	r6, [pc, #44]
	lsls	r5, r5, #19
	movs	r0, #0
	adds	r5, #192
	mov	sl, r0
.L_0819338a:
	ldrh	r2, [r5, #0]
	movs	r0, #31
	lsls	r3, r2, #16
	lsrs	r4, r3, #26
	lsrs	r1, r3, #21
	ands	r0, r2
	ands	r4, r6
	ands	r1, r6
	cmp	r0, #22
	bgt.n	.L_081933a0
	adds	r0, #1
.L_081933a0:
	cmp	r1, #1
	ble.n	.L_081933a6
	subs	r1, #1
.L_081933a6:
	cmp	r4, #14
	bgt.n	.L_081933b8
	adds	r4, #1
	b.n	.L_081933b8
	movs	r0, r0
	.4byte 0x0000001f
	.2byte 0x2ee0
	.2byte 0x0201
.L_081933b8:
	lsls	r2, r1, #5
	lsls	r3, r4, #10
	movs	r1, #1
	orrs	r3, r2
	add	sl, r1
	orrs	r3, r0
	mov	r2, sl
	strh	r3, [r5, #0]
	adds	r5, #2
	cmp	r2, #128
	bne.n	.L_0819338a
.L_081933ce:
	movs	r0, #188
	bl	sub_0801314c
	ldr	r4, [pc, #104]
	ldr	r6, [sp, #64]
	movs	r3, #18
	ldrsh	r5, [r4, r3]
	ldr	r0, [r6, #0]
	movs	r6, #224
	lsls	r5, r5, #1
	lsls	r6, r6, #3
	movs	r2, #128
	adds	r1, r5, #0
	lsls	r2, r2, #9
	adds	r0, r0, r6
	bl	sub_0815b434
	ldr	r1, [sp, #64]
	ldr	r3, [pc, #76]
	ldr	r0, [r1, #0]
	movs	r2, #2
	ldrsh	r1, [r3, r2]
	adds	r0, r0, r6
	movs	r4, #6
	ldrsh	r2, [r3, r4]
	adds	r3, r5, #0
	bl	sub_0818caa8
	ldr	r5, [sp, #168]
	ldr	r3, [r5, #0]
	cmp	r3, #175
	bne.n	.L_08193450
	ldr	r2, [pc, #44]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #12
	strh	r2, [r3, #0]
	ldr	r6, [sp, #176]
	movs	r1, #128
	ldr	r0, [r6, #0]
	lsls	r1, r1, #7
	ldr	r2, [pc, #32]
	ldr	r3, [pc, #36]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x20a0
	lsls	r0, r0, #19
	movs	r1, #128
	adds	r0, #192
	lsls	r1, r1, #1
	ldr	r2, [pc, #24]
	ldr	r4, [pc, #16]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe009
	.4byte 0x00000785
	.4byte 0x02012ee0
	.4byte 0x3f3f3f3f
	.4byte 0x03000260
	.2byte 0x7fff
	.2byte 0x7fff
.L_08193450:
	ldr	r5, [sp, #168]
	ldr	r3, [r5, #0]
	cmp	r3, #176
	beq.n	.L_0819345a
	b.n	.L_08193842
.L_0819345a:
	ldr	r6, [sp, #176]
	movs	r1, #128
	lsls	r1, r1, #7
	ldr	r2, [pc, #476]
	ldr	r0, [r6, #0]
	ldr	r3, [pc, #476]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9c10
	movs	r5, #240
	ldr	r3, [r4, #0]
	lsls	r5, r5, #7
	adds	r5, #232
	adds	r3, r3, r5
	movs	r5, #1
	str	r5, [r3, #0]
	movs	r0, #1
	bl	sub_08013560
	movs	r0, #237
	ldr	r6, [sp, #200]
	lsls	r0, r0, #3
	adds	r0, #255
	adds	r3, r6, r0
	strb	r5, [r3, #0]
	ldr	r1, [pc, #440]
	movs	r2, #0
	movs	r0, #1
	bl	sub_08118040
	ldr	r0, [pc, #436]
	bl	sub_08013300
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r5, r0, #0
	adds	r3, #212
	ldr	r1, [pc, #420]
	adds	r2, #128
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r1, #128
	lsls	r1, r1, #2
	adds	r5, r5, r1
	adds	r0, r5, #0
	ldr	r1, [pc, #408]
	bl	sub_0801587c
	movs	r5, #238
	movs	r3, #13
	ldr	r4, [pc, #404]
	ldr	r6, [pc, #396]
	movs	r2, #0
	negs	r3, r3
	lsls	r5, r5, #7
	mov	sl, r2
	adds	r7, r3, #0
	adds	r5, #220
.L_081934d2:
	movs	r1, #32
	ldr	r2, [pc, #388]
	movs	r3, #0
	movs	r0, #32
	str	r4, [sp, #16]
	bl	sub_0815b290
	ldr	r1, [sp, #64]
	movs	r2, #4
	ldr	r3, [r1, #0]
	ldr	r4, [sp, #16]
	str	r0, [r3, r5]
	ldrb	r3, [r0, #9]
	ands	r3, r7
	orrs	r3, r2
	strb	r3, [r0, #9]
	ldrb	r3, [r0, #16]
	ldr	r2, [pc, #360]
	lsls	r3, r3, #2
	adds	r3, r3, r4
	ldrh	r1, [r3, #2]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r1, r1, r2
	adds	r3, #212
	adds	r0, r6, #0
	ldr	r2, [pc, #348]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #1
	movs	r3, #128
	add	sl, r0
	lsls	r3, r3, #3
	mov	r1, sl
	adds	r6, r6, r3
	adds	r5, #4
	cmp	r1, #16
	bne.n	.L_081934d2
	ldr	r0, [pc, #328]
	bl	sub_08013300
	adds	r5, r0, #0
	ldr	r0, [pc, #324]
	movs	r2, #1
	movs	r1, #32
	negs	r2, r2
	ldr	r3, [pc, #276]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x3520
	ldr	r4, [sp, #64]
	movs	r6, #224
	ldr	r1, [r4, #0]
	lsls	r6, r6, #3
	adds	r1, r1, r6
	adds	r0, r5, #0
	bl	sub_0801587c
	ldr	r6, [pc, #272]
	movs	r0, #0
	mov	sl, r0
	movs	r5, #72
.L_0819354e:
	movs	r2, #128
	movs	r3, #240
	movs	r1, #16
	lsls	r2, r2, #23
	lsls	r3, r3, #8
	movs	r0, #16
	bl	sub_0815b290
	ldr	r4, [sp, #180]
	mov	r1, sl
	ldr	r3, [r4, #0]
	lsls	r2, r1, #2
	str	r0, [r2, r3]
	movs	r1, #13
	ldrb	r3, [r0, #9]
	negs	r1, r1
	adds	r2, r1, #0
	ands	r3, r2
	movs	r2, #4
	orrs	r3, r2
	strb	r3, [r0, #9]
	ldr	r3, [sp, #184]
	mov	r4, sl
	ldr	r2, [r3, #0]
	ldrh	r3, [r0, #8]
	lsls	r3, r3, #22
	lsrs	r3, r3, #22
	strh	r3, [r2, r5]
	adds	r2, r2, r5
	ldrb	r3, [r0, #16]
	strh	r4, [r2, #30]
	ldr	r4, [sp, #64]
	lsls	r3, r3, #2
	adds	r3, r3, r6
	ldrh	r1, [r3, #2]
	ldr	r3, [r4, #0]
	mov	r2, sl
	lsls	r0, r2, #7
	movs	r2, #224
	adds	r0, r0, r3
	lsls	r2, r2, #3
	ldr	r4, [pc, #188]
	adds	r0, r0, r2
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r1, r1, r4
	adds	r2, #32
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r0, #1
	add	sl, r0
	mov	r1, sl
	adds	r5, #2
	cmp	r1, #15
	bne.n	.L_0819354e
	ldr	r7, [sp, #180]
	movs	r2, #130
	mov	r8, r2
.L_081935c8:
	movs	r2, #128
	movs	r3, #240
	movs	r1, #16
	lsls	r2, r2, #23
	lsls	r3, r3, #8
	movs	r0, #16
	bl	sub_0815b3b0
	ldr	r2, [r7, #0]
	mov	r3, sl
	adds	r1, r0, #0
	lsls	r4, r3, #2
	str	r1, [r4, r2]
	movs	r3, #128
	ldr	r0, [r2, #0]
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r2, #6
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	str	r4, [sp, #16]
	bl	sub_08014878
	movs	r1, #15
	ldr	r5, [sp, #184]
	bl	sub_0800206c
	ldr	r6, [r5, #0]
	mov	r1, r8
	adds	r5, r6, #2
	strh	r0, [r5, r1]
	ldr	r3, [r7, #0]
	ldr	r4, [sp, #16]
	ldr	r2, [pc, #40]
	ldr	r0, [r4, r3]
	ldrh	r3, [r5, r1]
	lsls	r3, r3, #1
	adds	r3, #72
	ldrh	r1, [r6, r3]
	ldr	r3, [pc, #32]
	ands	r1, r3
	ldrh	r3, [r0, #8]
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r0, #8]
	movs	r3, #1
	add	sl, r3
	movs	r2, #2
	mov	r4, sl
	add	r8, r2
	cmp	r4, #36
	bne.n	.L_081935c8
	b.n	.L_08193670
	movs	r0, r0
	.4byte 0xfffffc00
	.4byte 0x000003ff
	.4byte 0x3f3f3f3f
	.4byte 0x03000260
	.4byte 0x00000076
	.4byte 0x000000a5
	.4byte 0x05000200
	.4byte 0x02010578
	.4byte 0x020036e0
	.4byte 0x80002000
	.4byte 0x06010000
	.4byte 0x84000100
	.4byte 0x0000009a
	.2byte 0x03e0
	.2byte 0x0500
.L_08193670:
	ldr	r7, [sp, #184]
	movs	r5, #0
	movs	r6, #63
	movs	r0, #176
	mov	sl, r5
	mov	r9, r6
	mov	r8, r0
.L_0819367e:
	bl	sub_08014878
	movs	r1, #15
	bl	sub_0800206c
	ldr	r5, [r7, #0]
	movs	r1, #228
	lsls	r1, r1, #14
	lsls	r0, r0, #16
	mov	r2, r8
	adds	r0, r0, r1
	str	r0, [r5, r2]
	bl	sub_08014878
	movs	r1, #15
	bl	sub_0800206c
	ldr	r6, [r7, #0]
	movs	r3, #158
	lsls	r3, r3, #15
	mov	r5, r8
	lsls	r0, r0, #16
	adds	r0, r0, r3
	adds	r5, #144
	str	r0, [r6, r5]
	bl	sub_08014878
	mov	r4, r9
	ldr	r2, [r7, #0]
	ands	r0, r4
	movs	r3, #144
	lsls	r3, r3, #1
	subs	r0, #32
	add	r3, r8
	lsls	r0, r0, #12
	str	r0, [r2, r3]
	bl	sub_08014878
	mov	r5, r9
	ldr	r2, [r7, #0]
	movs	r3, #216
	ands	r0, r5
	lsls	r3, r3, #1
	subs	r0, #32
	add	r3, r8
	lsls	r0, r0, #12
	str	r0, [r2, r3]
	movs	r0, #1
	add	sl, r0
	movs	r6, #4
	mov	r1, sl
	add	r8, r6
	cmp	r1, #18
	bne.n	.L_0819367e
	ldr	r7, [sp, #184]
	movs	r2, #63
	movs	r3, #248
	mov	r9, r2
	mov	r8, r3
.L_081936f4:
	bl	sub_08014878
	movs	r1, #15
	bl	sub_0800206c
	ldr	r5, [r7, #0]
	movs	r4, #169
	lsls	r4, r4, #16
	lsls	r0, r0, #16
	adds	r0, r0, r4
	mov	r6, r8
	str	r0, [r5, r6]
	bl	sub_08014878
	movs	r1, #15
	bl	sub_0800206c
	ldr	r6, [r7, #0]
	movs	r1, #158
	lsls	r1, r1, #15
	mov	r5, r8
	lsls	r0, r0, #16
	adds	r0, r0, r1
	adds	r5, #144
	str	r0, [r6, r5]
	bl	sub_08014878
	mov	r4, r9
	ldr	r2, [r7, #0]
	ands	r0, r4
	movs	r3, #144
	lsls	r3, r3, #1
	subs	r0, #32
	add	r3, r8
	lsls	r0, r0, #12
	str	r0, [r2, r3]
	bl	sub_08014878
	mov	r5, r9
	ldr	r2, [r7, #0]
	movs	r3, #216
	ands	r0, r5
	lsls	r3, r3, #1
	subs	r0, #32
	add	r3, r8
	lsls	r0, r0, #12
	str	r0, [r2, r3]
	movs	r0, #1
	add	sl, r0
	movs	r6, #4
	mov	r1, sl
	add	r8, r6
	cmp	r1, #36
	bne.n	.L_081936f4
	ldr	r2, [sp, #64]
	movs	r3, #224
	ldr	r1, [r2, #0]
	lsls	r3, r3, #3
	adds	r1, r1, r3
	ldr	r0, [pc, #312]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r6, [sp, #64]
	movs	r4, #0
	mov	sl, r4
	movs	r7, #4
	mov	r8, r4
.L_0819377e:
	bl	sub_08014878
	movs	r1, #15
	bl	sub_0800206c
	ldr	r5, [r6, #0]
	movs	r1, #132
	lsls	r1, r1, #14
	lsls	r0, r0, #16
	mov	r2, r8
	adds	r0, r0, r1
	str	r0, [r5, r2]
	bl	sub_08014878
	movs	r1, #15
	bl	sub_0800206c
	ldr	r5, [r6, #0]
	movs	r3, #228
	lsls	r3, r3, #14
	lsls	r0, r0, #16
	adds	r0, r0, r3
	str	r0, [r5, r7]
	bl	sub_08014878
	movs	r4, #31
	ldr	r2, [r6, #0]
	ands	r0, r4
	adds	r3, r7, #0
	negs	r0, r0
	lsls	r0, r0, #12
	adds	r3, #8
	str	r0, [r2, r3]
	movs	r0, #1
	add	sl, r0
	movs	r5, #28
	mov	r1, sl
	adds	r7, #28
	add	r8, r5
	cmp	r1, #6
	bne.n	.L_0819377e
	ldr	r6, [sp, #64]
	movs	r2, #168
	movs	r7, #172
	mov	r8, r2
.L_081937d8:
	bl	sub_08014878
	movs	r1, #15
	bl	sub_0800206c
	ldr	r5, [r6, #0]
	movs	r3, #162
	lsls	r3, r3, #15
	lsls	r0, r0, #16
	mov	r4, r8
	adds	r0, r0, r3
	str	r0, [r5, r4]
	bl	sub_08014878
	movs	r1, #15
	bl	sub_0800206c
	ldr	r5, [r6, #0]
	movs	r1, #228
	lsls	r1, r1, #14
	lsls	r0, r0, #16
	adds	r0, r0, r1
	str	r0, [r5, r7]
	bl	sub_08014878
	ldr	r2, [r6, #0]
	movs	r4, #31
	adds	r3, r7, #0
	ands	r0, r4
	lsls	r0, r0, #12
	adds	r3, #8
	str	r0, [r2, r3]
	movs	r0, #1
	add	sl, r0
	movs	r5, #28
	mov	r1, sl
	adds	r7, #28
	add	r8, r5
	cmp	r1, #12
	bne.n	.L_081937d8
	ldr	r3, [sp, #64]
	movs	r4, #239
	ldr	r2, [r3, #0]
	movs	r5, #238
	lsls	r4, r4, #7
	lsls	r5, r5, #7
	adds	r1, r2, r4
	movs	r3, #2
	adds	r5, #132
	str	r3, [r1, #0]
	adds	r2, r2, r5
	movs	r3, #70
	str	r3, [r2, #0]
.L_08193842:
	ldr	r6, [sp, #168]
	ldr	r3, [r6, #0]
	subs	r3, #177
	cmp	r3, #216
	bls.n	.L_0819384e
	b.n	.L_08193a6e
.L_0819384e:
	ldr	r3, [pc, #88]
	movs	r1, #175
	ldr	r4, [r3, #4]
	ldr	r3, [r3, #0]
	lsls	r1, r1, #1
	str	r3, [sp, #232]
	str	r4, [sp, #236]
	add	r3, sp, #376
	adds	r6, r3, #0
	movs	r3, #0
	str	r3, [r6, #12]
	movs	r3, #255
	lsls	r3, r3, #16
	str	r3, [r6, #4]
	ldr	r0, [sp, #168]
	movs	r7, #0
	ldr	r3, [r0, #0]
	cmp	r3, r1
	ble.n	.L_081938c6
	ldr	r4, [pc, #52]
	ldr	r5, [pc, #40]
	movs	r2, #0
	mov	sl, r2
.L_0819387c:
	ldrh	r3, [r4, #0]
	movs	r1, #31
	ands	r1, r3
	lsls	r3, r3, #16
	lsrs	r2, r3, #21
	lsrs	r3, r3, #26
	ands	r2, r5
	ands	r3, r5
	cmp	r1, #31
	beq.n	.L_08193892
	adds	r1, #1
.L_08193892:
	cmp	r2, #31
	beq.n	.L_08193898
	adds	r2, #1
.L_08193898:
	cmp	r3, #31
	beq.n	.L_081938b0
	adds	r3, #1
	b.n	.L_081938b0
	.4byte 0x0000001f
	.4byte 0x00000134
	.4byte 0x08196f48
	.2byte 0x0200
	.2byte 0x0500
.L_081938b0:
	lsls	r3, r3, #10
	lsls	r2, r2, #5
	orrs	r3, r2
	orrs	r3, r1
	strh	r3, [r4, #0]
	movs	r3, #1
	add	sl, r3
	mov	r0, sl
	adds	r4, #2
	cmp	r0, #255
	bne.n	.L_0819387c
.L_081938c6:
	ldr	r1, [sp, #168]
	movs	r2, #130
	ldr	r0, [r1, #0]
	adds	r2, #255
	cmp	r0, r2
	bgt.n	.L_08193980
	ldr	r4, [pc, #20]
	adds	r3, r0, r4
	cmp	r3, #14
	bhi.n	.L_081938f0
	ldr	r5, [pc, #16]
	adds	r0, r0, r5
	lsls	r0, r0, #10
	bl	sub_08002096
	lsls	r7, r0, #6
	b.n	.L_0819390a
	.4byte 0xfffffea1
	.2byte 0xfea2
	.2byte 0xffff
.L_081938f0:
	movs	r1, #110
	adds	r1, #255
	cmp	r0, r1
	ble.n	.L_0819390a
	movs	r2, #183
	lsls	r2, r2, #1
	subs	r2, r2, r0
	lsls	r3, r2, #2
	adds	r3, r3, r2
	movs	r2, #128
	lsls	r3, r3, #17
	lsls	r2, r2, #15
	adds	r7, r3, r2
.L_0819390a:
	ldr	r3, [sp, #168]
	ldr	r0, [r3, #0]
	cmp	r0, #229
	bgt.n	.L_08193922
	ldr	r4, [pc, #580]
	lsls	r0, r0, #6
	adds	r0, r0, r4
	bl	sub_08002096
	ldr	r5, [pc, #572]
	lsls	r0, r0, #6
	adds	r7, r0, r5
.L_08193922:
	mov	r1, sp
	adds	r1, #232
	movs	r2, #224
	str	r1, [sp, #36]
	movs	r0, #0
	lsls	r2, r2, #14
	mov	fp, r0
	adds	r4, r7, r2
	mov	r9, r0
.L_08193934:
	movs	r5, #238
	lsls	r5, r5, #7
	movs	r3, #0
	adds	r5, #220
	movs	r7, #144
	mov	r8, r3
	mov	sl, r4
	add	r5, r9
	lsls	r7, r7, #15
.L_08193946:
	mov	r0, sl
	str	r7, [r6, #0]
	str	r0, [r6, #8]
	ldr	r1, [sp, #64]
	ldr	r2, [sp, #36]
	ldr	r3, [r1, #0]
	adds	r1, r6, #0
	ldr	r0, [r3, r5]
	movs	r3, #0
	str	r4, [sp, #16]
	bl	sub_08020010
	movs	r3, #1
	movs	r2, #128
	add	r8, r3
	lsls	r2, r2, #14
	mov	r0, r8
	adds	r5, #4
	adds	r7, r7, r2
	ldr	r4, [sp, #16]
	cmp	r0, #4
	bne.n	.L_08193946
	add	fp, r3
	adds	r4, r4, r2
	movs	r1, #16
	mov	r2, fp
	add	r9, r1
	cmp	r2, #4
	bne.n	.L_08193934
.L_08193980:
	ldr	r4, [sp, #168]
	movs	r5, #193
	ldr	r3, [r4, #0]
	lsls	r5, r5, #1
	cmp	r3, r5
	bne.n	.L_08193a02
	movs	r5, #238
	movs	r6, #0
	lsls	r5, r5, #7
	mov	sl, r6
	adds	r5, #220
.L_08193996:
	ldr	r0, [sp, #64]
	ldr	r3, [r0, #0]
	ldr	r0, [r3, r5]
	bl	sub_08020048
	movs	r1, #1
	add	sl, r1
	mov	r2, sl
	adds	r5, #4
	cmp	r2, #16
	bne.n	.L_08193996
	ldr	r4, [sp, #64]
	movs	r6, #240
	ldr	r3, [r4, #0]
	lsls	r6, r6, #7
	adds	r6, #240
	ldr	r0, [r3, r6]
	bl	sub_0814cc4c
	bl	sub_08014c4c
	ldr	r0, [sp, #64]
	movs	r5, #0
	ldr	r3, [r0, #0]
	mov	sl, r5
	ldr	r3, [r3, r6]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	beq.n	.L_08193a02
	movs	r7, #1
	adds	r4, r6, #0
	adds	r5, r0, #0
	negs	r7, r7
	movs	r6, #36
.L_081939da:
	ldr	r3, [r5, #0]
	movs	r2, #5
	ldr	r3, [r3, r4]
	str	r4, [sp, #16]
	ldrsh	r0, [r3, r6]
	movs	r3, #0
	str	r3, [sp, #0]
	adds	r1, r7, #0
	adds	r3, r7, #0
	bl	sub_0814cd48
	ldr	r3, [r5, #0]
	ldr	r4, [sp, #16]
	movs	r2, #1
	ldr	r3, [r3, r4]
	add	sl, r2
	ldr	r3, [r3, #20]
	adds	r6, #2
	cmp	sl, r3
	bne.n	.L_081939da
.L_08193a02:
	ldr	r4, [sp, #168]
	ldr	r3, [r4, #0]
	cmp	r3, #191
	bgt.n	.L_08193a6e
	movs	r5, #0
	mov	sl, r5
.L_08193a0e:
	mov	r3, sl
	ldr	r6, [sp, #168]
	movs	r2, #1
	ands	r2, r3
	lsls	r3, r2, #1
	ldr	r1, [r6, #0]
	adds	r3, r3, r2
	lsls	r3, r3, #4
	subs	r3, r3, r2
	mov	r0, sl
	adds	r0, #62
	adds	r1, r1, r3
	movs	r3, #7
	ands	r3, r0
	adds	r1, #80
	asrs	r0, r0, #3
	lsls	r3, r3, #3
	lsls	r1, r1, #24
	lsls	r0, r0, #10
	adds	r4, r3, r0
	lsrs	r1, r1, #24
	movs	r3, #0
.L_08193a3a:
	adds	r5, r3, #0
	adds	r5, #8
	adds	r0, r3, #0
	cmp	r3, r5
	beq.n	.L_08193a5c
	ldr	r6, [sp, #176]
	mov	ip, r5
.L_08193a48:
	ldr	r3, [r6, #0]
	adds	r2, r3, r4
	ldrb	r3, [r2, #0]
	cmp	r3, r1
	bcs.n	.L_08193a54
	strb	r1, [r2, #0]
.L_08193a54:
	adds	r0, #1
	adds	r4, #1
	cmp	r0, ip
	bne.n	.L_08193a48
.L_08193a5c:
	adds	r3, r5, #0
	adds	r4, #56
	cmp	r3, #128
	bne.n	.L_08193a3a
	movs	r4, #1
	add	sl, r4
	mov	r5, sl
	cmp	r5, #3
	bne.n	.L_08193a0e
.L_08193a6e:
	ldr	r6, [sp, #168]
	ldr	r3, [r6, #0]
	subs	r3, #177
	cmp	r3, #46
	bls.n	.L_08193a7a
	b.n	.L_08193b64
.L_08193a7a:
	movs	r1, #224
	movs	r2, #160
	movs	r0, #0
	add	r1, sp
	lsls	r2, r2, #1
	mov	fp, r6
	mov	sl, r0
	mov	r9, r1
	add	r6, sp, #376
	movs	r7, #100
	mov	r8, r2
	movs	r4, #176
.L_08193a92:
	mov	r5, fp
	ldr	r3, [r5, #0]
	movs	r2, #128
	subs	r3, #177
	asrs	r3, r3, #1
	lsls	r3, r3, #11
	lsls	r2, r2, #9
	subs	r2, r2, r3
	movs	r3, #255
	mov	r0, r9
	movs	r1, #0
	lsls	r3, r3, #16
	str	r2, [r0, #4]
	str	r2, [sp, #224]
	str	r1, [r6, #12]
	str	r3, [r6, #4]
	ldr	r3, [sp, #164]
	mov	r1, r8
	ldr	r2, [r3, #0]
	mov	r0, sl
	ldr	r3, [r2, r4]
	lsls	r5, r0, #2
	str	r3, [r6, #0]
	str	r4, [sp, #16]
	ldr	r3, [r2, r1]
	adds	r1, r6, #0
	str	r3, [r6, #8]
	ldr	r2, [sp, #180]
	ldr	r3, [r2, #0]
	mov	r2, r9
	ldr	r0, [r5, r3]
	movs	r3, #0
	bl	sub_08020010
	ldr	r3, [sp, #164]
	ldr	r4, [sp, #16]
	ldr	r1, [r3, #0]
	movs	r0, #232
	lsls	r0, r0, #1
	adds	r2, r5, r0
	ldr	r2, [r1, r2]
	ldr	r3, [r1, r4]
	mov	r0, r8
	adds	r3, r3, r2
	str	r3, [r1, r4]
	movs	r3, #152
	lsls	r3, r3, #2
	adds	r2, r5, r3
	ldr	r2, [r1, r2]
	ldr	r3, [r1, r0]
	adds	r3, r3, r2
	str	r3, [r1, r0]
	mov	r2, fp
	ldr	r3, [r2, #0]
	movs	r2, #3
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08193b0e
	adds	r2, r1, #2
	ldrh	r3, [r2, r7]
	adds	r3, #1
	strh	r3, [r2, r7]
.L_08193b0e:
	ldr	r3, [sp, #184]
	ldr	r1, [r3, #0]
	adds	r2, r1, #2
	ldrh	r3, [r2, r7]
	cmp	r3, #14
	bls.n	.L_08193b1e
	ldr	r0, [pc, #48]
	strh	r0, [r2, r7]
.L_08193b1e:
	ldr	r2, [sp, #180]
	adds	r4, #4
	ldr	r3, [r2, #0]
	ldr	r2, [pc, #40]
	ldr	r0, [r5, r3]
	adds	r3, r1, #2
	ldrh	r3, [r3, r7]
	movs	r5, #1
	lsls	r3, r3, #1
	adds	r3, #72
	ldrh	r1, [r1, r3]
	ldr	r3, [pc, #28]
	add	sl, r5
	ands	r1, r3
	ldrh	r3, [r0, #8]
	adds	r7, #2
	ands	r3, r2
	orrs	r3, r1
	strh	r3, [r0, #8]
	movs	r3, #4
	mov	r0, sl
	add	r8, r3
	b.n	.L_08193b60
	.4byte 0x00000000
	.4byte 0xfffffc00
	.4byte 0x000003ff
	.4byte 0xffffd400
	.2byte 0x8e00
	.2byte 0xffeb
.L_08193b60:
	.2byte 0x2824
	bne.n	.L_08193a92
.L_08193b64:
	ldr	r1, [sp, #168]
	ldr	r3, [r1, #0]
	adds	r2, r3, #0
	subs	r2, #179
	cmp	r2, #38
	bhi.n	.L_08193c24
	adds	r7, r3, #0
	movs	r2, #0
	subs	r7, #170
	mov	sl, r2
.L_08193b78:
	mov	r3, sl
	lsls	r5, r3, #8
	adds	r0, r5, #0
	bl	sub_08002096
	adds	r6, r7, #0
	muls	r6, r0
	adds	r0, r5, #0
	bl	sub_08002090
	ldr	r2, [pc, #232]
	ldr	r5, [sp, #64]
	ldrh	r2, [r2, #6]
	ldr	r1, [r5, #0]
	ldr	r4, [sp, #176]
	adds	r1, r1, r2
	movs	r2, #224
	lsls	r2, r2, #3
	adds	r3, r7, #0
	muls	r3, r0
	adds	r1, r1, r2
	movs	r2, #4
	ldr	r0, [r4, #0]
	str	r2, [sp, #0]
	movs	r2, #8
	str	r2, [sp, #4]
	ldr	r5, [sp, #172]
	asrs	r6, r6, #16
	lsls	r3, r3, #1
	adds	r6, #58
	asrs	r3, r3, #16
	adds	r2, r6, #0
	adds	r3, #48
	ldr	r4, [r5, #0]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2601
	movs	r0, #128
	add	sl, r6
	lsls	r0, r0, #1
	cmp	sl, r0
	bne.n	.L_08193b78
	ldr	r6, [sp, #64]
	movs	r1, #0
	mov	sl, r1
	movs	r7, #4
	movs	r5, #0
.L_08193bd6:
	ldr	r3, [pc, #160]
	ldr	r2, [sp, #176]
	ldrh	r1, [r3, #2]
	ldr	r3, [r6, #0]
	ldr	r0, [r2, #0]
	adds	r1, r3, r1
	ldr	r2, [r3, r5]
	ldr	r3, [r3, r7]
	movs	r4, #224
	lsls	r4, r4, #3
	adds	r1, r1, r4
	asrs	r3, r3, #16
	movs	r4, #2
	str	r4, [sp, #0]
	subs	r3, #2
	movs	r4, #4
	mov	ip, r3
	str	r4, [sp, #4]
	ldr	r3, [sp, #172]
	asrs	r2, r2, #16
	ldr	r4, [r3, #0]
	subs	r2, #2
	mov	r3, ip
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x6831
	adds	r2, r7, #0
	adds	r2, #8
	ldr	r3, [r1, r5]
	ldr	r2, [r1, r2]
	movs	r4, #1
	add	sl, r4
	adds	r3, r3, r2
	mov	r0, sl
	str	r3, [r1, r5]
	adds	r7, #28
	adds	r5, #28
	cmp	r0, #12
	bne.n	.L_08193bd6
.L_08193c24:
	ldr	r1, [sp, #168]
	ldr	r0, [r1, #0]
	adds	r3, r0, #0
	subs	r3, #179
	cmp	r3, #31
	bhi.n	.L_08193c42
	movs	r2, #210
	ldr	r1, [pc, #56]
	movs	r3, #128
	subs	r2, r2, r0
	lsls	r3, r3, #19
	asrs	r2, r2, #1
	adds	r3, #82
	orrs	r2, r1
	strh	r2, [r3, #0]
.L_08193c42:
	cmp	r0, #230
	beq.n	.L_08193c48
	b.n	.L_08193e66
.L_08193c48:
	ldr	r3, [pc, #36]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
	ldr	r3, [pc, #32]
	subs	r2, #70
	strh	r3, [r2, #0]
	ldr	r3, [sp, #64]
	movs	r5, #192
	movs	r4, #224
	lsls	r5, r5, #2
	movs	r2, #1
	lsls	r4, r4, #3
	adds	r5, #2
	mov	sl, r2
	b.n	.L_08193c7c
	movs	r0, r0
	.4byte 0x00001000
	.4byte 0x00001010
	.4byte 0x00000784
	.2byte 0x7410
	.2byte 0x0819
.L_08193c7c:
	mov	r9, r3
	mov	r8, r4
	movs	r6, #8
	mov	lr, r5
.L_08193c84:
	movs	r0, #0
	mov	fp, r0
	mov	r7, lr
.L_08193c8a:
	ldr	r4, [pc, #244]
	mov	r1, fp
	lsls	r3, r1, #1
	mov	r5, r9
	ldrh	r2, [r4, r3]
	ldr	r1, [r5, #0]
	mov	r4, r8
	adds	r3, r1, r2
	adds	r2, r2, r7
	adds	r1, r1, r2
	mov	r2, fp
	adds	r2, #1
	adds	r0, r3, r4
	lsls	r3, r2, #1
	muls	r3, r2
	add	r1, r8
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_08193cce
	adds	r4, r6, #0
	mov	ip, r3
.L_08193cb4:
	ldrb	r3, [r0, #0]
	subs	r3, r3, r4
	lsls	r3, r3, #24
	lsrs	r3, r3, #24
	cmp	r3, #63
	bls.n	.L_08193cc2
	movs	r3, #0
.L_08193cc2:
	adds	r5, #1
	strb	r3, [r1, #0]
	adds	r0, #1
	adds	r1, #1
	cmp	r5, ip
	bne.n	.L_08193cb4
.L_08193cce:
	mov	fp, r2
	cmp	r2, #10
	bne.n	.L_08193c8a
	movs	r5, #192
	movs	r0, #1
	lsls	r5, r5, #2
	add	sl, r0
	adds	r5, #2
	mov	r1, sl
	adds	r6, #8
	add	lr, r5
	cmp	r1, #8
	bne.n	.L_08193c84
	ldr	r6, [sp, #64]
	movs	r2, #0
	mov	sl, r2
	movs	r4, #0
	movs	r7, #0
.L_08193cf2:
	ldr	r0, [r6, #0]
	movs	r3, #136
	lsls	r3, r3, #6
	mov	r5, sl
	adds	r0, r0, r7
	adds	r3, #18
	adds	r0, r0, r3
	lsls	r3, r5, #13
	movs	r5, #128
	lsls	r5, r5, #9
	subs	r5, r5, r3
	adds	r2, r5, #0
	movs	r1, #20
	str	r4, [sp, #16]
	bl	sub_0815b510
	ldr	r0, [r6, #0]
	ldr	r4, [sp, #16]
	movs	r1, #166
	lsls	r1, r1, #7
	adds	r0, r0, r4
	adds	r1, #18
	adds	r0, r0, r1
	adds	r2, r5, #0
	movs	r1, #12
	bl	sub_0815b510
	movs	r5, #1
	ldr	r4, [sp, #16]
	movs	r2, #144
	movs	r3, #200
	add	sl, r5
	lsls	r2, r2, #1
	lsls	r3, r3, #2
	mov	r0, sl
	adds	r4, r4, r2
	adds	r7, r7, r3
	cmp	r0, #8
	bne.n	.L_08193cf2
	ldr	r4, [sp, #184]
	ldr	r2, [pc, #56]
	ldr	r3, [r4, #0]
	movs	r1, #0
	mov	sl, r1
.L_08193d4a:
	movs	r5, #1
	add	sl, r5
	mov	r6, sl
	strh	r2, [r3, #0]
	adds	r3, #2
	cmp	r6, #36
	bne.n	.L_08193d4a
	ldr	r0, [pc, #40]
	ldr	r1, [sp, #204]
	movs	r2, #0
	movs	r3, #0
	bl	sub_08157cf4
	ldr	r5, [pc, #32]
	movs	r0, #0
	movs	r4, #254
	mov	sl, r0
	lsls	r4, r4, #1
.L_08193d6e:
	ldr	r2, [sp, #204]
	movs	r1, #0
	adds	r3, r0, r2
	mov	fp, r1
	adds	r2, r5, #0
	adds	r1, r3, r4
	b.n	.L_08193d8c
	.4byte 0x00000000
	.4byte 0x08197410
	.4byte 0x00000134
	.2byte 0xa038
	.2byte 0x0819
.L_08193d8c:
	ldrb	r3, [r2, #0]
	adds	r2, #1
	strb	r3, [r1, #0]
	movs	r3, #1
	add	fp, r3
	mov	r6, fp
	adds	r1, #1
	cmp	r6, #8
	bne.n	.L_08193d8c
	add	sl, r3
	mov	r1, sl
	adds	r0, #8
	cmp	r1, #32
	bne.n	.L_08193d6e
	ldr	r6, [pc, #696]
	ldr	r3, [sp, #64]
	ldr	r5, [sp, #204]
	ldr	r4, [pc, #696]
	movs	r2, #0
	mov	lr, r6
	mov	sl, r2
	mov	r8, r3
	movs	r7, #0
	mov	r9, lr
.L_08193dbc:
	movs	r0, #0
	mov	fp, r0
	movs	r6, #0
	mov	ip, r7
.L_08193dc4:
	mov	r2, lr
	ldr	r1, [r7, r2]
	ldr	r0, [pc, #668]
	lsls	r3, r1, #1
	subs	r3, #2
	ldrh	r2, [r0, r3]
	mov	r0, r8
	ldr	r3, [r0, #0]
	movs	r0, #224
	adds	r3, r3, r2
	lsls	r0, r0, #3
	adds	r2, r3, r0
	adds	r3, r1, #0
	muls	r3, r1
	lsls	r3, r3, #1
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L_08193e14
	mov	r1, ip
	add	r1, r9
	str	r1, [sp, #12]
	adds	r4, r6, #0
.L_08193df0:
	ldrb	r3, [r2, #0]
	subs	r3, r3, r4
	cmp	r3, #0
	bge.n	.L_08193dfa
	movs	r3, #0
.L_08193dfa:
	strb	r3, [r5, #0]
	ldr	r1, [sp, #12]
	adds	r0, #1
	ldr	r3, [r1, #0]
	adds	r5, #1
	adds	r1, r3, #0
	muls	r1, r3
	adds	r3, r1, #0
	lsls	r3, r3, #1
	adds	r2, #1
	cmp	r0, r3
	bne.n	.L_08193df0
	ldr	r4, [pc, #596]
.L_08193e14:
	movs	r2, #1
	add	fp, r2
	mov	r3, fp
	adds	r6, #16
	cmp	r3, #3
	bne.n	.L_08193dc4
	add	sl, r2
	mov	r6, sl
	adds	r7, #4
	cmp	r6, #2
	bne.n	.L_08193dbc
	movs	r0, #0
	mov	fp, r0
	b.n	.L_08193e32
.L_08193e30:
	ldr	r4, [pc, #564]
.L_08193e32:
	ldr	r1, [sp, #64]
	ldrh	r2, [r4, #2]
	ldr	r3, [r1, #0]
	movs	r4, #224
	adds	r3, r3, r2
	lsls	r4, r4, #3
	mov	r6, fp
	adds	r2, r3, r4
	movs	r0, #0
	lsls	r1, r6, #3
.L_08193e46:
	ldrb	r3, [r2, #0]
	subs	r3, r3, r1
	cmp	r3, #0
	bge.n	.L_08193e50
	movs	r3, #0
.L_08193e50:
	adds	r0, #1
	strb	r3, [r5, #0]
	adds	r2, #1
	adds	r5, #1
	cmp	r0, #8
	bne.n	.L_08193e46
	movs	r0, #1
	add	fp, r0
	mov	r1, fp
	cmp	r1, #8
	bne.n	.L_08193e30
.L_08193e66:
	movs	r0, #188
	movs	r1, #3
	bl	sub_081963ec
	ldr	r3, [sp, #168]
	ldr	r2, [r3, #0]
	adds	r3, r2, #0
	subs	r3, #240
	cmp	r3, #127
	bhi.n	.L_08193e96
	movs	r3, #15
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08193e96
	movs	r3, #240
	subs	r2, r3, r2
	cmp	r2, #0
	bge.n	.L_08193e8c
	adds	r2, #15
.L_08193e8c:
	asrs	r2, r2, #4
	adds	r0, r2, #0
	adds	r1, r2, #0
	bl	sub_08164b2c
.L_08193e96:
	movs	r4, #0
	str	r4, [sp, #28]
	mov	fp, r4
.L_08193e9c:
	ldr	r3, [pc, #460]
	ldr	r5, [sp, #28]
	ldr	r6, [sp, #168]
	ldrh	r1, [r3, r5]
	ldr	r2, [r6, #0]
	cmp	r2, r1
	bgt.n	.L_08193eac
	b.n	.L_081943f2
.L_08193eac:
	ldr	r0, [pc, #448]
	ldrh	r3, [r0, r5]
	cmp	r2, r3
	blt.n	.L_08193eb6
	b.n	.L_081943ae
.L_08193eb6:
	ldr	r3, [pc, #444]
	mov	r0, fp
	ldrb	r3, [r3, r0]
	subs	r2, r2, r1
	str	r3, [sp, #160]
	ldr	r3, [pc, #436]
	mov	sl, r2
	ldrb	r3, [r3, r0]
	ldr	r6, [pc, #436]
	mov	r9, r3
	ldr	r3, [pc, #436]
	ldrb	r3, [r3, r0]
	str	r3, [sp, #156]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #92]
	ldr	r5, [sp, #156]
	str	r1, [sp, #152]
	ldr	r2, [r3, #96]
	str	r2, [sp, #148]
	ldr	r4, [r3, #100]
	str	r4, [sp, #136]
	ldr	r3, [r3, #104]
	str	r3, [sp, #140]
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #188
	ldr	r3, [r3, #0]
	str	r3, [sp, #144]
	lsls	r3, r5, #2
	ldr	r3, [r3, r6]
	lsls	r0, r3, #1
	cmp	sl, r0
	blt.n	.L_08193efc
	b.n	.L_08194170
.L_08193efc:
	mov	r1, sl
	subs	r1, r0, r1
	mov	r8, r1
	mov	r0, r8
	movs	r1, #6
	bl	sub_08002054
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #7
	adds	r3, r3, r0
	lsls	r3, r3, #1
	str	r3, [sp, #132]
	ldr	r3, [sp, #136]
	lsls	r2, r0, #3
	adds	r2, r3, r2
	str	r2, [sp, #48]
	movs	r7, #0
.L_08193f20:
	lsls	r6, r7, #10
	adds	r0, r6, #0
	bl	sub_08002096
	mov	r5, r8
	muls	r5, r0
	ldr	r4, [sp, #160]
	lsrs	r3, r5, #31
	adds	r5, r5, r3
	adds	r0, r6, #0
	asrs	r5, r5, #17
	adds	r5, r5, r4
	bl	sub_08002090
	mov	r3, r8
	muls	r3, r0
	movs	r2, #2
	str	r2, [sp, #0]
	ldr	r6, [sp, #48]
	movs	r2, #4
	str	r2, [sp, #4]
	asrs	r3, r3, #16
	movs	r2, #222
	lsls	r2, r2, #1
	add	r3, r9
	adds	r1, r6, r2
	subs	r3, #2
	ldr	r0, [sp, #148]
	adds	r2, r5, #0
	ldr	r4, [sp, #140]
	adds	r7, #1
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2f40
	bne.n	.L_08193f20
	ldr	r5, [sp, #156]
	cmp	r5, #0
	bne.n	.L_08194056
	mov	r6, sl
	movs	r1, #0
	cmp	r6, #11
	bgt.n	.L_08193f78
	movs	r1, #196
	b.n	.L_08193f80
.L_08193f78:
	mov	r0, sl
	cmp	r0, #23
	bgt.n	.L_08193f80
	movs	r1, #98
.L_08193f80:
	ldr	r3, [sp, #136]
	ldr	r4, [sp, #140]
	mov	r2, sl
	lsls	r2, r2, #2
	adds	r1, r3, r1
	str	r2, [sp, #52]
	str	r1, [sp, #128]
	str	r4, [sp, #124]
	movs	r7, #0
.L_08193f92:
	ldr	r3, [sp, #52]
	lsls	r5, r7, #13
	add	r3, sl
	lsls	r3, r3, #9
	adds	r5, r5, r3
	adds	r0, r5, #0
	bl	sub_08002096
	mov	r3, r8
	muls	r3, r0
	ldr	r0, [sp, #160]
	asrs	r3, r3, #16
	adds	r6, r3, r0
	adds	r0, r5, #0
	bl	sub_08002090
	mov	r3, r8
	muls	r3, r0
	lsls	r3, r3, #1
	asrs	r3, r3, #16
	mov	r1, r9
	subs	r2, r6, #3
	adds	r0, r3, r1
	movs	r4, #7
	movs	r6, #14
	str	r4, [sp, #0]
	str	r6, [sp, #4]
	ldr	r1, [sp, #128]
	ldr	r4, [sp, #124]
	subs	r3, r0, #7
	ldr	r0, [sp, #148]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x106b
	adds	r5, r5, r3
	adds	r0, r5, #0
	bl	sub_08002096
	mov	r1, r8
	muls	r1, r0
	lsrs	r3, r1, #31
	adds	r1, r1, r3
	movs	r0, #160
	asrs	r1, r1, #1
	ldr	r6, [pc, #152]
	lsls	r0, r0, #10
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x9928
	asrs	r0, r0, #16
	adds	r6, r0, r1
	adds	r0, r5, #0
	bl	sub_08002090
	mov	r1, r8
	muls	r1, r0
	lsrs	r3, r1, #31
	adds	r1, r1, r3
	movs	r0, #160
	asrs	r1, r1, #1
	ldr	r2, [pc, #120]
	lsls	r0, r0, #11
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x1400
	add	r0, r9
	movs	r4, #7
	movs	r5, #14
	subs	r2, r6, #3
	subs	r3, r0, #7
	str	r4, [sp, #0]
	str	r5, [sp, #4]
	ldr	r0, [sp, #148]
	ldr	r1, [sp, #128]
	ldr	r6, [sp, #124]
	adds	r7, #1
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x2f08
	bne.n	.L_08193f92
	ldr	r3, [pc, #52]
	ldr	r0, [sp, #132]
	ldrh	r1, [r3, #16]
	ldr	r2, [sp, #152]
	adds	r1, r1, r0
	adds	r1, r2, r1
	movs	r3, #224
	ldr	r2, [sp, #160]
	lsls	r3, r3, #3
	movs	r0, #9
	adds	r1, r1, r3
	str	r0, [sp, #0]
	mov	r3, r9
	movs	r0, #18
	str	r0, [sp, #4]
	subs	r2, #4
	subs	r3, #9
	b.n	.L_08194166
.L_08194056:
	movs	r1, #147
	mov	r5, sl
	lsls	r1, r1, #1
	cmp	r5, #11
	bgt.n	.L_08194088
	adds	r1, #100
	b.n	.L_08194092
	.4byte 0x0819a040
	.4byte 0x08197410
	.4byte 0x0819a048
	.4byte 0x0819a054
	.4byte 0x0819a060
	.4byte 0x0819a066
	.4byte 0x08199f40
	.4byte 0x0819a06c
	.2byte 0x021c
	.2byte 0x0300
.L_08194088:
	mov	r6, sl
	cmp	r6, #23
	bgt.n	.L_08194092
	movs	r1, #172
	lsls	r1, r1, #1
.L_08194092:
	ldr	r2, [sp, #136]
	ldr	r3, [sp, #140]
	mov	r0, sl
	lsls	r0, r0, #2
	adds	r1, r2, r1
	str	r0, [sp, #52]
	str	r1, [sp, #120]
	str	r3, [sp, #116]
	movs	r7, #0
.L_081940a4:
	ldr	r3, [sp, #52]
	lsls	r5, r7, #13
	add	r3, sl
	lsls	r3, r3, #9
	adds	r5, r5, r3
	adds	r0, r5, #0
	bl	sub_08002096
	mov	r3, r8
	muls	r3, r0
	ldr	r4, [sp, #160]
	asrs	r3, r3, #16
	adds	r0, r5, #0
	adds	r6, r3, r4
	bl	sub_08002090
	mov	r3, r8
	muls	r3, r0
	lsls	r3, r3, #1
	asrs	r3, r3, #16
	mov	r1, r9
	subs	r2, r6, #2
	adds	r0, r3, r1
	movs	r4, #5
	movs	r6, #10
	str	r4, [sp, #0]
	str	r6, [sp, #4]
	ldr	r1, [sp, #120]
	ldr	r4, [sp, #116]
	subs	r3, r0, #5
	ldr	r0, [sp, #148]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x106b
	adds	r5, r5, r3
	adds	r0, r5, #0
	bl	sub_08002096
	mov	r1, r8
	muls	r1, r0
	lsrs	r3, r1, #31
	adds	r1, r1, r3
	movs	r0, #160
	asrs	r1, r1, #1
	ldr	r6, [pc, #472]
	lsls	r0, r0, #10
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x9928
	asrs	r0, r0, #16
	adds	r6, r0, r1
	adds	r0, r5, #0
	bl	sub_08002090
	mov	r1, r8
	muls	r1, r0
	lsrs	r3, r1, #31
	adds	r1, r1, r3
	movs	r0, #160
	asrs	r1, r1, #1
	ldr	r2, [pc, #440]
	lsls	r0, r0, #11
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x1400
	add	r0, r9
	movs	r4, #5
	movs	r5, #10
	subs	r2, r6, #2
	subs	r3, r0, #5
	str	r4, [sp, #0]
	str	r5, [sp, #4]
	ldr	r0, [sp, #148]
	ldr	r1, [sp, #120]
	ldr	r6, [sp, #116]
	adds	r7, #1
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x2f08
	bne.n	.L_081940a4
	ldr	r3, [pc, #404]
	ldr	r0, [sp, #132]
	ldrh	r1, [r3, #12]
	ldr	r2, [sp, #152]
	adds	r1, r1, r0
	adds	r1, r2, r1
	movs	r3, #224
	ldr	r2, [sp, #160]
	lsls	r3, r3, #3
	movs	r0, #7
	adds	r1, r1, r3
	str	r0, [sp, #0]
	mov	r3, r9
	movs	r0, #14
	str	r0, [sp, #4]
	subs	r2, #3
	subs	r3, #7
.L_08194166:
	ldr	r0, [sp, #148]
	ldr	r4, [sp, #140]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe0ce
.L_08194170:
	adds	r3, r0, r3
	cmp	sl, r3
	bge.n	.L_08194226
	mov	r5, sl
	subs	r5, r5, r0
	mov	r8, r5
	mov	r0, r8
	movs	r1, #3
	bl	sub_08002054
	lsls	r0, r0, #3
	cmp	r0, #64
	bne.n	.L_0819418c
	movs	r0, #56
.L_0819418c:
	movs	r6, #222
	ldr	r1, [sp, #136]
	lsls	r6, r6, #1
	adds	r0, r0, r6
	adds	r0, r1, r0
	str	r0, [sp, #44]
	movs	r7, #0
.L_0819419a:
	lsls	r6, r7, #10
	adds	r0, r6, #0
	bl	sub_08002096
	mov	r5, r8
	muls	r5, r0
	ldr	r2, [sp, #160]
	asrs	r5, r5, #16
	adds	r0, r6, #0
	adds	r5, r5, r2
	bl	sub_08002090
	mov	r3, r8
	muls	r3, r0
	lsls	r3, r3, #1
	movs	r2, #2
	asrs	r3, r3, #16
	str	r2, [sp, #0]
	add	r3, r9
	movs	r2, #4
	subs	r5, #1
	str	r2, [sp, #4]
	subs	r3, #2
	ldr	r0, [sp, #148]
	ldr	r1, [sp, #44]
	adds	r2, r5, #0
	ldr	r4, [sp, #140]
	adds	r7, #1
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2f40
	bne.n	.L_0819419a
	movs	r7, #0
.L_081941dc:
	lsls	r6, r7, #10
	adds	r0, r6, #0
	bl	sub_08002096
	mov	r5, r8
	muls	r5, r0
	ldr	r0, [sp, #160]
	lsls	r5, r5, #1
	asrs	r5, r5, #16
	adds	r5, r5, r0
	adds	r0, r6, #0
	bl	sub_08002090
	ldr	r2, [pc, #232]
	ldr	r1, [sp, #156]
	mov	r3, r8
	muls	r3, r0
	ldrb	r2, [r2, r1]
	asrs	r3, r3, #16
	lsls	r2, r2, #1
	add	r3, r9
	adds	r3, r3, r2
	movs	r2, #2
	str	r2, [sp, #0]
	subs	r5, #1
	movs	r2, #4
	str	r2, [sp, #4]
	subs	r3, #2
	ldr	r0, [sp, #148]
	ldr	r1, [sp, #44]
	adds	r2, r5, #0
	ldr	r4, [sp, #140]
	adds	r7, #1
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2f40
	bne.n	.L_081941dc
.L_08194226:
	ldr	r5, [sp, #156]
	ldr	r0, [pc, #184]
	lsls	r6, r5, #2
	ldr	r3, [r6, r0]
	mov	r1, sl
	lsls	r3, r3, #1
	movs	r7, #0
	subs	r3, r1, r3
	cmp	r7, r3
	bge.n	.L_0819427e
	mov	r2, r9
	cmp	r2, #112
	bgt.n	.L_0819427e
	ldr	r3, [sp, #160]
	ldr	r4, [sp, #136]
	movs	r5, #254
	lsls	r5, r5, #1
	adds	r5, r4, r5
	subs	r3, #4
	str	r5, [sp, #8]
	mov	r8, r3
	mov	r5, r9
.L_08194252:
	movs	r3, #8
	str	r3, [sp, #0]
	movs	r3, #32
	str	r3, [sp, #4]
	ldr	r1, [sp, #8]
	adds	r3, r5, #0
	ldr	r0, [sp, #148]
	mov	r2, r8
	ldr	r4, [sp, #140]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x481e
	mov	r1, sl
	ldr	r3, [r6, r0]
	adds	r7, #1
	lsls	r3, r3, #1
	subs	r3, r1, r3
	adds	r5, #32
	cmp	r7, r3
	bge.n	.L_0819427e
	cmp	r5, #112
	ble.n	.L_08194252
.L_0819427e:
	ldr	r2, [pc, #100]
	ldr	r3, [r6, r2]
	lsls	r2, r3, #1
	adds	r3, r2, #0
	adds	r3, #8
	cmp	sl, r3
	bge.n	.L_081942ac
	mov	r4, sl
	subs	r3, r2, r4
	adds	r3, #8
	ldr	r0, [pc, #84]
	asrs	r2, r3, #1
	movs	r3, #1
	movs	r1, #32
	ands	r3, r4
	strh	r1, [r0, #6]
	cmp	r3, #0
	beq.n	.L_081942a8
	adds	r3, r2, #0
	adds	r3, #32
	b.n	.L_081942aa
.L_081942a8:
	subs	r3, r1, r2
.L_081942aa:
	strh	r3, [r0, #6]
.L_081942ac:
	ldr	r5, [sp, #156]
	cmp	r5, #0
	bne.n	.L_081942ec
	ldr	r6, [sp, #152]
	movs	r0, #136
	lsls	r0, r0, #6
	adds	r0, #18
	ldr	r2, [sp, #160]
	adds	r1, r6, r0
	movs	r0, #20
	mov	r3, r9
	str	r0, [sp, #0]
	movs	r0, #40
	str	r0, [sp, #4]
	subs	r2, #10
	subs	r3, #20
	ldr	r0, [sp, #148]
	ldr	r4, [sp, #144]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0xe01b
	movs	r0, r0
	.4byte 0x0300021c
	.4byte 0x08197410
	.4byte 0x08199f48
	.4byte 0x08199f40
	.2byte 0x1120
	.2byte 0x0300
.L_081942ec:
	ldr	r5, [sp, #152]
	movs	r6, #166
	ldr	r2, [sp, #160]
	movs	r0, #12
	lsls	r6, r6, #7
	adds	r6, #18
	mov	r3, r9
	str	r0, [sp, #0]
	movs	r0, #24
	str	r0, [sp, #4]
	adds	r1, r5, r6
	subs	r2, #6
	subs	r3, #12
	ldr	r0, [sp, #148]
	ldr	r4, [sp, #144]
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x4b1b
	ldr	r5, [sp, #28]
	mov	r6, fp
	ldrh	r2, [r3, r5]
	ldr	r3, [pc, #104]
	ldr	r0, [sp, #168]
	ldrb	r3, [r3, r6]
	lsls	r3, r3, #4
	subs	r2, r2, r3
	ldr	r3, [r0, #0]
	adds	r2, #48
	cmp	r2, r3
	bne.n	.L_081943f2
	ldr	r6, [sp, #184]
	movs	r1, #0
	mov	sl, r1
	movs	r7, #31
.L_08194330:
	add	r2, sp, #420
	mov	r9, r2
	bl	sub_0819284c
	ldr	r1, [r6, #0]
	ldr	r3, [pc, #60]
	lsls	r2, r0, #1
	strh	r3, [r1, r2]
	ldr	r3, [pc, #64]
	mov	r4, fp
	ldrb	r3, [r3, r4]
	lsls	r5, r0, #2
	adds	r2, r5, #0
	adds	r2, #176
	lsls	r3, r3, #17
	str	r3, [r1, r2]
	ldr	r3, [pc, #52]
	movs	r0, #160
	ldrb	r3, [r3, r4]
	movs	r4, #128
	lsls	r4, r4, #14
	lsls	r0, r0, #1
	lsls	r3, r3, #16
	adds	r3, r3, r4
	adds	r2, r5, r0
	str	r3, [r1, r2]
	bl	sub_08014878
	ldr	r2, [r6, #0]
	movs	r1, #232
	ands	r0, r7
	lsls	r1, r1, #1
	subs	r0, #16
	adds	r3, r5, r1
	lsls	r0, r0, #12
	b.n	.L_0819438c
	.4byte 0x00007900
	.4byte 0x0819a048
	.4byte 0x0819a06c
	.4byte 0x0819a060
	.2byte 0xa066
	.2byte 0x0819
.L_0819438c:
	str	r0, [r2, r3]
	bl	sub_08014878
	movs	r4, #152
	ldr	r2, [r6, #0]
	lsls	r4, r4, #2
	ands	r0, r7
	adds	r3, r5, r4
	negs	r0, r0
	movs	r5, #1
	lsls	r0, r0, #12
	add	sl, r5
	str	r0, [r2, r3]
	mov	r0, sl
	cmp	r0, #6
	bne.n	.L_08194330
	b.n	.L_081943f2
.L_081943ae:
	ldr	r1, [sp, #28]
	ldr	r3, [pc, #300]
	ldr	r6, [sp, #168]
	mov	r4, fp
	ldrh	r2, [r0, r1]
	ldrb	r3, [r3, r4]
	ldr	r5, [r6, #0]
	adds	r3, r2, r3
	cmp	r5, r3
	bge.n	.L_081943f2
	ldr	r3, [pc, #288]
	subs	r5, r5, r2
	ldrb	r1, [r3, r4]
	adds	r0, r5, #0
	bl	sub_08002054
	ldr	r3, [pc, #280]
	mov	r2, fp
	ldrb	r1, [r3, r2]
	ldr	r3, [pc, #276]
	mov	r4, fp
	ldrb	r2, [r3, r2]
	ldr	r3, [pc, #276]
	ldrb	r3, [r3, r4]
	muls	r3, r5
	lsrs	r4, r3, #31
	adds	r3, r3, r4
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [pc, #264]
	mov	r5, fp
	ldrb	r3, [r3, r5]
	bl	sub_08192648
.L_081943f2:
	ldr	r6, [sp, #28]
	movs	r0, #1
	add	fp, r0
	adds	r6, #2
	mov	r1, fp
	str	r6, [sp, #28]
	cmp	r1, #6
	beq.n	.L_08194404
	b.n	.L_08193e9c
.L_08194404:
	ldr	r3, [sp, #168]
	ldr	r4, [pc, #240]
	ldr	r2, [r3, #0]
	adds	r3, r2, r4
	cmp	r3, #132
	bls.n	.L_08194418
	movs	r5, #223
	lsls	r5, r5, #1
	cmp	r2, r5
	ble.n	.L_08194420
.L_08194418:
	add	r6, sp, #420
	mov	r9, r6
	bl	sub_08192894
.L_08194420:
	movs	r0, #188
	bl	sub_0801314c
	ldr	r0, [sp, #168]
	movs	r1, #185
	ldr	r3, [r0, #0]
	lsls	r1, r1, #1
	cmp	r3, r1
	bne.n	.L_081944b8
	ldr	r2, [sp, #64]
	movs	r3, #160
	ldr	r0, [r2, #0]
	lsls	r3, r3, #4
	adds	r3, #232
	movs	r2, #128
	adds	r0, r0, r3
	movs	r1, #16
	lsls	r2, r2, #9
	bl	sub_0815b434
	ldr	r5, [sp, #64]
	movs	r4, #0
	mov	sl, r4
	movs	r7, #0
	movs	r6, #0
.L_08194452:
	ldr	r1, [r5, #0]
	movs	r2, #160
	adds	r4, r7, #0
	lsls	r2, r2, #4
	adds	r0, r1, r4
	adds	r2, #232
	adds	r1, r1, r6
	adds	r0, r0, r2
	subs	r2, #230
	adds	r1, r1, r2
	movs	r3, #128
	movs	r2, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r2, #8
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r2, [r5, #0]
	movs	r3, #0
	adds	r4, r4, r2
	mov	fp, r3
	movs	r0, #160
	adds	r3, r6, r2
	movs	r2, #159
	lsls	r0, r0, #4
	lsls	r2, r2, #4
	adds	r0, #10
	adds	r2, #255
	adds	r1, r3, r0
	adds	r4, r4, r2
.L_08194490:
	ldrb	r3, [r4, #0]
	subs	r4, #1
	strb	r3, [r1, #0]
	movs	r3, #1
	add	fp, r3
	mov	r0, fp
	adds	r1, #1
	cmp	r0, #8
	bne.n	.L_08194490
	add	sl, r3
	mov	r1, sl
	adds	r7, #8
	adds	r6, #16
	cmp	r1, #16
	bne.n	.L_08194452
	movs	r2, #128
	ldr	r3, [pc, #36]
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
.L_081944b8:
	ldr	r2, [sp, #168]
	ldr	r4, [pc, #64]
	ldr	r0, [r2, #0]
	adds	r3, r0, r4
	cmp	r3, #7
	bhi.n	.L_08194504
	ldr	r5, [pc, #56]
	ldr	r2, [pc, #20]
	movs	r1, #128
	adds	r3, r0, r5
	lsls	r1, r1, #19
	adds	r1, #82
	orrs	r3, r2
	strh	r3, [r1, #0]
	b.n	.L_08194504
	movs	r0, r0
	.4byte 0x00001008
	.4byte 0x00001000
	.4byte 0x0819a078
	.4byte 0x0819a07e
	.4byte 0x0819a060
	.4byte 0x0819a066
	.4byte 0x0819a072
	.4byte 0x0819a06c
	.4byte 0xfffffee9
	.4byte 0xfffffe62
	.2byte 0xfe6a
	.2byte 0xffff
.L_08194504:
	movs	r6, #130
	adds	r6, #255
	cmp	r0, r6
	ble.n	.L_08194522
	lsls	r2, r0, #1
	movs	r3, #144
	ldr	r1, [pc, #372]
	adds	r2, r2, r0
	lsls	r3, r3, #4
	lsls	r2, r2, #1
	adds	r3, #252
	subs	r3, r3, r2
	str	r3, [r1, #16]
	ldr	r1, [sp, #168]
	ldr	r0, [r1, #0]
.L_08194522:
	movs	r2, #211
	lsls	r2, r2, #1
	cmp	r0, r2
	ble.n	.L_0819452c
	b.n	.L_0819465a
.L_0819452c:
	movs	r3, #185
	lsls	r3, r3, #1
	cmp	r0, r3
	ble.n	.L_08194548
	ldr	r4, [pc, #340]
	adds	r3, r0, r4
	lsls	r2, r3, #3
	cmp	r2, #127
	ble.n	.L_08194540
	movs	r2, #127
.L_08194540:
	movs	r0, #3
	movs	r1, #34
	bl	sub_0819273c
.L_08194548:
	ldr	r5, [sp, #168]
	movs	r6, #120
	ldr	r3, [r5, #0]
	adds	r6, #255
	cmp	r3, r6
	ble.n	.L_08194580
	ldr	r2, [pc, #312]
	add	r0, sp, #356
	adds	r3, r0, #0
	ldmia	r2!, {r1, r4, r5}
	stmia	r3!, {r1, r4, r5}
	ldmia	r2!, {r1, r6}
	stmia	r3!, {r1, r6}
	ldr	r2, [sp, #168]
	ldr	r4, [pc, #300]
	ldr	r3, [r2, #0]
	adds	r3, r3, r4
	lsls	r2, r3, #3
	cmp	r2, #127
	ble.n	.L_08194572
	movs	r2, #127
.L_08194572:
	asrs	r1, r2, #5
	lsls	r3, r1, #2
	lsls	r1, r1, #1
	ldr	r0, [r0, r3]
	adds	r1, #94
	bl	sub_0819273c
.L_08194580:
	ldr	r5, [sp, #168]
	movs	r6, #190
	ldr	r3, [r5, #0]
	lsls	r6, r6, #1
	cmp	r3, r6
	ble.n	.L_081945c4
	ldr	r3, [pc, #264]
	add	r6, sp, #328
	adds	r2, r6, #0
	ldmia	r3!, {r0, r1, r4}
	stmia	r2!, {r0, r1, r4}
	ldmia	r3!, {r0, r1, r5}
	stmia	r2!, {r0, r1, r5}
	ldr	r3, [r3, #0]
	ldr	r4, [pc, #252]
	str	r3, [r2, #0]
	ldr	r2, [sp, #168]
	ldr	r3, [r2, #0]
	adds	r3, r3, r4
	lsls	r5, r3, #2
	cmp	r5, #127
	ble.n	.L_081945ae
	movs	r5, #127
.L_081945ae:
	movs	r1, #19
	adds	r0, r5, #0
	bl	sub_08002054
	adds	r1, r0, #0
	lsls	r3, r1, #2
	ldr	r0, [r6, r3]
	adds	r1, #12
	adds	r2, r5, #0
	bl	sub_0819273c
.L_081945c4:
	ldr	r5, [sp, #168]
	movs	r6, #191
	ldr	r3, [r5, #0]
	lsls	r6, r6, #1
	cmp	r3, r6
	ble.n	.L_0819460a
	ldr	r3, [pc, #204]
	add	r6, sp, #328
	adds	r2, r6, #0
	ldmia	r3!, {r0, r1, r4}
	stmia	r2!, {r0, r1, r4}
	ldmia	r3!, {r0, r1, r5}
	stmia	r2!, {r0, r1, r5}
	ldr	r3, [r3, #0]
	ldr	r4, [pc, #192]
	str	r3, [r2, #0]
	ldr	r2, [sp, #168]
	ldr	r3, [r2, #0]
	adds	r3, r3, r4
	lsls	r5, r3, #3
	cmp	r5, #127
	ble.n	.L_081945f2
	movs	r5, #127
.L_081945f2:
	movs	r1, #21
	adds	r0, r5, #0
	bl	sub_08002054
	adds	r1, r0, #0
	lsls	r3, r1, #2
	lsls	r1, r1, #1
	ldr	r0, [r6, r3]
	adds	r1, #70
	adds	r2, r5, #0
	bl	sub_0819273c
.L_0819460a:
	ldr	r5, [sp, #168]
	movs	r6, #130
	ldr	r3, [r5, #0]
	adds	r6, #255
	cmp	r3, r6
	ble.n	.L_0819462c
	ldr	r0, [pc, #144]
	adds	r3, r3, r0
	lsls	r2, r3, #2
	cmp	r2, #127
	ble.n	.L_08194622
	movs	r2, #127
.L_08194622:
	asrs	r1, r2, #3
	adds	r1, #48
	movs	r0, #14
	bl	sub_0819273c
.L_0819462c:
	ldr	r1, [sp, #168]
	movs	r2, #211
	ldr	r3, [r1, #0]
	lsls	r2, r2, #1
	cmp	r3, r2
	bne.n	.L_0819465a
	ldr	r4, [sp, #64]
	movs	r6, #238
	ldr	r3, [r4, #0]
	movs	r5, #239
	lsls	r6, r6, #7
	lsls	r5, r5, #7
	adds	r6, #132
	adds	r2, r3, r5
	movs	r1, #0
	adds	r3, r3, r6
	str	r1, [r2, #0]
	str	r1, [r3, #0]
	movs	r2, #128
	ldr	r3, [pc, #48]
	lsls	r2, r2, #19
	adds	r2, #82
	strh	r3, [r2, #0]
.L_0819465a:
	ldr	r0, [sp, #168]
	ldr	r1, [pc, #76]
	ldr	r2, [r0, #0]
	adds	r3, r2, r1
	cmp	r3, #19
	bhi.n	.L_08194706
	ldr	r4, [pc, #72]
	movs	r7, #120
	adds	r3, r2, r4
	lsls	r3, r3, #3
	subs	r3, r7, r3
	mov	sl, r3
	adds	r3, #40
	mov	r7, sl
	cmp	sl, r3
	beq.n	.L_08194706
	mov	r5, sl
	movs	r6, #0
	cmp	r5, #127
	bgt.n	.L_08194706
	b.n	.L_081946b4
	.4byte 0x00001010
	.4byte 0x030011e0
	.4byte 0xfffffe8e
	.4byte 0x08196f50
	.4byte 0xfffffe89
	.4byte 0x08196f64
	.4byte 0xfffffe84
	.4byte 0x08196f80
	.4byte 0xfffffe82
	.4byte 0xfffffe7f
	.4byte 0xfffffe59
	.2byte 0xfe5a
	.2byte 0xffff
.L_081946b4:
	mov	ip, r3
.L_081946b6:
	mov	r0, sl
	cmp	r0, #0
	blt.n	.L_081946f2
	cmp	r6, #63
	bls.n	.L_081946c2
	movs	r6, #63
.L_081946c2:
	mov	r1, sl
	movs	r2, #7
	ands	r2, r1
	asrs	r3, r1, #3
	ldr	r4, [sp, #176]
	lsls	r2, r2, #3
	lsls	r3, r3, #10
	adds	r2, r2, r3
	movs	r5, #0
.L_081946d4:
	movs	r0, #0
.L_081946d6:
	ldr	r3, [r4, #0]
	adds	r1, r3, r2
	ldrb	r3, [r1, #0]
	cmp	r3, r6
	bcs.n	.L_081946e2
	strb	r6, [r1, #0]
.L_081946e2:
	adds	r0, #1
	adds	r2, #1
	cmp	r0, #8
	bne.n	.L_081946d6
	adds	r5, #8
	adds	r2, #56
	cmp	r5, #128
	bne.n	.L_081946d4
.L_081946f2:
	movs	r2, #1
	add	sl, r2
	cmp	sl, ip
	beq.n	.L_08194706
	mov	r4, sl
	subs	r3, r4, r7
	lsls	r3, r3, #25
	lsrs	r6, r3, #24
	cmp	r4, #127
	ble.n	.L_081946b6
.L_08194706:
	ldr	r5, [sp, #168]
	ldr	r6, [pc, #72]
	ldr	r2, [r5, #0]
	adds	r3, r2, r6
	cmp	r3, #30
	bhi.n	.L_08194772
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L_08194772
	ldr	r0, [pc, #60]
	bl	sub_08013300
	movs	r0, #160
	ldr	r4, [pc, #44]
	lsls	r0, r0, #19
	movs	r1, #0
	adds	r0, #192
	mov	sl, r1
.L_0819472c:
	ldrh	r3, [r0, #0]
	movs	r1, #31
	ands	r1, r3
	lsls	r3, r3, #16
	lsrs	r2, r3, #21
	lsrs	r3, r3, #26
	ands	r2, r4
	ands	r3, r4
	cmp	r1, #3
	ble.n	.L_08194742
	subs	r1, #2
.L_08194742:
	cmp	r2, #3
	ble.n	.L_08194748
	subs	r2, #2
.L_08194748:
	cmp	r3, #3
	ble.n	.L_0819475c
	subs	r3, #2
	b.n	.L_0819475c
	.4byte 0x0000001f
	.4byte 0xfffffe6f
	.2byte 0x0045
	.2byte 0x0000
.L_0819475c:
	lsls	r2, r2, #5
	lsls	r3, r3, #10
	orrs	r3, r2
	movs	r2, #1
	orrs	r3, r1
	add	sl, r2
	strh	r3, [r0, #0]
	mov	r3, sl
	adds	r0, #2
	cmp	r3, #128
	bne.n	.L_0819472c
.L_08194772:
	ldr	r4, [sp, #168]
	movs	r5, #223
	ldr	r3, [r4, #0]
	lsls	r5, r5, #1
	cmp	r3, r5
	beq.n	.L_08194780
	b.n	.L_0819496c
.L_08194780:
	ldr	r6, [sp, #176]
	movs	r1, #128
	lsls	r1, r1, #7
	ldr	r2, [pc, #48]
	ldr	r3, [pc, #48]
	ldr	r0, [r6, #0]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0xf680
	.2byte 0xfb28
	.2byte 0x2001
	ldr	r1, [pc, #40]
	movs	r2, #0
	bl	sub_08118040
	movs	r4, #0
	movs	r7, #192
	movs	r0, #160
	mov	sl, r4
	ldr	r4, [pc, #12]
	lsls	r7, r7, #2
	lsls	r0, r0, #19
	adds	r7, #2
	adds	r0, #192
	movs	r5, #31
	b.n	.L_081947c4
	.4byte 0x0000001f
	.4byte 0x3f3f3f3f
	.4byte 0x03000260
	.2byte 0x0077
	.2byte 0x0000
.L_081947c4:
	ldrh	r3, [r0, #0]
	adds	r1, r5, #0
	ands	r1, r3
	lsls	r3, r3, #16
	lsrs	r2, r3, #21
	lsrs	r3, r3, #26
	ands	r2, r4
	ands	r3, r4
	adds	r2, #1
	adds	r3, #1
	lsls	r3, r3, #10
	lsls	r2, r2, #5
	movs	r6, #1
	adds	r1, #1
	orrs	r3, r2
	add	sl, r6
	orrs	r3, r1
	mov	r1, sl
	strh	r3, [r0, #0]
	adds	r0, #2
	cmp	r1, #128
	bne.n	.L_081947c4
	bl	sub_0815b410
	movs	r2, #0
	mov	sl, r2
	movs	r6, #28
.L_081947fa:
	ldr	r3, [sp, #64]
	mov	r5, sl
	ldr	r0, [r3, #0]
	movs	r4, #224
	adds	r5, #14
	adds	r0, r0, r7
	lsls	r4, r4, #3
	movs	r2, #128
	adds	r0, r0, r4
	adds	r1, r5, #0
	lsls	r2, r2, #9
	bl	sub_0815b434
	adds	r3, r6, #0
	muls	r3, r5
	movs	r5, #2
	add	sl, r5
	mov	r0, sl
	adds	r7, r7, r3
	adds	r6, #4
	cmp	r0, #6
	bne.n	.L_081947fa
	ldr	r1, [sp, #64]
	movs	r2, #248
	ldr	r0, [r1, #0]
	lsls	r2, r2, #5
	adds	r2, #16
	movs	r1, #128
	adds	r0, r0, r2
	lsls	r1, r1, #5
	movs	r2, #0
	ldr	r3, [pc, #140]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x24ca
	lsls	r4, r4, #1
	add	r4, sp
	str	r4, [sp, #64]
	movs	r6, #156
	ldr	r0, [r4, #0]
	lsls	r6, r6, #6
	movs	r5, #128
	lsls	r5, r5, #9
	adds	r6, #16
	adds	r3, r5, #0
	adds	r0, r0, r6
	movs	r1, #16
	movs	r2, #64
	bl	sub_08191c20
	ldr	r1, [sp, #64]
	movs	r2, #188
	ldr	r0, [r1, #0]
	lsls	r2, r2, #6
	adds	r2, #16
	adds	r3, r5, #0
	adds	r0, r0, r2
	movs	r1, #16
	movs	r2, #64
	bl	sub_08191c20
	ldr	r4, [sp, #64]
	movs	r5, #0
	ldr	r3, [r4, #0]
	mov	sl, r5
	adds	r2, r3, r6
.L_0819487e:
	movs	r6, #0
	mov	fp, r6
.L_08194882:
	ldrb	r3, [r2, #0]
	cmp	r3, sl
	ble.n	.L_0819488c
	mov	r0, sl
	strb	r0, [r2, #0]
.L_0819488c:
	movs	r1, #1
	add	fp, r1
	mov	r3, fp
	adds	r2, #1
	cmp	r3, #32
	bne.n	.L_08194882
	add	sl, r1
	mov	r4, sl
	cmp	r4, #64
	bne.n	.L_0819487e
	ldr	r6, [sp, #184]
	ldr	r2, [pc, #32]
	ldr	r3, [r6, #0]
	movs	r5, #0
	mov	sl, r5
.L_081948aa:
	movs	r0, #1
	add	sl, r0
	mov	r1, sl
	strh	r2, [r3, #0]
	adds	r3, #2
	cmp	r1, #36
	bne.n	.L_081948aa
	ldr	r3, [sp, #404]
	movs	r2, #0
	mov	sl, r2
	adds	r3, #24
	b.n	.L_081948cc
	movs	r0, r0
	.4byte 0x00000000
	.2byte 0x0260
	.2byte 0x0300
.L_081948cc:
	movs	r4, #1
	add	sl, r4
	mov	r5, sl
	str	r2, [r3, #0]
	adds	r3, #28
	cmp	r5, #32
	bne.n	.L_081948cc
	movs	r1, #192
	movs	r0, #224
	lsls	r1, r1, #2
	add	r6, sp, #404
	lsls	r0, r0, #3
	adds	r1, #2
	mov	r9, r6
	mov	sl, r4
	mov	r8, r0
	movs	r6, #8
	mov	lr, r1
.L_081948f0:
	movs	r2, #0
	mov	fp, r2
	mov	r7, lr
.L_081948f6:
	ldr	r5, [pc, #532]
	mov	r4, fp
	lsls	r3, r4, #1
	mov	r0, r9
	ldrh	r2, [r5, r3]
	ldr	r1, [r0, #0]
	mov	r4, r8
	adds	r3, r1, r2
	adds	r2, r2, r7
	adds	r1, r1, r2
	mov	r2, fp
	adds	r2, #1
	adds	r0, r3, r4
	lsls	r3, r2, #1
	muls	r3, r2
	add	r1, r8
	movs	r5, #0
	cmp	r3, #0
	beq.n	.L_0819493a
	adds	r4, r6, #0
	mov	ip, r3
.L_08194920:
	ldrb	r3, [r0, #0]
	subs	r3, r3, r4
	lsls	r3, r3, #24
	lsrs	r3, r3, #24
	cmp	r3, #63
	bls.n	.L_0819492e
	movs	r3, #0
.L_0819492e:
	adds	r5, #1
	strb	r3, [r1, #0]
	adds	r0, #1
	adds	r1, #1
	cmp	r5, ip
	bne.n	.L_08194920
.L_0819493a:
	mov	fp, r2
	cmp	r2, #10
	bne.n	.L_081948f6
	movs	r5, #192
	movs	r0, #1
	lsls	r5, r5, #2
	add	sl, r0
	adds	r5, #2
	mov	r1, sl
	adds	r6, #8
	add	lr, r5
	cmp	r1, #8
	bne.n	.L_081948f0
	ldr	r2, [sp, #404]
	movs	r3, #239
	movs	r4, #238
	lsls	r3, r3, #7
	lsls	r4, r4, #7
	adds	r1, r2, r3
	adds	r4, #132
	movs	r3, #2
	str	r3, [r1, #0]
	adds	r2, r2, r4
	movs	r3, #70
	str	r3, [r2, #0]
.L_0819496c:
	ldr	r5, [sp, #168]
	ldr	r6, [pc, #416]
	ldr	r2, [r5, #0]
	adds	r3, r2, r6
	cmp	r3, #15
	bhi.n	.L_081949e6
	ldr	r0, [pc, #408]
	adds	r3, r2, r0
	lsls	r3, r3, #3
	movs	r2, #136
	subs	r2, r2, r3
	mov	sl, r2
	mov	ip, sl
	cmp	r2, #0
	beq.n	.L_081949e6
	movs	r7, #0
.L_0819498c:
	mov	r1, sl
	lsrs	r4, r7, #24
	cmp	r1, #127
	bgt.n	.L_081949d4
	cmp	r4, #63
	bls.n	.L_0819499a
	movs	r4, #63
.L_0819499a:
	mov	r3, ip
	subs	r3, #8
	cmp	r3, sl
	bge.n	.L_081949a4
	movs	r4, #0
.L_081949a4:
	mov	r3, sl
	movs	r2, #7
	ands	r2, r3
	ldr	r5, [sp, #176]
	asrs	r3, r3, #3
	lsls	r2, r2, #3
	lsls	r3, r3, #10
	adds	r2, r2, r3
	movs	r6, #0
.L_081949b6:
	movs	r0, #0
.L_081949b8:
	ldr	r3, [r5, #0]
	adds	r1, r3, r2
	ldrb	r3, [r1, #0]
	cmp	r3, r4
	bcs.n	.L_081949c4
	strb	r4, [r1, #0]
.L_081949c4:
	adds	r0, #1
	adds	r2, #1
	cmp	r0, #8
	bne.n	.L_081949b8
	adds	r6, #8
	adds	r2, #56
	cmp	r6, #128
	bne.n	.L_081949b6
.L_081949d4:
	movs	r5, #1
	negs	r5, r5
	movs	r4, #128
	add	sl, r5
	lsls	r4, r4, #18
	mov	r6, sl
	adds	r7, r7, r4
	cmp	r6, #0
	bne.n	.L_0819498c
.L_081949e6:
	ldr	r0, [sp, #168]
	movs	r1, #223
	ldr	r3, [r0, #0]
	lsls	r1, r1, #1
	cmp	r3, r1
	ble.n	.L_081949fe
	add	r2, sp, #420
	mov	r9, r2
	bl	.L_08192a4c
	ldr	r4, [sp, #168]
	ldr	r3, [r4, #0]
.L_081949fe:
	ldr	r5, [pc, #272]
	adds	r3, r3, r5
	cmp	r3, #194
	bls.n	.L_08194a08
	b.n	.L_08194d7a
.L_08194a08:
	movs	r0, #80
	bl	sub_08014dac
	str	r0, [sp, #112]
	movs	r0, #1
	bl	sub_081969f8
	ldr	r2, [pc, #256]
	ldr	r3, [sp, #216]
	mov	fp, r0
	ands	r3, r2
	movs	r2, #5
	orrs	r3, r2
	ldr	r2, [pc, #248]
	mov	r6, fp
	ands	r3, r2
	movs	r2, #192
	lsls	r2, r2, #3
	orrs	r3, r2
	str	r3, [sp, #216]
	bl	sub_08014ee0
	movs	r3, #4
	str	r3, [r6, #0]
	ldr	r3, [pc, #228]
	movs	r2, #154
	str	r3, [r6, #8]
	ldr	r0, [sp, #112]
	lsls	r2, r2, #1
	str	r0, [r6, #12]
	ldr	r1, [sp, #168]
	adds	r2, #255
	ldr	r3, [r1, #0]
	cmp	r3, r2
	ble.n	.L_08194a50
	b.n	.L_08194d6e
.L_08194a50:
	movs	r4, #250
	movs	r2, #128
	lsls	r4, r4, #1
	lsls	r2, r2, #5
	cmp	r3, r4
	ble.n	.L_08194a70
	lsls	r3, r3, #6
	movs	r5, #250
	subs	r3, r2, r3
	lsls	r5, r5, #7
	adds	r3, r3, r5
	lsls	r3, r3, #16
	asrs	r2, r3, #16
	cmp	r2, #0
	bge.n	.L_08194a70
	movs	r2, #0
.L_08194a70:
	ldr	r6, [sp, #64]
	movs	r0, #248
	ldr	r3, [r6, #0]
	lsls	r0, r0, #5
	adds	r3, r3, r2
	adds	r0, #16
	mov	r1, sp
	adds	r3, r3, r0
	adds	r1, #216
	str	r1, [sp, #108]
	str	r3, [r1, #4]
	ldr	r2, [sp, #168]
	ldr	r3, [r2, #0]
	movs	r2, #7
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08194b6a
	movs	r3, #148
	lsls	r3, r3, #1
	add	r3, sp
	mov	sl, r3
	ldr	r3, [pc, #136]
	mov	r2, sl
	ldmia	r3!, {r4, r5, r6}
	stmia	r2!, {r4, r5, r6}
	ldmia	r3!, {r0, r1, r4}
	stmia	r2!, {r0, r1, r4}
	add	r0, sp, #420
	ldmia	r3!, {r5, r6}
	stmia	r2!, {r5, r6}
	mov	r9, r0
	bl	sub_0819284c
	movs	r1, #1
	adds	r5, r0, #0
	negs	r1, r1
	cmp	r5, r1
	beq.n	.L_08194b6a
	bl	sub_08014878
	ldr	r2, [sp, #164]
	adds	r6, r0, #0
	movs	r3, #127
	ldr	r1, [r2, #0]
	ands	r6, r3
	ldr	r3, [pc, #60]
	lsls	r2, r5, #1
	strh	r3, [r1, r2]
	bl	sub_08014878
	ldr	r3, [sp, #164]
	lsls	r7, r5, #2
	adds	r5, r6, #1
	movs	r4, #176
	asrs	r5, r5, #5
	ldr	r2, [r3, #0]
	lsls	r5, r5, #3
	adds	r4, r4, r7
	adds	r3, r5, #4
	mov	r8, r4
	mov	r4, sl
	ldr	r1, [r4, r3]
	str	r2, [sp, #20]
	bl	sub_0800206c
	mov	r1, sl
	ldr	r3, [r1, r5]
	ldr	r2, [sp, #20]
	adds	r3, r3, r0
	movs	r5, #160
	movs	r0, #128
	mov	r4, r8
	lsls	r3, r3, #16
	lsls	r5, r5, #1
	lsls	r0, r0, #13
	b.n	.L_08194b28
	.4byte 0xffff8400
	.4byte 0x08197410
	.4byte 0xfffffe41
	.4byte 0xfffffe42
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0x0819a0d4
	.2byte 0x6f9c
	.2byte 0x0819
.L_08194b28:
	lsls	r6, r6, #16
	str	r3, [r2, r4]
	adds	r6, r6, r0
	adds	r3, r7, r5
	str	r6, [r2, r3]
	bl	sub_08014878
	ldr	r2, [sp, #164]
	movs	r3, #232
	lsls	r3, r3, #1
	ldr	r1, [r2, #0]
	adds	r2, r7, r3
	movs	r3, #15
	ands	r0, r3
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #11
	str	r3, [r1, r2]
	bl	sub_08014878
	movs	r2, #31
	ldr	r5, [sp, #164]
	ands	r0, r2
	lsls	r2, r0, #1
	ldr	r3, [pc, #244]
	ldr	r4, [r5, #0]
	movs	r6, #152
	adds	r2, r2, r0
	lsls	r6, r6, #2
	lsls	r2, r2, #11
	adds	r1, r7, r6
	subs	r3, r3, r2
	str	r3, [r4, r1]
.L_08194b6a:
	ldr	r3, [sp, #108]
	mov	r4, fp
	str	r3, [r4, #16]
	ldr	r0, [pc, #224]
	ldr	r1, [sp, #112]
	movs	r2, #10
	bl	sub_08196958
	mov	r0, fp
	bl	sub_08196a7c
	ldr	r5, [sp, #168]
	movs	r6, #154
	ldr	r3, [r5, #0]
	lsls	r6, r6, #1
	adds	r6, #255
	cmp	r3, r6
	ble.n	.L_08194b90
	b.n	.L_08194d6e
.L_08194b90:
	movs	r0, #235
	movs	r2, #128
	lsls	r0, r0, #1
	lsls	r2, r2, #5
	cmp	r3, r0
	ble.n	.L_08194bb0
	lsls	r3, r3, #6
	movs	r1, #235
	subs	r3, r2, r3
	lsls	r1, r1, #7
	adds	r3, r3, r1
	lsls	r3, r3, #16
	asrs	r2, r3, #16
	cmp	r2, #0
	bge.n	.L_08194bb0
	movs	r2, #0
.L_08194bb0:
	ldr	r4, [sp, #64]
	movs	r5, #248
	ldr	r3, [r4, #0]
	lsls	r5, r5, #5
	ldr	r6, [sp, #108]
	adds	r3, r3, r2
	adds	r5, #16
	adds	r3, r3, r5
	str	r3, [r6, #4]
	ldr	r0, [sp, #168]
	movs	r3, #7
	ldr	r2, [r0, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08194c9e
	movs	r1, #246
	adds	r1, #255
	cmp	r2, r1
	bgt.n	.L_08194c9e
	ldr	r3, [pc, #128]
	movs	r2, #132
	lsls	r2, r2, #1
	add	r2, sp
	mov	sl, r2
	ldmia	r3!, {r4, r5, r6}
	stmia	r2!, {r4, r5, r6}
	ldmia	r3!, {r0, r1, r4}
	stmia	r2!, {r0, r1, r4}
	add	r0, sp, #420
	ldmia	r3!, {r5, r6}
	stmia	r2!, {r5, r6}
	mov	r9, r0
	bl	sub_0819284c
	movs	r1, #1
	adds	r5, r0, #0
	negs	r1, r1
	cmp	r5, r1
	beq.n	.L_08194c9e
	bl	sub_08014878
	ldr	r2, [sp, #164]
	adds	r6, r0, #0
	movs	r3, #127
	ldr	r1, [r2, #0]
	ands	r6, r3
	ldr	r3, [pc, #60]
	lsls	r2, r5, #1
	strh	r3, [r1, r2]
	bl	sub_08014878
	ldr	r3, [sp, #164]
	lsls	r7, r5, #2
	adds	r5, r6, #1
	movs	r4, #176
	asrs	r5, r5, #5
	ldr	r2, [r3, #0]
	lsls	r5, r5, #3
	adds	r4, r4, r7
	adds	r3, r5, #4
	mov	r8, r4
	mov	r4, sl
	ldr	r1, [r4, r3]
	str	r2, [sp, #20]
	bl	sub_0800206c
	mov	r1, sl
	ldr	r3, [r1, r5]
	ldr	r2, [sp, #20]
	adds	r3, r3, r0
	movs	r5, #160
	movs	r0, #128
	mov	r4, r8
	lsls	r3, r3, #16
	lsls	r5, r5, #1
	lsls	r0, r0, #13
	b.n	.L_08194c5c
	movs	r0, r0
	.4byte 0xffff8400
	.4byte 0xffff8000
	.4byte 0x0819a084
	.2byte 0x6fbc
	.2byte 0x0819
.L_08194c5c:
	lsls	r6, r6, #16
	str	r3, [r2, r4]
	adds	r6, r6, r0
	adds	r3, r7, r5
	str	r6, [r2, r3]
	bl	sub_08014878
	ldr	r2, [sp, #164]
	movs	r3, #232
	lsls	r3, r3, #1
	ldr	r1, [r2, #0]
	adds	r2, r7, r3
	movs	r3, #15
	ands	r0, r3
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #11
	str	r3, [r1, r2]
	bl	sub_08014878
	movs	r2, #31
	ldr	r5, [sp, #164]
	ands	r0, r2
	lsls	r2, r0, #1
	ldr	r3, [pc, #96]
	ldr	r4, [r5, #0]
	movs	r6, #152
	adds	r2, r2, r0
	lsls	r6, r6, #2
	lsls	r2, r2, #11
	adds	r1, r7, r6
	subs	r3, r3, r2
	str	r3, [r4, r1]
.L_08194c9e:
	ldr	r4, [sp, #168]
	movs	r2, #3
	ldr	r3, [r4, #0]
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L_08194d58
	add	r5, sp, #420
	mov	r9, r5
	bl	.L_081929fc
	adds	r6, r0, #0
	movs	r0, #1
	negs	r0, r0
	cmp	r6, r0
	beq.n	.L_08194d58
	ldr	r1, [sp, #64]
	lsls	r7, r6, #3
	ldr	r2, [r1, #0]
	subs	r3, r7, r6
	lsls	r5, r3, #2
	movs	r3, #128
	lsls	r3, r3, #14
	str	r3, [r2, r5]
	ldr	r2, [sp, #168]
	movs	r4, #254
	ldr	r3, [r2, #0]
	adds	r4, #255
	cmp	r3, r4
	bgt.n	.L_08194cf4
	bl	sub_08014878
	ldr	r2, [sp, #64]
	ldr	r1, [r2, #0]
	movs	r2, #63
	ldr	r3, [r1, r5]
	ands	r2, r0
	lsls	r2, r2, #16
	adds	r3, r3, r2
	str	r3, [r1, r5]
	b.n	.L_08194d08
	movs	r0, r0
	.2byte 0x8000
	.2byte 0xffff
.L_08194cf4:
	.2byte 0xf67f
	.2byte 0xfdc0
	.2byte 0x9b10
	movs	r4, #31
	ldr	r2, [r3, #0]
	ands	r0, r4
	ldr	r3, [r2, r5]
	lsls	r0, r0, #16
	adds	r3, r3, r0
	str	r3, [r2, r5]
.L_08194d08:
	ldr	r5, [sp, #64]
	subs	r3, r7, r6
	ldr	r1, [r5, #0]
	lsls	r5, r3, #2
	movs	r3, #254
	adds	r2, r5, #4
	lsls	r3, r3, #15
	str	r3, [r1, r2]
	bl	sub_08014878
	ldr	r6, [sp, #64]
	movs	r3, #15
	ldr	r1, [r6, #0]
	ands	r3, r0
	adds	r2, r5, #0
	adds	r2, #12
	lsls	r3, r3, #12
	str	r3, [r1, r2]
	bl	sub_08014878
	movs	r2, #31
	ands	r0, r2
	lsls	r2, r0, #3
	ldr	r3, [pc, #140]
	ldr	r4, [r6, #0]
	subs	r2, r2, r0
	lsls	r2, r2, #11
	adds	r1, r5, #0
	subs	r3, r3, r2
	adds	r1, #16
	str	r3, [r4, r1]
	bl	sub_08014878
	ldr	r1, [r6, #0]
	movs	r3, #3
	adds	r2, r5, #0
	ands	r3, r0
	adds	r2, #24
	adds	r3, #1
	str	r3, [r1, r2]
.L_08194d58:
	ldr	r3, [sp, #108]
	mov	r4, fp
	str	r3, [r4, #16]
	ldr	r0, [pc, #104]
	ldr	r1, [sp, #112]
	movs	r2, #10
	bl	sub_08196958
	mov	r0, fp
	bl	sub_08196a7c
.L_08194d6e:
	mov	r0, fp
	bl	sub_08013164
	ldr	r0, [sp, #112]
	bl	sub_08013164
.L_08194d7a:
	ldr	r5, [sp, #168]
	ldr	r6, [pc, #76]
	ldr	r2, [r5, #0]
	adds	r3, r2, r6
	cmp	r3, #209
	bhi.n	.L_08194da8
	ldr	r3, [pc, #52]
	movs	r0, #252
	subs	r3, r3, r2
	lsls	r3, r3, #16
	lsls	r0, r0, #14
	asrs	r1, r3, #16
	cmp	r3, r0
	bls.n	.L_08194d98
	movs	r1, #63
.L_08194d98:
	ldr	r3, [pc, #52]
	movs	r0, #0
	adds	r2, r2, r3
	lsls	r3, r1, #24
	lsrs	r3, r3, #24
	movs	r1, #127
	bl	sub_08192828
.L_08194da8:
	ldr	r4, [sp, #168]
	ldr	r5, [pc, #40]
	ldr	r0, [r4, #0]
	adds	r3, r0, r5
	cmp	r3, #209
	bhi.n	.L_08194e12
	ldr	r3, [pc, #8]
	movs	r6, #252
	b.n	.L_08194dd8
	movs	r0, r0
	.4byte 0x000002d3
	.4byte 0x000002dc
	.4byte 0xffff8000
	.4byte 0x0819a0ac
	.4byte 0xfffffdff
	.4byte 0xfffffe00
	.2byte 0xfdf6
	.2byte 0xffff
.L_08194dd8:
	subs	r3, r3, r0
	lsls	r3, r3, #16
	lsls	r6, r6, #14
	asrs	r5, r3, #16
	cmp	r3, r6
	bls.n	.L_08194de6
	movs	r5, #63
.L_08194de6:
	ldr	r1, [pc, #116]
	lsls	r0, r0, #16
	adds	r0, r0, r1
	movs	r1, #128
	ldr	r3, [pc, #112]
	lsls	r1, r1, #3
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9b2a
	movs	r2, #246
	adds	r1, r0, #0
	lsls	r2, r2, #15
	adds	r1, r1, r2
	ldr	r4, [pc, #96]
	ldr	r2, [r3, #0]
	lsls	r3, r5, #24
	asrs	r1, r1, #16
	adds	r2, r2, r4
	lsrs	r3, r3, #24
	movs	r0, #0
	bl	sub_08192828
.L_08194e12:
	ldr	r5, [sp, #168]
	ldr	r6, [pc, #80]
	ldr	r0, [r5, #0]
	adds	r3, r0, r6
	cmp	r3, #209
	bhi.n	.L_08194e78
	ldr	r3, [pc, #56]
	movs	r1, #252
	subs	r3, r3, r0
	movs	r6, #128
	lsls	r3, r3, #16
	lsls	r1, r1, #14
	lsls	r6, r6, #11
	asrs	r5, r3, #16
	cmp	r3, r1
	bls.n	.L_08194e34
	movs	r5, #63
.L_08194e34:
	ldr	r2, [pc, #52]
	lsls	r0, r0, #16
	movs	r1, #128
	adds	r0, r0, r2
	ldr	r3, [pc, #32]
	lsls	r1, r1, #3
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9b2a
	ldr	r4, [pc, #40]
	ldr	r2, [r3, #0]
	subs	r6, r6, r0
	lsls	r3, r5, #24
	asrs	r0, r6, #16
	adds	r2, r2, r4
	lsrs	r3, r3, #24
	movs	r1, #127
	b.n	.L_08194e74
	.4byte 0x000002e5
	.4byte 0xfdf70000
	.4byte 0x0300021c
	.4byte 0xfffffdf7
	.4byte 0xfffffded
	.4byte 0xfdee0000
	.2byte 0xfdee
	.2byte 0xffff
.L_08194e74:
	.2byte 0xf7fd
	.2byte 0xfcd8
.L_08194e78:
	.2byte 0x9d2a
	ldr	r6, [pc, #72]
	ldr	r0, [r5, #0]
	adds	r3, r0, r6
	cmp	r3, #209
	bhi.n	.L_08194eda
	ldr	r3, [pc, #56]
	movs	r1, #252
	subs	r3, r3, r0
	lsls	r3, r3, #16
	lsls	r1, r1, #14
	asrs	r5, r3, #16
	cmp	r3, r1
	bls.n	.L_08194e96
	movs	r5, #63
.L_08194e96:
	ldr	r2, [pc, #48]
	lsls	r0, r0, #16
	movs	r1, #192
	adds	r0, r0, r2
	ldr	r3, [pc, #44]
	lsls	r1, r1, #3
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9c2a
	movs	r3, #240
	ldr	r2, [r4, #0]
	ldr	r6, [pc, #32]
	adds	r1, r0, #0
	lsls	r3, r3, #15
	adds	r1, r1, r3
	lsls	r3, r5, #24
	asrs	r1, r1, #16
	adds	r2, r2, r6
	lsrs	r3, r3, #24
	b.n	.L_08194ed4
	movs	r0, r0
	.4byte 0x000002e9
	.4byte 0xfffffde9
	.4byte 0xfdea0000
	.4byte 0x0300021c
	.2byte 0xfdea
	.2byte 0xffff
.L_08194ed4:
	movs	r0, #0
	bl	sub_08192828
.L_08194eda:
	ldr	r1, [sp, #168]
	ldr	r2, [pc, #68]
	ldr	r0, [r1, #0]
	adds	r3, r0, r2
	cmp	r3, #209
	bhi.n	.L_08194f38
	ldr	r3, [pc, #56]
	movs	r4, #252
	subs	r3, r3, r0
	movs	r6, #192
	lsls	r3, r3, #16
	lsls	r4, r4, #14
	lsls	r6, r6, #11
	asrs	r5, r3, #16
	cmp	r3, r4
	bls.n	.L_08194efc
	movs	r5, #63
.L_08194efc:
	ldr	r1, [pc, #40]
	lsls	r0, r0, #16
	adds	r0, r0, r1
	movs	r1, #192
	ldr	r3, [pc, #36]
	lsls	r1, r1, #3
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9b2a
	ldr	r4, [pc, #32]
	ldr	r2, [r3, #0]
	subs	r6, r6, r0
	lsls	r3, r5, #24
	asrs	r0, r6, #16
	adds	r2, r2, r4
	lsrs	r3, r3, #24
	movs	r1, #127
	b.n	.L_08194f34
	.4byte 0x000002fc
	.4byte 0xfffffdd6
	.4byte 0xfdd70000
	.4byte 0x0300021c
	.2byte 0xfdd7
	.2byte 0xffff
.L_08194f34:
	bl	sub_08192828
.L_08194f38:
	ldr	r5, [sp, #168]
	ldr	r6, [pc, #572]
	ldr	r3, [r5, #0]
	adds	r3, r3, r6
	cmp	r3, #110
	bls.n	.L_08194f46
	b.n	.L_08195048
.L_08194f46:
	movs	r5, #240
	lsls	r5, r5, #7
	adds	r0, r5, #0
	bl	sub_08002096
	str	r0, [sp, #104]
	adds	r0, r5, #0
	bl	sub_08002090
	str	r0, [sp, #100]
	movs	r0, #0
	mov	fp, r0
.L_08194f5e:
	ldr	r1, [sp, #168]
	movs	r3, #128
	lsls	r3, r3, #2
	ldr	r2, [r1, #0]
	adds	r3, #126
	add	r3, fp
	cmp	r3, r2
	bge.n	.L_0819503e
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r3, #158
	add	r3, fp
	cmp	r2, r3
	bge.n	.L_0819503e
	ldr	r4, [pc, #512]
	mov	r3, fp
	subs	r1, r2, r3
	adds	r0, r1, r4
	asrs	r5, r0, #1
	str	r5, [sp, #96]
	ldr	r3, [pc, #504]
	adds	r2, r0, #0
	ldrh	r4, [r3, #0]
	cmp	r2, #0
	bge.n	.L_08194f94
	ldr	r6, [pc, #496]
	adds	r2, r1, r6
.L_08194f94:
	asrs	r2, r2, #2
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #7
	adds	r3, r3, r2
	lsls	r3, r3, #1
	adds	r4, r4, r3
	mov	r3, fp
	asrs	r0, r0, #2
	ldr	r2, [pc, #480]
	asrs	r3, r3, #1
	str	r4, [sp, #92]
	str	r0, [sp, #40]
	str	r3, [sp, #88]
	movs	r1, #0
	mov	sl, r1
	mov	r9, r2
.L_08194fb6:
	mov	r4, sl
	lsls	r5, r4, #9
	adds	r0, r5, #0
	bl	sub_08002096
	ldr	r1, [sp, #40]
	adds	r6, r1, #0
	muls	r6, r0
	adds	r0, r5, #0
	bl	sub_08002090
	ldr	r2, [sp, #96]
	mov	r8, r6
	ldr	r1, [sp, #100]
	adds	r6, r2, #0
	muls	r6, r0
	mov	r0, r8
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x0076
	ldr	r1, [sp, #104]
	adds	r5, r0, #0
	adds	r0, r6, #0
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x1a2f
	ldr	r1, [sp, #104]
	mov	r0, r8
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x9919
	adds	r5, r0, #0
	adds	r0, r6, #0
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x9c16
	ldr	r2, [sp, #64]
	asrs	r3, r7, #16
	adds	r3, r3, r4
	ldr	r1, [r2, #0]
	adds	r7, r3, #0
	ldr	r3, [sp, #92]
	ldr	r6, [sp, #176]
	adds	r1, r1, r3
	movs	r3, #1
	adds	r5, r5, r0
	ldr	r0, [r6, #0]
	str	r3, [sp, #0]
	movs	r3, #2
	str	r3, [sp, #4]
	ldr	r6, [sp, #172]
	asrs	r5, r5, #16
	subs	r5, r5, r4
	movs	r4, #224
	lsls	r4, r4, #3
	adds	r7, #64
	adds	r5, #63
	adds	r1, r1, r4
	adds	r2, r7, #0
	ldr	r4, [r6, #0]
	adds	r3, r5, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2001
	add	sl, r0
	mov	r1, sl
	cmp	r1, #128
	bne.n	.L_08194fb6
.L_0819503e:
	movs	r2, #8
	add	fp, r2
	mov	r3, fp
	cmp	r3, #80
	bne.n	.L_08194f5e
.L_08195048:
	ldr	r4, [sp, #168]
	ldr	r5, [pc, #320]
	ldr	r2, [r4, #0]
	adds	r3, r2, r5
	cmp	r3, #2
	bls.n	.L_0819505e
	movs	r6, #192
	lsls	r6, r6, #2
	adds	r6, #13
	cmp	r2, r6
	bne.n	sub_0819506e
.L_0819505e:
	ldr	r1, [sp, #176]
	ldr	r2, [pc, #300]
	ldr	r0, [r1, #0]
	movs	r1, #128
	lsls	r1, r1, #7
	ldr	r3, [pc, #296]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9c2a
	movs	r5, #128
	ldr	r3, [r4, #0]
	lsls	r5, r5, #2
	adds	r5, #242
	cmp	r3, r5
	beq.n	.L_0819507e
	b.n	.L_081951b0
.L_0819507e:
	ldr	r6, [sp, #64]
	movs	r1, #190
	ldr	r0, [r6, #0]
	lsls	r1, r1, #7
	movs	r5, #128
	lsls	r5, r5, #9
	adds	r1, #16
	adds	r3, r5, #0
	adds	r0, r0, r1
	movs	r2, #32
	movs	r1, #16
	bl	sub_08191c20
	ldr	r0, [r6, #0]
	movs	r2, #140
	lsls	r2, r2, #6
	adds	r2, #16
	adds	r0, r0, r2
	movs	r1, #32
	adds	r2, r5, #0
	bl	sub_0815b434
	ldr	r4, [sp, #64]
	movs	r3, #0
	mov	sl, r3
	movs	r5, #0
.L_081950b2:
	ldr	r1, [r4, #0]
	mov	r6, sl
	movs	r2, #140
	lsls	r0, r6, #4
	lsls	r2, r2, #6
	adds	r0, r1, r0
	adds	r2, #16
	movs	r6, #248
	adds	r0, r0, r2
	movs	r3, #128
	lsls	r6, r6, #5
	movs	r2, #132
	adds	r1, r1, r5
	adds	r6, #16
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r1, r1, r6
	adds	r2, #8
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [r4, #0]
	movs	r2, #249
	adds	r3, r5, r3
	lsls	r2, r2, #5
	movs	r0, #0
	adds	r6, #15
	adds	r1, r3, r2
	mov	fp, r0
	adds	r2, r3, r6
.L_081950ee:
	ldrb	r3, [r2, #0]
	movs	r0, #1
	add	fp, r0
	strb	r3, [r1, #0]
	mov	r3, fp
	subs	r2, #1
	adds	r1, #1
	cmp	r3, #16
	bne.n	.L_081950ee
	add	sl, r0
	mov	r6, sl
	adds	r5, #32
	cmp	r6, #32
	bne.n	.L_081950b2
	movs	r6, #136
	movs	r5, #140
	ldr	r4, [sp, #64]
	movs	r0, #0
	lsls	r6, r6, #6
	lsls	r5, r5, #6
	mov	sl, r0
	adds	r6, #240
	adds	r5, #16
.L_0819511c:
	mov	r1, sl
	lsls	r3, r1, #5
	ldr	r1, [r4, #0]
	movs	r2, #132
	subs	r0, r1, r3
	adds	r1, r1, r3
	movs	r3, #128
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, r0, r6
	adds	r1, r1, r5
	adds	r2, #16
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #32
	bne.n	.L_0819511c
	movs	r5, #248
	movs	r0, #248
	ldr	r6, [sp, #64]
	lsls	r5, r5, #5
	movs	r7, #128
	movs	r4, #224
	lsls	r0, r0, #24
	mov	sl, r2
	adds	r5, #16
	lsls	r7, r7, #4
	lsls	r4, r4, #22
	mov	ip, r0
.L_0819515c:
	ldr	r2, [r6, #0]
	mov	r1, sl
	lsls	r3, r1, #11
	adds	r0, r2, r5
	adds	r2, r2, r3
	movs	r3, #0
	adds	r2, r2, r5
	mov	fp, r3
	lsrs	r1, r4, #24
.L_0819516e:
	ldrb	r3, [r0, #0]
	cmp	r1, r3
	bcs.n	.L_08195198
	strb	r1, [r2, #0]
	b.n	.L_0819519a
	.4byte 0xfffffd81
	.4byte 0xfffffd82
	.4byte 0x08197410
	.4byte 0xfffffd85
	.4byte 0x0300021c
	.4byte 0xfffffd0d
	.4byte 0x3f3f3f3f
	.2byte 0x0260
	.2byte 0x0300
.L_08195198:
	strb	r3, [r2, #0]
.L_0819519a:
	movs	r3, #1
	add	fp, r3
	adds	r2, #1
	adds	r0, #1
	cmp	fp, r7
	bne.n	.L_0819516e
	add	sl, r3
	mov	r0, sl
	add	r4, ip
	cmp	r0, #8
	bne.n	.L_0819515c
.L_081951b0:
	ldr	r1, [sp, #168]
	ldr	r2, [pc, #684]
	ldr	r3, [r1, #0]
	adds	r3, r3, r2
	cmp	r3, #30
	bhi.n	.L_08195282
	movs	r5, #176
	lsls	r5, r5, #7
	adds	r0, r5, #0
	bl	sub_08002096
	str	r0, [sp, #84]
	adds	r0, r5, #0
	bl	sub_08002090
	str	r0, [sp, #80]
	ldr	r3, [sp, #168]
	ldr	r4, [pc, #656]
	ldr	r1, [r3, #0]
	ldr	r3, [pc, #656]
	adds	r7, r1, r4
	ldrh	r0, [r3, #10]
	adds	r2, r7, #0
	cmp	r7, #0
	bge.n	.L_081951e6
	ldr	r5, [pc, #648]
	adds	r2, r1, r5
.L_081951e6:
	asrs	r2, r2, #2
	lsls	r3, r2, #1
	adds	r3, r3, r2
	lsls	r3, r3, #7
	adds	r3, r3, r2
	lsls	r3, r3, #1
	adds	r0, r0, r3
	str	r0, [sp, #76]
	ldr	r1, [pc, #632]
	lsls	r0, r7, #1
	str	r0, [sp, #32]
	movs	r6, #0
	mov	sl, r6
	mov	r9, r1
	mov	fp, r6
.L_08195204:
	mov	r0, fp
	bl	sub_08002096
	ldr	r2, [sp, #32]
	adds	r5, r2, #0
	muls	r5, r0
	mov	r0, fp
	bl	sub_08002090
	adds	r3, r7, #0
	muls	r3, r0
	ldr	r1, [sp, #80]
	adds	r0, r5, #0
	mov	r8, r3
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x9915
	adds	r6, r0, #0
	mov	r0, r8
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x9915
	subs	r6, r6, r0
	adds	r0, r5, #0
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x9914
	adds	r5, r0, #0
	mov	r0, r8
	mov	lr, r9
	.2byte 0xf800
	.2byte 0x9a10
	ldr	r3, [sp, #76]
	ldr	r1, [r2, #0]
	ldr	r4, [sp, #176]
	adds	r1, r1, r3
	movs	r3, #6
	adds	r5, r5, r0
	ldr	r0, [r4, #0]
	str	r3, [sp, #0]
	movs	r3, #12
	str	r3, [sp, #4]
	ldr	r2, [sp, #172]
	asrs	r6, r6, #16
	asrs	r5, r5, #16
	movs	r4, #224
	lsls	r4, r4, #3
	adds	r5, #26
	adds	r6, #101
	adds	r3, r5, #0
	adds	r1, r1, r4
	ldr	r4, [r2, #0]
	adds	r2, r6, #0
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x2401
	movs	r3, #192
	add	sl, r4
	lsls	r3, r3, #1
	mov	r5, sl
	add	fp, r3
	cmp	r5, #96
	bne.n	.L_08195204
.L_08195282:
	ldr	r6, [sp, #168]
	ldr	r0, [pc, #492]
	ldr	r3, [r6, #0]
	adds	r3, r3, r0
	cmp	r3, #46
	bhi.n	.L_08195352
	movs	r0, #232
	lsls	r0, r0, #8
	bl	sub_08002096
	str	r0, [sp, #72]
	movs	r0, #224
	lsls	r0, r0, #8
	bl	sub_08002090
	ldr	r5, [r6, #0]
	ldr	r1, [pc, #468]
	ldr	r3, [pc, #448]
	adds	r5, r5, r1
	mov	fp, r0
	movs	r1, #6
	adds	r0, r5, #0
	ldrh	r6, [r3, #10]
	bl	sub_08002054
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #7
	adds	r3, r3, r0
	lsls	r3, r3, #1
	adds	r6, r6, r3
	asrs	r3, r5, #1
	lsls	r5, r5, #1
	str	r6, [sp, #68]
	str	r3, [sp, #56]
	str	r5, [sp, #60]
	ldr	r7, [pc, #420]
	movs	r2, #0
	mov	sl, r2
	mov	r9, r2
.L_081952d2:
	mov	r0, r9
	bl	sub_08002096
	ldr	r4, [sp, #56]
	adds	r5, r4, #0
	muls	r5, r0
	mov	r0, r9
	bl	sub_08002090
	ldr	r1, [sp, #60]
	adds	r6, r1, #0
	muls	r6, r0
	adds	r0, r5, #0
	mov	r1, fp
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x46b0
	ldr	r1, [sp, #72]
	adds	r6, r0, #0
	mov	r0, r8
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x9912
	subs	r6, r6, r0
	adds	r0, r5, #0
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x4659
	adds	r5, r0, #0
	mov	r0, r8
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x9b10
	ldr	r2, [sp, #176]
	ldr	r1, [r3, #0]
	movs	r3, #6
	ldr	r4, [sp, #68]
	adds	r5, r5, r0
	ldr	r0, [r2, #0]
	str	r3, [sp, #0]
	movs	r3, #12
	str	r3, [sp, #4]
	ldr	r3, [sp, #172]
	asrs	r6, r6, #16
	asrs	r5, r5, #16
	movs	r2, #224
	adds	r1, r1, r4
	lsls	r2, r2, #3
	adds	r6, #93
	adds	r5, #32
	ldr	r4, [r3, #0]
	adds	r1, r1, r2
	adds	r3, r5, #0
	adds	r2, r6, #0
	movs	r5, #1
	mov	lr, r4
	.2byte 0xf800
	.2byte 0x44aa
	movs	r4, #192
	lsls	r4, r4, #1
	mov	r6, sl
	add	r9, r4
	cmp	r6, #197
	bne.n	.L_081952d2
.L_08195352:
	ldr	r0, [sp, #168]
	movs	r1, #128
	ldr	r3, [r0, #0]
	lsls	r1, r1, #2
	adds	r1, #246
	cmp	r3, r1
	bgt.n	.L_08195362
	b.n	.L_081955e6
.L_08195362:
	movs	r0, #64
	bl	sub_08014dac
	adds	r6, r0, #0
	movs	r0, #1
	bl	sub_081969f8
	ldr	r2, [pc, #264]
	ldr	r3, [sp, #208]
	movs	r4, #190
	ands	r3, r2
	movs	r2, #5
	orrs	r3, r2
	ldr	r2, [pc, #256]
	lsls	r4, r4, #7
	ands	r3, r2
	movs	r2, #160
	lsls	r2, r2, #3
	orrs	r3, r2
	ldr	r2, [sp, #64]
	str	r3, [sp, #208]
	ldr	r3, [r2, #0]
	adds	r4, #16
	adds	r3, r3, r4
	add	r7, sp, #208
	str	r3, [r7, #4]
	adds	r5, r0, #0
	bl	sub_08014ee0
	movs	r3, #128
	add	r0, sp, #252
	lsls	r3, r3, #14
	str	r3, [r0, #0]
	ldr	r3, [pc, #220]
	movs	r1, #0
	str	r3, [r0, #4]
	str	r1, [r0, #8]
	mov	r8, r1
	bl	sub_08015128
	movs	r0, #152
	lsls	r0, r0, #8
	bl	sub_080150e4
	movs	r2, #128
	add	r0, sp, #240
	lsls	r2, r2, #9
	str	r2, [r0, #0]
	ldr	r4, [sp, #168]
	ldr	r1, [pc, #156]
	ldr	r3, [r4, #0]
	adds	r3, r3, r1
	lsls	r3, r3, #10
	str	r3, [r0, #4]
	cmp	r3, r2
	ble.n	.L_081953d4
	str	r2, [r0, #4]
.L_081953d4:
	mov	r2, r8
	str	r2, [r0, #8]
	bl	sub_080151ac
	movs	r3, #4
	str	r3, [r5, #0]
	ldr	r3, [pc, #164]
	adds	r1, r6, #0
	str	r3, [r5, #8]
	movs	r2, #8
	str	r7, [r5, #16]
	str	r6, [r5, #12]
	ldr	r0, [pc, #156]
	bl	sub_08196958
	adds	r0, r5, #0
	bl	sub_08196a7c
	adds	r0, r5, #0
	bl	sub_08013164
	adds	r0, r6, #0
	bl	sub_08013164
	add	r3, sp, #420
	mov	r9, r3
	bl	sub_0819284c
	movs	r4, #1
	negs	r4, r4
	adds	r5, r0, #0
	mov	sl, r4
	cmp	r5, sl
	beq.n	.L_081954ac
	bl	sub_08014878
	ldr	r6, [sp, #164]
	ldr	r3, [pc, #60]
	ldr	r1, [r6, #0]
	ands	r0, r3
	lsls	r2, r5, #1
	lsls	r0, r0, #15
	lsls	r5, r5, #2
	strh	r0, [r1, r2]
	movs	r3, #239
	adds	r2, r5, #0
	adds	r2, #176
	lsls	r3, r3, #16
	str	r3, [r1, r2]
	bl	sub_08014878
	movs	r3, #160
	lsls	r3, r3, #1
	ldr	r1, [r6, #0]
	adds	r2, r5, r3
	movs	r3, #255
	ands	r3, r0
	lsls	r3, r3, #16
	str	r3, [r1, r2]
	bl	sub_08014878
	ldr	r2, [r6, #0]
	movs	r6, #31
	movs	r4, #232
	ands	r0, r6
	lsls	r4, r4, #1
	negs	r0, r0
	b.n	.L_08195490
	.4byte 0x0000000f
	.4byte 0xfffffd09
	.4byte 0xfffffd0a
	.4byte 0x08197410
	.4byte 0xfffffd0d
	.4byte 0x0300021c
	.4byte 0xfffffcf1
	.4byte 0xfffffcf2
	.4byte 0xffffff00
	.4byte 0xffff00ff
	.4byte 0xffe80000
	.4byte 0x0819a160
	.2byte 0xa140
	.2byte 0x0819
.L_08195490:
	adds	r3, r5, r4
	lsls	r0, r0, #12
	str	r0, [r2, r3]
	bl	sub_08014878
	ldr	r1, [sp, #164]
	movs	r4, #152
	ldr	r2, [r1, #0]
	ands	r0, r6
	lsls	r4, r4, #2
	subs	r0, #16
	adds	r3, r5, r4
	lsls	r0, r0, #12
	str	r0, [r2, r3]
.L_081954ac:
	ldr	r5, [sp, #168]
	movs	r6, #3
	ldr	r3, [r5, #0]
	mov	r8, r6
	ands	r3, r6
	cmp	r3, #0
	beq.n	.L_081954bc
	b.n	.L_081955e6
.L_081954bc:
	add	r0, sp, #420
	mov	r9, r0
	bl	.L_081929fc
	adds	r6, r0, #0
	cmp	r6, sl
	beq.n	.L_08195556
	bl	sub_08014878
	ldr	r3, [sp, #64]
	mov	r1, r8
	ldr	r2, [r3, #0]
	lsls	r3, r6, #3
	subs	r3, r3, r6
	lsls	r7, r3, #2
	movs	r3, #192
	lsls	r3, r3, #15
	str	r3, [r2, r7]
	adds	r5, r0, #0
	ands	r5, r1
	bl	sub_08014878
	ldr	r4, [sp, #64]
	movs	r3, #7
	ldr	r1, [r4, #0]
	ands	r3, r0
	movs	r6, #144
	lsls	r6, r6, #14
	lsls	r3, r3, #16
	adds	r3, r3, r6
	adds	r2, r7, #4
	str	r3, [r1, r2]
	ldr	r0, [sp, #168]
	adds	r2, #4
	ldr	r3, [r0, #0]
	movs	r0, #152
	str	r3, [r1, r2]
	lsls	r0, r0, #8
	bl	sub_08002096
	adds	r5, #2
	lsls	r5, r5, #15
	adds	r1, r0, #0
	ldr	r6, [pc, #248]
	adds	r0, r5, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x9910
	adds	r3, r7, #0
	ldr	r2, [r1, #0]
	adds	r3, #12
	str	r0, [r2, r3]
	movs	r0, #152
	lsls	r0, r0, #8
	bl	sub_08002090
	adds	r1, r0, #0
	adds	r0, r5, #0
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x9b10
	negs	r0, r0
	ldr	r2, [r3, #0]
	adds	r3, r7, #0
	adds	r3, #16
	str	r0, [r2, r3]
	bl	sub_08014878
	ldr	r4, [sp, #64]
	mov	r5, r8
	ldr	r1, [r4, #0]
	adds	r2, r7, #0
	ands	r0, r5
	movs	r3, #16
	adds	r2, #24
	orrs	r3, r0
	str	r3, [r1, r2]
.L_08195556:
	add	r6, sp, #420
	mov	r9, r6
	bl	.L_081929fc
	adds	r6, r0, #0
	cmp	r6, sl
	beq.n	.L_081955e6
	bl	sub_08014878
	lsls	r3, r6, #3
	ldr	r1, [sp, #64]
	subs	r3, r3, r6
	lsls	r6, r3, #2
	movs	r3, #31
	ldr	r2, [r1, #0]
	ands	r0, r3
	movs	r4, #192
	lsls	r4, r4, #15
	lsls	r0, r0, #16
	adds	r0, r0, r4
	str	r0, [r2, r6]
	bl	sub_08014878
	ldr	r5, [sp, #64]
	movs	r2, #31
	ldr	r1, [r5, #0]
	ands	r0, r2
	adds	r3, r6, #4
	lsls	r0, r0, #16
	str	r0, [r1, r3]
	ldr	r4, [sp, #168]
	adds	r2, r6, #0
	ldr	r3, [r4, #0]
	adds	r2, #8
	str	r3, [r1, r2]
	bl	sub_08014878
	ldr	r2, [r5, #0]
	movs	r5, #15
	ands	r0, r5
	adds	r3, r6, #0
	lsls	r0, r0, #13
	adds	r3, #12
	negs	r0, r0
	str	r0, [r2, r3]
	bl	sub_08014878
	ldr	r2, [sp, #64]
	adds	r3, r6, #0
	ldr	r1, [r2, #0]
	ands	r0, r5
	adds	r3, #16
	lsls	r0, r0, #14
	str	r0, [r1, r3]
	adds	r2, r6, #0
	movs	r3, #128
	adds	r2, #20
	lsls	r3, r3, #8
	str	r3, [r1, r2]
	bl	sub_08014878
	ldr	r3, [sp, #64]
	mov	r4, r8
	ldr	r1, [r3, #0]
	ands	r0, r4
	movs	r3, #136
	adds	r2, r6, #0
	adds	r0, #1
	lsls	r3, r3, #1
	adds	r2, #24
	orrs	r0, r3
	str	r0, [r1, r2]
.L_081955e6:
	ldr	r5, [sp, #168]
	movs	r6, #192
	ldr	r1, [r5, #0]
	lsls	r6, r6, #2
	adds	r6, #14
	cmp	r1, r6
	ble.n	.L_08195622
	movs	r3, #2
	ands	r3, r1
	cmp	r3, #0
	beq.n	.L_08195614
	ldr	r2, [pc, #16]
	movs	r3, #1
	ands	r1, r3
	ldrh	r3, [r2, #6]
	adds	r3, r3, r1
	adds	r3, #2
	b.n	.L_08195620
	movs	r0, r0
	.4byte 0x0300021c
	.2byte 0x1120
	.2byte 0x0300
.L_08195614:
	ldr	r2, [pc, #76]
	movs	r3, #1
	ands	r1, r3
	ldrh	r3, [r2, #6]
	subs	r3, r3, r1
	subs	r3, #2
.L_08195620:
	strh	r3, [r2, #6]
.L_08195622:
	ldr	r0, [sp, #168]
	movs	r1, #204
	ldr	r3, [r0, #0]
	lsls	r1, r1, #2
	cmp	r3, r1
	ble.n	.L_0819567e
	movs	r5, #160
	ldr	r6, [pc, #44]
	lsls	r5, r5, #19
	movs	r2, #0
	adds	r5, #192
	mov	sl, r2
.L_0819563a:
	ldrh	r2, [r5, #0]
	movs	r0, #31
	lsls	r3, r2, #16
	lsrs	r4, r3, #26
	lsrs	r1, r3, #21
	ands	r0, r2
	ands	r4, r6
	ands	r1, r6
	cmp	r0, #30
	bgt.n	.L_08195650
	adds	r0, #1
.L_08195650:
	cmp	r1, #30
	bgt.n	.L_08195656
	adds	r1, #1
.L_08195656:
	cmp	r4, #30
	bgt.n	.L_08195668
	adds	r4, #1
	b.n	.L_08195668
	movs	r0, r0
	.4byte 0x0000001f
	.2byte 0x1120
	.2byte 0x0300
.L_08195668:
	lsls	r3, r4, #10
	lsls	r2, r1, #5
	orrs	r3, r2
	orrs	r3, r0
	strh	r3, [r5, #0]
	movs	r3, #1
	add	sl, r3
	mov	r4, sl
	adds	r5, #2
	cmp	r4, #128
	bne.n	.L_0819563a
.L_0819567e:
	ldr	r5, [sp, #168]
	movs	r0, #240
	ldr	r3, [r5, #0]
	lsls	r0, r0, #7
	adds	r3, #1
	str	r3, [r5, #0]
	ldr	r6, [sp, #64]
	adds	r0, #232
	ldr	r3, [r6, #0]
	movs	r2, #1
	adds	r3, r3, r0
	str	r2, [r3, #0]
	movs	r0, #1
	bl	sub_08013560
	bl	.L_08192ec8
.L_081956a0:
	add	r1, sp, #188
	ldrh	r1, [r1, #0]
	ldr	r2, [sp, #192]
	movs	r0, #104
	strh	r1, [r2, #54]
	bl	sub_0801314c
	bl	sub_08191cc4
	ldr	r2, [sp, #176]
	movs	r1, #128
	ldr	r0, [r2, #0]
	ldr	r3, [pc, #84]
	lsls	r1, r1, #7
	movs	r2, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x2280
	lsls	r2, r2, #19
	movs	r3, #128
	adds	r2, #212
	lsls	r3, r3, #24
.L_081956cc:
	ldr	r5, [r2, #8]
	ands	r5, r3
	cmp	r5, #0
	bne.n	.L_081956cc
	ldr	r4, [sp, #64]
	movs	r6, #240
	ldr	r3, [r4, #0]
	lsls	r6, r6, #7
	adds	r6, #232
	adds	r3, r3, r6
	movs	r2, #1
	str	r2, [r3, #0]
	movs	r0, #1
	bl	sub_08013560
	ldr	r3, [pc, #32]
	movs	r2, #128
	lsls	r2, r2, #19
	adds	r2, #32
	strh	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #19
	adds	r3, #40
	add	r0, sp, #196
	str	r5, [r3, #0]
	ldrh	r0, [r0, #0]
	ldr	r3, [pc, #16]
	movs	r1, #0
	strh	r0, [r3, #4]
	mov	sl, r1
	b.n	.L_08195718
	movs	r0, r0
	.4byte 0x00000080
	.4byte 0x03000260
	.2byte 0x1120
	.2byte 0x0300
.L_08195718:
	ldr	r4, [sp, #180]
	mov	r2, sl
	lsls	r3, r2, #2
	ldr	r2, [r4, #0]
	movs	r5, #1
	add	sl, r5
	ldr	r0, [r3, r2]
	mov	r6, sl
	bl	sub_08020048
	cmp	r6, #36
	bne.n	.L_08195718
	ldr	r0, [pc, #20]
	bl	sub_08014644
	bl	sub_08143bb8
	add	sp, #420
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.2byte 0x3001
	.2byte 0x0814
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #56
	str	r1, [sp, #52]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #96]
	adds	r6, r0, #0
	str	r1, [sp, #48]
	ldr	r2, [r3, #100]
	str	r2, [sp, #44]
	ldr	r3, [r3, #92]
	mov	r8, r3
	lsls	r3, r6, #1
	adds	r6, r3, r6
	movs	r3, #0
	str	r3, [sp, #40]
	cmp	r6, #0
	beq.n	.L_08195794
	ldr	r7, [pc, #784]
	adds	r5, r2, #0
.L_08195780:
	adds	r1, r5, #0
	adds	r0, r5, #0
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x990a
	adds	r5, #12
	adds	r1, #3
	str	r1, [sp, #40]
	cmp	r1, r6
	bne.n	.L_08195780
.L_08195794:
	ldr	r3, [sp, #52]
	movs	r2, #0
	str	r2, [sp, #40]
	cmp	r3, #0
	bne.n	.L_081957a0
	b.n	.L_08195a82
.L_081957a0:
	ldr	r5, [sp, #44]
	lsls	r3, r6, #2
	adds	r6, r3, r5
	movs	r1, #12
	adds	r2, r6, #0
	adds	r1, r1, r6
	adds	r2, #24
	adds	r3, r6, #0
	mov	sl, r1
	str	r2, [sp, #36]
	adds	r3, #28
	mov	r1, r8
	subs	r2, #8
	str	r3, [sp, #32]
	str	r1, [sp, #8]
	str	r2, [sp, #28]
	adds	r5, r6, #4
	mov	fp, r5
.L_081957c4:
	ldr	r3, [sp, #8]
	ldr	r5, [sp, #44]
	ldr	r0, [r3, #0]
	movs	r2, #132
	movs	r3, #128
	lsls	r0, r0, #2
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, r0, r5
	adds	r1, r6, #0
	adds	r2, #3
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r1, [sp, #8]
	movs	r2, #132
	ldr	r0, [r1, #4]
	lsls	r2, r2, #24
	lsls	r0, r0, #2
	adds	r0, r0, r5
	mov	r1, sl
	adds	r2, #3
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r2, [sp, #8]
	ldr	r1, [sp, #36]
	ldr	r0, [r2, #8]
	movs	r2, #132
	lsls	r0, r0, #2
	lsls	r2, r2, #24
	adds	r0, r0, r5
	adds	r2, #3
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r3, sl
	ldr	r5, [sp, #32]
	ldr	r0, [r3, #0]
	ldr	r3, [r6, #0]
	mov	r2, fp
	ldr	r1, [r5, #0]
	subs	r0, r0, r3
	ldr	r3, [r2, #0]
	subs	r1, r1, r3
	ldr	r3, [pc, #632]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9909
	ldr	r2, [sp, #28]
	ldr	r3, [r6, #0]
	adds	r5, r0, #0
	ldr	r0, [r1, #0]
	ldr	r1, [r2, #0]
	mov	r2, fp
	subs	r0, r0, r3
	ldr	r3, [r2, #0]
	subs	r1, r1, r3
	ldr	r3, [pc, #604]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1a2d
	cmp	r5, #0
	bge.n	.L_08195842
	b.n	.L_08195a6e
.L_08195842:
	ldr	r5, [sp, #8]
	movs	r2, #128
	ldrb	r5, [r5, #24]
	lsls	r2, r2, #15
	str	r5, [sp, #12]
	mov	r1, fp
	ldr	r3, [r6, #0]
	mov	r5, sl
	adds	r3, r3, r2
	str	r3, [r6, #0]
	ldr	r3, [r1, #0]
	adds	r3, r3, r2
	str	r3, [r1, #0]
	ldr	r3, [r5, #0]
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r1, [sp, #28]
	ldr	r3, [r1, #0]
	adds	r3, r3, r2
	str	r3, [r1, #0]
	ldr	r5, [sp, #36]
	ldr	r3, [r5, #0]
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r1, [sp, #32]
	ldr	r3, [r1, #0]
	adds	r3, r3, r2
	str	r3, [r1, #0]
	ldr	r5, [sp, #28]
	mov	r2, fp
	ldr	r3, [r2, #0]
	ldr	r2, [r5, #0]
	cmp	r3, r2
	ble.n	.L_081958c0
	mov	r2, sl
	ldr	r3, [r2, #0]
	ldr	r1, [r6, #0]
	mov	r5, fp
	eors	r1, r3
	str	r1, [r6, #0]
	mov	r3, sl
	ldr	r2, [r2, #0]
	eors	r2, r1
	str	r2, [r3, #0]
	ldr	r3, [r6, #0]
	eors	r3, r2
	str	r3, [r6, #0]
	ldr	r2, [sp, #28]
	ldr	r1, [r5, #0]
	ldr	r3, [r2, #0]
	eors	r1, r3
	str	r1, [r5, #0]
	ldr	r3, [sp, #28]
	mov	r5, sl
	ldr	r2, [r3, #0]
	eors	r2, r1
	str	r2, [r5, #4]
	mov	r1, fp
	ldr	r3, [r1, #0]
	eors	r3, r2
	str	r3, [r1, #0]
	ldr	r3, [sp, #28]
	ldr	r2, [r3, #0]
.L_081958c0:
	ldr	r5, [sp, #32]
	ldr	r3, [r5, #0]
	cmp	r2, r3
	ble.n	.L_0819593c
	ldr	r5, [sp, #36]
	mov	r2, sl
	ldr	r3, [r5, #0]
	ldr	r1, [r2, #0]
	eors	r1, r3
	str	r1, [r2, #0]
	ldr	r2, [r5, #0]
	eors	r2, r1
	str	r2, [r5, #0]
	mov	r1, sl
	ldr	r3, [r1, #0]
	eors	r3, r2
	str	r3, [r1, #0]
	ldr	r5, [sp, #32]
	ldr	r3, [sp, #28]
	ldr	r2, [r3, #0]
	ldr	r3, [r5, #0]
	eors	r2, r3
	str	r2, [r1, #4]
	ldr	r1, [sp, #36]
	ldr	r3, [r5, #0]
	eors	r3, r2
	str	r3, [r1, #4]
	ldr	r5, [sp, #28]
	mov	r1, fp
	ldr	r2, [r5, #0]
	eors	r2, r3
	str	r2, [r5, #0]
	ldr	r3, [r1, #0]
	cmp	r3, r2
	ble.n	.L_0819593c
	mov	r2, sl
	ldr	r3, [r2, #0]
	ldr	r1, [r6, #0]
	mov	r5, fp
	eors	r1, r3
	str	r1, [r6, #0]
	mov	r3, sl
	ldr	r2, [r2, #0]
	eors	r2, r1
	str	r2, [r3, #0]
	ldr	r3, [r6, #0]
	eors	r3, r2
	str	r3, [r6, #0]
	ldr	r2, [sp, #28]
	ldr	r1, [r5, #0]
	ldr	r3, [r2, #0]
	eors	r1, r3
	str	r1, [r5, #0]
	ldr	r3, [sp, #28]
	mov	r5, sl
	ldr	r2, [r3, #0]
	eors	r2, r1
	str	r2, [r5, #4]
	mov	r1, fp
	ldr	r3, [r1, #0]
	eors	r3, r2
	str	r3, [r1, #0]
.L_0819593c:
	ldr	r3, [r6, #4]
	ldr	r0, [r6, #16]
	asrs	r2, r3, #16
	mov	r8, r2
	movs	r2, #30
	ldrsh	r1, [r6, r2]
	asrs	r5, r0, #16
	mov	r9, r5
	str	r1, [sp, #16]
	cmp	r8, r9
	beq.n	.L_08195962
	subs	r0, r0, r3
	ldr	r1, [r6, #12]
	ldr	r3, [r6, #0]
	subs	r1, r1, r3
	ldr	r3, [pc, #316]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9006
.L_08195962:
	ldr	r3, [sp, #16]
	cmp	r8, r3
	beq.n	.L_0819597c
	ldr	r3, [r6, #4]
	ldr	r0, [r6, #28]
	ldr	r1, [r6, #24]
	subs	r0, r0, r3
	ldr	r3, [r6, #0]
	subs	r1, r1, r3
	ldr	r3, [pc, #288]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9005
.L_0819597c:
	ldr	r7, [r6, #0]
	mov	r4, r8
	str	r7, [sp, #0]
	cmp	r4, r9
	beq.n	.L_081959e6
.L_08195986:
	ldr	r5, [sp, #0]
	asrs	r0, r7, #16
	asrs	r1, r5, #16
	cmp	r1, r0
	ble.n	.L_08195996
	eors	r1, r0
	eors	r0, r1
	eors	r1, r0
.L_08195996:
	movs	r2, #7
	ands	r2, r4
	asrs	r3, r4, #3
	lsls	r2, r2, #3
	lsls	r3, r3, #10
	adds	r2, r2, r3
	mov	ip, r2
	cmp	r1, r0
	beq.n	.L_081959d4
	movs	r2, #7
	mov	lr, r2
.L_081959ac:
	asrs	r3, r1, #3
	lsls	r3, r3, #6
	mov	r8, r3
	adds	r2, r1, #0
	mov	r3, lr
	ands	r2, r3
	ldr	r5, [sp, #48]
	add	r2, r8
	add	r2, ip
	mov	r8, r2
	ldrb	r5, [r5, r2]
	ldr	r2, [sp, #12]
	cmp	r5, r2
	bcs.n	.L_081959ce
	ldr	r3, [sp, #48]
	mov	r5, r8
	strb	r2, [r3, r5]
.L_081959ce:
	adds	r1, #1
	cmp	r1, r0
	bne.n	.L_081959ac
.L_081959d4:
	ldr	r5, [sp, #0]
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #20]
	adds	r5, r5, r1
	adds	r4, #1
	str	r5, [sp, #0]
	adds	r7, r7, r2
	cmp	r4, r9
	bne.n	.L_08195986
.L_081959e6:
	ldr	r3, [sp, #16]
	cmp	r9, r3
	beq.n	.L_08195a00
	ldr	r3, [r6, #16]
	ldr	r0, [r6, #28]
	ldr	r1, [r6, #24]
	subs	r0, r0, r3
	ldr	r3, [r6, #12]
	subs	r1, r1, r3
	ldr	r3, [pc, #156]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9006
.L_08195a00:
	ldr	r5, [r6, #12]
	ldr	r1, [sp, #16]
	mov	r4, r9
	str	r5, [sp, #0]
	cmp	r4, r1
	beq.n	.L_08195a6e
.L_08195a0c:
	ldr	r2, [sp, #0]
	asrs	r0, r7, #16
	asrs	r1, r2, #16
	cmp	r1, r0
	ble.n	.L_08195a1c
	eors	r1, r0
	eors	r0, r1
	eors	r1, r0
.L_08195a1c:
	movs	r2, #7
	ands	r2, r4
	asrs	r3, r4, #3
	lsls	r2, r2, #3
	lsls	r3, r3, #10
	adds	r2, r2, r3
	mov	ip, r2
	cmp	r1, r0
	beq.n	.L_08195a5a
	movs	r3, #7
	mov	lr, r3
.L_08195a32:
	asrs	r3, r1, #3
	lsls	r3, r3, #6
	mov	r5, lr
	adds	r2, r1, #0
	mov	r8, r3
	ands	r2, r5
	ldr	r3, [sp, #48]
	add	r2, r8
	add	r2, ip
	ldrb	r3, [r3, r2]
	ldr	r5, [sp, #12]
	mov	r9, r3
	mov	r8, r2
	cmp	r9, r5
	bcs.n	.L_08195a54
	ldr	r3, [sp, #48]
	strb	r5, [r3, r2]
.L_08195a54:
	adds	r1, #1
	cmp	r1, r0
	bne.n	.L_08195a32
.L_08195a5a:
	ldr	r5, [sp, #0]
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #20]
	ldr	r3, [sp, #16]
	adds	r5, r5, r1
	adds	r4, #1
	str	r5, [sp, #0]
	adds	r7, r7, r2
	cmp	r4, r3
	bne.n	.L_08195a0c
.L_08195a6e:
	ldr	r5, [sp, #8]
	ldr	r1, [sp, #40]
	ldr	r2, [sp, #52]
	adds	r5, #28
	adds	r1, #1
	str	r5, [sp, #8]
	str	r1, [sp, #40]
	cmp	r1, r2
	beq.n	.L_08195a82
	b.n	.L_081957c4
.L_08195a82:
	add	sp, #56
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x03000354
	.4byte 0x0300021c
	.2byte 0x0230
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #144
	str	r3, [sp, #108]
	str	r1, [sp, #116]
	str	r2, [sp, #112]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #100]
	adds	r6, r0, #0
	str	r1, [sp, #100]
	ldr	r2, [r3, #92]
	ldr	r3, [r3, #96]
	mov	r8, r2
	str	r3, [sp, #96]
	lsls	r3, r6, #1
	adds	r6, r3, r6
	movs	r3, #0
	str	r3, [sp, #104]
	cmp	r6, #0
	beq.n	.L_08195ae8
	ldr	r7, [pc, #920]
	adds	r5, r1, #0
.L_08195ad4:
	adds	r1, r5, #0
	adds	r0, r5, #0
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x991a
	adds	r5, #12
	adds	r1, #3
	str	r1, [sp, #104]
	cmp	r1, r6
	bne.n	.L_08195ad4
.L_08195ae8:
	ldr	r3, [sp, #116]
	movs	r2, #0
	str	r2, [sp, #104]
	cmp	r3, #0
	bne.n	.L_08195af4
	b.n	.L_08196230
.L_08195af4:
	ldr	r1, [sp, #100]
	lsls	r3, r6, #2
	adds	r6, r3, r1
	adds	r2, r6, #0
	adds	r3, r6, #0
	adds	r1, r6, #0
	adds	r2, #12
	adds	r3, #24
	adds	r1, #28
	str	r2, [sp, #92]
	str	r3, [sp, #88]
	str	r1, [sp, #84]
	mov	r2, r8
	adds	r3, r6, #4
	subs	r1, #12
	str	r2, [sp, #8]
	str	r3, [sp, #80]
	str	r1, [sp, #76]
.L_08195b18:
	ldr	r2, [sp, #8]
	ldr	r1, [sp, #100]
	ldr	r0, [r2, #0]
	movs	r3, #128
	movs	r2, #132
	lsls	r0, r0, #2
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r0, r0, r1
	adds	r3, #212
	adds	r1, r6, #0
	adds	r2, #3
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r2, [sp, #8]
	ldr	r1, [sp, #100]
	ldr	r0, [r2, #4]
	movs	r2, #132
	lsls	r0, r0, #2
	lsls	r2, r2, #24
	adds	r0, r0, r1
	adds	r2, #3
	ldr	r1, [sp, #92]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r2, [sp, #8]
	ldr	r1, [sp, #100]
	ldr	r0, [r2, #8]
	movs	r2, #132
	lsls	r0, r0, #2
	lsls	r2, r2, #24
	adds	r0, r0, r1
	adds	r2, #3
	ldr	r1, [sp, #88]
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r2, [sp, #92]
	ldr	r3, [r6, #0]
	ldr	r0, [r2, #0]
	ldr	r2, [sp, #80]
	subs	r0, r0, r3
	ldr	r3, [sp, #84]
	ldr	r1, [r3, #0]
	ldr	r3, [r2, #0]
	subs	r1, r1, r3
	ldr	r3, [pc, #764]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9916
	ldr	r2, [sp, #76]
	ldr	r3, [r6, #0]
	adds	r5, r0, #0
	ldr	r0, [r1, #0]
	ldr	r1, [r2, #0]
	ldr	r2, [sp, #80]
	subs	r0, r0, r3
	ldr	r3, [r2, #0]
	subs	r1, r1, r3
	ldr	r3, [pc, #736]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1a2d
	cmp	r5, #0
	bge.n	.L_08195b9a
	b.n	.L_0819621c
.L_08195b9a:
	ldr	r1, [sp, #104]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #92]
	ldr	r2, [sp, #112]
	lsls	r0, r1, #1
	adds	r0, r0, r1
	lsls	r0, r0, #3
	str	r3, [sp, #12]
	adds	r0, r0, r2
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	add	r1, sp, #120
	adds	r2, #6
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	ldr	r3, [r6, #0]
	movs	r2, #128
	lsls	r2, r2, #15
	adds	r3, r3, r2
	str	r3, [r6, #0]
	ldr	r1, [sp, #80]
	ldr	r3, [r1, #0]
	adds	r3, r3, r2
	str	r3, [r1, #0]
	ldr	r1, [sp, #92]
	ldr	r3, [r1, #0]
	adds	r3, r3, r2
	str	r3, [r1, #0]
	ldr	r1, [sp, #76]
	ldr	r3, [r1, #0]
	adds	r3, r3, r2
	str	r3, [r1, #0]
	ldr	r1, [sp, #88]
	ldr	r3, [r1, #0]
	adds	r3, r3, r2
	str	r3, [r1, #0]
	ldr	r1, [sp, #84]
	ldr	r3, [r1, #0]
	adds	r3, r3, r2
	str	r3, [r1, #0]
	ldr	r2, [sp, #80]
	ldr	r1, [sp, #76]
	ldr	r3, [r2, #0]
	ldr	r2, [r1, #0]
	cmp	r3, r2
	ble.n	.L_08195c5e
	ldr	r2, [sp, #92]
	ldr	r1, [r6, #0]
	ldr	r3, [r2, #0]
	add	r7, sp, #120
	eors	r1, r3
	str	r1, [r6, #0]
	ldr	r3, [sp, #92]
	ldr	r2, [r3, #0]
	eors	r2, r1
	str	r2, [r3, #0]
	ldr	r3, [r6, #0]
	eors	r3, r2
	str	r3, [r6, #0]
	ldr	r2, [sp, #80]
	ldr	r1, [r2, #0]
	ldr	r2, [sp, #76]
	ldr	r3, [r2, #0]
	eors	r1, r3
	ldr	r3, [sp, #80]
	str	r1, [r3, #0]
	ldr	r3, [sp, #76]
	ldr	r2, [r3, #0]
	eors	r2, r1
	ldr	r1, [sp, #92]
	str	r2, [r1, #4]
	ldr	r1, [sp, #80]
	ldr	r3, [r1, #0]
	eors	r3, r2
	str	r3, [r1, #0]
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #0]
	eors	r2, r1
	adds	r3, r2, #0
	eors	r3, r1
	eors	r2, r3
	str	r2, [r7, #0]
	ldr	r1, [r7, #12]
	ldr	r2, [r7, #4]
	str	r3, [r7, #8]
	eors	r2, r1
	adds	r3, r2, #0
	eors	r3, r1
	eors	r2, r3
	str	r3, [r7, #12]
	str	r2, [r7, #4]
	ldr	r3, [sp, #76]
	ldr	r2, [r3, #0]
	b.n	.L_08195c60
.L_08195c5e:
	add	r7, sp, #120
.L_08195c60:
	ldr	r1, [sp, #84]
	ldr	r3, [r1, #0]
	cmp	r2, r3
	ble.n	.L_08195d2a
	ldr	r2, [sp, #92]
	ldr	r1, [r2, #0]
	ldr	r2, [sp, #88]
	ldr	r3, [r2, #0]
	eors	r1, r3
	ldr	r3, [sp, #92]
	str	r1, [r3, #0]
	ldr	r3, [sp, #88]
	ldr	r2, [r3, #0]
	eors	r2, r1
	str	r2, [r3, #0]
	ldr	r1, [sp, #92]
	ldr	r3, [r1, #0]
	eors	r3, r2
	str	r3, [r1, #0]
	ldr	r2, [sp, #76]
	ldr	r1, [r2, #0]
	ldr	r2, [sp, #84]
	ldr	r3, [r2, #0]
	eors	r1, r3
	ldr	r3, [sp, #92]
	str	r1, [r3, #4]
	ldr	r3, [sp, #84]
	ldr	r2, [r3, #0]
	eors	r2, r1
	ldr	r1, [sp, #88]
	str	r2, [r1, #4]
	ldr	r1, [sp, #76]
	ldr	r3, [r1, #0]
	eors	r3, r2
	str	r3, [r1, #0]
	ldr	r1, [r7, #16]
	ldr	r2, [r7, #8]
	eors	r2, r1
	adds	r3, r2, #0
	eors	r3, r1
	eors	r2, r3
	str	r2, [r7, #8]
	ldr	r1, [r7, #20]
	ldr	r2, [r7, #12]
	str	r3, [r7, #16]
	eors	r2, r1
	adds	r3, r2, #0
	eors	r3, r1
	eors	r2, r3
	str	r3, [r7, #20]
	str	r2, [r7, #12]
	ldr	r3, [sp, #80]
	ldr	r1, [sp, #76]
	ldr	r2, [r3, #0]
	ldr	r3, [r1, #0]
	cmp	r2, r3
	ble.n	.L_08195d2a
	ldr	r2, [sp, #92]
	ldr	r1, [r6, #0]
	ldr	r3, [r2, #0]
	eors	r1, r3
	str	r1, [r6, #0]
	ldr	r3, [sp, #92]
	ldr	r2, [r3, #0]
	eors	r2, r1
	str	r2, [r3, #0]
	ldr	r3, [r6, #0]
	eors	r3, r2
	str	r3, [r6, #0]
	ldr	r2, [sp, #80]
	ldr	r1, [r2, #0]
	ldr	r2, [sp, #76]
	ldr	r3, [r2, #0]
	eors	r1, r3
	ldr	r3, [sp, #80]
	str	r1, [r3, #0]
	ldr	r3, [sp, #76]
	ldr	r2, [r3, #0]
	eors	r2, r1
	ldr	r1, [sp, #92]
	str	r2, [r1, #4]
	ldr	r1, [sp, #80]
	ldr	r3, [r1, #0]
	eors	r3, r2
	str	r3, [r1, #0]
	ldr	r1, [r7, #8]
	ldr	r2, [r7, #0]
	eors	r2, r1
	adds	r3, r2, #0
	eors	r3, r1
	eors	r2, r3
	ldr	r1, [r7, #12]
	str	r2, [r7, #0]
	ldr	r2, [r7, #4]
	str	r3, [r7, #8]
	eors	r2, r1
	adds	r3, r2, #0
	eors	r3, r1
	eors	r2, r3
	str	r3, [r7, #12]
	str	r2, [r7, #4]
.L_08195d2a:
	ldr	r5, [r6, #16]
	ldr	r0, [r6, #4]
	asrs	r3, r5, #16
	str	r3, [sp, #24]
	asrs	r2, r0, #16
	mov	r8, r2
	movs	r2, #30
	ldrsh	r1, [r6, r2]
	str	r1, [sp, #20]
	cmp	r8, r3
	beq.n	.L_08195d74
	ldr	r3, [r6, #0]
	ldr	r1, [r6, #12]
	subs	r5, r5, r0
	subs	r1, r1, r3
	adds	r0, r5, #0
	ldr	r3, [pc, #296]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9012
	ldr	r3, [r7, #0]
	ldr	r1, [r7, #8]
	ldr	r2, [pc, #284]
	subs	r1, r1, r3
	adds	r0, r5, #0
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x9010
	ldr	r3, [r7, #4]
	ldr	r1, [r7, #12]
	adds	r0, r5, #0
	subs	r1, r1, r3
	ldr	r3, [pc, #264]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x900f
	ldr	r0, [r6, #4]
.L_08195d74:
	ldr	r1, [sp, #20]
	cmp	r8, r1
	beq.n	.L_08195db0
	ldr	r5, [r6, #28]
	ldr	r3, [r6, #0]
	ldr	r1, [r6, #24]
	subs	r5, r5, r0
	subs	r1, r1, r3
	ldr	r2, [pc, #236]
	adds	r0, r5, #0
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x9011
	ldr	r3, [r7, #0]
	ldr	r1, [r7, #16]
	adds	r0, r5, #0
	subs	r1, r1, r3
	ldr	r3, [pc, #220]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x900e
	ldr	r1, [r7, #20]
	ldr	r3, [r7, #4]
	adds	r0, r5, #0
	subs	r1, r1, r3
	ldr	r2, [pc, #204]
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x900d
	ldr	r0, [r6, #4]
.L_08195db0:
	ldr	r3, [r6, #0]
	str	r3, [sp, #44]
	str	r3, [sp, #48]
	ldr	r1, [r7, #0]
	str	r1, [sp, #32]
	str	r1, [sp, #40]
	ldr	r2, [r7, #4]
	str	r2, [sp, #28]
	str	r2, [sp, #36]
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #24]
	cmp	r2, r3
	blt.n	.L_08195e78
	ldr	r1, [r6, #16]
	ldr	r5, [pc, #160]
	subs	r1, r1, r0
	ldr	r0, [sp, #68]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x68f3
	ldr	r2, [r6, #0]
	subs	r3, r3, r2
	subs	r3, r3, r0
	cmp	r3, #0
	blt.n	.L_08195df8
	ldr	r3, [r6, #4]
	ldr	r1, [r6, #16]
	ldr	r0, [sp, #68]
	subs	r1, r1, r3
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x68f3
	ldr	r2, [r6, #0]
	subs	r3, r3, r2
	subs	r5, r3, r0
	b.n	.L_08195e0c
.L_08195df8:
	ldr	r3, [r6, #4]
	ldr	r1, [r6, #16]
	ldr	r0, [sp, #68]
	subs	r1, r1, r3
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x68f3
	ldr	r2, [r6, #0]
	subs	r3, r3, r2
	subs	r5, r0, r3
.L_08195e0c:
	ldr	r3, [r6, #4]
	ldr	r1, [r6, #16]
	ldr	r0, [sp, #56]
	subs	r1, r1, r3
	ldr	r3, [pc, #88]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x683b
	ldr	r1, [r7, #8]
	ldr	r2, [pc, #84]
	subs	r1, r1, r3
	subs	r1, r1, r0
	adds	r0, r5, #0
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x9004
	ldr	r0, [sp, #52]
	ldr	r3, [r6, #4]
	ldr	r1, [r6, #16]
	subs	r1, r1, r3
	ldr	r3, [pc, #56]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x687b
	ldr	r1, [r7, #12]
	ldr	r2, [pc, #52]
	subs	r1, r1, r3
	subs	r1, r1, r0
	adds	r0, r5, #0
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x990e
	ldr	r3, [sp, #64]
	ldr	r2, [sp, #60]
	eors	r3, r1
	eors	r1, r3
	eors	r3, r1
	str	r3, [sp, #64]
	ldr	r3, [sp, #52]
	mov	r9, r0
	eors	r2, r3
	eors	r3, r2
	eors	r2, r3
	str	r1, [sp, #56]
	str	r3, [sp, #52]
	str	r2, [sp, #60]
	b.n	.L_08195efe
	movs	r0, r0
	.4byte 0x03000354
	.4byte 0x0300021c
	.2byte 0x0230
	.2byte 0x0300
.L_08195e78:
	ldr	r1, [r6, #16]
	ldr	r5, [pc, #652]
	subs	r1, r1, r0
	ldr	r0, [sp, #68]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x68f3
	ldr	r2, [r6, #0]
	subs	r3, r3, r2
	subs	r0, r0, r3
	cmp	r0, #0
	blt.n	.L_08195ea6
	ldr	r3, [r6, #4]
	ldr	r1, [r6, #16]
	ldr	r0, [sp, #68]
	subs	r1, r1, r3
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x68f3
	ldr	r2, [r6, #0]
	subs	r3, r3, r2
	subs	r5, r0, r3
	b.n	.L_08195eba
.L_08195ea6:
	ldr	r3, [r6, #4]
	ldr	r1, [r6, #16]
	ldr	r0, [sp, #68]
	subs	r1, r1, r3
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x68f3
	ldr	r2, [r6, #0]
	subs	r3, r3, r2
	subs	r5, r3, r0
.L_08195eba:
	ldr	r3, [r6, #4]
	ldr	r1, [r6, #16]
	ldr	r0, [sp, #56]
	subs	r1, r1, r3
	ldr	r2, [pc, #580]
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x683a
	ldr	r3, [r7, #8]
	adds	r1, r0, #0
	subs	r3, r3, r2
	subs	r1, r1, r3
	adds	r0, r5, #0
	ldr	r3, [pc, #564]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9004
	ldr	r2, [pc, #552]
	ldr	r3, [r6, #4]
	ldr	r1, [r6, #16]
	ldr	r0, [sp, #52]
	subs	r1, r1, r3
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x68fb
	ldr	r2, [r7, #4]
	adds	r1, r0, #0
	subs	r3, r3, r2
	subs	r1, r1, r3
	adds	r0, r5, #0
	ldr	r3, [pc, #532]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4681
.L_08195efe:
	ldr	r1, [sp, #24]
	mov	r0, r8
	cmp	r0, r1
	beq.n	.L_08195fc6
.L_08195f06:
	ldr	r2, [sp, #12]
	ldr	r1, [sp, #36]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r2, r2, r3
	mov	ip, r2
	asrs	r3, r1, #16
	ldr	r2, [sp, #108]
	ldr	r1, [sp, #40]
	lsls	r3, r2
	asrs	r2, r1, #16
	adds	r3, r3, r2
	add	ip, r3
	ldr	r2, [sp, #48]
	ldr	r3, [sp, #44]
	asrs	r1, r2, #16
	asrs	r5, r3, #16
	movs	r2, #0
	mov	r8, r2
	mov	lr, r2
	cmp	r1, r5
	ble.n	.L_08195f38
	eors	r1, r5
	eors	r5, r1
	eors	r1, r5
.L_08195f38:
	movs	r2, #7
	ands	r2, r0
	lsrs	r3, r0, #3
	lsls	r2, r2, #3
	lsls	r3, r3, #10
	adds	r2, r2, r3
	adds	r4, r1, #0
	mov	sl, r2
	cmp	r4, r5
	beq.n	.L_08195f8e
	movs	r3, #7
	mov	fp, r3
.L_08195f50:
	mov	r2, r8
	asrs	r1, r2, #16
	mov	r2, lr
	asrs	r3, r2, #16
	ldr	r2, [sp, #108]
	lsls	r3, r2
	asrs	r2, r4, #3
	lsls	r2, r2, #6
	str	r2, [sp, #4]
	adds	r1, r1, r3
	mov	r2, fp
	adds	r3, r4, #0
	ands	r3, r2
	ldr	r2, [sp, #4]
	adds	r3, r2, r3
	adds	r2, r3, #0
	mov	r3, ip
	ldrb	r1, [r3, r1]
	ldr	r3, [sp, #96]
	add	r2, sl
	ldrb	r3, [r3, r2]
	cmp	r3, r1
	bcs.n	.L_08195f82
	ldr	r3, [sp, #96]
	strb	r1, [r3, r2]
.L_08195f82:
	ldr	r1, [sp, #16]
	adds	r4, #1
	add	r8, r1
	add	lr, r9
	cmp	r4, r5
	bne.n	.L_08195f50
.L_08195f8e:
	ldr	r2, [sp, #48]
	ldr	r3, [sp, #72]
	ldr	r1, [sp, #44]
	adds	r2, r2, r3
	str	r2, [sp, #48]
	ldr	r2, [sp, #68]
	ldr	r3, [sp, #40]
	adds	r1, r1, r2
	str	r1, [sp, #44]
	ldr	r1, [sp, #64]
	ldr	r2, [sp, #36]
	adds	r3, r3, r1
	str	r3, [sp, #40]
	ldr	r3, [sp, #60]
	ldr	r1, [sp, #32]
	adds	r2, r2, r3
	str	r2, [sp, #36]
	ldr	r2, [sp, #56]
	ldr	r3, [sp, #28]
	adds	r1, r1, r2
	str	r1, [sp, #32]
	ldr	r1, [sp, #52]
	ldr	r2, [sp, #24]
	adds	r3, r3, r1
	adds	r0, #1
	str	r3, [sp, #28]
	cmp	r0, r2
	bne.n	.L_08195f06
.L_08195fc6:
	ldr	r3, [sp, #24]
	ldr	r1, [sp, #20]
	cmp	r3, r1
	bne.n	.L_08195fd0
	b.n	.L_0819621c
.L_08195fd0:
	ldr	r2, [r6, #28]
	ldr	r3, [r6, #16]
	ldr	r1, [r6, #24]
	subs	r5, r2, r3
	ldr	r3, [r6, #12]
	ldr	r2, [pc, #304]
	subs	r1, r1, r3
	mov	r8, r2
	adds	r0, r5, #0
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x9012
	ldr	r2, [r6, #12]
	ldr	r3, [r6, #24]
	cmp	r2, r3
	blt.n	.L_08196000
	ldr	r3, [sp, #64]
	ldr	r1, [sp, #60]
	str	r3, [sp, #56]
	ldr	r2, [sp, #40]
	ldr	r3, [sp, #36]
	str	r1, [sp, #52]
	str	r2, [sp, #32]
	str	r3, [sp, #28]
.L_08196000:
	ldr	r3, [r7, #8]
	ldr	r1, [r7, #16]
	adds	r0, r5, #0
	subs	r1, r1, r3
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x9010
	ldr	r3, [r7, #12]
	ldr	r1, [r7, #20]
	adds	r0, r5, #0
	subs	r1, r1, r3
	mov	lr, r8
	.2byte 0xf800
	.2byte 0x900f
	ldr	r1, [r6, #12]
	str	r1, [sp, #48]
	ldr	r2, [r7, #8]
	str	r2, [sp, #40]
	ldr	r3, [r7, #12]
	str	r3, [sp, #36]
	ldr	r3, [r6, #0]
	cmp	r3, r1
	bgt.n	.L_081960d8
	ldr	r3, [r6, #4]
	ldr	r1, [r6, #16]
	ldr	r5, [pc, #212]
	subs	r1, r1, r3
	ldr	r0, [sp, #68]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x68f3
	ldr	r2, [r6, #0]
	subs	r3, r3, r2
	subs	r3, r3, r0
	cmp	r3, #0
	blt.n	.L_0819605e
	ldr	r3, [r6, #4]
	ldr	r1, [r6, #16]
	ldr	r0, [sp, #68]
	subs	r1, r1, r3
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x68f3
	ldr	r2, [r6, #0]
	subs	r3, r3, r2
	subs	r5, r3, r0
	b.n	.L_08196072
.L_0819605e:
	ldr	r3, [r6, #4]
	ldr	r1, [r6, #16]
	ldr	r0, [sp, #68]
	subs	r1, r1, r3
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x68f3
	ldr	r2, [r6, #0]
	subs	r3, r3, r2
	subs	r5, r0, r3
.L_08196072:
	ldr	r3, [r6, #4]
	ldr	r1, [r6, #16]
	ldr	r0, [sp, #56]
	subs	r1, r1, r3
	ldr	r2, [pc, #140]
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x683b
	ldr	r1, [r7, #8]
	subs	r1, r1, r3
	subs	r1, r1, r0
	ldr	r3, [pc, #128]
	adds	r0, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9004
	ldr	r2, [pc, #116]
	ldr	r3, [r6, #4]
	ldr	r1, [r6, #16]
	ldr	r0, [sp, #52]
	subs	r1, r1, r3
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x687b
	ldr	r1, [r7, #12]
	subs	r1, r1, r3
	subs	r1, r1, r0
	ldr	r3, [pc, #96]
	adds	r0, r5, #0
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9a0e
	ldr	r1, [sp, #64]
	ldr	r3, [sp, #60]
	eors	r1, r2
	eors	r2, r1
	eors	r1, r2
	str	r1, [sp, #64]
	ldr	r1, [sp, #52]
	str	r2, [sp, #56]
	eors	r3, r1
	eors	r1, r3
	eors	r3, r1
	str	r3, [sp, #60]
	ldr	r2, [sp, #32]
	ldr	r3, [sp, #28]
	mov	r9, r0
	str	r1, [sp, #52]
	str	r2, [sp, #40]
	str	r3, [sp, #36]
	b.n	.L_08196168
.L_081960d8:
	ldr	r3, [r6, #4]
	ldr	r1, [r6, #16]
	ldr	r5, [pc, #40]
	subs	r1, r1, r3
	ldr	r0, [sp, #68]
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x68f3
	ldr	r2, [r6, #0]
	subs	r3, r3, r2
	subs	r0, r0, r3
	cmp	r0, #0
	blt.n	.L_08196110
	ldr	r3, [r6, #4]
	ldr	r1, [r6, #16]
	ldr	r0, [sp, #68]
	subs	r1, r1, r3
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x68f3
	ldr	r2, [r6, #0]
	subs	r3, r3, r2
	subs	r5, r0, r3
	b.n	.L_08196124
	.4byte 0x0300021c
	.2byte 0x0230
	.2byte 0x0300
.L_08196110:
	ldr	r3, [r6, #4]
	ldr	r1, [r6, #16]
	ldr	r0, [sp, #68]
	subs	r1, r1, r3
	mov	lr, r5
	.2byte 0xf800
	.2byte 0x68f3
	ldr	r2, [r6, #0]
	subs	r3, r3, r2
	subs	r5, r3, r0
.L_08196124:
	ldr	r3, [r6, #4]
	ldr	r1, [r6, #16]
	ldr	r0, [sp, #56]
	subs	r1, r1, r3
	ldr	r2, [pc, #272]
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x683a
	ldr	r3, [r7, #8]
	adds	r1, r0, #0
	subs	r3, r3, r2
	subs	r1, r1, r3
	adds	r0, r5, #0
	ldr	r3, [pc, #260]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x9004
	ldr	r2, [pc, #248]
	ldr	r3, [r6, #4]
	ldr	r1, [r6, #16]
	ldr	r0, [sp, #52]
	subs	r1, r1, r3
	mov	lr, r2
	.2byte 0xf800
	.2byte 0x68fb
	ldr	r2, [r7, #4]
	adds	r1, r0, #0
	subs	r3, r3, r2
	subs	r1, r1, r3
	adds	r0, r5, #0
	ldr	r3, [pc, #224]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4681
.L_08196168:
	ldr	r0, [sp, #24]
	ldr	r1, [sp, #20]
	cmp	r0, r1
	beq.n	.L_0819621c
.L_08196170:
	ldr	r2, [sp, #12]
	ldr	r1, [sp, #36]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r7, r2, r3
	asrs	r3, r1, #16
	ldr	r2, [sp, #108]
	ldr	r1, [sp, #40]
	lsls	r3, r2
	asrs	r2, r1, #16
	adds	r3, r3, r2
	adds	r7, r7, r3
	ldr	r2, [sp, #48]
	ldr	r3, [sp, #44]
	asrs	r1, r2, #16
	asrs	r5, r3, #16
	movs	r2, #0
	mov	lr, r2
	mov	ip, r2
	cmp	r1, r5
	ble.n	.L_081961a0
	eors	r1, r5
	eors	r5, r1
	eors	r1, r5
.L_081961a0:
	movs	r2, #7
	ands	r2, r0
	lsrs	r3, r0, #3
	lsls	r2, r2, #3
	lsls	r3, r3, #10
	adds	r2, r2, r3
	adds	r4, r1, #0
	mov	r8, r2
	cmp	r4, r5
	beq.n	.L_081961f4
	movs	r3, #7
	mov	sl, r3
.L_081961b8:
	mov	r2, lr
	asrs	r1, r2, #16
	mov	r2, ip
	asrs	r3, r2, #16
	ldr	r2, [sp, #108]
	lsls	r3, r2
	asrs	r2, r4, #3
	lsls	r2, r2, #6
	adds	r1, r1, r3
	mov	fp, r2
	adds	r3, r4, #0
	mov	r2, sl
	ands	r3, r2
	add	fp, r3
	ldr	r3, [sp, #96]
	mov	r2, fp
	add	r2, r8
	ldrb	r3, [r3, r2]
	ldrb	r1, [r7, r1]
	mov	fp, r3
	cmp	fp, r1
	bcs.n	.L_081961e8
	ldr	r3, [sp, #96]
	strb	r1, [r3, r2]
.L_081961e8:
	ldr	r1, [sp, #16]
	adds	r4, #1
	add	lr, r1
	add	ip, r9
	cmp	r4, r5
	bne.n	.L_081961b8
.L_081961f4:
	ldr	r2, [sp, #48]
	ldr	r3, [sp, #72]
	ldr	r1, [sp, #44]
	adds	r2, r2, r3
	str	r2, [sp, #48]
	ldr	r2, [sp, #68]
	ldr	r3, [sp, #40]
	adds	r1, r1, r2
	str	r1, [sp, #44]
	ldr	r1, [sp, #64]
	ldr	r2, [sp, #36]
	adds	r3, r3, r1
	str	r3, [sp, #40]
	ldr	r3, [sp, #60]
	ldr	r1, [sp, #20]
	adds	r2, r2, r3
	adds	r0, #1
	str	r2, [sp, #36]
	cmp	r0, r1
	bne.n	.L_08196170
.L_0819621c:
	ldr	r2, [sp, #8]
	ldr	r3, [sp, #104]
	ldr	r1, [sp, #116]
	adds	r2, #28
	adds	r3, #1
	str	r2, [sp, #8]
	str	r3, [sp, #104]
	cmp	r3, r1
	beq.n	.L_08196230
	b.n	.L_08195b18
.L_08196230:
	add	sp, #144
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0300021c
	.2byte 0x0230
	.2byte 0x0300
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #20
	str	r1, [sp, #16]
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r2, [r3, #100]
	adds	r6, r0, #0
	str	r2, [sp, #12]
	ldr	r3, [r3, #92]
	mov	r8, r3
	lsls	r3, r6, #1
	adds	r6, r3, r6
	movs	r3, #0
	str	r3, [sp, #8]
	cmp	r6, #0
	beq.n	.L_0819628c
	ldr	r7, [pc, #364]
	adds	r5, r2, #0
.L_08196278:
	adds	r0, r5, #0
	adds	r1, r5, #0
	mov	lr, r7
	.2byte 0xf800
	.2byte 0x9c02
	adds	r5, #12
	adds	r4, #3
	str	r4, [sp, #8]
	cmp	r4, r6
	bne.n	.L_08196278
.L_0819628c:
	ldr	r3, [sp, #16]
	movs	r2, #0
	str	r2, [sp, #8]
	cmp	r3, #0
	bne.n	.L_08196298
	b.n	.L_081963d6
.L_08196298:
	ldr	r4, [sp, #12]
	lsls	r3, r6, #2
	adds	r6, r3, r4
	movs	r2, #12
	adds	r4, r6, #0
	adds	r2, r2, r6
	adds	r4, #28
	mov	r9, r2
	movs	r3, #24
	movs	r2, #16
	str	r4, [sp, #4]
	adds	r3, r3, r6
	adds	r2, r2, r6
	mov	sl, r3
	adds	r7, r6, #4
	mov	fp, r2
.L_081962b8:
	mov	r3, r8
	ldr	r0, [r3, #0]
	ldr	r4, [sp, #12]
	movs	r3, #128
	movs	r2, #132
	lsls	r0, r0, #2
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, r0, r4
	adds	r1, r6, #0
	adds	r2, #3
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r2, r8
	ldr	r0, [r2, #4]
	movs	r2, #132
	lsls	r0, r0, #2
	lsls	r2, r2, #24
	adds	r0, r0, r4
	mov	r1, r9
	adds	r2, #3
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r3, r8
	ldr	r0, [r3, #8]
	movs	r2, #132
	movs	r3, #128
	lsls	r0, r0, #2
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, r0, r4
	mov	r1, sl
	adds	r2, #3
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	mov	r4, r9
	ldr	r2, [sp, #4]
	ldr	r0, [r4, #0]
	ldr	r3, [r6, #0]
	ldr	r1, [r2, #0]
	subs	r0, r0, r3
	ldr	r3, [r7, #0]
	subs	r1, r1, r3
	ldr	r3, [pc, #212]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x4654
	ldr	r3, [r6, #0]
	adds	r5, r0, #0
	ldr	r0, [r4, #0]
	mov	r2, fp
	subs	r0, r0, r3
	ldr	r1, [r2, #0]
	ldr	r3, [r7, #0]
	subs	r1, r1, r3
	ldr	r3, [pc, #188]
	mov	lr, r3
	.2byte 0xf800
	.2byte 0x1a2d
	cmp	r5, #0
	blt.n	.L_081963c4
	ldr	r3, [r6, #0]
	movs	r2, #128
	lsls	r2, r2, #15
	adds	r3, r3, r2
	mov	r4, r8
	ldrb	r5, [r4, #24]
	str	r3, [r6, #0]
	ldr	r3, [r7, #0]
	mov	r4, r9
	adds	r3, r3, r2
	str	r3, [r7, #0]
	ldr	r3, [r4, #0]
	adds	r3, r3, r2
	str	r3, [r4, #0]
	mov	r2, fp
	ldr	r3, [r2, #0]
	movs	r4, #128
	lsls	r4, r4, #15
	adds	r3, r3, r4
	str	r3, [r2, #0]
	mov	r2, sl
	ldr	r3, [r2, #0]
	adds	r3, r3, r4
	str	r3, [r2, #0]
	ldr	r4, [sp, #4]
	movs	r2, #128
	ldr	r3, [r4, #0]
	lsls	r2, r2, #15
	adds	r3, r3, r2
	str	r3, [r4, #0]
	movs	r4, #2
	ldrsh	r1, [r7, r4]
	mov	r4, r9
	movs	r3, #2
	ldrsh	r2, [r4, r3]
	movs	r3, #2
	ldrsh	r0, [r6, r3]
	mov	r3, fp
	movs	r4, #2
	ldrsh	r3, [r3, r4]
	str	r5, [sp, #0]
	bl	sub_08143eb4
	mov	r2, r9
	movs	r4, #2
	ldrsh	r0, [r2, r4]
	mov	r4, fp
	movs	r3, #2
	ldrsh	r1, [r4, r3]
	mov	r4, sl
	movs	r3, #2
	ldrsh	r2, [r4, r3]
	ldr	r3, [sp, #4]
	movs	r4, #2
	ldrsh	r3, [r3, r4]
	str	r5, [sp, #0]
	bl	sub_08143eb4
	movs	r4, #2
	ldrsh	r0, [r6, r4]
	mov	r4, sl
	movs	r2, #2
	ldrsh	r1, [r7, r2]
	movs	r3, #2
	ldrsh	r2, [r4, r3]
	ldr	r3, [sp, #4]
	movs	r4, #2
	ldrsh	r3, [r3, r4]
	str	r5, [sp, #0]
	bl	sub_08143eb4
.L_081963c4:
	ldr	r2, [sp, #8]
	ldr	r3, [sp, #16]
	movs	r4, #28
	adds	r2, #1
	add	r8, r4
	str	r2, [sp, #8]
	cmp	r2, r3
	beq.n	.L_081963d6
	b.n	.L_081962b8
.L_081963d6:
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	.4byte 0x03000354
	.4byte 0x0300021c
