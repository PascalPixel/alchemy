.syntax unified
	.thumb
	.set sub_081c0010, 0x081c0010
	.global Overlay_080d2d08
Overlay_080d2d08:
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r2, #230
	lsls	r2, r2, #1
	adds	r3, r3, r2
	adds	r5, r1, #0
	ldr	r4, [r3, #0]
	cmp	r0, #0
	beq.n	.L_080d2d6c
	cmp	r4, #0
	beq.n	.L_080d2d6c
	ldr	r1, [r4, #8]
	ldr	r3, [r0, #8]
	subs	r2, r1, r3
	cmp	r2, #0
	blt.n	.L_080d2d36
	movs	r3, #152
	lsls	r3, r3, #16
	cmp	r2, r3
	bgt.n	.L_080d2d6c
	b.n	.L_080d2d40
.L_080d2d36:
	movs	r2, #152
	subs	r3, r3, r1
	lsls	r2, r2, #16
	cmp	r3, r2
	bgt.n	.L_080d2d6c
.L_080d2d40:
	ldr	r2, [r4, #16]
	ldr	r3, [r4, #12]
	subs	r1, r2, r3
	ldr	r2, [r0, #16]
	ldr	r3, [r0, #12]
	subs	r0, r2, r3
	subs	r4, r1, r0
	cmp	r4, #0
	blt.n	.L_080d2d5c
	movs	r3, #224
	lsls	r3, r3, #15
	cmp	r4, r3
	bgt.n	.L_080d2d6c
	b.n	.L_080d2d66
.L_080d2d5c:
	movs	r2, #224
	subs	r3, r0, r1
	lsls	r2, r2, #15
	cmp	r3, r2
	bgt.n	.L_080d2d6c
.L_080d2d66:
	adds	r0, r5, #0
	bl	sub_081c0010
.L_080d2d6c:
	pop	{r5, pc}
