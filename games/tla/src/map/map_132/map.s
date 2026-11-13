	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map132
Field_Map132:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map132, .LField_Map132_CELL_PICTURES
	map_block Field_Map132, .LField_Map132_SHAPES
	map_block Field_Map132, .LField_Map132_GRID
	map_block Field_Map132, .LField_Map132_SHAPE_GRID
	map_block Field_Map132, .LField_Map132_ANIMATION
	map_block Field_Map132, .LField_Map132_BLEND
	map_block Field_Map132, .LField_Map132_SCENE
.LField_Map132_CELL_PICTURES:
	.incbin "games/tla/src/map/map_132/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map132_SHAPES:
	.incbin "games/tla/src/map/map_132/shapes.table.lz"
	.balign 4, 0
.LField_Map132_GRID:
	.incbin "games/tla/src/map/map_132/map.grid.lz"
	.balign 4, 0
.LField_Map132_SHAPE_GRID:
	.incbin "games/tla/src/map/map_132/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map132_ANIMATION:
	.incbin "games/tla/src/map/map_041/animation.script.lz"
	.balign 4, 0
.LField_Map132_BLEND:
	.incbin "games/tla/src/map/map_010/blend.script.lz"
	.balign 4, 0
.LField_Map132_SCENE:
	.incbin "games/tla/src/map/map_132/scene.parts.lz"
	.balign 4, 0
	.global Tileset_Palette25
Tileset_Palette25:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_25.gbapal.lz"
	.byte 0xf6, 0x1c, 0xf7	@ PACKER: left in the buffer by Field_Map132+0x199
	.global Tileset_Palette26
Tileset_Palette26:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_26.gbapal.lz"
	.byte 0x44	@ PACKER: left in the buffer by Tileset_Palette25+0x15b
