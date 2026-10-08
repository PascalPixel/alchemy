	.section .rodata
	.global BattleFx_TornadoSheet
BattleFx_TornadoSheet:
	.incbin "GRAPHICS/BATTLE_FX/TORNADO_00.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/TORNADO.parts.lz"
	/* PACKER: left in the packer's buffer by BattleFx_FireBurstSheet at +0x1b32 */
	.byte 0x7f, 0x02
