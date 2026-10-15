	.section .rodata
	.global gLamakanDesert_SandPalette
gLamakanDesert_SandPalette:
	.incbin "games/tbs/src/graphics/lamakan/sand.gbapal"
	.global gLamakanDesert_SandEffectTiles
gLamakanDesert_SandEffectTiles:
	.incbin "games/tbs/src/graphics/lamakan/sand.4bpp.lz"
	.byte 0xff	@ PACKER: unused post-stream residue; LamakanDesert_ClaimSandEffectVram uses decoded tiles; writer unknown
	.balign 4, 0
