.syntax unified
	.thumb
	.set sub_080f8888, 0x080f8888
	.global Func_080facd8
	.thumb_func
Func_080facd8:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r7, [r3, #0]
	movs	r1, #13
	sub	sp, #4
	movs	r6, #76
	adds	r5, r0, #0
	mov	r8, r1
	movs	r2, #31
.L_080facf2:
	ldrh	r3, [r5, #0]
	adds	r5, #2
	cmp	r3, #0
	bne.n	.L_080fad0a
	ldr	r0, [r6, r7]
	str	r2, [sp, #0]
	bl	sub_080f8888
	ldr	r3, [r6, r7]
	mov	r1, r8
	strb	r1, [r3, #5]
	ldr	r2, [sp, #0]
.L_080fad0a:
	subs	r2, #1
	adds	r6, #4
	cmp	r2, #0
	bge.n	.L_080facf2
	add	sp, #4
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
