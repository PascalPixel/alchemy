@ 角度差の補正。Func_08097a10 で得た角度差 r4 を 0x3bffff / 0xff4c0000 の
@ 閾値で三区分し、区間ごとの係数を Q16 乗算・Q14 比率乗算して返す。
@ mov ip,pc / bx でIWRAM核へ飛ぶ呼出し規約は C では表現不能なため、アセンブリとして保持する。
.syntax unified
	.thumb
	.global Func_080979a4
	.thumb_func
Func_080979a4:
	push	{r5, r6, lr}
	adds	r6, r1, #0
	movs	r1, #180
	lsls	r1, r1, #17
	adds	r5, r2, #0
@ 角度差を求める。
	bl	Func_08097a10
	ldr	r2, [pc, #72]
	adds	r4, r0, #0
	cmp	r4, r2
	bgt.n	.L0
	ldr	r3, [pc, #68]
	adds	r0, r5, #0
	adds	r1, r4, #0
	b.n	.L1
.L0:
	ldr	r1, [pc, #64]
	adds	r3, r4, r1
	movs	r1, #240
	lsls	r1, r1, #15
	adds	r0, r5, #0
	cmp	r3, r1
	bcc.n	.L2
	ldr	r1, [pc, #52]
	adds	r3, r4, r1
	cmp	r3, r2
	bhi.n	.L3
	movs	r1, #240
	lsls	r1, r1, #16
	ldr	r3, [pc, #32]
	subs	r1, r1, r4
@ 共通経路: IwramMulQ16ReturnIp のあと IwramRatioMulQ14 (0x0300013c) を経由呼出し。
.L1:
	mov	ip, pc
	bx	r3
	adds	r1, r0, #0
	movs	r0, #240
	ldr	r3, [pc, #32]
	lsls	r0, r0, #14
	bl	Func_080072f0
	b.n	.L2
@ 範囲外: 引数 r1 をそのまま返す。
.L3:
	adds	r0, r6, #0
.L2:
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x003bffff
	.4byte 0x03000118
	.4byte 0xffc40000
	.4byte 0xff4c0000
	.4byte 0x0300013c
