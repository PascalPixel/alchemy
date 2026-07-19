.syntax unified
.text
	.thumb
	.set sub_080ddea2, 0x080ddea2
	.global Fragment_080dddb8
	.thumb_func
Fragment_080dddb8:
	.inst.n 0xeba6
	.inst.n 0x080e
	strb	r0, [r5, #30]
	movs	r0, r0
	movs	r0, r0
	lsls	r1, r0, #8
	ldrb	r0, [r5, #0]
	movs	r0, r0
	.inst.n 0xebb6
	.inst.n 0x080e
	.inst.n 0xebc0
	.inst.n 0x080e
	.inst.n 0xebc8
	.inst.n 0x080e
	.inst.n 0xebb9
	.inst.n 0x080e
	ldrb	r4, [r4, #0]
	movs	r0, r0
	bcs sub_080ddea2
	lsrs	r4, r1, #32
