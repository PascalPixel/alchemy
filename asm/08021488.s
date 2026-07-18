@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.pyでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_08021488
	.thumb_func
Func_08021488:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #48
	str	r1, [sp, #8]
	ldr	r3, [pc, #288]
	ldr	r3, [r3, #0]
	mov	r8, r3
	movs	r3, #0
	mov	r9, r3
	add	r3, sp, #24
	mov	sl, r3
	mov	r3, r9
	str	r3, [sp, #0]
	mov	fp, r0
	movs	r1, #1
	movs	r0, #1
	movs	r2, #28
	movs	r3, #5
	bl	Func_080162d4
	movs	r6, #0
	mov	r9, r0
	cmp	r0, #0
	beq.n	.L0
	movs	r1, #8
	movs	r3, #4
	movs	r2, #0
	str	r3, [sp, #0]
	bl	Func_0801e41c
	ldr	r2, [pc, #240]
	movs	r3, #1
	add	r2, r8
	strb	r3, [r2, #0]
	mov	r0, fp
	bl	Func_08021360
	bl	Func_08019d2c
	movs	r3, #14
	add	r5, sp, #16
	add	r2, sp, #20
	str	r3, [sp, #0]
	movs	r1, #0
	adds	r3, r5, #0
	str	r6, [sp, #4]
	bl	Func_0801a4fc
	mov	r3, sl
	str	r6, [r3, #0]
	ldr	r3, [pc, #204]
	movs	r2, #224
	str	r3, [sp, #28]
	ldr	r3, [sp, #16]
	lsls	r2, r2, #8
	ldr	r0, [sp, #8]
	orrs	r3, r2
	str	r3, [sp, #32]
	add	r7, sp, #36
	bl	Func_08021360
	bl	Func_08019d2c
	movs	r3, #15
	add	r2, sp, #12
	str	r3, [sp, #0]
	movs	r1, #0
	adds	r3, r5, #0
	str	r6, [sp, #4]
	bl	Func_0801a4fc
	ldr	r3, [pc, #164]
	str	r6, [r7, #0]
	movs	r2, #240
	str	r3, [sp, #40]
	ldr	r3, [sp, #16]
	lsls	r2, r2, #8
	orrs	r3, r2
	str	r3, [sp, #44]
	ldr	r3, [pc, #152]
	add	r3, r8
	strh	r6, [r3, #0]
	ldr	r3, [pc, #152]
	add	r3, r8
	strh	r6, [r3, #0]
	mov	r0, fp
	movs	r1, #1
	bl	Func_08019908
	movs	r1, #1
	ldr	r0, [sp, #8]
	bl	Func_08019908
	ldr	r0, [pc, #132]
	bl	Func_08019ba0
	movs	r2, #68
	adds	r1, r0, #0
	movs	r3, #2
	mov	r0, r9
	str	r6, [sp, #0]
	bl	Func_080165d8
	movs	r0, #81
	bl	Func_080f9010
.L2:
	mov	r0, sl
	movs	r1, #250
	bl	Func_08003dec
	adds	r0, r7, #0
	movs	r1, #250
	bl	Func_08003dec
	movs	r0, #1
	bl	Func_080030f8
	bl	Func_080f9048
	cmp	r0, #0
	beq.n	.L1
	ldr	r3, [pc, #80]
	ldr	r2, [pc, #84]
	ldr	r3, [r3, #0]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L2
.L1:
	movs	r1, #2
	mov	r0, r9
	bl	Func_08016418
	movs	r0, #1
	bl	Func_080030f8
	ldr	r0, [sp, #20]
	bl	Func_08003f3c
	ldr	r0, [sp, #12]
	bl	Func_08003f3c
.L0:
	add	sp, #48
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
	.4byte 0x03001e8c
	.4byte 0x00000ea3
	.4byte 0x800c000c
	.4byte 0x802c000c
	.4byte 0x000012f4
	.4byte 0x000012f6
	.4byte 0x0000001d
	.4byte 0x03001c94
	.4byte 0x00000303
