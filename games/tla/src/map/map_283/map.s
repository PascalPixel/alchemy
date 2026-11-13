	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map283
Field_Map283:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 2, 2, 0xf, 0xf
	map_block Field_Map283, .LField_Map283_CELL_PICTURES
	map_block Field_Map283, .LField_Map283_SHAPES
	map_block Field_Map283, .LField_Map283_GRID
	map_block Field_Map283, .LField_Map283_SHAPE_GRID
	map_block Field_Map283, .LField_Map283_ANIMATION
	map_block Field_Map283, .LField_Map283_BLEND
	map_block Field_Map283, .LField_Map283_SCENE
.LField_Map283_CELL_PICTURES:
	.incbin "games/tla/src/map/map_283/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map283_SHAPES:
	.incbin "games/tla/src/map/map_283/shapes.table.lz"
	.balign 4, 0
.LField_Map283_GRID:
	.incbin "games/tla/src/map/map_283/map.grid.lz"
	.balign 4, 0
.LField_Map283_SHAPE_GRID:
	.incbin "games/tla/src/map/map_283/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map283_ANIMATION:
	.incbin "games/tla/src/map/map_283/animation.script.lz"
	.balign 4, 0
.LField_Map283_BLEND:
	.incbin "games/tla/src/map/map_060/blend.script.lz"
	.balign 4, 0
.LField_Map283_SCENE:
	.incbin "games/tla/src/map/map_283/scene.parts.lz"
	.balign 4, 0
	.global Tileset_Set116Palette
Tileset_Set116Palette:
	.incbin "games/tla/src/graphics/tileset/set_116/palette.gbapal.lz"
	.byte 0x47, 0x90	@ PACKER: left in the buffer by Field_Map283+0x166
	.global Tileset_Set116TilesA
Tileset_Set116TilesA:
	.incbin "games/tla/src/graphics/tileset/set_116/tiles_a.4bpp.mtf"
	.byte 0xf8	@ PACKER: left in the buffer by Field_Map252+0x2fbf
	.global Tileset_Set116TilesB
Tileset_Set116TilesB:
	.incbin "games/tla/src/graphics/tileset/set_116/tiles_b.4bpp.mtf"
	.byte 0xc2, 0x04, 0x38	@ PACKER: left in the buffer by Tileset_Set116TilesA+0x126d
	.global Tileset_Set116TilesC
Tileset_Set116TilesC:
	.incbin "games/tla/src/graphics/tileset/set_116/tiles_c.4bpp.mtf"
	.global Tileset_Set116TilesD
Tileset_Set116TilesD:
	.incbin "games/tla/src/graphics/tileset/set_116/tiles_d.4bpp.mtf"
	.byte 0x81, 0x03	@ PACKER: left in the buffer by Tileset_Set116TilesA+0x1ac2
