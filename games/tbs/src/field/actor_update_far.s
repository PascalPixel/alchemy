@ Why assembly: three independent ARM functions share one trailing literal pool;
@ its references cross their boundaries, which GCC's per-function pool
@ flushing cannot emit. Complete handwritten extent: 36 bytes
@ (24 instruction bytes and 12 literal bytes).
.syntax unified
	.arm
	.global fdTrigAtanStubArm
fdTrigAtanStubArm:
	ldr	r4, .L_Trig_Atan
	bx	r4
	.global fdMathSqrtStubArm
fdMathSqrtStubArm:
	ldr	r4, .L_IwramSqrt
	bx	r4
	.global fdFixedDivStubArm
fdFixedDivStubArm:
	ldr	r4, .L_IwramDivFX
	bx	r4
.L_Trig_Atan:
	.4byte	syAtan
.L_IwramSqrt:
	.4byte	syIWRAMSquart
.L_IwramDivFX:
	.4byte	syIWRAMDivFX
