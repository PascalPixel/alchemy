	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map033
Field_Map033:
	map_header 0, 0, 72, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 80, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map033, .LField_Map033_CELL_PICTURES
	map_offset Field_Map033, .LField_Map033_SHAPES
	map_offset Field_Map033, .LField_Map033_GRID
	map_offset Field_Map033, .LField_Map033_ANIMATION
	map_offset Field_Map033, .LField_Map033_BLEND
	map_offset Field_Map033, .LField_Map033_MARKERS
.LField_Map033_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_033/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map033_SHAPES:
	.incbin "games/tbs/src/map/map_033/shapes.table.lz"
	.balign 4, 0
.LField_Map033_GRID:
	.incbin "games/tbs/src/map/map_033/map.grid.lz"
	.balign 4, 0
.LField_Map033_ANIMATION:
	.incbin "games/tbs/src/map/map_033/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map033_BLEND:
	.incbin "games/tbs/src/map/map_033/blend.script.lz"
	.balign 4, 0
.LField_Map033_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Set25Palette
Tileset_Set25Palette:
	.incbin "games/tbs/src/graphics/tileset/set_25/palette.gbapal.lz"
	.byte 0xf3	@ PACKER: left in the buffer by Field_Map033+0x15f
	.global Tileset_Set25TilesA
Tileset_Set25TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_25/tiles_a.4bpp.mtf"
	.byte 0x06	@ PACKER: left in the buffer by Field_Map032+0x1c57
