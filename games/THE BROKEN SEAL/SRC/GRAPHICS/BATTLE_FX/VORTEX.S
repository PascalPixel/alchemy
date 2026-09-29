@ BattleFx_VortexSheet: the 64-colour palette of its first picture, then its 4 pictures
@ (listed in VORTEX.TSV), joined and packed by the resource packer.
	.section .rodata
	.global BattleFx_VortexSheet
BattleFx_VortexSheet:
	.incbin "GRAPHICS/BATTLE_FX/VORTEX_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/VORTEX.parts.lz"
