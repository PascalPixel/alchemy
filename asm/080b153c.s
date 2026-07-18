@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080b153c
	.thumb_func
Func_080b153c:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #188]
	ldr	r3, [r3, #0]
	adds	r6, r1, #0
	mov	sl, r3
	adds	r5, r0, #0
	bl	Func_08077008
	adds	r7, r0, #0
	adds	r0, r6, #0
	bl	Func_08077018
	mov	r8, r0
	mov	r1, r8
	ldrb	r2, [r1, #3]
	movs	r3, #16
	ands	r3, r2
	movs	r0, #1
	cmp	r3, #0
	beq.n	.L0
	ldr	r0, [pc, #156]
	bl	Func_080b04dc
	adds	r0, r5, #0
	adds	r1, r6, #0
	bl	Func_08077038
	movs	r2, #1
	negs	r2, r2
	cmp	r0, r2
	beq.n	.L1
	lsls	r3, r0, #1
	adds	r3, #216
	ldrh	r3, [r7, r3]
	lsrs	r3, r3, #11
	adds	r7, r3, #1
	b.n	.L2
.L1:
	movs	r7, #0
.L2:
	mov	r2, r8
	movs	r3, #0
	ldrsh	r1, [r2, r3]
	movs	r5, #30
	cmp	r1, #0
	beq.n	.L3
	ldr	r3, [pc, #112]
	ldr	r0, [r3, #16]
	bl	Func_080022f4
	adds	r5, r0, #0
.L3:
	ldr	r3, [pc, #104]
	add	r3, sl
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #2
	bne.n	.L4
	adds	r0, r6, #0
	movs	r1, #0
	bl	Func_08077240
	cmp	r5, r0
	ble.n	.L5
	adds	r0, r6, #0
	movs	r1, #0
	bl	Func_08077240
	b.n	.L6
.L5:
	adds	r0, r5, #0
.L6:
	adds	r5, r0, #0
.L4:
	adds	r5, r5, r7
	cmp	r5, #30
	ble.n	.L7
	movs	r5, #30
.L7:
	movs	r3, #234
	lsls	r3, r3, #2
	add	r3, sl
	movs	r2, #12
	strb	r2, [r3, #0]
	movs	r0, #0
	movs	r1, #128
	movs	r2, #48
	bl	Func_080b0a6c
	mov	r1, r8
	movs	r3, #0
	ldrsh	r2, [r1, r3]
	adds	r0, r7, #0
	adds	r1, r5, #0
	bl	Func_080b1614
.L0:
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001f2c
	.4byte 0x00000ca0
	.4byte 0x02000240
	.4byte 0x000003aa
