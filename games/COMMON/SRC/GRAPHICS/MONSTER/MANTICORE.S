@ Monster_ManticoreSprites: 16 frames of 64x64, front and back of each pose, drawn with the
@ system palette; zero-skip coded and packed by the resource packer.
	.section .rodata
	.global Monster_ManticoreSprites
Monster_ManticoreSprites:
	.incbin "GRAPHICS/MONSTER/MANTICORE.frames.lz"
