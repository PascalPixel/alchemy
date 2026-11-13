	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map174
Field_Map174:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map174, .LField_Map174_CELL_PICTURES
	map_block Field_Map174, .LField_Map174_SHAPES
	map_block Field_Map174, .LField_Map174_GRID
	map_block Field_Map174, .LField_Map174_SHAPE_GRID
	map_block Field_Map174, .LField_Map174_ANIMATION
	map_block Field_Map174, .LField_Map174_BLEND
	map_block Field_Map174, .LField_Map174_SCENE
.LField_Map174_CELL_PICTURES:
	.incbin "games/tla/src/map/map_174/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_Map174_SHAPES:
	.incbin "games/tla/src/map/map_174/shapes.table.lz"
	.balign 4, 0
.LField_Map174_GRID:
	.incbin "games/tla/src/map/map_174/grid.parts.lz"
	.balign 4, 0
.LField_Map174_SHAPE_GRID:
	.incbin "games/tla/src/map/map_174/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map174_ANIMATION:
	.incbin "games/tla/src/map/map_174/animation.script.lz"
	.balign 4, 0
.LField_Map174_BLEND:
	.incbin "games/tla/src/map/map_057/blend.script.lz"
	.balign 4, 0
.LField_Map174_SCENE:
	.incbin "games/tla/src/map/map_174/scene.parts.lz"
	.balign 4, 0
	.global Tileset_Set79Palette
Tileset_Set79Palette:
	.incbin "games/tla/src/graphics/tileset/set_79/palette.gbapal.lz"
	.byte 0x78	@ PACKER: left in the buffer by Field_Map174+0x15f
	.global Tileset_Set79TilesA
Tileset_Set79TilesA:
	.incbin "games/tla/src/graphics/tileset/set_79/tiles_a.4bpp.mtf"
	.byte 0xe6	@ PACKER: left in the buffer by Field_Map148+0x2aa3
	.global Tileset_Set79TilesB
Tileset_Set79TilesB:
	.incbin "games/tla/src/graphics/tileset/set_79/tiles_b.4bpp.mtf"
	.byte 0xc7, 0x69, 0x1f	@ PACKER: left in the buffer by Tileset_Set79TilesA+0x21ad
	.global Tileset_Set79TilesC
Tileset_Set79TilesC:
	.incbin "games/tla/src/graphics/tileset/set_79/tiles_c.4bpp.mtf"
	.byte 0x92, 0xe5	@ PACKER: left in the buffer by Tileset_Set79TilesB+0x1e76
	.global Tileset_Set79TilesD
Tileset_Set79TilesD:
	.incbin "games/tla/src/graphics/tileset/set_79/tiles_d.4bpp.mtf"
	.byte 0x11	@ PACKER: left in the buffer by Tileset_Set79TilesC+0x13f
