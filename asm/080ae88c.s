@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080ae88c
	.thumb_func
Func_080ae88c:
	push	{r5, r6, lr}
	ldr	r3, [pc, #60]
	ldr	r5, [r3, #0]
	bl	Func_08004080
	ldr	r2, [pc, #56]
	movs	r6, #1
	adds	r3, r5, r2
	negs	r6, r6
	strh	r0, [r3, #0]
	cmp	r0, r6
	beq.n	.L0
	ldr	r2, [pc, #44]
	movs	r1, #128
	bl	Func_08003fa4
.L0:
	bl	Func_08004080
	movs	r2, #229
	lsls	r2, r2, #2
	adds	r3, r5, r2
	strh	r0, [r3, #0]
	cmp	r0, r6
	beq.n	.L1
	ldr	r2, [pc, #24]
	movs	r1, #128
	bl	Func_08003fa4
.L1:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f2c
	.4byte 0x00000392
	.4byte 0x080aed4c
	.4byte 0x080aedcc
