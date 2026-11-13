	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map257
Field_Map257:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map257, .LField_Map257_CELL_PICTURES
	map_block Field_Map257, .LField_Map257_SHAPES
	map_block Field_Map257, .LField_Map257_GRID
	map_block Field_Map257, .LField_Map257_SHAPE_GRID
	map_block Field_Map257, .LField_Map257_ANIMATION
	map_block Field_Map257, .LField_Map257_BLEND
	map_block Field_Map257, .LField_Map257_SCENE
.LField_Map257_CELL_PICTURES:
	.incbin "games/tla/src/map/map_257/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map257_SHAPES:
	.incbin "games/tla/src/map/map_257/shapes.table.lz"
	.balign 4, 0
.LField_Map257_GRID:
	.incbin "games/tla/src/map/map_257/map.grid.lz"
	.balign 4, 0
.LField_Map257_SHAPE_GRID:
	.incbin "games/tla/src/map/map_257/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map257_ANIMATION:
	.incbin "games/tla/src/map/map_206/animation.script.lz"
	.balign 4, 0
.LField_Map257_BLEND:
	.incbin "games/tla/src/map/map_010/blend.script.lz"
	.balign 4, 0
.LField_Map257_SCENE:
	.incbin "games/tla/src/map/map_257/scene.parts.lz"
	.balign 4, 0
	.global Tileset_Palette60
Tileset_Palette60:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_60.gbapal.lz"
	.byte 0x0f	@ PACKER: left in the buffer by Field_Map257+0x1b7
	.global Tileset_Palette61
Tileset_Palette61:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_61.gbapal.lz"
	.byte 0xb4	@ PACKER: left in the buffer by Tileset_Palette60+0x187
