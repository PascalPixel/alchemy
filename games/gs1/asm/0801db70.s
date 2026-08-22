@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_0801db70
	.thumb_func
Func_0801db70:
	push	{r5, r6, r7, lr}
	mov	r7, fp
	mov	r6, sl
	mov	r5, r9
	push	{r5, r6, r7}
	mov	r7, r8
	push	{r7}
	sub	sp, #20
	movs	r0, #1
	str	r0, [sp, #12]
	movs	r0, #191
	movs	r2, #3
	lsls	r0, r0, #1
	mov	fp, r2
	bl	Func_080770c0
	movs	r3, #0
	str	r3, [sp, #4]
	adds	r5, r0, #0
	bl	Func_0801d980
	ldr	r3, [pc, #368]
	ldr	r3, [r3, #0]
	mov	sl, r3
	bl	Func_0801d9d4
	ldr	r6, [pc, #360]
	str	r0, [sp, #16]
	ldrb	r3, [r6, #0]
	movs	r4, #24
	negs	r4, r4
	cmp	r3, #0
	beq.n	.L0
	adds	r4, #8
.L0:
	adds	r3, r4, #0
	ldr	r1, [sp, #16]
	movs	r2, #40
	movs	r0, #6
	bl	Func_08021620
	str	r0, [sp, #8]
	movs	r0, #1
	bl	Func_080030f8
	ldr	r3, [pc, #328]
	add	r3, sl
	ldrh	r3, [r3, #0]
	mov	r8, r3
	cmp	r5, #0
	beq.n	.L1
	movs	r2, #2
	movs	r0, #1
	str	r2, [sp, #4]
	mov	fp, r0
.L1:
	ldrb	r3, [r6, #0]
	cmp	r3, #0
	beq.n	.L2
	movs	r3, #3
	add	fp, r3
.L2:
	ldr	r0, [sp, #12]
	cmp	r0, #0
	beq.n	.L3
	mov	r0, r8
	movs	r2, #0
	add	r0, fp
	mov	r1, fp
	str	r2, [sp, #12]
	bl	Func_080022fc
	ldr	r3, [pc, #280]
	mov	r8, r0
	add	r3, sl
	mov	r0, r8
	strh	r0, [r3, #0]
	ldr	r2, [sp, #12]
	movs	r4, #0
	cmp	r2, fp
	bge.n	.L4
	ldr	r3, [pc, #264]
	movs	r7, #194
	lsls	r7, r7, #3
	ldr	r6, [sp, #4]
	mov	r9, r3
	add	r7, sl
.L6:
	ldmia	r7!, {r5}
	movs	r3, #251
	strb	r3, [r5, #15]
	adds	r0, r5, #0
	str	r4, [sp, #0]
	bl	Func_080a1038
	ldr	r3, [pc, #236]
	add	r3, sl
	ldrh	r3, [r3, #0]
	ldr	r4, [sp, #0]
	ldrb	r1, [r5, #14]
	movs	r2, #0
	cmp	r4, r3
	beq.n	.L5
	movs	r2, #1
.L5:
	mov	r3, r9
	ldrsb	r0, [r6, r3]
	str	r4, [sp, #0]
	bl	Func_080216e8
	ldr	r4, [sp, #0]
	adds	r4, #1
	adds	r6, #1
	cmp	r4, fp
	blt.n	.L6
.L4:
	ldr	r0, [sp, #16]
	ldrh	r2, [r0, #14]
	ldrh	r1, [r0, #12]
	mov	r0, r8
	lsls	r3, r0, #1
	add	r3, r8
	adds	r3, r3, r2
	lsls	r3, r3, #3
	adds	r4, r3, #0
	ldr	r0, [pc, #188]
	adds	r4, #16
	lsls	r1, r1, #3
	add	r0, sl
	adds	r2, r4, #0
	movs	r3, #3
	bl	Func_080b0030
.L3:
	ldr	r0, [sp, #8]
	bl	Func_080216b4
	movs	r0, #1
	bl	Func_080030f8
	ldr	r1, [pc, #164]
	ldr	r2, [r1, #0]
	movs	r3, #1
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L7
	movs	r0, #112
	mov	r5, r8
	bl	Func_080f9010
	b.n	.L8
.L7:
	ldr	r2, [r1, #0]
	movs	r3, #10
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L9
	movs	r5, #1
	movs	r0, #113
	negs	r5, r5
	bl	Func_080f9010
	b.n	.L8
.L9:
	ldr	r1, [pc, #124]
	ldr	r2, [r1, #0]
	movs	r3, #64
	ands	r2, r3
	cmp	r2, #0
	beq.n	.L10
	movs	r0, #111
	bl	Func_080f9010
	movs	r2, #1
	negs	r2, r2
	movs	r3, #1
	add	r8, r2
	str	r3, [sp, #12]
	b.n	.L2
.L10:
	ldr	r3, [r1, #0]
	movs	r2, #128
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L2
	movs	r0, #111
	bl	Func_080f9010
	movs	r0, #1
	add	r8, r0
	str	r0, [sp, #12]
	b.n	.L2
.L8:
	ldr	r0, [sp, #16]
	movs	r1, #2
	bl	Func_08016418
	bl	Func_0801d9bc
	movs	r0, #1
	bl	Func_080030f8
	cmp	r5, #0
	blt.n	.L11
	ldr	r2, [sp, #4]
	adds	r5, r5, r2
.L11:
	adds	r0, r5, #0
	add	sp, #20
	pop	{r3, r5, r6, r7}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	mov	fp, r7
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	movs	r0, r0
	.4byte 0x03001ea0
	.4byte 0x03001f54
	.4byte 0x00000574
	.4byte 0x080367dc
	.4byte 0x000005a4
	.4byte 0x03001c94
	.4byte 0x03001b04
