@ 補間の一段。0x03001e70 の場面記録で、対象 (Func_080048f4(27, 0xccc)) の +91 が 0 で
@ 残段数 +0x358 があれば、計数 +0x35a を進めて +0x350..+0x354 の差を段数で割った
@ 分だけ進め、+0x348 との Q16 積を +0x34c に置く。計数が段数に達すれば自身を再登録する。
@ mov ip,pc / bx でIWRAM核へ飛ぶ呼出し規約は C では表現不能なため、アセンブリとして保持する。
.syntax unified
	.thumb
	.global Func_080935d4
	.thumb_func
Func_080935d4:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #168]
	ldr	r1, [pc, #172]
	movs	r0, #27
	ldr	r7, [r3, #0]
	bl	Func_080048f4
	movs	r1, #240
	lsls	r1, r1, #1
	adds	r0, r0, r1
	ldr	r3, [r0, #0]
	adds	r3, #91
	ldrb	r3, [r3, #0]
	mov	sl, r3
	cmp	r3, #0
	bne.n	.L0
	movs	r2, #214
	lsls	r2, r2, #2
	adds	r2, r2, r7
	movs	r0, #0
	ldrsh	r3, [r2, r0]
	mov	r8, r2
	cmp	r3, #0
	beq.n	.L0
	movs	r1, #212
	movs	r2, #213
	lsls	r1, r1, #2
	lsls	r2, r2, #2
	adds	r6, r7, r1
	adds	r3, r7, r2
	ldr	r2, [r3, #0]
	ldr	r3, [r6, #0]
	subs	r2, r2, r3
	ldr	r3, [pc, #112]
	adds	r5, r7, r3
	ldrh	r3, [r5, #0]
	adds	r3, #1
	strh	r3, [r5, #0]
	lsls	r3, r3, #16
	asrs	r3, r3, #16
	adds	r0, r3, #0
	muls	r0, r2
	mov	r3, r8
	movs	r2, #0
	ldrsh	r1, [r3, r2]
@ 差 × 計数 / 段数。
	bl	Func_080022ec
	adds	r2, r0, #0
	movs	r0, #210
	ldr	r1, [r6, #0]
	lsls	r0, r0, #2
	adds	r3, r7, r0
	ldr	r4, [pc, #80]
	ldr	r0, [r3, #0]
	adds	r1, r1, r2
@ IwramMulQ16ReturnIp: +0x348 × 補間値。
	mov	ip, pc
	bx	r4
	movs	r1, #211
	lsls	r1, r1, #2
	adds	r3, r7, r1
	str	r0, [r3, #0]
	movs	r0, #140
	lsls	r0, r0, #1
	adds	r3, r7, r0
	ldrh	r3, [r3, #0]
	ldr	r2, [pc, #56]
	adds	r3, #1
	str	r3, [r2, #0]
	movs	r1, #0
	ldrsh	r2, [r5, r1]
	mov	r1, r8
	movs	r0, #0
	ldrsh	r3, [r1, r0]
	cmp	r2, r3
	bne.n	.L0
	mov	r2, sl
	mov	r3, r8
	strh	r2, [r3, #0]
	ldr	r0, [pc, #36]
@ 計数が段数に達した: 残段数を消し、自身 (Thumb bit 付き) を登録し直す。
	bl	Func_08004278
.L0:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001e70
	.4byte 0x00000ccc
	.4byte 0x0000035a
	.4byte 0x03000118
	.4byte 0x03001af4
	.4byte 0x080935d5
