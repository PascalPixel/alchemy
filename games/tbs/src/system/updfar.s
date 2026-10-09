@ C2: Three independent ARM functions share one trailing literal pool;
@ its references cross their boundaries, which GCC's per-function pool
@ flushing cannot emit. Complete handwritten extent: 36 bytes
@ (24 instruction bytes and 12 literal bytes).
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
