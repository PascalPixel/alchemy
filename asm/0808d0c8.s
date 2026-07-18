@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0808d0c8
	.thumb_func
Func_0808d0c8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	movs	r1, #0
	movs	r2, #1
	mov	r9, r1
	ldr	r0, [pc, #32]
	ldr	r1, [pc, #36]
	sub	sp, #12
	mov	sl, r2
	mov	r8, r2
	bl	Func_080052f4
	movs	r3, #0
	str	r3, [sp, #8]
	mov	fp, r3
	b.n	.L0
.L8:
	mov	r1, r9
	mov	r2, r9
	lsls	r1, r1, #12
	lsls	r2, r2, #5
	str	r1, [sp, #8]
	mov	fp, r2
	b.n	.L0
	.4byte 0x0809e4ce
	.4byte 0x06001a00
.L12:
	mov	r3, r9
	lsls	r3, r3, #12
	mov	r1, r9
.L9:
	lsls	r1, r1, #5
	str	r3, [sp, #8]
.L13:
	mov	fp, r1
.L0:
	ldr	r3, [pc, #64]
	ldr	r1, [pc, #68]
	add	r3, r9
	ldr	r0, [sp, #8]
	adds	r2, r1, #0
	strh	r3, [r1, #0]
	ldr	r3, [pc, #32]
	adds	r2, #64
	strh	r3, [r2, #0]
	ldr	r3, [pc, #32]
	adds	r2, #64
	strh	r3, [r2, #0]
	ldr	r3, [pc, #28]
	adds	r2, #64
	ldr	r7, [pc, #44]
	strh	r3, [r2, #0]
	ldr	r5, [pc, #24]
	movs	r2, #1
	ldr	r4, [pc, #24]
	adds	r0, #209
	add	r7, fp
	adds	r1, #2
	mov	ip, r2
	b.n	.L1
	.4byte 0x0000f052
	.4byte 0x0000f047
	.4byte 0x0000f042
	.4byte 0x0000001f
	.4byte 0x0000f0e0
	.4byte 0xfffff0e0
	.4byte 0x0600205a
	.4byte 0x05000002
.L1:
	strh	r0, [r1, #0]
	ldrh	r6, [r7, #0]
	adds	r3, r6, #0
	adds	r2, r1, #0
	ands	r3, r5
	adds	r2, #64
	adds	r3, r3, r4
	strh	r3, [r2, #0]
	lsrs	r3, r6, #5
	ands	r3, r5
	adds	r2, #64
	adds	r3, r3, r4
	strh	r3, [r2, #0]
	lsrs	r3, r6, #10
	ands	r3, r5
	adds	r2, #64
	adds	r3, r3, r4
	strh	r3, [r2, #0]
	movs	r3, #1
	add	ip, r3
	mov	r2, ip
	adds	r0, #1
	adds	r7, #2
	adds	r1, #2
	cmp	r2, #15
	bls.n	.L1
	movs	r0, #1
	bl	Func_080030f8
	ldr	r1, [pc, #356]
	movs	r4, #31
.L30:
	ldr	r2, [r1, #0]
	movs	r3, #64
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L2
	subs	r3, #65
	add	sl, r3
	mov	r2, sl
	cmp	r2, #0
	bgt.n	.L2
	movs	r3, #3
	mov	sl, r3
.L2:
	ldr	r2, [r1, #0]
	movs	r3, #128
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L3
	movs	r2, #1
	add	sl, r2
	mov	r3, sl
	cmp	r3, #3
	ble.n	.L3
	mov	sl, r2
.L3:
	ldr	r2, [r1, #0]
	movs	r3, #32
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L4
	movs	r2, #1
	negs	r2, r2
	add	r8, r2
	mov	r3, r8
	cmp	r3, #0
	bgt.n	.L4
	movs	r2, #15
	mov	r8, r2
.L4:
	ldr	r2, [r1, #0]
	movs	r3, #16
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L5
	movs	r3, #1
	add	r8, r3
	mov	r2, r8
	cmp	r2, #15
	ble.n	.L5
	mov	r8, r3
.L5:
	ldr	r2, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L6
	movs	r3, #1
	negs	r3, r3
	add	r9, r3
	mov	r1, r9
	cmp	r1, #0
	blt.n	.L7
	b.n	.L8
.L7:
	movs	r2, #13
	mov	r9, r2
	mov	r3, r9
	mov	r1, r9
	lsls	r3, r3, #12
	b.n	.L9
.L6:
	ldr	r2, [r1, #0]
	movs	r3, #128
	lsls	r3, r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L10
	movs	r2, #1
	add	r9, r2
	mov	r3, r9
	cmp	r3, #13
	bgt.n	.L11
	b.n	.L12
.L11:
	movs	r1, #0
	mov	r9, r1
	str	r1, [sp, #8]
	b.n	.L13
.L10:
	ldr	r2, [r1, #0]
	movs	r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L14
	mov	r2, r9
	lsls	r3, r2, #4
	add	r3, r8
	movs	r1, #160
	lsls	r3, r3, #1
	lsls	r1, r1, #19
	adds	r7, r3, r1
	ldrh	r6, [r7, #0]
	mov	r1, sl
	adds	r5, r6, #0
	lsrs	r2, r6, #5
	lsrs	r3, r6, #10
	ands	r5, r4
	ands	r2, r4
	ands	r3, r4
	cmp	r1, #1
	bne.n	.L15
	cmp	r5, #30
	bhi.n	.L15
	adds	r5, #1
.L15:
	mov	r1, sl
	cmp	r1, #2
	bne.n	.L16
	cmp	r2, #30
	bhi.n	.L16
	adds	r2, #1
.L16:
	mov	r1, sl
	cmp	r1, #3
	bne.n	.L17
	cmp	r3, #30
	bhi.n	.L17
	adds	r3, #1
	b.n	.L17
.L14:
	ldr	r2, [r1, #0]
	movs	r3, #2
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L18
	mov	r2, r9
	lsls	r3, r2, #4
	add	r3, r8
	movs	r1, #160
	lsls	r3, r3, #1
	lsls	r1, r1, #19
	adds	r7, r3, r1
	ldrh	r6, [r7, #0]
	mov	r1, sl
	adds	r5, r6, #0
	lsrs	r2, r6, #5
	lsrs	r3, r6, #10
	ands	r5, r4
	ands	r2, r4
	ands	r3, r4
	cmp	r1, #1
	bne.n	.L19
	cmp	r5, #0
	beq.n	.L19
	subs	r5, #1
.L19:
	mov	r1, sl
	cmp	r1, #2
	bne.n	.L20
	cmp	r2, #0
	beq.n	.L20
	subs	r2, #1
.L20:
	mov	r1, sl
	cmp	r1, #3
	bne.n	.L17
	cmp	r3, #0
	beq.n	.L17
	subs	r3, #1
.L17:
	lsls	r3, r3, #10
	lsls	r2, r2, #5
	orrs	r3, r2
	orrs	r3, r5
	strh	r3, [r7, #0]
	b.n	.L0
.L18:
	ldr	r2, [r1, #0]
	movs	r3, #8
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L21
	mov	r2, r9
	lsls	r3, r2, #4
	add	r3, r8
	movs	r2, #160
	lsls	r3, r3, #1
	lsls	r2, r2, #19
	adds	r7, r3, r2
	ldrh	r6, [r7, #0]
	b.n	.L22
	.4byte 0x03001b04
.L28:
	cmp	r5, #0
	bne.n	.L23
	ldr	r3, [pc, #28]
	strh	r3, [r7, #0]
.L23:
	cmp	r5, #10
	bne.n	.L24
	strh	r6, [r7, #0]
.L24:
	cmp	r5, #20
	bne.n	.L25
	ldr	r3, [pc, #20]
	strh	r3, [r7, #0]
.L25:
	cmp	r5, #30
	bne.n	.L26
	strh	r6, [r7, #0]
.L26:
	adds	r5, #1
	cmp	r5, #39
	bls.n	.L27
	b.n	.L22
	.4byte 0x00007fff
	.4byte 0x00000000
.L22:
	movs	r5, #0
.L27:
	movs	r0, #1
	str	r1, [sp, #4]
	str	r4, [sp, #0]
	bl	Func_080030f8
	ldr	r3, [pc, #72]
	ldr	r2, [r3, #0]
	movs	r3, #8
	ands	r2, r3
	ldr	r1, [sp, #4]
	ldr	r4, [sp, #0]
	cmp	r2, #0
	bne.n	.L28
	strh	r6, [r7, #0]
.L21:
	ldr	r2, [r1, #0]
	movs	r3, #4
	ands	r2, r3
	cmp	r2, #0
	bne.n	.L29
	ldr	r3, [pc, #48]
	movs	r0, #1
	str	r1, [sp, #4]
	str	r4, [sp, #0]
	ldr	r3, [r3, #0]
	bl	Func_080030f8
	ldr	r1, [sp, #4]
	ldr	r4, [sp, #0]
	b.n	.L30
.L29:
	bl	Func_0800479c
	bl	Func_08004760
	add	sp, #12
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001ae8
	.4byte 0x03001e40
