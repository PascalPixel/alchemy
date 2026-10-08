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
