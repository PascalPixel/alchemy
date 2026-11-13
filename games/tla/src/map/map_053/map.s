	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map053
Field_Map053:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map053, .LField_Map053_CELL_PICTURES
	map_block Field_Map053, .LField_Map053_SHAPES
	map_block Field_Map053, .LField_Map053_GRID
	map_block Field_Map053, .LField_Map053_SHAPE_GRID
	map_block Field_Map053, .LField_Map053_ANIMATION
	map_block Field_Map053, .LField_Map053_BLEND
	map_block Field_Map053, .LField_Map053_SCENE
.LField_Map053_CELL_PICTURES:
	.incbin "games/tla/src/map/map_053/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map053_SHAPES:
	.incbin "games/tla/src/map/map_053/shapes.table.lz"
	.balign 4, 0
.LField_Map053_GRID:
	.incbin "games/tla/src/map/map_053/map.grid.lz"
	.balign 4, 0
.LField_Map053_SHAPE_GRID:
	.incbin "games/tla/src/map/map_053/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map053_ANIMATION:
	.incbin "games/tla/src/map/map_041/animation.script.lz"
	.balign 4, 0
.LField_Map053_BLEND:
	.incbin "games/tla/src/map/map_010/blend.script.lz"
	.balign 4, 0
.LField_Map053_SCENE:
	.incbin "games/tla/src/map/map_053/scene.parts.lz"
	.global Tileset_Set34Palette
Tileset_Set34Palette:
	.incbin "games/tla/src/graphics/tileset/set_34/palette.gbapal.lz"
	.byte 0xe7, 0x0b	@ PACKER: left in the buffer by Field_Map053+0x186
	.global Tileset_Set34TilesA
Tileset_Set34TilesA:
	.incbin "games/tla/src/graphics/tileset/set_34/tiles_a.4bpp.mtf"
	.byte 0x8c, 0xeb	@ PACKER: left in the buffer by Field_Map053+0x11f6
	.global Tileset_Set34TilesB
Tileset_Set34TilesB:
	.incbin "games/tla/src/graphics/tileset/set_34/tiles_b.4bpp.mtf"
	.byte 0xc8, 0x4f, 0x0d	@ PACKER: left in the buffer by Tileset_Set34TilesA+0xec1
	.global Tileset_Set34TilesC
Tileset_Set34TilesC:
	.incbin "games/tla/src/graphics/tileset/set_34/tiles_c.4bpp.mtf"
	.global Tileset_Set34TilesD
Tileset_Set34TilesD:
	.incbin "games/tla/src/graphics/tileset/set_34/tiles_d.4bpp.mtf"
	.byte 0x20, 0x3c	@ PACKER: left in the buffer by Tileset_Set34TilesC+0xf1e
