@ BattleFx_WhirlwindSheet: the 64-colour palette of its first picture, then its 5 pictures
@ (listed in WHIRLWIND.TSV), joined and packed by the resource packer.
	.section .rodata
	.global BattleFx_WhirlwindSheet
BattleFx_WhirlwindSheet:
	.incbin "GRAPHICS/BATTLE_FX/WHIRLWIND_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/WHIRLWIND.parts.lz"
