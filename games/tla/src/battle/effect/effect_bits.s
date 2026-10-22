	.section .rodata
@ The operands the compositing routines add to their widths: 1 << n for n = 0
@ to 15, as ARM data-processing immediates (rotation in bits 8-11, the value
@ in bits 0-7). The Broken Seal's gBattleEffect_BitOperands (effect/effect_bits.c).
	.global Data_081973f0
Data_081973f0:
	.2byte 0x001, 0x002, 0x004, 0x008, 0x010, 0x020, 0x040, 0x080
	.2byte 0xc01, 0xc02, 0xb01, 0xb02, 0xa01, 0xa02, 0x901, 0x902
