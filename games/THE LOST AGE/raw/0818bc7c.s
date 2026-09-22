.syntax unified
	.thumb
	.set sub_0818bc98, 0x0818bc98
	.global Overlay_0818bc7c
Overlay_0818bc7c:
	push	{lr}
	movs	r3, #2
	movs	r1, #2
	str	r3, [r0, #24]
	bl	sub_0818bc98
	pop	{pc}
