.syntax unified
	.thumb
	.set sub_08002064, 0x08002064
	.set sub_08016ca4, 0x08016ca4
	.set sub_080ad000, 0x080ad000
	.set sub_080ad020, 0x080ad020
	.set sub_080ad048, 0x080ad048
	.set sub_080ad050, 0x080ad050
	.set sub_080ad080, 0x080ad080
	.set sub_080ad0d8, 0x080ad0d8
	.set sub_080ad0e0, 0x080ad0e0
	.set sub_080ad0f8, 0x080ad0f8
	.set sub_080ad110, 0x080ad110
	.set sub_080ad150, 0x080ad150
	.set sub_080ad158, 0x080ad158
	.set sub_080ad198, 0x080ad198
	.set sub_080ad298, 0x080ad298
	.set sub_08123534, 0x08123534
	.global Func_081185c4
	.thumb_func
Func_081185c4:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	ldr	r1, [pc, #348]
	ldr	r7, [pc, #348]
	movs	r4, #0
	movs	r2, #148
	sub	sp, #4
	mov	r9, r1
	mov	r8, r2
	mov	sl, r4
.L_081185de:
	ldr	r5, [pc, #340]
	str	r4, [sp, #0]
	ldrb	r0, [r5, r4]
	bl	sub_08016ca4
	ldr	r4, [sp, #0]
	adds	r6, r0, #0
	ldrb	r0, [r5, r4]
	bl	sub_08123534
	ldr	r4, [sp, #0]
	cmp	r0, #0
	bne.n	.L_081185fa
	b.n	.L_08118710
.L_081185fa:
	mov	r1, r9
	ldrb	r3, [r1, r4]
	cmp	r3, #0
	bne.n	.L_08118604
	b.n	.L_08118710
.L_08118604:
	cmp	r4, #2
	bgt.n	.L_08118626
	movs	r2, #150
	mov	r3, sl
	lsls	r2, r2, #1
	add	r3, r9
	adds	r1, r6, r2
	adds	r2, r3, #0
	adds	r2, #8
	movs	r0, #3
.L_08118618:
	ldrb	r3, [r2, #0]
	subs	r0, #1
	strb	r3, [r1, #0]
	adds	r2, #1
	adds	r1, #1
	cmp	r0, #0
	bge.n	.L_08118618
.L_08118626:
	ldrb	r2, [r7, #0]
	movs	r1, #152
	lsls	r1, r1, #1
	adds	r3, r6, r1
	strb	r2, [r3, #0]
	ldrb	r3, [r7, #1]
	adds	r1, #1
	adds	r2, r6, r1
	strb	r3, [r2, #0]
	ldrb	r3, [r7, #2]
	adds	r1, #1
	adds	r2, r6, r1
	strb	r3, [r2, #0]
	ldrb	r3, [r7, #3]
	adds	r1, #1
	adds	r2, r6, r1
	strb	r3, [r2, #0]
	ldrb	r2, [r7, #4]
	adds	r1, #1
	adds	r3, r6, r1
	strb	r2, [r3, #0]
	ldrb	r3, [r7, #5]
	adds	r1, #1
	adds	r2, r6, r1
	strb	r3, [r2, #0]
	ldrb	r3, [r7, #6]
	adds	r1, #1
	adds	r2, r6, r1
	strb	r3, [r2, #0]
	ldrb	r3, [r7, #7]
	adds	r1, #1
	adds	r2, r6, r1
	strb	r3, [r2, #0]
	ldrb	r2, [r7, #8]
	adds	r1, #1
	adds	r3, r6, r1
	strb	r2, [r3, #0]
	ldrb	r3, [r7, #9]
	adds	r1, #1
	adds	r2, r6, r1
	strb	r3, [r2, #0]
	ldrb	r3, [r7, #10]
	adds	r1, #1
	adds	r2, r6, r1
	strb	r3, [r2, #0]
	ldrb	r3, [r7, #11]
	adds	r1, #1
	adds	r2, r6, r1
	strb	r3, [r2, #0]
	ldrb	r2, [r7, #12]
	adds	r1, #1
	adds	r3, r6, r1
	strb	r2, [r3, #0]
	ldrb	r3, [r7, #13]
	adds	r1, #1
	adds	r2, r6, r1
	strb	r3, [r2, #0]
	ldrb	r3, [r7, #14]
	adds	r1, #1
	adds	r2, r6, r1
	strb	r3, [r2, #0]
	ldrb	r3, [r7, #15]
	adds	r1, #1
	adds	r2, r6, r1
	strb	r3, [r2, #0]
	ldrb	r2, [r7, #16]
	adds	r1, #1
	adds	r3, r6, r1
	strb	r2, [r3, #0]
	ldrb	r3, [r7, #17]
	adds	r1, #1
	adds	r2, r6, r1
	strb	r3, [r2, #0]
	ldrb	r3, [r7, #18]
	adds	r1, #1
	adds	r2, r6, r1
	strb	r3, [r2, #0]
	ldrb	r3, [r7, #19]
	adds	r1, #1
	adds	r2, r6, r1
	strb	r3, [r2, #0]
	ldrb	r2, [r7, #20]
	adds	r1, #1
	adds	r3, r6, r1
	strb	r2, [r3, #0]
	ldrb	r3, [r7, #21]
	adds	r1, #1
	adds	r2, r6, r1
	strb	r3, [r2, #0]
	ldrb	r3, [r7, #22]
	adds	r1, #1
	adds	r2, r6, r1
	strb	r3, [r2, #0]
	ldrb	r3, [r7, #23]
	adds	r1, #1
	adds	r2, r6, r1
	strb	r3, [r2, #0]
	ldrb	r2, [r7, #24]
	adds	r1, #1
	adds	r3, r6, r1
	strb	r2, [r3, #0]
	cmp	r4, #2
	ble.n	.L_08118710
	mov	r2, r8
	mov	r3, r9
	ldrh	r1, [r2, r3]
	adds	r0, r6, #0
	str	r4, [sp, #0]
	bl	sub_080ad0d8
	ldr	r3, [pc, #40]
	adds	r0, r6, #0
	add	r3, r8
	ldrh	r1, [r3, #2]
	bl	sub_080ad0e0
	ldr	r4, [sp, #0]
.L_08118710:
	movs	r1, #4
	adds	r4, #1
	add	r8, r1
	add	sl, r1
	adds	r7, #28
	cmp	r4, #4
	bgt.n	.L_08118720
	b.n	.L_081185de
.L_08118720:
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.4byte 0x0200ff58
	.4byte 0x0200ff6c
	.2byte 0xa16c
	.2byte 0x0812
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #40
	movs	r1, #0
	str	r0, [sp, #20]
	movs	r0, #0
	str	r1, [sp, #16]
	bl	sub_080ad110
	movs	r0, #1
	bl	sub_080ad110
	movs	r0, #2
	bl	sub_080ad110
	movs	r0, #3
	bl	sub_080ad110
	movs	r0, #5
	bl	sub_080ad110
	movs	r0, #4
	bl	sub_080ad110
	movs	r0, #6
	bl	sub_080ad110
	movs	r0, #7
	bl	sub_080ad110
	movs	r0, #0
	bl	sub_080ad000
	movs	r2, #148
	ldr	r3, [sp, #16]
	lsls	r2, r2, #1
	mov	r1, sp
	adds	r0, r0, r2
	adds	r1, #24
	movs	r5, #0
	str	r3, [r0, #0]
	str	r5, [sp, #8]
	str	r1, [sp, #4]
	movs	r2, #0
	add	r3, sp, #36
	mov	ip, r1
.L_0811879e:
	str	r2, [r3, #0]
	subs	r3, #4
	cmp	r3, ip
	bge.n	.L_0811879e
	movs	r2, #0
	str	r2, [sp, #12]
	b.n	.L_081188f0
.L_081187ac:
	ldr	r3, [sp, #8]
	adds	r3, #1
	str	r3, [sp, #8]
	b.n	.L_081188ea
.L_081187b4:
	ldr	r1, [sp, #8]
	ldr	r2, [sp, #20]
	cmp	r1, r2
	bge.n	.L_081187be
	b.n	.L_081188ea
.L_081187be:
	mov	r0, r8
	bl	sub_080ad0f8
	ldr	r1, [pc, #104]
	mov	r0, r8
	adds	r3, r5, r1
	ldrb	r1, [r3, #1]
	bl	sub_080ad198
	mov	r0, r8
	bl	sub_08016ca4
	movs	r5, #140
	adds	r1, r0, #0
	adds	r2, r1, #0
	lsls	r5, r5, #1
	movs	r4, #0
	movs	r0, #0
	adds	r2, #248
	adds	r3, r1, r5
	movs	r7, #3
.L_081187e8:
	subs	r7, #1
	strb	r4, [r3, #0]
	strb	r4, [r3, #4]
	str	r0, [r2, #0]
	str	r0, [r2, #16]
	adds	r3, #1
	adds	r2, #4
	cmp	r7, #0
	bge.n	.L_081187e8
	ldr	r3, [pc, #48]
	adds	r0, r1, #0
	movs	r7, #31
	adds	r0, #212
.L_08118802:
	subs	r7, #1
	strh	r3, [r0, #0]
	subs	r0, #4
	cmp	r7, #0
	bge.n	.L_08118802
	ldr	r3, [sp, #12]
	ldr	r1, [pc, #32]
	add	r3, fp
	lsls	r3, r3, #3
	adds	r3, r1, r3
	adds	r5, r3, #0
	adds	r5, #18
	movs	r7, #1
.L_0811881c:
	ldrh	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_08118834
	adds	r1, r3, #0
	mov	r0, r8
	bl	sub_080ad080
	b.n	.L_08118834
	.4byte 0x00000000
	.2byte 0xa174
	.2byte 0x0812
.L_08118834:
	subs	r7, #1
	adds	r5, #2
	cmp	r7, #0
	bge.n	.L_0811881c
	ldr	r3, [sp, #12]
	movs	r2, #0
	add	r3, fp
	lsls	r3, r3, #3
	movs	r7, #0
	mov	sl, r3
	mov	r9, r2
.L_0811884a:
	ldr	r2, [pc, #256]
	mov	r5, sl
	adds	r2, #2
	ldrsb	r3, [r2, r5]
	movs	r4, #0
	cmp	r4, r3
	bge.n	.L_0811889a
	ldr	r6, [sp, #4]
	mov	r5, r9
.L_0811885c:
	ldr	r0, [r6, r5]
	movs	r1, #18
	adds	r0, #7
	str	r4, [sp, #0]
	bl	sub_08002064
	adds	r1, r7, #0
	adds	r2, r0, #0
	mov	r0, r8
	bl	sub_080ad150
	ldr	r0, [r6, r5]
	movs	r1, #18
	adds	r0, #7
	bl	sub_08002064
	adds	r1, r7, #0
	adds	r2, r0, #0
	mov	r0, r8
	bl	sub_080ad158
	ldr	r3, [r6, r5]
	ldr	r1, [pc, #196]
	adds	r3, #1
	ldr	r4, [sp, #0]
	mov	r2, sl
	str	r3, [r6, r5]
	ldrsb	r3, [r1, r2]
	adds	r4, #1
	cmp	r4, r3
	blt.n	.L_0811885c
.L_0811889a:
	movs	r3, #1
	movs	r5, #4
	adds	r7, #1
	add	sl, r3
	add	r9, r5
	cmp	r7, #3
	ble.n	.L_0811884a
	movs	r7, #15
.L_081188aa:
	mov	r0, r8
	movs	r1, #0
	subs	r7, #1
	bl	sub_080ad050
	cmp	r7, #0
	bge.n	.L_081188aa
	ldr	r3, [sp, #12]
	ldr	r2, [pc, #144]
	add	r3, fp
	lsls	r3, r3, #3
	adds	r3, r3, r2
	adds	r5, r3, #6
	movs	r7, #5
.L_081188c6:
	ldrh	r3, [r5, #0]
	cmp	r3, #0
	beq.n	.L_081188dc
	adds	r1, r3, #0
	mov	r0, r8
	bl	sub_080ad020
	adds	r1, r0, #0
	mov	r0, r8
	bl	sub_080ad048
.L_081188dc:
	subs	r7, #1
	adds	r5, #2
	cmp	r7, #0
	bge.n	.L_081188c6
	mov	r0, r8
	bl	sub_080ad298
.L_081188ea:
	ldr	r1, [sp, #12]
	adds	r1, #1
	str	r1, [sp, #12]
.L_081188f0:
	ldr	r2, [sp, #12]
	ldr	r3, [sp, #12]
	lsls	r2, r2, #1
	mov	fp, r2
	add	r3, fp
	lsls	r5, r3, #3
	ldr	r3, [pc, #76]
	ldr	r1, [sp, #12]
	ldrsb	r3, [r3, r5]
	movs	r2, #220
	lsls	r2, r2, #1
	mov	r8, r3
	cmp	r1, r2
	bls.n	.L_08118912
	movs	r3, #1
	str	r3, [sp, #16]
	b.n	.L_0811893a
.L_08118912:
	movs	r1, #1
	negs	r1, r1
	cmp	r8, r1
	beq.n	.L_0811891c
	b.n	.L_081187b4
.L_0811891c:
	ldr	r2, [sp, #8]
	cmp	r2, #69
	bgt.n	.L_0811892c
	ldr	r3, [sp, #20]
	cmp	r2, r3
	beq.n	.L_0811892a
	b.n	.L_081187ac
.L_0811892a:
	b.n	.L_0811893a
.L_0811892c:
	ldr	r5, [sp, #20]
	ldr	r1, [sp, #8]
	movs	r3, #1
	orrs	r3, r5
	cmp	r1, r3
	beq.n	.L_0811893a
	b.n	.L_081187ac
.L_0811893a:
	ldr	r0, [sp, #16]
	add	sp, #40
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0812a174
	.2byte 0xa176
	.2byte 0x0812
	bx	lr
	.2byte 0x0000
