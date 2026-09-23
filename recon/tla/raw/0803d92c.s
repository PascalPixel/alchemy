.syntax unified
	.thumb
	.set sub_0803d9bc, 0x0803d9bc
	.set sub_0803db54, 0x0803db54
	.set sub_080ad010, 0x080ad010
	.set sub_080ad078, 0x080ad078
	.global Func_0803d92c
	.thumb_func
Func_0803d92c:
	push	{r5, r6, lr}
	adds	r5, r0, #0
	movs	r0, #128
	lsls	r0, r0, #1
	movs	r3, #192
	adds	r0, #255
	lsls	r3, r3, #18
	ands	r0, r5
	ldr	r6, [r3, #68]
	bl	sub_080ad010
	cmp	r5, #0
	beq.n	.L_0803d95a
	movs	r2, #192
	lsls	r2, r2, #3
	adds	r2, #4
	ldrh	r3, [r0, #6]
	adds	r1, r6, r2
	ldr	r2, [pc, #52]
	lsls	r3, r3, #2
	ldr	r3, [r2, r3]
	str	r3, [r1, #0]
	b.n	.L_0803d968
.L_0803d95a:
	ldr	r2, [pc, #44]
	movs	r1, #192
	lsls	r1, r1, #3
	ldr	r2, [r2, #0]
	adds	r1, #4
	adds	r3, r6, r1
	str	r2, [r3, #0]
.L_0803d968:
	movs	r2, #192
	movs	r1, #192
	lsls	r2, r2, #3
	lsls	r1, r1, #3
	adds	r3, r6, r2
	adds	r1, #2
	movs	r2, #2
	strh	r2, [r3, #0]
	adds	r3, r6, r1
	strh	r2, [r3, #0]
	adds	r0, r6, #0
	movs	r1, #0
	bl	sub_0803db54
	pop	{r5, r6, pc}
	movs	r0, r0
	.2byte 0xeb58
	.2byte 0x0804
	push	{r5, r6, lr}
	mov	r6, sl
	mov	r5, r8
	push	{r5, r6}
	sub	sp, #4
	ldr	r5, [sp, #24]
	adds	r6, r1, #0
	mov	r8, r2
	mov	sl, r3
	bl	sub_080ad078
	adds	r1, r6, #0
	ldrh	r0, [r0, #4]
	mov	r2, r8
	mov	r3, sl
	str	r5, [sp, #0]
	bl	sub_0803d9bc
	add	sp, #4
	pop	{r3, r5}
	mov	r8, r3
	mov	sl, r5
	pop	{r5, r6, pc}
	.align 2, 0
