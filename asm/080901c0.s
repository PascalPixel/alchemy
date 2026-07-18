@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080901c0
	.thumb_func
Func_080901c0:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #255
	asrs	r2, r0, #8
	adds	r6, r3, #0
	ands	r2, r3
	adds	r7, r1, #0
	ands	r6, r0
	cmp	r2, #4
	bls.n	.L0
	b.n	.L1
.L0:
	lsls	r3, r2, #2
	ldr	r2, [pc, #132]
	ldr	r3, [r3, r2]
	mov	pc, r3
	.4byte 0x080901f4
	.4byte 0x08090202
	.4byte 0x08090214
	.4byte 0x08090278
	.4byte 0x080902e8
	movs	r0, #0
	bl	Func_08003bb4
	adds	r0, r7, #0
	bl	Func_08003b70
	b.n	.L1
	movs	r0, #128
	lsls	r0, r0, #8
	movs	r1, #0
	bl	Func_08091200
	adds	r0, r7, #0
	bl	Func_08091254
	b.n	.L1
	bl	Func_0808fecc
	movs	r2, #165
	adds	r5, r0, #0
	lsls	r2, r2, #3
	adds	r3, r5, r2
	strh	r6, [r3, #0]
	ldr	r2, [pc, #56]
	ldr	r3, [pc, #60]
	mov	r8, r2
	adds	r2, r5, r3
	movs	r3, #32
	strh	r3, [r2, #0]
	ldr	r3, [pc, #56]
	adds	r2, r5, r3
	movs	r3, #63
	strh	r3, [r2, #0]
	ldr	r3, [pc, #52]
	movs	r1, #200
	adds	r2, r5, r3
	movs	r3, #1
	strh	r3, [r2, #0]
	lsls	r1, r1, #4
	ldr	r0, [pc, #44]
	bl	Func_080041d8
	movs	r1, #144
	lsls	r1, r1, #3
	ldr	r0, [pc, #36]
	bl	Func_080041d8
	movs	r0, #1
	bl	Func_080030f8
	b.n	.L2
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x080901e0
	.4byte 0x0000052a
	.4byte 0x00000534
	.4byte 0x00000536
	.4byte 0x0808f52d
	.4byte 0x0808f499
	bl	Func_0808fecc
	movs	r2, #165
	adds	r5, r0, #0
	lsls	r2, r2, #3
	adds	r3, r5, r2
	strh	r6, [r3, #0]
	ldr	r2, [pc, #56]
	ldr	r3, [pc, #60]
	mov	r8, r2
	adds	r2, r5, r3
	movs	r3, #32
	strh	r3, [r2, #0]
	movs	r0, #0
	bl	Func_080907b0
	movs	r0, #1
	bl	Func_080030f8
	movs	r1, #200
	ldr	r0, [pc, #40]
	lsls	r1, r1, #4
	bl	Func_080041d8
.L2:
	ldr	r2, [pc, #36]
	ldr	r6, [pc, #24]
	adds	r3, r5, r2
	strb	r6, [r3, #0]
	ldr	r3, [pc, #32]
	adds	r2, r5, r3
	movs	r3, #64
	strb	r3, [r2, #0]
	ldr	r2, [pc, #28]
	adds	r3, r5, r2
	b.n	.L3
	movs	r0, r0
	.4byte 0x00000000
	.4byte 0x00000020
	.4byte 0x0000052a
	.4byte 0x08090659
	.4byte 0x0000053a
	.4byte 0x0000053b
	.4byte 0x0000053c
.L3:
	strb	r7, [r3, #0]
	ldr	r3, [pc, #124]
	mov	r2, r8
	adds	r5, r5, r3
	strb	r2, [r5, #0]
	b.n	.L1
	bl	Func_0808fecc
	adds	r5, r0, #0
	cmp	r6, #0
	bne.n	.L4
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #104]
	bl	Func_080041d8
	ldr	r2, [pc, #100]
	movs	r0, #1
	movs	r1, #0
	bl	Func_0800307c
	ldr	r2, [pc, #96]
	adds	r3, r5, r2
	strb	r6, [r3, #0]
	ldr	r3, [pc, #92]
	adds	r2, r5, r3
	movs	r3, #80
	strb	r3, [r2, #0]
	ldr	r2, [pc, #88]
	adds	r3, r5, r2
	adds	r2, #1
	strb	r7, [r3, #0]
	adds	r3, r5, r2
	strb	r6, [r3, #0]
	b.n	.L1
.L4:
	movs	r1, #200
	lsls	r1, r1, #4
	ldr	r0, [pc, #76]
	bl	Func_080041d8
	ldr	r2, [pc, #52]
	movs	r1, #0
	movs	r0, #1
	bl	Func_0800307c
	ldr	r2, [pc, #48]
	movs	r1, #0
	adds	r3, r5, r2
	strb	r1, [r3, #0]
	ldr	r3, [pc, #44]
	adds	r2, r5, r3
	movs	r3, #80
	strb	r3, [r2, #0]
	ldr	r2, [pc, #40]
	adds	r3, r5, r2
	adds	r2, #1
	strb	r7, [r3, #0]
	adds	r3, r5, r2
	strb	r1, [r3, #0]
.L1:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x0000053d
	.4byte 0x080903bd
	.4byte 0x08090585
	.4byte 0x0000053a
	.4byte 0x0000053b
	.4byte 0x0000053c
	.4byte 0x08090489
