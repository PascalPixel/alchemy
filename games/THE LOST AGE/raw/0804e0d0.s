.syntax unified
	.thumb
	.set sub_08016ce4, 0x08016ce4
	.set sub_0803a7ac, 0x0803a7ac
	.set sub_080400e8, 0x080400e8
	.set sub_08040798, 0x08040798
	.set sub_08043690, 0x08043690
	.global Overlay_0804e0d0
Overlay_0804e0d0:
	push	{r5, r6, lr}
	movs	r5, #0
.L_0804e0d4:
	adds	r0, r5, #0
	bl	sub_08040798
	movs	r6, #1
	adds	r5, r0, #0
	negs	r6, r6
	adds	r0, r6, #0
	cmp	r5, r6
	beq.n	.L_0804e13a
	cmp	r5, #0
	bne.n	.L_0804e11a
	movs	r0, #190
	lsls	r0, r0, #1
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_0804e0fa
	ldr	r0, [pc, #68]
	b.n	.L_0804e108
.L_0804e0fa:
	movs	r0, #126
	adds	r0, #255
	bl	sub_08016ce4
	cmp	r0, #0
	beq.n	.L_0804e110
	ldr	r0, [pc, #56]
.L_0804e108:
	movs	r1, #1
	bl	sub_0803a7ac
	b.n	.L_0804e0d4
.L_0804e110:
	bl	sub_08043690
	cmp	r0, r6
	bne.n	.L_0804e138
	b.n	.L_0804e0d4
.L_0804e11a:
	cmp	r5, #1
	bne.n	.L_0804e12c
	ldr	r0, [pc, #36]
	movs	r1, #1
	bl	sub_0803a7ac
	ldr	r3, [pc, #32]
	strb	r5, [r3, #0]
	b.n	.L_0804e138
.L_0804e12c:
	cmp	r5, #2
	bne.n	.L_0804e138
	bl	sub_080400e8
	cmp	r0, r6
	beq.n	.L_0804e0d4
.L_0804e138:
	movs	r0, #0
.L_0804e13a:
	pop	{r5, r6, pc}
	.4byte 0x00000022
	.4byte 0x00000023
	.4byte 0x00001160
	.4byte 0x030011d0
