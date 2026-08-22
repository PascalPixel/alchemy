@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08095778
	.thumb_func
Func_08095778:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	mov	r8, r0
	ldr	r1, [pc, #232]
	movs	r0, #141
	lsls	r0, r0, #2
	adds	r7, r1, r0
	movs	r3, #0
	ldrsh	r5, [r7, r3]
	ldrh	r2, [r7, #0]
	movs	r3, #240
	ldr	r6, [pc, #220]
	lsls	r3, r3, #8
	mov	r0, r8
	ands	r5, r3
	ands	r6, r2
	cmp	r0, #0
	bne.n	.L0
	cmp	r5, #0
	bne.n	.L1
	ldr	r3, [pc, #208]
	ldr	r2, [pc, #208]
	ands	r6, r3
	adds	r3, r6, r2
	cmp	r3, #80
	bhi.n	.L2
	ldr	r0, [pc, #204]
	adds	r3, r1, r0
	movs	r1, #0
	ldrsh	r2, [r3, r1]
	cmp	r2, #0
	ble.n	.L3
	ldr	r3, [pc, #196]
	cmp	r2, r3
	bne.n	.L2
.L3:
	adds	r0, r6, #0
	subs	r0, #172
	bl	Func_080770c8
	strh	r5, [r7, #0]
	b.n	.L2
.L1:
	movs	r0, #128
	lsls	r0, r0, #5
	cmp	r5, r0
	bne.n	.L2
	ldr	r2, [pc, #164]
	adds	r3, r1, r2
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #1
	bne.n	.L4
	adds	r0, r6, #0
	bl	Func_080770c8
.L4:
	mov	r1, r8
	strh	r1, [r7, #0]
	b.n	.L2
.L0:
	cmp	r5, #0
	bne.n	.L5
	ldr	r2, [pc, #128]
	ldr	r0, [pc, #132]
	ands	r6, r2
	adds	r3, r6, r0
	cmp	r3, #80
	bhi.n	.L5
	ands	r6, r2
	ldr	r2, [pc, #124]
	adds	r3, r1, r2
	movs	r0, #0
	ldrsh	r3, [r3, r0]
	cmp	r3, #0
	ble.n	.L5
	ldr	r1, [pc, #108]
	adds	r5, r6, r1
	adds	r0, r5, #0
	movs	r1, #20
	bl	Func_080022ec
	movs	r1, #20
	mov	r8, r0
	adds	r0, r5, #0
	bl	Func_080022fc
	movs	r5, #8
	adds	r7, r0, #0
	b.n	.L6
.L8:
	adds	r5, #1
.L6:
	cmp	r5, #65
	bgt.n	.L7
	adds	r0, r5, #0
	bl	Func_0808d394
	cmp	r0, #0
	beq.n	.L8
	movs	r2, #2
	ldrsh	r3, [r0, r2]
	ldr	r0, [pc, #60]
	subs	r3, #48
	adds	r2, r6, r0
	cmp	r3, r2
	bne.n	.L8
	movs	r0, #40
	bl	Func_080030f8
	mov	r1, r8
	adds	r0, r5, #0
	adds	r2, r7, #0
	bl	Func_080955b0
.L7:
	ldr	r1, [pc, #20]
.L5:
	movs	r3, #141
	lsls	r3, r3, #2
	adds	r2, r1, r3
	movs	r3, #0
	strh	r3, [r2, #0]
.L2:
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02000240
	.4byte 0x00000fff
	.4byte 0x000007ff
	.4byte 0xfffffed4
	.4byte 0x00000236
	.4byte 0x000003e7
