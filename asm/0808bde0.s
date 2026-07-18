@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0808bde0
	.thumb_func
Func_0808bde0:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #20
	str	r0, [sp, #16]
	str	r1, [sp, #12]
	str	r2, [sp, #8]
	ldr	r3, [pc, #192]
	ldr	r0, [r3, #44]
	bl	Func_080072e4
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L0
	ldr	r3, [pc, #180]
	ldr	r3, [r3, #0]
	str	r3, [sp, #0]
	movs	r2, #1
	movs	r1, #0
	ldrsh	r6, [r5, r1]
	negs	r2, r2
	cmp	r6, r2
	beq.n	.L0
.L2:
	movs	r1, #2
	ldrsh	r3, [r5, r1]
	mov	r8, r3
	movs	r3, #4
	ldrsh	r2, [r5, r3]
	mov	fp, r2
	movs	r2, #6
	ldrsh	r1, [r5, r2]
	mov	sl, r1
	movs	r2, #10
	ldrsh	r1, [r5, r2]
	mov	r9, r1
	movs	r2, #14
	ldrsh	r1, [r5, r2]
	movs	r3, #12
	ldrsh	r0, [r5, r3]
	movs	r3, #8
	ldrsh	r7, [r5, r3]
	str	r1, [sp, #4]
	bl	Func_0808d428
	cmp	r0, #0
	beq.n	.L1
	mov	r1, r8
	ldr	r2, [sp, #12]
	lsls	r3, r1, #16
	cmp	r2, r3
	blt.n	.L1
	lsls	r3, r7, #16
	cmp	r2, r3
	bge.n	.L1
	ldr	r1, [sp, #16]
	lsls	r3, r6, #16
	cmp	r1, r3
	blt.n	.L1
	mov	r2, sl
	lsls	r3, r2, #16
	cmp	r1, r3
	bge.n	.L1
	mov	r1, fp
	ldr	r2, [sp, #8]
	lsls	r3, r1, #16
	cmp	r2, r3
	blt.n	.L1
	mov	r1, r9
	lsls	r3, r1, #16
	cmp	r2, r3
	bge.n	.L1
	ldr	r2, [sp, #0]
	movs	r1, #184
	lsls	r1, r1, #1
	adds	r3, r2, r1
	add	r2, sp, #4
	ldrh	r2, [r2, #0]
	movs	r0, #123
	strh	r2, [r3, #0]
	bl	Func_080f9010
	bl	Func_08091660
	b.n	.L0
.L1:
	ldr	r3, [pc, #40]
	ldr	r3, [r3, #0]
	str	r3, [sp, #0]
	adds	r5, #16
	movs	r1, #1
	movs	r3, #0
	ldrsh	r6, [r5, r3]
	negs	r1, r1
	cmp	r6, r1
	bne.n	.L2
.L0:
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02008000
	.4byte 0x03001ebc
