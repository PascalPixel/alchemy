@ Monster_GriffinSprites: 12 frames of 64x64, front and back of each pose, drawn with the
@ system palette; zero-skip coded and packed by the resource packer.
	.section .rodata
	.global Monster_GriffinSprites
Monster_GriffinSprites:
	.incbin "GRAPHICS/MONSTER/GRIFFIN.frames.lz"
