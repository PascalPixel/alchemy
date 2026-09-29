@ Unidentified ROM data, read from your own ROM at build time as early pret
@ projects read their base ROM. Each section shrinks as its data gains source.
	.section .unidentified.08000000,"a"
	.incbin "baserom.gba", 0x00000000, 0x000000c0
	.section .unidentified.080178b4,"a"
	.incbin "baserom.gba", 0x000178b4, 0x00000430
	.section .unidentified.08017d08,"a"
	.incbin "baserom.gba", 0x00017d08, 0x000082f8
	.section .unidentified.080203a8,"a"
	.incbin "baserom.gba", 0x000203a8, 0x00000a00
	.section .unidentified.0802e89c,"a"
	.incbin "baserom.gba", 0x0002e89c, 0x000003ac
	.global Data_0802ec48
Data_0802ec48:
	.incbin "baserom.gba", 0x0002ec48, 0x0000027c
	.global Data_0802eec4
Data_0802eec4:
	.incbin "baserom.gba", 0x0002eec4, 0x0000030c
	.global ObjectDispatch_Table4
ObjectDispatch_Table4:
	.incbin "baserom.gba", 0x0002f1d0, 0x00000030
	.global ObjectDispatch_Table6
ObjectDispatch_Table6:
	.incbin "baserom.gba", 0x0002f200, 0x000000dc
	.global Data_0802f2dc
Data_0802f2dc:
	.incbin "baserom.gba", 0x0002f2dc, 0x00001034
	.section .unidentified.08030320,"a"
	.incbin "baserom.gba", 0x00030320, 0x00007ce0
	.section .unidentified.0804e584,"a"
	.incbin "baserom.gba", 0x0004e584, 0x000005d4
	.global Data_0804eb58
Data_0804eb58:
	.incbin "baserom.gba", 0x0004eb58, 0x000005cc
	.global Data_0804f124
Data_0804f124:
	.incbin "baserom.gba", 0x0004f124, 0x000058f0
	.global Data_08054a14
Data_08054a14:
	.incbin "baserom.gba", 0x00054a14, 0x00000410
	.global Data_08054e24
Data_08054e24:
	.incbin "baserom.gba", 0x00054e24, 0x0000aaf0
	.section .unidentified.080aa0dc,"a"
	.incbin "baserom.gba", 0x000aa0dc, 0x00002f24
	.section .unidentified.080b127c,"a"
	.incbin "baserom.gba", 0x000b127c, 0x00000cac
	.section .unidentified.080b1f2c,"a"
	.incbin "baserom.gba", 0x000b1f2c, 0x0000f6c8
	.global Data_080c15f4
Data_080c15f4:
	.incbin "baserom.gba", 0x000c15f4, 0x000055bc
	.global Djinn_Definitions
Djinn_Definitions:
	.incbin "baserom.gba", 0x000c6bb0, 0x00001450
	.section .unidentified.080c89cc,"a"
	.incbin "baserom.gba", 0x000c89cc, 0x00000a00
	.section .unidentified.080ed80c,"a"
	.incbin "baserom.gba", 0x000ed80c, 0x00002010
	.section .unidentified.080ef824,"a"
	.incbin "baserom.gba", 0x000ef824, 0x00001f84
	.global Data_080f17a8
Data_080f17a8:
	.incbin "baserom.gba", 0x000f17a8, 0x00001b28
	.global Object_OffsetMotionScript
Object_OffsetMotionScript:
	.incbin "baserom.gba", 0x000f32d0, 0x000004b0
	.global Object_LinkedMotionScript
Object_LinkedMotionScript:
	.incbin "baserom.gba", 0x000f3780, 0x00004880
	.section .unidentified.081055f8,"a"
	.incbin "baserom.gba", 0x001055f8, 0x00002a08
	.section .unidentified.0810c008,"a"
	.incbin "baserom.gba", 0x0010c008, 0x0000bff8
	.section .unidentified.081287c4,"a"
	.incbin "baserom.gba", 0x001287c4, 0x00000080
	.global Data_08128844
Data_08128844:
	.incbin "baserom.gba", 0x00128844, 0x00004630
	.section .unidentified.0812ce7c,"a"
	.incbin "baserom.gba", 0x0012ce7c, 0x0000b184
	.section .unidentified.08161ac4,"a"
	.incbin "baserom.gba", 0x00161ac4, 0x00001966
	.section .unidentified.08196dd8,"a"
	.incbin "baserom.gba", 0x00196dd8, 0x00000204
	.section .unidentified.081973f0,"a"
	.incbin "baserom.gba", 0x001973f0, 0x00008c10
	.section .unidentified.081a1928,"a"
	.incbin "baserom.gba", 0x001a1928, 0x000046d8
	.section .unidentified.081a8288,"a"
	.incbin "baserom.gba", 0x001a8288, 0x00003d78
	.section .unidentified.081ad348,"a"
	.incbin "baserom.gba", 0x001ad348, 0x00004cb8
	.section .unidentified.081b4888,"a"
	.incbin "baserom.gba", 0x001b4888, 0x00003778
	.section .unidentified.081ba30c,"a"
	.incbin "baserom.gba", 0x001ba30c, 0x00005cf4
	.section .unidentified.081c342e,"a"
	.incbin "baserom.gba", 0x001c342e, 0x0000009e
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
	.incbin "baserom.gba", 0x001c3664, 0x00000080
	.global Sound_ExtendedCommandTable
Sound_ExtendedCommandTable:
	.incbin "baserom.gba", 0x001c36e4, 0x00000030
	.section .unidentified.081c43b0,"a"
	.incbin "baserom.gba", 0x001c43b0, 0x00000090
	.section .unidentified.081c44d0,"a"
	.global Sound_PlayerSlots
Sound_PlayerSlots:
	.incbin "baserom.gba", 0x001c44d0, 0x00000060
	.section .unidentified.082f9030,"a"
	.incbin "baserom.gba", 0x002f9030, 0x00386fd0
	.global Data_08680000
Data_08680000:
	.incbin "baserom.gba", 0x00680000, 0x00843878
	.section .unidentified.08f79646,"a"
	.incbin "baserom.gba", 0x00f79646, 0x000869ba
