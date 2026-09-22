.syntax unified
	.thumb
	.global Overlay_080cad64
Overlay_080cad64:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	ldr	r3, [r3, #108]
	movs	r0, #168
	lsls	r0, r0, #1
	movs	r1, #0
	movs	r2, #79
	adds	r3, r3, r0
.L_080cad76:
	subs	r2, #1
	str	r1, [r3, #0]
	subs	r3, #4
	cmp	r2, #0
	bge.n	.L_080cad76
	pop	{pc}
