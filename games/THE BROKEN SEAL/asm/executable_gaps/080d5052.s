.syntax unified
.text
	.thumb
	.set sub_080d55a6, 0x080d55a6
	.global Fragment_080d5052
	.thumb_func
Fragment_080d5052:
	movs	r0, r0
	lsls	r7, r0, #2
	movs	r0, r0
	asrs	r0, r1, #14
	lsls	r0, r0, #12
	subs	r0, r2, #1
	lsls	r0, r0, #12
	movs	r0, r3
	lsls	r1, r0, #8
	ldrb	r0, [r5, #0]
	movs	r0, r0
	b sub_080d55a6
	.inst.n 0x080e
	.inst.n 0x7784
	.inst.n 0x0000
	.inst.n 0xd261
	.inst.n 0x080c
	.inst.n 0x1e80
	.inst.n 0x0300
	.inst.n 0xe29a
	.inst.n 0x080e
	.inst.n 0xe2a9
	.inst.n 0x080e
	.inst.n 0x10b4
	.inst.n 0x0000
	.inst.n 0x0000
	.inst.n 0x0201
	.inst.n 0x03ff
	.inst.n 0x0000
	.inst.n 0x7fff
	.inst.n 0x0000
	.inst.n 0xc000
	.inst.n 0xffff
