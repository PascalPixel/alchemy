@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0808a6e4
	.thumb_func
Func_0808a6e4:
	push	{r5, r6, lr}
	ldr	r1, [pc, #480]
	ldr	r2, [pc, #480]
	movs	r4, #1
	adds	r3, r1, r2
	negs	r4, r4
	strh	r0, [r3, #0]
	cmp	r0, r4
	beq.n	.L0
	b.n	.L1
.L0:
	movs	r4, #250
	lsls	r4, r4, #1
	adds	r3, r1, r4
	ldr	r0, [r3, #0]
	bl	Func_08077008
	adds	r6, r0, #0
	movs	r5, #56
	ldrsh	r3, [r6, r5]
	cmp	r3, #0
	bne.n	.L2
	movs	r5, #1
	strh	r5, [r6, #56]
	lsls	r5, r5, #14
	movs	r0, #52
	ldrsh	r1, [r6, r0]
	adds	r0, r5, #0
	bl	Func_080022ec
	movs	r1, #128
	lsls	r1, r1, #7
	cmp	r0, r1
	bgt.n	.L3
	movs	r5, #0
	cmp	r0, #0
	blt.n	.L3
	adds	r5, r0, #0
.L3:
	lsls	r3, r5, #16
	strh	r5, [r6, #20]
	cmp	r3, #0
	bne.n	.L4
	movs	r2, #56
	ldrsh	r3, [r6, r2]
	cmp	r3, #0
	beq.n	.L4
	movs	r3, #1
	strh	r3, [r6, #20]
.L4:
	movs	r3, #58
	ldrsh	r0, [r6, r3]
	movs	r4, #54
	ldrsh	r1, [r6, r4]
	lsls	r0, r0, #14
	bl	Func_080022ec
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bgt.n	.L5
	movs	r3, #0
	cmp	r0, #0
	blt.n	.L5
	adds	r3, r0, #0
.L5:
	strh	r3, [r6, #22]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L2
	movs	r5, #58
	ldrsh	r3, [r6, r5]
	cmp	r3, #0
	beq.n	.L2
	movs	r3, #1
	strh	r3, [r6, #22]
.L2:
	movs	r0, #32
	bl	Func_080770c0
	cmp	r0, #0
	beq.n	.L6
	movs	r5, #0
.L11:
	adds	r0, r5, #0
	bl	Func_08077008
	adds	r6, r0, #0
	ldrh	r1, [r6, #52]
	ldrh	r3, [r6, #54]
	strh	r1, [r6, #56]
	strh	r3, [r6, #58]
	lsls	r1, r1, #16
	asrs	r1, r1, #16
	lsls	r0, r1, #14
	bl	Func_080022ec
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bgt.n	.L7
	movs	r3, #0
	cmp	r0, #0
	blt.n	.L7
	adds	r3, r0, #0
.L7:
	strh	r3, [r6, #20]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L8
	movs	r0, #56
	ldrsh	r3, [r6, r0]
	cmp	r3, #0
	beq.n	.L8
	movs	r3, #1
	strh	r3, [r6, #20]
.L8:
	movs	r1, #58
	ldrsh	r0, [r6, r1]
	movs	r2, #54
	ldrsh	r1, [r6, r2]
	lsls	r0, r0, #14
	bl	Func_080022ec
	movs	r3, #128
	lsls	r3, r3, #7
	cmp	r0, r3
	bgt.n	.L9
	movs	r3, #0
	cmp	r0, #0
	blt.n	.L9
	adds	r3, r0, #0
.L9:
	strh	r3, [r6, #22]
	lsls	r3, r3, #16
	cmp	r3, #0
	bne.n	.L10
	movs	r4, #58
	ldrsh	r3, [r6, r4]
	cmp	r3, #0
	beq.n	.L10
	movs	r3, #1
	strh	r3, [r6, #22]
.L10:
	adds	r5, #1
	cmp	r5, #1
	ble.n	.L11
.L6:
	ldr	r1, [pc, #208]
	movs	r5, #233
	lsls	r5, r5, #1
	movs	r4, #234
	adds	r3, r1, r5
	lsls	r4, r4, #1
	movs	r0, #0
	ldrsh	r2, [r3, r0]
	adds	r3, r1, r4
	movs	r5, #0
	ldrsh	r0, [r3, r5]
	movs	r3, #1
	negs	r3, r3
	cmp	r2, r3
	bne.n	.L12
	cmp	r0, r2
	bne.n	.L13
	subs	r4, #16
	adds	r3, r1, r4
	movs	r5, #224
	ldrh	r2, [r3, #0]
	lsls	r5, r5, #1
	adds	r3, r1, r5
	movs	r0, #227
	strh	r2, [r3, #0]
	lsls	r0, r0, #1
	adds	r3, r1, r0
	ldrh	r3, [r3, #0]
	subs	r4, #2
	adds	r2, r1, r4
	strh	r3, [r2, #0]
	b.n	.L14
.L12:
	movs	r5, #224
	lsls	r5, r5, #1
	adds	r3, r1, r5
	b.n	.L15
.L13:
	movs	r2, #228
	lsls	r2, r2, #1
	adds	r3, r1, r2
	b.n	.L16
.L1:
	movs	r0, #231
	lsls	r0, r0, #1
	adds	r3, r1, r0
	movs	r5, #0
	ldrsh	r2, [r3, r5]
	adds	r0, #2
	adds	r3, r1, r0
	movs	r5, #0
	ldrsh	r0, [r3, r5]
	cmp	r2, r4
	bne.n	.L17
	cmp	r0, r2
	beq.n	.L18
	b.n	.L19
.L17:
	movs	r4, #224
	b.n	.L20
.L19:
	movs	r5, #228
	lsls	r5, r5, #1
	adds	r3, r1, r5
.L16:
	movs	r4, #224
	ldrh	r2, [r3, #0]
.L20:
	lsls	r4, r4, #1
	adds	r3, r1, r4
.L15:
	strh	r2, [r3, #0]
	movs	r5, #1
	negs	r5, r5
	cmp	r0, r5
	beq.n	.L21
	ldr	r3, [pc, #72]
	movs	r1, #225
	lsls	r1, r1, #1
	adds	r3, r3, r1
	strh	r0, [r3, #0]
	b.n	.L14
.L21:
	ldr	r2, [pc, #60]
	movs	r4, #229
	lsls	r4, r4, #1
	adds	r3, r2, r4
	movs	r5, #225
	ldrh	r3, [r3, #0]
	lsls	r5, r5, #1
	adds	r2, r2, r5
	strh	r3, [r2, #0]
	b.n	.L14
.L18:
	movs	r0, #228
	lsls	r0, r0, #1
	adds	r3, r1, r0
	movs	r4, #224
	ldrh	r2, [r3, #0]
	lsls	r4, r4, #1
	adds	r3, r1, r4
	movs	r5, #229
	strh	r2, [r3, #0]
	lsls	r5, r5, #1
	adds	r3, r1, r5
	subs	r0, #6
	ldrh	r3, [r3, #0]
	adds	r2, r1, r0
	strh	r3, [r2, #0]
	subs	r0, #185
	bl	Func_080770c8
.L14:
	pop	{r5, r6}
	pop	{r0}
	bx	r0
	.4byte 0x02000240
	.4byte 0x00000236
