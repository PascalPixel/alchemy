	.section .rodata
	.global BattleFx_DustBlobSheet
BattleFx_DustBlobSheet:
	.incbin "games/tla/src/graphics/battle_fx/dust_blob.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/dust_blob.bitmap.lz"
	.byte 0x54, 0x7b, 0x1f	@ PACKER: left in the buffer by BattleFx_CloudSwirlSheet+0x329
