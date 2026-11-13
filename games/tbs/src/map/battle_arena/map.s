	.include "games/tbs/src/map/map.inc"
	.section .rodata
	.global Field_BattleArena
Field_BattleArena:
	map_header 0, 0, 128, 128, 3, 1, 2, 1, 1, 2, 0, 0
	map_layer 0, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 128, 0, 16, 16, 0, 0, 0xff, 0xff
	map_layer 0, 128, 16, 16, 0, 0, 0xff, 0xff
	map_offset Field_BattleArena, .LField_BattleArena_CELL_PICTURES
	map_offset Field_BattleArena, .LField_BattleArena_SHAPES
	map_offset Field_BattleArena, .LField_BattleArena_GRID
	map_offset Field_BattleArena, .LField_BattleArena_ANIMATION
	map_offset Field_BattleArena, .LField_BattleArena_BLEND
	map_offset Field_BattleArena, .LField_BattleArena_MARKERS
.LField_BattleArena_CELL_PICTURES:
	.incbin "games/tbs/src/map/battle_arena/cell_pictures.delta1.lz"
	.balign 4, 0
.LField_BattleArena_SHAPES:
	.incbin "games/tbs/src/map/battle_arena/shapes.table.lz"
	.balign 4, 0
.LField_BattleArena_GRID:
	.incbin "games/tbs/src/map/battle_arena/map.grid.lz"
	.balign 4, 0
.LField_BattleArena_ANIMATION:
	.incbin "games/tbs/src/map/battle_arena/tileset_anims.script.lz"
	.balign 4, 0
.LField_BattleArena_BLEND:
	.incbin "games/tbs/src/map/goma_cave_entrance/blend.script.lz"
	.balign 4, 0
.LField_BattleArena_MARKERS:
	treasure_spots_end
	.balign 4, 0
	.global Tileset_PaletteBattleArena
Tileset_PaletteBattleArena:
	.incbin "games/tbs/src/graphics/tileset/palettes/battle_arena.gbapal.lz"
	.byte 0x1c	@ PACKER: left in the buffer by Field_BattleArena+0x1c7
