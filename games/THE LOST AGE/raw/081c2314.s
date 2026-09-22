.syntax unified
	.thumb
	.set sub_0801787c, 0x0801787c
	.global Overlay_081c2314
Overlay_081c2314:
	push	{lr}
	ldr	r1, [pc, #12]
	ldr	r1, [r1, #0]
	bl	sub_0801787c
	pop	{r0}
	bx	r0
	movs	r0, r0
	.2byte 0x6888
	.2byte 0x0200
	push	{lr}
	ldr	r1, [pc, #12]
	ldr	r1, [r1, #0]
	bl	sub_0801787c
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x0200688c
