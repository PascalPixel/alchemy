	.section .rodata
	.global BattleFx_TornadoSheet
BattleFx_TornadoSheet:
	.incbin "GRAPHICS/BATTLE_FX/TORNADO_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/TORNADO.parts.lz"
	.byte 0x7f, 0x02	@ PACKER: left in the buffer by BattleFx_FireBurstSheet+0x1b32
