@ Field_PerspectiveDataA: the world map's cell words as 640 blocks of 16x16,
@ cut from a grid 32 blocks wide: a word offset per block, then each block
@ in the byte LZ the resource loader decodes.
	.section .rodata
	.global Field_PerspectiveDataA
Field_PerspectiveDataA:
	.incbin "MAP/WORLD/BLOCKS.blocks"
