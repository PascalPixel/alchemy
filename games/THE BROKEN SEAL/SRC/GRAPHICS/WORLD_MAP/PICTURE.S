@ Field_WorldMapPicture: its 224-colour palette, then 600 tiles of 8-bit colour packed
@ by the resource packer.
	.section .rodata
	.global Field_WorldMapPicture
Field_WorldMapPicture:
	.incbin "GRAPHICS/WORLD_MAP/PICTURE.gbapal"
	.incbin "GRAPHICS/WORLD_MAP/PICTURE.8bpp.lz"
