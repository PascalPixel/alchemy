@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b1868
	.thumb_func
Func_080b1868:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #224]
	ldr	r3, [r3, #0]
	mov	r9, r1
	mov	fp, r3
	mov	r8, r0
	bl	Func_08077008
	mov	r3, r9
	lsls	r5, r3, #1
	adds	r7, r0, #0
	adds	r5, #216
	ldrh	r3, [r7, r5]
	ldr	r6, [pc, #204]
	ands	r6, r3
	adds	r0, r6, #0
	bl	Func_08077018
	ldrh	r2, [r7, r5]
	movs	r3, #128
	lsls	r3, r3, #2
	ands	r3, r2
	mov	sl, r0
	movs	r0, #0
	cmp	r3, #0
	bne.n	.L0
	mov	r0, r8
	adds	r1, r6, #0
	bl	Func_08077218
	cmp	r0, #0
	beq.n	.L1
	mov	r3, sl
	ldrb	r1, [r3, #2]
	mov	r0, r8
	bl	Func_08077228
	movs	r3, #1
	negs	r3, r3
	cmp	r0, r3
	beq.n	.L2
	lsls	r3, r0, #1
	adds	r3, #216
	ldrh	r0, [r7, r3]
	bl	Func_08077018
	ldrb	r2, [r0, #3]
	movs	r3, #2
	ands	r3, r2
	movs	r0, #0
	cmp	r3, #0
	bne.n	.L0
.L2:
	mov	r0, r8
	movs	r1, #1
	bl	Func_08015120
	ldr	r0, [pc, #120]
	bl	Func_080b04dc
	movs	r0, #0
	bl	Func_080b0634
	cmp	r0, #0
	beq.n	.L3
.L1:
	movs	r0, #0
	b.n	.L0
.L3:
	mov	r0, r8
	mov	r1, r9
	bl	Func_08077050
	mov	r3, fp
	ldr	r0, [r3, #32]
	cmp	r0, #0
	beq.n	.L4
	mov	r1, r8
	bl	Func_080b1dec
.L4:
	mov	r3, sl
	ldrb	r2, [r3, #3]
	movs	r3, #1
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L5
	movs	r0, #103
	bl	Func_080f9010
	bl	Func_08015140
	movs	r1, #8
	movs	r2, #4
	movs	r3, #2
	ldr	r0, [pc, #56]
	bl	Func_08015038
	b.n	.L6
.L7:
	movs	r0, #1
	bl	Func_080030f8
.L6:
	bl	Func_08015048
	cmp	r0, #0
	beq.n	.L7
.L5:
	ldr	r0, [pc, #36]
	bl	Func_080b0574
	movs	r0, #1
.L0:
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001f2c
	.4byte 0x000001ff
	.4byte 0x00000ca2
	.4byte 0x00000ad0
	.4byte 0x00000ca3
