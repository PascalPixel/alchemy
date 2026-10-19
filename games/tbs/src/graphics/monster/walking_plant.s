	.section .rodata
	.global Monster_WalkingPlantSprites
Monster_WalkingPlantSprites:
	.incbin "games/tbs/src/graphics/monster/walking_plant.frames.lz"
	.byte 0xd5	@ PACKER: left in the buffer by Monster_SmallLizardSprites+0xc17
