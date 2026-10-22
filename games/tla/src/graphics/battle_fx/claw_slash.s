	.section .rodata
	.global BattleEffect_ClawSlashSheet
BattleEffect_ClawSlashSheet:
	.incbin "games/tla/src/graphics/battle_fx/claw_slash.gbapal"
	.incbin "games/tla/src/graphics/battle_fx/claw_slash.bitmap.lz"
	.byte 0x01	@ PACKER: left in the buffer by BattleEffect_BoulderPicture+0x6af
