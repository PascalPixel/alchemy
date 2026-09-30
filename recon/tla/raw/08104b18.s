.syntax unified
	.thumb
	.global Func_08104b18
	.thumb_func
Func_08104b18:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r8
	push	{r6, r7}
	sub	sp, #4
	mov	r8, r0
	mov	sl, r1
	adds	r7, r2, #0
	adds	r6, r3, #0
	bl	Resource_FindFreeEntry
	adds	r5, r0, #0
	cmp	r5, #96
	beq.n	.L_08104b4c
	adds	r1, r5, #0
	adds	r0, r6, #0
	bl	Func_08104acc
	movs	r1, #128
	lsls	r1, r1, #23
	adds	r0, r5, #0
	mov	r2, r8
	mov	r3, sl
	str	r7, [sp, #0]
	bl	RenderOutput_CreateFar
.L_08104b4c:
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, r7, pc}
	.2byte 0x0000
