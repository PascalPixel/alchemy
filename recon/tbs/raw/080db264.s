.syntax unified
	.thumb
	.set sub_080022fc, 0x080022fc
	.set sub_0800231c, 0x0800231c
	.set sub_08002322, 0x08002322
	.set sub_08002dd8, 0x08002dd8
	.set sub_080030f8, 0x080030f8
	.set sub_080041d8, 0x080041d8
	.set sub_08004278, 0x08004278
	.set sub_08004458, 0x08004458
	.set sub_080072f0, 0x080072f0
	.set sub_080072f4, 0x080072f4
	.set sub_080b50e8, 0x080b50e8
	.set sub_080cd52c, 0x080cd52c
	.set sub_080cdb24, 0x080cdb24
	.set sub_080cdbc0, 0x080cdbc0
	.set sub_080d6888, 0x080d6888
	.set sub_080e0524, 0x080e0524
	.set sub_080e155c, 0x080e155c
	.set sub_080e3908, 0x080e3908
	.set sub_080ed408, 0x080ed408
	.set sub_080f9010, 0x080f9010
	.global Unnamed_080db264
	.global Func_080db264
	.thumb_func
Unnamed_080db264:
Func_080db264:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r2, [pc, #48]
	adds	r3, r2, #0
	ldmia	r3!, {r1}
	ldr	r3, [r3, #0]
	sub	sp, #44
	str	r3, [sp, #32]
	ldr	r5, [pc, #40]
	mov	sl, r1
	ldr	r2, [r2, #8]
	add	r5, sl
	str	r2, [sp, #24]
	str	r0, [r5, #0]
	movs	r0, #1
	bl	sub_080cdb24
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #24]
	cmp	r3, #2
	bne.n	.L_080db2b0
	ldr	r2, [pc, #16]
	ldr	r3, [pc, #4]
	b.n	.L_080db2b4
	movs	r0, r0
	.4byte 0x00000080
	.4byte 0x03001eec
	.4byte 0x00007828
	.2byte 0x0020
	.2byte 0x0400
.L_080db2b0:
	ldr	r2, [pc, #60]
	ldr	r3, [pc, #56]
.L_080db2b4:
	strh	r3, [r2, #0]
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #3
	movs	r0, #46
	bl	sub_080ed408
	ldr	r5, [pc, #44]
	adds	r3, r5, #0
	adds	r3, #184
	ldr	r3, [r3, #0]
	str	r3, [sp, #36]
	movs	r3, #3
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #7
	movs	r3, #7
	movs	r0, #47
	bl	sub_080ed408
	adds	r5, #188
	ldr	r3, [r5, #0]
	mov	r2, sp
	ldr	r1, [pc, #16]
	adds	r2, #36
	b.n	.L_080db2fc
	.4byte 0x00000100
	.4byte 0x04000020
	.4byte 0x03001e50
	.2byte 0x060e
	.2byte 0x0000
.L_080db2fc:
	str	r2, [sp, #12]
	ldr	r0, [pc, #880]
	str	r3, [r2, #4]
	add	r1, sl
	movs	r2, #1
	movs	r3, #1
	bl	sub_080e0524
	movs	r2, #0
	movs	r3, #0
	ldr	r0, [pc, #864]
	ldr	r1, [sp, #24]
	bl	sub_080e0524
	ldr	r3, [pc, #860]
	add	r3, sl
	ldr	r2, [r3, #0]
	ldr	r3, [r2, #24]
	cmp	r3, #2
	bne.n	.L_080db338
	ldr	r3, [r2, #4]
	cmp	r3, #1
	bne.n	.L_080db330
	ldr	r2, [pc, #848]
	ldr	r3, [pc, #848]
	b.n	.L_080db342
.L_080db330:
	ldr	r2, [pc, #840]
	movs	r3, #128
	lsls	r3, r3, #5
	b.n	.L_080db342
.L_080db338:
	ldr	r3, [r2, #4]
	cmp	r3, #1
	bne.n	.L_080db344
	ldr	r2, [pc, #828]
	ldr	r3, [pc, #832]
.L_080db342:
	str	r3, [r2, #0]
.L_080db344:
	movs	r3, #0
	ldr	r7, [pc, #832]
	mov	r8, r3
.L_080db34a:
	bl	sub_08004458
	ldr	r6, [pc, #828]
	movs	r1, #128
	lsls	r1, r1, #1
	ands	r6, r0
	adds	r6, r6, r1
	bl	sub_08004458
	ldr	r5, [pc, #816]
	movs	r3, #128
	ldr	r2, [pc, #816]
	lsls	r3, r3, #7
	str	r3, [r7, #0]
	ands	r5, r0
	movs	r3, #224
	adds	r5, r5, r2
	lsls	r3, r3, #7
	str	r3, [r7, #4]
	adds	r0, r5, #0
	bl	sub_08002322
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #16
	str	r3, [r7, #8]
	adds	r0, r5, #0
	bl	sub_0800231c
	adds	r3, r6, #0
	muls	r3, r0
	lsls	r3, r3, #1
	negs	r3, r3
	asrs	r3, r3, #16
	str	r3, [r7, #20]
	movs	r3, #0
	str	r3, [r7, #24]
	movs	r1, #128
	movs	r3, #1
	add	r8, r3
	lsls	r1, r1, #3
	adds	r7, #28
	cmp	r8, r1
	bne.n	.L_080db34a
	movs	r2, #239
	lsls	r2, r2, #7
	add	r2, sl
	movs	r3, #2
	str	r3, [r2, #0]
	ldr	r2, [pc, #744]
	movs	r3, #75
	add	r2, sl
	str	r3, [r2, #0]
	adds	r1, #128
	ldr	r0, [pc, #740]
	bl	sub_080041d8
	movs	r0, #138
	bl	sub_080f9010
	movs	r2, #0
	str	r2, [sp, #28]
.L_080db3c6:
	ldr	r3, [sp, #28]
	cmp	r3, #20
	bne.n	.L_080db3d2
	movs	r0, #133
	bl	sub_080b50e8
.L_080db3d2:
	ldr	r1, [sp, #28]
	cmp	r1, #15
	ble.n	.L_080db3da
	b.n	.L_080db55a
.L_080db3da:
	adds	r0, r1, #0
	movs	r1, #5
	bl	sub_080022fc
	cmp	r0, #2
	bne.n	.L_080db3f4
	movs	r1, #128
	ldr	r3, [pc, #692]
	ldr	r0, [sp, #32]
	lsls	r1, r1, #7
	ldr	r2, [pc, #692]
	bl	sub_080072f0
.L_080db3f4:
	ldr	r3, [sp, #28]
	movs	r2, #0
	lsls	r3, r3, #11
	str	r2, [sp, #20]
	str	r3, [sp, #8]
.L_080db3fe:
	movs	r1, #0
	ldr	r2, [sp, #8]
	str	r1, [sp, #16]
	movs	r1, #128
	lsls	r1, r1, #7
	adds	r3, r2, r1
	ldr	r2, [sp, #20]
	adds	r5, r2, #0
	muls	r5, r3
	adds	r0, r5, #0
	bl	sub_08002322
	ldr	r1, [sp, #28]
	movs	r3, #32
	subs	r3, r3, r1
	muls	r3, r0
	asrs	r3, r3, #16
	adds	r3, #64
	adds	r0, r5, #0
	mov	r9, r3
	bl	sub_0800231c
	ldr	r3, [pc, #588]
	add	r3, sl
	lsls	r0, r0, #3
	ldr	r3, [r3, #0]
	asrs	r0, r0, #16
	negs	r0, r0
	ldr	r3, [r3, #24]
	adds	r6, r0, #0
	subs	r6, #8
	cmp	r3, #0
	bne.n	.L_080db47a
	bl	sub_08004458
	movs	r3, #3
	ands	r0, r3
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r5, r3, #4
	subs	r5, r5, r3
	ldr	r2, [pc, #596]
	lsls	r5, r5, #6
	add	r5, sl
	adds	r5, r5, r2
	bl	sub_08004458
	movs	r2, #7
	ands	r2, r0
	movs	r3, #24
	add	r2, r9
	str	r3, [sp, #0]
	movs	r3, #120
	str	r3, [sp, #4]
	subs	r2, #16
	ldr	r4, [sp, #36]
	ldr	r0, [sp, #32]
	adds	r1, r5, #0
	adds	r3, r6, #0
	bl	sub_080072f4
	b.n	.L_080db4bc
.L_080db47a:
	bl	sub_08004458
	movs	r3, #3
	ands	r0, r3
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r5, r3, #4
	subs	r5, r5, r3
	lsls	r5, r5, #6
	ldr	r3, [pc, #536]
	add	r5, sl
	adds	r5, r5, r3
	bl	sub_08004458
	ldr	r3, [sp, #20]
	movs	r1, #1
	ands	r1, r3
	movs	r3, #24
	str	r3, [sp, #0]
	movs	r2, #7
	movs	r3, #120
	ands	r2, r0
	str	r3, [sp, #4]
	ldr	r3, [sp, #12]
	lsls	r1, r1, #2
	add	r2, r9
	ldr	r4, [r1, r3]
	subs	r2, #16
	ldr	r0, [sp, #32]
	adds	r1, r5, #0
	adds	r3, r6, #0
	bl	sub_080072f4
.L_080db4bc:
	adds	r3, r6, #0
	adds	r3, #112
	movs	r1, #0
	lsls	r3, r3, #16
	ldr	r7, [pc, #448]
	mov	r8, r1
	mov	fp, r3
.L_080db4ca:
	ldr	r3, [r7, #24]
	cmp	r3, #0
	bne.n	.L_080db538
	bl	sub_08004458
	ldr	r6, [pc, #468]
	ands	r6, r0
	bl	sub_08004458
	ldr	r5, [pc, #432]
	ldr	r2, [pc, #436]
	mov	r1, r9
	ands	r5, r0
	lsls	r3, r1, #16
	adds	r5, r5, r2
	mov	r2, fp
	str	r2, [r7, #4]
	str	r3, [r7, #0]
	adds	r0, r5, #0
	bl	sub_08002322
	adds	r6, #128
	adds	r3, r6, #0
	muls	r3, r0
	asrs	r3, r3, #9
	str	r3, [r7, #12]
	adds	r0, r5, #0
	bl	sub_0800231c
	adds	r3, r6, #0
	muls	r3, r0
	lsls	r3, r3, #1
	negs	r3, r3
	asrs	r3, r3, #7
	str	r3, [r7, #16]
	bl	sub_08004458
	movs	r3, #7
	ands	r3, r0
	adds	r3, #32
	str	r3, [r7, #24]
	ldr	r3, [sp, #16]
	adds	r3, #1
	str	r3, [sp, #16]
	ldr	r3, [pc, #340]
	add	r3, sl
	ldr	r3, [r3, #0]
	ldr	r3, [r3, #24]
	ldr	r1, [pc, #388]
	lsls	r3, r3, #1
	adds	r3, #1
	ldrb	r3, [r1, r3]
	ldr	r2, [sp, #16]
	cmp	r2, r3
	beq.n	.L_080db546
.L_080db538:
	movs	r3, #1
	movs	r1, #128
	add	r8, r3
	lsls	r1, r1, #3
	adds	r7, #28
	cmp	r8, r1
	bne.n	.L_080db4ca
.L_080db546:
	ldr	r2, [sp, #20]
	adds	r2, #1
	str	r2, [sp, #20]
	cmp	r2, #4
	beq.n	.L_080db552
	b.n	.L_080db3fe
.L_080db552:
	ldr	r2, [pc, #352]
	movs	r3, #1
	add	r2, sl
	str	r3, [r2, #0]
.L_080db55a:
	movs	r3, #0
	ldr	r5, [pc, #296]
	mov	r8, r3
.L_080db560:
	ldr	r3, [r5, #24]
	cmp	r3, #0
	ble.n	.L_080db5ca
	subs	r3, #1
	str	r3, [r5, #24]
	movs	r1, #60
	adds	r0, r5, #0
	ldr	r2, [pc, #328]
	bl	sub_080e3908
	movs	r1, #240
	ldr	r3, [r5, #4]
	lsls	r1, r1, #15
	cmp	r3, r1
	ble.n	.L_080db58c
	ldr	r3, [r5, #16]
	negs	r3, r3
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	asrs	r3, r3, #1
	str	r3, [r5, #16]
	b.n	.L_080db5ca
.L_080db58c:
	ldr	r2, [r5, #0]
	ldr	r1, [pc, #300]
	cmp	r2, r1
	bhi.n	.L_080db5ca
	cmp	r3, #0
	blt.n	.L_080db5ca
	ldr	r0, [r5, #24]
	asrs	r6, r2, #16
	asrs	r7, r3, #16
	cmp	r0, #0
	bge.n	.L_080db5a4
	adds	r0, #7
.L_080db5a4:
	asrs	r0, r0, #3
	adds	r0, #1
	lsls	r4, r0, #1
	ldr	r2, [pc, #276]
	subs	r3, r4, #2
	ldrh	r1, [r2, r3]
	ldr	r2, [sp, #24]
	adds	r1, r2, r1
	lsrs	r2, r0, #31
	adds	r2, r0, r2
	asrs	r2, r2, #1
	subs	r3, r7, r0
	str	r0, [sp, #0]
	str	r4, [sp, #4]
	subs	r2, r6, r2
	ldr	r4, [sp, #36]
	ldr	r0, [sp, #32]
	bl	sub_080072f4
.L_080db5ca:
	movs	r3, #1
	movs	r1, #128
	add	r8, r3
	lsls	r1, r1, #3
	adds	r5, #28
	cmp	r8, r1
	bne.n	.L_080db560
	ldr	r3, [sp, #28]
	subs	r3, #4
	cmp	r3, #91
	bhi.n	.L_080db622
	movs	r2, #0
	mov	r8, r2
	ldr	r2, [pc, #144]
	mov	r1, sl
	ldr	r3, [r1, r2]
	ldr	r3, [r3, #20]
	cmp	r3, #0
	beq.n	.L_080db622
	movs	r6, #36
	movs	r5, #4
.L_080db5f4:
	ldr	r3, [sp, #28]
	cmp	r3, r5
	bne.n	.L_080db60e
	mov	r1, sl
	ldr	r3, [r1, r2]
	ldrsh	r0, [r3, r6]
	movs	r3, #10
	str	r3, [sp, #0]
	movs	r1, #7
	movs	r2, #5
	mov	r3, r8
	bl	sub_080d6888
.L_080db60e:
	ldr	r2, [pc, #104]
	movs	r3, #1
	mov	r1, sl
	add	r8, r3
	ldr	r3, [r1, r2]
	ldr	r3, [r3, #20]
	adds	r6, #2
	adds	r5, #4
	cmp	r8, r3
	bne.n	.L_080db5f4
.L_080db622:
	movs	r0, #2
	movs	r1, #4
	bl	sub_080e155c
	bl	sub_080cd52c
	ldr	r2, [pc, #148]
	movs	r3, #1
	add	r2, sl
	str	r3, [r2, #0]
	movs	r0, #1
	bl	sub_080030f8
	ldr	r2, [sp, #28]
	adds	r2, #1
	str	r2, [sp, #28]
	cmp	r2, #64
	beq.n	.L_080db648
	b.n	.L_080db3c6
.L_080db648:
	ldr	r0, [pc, #80]
	bl	sub_08004278
	movs	r0, #47
	bl	sub_08002dd8
	movs	r0, #46
	bl	sub_08002dd8
	bl	sub_080cdbc0
	add	sp, #44
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x000000c4
	.4byte 0x00000073
	.4byte 0x00007828
	.4byte 0x04000028
	.4byte 0xfffff000
	.4byte 0xffff8000
	.4byte 0x02010000
	.4byte 0x000003ff
	.4byte 0x00007fff
	.4byte 0xffffc000
	.4byte 0x00007784
	.4byte 0x080cd261
	.4byte 0x03000168
	.4byte 0x10101010
	.4byte 0x0000060e
	.4byte 0x000001ff
	.4byte 0x080eeadc
	.4byte 0x000077a8
	.4byte 0xfffff800
	.4byte 0x007effff
	.4byte 0x080ede48
	.4byte 0x00007824
