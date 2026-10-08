	.section .rodata
	.global RamakanSabaku_SandPalette
RamakanSabaku_SandPalette:
	.incbin "GRAPHICS/RAMAKAN/SAND.gbapal"
	.global RamakanSabaku_SandEffectTiles
RamakanSabaku_SandEffectTiles:
	.incbin "GRAPHICS/RAMAKAN/SAND.4bpp.lz"
	.byte 0xff	@ PACKER: left in the buffer, no earlier resource holds these bytes
	.balign 4, 0
