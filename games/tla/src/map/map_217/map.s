	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map217
Field_Map217:
	map_header 0, 0, 128, 128, 2, 1, 3, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map217, .LField_Map217_CELL_PICTURES
	map_block Field_Map217, .LField_Map217_SHAPES
	map_block Field_Map217, .LField_Map217_GRID
	map_block Field_Map217, .LField_Map217_SHAPE_GRID
	map_block Field_Map217, .LField_Map217_ANIMATION
	map_block Field_Map217
	map_block Field_Map217, .LField_Map217_SCENE
.LField_Map217_CELL_PICTURES:
	.incbin "games/tla/src/map/map_217/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map217_SHAPES:
	.incbin "games/tla/src/map/map_217/shapes.table.lz"
	.balign 4, 0
.LField_Map217_GRID:
	.incbin "games/tla/src/map/map_217/map.grid.lz"
	.balign 4, 0
.LField_Map217_SHAPE_GRID:
	.incbin "games/tla/src/map/map_217/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map217_ANIMATION:
	.incbin "games/tla/src/map/map_217/animation.script.lz"
	.balign 4, 0
.LField_Map217_SCENE:
	.incbin "games/tla/src/map/map_217/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Palette48
Tileset_Palette48:
	.incbin "games/tla/src/graphics/tileset/palettes/pal_48.gbapal.lz"
	.byte 0x19	@ PACKER: left in the buffer by Field_Map217+0x167
