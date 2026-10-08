	.section .rodata
	.global BattleFx_TornadoSheet
BattleFx_TornadoSheet:
	.incbin "graphics/battle_fx/tornado_00.gbapal"
	.incbin "graphics/battle_fx/tornado.parts.lz"
	.byte 0x7f, 0x02	@ PACKER: left in the buffer by BattleFx_FireBurstSheet+0x1b32
