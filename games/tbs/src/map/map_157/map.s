	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_Map157
Field_Map157:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map157, .LField_Map157_METATILES
	map_block Field_Map157, .LField_Map157_SHAPES
	map_block Field_Map157, .LField_Map157_GRID
	map_block Field_Map157, .LField_Map157_ANIMATION
	map_block Field_Map157, .LField_Map157_BLEND
	map_block Field_Map157, .LField_Map157_MARKERS
.LField_Map157_METATILES:
	.incbin "games/tbs/src/map/map_157/metatiles.delta1.lz"
	.balign 4, 0
.LField_Map157_SHAPES:
	.incbin "games/tbs/src/map/map_157/shapes.table.lz"
	.balign 4, 0
.LField_Map157_GRID:
	.incbin "games/tbs/src/map/map_157/grid.parts.lz"
	.balign 4, 0
.LField_Map157_ANIMATION:
	.incbin "games/tbs/src/map/map_037/animation.script.lz"
	.balign 4, 0
.LField_Map157_BLEND:
	.incbin "games/tbs/src/map/map_020/blend.script.lz"
	.balign 4, 0
.LField_Map157_MARKERS:
	.incbin "games/tbs/src/map/map_157/markers.table"
	.balign 4, 0
	.global Tileset_Palette44
Tileset_Palette44:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_44.gbapal.lz"
	.byte 0x32	@ PACKER: left in the buffer by Field_Map157+0x173
	.global Tileset_Palette45
Tileset_Palette45:
	.incbin "games/tbs/src/graphics/tileset/palettes/pal_45.gbapal.lz"
