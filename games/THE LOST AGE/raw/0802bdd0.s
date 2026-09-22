.syntax unified
	.thumb
	.set sub_0802b878, 0x0802b878
	.global Overlay_0802bdd0
Overlay_0802bdd0:
	push	{r5, r6, r7, lr}
	mov	r7, r8
	push	{r7}
	adds	r7, r2, #0
	adds	r2, r3, #0
	asrs	r2, r2, #3
	movs	r3, #192
	asrs	r7, r7, #3
	lsrs	r6, r2, #31
	lsls	r3, r3, #18
	ldr	r3, [r3, #32]
	adds	r6, r2, r6
	lsrs	r5, r7, #31
	asrs	r6, r6, #1
	asrs	r1, r1, #4
	adds	r5, r7, r5
	asrs	r5, r5, #1
	lsls	r4, r1, #5
	adds	r2, r6, #0
	movs	r1, #31
	mov	ip, r3
	ands	r2, r1
	adds	r3, r5, #0
	ands	r3, r1
	lsls	r2, r2, #5
	adds	r2, r2, r3
	movs	r3, #164
	asrs	r0, r0, #4
	lsls	r3, r3, #1
	lsls	r2, r2, #1
	adds	r4, r4, r0
	adds	r2, r2, r3
	mov	r3, ip
	sub	sp, #8
	strh	r4, [r3, r2]
	movs	r3, #1
	str	r3, [sp, #0]
	mov	r8, r3
	adds	r1, r5, #0
	adds	r2, r6, #0
	adds	r3, r4, #0
	movs	r0, #0
	str	r4, [sp, #4]
	bl	sub_0802b878
	ldr	r4, [sp, #4]
	movs	r3, #128
	lsls	r3, r3, #3
	adds	r4, r4, r3
	mov	r3, r8
	str	r3, [sp, #0]
	adds	r1, r5, #0
	adds	r2, r6, #0
	adds	r3, r4, #0
	movs	r0, #1
	bl	sub_0802b878
	add	sp, #8
	pop	{r3}
	mov	r8, r3
	pop	{r5, r6, r7, pc}
