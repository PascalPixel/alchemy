	.section .unidentified.080178b4,"a"
	.global Runtime_IrqHandlers
Runtime_IrqHandlers:
	.incbin "baserom.gba", 0x000178b4, 0x00000038
	.section .unidentified.08017cd0,"a"
	.global Data_08017cd0
Data_08017cd0:
	.incbin "baserom.gba", 0x00017cd0, 0x00000014
	.section .unidentified.08017d08,"a"
	.global Data_08017d08
Data_08017d08:
	.incbin "baserom.gba", 0x00017d08, 0x00000008
	.global Data_08017d10
Data_08017d10:
	.incbin "baserom.gba", 0x00017d10, 0x00000054
	.global Flash_Chips
Flash_Chips:
	.incbin "baserom.gba", 0x00017d64, 0x00000014
	.section .unidentified.08017dfc,"a"
	.incbin "baserom.gba", 0x00017dfc, 0x00000018
	.global Flash_ChipUnknown
Flash_ChipUnknown:
	.incbin "baserom.gba", 0x00017e14, 0x00000058
	.section .unidentified.08017e94,"a"
	.incbin "baserom.gba", 0x00017e94, 0x00000088
	.section .unidentified.08017f24,"a"
	.incbin "baserom.gba", 0x00017f24, 0x00000018
	.global Flash_ChipAtmel
Flash_ChipAtmel:
	.incbin "baserom.gba", 0x00017f3c, 0x0000002c
	.global Flash_ChipAtmelLayout
Flash_ChipAtmelLayout:
	.incbin "baserom.gba", 0x00017f68, 0x0000002c
	.section .unidentified.080f3984,"a"
	.global Data_080f3984
Data_080f3984:
	.incbin "baserom.gba", 0x000f3984, 0x00000040
	.section .unidentified.081c3430,"a"
	.global Data_081c3430
Data_081c3430:
	.incbin "baserom.gba", 0x001c3430, 0x0000000c
	.global Sound_CommandTableTemplate
Sound_CommandTableTemplate:
	.incbin "baserom.gba", 0x001c343c, 0x00000090
	.global Sound_PcmPitchCodes
Sound_PcmPitchCodes:
	.incbin "baserom.gba", 0x001c34cc, 0x000000b4
	.global Sound_PcmFrequencySteps
Sound_PcmFrequencySteps:
	.incbin "baserom.gba", 0x001c3580, 0x00000030
	.global Sound_FrameLengths
Sound_FrameLengths:
	.incbin "baserom.gba", 0x001c35b0, 0x00000018
	.global Sound_CgbPitchCodes
Sound_CgbPitchCodes:
	.incbin "baserom.gba", 0x001c35c8, 0x00000084
	.global Sound_CgbFrequencySteps
Sound_CgbFrequencySteps:
	.incbin "baserom.gba", 0x001c364c, 0x00000018
	.global Sound_NoisePitchCodes
Sound_NoisePitchCodes:
	.incbin "baserom.gba", 0x001c3664, 0x0000003c
	.global Sound_Cgb3LevelCodes
Sound_Cgb3LevelCodes:
	.incbin "baserom.gba", 0x001c36a0, 0x00000010
	.global Sound_ClockLengths
Sound_ClockLengths:
	.incbin "baserom.gba", 0x001c36b0, 0x00000031
	.global Data_081c36e1
Data_081c36e1:
	.incbin "baserom.gba", 0x001c36e1, 0x00000003
	.global Sound_ExtendedCommandTable
Sound_ExtendedCommandTable:
	.incbin "baserom.gba", 0x001c36e4, 0x00000030
	.section .unidentified.081c43b0,"a"
	.incbin "baserom.gba", 0x001c43b0, 0x00000090
	.section .unidentified.081c44d0,"a"
	.global Sound_PlayerSlots
Sound_PlayerSlots:
	.incbin "baserom.gba", 0x001c44d0, 0x00000060
	.section .unidentified.0885af9d,"a"
	.incbin "baserom.gba", 0x0085af9d, 0x00000003
	.section .unidentified.088902a1,"a"
	.incbin "baserom.gba", 0x008902a1, 0x00000003
	.section .unidentified.088aba5d,"a"
	.incbin "baserom.gba", 0x008aba5d, 0x00000003
	.section .unidentified.0893c371,"a"
	.incbin "baserom.gba", 0x0093c371, 0x00000003
	.section .unidentified.08949e9a,"a"
	.incbin "baserom.gba", 0x00949e9a, 0x00000002
	.section .unidentified.08969da9,"a"
	.incbin "baserom.gba", 0x00969da9, 0x00000003
	.section .unidentified.08969f7e,"a"
	.incbin "baserom.gba", 0x00969f7e, 0x00000002
	.section .unidentified.0896a467,"a"
	.incbin "baserom.gba", 0x0096a467, 0x00000001
	.section .unidentified.08a3f0c6,"a"
	.incbin "baserom.gba", 0x00a3f0c6, 0x00000002
	.section .unidentified.08b41536,"a"
	.incbin "baserom.gba", 0x00b41536, 0x00000002
	.section .unidentified.08bad636,"a"
	.incbin "baserom.gba", 0x00bad636, 0x00000002
	.section .unidentified.08c2e395,"a"
	.incbin "baserom.gba", 0x00c2e395, 0x00000003
	.section .unidentified.08d3fb37,"a"
	.incbin "baserom.gba", 0x00d3fb37, 0x00000001
	.section .unidentified.08d57eb2,"a"
	.incbin "baserom.gba", 0x00d57eb2, 0x00000002
	.section .unidentified.08e0e011,"a"
	.incbin "baserom.gba", 0x00e0e011, 0x00000003
	.section .unidentified.08eba3d1,"a"
	.incbin "baserom.gba", 0x00eba3d1, 0x00000003
