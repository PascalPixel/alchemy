.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
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
	.set sub_08004c1c, 0x08004c1c
	.set sub_08004cb4, 0x08004cb4
	.set sub_080051d8, 0x080051d8
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_080b5078, 0x080b5078
	.set sub_080b5088, 0x080b5088
	.set sub_080b5098, 0x080b5098
	.set sub_080b50e8, 0x080b50e8
	.set sub_080cd52c, 0x080cd52c
	.set sub_080cd594, 0x080cd594
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080d6888, 0x080d6888
	.set sub_080e0524, 0x080e0524
	.set sub_080e155c, 0x080e155c
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
	.4byte 0x080dbb9d
