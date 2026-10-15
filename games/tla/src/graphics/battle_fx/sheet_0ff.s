	.section .rodata
@ A 17x2 strip in its own 64-colour palette: BattleFx_RunProjectileVolley draws it in modes 15 to 17
	.global Resource_Data0FF
Resource_Data0FF:
	.incbin "games/tla/src/graphics/battle_fx/sheet_0ff.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/sheet_0ff.bitmap.lz"
	.byte 0x72	@ PACKER: left in the buffer by BattleFx_ArcFragmentsSheet+0xa7
