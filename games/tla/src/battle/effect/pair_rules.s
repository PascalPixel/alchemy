	.section .rodata
@ 16-byte rows, terminated by key -1.
	.global gParty_PairResolveRules
gParty_PairResolveRules:
	.incbin "games/tla/src/battle/effect/pair_rules.table"
@ Func_080ca4a8 reads one word at a time; 0 terminates (bit 31 is a marker).
	.global gData_080ef984
gData_080ef984:
	.4byte 0x80000000 + 72, 4, 0x80000000 + 74, 6
	.4byte 7, 0x80000000 + 73, 8, 0x80000000 + 75
	.4byte 14, 15, 16, 17
	.4byte 18, 19, 20, 0x80000000 + 73
	.4byte 23, 24, 25, 26
	.4byte 27, 28, 29, 30
	.4byte 0x80000000 + 76, 34, 35, 36
	.4byte 0x80000000 + 90, 37, 38, 39
	.4byte 0x80000000 + 92, 50, 51, 52
	.4byte 137, 138, 0x80000000 + 77, 82
	.4byte 0x80000000 + 78, 83, 84, 0x80000000 + 79
	.4byte 85, 85, 86, 87
	.4byte 88, 89, 90, 91
	.4byte 92, 93, 94, 0x80000000 + 87
	.4byte 67, 68, 69, 70
	.4byte 71, 72, 73, 74
	.4byte 0x80000000 + 90, 75, 76, 77
	.4byte 78, 79, 80, 81
	.4byte 0x80000000 + 83, 95, 0x80000000 + 82, 96
	.4byte 97, 0x80000000 + 76, 102, 0x80000000 + 80
	.4byte 107, 108, 0x80000000 + 73, 109
	.4byte 0x80000000 + 81, 115, 116, 117
	.4byte 118, 119, 120, 121
	.4byte 122, 124, 125, 0x80000000 + 75
	.4byte 123, 0x80000000 + 75, 145, 146
	.4byte 147, 148, 149, 150
	.4byte 157, 0x80000000 + 88, 151, 152
	.4byte 153, 188, 0x80000000 + 89, 158
	.4byte 159, 0x80000000 + 91, 160, 161
	.4byte 162, 163, 164, 165
	.4byte 166, 167, 168, 169
	.4byte 0x80000000 + 84, 174, 175, 176
	.4byte 0x80000000 + 85, 177, 178, 179
	.4byte 180, 181, 0x80000000 + 86, 189
	.4byte 190, 191, 192, 193
	.4byte 194, 0x80000000 + 101, 207, 208
	.4byte 209, 210, 211, 0x80000000 + 96
	.4byte 197, 198, 0x80000000 + 97, 254
	.4byte 255, 256, 257, 258
	.4byte 259, 260, 261, 262
	.4byte 0x80000000 + 98, 263, 0x80000000 + 102, 264
	.4byte 265, 0x80000000 + 90, 252, 0x80000000 + 93
	.4byte 223, 224, 225, 0x80000000 + 94
	.4byte 226, 227, 228, 229
	.4byte 230, 231, 232, 233
	.4byte 0x80000000 + 106, 280, 281, 282
	.4byte 283, 284, 285, 287
	.4byte 292, 293, 294, 295
	.4byte 286, 0x80000000 + 99, 288, 289
	.4byte 290, 291, 296, 0x80000000 + 100
	.4byte 297, 0x80000000 + 103, 298, 299
	.4byte 300, 301, 302, 0x80000000 + 95
	.4byte 248, 0x80000000 + 107, 250, 0x80000000 + 83
	.4byte 268, 0x80000000 + 108, 303, 304
	.4byte 305, 306, 307, 308
	.4byte 0x80000000 + 83, 43, 0x80000000 + 82, 40
	.4byte 41, 99, 0x80000000 + 43, 312
	.4byte 0x80000000 + 73, 0
@ Battle music (BattleEffect_SelectBattleCue): each song marker sets the music
@ for the encounter keys after it, each a slot of a zone. The search stops at
@ the battle's key or at the end (0), so a battle not listed takes the last
@ marker's song, 0, for which the function chooses music of its own.
	.macro battle_music song:req
	.2byte 0x8000 + \song
	.endm
	.macro encounter_key zone:req, slot:req
	.2byte (\zone << 4) + \slot
	.endm
	.global Data_080efd1c
Data_080efd1c:
	battle_music 754
	encounter_key 103, 3
	battle_music 753
	encounter_key 103, 2
	battle_music 752
	encounter_key 103, 1
	battle_music 751
	encounter_key 103, 0
	battle_music 750
	encounter_key 102, 3
	battle_music 76
	encounter_key 102, 2
	battle_music 54
	encounter_key 102, 1
	battle_music 53
	encounter_key 102, 0
	battle_music 57
	encounter_key 13, 5
	encounter_key 13, 6
	encounter_key 13, 7
	battle_music 34
	encounter_key 102, 4
	battle_music 52
	encounter_key 9, 4
	encounter_key 10, 0
	encounter_key 11, 1
	encounter_key 12, 0
	encounter_key 12, 1
	encounter_key 12, 2
	encounter_key 12, 3
	encounter_key 12, 4
	encounter_key 12, 5
	encounter_key 12, 6
	battle_music 51
	encounter_key 10, 1
	encounter_key 13, 0
	encounter_key 13, 1
	encounter_key 13, 2
	encounter_key 13, 3
	encounter_key 13, 4
	battle_music 0
	.2byte 0
