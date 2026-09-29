@ BattleFx_BlueFlameSheet: the 64-colour palette of its first picture, then its 2 pictures
@ (listed in BLUE_FLAME.TSV), joined and packed by the resource packer.
	.section .rodata
	.global BattleFx_BlueFlameSheet
BattleFx_BlueFlameSheet:
	.incbin "GRAPHICS/BATTLE_FX/BLUE_FLAME_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/BLUE_FLAME.parts.lz"
