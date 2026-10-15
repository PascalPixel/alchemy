	.section .rodata
	.global Monster_ClubTrollSprites
Monster_ClubTrollSprites:
	.incbin "games/tla/src/graphics/monster/club_troll.frames.lz"
	.byte 0x47, 0x8e, 0x49	@ PACKER: left in the buffer by Monster_BlueDevilSprites+0x3c21
