.syntax unified
	.thumb
	.set sub_080072e8, 0x080072e8
	.global Overlay_080fa678
Overlay_080fa678:
	push	{lr}
	ldr	r1, [pc, #12]
	ldr	r1, [r1, #0]
	bl	sub_080072e8
	pop	{r0}
	bx	r0
	movs	r0, r0
	.2byte 0x4088
	.2byte 0x0200
	push	{lr}
	ldr	r1, [pc, #12]
	ldr	r1, [r1, #0]
	bl	sub_080072e8
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x0200408c
