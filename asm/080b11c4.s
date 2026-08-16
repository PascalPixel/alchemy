@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b11c4
	.thumb_func
Func_080b11c4:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #128]
	mov	fp, r1
	mov	r9, r2
	ldr	r7, [r3, #0]
	cmp	r0, #0
	beq.n	.L0
	ldr	r1, [pc, #120]
	adds	r3, r7, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	movs	r6, #0
	cmp	r6, r3
	bge.n	.L0
	movs	r3, #219
	movs	r1, #138
	adds	r2, r7, #2
	lsls	r3, r3, #2
	lsls	r1, r1, #1
	mov	sl, r2
	mov	r8, r3
	adds	r5, r7, r1
.L4:
	cmp	r6, fp
	bne.n	.L1
	ldr	r0, [r5, #0]
	movs	r1, #30
	bl	Func_08009020
	b.n	.L2
.L1:
	ldr	r0, [r5, #0]
	movs	r1, #1
	bl	Func_08009020
.L2:
	movs	r3, #128
	lsls	r3, r3, #9
	str	r3, [r5, #64]
	mov	r1, r8
	mov	r2, sl
	ldrsh	r0, [r2, r1]
	mov	r1, r9
	bl	Func_08077220
	cmp	r0, #0
	bne.n	.L3
	ldr	r3, [pc, #48]
	str	r3, [r5, #64]
.L3:
	ldr	r1, [pc, #40]
	adds	r3, r7, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	movs	r2, #2
	adds	r6, #1
	add	r8, r2
	adds	r5, #4
	cmp	r6, r3
	blt.n	.L4
.L0:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f2c
	.4byte 0x000003a7
	.4byte 0x0000cccc
