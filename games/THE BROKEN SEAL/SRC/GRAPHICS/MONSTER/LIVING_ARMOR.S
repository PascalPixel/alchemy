@ Monster_LivingArmorSprites: 18 frames of 64x64, front and back of each pose, drawn with the
@ system palette; zero-skip coded and packed by the resource packer.
	.section .rodata
	.global Monster_LivingArmorSprites
Monster_LivingArmorSprites:
	.incbin "GRAPHICS/MONSTER/LIVING_ARMOR.frames.lz"
