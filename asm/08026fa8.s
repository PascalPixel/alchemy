@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08026fa8
	.thumb_func
Func_08026fa8:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #160]
	ldr	r1, [pc, #164]
	ldr	r2, [r3, #0]
	ldr	r3, [pc, #164]
	adds	r3, r3, r1
	ldrb	r3, [r3, #0]
	sub	sp, #132
	cmp	r3, #2
	beq.n	.L0
	cmp	r3, #2
	ble.n	.L0
	movs	r1, #1
	mov	r8, r1
	cmp	r3, #4
	ble.n	.L1
.L0:
	movs	r3, #0
	mov	r8, r3
.L1:
	mov	r1, r8
	cmp	r1, #0
	bne.n	.L2
	adds	r3, r2, #0
	adds	r3, #67
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L3
	movs	r3, #1
	mov	r8, r3
.L3:
	mov	r1, r8
	cmp	r1, #0
	beq.n	.L4
.L2:
	movs	r3, #42
	str	r3, [sp, #0]
	movs	r3, #4
	movs	r1, #7
	movs	r2, #30
	movs	r0, #0
	bl	Func_080162d4
	mov	sl, r0
	bl	Func_08016738
	add	r5, sp, #4
	adds	r1, r5, #0
	movs	r2, #52
	ldr	r0, [pc, #80]
	bl	Func_0801965c
	adds	r0, r5, #0
	mov	r1, sl
	movs	r2, #0
	movs	r3, #4
	bl	Func_08017aa4
	ldr	r7, [pc, #68]
	ldr	r5, [pc, #68]
	movs	r6, #3
.L6:
	movs	r0, #1
	bl	Func_080030f8
	ldr	r3, [r7, #0]
	ands	r3, r6
	cmp	r3, #0
	bne.n	.L5
	ldr	r3, [r5, #0]
	ldr	r3, [r3, #76]
	cmp	r3, #0
	bne.n	.L6
.L5:
	mov	r0, sl
	movs	r1, #1
	bl	Func_08016418
.L4:
	mov	r0, r8
	add	sp, #132
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001e74
	.4byte 0x0000022b
	.4byte 0x02000240
	.4byte 0x00000845
	.4byte 0x03001c94
	.4byte 0x03001f34
