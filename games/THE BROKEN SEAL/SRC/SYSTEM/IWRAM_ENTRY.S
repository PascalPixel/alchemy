@ Division entries: four aligned 8-byte veneers that load a resident IWRAM
@ arithmetic routine's address into r3 and branch to it, which the calling
@ convention permits. No compiler emits them; the macro builds each from its
@ target.
@ credit: reconstructed_veneer — main-image IWRAM division entry veneers
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

	.global FixedPoint_Ratio
	.thumb_func
FixedPoint_Ratio:
	.global Math_Div
	.thumb_func
Math_Div:
	IwramEntry IwramSignedDivideEntry, IwramSignedDivide
	.global Math_DivU
	.thumb_func
Math_DivU:
	IwramEntry IwramUnsignedDivideEntry, IwramUnsignedDivide
	.global Func_080022fc
	.thumb_func
Func_080022fc:
	.global Math_Mod
	.thumb_func
Math_Mod:
	IwramEntry IwramSignedRemainderEntry, IwramSignedRemainder
	.global Math_ModU
	.thumb_func
Math_ModU:
	IwramEntry IwramUnsignedRemainderEntry, IwramUnsignedRemainder
