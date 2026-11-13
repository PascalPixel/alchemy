	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map126
Field_Map126:
	map_header 0, 0, 50, 128, 3, 1, 1, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, -1, -3, 0x3f, 0x3f
	map_block Field_Map126, .LField_Map126_CELL_PICTURES
	map_block Field_Map126, .LField_Map126_SHAPES
	map_block Field_Map126, .LField_Map126_GRID
	map_block Field_Map126, .LField_Map126_SHAPE_GRID
	map_block Field_Map126, .LField_Map126_ANIMATION
	map_block Field_Map126, .LField_Map126_BLEND
	map_block Field_Map126, .LField_Map126_SCENE
.LField_Map126_CELL_PICTURES:
	.incbin "games/tla/src/map/map_126/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map126_SHAPES:
	.incbin "games/tla/src/map/map_126/shapes.table.lz"
	.balign 4, 0
.LField_Map126_GRID:
	.incbin "games/tla/src/map/map_126/grid.parts.lz"
	.balign 4, 0
.LField_Map126_SHAPE_GRID:
	.incbin "games/tla/src/map/map_126/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map126_ANIMATION:
	.incbin "games/tla/src/map/map_126/animation.script.lz"
	.balign 4, 0
.LField_Map126_BLEND:
	.incbin "games/tla/src/map/map_126/blend.script.lz"
	.balign 4, 0
.LField_Map126_SCENE:
	.incbin "games/tla/src/map/map_126/scene.parts.lz"
	.global Tileset_Set61Palette
Tileset_Set61Palette:
	.incbin "games/tla/src/graphics/tileset/set_61/palette.gbapal.lz"
	.byte 0x11, 0x98, 0x70	@ PACKER: left in the buffer by Field_Map126+0x171
	.global Tileset_Set61TilesA
Tileset_Set61TilesA:
	.incbin "games/tla/src/graphics/tileset/set_61/tiles_a.4bpp.mtf"
	.byte 0x4d, 0x25	@ PACKER: left in the buffer by Tileset_Set59TilesA+0x2a36
	.global Tileset_Set61TilesB
Tileset_Set61TilesB:
	.incbin "games/tla/src/graphics/tileset/set_61/tiles_b.4bpp.mtf"
	.byte 0x02	@ PACKER: left in the buffer by Tileset_Set61TilesA+0x28cf
	.global Tileset_Set61TilesC
Tileset_Set61TilesC:
	.incbin "games/tla/src/graphics/tileset/set_61/tiles_c.4bpp.mtf"
	.global Tileset_Set61TilesD
Tileset_Set61TilesD:
	.incbin "games/tla/src/graphics/tileset/set_61/tiles_d.4bpp.mtf"
	.byte 0xf3	@ PACKER: left in the buffer by Tileset_Set61TilesB+0x27d3
