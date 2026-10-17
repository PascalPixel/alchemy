	.section .rodata
@ A 32x4 texture with no palette of its own, a band that brightens to its
@ middle, as 0xC9 is: BattleFx_RunCruelRuin maps it with the same 32 by 4 texels, and
@ BattleFx_RunEclipse colours it with BattleFx_BlueFlameSheet
	.global Resource_Data0CA
Resource_Data0CA:
	.incbin "games/tla/src/graphics/battle_fx/sheet_0ca.bitmap.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
