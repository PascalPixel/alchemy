	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map265
Field_Map265:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map265, .LField_Map265_CELL_PICTURES
	map_block Field_Map265, .LField_Map265_SHAPES
	map_block Field_Map265, .LField_Map265_GRID
	map_block Field_Map265, .LField_Map265_SHAPE_GRID
	map_block Field_Map265, .LField_Map265_ANIMATION
	map_block Field_Map265, .LField_Map265_BLEND
	map_block Field_Map265, .LField_Map265_SCENE
.LField_Map265_CELL_PICTURES:
	.incbin "games/tla/src/map/map_265/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map265_SHAPES:
	.incbin "games/tla/src/map/map_265/shapes.table.lz"
	.balign 4, 0
.LField_Map265_GRID:
	.incbin "games/tla/src/map/map_265/map.grid.lz"
	.balign 4, 0
.LField_Map265_SHAPE_GRID:
	.incbin "games/tla/src/map/map_265/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map265_ANIMATION:
	.incbin "games/tla/src/map/map_259/animation.script.lz"
	.balign 4, 0
.LField_Map265_BLEND:
	.incbin "games/tla/src/map/map_260/blend.script.lz"
	.balign 4, 0
.LField_Map265_SCENE:
	.incbin "games/tla/src/map/map_265/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set111Palette
Tileset_Set111Palette:
	.incbin "games/tla/src/graphics/tileset/set_111/palette.gbapal.lz"
	.byte 0x05, 0x18	@ PACKER: left in the buffer by Field_Map265+0x1b2
	.global Tileset_Set111TilesA
Tileset_Set111TilesA:
	.incbin "games/tla/src/graphics/tileset/set_111/tiles_a.4bpp.mtf"
	.byte 0x23, 0x44, 0x28	@ PACKER: left in the buffer by Tileset_Set110TilesA+0x223d
	.global Tileset_Set111TilesB
Tileset_Set111TilesB:
	.incbin "games/tla/src/graphics/tileset/set_111/tiles_b.4bpp.mtf"
	.byte 0x8e	@ PACKER: left in the buffer by Tileset_Set111TilesA+0xb23
