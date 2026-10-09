@ C2: IwramEntry emits aligned 8-byte Thumb ldr r3/bx r3/target-word stubs.
.syntax unified
	.thumb

	.macro IwramEntry name, target
	.global \name
	.thumb_func
\name:
	ldr r3, 1f
	bx r3
1:
	.4byte \target
	.endm

	IwramEntry __divsi3, IwramSignedDivide
	IwramEntry __udivsi3, IwramUnsignedDivide
	IwramEntry __modsi3, IwramSignedRemainder
	IwramEntry __umodsi3, IwramUnsignedRemainder
