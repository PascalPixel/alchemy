	.global Field_GarohWerewolfCave
Field_GarohWerewolfCave:
	map_header 0, 0, 128, 128, 3, 1, 1, 0, 2, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_block Field_GarohWerewolfCave, .LField_GarohWerewolfCave_CELL_TILES
	map_block Field_GarohWerewolfCave, .LField_GarohWerewolfCave_SHAPES
	map_block Field_GarohWerewolfCave, .LField_GarohWerewolfCave_GRID
	map_block Field_GarohWerewolfCave, .LField_GarohWerewolfCave_SHAPE_GRID
	map_block Field_GarohWerewolfCave, .LField_GarohWerewolfCave_ANIMATION
	map_block Field_GarohWerewolfCave, .LField_GarohWerewolfCave_BLEND
	map_block Field_GarohWerewolfCave, .LField_GarohWerewolfCave_SCENE
.LField_GarohWerewolfCave_CELL_TILES:
	.incbin "games/tla/src/map/garoh_werewolf_cave/cell_tiles.delta1.lz"
	.balign 4, 0
.LField_GarohWerewolfCave_SHAPES:
	.incbin "games/tla/src/map/garoh_werewolf_cave/shapes.table.lz"
	.balign 4, 0
.LField_GarohWerewolfCave_GRID:
	.incbin "games/tla/src/map/garoh_werewolf_cave/map.grid.lz"
	.balign 4, 0
.LField_GarohWerewolfCave_SHAPE_GRID:
	.incbin "games/tla/src/map/garoh_werewolf_cave/shape_grid.plane.lz"
	.balign 4, 0
.LField_GarohWerewolfCave_ANIMATION:
	.incbin "games/tla/src/map/kandorean_temple_mimic/animation.script.lz"
	.balign 4, 0
.LField_GarohWerewolfCave_BLEND:
	.incbin "games/tla/src/map/garoh_tunnels/blend.script.lz"
	.balign 4, 0
.LField_GarohWerewolfCave_SCENE:
	.incbin "games/tla/src/map/garoh_werewolf_cave/scene.scene.lz"
	.balign 4, 0
	.global Tileset_PaletteGarohWerewolfCaveA
Tileset_PaletteGarohWerewolfCaveA:
	.incbin "games/tla/src/graphics/tileset/palettes/garoh_werewolf_cave_a.gbapal.lz"
	.global Tileset_PaletteGarohWerewolfCaveB
Tileset_PaletteGarohWerewolfCaveB:
	.incbin "games/tla/src/graphics/tileset/palettes/garoh_werewolf_cave_b.gbapal.lz"
	.byte 0x33, 0xbc, 0x5a	@ PACKER: left in the buffer by Field_Garoh5+0x13d
