.syntax unified
	.thumb
	.set sub_08005268, 0x08005268
	.set sub_080b7dd0, 0x080b7dd0
	.set sub_080b7ed8, 0x080b7ed8
	.set sub_080b7f70, 0x080b7f70
	.set sub_080b8530, 0x080b8530
	.global BattleMotion_ProjectScaledPosition
	.global Func_080b845c
	.thumb_func
BattleMotion_ProjectScaledPosition:
Func_080b845c:
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	sub	sp, #12
	mov	r8, r1
	mov	sl, r0
	bl	sub_080b7dd0
	ldr	r5, [r0, #0]
	movs	r1, #0
	adds	r0, r5, #0
	bl	sub_080b7f70
	adds	r5, #8
	adds	r6, r0, #0
	bl	sub_080b7ed8
	mov	r1, r8
	adds	r0, r5, #0
	bl	sub_08005268
	ldr	r5, [pc, #48]
	ldr	r1, [r6, #24]
	mov	ip, pc
	bx	r5
	adds	r6, r0, #0
	mov	r0, sl
	bl	sub_080b8530
	adds	r1, r0, #0
	asrs	r1, r1, #16
	adds	r0, r6, #0
	movs	r0, r0
	mov	ip, pc
	bx	r5
	mov	r2, r8
	ldr	r3, [r2, #4]
	subs	r3, r3, r0
	str	r3, [r2, #4]
	movs	r0, #0
	add	sp, #12
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6}
	pop	{r1}
	bx	r1
	.4byte 0x03000118
