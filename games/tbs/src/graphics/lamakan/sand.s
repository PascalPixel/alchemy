	.section .rodata
	.global RamakanSabaku_SandPalette
RamakanSabaku_SandPalette:
	.incbin "games/tbs/src/graphics/lamakan/sand.gbapal"
	.global RamakanSabaku_SandEffectTiles
RamakanSabaku_SandEffectTiles:
	.incbin "games/tbs/src/graphics/lamakan/sand.4bpp.lz"
	.byte 0xff	@ PACKER: unused post-stream residue; RamakanSabaku_ClaimSandEffectVram uses decoded tiles; writer unknown
	.balign 4, 0
