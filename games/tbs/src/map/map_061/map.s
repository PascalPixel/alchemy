	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map061
Field_Map061:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map061, .LField_Map061_METATILES
	map_offset Field_Map061, .LField_Map061_SHAPES
	map_offset Field_Map061, .LField_Map061_GRID
	map_offset Field_Map061, .LField_Map061_ANIMATION
	map_offset Field_Map061, .LField_Map061_BLEND
	map_offset Field_Map061, .LField_Map061_MARKERS
.LField_Map061_METATILES:
	.incbin "games/tbs/src/map/map_061/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map061_SHAPES:
	.incbin "games/tbs/src/map/map_061/shapes.table.lz"
	.balign 4, 0
.LField_Map061_GRID:
	.incbin "games/tbs/src/map/map_061/map.grid.lz"
	.balign 4, 0
.LField_Map061_ANIMATION:
	.incbin "games/tbs/src/map/map_061/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map061_BLEND:
	.incbin "games/tbs/src/map/map_061/blend.script.lz"
	.balign 4, 0
.LField_Map061_MARKERS:
	treasure_spot 6, 14, 100
	treasure_spot 12, 27, 101
	treasure_spot 27, 27, 102
	treasure_spots_end
	.global Tileset_Set45Palette
Tileset_Set45Palette:
	.incbin "games/tbs/src/graphics/tileset/set_45/palette.gbapal.lz"
	.byte 0xa8, 0x80, 0x60	@ PACKER: left in the buffer by Field_Map061+0x1d9
	.global Tileset_Set45TilesA
Tileset_Set45TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_45/tiles_a.4bpp.mtf"
	.byte 0x13, 0x00	@ PACKER: left in the buffer by Field_Map060+0x24ee
	.global Tileset_Set45TilesB
Tileset_Set45TilesB:
	.incbin "games/tbs/src/graphics/tileset/set_45/tiles_b.4bpp.mtf"
	.global Tileset_Set45TilesC
Tileset_Set45TilesC:
	.incbin "games/tbs/src/graphics/tileset/set_45/tiles_c.4bpp.mtf"
	.byte 0x18	@ PACKER: left in the buffer by Tileset_Set45TilesB+0x1573
	.global Tileset_Set45TilesD
Tileset_Set45TilesD:
	.incbin "games/tbs/src/graphics/tileset/set_45/tiles_d.4bpp.mtf"
	.byte 0xfb, 0x14, 0x4d	@ PACKER: left in the buffer by Tileset_Set45TilesC+0xb99
