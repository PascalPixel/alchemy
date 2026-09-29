@ Monster_BlueDevilSprites: 16 frames of 64x64, front and back of each pose, drawn with the
@ system palette; zero-skip coded and packed by the resource packer.
	.section .rodata
	.global Monster_BlueDevilSprites
Monster_BlueDevilSprites:
	.incbin "GRAPHICS/MONSTER/BLUE_DEVIL.frames.lz"
