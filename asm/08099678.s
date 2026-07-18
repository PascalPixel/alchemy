@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08099678
	.thumb_func
Func_08099678:
	push	{r5, r6, lr}
	ldr	r3, [pc, #160]
	movs	r2, #250
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r5, [pc, #156]
	ldr	r0, [r3, #0]
	ldr	r6, [r5, #0]
	bl	Func_0808ba1c
	movs	r2, #207
	lsls	r2, r2, #1
	adds	r3, r6, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	subs	r5, #76
	ldr	r5, [r5, #0]
	cmp	r3, #3
	bne.n	.L0
	ldr	r3, [r0, #8]
	cmp	r3, #0
	bge.n	.L1
	ldr	r2, [pc, #124]
	adds	r3, r3, r2
.L1:
	ldr	r0, [r0, #16]
	asrs	r2, r3, #21
	movs	r1, #31
	ands	r2, r1
	cmp	r0, #0
	bge.n	.L2
	ldr	r3, [pc, #108]
	adds	r0, r0, r3
.L2:
	asrs	r3, r0, #21
	ands	r3, r1
	lsls	r3, r3, #5
	adds	r3, r2, r3
	ldr	r2, [pc, #100]
	lsls	r3, r3, #2
	adds	r5, r3, r2
	b.n	.L3
.L0:
	adds	r2, r0, #0
	adds	r2, #34
	ldrb	r3, [r2, #0]
	cmp	r3, #2
	bhi.n	.L4
	adds	r2, r3, #0
	lsls	r3, r2, #1
	adds	r3, r3, r2
	movs	r2, #152
	lsls	r3, r3, #4
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r5, [r5, r3]
	b.n	.L5
.L4:
	ldr	r5, [pc, #68]
.L5:
	ldr	r3, [r0, #8]
	cmp	r3, #0
	bge.n	.L6
	ldr	r2, [pc, #64]
	adds	r3, r3, r2
.L6:
	ldr	r0, [r0, #16]
	asrs	r2, r3, #20
	cmp	r0, #0
	bge.n	.L7
	ldr	r3, [pc, #52]
	adds	r0, r0, r3
.L7:
	asrs	r3, r0, #20
	lsls	r3, r3, #7
	adds	r3, r2, r3
	lsls	r3, r3, #2
	adds	r5, r5, r3
.L3:
	ldrb	r3, [r5, #2]
	cmp	r3, #251
	beq.n	.L8
	movs	r3, #191
	lsls	r3, r3, #1
	adds	r2, r6, r3
	ldr	r3, [pc, #32]
	strh	r3, [r2, #0]
.L8:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x02000240
	.4byte 0x03001ebc
	.4byte 0x001fffff
	.4byte 0x02020000
	.4byte 0x02010000
	.4byte 0x000fffff
	.4byte 0x00002092
