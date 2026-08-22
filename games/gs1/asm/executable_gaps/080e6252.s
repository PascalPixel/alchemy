.syntax unified
.text
	.thumb
	.global Fragment_080e6252
	.thumb_func
Fragment_080e6252:
	movs	r0, r0
	ldrb	r0, [r5, #0]
	movs	r0, r0
	movs	r0, r0
	lsls	r1, r0, #8
	cmp	r5, #128
	lsls	r1, r0, #8
	ldr	r3, [pc, #0]
	lsls	r1, r0, #8
	.4byte 0x02015980
	udf	#159
	lsrs	r6, r1, #32
	udf	#178
	lsrs	r6, r1, #32
	udf	#171
	lsrs	r6, r1, #32
	udf	#165
	lsrs	r6, r1, #32
	.inst.n 0xffff
	.inst.n 0x0000
	ands	r0, r0
	lsls	r1, r0, #8
	mov	ip, ip
	lsls	r1, r0, #8
	.inst.n 0xedea
	.inst.n 0x080e
