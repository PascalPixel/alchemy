	.section .rodata
	.global Monster_LivingArmorSprites
Monster_LivingArmorSprites:
	.incbin "GRAPHICS/MONSTER/LIVING_ARMOR.frames.lz"
	.byte 0x0e, 0x40	@ PACKER: left in the buffer by Monster_ChimeraSprites+0x3e7e
