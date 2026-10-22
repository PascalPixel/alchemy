	.section .rodata
@ A 32x4 texture with no palette of its own, a band that brightens to its
@ middle: the effects that map it give it 32 by 4 texels (2^5 by 2^2), and
@ most of them colour it with BattleEffect_OrangePaletteB
	.global Resource_Data0C9
Resource_Data0C9:
	.incbin "games/tla/src/graphics/battle_fx/sheet_0c9.bitmap.lz"
	.byte 0x1e, 0x00, 0x84	@ PACKER: left in the buffer by Resource_Data0C8+0x29
