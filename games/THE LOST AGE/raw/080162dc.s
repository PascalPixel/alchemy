.syntax unified
	.thumb
	.global Overlay_080162dc
Overlay_080162dc:
	push	{lr}
	ldr	r2, [pc, #12]
	ldrb	r3, [r2, #0]
	cmp	r3, #0
	beq.n	.L_080162ea
	movs	r3, #1
	strb	r3, [r2, #8]
.L_080162ea:
	pop	{pc}
	.4byte 0x02005360
