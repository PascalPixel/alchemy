.syntax unified
	.thumb
	.set sub_080aed6c, 0x080aed6c
	.set sub_080af148, 0x080af148
	.set sub_080afe78, 0x080afe78
	.global Func_080aee40
	.thumb_func
Func_080aee40:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	sub	sp, #20
	mov	r5, sp
	mov	sl, r0
	adds	r0, r5, #0
	bl	sub_080afe78
	movs	r6, #0
	mov	r8, r0
	cmp	r6, r8
	bge.n	.L_080aee86
	mov	r9, r5
	movs	r7, #0
.L_080aee62:
	mov	r2, r9
	ldrsh	r5, [r7, r2]
	mov	r1, sl
	adds	r0, r5, #0
	bl	sub_080aed6c
	adds	r1, r0, #0
	cmp	r1, #0
	blt.n	.L_080aee7e
	adds	r0, r5, #0
	bl	sub_080af148
	adds	r0, r5, #0
	b.n	.L_080aee8a
.L_080aee7e:
	adds	r6, #1
	adds	r7, #2
	cmp	r6, r8
	blt.n	.L_080aee62
.L_080aee86:
	movs	r0, #1
	negs	r0, r0
.L_080aee8a:
	add	sp, #20
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
