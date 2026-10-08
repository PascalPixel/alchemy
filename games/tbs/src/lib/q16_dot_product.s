@ C2: smull and both smlal keep the multiply halves in unpaired r12/r0.
.syntax unified
	.arm
	.global RelocatedQ16DotProduct
RelocatedQ16DotProduct:
	smull	r12, r0, r1, r0
	smlal	r12, r0, r2, r3
	ldmia	sp, {r2, r3}
	smlal	r12, r0, r2, r3
	lsl	r0, r0, #16
	orr	r0, r0, r12, lsr #16
	bx	lr
