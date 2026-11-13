	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map092
Field_Map092:
	map_header 0, 0, 90, 90, 3, 1, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 90, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 90, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map092, .LField_Map092_CELL_PICTURES
	map_block Field_Map092, .LField_Map092_SHAPES
	map_block Field_Map092, .LField_Map092_GRID
	map_block Field_Map092, .LField_Map092_SHAPE_GRID
	map_block Field_Map092, .LField_Map092_ANIMATION
	map_block Field_Map092, .LField_Map092_BLEND
	map_block Field_Map092, .LField_Map092_SCENE
.LField_Map092_CELL_PICTURES:
	.incbin "games/tla/src/map/map_092/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map092_SHAPES:
	.incbin "games/tla/src/map/map_092/shapes.table.lz"
	.balign 4, 0
.LField_Map092_GRID:
	.incbin "games/tla/src/map/map_092/map.grid.lz"
	.balign 4, 0
.LField_Map092_SHAPE_GRID:
	.incbin "games/tla/src/map/map_092/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map092_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map092_BLEND:
	.incbin "games/tla/src/map/map_010/blend.script.lz"
	.balign 4, 0
.LField_Map092_SCENE:
	.incbin "games/tla/src/map/map_092/scene.scene.lz"
	.balign 4, 0
	.global Tileset_Set44Palette
Tileset_Set44Palette:
	.incbin "games/tla/src/graphics/tileset/set_44/palette.gbapal.lz"
	.global Tileset_Set44TilesA
Tileset_Set44TilesA:
	.incbin "games/tla/src/graphics/tileset/set_44/tiles_a.4bpp.mtf"
	.byte 0xa0, 0x32, 0x14	@ PACKER: left in the buffer by Tileset_Set43TilesB+0x245d
	.global Tileset_Set44TilesB
Tileset_Set44TilesB:
	.incbin "games/tla/src/graphics/tileset/set_44/tiles_b.4bpp.mtf"
	.byte 0x9a, 0xa6, 0xe3	@ PACKER: left in the buffer by Tileset_Set44TilesA+0x1ce9
	.global Tileset_Set44TilesC
Tileset_Set44TilesC:
	.incbin "games/tla/src/graphics/tileset/set_44/tiles_c.4bpp.mtf"
	.byte 0x88, 0xf0, 0x05	@ PACKER: left in the buffer by Tileset_Set44TilesB+0x16fd
	.global Tileset_Set44TilesD
Tileset_Set44TilesD:
	.incbin "games/tla/src/graphics/tileset/set_44/tiles_d.4bpp.mtf"
	.byte 0xf6, 0x81, 0x3a	@ PACKER: left in the buffer by Tileset_Set44TilesC+0x1579
