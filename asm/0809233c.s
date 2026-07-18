@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0809233c
	.thumb_func
Func_0809233c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r6, r0, #0
	adds	r7, r1, #0
	mov	r8, r2
	mov	sl, r3
	bl	Func_0808ba1c
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L0
	ldr	r2, [pc, #92]
	adds	r0, r6, #0
	ldr	r1, [pc, #92]
	bl	Func_08092064
	ldr	r3, [pc, #88]
	movs	r2, #250
	lsls	r2, r2, #1
	adds	r3, r3, r2
	ldr	r0, [r3, #0]
	bl	Func_08092054
	cmp	r0, #0
	beq.n	.L1
	ldr	r1, [r0, #8]
	ldr	r2, [r0, #16]
	adds	r0, r6, #0
	bl	Func_080923e4
.L1:
	adds	r2, r5, #0
	movs	r3, #0
	adds	r2, #91
	strb	r3, [r2, #0]
	adds	r0, r5, #0
	movs	r1, #2
	bl	Func_08009080
	mov	r2, r8
	adds	r0, r6, #0
	adds	r1, r7, #0
	bl	Func_0809228c
	ldr	r1, [pc, #40]
	adds	r0, r5, #0
	bl	Func_08009098
	adds	r3, r5, #0
	adds	r3, #100
	mov	r2, sl
	strh	r2, [r3, #0]
.L0:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00004ccc
	.4byte 0x00009999
	.4byte 0x02000240
	.4byte 0x0809fbcc
