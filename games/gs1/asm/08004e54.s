@ 回転行列の生成、積の多い版 (スタック 56 バイト)。三軸の sin, cos から Q16 積を
@ 組み、IwramTransformMatrix (0x030002c0) に読み込ませる。
@ mov ip,pc / bx でIWRAM核へ飛ぶ呼出し規約は C では表現不能なため、アセンブリとして保持する。
.syntax unified
	.thumb
	.global Func_08004e54
	.thumb_func
Func_08004e54:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #56
	str	r1, [sp, #4]
	str	r2, [sp, #0]
	adds	r5, r0, #0
	ldr	r0, [r5, #0]
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
	ldr	r2, [sp, #0]
	mov	lr, r0
	ldr	r7, [r2, #0]
	ldr	r3, [pc, #316]
	mov	r0, fp
	mov	r1, lr
@ 以降の各 ip 呼出しは IwramMulQ16ReturnIp による Q16 積。
	mov	ip, pc
	bx	r3
	adds	r1, r0, #0
	adds	r0, r7, #0
	mov	ip, pc
	bx	r3
	add	r5, sp, #8
	str	r0, [r5, #0]
	adds	r1, r6, #0
	mov	r0, fp
	mov	ip, pc
	bx	r3
	adds	r1, r0, #0
	adds	r0, r7, #0
	mov	ip, pc
	bx	r3
	mov	r2, r8
	str	r0, [r5, #4]
	negs	r1, r2
	adds	r0, r7, #0
	mov	ip, pc
	bx	r3
	str	r0, [r5, #8]
	ldr	r2, [sp, #0]
	mov	r0, sl
	ldr	r7, [r2, #4]
	mov	r1, r8
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
	adds	r1, r4, #0
	adds	r0, r7, #0
	movs	r0, r0
	mov	ip, pc
	bx	r3
	str	r0, [r5, #12]
	mov	r1, r8
	mov	r0, sl
	movs	r0, r0
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
	adds	r1, r4, #0
	adds	r0, r7, #0
	movs	r0, r0
	mov	ip, pc
	bx	r3
	str	r0, [r5, #16]
	mov	r1, fp
	mov	r0, sl
	movs	r0, r0
	mov	ip, pc
	bx	r3
	adds	r1, r0, #0
	adds	r0, r7, #0
	mov	ip, pc
	bx	r3
	str	r0, [r5, #20]
	ldr	r2, [sp, #0]
	mov	r0, r9
	ldr	r7, [r2, #8]
	mov	r1, r8
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
	adds	r1, r4, #0
	adds	r0, r7, #0
	movs	r0, r0
	mov	ip, pc
	bx	r3
	str	r0, [r5, #24]
	mov	r1, r8
	mov	r0, r9
	movs	r0, r0
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
	adds	r1, r4, #0
	adds	r0, r7, #0
	movs	r0, r0
	mov	ip, pc
	bx	r3
	str	r0, [r5, #28]
	mov	r1, fp
	mov	r0, r9
	movs	r0, r0
	mov	ip, pc
	bx	r3
	adds	r1, r0, #0
	adds	r0, r7, #0
	mov	ip, pc
	bx	r3
	str	r0, [r5, #32]
	ldr	r2, [sp, #4]
	ldr	r3, [r2, #0]
	str	r3, [r5, #36]
	ldr	r3, [r2, #4]
	str	r3, [r5, #40]
	ldr	r3, [r2, #8]
	adds	r0, r5, #0
	str	r3, [r5, #44]
	ldr	r3, [pc, #28]
@ IwramTransformMatrix へ渡す。
	bl	Func_080072f0
	add	sp, #56
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03000118
	.4byte 0x030002c0
