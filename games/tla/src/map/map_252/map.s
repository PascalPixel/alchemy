	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map252
Field_Map252:
	map_header 0, 0, 120, 120, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 120, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 120, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map252, .LField_Map252_CELL_PICTURES
	map_block Field_Map252, .LField_Map252_SHAPES
	map_block Field_Map252, .LField_Map252_GRID
	map_block Field_Map252, .LField_Map252_SHAPE_GRID
	map_block Field_Map252, .LField_Map252_ANIMATION
	map_block Field_Map252, .LField_Map252_BLEND
	map_block Field_Map252, .LField_Map252_SCENE
.LField_Map252_CELL_PICTURES:
	.incbin "games/tla/src/map/map_252/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map252_SHAPES:
	.incbin "games/tla/src/map/map_252/shapes.table.lz"
	.balign 4, 0
.LField_Map252_GRID:
	.incbin "games/tla/src/map/map_252/grid.parts.lz"
	.balign 4, 0
.LField_Map252_SHAPE_GRID:
	.incbin "games/tla/src/map/map_252/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map252_ANIMATION:
	.incbin "games/tla/src/map/map_252/animation.script.lz"
	.balign 4, 0
.LField_Map252_BLEND:
	.incbin "games/tla/src/map/map_010/blend.script.lz"
	.balign 4, 0
.LField_Map252_SCENE:
	.incbin "games/tla/src/map/map_252/scene.parts.lz"
	.balign 4, 0
	.global Tileset_Set106Palette
Tileset_Set106Palette:
	.incbin "games/tla/src/graphics/tileset/set_106/palette.gbapal.lz"
	.global Tileset_Set106TilesA
Tileset_Set106TilesA:
	.incbin "games/tla/src/graphics/tileset/set_106/tiles_a.4bpp.mtf"
	.byte 0x00	@ PACKER: left in the buffer by Field_Map252+0x14fb
	.global Tileset_Set106TilesB
Tileset_Set106TilesB:
	.incbin "games/tla/src/graphics/tileset/set_106/tiles_b.4bpp.mtf"
	.byte 0xb5	@ PACKER: left in the buffer by Field_Map252+0x1bc7
	.global Tileset_Set106TilesC
Tileset_Set106TilesC:
	.incbin "games/tla/src/graphics/tileset/set_106/tiles_c.4bpp.mtf"
	.byte 0x95, 0x3b	@ PACKER: left in the buffer by Tileset_Set106TilesB+0x1b7a
	.global Tileset_Set106TilesD
Tileset_Set106TilesD:
	.incbin "games/tla/src/graphics/tileset/set_106/tiles_d.4bpp.mtf"
	.byte 0x82, 0x94, 0x06	@ PACKER: left in the buffer by Tileset_Set106TilesC+0x449
