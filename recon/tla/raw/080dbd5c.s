.syntax unified
	.thumb
	.set sub_08020358, 0x08020358
	.set sub_080dbb78, 0x080dbb78
	.global Func_080dbd5c
	.thumb_func
Func_080dbd5c:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	adds	r4, r0, #0
	movs	r0, #197
	lsls	r0, r0, #1
	adds	r3, r3, r0
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	sub	sp, #12
	cmp	r3, #3
	bne.n	.L_080dbd94
	mov	r0, sp
	movs	r3, #0
	str	r3, [r0, #4]
	str	r4, [r0, #0]
	str	r1, [r0, #8]
	bl	sub_08020358
	subs	r0, #2
	movs	r3, #0
	cmp	r0, #2
	bhi.n	.L_080dbd90
	movs	r3, #1
.L_080dbd90:
	adds	r0, r3, #0
	b.n	.L_080dbda4
.L_080dbd94:
	adds	r0, r4, #0
	bl	sub_080dbb78
	ldrb	r3, [r0, #3]
	movs	r0, #16
	ands	r0, r3
	lsls	r0, r0, #24
	lsrs	r0, r0, #24
.L_080dbda4:
	add	sp, #12
	pop	{pc}
	push	{lr}
	movs	r2, #2
	bl	sub_080dbb78
	ldrb	r3, [r0, #2]
	movs	r0, #1
	cmp	r3, #233
	beq.n	.L_080dbdc6
	movs	r0, #2
	cmp	r3, #234
	beq.n	.L_080dbdc6
	movs	r0, #3
	cmp	r3, #235
	beq.n	.L_080dbdc6
	movs	r0, #0
.L_080dbdc6:
	pop	{pc}
	push	{lr}
	bl	sub_080dbb78
	ldrb	r0, [r0, #2]
	pop	{pc}
	.align 2, 0
