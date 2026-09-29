@ BattleFx_ParticleSheet: its 240-colour palette, then 424 tiles of 8-bit colour packed
@ by the resource packer.
	.section .rodata
	.global BattleFx_ParticleSheet
BattleFx_ParticleSheet:
	.incbin "GRAPHICS/BATTLE_FX/PARTICLES.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/PARTICLES.8bpp.lz"
