.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_080022fc, 0x080022fc
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_08002dd8, 0x08002dd8
	.set sub_08002f40, 0x08002f40
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080049ac, 0x080049ac
	.set sub_08004bd4, 0x08004bd4
	.set sub_08004cb4, 0x08004cb4
	.set sub_080051d8, 0x080051d8
	.set sub_08005340, 0x08005340
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_080072f8, 0x080072f8
	.set sub_08009080, 0x08009080
	.set sub_08009140, 0x08009140
	.set sub_08009150, 0x08009150
	.set sub_080b5088, 0x080b5088
	.set sub_080b5098, 0x080b5098
	.set sub_080b50e8, 0x080b50e8
	.set sub_080cd52c, 0x080cd52c
	.set sub_080cd594, 0x080cd594
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080d6888, 0x080d6888
	.set sub_080e155c, 0x080e155c
	.set sub_080e3944, 0x080e3944
	.set sub_080e3980, 0x080e3980
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global Overlay_080cde90
Overlay_080cde90:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	adds	r6, r3, #0
	ldr	r3, [pc, #392]
	adds	r5, r1, #0
	ldr	r3, [r3, #0]
	adds	r4, r2, #0
	subs	r1, r6, r5
	movs	r2, #128
	sub	sp, #4
	mov	r8, r0
	subs	r7, r4, r0
	mov	sl, r1
	mov	r9, r2
	mov	fp, r3
	cmp	r5, #0
	bge.n	.L_080cdebe
	movs	r5, #0
.L_080cdebe:
	cmp	r5, #127
	ble.n	.L_080cdec4
	movs	r5, #127
.L_080cdec4:
	cmp	r6, #0
	bge.n	.L_080cdeca
	movs	r6, #0
.L_080cdeca:
	cmp	r6, #127
	ble.n	.L_080cded0
	movs	r6, #127
.L_080cded0:
	adds	r2, r7, #0
	cmp	r7, #0
	bge.n	.L_080cded8
	negs	r2, r7
.L_080cded8:
	mov	r3, sl
	cmp	r3, #0
	bge.n	.L_080cdee0
	negs	r3, r3
.L_080cdee0:
	cmp	r2, r3
	bge.n	.L_080cdf74
	mov	r3, sl
	cmp	r3, #0
	bge.n	.L_080cdefa
	mov	ip, r8
	mov	r8, r4
	mov	r4, ip
	mov	ip, r5
	mov	r1, r8
	adds	r5, r6, #0
	mov	r6, ip
	subs	r7, r4, r1
.L_080cdefa:
	subs	r1, r6, r5
	lsls	r0, r7, #8
	cmp	r7, #0
	bge.n	.L_080cdf08
	mov	r2, r8
	subs	r3, r2, r4
	lsls	r0, r3, #8
.L_080cdf08:
	cmp	r1, #0
	bge.n	.L_080cdf0e
	subs	r1, r5, r6
.L_080cdf0e:
	bl	sub_080022ec
	mov	ip, r0
	adds	r0, r5, #0
	mov	r1, r8
	cmp	r0, r6
	beq.n	.L_080ce018
	movs	r3, #128
	ldr	r5, [pc, #272]
	lsls	r3, r3, #1
	movs	r4, #7
	mov	lr, r3
	mov	r8, r5
.L_080cdf28:
	lsrs	r2, r0, #3
	lsrs	r3, r1, #3
	lsls	r2, r2, #4
	adds	r2, r2, r3
	adds	r3, r0, #0
	ands	r3, r4
	lsls	r2, r2, #3
	adds	r2, r2, r3
	adds	r3, r1, #0
	ands	r3, r4
	lsls	r2, r2, #3
	mov	r5, fp
	adds	r2, r2, r3
	ldrb	r3, [r5, r2]
	ldr	r5, [sp, #36]
	cmp	r3, r5
	bge.n	.L_080cdf4e
	mov	r3, fp
	strb	r5, [r3, r2]
.L_080cdf4e:
	add	r9, ip
	mov	r3, r9
	mov	r5, lr
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_080cdf6c
	cmp	r7, #0
	ble.n	.L_080cdf62
	adds	r1, #1
	b.n	.L_080cdf64
.L_080cdf62:
	subs	r1, #1
.L_080cdf64:
	mov	r2, r9
	mov	r3, r8
	ands	r2, r3
	mov	r9, r2
.L_080cdf6c:
	adds	r0, #1
	cmp	r0, r6
	bne.n	.L_080cdf28
	b.n	.L_080ce018
.L_080cdf74:
	cmp	r7, #0
	bge.n	.L_080cdf90
	mov	ip, r8
	mov	r8, r4
	mov	r4, ip
	mov	ip, r5
	adds	r5, r6, #0
	mov	r6, ip
	subs	r2, r6, r5
	mov	r1, r8
	mov	sl, r2
	subs	r7, r4, r1
	mov	r1, sl
	b.n	.L_080cdf92
.L_080cdf90:
	subs	r1, r6, r5
.L_080cdf92:
	lsls	r0, r1, #8
	cmp	r1, #0
	bge.n	.L_080cdf9c
	subs	r3, r5, r6
	lsls	r0, r3, #8
.L_080cdf9c:
	cmp	r7, #0
	blt.n	.L_080cdfae
	adds	r1, r7, #0
	str	r4, [sp, #0]
	bl	sub_080022ec
	mov	ip, r0
	ldr	r4, [sp, #0]
	b.n	.L_080cdfbc
.L_080cdfae:
	mov	r3, r8
	subs	r1, r3, r4
	str	r4, [sp, #0]
	bl	sub_080022ec
	ldr	r4, [sp, #0]
	mov	ip, r0
.L_080cdfbc:
	mov	r0, r8
	adds	r1, r5, #0
	cmp	r0, r4
	beq.n	.L_080ce018
	ldr	r6, [pc, #104]
	movs	r7, #128
	movs	r5, #7
	lsls	r7, r7, #1
	mov	lr, r6
.L_080cdfce:
	lsrs	r2, r1, #3
	lsrs	r3, r0, #3
	lsls	r2, r2, #4
	adds	r2, r2, r3
	adds	r3, r1, #0
	ands	r3, r5
	lsls	r2, r2, #3
	adds	r2, r2, r3
	adds	r3, r0, #0
	ands	r3, r5
	lsls	r2, r2, #3
	mov	r6, fp
	adds	r2, r2, r3
	ldrb	r3, [r6, r2]
	ldr	r6, [sp, #36]
	cmp	r3, r6
	bge.n	.L_080cdff4
	mov	r3, fp
	strb	r6, [r3, r2]
.L_080cdff4:
	add	r9, ip
	mov	r3, r9
	ands	r3, r7
	cmp	r3, #0
	beq.n	.L_080ce012
	mov	r6, sl
	cmp	r6, #0
	ble.n	.L_080ce008
	adds	r1, #1
	b.n	.L_080ce00a
.L_080ce008:
	subs	r1, #1
.L_080ce00a:
	mov	r2, r9
	mov	r3, lr
	ands	r2, r3
	mov	r9, r2
.L_080ce012:
	adds	r0, #1
	cmp	r0, r4
	bne.n	.L_080cdfce
.L_080ce018:
	add	sp, #4
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001ef0
	.2byte 0xfeff
	.2byte 0xffff
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r6, [pc, #940]
	adds	r3, r6, #0
	ldmia	r3!, {r1}
	sub	sp, #60
	str	r1, [sp, #36]
	ldr	r3, [r3, #0]
	str	r3, [sp, #32]
	ldr	r2, [r6, #8]
	str	r2, [sp, #28]
	ldr	r2, [pc, #924]
	adds	r3, r1, r2
	str	r0, [r3, #0]
	movs	r0, #0
	bl	sub_080cd594
	ldr	r0, [pc, #916]
	bl	sub_08002f40
	ldr	r1, [sp, #28]
	bl	sub_08005340
	ldr	r0, [pc, #908]
	bl	sub_08002f40
	adds	r5, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #904]
	adds	r1, r5, #0
	movs	r2, #128
	lsls	r0, r0, #19
	bl	sub_080072f0
	adds	r5, #128
	ldr	r1, [sp, #36]
	adds	r0, r5, #0
	bl	sub_08005340
	movs	r5, #2
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r5, [sp, #0]
	bl	sub_080ed408
	ldr	r3, [r6, #28]
	movs	r1, #7
	str	r3, [sp, #40]
	movs	r2, #7
	movs	r3, #7
	movs	r0, #47
	str	r5, [sp, #0]
	bl	sub_080ed408
	ldr	r3, [r6, #32]
	mov	r0, sp
	adds	r0, #40
	str	r0, [sp, #8]
	str	r3, [r0, #4]
	ldr	r1, [sp, #36]
	movs	r2, #239
	lsls	r2, r2, #7
	adds	r3, r1, r2
	str	r5, [r3, #0]
	ldr	r3, [pc, #832]
	adds	r2, r1, r3
	movs	r3, #75
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #824]
	bl	sub_080041d8
	movs	r0, #0
	movs	r2, #128
	ldr	r3, [pc, #816]
	mov	r8, r0
	movs	r1, #0
	lsls	r2, r2, #3
.L_080ce0e0:
	movs	r0, #1
	add	r8, r0
	str	r1, [r3, #0]
	adds	r3, #28
	cmp	r8, r2
	bne.n	.L_080ce0e0
	ldr	r1, [sp, #36]
	ldr	r2, [pc, #772]
	adds	r5, r1, r2
	ldr	r3, [r5, #0]
	ldr	r0, [r3, #8]
	bl	sub_080b5098
	ldr	r3, [r5, #0]
	ldr	r0, [r0, #0]
	mov	sl, r0
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	bl	sub_080b5098
	ldr	r0, [r0, #0]
	str	r0, [sp, #24]
	mov	r2, sl
	ldr	r3, [r2, #8]
	ldr	r0, [pc, #764]
	str	r0, [sp, #20]
	cmp	r3, #0
	bgt.n	.L_080ce11e
	movs	r1, #240
	lsls	r1, r1, #12
	str	r1, [sp, #20]
.L_080ce11e:
	ldr	r0, [pc, #724]
	ldr	r3, [sp, #36]
	mov	r1, sp
	adds	r0, r3, r0
	adds	r1, #48
	movs	r2, #0
	str	r0, [sp, #16]
	str	r1, [sp, #12]
	mov	fp, r2
.L_080ce130:
	ldr	r3, [pc, #736]
	ldr	r5, [r3, #0]
	bl	sub_080049ac
	adds	r1, r5, #0
	adds	r1, #12
	adds	r0, r5, #0
	bl	sub_080051d8
	mov	r2, fp
	cmp	r2, #17
	bgt.n	.L_080ce14c
	cmp	r2, #0
	bne.n	.L_080ce164
.L_080ce14c:
	ldr	r0, [sp, #16]
	ldr	r5, [sp, #12]
	ldr	r3, [r0, #0]
	adds	r1, r5, #0
	ldr	r0, [r3, #8]
	bl	sub_080e3980
	ldr	r3, [r5, #0]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r5, #0]
.L_080ce164:
	mov	r3, fp
	subs	r3, #2
	cmp	r3, #1
	bhi.n	.L_080ce188
	ldr	r5, [sp, #12]
	movs	r1, #32
	ldr	r2, [r5, #0]
	ldr	r3, [r5, #4]
	str	r1, [sp, #0]
	movs	r1, #64
	str	r1, [sp, #4]
	subs	r2, #16
	subs	r3, #64
	ldr	r4, [sp, #40]
	ldr	r0, [sp, #32]
	ldr	r1, [sp, #36]
	bl	sub_080072f4
.L_080ce188:
	mov	r2, fp
	subs	r2, #4
	cmp	r2, #11
	bhi.n	.L_080ce1ec
	lsrs	r3, r2, #31
	adds	r3, r2, r3
	asrs	r3, r3, #1
	movs	r1, #0
	lsls	r3, r3, #11
	mov	r8, r1
	add	r7, sp, #48
	mov	r9, r3
.L_080ce1a0:
	mov	r2, r8
	lsls	r6, r2, #12
	adds	r0, r6, #0
	bl	sub_08002322
	mov	r3, fp
	muls	r3, r0
	ldr	r5, [r7, #0]
	asrs	r3, r3, #16
	adds	r0, r6, #0
	adds	r5, r5, r3
	bl	sub_0800231c
	mov	r2, fp
	muls	r2, r0
	ldr	r3, [r7, #4]
	asrs	r2, r2, #16
	adds	r3, r3, r2
	mov	r0, fp
	movs	r2, #32
	ldr	r1, [sp, #36]
	subs	r3, r3, r0
	str	r2, [sp, #0]
	subs	r5, #16
	movs	r2, #64
	str	r2, [sp, #4]
	add	r1, r9
	adds	r2, r5, #0
	subs	r3, #64
	ldr	r4, [sp, #40]
	ldr	r0, [sp, #32]
	bl	sub_080072f4
	movs	r1, #1
	add	r8, r1
	mov	r2, r8
	cmp	r2, #16
	bne.n	.L_080ce1a0
.L_080ce1ec:
	mov	r3, fp
	cmp	r3, #4
	bne.n	.L_080ce23a
	movs	r3, #160
	mov	r0, sl
	lsls	r3, r3, #13
	str	r3, [r0, #40]
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r0, #52]
	movs	r3, #192
	lsls	r3, r3, #10
	str	r3, [r0, #48]
	ldr	r3, [pc, #528]
	str	r3, [r0, #72]
	mov	r3, sl
	movs	r2, #0
	adds	r3, #90
	strb	r2, [r3, #0]
	subs	r3, #2
	strb	r2, [r3, #0]
	ldr	r3, [r0, #8]
	lsls	r1, r3, #1
	adds	r1, r1, r3
	ldr	r3, [r0, #16]
	bl	sub_08009150
	mov	r0, sl
	movs	r1, #2
	bl	sub_08009080
	ldr	r2, [pc, #496]
	ldr	r1, [sp, #36]
	mov	r0, fp
	adds	r3, r1, r2
	str	r0, [r3, #0]
	movs	r0, #136
	bl	sub_080f9010
.L_080ce23a:
	mov	r1, fp
	cmp	r1, #16
	bne.n	.L_080ce276
	ldr	r0, [pc, #476]
	bl	sub_08002f40
	adds	r5, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #436]
	adds	r1, r5, #0
	movs	r2, #128
	lsls	r0, r0, #19
	bl	sub_080072f0
	adds	r5, #128
	adds	r0, r5, #0
	ldr	r1, [sp, #36]
	bl	sub_08005340
	movs	r3, #0
	mov	r2, sl
	str	r3, [r2, #72]
	str	r3, [r2, #36]
	str	r3, [r2, #40]
	ldr	r0, [sp, #24]
	ldr	r3, [r0, #16]
	mov	r0, sl
	str	r3, [r2, #16]
	bl	sub_08009140
.L_080ce276:
	mov	r1, fp
	cmp	r1, #17
	bgt.n	.L_080ce27e
	b.n	.L_080ce392
.L_080ce27e:
	mov	r3, sl
	ldr	r2, [r3, #12]
	cmp	r2, #0
	ble.n	.L_080ce2f0
	ldr	r0, [sp, #20]
	ldr	r3, [r3, #8]
	adds	r3, r3, r0
	ldr	r0, [pc, #404]
	mov	r1, sl
	str	r3, [r1, #8]
	adds	r3, r2, r0
	str	r3, [r1, #12]
	ldr	r1, [sp, #16]
	ldr	r3, [r1, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #0
	bne.n	.L_080ce2c8
	ldr	r5, [sp, #12]
	movs	r1, #40
	ldr	r2, [r5, #0]
	ldr	r3, [r5, #4]
	str	r1, [sp, #0]
	movs	r1, #64
	subs	r2, #20
	subs	r3, #52
	str	r1, [sp, #4]
	ldr	r4, [sp, #40]
	ldr	r0, [sp, #32]
	ldr	r1, [sp, #36]
	bl	sub_080072f4
	ldr	r3, [r5, #0]
	subs	r3, #8
	str	r3, [r5, #0]
	mov	r3, sl
	ldr	r2, [r3, #12]
	b.n	.L_080ce2f0
.L_080ce2c8:
	ldr	r5, [sp, #12]
	movs	r1, #40
	ldr	r2, [r5, #0]
	ldr	r3, [r5, #4]
	str	r1, [sp, #0]
	movs	r1, #64
	str	r1, [sp, #4]
	ldr	r0, [sp, #8]
	subs	r2, #26
	subs	r3, #52
	ldr	r4, [r0, #4]
	ldr	r1, [sp, #36]
	ldr	r0, [sp, #32]
	bl	sub_080072f4
	ldr	r3, [r5, #4]
	adds	r3, #8
	str	r3, [r5, #4]
	mov	r1, sl
	ldr	r2, [r1, #12]
.L_080ce2f0:
	cmp	r2, #0
	bge.n	.L_080ce392
	movs	r3, #0
	mov	r2, sl
	str	r3, [r2, #12]
	mov	r8, r3
	add	r3, sp, #48
	ldr	r7, [pc, #296]
	mov	r9, r3
.L_080ce302:
	bl	sub_08004458
	ldr	r5, [pc, #292]
	ands	r5, r0
	bl	sub_08004458
	ldr	r3, [pc, #288]
	adds	r6, r0, #0
	mov	r0, r9
	ands	r6, r3
	ldr	r3, [r0, #0]
	lsls	r3, r3, #16
	str	r3, [r7, #0]
	ldr	r3, [r0, #4]
	subs	r3, #24
	lsls	r3, r3, #16
	str	r3, [r7, #4]
	adds	r0, r6, #0
	bl	sub_08002322
	adds	r5, #32
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #8]
	adds	r0, r6, #0
	bl	sub_0800231c
	adds	r3, r5, #0
	muls	r3, r0
	lsls	r3, r3, #1
	negs	r3, r3
	asrs	r3, r3, #6
	str	r3, [r7, #16]
	bl	sub_08004458
	movs	r3, #7
	ands	r3, r0
	movs	r1, #1
	movs	r2, #128
	adds	r3, #32
	add	r8, r1
	lsls	r2, r2, #1
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r8, r2
	bne.n	.L_080ce302
	ldr	r0, [sp, #36]
	ldr	r1, [pc, #184]
	movs	r5, #8
	adds	r3, r0, r1
	str	r5, [r3, #0]
	movs	r0, #145
	bl	sub_080b50e8
	ldr	r2, [sp, #16]
	ldr	r3, [r2, #0]
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	movs	r1, #4
	bl	sub_080b5088
	ldr	r2, [sp, #16]
	ldr	r3, [r2, #0]
	movs	r2, #5
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	movs	r1, #7
	movs	r3, #0
	str	r5, [sp, #0]
	bl	sub_080d6888
.L_080ce392:
	movs	r2, #0
	ldr	r6, [pc, #144]
	mov	r8, r2
.L_080ce398:
	ldr	r4, [r6, #24]
	cmp	r4, #0
	ble.n	.L_080ce47a
	ldr	r2, [r6, #8]
	ldr	r3, [r6, #0]
	adds	r3, r3, r2
	mov	ip, r3
	str	r3, [r6, #0]
	ldr	r1, [r6, #16]
	ldr	r3, [r6, #4]
	adds	r7, r3, r1
	lsls	r3, r2, #3
	subs	r3, r3, r2
	subs	r0, r4, #1
	lsls	r3, r3, #3
	str	r0, [r6, #24]
	str	r7, [r6, #4]
	cmp	r3, #0
	bge.n	.L_080ce3c0
	adds	r3, #63
.L_080ce3c0:
	asrs	r3, r3, #6
	str	r3, [r6, #8]
	lsls	r3, r1, #3
	subs	r3, r3, r1
	lsls	r3, r3, #3
	cmp	r3, #0
	bge.n	.L_080ce3d0
	adds	r3, #63
.L_080ce3d0:
	movs	r1, #128
	asrs	r3, r3, #6
	lsls	r1, r1, #6
	movs	r2, #224
	adds	r3, r3, r1
	lsls	r2, r2, #15
	str	r3, [r6, #16]
	cmp	r7, r2
	ble.n	.L_080ce434
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r6, #16]
	b.n	.L_080ce47a
	movs	r0, r0
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x00000073
	.4byte 0x0000007d
	.4byte 0x03001388
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x02010018
	.4byte 0xfff10000
	.4byte 0x03001e80
	.4byte 0x0000ab85
	.4byte 0x000077a8
	.4byte 0x00000089
	.4byte 0xfff80000
	.4byte 0x02010000
	.4byte 0x000003ff
	.2byte 0xffff
	.2byte 0x0000
.L_080ce434:
	ldr	r3, [pc, #160]
	cmp	ip, r3
	bhi.n	.L_080ce47a
	cmp	r7, #0
	blt.n	.L_080ce47a
	cmp	r0, #0
	bge.n	.L_080ce444
	adds	r0, r4, #6
.L_080ce444:
	asrs	r0, r0, #3
	adds	r0, #1
	lsls	r5, r0, #1
	ldr	r2, [pc, #144]
	mov	r1, r8
	subs	r3, r5, #2
	movs	r4, #1
	ands	r4, r1
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #28]
	mov	r3, ip
	adds	r1, r2, r1
	asrs	r2, r3, #16
	lsrs	r3, r0, #31
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	asrs	r3, r7, #16
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r5, [sp, #4]
	ldr	r0, [sp, #8]
	lsls	r4, r4, #2
	ldr	r4, [r4, r0]
	ldr	r0, [sp, #32]
	bl	sub_080072f4
.L_080ce47a:
	movs	r1, #1
	movs	r2, #128
	add	r8, r1
	lsls	r2, r2, #1
	adds	r6, #28
	cmp	r8, r2
	bne.n	.L_080ce398
	movs	r1, #16
	movs	r0, #16
	bl	sub_080e155c
	bl	sub_080cd52c
	ldr	r0, [pc, #72]
	ldr	r3, [sp, #36]
	adds	r2, r3, r0
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	movs	r1, #1
	add	fp, r1
	mov	r2, fp
	cmp	r2, #88
	beq.n	.L_080ce4b0
	b.n	.L_080ce130
.L_080ce4b0:
	ldr	r0, [pc, #48]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080cdbc0
	add	sp, #60
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x007effff
	.4byte 0x080ede48
	.4byte 0x00007824
	.2byte 0xd261
	.2byte 0x080c
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r1, [pc, #816]
	mov	r8, r1
	mov	r3, r8
	ldmia	r3!, {r2}
	ldr	r3, [r3, #0]
	sub	sp, #72
	str	r3, [sp, #44]
	ldr	r3, [pc, #804]
	mov	sl, r2
	add	r3, sl
	str	r0, [r3, #0]
	movs	r0, #0
	bl	sub_080cd594
	ldr	r0, [pc, #796]
	bl	sub_08002f40
	adds	r6, r0, #0
	movs	r0, #160
	movs	r2, #128
	ldr	r5, [pc, #788]
	adds	r1, r6, #0
	lsls	r0, r0, #19
	adds	r6, #128
	bl	sub_080072f8
	mov	r1, sl
	adds	r0, r6, #0
	bl	sub_08005340
	ldr	r0, [pc, #772]
	bl	sub_08002f40
	adds	r6, r0, #0
	movs	r0, #160
	adds	r1, r6, #0
	movs	r2, #128
	lsls	r0, r0, #19
	bl	sub_080072f8
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	bl	sub_080ed408
	mov	r3, r8
	ldr	r3, [r3, #28]
	movs	r6, #3
	str	r3, [sp, #32]
	movs	r2, #7
	movs	r3, #3
	movs	r1, #7
	movs	r0, #47
	str	r6, [sp, #0]
	bl	sub_080ed408
	mov	r5, r8
	ldr	r5, [r5, #32]
	str	r5, [sp, #36]
	movs	r5, #144
	lsls	r5, r5, #3
	adds	r1, r5, #0
	ldr	r0, [pc, #704]
	bl	sub_080041d8
	movs	r3, #239
	lsls	r3, r3, #7
	add	r3, sl
	ldr	r2, [pc, #696]
	str	r6, [r3, #0]
	ldr	r3, [pc, #696]
	add	r2, sl
	adds	r1, r5, #0
	str	r3, [r2, #0]
	ldr	r0, [pc, #692]
	ldr	r5, [pc, #696]
	bl	sub_080041d8
	movs	r6, #255
	movs	r4, #0
.L_080ce59c:
	str	r4, [sp, #8]
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #15
	str	r0, [r5, #0]
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #15
	str	r0, [r5, #4]
	bl	sub_08004458
	ands	r0, r6
	ldr	r4, [sp, #8]
	subs	r0, #127
	movs	r1, #128
	lsls	r0, r0, #15
	adds	r4, #1
	lsls	r1, r1, #2
	str	r0, [r5, #8]
	adds	r5, #28
	cmp	r4, r1
	bne.n	.L_080ce59c
	movs	r0, #142
	bl	sub_080f9010
	movs	r2, #0
	ldr	r3, [pc, #592]
	str	r2, [sp, #40]
	add	r3, sl
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #20]
	movs	r5, #96
	lsls	r3, r3, #5
	negs	r5, r5
	cmp	r3, r5
	bne.n	.L_080ce5ee
	b.n	.L_080ce7fa
.L_080ce5ee:
	ldr	r3, [pc, #608]
	ldr	r1, [sp, #40]
	ldr	r3, [r3, #0]
	str	r3, [sp, #28]
	cmp	r1, #96
	bne.n	.L_080ce600
	movs	r0, #0
	bl	sub_080b50e8
.L_080ce600:
	ldr	r3, [pc, #552]
	add	r3, sl
	ldr	r3, [r3, #0]
	movs	r6, #211
	ldr	r3, [r3, #4]
	lsls	r6, r6, #7
	add	r6, sl
	cmp	r3, #0
	bne.n	.L_080ce640
	ldr	r2, [sp, #40]
	movs	r4, #0
	lsls	r5, r2, #11
.L_080ce618:
	adds	r0, r5, #0
	str	r4, [sp, #8]
	bl	sub_08002322
	lsls	r2, r0, #1
	adds	r2, r2, r0
	movs	r3, #192
	lsls	r2, r2, #1
	lsls	r3, r3, #11
	subs	r3, r3, r2
	asrs	r3, r3, #10
	ldr	r4, [sp, #8]
	stmia	r6!, {r3}
	movs	r3, #128
	lsls	r3, r3, #4
	adds	r4, #1
	adds	r5, r5, r3
	cmp	r4, #160
	bne.n	.L_080ce618
	b.n	.L_080ce666
.L_080ce640:
	ldr	r1, [sp, #40]
	movs	r4, #0
	lsls	r5, r1, #11
.L_080ce646:
	adds	r0, r5, #0
	str	r4, [sp, #8]
	bl	sub_08002322
	lsls	r3, r0, #1
	adds	r3, r3, r0
	ldr	r4, [sp, #8]
	lsls	r3, r3, #1
	movs	r2, #128
	asrs	r3, r3, #10
	lsls	r2, r2, #4
	adds	r4, #1
	stmia	r6!, {r3}
	adds	r5, r5, r2
	cmp	r4, #160
	bne.n	.L_080ce646
.L_080ce666:
	ldr	r2, [pc, #452]
	movs	r3, #0
	mov	r5, sl
	mov	fp, r3
	ldr	r3, [r5, r2]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	bne.n	.L_080ce678
	b.n	.L_080ce7d0
.L_080ce678:
	ldr	r1, [sp, #28]
	adds	r1, #12
	str	r1, [sp, #24]
	movs	r3, #48
	movs	r5, #36
	movs	r1, #0
	add	r3, sp
	str	r5, [sp, #16]
	str	r1, [sp, #12]
	mov	r9, r3
.L_080ce68c:
	mov	r3, sl
	adds	r6, r3, r2
	ldr	r5, [sp, #16]
	ldr	r3, [r6, #0]
	ldrsh	r0, [r3, r5]
	bl	sub_080b5098
	ldr	r5, [r0, #0]
	bl	sub_080049ac
	ldr	r0, [sp, #28]
	ldr	r1, [sp, #24]
	bl	sub_080051d8
	ldr	r3, [r5, #8]
	mov	r2, r9
	str	r3, [r2, #0]
	movs	r3, #160
	lsls	r3, r3, #13
	str	r3, [r2, #4]
	ldr	r3, [r5, #16]
	mov	r0, r9
	str	r3, [r2, #8]
	bl	sub_08004cb4
	mov	r3, fp
	ldr	r1, [sp, #40]
	lsls	r5, r3, #5
	cmp	r1, r5
	ble.n	.L_080ce7ae
	lsls	r0, r1, #9
	bl	sub_08004bd4
	adds	r3, r5, #0
	ldr	r2, [sp, #40]
	adds	r3, #32
	cmp	r2, r3
	bne.n	.L_080ce6ec
	ldr	r3, [r6, #0]
	ldr	r5, [sp, #16]
	ldrsh	r0, [r3, r5]
	movs	r3, #32
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	mov	r3, fp
	bl	sub_080d6888
.L_080ce6ec:
	mov	r2, fp
	lsls	r2, r2, #3
	movs	r3, #60
	ldr	r5, [sp, #12]
	ldr	r1, [pc, #340]
	add	r3, sp
	str	r2, [sp, #20]
	movs	r4, #0
	mov	r8, r3
	adds	r6, r5, r1
.L_080ce700:
	ldr	r2, [sp, #20]
	ldr	r5, [sp, #40]
	adds	r3, r2, r4
	lsls	r3, r3, #2
	cmp	r5, r3
	ble.n	.L_080ce7a6
	ldr	r3, [r6, #0]
	asrs	r3, r3, #8
	adds	r0, r3, #0
	muls	r0, r3
	ldr	r3, [r6, #4]
	asrs	r3, r3, #8
	adds	r2, r3, #0
	muls	r2, r3
	ldr	r3, [r6, #8]
	asrs	r3, r3, #8
	adds	r1, r3, #0
	muls	r1, r3
	adds	r0, r0, r2
	adds	r3, r1, #0
	adds	r0, r0, r3
	str	r4, [sp, #8]
	ldr	r3, [pc, #292]
	bl	sub_080072f0
	asrs	r7, r0, #8
	ldr	r4, [sp, #8]
	cmp	r7, #0
	beq.n	.L_080ce7a6
	mov	r1, r8
	adds	r0, r6, #0
	bl	sub_080e3944
	mov	r2, r8
	ldr	r5, [r2, #0]
	ldr	r4, [sp, #8]
	asrs	r5, r5, #1
	adds	r0, r4, #0
	str	r5, [r2, #0]
	movs	r1, #3
	bl	sub_080022fc
	mov	r2, r8
	lsls	r1, r0, #3
	ldr	r3, [r2, #4]
	adds	r1, r1, r0
	movs	r2, #24
	subs	r5, #12
	lsls	r1, r1, #6
	subs	r3, #12
	str	r2, [sp, #0]
	str	r2, [sp, #4]
	ldr	r0, [sp, #44]
	adds	r2, r5, #0
	add	r1, sl
	ldr	r5, [sp, #32]
	bl	sub_080072f8
	ldr	r5, [r6, #0]
	adds	r1, r7, #0
	adds	r0, r5, #0
	bl	sub_080022ec
	subs	r5, r5, r0
	str	r5, [r6, #0]
	ldr	r5, [r6, #4]
	adds	r1, r7, #0
	adds	r0, r5, #0
	bl	sub_080022ec
	subs	r5, r5, r0
	str	r5, [r6, #4]
	ldr	r5, [r6, #8]
	adds	r1, r7, #0
	adds	r0, r5, #0
	bl	sub_080022ec
	ldr	r3, [r6, #24]
	subs	r5, r5, r0
	adds	r3, #1
	str	r5, [r6, #8]
	str	r3, [r6, #24]
	ldr	r4, [sp, #8]
.L_080ce7a6:
	adds	r4, #1
	adds	r6, #28
	cmp	r4, #8
	bne.n	.L_080ce700
.L_080ce7ae:
	ldr	r1, [sp, #16]
	ldr	r2, [sp, #12]
	movs	r3, #224
	lsls	r3, r3, #3
	adds	r2, r2, r3
	adds	r1, #2
	str	r1, [sp, #16]
	str	r2, [sp, #12]
	ldr	r2, [pc, #108]
	mov	r1, sl
	ldr	r3, [r1, r2]
	movs	r5, #1
	ldr	r3, [r3, #20]
	add	fp, r5
	cmp	fp, r3
	beq.n	.L_080ce7d0
	b.n	.L_080ce68c
.L_080ce7d0:
	bl	sub_080cd52c
	ldr	r2, [pc, #128]
	movs	r3, #1
	add	r2, sl
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r2, [sp, #40]
	ldr	r3, [pc, #68]
	adds	r2, #1
	str	r2, [sp, #40]
	add	r3, sl
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #20]
	lsls	r3, r3, #5
	adds	r3, #96
	cmp	r2, r3
	beq.n	.L_080ce7fa
	b.n	.L_080ce5ee
.L_080ce7fa:
	ldr	r0, [pc, #76]
	bl	sub_08004278
	ldr	r0, [pc, #56]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080cdbc0
	add	sp, #72
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x000000a9
	.4byte 0x03001388
	.4byte 0x000000bb
	.4byte 0x080dbb9d
	.4byte 0x00007784
	.4byte 0x04040404
	.4byte 0x080cd261
	.4byte 0x02010000
	.4byte 0x03001e80
	.4byte 0x030001d8
	.4byte 0x00007824
