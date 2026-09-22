.syntax unified
	.thumb
	.set sub_0801314c, 0x0801314c
	.set sub_080143f8, 0x080143f8
	.set sub_08014d00, 0x08014d00
	.set sub_080383b0, 0x080383b0
	.global Overlay_08104acc
Overlay_08104acc:
	push	{r5, r6, lr}
	mov	r6, r8
	push	{r6}
	adds	r6, r1, #0
	movs	r1, #193
	mov	r8, r0
	lsls	r1, r1, #3
	movs	r0, #68
	sub	sp, #12
	bl	sub_08014d00
	movs	r1, #1
	add	r2, sp, #8
	add	r3, sp, #4
	str	r1, [sp, #0]
	adds	r5, r0, #0
	movs	r1, #0
	mov	r0, r8
	str	r6, [sp, #8]
	bl	sub_080383b0
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r5, r5, r3
	adds	r1, r5, #0
	adds	r0, r6, #0
	bl	sub_080143f8
	adds	r5, r0, #0
	movs	r0, #68
	bl	sub_0801314c
	adds	r0, r5, #0
	add	sp, #12
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, pc}
