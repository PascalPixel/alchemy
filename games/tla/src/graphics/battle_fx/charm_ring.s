	.section .rodata
	.global BattleEffect_CharmRingSheet
BattleEffect_CharmRingSheet:
	.incbin "games/tla/src/graphics/battle_fx/charm_ring.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/charm_ring.bitmap.lz"
	.byte 0x76	@ PACKER: left in the buffer by Resource_Data0B5+0x637
