@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b04dc
	.thumb_func
Func_080b04dc:
	push	{r5, r6, r7, lr}
	ldr	r3, [pc, #124]
	movs	r2, #233
	ldr	r6, [r3, #0]
	lsls	r2, r2, #2
	adds	r3, r6, r2
	adds	r5, r0, #0
	ldrh	r0, [r3, #0]
	bl	Func_0808a540
	adds	r7, r0, #0
	bl	Func_08015140
	ldr	r2, [pc, #104]
	adds	r3, r6, r2
	movs	r1, #0
	ldrsb	r1, [r3, r1]
	cmp	r1, #2
	bne.n	.L0
	ldr	r3, [pc, #96]
	ldr	r2, [pc, #96]
	subs	r3, r3, r2
	adds	r5, r5, r3
.L0:
	cmp	r1, #0
	bne.n	.L1
	ldr	r3, [pc, #92]
	ldr	r2, [pc, #84]
	subs	r3, r3, r2
	adds	r5, r5, r3
.L1:
	movs	r2, #235
	lsls	r2, r2, #2
	adds	r3, r6, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L2
	ldr	r3, [pc, #72]
	ldr	r2, [pc, #60]
	subs	r3, r3, r2
	adds	r5, r5, r3
.L2:
	lsls	r3, r7, #16
	movs	r2, #34
	orrs	r3, r2
	adds	r0, r5, #0
	movs	r1, #5
	movs	r2, #0
	bl	Func_08015038
	b.n	.L3
.L4:
	movs	r0, #1
	bl	Func_080030f8
.L3:
	bl	Func_08015048
	cmp	r0, #0
	beq.n	.L4
	movs	r0, #1
	bl	Func_080030f8
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001f2c
	.4byte 0x000003a9
	.4byte 0x00000cc6
	.4byte 0x00000c9b
	.4byte 0x00000cf1
	.4byte 0x00000d4c
