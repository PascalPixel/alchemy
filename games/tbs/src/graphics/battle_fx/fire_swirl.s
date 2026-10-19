	.section .rodata
	.global BattleFx_FireSwirlSheet
BattleFx_FireSwirlSheet:
	.incbin "games/tbs/src/graphics/battle_fx/fire_swirl_00.gbapal"
	.incbin "games/tbs/src/graphics/battle_fx/fire_swirl.parts.lz"
	.byte 0x12, 0x21	@ PACKER: left in the buffer by BattleFx_FlareSheet+0x442
