@ BattleFx_FireSwirlSheet: the 64-colour palette of its first picture, then its 7 pictures
@ (listed in FIRE_SWIRL.TSV), joined and packed by the resource packer.
	.section .rodata
	.global BattleFx_FireSwirlSheet
BattleFx_FireSwirlSheet:
	.incbin "GRAPHICS/BATTLE_FX/FIRE_SWIRL_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/FIRE_SWIRL.parts.lz"
