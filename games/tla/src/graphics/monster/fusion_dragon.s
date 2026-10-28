	.section .rodata
	.global Monster_FusionDragonSprites
Monster_FusionDragonSprites:
	.incbin "games/tla/src/graphics/monster/fusion_dragon.pictures.lz"
	.byte 0x70	@ PACKER: left in the buffer by Monster_FireDragonSprites+0x47f3
