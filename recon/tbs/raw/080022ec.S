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
