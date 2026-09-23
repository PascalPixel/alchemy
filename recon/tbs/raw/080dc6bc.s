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
	.set sub_08004cb4, 0x08004cb4
	.set sub_080051d8, 0x080051d8
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_080b5098, 0x080b5098
	.set sub_080cd594, 0x080cd594
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080e0524, 0x080e0524
	.set sub_080e38b8, 0x080e38b8
	.set sub_080e3944, 0x080e3944
	.set sub_080ed408, 0x080ed408
	.global Unnamed_080dc6bc
	.global Func_080dc6bc
	.thumb_func
Unnamed_080dc6bc:
Func_080dc6bc:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r5, [pc, #624]
	adds	r3, r5, #0
	ldmia	r3!, {r1}
	ldr	r3, [r3, #0]
	sub	sp, #64
	str	r3, [sp, #36]
	ldr	r3, [pc, #616]
	mov	sl, r1
	add	r3, sl
	str	r0, [r3, #0]
	movs	r0, #0
	bl	sub_080cd594
	ldr	r0, [pc, #604]
	mov	r1, sl
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	movs	r1, #216
	lsls	r1, r1, #5
	add	r1, sl
	movs	r2, #0
	movs	r3, #0
	ldr	r0, [pc, #588]
	bl	sub_080e0524
	ldr	r0, [pc, #584]
	bl	sub_08002f40
	adds	r1, r0, #0
	movs	r0, #160
	ldr	r3, [pc, #580]
	movs	r2, #128
	lsls	r0, r0, #19
	bl	sub_080072f0
	movs	r3, #3
	movs	r1, #7
	movs	r2, #7
	movs	r0, #46
	str	r3, [sp, #0]
	bl	sub_080ed408
	ldr	r2, [r5, #28]
	movs	r3, #2
	str	r2, [sp, #24]
	str	r3, [sp, #0]
	movs	r0, #47
	movs	r3, #3
	movs	r1, #7
	movs	r2, #7
	bl	sub_080ed408
	ldr	r5, [r5, #32]
	str	r5, [sp, #28]
	movs	r5, #225
	movs	r3, #0
	lsls	r5, r5, #7
	mov	r9, r3
	mov	r8, r3
	add	r5, sl
.L_080dc746:
	bl	sub_08004458
	ldr	r3, [pc, #520]
	adds	r7, r0, #0
	ands	r7, r3
	bl	sub_08004458
	mov	r4, r8
	adds	r6, r0, #0
	movs	r3, #255
	str	r4, [r5, #0]
	str	r4, [r5, #4]
	str	r4, [r5, #8]
	mov	r0, r9
	movs	r1, #6
	ands	r6, r3
	bl	sub_080022fc
	cmp	r0, #5
	bne.n	.L_080dc776
	mov	r1, r8
	str	r1, [r5, #12]
	str	r1, [r5, #16]
	b.n	.L_080dc792
.L_080dc776:
	adds	r0, r7, #0
	bl	sub_08002322
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #7
	str	r3, [r5, #12]
	adds	r0, r7, #0
	bl	sub_0800231c
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #9
	str	r3, [r5, #16]
.L_080dc792:
	movs	r3, #1
	add	r9, r3
	mov	r2, r8
	mov	r4, r9
	str	r2, [r5, #20]
	str	r2, [r5, #24]
	adds	r5, #28
	cmp	r4, #64
	bne.n	.L_080dc746
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, sl
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #424]
	movs	r3, #75
	add	r2, sl
	movs	r1, #144
	lsls	r1, r1, #3
	str	r3, [r2, #0]
	ldr	r0, [pc, #416]
	bl	sub_080041d8
	movs	r1, #0
	str	r1, [sp, #32]
.L_080dc7c4:
	ldr	r3, [pc, #408]
	ldr	r3, [r3, #0]
	movs	r2, #0
	str	r2, [sp, #20]
	str	r3, [sp, #16]
	ldr	r2, [pc, #368]
	mov	r4, sl
	ldr	r3, [r4, r2]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	bne.n	.L_080dc7dc
	b.n	.L_080dc8fa
.L_080dc7dc:
	ldr	r1, [sp, #16]
	movs	r3, #40
	adds	r1, #12
	movs	r4, #0
	add	r3, sp
	str	r1, [sp, #12]
	str	r4, [sp, #8]
	mov	r8, r3
.L_080dc7ec:
	ldr	r4, [sp, #20]
	mov	r1, sl
	ldr	r2, [r1, r2]
	lsls	r3, r4, #1
	adds	r3, #36
	ldrsh	r0, [r2, r3]
	bl	sub_080b5098
	ldr	r5, [r0, #0]
	bl	sub_080049ac
	ldr	r0, [sp, #16]
	ldr	r1, [sp, #12]
	bl	sub_080051d8
	ldr	r3, [r5, #8]
	mov	r2, r8
	str	r3, [r2, #0]
	movs	r3, #160
	lsls	r3, r3, #13
	str	r3, [r2, #4]
	ldr	r3, [r5, #16]
	mov	r0, r8
	str	r3, [r2, #8]
	bl	sub_08004cb4
	ldr	r4, [sp, #20]
	ldr	r1, [sp, #32]
	lsls	r3, r4, #3
	cmp	r1, r3
	blt.n	.L_080dc8e0
	adds	r3, #40
	cmp	r1, r3
	bge.n	.L_080dc8e0
	ldr	r4, [sp, #8]
	lsls	r3, r4, #3
	subs	r3, r3, r4
	lsls	r3, r3, #2
	movs	r1, #225
	movs	r2, #0
	add	r7, sp, #52
	add	r3, sl
	lsls	r1, r1, #7
	mov	r9, r2
	mov	fp, r7
	adds	r6, r3, r1
.L_080dc848:
	ldr	r0, [r6, #24]
	movs	r1, #6
	bl	sub_080022ec
	adds	r5, r0, #0
	cmp	r5, #5
	ble.n	.L_080dc858
	movs	r5, #5
.L_080dc858:
	adds	r0, r6, #0
	mov	r1, fp
	bl	sub_080e3944
	mov	r2, fp
	ldr	r3, [r2, #0]
	asrs	r3, r3, #1
	str	r3, [r2, #0]
	mov	r3, r9
	cmp	r3, #5
	bne.n	.L_080dc8a2
	movs	r2, #128
	adds	r0, r6, #0
	movs	r1, #62
	lsls	r2, r2, #4
	bl	sub_080e38b8
	lsls	r1, r5, #3
	adds	r1, r1, r5
	ldr	r2, [r7, #0]
	ldr	r3, [r7, #4]
	movs	r0, #24
	lsls	r1, r1, #7
	movs	r4, #216
	lsls	r4, r4, #5
	add	r1, sl
	str	r0, [sp, #0]
	movs	r0, #48
	adds	r1, r1, r4
	str	r0, [sp, #4]
	subs	r2, #12
	subs	r3, #36
	ldr	r0, [sp, #36]
	ldr	r4, [sp, #24]
	bl	sub_080072f4
	b.n	.L_080dc8ce
.L_080dc8a2:
	movs	r2, #128
	adds	r0, r6, #0
	movs	r1, #60
	lsls	r2, r2, #2
	bl	sub_080e38b8
	lsls	r1, r5, #3
	ldr	r2, [r7, #0]
	ldr	r3, [r7, #4]
	movs	r0, #24
	adds	r1, r1, r5
	lsls	r1, r1, #7
	str	r0, [sp, #0]
	movs	r0, #48
	str	r0, [sp, #4]
	add	r1, sl
	subs	r2, #12
	subs	r3, #36
	ldr	r0, [sp, #36]
	ldr	r4, [sp, #28]
	bl	sub_080072f4
.L_080dc8ce:
	ldr	r3, [r6, #24]
	movs	r1, #1
	add	r9, r1
	adds	r3, #1
	mov	r2, r9
	str	r3, [r6, #24]
	adds	r6, #28
	cmp	r2, #6
	bne.n	.L_080dc848
.L_080dc8e0:
	ldr	r3, [sp, #8]
	ldr	r4, [sp, #20]
	adds	r3, #6
	adds	r4, #1
	str	r3, [sp, #8]
	str	r4, [sp, #20]
	ldr	r2, [pc, #80]
	mov	r1, sl
	ldr	r3, [r1, r2]
	ldr	r3, [r3, #20]
	cmp	r4, r3
	beq.n	.L_080dc8fa
	b.n	.L_080dc7ec
.L_080dc8fa:
	ldr	r2, [pc, #104]
	movs	r3, #1
	add	r2, sl
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r2, [sp, #32]
	adds	r2, #1
	str	r2, [sp, #32]
	cmp	r2, #96
	beq.n	.L_080dc914
	b.n	.L_080dc7c4
.L_080dc914:
	ldr	r0, [pc, #68]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080cdbc0
	add	sp, #64
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
	.4byte 0x0000009e
	.4byte 0x0000006c
	.4byte 0x000000bb
	.4byte 0x03001388
	.4byte 0x0000ffff
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x03001e80
	.4byte 0x00007824
