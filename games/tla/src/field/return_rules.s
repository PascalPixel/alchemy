	.include "games/tla/src/field/facing.inc"

@ Where the party restarts after a defeat and where the ship waits
@ (SceneReturnRules), eight halfwords a row as fdSetRestartPoint and
@ the World Map's return read them: the scene and entrance (-1 for any) it
@ matches, the scene and entrance the party restarts at after a defeat, the
@ ship's position in pixels (x, z) and its facing, a word whose halves are the
@ last two. A row of -1 ends the table.
	.macro return_rule scene:req, entrance:req, restart_scene:req, restart_entrance:req, x:req, z:req, facing:req
	.2byte SceneId\scene, \entrance, SceneId\restart_scene, \restart_entrance, \x, \z
	.4byte \facing
	.endm

	.section .rodata
	.balign 4
	.global SceneReturnRules
SceneReturnRules:
	return_rule Idejima, -1, Idejima, 2, 0, 0, FACING_NORTH
	return_rule DailaFlooded, -1, DailaRoomsSanctum, 1, 10085, 8126, FACING_SOUTH
	return_rule Daila, -1, DailaRoomsSanctum, 1, 10085, 8126, FACING_SOUTH
	return_rule Madra, -1, MadraRoomsHouses, 9, 10591, 9119, FACING_SOUTH
	return_rule Mikasalla, -1, MikasallaRoomsSanctum, 1, 12226, 11002, FACING_SOUTH
	return_rule GarohWayUp, -1, GarohRoomsSanctum, 2, 12226, 11002, FACING_SOUTH
	return_rule Alhafra, -1, AlhafraRooms, 7, 11051, 9317, FACING_SOUTHWEST
	return_rule Naribwe, -1, Naribwe, 6, 9282, 9162, FACING_SOUTH
	return_rule KibomboCeremony, -1, KibomboCeremony, 19, 9386, 7896, FACING_WEST
	return_rule Kibombo, -1, KibomboHouses, 20, 9386, 7896, FACING_WEST
	return_rule Yallam, -1, MikasallaRoomsSanctum, 4, 12882, 9865, FACING_SOUTH
	return_rule ApojiiIslands, -1, MikasallaRoomsSanctum, 5, 14401, 8348, FACING_NORTHEAST
	return_rule Izumo, -1, IzumoRooms, 6, 13313, 6018, FACING_NORTHEAST
	return_rule Champa, -1, ChampaSanctum, 1, 10888, 6375, FACING_NORTHEAST
	return_rule Lemuria, -1, Lemuria, 18, 13074, 1176, FACING_SOUTH
	return_rule Contigo, -1, ContigoShops, 6, 7271, 8269, FACING_EAST
	return_rule ShamanVillage, -1, ShamanVillageSanctum, 6, 7262, 5678, FACING_WEST
	return_rule Loho, -1, LohoRooms, 5, 8830, 5957, FACING_SOUTHWEST
	return_rule Prox, -1, ProxCaves, 7, 8975, 1543, FACING_NORTH
	return_rule EastTundariaIslet, -1, EastTundariaIslet, 7, 12844, 11740, FACING_WEST
	return_rule Lobby, -1, Lobby, 1, 0, 0, FACING_NORTH
	.fill 8, 2, -1 @ the end
