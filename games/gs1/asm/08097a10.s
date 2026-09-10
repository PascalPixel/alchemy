@ 比率補正。y が 0 なら 0。y の上位4bitが立つ場合は符号を反転し、
@ IwramRatioMulQ14(y, x) の上位16bitに y を Q16 乗算して x から引く。
@ mov ip,pc / bx でIWRAM核へ飛ぶ呼出し規約は C では表現不能なため、アセンブリとして保持する。
.syntax unified
	.thumb
	.global Func_08097a10
	.thumb_func
Func_08097a10:
	push	{r5, r6, lr}
	adds	r5, r1, #0
	adds	r6, r0, #0
	movs	r0, #0
	cmp	r5, #0
	beq.n	.L0
	movs	r3, #240
	lsls	r3, r3, #24
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L1
	negs	r5, r5
.L1:
	adds	r1, r6, #0
	ldr	r3, [pc, #28]
	adds	r0, r5, #0
@ IwramRatioMulQ14 を経由呼出し (0x0300013c)。
	bl	Func_080072f0
	ldr	r3, [pc, #24]
	ldr	r4, [pc, #24]
	ands	r0, r3
	adds	r1, r5, #0
	movs	r0, r0
@ 上位16bitのみを IwramMulQ16ReturnIp (0x03000118) で y 倍する。
	mov	ip, pc
	bx	r4
	subs	r0, r6, r0
.L0:
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	.4byte 0x0300013c
	.4byte 0xffff0000
	.4byte 0x03000118
