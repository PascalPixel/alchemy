	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map150
Field_Map150:
	map_header 0, 0, 130, 112, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map150, .LField_Map150_CELL_PICTURES
	map_block Field_Map150, .LField_Map150_SHAPES
	map_block Field_Map150, .LField_Map150_GRID
	map_block Field_Map150, .LField_Map150_SHAPE_GRID
	map_block Field_Map150, .LField_Map150_ANIMATION
	map_block Field_Map150, .LField_Map150_BLEND
	map_block Field_Map150, .LField_Map150_SCENE
.LField_Map150_CELL_PICTURES:
	.incbin "games/tla/src/map/map_150/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map150_SHAPES:
	.incbin "games/tla/src/map/map_150/shapes.table.lz"
	.balign 4, 0
.LField_Map150_GRID:
	.incbin "games/tla/src/map/map_150/map.grid.lz"
	.balign 4, 0
.LField_Map150_SHAPE_GRID:
	.incbin "games/tla/src/map/map_150/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map150_ANIMATION:
	.incbin "games/tla/src/map/map_150/animation.script.lz"
	.balign 4, 0
.LField_Map150_BLEND:
	.incbin "games/tla/src/map/map_150/blend.script.lz"
	.balign 4, 0
.LField_Map150_SCENE:
	.incbin "games/tla/src/map/map_150/scene.parts.lz"
	.balign 4, 0
	.global Tileset_Set72Palette
Tileset_Set72Palette:
	.incbin "games/tla/src/graphics/tileset/set_72/palette.gbapal.lz"
	.global Tileset_Set72TilesA
Tileset_Set72TilesA:
	.incbin "games/tla/src/graphics/tileset/set_72/tiles_a.4bpp.mtf"
	.byte 0xbd	@ PACKER: left in the buffer by Field_Map149+0x217b
	.global Tileset_Set72TilesB
Tileset_Set72TilesB:
	.incbin "games/tla/src/graphics/tileset/set_72/tiles_b.4bpp.mtf"
	.byte 0x3a	@ PACKER: left in the buffer by Tileset_Set72TilesA+0x1be3
	.global Tileset_Set72TilesC
Tileset_Set72TilesC:
	.incbin "games/tla/src/graphics/tileset/set_72/tiles_c.4bpp.mtf"
	.byte 0x05, 0xb0, 0x73	@ PACKER: left in the buffer by Tileset_Set72TilesA+0x1cf5
	.global Tileset_Set72TilesD
Tileset_Set72TilesD:
	.incbin "games/tla/src/graphics/tileset/set_72/tiles_d.4bpp.mtf"
