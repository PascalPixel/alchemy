.syntax unified
	.thumb
	.set sub_08016ca4, 0x08016ca4
	.global Overlay_080fad1c
Overlay_080fad1c:
	push	{r5, lr}
	bl	sub_08016ca4
	movs	r4, #128
	lsls	r4, r4, #1
	movs	r5, #0
	adds	r4, #255
	adds	r0, #216
	movs	r1, #14
.L_080fad2e:
	ldrh	r2, [r0, #0]
	adds	r3, r4, #0
	ands	r3, r2
	adds	r0, #2
	cmp	r3, #0
	beq.n	.L_080fad3c
	adds	r5, #1
.L_080fad3c:
	subs	r1, #1
	cmp	r1, #0
	bge.n	.L_080fad2e
	adds	r0, r5, #0
	pop	{r5, pc}
