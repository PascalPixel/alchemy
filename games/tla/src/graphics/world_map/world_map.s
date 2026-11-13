	.section .rodata
	.global WorldMap_TilesA1
WorldMap_TilesA1:
	.incbin "games/tla/src/graphics/world_map/tiles_a1.8bpp.lz"
	.byte 0x23, 0xe0	@ PACKER: left in the buffer by Field_DefaultMapCells+0x176a
	.global WorldMap_TilesA2
WorldMap_TilesA2:
	.incbin "games/tla/src/graphics/world_map/tiles_a2.8bpp.lz"
	.byte 0xbe, 0x9c, 0x39	@ PACKER: left in the buffer by WorldMap_TilesA1+0x1535
	.global WorldMap_TilesA3
WorldMap_TilesA3:
	.incbin "games/tla/src/graphics/world_map/tiles_a3.8bpp.lz"
	.global WorldMap_TilesA4
WorldMap_TilesA4:
	.incbin "games/tla/src/graphics/world_map/tiles_a4.8bpp.lz"
	.byte 0x23	@ PACKER: left in the buffer by WorldMap_TilesA2+0x1083
	.global WorldMap_TilesA5
WorldMap_TilesA5:
	.incbin "games/tla/src/graphics/world_map/tiles_a5.8bpp.lz"
	.global WorldMap_TilesA6
WorldMap_TilesA6:
	.incbin "games/tla/src/graphics/world_map/tiles_a6.8bpp.lz"
	.byte 0x4c	@ PACKER: left in the buffer by WorldMap_TilesA5+0xf3f
	.global WorldMap_PaletteB
WorldMap_PaletteB:
	.incbin "games/tla/src/graphics/world_map/palette_b.gbapal.plz"
@ World map B animated tiles; Field_DefaultMapAnimation format.
	.global Resource_Data1A4
Resource_Data1A4:
	.incbin "games/tla/src/graphics/world_map/anim_b.script.lz"
@ World map B attributes; Field_DefaultCellPictureAttributes format.
	.global Resource_Data1A5
Resource_Data1A5:
	.incbin "games/tla/src/graphics/world_map/attr_b.table.lz"
	.byte 0x11, 0xf9, 0xdc	@ PACKER: left in the buffer by WorldMap_TilesA6+0x3bd
	.global WorldMap_TilesB1
WorldMap_TilesB1:
	.incbin "games/tla/src/graphics/world_map/tiles_b1.8bpp.lz"
	.global WorldMap_TilesB2
WorldMap_TilesB2:
	.incbin "games/tla/src/graphics/world_map/tiles_b2.8bpp.lz"
	.byte 0x00	@ PACKER: left in the buffer by WorldMap_TilesB1+0x10c3
	.global WorldMap_TilesB3
WorldMap_TilesB3:
	.incbin "games/tla/src/graphics/world_map/tiles_b3.8bpp.lz"
	.byte 0x1c	@ PACKER: left in the buffer by WorldMap_TilesB2+0xa07
	.global WorldMap_TilesB4
WorldMap_TilesB4:
	.incbin "games/tla/src/graphics/world_map/tiles_b4.8bpp.lz"
	.global WorldMap_TilesB5
WorldMap_TilesB5:
	.incbin "games/tla/src/graphics/world_map/tiles_b5.8bpp.lz"
	.global WorldMap_TilesB6
WorldMap_TilesB6:
	.incbin "games/tla/src/graphics/world_map/tiles_b6.8bpp.lz"
	.byte 0x0b, 0x4c	@ PACKER: left in the buffer by WorldMap_TilesB5+0xf92
	.global WorldMap_PaletteC
WorldMap_PaletteC:
	.incbin "games/tla/src/graphics/world_map/palette_c.gbapal.plz"
	.section .rodata.a
@ World map C cell picture attributes.
	.global Resource_Data1AE
Resource_Data1AE:
	.incbin "games/tla/src/graphics/world_map/attr_c.table.lz"
	.byte 0xef, 0x01, 0x7d	@ PACKER: left in the buffer by WorldMap_TilesB6+0x45d
	.global WorldMap_TilesC1
WorldMap_TilesC1:
	.incbin "games/tla/src/graphics/world_map/tiles_c1.8bpp.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global WorldMap_TilesC2
WorldMap_TilesC2:
	.incbin "games/tla/src/graphics/world_map/tiles_c2.8bpp.lz"
	.byte 0xc8, 0x74	@ PACKER: left in the buffer by WorldMap_TilesC1+0x146e
	.global WorldMap_TilesC3
WorldMap_TilesC3:
	.incbin "games/tla/src/graphics/world_map/tiles_c3.8bpp.lz"
	.byte 0x05, 0xf0	@ PACKER: left in the buffer by WorldMap_TilesC2+0x8f2
	.global WorldMap_TilesC4
WorldMap_TilesC4:
	.incbin "games/tla/src/graphics/world_map/tiles_c4.8bpp.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
	.global WorldMap_TilesC5
WorldMap_TilesC5:
	.incbin "games/tla/src/graphics/world_map/tiles_c5.8bpp.lz"
	.byte 0xec, 0xab	@ PACKER: left in the buffer by WorldMap_TilesC2+0xeee
	.global WorldMap_TilesC6
WorldMap_TilesC6:
	.incbin "games/tla/src/graphics/world_map/tiles_c6.8bpp.lz"
	.byte 0x21	@ PACKER: left in the buffer by WorldMap_TilesC5+0xbeb
	.global WorldMap_PaletteD
WorldMap_PaletteD:
	.incbin "games/tla/src/graphics/world_map/palette_d.gbapal.plz"
	.byte 0x40, 0x92	@ PACKER: left in the buffer by WorldMap_TilesC6+0x1da
@ World map D animated tiles.
	.global Resource_Data1B6
Resource_Data1B6:
	.incbin "games/tla/src/graphics/world_map/anim_d.script.lz"
	.balign 4, 0	@ PACKER: zero fill, not an earlier stream's bytes
@ World map D cell picture attributes.
	.global Resource_Data1B7
Resource_Data1B7:
	.incbin "games/tla/src/graphics/world_map/attr_d.table.lz"
	.global WorldMap_TilesD1
WorldMap_TilesD1:
	.incbin "games/tla/src/graphics/world_map/tiles_d1.8bpp.lz"
	.byte 0xc2, 0x44	@ PACKER: left in the buffer by WorldMap_TilesC6+0x896
	.global WorldMap_TilesD2
WorldMap_TilesD2:
	.incbin "games/tla/src/graphics/world_map/tiles_d2.8bpp.lz"
	.global WorldMap_TilesD3
WorldMap_TilesD3:
	.incbin "games/tla/src/graphics/world_map/tiles_d3.8bpp.lz"
	.byte 0xe6, 0xf4, 0x80	@ PACKER: left in the buffer by WorldMap_TilesD2+0xf3d
	.global WorldMap_TilesD4
WorldMap_TilesD4:
	.incbin "games/tla/src/graphics/world_map/tiles_d4.8bpp.lz"
	.byte 0x84	@ PACKER: left in the buffer by WorldMap_TilesD3+0x5cf
	.global WorldMap_TilesD5
WorldMap_TilesD5:
	.incbin "games/tla/src/graphics/world_map/tiles_d5.8bpp.lz"
	.byte 0xc7	@ PACKER: left in the buffer by WorldMap_TilesD4+0x49f
	.global WorldMap_TilesD6
WorldMap_TilesD6:
	.incbin "games/tla/src/graphics/world_map/tiles_d6.8bpp.lz"
	.byte 0x2c, 0xa2	@ PACKER: left in the buffer by WorldMap_TilesD5+0x352
