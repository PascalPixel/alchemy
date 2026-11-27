@ Where the party returns to after a defeat (fdSetRestartPoint). Each
@ row: the scene and entrance (-1 for any) it matches, then the scene and
@ entrance the party restarts at; scene -1 ends the table.
	.macro restart_point scene:req, entrance:req, restart_scene:req, restart_entrance:req
	.2byte SceneId\scene, \entrance, SceneId\restart_scene, \restart_entrance
	.endm

	.section .rodata
	.balign 2
	.global SceneRestartRules
SceneRestartRules:
	restart_point ValePrologue, 15, CottageRoomsB, 16
	restart_point Vale, 16, Sanctum, 2
	restart_point Vault, 1, Sanctum, 12
	restart_point Vault, 3, Sanctum, 12
	restart_point Bilibin, -1, Sanctum, 35
	restart_point Kolima, -1, Kolima, 12
	restart_point Imil, -1, IceRooms, 7
	restart_point FuchinTempleGrounds, -1, FuchinTempleGrounds, 5
	restart_point Xian, -1, Sanctum, 32
	restart_point Altin, -1, Sanctum, 29
	restart_point LamaTemple, -1, LamaTemple, 5
	restart_point VaultRevisit, -1, Sanctum, 11
	restart_point Kalay, -1, KalaySanctum, 2
	restart_point Lunpa, 3, RockRooms, 11
	restart_point Tolbi, -1, StuccoRoomsB, 30
	restart_point Suhalla, -1, TimberRoomsA, 6
	restart_point Lalivero, -1, TimberRoomsB, 12
	restart_point BattleArena, -1, BattleArena, 1
	.2byte -1, -1, -1, -1 @ the end
