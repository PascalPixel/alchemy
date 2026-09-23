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
	.set sub_080049e8, 0x080049e8
	.set sub_08004a5c, 0x08004a5c
	.set sub_08004bd4, 0x08004bd4
	.set sub_08004c1c, 0x08004c1c
	.set sub_08004c6c, 0x08004c6c
	.set sub_08004cb4, 0x08004cb4
	.set sub_080051d8, 0x080051d8
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_08007300, 0x08007300
	.set sub_080b5078, 0x080b5078
	.set sub_080b5088, 0x080b5088
	.set sub_080b5098, 0x080b5098
	.set sub_080b50e8, 0x080b50e8
	.set sub_080cd52c, 0x080cd52c
	.set sub_080cd594, 0x080cd594
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080cef64, 0x080cef64
	.set sub_080d6888, 0x080d6888
	.set sub_080e0524, 0x080e0524
	.set sub_080e155c, 0x080e155c
	.set sub_080e38b8, 0x080e38b8
	.set sub_080e3944, 0x080e3944
	.set sub_080e396c, 0x080e396c
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global Func_080d52c8
	.thumb_func
Func_080d52c8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #532]
	mov	r8, r1
	ldmia	r3!, {r1}
	ldr	r3, [r3, #0]
	sub	sp, #116
	str	r3, [sp, #68]
	ldr	r3, [pc, #524]
	mov	r9, r1
	movs	r2, #16
	add	r3, r9
	str	r2, [sp, #52]
	str	r0, [r3, #0]
	movs	r0, #0
	bl	sub_080cd594
	movs	r3, #1
	ldr	r0, [pc, #508]
	mov	r1, r9
	movs	r2, #1
	bl	sub_080e0524
	mov	r3, r8
	cmp	r3, #0
	bne.n	.L_080d530a
	ldr	r0, [pc, #496]
	b.n	.L_080d536e
.L_080d530a:
	mov	r4, r8
	cmp	r4, #1
	bne.n	.L_080d5314
	ldr	r0, [pc, #488]
	b.n	.L_080d536e
.L_080d5314:
	mov	r0, r8
	cmp	r0, #2
	bne.n	.L_080d531e
	ldr	r0, [pc, #484]
	b.n	.L_080d536e
.L_080d531e:
	mov	r1, r8
	cmp	r1, #3
	bne.n	.L_080d5328
	ldr	r0, [pc, #476]
	b.n	.L_080d536e
.L_080d5328:
	mov	r2, r8
	cmp	r2, #4
	bne.n	.L_080d5332
	ldr	r0, [pc, #460]
	b.n	.L_080d536e
.L_080d5332:
	mov	r3, r8
	cmp	r3, #5
	bne.n	.L_080d5340
	ldr	r5, [pc, #460]
	mov	r1, r9
	adds	r0, r5, #0
	b.n	.L_080d5364
.L_080d5340:
	mov	r4, r8
	cmp	r4, #7
	bne.n	.L_080d535a
	movs	r0, #24
	str	r0, [sp, #52]
	mov	r1, r9
	ldr	r0, [pc, #440]
	movs	r2, #1
	movs	r3, #0
	bl	sub_080e0524
	ldr	r0, [pc, #436]
	b.n	.L_080d536e
.L_080d535a:
	ldr	r5, [pc, #436]
	movs	r1, #32
	str	r1, [sp, #52]
	adds	r0, r5, #0
	mov	r1, r9
.L_080d5364:
	movs	r2, #1
	movs	r3, #0
	bl	sub_080e0524
	adds	r0, r5, #0
.L_080d536e:
	bl	sub_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	movs	r2, #128
	ldr	r3, [pc, #408]
	lsls	r0, r0, #19
	bl	sub_080072f0
	mov	r2, r8
	cmp	r2, #4
	bne.n	.L_080d5392
	ldr	r0, [pc, #400]
	mov	r1, r9
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
.L_080d5392:
	mov	r3, r8
	cmp	r3, #3
	bne.n	.L_080d53a8
	movs	r1, #150
	lsls	r1, r1, #6
	ldr	r0, [pc, #380]
	add	r1, r9
	movs	r2, #1
	movs	r3, #0
	bl	sub_080e0524
.L_080d53a8:
	movs	r4, #0
	ldr	r5, [pc, #372]
	mov	fp, r4
	movs	r6, #255
.L_080d53b0:
	mov	r0, r8
	cmp	r0, #1
	bls.n	.L_080d53c6
	cmp	r0, #4
	beq.n	.L_080d53c6
	cmp	r0, #5
	beq.n	.L_080d53c6
	cmp	r0, #6
	beq.n	.L_080d53c6
	cmp	r0, #7
	bne.n	.L_080d53ea
.L_080d53c6:
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #15
	str	r0, [r5, #0]
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #14
	str	r0, [r5, #4]
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #15
	b.n	.L_080d540c
.L_080d53ea:
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #13
	str	r0, [r5, #0]
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #255
	lsls	r0, r0, #13
	str	r0, [r5, #4]
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #13
.L_080d540c:
	str	r0, [r5, #8]
	movs	r1, #1
	movs	r2, #128
	movs	r3, #0
	add	fp, r1
	lsls	r2, r2, #2
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	fp, r2
	bne.n	.L_080d53b0
	mov	r3, r8
	cmp	r3, #1
	bls.n	.L_080d5436
	cmp	r3, #4
	beq.n	.L_080d5436
	cmp	r3, #5
	beq.n	.L_080d5436
	cmp	r3, #6
	beq.n	.L_080d5436
	cmp	r3, #7
	bne.n	.L_080d5444
.L_080d5436:
	ldr	r3, [pc, #184]
	add	r3, r9
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #20]
	lsls	r3, r3, #3
	adds	r3, #64
	b.n	.L_080d5450
.L_080d5444:
	ldr	r3, [pc, #168]
	add	r3, r9
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #20]
	lsls	r3, r3, #3
	adds	r3, #32
.L_080d5450:
	str	r3, [sp, #60]
	mov	r4, r8
	cmp	r4, #1
	bls.n	.L_080d5466
	cmp	r4, #3
	beq.n	.L_080d5466
	movs	r1, #144
	ldr	r0, [pc, #196]
	lsls	r1, r1, #3
	bl	sub_080041d8
.L_080d5466:
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, r9
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #180]
	movs	r3, #75
	add	r2, r9
	movs	r1, #144
	lsls	r1, r1, #3
	str	r3, [r2, #0]
	ldr	r0, [pc, #172]
	bl	sub_080041d8
	movs	r0, #142
	bl	sub_080f9010
	ldr	r1, [sp, #60]
	movs	r0, #0
	str	r0, [sp, #64]
	cmp	r1, #0
	bne.n	.L_080d5494
	b.n	.L_080d5972
.L_080d5494:
	subs	r1, #32
	str	r1, [sp, #24]
.L_080d5498:
	ldr	r3, [pc, #148]
	ldr	r3, [r3, #0]
	mov	r2, r8
	str	r3, [sp, #48]
	cmp	r2, #7
	bne.n	.L_080d5538
	ldr	r3, [sp, #60]
	ldr	r4, [sp, #64]
	subs	r3, #46
	cmp	r4, r3
	bne.n	.L_080d54c2
	ldr	r3, [pc, #64]
	add	r3, r9
	ldr	r3, [r3, #0]
	movs	r2, #36
	ldrsh	r1, [r3, r2]
	ldr	r0, [r3, #8]
	movs	r2, #16
	movs	r3, #0
	bl	sub_080b5078
.L_080d54c2:
	ldr	r3, [sp, #64]
	ldr	r4, [sp, #24]
	cmp	r3, r4
	bne.n	.L_080d5546
	movs	r0, #134
	bl	sub_080b50e8
	ldr	r3, [pc, #28]
	add	r3, r9
	ldr	r3, [r3, #0]
	movs	r1, #36
	ldrsh	r0, [r3, r1]
	movs	r1, #4
	bl	sub_080b5088
	ldr	r2, [pc, #80]
	movs	r3, #8
	add	r2, r9
	str	r3, [r2, #0]
	b.n	.L_080d5546
	movs	r0, r0
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x0000009e
	.4byte 0x000000a0
	.4byte 0x000000bb
	.4byte 0x000000a3
	.4byte 0x000000c0
	.4byte 0x000000b7
	.4byte 0x0000008d
	.4byte 0x000000cd
	.4byte 0x03001388
	.4byte 0x000000aa
	.4byte 0x000000ce
	.4byte 0x02010000
	.4byte 0x080dbb9d
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x03001e80
	.2byte 0x77a8
	.2byte 0x0000
.L_080d5538:
	ldr	r2, [sp, #64]
	ldr	r3, [sp, #24]
	cmp	r2, r3
	bne.n	.L_080d5546
	movs	r0, #133
	bl	sub_080b50e8
.L_080d5546:
	movs	r6, #211
	ldr	r0, [sp, #64]
	lsls	r6, r6, #7
	movs	r4, #0
	movs	r7, #128
	add	r6, r9
	mov	fp, r4
	lsls	r7, r7, #11
	lsls	r5, r0, #12
.L_080d5558:
	adds	r0, r5, #0
	bl	sub_08002322
	movs	r2, #1
	lsls	r0, r0, #2
	subs	r0, r7, r0
	movs	r1, #128
	add	fp, r2
	asrs	r0, r0, #10
	lsls	r1, r1, #4
	mov	r3, fp
	stmia	r6!, {r0}
	adds	r5, r5, r1
	cmp	r3, #160
	bne.n	.L_080d5558
	movs	r4, #0
	str	r4, [sp, #56]
	ldr	r2, [pc, #792]
	mov	r0, r9
	ldr	r3, [r0, r2]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	bne.n	.L_080d5588
	b.n	.L_080d594a
.L_080d5588:
	ldr	r3, [sp, #48]
	mov	r1, sp
	adds	r1, #72
	adds	r3, #12
	mov	r4, sp
	ldr	r0, [pc, #772]
	str	r1, [sp, #32]
	str	r3, [sp, #36]
	adds	r4, #92
	movs	r1, #36
	movs	r3, #0
	str	r4, [sp, #28]
	str	r0, [sp, #40]
	str	r1, [sp, #16]
	str	r3, [sp, #12]
.L_080d55a6:
	mov	r4, r9
	adds	r5, r4, r2
	ldr	r3, [r5, #0]
	ldr	r1, [sp, #16]
	ldrsh	r0, [r3, r1]
	bl	sub_080b5098
	ldr	r3, [sp, #56]
	ldr	r0, [r0, #0]
	lsls	r3, r3, #3
	mov	r4, r8
	str	r0, [sp, #44]
	str	r3, [sp, #20]
	cmp	r4, #3
	bne.n	.L_080d569a
	ldr	r0, [sp, #12]
	ldr	r1, [sp, #64]
	str	r0, [sp, #20]
	cmp	r1, r0
	ble.n	.L_080d569a
	adds	r3, r0, #0
	adds	r3, #32
	cmp	r1, r3
	bge.n	.L_080d569a
	ldr	r3, [r5, #0]
	ldr	r2, [sp, #16]
	add	r5, sp, #80
	ldrsh	r0, [r3, r2]
	adds	r1, r5, #0
	bl	sub_080e396c
	ldr	r2, [pc, #692]
	ldr	r7, [sp, #64]
	movs	r0, #0
	mov	r1, r8
	mov	fp, r0
	ands	r7, r1
	mov	sl, r2
.L_080d55f2:
	bl	sub_08004458
	ldr	r3, [pc, #680]
	adds	r2, r0, #0
	ands	r2, r3
	str	r2, [sp, #8]
	bl	sub_08004458
	ldr	r2, [sp, #8]
	movs	r5, #31
	ands	r5, r0
	adds	r0, r2, #0
	bl	sub_08002322
	ldr	r6, [sp, #80]
	adds	r5, #4
	lsrs	r3, r6, #31
	adds	r6, r6, r3
	adds	r3, r5, #0
	muls	r3, r0
	ldr	r4, [pc, #648]
	asrs	r3, r3, #17
	asrs	r6, r6, #1
	ldr	r2, [sp, #8]
	adds	r6, r6, r3
	ldrb	r3, [r4, r7]
	adds	r0, r2, #0
	lsrs	r3, r3, #1
	subs	r6, r6, r3
	bl	sub_0800231c
	adds	r3, r5, #0
	muls	r3, r0
	ldr	r5, [sp, #84]
	asrs	r3, r3, #16
	mov	r0, sl
	subs	r5, r5, r3
	ldrb	r3, [r0, r7]
	lsrs	r3, r3, #1
	subs	r5, r5, r3
	bl	sub_08004458
	ldr	r3, [pc, #608]
	movs	r1, #3
	ands	r0, r1
	ldrb	r2, [r3, r0]
	movs	r3, #3
	orrs	r3, r2
	movs	r2, #2
	str	r2, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r0, #47
	bl	sub_080ed408
	ldr	r2, [pc, #584]
	ldr	r4, [pc, #576]
	lsls	r3, r7, #1
	ldrh	r1, [r2, r3]
	ldrb	r3, [r4, r7]
	str	r3, [sp, #0]
	mov	r0, sl
	ldrb	r3, [r0, r7]
	movs	r2, #150
	lsls	r2, r2, #6
	add	r1, r9
	str	r3, [sp, #4]
	adds	r1, r1, r2
	ldr	r2, [pc, #564]
	adds	r5, #16
	ldr	r4, [r2, #0]
	adds	r3, r5, #0
	ldr	r0, [sp, #68]
	adds	r2, r6, #0
	bl	sub_080072f4
	movs	r0, #47
	bl	sub_08002dd8
	movs	r3, #1
	add	fp, r3
	mov	r4, fp
	cmp	r4, #2
	bne.n	.L_080d55f2
.L_080d569a:
	movs	r3, #3
	movs	r1, #7
	movs	r2, #7
	movs	r0, #46
	str	r3, [sp, #0]
	bl	sub_080ed408
	ldr	r3, [sp, #40]
	adds	r3, #184
	ldr	r3, [r3, #0]
	str	r3, [sp, #72]
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r2, #7
	movs	r1, #7
	movs	r3, #3
	movs	r0, #47
	bl	sub_080ed408
	ldr	r0, [pc, #492]
	ldr	r1, [sp, #32]
	ldr	r3, [r0, #0]
	str	r3, [r1, #4]
	bl	sub_080049ac
	ldr	r0, [sp, #48]
	ldr	r1, [sp, #36]
	bl	sub_080051d8
	ldr	r2, [sp, #44]
	ldr	r4, [sp, #28]
	ldr	r3, [r2, #8]
	str	r3, [r4, #0]
	movs	r3, #160
	lsls	r3, r3, #13
	str	r3, [r4, #4]
	ldr	r3, [r2, #16]
	str	r3, [r4, #8]
	ldr	r0, [sp, #28]
	bl	sub_08004cb4
	ldr	r0, [sp, #64]
	ldr	r1, [sp, #12]
	cmp	r0, r1
	bgt.n	.L_080d56f6
	b.n	.L_080d58f4
.L_080d56f6:
	lsls	r5, r0, #9
	adds	r0, r5, #0
	bl	sub_08004c1c
	mov	r2, r8
	cmp	r2, #1
	bls.n	.L_080d5708
	cmp	r2, #4
	bne.n	.L_080d570e
.L_080d5708:
	adds	r0, r5, #0
	bl	sub_08004bd4
.L_080d570e:
	ldr	r4, [sp, #52]
	movs	r3, #0
	mov	fp, r3
	cmp	r4, #0
	bne.n	.L_080d571a
	b.n	.L_080d58f4
.L_080d571a:
	ldr	r0, [sp, #56]
	lsls	r2, r0, #6
	lsls	r3, r0, #9
	subs	r3, r3, r2
	ldr	r1, [pc, #400]
	lsls	r3, r3, #2
	adds	r7, r3, r1
.L_080d5728:
	ldr	r3, [sp, #20]
	ldr	r2, [sp, #64]
	add	r3, fp
	cmp	r2, r3
	bgt.n	.L_080d5734
	b.n	.L_080d58e6
.L_080d5734:
	ldr	r3, [r7, #0]
	asrs	r3, r3, #8
	adds	r0, r3, #0
	muls	r0, r3
	ldr	r3, [r7, #4]
	asrs	r3, r3, #8
	adds	r2, r3, #0
	muls	r2, r3
	ldr	r3, [r7, #8]
	asrs	r3, r3, #8
	adds	r4, r3, #0
	muls	r4, r3
	adds	r0, r0, r2
	adds	r3, r4, #0
	adds	r0, r0, r3
	ldr	r3, [pc, #356]
	bl	sub_080072f0
	asrs	r0, r0, #9
	mov	sl, r0
	cmp	r0, #0
	bne.n	.L_080d5762
	b.n	.L_080d58e6
.L_080d5762:
	ldr	r3, [r7, #24]
	cmp	r3, #23
	ble.n	.L_080d576a
	b.n	.L_080d58e6
.L_080d576a:
	adds	r1, r3, #0
	cmp	r1, #0
	bge.n	.L_080d5772
	adds	r1, #3
.L_080d5772:
	add	r5, sp, #104
	asrs	r6, r1, #2
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	sub_080e3944
	ldr	r3, [r5, #0]
	mov	r0, r8
	asrs	r2, r3, #1
	str	r2, [r5, #0]
	cmp	r0, #5
	beq.n	.L_080d578e
	cmp	r0, #7
	bne.n	.L_080d57b2
.L_080d578e:
	lsls	r1, r6, #1
	adds	r1, r1, r6
	movs	r0, #40
	lsls	r1, r1, #3
	ldr	r3, [r5, #4]
	adds	r1, r1, r6
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	ldr	r0, [sp, #32]
	lsls	r1, r1, #6
	ldr	r4, [r0, #4]
	add	r1, r9
	subs	r2, #20
	subs	r3, #20
	ldr	r0, [sp, #68]
	bl	sub_080072f4
	b.n	.L_080d5828
.L_080d57b2:
	mov	r1, r8
	cmp	r1, #6
	bne.n	.L_080d57d8
	movs	r1, #12
	ldr	r3, [r5, #4]
	str	r1, [sp, #0]
	movs	r1, #24
	str	r1, [sp, #4]
	ldr	r0, [sp, #32]
	movs	r1, #192
	lsls	r1, r1, #4
	ldr	r4, [r0, #4]
	subs	r2, #6
	subs	r3, #12
	ldr	r0, [sp, #68]
	add	r1, r9
	bl	sub_080072f4
	b.n	.L_080d5828
.L_080d57d8:
	mov	r1, r8
	cmp	r1, #4
	bne.n	.L_080d57fa
	movs	r1, #22
	ldr	r3, [r5, #4]
	str	r1, [sp, #0]
	movs	r1, #42
	str	r1, [sp, #4]
	ldr	r0, [sp, #32]
	subs	r2, #11
	ldr	r4, [r0, #4]
	subs	r3, #21
	ldr	r0, [sp, #68]
	mov	r1, r9
	bl	sub_080072f4
	b.n	.L_080d5828
.L_080d57fa:
	mov	r1, fp
	movs	r3, #3
	ands	r3, r1
	negs	r4, r3
	movs	r0, #24
	orrs	r4, r3
	lsls	r1, r6, #3
	ldr	r3, [r5, #4]
	str	r0, [sp, #0]
	movs	r0, #48
	str	r0, [sp, #4]
	lsrs	r4, r4, #31
	ldr	r0, [sp, #32]
	adds	r1, r1, r6
	lsls	r4, r4, #2
	lsls	r1, r1, #7
	ldr	r4, [r4, r0]
	add	r1, r9
	subs	r2, #12
	subs	r3, #24
	ldr	r0, [sp, #68]
	bl	sub_080072f4
.L_080d5828:
	mov	r1, r8
	cmp	r1, #1
	bls.n	.L_080d583a
	cmp	r1, #4
	beq.n	.L_080d583a
	cmp	r1, #5
	beq.n	.L_080d583a
	cmp	r1, #6
	bne.n	.L_080d5866
.L_080d583a:
	ldr	r5, [r7, #0]
	mov	r1, sl
	adds	r0, r5, #0
	bl	sub_080022ec
	subs	r5, r5, r0
	str	r5, [r7, #0]
	ldr	r5, [r7, #4]
	mov	r1, sl
	adds	r0, r5, #0
	bl	sub_080022ec
	subs	r5, r5, r0
	str	r5, [r7, #4]
	ldr	r5, [r7, #8]
	mov	r1, sl
	adds	r0, r5, #0
	bl	sub_080022ec
	subs	r5, r5, r0
	str	r5, [r7, #8]
	b.n	.L_080d5870
.L_080d5866:
	ldr	r3, [r7, #4]
	movs	r2, #128
	lsls	r2, r2, #9
	adds	r3, r3, r2
	str	r3, [r7, #4]
.L_080d5870:
	ldr	r3, [r7, #24]
	adds	r3, #1
	str	r3, [r7, #24]
	cmp	r3, #24
	bne.n	.L_080d58e6
	mov	r3, r8
	cmp	r3, #1
	bls.n	.L_080d588c
	cmp	r3, #4
	beq.n	.L_080d588c
	cmp	r3, #5
	beq.n	.L_080d588c
	cmp	r3, #6
	bne.n	.L_080d58bc
.L_080d588c:
	movs	r3, #0
	str	r3, [r7, #24]
	b.n	.L_080d58e6
	movs	r0, r0
	.4byte 0x00007828
	.4byte 0x03001e50
	.4byte 0x080eded0
	.4byte 0x0000ffff
	.4byte 0x080edeca
	.4byte 0x080ee2ae
	.4byte 0x080edebe
	.4byte 0x03001f0c
	.4byte 0x02010000
	.2byte 0x01d8
	.2byte 0x0300
.L_080d58bc:
	bl	sub_08004458
	movs	r4, #255
	ands	r0, r4
	subs	r0, #127
	lsls	r0, r0, #13
	str	r0, [r7, #0]
	bl	sub_08004458
	movs	r1, #255
	ands	r0, r1
	subs	r0, #255
	lsls	r0, r0, #12
	str	r0, [r7, #4]
	bl	sub_08004458
	movs	r2, #255
	ands	r0, r2
	subs	r0, #127
	lsls	r0, r0, #13
	str	r0, [r7, #8]
.L_080d58e6:
	movs	r3, #1
	ldr	r4, [sp, #52]
	add	fp, r3
	adds	r7, #28
	cmp	fp, r4
	beq.n	.L_080d58f4
	b.n	.L_080d5728
.L_080d58f4:
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	ldr	r3, [sp, #12]
	ldr	r0, [sp, #64]
	adds	r3, #16
	cmp	r0, r3
	bne.n	.L_080d592a
	ldr	r1, [sp, #60]
	subs	r2, r1, r0
	cmp	r2, #31
	ble.n	.L_080d5914
	movs	r2, #31
.L_080d5914:
	ldr	r3, [pc, #136]
	add	r3, r9
	ldr	r3, [r3, #0]
	ldr	r4, [sp, #16]
	ldrsh	r0, [r3, r4]
	str	r2, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	ldr	r3, [sp, #56]
	bl	sub_080d6888
.L_080d592a:
	ldr	r2, [sp, #16]
	ldr	r3, [sp, #12]
	ldr	r4, [sp, #56]
	adds	r2, #2
	adds	r3, #8
	adds	r4, #1
	str	r2, [sp, #16]
	str	r3, [sp, #12]
	str	r4, [sp, #56]
	ldr	r2, [pc, #96]
	mov	r0, r9
	ldr	r3, [r0, r2]
	ldr	r3, [r3, #20]
	cmp	r4, r3
	beq.n	.L_080d594a
	b.n	.L_080d55a6
.L_080d594a:
	movs	r1, #16
	movs	r0, #16
	bl	sub_080e155c
	bl	sub_080cd52c
	ldr	r2, [pc, #76]
	movs	r3, #1
	add	r2, r9
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r1, [sp, #64]
	ldr	r2, [sp, #60]
	adds	r1, #1
	str	r1, [sp, #64]
	cmp	r1, r2
	beq.n	.L_080d5972
	b.n	.L_080d5498
.L_080d5972:
	ldr	r0, [pc, #52]
	bl	sub_08004278
	mov	r3, r8
	cmp	r3, #1
	bls.n	.L_080d5988
	cmp	r3, #3
	beq.n	.L_080d5988
	ldr	r0, [pc, #40]
	bl	sub_08004278
.L_080d5988:
	bl	sub_080cdbc0
	add	sp, #116
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00007828
	.4byte 0x00007824
	.4byte 0x080cd261
	.2byte 0xbb9d
	.2byte 0x080d
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r6, [pc, #92]
	adds	r3, r6, #0
	ldmia	r3!, {r1}
	ldr	r3, [r3, #0]
	sub	sp, #40
	str	r3, [sp, #24]
	ldr	r3, [pc, #84]
	mov	r9, r1
	add	r3, r9
	str	r0, [r3, #0]
	movs	r0, #0
	bl	sub_080cd594
	ldr	r2, [pc, #72]
	ldr	r3, [pc, #56]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #56]
	adds	r2, #48
	strh	r3, [r2, #0]
	ldr	r0, [pc, #64]
	mov	r1, r9
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	movs	r5, #1
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r5, [sp, #0]
	bl	sub_080ed408
	ldr	r2, [r6, #28]
	movs	r0, #47
	str	r2, [sp, #12]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #15
	str	r5, [sp, #0]
	bl	sub_080ed408
	b.n	.L_080d5a2c
	.4byte 0x00000100
	.4byte 0x00000000
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x04000020
	.2byte 0x00a8
	.2byte 0x0000
.L_080d5a2c:
	ldr	r6, [r6, #32]
	ldr	r5, [pc, #488]
	str	r6, [sp, #16]
	movs	r7, #0
.L_080d5a34:
	bl	sub_08004458
	movs	r3, #63
	ands	r3, r0
	adds	r3, #32
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	ldr	r3, [pc, #472]
	movs	r6, #0
	str	r3, [r5, #4]
	bl	sub_08004458
	str	r6, [r5, #16]
	bl	sub_08004458
	movs	r3, #3
	ands	r3, r0
	str	r3, [r5, #8]
	bl	sub_08004458
	movs	r3, #255
	ands	r3, r0
	adds	r7, #1
	str	r3, [r5, #24]
	adds	r5, #28
	cmp	r7, #32
	bne.n	.L_080d5a34
	ldr	r3, [pc, #436]
	add	r3, r9
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L_080d5a7c
	ldr	r2, [pc, #428]
	ldr	r3, [pc, #428]
	str	r3, [r2, #0]
.L_080d5a7c:
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, r9
	movs	r3, #1
	str	r3, [r2, #0]
	ldr	r3, [pc, #420]
	movs	r1, #144
	add	r3, r9
	str	r6, [r3, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #412]
	bl	sub_080041d8
	movs	r0, #142
	bl	sub_080f9010
	movs	r3, #0
	str	r3, [sp, #20]
.L_080d5aa0:
	ldr	r3, [pc, #400]
	ldr	r4, [sp, #20]
	ldr	r3, [r3, #0]
	mov	fp, r3
	cmp	r4, #80
	bne.n	.L_080d5ab2
	movs	r0, #0
	bl	sub_080b50e8
.L_080d5ab2:
	ldr	r2, [pc, #364]
	mov	r1, r9
	ldr	r3, [r1, r2]
	ldr	r3, [r3, #20]
	movs	r0, #0
	mov	sl, r0
	cmp	r3, #0
	beq.n	.L_080d5b2c
	mov	r3, fp
	adds	r3, #12
	movs	r4, #36
	str	r3, [sp, #8]
	add	r6, sp, #28
	mov	r8, r4
	adds	r7, r1, r2
.L_080d5ad0:
	ldr	r3, [r7, #0]
	mov	r1, r8
	ldrsh	r0, [r3, r1]
	bl	sub_080b5098
	ldr	r5, [r0, #0]
	bl	sub_080049ac
	mov	r0, fp
	ldr	r1, [sp, #8]
	bl	sub_080051d8
	ldr	r3, [r5, #8]
	str	r3, [r6, #0]
	movs	r3, #160
	lsls	r3, r3, #14
	str	r3, [r6, #4]
	ldr	r3, [r5, #16]
	adds	r0, r6, #0
	str	r3, [r6, #8]
	bl	sub_08004cb4
	mov	r4, sl
	lsls	r3, r4, #4
	ldr	r0, [sp, #20]
	adds	r3, #64
	cmp	r0, r3
	bne.n	.L_080d5b1c
	ldr	r3, [r7, #0]
	mov	r1, r8
	ldrsh	r0, [r3, r1]
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #5
	subs	r3, #1
	bl	sub_080d6888
.L_080d5b1c:
	movs	r3, #2
	add	r8, r3
	ldr	r3, [r7, #0]
	movs	r4, #1
	ldr	r3, [r3, #20]
	add	sl, r4
	cmp	sl, r3
	bne.n	.L_080d5ad0
.L_080d5b2c:
	movs	r0, #32
	ldr	r6, [pc, #232]
	movs	r7, #0
	mov	r8, r0
.L_080d5b34:
	ldr	r1, [sp, #20]
	lsls	r3, r7, #2
	cmp	r1, r3
	ble.n	.L_080d5bcc
	ldr	r0, [r6, #4]
	ldr	r2, [pc, #248]
	cmp	r0, r2
	bgt.n	.L_080d5bcc
	ldr	r1, [r6, #24]
	cmp	r1, #0
	bge.n	.L_080d5b4c
	adds	r1, #15
.L_080d5b4c:
	asrs	r1, r1, #4
	movs	r3, #7
	ands	r1, r3
	cmp	r1, #3
	bgt.n	.L_080d5b74
	movs	r3, #2
	ldrsh	r2, [r6, r3]
	mov	r4, r8
	asrs	r3, r0, #16
	lsls	r1, r1, #10
	str	r4, [sp, #0]
	str	r4, [sp, #4]
	add	r1, r9
	subs	r2, #16
	subs	r3, #16
	ldr	r0, [sp, #24]
	ldr	r4, [sp, #12]
	bl	sub_080072f4
	b.n	.L_080d5b94
.L_080d5b74:
	ldr	r2, [pc, #196]
	lsls	r1, r1, #10
	add	r1, r9
	adds	r1, r1, r2
	movs	r3, #2
	ldrsh	r2, [r6, r3]
	mov	r4, r8
	asrs	r3, r0, #16
	str	r4, [sp, #0]
	str	r4, [sp, #4]
	subs	r2, #16
	subs	r3, #16
	ldr	r0, [sp, #24]
	ldr	r4, [sp, #16]
	bl	sub_080072f4
.L_080d5b94:
	ldr	r4, [r6, #16]
	movs	r0, #128
	lsls	r0, r0, #6
	ldr	r2, [r6, #4]
	adds	r1, r4, r0
	ldr	r3, [r6, #24]
	ldr	r0, [r6, #8]
	movs	r5, #184
	adds	r2, r2, r4
	adds	r3, r3, r0
	lsls	r5, r5, #15
	str	r2, [r6, #4]
	str	r1, [r6, #16]
	str	r3, [r6, #24]
	cmp	r2, r5
	ble.n	.L_080d5bcc
	cmp	r1, #0
	bne.n	.L_080d5bcc
	ldr	r1, [pc, #132]
	adds	r3, r4, r1
	adds	r2, r0, #4
	negs	r3, r3
	str	r2, [r6, #8]
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r5, [r6, #4]
	str	r3, [r6, #16]
.L_080d5bcc:
	adds	r7, #1
	adds	r6, #28
	cmp	r7, #12
	bne.n	.L_080d5b34
	ldr	r2, [pc, #108]
	movs	r3, #1
	add	r2, r9
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r2, [sp, #20]
	adds	r2, #1
	str	r2, [sp, #20]
	cmp	r2, #148
	beq.n	.L_080d5bee
	b.n	.L_080d5aa0
.L_080d5bee:
	ldr	r0, [pc, #64]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080cdbc0
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02010000
	.4byte 0xffe00000
	.4byte 0x00007828
	.4byte 0x04000028
	.4byte 0xffff9000
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x03001e80
	.4byte 0x007fffff
	.4byte 0xfffff000
	.4byte 0x00002001
	.2byte 0x7824
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r7, [pc, #72]
	ldr	r1, [r7, #0]
	sub	sp, #36
	str	r1, [sp, #20]
	subs	r3, r7, #4
	ldr	r3, [r3, #0]
	ldr	r6, [pc, #64]
	mov	r8, r3
	add	r6, r8
	str	r0, [r6, #0]
	movs	r0, #1
	bl	sub_080cd594
	ldr	r2, [pc, #52]
	ldr	r3, [pc, #36]
	strh	r3, [r2, #0]
	ldr	r3, [pc, #36]
	adds	r2, #48
	strh	r3, [r2, #0]
	ldr	r0, [pc, #44]
	movs	r3, #1
	mov	r1, r8
	movs	r2, #1
	bl	sub_080e0524
	ldr	r3, [r6, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L_080d5cb8
	ldr	r2, [pc, #28]
	ldr	r3, [pc, #32]
	str	r3, [r2, #0]
	b.n	.L_080d5cb8
	.4byte 0x00000100
	.4byte 0x00000000
	.4byte 0x03001ef0
	.4byte 0x00007828
	.4byte 0x04000020
	.4byte 0x0000007a
	.4byte 0x04000028
	.2byte 0x9000
	.2byte 0xffff
.L_080d5cb8:
	.2byte 0x2501
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r5, [sp, #0]
	bl	sub_080ed408
	movs	r3, #239
	lsls	r3, r3, #7
	ldr	r2, [pc, #368]
	add	r3, r8
	str	r5, [r3, #0]
	add	r2, r8
	movs	r3, #0
	ldr	r7, [r7, #24]
	movs	r1, #144
	str	r3, [r2, #0]
	ldr	r0, [pc, #356]
	lsls	r1, r1, #3
	str	r7, [sp, #12]
	bl	sub_080041d8
	ldr	r3, [r6, #0]
	ldr	r3, [r3, #20]
	movs	r2, #0
	mov	sl, r2
	cmp	r3, #0
	beq.n	.L_080d5d12
	ldr	r5, [pc, #340]
	movs	r6, #63
	add	r5, r8
.L_080d5cf8:
	bl	sub_08004458
	movs	r3, #1
	add	sl, r3
	ldr	r3, [pc, #328]
	ands	r0, r6
	add	r3, r8
	str	r0, [r5, #0]
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #20]
	adds	r5, #28
	cmp	sl, r3
	bne.n	.L_080d5cf8
.L_080d5d12:
	ldr	r3, [pc, #312]
	add	r3, r8
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #20]
	movs	r1, #32
	movs	r0, #0
	lsls	r3, r3, #5
	negs	r1, r1
	mov	r9, r0
	cmp	r3, r1
	beq.n	.L_080d5e1e
.L_080d5d28:
	mov	r2, r9
	cmp	r2, #32
	bne.n	.L_080d5d34
	movs	r0, #0
	bl	sub_080b50e8
.L_080d5d34:
	ldr	r2, [pc, #276]
	movs	r3, #0
	add	r2, r8
	mov	sl, r3
	ldr	r3, [r2, #0]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	beq.n	.L_080d5dfc
	movs	r0, #225
	lsls	r0, r0, #7
	add	r0, r8
	str	r0, [sp, #8]
	mov	fp, r2
.L_080d5d4e:
	mov	r1, sl
	lsls	r6, r1, #4
	cmp	r9, r6
	bne.n	.L_080d5d5c
	movs	r0, #143
	bl	sub_080f9010
.L_080d5d5c:
	cmp	r9, r6
	blt.n	.L_080d5de8
	adds	r3, r6, #0
	adds	r3, #72
	cmp	r9, r3
	bge.n	.L_080d5de8
	mov	r3, fp
	mov	r0, sl
	ldr	r2, [r3, #0]
	lsls	r3, r0, #1
	adds	r3, #36
	add	r5, sp, #24
	ldrsh	r0, [r2, r3]
	adds	r1, r5, #0
	bl	sub_080e396c
	mov	r2, fp
	ldr	r3, [r2, #0]
	ldr	r3, [r3, #4]
	cmp	r3, #1
	bne.n	.L_080d5d8c
	ldr	r3, [r5, #0]
	subs	r3, #112
	str	r3, [r5, #0]
.L_080d5d8c:
	ldr	r3, [r5, #4]
	adds	r2, r3, #0
	subs	r2, #16
	movs	r1, #20
	str	r2, [r5, #4]
	ldr	r2, [r5, #0]
	str	r1, [sp, #4]
	movs	r1, #216
	movs	r0, #16
	lsls	r1, r1, #3
	str	r0, [sp, #0]
	subs	r2, #8
	subs	r3, #20
	ldr	r7, [sp, #12]
	ldr	r0, [sp, #20]
	add	r1, r8
	bl	sub_08007300
	cmp	r9, r6
	blt.n	.L_080d5de8
	ldr	r2, [sp, #8]
	mov	r1, r9
	ldr	r3, [r2, #24]
	subs	r0, r1, r6
	adds	r0, r0, r3
	movs	r1, #6
	bl	sub_080022ec
	movs	r1, #9
	bl	sub_080022fc
	lsls	r1, r0, #1
	ldr	r2, [r5, #0]
	ldr	r3, [r5, #4]
	adds	r1, r1, r0
	movs	r0, #16
	lsls	r1, r1, #6
	str	r0, [sp, #0]
	movs	r0, #12
	str	r0, [sp, #4]
	add	r1, r8
	subs	r2, #8
	subs	r3, #16
	ldr	r0, [sp, #20]
	bl	sub_08007300
.L_080d5de8:
	ldr	r1, [sp, #8]
	adds	r1, #28
	str	r1, [sp, #8]
	mov	r0, fp
	ldr	r3, [r0, #0]
	movs	r2, #1
	ldr	r3, [r3, #20]
	add	sl, r2
	cmp	sl, r3
	bne.n	.L_080d5d4e
.L_080d5dfc:
	ldr	r2, [pc, #80]
	movs	r3, #1
	add	r2, r8
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r3, [pc, #64]
	add	r3, r8
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #20]
	movs	r1, #1
	lsls	r3, r3, #5
	add	r9, r1
	adds	r3, #32
	cmp	r9, r3
	bne.n	.L_080d5d28
.L_080d5e1e:
	ldr	r0, [pc, #36]
	bl	sub_08004278
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080cdbc0
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x00007098
	.4byte 0x00007828
	.2byte 0x7824
	.2byte 0x0000
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #576]
	adds	r3, r2, #0
	ldmia	r3!, {r1}
	sub	sp, #172
	str	r1, [sp, #76]
	ldr	r3, [r3, #0]
	str	r3, [sp, #72]
	ldr	r2, [r2, #8]
	str	r2, [sp, #64]
	ldr	r2, [pc, #560]
	adds	r5, r1, r2
	str	r0, [r5, #0]
	movs	r0, #0
	bl	sub_080cd594
	ldr	r0, [pc, #552]
	ldr	r1, [sp, #76]
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	movs	r2, #0
	ldr	r1, [sp, #64]
	ldr	r0, [pc, #540]
	movs	r3, #0
	bl	sub_080e0524
	ldr	r3, [r5, #0]
	ldr	r0, [r3, #4]
	movs	r3, #1
	eors	r0, r3
	mov	r3, sp
	adds	r3, #80
	adds	r1, r3, #0
	str	r3, [sp, #60]
	bl	sub_080cef64
	ldr	r3, [r5, #0]
	ldr	r0, [r3, #8]
	bl	sub_080b5098
	ldr	r3, [r5, #0]
	ldr	r0, [r0, #0]
	mov	r9, r0
	movs	r5, #36
	ldrsh	r0, [r3, r5]
	bl	sub_080b5098
	ldr	r0, [r0, #0]
	ldr	r7, [pc, #492]
	str	r0, [sp, #56]
	movs	r0, #0
	mov	sl, r0
	mov	r8, r0
.L_080d5ece:
	bl	sub_08004458
	ldr	r3, [pc, #484]
	adds	r6, r0, #0
	ands	r6, r3
	bl	sub_08004458
	mov	r1, r8
	str	r1, [r7, #0]
	movs	r5, #255
	ands	r5, r0
	bl	sub_08004458
	movs	r3, #31
	ands	r3, r0
	adds	r3, #20
	lsls	r3, r3, #16
	mov	r2, r8
	str	r3, [r7, #4]
	str	r2, [r7, #8]
	adds	r0, r6, #0
	bl	sub_08002322
	adds	r5, #128
	adds	r3, r5, #0
	muls	r3, r0
	asrs	r3, r3, #5
	str	r3, [r7, #12]
	mov	r3, r8
	str	r3, [r7, #16]
	adds	r0, r6, #0
	bl	sub_0800231c
	adds	r3, r5, #0
	muls	r3, r0
	movs	r0, #1
	add	sl, r0
	asrs	r3, r3, #5
	mov	r5, r8
	mov	r1, sl
	str	r3, [r7, #20]
	str	r5, [r7, #24]
	adds	r7, #28
	cmp	r1, #64
	bne.n	.L_080d5ece
	ldr	r3, [sp, #76]
	movs	r5, #239
	lsls	r5, r5, #7
	adds	r2, r3, r5
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r0, [sp, #76]
	ldr	r1, [pc, #388]
	movs	r3, #75
	adds	r2, r0, r1
	movs	r1, #144
	str	r3, [r2, #0]
	lsls	r1, r1, #3
	ldr	r0, [pc, #380]
	bl	sub_080041d8
	mov	r5, r9
	movs	r2, #160
	ldr	r3, [r5, #8]
	add	r2, sp
	mov	fp, r2
	str	r3, [r2, #0]
	mov	r0, fp
	movs	r2, #0
	str	r2, [r0, #4]
	ldr	r3, [r5, #16]
	mov	r1, sp
	str	r3, [r0, #8]
	adds	r1, #148
	ldr	r3, [sp, #56]
	str	r1, [sp, #52]
	mov	r5, sp
	ldr	r0, [r3, #8]
	movs	r3, #180
	adds	r5, #136
	lsls	r3, r3, #15
	str	r0, [r1, #0]
	str	r3, [r1, #4]
	str	r2, [r1, #8]
	str	r5, [sp, #48]
	mov	r1, fp
	ldr	r3, [r1, #0]
	movs	r1, #40
	subs	r0, r0, r3
	bl	sub_080022ec
	str	r0, [r5, #0]
	ldr	r2, [sp, #52]
	mov	r5, fp
	ldr	r3, [r5, #4]
	ldr	r0, [r2, #4]
	movs	r1, #40
	subs	r0, r0, r3
	bl	sub_080022ec
	ldr	r1, [sp, #48]
	str	r0, [r1, #4]
	ldr	r2, [sp, #52]
	ldr	r3, [r5, #8]
	ldr	r0, [r2, #8]
	movs	r1, #40
	subs	r0, r0, r3
	bl	sub_080022ec
	ldr	r3, [sp, #48]
	movs	r5, #0
	str	r0, [r3, #8]
	mov	r9, r5
.L_080d5fb0:
	ldr	r3, [pc, #272]
	mov	r0, r9
	ldr	r5, [r3, #0]
	cmp	r0, #8
	bne.n	.L_080d5fc0
	movs	r0, #212
	bl	sub_080f9010
.L_080d5fc0:
	mov	r1, r9
	cmp	r1, #80
	bne.n	.L_080d5fcc
	movs	r0, #142
	bl	sub_080f9010
.L_080d5fcc:
	bl	sub_080049ac
	adds	r1, r5, #0
	adds	r1, #12
	adds	r0, r5, #0
	bl	sub_080051d8
	mov	r3, r9
	subs	r3, #30
	cmp	r3, #39
	bhi.n	.L_080d6000
	ldr	r5, [sp, #48]
	mov	r2, fp
	ldr	r3, [r2, #0]
	ldr	r2, [r5, #0]
	mov	r0, fp
	adds	r3, r3, r2
	str	r3, [r0, #0]
	ldr	r3, [r0, #4]
	ldr	r2, [r5, #4]
	adds	r3, r3, r2
	str	r3, [r0, #4]
	ldr	r3, [r0, #8]
	ldr	r2, [r5, #8]
	adds	r3, r3, r2
	str	r3, [r0, #8]
.L_080d6000:
	mov	r0, fp
	bl	sub_08004cb4
	mov	r1, r9
	cmp	r1, #0
	bne.n	.L_080d6026
	ldr	r2, [sp, #76]
	ldr	r5, [pc, #152]
	adds	r3, r2, r5
	ldr	r3, [r3, #0]
	movs	r2, #1
	ldr	r0, [r3, #8]
	movs	r3, #1
	str	r1, [sp, #0]
	negs	r2, r2
	movs	r1, #7
	negs	r3, r3
	bl	sub_080d6888
.L_080d6026:
	mov	r0, r9
	cmp	r0, #24
	bne.n	.L_080d6046
	ldr	r1, [sp, #76]
	ldr	r2, [pc, #120]
	adds	r3, r1, r2
	ldr	r3, [r3, #0]
	movs	r2, #1
	ldr	r0, [r3, #8]
	movs	r3, #0
	str	r3, [sp, #0]
	movs	r1, #0
	negs	r2, r2
	subs	r3, #1
	bl	sub_080d6888
.L_080d6046:
	mov	r0, r9
	negs	r0, r0
	mov	r1, r9
	movs	r5, #0
	lsls	r1, r1, #8
	str	r0, [sp, #44]
	ldr	r6, [pc, #96]
	mov	sl, r5
	lsls	r7, r0, #8
	mov	r8, r1
.L_080d605a:
	cmp	r9, sl
	bgt.n	.L_080d6060
	b.n	.L_080d616a
.L_080d6060:
	ldr	r3, [r6, #24]
	cmp	r3, #0
	beq.n	.L_080d6068
	b.n	.L_080d616a
.L_080d6068:
	bl	sub_080049e8
	movs	r3, #3
	mov	r2, sl
	ands	r3, r2
	cmp	r3, #1
	beq.n	.L_080d6092
	cmp	r3, #1
	bgt.n	.L_080d6080
	cmp	r3, #0
	beq.n	.L_080d608a
	b.n	.L_080d60d4
.L_080d6080:
	cmp	r3, #2
	beq.n	.L_080d609a
	cmp	r3, #3
	beq.n	.L_080d60c8
	b.n	.L_080d60d4
.L_080d608a:
	mov	r0, r8
	bl	sub_08004c1c
	b.n	.L_080d60d4
.L_080d6092:
	adds	r0, r7, #0
	bl	sub_08004bd4
	b.n	.L_080d60d4
.L_080d609a:
	adds	r0, r7, #0
	bl	sub_08004c6c
	b.n	.L_080d60d4
	movs	r0, r0
	.4byte 0x03001eec
	.4byte 0x00007828
	.4byte 0x00000092
	.4byte 0x00000073
	.4byte 0x02010000
	.4byte 0x0000ffff
	.4byte 0x00007784
	.4byte 0x080cd261
	.2byte 0x1e80
	.2byte 0x0300
.L_080d60c8:
	adds	r0, r7, #0
	bl	sub_08004bd4
	adds	r0, r7, #0
	bl	sub_08004c6c
.L_080d60d4:
	add	r5, sp, #112
	adds	r1, r5, #0
	adds	r0, r6, #0
	bl	sub_080e3944
	ldr	r3, [r5, #0]
	asrs	r3, r3, #1
	str	r3, [r5, #0]
	bl	sub_08004a5c
	ldr	r2, [r5, #8]
	cmp	r2, #249
	bgt.n	.L_080d60f4
	movs	r3, #250
	str	r3, [r5, #8]
	movs	r2, #250
.L_080d60f4:
	ldr	r3, [pc, #936]
	cmp	r2, r3
	ble.n	.L_080d60fe
	str	r3, [r5, #8]
	adds	r2, r3, #0
.L_080d60fe:
	adds	r3, r2, #0
	subs	r3, #250
	cmp	r3, #0
	bge.n	.L_080d6108
	adds	r3, #63
.L_080d6108:
	asrs	r3, r3, #6
	movs	r0, #9
	subs	r0, r0, r3
	lsls	r4, r0, #1
	ldr	r2, [pc, #912]
	subs	r3, r4, #2
	ldrh	r1, [r2, r3]
	ldr	r3, [sp, #64]
	adds	r1, r3, r1
	lsrs	r3, r0, #31
	ldr	r2, [r5, #0]
	adds	r3, r0, r3
	asrs	r3, r3, #1
	subs	r2, r2, r3
	ldr	r3, [r5, #4]
	str	r0, [sp, #0]
	subs	r3, r3, r0
	str	r4, [sp, #4]
	ldr	r0, [sp, #72]
	ldr	r4, [sp, #80]
	bl	sub_080072f4
	adds	r0, r6, #0
	movs	r1, #60
	movs	r2, #0
	bl	sub_080e38b8
	mov	r3, sl
	adds	r3, #30
	cmp	r9, r3
	ble.n	.L_080d616a
	ldr	r2, [r6, #0]
	ldr	r3, [r6, #12]
	negs	r2, r2
	asrs	r2, r2, #8
	ldr	r1, [r6, #4]
	adds	r3, r3, r2
	negs	r1, r1
	str	r3, [r6, #12]
	ldr	r3, [r6, #16]
	asrs	r1, r1, #8
	ldr	r0, [r6, #8]
	adds	r3, r3, r1
	negs	r0, r0
	str	r3, [r6, #16]
	ldr	r3, [r6, #20]
	asrs	r0, r0, #8
	adds	r3, r3, r0
	str	r3, [r6, #20]
.L_080d616a:
	ldr	r5, [sp, #44]
	movs	r1, #1
	lsls	r3, r5, #5
	mov	r0, r9
	add	sl, r1
	adds	r7, r7, r3
	mov	r2, sl
	lsls	r3, r0, #5
	add	r8, r3
	adds	r6, #28
	cmp	r2, #32
	beq.n	.L_080d6184
	b.n	.L_080d605a
.L_080d6184:
	cmp	r0, #82
	ble.n	.L_080d61c6
	add	r6, sp, #124
	movs	r3, #0
	str	r3, [r6, #0]
	lsls	r0, r0, #10
	bl	sub_08002322
	movs	r5, #0
	lsls	r0, r0, #2
	str	r5, [r6, #8]
	add	r5, sp, #112
	str	r0, [r6, #4]
	adds	r1, r5, #0
	adds	r0, r6, #0
	bl	sub_080e3944
	ldr	r2, [r5, #0]
	movs	r1, #20
	asrs	r2, r2, #1
	str	r2, [r5, #0]
	ldr	r3, [r5, #4]
	str	r1, [sp, #0]
	movs	r1, #34
	str	r1, [sp, #4]
	ldr	r0, [sp, #60]
	subs	r2, #10
	ldr	r4, [r0, #4]
	subs	r3, #17
	ldr	r0, [sp, #72]
	ldr	r1, [sp, #76]
	bl	sub_080072f4
.L_080d61c6:
	ldr	r1, [sp, #76]
	ldr	r3, [pc, #732]
	movs	r5, #1
	adds	r2, r1, r3
	movs	r3, #1
	movs	r0, #1
	str	r3, [r2, #0]
	add	r9, r5
	bl	sub_080030f8
	mov	r0, r9
	cmp	r0, #98
	beq.n	.L_080d61e2
	b.n	.L_080d5fb0
.L_080d61e2:
	movs	r1, #0
	ldr	r5, [pc, #708]
	mov	sl, r1
	movs	r6, #255
.L_080d61ea:
	bl	sub_08004458
	ands	r0, r6
	subs	r0, #127
	lsls	r0, r0, #15
	str	r0, [r5, #0]
	bl	sub_08004458
	movs	r3, #127
	ands	r3, r0
	adds	r3, #64
	lsls	r3, r3, #15
	str	r3, [r5, #4]
	bl	sub_08004458
	movs	r3, #0
	ands	r0, r6
	str	r3, [r5, #24]
	subs	r0, #127
	movs	r2, #1
	movs	r3, #128
	lsls	r0, r0, #15
	add	sl, r2
	lsls	r3, r3, #2
	str	r0, [r5, #8]
	adds	r5, #28
	cmp	sl, r3
	bne.n	.L_080d61ea
	ldr	r0, [pc, #652]
	ldr	r1, [sp, #64]
	movs	r2, #0
	movs	r3, #0
	bl	sub_080e0524
	ldr	r1, [pc, #644]
	ldr	r0, [sp, #76]
	adds	r3, r0, r1
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #20]
	movs	r2, #72
	movs	r5, #0
	lsls	r3, r3, #3
	negs	r2, r2
	mov	r9, r5
	cmp	r3, r2
	bne.n	.L_080d6248
	b.n	.L_080d64d6
.L_080d6248:
	mov	r3, sp
	mov	r5, sp
	ldr	r2, [sp, #52]
	adds	r3, #100
	adds	r5, #88
	adds	r1, r0, r1
	str	r3, [sp, #16]
	str	r5, [sp, #20]
	str	r1, [sp, #32]
	str	r2, [sp, #28]
.L_080d625c:
	ldr	r3, [pc, #600]
	ldr	r3, [r3, #0]
	str	r3, [sp, #40]
	bl	sub_080049ac
	ldr	r3, [sp, #40]
	adds	r3, #12
	adds	r1, r3, #0
	ldr	r0, [sp, #40]
	str	r3, [sp, #36]
	bl	sub_080051d8
	ldr	r5, [sp, #32]
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #20]
	lsls	r3, r3, #3
	adds	r3, #40
	cmp	r9, r3
	blt.n	.L_080d628e
	ldr	r0, [sp, #28]
	movs	r1, #128
	ldr	r3, [r0, #4]
	lsls	r1, r1, #11
	adds	r3, r3, r1
	str	r3, [r0, #4]
.L_080d628e:
	ldr	r2, [sp, #28]
	ldr	r5, [sp, #16]
	ldr	r3, [r2, #0]
	str	r3, [r5, #0]
	ldr	r3, [r2, #4]
	mov	r1, r9
	str	r3, [r5, #4]
	lsls	r0, r1, #11
	bl	sub_08002322
	ldr	r5, [sp, #28]
	lsls	r2, r0, #2
	ldr	r3, [r5, #8]
	adds	r2, r2, r0
	lsls	r2, r2, #3
	ldr	r0, [sp, #16]
	adds	r3, r3, r2
	str	r3, [r0, #8]
	add	r2, sp, #88
	adds	r1, r2, #0
	ldr	r0, [sp, #16]
	bl	sub_080e3944
	ldr	r2, [sp, #88]
	ldr	r3, [sp, #92]
	movs	r1, #20
	asrs	r2, r2, #1
	str	r1, [sp, #0]
	movs	r1, #34
	str	r2, [sp, #88]
	subs	r3, #17
	str	r1, [sp, #4]
	subs	r2, #10
	ldr	r4, [sp, #80]
	ldr	r0, [sp, #72]
	ldr	r1, [sp, #76]
	bl	sub_080072f4
	movs	r3, #0
	str	r3, [sp, #68]
	ldr	r5, [sp, #32]
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	bne.n	.L_080d62ea
	b.n	.L_080d6484
.L_080d62ea:
	ldr	r1, [pc, #456]
	ldr	r0, [sp, #76]
	movs	r2, #36
	adds	r1, r0, r1
	movs	r3, #0
	str	r1, [sp, #24]
	str	r2, [sp, #12]
	str	r3, [sp, #8]
.L_080d62fa:
	ldr	r5, [sp, #24]
	ldr	r1, [sp, #12]
	ldr	r3, [r5, #0]
	ldrsh	r0, [r3, r1]
	bl	sub_080b5098
	ldr	r3, [sp, #68]
	lsls	r3, r3, #3
	ldr	r5, [r0, #0]
	mov	r8, r3
	bl	sub_080049ac
	ldr	r0, [sp, #40]
	ldr	r1, [sp, #36]
	bl	sub_080051d8
	ldr	r3, [r5, #8]
	mov	r0, fp
	str	r3, [r0, #0]
	movs	r3, #160
	lsls	r3, r3, #14
	str	r3, [r0, #4]
	ldr	r3, [r5, #16]
	str	r3, [r0, #8]
	bl	sub_08004cb4
	mov	r3, r8
	adds	r3, #30
	cmp	r9, r3
	bne.n	.L_080d633c
	movs	r0, #126
	bl	sub_080f9010
.L_080d633c:
	mov	r3, r8
	adds	r3, #40
	cmp	r9, r3
	bne.n	.L_080d635c
	ldr	r1, [sp, #24]
	ldr	r2, [sp, #12]
	ldr	r3, [r1, #0]
	ldrsh	r0, [r3, r2]
	movs	r3, #0
	movs	r2, #1
	str	r3, [sp, #0]
	movs	r1, #7
	negs	r2, r2
	subs	r3, #1
	bl	sub_080d6888
.L_080d635c:
	mov	r3, r8
	adds	r3, #64
	cmp	r9, r3
	bne.n	.L_080d637c
	ldr	r0, [sp, #24]
	ldr	r1, [sp, #12]
	ldr	r3, [r0, #0]
	ldrsh	r0, [r3, r1]
	movs	r3, #0
	movs	r2, #1
	str	r3, [sp, #0]
	movs	r1, #0
	negs	r2, r2
	subs	r3, #1
	bl	sub_080d6888
.L_080d637c:
	cmp	r9, r8
	ble.n	.L_080d6462
	mov	r3, r9
	mov	r5, r8
	subs	r0, r3, r5
	lsls	r0, r0, #9
	bl	sub_08004c1c
	ldr	r2, [pc, #284]
	ldr	r1, [sp, #8]
	movs	r0, #0
	mov	sl, r0
	adds	r6, r1, r2
.L_080d6396:
	mov	r5, sl
	lsrs	r3, r5, #31
	add	r3, sl
	asrs	r3, r3, #1
	add	r3, r8
	cmp	r9, r3
	ble.n	.L_080d6456
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
	ldr	r3, [pc, #248]
	bl	sub_080072f0
	asrs	r7, r0, #9
	cmp	r7, #0
	beq.n	.L_080d6456
	add	r2, sp, #88
	adds	r0, r6, #0
	adds	r1, r2, #0
	bl	sub_080e3944
	ldr	r3, [sp, #88]
	asrs	r3, r3, #1
	str	r3, [sp, #88]
	ldr	r5, [pc, #224]
	ldr	r3, [sp, #96]
	cmp	r3, r5
	bgt.n	.L_080d63ee
	movs	r3, #157
	ldr	r0, [sp, #20]
	lsls	r3, r3, #1
	str	r3, [r0, #8]
.L_080d63ee:
	ldr	r2, [pc, #176]
	cmp	r3, r2
	ble.n	.L_080d63f8
	ldr	r1, [sp, #20]
	str	r2, [r1, #8]
.L_080d63f8:
	mov	r3, sl
	lsls	r0, r3, #2
	movs	r1, #9
	add	r0, r9
	bl	sub_080022fc
	ldr	r2, [pc, #188]
	lsls	r3, r0, #1
	ldrh	r1, [r2, r3]
	ldr	r3, [pc, #188]
	ldr	r5, [sp, #64]
	ldrb	r0, [r3, r0]
	ldr	r3, [sp, #20]
	adds	r1, r5, r1
	ldr	r2, [r3, #0]
	ldr	r5, [sp, #60]
	ldr	r3, [r3, #4]
	str	r0, [sp, #0]
	str	r0, [sp, #4]
	lsrs	r4, r0, #1
	subs	r2, r2, r4
	subs	r3, r3, r4
	ldr	r0, [sp, #72]
	ldr	r4, [r5, #4]
	bl	sub_080072f4
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
	subs	r5, r5, r0
	str	r5, [r6, #8]
.L_080d6456:
	movs	r0, #1
	add	sl, r0
	mov	r1, sl
	adds	r6, #28
	cmp	r1, #64
	bne.n	.L_080d6396
.L_080d6462:
	ldr	r2, [sp, #12]
	ldr	r3, [sp, #8]
	ldr	r0, [sp, #68]
	movs	r5, #224
	lsls	r5, r5, #3
	adds	r3, r3, r5
	adds	r2, #2
	adds	r0, #1
	str	r2, [sp, #12]
	str	r3, [sp, #8]
	str	r0, [sp, #68]
	ldr	r1, [sp, #24]
	ldr	r3, [r1, #0]
	ldr	r3, [r3, #20]
	cmp	r0, r3
	beq.n	.L_080d6484
	b.n	.L_080d62fa
.L_080d6484:
	ldr	r3, [sp, #76]
	ldr	r5, [pc, #32]
	adds	r2, r3, r5
	movs	r3, #1
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r1, [sp, #32]
	ldr	r3, [r1, #0]
	ldr	r3, [r3, #20]
	movs	r0, #1
	lsls	r3, r3, #3
	b.n	.L_080d64cc
	.4byte 0x0000027a
	.4byte 0x080ede48
	.4byte 0x00007824
	.4byte 0x02010000
	.4byte 0x000000ba
	.4byte 0x00007828
	.4byte 0x03001e80
	.4byte 0x030001d8
	.4byte 0x00000139
	.4byte 0x080ede84
	.2byte 0xde96
	.2byte 0x080e
.L_080d64cc:
	add	r9, r0
	adds	r3, #72
	cmp	r9, r3
	beq.n	.L_080d64d6
	b.n	.L_080d625c
.L_080d64d6:
	ldr	r0, [pc, #40]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080cdbc0
	add	sp, #172
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x080cd261
