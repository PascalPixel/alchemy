@ Division and remainder of the resident runtime, taking the dividend in r0
@ and the divisor in r1 like the compiler's division helpers: signed
@ divide, signed remainder, unsigned remainder and unsigned divide over one
@ unrolled restoring core, each step rsbs then orrcs and subcs. The signed
@ divide branches on the carry out of lsrs r0, ip, #32 into that core
@ inside IwramUnsignedDivide, which the other entries also reach with bl.
@ GCC never branches on the carry out of a shift and never enters another
@ function's body.
@ credit: handwritten — IWRAM runtime division helpers
.syntax unified
	.arm
	.set AlchemyLoadAddress, 0x08000af0
	.set AlchemyRunAddress, 0x03000380

@ コンパイラ実行時系と同型の符号付き除算。
	.global IwramSignedDivide
IwramSignedDivide:
	eor	ip, r0, r1
	movs	r2, r1
	rsbmi	r2, r2, #0
	movs	r1, r0
	rsbmi	r1, r1, #0
	lsrs	r0, ip, #32
	bcc	.L_divide_core
	mov	ip, lr
	bl	.L_divide_core
	rsb	r0, r0, #0
	bx	ip

@ コンパイラ実行時系と同型の符号付き剰余。
	.global IwramSignedRemainder
IwramSignedRemainder:
	stmfd	sp!, {lr}
	eor	ip, r0, r1
	movs	r2, r1
	rsbmi	r2, r2, #0
	movs	r1, r0
	rsbmi	r1, r1, #0
	mov	r0, #0
	bl	.L_divide_core
	mov	r0, r1
	movs	ip, ip
	rsbmi	r0, r0, #0
	ldmfd	sp!, {lr}
	bx	lr

@ コンパイラ実行時系と同型の符号なし剰余。
	.global IwramUnsignedRemainder
IwramUnsignedRemainder:
	mov	ip, lr
	bl	IwramUnsignedDivide
	mov	r0, r1
	bx	ip

@ コンパイラ実行時系と同型の符号なし除算。
	.global IwramUnsignedDivide
IwramUnsignedDivide:
	mov	r2, r1
	mov	r1, r0
	mov	r0, #0
.L_divide_core:
	rsbs	r3, r2, r1, lsr #28
	bcc	.L_divide_24
	rsbs	r3, r2, r1, lsr #31
	orrcs	r0, r0, #0x80000000
	subcs	r1, r1, r2, lsl #31
	rsbs	r3, r2, r1, lsr #30
	orrcs	r0, r0, #0x40000000
	subcs	r1, r1, r2, lsl #30
	rsbs	r3, r2, r1, lsr #29
	orrcs	r0, r0, #0x20000000
	subcs	r1, r1, r2, lsl #29
	rsbs	r3, r2, r1, lsr #28
	orrcs	r0, r0, #0x10000000
	subcs	r1, r1, r2, lsl #28
.L_divide_24:
	rsbs	r3, r2, r1, lsr #24
	bcc	.L_divide_20
	rsbs	r3, r2, r1, lsr #27
	orrcs	r0, r0, #0x08000000
	subcs	r1, r1, r2, lsl #27
	rsbs	r3, r2, r1, lsr #26
	orrcs	r0, r0, #0x04000000
	subcs	r1, r1, r2, lsl #26
	rsbs	r3, r2, r1, lsr #25
	orrcs	r0, r0, #0x02000000
	subcs	r1, r1, r2, lsl #25
	rsbs	r3, r2, r1, lsr #24
	orrcs	r0, r0, #0x01000000
	subcs	r1, r1, r2, lsl #24
.L_divide_20:
	rsbs	r3, r2, r1, lsr #20
	bcc	.L_divide_16
	rsbs	r3, r2, r1, lsr #23
	orrcs	r0, r0, #0x00800000
	subcs	r1, r1, r2, lsl #23
	rsbs	r3, r2, r1, lsr #22
	orrcs	r0, r0, #0x00400000
	subcs	r1, r1, r2, lsl #22
	rsbs	r3, r2, r1, lsr #21
	orrcs	r0, r0, #0x00200000
	subcs	r1, r1, r2, lsl #21
	rsbs	r3, r2, r1, lsr #20
	orrcs	r0, r0, #0x00100000
	subcs	r1, r1, r2, lsl #20
.L_divide_16:
	rsbs	r3, r2, r1, lsr #16
	bcc	.L_divide_12
	rsbs	r3, r2, r1, lsr #19
	orrcs	r0, r0, #0x00080000
	subcs	r1, r1, r2, lsl #19
	rsbs	r3, r2, r1, lsr #18
	orrcs	r0, r0, #0x00040000
	subcs	r1, r1, r2, lsl #18
	rsbs	r3, r2, r1, lsr #17
	orrcs	r0, r0, #0x00020000
	subcs	r1, r1, r2, lsl #17
	rsbs	r3, r2, r1, lsr #16
	orrcs	r0, r0, #0x00010000
	subcs	r1, r1, r2, lsl #16
.L_divide_12:
	rsbs	r3, r2, r1, lsr #12
	bcc	.L_divide_8
	rsbs	r3, r2, r1, lsr #15
	orrcs	r0, r0, #0x00008000
	subcs	r1, r1, r2, lsl #15
	rsbs	r3, r2, r1, lsr #14
	orrcs	r0, r0, #0x00004000
	subcs	r1, r1, r2, lsl #14
	rsbs	r3, r2, r1, lsr #13
	orrcs	r0, r0, #0x00002000
	subcs	r1, r1, r2, lsl #13
	rsbs	r3, r2, r1, lsr #12
	orrcs	r0, r0, #0x00001000
	subcs	r1, r1, r2, lsl #12
.L_divide_8:
	rsbs	r3, r2, r1, lsr #8
	bcc	.L_divide_4
	rsbs	r3, r2, r1, lsr #11
	orrcs	r0, r0, #0x00000800
	subcs	r1, r1, r2, lsl #11
	rsbs	r3, r2, r1, lsr #10
	orrcs	r0, r0, #0x00000400
	subcs	r1, r1, r2, lsl #10
	rsbs	r3, r2, r1, lsr #9
	orrcs	r0, r0, #0x00000200
	subcs	r1, r1, r2, lsl #9
	rsbs	r3, r2, r1, lsr #8
	orrcs	r0, r0, #0x00000100
	subcs	r1, r1, r2, lsl #8
.L_divide_4:
	rsbs	r3, r2, r1, lsr #4
	bcc	.L_divide_low
	rsbs	r3, r2, r1, lsr #7
	orrcs	r0, r0, #0x80
	subcs	r1, r1, r2, lsl #7
	rsbs	r3, r2, r1, lsr #6
	orrcs	r0, r0, #0x40
	subcs	r1, r1, r2, lsl #6
	rsbs	r3, r2, r1, lsr #5
	orrcs	r0, r0, #0x20
	subcs	r1, r1, r2, lsl #5
	rsbs	r3, r2, r1, lsr #4
	orrcs	r0, r0, #0x10
	subcs	r1, r1, r2, lsl #4
.L_divide_low:
	rsbs	r3, r2, r1
	bxcc	lr
	rsbs	r3, r2, r1, lsr #3
	orrcs	r0, r0, #8
	subcs	r1, r1, r2, lsl #3
	rsbs	r3, r2, r1, lsr #2
	orrcs	r0, r0, #4
	subcs	r1, r1, r2, lsl #2
	rsbs	r3, r2, r1, lsr #1
	orrcs	r0, r0, #2
	subcs	r1, r1, r2, lsl #1
	rsbs	r3, r2, r1
	orrcs	r0, r0, #1
	subcs	r1, r1, r2
	bx	lr
