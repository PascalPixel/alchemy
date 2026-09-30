.syntax unified
	.thumb
	.global Func_080f8bcc
	.thumb_func
Func_080f8bcc:
	push	{r5, r6, r7, lr}
	mov	r7, sl
	mov	r6, r9
	mov	r5, r8
	push	{r5, r6, r7}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #220
	ldr	r3, [r3, #0]
	sub	sp, #4
	adds	r5, r3, #0
	adds	r5, #76
	mov	r9, r0
	mov	sl, r1
	mov	r8, r2
	movs	r6, #0
	adds	r7, r5, #0
.L_080f8bee:
	ldmia	r7!, {r3}
	cmp	r3, #0
	beq.n	.L_080f8c04
	mov	r3, r8
	str	r3, [sp, #0]
	adds	r0, r5, #0
	adds	r1, r6, #0
	mov	r2, r9
	mov	r3, sl
	bl	PsynergyMenu_PositionOwnerEntry
.L_080f8c04:
	adds	r6, #1
	adds	r5, #4
	cmp	r6, #31
	ble.n	.L_080f8bee
	add	sp, #4
	pop	{r3, r5, r6}
	mov	r8, r3
	mov	r9, r5
	mov	sl, r6
	pop	{r5, r6, r7, pc}
