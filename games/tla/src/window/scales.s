	.section .rodata
	.balign 4
@ Scale factors, 256 to 1x
	.global gWindowSprite_BounceScales
gWindowSprite_BounceScales:
	.2byte 256, 256, 260, 266, 274, 284, 294, 306
	.2byte 320, 332, 344, 354, 364, 372, 378, 382
	.2byte 384, 382, 378, 372, 364, 354, 344, 332
	.2byte 320, 306, 294, 284, 274, 266, 260, 256
	.global gWindowSprite_BounceScalesTail
gWindowSprite_BounceScalesTail:
	.byte -15, -28, -36, -40, -36, -28, -15, 0
	.global gMenu_CursorBobOffsets
gMenu_CursorBobOffsets:
	.byte 0, 1, 2, 3, 2, 1, 0, 0, 0, -1, -2, -3, -2, -1, 0, 0
@ The glow's fade out, rest and fade in (☀️'s 25 further steps are gone)
	.global gPaletteGlow_WaveTable
gPaletteGlow_WaveTable:
	.byte 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16
	.byte 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
	.byte 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15
	.byte 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31
@ The options screen's command icons
	.global gOptionMenu_SpeedIcons
gOptionMenu_SpeedIcons:
	.byte 0x22, 0x23, 0x24
	.global gOptionMenu_SpeechIcons
gOptionMenu_SpeechIcons:
	.byte 0x25, 0x26
	.global gOptionMenu_SleepIcons
gOptionMenu_SleepIcons:
	.byte 0x27, 0x28
@ The window colour presets
	.global gOptionMenu_FirstColorPresets
gOptionMenu_FirstColorPresets:
	.byte 4, 16, 8, 21, 1, 13
	.global gOptionMenu_SecondColorPresets
gOptionMenu_SecondColorPresets:
	.byte 0, 11, 13, 7, 14, 14
	.global gMenu_WorkspaceIcons
gMenu_WorkspaceIcons:
	.byte 0x29, 0x2a, 0x2b, 0x0b, 0x0b, 0x0b, 0x00, 0x00
	.balign 4, 0
@ The sound test's starting state and labels, then the debug warp presets' names
	.global Data_0805ea98
Data_0805ea98:
	.4byte 0, 80, 100, 2
	.global SoundTest_MusicString
SoundTest_MusicString:
	.asciz "Music"
	.balign 4, 0
	.global SoundTest_JingleString
SoundTest_JingleString:
	.asciz "Jingle"
	.balign 4, 0
	.global SoundTest_SeString
SoundTest_SeString:
	.asciz "SE"
	.balign 4, 0
	.global SoundTest_FxString
SoundTest_FxString:
	.asciz "FX"
	.balign 4, 0
	.global DebugWarp_Name8
DebugWarp_Name8:
	.asciz "8:Kaoru Special1"
	.balign 4, 0
	.global DebugWarp_Name7
DebugWarp_Name7:
	.asciz "7:Kaoru Special2"
	.balign 4, 0
	.global DebugWarp_Name6
DebugWarp_Name6:
	.asciz "6:Kaoru Special3"
	.balign 4, 0
	.global DebugWarp_Name5
DebugWarp_Name5:
	.asciz "5:Kaoru Special4"
	.balign 4, 0
	.global DebugWarp_Name4
DebugWarp_Name4:
	.asciz "4:CHAMPA2"
	.balign 4, 0
	.global DebugWarp_Name3
DebugWarp_Name3:
	.asciz "3:Arafura3"
	.balign 4, 0
	.global DebugWarp_Name2
DebugWarp_Name2:
	.asciz "2:Energy Sample"
	.balign 4, 0
	.global DebugWarp_Name1
DebugWarp_Name1:
	.asciz "1:Arafura2"
	.balign 4, 0
	.global DebugWarp_Name0
DebugWarp_Name0:
	.asciz "0:Jupiter"
	.balign 4, 0
@ An empty ability's name
	.global gData_0805eb50
gData_0805eb50:
	.asciz "----"
	.balign 4, 0
@ {sprite, portrait} pairs, -1 ending each list
	.global gPortrait_CharacterMap
gPortrait_CharacterMap:
	.2byte 0, 0
	.2byte 1, 1
	.2byte 2, 2
	.2byte 3, 3
	.2byte 4, 4
	.2byte 5, 5
	.2byte 6, 6
	.2byte 7, 8
	.2byte -1, -1
	.global gPortrait_SpriteMap
gPortrait_SpriteMap:
	.2byte 246, 0
	.2byte 243, 1
	.2byte 245, 2
	.2byte 244, 3
	.2byte 201, 14
	.2byte 202, 15
	.2byte 205, 16
	.2byte 206, 17
	.2byte 198, 18
	.2byte 209, 19
	.2byte 199, 20
	.2byte 108, 54
	.2byte 109, 55
	.2byte 110, 56
	.2byte 134, 57
	.2byte 135, 58
	.2byte 130, 59
	.2byte 111, 60
	.2byte 56, 4
	.2byte 55, 4
	.2byte 57, 7
	.2byte 21, 30
	.2byte 18, 31
	.2byte 17, 32
	.2byte 20, 33
	.2byte 19, 34
	.2byte 24, 35
	.2byte 25, 36
	.2byte 26, 37
	.2byte 27, 37
	.2byte 41, 38
	.2byte 42, 39
	.2byte 60, 40
	.2byte 61, 41
	.2byte 16, 42
	.2byte 37, 43
	.2byte 38, 44
	.2byte 36, 45
	.2byte 29, 46
	.2byte 30, 47
	.2byte 32, 48
	.2byte 31, 49
	.2byte 34, 50
	.2byte 23, 51
	.2byte 33, 52
	.2byte 62, 9
	.2byte 43, 61
	.2byte 44, 62
	.2byte 46, 63
	.2byte 45, 64
	.2byte 104, 65
	.2byte 105, 66
	.2byte 129, 67
	.2byte 121, 68
	.2byte 119, 69
	.2byte 123, 70
	.2byte 63, 73
	.2byte 59, 74
	.2byte 66, 8
	.2byte 65, 10
	.2byte 64, 13
	.2byte 67, 71
	.2byte 68, 72
	.2byte 70, 11
	.2byte 69, 76
	.2byte -1, -1
