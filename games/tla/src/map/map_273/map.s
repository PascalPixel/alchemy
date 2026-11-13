	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map273
Field_Map273:
	map_header 0, 0, 204, 64, 3, 3, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 12, 12, -2, -2, 0xf, 0xf
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map273, .LField_Map273_CELL_PICTURES
	map_block Field_Map273, .LField_Map273_SHAPES
	map_block Field_Map273, .LField_Map273_GRID
	map_block Field_Map273, .LField_Map273_SHAPE_GRID
	map_block Field_Map273, .LField_Map273_ANIMATION
	map_block Field_Map273
	map_block Field_Map273, .LField_Map273_SCENE
.LField_Map273_CELL_PICTURES:
	.incbin "games/tla/src/map/map_273/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map273_SHAPES:
	.incbin "games/tla/src/map/map_273/shapes.table.lz"
	.balign 4, 0
.LField_Map273_GRID:
	.incbin "games/tla/src/map/map_273/map.grid.lz"
	.balign 4, 0
.LField_Map273_SHAPE_GRID:
	.incbin "games/tla/src/map/map_273/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map273_ANIMATION:
	.incbin "games/tla/src/map/map_263/animation.script.lz"
	.balign 4, 0
.LField_Map273_SCENE:
	.incbin "games/tla/src/map/map_273/scene.scene.lz"
	.global Tileset_Map273TilesD
Tileset_Map273TilesD:
	.incbin "games/tla/src/graphics/tileset/map_273/tiles_d.4bpp.mtf"
	.byte 0x02, 0x19, 0xdd	@ PACKER: left in the buffer by Field_Map273+0x7cd
