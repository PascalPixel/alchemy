@ 角度 r1 と大きさ r0 から、ベクトル r2 の X に cos 成分、Z に sin 成分を加える。
@ cos は角度 +0x4000 の正弦。
@ mov ip,pc / bx でIWRAM核へ飛ぶ呼出し規約は C では表現不能なため、アセンブリとして保持する。
.syntax unified
	.thumb
	.global Func_0800447c
	.thumb_func
Func_0800447c:
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	mov	sl, r0
	movs	r0, #128
	mov	r8, r1
	lsls	r0, r0, #7
	add	r0, r8
	adds	r5, r2, #0
@ cos(角度) = sin(角度 + 0x4000)。
	bl	Func_08002322
	ldr	r6, [pc, #52]
	adds	r1, r0, #0
	mov	r0, sl
	movs	r0, r0
@ 大きさ × cos を IwramMulQ16ReturnIp で求め X に足す。
	mov	ip, pc
	bx	r6
	ldr	r3, [r5, #0]
	adds	r3, r3, r0
	stmia	r5!, {r3}
	mov	r0, r8
@ sin(角度)。
	bl	Func_08002322
	adds	r5, #4
	adds	r1, r0, #0
	mov	r0, sl
	movs	r0, r0
@ 大きさ × sin を Z に足す。
	mov	ip, pc
	bx	r6
	ldr	r3, [r5, #0]
	adds	r3, r3, r0
	str	r3, [r5, #0]
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03000118
