	.section .rodata
	.global BattleEffect_ThornSheet
BattleEffect_ThornSheet:
	.incbin "games/tbs/src/graphics/battle_fx/thorn_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/thorn.parts.lz"
	.byte 0x78	@ PACKER: left in the buffer by BattleEffect_FlashBurstSheet+0xd9b
