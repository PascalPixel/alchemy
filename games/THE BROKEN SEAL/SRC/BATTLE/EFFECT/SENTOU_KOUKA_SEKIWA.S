@ Signed 16.16 fixed-point three-term dot product in ARM state: smull and
@ smlal accumulate all three 64-bit products in ip (low) and r0 (high) before
@ one combine. GCC's DImode multiply-accumulate needs a consecutive register
@ pair, which ip and r0 are not. These 28 bytes equal LIB/Q16_DOT_PRODUCT.S;
@ this copy sits in the battle-effect bank before the model decoder. Every
@ TBS and TLA edition has it (TLA 0x08017cec, 0x081a0030 and 0x081a0280).
@ credit: library — TBS/TLA common relocated ARM Q16 dot-product kernel, battle-effect bank copy
.syntax unified
.text
	.arm
	.set AlchemyLoadAddress, 0x080f0008
	.global SentouKouka_SekiWa
SentouKouka_SekiWa:
	smull	ip, r0, r1, r0
	smlal	ip, r0, r2, r3
	ldm	sp, {r2, r3}
	smlal	ip, r0, r2, r3
	lsl	r0, r0, #16
	orr	r0, r0, ip, lsr #16
	bx	lr
