.syntax unified
.text
	.thumb
	.global Fragment_080bef58
	.thumb_func
Fragment_080bef58:
	lsrs	r7, r2, #2
	movs	r0, r0
	lsrs	r3, r3, #1
	movs	r0, r0
	lsrs	r7, r7, #32
	movs	r0, r0
	lsrs	r2, r0, #1
	movs	r0, r0
	lsrs	r1, r0, #1
	movs	r0, r0
	lsls	r1, r5, #4
	movs	r0, r0
	.inst.n 0xee70
	.inst.n 0x080b
	ands	r1, r0
	movs	r0, r0
	ands	r4, r0
	movs	r0, r0
	lsrs	r4, r2, #32
	movs	r0, r0
	lsls	r3, r7, #4
	movs	r0, r0
	lsls	r5, r0, #5
	.inst.n 0x0000
