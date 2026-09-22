.syntax unified
	.thumb
	.global Overlay_08013ef8
Overlay_08013ef8:
	ldr	r2, [pc, #36]
	movs	r3, #1
	strb	r3, [r2, #0]
	ldr	r2, [pc, #36]
	ldr	r3, [pc, #24]
	ldr	r1, [pc, #36]
	strh	r3, [r2, #0]
	ldr	r2, [pc, #36]
	ldrb	r3, [r2, #0]
	strb	r3, [r1, #0]
	movs	r3, #16
	strb	r3, [r2, #0]
	ldr	r3, [pc, #28]
	ldr	r2, [pc, #32]
	strb	r0, [r3, #0]
	ldrb	r3, [r3, #0]
	strb	r3, [r2, #0]
	b.n	.L_08013f38
	.4byte 0x0000003e
	.4byte 0x030011dc
	.4byte 0x030011f4
	.4byte 0x0300113c
	.4byte 0x030011b0
	.4byte 0x03001178
	.2byte 0x110c
	.2byte 0x0300
.L_08013f38:
	bx	lr
