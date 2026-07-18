@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0809c314
	.thumb_func
Func_0809c314:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #116]
	movs	r2, #250
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl	Func_08092054
	ldr	r3, [pc, #108]
	movs	r2, #240
	ldr	r3, [r3, #0]
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	ldr	r2, [pc, #100]
	ldr	r3, [r0, #8]
	adds	r2, r2, r3
	mov	sl, r2
	movs	r2, #160
	lsls	r2, r2, #17
	adds	r2, r2, r3
	mov	r8, r2
	ldr	r3, [r0, #16]
	ldr	r2, [pc, #84]
	adds	r7, r3, r2
	movs	r2, #200
	lsls	r2, r2, #17
	adds	r6, r3, r2
	movs	r5, #8
.L4:
	adds	r0, r5, #0
	bl	Func_0808ba1c
	cmp	r0, #0
	beq.n	.L0
	ldr	r3, [r0, #8]
	ldr	r2, [r0, #16]
	cmp	r3, sl
	blt.n	.L1
	cmp	r3, r8
	bgt.n	.L1
	cmp	r2, r7
	blt.n	.L1
	cmp	r2, r6
	ble.n	.L2
.L1:
	adds	r2, r0, #0
	adds	r2, #84
	movs	r3, #0
	b.n	.L3
.L2:
	adds	r2, r0, #0
	adds	r2, #84
	movs	r3, #1
.L3:
	strb	r3, [r2, #0]
.L0:
	adds	r5, #1
	cmp	r5, #65
	bls.n	.L4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x02000240
	.4byte 0x03001ebc
	.4byte 0xfec00000
	.4byte 0xfda80000
