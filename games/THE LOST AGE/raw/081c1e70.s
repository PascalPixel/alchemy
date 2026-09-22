.syntax unified
	.thumb
	.set sub_081c1e48, 0x081c1e48
	.set sub_081c1e64, 0x081c1e64
	.global Overlay_081c1e70
Overlay_081c1e70:
	mov	ip, lr
	bl	sub_081c1e64
	strb	r3, [r1, #25]
	cmp	r3, #0
	bne.n	.L_081c1e80
	bl	sub_081c1e48
.L_081c1e80:
	bx	ip
