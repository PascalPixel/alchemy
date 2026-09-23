@ Long-multiply helpers behind Thumb entries: each bx pc; nop stub enters an
@ ARM umulls. The first returns the high word of r0 * r1; the second
@ multiplies that high word by r2 and adds r0 (mla). umulls sets flags,
@ which none of GCC 2.96's long-multiply patterns does. TBS has no such
@ helpers.
@ Its two 4-byte Thumb entries, which are TLA's own, carry no credit.
@ credit: handwritten — unsigned high-word multiply and multiply-accumulate helpers
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x08002074
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
