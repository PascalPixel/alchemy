	.section .rodata
	.global BattleEffect_CloudPuffSheet
BattleEffect_CloudPuffSheet:
	.incbin "games/tla/src/graphics/battle_effects/cloud_puff.gbapal"
	.incbin "games/tla/src/graphics/battle_effects/cloud_puff.bitmap.lz"
	.ifdef TLA_EDITION_EN
	.byte 0x4a, 0x05, 0x08	@ PACKER: unused post-stream residue; Resource_DecodeType01 uses decoded RAM and output length; writer unknown
	.endif
