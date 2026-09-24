.syntax unified
	.thumb
	.set sub_080022ec, 0x080022ec
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_08002dd8, 0x08002dd8
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_080e0524, 0x080e0524
	.set sub_080e3908, 0x080e3908
	.set sub_080e6d3c, 0x080e6d3c
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global Unnamed_080e6eac
	.global Func_080e6eac
	.thumb_func
Unnamed_080e6eac:
Func_080e6eac:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #36
	ldr	r5, [pc, #104]
	str	r2, [sp, #28]
	str	r0, [sp, #32]
	adds	r3, r5, #0
	ldmia	r3!, {r0}
	mov	fp, r1
	ldr	r3, [r3, #0]
	mov	r2, fp
	movs	r4, #160
	str	r3, [sp, #24]
	lsls	r4, r4, #14
	adds	r3, r2, #0
	adds	r3, r3, r4
	mov	fp, r3
	lsrs	r3, r3, #31
	add	r3, fp
	asrs	r3, r3, #1
	ldr	r1, [r5, #8]
	mov	fp, r3
	str	r2, [sp, #8]
	ldr	r3, [pc, #56]
	ldr	r2, [pc, #68]
	str	r1, [sp, #12]
	strh	r3, [r2, #0]
	adds	r2, #8
	movs	r3, #0
	str	r3, [r2, #0]
	ldr	r3, [pc, #48]
	adds	r2, #40
	mov	sl, r0
	strh	r3, [r2, #0]
	movs	r6, #2
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	str	r6, [sp, #0]
	bl	sub_080ed408
	ldr	r0, [r5, #28]
	movs	r3, #3
	str	r0, [sp, #16]
	movs	r1, #7
	movs	r2, #7
	movs	r0, #47
	str	r3, [sp, #0]
	bl	sub_080ed408
	ldr	r5, [r5, #32]
	b.n	.L_080e6f30
	.4byte 0x00000080
	.4byte 0x00003f46
	.4byte 0x03001eec
	.2byte 0x0020
	.2byte 0x0400
.L_080e6f30:
	ldr	r1, [sp, #12]
	ldr	r0, [pc, #772]
	movs	r2, #0
	movs	r3, #0
	str	r5, [sp, #20]
	bl	sub_080e0524
	ldr	r0, [pc, #764]
	mov	r1, sl
	movs	r2, #1
	movs	r3, #0
	bl	sub_080e0524
	ldr	r1, [pc, #756]
	ldr	r0, [pc, #756]
	add	r1, sl
	movs	r2, #0
	movs	r3, #0
	bl	sub_080e0524
	movs	r3, #239
	lsls	r3, r3, #7
	ldr	r2, [pc, #744]
	add	r3, sl
	str	r6, [r3, #0]
	add	r2, sl
	movs	r3, #50
	movs	r1, #144
	lsls	r1, r1, #3
	str	r3, [r2, #0]
	ldr	r0, [pc, #732]
	movs	r7, #225
	bl	sub_080041d8
	lsls	r7, r7, #7
	movs	r1, #0
	mov	r8, r1
	add	r7, sl
.L_080e6f7c:
	bl	sub_08004458
	movs	r6, #255
	movs	r2, #128
	lsls	r2, r2, #1
	ands	r6, r0
	adds	r6, r6, r2
	bl	sub_08004458
	ldr	r3, [pc, #704]
	adds	r5, r0, #0
	ands	r5, r3
	mov	r3, fp
	str	r3, [r7, #0]
	ldr	r4, [sp, #28]
	adds	r0, r5, #0
	str	r4, [r7, #4]
	bl	sub_08002322
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #7
	str	r3, [r7, #12]
	adds	r0, r5, #0
	bl	sub_0800231c
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #6
	negs	r3, r3
	str	r3, [r7, #16]
	bl	sub_08004458
	movs	r3, #15
	ands	r3, r0
	movs	r0, #1
	add	r8, r0
	adds	r3, #16
	mov	r1, r8
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r1, #64
	bne.n	.L_080e6f7c
	ldr	r5, [pc, #640]
	movs	r2, #0
	mov	r8, r2
	movs	r6, #0
	add	r5, sl
.L_080e6fdc:
	mov	r3, fp
	str	r3, [r5, #0]
	ldr	r4, [sp, #28]
	adds	r0, r6, #0
	str	r4, [r5, #4]
	bl	sub_08002322
	lsls	r0, r0, #5
	asrs	r0, r0, #6
	str	r0, [r5, #12]
	adds	r0, r6, #0
	bl	sub_0800231c
	lsls	r0, r0, #5
	asrs	r0, r0, #5
	negs	r0, r0
	movs	r1, #1
	str	r0, [r5, #16]
	add	r8, r1
	ldr	r0, [pc, #596]
	mov	r2, r8
	adds	r6, r6, r0
	adds	r5, #28
	cmp	r2, #3
	bne.n	.L_080e6fdc
	movs	r3, #0
	ldr	r7, [pc, #584]
	mov	r8, r3
.L_080e7014:
	bl	sub_08004458
	movs	r6, #255
	ands	r6, r0
	bl	sub_08004458
	mov	r4, fp
	str	r4, [r7, #0]
	ldr	r3, [pc, #552]
	adds	r5, r0, #0
	ldr	r0, [sp, #28]
	ands	r5, r3
	str	r0, [r7, #4]
	adds	r0, r5, #0
	bl	sub_08002322
	adds	r6, #32
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #6
	str	r3, [r7, #12]
	adds	r0, r5, #0
	bl	sub_0800231c
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #5
	negs	r3, r3
	str	r3, [r7, #16]
	bl	sub_08004458
	movs	r3, #15
	movs	r1, #1
	ands	r3, r0
	add	r8, r1
	adds	r3, #20
	mov	r2, r8
	str	r3, [r7, #24]
	adds	r7, #28
	cmp	r2, #64
	bne.n	.L_080e7014
	movs	r7, #0
.L_080e7068:
	cmp	r7, #4
	bne.n	.L_080e7072
	movs	r0, #154
	bl	sub_080f9010
.L_080e7072:
	cmp	r7, #32
	bne.n	.L_080e707c
	movs	r0, #212
	bl	sub_080f9010
.L_080e707c:
	cmp	r7, #47
	bgt.n	.L_080e70be
	adds	r0, r7, #0
	subs	r0, #8
	movs	r1, #5
	bl	sub_080022ec
	adds	r4, r0, #0
	cmp	r4, #0
	bge.n	.L_080e7092
	movs	r4, #0
.L_080e7092:
	ldr	r2, [pc, #460]
	lsls	r3, r4, #1
	ldrh	r1, [r2, r3]
	mov	r3, fp
	asrs	r2, r3, #16
	ldr	r3, [pc, #452]
	ldrb	r5, [r3, r4]
	ldr	r0, [sp, #28]
	lsrs	r3, r5, #1
	subs	r2, r2, r3
	asrs	r3, r0, #16
	ldr	r0, [pc, #444]
	ldrb	r4, [r0, r4]
	lsrs	r0, r4, #1
	subs	r3, r3, r0
	str	r4, [sp, #4]
	add	r1, sl
	str	r5, [sp, #0]
	ldr	r0, [sp, #24]
	ldr	r4, [sp, #16]
	bl	sub_080072f4
.L_080e70be:
	movs	r6, #225
	movs	r0, #0
	lsls	r6, r6, #7
	mov	r8, r0
	add	r6, sl
.L_080e70c8:
	mov	r1, r8
	lsrs	r3, r1, #31
	add	r3, r8
	asrs	r3, r3, #1
	cmp	r7, r3
	ble.n	.L_080e711e
	ldr	r3, [r6, #24]
	cmp	r3, #0
	ble.n	.L_080e711e
	subs	r3, #1
	str	r3, [r6, #24]
	adds	r0, r6, #0
	movs	r1, #60
	movs	r2, #0
	bl	sub_080e3908
	ldr	r4, [r6, #24]
	cmp	r4, #0
	bge.n	.L_080e70f0
	adds	r4, #15
.L_080e70f0:
	asrs	r4, r4, #4
	adds	r4, #3
	movs	r3, #2
	ldrsh	r2, [r6, r3]
	lsls	r5, r4, #1
	movs	r0, #6
	ldrsh	r3, [r6, r0]
	ldr	r0, [pc, #364]
	subs	r1, r5, #2
	ldrh	r1, [r0, r1]
	ldr	r0, [sp, #12]
	adds	r1, r0, r1
	lsrs	r0, r4, #31
	adds	r0, r4, r0
	asrs	r0, r0, #1
	subs	r2, r2, r0
	subs	r3, r3, r4
	str	r4, [sp, #0]
	str	r5, [sp, #4]
	ldr	r0, [sp, #24]
	ldr	r4, [sp, #20]
	bl	sub_080072f4
.L_080e711e:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r6, #28
	cmp	r1, #30
	bne.n	.L_080e70c8
	ldr	r3, [pc, #320]
	movs	r2, #0
	ldr	r6, [pc, #300]
	mov	r8, r2
	mov	r9, r3
.L_080e7134:
	cmp	r7, #35
	ble.n	.L_080e7182
	ldr	r3, [r6, #24]
	cmp	r3, #0
	ble.n	.L_080e7182
	subs	r3, #1
	str	r3, [r6, #24]
	adds	r0, r6, #0
	movs	r1, #60
	movs	r2, #0
	bl	sub_080e3908
	ldr	r4, [r6, #24]
	cmp	r4, #0
	bge.n	.L_080e7154
	adds	r4, #15
.L_080e7154:
	asrs	r4, r4, #4
	adds	r4, #1
	lsls	r5, r4, #1
	movs	r0, #2
	ldrsh	r2, [r6, r0]
	movs	r1, #6
	ldrsh	r3, [r6, r1]
	mov	r0, r9
	subs	r1, r5, #2
	ldrh	r1, [r0, r1]
	ldr	r0, [sp, #12]
	adds	r1, r0, r1
	lsrs	r0, r4, #31
	adds	r0, r4, r0
	asrs	r0, r0, #1
	subs	r2, r2, r0
	subs	r3, r3, r4
	str	r4, [sp, #0]
	str	r5, [sp, #4]
	ldr	r0, [sp, #24]
	ldr	r4, [sp, #20]
	bl	sub_080072f4
.L_080e7182:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r6, #28
	cmp	r1, #60
	bne.n	.L_080e7134
	ldr	r5, [pc, #196]
	movs	r2, #0
	adds	r6, r7, #0
	mov	r8, r2
	subs	r6, #36
	add	r5, sl
.L_080e719a:
	cmp	r6, #27
	bhi.n	.L_080e71d8
	movs	r2, #0
	adds	r0, r5, #0
	movs	r1, #64
	bl	sub_080e3908
	movs	r1, #7
	adds	r0, r6, #0
	bl	sub_080022ec
	lsls	r1, r0, #3
	adds	r1, r1, r0
	lsls	r1, r1, #5
	ldr	r0, [pc, #136]
	add	r1, sl
	movs	r3, #2
	ldrsh	r2, [r5, r3]
	adds	r1, r1, r0
	movs	r4, #6
	ldrsh	r3, [r5, r4]
	movs	r0, #12
	str	r0, [sp, #0]
	movs	r0, #24
	str	r0, [sp, #4]
	subs	r2, #6
	subs	r3, #12
	ldr	r0, [sp, #24]
	ldr	r4, [sp, #16]
	bl	sub_080072f4
.L_080e71d8:
	movs	r0, #1
	add	r8, r0
	mov	r1, r8
	adds	r5, #28
	cmp	r1, #3
	bne.n	.L_080e719a
	cmp	r7, #35
	bgt.n	.L_080e71f2
	ldr	r0, [sp, #32]
	ldr	r1, [sp, #8]
	ldr	r2, [sp, #28]
	bl	sub_080e6d3c
.L_080e71f2:
	ldr	r2, [pc, #124]
	movs	r3, #1
	add	r2, sl
	str	r3, [r2, #0]
	movs	r0, #1
	adds	r7, #1
	bl	sub_080030f8
	cmp	r7, #72
	beq.n	.L_080e7208
	b.n	.L_080e7068
.L_080e7208:
	ldr	r0, [pc, #64]
	bl	sub_08004278
	movs	r1, #128
	ldr	r3, [pc, #96]
	lsls	r1, r1, #7
	ldr	r0, [pc, #96]
	bl	sub_080072f0
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	add	sp, #36
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x00000073
	.4byte 0x0000005e
	.4byte 0x000059d8
	.4byte 0x0000005f
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x0000ffff
	.4byte 0x0000772c
	.4byte 0x00005555
	.4byte 0x02010000
	.4byte 0x080eee66
	.4byte 0x080eee56
	.4byte 0x080eee5e
	.4byte 0x080ede48
	.4byte 0x00007824
	.4byte 0x03000164
	.4byte 0x06004000
