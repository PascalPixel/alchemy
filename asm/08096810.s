@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08096810
	.thumb_func
Func_08096810:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #304]
	ldr	r5, [r3, #0]
	movs	r2, #30
	ldrsh	r6, [r5, r2]
	subs	r3, #116
	ldr	r1, [r3, #0]
	movs	r3, #26
	ldrsh	r7, [r5, r3]
	subs	r3, r6, #1
	cmp	r3, #15
	bls.n	.L0
	b.n	.L1
.L0:
	ldr	r2, [pc, #284]
	lsls	r3, r3, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	movs	r0, r0
	.4byte 0x08096878
	.4byte 0x0809690c
	.4byte 0x080968a2
	.4byte 0x0809688a
	.4byte 0x08096890
	.4byte 0x0809689c
	.4byte 0x0809687e
	.4byte 0x08096928
	.4byte 0x080968b4
	.4byte 0x0809692e
	.4byte 0x08096884
	.4byte 0x080968a8
	.4byte 0x080968ae
	.4byte 0x08096896
	.4byte 0x08096934
	.4byte 0x0809693a
	bl	Func_0809802c
	b.n	.L1
	bl	Func_08098954
	b.n	.L1
	bl	Func_0809a294
	b.n	.L1
	bl	Func_08098cd8
	b.n	.L1
	bl	Func_080999f0
	b.n	.L1
	bl	Func_0809a8c4
	b.n	.L1
	bl	Func_08099160
	b.n	.L1
	bl	Func_080994d0
	b.n	.L1
	bl	Func_080985fc
	b.n	.L1
	bl	Func_0809ae64
	b.n	.L1
	ldr	r2, [pc, #152]
	ldr	r5, [pc, #156]
	adds	r7, r5, r2
	movs	r3, #0
	ldrsh	r0, [r7, r3]
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	beq.n	.L2
	bl	Func_0809ade8
	ldr	r3, [pc, #140]
	strh	r3, [r7, #0]
.L2:
	movs	r3, #250
	lsls	r3, r3, #1
	adds	r3, r3, r5
	ldr	r0, [r3, #0]
	adds	r1, r6, #0
	mov	r8, r3
	bl	Func_0808df1c
	bl	Func_0809ae3c
	adds	r5, r0, #0
	bl	Func_0808d5a4
	cmp	r0, #0
	beq.n	.L3
	mov	r2, r8
	ldr	r0, [r2, #0]
	adds	r1, r5, #0
	bl	Func_080970f8
	adds	r0, r5, #0
	bl	Func_0809ab98
	adds	r0, r5, #0
	bl	Func_0809ad90
	strh	r5, [r7, #0]
	b.n	.L1
.L3:
	bl	Func_0809abb4
	b.n	.L1
	ldr	r2, [pc, #76]
	adds	r3, r1, r2
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #0
	beq.n	.L4
	bl	Func_080984c0
.L4:
	movs	r3, #24
	ldrsh	r0, [r5, r3]
	adds	r1, r7, #0
	bl	Func_08097540
	b.n	.L1
	bl	Func_080983a0
	b.n	.L1
	bl	Func_08099838
	b.n	.L1
	bl	Func_0809b208
	b.n	.L1
	bl	Func_0809b698
.L1:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f30
	.4byte 0x08096838
	.4byte 0x0000024a
	.4byte 0x02000240
	.4byte 0x0000ffff
	.4byte 0x00000cb8
