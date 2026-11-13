	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map106
Field_Map106:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map106, .LField_Map106_CELL_PICTURES
	map_block Field_Map106, .LField_Map106_SHAPES
	map_block Field_Map106, .LField_Map106_GRID
	map_block Field_Map106, .LField_Map106_SHAPE_GRID
	map_block Field_Map106, .LField_Map106_ANIMATION
	map_block Field_Map106, .LField_Map106_BLEND
	map_block Field_Map106, .LField_Map106_SCENE
.LField_Map106_CELL_PICTURES:
	.incbin "games/tla/src/map/map_106/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map106_SHAPES:
	.incbin "games/tla/src/map/map_106/shapes.table.lz"
	.balign 4, 0
.LField_Map106_GRID:
	.incbin "games/tla/src/map/map_106/map.grid.lz"
	.balign 4, 0
.LField_Map106_SHAPE_GRID:
	.incbin "games/tla/src/map/map_106/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map106_ANIMATION:
	.incbin "games/tla/src/map/map_096/animation.script.lz"
	.balign 4, 0
.LField_Map106_BLEND:
	.incbin "games/tla/src/map/map_010/blend.script.lz"
	.balign 4, 0
.LField_Map106_SCENE:
	.incbin "games/tla/src/map/map_106/scene.scene.lz"
	.global Tileset_Palette13
Tileset_Palette13:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_13.gbapal.lz"
	.byte 0xa0, 0x05	@ PACKER: left in the buffer by Field_Map106+0x1e2
