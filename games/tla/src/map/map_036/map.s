	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map036
Field_Map036:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map036, .LField_Map036_CELL_PICTURES
	map_block Field_Map036, .LField_Map036_SHAPES
	map_block Field_Map036, .LField_Map036_GRID
	map_block Field_Map036, .LField_Map036_SHAPE_GRID
	map_block Field_Map036, .LField_Map036_ANIMATION
	map_block Field_Map036, .LField_Map036_BLEND
	map_block Field_Map036, .LField_Map036_SCENE
.LField_Map036_CELL_PICTURES:
	.incbin "games/tla/src/map/map_036/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Map036_SHAPES:
	.incbin "games/tla/src/map/map_036/shapes.table.lz"
	.balign 4, 0
.LField_Map036_GRID:
	.incbin "games/tla/src/map/map_036/map.grid.lz"
	.balign 4, 0
.LField_Map036_SHAPE_GRID:
	.incbin "games/tla/src/map/map_036/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map036_ANIMATION:
	.incbin "games/tla/src/map/map_035/animation.script.lz"
	.balign 4, 0
.LField_Map036_BLEND:
	.incbin "games/tla/src/map/map_035/blend.script.lz"
	.balign 4, 0
.LField_Map036_SCENE:
	.incbin "games/tla/src/map/map_036/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set19Palette
Tileset_Set19Palette:
	.incbin "games/tla/src/graphics/tileset/set_19/palette.gbapal.lz"
	.global Tileset_Set19TilesA
Tileset_Set19TilesA:
	.incbin "games/tla/src/graphics/tileset/set_19/tiles_a.4bpp.mtf"
	.byte 0xf8, 0x3e	@ PACKER: left in the buffer by Tileset_Set18TilesA+0x1b56
	.global Tileset_Set19TilesB
Tileset_Set19TilesB:
	.incbin "games/tla/src/graphics/tileset/set_19/tiles_b.4bpp.mtf"
	.global Tileset_Set19TilesC
Tileset_Set19TilesC:
	.incbin "games/tla/src/graphics/tileset/set_19/tiles_c.4bpp.mtf"
	.byte 0x83, 0xb4, 0x9c	@ PACKER: left in the buffer by Tileset_Set19TilesB+0x1f5
	.global Tileset_Set19TilesD
Tileset_Set19TilesD:
	.incbin "games/tla/src/graphics/tileset/set_19/tiles_d.4bpp.mtf"
	.byte 0x1f, 0xf8	@ PACKER: left in the buffer by Tileset_Set19TilesC+0x1be
