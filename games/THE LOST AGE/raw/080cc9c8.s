.syntax unified
	.thumb
	.set sub_080cc67c, 0x080cc67c
	.set sub_080cc7c4, 0x080cc7c4
	.global Overlay_080cc9c8
Overlay_080cc9c8:
	push	{r5, lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r5, [r3, #108]
	movs	r0, #1
	bl	sub_080cc67c
	movs	r2, #197
	lsls	r2, r2, #1
	adds	r3, r5, r2
	ldrb	r3, [r3, #0]
	lsls	r3, r3, #24
	asrs	r3, r3, #24
	cmp	r3, #2
	bne.n	.L_080cc9ee
	cmp	r0, #0
	bne.n	.L_080cc9f8
	bl	sub_080cc7c4
.L_080cc9ee:
	cmp	r0, #0
	bne.n	.L_080cc9f8
	movs	r0, #0
	bl	sub_080cc67c
.L_080cc9f8:
	pop	{r5, pc}
