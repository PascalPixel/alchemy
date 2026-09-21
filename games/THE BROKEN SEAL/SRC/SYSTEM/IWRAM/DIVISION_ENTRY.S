.syntax unified
	.thumb

	.set IwramSignedDivide, 0x03000380
	.set IwramSignedRemainder, 0x030003ac
	.set IwramUnsignedRemainder, 0x030003e0
	.set IwramUnsignedDivide, 0x030003f0

	.macro IwramEntry name, target
	.global \name
	.thumb_func
\name:
	ldr r3, 1f
	bx r3
1:
	.4byte \target
	.endm

	IwramEntry IwramSignedDivideEntry, IwramSignedDivide
	IwramEntry IwramUnsignedDivideEntry, IwramUnsignedDivide
	IwramEntry IwramSignedRemainderEntry, IwramSignedRemainder
	IwramEntry IwramUnsignedRemainderEntry, IwramUnsignedRemainder
