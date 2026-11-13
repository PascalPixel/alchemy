	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map003
Field_Map003:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map003, .LField_Map003_CELL_PICTURES
	map_block Field_Map003, .LField_Map003_SHAPES
	map_block Field_Map003, .LField_Map003_GRID
	map_block Field_Map003, .LField_Map003_SHAPE_GRID
	map_block Field_Map003, .LField_Map003_ANIMATION
	map_block Field_Map003, .LField_Map003_BLEND
	map_block Field_Map003, .LField_Map003_SCENE
.LField_Map003_CELL_PICTURES:
	.incbin "games/tla/src/map/map_003/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map003_SHAPES:
	.incbin "games/tla/src/map/map_003/shapes.table.lz"
	.balign 4, 0
.LField_Map003_GRID:
	.incbin "games/tla/src/map/map_003/grid.parts.lz"
	.balign 4, 0
.LField_Map003_SHAPE_GRID:
	.incbin "games/tla/src/map/map_003/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map003_ANIMATION:
	.incbin "games/tla/src/map/map_003/animation.script.lz"
	.balign 4, 0
.LField_Map003_BLEND:
	.incbin "games/tla/src/map/map_003/blend.script.lz"
	.balign 4, 0
.LField_Map003_SCENE:
	.incbin "games/tla/src/map/map_003/scene.parts.lz"
	.balign 4, 0
	.global Tileset_Set02Palette
Tileset_Set02Palette:
	.incbin "games/tla/src/graphics/tileset/set_02/pal_50.gbapal.lz"
	.global Tileset_Set02TilesA
Tileset_Set02TilesA:
	.incbin "games/tla/src/graphics/tileset/set_02/tiles_a.4bpp.mtf"
	.byte 0x14, 0xf0	@ PACKER: left in the buffer by Field_Map002+0x2866
	.global Tileset_Set02TilesB
Tileset_Set02TilesB:
	.incbin "games/tla/src/graphics/tileset/set_02/tiles_b.4bpp.mtf"
	.byte 0xb0	@ PACKER: left in the buffer by Tileset_Set02TilesA+0x27c7
	.global Tileset_Set02TilesC
Tileset_Set02TilesC:
	.incbin "games/tla/src/graphics/tileset/set_02/tiles_c.4bpp.mtf"
	.byte 0x08	@ PACKER: left in the buffer by Tileset_Set02TilesB+0x151f
	.global Tileset_Set02TilesD
Tileset_Set02TilesD:
	.incbin "games/tla/src/graphics/tileset/set_02/tiles_d.4bpp.mtf"
	.byte 0x0c	@ PACKER: left in the buffer by Tileset_Set02TilesB+0x1943
