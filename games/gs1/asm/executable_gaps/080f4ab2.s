.syntax unified
.text
	.thumb
	.global Fragment_080f4ab2
	.thumb_func
Fragment_080f4ab2:
	movs	r0, r0
	lsls	r4, r1, #19
	movs	r0, r0
	lsls	r0, r0, #9
	lsls	r0, r0, #8
	lsrs	r0, r2, #4
	movs	r0, r0
	subs	r4, r0, r4
	lsls	r0, r0, #12
	subs	r0, r5, r3
	lsls	r0, r0, #12
	strb	r4, [r0, #2]
	movs	r0, r0
	strb	r0, [r4, #2]
	movs	r0, r0
	movs	r0, r0
	.inst.n 0xfffc
	.inst.n 0xffff
	movs	r7, r3
	movs	r0, r0
	.inst.n 0xff4c
	.inst.n 0x7088
	movs	r0, r0
	movs	r0, r0
	.inst.n 0xff60
	.inst.n 0x709c
	movs	r0, r0
	movs	r0, r0
	.inst.n 0xffd8
	.inst.n 0x70a4
	.inst.n 0x0000
