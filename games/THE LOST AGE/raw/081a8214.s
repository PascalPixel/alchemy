.syntax unified
	.thumb
	.global Overlay_081a8214
Overlay_081a8214:
	push	{lr}
	movs	r3, #192
	lsls	r3, r3, #18
	adds	r3, #128
	ldr	r3, [r3, #0]
	cmp	r3, #0
	beq.n	.L_081a8224
	strh	r0, [r3, #0]
.L_081a8224:
	pop	{pc}
