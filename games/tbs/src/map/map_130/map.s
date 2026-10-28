	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map130
Field_Map130:
	map_header 0, 0, 94, 94, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 96, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_Map130, .LField_Map130_CELL_PICTURES
	map_offset Field_Map130, .LField_Map130_SHAPES
	map_offset Field_Map130, .LField_Map130_GRID
	map_offset Field_Map130, .LField_Map130_ANIMATION
	map_offset Field_Map130, .LField_Map130_BLEND
	map_offset Field_Map130, .LField_Map130_MARKERS
.LField_Map130_CELL_PICTURES:
	.incbin "games/tbs/src/map/map_130/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map130_SHAPES:
	.incbin "games/tbs/src/map/map_130/shapes.table.lz"
	.balign 4, 0
.LField_Map130_GRID:
	.incbin "games/tbs/src/map/map_130/map.grid.lz"
	.balign 4, 0
.LField_Map130_ANIMATION:
	.incbin "games/tbs/src/map/map_130/tileset_anims.script.lz"
	.balign 4, 0
.LField_Map130_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map130_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_Palette31
Tileset_Palette31:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_31.gbapal.lz"
	.byte 0xc8, 0x1f, 0x81	@ PACKER: left in the buffer by Field_Map130+0x159
