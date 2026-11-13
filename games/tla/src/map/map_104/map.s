	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map104
Field_Map104:
	map_header 0, 0, 64, 66, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map104, .LField_Map104_CELL_PICTURES
	map_block Field_Map104, .LField_Map104_SHAPES
	map_block Field_Map104, .LField_Map104_GRID
	map_block Field_Map104, .LField_Map104_SHAPE_GRID
	map_block Field_Map104, .LField_Map104_ANIMATION
	map_block Field_Map104, .LField_Map104_BLEND
	map_block Field_Map104, .LField_Map104_SCENE
.LField_Map104_CELL_PICTURES:
	.incbin "games/tla/src/map/map_104/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Map104_SHAPES:
	.incbin "games/tla/src/map/map_104/shapes.table.lz"
	.balign 4, 0
.LField_Map104_GRID:
	.incbin "games/tla/src/map/map_104/grid.parts.lz"
	.balign 4, 0
.LField_Map104_SHAPE_GRID:
	.incbin "games/tla/src/map/map_104/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map104_ANIMATION:
	.incbin "games/tla/src/map/map_012/animation.script.lz"
	.balign 4, 0
.LField_Map104_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map104_SCENE:
	.incbin "games/tla/src/map/map_104/scene.parts.lz"
	.global Tileset_Set53Palette
Tileset_Set53Palette:
	.incbin "games/tla/src/graphics/tileset/set_53/palette.gbapal.lz"
	.byte 0xd0, 0xe0	@ PACKER: left in the buffer by Field_Map104+0x166
	.global Tileset_Set53TilesA
Tileset_Set53TilesA:
	.incbin "games/tla/src/graphics/tileset/set_53/tiles_a.4bpp.mtf"
	.byte 0x0c, 0x09	@ PACKER: left in the buffer by Field_Map104+0x2482
	.global Tileset_Set53TilesB
Tileset_Set53TilesB:
	.incbin "games/tla/src/graphics/tileset/set_53/tiles_b.4bpp.mtf"
	.byte 0x0f, 0xbc, 0x0e	@ PACKER: left in the buffer by Tileset_Set53TilesA+0x20e5
	.global Tileset_Set53TilesC
Tileset_Set53TilesC:
	.incbin "games/tla/src/graphics/tileset/set_53/tiles_c.4bpp.mtf"
	.global Tileset_Set53TilesD
Tileset_Set53TilesD:
	.incbin "games/tla/src/graphics/tileset/set_53/tiles_d.4bpp.mtf"
	.byte 0x7c, 0xfe, 0x81	@ PACKER: left in the buffer by Tileset_Set53TilesC+0x3a1
