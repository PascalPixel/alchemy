@ BattleFx_BlueRingSheet: the 64-colour palette of its first picture, then its 11 pictures
@ (listed in BLUE_RING.TSV), joined and packed by the resource packer.
	.section .rodata
	.global BattleFx_BlueRingSheet
BattleFx_BlueRingSheet:
	.incbin "GRAPHICS/BATTLE_FX/BLUE_RING_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/BLUE_RING.parts.lz"
