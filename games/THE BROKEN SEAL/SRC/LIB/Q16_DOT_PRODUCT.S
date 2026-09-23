@ Signed 16.16 fixed-point three-term dot product. The routine at 0x08004fe4
@ copies these 28 bytes with DMA3 into its stack frame and calls the copy with
@ four factors in r0-r3 and two on the stack. smull and smlal keep the 64-bit
@ sum in ip (low) and r0 (high) until one final combine; GCC's DImode
@ multiply-accumulate needs a consecutive register pair, which ip and r0 are
@ not. Every TBS and TLA edition has these bytes (TLA LIB/Q16_DOT_PRODUCT.S,
@ behind a Thumb entry).
@ credit: library — TBS/TLA common relocated ARM Q16 dot-product kernel
.syntax unified
	.arm
	.global Func_08007994
	.global RelocatedQ16DotProduct
Func_08007994:
RelocatedQ16DotProduct:
	smull	r12, r0, r1, r0
	smlal	r12, r0, r2, r3
	ldmia	sp, {r2, r3}
	smlal	r12, r0, r2, r3
	lsl	r0, r0, #16
	orr	r0, r0, r12, lsr #16
	bx	lr
