	.section .rodata
	.global RamakanSabaku_SandPalette
RamakanSabaku_SandPalette:
	.incbin "graphics/ramakan/sand.gbapal"
	.global RamakanSabaku_SandEffectTiles
RamakanSabaku_SandEffectTiles:
	.incbin "graphics/ramakan/sand.4bpp.lz"
	.byte 0xff	@ PACKER: left in the buffer, no earlier resource holds these bytes
	.balign 4, 0
