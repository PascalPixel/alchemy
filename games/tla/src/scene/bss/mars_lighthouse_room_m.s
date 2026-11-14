.syntax unified
	.thumb
	.section .bss,"aw",%nobits
	.global gPaletteCycle_Flash
gPaletteCycle_Flash:
	.space 0x00000004
	.global gPaletteCycle_Order
gPaletteCycle_Order:
	.space 0x00000004
	.global gPaletteCycle_Countdown
gPaletteCycle_Countdown:
	.space 0x00000004
	.global gPaletteCycle_Period
gPaletteCycle_Period:
	.space 0x00000004
	.global gPaletteCycle_Slots
gPaletteCycle_Slots:
	.space 0x00000004
	.global gPaletteCycle_FlashColors
gPaletteCycle_FlashColors:
	.space 0x00000004
	.global gPaletteCycle_Colors
gPaletteCycle_Colors:
	.space 0x00000004
	.global gPaletteCycle_Count
gPaletteCycle_Count:
	.space 0x00000004
	.global gPaletteCycle_Step
gPaletteCycle_Step:
	.space 0x00000004
	.global gMarsLighthouseRoomM_LedgeFlameColors
gMarsLighthouseRoomM_LedgeFlameColors:
	.space 0x00000040
	.global gMarsLighthouseRoomM_LedgePalette
gMarsLighthouseRoomM_LedgePalette:
	.space 0x00000040
	.global gMarsLighthouseRoomM_LedgeSavedPalette
gMarsLighthouseRoomM_LedgeSavedPalette:
	.space 0x00000020
	.global gMarsLighthouseRoomM_HallFlameColors
gMarsLighthouseRoomM_HallFlameColors:
	.space 0x00000040
	.global gMarsLighthouseRoomM_HallPalette
gMarsLighthouseRoomM_HallPalette:
	.space 0x00000040
	.global gMarsLighthouseRoomM_HallSavedPalette
gMarsLighthouseRoomM_HallSavedPalette:
	.space 0x00000014
	.global gMarsLighthouseRoomM_UnlitFlameColors
gMarsLighthouseRoomM_UnlitFlameColors:
	.space 0x0000000c
	.global gMarsLighthouseRoomM_DoorwayBlend
gMarsLighthouseRoomM_DoorwayBlend:
	.space 0x00000002
	.global gMarsLighthouseRoomM_DoorwayBlendTarget
gMarsLighthouseRoomM_DoorwayBlendTarget:
	.space 0x00000002
	.global gMarsLighthouseRoomM_Doorway
gMarsLighthouseRoomM_Doorway:
	.space 0x00000002
	.global gMarsLighthouseRoomM_DoorwayStep
gMarsLighthouseRoomM_DoorwayStep:
