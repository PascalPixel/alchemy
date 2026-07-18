@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08096960
	.thumb_func
Func_08096960:
	push	{r5, r6, lr}
	ldr	r3, [pc, #308]
	ldr	r6, [r3, #0]
	movs	r2, #30
	ldrsh	r1, [r6, r2]
	subs	r3, #116
	adds	r2, r6, #0
	ldr	r0, [r3, #0]
	adds	r2, #32
	movs	r3, #26
	ldrsh	r5, [r6, r3]
	subs	r1, #1
	movs	r3, #0
	strb	r3, [r2, #0]
	cmp	r1, #15
	bls.n	.L0
	b.n	.L1
.L0:
	ldr	r2, [pc, #280]
	lsls	r3, r1, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x08096a02
	.4byte 0x080969cc
	.4byte 0x08096a64
	.4byte 0x08096a1a
	.4byte 0x08096a22
	.4byte 0x08096a2a
	.4byte 0x08096a0a
	.4byte 0x08096a7c
	.4byte 0x08096a3a
	.4byte 0x08096a82
	.4byte 0x08096a12
	.4byte 0x08096a32
	.4byte 0x08096a74
	.4byte 0x08096a6c
	.4byte 0x08096a88
	.4byte 0x08096a8e
	ldr	r1, [pc, #208]
	adds	r3, r0, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L2
	bl	Func_080984c0
.L2:
	ldr	r1, [pc, #196]
	ldr	r3, [pc, #200]
	adds	r3, r3, r1
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	movs	r1, #26
	ldrsh	r3, [r6, r1]
	cmp	r2, r3
	beq.n	.L3
	ldr	r3, [r6, #20]
	movs	r2, #1
	adds	r3, #91
	strb	r2, [r3, #0]
.L3:
	movs	r2, #24
	ldrsh	r0, [r6, r2]
	adds	r1, r5, #0
	bl	Func_08097540
	b.n	.L1
	adds	r0, r5, #0
	bl	Func_08097c3c
	b.n	.L1
	adds	r0, r5, #0
	bl	Func_08098848
	b.n	.L1
	adds	r0, r5, #0
	bl	Func_08099da4
	b.n	.L1
	adds	r0, r5, #0
	bl	Func_08098ccc
	b.n	.L1
	adds	r0, r5, #0
	bl	Func_080999e4
	b.n	.L1
	adds	r0, r5, #0
	bl	Func_08099128
	b.n	.L1
	adds	r0, r5, #0
	bl	Func_080985a8
	b.n	.L1
	ldr	r3, [pc, #108]
	ldr	r1, [pc, #100]
	adds	r6, r3, r1
	movs	r2, #0
	ldrsh	r0, [r6, r2]
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	beq.n	.L4
	bl	Func_0809ade8
	ldr	r3, [pc, #88]
	strh	r3, [r6, #0]
.L4:
	adds	r0, r5, #0
	bl	Func_0809ad90
	strh	r5, [r6, #0]
	adds	r0, r5, #0
	bl	Func_0809ab98
	b.n	.L1
	adds	r0, r5, #0
	bl	Func_080994c0
	b.n	.L1
	adds	r0, r5, #0
	bl	Func_0809a8b8
	b.n	.L1
	adds	r0, r5, #0
	bl	Func_0809ae58
	b.n	.L1
	bl	Func_080983a0
	b.n	.L1
	bl	Func_08099838
	b.n	.L1
	bl	Func_0809b208
	b.n	.L1
	bl	Func_0809b698
.L1:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x03001f30
	.4byte 0x0809698c
	.4byte 0x00000cb8
	.4byte 0x0000024a
	.4byte 0x02000240
	.4byte 0x0000ffff
