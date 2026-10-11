	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map019
Field_Map019:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map019, .LField_Map019_METATILES
	map_block Field_Map019, .LField_Map019_SHAPES
	map_block Field_Map019, .LField_Map019_GRID
	map_block Field_Map019, .LField_Map019_ANIMATION
	map_block Field_Map019, .LField_Map019_BLEND
	map_block Field_Map019, .LField_Map019_MARKERS
.LField_Map019_METATILES:
	.incbin "map/map_019/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map019_SHAPES:
	.incbin "common/map/map_001/shapes.table.lz"
	.balign 4, 0
.LField_Map019_GRID:
	.incbin "map/map_019/grid.parts.lz"
	.balign 4, 0
.LField_Map019_ANIMATION:
	.incbin "map/map_019/animation.script.lz"
	.balign 4, 0
.LField_Map019_BLEND:
	.incbin "map/map_010/blend.script.lz"
	.balign 4, 0
.LField_Map019_MARKERS:
	.incbin "map/map_019/markers.table"
	.balign 4, 0
	.global Tileset_Set16Palette
Tileset_Set16Palette:
	.incbin "graphics/tileset/set_16/palette.gbapal.lz"
	.byte 0x3f	@ PACKER: left in the buffer by Field_Map019+0xf3
	.global Tileset_Set16TilesA
Tileset_Set16TilesA:
	.incbin "graphics/tileset/set_16/tiles_a.4bpp.mtf"
	.global Tileset_Set16TilesB
Tileset_Set16TilesB:
	.incbin "graphics/tileset/set_16/tiles_b.4bpp.mtf"
	.global Tileset_Set16TilesC
Tileset_Set16TilesC:
	.incbin "graphics/tileset/set_16/tiles_c.4bpp.mtf"
	.byte 0x1d, 0x65	@ PACKER: left in the buffer by Tileset_Set16TilesB+0x186
	.global Tileset_Set16TilesD
Tileset_Set16TilesD:
	.incbin "graphics/tileset/set_16/tiles_d.4bpp.mtf"
	.byte 0x82, 0xb0	@ PACKER: left in the buffer by Tileset_Set16TilesA+0xda2
