	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map131
Field_Map131:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map131, .LField_Map131_CELL_PICTURES
	map_block Field_Map131, .LField_Map131_SHAPES
	map_block Field_Map131, .LField_Map131_GRID
	map_block Field_Map131, .LField_Map131_SHAPE_GRID
	map_block Field_Map131, .LField_Map131_ANIMATION
	map_block Field_Map131, .LField_Map131_BLEND
	map_block Field_Map131, .LField_Map131_SCENE
.LField_Map131_CELL_PICTURES:
	.incbin "games/tla/src/map/map_131/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map131_SHAPES:
	.incbin "games/tla/src/map/map_131/shapes.table.lz"
	.balign 4, 0
.LField_Map131_GRID:
	.incbin "games/tla/src/map/map_131/map.grid.lz"
	.balign 4, 0
.LField_Map131_SHAPE_GRID:
	.incbin "games/tla/src/map/map_131/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map131_ANIMATION:
	.incbin "games/tla/src/map/map_131/animation.script.lz"
	.balign 4, 0
.LField_Map131_BLEND:
	.incbin "games/tla/src/map/map_010/blend.script.lz"
	.balign 4, 0
.LField_Map131_SCENE:
	.incbin "games/tla/src/map/map_131/scene.parts.lz"
	.balign 4, 0
	.global Tileset_Set63Palette
Tileset_Set63Palette:
	.incbin "games/tla/src/graphics/tileset/set_63/palette.gbapal.lz"
	.byte 0x04	@ PACKER: left in the buffer by Field_Map131+0x1db
	.global Tileset_Set63TilesA
Tileset_Set63TilesA:
	.incbin "games/tla/src/graphics/tileset/set_63/tiles_a.4bpp.mtf"
	.global Tileset_Set63TilesB
Tileset_Set63TilesB:
	.incbin "games/tla/src/graphics/tileset/set_63/tiles_b.4bpp.mtf"
	.byte 0x0c	@ PACKER: left in the buffer by Field_Map131+0x1aef
	.global Tileset_Set63TilesC
Tileset_Set63TilesC:
	.incbin "games/tla/src/graphics/tileset/set_63/tiles_c.4bpp.mtf"
	.byte 0xbb, 0x1f	@ PACKER: left in the buffer by Tileset_Set63TilesB+0x1482
	.global Tileset_Set63TilesD
Tileset_Set63TilesD:
	.incbin "games/tla/src/graphics/tileset/set_63/tiles_d.4bpp.mtf"
	.byte 0x7b, 0x9c, 0xde	@ PACKER: left in the buffer by Tileset_Set63TilesC+0x104d
