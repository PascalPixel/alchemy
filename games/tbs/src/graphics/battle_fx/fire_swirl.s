	.section .rodata
	.global BattleEffect_FireSwirlSheet
BattleEffect_FireSwirlSheet:
	.incbin "games/tbs/src/graphics/battle_fx/fire_swirl_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/fire_swirl.parts.lz"
	.byte 0x12, 0x21	@ PACKER: left in the buffer by BattleEffect_FlareSheet+0x442
