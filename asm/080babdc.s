@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080babdc
	.thumb_func
Func_080babdc:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r7, r0, #0
	sub	sp, #4
	bl	Func_08077008
	adds	r0, r7, #0
	bl	Func_080b7dd0
	movs	r1, #5
	ldr	r0, [r0, #0]
	bl	Func_08009080
	movs	r3, #1
	mov	r6, sp
	mov	r8, r3
.L0:
	movs	r3, #255
	adds	r0, r6, #0
	strh	r3, [r6, #2]
	strh	r7, [r6, #0]
	bl	Func_080152b8
	adds	r0, r7, #0
	bl	Func_080b7dd0
	movs	r1, #7
	ldr	r0, [r0, #0]
	bl	Func_080ba918
	movs	r0, #2
	bl	Func_080030f8
	adds	r0, r6, #0
	strh	r7, [r6, #0]
	bl	Func_080152b8
	adds	r0, r7, #0
	bl	Func_080b7dd0
	adds	r5, r0, #0
	adds	r0, r7, #0
	bl	Func_080b6cd0
	adds	r1, r0, #0
	ldr	r0, [r5, #0]
	bl	Func_080ba918
	movs	r0, #2
	bl	Func_080030f8
	movs	r3, #1
	negs	r3, r3
	add	r8, r3
	mov	r3, r8
	cmp	r3, #0
	bge.n	.L0
	ldr	r3, [pc, #24]
	ldr	r3, [r3, #0]
	adds	r3, #65
	ldrb	r0, [r3, #0]
	bl	Func_08015130
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e74
