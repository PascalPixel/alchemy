@ 効果の発生。obj+100 の角度計数が -1 でなければ、sin に 0x60000 を Q16 乗算した値で
@ obj[8..16] を基準記録から置き、計数を +1 して ±32 に折り返す。Func_08002304 が 0 なら
@ 位置を写し、乱数の大きさと角度で Func_0800447c を掛けて Func_08096c80 で新しい物を作る。
@ mov ip,pc / bx でIWRAM核へ飛ぶ呼出し規約は C では表現不能なため、アセンブリとして保持する。
.syntax unified
	.thumb
	.global Func_080993b0
	.thumb_func
Func_080993b0:
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	ldr	r3, [pc, #236]
	adds	r7, r5, #0
	adds	r7, #100
	ldr	r6, [r3, #0]
	movs	r2, #0
	ldrsh	r0, [r7, r2]
	movs	r3, #1
	negs	r3, r3
	sub	sp, #12
	cmp	r0, r3
	beq.n	.L0
	lsls	r0, r0, #10
@ sin。
	bl	Func_08002322
	adds	r1, r0, #0
	movs	r0, #192
	ldr	r3, [pc, #208]
	lsls	r0, r0, #11
@ IwramMulQ16ReturnIp: 0x60000 × sin。
	mov	ip, pc
	bx	r3
	ldr	r3, [r6, #4]
	adds	r3, r3, r0
	str	r3, [r5, #8]
	movs	r2, #128
	ldr	r3, [r6, #8]
	lsls	r2, r2, #13
	adds	r3, r3, r2
	str	r3, [r5, #12]
	ldr	r3, [r6, #12]
	str	r3, [r5, #16]
@ 角度計数を進め、6bit 符号付きに丸める。
	ldrh	r3, [r7, #0]
	adds	r3, #1
	strh	r3, [r7, #0]
	lsls	r3, r3, #16
	asrs	r1, r3, #16
	adds	r2, r1, #0
	adds	r2, #64
	adds	r3, r2, #0
	cmp	r2, #0
	bge.n	.L1
	adds	r3, r1, #0
	adds	r3, #127
.L1:
	asrs	r3, r3, #6
	lsls	r3, r3, #6
	subs	r3, r2, r3
	strh	r3, [r7, #0]
@ 3 番の条件が成り立たないときだけ新しい物を出す。
.L0:
	ldr	r3, [pc, #152]
	movs	r1, #3
	ldr	r0, [r3, #0]
	bl	Func_08002304
	cmp	r0, #0
	bne.n	.L2
	ldr	r3, [r5, #8]
	mov	r6, sp
	str	r3, [r6, #0]
	movs	r2, #128
	ldr	r3, [r5, #12]
	lsls	r2, r2, #10
	adds	r3, r3, r2
	str	r3, [r6, #4]
	ldr	r3, [r5, #16]
	str	r3, [r6, #8]
	bl	Func_08004458
	lsls	r5, r0, #1
	adds	r5, r5, r0
	bl	Func_08004458
	lsls	r5, r5, #1
	adds	r1, r0, #0
	adds	r2, r6, #0
	adds	r0, r5, #0
	bl	Func_0800447c
	ldr	r0, [pc, #100]
	ldr	r1, [r6, #0]
	ldr	r2, [r6, #4]
	ldr	r3, [r6, #8]
@ 生成した物に継続関数と初期値を置く。
	bl	Func_08096c80
	adds	r5, r0, #0
	cmp	r5, #0
	beq.n	.L2
	ldr	r3, [pc, #84]
	str	r3, [r5, #108]
	ldr	r3, [pc, #84]
	adds	r2, r5, #0
	adds	r2, #85
	str	r3, [r5, #28]
	str	r3, [r5, #24]
	movs	r3, #2
	strb	r3, [r2, #0]
	movs	r3, #229
	lsls	r3, r3, #1
	str	r3, [r5, #72]
	bl	Func_08004458
	adds	r3, r5, #0
	lsrs	r0, r0, #9
	adds	r3, #100
	strh	r0, [r3, #0]
	ldr	r3, [r5, #8]
	adds	r0, r5, #0
	str	r3, [r5, #56]
	movs	r1, #9
	bl	Func_08009240
	adds	r2, r5, #0
	adds	r2, #94
	movs	r3, #72
	strh	r3, [r2, #0]
	ldr	r1, [pc, #36]
	adds	r0, r5, #0
	bl	Func_08009098
.L2:
	add	sp, #12
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001f30
	.4byte 0x03000118
	.4byte 0x03001e40
	.4byte 0x0000011d
	.4byte 0x080992f1
	.4byte 0x00009999
	.4byte 0x0809f0b0
