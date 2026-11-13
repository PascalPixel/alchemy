	.include "games/tla/src/map/map.inc"
	.section .rodata
	.global Field_Map099
Field_Map099:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_Map099, .LField_Map099_CELL_PICTURES
	map_block Field_Map099, .LField_Map099_SHAPES
	map_block Field_Map099, .LField_Map099_GRID
	map_block Field_Map099, .LField_Map099_SHAPE_GRID
	map_block Field_Map099, .LField_Map099_ANIMATION
	map_block Field_Map099, .LField_Map099_BLEND
	map_block Field_Map099, .LField_Map099_SCENE
.LField_Map099_CELL_PICTURES:
	.incbin "games/tla/src/map/map_099/cell_pictures.delta0.lz"
	.balign 4, 0
.LField_Map099_SHAPES:
	.incbin "games/tla/src/map/map_099/shapes.table.lz"
	.balign 4, 0
.LField_Map099_GRID:
	.incbin "games/tla/src/map/map_099/map.grid.lz"
	.balign 4, 0
.LField_Map099_SHAPE_GRID:
	.incbin "games/tla/src/map/map_099/shape_grid.plane.lz"
	.balign 4, 0
.LField_Map099_ANIMATION:
	.incbin "games/tla/src/map/map_099/animation.script.lz"
	.balign 4, 0
.LField_Map099_BLEND:
	.incbin "games/tla/src/map/map_001/blend.script.lz"
	.balign 4, 0
.LField_Map099_SCENE:
	.incbin "games/tla/src/map/map_099/scene.scene.lz"
	.global Tileset_Set49Palette
Tileset_Set49Palette:
	.incbin "games/tla/src/graphics/tileset/set_49/palette.gbapal.lz"
	.global Tileset_Set49TilesA
Tileset_Set49TilesA:
	.incbin "games/tla/src/graphics/tileset/set_49/tiles_a.4bpp.mtf"
	.byte 0x96, 0x80	@ PACKER: left in the buffer by Field_Map099+0x2756
	.global Tileset_Set49TilesB
Tileset_Set49TilesB:
	.incbin "games/tla/src/graphics/tileset/set_49/tiles_b.4bpp.mtf"
	.global Tileset_Set49TilesC
Tileset_Set49TilesC:
	.incbin "games/tla/src/graphics/tileset/set_49/tiles_c.4bpp.mtf"
	.global Tileset_Set49TilesD
Tileset_Set49TilesD:
	.incbin "games/tla/src/graphics/tileset/set_49/tiles_d.4bpp.mtf"
