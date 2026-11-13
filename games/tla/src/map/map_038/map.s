	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map038
Field_Map038:
	map_header 0, 0, 64, 64, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 64, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 64, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map038, .LField_Map038_CELL_PICTURES
	map_block Field_Map038, .LField_Map038_SHAPES
	map_block Field_Map038, .LField_Map038_GRID
	map_block Field_Map038, .LField_Map038_SHAPE_GRID
	map_block Field_Map038, .LField_Map038_ANIMATION
	map_block Field_Map038, .LField_Map038_BLEND
	map_block Field_Map038, .LField_Map038_SCENE
.LField_Map038_CELL_PICTURES:
	.incbin "games/tla/src/map/map_038/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Map038_SHAPES:
	.incbin "games/tla/src/map/map_038/shapes.table.lz"
	.balign 4, 0
.LField_Map038_GRID:
	.incbin "games/tla/src/map/map_038/map.grid.lz"
	.balign 4, 0
.LField_Map038_SHAPE_GRID:
	.incbin "games/tla/src/map/map_038/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map038_ANIMATION:
	.incbin "games/tla/src/map/map_038/animation.script.lz"
	.balign 4, 0
.LField_Map038_BLEND:
	.incbin "games/tla/src/map/map_038/blend.script.lz"
	.balign 4, 0
.LField_Map038_SCENE:
	.incbin "games/tla/src/map/map_038/scene.parts.lz"
	.balign 4, 0
	.global Tileset_Set21Palette
Tileset_Set21Palette:
	.incbin "games/tla/src/graphics/tileset/set_21/palette.gbapal.lz"
	.byte 0x3a, 0x16, 0xda	@ PACKER: left in the buffer by Field_Map038+0x105
	.global Tileset_Set21TilesA
Tileset_Set21TilesA:
	.incbin "games/tla/src/graphics/tileset/set_21/tiles_a.4bpp.mtf"
	.byte 0x7e, 0xa7	@ PACKER: left in the buffer by Tileset_Set17TilesC+0x1cbe
	.global Tileset_Set21TilesB
Tileset_Set21TilesB:
	.incbin "games/tla/src/graphics/tileset/set_21/tiles_b.4bpp.mtf"
	.global Tileset_Set21TilesC
Tileset_Set21TilesC:
	.incbin "games/tla/src/graphics/tileset/set_21/tiles_c.4bpp.mtf"
	.byte 0xd4	@ PACKER: left in the buffer by Tileset_Set21TilesB+0x2df
	.global Tileset_Set21TilesD
Tileset_Set21TilesD:
	.incbin "games/tla/src/graphics/tileset/set_21/tiles_d.4bpp.mtf"
	.byte 0x0f, 0x7c, 0xfe	@ PACKER: left in the buffer by Tileset_Set21TilesC+0x2b5
