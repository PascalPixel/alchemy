.syntax unified
	.thumb
	.set sub_0801489c, 0x0801489c
	.set sub_08016ce4, 0x08016ce4
	.set sub_080201c0, 0x080201c0
	.set sub_08020358, 0x08020358
	.set sub_080cb8e8, 0x080cb8e8
	.set sub_080cc9ac, 0x080cc9ac
	.set sub_080ccd78, 0x080ccd78
	.global Overlay_080cc54c
Overlay_080cc54c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r0, #0
	sub	sp, #12
	mov	r8, r0
	bl	sub_080cb8e8
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r1, [r3, #108]
	adds	r7, r0, #0
	mov	sl, r1
	ldr	r6, [r3, #32]
	cmp	r7, #0
	beq.n	.L_080cc65e
	ldr	r3, [r7, #8]
	mov	r5, sp
	str	r3, [r5, #0]
	ldr	r3, [r7, #12]
	movs	r0, #128
	str	r3, [r5, #4]
	ldr	r3, [r7, #16]
	lsls	r0, r0, #13
	str	r3, [r5, #8]
	adds	r2, r5, #0
	ldrh	r1, [r7, #6]
	bl	sub_0801489c
	movs	r3, #197
	lsls	r3, r3, #1
	add	r3, sl
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #3
	bne.n	.L_080cc5c4
	ldr	r3, [r5, #0]
	ldr	r1, [r5, #8]
	cmp	r3, #0
	bge.n	.L_080cc5a4
	ldr	r2, [pc, #200]
	adds	r3, r3, r2
.L_080cc5a4:
	asrs	r2, r3, #21
	movs	r0, #31
	adds	r3, r1, #0
	ands	r2, r0
	cmp	r3, #0
	bge.n	.L_080cc5b4
	ldr	r1, [pc, #184]
	adds	r3, r3, r1
.L_080cc5b4:
	asrs	r3, r3, #21
	ands	r3, r0
	lsls	r3, r3, #5
	adds	r3, r2, r3
	ldr	r2, [pc, #176]
	lsls	r3, r3, #2
	adds	r1, r3, r2
	b.n	.L_080cc5ee
.L_080cc5c4:
	movs	r0, #156
	lsls	r0, r0, #1
	adds	r3, r6, r0
	ldr	r1, [r3, #0]
	ldr	r3, [r5, #0]
	ldr	r2, [r5, #8]
	cmp	r3, #0
	bge.n	.L_080cc5d8
	ldr	r0, [pc, #156]
	adds	r3, r3, r0
.L_080cc5d8:
	asrs	r0, r3, #20
	adds	r3, r2, #0
	cmp	r3, #0
	bge.n	.L_080cc5e4
	ldr	r2, [pc, #144]
	adds	r3, r3, r2
.L_080cc5e4:
	asrs	r3, r3, #20
	lsls	r3, r3, #7
	adds	r3, r0, r3
	lsls	r3, r3, #2
	adds	r1, r1, r3
.L_080cc5ee:
	ldrb	r6, [r1, #2]
	adds	r3, r6, #0
	subs	r3, #242
	cmp	r3, #5
	bhi.n	.L_080cc61a
	adds	r3, r7, #0
	adds	r3, #34
	ldr	r2, [r5, #8]
	ldrb	r0, [r3, #0]
	ldr	r1, [r5, #0]
	bl	sub_080201c0
	adds	r2, r0, #0
	ldr	r0, [r7, #12]
	cmp	r2, r0
	blt.n	.L_080cc628
	movs	r1, #128
	lsls	r1, r1, #13
	adds	r3, r0, r1
	cmp	r2, r3
	bgt.n	.L_080cc628
	b.n	.L_080cc626
.L_080cc61a:
	movs	r0, #3
	adds	r1, r6, #0
	bl	sub_080ccd78
	cmp	r0, #0
	beq.n	.L_080cc628
.L_080cc626:
	mov	r8, r6
.L_080cc628:
	mov	r2, r8
	cmp	r2, #0
	bne.n	.L_080cc65e
	movs	r3, #197
	lsls	r3, r3, #1
	add	r3, sl
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #3
	bne.n	.L_080cc65e
	ldr	r3, [pc, #56]
	movs	r0, #128
	lsls	r0, r0, #2
	adds	r0, #118
	adds	r3, r3, r0
	movs	r1, #0
	ldrsh	r3, [r3, r1]
	cmp	r3, #0
	beq.n	.L_080cc65e
	adds	r0, r5, #0
	bl	sub_08020358
	cmp	r0, #3
	bne.n	.L_080cc65e
	movs	r2, #99
	mov	r8, r2
.L_080cc65e:
	mov	r0, r8
	add	sp, #12
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x001fffff
	.4byte 0x02020000
	.4byte 0x000fffff
	.2byte 0x0240
	.2byte 0x0200
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r7, [r3, #32]
	ldr	r3, [pc, #296]
	adds	r5, r0, #0
	movs	r0, #241
	lsls	r0, r0, #1
	adds	r3, r3, r0
	movs	r2, #0
	ldrsh	r1, [r3, r2]
	ldr	r3, [pc, #284]
	mov	r8, r1
	ldr	r0, [r3, #12]
	mov	lr, r0
	.2byte 0xf800
	.2byte 0x2300
	adds	r6, r0, #0
	mov	sl, r3
	cmp	r5, #0
	beq.n	.L_080cc6dc
	movs	r5, #1
	negs	r5, r5
	b.n	.L_080cc6b4
.L_080cc6b2:
	adds	r6, #24
.L_080cc6b4:
	movs	r4, #0
	ldrsh	r3, [r6, r4]
	cmp	r3, r5
	beq.n	.L_080cc6d4
	cmp	r3, r8
	bne.n	.L_080cc6b2
	movs	r1, #2
	ldrsh	r0, [r6, r1]
	cmp	r0, r5
	beq.n	.L_080cc6d0
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_080cc6b2
.L_080cc6d0:
	movs	r2, #1
	mov	sl, r2
.L_080cc6d4:
	mov	r3, sl
	movs	r0, #0
	cmp	r3, #0
	beq.n	.L_080cc7ac
.L_080cc6dc:
	movs	r0, #10
	adds	r0, #255
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080cc726
	ldr	r1, [pc, #200]
	movs	r4, #254
	lsls	r4, r4, #1
	adds	r2, r1, r4
	movs	r4, #4
	ldrsh	r3, [r6, r4]
	lsls	r3, r3, #16
	str	r3, [r2, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	adds	r2, r1, r3
	movs	r4, #6
	ldrsh	r3, [r6, r4]
	lsls	r3, r3, #16
	str	r3, [r2, #0]
	movs	r3, #129
	lsls	r3, r3, #2
	adds	r2, r1, r3
	movs	r4, #8
	ldrsh	r3, [r6, r4]
	movs	r4, #131
	lsls	r3, r3, #16
	str	r3, [r2, #0]
	movs	r3, #130
	lsls	r3, r3, #2
	adds	r2, r1, r3
	ldrh	r3, [r6, #10]
	lsls	r4, r4, #2
	str	r3, [r2, #0]
	adds	r3, r1, r4
	strh	r0, [r3, #0]
.L_080cc726:
	movs	r0, #14
	ldrsh	r3, [r6, r0]
	movs	r1, #1
	negs	r1, r1
	cmp	r3, r1
	beq.n	.L_080cc73c
	adds	r5, r7, #0
	adds	r5, #236
	lsls	r3, r3, #16
	str	r3, [r5, #0]
	b.n	.L_080cc740
.L_080cc73c:
	adds	r5, r7, #0
	adds	r5, #236
.L_080cc740:
	movs	r2, #16
	ldrsh	r3, [r6, r2]
	cmp	r3, r1
	beq.n	.L_080cc752
	adds	r4, r7, #0
	adds	r4, #240
	lsls	r3, r3, #16
	str	r3, [r4, #0]
	b.n	.L_080cc756
.L_080cc752:
	adds	r4, r7, #0
	adds	r4, #240
.L_080cc756:
	movs	r0, #18
	ldrsh	r3, [r6, r0]
	cmp	r3, r1
	beq.n	.L_080cc768
	adds	r2, r7, #0
	adds	r2, #244
	lsls	r3, r3, #16
	str	r3, [r2, #0]
	b.n	.L_080cc76c
.L_080cc768:
	adds	r2, r7, #0
	adds	r2, #244
.L_080cc76c:
	movs	r3, #20
	ldrsh	r0, [r6, r3]
	cmp	r0, r1
	beq.n	.L_080cc77e
	adds	r1, r7, #0
	adds	r1, #248
	lsls	r3, r0, #16
	str	r3, [r1, #0]
	b.n	.L_080cc782
.L_080cc77e:
	adds	r1, r7, #0
	adds	r1, #248
.L_080cc782:
	ldr	r3, [r5, #0]
	movs	r0, #240
	ldr	r2, [r2, #0]
	lsls	r0, r0, #16
	adds	r3, r3, r0
	cmp	r3, r2
	ble.n	.L_080cc796
	ldr	r0, [pc, #40]
	adds	r3, r2, r0
	str	r3, [r5, #0]
.L_080cc796:
	ldr	r3, [r4, #0]
	movs	r2, #160
	lsls	r2, r2, #16
	adds	r3, r3, r2
	ldr	r2, [r1, #0]
	cmp	r3, r2
	ble.n	.L_080cc7aa
	ldr	r0, [pc, #24]
	adds	r3, r2, r0
	str	r3, [r4, #0]
.L_080cc7aa:
	movs	r0, #1
.L_080cc7ac:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.4byte 0x02000240
	.4byte 0x02008000
	.4byte 0xff100000
	.2byte 0x0000
	.2byte 0xff60
	.2byte 0xb5e0
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	sub	sp, #8
	ldr	r3, [r3, #32]
	ldr	r6, [pc, #412]
	movs	r2, #4
	ldr	r5, [pc, #412]
	add	r2, sp
	movs	r0, #241
	lsls	r0, r0, #1
	mov	sl, r2
	mov	fp, r3
	adds	r3, r6, r0
	mov	r0, sl
	str	r5, [sp, #4]
	movs	r1, #0
	ldrsh	r7, [r3, r1]
	bl	.L_080cc994
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	adds	r0, r0, r5
	str	r0, [sp, #4]
.L_080cc800:
	ldr	r3, [sp, #4]
	movs	r5, #1
	movs	r2, #0
	ldrsb	r2, [r3, r2]
	negs	r5, r5
	cmp	r2, r5
	bne.n	.L_080cc820
	ldr	r0, [pc, #368]
	movs	r3, #128
	ldr	r1, [pc, #368]
	lsls	r3, r3, #7
	str	r3, [r0, #0]
	movs	r3, #0
	strh	r3, [r1, #0]
	movs	r0, #0
	b.n	.L_080cc968
.L_080cc820:
	cmp	r2, r7
	beq.n	.L_080cc826
	b.n	.L_080cc962
.L_080cc826:
	adds	r3, #1
	mov	r0, sl
	str	r3, [sp, #4]
	bl	sub_080cc9ac
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	mov	r8, r0
	mov	r0, sl
	bl	sub_080cc9ac
	lsls	r0, r0, #16
	asrs	r0, r0, #16
	mov	r2, r8
	lsls	r6, r2, #16
	lsls	r7, r0, #16
	mov	r9, r0
	adds	r1, r6, #0
	movs	r0, #0
	adds	r2, r7, #0
	bl	sub_080201c0
	asrs	r0, r0, #16
	str	r0, [sp, #0]
	ldr	r3, [sp, #4]
	movs	r0, #10
	adds	r0, #255
	ldrb	r5, [r3, #0]
	bl	sub_08016ce4
	cmp	r0, #0
	bne.n	.L_080cc898
	ldr	r2, [pc, #272]
	movs	r1, #254
	lsls	r1, r1, #1
	adds	r3, r2, r1
	adds	r1, #8
	str	r6, [r3, #0]
	adds	r3, r2, r1
	str	r7, [r3, #0]
	ldr	r1, [pc, #264]
	movs	r3, #15
	ands	r3, r5
	lsls	r3, r3, #12
	str	r3, [r1, #0]
	ldr	r3, [pc, #256]
	strh	r0, [r3, #0]
	movs	r3, #16
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L_080cc898
	movs	r0, #128
	lsls	r0, r0, #2
	movs	r3, #128
	adds	r2, r2, r0
	lsls	r3, r3, #12
	str	r3, [r2, #0]
.L_080cc898:
	movs	r3, #32
	ands	r3, r5
	movs	r0, #1
	cmp	r3, #0
	bne.n	.L_080cc968
	ldr	r3, [pc, #228]
	mov	r0, sl
	str	r3, [sp, #4]
	bl	.L_080cc994
	ldr	r1, [pc, #204]
	lsls	r0, r0, #16
	lsrs	r0, r0, #16
	adds	r0, r0, r1
	str	r0, [sp, #4]
	movs	r2, #1
	movs	r3, #0
	ldrsb	r3, [r0, r3]
	negs	r2, r2
	cmp	r3, r2
	beq.n	.L_080cc95e
	movs	r3, #244
	add	r3, fp
	mov	lr, r3
	mov	sl, r2
.L_080cc8ca:
	adds	r3, r0, #1
	movs	r5, #0
	ldrsb	r5, [r0, r5]
	str	r3, [sp, #4]
	mov	ip, r5
	movs	r7, #0
	ldrsb	r7, [r3, r7]
	adds	r3, #1
	str	r3, [sp, #4]
	lsls	r2, r5, #4
	movs	r6, #0
	ldrsb	r6, [r3, r6]
	adds	r3, #1
	str	r3, [sp, #4]
	lsls	r4, r7, #4
	movs	r5, #0
	ldrsb	r5, [r3, r5]
	adds	r3, #1
	lsls	r0, r6, #4
	lsls	r1, r5, #4
	str	r3, [sp, #4]
	cmp	r8, r2
	blt.n	.L_080cc954
	cmp	r8, r0
	bgt.n	.L_080cc954
	ldr	r2, [sp, #0]
	mov	r0, r9
	subs	r3, r0, r2
	cmp	r3, r4
	blt.n	.L_080cc954
	cmp	r3, r1
	bgt.n	.L_080cc954
	mov	r0, ip
	mov	r1, fp
	lsls	r3, r0, #20
	adds	r1, #236
	mov	r0, fp
	str	r3, [r1, #0]
	adds	r0, #240
	lsls	r3, r7, #20
	str	r3, [r0, #0]
	mov	r2, lr
	lsls	r3, r6, #20
	mov	r4, fp
	str	r3, [r2, #0]
	adds	r4, #248
	lsls	r3, r5, #20
	str	r3, [r4, #0]
	movs	r5, #240
	ldr	r3, [r1, #0]
	ldr	r2, [r2, #0]
	lsls	r5, r5, #16
	adds	r3, r3, r5
	cmp	r3, r2
	ble.n	.L_080cc93e
	ldr	r5, [pc, #80]
	adds	r3, r2, r5
	str	r3, [r1, #0]
.L_080cc93e:
	ldr	r3, [r0, #0]
	movs	r1, #160
	ldr	r2, [r4, #0]
	lsls	r1, r1, #16
	adds	r3, r3, r1
	cmp	r3, r2
	ble.n	.L_080cc95e
	ldr	r5, [pc, #64]
	adds	r3, r2, r5
	str	r3, [r0, #0]
	b.n	.L_080cc95e
.L_080cc954:
	ldr	r0, [sp, #4]
	movs	r3, #0
	ldrsb	r3, [r0, r3]
	cmp	r3, sl
	bne.n	.L_080cc8ca
.L_080cc95e:
	movs	r0, #1
	b.n	.L_080cc968
.L_080cc962:
	adds	r3, #6
	str	r3, [sp, #4]
	b.n	.L_080cc800
.L_080cc968:
	add	sp, #8
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x0202e000
	.4byte 0x02000448
	.4byte 0x0200044c
	.4byte 0x0202e004
	.4byte 0xff100000
	.2byte 0x0000
	.2byte 0xff60
.L_080cc994:
	.2byte 0x6803
	ldrb	r1, [r3, #0]
	adds	r3, #1
	str	r3, [r0, #0]
	ldrb	r2, [r3, #0]
	adds	r3, #1
	lsls	r2, r2, #8
	orrs	r1, r2
	str	r3, [r0, #0]
	adds	r0, r1, #0
	bx	lr
