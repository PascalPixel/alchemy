@ 検出済み関数の再構築サム逆アセンブル。ROM作成時の
@ コンパイラは自由配布ツールで再現不能なため、アセンブリなしのC一致は
@ 未達。build_asm.pyでROMとの一致を確認する。
.syntax unified
	.thumb
	.global Func_0801bcd4
	.thumb_func
Func_0801bcd4:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	mov	r8, r3
	movs	r3, #1
	sub	sp, #12
	adds	r4, r2, #0
	negs	r3, r3
	adds	r7, r0, #0
	adds	r5, r1, #0
	str	r2, [sp, #8]
	adds	r6, r4, #0
	cmp	r4, r3
	bne.n	.L0
	bl	Func_08004080
	adds	r4, r0, #0
	str	r0, [sp, #8]
	adds	r0, r6, #0
	cmp	r4, #96
	beq.n	.L1
.L0:
	subs	r0, r7, #1
	cmp	r0, #8
	bhi.n	.L2
	ldr	r2, [pc, #140]
	lsls	r3, r0, #2
	ldr	r3, [r3, r2]
	mov	pc, r3
	pop	{r4, r5, pc}
	lsrs	r1, r0, #32
	pop	{r1, r6, pc}
	lsrs	r1, r0, #32
	pop	{r2, r7, pc}
	lsrs	r1, r0, #32
	pop	{r1, r3, r4, r6, pc}
	lsrs	r1, r0, #32
	pop	{r2, r7, pc}
	lsrs	r1, r0, #32
	pop	{r4, r5, pc}
	lsrs	r1, r0, #32
	pop	{r1, r2, r3, r6, pc}
	lsrs	r1, r0, #32
	pop	{r2, r3, r5, r6, pc}
	lsrs	r1, r0, #32
	pop	{r3, r4, r5, r6, pc}
	lsrs	r1, r0, #32
	movs	r1, #1
	str	r1, [sp, #0]
	add	r2, sp, #8
	add	r3, sp, #4
	adds	r0, r5, #0
	mov	r1, r8
	bl	Func_08019ee4
	b.n	.L3
	adds	r2, r4, #0
	adds	r0, r5, #0
	movs	r1, #58
	bl	Func_0801a2a4
	b.n	.L3
	adds	r2, r4, #0
	adds	r0, r5, #0
	movs	r1, #42
	bl	Func_0801a2a4
	b.n	.L3
	movs	r1, #1
	str	r1, [sp, #0]
	add	r2, sp, #8
	add	r3, sp, #4
	adds	r0, r5, #0
	mov	r1, r8
	bl	Func_0801a3d0
	b.n	.L3
	adds	r2, r4, #0
	adds	r0, r5, #0
	movs	r1, #0
	bl	Func_0801a2ec
	b.n	.L3
	adds	r2, r4, #0
	adds	r0, r5, #0
	movs	r1, #0
	bl	Func_0801a32c
.L3:
	ldr	r4, [sp, #8]
.L2:
	adds	r0, r4, #0
.L1:
	add	sp, #12
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x0801bd0c
