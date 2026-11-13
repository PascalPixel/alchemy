	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map116
Field_Map116:
	map_header 0, 0, 100, 80, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map116, .LField_Map116_CELL_PICTURES
	map_block Field_Map116, .LField_Map116_SHAPES
	map_block Field_Map116, .LField_Map116_GRID
	map_block Field_Map116, .LField_Map116_SHAPE_GRID
	map_block Field_Map116, .LField_Map116_ANIMATION
	map_block Field_Map116, .LField_Map116_BLEND
	map_block Field_Map116, .LField_Map116_SCENE
.LField_Map116_CELL_PICTURES:
	.incbin "games/tla/src/map/map_116/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Map116_SHAPES:
	.incbin "games/tla/src/map/map_116/shapes.table.lz"
	.balign 4, 0
.LField_Map116_GRID:
	.incbin "games/tla/src/map/map_116/map.grid.lz"
	.balign 4, 0
.LField_Map116_SHAPE_GRID:
	.incbin "games/tla/src/map/map_116/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map116_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map116_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map116_SCENE:
	.incbin "games/tla/src/map/map_116/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Palette20
Tileset_Palette20:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_20.gbapal.lz"
	.byte 0x5c	@ PACKER: left in the buffer by Field_Map116+0xd3
