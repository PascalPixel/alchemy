	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map049
Field_Map049:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map049, .LField_Map049_CELL_PICTURES
	map_offset Field_Map049, .LField_Map049_SHAPES
	map_offset Field_Map049, .LField_Map049_GRID
	map_offset Field_Map049, .LField_Map049_ANIMATION
	map_offset Field_Map049
	map_offset Field_Map049, .LField_Map049_MARKERS
.LField_Map049_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_049/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map049_SHAPES:
	.incbin "games/tbs/src/map/map_049/shapes.table.lz"
	.balign 4, 0
.LField_Map049_GRID:
	.incbin "games/tbs/src/map/map_049/map.grid.lz"
	.balign 4, 0
.LField_Map049_ANIMATION:
	.incbin "games/tbs/src/map/map_033/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map049_MARKERS:
	treasure_spot 35, 28, 100
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Set39Palette
Tileset_Set39Palette:
	.incbin "games/tbs/src/graphics/tileset/set_39/palette.gbapal.lz"
	.byte 0xdc, 0x32	@ PACKER: left in the buffer by Field_Map049+0x18e
	.global Tileset_Set39TilesA
Tileset_Set39TilesA:
	.incbin "games/tbs/src/graphics/tileset/set_39/tiles_a.4bpp.mtf"
	.byte 0x71	@ PACKER: left in the buffer by Tileset_Set37TilesC+0x22b7
