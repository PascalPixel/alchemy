	.section .rodata
@ Uncolored art using BattleEffect_OrangePaletteB colors (same effect).
	.global Resource_Data0D7
Resource_Data0D7:
	.incbin "games/tla/src/graphics/battle_effects/sheet_0d7.bitmap.lz"
	.ifdef TLA_EDITION_JA
	.byte 0x3e, 0x05, 0x08	@ PACKER: unused post-stream residue; Resource_DecodeType01 uses decoded RAM and output length; writer unknown
	.else
	.byte 0x40, 0x05, 0x08	@ PACKER: unused post-stream residue; Resource_DecodeType01 uses decoded RAM and output length; writer unknown
	.endif
