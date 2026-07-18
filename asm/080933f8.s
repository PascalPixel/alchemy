@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080933f8
	.thumb_func
Func_080933f8:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #16
	adds	r6, r0, #0
	mov	sl, r1
	movs	r0, #27
	ldr	r1, [pc, #224]
	str	r3, [sp, #12]
	adds	r7, r2, #0
	bl	Func_080048f4
	movs	r1, #240
	str	r0, [sp, #8]
	lsls	r1, r1, #1
	adds	r3, r0, r1
	ldr	r5, [r3, #0]
	ldr	r3, [pc, #208]
	ldr	r1, [r3, #0]
	adds	r3, r1, #0
	adds	r3, #236
	ldr	r3, [r3, #0]
	movs	r2, #240
	lsls	r2, r2, #15
	adds	r2, r3, r2
	str	r2, [sp, #4]
	adds	r3, r1, #0
	adds	r3, #240
	ldr	r2, [r5, #12]
	ldr	r3, [r3, #0]
	movs	r0, #192
	adds	r3, r3, r2
	lsls	r0, r0, #15
	adds	r0, r3, r0
	str	r0, [sp, #0]
	adds	r3, r1, #0
	adds	r3, #244
	ldr	r3, [r3, #0]
	ldr	r0, [pc, #168]
	adds	r0, r0, r3
	adds	r3, r1, #0
	adds	r3, #248
	ldr	r3, [r3, #0]
	adds	r3, r3, r2
	ldr	r2, [pc, #160]
	adds	r2, r2, r3
	movs	r3, #8
	adds	r3, r3, r5
	str	r3, [r1, #0]
	mov	fp, r0
	adds	r0, r5, #0
	mov	r8, r3
	mov	r9, r2
	bl	Func_08009140
	movs	r3, #1
	negs	r3, r3
	cmp	r6, r3
	bne.n	.L0
	mov	r0, r8
	ldr	r6, [r0, #0]
.L0:
	cmp	sl, r3
	bne.n	.L1
	ldr	r1, [r5, #12]
	mov	sl, r1
.L1:
	cmp	r7, r3
	bne.n	.L2
	ldr	r7, [r5, #16]
.L2:
	ldr	r2, [sp, #4]
	cmp	r6, r2
	bge.n	.L3
	adds	r6, r2, #0
.L3:
	ldr	r3, [sp, #0]
	cmp	r7, r3
	bge.n	.L4
	adds	r7, r3, #0
.L4:
	cmp	r6, fp
	ble.n	.L5
	mov	r6, fp
.L5:
	cmp	r7, r9
	ble.n	.L6
	mov	r7, r9
.L6:
	ldr	r0, [sp, #12]
	cmp	r0, #0
	bne.n	.L7
	mov	r1, r8
	mov	r2, sl
	str	r6, [r1, #0]
	movs	r0, #1
	str	r2, [r5, #12]
	str	r7, [r5, #16]
	bl	Func_080030f8
	movs	r1, #207
	ldr	r0, [sp, #8]
	lsls	r1, r1, #1
	adds	r3, r0, r1
	movs	r2, #0
	ldrsh	r3, [r3, r2]
	cmp	r3, #3
	beq.n	.L8
	bl	Func_08009128
	b.n	.L8
.L7:
	adds	r0, r5, #0
	adds	r1, r6, #0
	mov	r2, sl
	adds	r3, r7, #0
	bl	Func_08009150
.L8:
	add	sp, #16
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x00000ccc
	.4byte 0x03001e70
	.4byte 0xff880000
	.4byte 0xffc00000
