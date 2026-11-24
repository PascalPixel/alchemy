	.global Field_KibomboMountainsDiskAxeChest
Field_KibomboMountainsDiskAxeChest:
	map_header 0, 0, 128, 128, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_KibomboMountainsDiskAxeChest, .LField_KibomboMountainsDiskAxeChest_CELL_TILES
	map_block Field_KibomboMountainsDiskAxeChest, .LField_KibomboMountainsDiskAxeChest_SHAPES
	map_block Field_KibomboMountainsDiskAxeChest, .LField_KibomboMountainsDiskAxeChest_GRID
	map_block Field_KibomboMountainsDiskAxeChest, .LField_KibomboMountainsDiskAxeChest_SHAPE_GRID
	map_block Field_KibomboMountainsDiskAxeChest, .LField_KibomboMountainsDiskAxeChest_ANIMATION
	map_block Field_KibomboMountainsDiskAxeChest, .LField_KibomboMountainsDiskAxeChest_BLEND
	map_block Field_KibomboMountainsDiskAxeChest, .LField_KibomboMountainsDiskAxeChest_SCENE
.LField_KibomboMountainsDiskAxeChest_CELL_TILES:
	.incbin "games/tla/src/map/kibombo_mountains_disk_axe_chest/cell_tiles.delta0.lz"
	.balign 4, 0
.LField_KibomboMountainsDiskAxeChest_SHAPES:
	.incbin "games/tla/src/map/kibombo_mountains_disk_axe_chest/shapes.table.lz"
	.balign 4, 0
.LField_KibomboMountainsDiskAxeChest_GRID:
	.incbin "games/tla/src/map/kibombo_mountains_disk_axe_chest/map.grid.lz"
	.balign 4, 0
.LField_KibomboMountainsDiskAxeChest_SHAPE_GRID:
	.incbin "games/tla/src/map/kibombo_mountains_disk_axe_chest/shape_grid.plane.lz"
	.balign 4, 0
.LField_KibomboMountainsDiskAxeChest_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_KibomboMountainsDiskAxeChest_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_KibomboMountainsDiskAxeChest_SCENE:
	.incbin "games/tla/src/map/kibombo_mountains_disk_axe_chest/scene.scene.lz"
	.global Tileset_PaletteKibomboMountainsDiskAxeChest
Tileset_PaletteKibomboMountainsDiskAxeChest:
	.incbin "games/tla/src/graphics/tileset/palettes/kibombo_mountains_disk_axe_chest.gbapal.lz"
	.byte 0xe1, 0x37, 0x03	@ PACKER: left in the buffer by Field_KibomboMountains2+0x131
