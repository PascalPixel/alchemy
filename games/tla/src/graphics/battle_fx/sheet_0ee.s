	.section .rodata
@ Uncolored art using BattleFx_EffectPalette02 colors (same effect).
	.global Resource_Data0EE
Resource_Data0EE:
	.incbin "games/tla/src/graphics/battle_fx/sheet_0ee.bitmap.lz"
	.byte 0x01, 0x2e	@ PACKER: left in the buffer by BattleFx_CornerRuneBSheet+0x65a
