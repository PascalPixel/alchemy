@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080197c4
	.thumb_func
Func_080197c4:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #132]
	ldr	r3, [r3, #0]
	movs	r5, #196
	movs	r7, #160
	mov	r8, r3
	lsls	r5, r5, #3
	lsls	r7, r7, #3
	add	r5, r8
	add	r7, r8
	movs	r6, #0
.L1:
	ldr	r0, [r5, #0]
	cmp	r0, #0
	beq.n	.L0
	ldrh	r3, [r0, #22]
	cmp	r3, #0
	beq.n	.L0
	movs	r1, #0
	bl	Func_08016418
.L0:
	adds	r6, #1
	adds	r5, #40
	cmp	r6, #3
	bne.n	.L1
.L6:
	movs	r5, #196
	lsls	r5, r5, #3
	movs	r1, #1
	add	r5, r8
	movs	r6, #0
.L4:
	ldr	r2, [r5, #0]
	cmp	r2, #0
	beq.n	.L2
	ldr	r3, [r2, #24]
	cmp	r3, #0
	bne.n	.L3
	ldrh	r3, [r2, #22]
	cmp	r3, #0
	bne.n	.L3
	str	r3, [r5, #0]
	b.n	.L2
.L3:
	movs	r1, #0
.L2:
	adds	r6, #1
	adds	r5, #40
	cmp	r6, #3
	bne.n	.L4
	movs	r6, #0
	cmp	r1, #0
	bne.n	.L5
	movs	r0, #1
	bl	Func_080030f8
	b.n	.L6
.L8:
	ldrh	r3, [r7, #22]
	cmp	r3, #0
	beq.n	.L7
	adds	r0, r7, #0
	movs	r1, #0
	bl	Func_08016418
.L7:
	adds	r7, #36
	adds	r6, #1
.L5:
	cmp	r6, #8
	bne.n	.L8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e8c
