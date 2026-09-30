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

	IwramEntry Math_Div, IwramSignedDivide
	IwramEntry Math_DivU, IwramUnsignedDivide
	IwramEntry Math_Mod, IwramSignedRemainder
	IwramEntry Math_ModU, IwramUnsignedRemainder
