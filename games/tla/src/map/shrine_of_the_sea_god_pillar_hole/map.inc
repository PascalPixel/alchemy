	.global Field_ShrineOfTheSeaGodPillarHole
Field_ShrineOfTheSeaGodPillarHole:
	map_header 0, 0, 90, 100, 3, 2, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 100, 16, 16, 0, 0, 0xff, 0xff
	map_layer 100, 0, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_ShrineOfTheSeaGodPillarHole, .LField_ShrineOfTheSeaGodPillarHole_CELL_TILES
	map_block Field_ShrineOfTheSeaGodPillarHole, .LField_ShrineOfTheSeaGodPillarHole_SHAPES
	map_block Field_ShrineOfTheSeaGodPillarHole, .LField_ShrineOfTheSeaGodPillarHole_GRID
	map_block Field_ShrineOfTheSeaGodPillarHole, .LField_ShrineOfTheSeaGodPillarHole_SHAPE_GRID
	map_block Field_ShrineOfTheSeaGodPillarHole, .LField_ShrineOfTheSeaGodPillarHole_ANIMATION
	map_block Field_ShrineOfTheSeaGodPillarHole, .LField_ShrineOfTheSeaGodPillarHole_BLEND
	map_block Field_ShrineOfTheSeaGodPillarHole, .LField_ShrineOfTheSeaGodPillarHole_SCENE
.LField_ShrineOfTheSeaGodPillarHole_CELL_TILES:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_pillar_hole/cell_tiles.delta2.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodPillarHole_SHAPES:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_pillar_hole/shapes.table.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodPillarHole_GRID:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_pillar_hole/map.grid.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodPillarHole_SHAPE_GRID:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_pillar_hole/shape_grid.plane.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodPillarHole_ANIMATION:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_pillar_hole/animation.script.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodPillarHole_BLEND:
	.incbin "games/tla/src/map/lobby/blend.script.lz"
	.balign 4, 0
.LField_ShrineOfTheSeaGodPillarHole_SCENE:
	.incbin "games/tla/src/map/shrine_of_the_sea_god_pillar_hole/scene.scene.lz"
	.global Tileset_PaletteShrineOfTheSeaGod6
Tileset_PaletteShrineOfTheSeaGod6:
	.incbin "games/tla/src/graphics/tileset/palettes/shrine_of_the_sea_god_pillar_hole.gbapal.lz"
