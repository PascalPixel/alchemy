.syntax unified
.text
	.thumb
	.global Fragment_080ecaba
	.thumb_func
Fragment_080ecaba:
	movs	r0, r0
	udf	#92
	lsrs	r6, r1, #32
	ldr	r6, [pc, #128]
	movs	r0, r0
	lsls	r0, r6, #3
	movs	r0, r0
	movs	r0, r0
	lsls	r1, r0, #8
	udf	#72
	lsrs	r6, r1, #32
	.inst.n 0xef78
	.inst.n 0x080e
	ldrb	r4, [r4, #0]
	movs	r0, r0
	subs	r4, r0, r4
	lsls	r0, r0, #12
	subs	r0, r2, r3
	lsls	r0, r0, #12
	stmia	r0!, {r0, r5, r6, r7}
	lsrs	r6, r1, #32
	lsls	r0, r0, #1
	lsls	r0, r0, #16
