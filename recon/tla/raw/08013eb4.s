.syntax unified
	.thumb
	.global Func_08013eb4
	.thumb_func
Func_08013eb4:
	ldr	r3, [pc, #36]
	movs	r4, #0
	strb	r4, [r3, #0]
	ldr	r2, [pc, #36]
	ldr	r3, [pc, #24]
	ldr	r1, [pc, #36]
	strh	r3, [r2, #0]
	ldr	r2, [pc, #36]
	ldrb	r3, [r2, #0]
	strb	r3, [r1, #0]
	ldr	r3, [pc, #32]
	strb	r4, [r2, #0]
	strb	r0, [r3, #0]
	ldr	r2, [pc, #32]
	ldrb	r3, [r3, #0]
	strb	r3, [r2, #0]
	b.n	.L_08013ef4
	movs	r0, r0
	.4byte 0x0000003e
	.4byte 0x030011dc
	.4byte 0x030011f4
	.4byte 0x0300113c
	.4byte 0x030011b0
	.4byte 0x03001178
	.2byte 0x110c
	.2byte 0x0300
.L_08013ef4:
	bx	lr
	.align 2, 0
