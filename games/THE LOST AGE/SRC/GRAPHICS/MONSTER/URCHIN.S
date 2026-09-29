@ Monster_UrchinSprites: 18 frames of 32x32, front and back of each pose, drawn with the
@ system palette; zero-skip coded and packed by the resource packer.
	.section .rodata
	.global Monster_UrchinSprites
Monster_UrchinSprites:
	.incbin "GRAPHICS/MONSTER/URCHIN.frames.lz"
