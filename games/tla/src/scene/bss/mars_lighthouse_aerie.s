.syntax unified
	.thumb
	.section .bss,"aw",%nobits
	.balign 16
	.global gMarsLighthouseAerie_BeaconFlameFrame
gMarsLighthouseAerie_BeaconFlameFrame:
	.space 0x00000004
	.global gMarsLighthouseAerie_FlickerFrame
gMarsLighthouseAerie_FlickerFrame:
	.space 0x00000004
	.global gMarsLighthouseAerie_BeaconFireFrames
gMarsLighthouseAerie_BeaconFireFrames:
	.space 0x00000004
	.global gMarsLighthouseAerie_FlickerRate
gMarsLighthouseAerie_FlickerRate:
	.space 0x00000004
	.global gMarsLighthouseAerie_RisingRingsB
gMarsLighthouseAerie_RisingRingsB:
	.space 0x000000b0
	.global gMarsLighthouseAerie_RisingRingsA
gMarsLighthouseAerie_RisingRingsA:
	.space 0x000000ac
	.global gMarsLighthouseAerie_BeaconFireFrame
gMarsLighthouseAerie_BeaconFireFrame:
	.space 0x00000004
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
