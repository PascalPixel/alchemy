@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08021390
	.thumb_func
Func_08021390:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	ldr	r3, [pc, #208]
	sub	sp, #28
	movs	r2, #0
	ldr	r5, [r3, #0]
	mov	sl, r0
	str	r2, [sp, #0]
	movs	r0, #2
	movs	r1, #1
	movs	r2, #26
	movs	r3, #5
	add	r7, sp, #16
	bl	Func_080162d4
	movs	r6, #0
	mov	r8, r0
	cmp	r0, #0
	beq.n	.L0
	movs	r1, #4
	movs	r3, #4
	movs	r2, #0
	str	r3, [sp, #0]
	bl	Func_0801e41c
	ldr	r3, [pc, #168]
	adds	r2, r5, r3
	movs	r3, #1
	strb	r3, [r2, #0]
	mov	r0, sl
	bl	Func_08021360
	bl	Func_08019d2c
	movs	r1, #14
	add	r2, sp, #12
	add	r3, sp, #8
	str	r1, [sp, #0]
	movs	r1, #0
	str	r6, [sp, #4]
	bl	Func_0801a4fc
	ldr	r3, [pc, #136]
	str	r6, [r7, #0]
	movs	r2, #224
	str	r3, [sp, #20]
	ldr	r3, [sp, #8]
	lsls	r2, r2, #8
	orrs	r3, r2
	ldr	r2, [pc, #128]
	str	r3, [sp, #24]
	adds	r3, r5, r2
	adds	r2, #2
	strh	r6, [r3, #0]
	adds	r3, r5, r2
	strh	r6, [r3, #0]
	movs	r1, #1
	mov	r0, sl
	bl	Func_08019908
	ldr	r0, [pc, #108]
	bl	Func_08019ba0
	movs	r2, #36
	adds	r1, r0, #0
	movs	r3, #2
	mov	r0, r8
	str	r6, [sp, #0]
	bl	Func_080165d8
	movs	r0, #81
	bl	Func_080f9010
	ldr	r5, [pc, #88]
	ldr	r6, [pc, #88]
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
	mov	r0, r8
	movs	r1, #2
	bl	Func_08016418
	movs	r0, #1
	bl	Func_080030f8
	ldr	r0, [sp, #12]
	bl	Func_08003f3c
.L0:
	add	sp, #28
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x03001e8c
	.4byte 0x00000ea3
	.4byte 0x8014000c
	.4byte 0x000012f4
	.4byte 0x0000001b
	.4byte 0x00000303
	.4byte 0x03001c94
