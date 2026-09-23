.syntax unified
	.thumb
	.global Func_080a1f74
	.thumb_func
Func_080a1f74:
	push	{lr}
	ldr	r2, [pc, #76]
	cmp	r0, #1
	beq.n	.L_080a1f90
	cmp	r0, #1
	bgt.n	.L_080a1f86
	cmp	r0, #0
	beq.n	.L_080a1f8c
	b.n	.L_080a1f96
.L_080a1f86:
	cmp	r0, #2
	beq.n	.L_080a1f94
	b.n	.L_080a1f96
.L_080a1f8c:
	ldr	r2, [pc, #56]
	b.n	.L_080a1f96
.L_080a1f90:
	ldr	r2, [pc, #56]
	b.n	.L_080a1f96
.L_080a1f94:
	ldr	r2, [pc, #56]
.L_080a1f96:
	ldrb	r3, [r2, #0]
	movs	r4, #255
	strb	r3, [r1, #0]
	lsls	r4, r4, #24
	lsls	r3, r3, #24
	movs	r0, #0
	cmp	r3, r4
	beq.n	.L_080a1fbe
.L_080a1fa6:
	adds	r0, #1
	cmp	r0, #31
	bgt.n	.L_080a1fbe
	adds	r2, #1
	ldrb	r3, [r2, #0]
	adds	r1, #1
	movs	r4, #255
	strb	r3, [r1, #0]
	lsls	r4, r4, #24
	lsls	r3, r3, #24
	cmp	r3, r4
	bne.n	.L_080a1fa6
.L_080a1fbe:
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x080af2a6
	.4byte 0x080af2d0
	.4byte 0x080af2bc
	.4byte 0x080af2b1
