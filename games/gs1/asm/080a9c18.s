@ コード間隙関数の再構築サム逆アセンブル。範囲は
@ 制御フロー走査で確定。build_asm.tsでバイト一致確認済み。
.syntax unified
	.thumb
	.global Func_080a9c18
	.thumb_func
Func_080a9c18:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #80]
	ldr	r6, [r3, #0]
	adds	r5, r0, #0
	adds	r7, r6, #0
	sub	sp, #4
	bl	Func_080a9cbc
	movs	r3, #14
	movs	r1, #216
	adds	r7, #72
	adds	r6, r5, #0
	mov	r8, r3
.L9:
	ldrh	r2, [r6, #0]
	adds	r3, r2, #0
	adds	r6, #2
	cmp	r3, #0
	beq.n	.L0
	ldr	r3, [pc, #36]
	ands	r3, r2
	cmp	r3, #0
	beq.n	.L0
	ldr	r5, [r7, #0]
	cmp	r5, #0
	beq.n	.L0
	ldr	r0, [pc, #28]
	ands	r0, r2
	str	r1, [sp, #0]
	bl	Func_08077018
	ldrb	r3, [r0, #2]
	ldr	r1, [sp, #0]
	cmp	r3, #2
	beq.n	.L1
	cmp	r3, #2
	bgt.n	.L2
	b.n	.L3
	movs	r0, r0
	.4byte 0x00000200
	.4byte 0x000001ff
	.4byte 0x03001f2c
.L3:
	cmp	r3, #1
	beq.n	.L4
	b.n	.L5
.L2:
	cmp	r3, #3
	beq.n	.L6
	cmp	r3, #4
	beq.n	.L7
	b.n	.L5
.L4:
	movs	r3, #32
	b.n	.L8
.L1:
	movs	r3, #80
	b.n	.L8
.L6:
	movs	r3, #64
	b.n	.L8
.L7:
	movs	r3, #48
.L8:
	strh	r1, [r5, #6]
	strh	r3, [r5, #8]
.L5:
	adds	r0, r5, #0
	str	r1, [sp, #0]
	bl	Func_080a17c4
	ldr	r1, [sp, #0]
.L0:
	movs	r3, #1
	negs	r3, r3
	add	r8, r3
	mov	r3, r8
	adds	r7, #4
	cmp	r3, #0
	bge.n	.L9
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7}
	pop	{r0}
	bx	r0
