.syntax unified
	.thumb
	.global Runtime_ReleaseHeapBlock
	.thumb_func
Runtime_ReleaseHeapBlock:
	movs	r4, #3
	lsls	r4, r4, #24
	ldr	r1, [r0, r4]
	lsrs	r3, r1, #22
	beq.n	.L_08013160
	movs	r2, #0
	str	r2, [r0, r4]
	movs	r0, #4
	ands	r3, r0
	str	r1, [r3, r4]
.L_08013160:
	bx	lr
AlchemyUncredited_08013162:
	.2byte	0x0000
AlchemyUncreditedEnd_08013162:
