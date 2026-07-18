@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08019fcc
	.thumb_func
Func_08019fcc:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	adds	r7, r1, #0
	movs	r1, #0
	mov	sl, r1
	movs	r1, #193
	adds	r6, r0, #0
	lsls	r1, r1, #3
	movs	r0, #17
	mov	r8, r2
	mov	r9, r3
	bl	Func_080048b0
	adds	r5, r0, #0
	bl	Func_08019ebc
	cmp	r6, r0
	bcc.n	.L0
	movs	r6, #0
.L0:
	cmp	r7, #0
	beq.n	.L1
	ldr	r3, [pc, #124]
	adds	r2, r5, r3
	ldr	r3, [pc, #124]
	movs	r1, #192
	ldr	r3, [r3, #8]
	lsls	r1, r1, #3
	str	r3, [r2, #0]
	movs	r2, #2
	adds	r3, r5, r1
	adds	r1, #2
	strh	r2, [r3, #0]
	adds	r3, r5, r1
	strh	r2, [r3, #0]
	adds	r0, r5, #0
	movs	r1, #0
	bl	Func_0801a5a4
	movs	r2, #1
	mov	sl, r2
.L1:
	ldr	r3, [pc, #88]
	ldr	r2, [pc, #92]
	adds	r1, r5, r3
	lsls	r3, r6, #2
	ldr	r3, [r2, r3]
	str	r3, [r1, #0]
	movs	r1, #192
	lsls	r1, r1, #3
	movs	r2, #2
	adds	r3, r5, r1
	adds	r1, #2
	strh	r2, [r3, #0]
	adds	r3, r5, r1
	strh	r2, [r3, #0]
	adds	r0, r5, #0
	mov	r1, sl
	bl	Func_0801a5a4
	ldr	r2, [sp, #28]
	cmp	r2, #0
	bne.n	.L2
	bl	Func_08004080
	mov	r3, r8
	str	r0, [r3, #0]
.L2:
	movs	r3, #128
	mov	r1, r8
	lsls	r3, r3, #3
	ldr	r0, [r1, #0]
	adds	r2, r5, r3
	movs	r1, #128
	bl	Func_08003fa4
	mov	r1, r9
	str	r0, [r1, #0]
	movs	r0, #17
	bl	Func_08002dd8
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x00000604
	.4byte 0x08029a10
	.4byte 0x08029ee4
