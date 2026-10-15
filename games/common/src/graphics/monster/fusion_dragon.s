	.section .rodata
	.global Monster_FusionDragonSprites
Monster_FusionDragonSprites:
	.incbin "games/common/src/graphics/monster/fusion_dragon.frames.lz"
	.byte 0x70	@ PACKER: left in the buffer by Monster_FireDragonSprites+0x47f3
