.syntax unified
.text
	.thumb
	.set sub_080c01fa, 0x080c01fa
	.global Fragment_080bf1e8
	.thumb_func
Fragment_080bf1e8:
	.inst.n 0xffff
	.inst.n 0x0000
	.inst.n 0xf060
	.inst.n 0x080b
	lsls	r6, r0, #8
	movs	r0, r0
	cmp	r5, #160
	lsrs	r4, r1, #32
	bl sub_080c01fa
	lsls	r5, r0, #8
	movs	r0, r0
	cmp	r3, #152
	lsrs	r4, r1, #32
	lsls	r1, r5, #4
	.inst.n 0x0000
