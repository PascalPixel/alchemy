.syntax unified
	.thumb
	.set sub_080ed804, 0x080ed804
	.global Overlay_080c9e1c
Overlay_080c9e1c:
	push	{r5, lr}
	adds	r5, r0, #0
	bl	sub_080ed804
	ldrb	r3, [r0, #0]
	cmp	r3, #0
	beq.n	.L_080c9e42
.L_080c9e2a:
	ldrb	r3, [r0, #0]
	cmp	r3, r5
	bne.n	.L_080c9e3a
	ldrb	r3, [r0, #1]
	cmp	r3, #0
	beq.n	.L_080c9e3a
	adds	r0, r3, #0
	b.n	.L_080c9e44
.L_080c9e3a:
	adds	r0, #20
	ldrb	r3, [r0, #0]
	cmp	r3, #0
	bne.n	.L_080c9e2a
.L_080c9e42:
	movs	r0, #1
.L_080c9e44:
	pop	{r5, pc}
