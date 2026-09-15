@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08021228
	.thumb_func
Func_08021228:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #32
	str	r2, [sp, #8]
	ldr	r3, [pc, #256]
	adds	r6, r1, #0
	ldr	r5, [r3, #0]
	movs	r3, #3
	ldr	r2, [pc, #252]
	ands	r3, r6
	lsls	r3, r3, #1
	mov	fp, r0
	movs	r0, #0
	mov	r9, r0
	mov	sl, r0
	ldrsh	r1, [r2, r3]
	mov	r8, r1
	mov	r1, r9
	str	r1, [sp, #0]
	movs	r0, #2
	movs	r1, #1
	movs	r2, #26
	movs	r3, #5
	add	r7, sp, #20
	bl	Func_080162d4
	mov	r9, r0
	cmp	r0, #0
	beq.n	.L0
	movs	r3, #4
	movs	r1, #4
	movs	r2, #0
	str	r3, [sp, #0]
	bl	Func_0801e41c
	ldr	r3, [pc, #200]
	adds	r2, r5, r3
	movs	r3, #1
	strb	r3, [r2, #0]
	mov	r0, r8
	bl	Func_08019d2c
	movs	r1, #14
	add	r2, sp, #16
	add	r3, sp, #12
	str	r1, [sp, #0]
	mov	r1, sl
	str	r1, [sp, #4]
	bl	Func_0801a4fc
	mov	r2, sl
	ldr	r3, [pc, #172]
	str	r2, [r7, #0]
	str	r3, [sp, #24]
	movs	r2, #224
	ldr	r3, [sp, #12]
	lsls	r2, r2, #8
	ldr	r0, [pc, #164]
	orrs	r3, r2
	ldr	r2, [pc, #164]
	str	r3, [sp, #28]
	mov	r1, sl
	adds	r3, r5, r0
	strh	r1, [r3, #0]
	mov	r0, sl
	adds	r3, r5, r2
	strh	r0, [r3, #0]
	movs	r1, #1
	mov	r0, fp
	bl	Func_08019908
	lsls	r0, r6, #2
	ldr	r1, [sp, #8]
	adds	r0, r0, r6
	lsls	r0, r0, #2
	movs	r2, #150
	lsls	r2, r2, #1
	adds	r0, r0, r1
	adds	r0, r0, r2
	movs	r1, #4
	bl	Func_08019908
	ldr	r0, [pc, #124]
	adds	r0, r6, r0
	bl	Func_08019ba0
	mov	r3, sl
	adds	r1, r0, #0
	str	r3, [sp, #0]
	movs	r2, #36
	movs	r3, #2
	mov	r0, r9
	bl	Func_080165d8
	movs	r0, #81
	bl	Func_080f9010
	ldr	r5, [pc, #96]
	ldr	r6, [pc, #100]
.L2:
	adds	r0, r7, #0
	movs	r1, #250
	bl	Func_08003dec
	movs	r0, #1
	bl	Func_080030f8
	bl	Func_080f9048
	cmp	r0, #0
	beq.n	.L1
	ldr	r3, [r6, #0]
	ands	r3, r5
	cmp	r3, #0
	beq.n	.L2
.L1:
	mov	r0, r9
	movs	r1, #2
	bl	Func_08016418
	movs	r0, #1
	bl	Func_080030f8
	ldr	r0, [sp, #16]
	bl	Func_08003f3c
.L0:
	add	sp, #32
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e8c
	.4byte 0x080371fe
	.4byte 0x00000ea3
	.4byte 0x8014000c
	.4byte 0x000012f4
	.4byte 0x000012f6
	.4byte 0x00000980
	.4byte 0x00000303
	.4byte 0x03001c94
