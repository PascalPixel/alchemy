	.section .rodata
	.global BattleAnim_FireSwirlSheet
BattleAnim_FireSwirlSheet:
	.incbin "games/tbs/src/graphics/battle_anims/fire_swirl_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_anims/fire_swirl.parts.lz"
	.byte 0x12, 0x21	@ PACKER: left in the buffer by BattleAnim_FlareSheet+0x442
