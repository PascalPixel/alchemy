@ 回転行列の生成。r0[0..8] の3角から sin, cos を引き、9 個の積を IwramMulQ16ReturnIp で
@ 求めて 12 語 (末尾3語は 0) の行列を組み、IwramTransformMatrix (0x030002c0) に読み込ませる。
@ mov ip,pc / bx でIWRAM核へ飛ぶ呼出し規約は C では表現不能なため、アセンブリとして保持する。
.syntax unified
	.thumb
	.global Func_08004ab0
	.thumb_func
Func_08004ab0:
	push	{r5, r6, lr}
	mov	r6, fp
	mov	r5, sl
	push	{r5, r6}
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6}
	adds	r5, r0, #0
	ldr	r0, [r5, #0]
	sub	sp, #48
@ 三軸それぞれの sin と cos。
	bl	Func_08002322
	mov	sl, r0
	ldr	r0, [r5, #0]
	bl	Func_0800231c
	mov	r9, r0
	ldr	r0, [r5, #4]
	bl	Func_08002322
	mov	r8, r0
	ldr	r0, [r5, #4]
	bl	Func_0800231c
	mov	fp, r0
	ldr	r0, [r5, #8]
	bl	Func_08002322
	adds	r6, r0, #0
	ldr	r0, [r5, #8]
	bl	Func_0800231c
	mov	lr, r0
	ldr	r3, [pc, #216]
	mov	r0, fp
	mov	r1, lr
@ 以降の各 ip 呼出しは IwramMulQ16ReturnIp による Q16 積。
	mov	ip, pc
	bx	r3
	mov	r5, sp
	str	r0, [r5, #0]
	adds	r1, r6, #0
	mov	r0, fp
	mov	ip, pc
	bx	r3
	mov	r1, r8
	negs	r2, r1
	str	r0, [r5, #4]
	str	r2, [r5, #8]
	mov	r0, sl
	movs	r0, r0
	mov	ip, pc
	bx	r3
	mov	r1, lr
	movs	r0, r0
	mov	ip, pc
	bx	r3
	adds	r4, r0, #0
	adds	r1, r6, #0
	mov	r0, r9
	movs	r0, r0
	mov	ip, pc
	bx	r3
	subs	r4, r4, r0
	str	r4, [r5, #12]
	mov	r0, sl
	mov	r1, r8
	mov	ip, pc
	bx	r3
	adds	r1, r6, #0
	movs	r0, r0
	mov	ip, pc
	bx	r3
	adds	r4, r0, #0
	mov	r1, lr
	mov	r0, r9
	movs	r0, r0
	mov	ip, pc
	bx	r3
	adds	r4, r4, r0
	str	r4, [r5, #16]
	mov	r0, sl
	mov	r1, fp
	mov	ip, pc
	bx	r3
	str	r0, [r5, #20]
	mov	r1, r8
	mov	r0, r9
	movs	r0, r0
	mov	ip, pc
	bx	r3
	mov	r1, lr
	movs	r0, r0
	mov	ip, pc
	bx	r3
	adds	r4, r0, #0
	adds	r1, r6, #0
	mov	r0, sl
	movs	r0, r0
	mov	ip, pc
	bx	r3
	adds	r4, r4, r0
	str	r4, [r5, #24]
	mov	r0, r9
	mov	r1, r8
	mov	ip, pc
	bx	r3
	adds	r1, r6, #0
	movs	r0, r0
	mov	ip, pc
	bx	r3
	adds	r4, r0, #0
	mov	r1, lr
	mov	r0, sl
	movs	r0, r0
	mov	ip, pc
	bx	r3
	subs	r4, r4, r0
	str	r4, [r5, #28]
	mov	r0, r9
	mov	r1, fp
	mov	ip, pc
	bx	r3
	movs	r3, #0
	str	r0, [r5, #32]
	str	r3, [r5, #36]
	str	r3, [r5, #40]
	str	r3, [r5, #44]
	adds	r0, r5, #0
	ldr	r3, [pc, #28]
@ 組み上げた行列を IwramTransformMatrix へ経由呼出しで渡す。
	bl	Func_080072f0
	add	sp, #48
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r3}
	mov	fp, r3
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03000118
	.4byte 0x030002c0
