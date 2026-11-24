	.global Field_AquaRockInsideParchTablet
Field_AquaRockInsideParchTablet:
	map_header 0, 0, 128, 128, 3, 2, 2, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_AquaRockInsideParchTablet, .LField_AquaRockInsideParchTablet_CELL_TILES
	map_block Field_AquaRockInsideParchTablet, .LField_AquaRockInsideParchTablet_SHAPES
	map_block Field_AquaRockInsideParchTablet, .LField_AquaRockInsideParchTablet_GRID
	map_block Field_AquaRockInsideParchTablet, .LField_AquaRockInsideParchTablet_SHAPE_GRID
	map_block Field_AquaRockInsideParchTablet, .LField_AquaRockInsideParchTablet_ANIMATION
	map_block Field_AquaRockInsideParchTablet, .LField_AquaRockInsideParchTablet_BLEND
	map_block Field_AquaRockInsideParchTablet, .LField_AquaRockInsideParchTablet_SCENE
.LField_AquaRockInsideParchTablet_CELL_TILES:
	.incbin "games/tla/src/map/aqua_rock_inside_parch_tablet/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_AquaRockInsideParchTablet_SHAPES:
	.incbin "games/tla/src/map/aqua_rock_inside_parch_tablet/shapes.table.lz"
	.balign 4, 0
.LField_AquaRockInsideParchTablet_GRID:
	.incbin "games/tla/src/map/aqua_rock_inside_parch_tablet/map.grid.lz"
	.balign 4, 0
.LField_AquaRockInsideParchTablet_SHAPE_GRID:
	.incbin "games/tla/src/map/aqua_rock_inside_parch_tablet/shape_grid.plane.lz"
	.balign 4, 0
.LField_AquaRockInsideParchTablet_ANIMATION:
	.incbin "games/tla/src/map/aqua_rock_inside_parch_tablet/animation.script.lz"
	.balign 4, 0
.LField_AquaRockInsideParchTablet_BLEND:
	.incbin "games/tla/src/map/aqua_rock_inside_water_of_life_chest/blend.script.lz"
	.balign 4, 0
.LField_AquaRockInsideParchTablet_SCENE:
	.incbin "games/tla/src/map/aqua_rock_inside_parch_tablet/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteAquaRockInsideParchTablet
Tileset_PaletteAquaRockInsideParchTablet:
	.incbin "games/tla/src/graphics/tileset/palettes/aqua_rock_inside_parch_tablet.gbapal.lz"
	.byte 0xb3, 0x00, 0x0d	@ PACKER: left in the buffer by Field_AquaRockInsideParchTablet+0x125
