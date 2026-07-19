.syntax unified
.text
	.thumb
	.global Fragment_080a2408
	.thumb_func
Fragment_080a2408:
	ldr	r3, [pc, #12]
	ldr	r2, [pc, #16]
	ldr	r3, [r3, #0]
	adds	r3, r3, r2
	movs	r2, #1
	strb	r2, [r3, #0]
	bx	lr
	.inst.n 0x0000
	.4byte 0x03001e8c
	.4byte 0x00000ea6
	.inst.n 0x4b03
	.inst.n 0x4a04
	.inst.n 0x681b
	.inst.n 0x189b
	.inst.n 0x2200
	.inst.n 0x701a
	.inst.n 0x4770
	.inst.n 0x0000
	.inst.n 0x1e8c
	.inst.n 0x0300
	.inst.n 0x0ea6
	.inst.n 0x0000
