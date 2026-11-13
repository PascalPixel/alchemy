	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map233
Field_Map233:
	map_header 0, 0, 84, 240, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 84, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 168, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map233, .LField_Map233_CELL_PICTURES
	map_block Field_Map233, .LField_Map233_SHAPES
	map_block Field_Map233, .LField_Map233_GRID
	map_block Field_Map233, .LField_Map233_SHAPE_GRID
	map_block Field_Map233, .LField_Map233_ANIMATION
	map_block Field_Map233, .LField_Map233_BLEND
	map_block Field_Map233, .LField_Map233_SCENE
.LField_Map233_CELL_PICTURES:
	.incbin "games/tla/src/map/map_233/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Map233_SHAPES:
	.incbin "games/tla/src/map/map_233/shapes.table.lz"
	.balign 4, 0
.LField_Map233_GRID:
	.incbin "games/tla/src/map/map_233/map.grid.lz"
	.balign 4, 0
.LField_Map233_SHAPE_GRID:
	.incbin "games/tla/src/map/map_233/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map233_ANIMATION:
	.incbin "games/tla/src/map/map_233/animation.script.lz"
	.balign 4, 0
.LField_Map233_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map233_SCENE:
	.incbin "games/tla/src/map/map_233/scene.parts.lz"
	.balign 4, 0
	.global Tileset_Set100Palette
Tileset_Set100Palette:
	.incbin "games/tla/src/graphics/tileset/set_100/palette.gbapal.lz"
	.byte 0xfa	@ PACKER: left in the buffer by Field_Map233+0x13f
	.global Tileset_Set100TilesA
Tileset_Set100TilesA:
	.incbin "games/tla/src/graphics/tileset/set_100/tiles_a.4bpp.mtf"
	.global Tileset_Set100TilesB
Tileset_Set100TilesB:
	.incbin "games/tla/src/graphics/tileset/set_100/tiles_b.4bpp.mtf"
	.byte 0xc7	@ PACKER: left in the buffer by Tileset_Set100TilesA+0x1acb
	.global Tileset_Set100TilesC
Tileset_Set100TilesC:
	.incbin "games/tla/src/graphics/tileset/set_100/tiles_c.4bpp.mtf"
	.byte 0x7b, 0xcf	@ PACKER: left in the buffer by Tileset_Set100TilesB+0x71e
	.global Tileset_Set100TilesD
Tileset_Set100TilesD:
	.incbin "games/tla/src/graphics/tileset/set_100/tiles_d.4bpp.mtf"
	.byte 0x12, 0x05, 0x4a	@ PACKER: left in the buffer by Tileset_Set100TilesB+0x13bd
