	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map182
Field_Map182:
	map_header 0, 0, 128, 128, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map182, .LField_Map182_CELL_PICTURES
	map_block Field_Map182, .LField_Map182_SHAPES
	map_block Field_Map182, .LField_Map182_GRID
	map_block Field_Map182, .LField_Map182_SHAPE_GRID
	map_block Field_Map182, .LField_Map182_ANIMATION
	map_block Field_Map182, .LField_Map182_BLEND
	map_block Field_Map182, .LField_Map182_SCENE
.LField_Map182_CELL_PICTURES:
	.incbin "games/tla/src/map/map_182/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map182_SHAPES:
	.incbin "games/tla/src/map/map_182/shapes.table.lz"
	.balign 4, 0
.LField_Map182_GRID:
	.incbin "games/tla/src/map/map_182/map.grid.lz"
	.balign 4, 0
.LField_Map182_SHAPE_GRID:
	.incbin "games/tla/src/map/map_182/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map182_ANIMATION:
	.incbin "games/tla/src/map/map_182/animation.script.lz"
	.balign 4, 0
.LField_Map182_BLEND:
	.incbin "games/tla/src/map/map_030/blend.script.lz"
	.balign 4, 0
.LField_Map182_SCENE:
	.incbin "games/tla/src/map/map_182/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Palette33
Tileset_Palette33:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_33.gbapal.lz"
	.byte 0x85, 0x1e	@ PACKER: left in the buffer by Field_Map182+0x12a
