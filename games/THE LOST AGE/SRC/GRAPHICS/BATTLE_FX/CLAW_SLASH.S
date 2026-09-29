@ BattleFx_ClawSlashSheet: a 64x64 picture, its 64-colour palette and then its
@ pixels as a packed 8-bit bitmap.
	.section .rodata
	.global BattleFx_ClawSlashSheet
BattleFx_ClawSlashSheet:
	.incbin "GRAPHICS/BATTLE_FX/CLAW_SLASH.gbapal"
	.incbin "GRAPHICS/BATTLE_FX/CLAW_SLASH.bitmap.lz"
