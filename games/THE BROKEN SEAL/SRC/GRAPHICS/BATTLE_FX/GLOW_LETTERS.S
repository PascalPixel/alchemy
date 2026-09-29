@ BattleFx_GlowLetters: its 16-colour palette, then 128 tiles of 4-bit colour packed
@ by the resource packer.
	.section .rodata
	.global BattleFx_GlowLetters
BattleFx_GlowLetters:
	.incbin "GRAPHICS/BATTLE_FX/GLOW_LETTERS.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/GLOW_LETTERS.4bpp.lz"
