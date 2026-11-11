@ Why Math_UnsignedMulHigh to Math_UnsignedMulHighAdd are assembly:
@ each Thumb entry's bx pc switches to the ARM code that follows, a mode
@ switch inside a function; the nop after bx pc is never run.
@ Secondary: umulls sets flags nothing reads; GCC 2.95.2's ARM patterns
@ emit umull without the s bit.
.syntax unified
	.thumb
	.global Math_UnsignedMulHigh
	.thumb_func
Math_UnsignedMulHigh:
	bx	pc
AlchemyUncredited_08002076:
	nop
	.arm
AlchemyUncreditedEnd_08002076:
	umulls	r3, r0, r1, r0
	bx	lr
	.thumb
	.global Math_UnsignedMulHighAdd
	.thumb_func
Math_UnsignedMulHighAdd:
	bx	pc
AlchemyUncredited_08002082:
	nop
	.arm
AlchemyUncreditedEnd_08002082:
	umulls	ip, r3, r1, r0
	mla	r0, r3, r2, r0
	bx	lr
