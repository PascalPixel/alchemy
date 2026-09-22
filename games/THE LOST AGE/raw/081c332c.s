.syntax unified
	.thumb
	.set sub_08017880, 0x08017880
	.global Overlay_081c332c
Overlay_081c332c:
	push	{lr}
	ldr	r2, [pc, #12]
	ldr	r2, [r2, #0]
	bl	sub_08017880
	pop	{r0}
	bx	r0
	movs	r0, r0
	.4byte 0x02006800
