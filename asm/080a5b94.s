@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a5b94
	.thumb_func
Func_080a5b94:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r1, #167
	lsls	r1, r1, #4
	movs	r0, #55
	sub	sp, #16
	bl	Func_080048b0
	ldr	r1, [pc, #240]
	ldr	r2, [r1, #0]
	movs	r3, #1
	adds	r6, r0, #0
	strh	r3, [r2, #4]
	movs	r0, #0
	movs	r3, #20
	movs	r2, #30
	mov	r8, r1
	movs	r1, #0
	bl	Func_08015408
	movs	r0, #1
	bl	Func_080030f8
	movs	r0, #0
	bl	Func_080a1090
	movs	r2, #130
	lsls	r2, r2, #2
	adds	r0, r6, r2
	bl	Func_08077158
	ldr	r1, [pc, #196]
	adds	r3, r6, r1
	strb	r0, [r3, #0]
	movs	r1, #3
	movs	r0, #0
	movs	r2, #0
	movs	r3, #7
	bl	Func_080a3354
	movs	r3, #2
	str	r3, [sp, #0]
	movs	r1, #0
	movs	r2, #17
	movs	r3, #3
	movs	r0, #13
	bl	Func_08015010
	movs	r2, #134
	lsls	r2, r2, #1
	adds	r3, r6, r2
	str	r0, [r3, #0]
	movs	r0, #14
	bl	Func_080a2144
	ldr	r0, [pc, #152]
	bl	Func_08015418
	bl	Func_080a2474
	add	r0, sp, #12
	add	r1, sp, #8
	add	r2, sp, #4
	bl	Func_080a5cc0
	adds	r7, r0, #0
	bl	Func_080a2490
	cmp	r7, #1
	bne.n	.L0
	movs	r1, #188
	lsls	r1, r1, #1
	mov	r3, r8
	ldr	r5, [r3, #84]
	adds	r3, r6, r1
	ldrh	r3, [r3, #0]
	ldr	r0, [pc, #116]
	ands	r0, r3
	bl	Func_08077080
	ldr	r3, [sp, #12]
	ldr	r2, [sp, #4]
	movs	r1, #191
	lsls	r3, r3, #10
	lsls	r1, r1, #1
	orrs	r2, r3
	adds	r3, r5, r1
	strh	r2, [r3, #0]
.L0:
	ldr	r0, [r6, #36]
	mov	r6, r8
	adds	r6, #36
	bl	Func_08015278
	ldr	r5, [pc, #84]
	ldr	r2, [r6, #0]
	ldr	r3, [pc, #56]
	strb	r3, [r2, r5]
	bl	Func_080a34c0
	movs	r1, #0
	movs	r2, #30
	movs	r3, #20
	movs	r0, #0
	bl	Func_08015408
	movs	r0, #55
	bl	Func_08002dd8
	mov	r3, r8
	ldr	r2, [r3, #0]
	movs	r3, #0
	strh	r3, [r2, #4]
	movs	r0, #1
	bl	Func_080030f8
	movs	r0, #0
	movs	r1, #0
	movs	r2, #30
	movs	r3, #20
	bl	Func_08015410
	ldr	r3, [r6, #0]
	ldr	r2, [pc, #8]
	adds	r3, r3, r5
	b.n	.L1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x03001e68
	.4byte 0x00000219
	.4byte 0x06002500
	.4byte 0x00003fff
	.4byte 0x00000ea6
.L1:
	strb	r2, [r3, #0]
	bl	Func_0808a548
	adds	r0, r7, #0
	add	sp, #16
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
