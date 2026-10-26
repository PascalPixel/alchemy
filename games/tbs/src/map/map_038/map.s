	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map038
Field_Map038:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map038, .LField_Map038_METATILES
	map_offset Field_Map038, .LField_Map038_SHAPES
	map_offset Field_Map038, .LField_Map038_GRID
	map_offset Field_Map038, .LField_Map038_ANIMATION
	map_offset Field_Map038, .LField_Map038_BLEND
	map_offset Field_Map038, .LField_Map038_MARKERS
.LField_Map038_METATILES:
	.incbin "games/tbs/src/map/map_038/metatiles.delta2.lz"
	.balign 4, 0
.LField_Map038_SHAPES:
	.incbin "games/tbs/src/map/map_038/shapes.table.lz"
	.balign 4, 0
.LField_Map038_GRID:
	.incbin "games/tbs/src/map/map_038/map.grid.lz"
	.balign 4, 0
.LField_Map038_ANIMATION:
	.incbin "games/tbs/src/map/map_018/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map038_BLEND:
	.incbin "games/tbs/src/map/map_018/blend.script.lz"
	.balign 4, 0
.LField_Map038_MARKERS:
	treasure_spot 5, 7, 200
	treasure_spot 6, 7, 200
	treasure_spot 4, 23, 100
	treasure_spot 25, 26, 201
	treasure_spot 26, 26, 201
	treasure_spot 27, 26, 201
	treasure_spot 24, 37, 202
	treasure_spot 25, 37, 202
	treasure_spot 26, 37, 202
	treasure_spot 22, 39, 101
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Set30Palette
Tileset_Set30Palette:
	.incbin "games/tbs/src/graphics/tileset/set_30/palette.gbapal.lz"
	.byte 0x8e, 0x9e, 0x21	@ PACKER: left in the buffer by Field_Map038+0x1a1
	.global Tileset_Set30TilesA
Tileset_Set30TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_30/tiles_a.4bpp.mtf"
	.byte 0x21, 0x2e	@ PACKER: left in the buffer by Tileset_Set29TilesC+0x1902
