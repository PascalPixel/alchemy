@ Q14 ratio multiply of the resident runtime: it divides 0x40000000 by r0
@ with the signed divide at 0x03000380 (RUNTIME_DIVIDE.S), multiplies the
@ quotient by r1 and returns the Q14 product. It saves only lr
@ (stmfd sp!, {lr}), keeps r1 in r4 across the call without saving r4, and
@ reloads lr before it computes the result (ldmfd sp!, {lr}; lsl; orr;
@ bx lr). A value kept in r4 across a call without saving it breaks the
@ procedure call standard under either r4 convention. TLA's runtime has its
@ own variant (IWRAM_RUNTIME_RATIO_Q14.S).
@ credit: handwritten — IWRAM runtime Q14 ratio multiply
.syntax unified
	.set	IwramSignedDivide, 0x03000380
	.arm
	.set AlchemyLoadAddress, 0x080008ac
	.set AlchemyRunAddress, 0x0300013c

	.global IwramRatioMulQ14
IwramRatioMulQ14:
	stmfd	sp!, {lr}
	mov	r4, r1
	mov	r1, r0
	mov	r0, #0x40000000
	bl	IwramSignedDivide
	smull	r3, r0, r4, r0
	ldmfd	sp!, {lr}
	lsl	r0, r0, #18
	orr	r0, r0, r3, lsr #14
	bx	lr
