@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080120dc
	.thumb_func
Func_080120dc:
	push	{r5, r6, r7, lr}
	movs	r3, #10
	ldrsh	r6, [r1, r3]
	ldr	r3, [pc, #160]
	movs	r2, #2
	ldrsh	r5, [r1, r2]
	ldr	r1, [r3, #0]
	adds	r7, r0, #0
	movs	r0, #0
	cmp	r1, #0
	beq.n	.L0
	adds	r2, r7, #0
	adds	r2, #34
	ldrb	r3, [r2, #0]
	cmp	r3, #2
	bhi.n	.L1
	adds	r2, r3, #0
	lsls	r3, r2, #1
	adds	r3, r3, r2
	movs	r2, #152
	lsls	r2, r2, #1
	lsls	r3, r3, #4
	adds	r3, r3, r2
	ldr	r2, [r1, r3]
	b.n	.L2
.L1:
	ldr	r2, [pc, #120]
.L2:
	adds	r3, r5, #0
	cmp	r5, #0
	bge.n	.L3
	adds	r3, #15
.L3:
	asrs	r1, r3, #4
	adds	r3, r6, #0
	cmp	r6, #0
	bge.n	.L4
	adds	r3, #15
.L4:
	asrs	r3, r3, #4
	lsls	r3, r3, #7
	adds	r3, r1, r3
	lsls	r3, r3, #2
	adds	r2, r2, r3
	ldrb	r3, [r2, #2]
	movs	r0, #2
	cmp	r3, #255
	beq.n	.L0
	ldrb	r1, [r2, #3]
	ldr	r3, [pc, #84]
	lsls	r1, r1, #2
	adds	r0, r1, r3
	movs	r2, #15
	ldrb	r0, [r0, #0]
	adds	r3, r2, #0
	ands	r3, r0
	lsls	r3, r3, #2
	mov	ip, r3
	ldr	r4, [pc, #68]
	ldr	r3, [pc, #72]
	ands	r5, r2
	ands	r6, r2
	mov	r2, ip
	adds	r0, r1, r3
	ldr	r3, [r4, r2]
	adds	r1, r5, #0
	adds	r2, r6, #0
	bl	Func_080072f0
	ldr	r3, [r7, #20]
	subs	r0, r0, r3
	movs	r3, #128
	lsls	r3, r3, #12
	cmp	r0, r3
	ble.n	.L5
	movs	r0, #1
	b.n	.L0
.L5:
	ldr	r2, [pc, #40]
	cmp	r0, r2
	bge.n	.L6
	movs	r0, #1
	negs	r0, r0
	b.n	.L0
.L6:
	movs	r0, #0
.L0:
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001e70
	.4byte 0x02010000
	.4byte 0x0202c000
	.4byte 0x080134fc
	.4byte 0x0202c001
	.4byte 0xfff40000
