.syntax unified
.text
	.thumb
	.global Fragment_080268da
	.thumb_func
Fragment_080268da:
	movs	r0, r0
	str	r4, [r7, #100]
	lsrs	r2, r0, #32
	lsrs	r4, r5, #2
	movs	r0, r0
	strb	r4, [r3, #15]
	lsrs	r3, r0, #32
	strb	r0, [r4, #15]
	lsrs	r3, r0, #32
	strb	r4, [r4, #15]
	lsrs	r3, r0, #32
	lsrs	r3, r5, #2
	movs	r0, r0
	lsls	r1, r6, #4
	movs	r0, r0
	lsrs	r4, r4, #2
	movs	r0, r0
	lsls	r3, r7, #4
	movs	r0, r0
	lsls	r5, r7, #4
	movs	r0, r0
	lsls	r1, r0, #5
	movs	r0, r0
	lsrs	r5, r4, #2
	movs	r0, r0
	lsrs	r6, r4, #2
	movs	r0, r0
	lsrs	r7, r4, #2
	movs	r0, r0
	lsrs	r0, r5, #2
	movs	r0, r0
	lsrs	r1, r5, #2
	.inst.n 0x0000
