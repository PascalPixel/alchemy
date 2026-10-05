.syntax unified
	.thumb
	.global Math_UnsignedMulHigh
	.thumb_func
Math_UnsignedMulHigh:
AlchemyUncredited_08002074:
	bx	pc
	nop
	.arm
AlchemyUncreditedEnd_08002074:
	umulls	r3, r0, r1, r0
	bx	lr
	.thumb
	.global Math_UnsignedMulHighAdd
	.thumb_func
Math_UnsignedMulHighAdd:
AlchemyUncredited_08002080:
	bx	pc
	nop
	.arm
AlchemyUncreditedEnd_08002080:
	umulls	ip, r3, r1, r0
	mla	r0, r3, r2, r0
	bx	lr
