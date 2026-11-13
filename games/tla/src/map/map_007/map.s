	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map007
Field_Map007:
	map_header 0, 0, 128, 128, 3, 3, 3, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, -1, -2, 0x1, 0x1
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map007, .LField_Map007_CELL_PICTURES
	map_block Field_Map007, .LField_Map007_SHAPES
	map_block Field_Map007, .LField_Map007_GRID
	map_block Field_Map007, .LField_Map007_SHAPE_GRID
	map_block Field_Map007, .LField_Map007_ANIMATION
	map_block Field_Map007, .LField_Map007_BLEND
	map_block Field_Map007, .LField_Map007_SCENE
.LField_Map007_CELL_PICTURES:
	.incbin "games/tla/src/map/map_007/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map007_SHAPES:
	.incbin "games/tla/src/map/map_007/shapes.table.lz"
	.balign 4, 0
.LField_Map007_GRID:
	.incbin "games/tla/src/map/map_007/map.grid.lz"
	.balign 4, 0
.LField_Map007_SHAPE_GRID:
	.incbin "games/tla/src/map/map_007/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map007_ANIMATION:
	.incbin "games/tla/src/map/map_007/animation.script.lz"
	.balign 4, 0
.LField_Map007_BLEND:
	.incbin "games/tla/src/map/map_007/blend.script.lz"
	.balign 4, 0
.LField_Map007_SCENE:
	.incbin "games/tla/src/map/map_007/scene.parts.lz"
	.balign 4, 0
	.global Tileset_Set05Palette
Tileset_Set05Palette:
	.incbin "games/tla/src/graphics/tileset/set_05/palette.gbapal.lz"
	.byte 0xaa, 0x2e	@ PACKER: left in the buffer by Field_Map007+0xfe
	.global Tileset_Set05TilesA
Tileset_Set05TilesA:
	.incbin "games/tla/src/graphics/tileset/set_05/tiles_a.4bpp.mtf"
	.global Tileset_Set05TilesB
Tileset_Set05TilesB:
	.incbin "games/tla/src/graphics/tileset/set_05/tiles_b.4bpp.mtf"
	.byte 0x04, 0x92, 0x9e	@ PACKER: left in the buffer by Tileset_Set04TilesC+0x1c7d
	.global Tileset_Set05TilesC
Tileset_Set05TilesC:
	.incbin "games/tla/src/graphics/tileset/set_05/tiles_c.4bpp.mtf"
	.global Tileset_Set05TilesD
Tileset_Set05TilesD:
	.incbin "games/tla/src/graphics/tileset/set_05/tiles_d.4bpp.mtf"
	.byte 0x22, 0x60, 0x76	@ PACKER: left in the buffer by Tileset_Set05TilesC+0x8dd
