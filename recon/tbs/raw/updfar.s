@ Not yet C: these ARM tail entries have no C2 handwritten marker.
@ Each entry has eight instruction bytes, but their target words form a
@ shared trailing pool, so their complete extents are not eight-byte veneers.
.syntax unified
	.arm
	.global ArcTan2FarArm
ArcTan2FarArm:
	ldr	r4, .L_ArcTan2
	bx	r4
	.global IwramSqrtFarArm
IwramSqrtFarArm:
	ldr	r4, .L_IwramSqrt
	bx	r4
	.global IwramRatioMulQ14FarArm
IwramRatioMulQ14FarArm:
	ldr	r4, .L_IwramRatioMulQ14
	bx	r4
.L_ArcTan2:
	.4byte	ArcTan2
.L_IwramSqrt:
	.4byte	IwramSqrt
.L_IwramRatioMulQ14:
	.4byte	IwramRatioMulQ14
