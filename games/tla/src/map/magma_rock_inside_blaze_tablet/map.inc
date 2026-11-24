	.global Field_MagmaRockInsideBlazeTablet
Field_MagmaRockInsideBlazeTablet:
	map_header 0, 0, 128, 128, 2, 1, 3, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_MagmaRockInsideBlazeTablet, .LField_MagmaRockInsideBlazeTablet_CELL_TILES
	map_block Field_MagmaRockInsideBlazeTablet, .LField_MagmaRockInsideBlazeTablet_SHAPES
	map_block Field_MagmaRockInsideBlazeTablet, .LField_MagmaRockInsideBlazeTablet_GRID
	map_block Field_MagmaRockInsideBlazeTablet, .LField_MagmaRockInsideBlazeTablet_SHAPE_GRID
	map_block Field_MagmaRockInsideBlazeTablet, .LField_MagmaRockInsideBlazeTablet_ANIMATION
	map_block Field_MagmaRockInsideBlazeTablet
	map_block Field_MagmaRockInsideBlazeTablet, .LField_MagmaRockInsideBlazeTablet_SCENE
.LField_MagmaRockInsideBlazeTablet_CELL_TILES:
	.incbin "games/tla/src/map/magma_rock_inside_blaze_tablet/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_MagmaRockInsideBlazeTablet_SHAPES:
	.incbin "games/tla/src/map/magma_rock_inside_blaze_tablet/shapes.table.lz"
	.balign 4, 0
.LField_MagmaRockInsideBlazeTablet_GRID:
	.incbin "games/tla/src/map/magma_rock_inside_blaze_tablet/map.grid.lz"
	.balign 4, 0
.LField_MagmaRockInsideBlazeTablet_SHAPE_GRID:
	.incbin "games/tla/src/map/magma_rock_inside_blaze_tablet/shape_grid.plane.lz"
	.balign 4, 0
.LField_MagmaRockInsideBlazeTablet_ANIMATION:
	.incbin "games/tla/src/map/magma_rock_inside_blaze_tablet/animation.script.lz"
	.balign 4, 0
.LField_MagmaRockInsideBlazeTablet_SCENE:
	.incbin "games/tla/src/map/magma_rock_inside_blaze_tablet/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteMagmaRockInsideBlazeTablet
Tileset_PaletteMagmaRockInsideBlazeTablet:
	.incbin "games/tla/src/graphics/tileset/palettes/magma_rock_inside_blaze_tablet.gbapal.lz"
	.byte 0x19	@ PACKER: left in the buffer by Field_MagmaRockInsideBlazeTablet+0x167
