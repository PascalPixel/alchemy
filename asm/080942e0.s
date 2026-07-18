@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080942e0
	.thumb_func
Func_080942e0:
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	mov	r8, r0
	ldr	r3, [pc, #96]
	movs	r0, #250
	lsls	r0, r0, #1
	adds	r3, r3, r0
	ldr	r0, [r3, #0]
	bl	Func_0808ba1c
	adds	r5, r0, #0
	ldr	r6, [r5, #80]
	movs	r1, #27
	adds	r0, r6, #0
	bl	Func_08009048
	adds	r6, #38
	movs	r1, #0
	strb	r1, [r6, #0]
	movs	r3, #15
	strb	r3, [r0, #5]
	ldr	r2, [pc, #64]
	ldr	r3, [r5, #8]
	movs	r0, #128
	ands	r3, r2
	lsls	r0, r0, #12
	adds	r3, r3, r0
	str	r3, [r5, #8]
	ldr	r3, [r5, #16]
	ands	r3, r2
	movs	r2, #128
	lsls	r2, r2, #13
	adds	r3, r3, r2
	str	r3, [r5, #16]
	movs	r3, #128
	lsls	r3, r3, #24
	str	r1, [r5, #36]
	str	r1, [r5, #44]
	adds	r0, r5, #0
	str	r3, [r5, #56]
	str	r3, [r5, #64]
	mov	r1, r8
	bl	Func_08009080
	movs	r0, #18
	bl	Func_080030f8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0xfff00000
