@ Q14 ratio multiply of TLA's resident runtime, behind a 4-byte Thumb entry
@ (bx pc): it divides 0x40000000 by r0 through the divide entry at
@ 0x03000528 (IWRAM_RUNTIME_DIVIDE.S), multiplies the quotient by r1 and
@ returns the Q14 product. It keeps r1 across that call by pushing it with
@ lr (push {r1, lr}) and pops both (pop {r1, lr}) before it computes the
@ result. GCC 2.96 saves only call-saved registers with lr in a prologue and
@ restores them in the epilogue after the body; r1 is an argument register.
@ TBS's runtime has its own variant (RUNTIME_RATIO_Q14.S), which keeps r1
@ in r4. The entry is TLA's own and carries no credit.
@ credit: handwritten — TLA IWRAM runtime Q14 ratio multiply
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x080007e8
	.set AlchemyRunAddress, 0x03000230
AlchemyUncredited_080007e8:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_080007e8:
	push	{r1, lr}
	mov	r1, r0
	mov	r0, #0x40000000
	bl	0x3000528
	pop	{r1, lr}
	smull	r3, r0, r1, r0
	lsl	r0, r0, #18
	orr	r0, r0, r3, lsr #14
	bx	lr
