@ BattleFx_TornadoSheet: the 64-colour palette of its first picture, then its 15 pictures
@ (listed in TORNADO.TSV), joined and packed by the resource packer.
	.section .rodata
	.global BattleFx_TornadoSheet
BattleFx_TornadoSheet:
	.incbin "GRAPHICS/BATTLE_FX/TORNADO_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/TORNADO.parts.lz"
