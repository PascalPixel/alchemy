	.section .rodata
	.global Monster_YoungSwordsmanSprites
Monster_YoungSwordsmanSprites:
	.incbin "games/tla/src/graphics/monster/young_swordsman.pictures.lz"
	.byte 0xbc, 0x12	@ PACKER: left in the buffer by Monster_MermanSprites+0x22fe
