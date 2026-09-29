.syntax unified
	.thumb
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	ldr	r3, [pc, #68]
	adds	r7, r2, #0
	adds	r6, r1, #0
	mov	r8, r3
	adds	r5, r0, #0
	bl	0x0804d0dc
	movs	r0, #5
	bl	0x0804d38c
	movs	r0, #6
	bl	0x0804d38c
	mov	r3, r8
	adds	r1, r6, #0
	movs	r2, #3
	adds	r0, r5, #0
	bl	Menu_LayoutResourceEntries
	adds	r0, r7, #0
	bl	0x0804d16c
	adds	r7, r0, #0
	bl	0x0804d118
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
	bl	0x0804d0dc
	movs	r0, #32
	bl	0x0804d38c
	movs	r0, #33
	bl	0x0804d38c
	movs	r1, #9
	movs	r2, #0
	movs	r0, #17
	bl	0x0804d3e8
	adds	r0, r5, #0
	bl	0x0804d16c
	adds	r5, r0, #0
	bl	0x0804d118
	adds	r0, r5, #0
	pop	{r5, pc}
	.2byte 0x0000
