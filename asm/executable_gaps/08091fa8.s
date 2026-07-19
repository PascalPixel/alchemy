.syntax unified
.text
	.thumb
	.global Fragment_08091fa8
	.thumb_func
Fragment_08091fa8:
	ldr	r3, [pc, #16]
	movs	r4, #233
	lsls	r4, r4, #1
	adds	r2, r3, r4
	strh	r0, [r2, #0]
	movs	r2, #234
	lsls	r2, r2, #1
	adds	r3, r3, r2
	strh	r1, [r3, #0]
	bx	lr
	.4byte 0x02000240
	.inst.n 0x4b08
	.inst.n 0x681a
	.inst.n 0x4b08
	.inst.n 0x469c
	.inst.n 0x23e0
	.inst.n 0x005b
	.inst.n 0x4463
	.inst.n 0x8018
	.inst.n 0x23e1
	.inst.n 0x005b
	.inst.n 0x4463
	.inst.n 0x8019
	.inst.n 0x23b8
	.inst.n 0x005b
	.inst.n 0x18d2
	.inst.n 0x4b03
	.inst.n 0x8013
	.inst.n 0x4770
	.inst.n 0x1ebc
	.inst.n 0x0300
	.inst.n 0x0240
	.inst.n 0x0200
	.inst.n 0x03e7
	.inst.n 0x0000
