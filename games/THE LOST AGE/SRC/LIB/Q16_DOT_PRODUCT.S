@ Signed 16.16 three-term dot product. The routine at 0x08015510 copies it to
@ its stack frame with DMA3 (source 0x08017ce8 from its literal pool at
@ 0x080156e0, destination sp + 36, control 0x84000008: all 32 bytes) and
@ calls the copy with mov lr, sl. smull then two smlal keep the 64-bit sum
@ in ip (low) and r0 (high) before one combine: GCC's DImode multiply
@ patterns need a consecutive register pair, which ip and r0 are not. After
@ its 4-byte Thumb entry (bx pc) the 28-byte ARM body equals TBS
@ LIB/Q16_DOT_PRODUCT.S (0x08007994) and the copies in
@ BATTLE/EFFECT/SENTOU_KOUKA_SEKIWA.S and SENTOU_KOUKA_SEKIWA_2.S.
@ Its 4-byte Thumb entry, which is TLA's own, carries no credit.
@ credit: library — TBS/TLA common relocated ARM Q16 dot-product kernel
.syntax unified
	.thumb
	.set AlchemyLoadAddress, 0x08017ce8
	.global Math_DotProductQ16
	.thumb_func
Math_DotProductQ16:
AlchemyUncredited_08017ce8:
	bx	pc
	.2byte	0x0200
	.arm
AlchemyUncreditedEnd_08017ce8:
	smull	ip, r0, r1, r0
	smlal	ip, r0, r2, r3
	ldm	sp, {r2, r3}
	smlal	ip, r0, r2, r3
	lsl	r0, r0, #16
	orr	r0, r0, ip, lsr #16
	bx	lr
