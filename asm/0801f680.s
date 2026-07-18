@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801f680
	.thumb_func
Func_0801f680:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r7, r1, #0
	movs	r1, #225
	lsls	r1, r1, #4
	sub	sp, #64
	bl	Func_080022f4
	ldr	r3, [pc, #108]
	adds	r6, r0, #0
	cmp	r6, r3
	bls.n	.L0
	adds	r6, r3, #0
.L0:
	adds	r0, r6, #0
	movs	r1, #60
	bl	Func_080022f4
	movs	r1, #60
	adds	r5, r0, #0
	adds	r0, r6, #0
	bl	Func_08002304
	mov	r8, sp
	adds	r1, r5, #0
	adds	r6, r0, #0
	movs	r2, #3
	mov	r0, r8
	bl	Func_08017dd4
	ldrb	r3, [r0, #0]
	strb	r3, [r7, #0]
	adds	r0, #1
	ldrb	r3, [r0, #0]
	adds	r5, r7, #1
	strb	r3, [r5, #0]
	ldrb	r3, [r0, #1]
	adds	r5, #1
	strb	r3, [r5, #0]
	adds	r6, #100
	movs	r3, #58
	adds	r5, #1
	strb	r3, [r5, #0]
	mov	r0, r8
	adds	r1, r6, #0
	movs	r2, #2
	bl	Func_08017dd4
	ldrb	r3, [r0, #0]
	adds	r5, #1
	strb	r3, [r5, #0]
	ldrb	r3, [r0, #1]
	adds	r5, #1
	strb	r3, [r5, #0]
	movs	r3, #0
	adds	r0, r7, #0
	strb	r3, [r5, #1]
	add	sp, #64
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x0000ea5f
