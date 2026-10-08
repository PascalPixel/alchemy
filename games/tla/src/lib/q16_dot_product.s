.syntax unified
	.thumb
	.global Math_DotProductQ16Code
Math_DotProductQ16Code:
	.global Math_DotProductQ16
	.thumb_func
Math_DotProductQ16:
	bx	pc
AlchemyUncredited_08017cea:
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08017cea:
	smull	ip, r0, r1, r0
	smlal	ip, r0, r2, r3
	ldm	sp, {r2, r3}
	smlal	ip, r0, r2, r3
	lsl	r0, r0, #16
	orr	r0, r0, ip, lsr #16
	bx	lr
