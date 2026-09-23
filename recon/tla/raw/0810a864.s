.syntax unified
	.thumb
	.set sub_08016ca4, 0x08016ca4
	.global Func_0810a864
	.thumb_func
Func_0810a864:
	push	{r5, lr}
	adds	r5, r1, #0
	bl	sub_08016ca4
	ldrb	r2, [r0, #15]
	movs	r0, #0
	cmp	r5, #0
	bne.n	.L_0810a87c
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r0, r3, #2
	b.n	.L_0810a896
.L_0810a87c:
	cmp	r5, #1
	bne.n	.L_0810a884
	movs	r0, #10
	b.n	.L_0810a896
.L_0810a884:
	cmp	r5, #2
	bne.n	.L_0810a88c
	movs	r0, #50
	b.n	.L_0810a896
.L_0810a88c:
	cmp	r5, #3
	bne.n	.L_0810a896
	lsls	r3, r2, #2
	adds	r3, r3, r2
	lsls	r0, r3, #1
.L_0810a896:
	pop	{r5, pc}
.L_0810a898:
	push	{r5, lr}
	adds	r5, r1, #0
	bl	sub_08016ca4
	adds	r2, r0, #0
	movs	r0, #0
	cmp	r5, #0
	bne.n	.L_0810a8b0
	movs	r1, #56
	ldrsh	r3, [r2, r1]
	cmp	r3, #0
	ble.n	.L_0810a8e8
.L_0810a8b0:
	cmp	r5, #1
	bne.n	.L_0810a8c4
	movs	r1, #50
	adds	r1, #255
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	bne.n	.L_0810a8e8
.L_0810a8c4:
	cmp	r5, #2
	bne.n	.L_0810a8d4
	movs	r1, #160
	lsls	r1, r1, #1
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	cmp	r3, #0
	bne.n	.L_0810a8e8
.L_0810a8d4:
	cmp	r5, #3
	bne.n	.L_0810a8ea
	movs	r1, #152
	lsls	r1, r1, #1
	adds	r3, r2, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #0
	beq.n	.L_0810a8ea
.L_0810a8e8:
	movs	r0, #1
.L_0810a8ea:
	pop	{r5, pc}
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r5, [r3, #0]
	movs	r1, #129
	lsls	r1, r1, #3
	adds	r1, #255
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	subs	r1, #3
	mov	sl, r3
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	movs	r2, #0
	sub	sp, #4
	movs	r7, #0
	cmp	r2, r3
	bge.n	.L_0810a952
	adds	r3, r5, #2
	movs	r6, #153
	mov	r8, r3
	lsls	r6, r6, #3
.L_0810a928:
	mov	r1, r8
	ldrsh	r0, [r1, r6]
	mov	r1, sl
	str	r2, [sp, #0]
	bl	.L_0810a898
	ldr	r2, [sp, #0]
	cmp	r0, #0
	beq.n	.L_0810a93c
	adds	r2, #1
.L_0810a93c:
	movs	r1, #160
	lsls	r1, r1, #3
	adds	r1, #4
	adds	r3, r5, r1
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	adds	r7, #1
	adds	r6, #2
	cmp	r7, r3
	blt.n	.L_0810a928
.L_0810a952:
	adds	r0, r2, #0
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.align 2, 0
