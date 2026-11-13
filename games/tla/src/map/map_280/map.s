	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map280
Field_Map280:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map280, .LField_Map280_CELL_PICTURES
	map_block Field_Map280, .LField_Map280_SHAPES
	map_block Field_Map280, .LField_Map280_GRID
	map_block Field_Map280, .LField_Map280_SHAPE_GRID
	map_block Field_Map280, .LField_Map280_ANIMATION
	map_block Field_Map280, .LField_Map280_BLEND
	map_block Field_Map280, .LField_Map280_SCENE
.LField_Map280_CELL_PICTURES:
	.incbin "games/tla/src/map/map_280/cell_pictures.delta2.lz"
	.balign 4, 0
.LField_Map280_SHAPES:
	.incbin "games/tla/src/map/map_280/shapes.table.lz"
	.balign 4, 0
.LField_Map280_GRID:
	.incbin "games/tla/src/map/map_280/grid.parts.lz"
	.balign 4, 0
.LField_Map280_SHAPE_GRID:
	.incbin "games/tla/src/map/map_280/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map280_ANIMATION:
	.incbin "games/tla/src/map/map_279/animation.script.lz"
	.balign 4, 0
.LField_Map280_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map280_SCENE:
	.incbin "games/tla/src/map/map_280/scene.parts.lz"
	.balign 4, 0
	.global Tileset_Set115Palette
Tileset_Set115Palette:
	.incbin "games/tla/src/graphics/tileset/set_115/palette.gbapal.lz"
	.byte 0x83, 0xf0	@ PACKER: left in the buffer by Field_Map280+0x152
	.global Tileset_Set115TilesA
Tileset_Set115TilesA:
	.incbin "games/tla/src/graphics/tileset/set_115/tiles_a.4bpp.mtf"
	.global Tileset_Set115TilesB
Tileset_Set115TilesB:
	.incbin "games/tla/src/graphics/tileset/set_115/tiles_b.4bpp.mtf"
	.byte 0x0f	@ PACKER: left in the buffer by Tileset_Set115TilesA+0x172b
	.global Tileset_Set115TilesC
Tileset_Set115TilesC:
	.incbin "games/tla/src/graphics/tileset/set_115/tiles_c.4bpp.mtf"
	.global Tileset_Set115TilesD
Tileset_Set115TilesD:
	.incbin "games/tla/src/graphics/tileset/set_115/tiles_d.4bpp.mtf"
	.byte 0x73	@ PACKER: left in the buffer by Tileset_Set115TilesC+0x4e3
