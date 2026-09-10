@ 3D→画面投影。r1 のベクトルを IwramTransformVector で回し、奥行 -z が 0x03001ce0 の
@ 表の範囲内なら尺度 = tbl[0]<<5 / (z>>11) (tbl[0] が 0 なら 0x151eb) を求め、
@ x, y に Q16 乗算して表の中心 tbl[12], tbl[16] に加える。戻り値は尺度、範囲外は 0。
@ mov ip,pc / bx でIWRAM核へ飛ぶ呼出し規約は C では表現不能なため、アセンブリとして保持する。
.syntax unified
	.thumb
	.global Func_08005268
	.thumb_func
Func_08005268:
	push	{r5, r6, r7, lr}
	adds	r5, r1, #0
	ldr	r3, [pc, #108]
@ IwramTransformVector (0x03000250) を経由呼出し。
	bl	Func_080072f0
	ldr	r3, [r5, #8]
	ldr	r7, [pc, #104]
	negs	r1, r3
@ -z が tbl[4]..tbl[8] の外なら 0 を返す。
	ldr	r3, [r7, #4]
	movs	r0, #0
	cmp	r1, r3
	blt.n	.L0
	ldr	r3, [r7, #8]
	cmp	r1, r3
	bgt.n	.L0
	asrs	r3, r1, #16
	str	r3, [r5, #8]
	ldr	r0, [r7, #0]
	cmp	r0, #0
	beq.n	.L1
@ IwramUnsignedDivide (0x030003f0) で尺度を求める。
	lsrs	r1, r1, #11
	lsls	r0, r0, #5
	ldr	r3, [pc, #76]
	bl	Func_080072f0
	adds	r4, r0, #0
	b.n	.L2
.L1:
	ldr	r4, [pc, #72]
.L2:
	ldr	r6, [pc, #72]
	ldr	r0, [r5, #0]
	adds	r1, r4, #0
	movs	r0, r0
@ x × 尺度 (負値は 0xffff を足して切り上げ)。
	mov	ip, pc
	bx	r6
	cmp	r0, #0
	bge.n	.L3
	ldr	r3, [pc, #60]
	adds	r0, r0, r3
.L3:
	ldr	r3, [r7, #12]
	asrs	r2, r0, #16
	adds	r3, r3, r2
	str	r3, [r5, #0]
	ldr	r0, [r5, #4]
	adds	r1, r4, #0
@ y × 尺度、画面 y は下向きなので引く。
	mov	ip, pc
	bx	r6
	cmp	r0, #0
	bge.n	.L4
	ldr	r3, [pc, #36]
	adds	r0, r0, r3
.L4:
	ldr	r3, [r7, #16]
	asrs	r2, r0, #16
	subs	r3, r3, r2
	str	r3, [r5, #4]
	adds	r0, r4, #0
.L0:
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03000250
	.4byte 0x03001ce0
	.4byte 0x030003f0
	.4byte 0x000151eb
	.4byte 0x03000118
	.4byte 0x0000ffff
