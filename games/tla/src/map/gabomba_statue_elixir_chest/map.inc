	.global Field_GabombaStatueElixirChest
Field_GabombaStatueElixirChest:
	map_header 0, 0, 100, 80, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GabombaStatueElixirChest, .LField_GabombaStatueElixirChest_CELL_TILES
	map_block Field_GabombaStatueElixirChest, .LField_GabombaStatueElixirChest_SHAPES
	map_block Field_GabombaStatueElixirChest, .LField_GabombaStatueElixirChest_GRID
	map_block Field_GabombaStatueElixirChest, .LField_GabombaStatueElixirChest_SHAPE_GRID
	map_block Field_GabombaStatueElixirChest, .LField_GabombaStatueElixirChest_ANIMATION
	map_block Field_GabombaStatueElixirChest, .LField_GabombaStatueElixirChest_BLEND
	map_block Field_GabombaStatueElixirChest, .LField_GabombaStatueElixirChest_SCENE
.LField_GabombaStatueElixirChest_CELL_TILES:
	.incbin "games/tla/src/map/gabomba_statue_elixir_chest/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_GabombaStatueElixirChest_SHAPES:
	.incbin "games/tla/src/map/gabomba_statue_elixir_chest/shapes.table.lz"
	.balign 4, 0
.LField_GabombaStatueElixirChest_GRID:
	.incbin "games/tla/src/map/gabomba_statue_elixir_chest/map.grid.lz"
	.balign 4, 0
.LField_GabombaStatueElixirChest_SHAPE_GRID:
	.incbin "games/tla/src/map/gabomba_statue_elixir_chest/shape_grid.plane.lz"
	.balign 4, 0
.LField_GabombaStatueElixirChest_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_GabombaStatueElixirChest_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_GabombaStatueElixirChest_SCENE:
	.incbin "games/tla/src/map/gabomba_statue_elixir_chest/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteGabombaStatueElixirChest
Tileset_PaletteGabombaStatueElixirChest:
	.incbin "games/tla/src/graphics/tileset/palettes/gabomba_statue_elixir_chest.gbapal.lz"
	.byte 0xb5	@ PACKER: left in the buffer by Field_GabombaStatue2+0xe3
