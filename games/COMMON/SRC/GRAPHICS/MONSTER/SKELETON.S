@ Monster_SkeletonSprites: 12 frames of 64x64, front and back of each pose, drawn with the
@ system palette; zero-skip coded and packed by the resource packer.
	.section .rodata
	.global Monster_SkeletonSprites
Monster_SkeletonSprites:
	.incbin "GRAPHICS/MONSTER/SKELETON.frames.lz"
