@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080ba584
	.thumb_func
Func_080ba584:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #276]
	adds	r6, r0, #0
	ldrb	r0, [r6, #0]
	sub	sp, #84
	mov	r8, r1
	ldr	r5, [r3, #0]
	bl	Func_080b7dd0
	ldr	r3, [r0, #0]
	ldr	r1, [r3, #16]
	ldr	r0, [r3, #8]
	bl	Func_080044d0
	ldr	r1, [pc, #252]
	lsls	r0, r0, #16
	ldrb	r3, [r6, #0]
	lsrs	r0, r0, #16
	adds	r2, r0, r1
	cmp	r3, #7
	bls.n	.L0
	movs	r3, #192
	lsls	r3, r3, #7
	adds	r2, r0, r3
.L0:
	ldr	r3, [pc, #236]
	ldr	r1, [pc, #232]
	ands	r2, r3
	adds	r3, r2, r1
	lsrs	r2, r3, #31
	adds	r3, r3, r2
	movs	r1, #128
	asrs	r3, r3, #1
	lsls	r1, r1, #6
	adds	r2, r3, r1
	ldr	r3, [r5, #0]
	cmp	r3, r2
	bne.n	.L1
	str	r2, [r5, #0]
	movs	r0, #5
	bl	Func_080030f8
	b.n	.L2
.L1:
	str	r2, [r5, #0]
	movs	r0, #20
	bl	Func_080030f8
.L2:
	movs	r0, #0
	movs	r1, #0
	bl	Func_080c10e8
	mov	r5, sp
	adds	r1, r5, #0
	adds	r0, r6, #0
	bl	Func_080b9d34
	ldr	r0, [r5, #8]
	bl	Func_08077008
	ldrb	r0, [r6, #2]
	bl	Func_08077008
	movs	r7, #2
	mov	r2, r8
	ldrb	r0, [r6, #0]
	ands	r7, r2
	bl	Func_080b7dd0
	movs	r1, #0
	ldr	r0, [r0, #0]
	bl	Func_080b7f70
	ldr	r3, [r0, #40]
	movs	r2, #1
	movs	r1, #0
	ldrsh	r0, [r3, r1]
	movs	r1, #2
	bl	Func_08009260
	movs	r3, #0
	adds	r2, r0, #0
	ldrb	r1, [r6, #2]
	ldr	r0, [r5, #8]
	bl	Func_080b82c4
	ldr	r0, [r5, #8]
	bl	Func_080b7dd0
	movs	r1, #16
	ldr	r0, [r0, #0]
	bl	Func_08009088
	ldrb	r0, [r6, #2]
	bl	Func_080b7dd0
	ldrb	r3, [r6, #2]
	cmp	r3, #7
	bhi.n	.L3
	movs	r3, #1
	b.n	.L4
.L3:
	movs	r3, #0
.L4:
	str	r3, [r5, #4]
	cmp	r7, #0
	beq.n	.L5
	movs	r0, #10
	bl	Func_080030f8
	ldrb	r0, [r6, #2]
	bl	Func_080b8178
	movs	r0, #2
	bl	Func_080030f8
	movs	r0, #4
	bl	Func_080030f8
	movs	r0, #10
	bl	Func_080030f8
	ldrb	r0, [r6, #2]
	bl	Func_080b8000
	b.n	.L6
.L5:
	adds	r0, r5, #0
	bl	Func_080c9008
	bl	Func_080bb938
	ldrb	r0, [r6, #2]
	bl	Func_080b8000
.L6:
	ldr	r0, [r5, #8]
	bl	Func_080b8000
	movs	r0, #0
	add	sp, #84
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r1}
	bx	r1
	.4byte 0x03001f00
	.4byte 0xffffe000
	.4byte 0x00007fff
