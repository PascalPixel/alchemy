@ Signed 16.16 three-term dot product in ARM state, the copy the battle-effect
@ bank keeps in front of its second model decoder. smull then two smlal keep
@ the 64-bit sum in ip (low) and r0 (high) before one combine: GCC's DImode
@ multiply patterns need a consecutive register pair, which ip and r0 are
@ not. These 28 bytes equal TBS BATTLE/EFFECT/SENTOU_KOUKA_SEKIWA.S
@ (0x080f0008) and LIB/Q16_DOT_PRODUCT.S (0x08007994); TLA repeats them at
@ 0x08017cec and 0x081a0030.
@ credit: library — TBS/TLA common relocated ARM Q16 dot-product kernel, battle-effect bank copy
.syntax unified
	.arm
	.set AlchemyLoadAddress, 0x081a0280
	.global SentouKouka_SekiWa2
SentouKouka_SekiWa2:
	smull	ip, r0, r1, r0
	smlal	ip, r0, r2, r3
	ldm	sp, {r2, r3}
	smlal	ip, r0, r2, r3
	lsl	r0, r0, #16
	orr	r0, r0, ip, lsr #16
	bx	lr
