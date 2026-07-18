@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080286a0
	.thumb_func
Func_080286a0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #160]
	ldr	r6, [r3, #0]
	adds	r5, r6, #0
	movs	r2, #1
	movs	r3, #12
	adds	r5, #140
	adds	r7, r1, #0
	mov	r9, r2
	mov	sl, r3
	strh	r0, [r5, #0]
	cmp	r7, r0
	bge.n	.L0
	subs	r2, #2
	mov	r9, r2
.L0:
	mov	r8, r0
	movs	r3, #146
	adds	r3, r3, r6
	mov	fp, r3
	b.n	.L1
.L5:
	ldrh	r3, [r5, #0]
	add	r3, r9
	strh	r3, [r5, #0]
	movs	r0, #111
	bl	Func_080f9010
	movs	r2, #0
	mov	sl, r2
	add	r8, r9
.L1:
	ldr	r0, [r6, #120]
	bl	Func_08016478
	mov	r2, fp
	movs	r3, #0
	ldrsh	r0, [r2, r3]
	cmp	r0, #0
	beq.n	.L2
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	adds	r0, r0, r3
	b.n	.L3
.L2:
	movs	r2, #0
	ldrsh	r3, [r5, r2]
	adds	r3, #132
	ldrb	r2, [r6, r3]
	ldr	r3, [pc, #76]
	adds	r0, r2, r3
.L3:
	movs	r3, #0
	ldr	r1, [r6, #120]
	movs	r2, #0
	bl	Func_0801e7c0
	movs	r3, #0
	ldrsh	r1, [r5, r3]
	subs	r3, r1, r7
	ldr	r0, [pc, #60]
	adds	r2, r3, #0
	cmp	r3, #0
	bge.n	.L4
	subs	r2, r7, r1
.L4:
	ldrb	r0, [r0, r2]
	add	r0, sl
	bl	Func_080030f8
	cmp	r8, r7
	bne.n	.L5
	movs	r0, #48
	bl	Func_080030f8
	movs	r0, #112
	bl	Func_080f9010
	adds	r0, r7, #0
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001f38
	.4byte 0x0000001f
	.4byte 0x080373ef
