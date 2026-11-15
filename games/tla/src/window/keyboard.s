	.include "games/tla/include/edition.inc"
	.section .rodata
	.balign 4
@ The debug warp menu's presets: {name, handler}
	.global gDebugWarp_Presets
gDebugWarp_Presets:
	.4byte DebugWarp_JupiterName, DebugWarp_Jupiter
	.4byte DebugWarp_Arafura2Name, DebugWarp_Arafura2
	.4byte DebugWarp_EnergySampleName, DebugWarp_EnergySample
	.4byte DebugWarp_Arafura3Name, DebugWarp_Arafura3
	.4byte DebugWarp_Champa2Name, DebugWarp_Champa2
	.4byte DebugWarp_KaoruSpecial4Name, DebugWarp_KaoruSpecial4
	.4byte DebugWarp_KaoruSpecial3Name, DebugWarp_KaoruSpecial3
	.4byte DebugWarp_KaoruSpecial2Name, DebugWarp_KaoruSpecial2
	.4byte DebugWarp_KaoruSpecial1Name, DebugWarp_KaoruSpecial1
	.global gDebugShortcut_Abilities
gDebugShortcut_Abilities:
	.2byte 0x8b, 0x9a, 0x86, 0x9d, 0x97, 0x89, 0x85, 0x88, 0x8a
	.2byte 0x9c, 0x87, 0x99, 0x98, 0x9b, 0x00, 0x00, 0x00, 0x00
	.2byte 0x8c, 0x8f, 0x93, 0x94, 0x8e, 0x00, 0x95, 0x90, 0x00
	.2byte 0x96, 0x92, 0x91, 0x4e, 0x0c, 0x21, 0x5d, 0x18, 0x8d
@ The digits 0 to 8 the party status draws
	.global Data_080aa1b8
Data_080aa1b8:
	.incbin "games/tla/src/window/digits.4bpp"
@ The name entry keyboards, ☀️'s
	.global gNameEntry_KeyboardTilemap
gNameEntry_KeyboardTilemap:
	.ifdef TLA_EDITION_JA
	.incbin "games/tla/src/window/keyboard_ja.table.lz"
	.global gNameEntry_AlternateKeyboardTilemap
gNameEntry_AlternateKeyboardTilemap:
	.incbin "games/tla/src/window/keyboard_j2.table.lz"
	.global NameEntry_KeyboardTilemap3
NameEntry_KeyboardTilemap3:
	.incbin "games/tla/src/window/keyboard_j3.table.lz"
	.else
	.ifdef TLA_EDITION_ES
	.incbin "games/tla/src/window/keyboard_es.table.lz"
	.else
	.incbin "games/tla/src/window/keyboard.table.lz"
	.endif
	.endif
	.balign 4, 0
	.global gWindow_FixedTileBlocks
gWindow_FixedTileBlocks:
	.4byte gResource_FixedBlockBTiles
