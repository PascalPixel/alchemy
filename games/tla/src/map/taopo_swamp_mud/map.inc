	.global Field_TaopoSwampMud
Field_TaopoSwampMud:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_TaopoSwampMud, .LField_TaopoSwampMud_CELL_TILES
	map_block Field_TaopoSwampMud, .LField_TaopoSwampMud_SHAPES
	map_block Field_TaopoSwampMud, .LField_TaopoSwampMud_GRID
	map_block Field_TaopoSwampMud, .LField_TaopoSwampMud_SHAPE_GRID
	map_block Field_TaopoSwampMud, .LField_TaopoSwampMud_ANIMATION
	map_block Field_TaopoSwampMud, .LField_TaopoSwampMud_BLEND
	map_block Field_TaopoSwampMud, .LField_TaopoSwampMud_SCENE
.LField_TaopoSwampMud_CELL_TILES:
	.incbin "games/tla/src/map/taopo_swamp_mud/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_TaopoSwampMud_SHAPES:
	.incbin "games/tla/src/map/taopo_swamp_mud/shapes.table.lz"
	.balign 4, 0
.LField_TaopoSwampMud_GRID:
	.incbin "games/tla/src/map/taopo_swamp_mud/map.grid.lz"
	.balign 4, 0
.LField_TaopoSwampMud_SHAPE_GRID:
	.incbin "games/tla/src/map/taopo_swamp_mud/shape_grid.plane.lz"
	.balign 4, 0
.LField_TaopoSwampMud_ANIMATION:
	.incbin "games/tla/src/map/taopo_swamp_mud/animation.script.lz"
	.balign 4, 0
.LField_TaopoSwampMud_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_TaopoSwampMud_SCENE:
	.incbin "games/tla/src/map/taopo_swamp_mud/scene.scene.lz"
	.balign 4, 0
	.global Tileset_TaopoSwampMudPalette
Tileset_TaopoSwampMudPalette:
	.incbin "games/tla/src/graphics/tileset/taopo_swamp_mud/palette.gbapal.lz"
	.byte 0x27, 0x95	@ PACKER: left in the buffer by Field_TaopoSwamp2+0x10a
	.global Tileset_TaopoSwampMudTilesA
Tileset_TaopoSwampMudTilesA:
	.incbin "games/tla/src/graphics/tileset/taopo_swamp_mud/tiles_a.4bpp.mtf"
	.byte 0x4c, 0x4f, 0x3a	@ PACKER: left in the buffer by Field_TaopoSwamp2+0x2091
	.global Tileset_TaopoSwampMudTilesB
Tileset_TaopoSwampMudTilesB:
	.incbin "games/tla/src/graphics/tileset/taopo_swamp_mud/tiles_b.4bpp.mtf"
	.global Tileset_TaopoSwampMudTilesC
Tileset_TaopoSwampMudTilesC:
	.incbin "games/tla/src/graphics/tileset/taopo_swamp_mud/tiles_c.4bpp.mtf"
	.global Tileset_TaopoSwampMudTilesD
Tileset_TaopoSwampMudTilesD:
	.incbin "games/tla/src/graphics/tileset/taopo_swamp_mud/tiles_d.4bpp.mtf"
	.byte 0x4b	@ PACKER: left in the buffer by Tileset_TaopoSwampMudTilesB+0x162b
