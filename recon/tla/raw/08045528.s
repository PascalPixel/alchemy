.syntax unified
	.thumb
	.set sub_08013164, 0x08013164
	.set sub_08014d78, 0x08014d78
	.set sub_0803d2f0, 0x0803d2f0
	.set sub_0803dab0, 0x0803dab0
	.global Func_08045528
	.thumb_func
Func_08045528:
	push	{lr}
	sub	sp, #16
	str	r1, [sp, #12]
	cmp	r0, #7
	bls.n	.L_08045534
	movs	r0, #0
.L_08045534:
	bl	sub_0803d2f0
	movs	r1, #14
	str	r1, [sp, #0]
	movs	r1, #1
	add	r2, sp, #12
	add	r3, sp, #8
	str	r1, [sp, #4]
	movs	r1, #0
	bl	sub_0803dab0
	ldr	r0, [sp, #8]
	add	sp, #16
	pop	{pc}
	push	{lr}
	cmp	r0, #0
	beq.n	.L_08045558
	movs	r0, #0
.L_08045558:
	ldr	r3, [pc, #4]
	lsls	r2, r0, #2
	ldr	r0, [r3, r2]
	pop	{pc}
	.2byte 0xa3dc
	.2byte 0x080a
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	mov	r8, r0
	ldr	r5, [pc, #56]
	adds	r0, r5, #0
	bl	sub_08014d78
	movs	r2, #132
	movs	r3, #128
	adds	r6, r0, #0
	lsrs	r5, r5, #2
	lsls	r2, r2, #24
	lsls	r3, r3, #19
	adds	r3, #212
	ldr	r0, [pc, #40]
	adds	r1, r6, #0
	orrs	r2, r5
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	movs	r3, #192
	lsls	r3, r3, #3
	adds	r3, #4
	add	r3, r8
	ldr	r0, [r3, #0]
	mov	r1, r8
	mov	lr, r6
	.2byte 0xf800
	.2byte 0x1c30
	bl	sub_08013164
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
	.4byte 0x0000027c
	.4byte 0x08038b14
