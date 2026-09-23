.syntax unified
	.thumb
	.set sub_0803a8fc, 0x0803a8fc
	.global Func_0803c170
	.thumb_func
Func_0803c170:
	push	{r5, r6, lr}
	sub	sp, #128
	mov	r5, sp
	adds	r6, r1, #0
	adds	r1, r5, #0
	bl	sub_0803a8fc
	movs	r3, #128
	movs	r2, #132
	lsls	r3, r3, #19
	lsls	r2, r2, #24
	adds	r3, #212
	adds	r0, r5, #0
	adds	r1, r6, #0
	adds	r2, #32
	stmia	r3!, {r0, r1, r2}
	subs	r3, #12
	add	sp, #128
	pop	{r5, r6, pc}
	.2byte 0x0000
