.syntax unified
	.thumb
	.set sub_0804d0dc, 0x0804d0dc
	.set sub_0804d118, 0x0804d118
	.set sub_0804d16c, 0x0804d16c
	.set sub_0804d38c, 0x0804d38c
	.set sub_0804d3e8, 0x0804d3e8
	.set sub_0804d484, 0x0804d484
	.global Func_0804d9e0
	.thumb_func
Func_0804d9e0:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	adds	r5, r2, #0
	adds	r6, r3, #0
	movs	r3, #0
	adds	r7, r0, #0
	mov	sl, r1
	mov	r8, r3
	bl	sub_0804d0dc
	cmp	r5, #0
	bne.n	.L_0804d9fe
	movs	r5, #3
.L_0804d9fe:
	cmp	r7, #0
	beq.n	.L_0804da06
	movs	r3, #17
	mov	r8, r3
.L_0804da06:
	movs	r0, #5
	bl	sub_0804d38c
	movs	r0, #6
	bl	sub_0804d38c
	adds	r1, r5, #0
	mov	r2, sl
	mov	r0, r8
	bl	sub_0804d3e8
	adds	r0, r6, #0
	bl	sub_0804d16c
	adds	r6, r0, #0
	bl	sub_0804d118
	movs	r3, #1
	negs	r3, r3
	cmp	r6, r3
	bne.n	.L_0804da32
	movs	r6, #1
.L_0804da32:
	adds	r0, r6, #0
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #68]
	adds	r7, r2, #0
	adds	r6, r1, #0
	mov	r8, r3
	adds	r5, r0, #0
	bl	sub_0804d0dc
	movs	r0, #5
	bl	sub_0804d38c
	movs	r0, #6
	bl	sub_0804d38c
	mov	r3, r8
	adds	r1, r6, #0
	movs	r2, #3
	adds	r0, r5, #0
	bl	sub_0804d484
	adds	r0, r7, #0
	bl	sub_0804d16c
	adds	r7, r0, #0
	bl	sub_0804d118
	movs	r3, #1
	negs	r3, r3
	cmp	r7, r3
	bne.n	.L_0804da7e
	movs	r7, #1
.L_0804da7e:
	adds	r0, r7, #0
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	movs	r0, r0
	.2byte 0x003f
	.2byte 0x0000
	push	{r5, lr}
	adds	r5, r0, #0
	bl	sub_0804d0dc
	movs	r0, #32
	bl	sub_0804d38c
	movs	r0, #33
	bl	sub_0804d38c
	movs	r1, #9
	movs	r2, #0
	movs	r0, #17
	bl	sub_0804d3e8
	adds	r0, r5, #0
	bl	sub_0804d16c
	adds	r5, r0, #0
	bl	sub_0804d118
	adds	r0, r5, #0
	pop	{r5, pc}
	.align 2, 0
