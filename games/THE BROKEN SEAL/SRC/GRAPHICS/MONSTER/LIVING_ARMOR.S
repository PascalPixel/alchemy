	.section .rodata
	.global Monster_LivingArmorSprites
Monster_LivingArmorSprites:
	.incbin "GRAPHICS/MONSTER/LIVING_ARMOR.frames.lz"
	/* PACKER: left in the packer's buffer by Monster_ChimeraSprites at +0x3e7e */
	.byte 0x0e, 0x40
