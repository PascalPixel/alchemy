@ 役者の描画。DMA3 で 0x08009bb8 の ARM 核 (0x2c4 バイト) を Func_080048b0(52) の緩衝へ写し、
@ 0x03001e70-12 の 64 役者 (112 バイトずつ) について、状態 +84 の下位 4 bit が 1 で視点からの
@ 距離が範囲内なら、地図語から影と高さの旗を取り、obj[16], obj[20] に [r5+24] を IwramMulQ16 で
@ 掛けた尺度と位置 3 語を組んで Func_0800b168 で描く。範囲外や非表示は Func_08003f78 で
@ 絵を返し +37 に印を付ける。終わりに Func_08002dd8(52) で緩衝を返す。
@ mov ip,pc / bx でIWRAM核へ飛ぶ呼出し規約は C では表現不能なため、アセンブリとして保持する。
.syntax unified
	.thumb
	.global Func_0800c62c
	.thumb_func
Func_0800c62c:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r6, [pc, #532]
	ldr	r0, [r6, #0]
	sub	sp, #80
	str	r0, [sp, #12]
	adds	r2, r0, #0
	adds	r2, #228
	ldr	r1, [r2, #0]
	ldr	r3, [pc, #520]
	ands	r1, r3
	str	r1, [sp, #8]
	ldr	r2, [r2, #4]
	ands	r2, r3
	str	r2, [sp, #4]
	adds	r3, r6, #0
	subs	r3, #8
	ldr	r3, [r3, #0]
	str	r3, [sp, #0]
	ldr	r5, [pc, #504]
	movs	r0, #52
	adds	r1, r5, #0
	bl	Func_080048b0
	movs	r2, #132
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	adds	r1, r0, #0
	ldr	r3, [pc, #492]
	ldr	r0, [pc, #492]
	orrs	r2, r5
@ DMA3: ARM 核を緩衝へ。
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #0
	subs	r6, #12
	ldr	r6, [r6, #0]
	ldr	r2, [sp, #0]
	mov	sl, r6
	strh	r3, [r2, #0]
	movs	r4, #63
	ldr	r3, [pc, #476]
	movs	r0, #84
	add	r0, sl
	mov	r7, sl
	str	r4, [sp, #16]
	mov	fp, r3
	mov	r8, r0
	adds	r7, #8
@ 役者ループ。
.L16:
	mov	r1, sl
	ldr	r3, [r1, #0]
	cmp	r3, #0
	bne.n	.L0
	b.n	.L1
.L0:
	ldr	r1, [r7, #0]
	cmp	r1, #0
	bne.n	.L2
	ldr	r3, [r7, #8]
	cmp	r3, #0
	bne.n	.L2
	b.n	.L3
.L2:
	mov	r2, r8
	ldrb	r3, [r2, #0]
	movs	r6, #15
	ands	r6, r3
	cmp	r6, #0
	bne.n	.L4
	b.n	.L1
.L4:
	cmp	r6, #1
	beq.n	.L5
	b.n	.L1
.L5:
	ldr	r0, [sp, #0]
	movs	r4, #4
	ldrsh	r3, [r0, r4]
	cmp	r3, #0
	beq.n	.L6
	ldrb	r3, [r2, #8]
	cmp	r3, #0
	bne.n	.L6
	ldr	r5, [r7, #72]
	ldrb	r0, [r5, #28]
	adds	r5, #37
	bl	Func_08003f78
	strb	r6, [r5, #0]
	b.n	.L1
@ 距離の範囲判定。
.L6:
	ldr	r3, [sp, #4]
	ldr	r0, [r7, #8]
	ldr	r2, [sp, #8]
	subs	r6, r0, r3
	ldr	r3, [r7, #4]
	subs	r2, r1, r2
	mov	r9, r2
	subs	r2, r6, r3
	ldr	r3, [pc, #372]
	ldr	r4, [pc, #376]
	add	r3, r9
	ldr	r5, [r7, #72]
	cmp	r3, r4
	bls.n	.L7
	b.n	.L8
.L7:
	ldr	r3, [pc, #368]
	cmp	r2, r3
	ble.n	.L8
	ldr	r4, [pc, #364]
	cmp	r2, r4
	bgt.n	.L8
	ldrb	r3, [r7, #26]
	movs	r2, #34
	add	r2, sl
	mov	ip, r2
	lsls	r2, r3, #1
	adds	r2, r2, r3
	movs	r3, #152
	lsls	r3, r3, #1
	lsls	r2, r2, #4
	adds	r2, r2, r3
	ldr	r4, [sp, #12]
	asrs	r3, r0, #20
	lsls	r3, r3, #7
	asrs	r1, r1, #20
	adds	r1, r1, r3
	ldr	r3, [r4, r2]
	lsls	r1, r1, #2
	ldrb	r2, [r7, #27]
	adds	r1, r3, r1
	movs	r0, #35
	movs	r3, #1
	add	r0, sl
	ands	r3, r2
	mov	lr, r0
	cmp	r3, #0
	beq.n	.L9
	ldr	r4, [r1, #0]
	lsls	r3, r4, #16
	lsrs	r0, r3, #30
	cmp	r0, #0
	beq.n	.L10
	movs	r2, #13
	ldrb	r1, [r5, #9]
	negs	r2, r2
	adds	r3, r2, #0
	lsls	r0, r0, #2
	ands	r3, r1
	orrs	r3, r0
	strb	r3, [r5, #9]
	ldrb	r3, [r5, #21]
	ands	r2, r3
	orrs	r2, r0
	strb	r2, [r5, #21]
	b.n	.L10
@ 地図語: 影 (bit 14-15) と高さ (bit 12-13)。
.L9:
	ldr	r4, [r1, #0]
.L10:
	lsls	r3, r4, #18
	lsrs	r1, r3, #30
	cmp	r1, #0
	beq.n	.L11
	adds	r3, r1, #0
	adds	r3, #255
	mov	r1, ip
	strb	r3, [r1, #0]
.L11:
	ldr	r0, [r7, #16]
	ldr	r1, [r5, #24]
@ IwramMulQ16ReturnIp: obj[16] × 尺度、obj[20] × 尺度。
	mov	ip, pc
	bx	fp
	str	r0, [sp, #20]
	ldr	r0, [r7, #20]
	ldr	r1, [r5, #24]
	movs	r0, r0
	mov	ip, pc
	bx	fp
	movs	r2, #20
	add	r1, sp, #28
	add	r2, sp
	mov	r3, r9
	str	r0, [r2, #4]
	str	r3, [r1, #0]
	mov	ip, r2
	ldr	r2, [r7, #4]
	str	r6, [r1, #8]
	str	r2, [r1, #4]
	ldr	r4, [r7, #12]
	str	r4, [r1, #12]
	mov	r3, lr
	ldrb	r0, [r3, #0]
	movs	r3, #2
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L12
	ldr	r3, [pc, #200]
	adds	r2, r2, r3
	str	r2, [r1, #4]
	adds	r2, r6, r3
	adds	r3, r4, r3
	str	r2, [r1, #8]
	str	r3, [r1, #12]
	mov	r4, lr
	ldrb	r0, [r4, #0]
@ 旗 2 / 4 で位置をずらす。
.L12:
	movs	r3, #4
	ands	r3, r0
	cmp	r3, #0
	beq.n	.L13
	ldr	r3, [r1, #4]
	movs	r2, #160
	lsls	r2, r2, #17
	adds	r3, r3, r2
	str	r3, [r1, #4]
	ldr	r3, [r1, #8]
	adds	r3, r3, r2
	str	r3, [r1, #8]
	ldr	r3, [r1, #12]
	adds	r3, r3, r2
	str	r3, [r1, #12]
.L13:
	mov	r0, sl
	ldrh	r3, [r0, #6]
	mov	r2, ip
	adds	r0, r5, #0
	bl	Func_0800b168
	b.n	.L1
.L8:
	mov	r1, r8
	ldrb	r3, [r1, #8]
	cmp	r3, #0
	bne.n	.L1
	ldrb	r2, [r5, #29]
	movs	r6, #1
	b.n	.L14
.L3:
	mov	r2, r8
	ldrb	r3, [r2, #0]
	movs	r6, #15
	ands	r6, r3
	cmp	r6, #1
	bne.n	.L1
	ldrb	r3, [r2, #8]
	ldr	r5, [r7, #72]
	cmp	r3, #0
	bne.n	.L1
	ldrb	r2, [r5, #29]
@ 非表示: 絵を返す。
.L14:
	adds	r3, r6, #0
	ands	r3, r2
	cmp	r3, #0
	bne.n	.L1
	ldrb	r0, [r5, #28]
	bl	Func_08003f78
	adds	r3, r5, #0
	adds	r3, #37
	strb	r6, [r3, #0]
.L1:
	ldr	r3, [sp, #16]
	movs	r4, #112
	subs	r3, #1
	str	r3, [sp, #16]
	add	r8, r4
	adds	r7, #112
	add	sl, r4
	cmp	r3, #0
	blt.n	.L15
	b.n	.L16
.L15:
	movs	r0, #52
	bl	Func_08002dd8
	add	sp, #80
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e70
	.4byte 0xffff0000
	.4byte 0x000002c4
	.4byte 0x040000d4
	.4byte 0x08009bb8
	.4byte 0x03000118
	.4byte 0x001fffff
	.4byte 0x012ffffe
	.4byte 0xffe00000
	.4byte 0x00dfffff
	.4byte 0xfec00000
