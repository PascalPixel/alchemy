.syntax unified
.text
	.thumb
	.global Fragment_080118a8
	.thumb_func
Fragment_080118a8:
	ldr	r3, [pc, #16]
	ldr	r2, [r3, #0]
	lsls	r3, r0, #1
	adds	r3, r3, r0
	lsls	r3, r3, #2
	adds	r2, r2, r3
	movs	r3, #0
	strh	r3, [r2, #34]
	bx	lr
	.inst.n 0x0000
	.4byte 0x03001e70
	.inst.n 0x4b04
	.inst.n 0x681a
	.inst.n 0x0043
	.inst.n 0x181b
	.inst.n 0x009b
	.inst.n 0x18d2
	.inst.n 0x2301
	.inst.n 0x8453
	.inst.n 0x4770
	.inst.n 0x0000
	.inst.n 0x1e70
	.inst.n 0x0300
