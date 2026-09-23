.syntax unified
	.thumb
	.set sub_0804d0dc, 0x0804d0dc
	.set sub_0804d118, 0x0804d118
	.set sub_0804d16c, 0x0804d16c
	.set sub_0804d38c, 0x0804d38c
	.set sub_0804d3e8, 0x0804d3e8
	.set sub_080ad250, 0x080ad250
	.global Menu_SelectTopEntry
	.global Func_0804d4f8
	.thumb_func
Menu_SelectTopEntry:
Func_0804d4f8:
	push	{r5, r6, r7, lr}
	adds	r5, r0, #0
	movs	r0, #1
	negs	r0, r0
	movs	r6, #0
	bl	sub_080ad250
	cmp	r0, #0
	bne.n	.L_0804d50c
	movs	r6, #1
.L_0804d50c:
	lsls	r3, r6, #1
	adds	r3, r3, r6
	ldr	r2, [pc, #84]
	lsls	r7, r3, #1
	adds	r3, r5, r7
	ldrsb	r3, [r2, r3]
	subs	r5, r3, #1
	cmp	r5, #0
	bge.n	.L_0804d520
	movs	r5, #0
.L_0804d520:
	bl	sub_0804d0dc
	movs	r0, #1
	bl	sub_0804d38c
	cmp	r6, #0
	bne.n	.L_0804d534
	movs	r0, #15
	bl	sub_0804d38c
.L_0804d534:
	movs	r0, #2
	bl	sub_0804d38c
	movs	r0, #7
	bl	sub_0804d38c
	movs	r0, #17
	movs	r1, #7
	movs	r2, #0
	bl	sub_0804d3e8
	adds	r0, r5, #0
	bl	sub_0804d16c
	adds	r5, r0, #0
	bl	sub_0804d118
	cmp	r5, #0
	blt.n	.L_0804d562
	ldr	r2, [pc, #16]
	adds	r3, r5, r7
	adds	r3, #1
	ldrsb	r5, [r2, r3]
.L_0804d562:
	adds	r0, r5, #0
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.4byte 0x0805f8b3
	.4byte 0x0805f8a7
