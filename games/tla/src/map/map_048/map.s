	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map048
Field_Map048:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map048, .LField_Map048_CELL_PICTURES
	map_block Field_Map048, .LField_Map048_SHAPES
	map_block Field_Map048, .LField_Map048_GRID
	map_block Field_Map048, .LField_Map048_SHAPE_GRID
	map_block Field_Map048, .LField_Map048_ANIMATION
	map_block Field_Map048
	map_block Field_Map048, .LField_Map048_SCENE
.LField_Map048_CELL_PICTURES:
	.incbin "games/tla/src/map/map_048/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map048_SHAPES:
	.incbin "games/tla/src/map/map_048/shapes.table.lz"
	.balign 4, 0
.LField_Map048_GRID:
	.incbin "games/tla/src/map/map_048/grid.parts.lz"
	.balign 4, 0
.LField_Map048_SHAPE_GRID:
	.incbin "games/tla/src/map/map_048/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map048_ANIMATION:
	.incbin "games/tla/src/map/map_048/animation.script.lz"
	.balign 4, 0
.LField_Map048_SCENE:
	.incbin "games/tla/src/map/map_048/scene.parts.lz"
	.balign 4, 0
	.global Tileset_Set29Palette
Tileset_Set29Palette:
	.incbin "games/tla/src/graphics/tileset/set_29/palette.gbapal.lz"
	.byte 0x70, 0x28	@ PACKER: left in the buffer by Field_Map048+0x16a
	.global Tileset_Set29TilesA
Tileset_Set29TilesA:
	.incbin "games/tla/src/graphics/tileset/set_29/tiles_a.4bpp.mtf"
	.byte 0x3e	@ PACKER: left in the buffer by Field_Map040+0x2e4f
	.global Tileset_Set29TilesB
Tileset_Set29TilesB:
	.incbin "games/tla/src/graphics/tileset/set_29/tiles_b.4bpp.mtf"
	.byte 0xcf, 0xde	@ PACKER: left in the buffer by Tileset_Set29TilesA+0x27be
	.global Tileset_Set29TilesC
Tileset_Set29TilesC:
	.incbin "games/tla/src/graphics/tileset/set_29/tiles_c.4bpp.mtf"
	.global Tileset_Set29TilesD
Tileset_Set29TilesD:
	.incbin "games/tla/src/graphics/tileset/set_29/tiles_d.4bpp.mtf"
