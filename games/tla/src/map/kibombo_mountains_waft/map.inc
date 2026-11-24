	.global Field_KibomboMountainsWaft
Field_KibomboMountainsWaft:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_KibomboMountainsWaft, .LField_KibomboMountainsWaft_CELL_TILES
	map_block Field_KibomboMountainsWaft, .LField_KibomboMountainsWaft_SHAPES
	map_block Field_KibomboMountainsWaft, .LField_KibomboMountainsWaft_GRID
	map_block Field_KibomboMountainsWaft, .LField_KibomboMountainsWaft_SHAPE_GRID
	map_block Field_KibomboMountainsWaft, .LField_KibomboMountainsWaft_ANIMATION
	map_block Field_KibomboMountainsWaft, .LField_KibomboMountainsWaft_BLEND
	map_block Field_KibomboMountainsWaft, .LField_KibomboMountainsWaft_SCENE
.LField_KibomboMountainsWaft_CELL_TILES:
	.incbin "games/tla/src/map/kibombo_mountains_waft/cell_tiles.delta0.lz"
	.balign 4, 0
.LField_KibomboMountainsWaft_SHAPES:
	.incbin "games/tla/src/map/kibombo_mountains_waft/shapes.table.lz"
	.balign 4, 0
.LField_KibomboMountainsWaft_GRID:
	.incbin "games/tla/src/map/kibombo_mountains_waft/map.grid.lz"
	.balign 4, 0
.LField_KibomboMountainsWaft_SHAPE_GRID:
	.incbin "games/tla/src/map/kibombo_mountains_waft/shape_grid.plane.lz"
	.balign 4, 0
.LField_KibomboMountainsWaft_ANIMATION:
	.incbin "games/tla/src/map/kibombo_mountains_waft/animation.script.lz"
	.balign 4, 0
.LField_KibomboMountainsWaft_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_KibomboMountainsWaft_SCENE:
	.incbin "games/tla/src/map/kibombo_mountains_waft/scene.scene.lz"
	.global Tileset_KibomboMountainsWaftPalette
Tileset_KibomboMountainsWaftPalette:
	.incbin "games/tla/src/graphics/tileset/kibombo_mountains_waft/palette.gbapal.lz"
	.global Tileset_KibomboMountainsWaftTilesA
Tileset_KibomboMountainsWaftTilesA:
	.incbin "games/tla/src/graphics/tileset/kibombo_mountains_waft/tiles_a.4bpp.mtf"
	.byte 0x96, 0x80	@ PACKER: left in the buffer by Field_KibomboMountainsWaft+0x2756
	.global Tileset_KibomboMountainsWaftTilesB
Tileset_KibomboMountainsWaftTilesB:
	.incbin "games/tla/src/graphics/tileset/kibombo_mountains_waft/tiles_b.4bpp.mtf"
	.global Tileset_KibomboMountainsWaftTilesC
Tileset_KibomboMountainsWaftTilesC:
	.incbin "games/tla/src/graphics/tileset/kibombo_mountains_waft/tiles_c.4bpp.mtf"
	.global Tileset_KibomboMountainsWaftTilesD
Tileset_KibomboMountainsWaftTilesD:
	.incbin "games/tla/src/graphics/tileset/kibombo_mountains_waft/tiles_d.4bpp.mtf"
