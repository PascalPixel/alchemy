	.section .rodata
@ An archer's two 88x96 frames and three small cels, in their own 64-colour
@ palette: the sizes BattleEffect_RunVengeance draws them at
	.global Resource_Data0B0
Resource_Data0B0:
	.incbin "games/tla/src/graphics/battle_fx/sheet_0b0_00.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/sheet_0b0.parts.lz"
	.byte 0x46, 0xcd	@ PACKER: left in the buffer by Summon_ChainedDemonTiles+0x1c4e
