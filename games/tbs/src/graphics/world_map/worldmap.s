	.section .rodata
	.global WorldMap_PaletteA
WorldMap_PaletteA:
	.incbin "graphics/world_map/palette_a.gbapal.plz"
	.byte 0x11, 0x82, 0x87	@ PACKER: left in the buffer by Field_DefaultMetatileAttributes+0x1f5
	.global WorldMap_TilesA
WorldMap_TilesA:
	.incbin "graphics/world_map/tiles_a.8bpp.lz"
	.byte 0x91, 0xb3	@ PACKER: left in the buffer by Field_DefaultMapCells+0x16ba
	.global WorldMap_TilesB
WorldMap_TilesB:
	.incbin "graphics/world_map/tiles_b.8bpp.lz"
	.byte 0x10, 0x08, 0xc4	@ PACKER: left in the buffer by WorldMap_TilesA+0x151d
	.global WorldMap_TilesC
WorldMap_TilesC:
	.incbin "graphics/world_map/tiles_c.8bpp.lz"
	.byte 0x63, 0xd3, 0xb6	@ PACKER: left in the buffer by WorldMap_TilesB+0xbc9
	.global WorldMap_TilesD
WorldMap_TilesD:
	.incbin "graphics/world_map/tiles_d.8bpp.lz"
	.byte 0x12, 0x9f	@ PACKER: left in the buffer by WorldMap_TilesC+0xb72
	.global WorldMap_TilesE
WorldMap_TilesE:
	.incbin "graphics/world_map/tiles_e.8bpp.lz"
	.byte 0x48	@ PACKER: left in the buffer by Field_DefaultMapCells+0x1ea7
	.global WorldMap_PaletteB
WorldMap_PaletteB:
	.incbin "graphics/world_map/palette_b.gbapal.plz"
	.global WorldMap_TilesF
WorldMap_TilesF:
	.incbin "graphics/world_map/tiles_f.8bpp.lz"
	.byte 0x4a, 0xa8, 0x82	@ PACKER: left in the buffer by WorldMap_TilesE+0x159d
	.global WorldMap_TilesG
WorldMap_TilesG:
	.incbin "graphics/world_map/tiles_g.8bpp.lz"
	.byte 0x20, 0x75	@ PACKER: left in the buffer by WorldMap_TilesF+0x14ae
	.global WorldMap_TilesH
WorldMap_TilesH:
	.incbin "graphics/world_map/tiles_h.8bpp.lz"
	.global WorldMap_TilesI
WorldMap_TilesI:
	.incbin "graphics/world_map/tiles_i.8bpp.lz"
	.global WorldMap_TilesJ
WorldMap_TilesJ:
	.incbin "graphics/world_map/tiles_j.8bpp.lz"
	.byte 0x52	@ PACKER: left in the buffer by WorldMap_TilesF+0x153f
