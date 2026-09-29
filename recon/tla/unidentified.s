@ Unidentified ROM data, read from your own ROM at build time as early pret
@ projects read their base ROM. Each section shrinks as its data gains source.
	.section .unidentified.08000000,"a"
	.global Resource_Data000
Resource_Data000:
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
	.incbin "baserom.gba", 0x00054e24, 0x000056bc
	.section .unidentified.0805c0e0,"a"
	.incbin "baserom.gba", 0x0005c0e0, 0x00003834
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
	.incbin "baserom.gba", 0x001c342e, 0x0000000e
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
	.incbin "baserom.gba", 0x001c3664, 0x0000004c
	.global Sound_ClockLengths
Sound_ClockLengths:
	.incbin "baserom.gba", 0x001c36b0, 0x00000034
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
	.incbin "baserom.gba", 0x002f9030, 0x00006fd0
	.global Resource_Data012
Resource_Data012:
	.incbin "baserom.gba", 0x00300000, 0x00380000
	.section .unidentified.08682000,"a"
	.global Resource_Data002
Resource_Data002:
	.incbin "baserom.gba", 0x00682000, 0x00000010
	.global Resource_Data013
Resource_Data013:
	.incbin "baserom.gba", 0x00682010, 0x00002000
	.global Resource_Data014
Resource_Data014:
	.incbin "baserom.gba", 0x00684010, 0x000008c0
	.global Resource_Data015
Resource_Data015:
	.incbin "baserom.gba", 0x006848d0, 0x000000c0
	.global Resource_Data016
Resource_Data016:
	.incbin "baserom.gba", 0x00684990, 0x00005a70
	.global Resource_Data017
Resource_Data017:
	.incbin "baserom.gba", 0x0068a400, 0x000086f8
	.global Resource_Data018
Resource_Data018:
	.incbin "baserom.gba", 0x00692af8, 0x00005250
	.global Resource_Data019
Resource_Data019:
	.incbin "baserom.gba", 0x00697d48, 0x0000b9a8
	.global Resource_Data01A
Resource_Data01A:
	.incbin "baserom.gba", 0x006a36f0, 0x000011f0
	.global Resource_Data01B
Resource_Data01B:
	.incbin "baserom.gba", 0x006a48e0, 0x00000200
	.global Resource_Data01C
Resource_Data01C:
	.incbin "baserom.gba", 0x006a4ae0, 0x00000770
	.global Resource_Data01D
Resource_Data01D:
	.incbin "baserom.gba", 0x006a5250, 0x00000984
	.global Resource_Data01E
Resource_Data01E:
	.incbin "baserom.gba", 0x006a5bd4, 0x00000878
	.global Resource_Data01F
Resource_Data01F:
	.incbin "baserom.gba", 0x006a644c, 0x000005d4
	.global Resource_Data020
Resource_Data020:
	.incbin "baserom.gba", 0x006a6a20, 0x00000828
	.global Resource_Data021
Resource_Data021:
	.incbin "baserom.gba", 0x006a7248, 0x0000186c
	.global Resource_Data022
Resource_Data022:
	.incbin "baserom.gba", 0x006a8ab4, 0x000015b8
	.global Resource_Data023
Resource_Data023:
	.incbin "baserom.gba", 0x006aa06c, 0x0000761c
	.global Resource_Data024
Resource_Data024:
	.incbin "baserom.gba", 0x006b1688, 0x0001bf8c
	.global Resource_Data025
Resource_Data025:
	.incbin "baserom.gba", 0x006cd614, 0x000002b4
	.global Resource_Data026
Resource_Data026:
	.incbin "baserom.gba", 0x006cd8c8, 0x00000f0c
	.section .unidentified.086d11c2,"a"
	.incbin "baserom.gba", 0x006d11c2, 0x00000002
	.section .unidentified.086d5606,"a"
	.incbin "baserom.gba", 0x006d5606, 0x00000002
	.section .unidentified.086e5d76,"a"
	.incbin "baserom.gba", 0x006e5d76, 0x00000002
	.section .unidentified.086e9366,"a"
	.incbin "baserom.gba", 0x006e9366, 0x00000002
	.section .unidentified.086f4e0a,"a"
	.incbin "baserom.gba", 0x006f4e0a, 0x00000002
	.section .unidentified.087054ba,"a"
	.incbin "baserom.gba", 0x007054ba, 0x00000002
	.section .unidentified.0870cf5a,"a"
	.incbin "baserom.gba", 0x0070cf5a, 0x00000002
	.section .unidentified.0871079a,"a"
	.incbin "baserom.gba", 0x0071079a, 0x00000002
	.section .unidentified.08719bce,"a"
	.incbin "baserom.gba", 0x00719bce, 0x00000002
	.section .unidentified.08720d7a,"a"
	.incbin "baserom.gba", 0x00720d7a, 0x00000002
	.section .unidentified.08728c36,"a"
	.incbin "baserom.gba", 0x00728c36, 0x00000002
	.section .unidentified.0872d6c6,"a"
	.incbin "baserom.gba", 0x0072d6c6, 0x00000002
	.section .unidentified.0873197a,"a"
	.incbin "baserom.gba", 0x0073197a, 0x00000002
	.section .unidentified.087352fa,"a"
	.incbin "baserom.gba", 0x007352fa, 0x00000002
	.section .unidentified.08741b36,"a"
	.incbin "baserom.gba", 0x00741b36, 0x00000002
	.section .unidentified.0874d276,"a"
	.incbin "baserom.gba", 0x0074d276, 0x00000002
	.section .unidentified.08750646,"a"
	.incbin "baserom.gba", 0x00750646, 0x00000002
	.section .unidentified.08762562,"a"
	.incbin "baserom.gba", 0x00762562, 0x00000002
	.section .unidentified.08765fca,"a"
	.incbin "baserom.gba", 0x00765fca, 0x00000002
	.section .unidentified.087699ba,"a"
	.incbin "baserom.gba", 0x007699ba, 0x00000002
	.section .unidentified.08779f22,"a"
	.incbin "baserom.gba", 0x00779f22, 0x00000002
	.section .unidentified.0877de52,"a"
	.incbin "baserom.gba", 0x0077de52, 0x00000002
	.section .unidentified.08781876,"a"
	.incbin "baserom.gba", 0x00781876, 0x00000002
	.section .unidentified.08789b96,"a"
	.incbin "baserom.gba", 0x00789b96, 0x00000002
	.section .unidentified.08791d02,"a"
	.incbin "baserom.gba", 0x00791d02, 0x00000002
	.section .unidentified.0879efce,"a"
	.incbin "baserom.gba", 0x0079efce, 0x00000002
	.section .unidentified.087a2e8a,"a"
	.incbin "baserom.gba", 0x007a2e8a, 0x00000002
	.section .unidentified.087a6c86,"a"
	.incbin "baserom.gba", 0x007a6c86, 0x00000002
	.section .unidentified.087ab156,"a"
	.incbin "baserom.gba", 0x007ab156, 0x00000002
	.section .unidentified.087b2b32,"a"
	.incbin "baserom.gba", 0x007b2b32, 0x00000002
	.section .unidentified.087b715e,"a"
	.incbin "baserom.gba", 0x007b715e, 0x00000002
	.section .unidentified.087bedda,"a"
	.incbin "baserom.gba", 0x007bedda, 0x00000002
	.section .unidentified.087c29d6,"a"
	.incbin "baserom.gba", 0x007c29d6, 0x00000002
	.section .unidentified.087c6386,"a"
	.incbin "baserom.gba", 0x007c6386, 0x00000002
	.section .unidentified.087ca7d6,"a"
	.incbin "baserom.gba", 0x007ca7d6, 0x00000002
	.section .unidentified.087ce3ce,"a"
	.incbin "baserom.gba", 0x007ce3ce, 0x00000002
	.section .unidentified.087da02e,"a"
	.incbin "baserom.gba", 0x007da02e, 0x00000002
	.section .unidentified.087ddc7e,"a"
	.incbin "baserom.gba", 0x007ddc7e, 0x00000002
	.section .unidentified.087e61fe,"a"
	.incbin "baserom.gba", 0x007e61fe, 0x00000002
	.section .unidentified.087f284e,"a"
	.incbin "baserom.gba", 0x007f284e, 0x00000002
	.section .unidentified.087f9302,"a"
	.incbin "baserom.gba", 0x007f9302, 0x00000002
	.section .unidentified.088025ca,"a"
	.incbin "baserom.gba", 0x008025ca, 0x00000002
	.section .unidentified.08809fee,"a"
	.incbin "baserom.gba", 0x00809fee, 0x00000002
	.section .unidentified.0881fc6e,"a"
	.incbin "baserom.gba", 0x0081fc6e, 0x00000002
	.section .unidentified.088290b2,"a"
	.incbin "baserom.gba", 0x008290b2, 0x00000002
	.section .unidentified.088322ce,"a"
	.incbin "baserom.gba", 0x008322ce, 0x00000002
	.section .unidentified.0883fbf6,"a"
	.incbin "baserom.gba", 0x0083fbf6, 0x00000002
	.section .unidentified.08845a4a,"a"
	.incbin "baserom.gba", 0x00845a4a, 0x00000002
	.section .unidentified.0884b76a,"a"
	.incbin "baserom.gba", 0x0084b76a, 0x00000002
	.global Resource_Data081
Resource_Data081:
	.incbin "baserom.gba", 0x0084b76c, 0x00000784
	.section .unidentified.0884d224,"a"
	.global Resource_Data083
Resource_Data083:
	.incbin "baserom.gba", 0x0084d224, 0x0000061c
	.global Resource_Data084
Resource_Data084:
	.incbin "baserom.gba", 0x0084d840, 0x00002e20
	.global Resource_Data085
Resource_Data085:
	.incbin "baserom.gba", 0x00850660, 0x00003f14
	.global Resource_Data086
Resource_Data086:
	.incbin "baserom.gba", 0x00854574, 0x00003024
	.global Resource_Data087
Resource_Data087:
	.incbin "baserom.gba", 0x00857598, 0x000009bc
	.global Resource_Data088
Resource_Data088:
	.incbin "baserom.gba", 0x00857f54, 0x00000458
	.global Resource_Data089
Resource_Data089:
	.incbin "baserom.gba", 0x008583ac, 0x00000410
	.global Resource_Data08A
Resource_Data08A:
	.incbin "baserom.gba", 0x008587bc, 0x000002e8
	.global Resource_Data08B
Resource_Data08B:
	.incbin "baserom.gba", 0x00858aa4, 0x00000278
	.global Resource_Data08C
Resource_Data08C:
	.incbin "baserom.gba", 0x00858d1c, 0x00000494
	.global Resource_Data08D
Resource_Data08D:
	.incbin "baserom.gba", 0x008591b0, 0x00000490
	.global Resource_Data08E
Resource_Data08E:
	.incbin "baserom.gba", 0x00859640, 0x00000370
	.global Resource_Data08F
Resource_Data08F:
	.incbin "baserom.gba", 0x008599b0, 0x00000488
	.global Resource_Data090
Resource_Data090:
	.incbin "baserom.gba", 0x00859e38, 0x00000734
	.global Resource_Data091
Resource_Data091:
	.incbin "baserom.gba", 0x0085a56c, 0x000000e0
	.global Resource_Data092
Resource_Data092:
	.incbin "baserom.gba", 0x0085a64c, 0x000001bc
	.global Resource_Data093
Resource_Data093:
	.incbin "baserom.gba", 0x0085a808, 0x000003cc
	.global Resource_Data094
Resource_Data094:
	.incbin "baserom.gba", 0x0085abd4, 0x000003cc
	.global Resource_Data095
Resource_Data095:
	.incbin "baserom.gba", 0x0085afa0, 0x00000398
	.global Resource_Data096
Resource_Data096:
	.incbin "baserom.gba", 0x0085b338, 0x00000664
	.global Resource_Data097
Resource_Data097:
	.incbin "baserom.gba", 0x0085b99c, 0x00000294
	.global Resource_Data098
Resource_Data098:
	.incbin "baserom.gba", 0x0085bc30, 0x000014a0
	.global Resource_Data099
Resource_Data099:
	.incbin "baserom.gba", 0x0085d0d0, 0x00001200
	.global Resource_Data09A
Resource_Data09A:
	.incbin "baserom.gba", 0x0085e2d0, 0x0000077c
	.global Resource_Data09B
Resource_Data09B:
	.incbin "baserom.gba", 0x0085ea4c, 0x00001644
	.global Resource_Data09C
Resource_Data09C:
	.incbin "baserom.gba", 0x00860090, 0x0000052c
	.global Resource_Data09D
Resource_Data09D:
	.incbin "baserom.gba", 0x008605bc, 0x000005ac
	.global Resource_Data09E
Resource_Data09E:
	.incbin "baserom.gba", 0x00860b68, 0x0000025c
	.global Resource_Data09F
Resource_Data09F:
	.incbin "baserom.gba", 0x00860dc4, 0x00000578
	.global Resource_Data0A0
Resource_Data0A0:
	.incbin "baserom.gba", 0x0086133c, 0x00001740
	.global Resource_Data0A1
Resource_Data0A1:
	.incbin "baserom.gba", 0x00862a7c, 0x00002ec8
	.global Resource_Data0A2
Resource_Data0A2:
	.incbin "baserom.gba", 0x00865944, 0x000002b4
	.global Resource_Data0A3
Resource_Data0A3:
	.incbin "baserom.gba", 0x00865bf8, 0x000013e0
	.global Resource_Data0A4
Resource_Data0A4:
	.incbin "baserom.gba", 0x00866fd8, 0x0000483c
	.global Resource_Data0A5
Resource_Data0A5:
	.incbin "baserom.gba", 0x0086b814, 0x00002138
	.global Resource_Data0A6
Resource_Data0A6:
	.incbin "baserom.gba", 0x0086d94c, 0x00001670
	.global Resource_Data0A7
Resource_Data0A7:
	.incbin "baserom.gba", 0x0086efbc, 0x00001d24
	.global Resource_Data0A8
Resource_Data0A8:
	.incbin "baserom.gba", 0x00870ce0, 0x00001f00
	.global Resource_Data0A9
Resource_Data0A9:
	.incbin "baserom.gba", 0x00872be0, 0x000019ac
	.global Resource_Data0AA
Resource_Data0AA:
	.incbin "baserom.gba", 0x0087458c, 0x00001578
	.global Resource_Data0AB
Resource_Data0AB:
	.incbin "baserom.gba", 0x00875b04, 0x000036a0
	.global Resource_Data0AC
Resource_Data0AC:
	.incbin "baserom.gba", 0x008791a4, 0x00000594
	.global Resource_Data0AD
Resource_Data0AD:
	.incbin "baserom.gba", 0x00879738, 0x00000360
	.global Resource_Data0AE
Resource_Data0AE:
	.incbin "baserom.gba", 0x00879a98, 0x000004ec
	.global Resource_Data0AF
Resource_Data0AF:
	.incbin "baserom.gba", 0x00879f84, 0x00000310
	.global Resource_Data0B0
Resource_Data0B0:
	.incbin "baserom.gba", 0x0087a294, 0x00001c50
	.global Resource_Data0B1
Resource_Data0B1:
	.incbin "baserom.gba", 0x0087bee4, 0x00000440
	.global Resource_Data0B2
Resource_Data0B2:
	.incbin "baserom.gba", 0x0087c324, 0x0000024c
	.global Resource_Data0B3
Resource_Data0B3:
	.incbin "baserom.gba", 0x0087c570, 0x00000198
	.global Resource_Data0B4
Resource_Data0B4:
	.incbin "baserom.gba", 0x0087c708, 0x0000082c
	.global Resource_Data0B5
Resource_Data0B5:
	.incbin "baserom.gba", 0x0087cf34, 0x00000e98
	.section .unidentified.0887e403,"a"
	.incbin "baserom.gba", 0x0087e403, 0x00000001
	.section .unidentified.0888056d,"a"
	.incbin "baserom.gba", 0x0088056d, 0x00000003
	.section .unidentified.08880678,"a"
	.global Resource_Data0BA
Resource_Data0BA:
	.incbin "baserom.gba", 0x00880678, 0x0000024c
	.global Resource_Data0BB
Resource_Data0BB:
	.incbin "baserom.gba", 0x008808c4, 0x00000184
	.section .unidentified.08881301,"a"
	.incbin "baserom.gba", 0x00881301, 0x00000003
	.section .unidentified.08882ee5,"a"
	.incbin "baserom.gba", 0x00882ee5, 0x00000003
	.section .unidentified.08884a42,"a"
	.incbin "baserom.gba", 0x00884a42, 0x00000002
	.section .unidentified.08884e81,"a"
	.incbin "baserom.gba", 0x00884e81, 0x00000003
	.section .unidentified.08885296,"a"
	.incbin "baserom.gba", 0x00885296, 0x00000002
	.global Resource_Data0C1
Resource_Data0C1:
	.incbin "baserom.gba", 0x00885298, 0x00000338
	.global Resource_Data0C2
Resource_Data0C2:
	.incbin "baserom.gba", 0x008855d0, 0x000010cc
	.global Resource_Data0C3
Resource_Data0C3:
	.incbin "baserom.gba", 0x0088669c, 0x000002e8
	.global Resource_Data0C4
Resource_Data0C4:
	.incbin "baserom.gba", 0x00886984, 0x00000154
	.section .unidentified.08888fe3,"a"
	.incbin "baserom.gba", 0x00888fe3, 0x00000001
	.section .unidentified.0888999f,"a"
	.incbin "baserom.gba", 0x0088999f, 0x00000001
	.section .unidentified.0888aae2,"a"
	.incbin "baserom.gba", 0x0088aae2, 0x00000002
	.global Resource_Data0C8
Resource_Data0C8:
	.incbin "baserom.gba", 0x0088aae4, 0x00000c7c
	.global Resource_Data0C9
Resource_Data0C9:
	.incbin "baserom.gba", 0x0088b760, 0x0000002c
	.global Resource_Data0CA
Resource_Data0CA:
	.incbin "baserom.gba", 0x0088b78c, 0x0000002c
	.global Resource_Data0CB
Resource_Data0CB:
	.incbin "baserom.gba", 0x0088b7b8, 0x000007c0
	.section .unidentified.0888d4f3,"a"
	.incbin "baserom.gba", 0x0088d4f3, 0x00000001
	.section .unidentified.0888d81d,"a"
	.incbin "baserom.gba", 0x0088d81d, 0x00000003
	.global Resource_Data0CE
Resource_Data0CE:
	.incbin "baserom.gba", 0x0088d820, 0x00000528
	.global Resource_Data0CF
Resource_Data0CF:
	.incbin "baserom.gba", 0x0088dd48, 0x00000530
	.global Resource_Data0D0
Resource_Data0D0:
	.incbin "baserom.gba", 0x0088e278, 0x000005f0
	.section .unidentified.0888eff1,"a"
	.incbin "baserom.gba", 0x0088eff1, 0x00000003
	.global Resource_Data0D2
Resource_Data0D2:
	.incbin "baserom.gba", 0x0088eff4, 0x00000200
	.global Resource_Data0D3
Resource_Data0D3:
	.incbin "baserom.gba", 0x0088f1f4, 0x00000420
	.section .unidentified.0888fc3d,"a"
	.incbin "baserom.gba", 0x0088fc3d, 0x00000003
	.section .unidentified.08890047,"a"
	.incbin "baserom.gba", 0x00890047, 0x00000001
	.global Resource_Data0D7
Resource_Data0D7:
	.incbin "baserom.gba", 0x00890048, 0x0000025c
	.global Resource_Data0D8
Resource_Data0D8:
	.incbin "baserom.gba", 0x008902a4, 0x0000057c
	.section .unidentified.08890ac9,"a"
	.incbin "baserom.gba", 0x00890ac9, 0x00000003
	.global Resource_Data0DA
Resource_Data0DA:
	.incbin "baserom.gba", 0x00890acc, 0x0000095c
	.section .unidentified.08891ae5,"a"
	.incbin "baserom.gba", 0x00891ae5, 0x00000003
	.global Resource_Data0DC
Resource_Data0DC:
	.incbin "baserom.gba", 0x00891ae8, 0x00002ab0
	.global Resource_Data0DD
Resource_Data0DD:
	.incbin "baserom.gba", 0x00894598, 0x000011cc
	.section .unidentified.08895dc3,"a"
	.incbin "baserom.gba", 0x00895dc3, 0x00000001
	.section .unidentified.08896dfd,"a"
	.incbin "baserom.gba", 0x00896dfd, 0x00000003
	.section .unidentified.08897453,"a"
	.incbin "baserom.gba", 0x00897453, 0x00000001
	.section .unidentified.08897ad1,"a"
	.incbin "baserom.gba", 0x00897ad1, 0x00000003
	.section .unidentified.088980f1,"a"
	.incbin "baserom.gba", 0x008980f1, 0x00000003
	.section .unidentified.088994be,"a"
	.incbin "baserom.gba", 0x008994be, 0x00000002
	.section .unidentified.0889a502,"a"
	.incbin "baserom.gba", 0x0089a502, 0x00000002
	.section .unidentified.0889af8b,"a"
	.incbin "baserom.gba", 0x0089af8b, 0x00000001
	.global Resource_Data0E9
Resource_Data0E9:
	.incbin "baserom.gba", 0x0089af8c, 0x000002cc
	.section .unidentified.0889bf91,"a"
	.incbin "baserom.gba", 0x0089bf91, 0x00000003
	.section .unidentified.0889d1cd,"a"
	.incbin "baserom.gba", 0x0089d1cd, 0x00000003
	.section .unidentified.0889db17,"a"
	.incbin "baserom.gba", 0x0089db17, 0x00000001
	.global Resource_Data0EE
Resource_Data0EE:
	.incbin "baserom.gba", 0x0089db18, 0x0000065c
	.global Resource_Data0EF
Resource_Data0EF:
	.incbin "baserom.gba", 0x0089e174, 0x0000052c
	.global Resource_Data0F0
Resource_Data0F0:
	.incbin "baserom.gba", 0x0089e6a0, 0x000022bc
	.global Resource_Data0F1
Resource_Data0F1:
	.incbin "baserom.gba", 0x008a095c, 0x00001794
	.global Resource_Data0F2
Resource_Data0F2:
	.incbin "baserom.gba", 0x008a20f0, 0x000006e4
	.global Resource_Data0F3
Resource_Data0F3:
	.incbin "baserom.gba", 0x008a27d4, 0x00001f4c
	.section .unidentified.088a4c54,"a"
	.global Resource_Data0F5
Resource_Data0F5:
	.incbin "baserom.gba", 0x008a4c54, 0x000011d8
	.section .unidentified.088a631a,"a"
	.incbin "baserom.gba", 0x008a631a, 0x00000002
	.global Resource_Data0F7
Resource_Data0F7:
	.incbin "baserom.gba", 0x008a631c, 0x00000648
	.global Resource_Data0F8
Resource_Data0F8:
	.incbin "baserom.gba", 0x008a6964, 0x00000c24
	.global Resource_Data0F9
Resource_Data0F9:
	.incbin "baserom.gba", 0x008a7588, 0x000003c4
	.global Resource_Data0FA
Resource_Data0FA:
	.incbin "baserom.gba", 0x008a794c, 0x000001c8
	.global Resource_Data0FB
Resource_Data0FB:
	.incbin "baserom.gba", 0x008a7b14, 0x0000054c
	.global Resource_Data0FC
Resource_Data0FC:
	.incbin "baserom.gba", 0x008a8060, 0x0000034c
	.global Resource_Data0FD
Resource_Data0FD:
	.incbin "baserom.gba", 0x008a83ac, 0x0000076c
	.section .unidentified.088a915b,"a"
	.incbin "baserom.gba", 0x008a915b, 0x00000001
	.global Resource_Data0FF
Resource_Data0FF:
	.incbin "baserom.gba", 0x008a915c, 0x000000a8
	.section .unidentified.088a951a,"a"
	.incbin "baserom.gba", 0x008a951a, 0x00000002
	.global Resource_Data101
Resource_Data101:
	.incbin "baserom.gba", 0x008a951c, 0x000009a8
	.global Resource_Data102
Resource_Data102:
	.incbin "baserom.gba", 0x008a9ec4, 0x000002f8
	.global Resource_Data103
Resource_Data103:
	.incbin "baserom.gba", 0x008aa1bc, 0x00000b30
	.global Resource_Data104
Resource_Data104:
	.incbin "baserom.gba", 0x008aacec, 0x00000100
	.section .unidentified.088aba5d,"a"
	.incbin "baserom.gba", 0x008aba5d, 0x00000003
	.global Resource_Data106
Resource_Data106:
	.incbin "baserom.gba", 0x008aba60, 0x00000848
	.global Resource_Data107
Resource_Data107:
	.incbin "baserom.gba", 0x008ac2a8, 0x00001200
	.section .unidentified.088ae367,"a"
	.incbin "baserom.gba", 0x008ae367, 0x00000001
	.section .unidentified.088aee43,"a"
	.incbin "baserom.gba", 0x008aee43, 0x00000001
	.section .unidentified.088af506,"a"
	.incbin "baserom.gba", 0x008af506, 0x00000002
	.section .unidentified.088af7ed,"a"
	.incbin "baserom.gba", 0x008af7ed, 0x00000003
	.section .unidentified.088b0b53,"a"
	.incbin "baserom.gba", 0x008b0b53, 0x00000001
	.section .unidentified.088b0f3d,"a"
	.incbin "baserom.gba", 0x008b0f3d, 0x00000003
	.section .unidentified.088b130f,"a"
	.incbin "baserom.gba", 0x008b130f, 0x00000001
	.section .unidentified.088b1baf,"a"
	.incbin "baserom.gba", 0x008b1baf, 0x00000001
	.section .unidentified.088b2052,"a"
	.incbin "baserom.gba", 0x008b2052, 0x00000002
	.section .unidentified.088b320b,"a"
	.incbin "baserom.gba", 0x008b320b, 0x00000001
	.section .unidentified.088b3664,"a"
	.global Resource_Data119
Resource_Data119:
	.incbin "baserom.gba", 0x008b3664, 0x0000106c
	.global Resource_Data11A
Resource_Data11A:
	.incbin "baserom.gba", 0x008b46d0, 0x00000fc8
	.section .unidentified.088b58f2,"a"
	.incbin "baserom.gba", 0x008b58f2, 0x00000002
	.section .unidentified.088b734d,"a"
	.incbin "baserom.gba", 0x008b734d, 0x00000003
	.global Resource_Data11E
Resource_Data11E:
	.incbin "baserom.gba", 0x008b7350, 0x000001b4
	.section .unidentified.088b7899,"a"
	.incbin "baserom.gba", 0x008b7899, 0x00000003
	.global Resource_Data120
Resource_Data120:
	.incbin "baserom.gba", 0x008b789c, 0x00001d88
	.global Resource_Data121
Resource_Data121:
	.incbin "baserom.gba", 0x008b9624, 0x00000278
	.section .unidentified.088b9d62,"a"
	.incbin "baserom.gba", 0x008b9d62, 0x00000002
	.section .unidentified.088bb937,"a"
	.incbin "baserom.gba", 0x008bb937, 0x00000001
	.section .unidentified.088bd535,"a"
	.incbin "baserom.gba", 0x008bd535, 0x00000003
	.section .unidentified.088bd756,"a"
	.incbin "baserom.gba", 0x008bd756, 0x00000002
	.global Resource_Data127
Resource_Data127:
	.incbin "baserom.gba", 0x008bd758, 0x0000043c
	.section .unidentified.088bdca5,"a"
	.incbin "baserom.gba", 0x008bdca5, 0x00000003
	.section .unidentified.088be287,"a"
	.incbin "baserom.gba", 0x008be287, 0x00000001
	.global Resource_Data12A
Resource_Data12A:
	.incbin "baserom.gba", 0x008be288, 0x000004a0
	.section .unidentified.088bf43a,"a"
	.incbin "baserom.gba", 0x008bf43a, 0x00000002
	.section .unidentified.088bf97c,"a"
	.global Resource_Data12D
Resource_Data12D:
	.incbin "baserom.gba", 0x008bf97c, 0x00000840
	.section .unidentified.088c054b,"a"
	.incbin "baserom.gba", 0x008c054b, 0x00000001
	.global Resource_Data12F
Resource_Data12F:
	.incbin "baserom.gba", 0x008c054c, 0x00001258
	.section .unidentified.088c2292,"a"
	.incbin "baserom.gba", 0x008c2292, 0x00000002
	.section .unidentified.088c3069,"a"
	.incbin "baserom.gba", 0x008c3069, 0x00000003
	.section .unidentified.088c3896,"a"
	.incbin "baserom.gba", 0x008c3896, 0x00000002
	.section .unidentified.088c3c88,"a"
	.global Resource_Data134
Resource_Data134:
	.incbin "baserom.gba", 0x008c3c88, 0x000001bc
	.global Resource_Data135
Resource_Data135:
	.incbin "baserom.gba", 0x008c3e44, 0x000001bc
	.global Resource_Data136
Resource_Data136:
	.incbin "baserom.gba", 0x008c4000, 0x00000940
	.global Resource_Data137
Resource_Data137:
	.incbin "baserom.gba", 0x008c4940, 0x00000418
	.section .unidentified.088c56c9,"a"
	.incbin "baserom.gba", 0x008c56c9, 0x00000003
	.section .unidentified.088c5a77,"a"
	.incbin "baserom.gba", 0x008c5a77, 0x00000001
	.section .unidentified.088c7a1b,"a"
	.incbin "baserom.gba", 0x008c7a1b, 0x00000001
	.section .unidentified.088c87b7,"a"
	.incbin "baserom.gba", 0x008c87b7, 0x00000001
	.section .unidentified.088c89d3,"a"
	.incbin "baserom.gba", 0x008c89d3, 0x00000001
	.section .unidentified.088c8ccf,"a"
	.incbin "baserom.gba", 0x008c8ccf, 0x00000001
	.section .unidentified.088c9070,"a"
	.global Resource_Data143
Resource_Data143:
	.incbin "baserom.gba", 0x008c9070, 0x000016b0
	.section .unidentified.088cae7d,"a"
	.incbin "baserom.gba", 0x008cae7d, 0x00000003
	.section .unidentified.088cbc4b,"a"
	.incbin "baserom.gba", 0x008cbc4b, 0x00000001
	.global Resource_Data146
Resource_Data146:
	.incbin "baserom.gba", 0x008cbc4c, 0x00000c74
	.section .unidentified.088cceb5,"a"
	.incbin "baserom.gba", 0x008cceb5, 0x00000003
	.section .unidentified.088cd40f,"a"
	.incbin "baserom.gba", 0x008cd40f, 0x00000001
	.section .unidentified.088cecf9,"a"
	.incbin "baserom.gba", 0x008cecf9, 0x00000003
	.global Resource_Data14F
Resource_Data14F:
	.incbin "baserom.gba", 0x008cecfc, 0x000008a0
	.section .unidentified.088cf977,"a"
	.incbin "baserom.gba", 0x008cf977, 0x00000001
	.section .unidentified.088cfc01,"a"
	.incbin "baserom.gba", 0x008cfc01, 0x00000003
	.section .unidentified.088cffa7,"a"
	.incbin "baserom.gba", 0x008cffa7, 0x00000001
	.section .unidentified.088d0202,"a"
	.incbin "baserom.gba", 0x008d0202, 0x00000002
	.section .unidentified.088d05ba,"a"
	.incbin "baserom.gba", 0x008d05ba, 0x00000002
	.section .unidentified.088d1aa3,"a"
	.incbin "baserom.gba", 0x008d1aa3, 0x00000001
	.section .unidentified.088d3066,"a"
	.incbin "baserom.gba", 0x008d3066, 0x00000002
	.global Resource_Data15A
Resource_Data15A:
	.incbin "baserom.gba", 0x008d3068, 0x00000a6c
	.section .unidentified.088d48aa,"a"
	.incbin "baserom.gba", 0x008d48aa, 0x00000002
	.section .unidentified.088d4c2d,"a"
	.incbin "baserom.gba", 0x008d4c2d, 0x00000003
	.section .unidentified.088d58dd,"a"
	.incbin "baserom.gba", 0x008d58dd, 0x00000003
	.section .unidentified.088d6425,"a"
	.incbin "baserom.gba", 0x008d6425, 0x00000003
	.global Resource_Data160
Resource_Data160:
	.incbin "baserom.gba", 0x008d6428, 0x00000198
	.global Resource_Data161
Resource_Data161:
	.incbin "baserom.gba", 0x008d65c0, 0x0000088c
	.section .unidentified.088d7a1a,"a"
	.incbin "baserom.gba", 0x008d7a1a, 0x00000002
	.section .unidentified.088d7f32,"a"
	.incbin "baserom.gba", 0x008d7f32, 0x00000002
	.global Resource_Data16C
Resource_Data16C:
	.incbin "baserom.gba", 0x008d7f34, 0x00000624
	.section .unidentified.088d8979,"a"
	.incbin "baserom.gba", 0x008d8979, 0x00000003
	.section .unidentified.088d8c13,"a"
	.incbin "baserom.gba", 0x008d8c13, 0x00000001
	.section .unidentified.088da114,"a"
	.global Resource_Data170
Resource_Data170:
	.incbin "baserom.gba", 0x008da114, 0x0000049c
	.global Resource_Data171
Resource_Data171:
	.incbin "baserom.gba", 0x008da5b0, 0x0000198c
	.section .unidentified.088dc326,"a"
	.incbin "baserom.gba", 0x008dc326, 0x00000002
	.section .unidentified.088de29b,"a"
	.incbin "baserom.gba", 0x008de29b, 0x00000001
	.section .unidentified.088de76b,"a"
	.incbin "baserom.gba", 0x008de76b, 0x00000001
	.global Resource_Data176
Resource_Data176:
	.incbin "baserom.gba", 0x008de76c, 0x00000694
	.global Resource_Data177
Resource_Data177:
	.incbin "baserom.gba", 0x008dee00, 0x00000a34
	.global Resource_Data178
Resource_Data178:
	.incbin "baserom.gba", 0x008df834, 0x00000bfc
	.section .unidentified.088e0c05,"a"
	.incbin "baserom.gba", 0x008e0c05, 0x00000003
	.section .unidentified.088e16d6,"a"
	.incbin "baserom.gba", 0x008e16d6, 0x00000002
	.section .unidentified.088e2213,"a"
	.incbin "baserom.gba", 0x008e2213, 0x00000001
	.global Resource_Data17C
Resource_Data17C:
	.incbin "baserom.gba", 0x008e2214, 0x00000640
	.global Resource_Data17D
Resource_Data17D:
	.incbin "baserom.gba", 0x008e2854, 0x00001588
	.global Resource_Data17E
Resource_Data17E:
	.incbin "baserom.gba", 0x008e3ddc, 0x00000064
	.section .unidentified.088e41ab,"a"
	.incbin "baserom.gba", 0x008e41ab, 0x00000001
	.section .unidentified.088e4749,"a"
	.incbin "baserom.gba", 0x008e4749, 0x00000003
	.global Resource_Data181
Resource_Data181:
	.incbin "baserom.gba", 0x008e474c, 0x00000204
	.section .unidentified.088e4c89,"a"
	.incbin "baserom.gba", 0x008e4c89, 0x00000003
	.global Resource_Data183
Resource_Data183:
	.incbin "baserom.gba", 0x008e4c8c, 0x00001018
	.global Resource_Data184
Resource_Data184:
	.incbin "baserom.gba", 0x008e5ca4, 0x0000166c
	.section .unidentified.088e92ae,"a"
	.incbin "baserom.gba", 0x008e92ae, 0x00000002
	.section .unidentified.088e9a51,"a"
	.incbin "baserom.gba", 0x008e9a51, 0x00000003
	.section .unidentified.088eaa68,"a"
	.global Resource_Data189
Resource_Data189:
	.incbin "baserom.gba", 0x008eaa68, 0x0000037c
	.global Resource_Data18A
Resource_Data18A:
	.incbin "baserom.gba", 0x008eade4, 0x00000430
	.global Resource_Data18B
Resource_Data18B:
	.incbin "baserom.gba", 0x008eb214, 0x000010cc
	.section .unidentified.088ec364,"a"
	.global Resource_Data18D
Resource_Data18D:
	.incbin "baserom.gba", 0x008ec364, 0x000004e8
	.section .unidentified.088ec954,"a"
	.global Resource_Data190
Resource_Data190:
	.incbin "baserom.gba", 0x008ec954, 0x000006b8
	.section .unidentified.088ed91e,"a"
	.incbin "baserom.gba", 0x008ed91e, 0x00000002
	.global Resource_Data192
Resource_Data192:
	.incbin "baserom.gba", 0x008ed920, 0x00001b34
	.global Resource_Data193
Resource_Data193:
	.incbin "baserom.gba", 0x008ef454, 0x00001050
	.section .unidentified.088f1549,"a"
	.incbin "baserom.gba", 0x008f1549, 0x00000003
	.global Resource_Data195
Resource_Data195:
	.incbin "baserom.gba", 0x008f154c, 0x000001a8
	.global Resource_Data196
Resource_Data196:
	.incbin "baserom.gba", 0x008f16f4, 0x00000054
	.global Resource_Data197
Resource_Data197:
	.incbin "baserom.gba", 0x008f1748, 0x00041868
	.global Resource_Data198
Resource_Data198:
	.incbin "baserom.gba", 0x00932fb0, 0x000093c4
	.global Resource_Data199
Resource_Data199:
	.incbin "baserom.gba", 0x0093c374, 0x00000028
	.section .unidentified.0893c589,"a"
	.incbin "baserom.gba", 0x0093c589, 0x00000003
	.global Resource_Data19B
Resource_Data19B:
	.incbin "baserom.gba", 0x0093c58c, 0x00000154
	.global Resource_Data19C
Resource_Data19C:
	.incbin "baserom.gba", 0x0093c6e0, 0x000004b8
	.section .unidentified.0893e302,"a"
	.incbin "baserom.gba", 0x0093e302, 0x00000002
	.section .unidentified.0893f839,"a"
	.incbin "baserom.gba", 0x0093f839, 0x00000003
	.section .unidentified.0894168f,"a"
	.incbin "baserom.gba", 0x0094168f, 0x00000001
	.section .unidentified.089437a3,"a"
	.incbin "baserom.gba", 0x009437a3, 0x00000001
	.section .unidentified.0894397c,"a"
	.global Resource_Data1A4
Resource_Data1A4:
	.incbin "baserom.gba", 0x0094397c, 0x000001e4
	.global Resource_Data1A5
Resource_Data1A5:
	.incbin "baserom.gba", 0x00943b60, 0x000003c0
	.section .unidentified.0894659b,"a"
	.incbin "baserom.gba", 0x0094659b, 0x00000001
	.section .unidentified.08946fa3,"a"
	.incbin "baserom.gba", 0x00946fa3, 0x00000001
	.section .unidentified.08949cc2,"a"
	.incbin "baserom.gba", 0x00949cc2, 0x00000002
	.section .unidentified.08949e94,"a"
	.global Resource_Data1AD
Resource_Data1AD:
	.incbin "baserom.gba", 0x00949e94, 0x00000008
	.global Resource_Data1AE
Resource_Data1AE:
	.incbin "baserom.gba", 0x00949e9c, 0x00000460
	.section .unidentified.0894b8ee,"a"
	.incbin "baserom.gba", 0x0094b8ee, 0x00000002
	.section .unidentified.0894cd5e,"a"
	.incbin "baserom.gba", 0x0094cd5e, 0x00000002
	.section .unidentified.0894d652,"a"
	.incbin "baserom.gba", 0x0094d652, 0x00000002
	.section .unidentified.0894df79,"a"
	.incbin "baserom.gba", 0x0094df79, 0x00000003
	.section .unidentified.0894ee6a,"a"
	.incbin "baserom.gba", 0x0094ee6a, 0x00000002
	.section .unidentified.0894fa57,"a"
	.incbin "baserom.gba", 0x0094fa57, 0x00000001
	.section .unidentified.0894fc32,"a"
	.incbin "baserom.gba", 0x0094fc32, 0x00000002
	.global Resource_Data1B6
Resource_Data1B6:
	.incbin "baserom.gba", 0x0094fc34, 0x000001f0
	.global Resource_Data1B7
Resource_Data1B7:
	.incbin "baserom.gba", 0x0094fe24, 0x000002d8
	.section .unidentified.08950992,"a"
	.incbin "baserom.gba", 0x00950992, 0x00000002
	.section .unidentified.08952dcd,"a"
	.incbin "baserom.gba", 0x00952dcd, 0x00000003
	.section .unidentified.0895339f,"a"
	.incbin "baserom.gba", 0x0095339f, 0x00000001
	.section .unidentified.0895383f,"a"
	.incbin "baserom.gba", 0x0095383f, 0x00000001
	.section .unidentified.08953b92,"a"
	.incbin "baserom.gba", 0x00953b92, 0x00000002
	.global Resource_Data1BE
Resource_Data1BE:
	.incbin "baserom.gba", 0x00953b94, 0x000002f8
	.global Resource_Data1BF
Resource_Data1BF:
	.incbin "baserom.gba", 0x00953e8c, 0x000000dc
	.global Resource_Data1C0
Resource_Data1C0:
	.incbin "baserom.gba", 0x00953f68, 0x000002f0
	.global Resource_Data1C1
Resource_Data1C1:
	.incbin "baserom.gba", 0x00954258, 0x00000094
	.global Resource_Data1C2
Resource_Data1C2:
	.incbin "baserom.gba", 0x009542ec, 0x00000bb4
	.global Resource_Data1C3
Resource_Data1C3:
	.incbin "baserom.gba", 0x00954ea0, 0x00000cac
	.global Resource_Data1C4
Resource_Data1C4:
	.incbin "baserom.gba", 0x00955b4c, 0x00000b04
	.global Resource_Data1C5
Resource_Data1C5:
	.incbin "baserom.gba", 0x00956650, 0x00000f40
	.global Resource_Data1C6
Resource_Data1C6:
	.incbin "baserom.gba", 0x00957590, 0x0000083c
	.global Resource_Data1C7
Resource_Data1C7:
	.incbin "baserom.gba", 0x00957dcc, 0x00000c20
	.global Resource_Data1C8
Resource_Data1C8:
	.incbin "baserom.gba", 0x009589ec, 0x00000884
	.global Resource_Data1C9
Resource_Data1C9:
	.incbin "baserom.gba", 0x00959270, 0x00000ab8
	.global Resource_Data1CA
Resource_Data1CA:
	.incbin "baserom.gba", 0x00959d28, 0x00000168
	.global Resource_Data1CB
Resource_Data1CB:
	.incbin "baserom.gba", 0x00959e90, 0x000001c4
	.global Resource_Data1CC
Resource_Data1CC:
	.incbin "baserom.gba", 0x0095a054, 0x00000f04
	.global Resource_Data1CD
Resource_Data1CD:
	.incbin "baserom.gba", 0x0095af58, 0x00000100
	.global Resource_Data1CE
Resource_Data1CE:
	.incbin "baserom.gba", 0x0095b058, 0x00000100
	.global Resource_Data1CF
Resource_Data1CF:
	.incbin "baserom.gba", 0x0095b158, 0x00000100
	.global Resource_Data1D0
Resource_Data1D0:
	.incbin "baserom.gba", 0x0095b258, 0x00000100
	.global Resource_Data1D1
Resource_Data1D1:
	.incbin "baserom.gba", 0x0095b358, 0x00000100
	.global Resource_Data1D2
Resource_Data1D2:
	.incbin "baserom.gba", 0x0095b458, 0x00000100
	.global Resource_Data1D3
Resource_Data1D3:
	.incbin "baserom.gba", 0x0095b558, 0x00000100
	.global Resource_Data1D4
Resource_Data1D4:
	.incbin "baserom.gba", 0x0095b658, 0x00000100
	.global Resource_Data1D5
Resource_Data1D5:
	.incbin "baserom.gba", 0x0095b758, 0x00000400
	.global Resource_Data1D6
Resource_Data1D6:
	.incbin "baserom.gba", 0x0095bb58, 0x00008734
	.global Resource_Data1D7
Resource_Data1D7:
	.incbin "baserom.gba", 0x0096428c, 0x00003c88
	.global Resource_Data1D8
Resource_Data1D8:
	.incbin "baserom.gba", 0x00967f14, 0x00000100
	.global Resource_Data1D9
Resource_Data1D9:
	.incbin "baserom.gba", 0x00968014, 0x000004c8
	.global Resource_Data1DA
Resource_Data1DA:
	.incbin "baserom.gba", 0x009684dc, 0x00000268
	.global Resource_Data1DB
Resource_Data1DB:
	.incbin "baserom.gba", 0x00968744, 0x000001c8
	.global Resource_Data1DC
Resource_Data1DC:
	.incbin "baserom.gba", 0x0096890c, 0x00000238
	.global Resource_Data1DD
Resource_Data1DD:
	.incbin "baserom.gba", 0x00968b44, 0x00000230
	.global Resource_Data1DE
Resource_Data1DE:
	.incbin "baserom.gba", 0x00968d74, 0x0000020c
	.global Resource_Data1DF
Resource_Data1DF:
	.incbin "baserom.gba", 0x00968f80, 0x00000204
	.global Resource_Data1E0
Resource_Data1E0:
	.incbin "baserom.gba", 0x00969184, 0x00000090
	.global Resource_Data1E1
Resource_Data1E1:
	.incbin "baserom.gba", 0x00969214, 0x00000270
	.global Resource_Data1E2
Resource_Data1E2:
	.incbin "baserom.gba", 0x00969484, 0x00000270
	.global Resource_Data1E3
Resource_Data1E3:
	.incbin "baserom.gba", 0x009696f4, 0x00000070
	.global Resource_Data1E4
Resource_Data1E4:
	.incbin "baserom.gba", 0x00969764, 0x0000005c
	.global Resource_Data1E5
Resource_Data1E5:
	.incbin "baserom.gba", 0x009697c0, 0x00000034
	.global Resource_Data1E6
Resource_Data1E6:
	.incbin "baserom.gba", 0x009697f4, 0x00000034
	.global Resource_Data1E7
Resource_Data1E7:
	.incbin "baserom.gba", 0x00969828, 0x00000198
	.global Resource_Data1E8
Resource_Data1E8:
	.incbin "baserom.gba", 0x009699c0, 0x0000024c
	.global Resource_Data1E9
Resource_Data1E9:
	.incbin "baserom.gba", 0x00969c0c, 0x000000e4
	.global Resource_Data1EA
Resource_Data1EA:
	.incbin "baserom.gba", 0x00969cf0, 0x0000005c
	.global Resource_Data1EB
Resource_Data1EB:
	.incbin "baserom.gba", 0x00969d4c, 0x00000060
	.global Resource_Data1EC
Resource_Data1EC:
	.incbin "baserom.gba", 0x00969dac, 0x000001d4
	.global Resource_Data1ED
Resource_Data1ED:
	.incbin "baserom.gba", 0x00969f80, 0x00000098
	.global Resource_Data1EE
Resource_Data1EE:
	.incbin "baserom.gba", 0x0096a018, 0x00000024
	.global Resource_Data1EF
Resource_Data1EF:
	.incbin "baserom.gba", 0x0096a03c, 0x00000154
	.global Resource_Data1F0
Resource_Data1F0:
	.incbin "baserom.gba", 0x0096a190, 0x000000e4
	.global Resource_Data1F1
Resource_Data1F1:
	.incbin "baserom.gba", 0x0096a274, 0x000000d0
	.global Resource_Data1F2
Resource_Data1F2:
	.incbin "baserom.gba", 0x0096a344, 0x00000124
	.global Resource_Data1F3
Resource_Data1F3:
	.incbin "baserom.gba", 0x0096a468, 0x00000030
	.global Resource_Data1F4
Resource_Data1F4:
	.incbin "baserom.gba", 0x0096a498, 0x00000264
	.global Resource_Data1F5
Resource_Data1F5:
	.incbin "baserom.gba", 0x0096a6fc, 0x0000009c
	.global Resource_Data1F6
Resource_Data1F6:
	.incbin "baserom.gba", 0x0096a798, 0x00000064
	.global Resource_Data1F7
Resource_Data1F7:
	.incbin "baserom.gba", 0x0096a7fc, 0x00000400
	.global Resource_Data1F8
Resource_Data1F8:
	.incbin "baserom.gba", 0x0096abfc, 0x000000b0
	.global Resource_Data1F9
Resource_Data1F9:
	.incbin "baserom.gba", 0x0096acac, 0x00000560
	.global Resource_Data1FA
Resource_Data1FA:
	.incbin "baserom.gba", 0x0096b20c, 0x00000074
	.global Resource_Data1FB
Resource_Data1FB:
	.incbin "baserom.gba", 0x0096b280, 0x00000048
	.global Resource_Data1FC
Resource_Data1FC:
	.incbin "baserom.gba", 0x0096b2c8, 0x00000050
	.global Resource_Data1FD
Resource_Data1FD:
	.incbin "baserom.gba", 0x0096b318, 0x00000050
	.global Resource_Data1FE
Resource_Data1FE:
	.incbin "baserom.gba", 0x0096b368, 0x00000058
	.global Resource_Data1FF
Resource_Data1FF:
	.incbin "baserom.gba", 0x0096b3c0, 0x00000058
	.global Resource_Data200
Resource_Data200:
	.incbin "baserom.gba", 0x0096b418, 0x00000040
	.global Resource_Data201
Resource_Data201:
	.incbin "baserom.gba", 0x0096b458, 0x00000044
	.global Resource_Data202
Resource_Data202:
	.incbin "baserom.gba", 0x0096b49c, 0x00000054
	.section .unidentified.08970727,"a"
	.incbin "baserom.gba", 0x00970727, 0x00000001
	.section .unidentified.08973672,"a"
	.incbin "baserom.gba", 0x00973672, 0x00000002
	.section .unidentified.08977295,"a"
	.incbin "baserom.gba", 0x00977295, 0x00000003
	.section .unidentified.089794e1,"a"
	.incbin "baserom.gba", 0x009794e1, 0x00000003
	.section .unidentified.0897ea7d,"a"
	.incbin "baserom.gba", 0x0097ea7d, 0x00000003
	.section .unidentified.089822f7,"a"
	.incbin "baserom.gba", 0x009822f7, 0x00000001
	.section .unidentified.08988161,"a"
	.incbin "baserom.gba", 0x00988161, 0x00000003
	.section .unidentified.08989235,"a"
	.incbin "baserom.gba", 0x00989235, 0x00000003
	.section .unidentified.0898a2a1,"a"
	.incbin "baserom.gba", 0x0098a2a1, 0x00000003
	.section .unidentified.0898f8fb,"a"
	.incbin "baserom.gba", 0x0098f8fb, 0x00000001
	.section .unidentified.08992a3d,"a"
	.incbin "baserom.gba", 0x00992a3d, 0x00000003
	.section .unidentified.08994add,"a"
	.incbin "baserom.gba", 0x00994add, 0x00000003
	.section .unidentified.08996dfd,"a"
	.incbin "baserom.gba", 0x00996dfd, 0x00000003
	.section .unidentified.08999a76,"a"
	.incbin "baserom.gba", 0x00999a76, 0x00000002
	.section .unidentified.0899b971,"a"
	.incbin "baserom.gba", 0x0099b971, 0x00000003
	.section .unidentified.089a3eb5,"a"
	.incbin "baserom.gba", 0x009a3eb5, 0x00000003
	.section .unidentified.089acb37,"a"
	.incbin "baserom.gba", 0x009acb37, 0x00000001
	.section .unidentified.089af6b1,"a"
	.incbin "baserom.gba", 0x009af6b1, 0x00000003
	.section .unidentified.089b60ee,"a"
	.incbin "baserom.gba", 0x009b60ee, 0x00000002
	.section .unidentified.089b8be2,"a"
	.incbin "baserom.gba", 0x009b8be2, 0x00000002
	.section .unidentified.089ba6d2,"a"
	.incbin "baserom.gba", 0x009ba6d2, 0x00000002
	.section .unidentified.089bcfe8,"a"
	.global Resource_Data21F
Resource_Data21F:
	.incbin "baserom.gba", 0x009bcfe8, 0x00003440
	.section .unidentified.089c4dce,"a"
	.incbin "baserom.gba", 0x009c4dce, 0x00000002
	.section .unidentified.089c5d6a,"a"
	.incbin "baserom.gba", 0x009c5d6a, 0x00000002
	.section .unidentified.089c8495,"a"
	.incbin "baserom.gba", 0x009c8495, 0x00000003
	.section .unidentified.089c9baa,"a"
	.incbin "baserom.gba", 0x009c9baa, 0x00000002
	.section .unidentified.089d23c3,"a"
	.incbin "baserom.gba", 0x009d23c3, 0x00000001
	.section .unidentified.089d668b,"a"
	.incbin "baserom.gba", 0x009d668b, 0x00000001
	.section .unidentified.089ddec5,"a"
	.incbin "baserom.gba", 0x009ddec5, 0x00000003
	.section .unidentified.089df6aa,"a"
	.incbin "baserom.gba", 0x009df6aa, 0x00000002
	.section .unidentified.089e2441,"a"
	.incbin "baserom.gba", 0x009e2441, 0x00000003
	.section .unidentified.089e348b,"a"
	.incbin "baserom.gba", 0x009e348b, 0x00000001
	.section .unidentified.089eba1b,"a"
	.incbin "baserom.gba", 0x009eba1b, 0x00000001
	.section .unidentified.089ed815,"a"
	.incbin "baserom.gba", 0x009ed815, 0x00000003
	.section .unidentified.089eebdb,"a"
	.incbin "baserom.gba", 0x009eebdb, 0x00000001
	.section .unidentified.089f5add,"a"
	.incbin "baserom.gba", 0x009f5add, 0x00000003
	.section .unidentified.089f7dde,"a"
	.incbin "baserom.gba", 0x009f7dde, 0x00000002
	.section .unidentified.089fd02b,"a"
	.incbin "baserom.gba", 0x009fd02b, 0x00000001
	.section .unidentified.08a0242a,"a"
	.incbin "baserom.gba", 0x00a0242a, 0x00000002
	.section .unidentified.08a045de,"a"
	.incbin "baserom.gba", 0x00a045de, 0x00000002
	.section .unidentified.08a08745,"a"
	.incbin "baserom.gba", 0x00a08745, 0x00000003
	.section .unidentified.08a0b96a,"a"
	.incbin "baserom.gba", 0x00a0b96a, 0x00000002
	.section .unidentified.08a0d06e,"a"
	.incbin "baserom.gba", 0x00a0d06e, 0x00000002
	.section .unidentified.08a13c56,"a"
	.incbin "baserom.gba", 0x00a13c56, 0x00000002
	.section .unidentified.08a20236,"a"
	.incbin "baserom.gba", 0x00a20236, 0x00000002
	.section .unidentified.08a232ca,"a"
	.incbin "baserom.gba", 0x00a232ca, 0x00000002
	.section .unidentified.08a25b35,"a"
	.incbin "baserom.gba", 0x00a25b35, 0x00000003
	.section .unidentified.08a27626,"a"
	.incbin "baserom.gba", 0x00a27626, 0x00000002
	.section .unidentified.08a2843e,"a"
	.incbin "baserom.gba", 0x00a2843e, 0x00000002
	.section .unidentified.08a29129,"a"
	.incbin "baserom.gba", 0x00a29129, 0x00000003
	.section .unidentified.08a325e6,"a"
	.incbin "baserom.gba", 0x00a325e6, 0x00000002
	.section .unidentified.08a332f1,"a"
	.incbin "baserom.gba", 0x00a332f1, 0x00000003
	.section .unidentified.08a34555,"a"
	.incbin "baserom.gba", 0x00a34555, 0x00000003
	.section .unidentified.08a35487,"a"
	.incbin "baserom.gba", 0x00a35487, 0x00000001
	.section .unidentified.08a360f7,"a"
	.incbin "baserom.gba", 0x00a360f7, 0x00000001
	.section .unidentified.08a36d0f,"a"
	.incbin "baserom.gba", 0x00a36d0f, 0x00000001
	.section .unidentified.08a37483,"a"
	.incbin "baserom.gba", 0x00a37483, 0x00000001
	.section .unidentified.08a38d13,"a"
	.incbin "baserom.gba", 0x00a38d13, 0x00000001
	.section .unidentified.08a396e1,"a"
	.incbin "baserom.gba", 0x00a396e1, 0x00000003
	.section .unidentified.08a3a2ff,"a"
	.incbin "baserom.gba", 0x00a3a2ff, 0x00000001
	.section .unidentified.08a3c9b3,"a"
	.incbin "baserom.gba", 0x00a3c9b3, 0x00000001
	.section .unidentified.08a3f0c6,"a"
	.incbin "baserom.gba", 0x00a3f0c6, 0x00000002
	.section .unidentified.08a4401b,"a"
	.incbin "baserom.gba", 0x00a4401b, 0x00000001
	.section .unidentified.08a48345,"a"
	.incbin "baserom.gba", 0x00a48345, 0x00000003
	.section .unidentified.08a48f32,"a"
	.incbin "baserom.gba", 0x00a48f32, 0x00000002
	.section .unidentified.08a4dae7,"a"
	.incbin "baserom.gba", 0x00a4dae7, 0x00000001
	.section .unidentified.08a4fe51,"a"
	.incbin "baserom.gba", 0x00a4fe51, 0x00000003
	.section .unidentified.08a517f3,"a"
	.incbin "baserom.gba", 0x00a517f3, 0x00000001
	.section .unidentified.08a560b5,"a"
	.incbin "baserom.gba", 0x00a560b5, 0x00000003
	.section .unidentified.08a594c5,"a"
	.incbin "baserom.gba", 0x00a594c5, 0x00000003
	.section .unidentified.08a5d58d,"a"
	.incbin "baserom.gba", 0x00a5d58d, 0x00000003
	.section .unidentified.08a60c56,"a"
	.incbin "baserom.gba", 0x00a60c56, 0x00000002
	.section .unidentified.08a68c27,"a"
	.incbin "baserom.gba", 0x00a68c27, 0x00000001
	.section .unidentified.08a6ff37,"a"
	.incbin "baserom.gba", 0x00a6ff37, 0x00000001
	.section .unidentified.08a74eb7,"a"
	.incbin "baserom.gba", 0x00a74eb7, 0x00000001
	.section .unidentified.08a79939,"a"
	.incbin "baserom.gba", 0x00a79939, 0x00000003
	.global Resource_Data26D
Resource_Data26D:
	.incbin "baserom.gba", 0x00a7993c, 0x0000000c
	.global Resource_Data26E
Resource_Data26E:
	.incbin "baserom.gba", 0x00a79948, 0x00000150
	.global Resource_Data26F
Resource_Data26F:
	.incbin "baserom.gba", 0x00a79a98, 0x00000140
	.global Resource_Data270
Resource_Data270:
	.incbin "baserom.gba", 0x00a79bd8, 0x00000140
	.global Resource_Data271
Resource_Data271:
	.incbin "baserom.gba", 0x00a79d18, 0x00000140
	.section .unidentified.08a7b0b5,"a"
	.incbin "baserom.gba", 0x00a7b0b5, 0x00000003
	.section .unidentified.08a7b286,"a"
	.incbin "baserom.gba", 0x00a7b286, 0x00000002
	.section .unidentified.08a7d327,"a"
	.incbin "baserom.gba", 0x00a7d327, 0x00000001
	.section .unidentified.08a7e2b4,"a"
	.global Resource_Data276
Resource_Data276:
	.incbin "baserom.gba", 0x00a7e2b4, 0x000022d8
	.section .unidentified.08a817e0,"a"
	.global Resource_Data278
Resource_Data278:
	.incbin "baserom.gba", 0x00a817e0, 0x0000461c
	.section .unidentified.08a85f02,"a"
	.incbin "baserom.gba", 0x00a85f02, 0x00000002
	.section .unidentified.08a870ed,"a"
	.incbin "baserom.gba", 0x00a870ed, 0x00000003
	.section .unidentified.08a88d75,"a"
	.incbin "baserom.gba", 0x00a88d75, 0x00000003
	.section .unidentified.08a89283,"a"
	.incbin "baserom.gba", 0x00a89283, 0x00000001
	.section .unidentified.08a893c3,"a"
	.incbin "baserom.gba", 0x00a893c3, 0x00000001
	.section .unidentified.08a8b046,"a"
	.incbin "baserom.gba", 0x00a8b046, 0x00000002
	.section .unidentified.08a8da1e,"a"
	.incbin "baserom.gba", 0x00a8da1e, 0x00000002
	.section .unidentified.08a901e7,"a"
	.incbin "baserom.gba", 0x00a901e7, 0x00000001
	.section .unidentified.08a91707,"a"
	.incbin "baserom.gba", 0x00a91707, 0x00000001
	.section .unidentified.08a9304b,"a"
	.incbin "baserom.gba", 0x00a9304b, 0x00000001
	.section .unidentified.08a94af9,"a"
	.incbin "baserom.gba", 0x00a94af9, 0x00000003
	.section .unidentified.08a94c6d,"a"
	.incbin "baserom.gba", 0x00a94c6d, 0x00000003
	.section .unidentified.08a97a51,"a"
	.incbin "baserom.gba", 0x00a97a51, 0x00000003
	.section .unidentified.08a9a2fb,"a"
	.incbin "baserom.gba", 0x00a9a2fb, 0x00000001
	.section .unidentified.08a9e792,"a"
	.incbin "baserom.gba", 0x00a9e792, 0x00000002
	.section .unidentified.08a9ffc9,"a"
	.incbin "baserom.gba", 0x00a9ffc9, 0x00000003
	.section .unidentified.08aa1b0a,"a"
	.incbin "baserom.gba", 0x00aa1b0a, 0x00000002
	.section .unidentified.08aa4607,"a"
	.incbin "baserom.gba", 0x00aa4607, 0x00000001
	.section .unidentified.08aa66e5,"a"
	.incbin "baserom.gba", 0x00aa66e5, 0x00000003
	.section .unidentified.08aa8931,"a"
	.incbin "baserom.gba", 0x00aa8931, 0x00000003
	.section .unidentified.08aa8a73,"a"
	.incbin "baserom.gba", 0x00aa8a73, 0x00000001
	.section .unidentified.08aaa146,"a"
	.incbin "baserom.gba", 0x00aaa146, 0x00000002
	.section .unidentified.08aaa246,"a"
	.incbin "baserom.gba", 0x00aaa246, 0x00000002
	.section .unidentified.08aac139,"a"
	.incbin "baserom.gba", 0x00aac139, 0x00000003
	.section .unidentified.08aadb11,"a"
	.incbin "baserom.gba", 0x00aadb11, 0x00000003
	.section .unidentified.08ab0542,"a"
	.incbin "baserom.gba", 0x00ab0542, 0x00000002
	.section .unidentified.08ab5793,"a"
	.incbin "baserom.gba", 0x00ab5793, 0x00000001
	.section .unidentified.08ab80d7,"a"
	.incbin "baserom.gba", 0x00ab80d7, 0x00000001
	.section .unidentified.08ab95aa,"a"
	.incbin "baserom.gba", 0x00ab95aa, 0x00000002
	.section .unidentified.08abe375,"a"
	.incbin "baserom.gba", 0x00abe375, 0x00000003
	.section .unidentified.08ac101f,"a"
	.incbin "baserom.gba", 0x00ac101f, 0x00000001
	.section .unidentified.08ac4257,"a"
	.incbin "baserom.gba", 0x00ac4257, 0x00000001
	.section .unidentified.08ac43ef,"a"
	.incbin "baserom.gba", 0x00ac43ef, 0x00000001
	.section .unidentified.08ac71ef,"a"
	.incbin "baserom.gba", 0x00ac71ef, 0x00000001
	.section .unidentified.08ac89a3,"a"
	.incbin "baserom.gba", 0x00ac89a3, 0x00000001
	.section .unidentified.08ac993e,"a"
	.incbin "baserom.gba", 0x00ac993e, 0x00000002
	.section .unidentified.08acb10a,"a"
	.incbin "baserom.gba", 0x00acb10a, 0x00000002
	.section .unidentified.08acbf9f,"a"
	.incbin "baserom.gba", 0x00acbf9f, 0x00000001
	.section .unidentified.08acc146,"a"
	.incbin "baserom.gba", 0x00acc146, 0x00000002
	.section .unidentified.08acf0ff,"a"
	.incbin "baserom.gba", 0x00acf0ff, 0x00000001
	.section .unidentified.08acfda5,"a"
	.incbin "baserom.gba", 0x00acfda5, 0x00000003
	.section .unidentified.08ad1375,"a"
	.incbin "baserom.gba", 0x00ad1375, 0x00000003
	.section .unidentified.08ad27cb,"a"
	.incbin "baserom.gba", 0x00ad27cb, 0x00000001
	.section .unidentified.08ad329b,"a"
	.incbin "baserom.gba", 0x00ad329b, 0x00000001
	.section .unidentified.08ad3ddb,"a"
	.incbin "baserom.gba", 0x00ad3ddb, 0x00000001
	.section .unidentified.08ad5545,"a"
	.incbin "baserom.gba", 0x00ad5545, 0x00000003
	.section .unidentified.08ad682d,"a"
	.incbin "baserom.gba", 0x00ad682d, 0x00000003
	.section .unidentified.08ad76bb,"a"
	.incbin "baserom.gba", 0x00ad76bb, 0x00000001
	.section .unidentified.08ad9341,"a"
	.incbin "baserom.gba", 0x00ad9341, 0x00000003
	.section .unidentified.08adae27,"a"
	.incbin "baserom.gba", 0x00adae27, 0x00000001
	.section .unidentified.08adc311,"a"
	.incbin "baserom.gba", 0x00adc311, 0x00000003
	.section .unidentified.08ae253b,"a"
	.incbin "baserom.gba", 0x00ae253b, 0x00000001
	.section .unidentified.08ae2a0d,"a"
	.incbin "baserom.gba", 0x00ae2a0d, 0x00000003
	.section .unidentified.08ae3b55,"a"
	.incbin "baserom.gba", 0x00ae3b55, 0x00000003
	.section .unidentified.08ae3d06,"a"
	.incbin "baserom.gba", 0x00ae3d06, 0x00000002
	.section .unidentified.08ae7af7,"a"
	.incbin "baserom.gba", 0x00ae7af7, 0x00000001
	.section .unidentified.08ae8633,"a"
	.incbin "baserom.gba", 0x00ae8633, 0x00000001
	.section .unidentified.08ae9771,"a"
	.incbin "baserom.gba", 0x00ae9771, 0x00000003
	.section .unidentified.08aeb53e,"a"
	.incbin "baserom.gba", 0x00aeb53e, 0x00000002
	.section .unidentified.08aed52e,"a"
	.incbin "baserom.gba", 0x00aed52e, 0x00000002
	.section .unidentified.08aedff1,"a"
	.incbin "baserom.gba", 0x00aedff1, 0x00000003
	.section .unidentified.08aef9f3,"a"
	.incbin "baserom.gba", 0x00aef9f3, 0x00000001
	.section .unidentified.08aefb13,"a"
	.incbin "baserom.gba", 0x00aefb13, 0x00000001
	.section .unidentified.08af1eb3,"a"
	.incbin "baserom.gba", 0x00af1eb3, 0x00000001
	.section .unidentified.08af30ff,"a"
	.incbin "baserom.gba", 0x00af30ff, 0x00000001
	.section .unidentified.08af548f,"a"
	.incbin "baserom.gba", 0x00af548f, 0x00000001
	.section .unidentified.08af6d8d,"a"
	.incbin "baserom.gba", 0x00af6d8d, 0x00000003
	.section .unidentified.08af7d7e,"a"
	.incbin "baserom.gba", 0x00af7d7e, 0x00000002
	.section .unidentified.08afa31a,"a"
	.incbin "baserom.gba", 0x00afa31a, 0x00000002
	.section .unidentified.08afe942,"a"
	.incbin "baserom.gba", 0x00afe942, 0x00000002
	.section .unidentified.08aff007,"a"
	.incbin "baserom.gba", 0x00aff007, 0x00000001
	.section .unidentified.08b01a9d,"a"
	.incbin "baserom.gba", 0x00b01a9d, 0x00000003
	.section .unidentified.08b01bee,"a"
	.incbin "baserom.gba", 0x00b01bee, 0x00000002
	.section .unidentified.08b03ffe,"a"
	.incbin "baserom.gba", 0x00b03ffe, 0x00000002
	.section .unidentified.08b0617f,"a"
	.incbin "baserom.gba", 0x00b0617f, 0x00000001
	.section .unidentified.08b062bf,"a"
	.incbin "baserom.gba", 0x00b062bf, 0x00000001
	.section .unidentified.08b08d5e,"a"
	.incbin "baserom.gba", 0x00b08d5e, 0x00000002
	.section .unidentified.08b08eaf,"a"
	.incbin "baserom.gba", 0x00b08eaf, 0x00000001
	.section .unidentified.08b0b2be,"a"
	.incbin "baserom.gba", 0x00b0b2be, 0x00000002
	.section .unidentified.08b0d43f,"a"
	.incbin "baserom.gba", 0x00b0d43f, 0x00000001
	.section .unidentified.08b0d57f,"a"
	.incbin "baserom.gba", 0x00b0d57f, 0x00000001
	.section .unidentified.08b10bae,"a"
	.incbin "baserom.gba", 0x00b10bae, 0x00000002
	.section .unidentified.08b13112,"a"
	.incbin "baserom.gba", 0x00b13112, 0x00000002
	.section .unidentified.08b15293,"a"
	.incbin "baserom.gba", 0x00b15293, 0x00000001
	.section .unidentified.08b153d3,"a"
	.incbin "baserom.gba", 0x00b153d3, 0x00000001
	.section .unidentified.08b16883,"a"
	.incbin "baserom.gba", 0x00b16883, 0x00000001
	.section .unidentified.08b1698e,"a"
	.incbin "baserom.gba", 0x00b1698e, 0x00000002
	.section .unidentified.08b184e6,"a"
	.incbin "baserom.gba", 0x00b184e6, 0x00000002
	.section .unidentified.08b19b89,"a"
	.incbin "baserom.gba", 0x00b19b89, 0x00000003
	.section .unidentified.08b19d4a,"a"
	.incbin "baserom.gba", 0x00b19d4a, 0x00000002
	.global Resource_Data2EF
Resource_Data2EF:
	.incbin "baserom.gba", 0x00b19d4c, 0x00000d9c
	.section .unidentified.08b1c73e,"a"
	.incbin "baserom.gba", 0x00b1c73e, 0x00000002
	.section .unidentified.08b1df95,"a"
	.incbin "baserom.gba", 0x00b1df95, 0x00000003
	.section .unidentified.08b1e156,"a"
	.incbin "baserom.gba", 0x00b1e156, 0x00000002
	.section .unidentified.08b223d1,"a"
	.incbin "baserom.gba", 0x00b223d1, 0x00000003
	.section .unidentified.08b22592,"a"
	.incbin "baserom.gba", 0x00b22592, 0x00000002
	.section .unidentified.08b23009,"a"
	.incbin "baserom.gba", 0x00b23009, 0x00000003
	.section .unidentified.08b23111,"a"
	.incbin "baserom.gba", 0x00b23111, 0x00000003
	.section .unidentified.08b24dd2,"a"
	.incbin "baserom.gba", 0x00b24dd2, 0x00000002
	.section .unidentified.08b2655f,"a"
	.incbin "baserom.gba", 0x00b2655f, 0x00000001
	.section .unidentified.08b26815,"a"
	.incbin "baserom.gba", 0x00b26815, 0x00000003
	.section .unidentified.08b28325,"a"
	.incbin "baserom.gba", 0x00b28325, 0x00000003
	.section .unidentified.08b2abb2,"a"
	.incbin "baserom.gba", 0x00b2abb2, 0x00000002
	.section .unidentified.08b2d3a2,"a"
	.incbin "baserom.gba", 0x00b2d3a2, 0x00000002
	.section .unidentified.08b2e342,"a"
	.incbin "baserom.gba", 0x00b2e342, 0x00000002
	.section .unidentified.08b30495,"a"
	.incbin "baserom.gba", 0x00b30495, 0x00000003
	.section .unidentified.08b33b22,"a"
	.incbin "baserom.gba", 0x00b33b22, 0x00000002
	.section .unidentified.08b358b7,"a"
	.incbin "baserom.gba", 0x00b358b7, 0x00000001
	.section .unidentified.08b376d5,"a"
	.incbin "baserom.gba", 0x00b376d5, 0x00000003
	.section .unidentified.08b391b1,"a"
	.incbin "baserom.gba", 0x00b391b1, 0x00000003
	.section .unidentified.08b3a79a,"a"
	.incbin "baserom.gba", 0x00b3a79a, 0x00000002
	.section .unidentified.08b3cd82,"a"
	.incbin "baserom.gba", 0x00b3cd82, 0x00000002
	.section .unidentified.08b3cefb,"a"
	.incbin "baserom.gba", 0x00b3cefb, 0x00000001
	.section .unidentified.08b3e3ba,"a"
	.incbin "baserom.gba", 0x00b3e3ba, 0x00000002
	.section .unidentified.08b3fbbe,"a"
	.incbin "baserom.gba", 0x00b3fbbe, 0x00000002
	.section .unidentified.08b41536,"a"
	.incbin "baserom.gba", 0x00b41536, 0x00000002
	.section .unidentified.08b42456,"a"
	.incbin "baserom.gba", 0x00b42456, 0x00000002
	.section .unidentified.08b43f02,"a"
	.incbin "baserom.gba", 0x00b43f02, 0x00000002
	.section .unidentified.08b4400f,"a"
	.incbin "baserom.gba", 0x00b4400f, 0x00000001
	.section .unidentified.08b4613f,"a"
	.incbin "baserom.gba", 0x00b4613f, 0x00000001
	.section .unidentified.08b47d15,"a"
	.incbin "baserom.gba", 0x00b47d15, 0x00000003
	.section .unidentified.08b48327,"a"
	.incbin "baserom.gba", 0x00b48327, 0x00000001
	.section .unidentified.08b48467,"a"
	.incbin "baserom.gba", 0x00b48467, 0x00000001
	.section .unidentified.08b4ab67,"a"
	.incbin "baserom.gba", 0x00b4ab67, 0x00000001
	.section .unidentified.08b4c3c6,"a"
	.incbin "baserom.gba", 0x00b4c3c6, 0x00000002
	.section .unidentified.08b4c9d7,"a"
	.incbin "baserom.gba", 0x00b4c9d7, 0x00000001
	.section .unidentified.08b4cb17,"a"
	.incbin "baserom.gba", 0x00b4cb17, 0x00000001
	.section .unidentified.08b4d992,"a"
	.incbin "baserom.gba", 0x00b4d992, 0x00000002
	.section .unidentified.08b4daa9,"a"
	.incbin "baserom.gba", 0x00b4daa9, 0x00000003
	.section .unidentified.08b4fd7f,"a"
	.incbin "baserom.gba", 0x00b4fd7f, 0x00000001
	.section .unidentified.08b519d2,"a"
	.incbin "baserom.gba", 0x00b519d2, 0x00000002
	.section .unidentified.08b52017,"a"
	.incbin "baserom.gba", 0x00b52017, 0x00000001
	.section .unidentified.08b52157,"a"
	.incbin "baserom.gba", 0x00b52157, 0x00000001
	.section .unidentified.08b52d5d,"a"
	.incbin "baserom.gba", 0x00b52d5d, 0x00000003
	.section .unidentified.08b5530f,"a"
	.incbin "baserom.gba", 0x00b5530f, 0x00000001
	.section .unidentified.08b56fce,"a"
	.incbin "baserom.gba", 0x00b56fce, 0x00000002
	.section .unidentified.08b58a9a,"a"
	.incbin "baserom.gba", 0x00b58a9a, 0x00000002
	.section .unidentified.08b59b6e,"a"
	.incbin "baserom.gba", 0x00b59b6e, 0x00000002
	.section .unidentified.08b59cd5,"a"
	.incbin "baserom.gba", 0x00b59cd5, 0x00000003
	.section .unidentified.08b5aeee,"a"
	.incbin "baserom.gba", 0x00b5aeee, 0x00000002
	.section .unidentified.08b5bacd,"a"
	.incbin "baserom.gba", 0x00b5bacd, 0x00000003
	.section .unidentified.08b5bc3a,"a"
	.incbin "baserom.gba", 0x00b5bc3a, 0x00000002
	.section .unidentified.08b5ea8b,"a"
	.incbin "baserom.gba", 0x00b5ea8b, 0x00000001
	.section .unidentified.08b6124a,"a"
	.incbin "baserom.gba", 0x00b6124a, 0x00000002
	.section .unidentified.08b672fb,"a"
	.incbin "baserom.gba", 0x00b672fb, 0x00000001
	.section .unidentified.08b68e2b,"a"
	.incbin "baserom.gba", 0x00b68e2b, 0x00000001
	.section .unidentified.08b6b162,"a"
	.incbin "baserom.gba", 0x00b6b162, 0x00000002
	.section .unidentified.08b6c313,"a"
	.incbin "baserom.gba", 0x00b6c313, 0x00000001
	.section .unidentified.08b6dc35,"a"
	.incbin "baserom.gba", 0x00b6dc35, 0x00000003
	.section .unidentified.08b6fac1,"a"
	.incbin "baserom.gba", 0x00b6fac1, 0x00000003
	.section .unidentified.08b71ca2,"a"
	.incbin "baserom.gba", 0x00b71ca2, 0x00000002
	.section .unidentified.08b74496,"a"
	.incbin "baserom.gba", 0x00b74496, 0x00000002
	.section .unidentified.08b75669,"a"
	.incbin "baserom.gba", 0x00b75669, 0x00000003
	.section .unidentified.08b75755,"a"
	.incbin "baserom.gba", 0x00b75755, 0x00000003
	.section .unidentified.08b7898e,"a"
	.incbin "baserom.gba", 0x00b7898e, 0x00000002
	.section .unidentified.08b79005,"a"
	.incbin "baserom.gba", 0x00b79005, 0x00000003
	.section .unidentified.08b7cdc7,"a"
	.incbin "baserom.gba", 0x00b7cdc7, 0x00000001
	.section .unidentified.08b7f059,"a"
	.incbin "baserom.gba", 0x00b7f059, 0x00000003
	.section .unidentified.08b80a76,"a"
	.incbin "baserom.gba", 0x00b80a76, 0x00000002
	.section .unidentified.08b81b83,"a"
	.incbin "baserom.gba", 0x00b81b83, 0x00000001
	.section .unidentified.08b84d22,"a"
	.incbin "baserom.gba", 0x00b84d22, 0x00000002
	.section .unidentified.08b85f1a,"a"
	.incbin "baserom.gba", 0x00b85f1a, 0x00000002
	.section .unidentified.08b86ddd,"a"
	.incbin "baserom.gba", 0x00b86ddd, 0x00000003
	.section .unidentified.08b88e7e,"a"
	.incbin "baserom.gba", 0x00b88e7e, 0x00000002
	.section .unidentified.08b8a8d3,"a"
	.incbin "baserom.gba", 0x00b8a8d3, 0x00000001
	.section .unidentified.08b8bd9e,"a"
	.incbin "baserom.gba", 0x00b8bd9e, 0x00000002
	.section .unidentified.08b8cf55,"a"
	.incbin "baserom.gba", 0x00b8cf55, 0x00000003
	.section .unidentified.08b8dfa2,"a"
	.incbin "baserom.gba", 0x00b8dfa2, 0x00000002
	.section .unidentified.08b8e097,"a"
	.incbin "baserom.gba", 0x00b8e097, 0x00000001
	.section .unidentified.08b8f66a,"a"
	.incbin "baserom.gba", 0x00b8f66a, 0x00000002
	.section .unidentified.08b8f889,"a"
	.incbin "baserom.gba", 0x00b8f889, 0x00000003
	.section .unidentified.08b9196d,"a"
	.incbin "baserom.gba", 0x00b9196d, 0x00000003
	.section .unidentified.08b91b32,"a"
	.incbin "baserom.gba", 0x00b91b32, 0x00000002
	.section .unidentified.08b93bf5,"a"
	.incbin "baserom.gba", 0x00b93bf5, 0x00000003
	.section .unidentified.08b93d25,"a"
	.incbin "baserom.gba", 0x00b93d25, 0x00000003
	.section .unidentified.08b967de,"a"
	.incbin "baserom.gba", 0x00b967de, 0x00000002
	.section .unidentified.08b98336,"a"
	.incbin "baserom.gba", 0x00b98336, 0x00000002
	.section .unidentified.08b9927d,"a"
	.incbin "baserom.gba", 0x00b9927d, 0x00000003
	.section .unidentified.08b9ac86,"a"
	.incbin "baserom.gba", 0x00b9ac86, 0x00000002
	.section .unidentified.08b9c5aa,"a"
	.incbin "baserom.gba", 0x00b9c5aa, 0x00000002
	.section .unidentified.08b9e07a,"a"
	.incbin "baserom.gba", 0x00b9e07a, 0x00000002
	.section .unidentified.08b9f4ee,"a"
	.incbin "baserom.gba", 0x00b9f4ee, 0x00000002
	.section .unidentified.08ba0bb2,"a"
	.incbin "baserom.gba", 0x00ba0bb2, 0x00000002
	.section .unidentified.08ba20d5,"a"
	.incbin "baserom.gba", 0x00ba20d5, 0x00000003
	.section .unidentified.08ba6303,"a"
	.incbin "baserom.gba", 0x00ba6303, 0x00000001
	.section .unidentified.08ba6fb1,"a"
	.incbin "baserom.gba", 0x00ba6fb1, 0x00000003
	.section .unidentified.08ba7d31,"a"
	.incbin "baserom.gba", 0x00ba7d31, 0x00000003
	.section .unidentified.08baa9db,"a"
	.incbin "baserom.gba", 0x00baa9db, 0x00000001
	.section .unidentified.08baab2b,"a"
	.incbin "baserom.gba", 0x00baab2b, 0x00000001
	.section .unidentified.08bad636,"a"
	.incbin "baserom.gba", 0x00bad636, 0x00000002
	.section .unidentified.08baf23f,"a"
	.incbin "baserom.gba", 0x00baf23f, 0x00000001
	.section .unidentified.08bb0c46,"a"
	.incbin "baserom.gba", 0x00bb0c46, 0x00000002
	.section .unidentified.08bb2961,"a"
	.incbin "baserom.gba", 0x00bb2961, 0x00000003
	.section .unidentified.08bb2ab3,"a"
	.incbin "baserom.gba", 0x00bb2ab3, 0x00000001
	.section .unidentified.08bb44ba,"a"
	.incbin "baserom.gba", 0x00bb44ba, 0x00000002
	.section .unidentified.08bb8242,"a"
	.incbin "baserom.gba", 0x00bb8242, 0x00000002
	.section .unidentified.08bb83d5,"a"
	.incbin "baserom.gba", 0x00bb83d5, 0x00000003
	.section .unidentified.08bbd3df,"a"
	.incbin "baserom.gba", 0x00bbd3df, 0x00000001
	.section .unidentified.08bbfa61,"a"
	.incbin "baserom.gba", 0x00bbfa61, 0x00000003
	.section .unidentified.08bbfd97,"a"
	.incbin "baserom.gba", 0x00bbfd97, 0x00000001
	.section .unidentified.08bc1a23,"a"
	.incbin "baserom.gba", 0x00bc1a23, 0x00000001
	.section .unidentified.08bc3ccd,"a"
	.incbin "baserom.gba", 0x00bc3ccd, 0x00000003
	.section .unidentified.08bc61ed,"a"
	.incbin "baserom.gba", 0x00bc61ed, 0x00000003
	.section .unidentified.08bc633a,"a"
	.incbin "baserom.gba", 0x00bc633a, 0x00000002
	.section .unidentified.08bc81bf,"a"
	.incbin "baserom.gba", 0x00bc81bf, 0x00000001
	.section .unidentified.08bc9766,"a"
	.incbin "baserom.gba", 0x00bc9766, 0x00000002
	.section .unidentified.08bcb711,"a"
	.incbin "baserom.gba", 0x00bcb711, 0x00000003
	.section .unidentified.08bcc682,"a"
	.incbin "baserom.gba", 0x00bcc682, 0x00000002
	.section .unidentified.08bcf253,"a"
	.incbin "baserom.gba", 0x00bcf253, 0x00000001
	.section .unidentified.08bd48f2,"a"
	.incbin "baserom.gba", 0x00bd48f2, 0x00000002
	.section .unidentified.08bd607d,"a"
	.incbin "baserom.gba", 0x00bd607d, 0x00000003
	.section .unidentified.08bd70ef,"a"
	.incbin "baserom.gba", 0x00bd70ef, 0x00000001
	.section .unidentified.08bd7c8b,"a"
	.incbin "baserom.gba", 0x00bd7c8b, 0x00000001
	.section .unidentified.08bd7d47,"a"
	.incbin "baserom.gba", 0x00bd7d47, 0x00000001
	.section .unidentified.08bd8a86,"a"
	.incbin "baserom.gba", 0x00bd8a86, 0x00000002
	.section .unidentified.08bd9463,"a"
	.incbin "baserom.gba", 0x00bd9463, 0x00000001
	.section .unidentified.08bda07b,"a"
	.incbin "baserom.gba", 0x00bda07b, 0x00000001
	.section .unidentified.08bdbf36,"a"
	.incbin "baserom.gba", 0x00bdbf36, 0x00000002
	.section .unidentified.08bdc052,"a"
	.incbin "baserom.gba", 0x00bdc052, 0x00000002
	.section .unidentified.08bde8d3,"a"
	.incbin "baserom.gba", 0x00bde8d3, 0x00000001
	.section .unidentified.08be005e,"a"
	.incbin "baserom.gba", 0x00be005e, 0x00000002
	.section .unidentified.08be3f8e,"a"
	.incbin "baserom.gba", 0x00be3f8e, 0x00000002
	.section .unidentified.08be40d7,"a"
	.incbin "baserom.gba", 0x00be40d7, 0x00000001
	.section .unidentified.08be6787,"a"
	.incbin "baserom.gba", 0x00be6787, 0x00000001
	.section .unidentified.08be8f2a,"a"
	.incbin "baserom.gba", 0x00be8f2a, 0x00000002
	.section .unidentified.08be9cb5,"a"
	.incbin "baserom.gba", 0x00be9cb5, 0x00000003
	.section .unidentified.08becaf6,"a"
	.incbin "baserom.gba", 0x00becaf6, 0x00000002
	.section .unidentified.08bef11d,"a"
	.incbin "baserom.gba", 0x00bef11d, 0x00000003
	.section .unidentified.08bf0e09,"a"
	.incbin "baserom.gba", 0x00bf0e09, 0x00000003
	.section .unidentified.08bf2509,"a"
	.incbin "baserom.gba", 0x00bf2509, 0x00000003
	.section .unidentified.08bf3a85,"a"
	.incbin "baserom.gba", 0x00bf3a85, 0x00000003
	.section .unidentified.08bf51f7,"a"
	.incbin "baserom.gba", 0x00bf51f7, 0x00000001
	.section .unidentified.08bf7e82,"a"
	.incbin "baserom.gba", 0x00bf7e82, 0x00000002
	.section .unidentified.08bf8ece,"a"
	.incbin "baserom.gba", 0x00bf8ece, 0x00000002
	.section .unidentified.08bf9fcb,"a"
	.incbin "baserom.gba", 0x00bf9fcb, 0x00000001
	.section .unidentified.08bfb79f,"a"
	.incbin "baserom.gba", 0x00bfb79f, 0x00000001
	.section .unidentified.08bfcc63,"a"
	.incbin "baserom.gba", 0x00bfcc63, 0x00000001
	.section .unidentified.08bff196,"a"
	.incbin "baserom.gba", 0x00bff196, 0x00000002
	.section .unidentified.08bff2ca,"a"
	.incbin "baserom.gba", 0x00bff2ca, 0x00000002
	.section .unidentified.08c01a36,"a"
	.incbin "baserom.gba", 0x00c01a36, 0x00000002
	.section .unidentified.08c03809,"a"
	.incbin "baserom.gba", 0x00c03809, 0x00000003
	.section .unidentified.08c070ed,"a"
	.incbin "baserom.gba", 0x00c070ed, 0x00000003
	.section .unidentified.08c09853,"a"
	.incbin "baserom.gba", 0x00c09853, 0x00000001
	.section .unidentified.08c0b1bd,"a"
	.incbin "baserom.gba", 0x00c0b1bd, 0x00000003
	.section .unidentified.08c0d9e6,"a"
	.incbin "baserom.gba", 0x00c0d9e6, 0x00000002
	.section .unidentified.08c11dc5,"a"
	.incbin "baserom.gba", 0x00c11dc5, 0x00000003
	.section .unidentified.08c1516f,"a"
	.incbin "baserom.gba", 0x00c1516f, 0x00000001
	.section .unidentified.08c15f62,"a"
	.incbin "baserom.gba", 0x00c15f62, 0x00000002
	.section .unidentified.08c16cda,"a"
	.incbin "baserom.gba", 0x00c16cda, 0x00000002
	.section .unidentified.08c190cd,"a"
	.incbin "baserom.gba", 0x00c190cd, 0x00000003
	.section .unidentified.08c1ee02,"a"
	.incbin "baserom.gba", 0x00c1ee02, 0x00000002
	.section .unidentified.08c25181,"a"
	.incbin "baserom.gba", 0x00c25181, 0x00000003
	.section .unidentified.08c25b87,"a"
	.incbin "baserom.gba", 0x00c25b87, 0x00000001
	.section .unidentified.08c27635,"a"
	.incbin "baserom.gba", 0x00c27635, 0x00000003
	.global Resource_Data3CC
Resource_Data3CC:
	.incbin "baserom.gba", 0x00c27638, 0x00001490
	.section .unidentified.08c28f2f,"a"
	.incbin "baserom.gba", 0x00c28f2f, 0x00000001
	.section .unidentified.08c290ee,"a"
	.incbin "baserom.gba", 0x00c290ee, 0x00000002
	.section .unidentified.08c2b8f6,"a"
	.incbin "baserom.gba", 0x00c2b8f6, 0x00000002
	.section .unidentified.08c2ba4e,"a"
	.incbin "baserom.gba", 0x00c2ba4e, 0x00000002
	.section .unidentified.08c2e395,"a"
	.incbin "baserom.gba", 0x00c2e395, 0x00000003
	.section .unidentified.08c305b9,"a"
	.incbin "baserom.gba", 0x00c305b9, 0x00000003
	.section .unidentified.08c31aae,"a"
	.incbin "baserom.gba", 0x00c31aae, 0x00000002
	.section .unidentified.08c33d11,"a"
	.incbin "baserom.gba", 0x00c33d11, 0x00000003
	.section .unidentified.08c365bf,"a"
	.incbin "baserom.gba", 0x00c365bf, 0x00000001
	.section .unidentified.08c38552,"a"
	.incbin "baserom.gba", 0x00c38552, 0x00000002
	.section .unidentified.08c39573,"a"
	.incbin "baserom.gba", 0x00c39573, 0x00000001
	.section .unidentified.08c396b3,"a"
	.incbin "baserom.gba", 0x00c396b3, 0x00000001
	.section .unidentified.08c3c2d2,"a"
	.incbin "baserom.gba", 0x00c3c2d2, 0x00000002
	.section .unidentified.08c3e756,"a"
	.incbin "baserom.gba", 0x00c3e756, 0x00000002
	.section .unidentified.08c4083d,"a"
	.incbin "baserom.gba", 0x00c4083d, 0x00000003
	.section .unidentified.08c420a1,"a"
	.incbin "baserom.gba", 0x00c420a1, 0x00000003
	.section .unidentified.08c44607,"a"
	.incbin "baserom.gba", 0x00c44607, 0x00000001
	.section .unidentified.08c4476d,"a"
	.incbin "baserom.gba", 0x00c4476d, 0x00000003
	.section .unidentified.08c46f67,"a"
	.incbin "baserom.gba", 0x00c46f67, 0x00000001
	.section .unidentified.08c48f9b,"a"
	.incbin "baserom.gba", 0x00c48f9b, 0x00000001
	.section .unidentified.08c4abc2,"a"
	.incbin "baserom.gba", 0x00c4abc2, 0x00000002
	.section .unidentified.08c4d5e2,"a"
	.incbin "baserom.gba", 0x00c4d5e2, 0x00000002
	.section .unidentified.08c4e14d,"a"
	.incbin "baserom.gba", 0x00c4e14d, 0x00000003
	.section .unidentified.08c4e233,"a"
	.incbin "baserom.gba", 0x00c4e233, 0x00000001
	.section .unidentified.08c4fb7d,"a"
	.incbin "baserom.gba", 0x00c4fb7d, 0x00000003
	.section .unidentified.08c514e3,"a"
	.incbin "baserom.gba", 0x00c514e3, 0x00000001
	.section .unidentified.08c528b1,"a"
	.incbin "baserom.gba", 0x00c528b1, 0x00000003
	.section .unidentified.08c52997,"a"
	.incbin "baserom.gba", 0x00c52997, 0x00000001
	.section .unidentified.08c53505,"a"
	.incbin "baserom.gba", 0x00c53505, 0x00000003
	.section .unidentified.08c535eb,"a"
	.incbin "baserom.gba", 0x00c535eb, 0x00000001
	.section .unidentified.08c5434f,"a"
	.incbin "baserom.gba", 0x00c5434f, 0x00000001
	.section .unidentified.08c54433,"a"
	.incbin "baserom.gba", 0x00c54433, 0x00000001
	.section .unidentified.08c54c39,"a"
	.incbin "baserom.gba", 0x00c54c39, 0x00000003
	.section .unidentified.08c54d1f,"a"
	.incbin "baserom.gba", 0x00c54d1f, 0x00000001
	.section .unidentified.08c5587f,"a"
	.incbin "baserom.gba", 0x00c5587f, 0x00000001
	.section .unidentified.08c560fd,"a"
	.incbin "baserom.gba", 0x00c560fd, 0x00000003
	.section .unidentified.08c561fa,"a"
	.incbin "baserom.gba", 0x00c561fa, 0x00000002
	.section .unidentified.08c58053,"a"
	.incbin "baserom.gba", 0x00c58053, 0x00000001
	.section .unidentified.08c58e61,"a"
	.incbin "baserom.gba", 0x00c58e61, 0x00000003
	.section .unidentified.08c5a0f5,"a"
	.incbin "baserom.gba", 0x00c5a0f5, 0x00000003
	.section .unidentified.08c5b0d1,"a"
	.incbin "baserom.gba", 0x00c5b0d1, 0x00000003
	.section .unidentified.08c5d4d7,"a"
	.incbin "baserom.gba", 0x00c5d4d7, 0x00000001
	.section .unidentified.08c5d617,"a"
	.incbin "baserom.gba", 0x00c5d617, 0x00000001
	.section .unidentified.08c5dce5,"a"
	.incbin "baserom.gba", 0x00c5dce5, 0x00000003
	.section .unidentified.08c5ddbb,"a"
	.incbin "baserom.gba", 0x00c5ddbb, 0x00000001
	.section .unidentified.08c5e631,"a"
	.incbin "baserom.gba", 0x00c5e631, 0x00000003
	.section .unidentified.08c5eae2,"a"
	.incbin "baserom.gba", 0x00c5eae2, 0x00000002
	.section .unidentified.08c5ff5d,"a"
	.incbin "baserom.gba", 0x00c5ff5d, 0x00000003
	.section .unidentified.08c60773,"a"
	.incbin "baserom.gba", 0x00c60773, 0x00000001
	.section .unidentified.08c61409,"a"
	.incbin "baserom.gba", 0x00c61409, 0x00000003
	.section .unidentified.08c615ae,"a"
	.incbin "baserom.gba", 0x00c615ae, 0x00000002
	.section .unidentified.08c64472,"a"
	.incbin "baserom.gba", 0x00c64472, 0x00000002
	.section .unidentified.08c65df1,"a"
	.incbin "baserom.gba", 0x00c65df1, 0x00000003
	.section .unidentified.08c66e3e,"a"
	.incbin "baserom.gba", 0x00c66e3e, 0x00000002
	.section .unidentified.08c694af,"a"
	.incbin "baserom.gba", 0x00c694af, 0x00000001
	.section .unidentified.08c6a235,"a"
	.incbin "baserom.gba", 0x00c6a235, 0x00000003
	.section .unidentified.08c6b099,"a"
	.incbin "baserom.gba", 0x00c6b099, 0x00000003
	.section .unidentified.08c6e583,"a"
	.incbin "baserom.gba", 0x00c6e583, 0x00000001
	.section .unidentified.08c75d77,"a"
	.incbin "baserom.gba", 0x00c75d77, 0x00000001
	.section .unidentified.08c7c371,"a"
	.incbin "baserom.gba", 0x00c7c371, 0x00000003
	.section .unidentified.08c7edaa,"a"
	.incbin "baserom.gba", 0x00c7edaa, 0x00000002
	.section .unidentified.08c8167b,"a"
	.incbin "baserom.gba", 0x00c8167b, 0x00000001
	.section .unidentified.08c85ec7,"a"
	.incbin "baserom.gba", 0x00c85ec7, 0x00000001
	.section .unidentified.08c86ec5,"a"
	.incbin "baserom.gba", 0x00c86ec5, 0x00000003
	.section .unidentified.08c86fde,"a"
	.incbin "baserom.gba", 0x00c86fde, 0x00000002
	.section .unidentified.08c88586,"a"
	.incbin "baserom.gba", 0x00c88586, 0x00000002
	.section .unidentified.08c89fe3,"a"
	.incbin "baserom.gba", 0x00c89fe3, 0x00000001
	.section .unidentified.08c8a11e,"a"
	.incbin "baserom.gba", 0x00c8a11e, 0x00000002
	.section .unidentified.08c8b625,"a"
	.incbin "baserom.gba", 0x00c8b625, 0x00000003
	.section .unidentified.08c8e8ae,"a"
	.incbin "baserom.gba", 0x00c8e8ae, 0x00000002
	.section .unidentified.08c8fcf5,"a"
	.incbin "baserom.gba", 0x00c8fcf5, 0x00000003
	.section .unidentified.08c929ed,"a"
	.incbin "baserom.gba", 0x00c929ed, 0x00000003
	.section .unidentified.08c94e23,"a"
	.incbin "baserom.gba", 0x00c94e23, 0x00000001
	.section .unidentified.08c94fff,"a"
	.incbin "baserom.gba", 0x00c94fff, 0x00000001
	.section .unidentified.08c98113,"a"
	.incbin "baserom.gba", 0x00c98113, 0x00000001
	.section .unidentified.08c99596,"a"
	.incbin "baserom.gba", 0x00c99596, 0x00000002
	.section .unidentified.08c9a5e5,"a"
	.incbin "baserom.gba", 0x00c9a5e5, 0x00000003
	.section .unidentified.08c9c44a,"a"
	.incbin "baserom.gba", 0x00c9c44a, 0x00000002
	.section .unidentified.08c9c5e5,"a"
	.incbin "baserom.gba", 0x00c9c5e5, 0x00000003
	.section .unidentified.08c9c743,"a"
	.incbin "baserom.gba", 0x00c9c743, 0x00000001
	.section .unidentified.08c9d6c2,"a"
	.incbin "baserom.gba", 0x00c9d6c2, 0x00000002
	.section .unidentified.08c9d7da,"a"
	.incbin "baserom.gba", 0x00c9d7da, 0x00000002
	.section .unidentified.08c9f86d,"a"
	.incbin "baserom.gba", 0x00c9f86d, 0x00000003
	.section .unidentified.08ca16ef,"a"
	.incbin "baserom.gba", 0x00ca16ef, 0x00000001
	.section .unidentified.08ca3c15,"a"
	.incbin "baserom.gba", 0x00ca3c15, 0x00000003
	.section .unidentified.08ca3d22,"a"
	.incbin "baserom.gba", 0x00ca3d22, 0x00000002
	.section .unidentified.08ca5db5,"a"
	.incbin "baserom.gba", 0x00ca5db5, 0x00000003
	.section .unidentified.08ca928b,"a"
	.incbin "baserom.gba", 0x00ca928b, 0x00000001
	.section .unidentified.08ca9d75,"a"
	.incbin "baserom.gba", 0x00ca9d75, 0x00000003
	.section .unidentified.08ca9ee5,"a"
	.incbin "baserom.gba", 0x00ca9ee5, 0x00000003
	.section .unidentified.08cacdff,"a"
	.incbin "baserom.gba", 0x00cacdff, 0x00000001
	.section .unidentified.08caed8a,"a"
	.incbin "baserom.gba", 0x00caed8a, 0x00000002
	.section .unidentified.08cb06f5,"a"
	.incbin "baserom.gba", 0x00cb06f5, 0x00000003
	.section .unidentified.08cb12dd,"a"
	.incbin "baserom.gba", 0x00cb12dd, 0x00000003
	.section .unidentified.08cb2f4f,"a"
	.incbin "baserom.gba", 0x00cb2f4f, 0x00000001
	.section .unidentified.08cb5327,"a"
	.incbin "baserom.gba", 0x00cb5327, 0x00000001
	.section .unidentified.08cb61e2,"a"
	.incbin "baserom.gba", 0x00cb61e2, 0x00000002
	.section .unidentified.08cb6392,"a"
	.incbin "baserom.gba", 0x00cb6392, 0x00000002
	.section .unidentified.08cb89bf,"a"
	.incbin "baserom.gba", 0x00cb89bf, 0x00000001
	.section .unidentified.08cbad0a,"a"
	.incbin "baserom.gba", 0x00cbad0a, 0x00000002
	.section .unidentified.08cbae62,"a"
	.incbin "baserom.gba", 0x00cbae62, 0x00000002
	.section .unidentified.08cbe42f,"a"
	.incbin "baserom.gba", 0x00cbe42f, 0x00000001
	.section .unidentified.08cbfcb1,"a"
	.incbin "baserom.gba", 0x00cbfcb1, 0x00000003
	.section .unidentified.08cc19d9,"a"
	.incbin "baserom.gba", 0x00cc19d9, 0x00000003
	.section .unidentified.08cc50fa,"a"
	.incbin "baserom.gba", 0x00cc50fa, 0x00000002
	.section .unidentified.08cca283,"a"
	.incbin "baserom.gba", 0x00cca283, 0x00000001
	.section .unidentified.08ccc851,"a"
	.incbin "baserom.gba", 0x00ccc851, 0x00000003
	.section .unidentified.08cce1dd,"a"
	.incbin "baserom.gba", 0x00cce1dd, 0x00000003
	.section .unidentified.08ccf30a,"a"
	.incbin "baserom.gba", 0x00ccf30a, 0x00000002
	.section .unidentified.08ccfeba,"a"
	.incbin "baserom.gba", 0x00ccfeba, 0x00000002
	.section .unidentified.08cd1882,"a"
	.incbin "baserom.gba", 0x00cd1882, 0x00000002
	.section .unidentified.08cd1966,"a"
	.incbin "baserom.gba", 0x00cd1966, 0x00000002
	.section .unidentified.08cd3dc3,"a"
	.incbin "baserom.gba", 0x00cd3dc3, 0x00000001
	.section .unidentified.08cd3f03,"a"
	.incbin "baserom.gba", 0x00cd3f03, 0x00000001
	.section .unidentified.08cd7579,"a"
	.incbin "baserom.gba", 0x00cd7579, 0x00000003
	.section .unidentified.08cd76dd,"a"
	.incbin "baserom.gba", 0x00cd76dd, 0x00000003
	.section .unidentified.08cd9dbd,"a"
	.incbin "baserom.gba", 0x00cd9dbd, 0x00000003
	.section .unidentified.08cdcc6d,"a"
	.incbin "baserom.gba", 0x00cdcc6d, 0x00000003
	.section .unidentified.08cde1e2,"a"
	.incbin "baserom.gba", 0x00cde1e2, 0x00000002
	.section .unidentified.08ce22de,"a"
	.incbin "baserom.gba", 0x00ce22de, 0x00000002
	.section .unidentified.08ce4563,"a"
	.incbin "baserom.gba", 0x00ce4563, 0x00000001
	.section .unidentified.08ce6147,"a"
	.incbin "baserom.gba", 0x00ce6147, 0x00000001
	.section .unidentified.08ce7e3d,"a"
	.incbin "baserom.gba", 0x00ce7e3d, 0x00000003
	.section .unidentified.08ceab8d,"a"
	.incbin "baserom.gba", 0x00ceab8d, 0x00000003
	.section .unidentified.08cecbd5,"a"
	.incbin "baserom.gba", 0x00cecbd5, 0x00000003
	.section .unidentified.08cf1276,"a"
	.incbin "baserom.gba", 0x00cf1276, 0x00000002
	.section .unidentified.08cf1e05,"a"
	.incbin "baserom.gba", 0x00cf1e05, 0x00000003
	.section .unidentified.08cf5461,"a"
	.incbin "baserom.gba", 0x00cf5461, 0x00000003
	.section .unidentified.08cf5589,"a"
	.incbin "baserom.gba", 0x00cf5589, 0x00000003
	.section .unidentified.08cf779a,"a"
	.incbin "baserom.gba", 0x00cf779a, 0x00000002
	.section .unidentified.08cf792b,"a"
	.incbin "baserom.gba", 0x00cf792b, 0x00000001
	.section .unidentified.08cfeddd,"a"
	.incbin "baserom.gba", 0x00cfeddd, 0x00000003
	.section .unidentified.08d015b3,"a"
	.incbin "baserom.gba", 0x00d015b3, 0x00000001
	.section .unidentified.08d03d0e,"a"
	.incbin "baserom.gba", 0x00d03d0e, 0x00000002
	.section .unidentified.08d03ec1,"a"
	.incbin "baserom.gba", 0x00d03ec1, 0x00000003
	.section .unidentified.08d055c5,"a"
	.incbin "baserom.gba", 0x00d055c5, 0x00000003
	.section .unidentified.08d0761f,"a"
	.incbin "baserom.gba", 0x00d0761f, 0x00000001
	.section .unidentified.08d08969,"a"
	.incbin "baserom.gba", 0x00d08969, 0x00000003
	.section .unidentified.08d0a3b3,"a"
	.incbin "baserom.gba", 0x00d0a3b3, 0x00000001
	.section .unidentified.08d0a4d7,"a"
	.incbin "baserom.gba", 0x00d0a4d7, 0x00000001
	.section .unidentified.08d0a9a9,"a"
	.incbin "baserom.gba", 0x00d0a9a9, 0x00000003
	.section .unidentified.08d0cb8d,"a"
	.incbin "baserom.gba", 0x00d0cb8d, 0x00000003
	.section .unidentified.08d0d061,"a"
	.incbin "baserom.gba", 0x00d0d061, 0x00000003
	.section .unidentified.08d0f597,"a"
	.incbin "baserom.gba", 0x00d0f597, 0x00000001
	.section .unidentified.08d0f71f,"a"
	.incbin "baserom.gba", 0x00d0f71f, 0x00000001
	.section .unidentified.08d110d2,"a"
	.incbin "baserom.gba", 0x00d110d2, 0x00000002
	.section .unidentified.08d1232d,"a"
	.incbin "baserom.gba", 0x00d1232d, 0x00000003
	.section .unidentified.08d13fc1,"a"
	.incbin "baserom.gba", 0x00d13fc1, 0x00000003
	.section .unidentified.08d140f7,"a"
	.incbin "baserom.gba", 0x00d140f7, 0x00000001
	.section .unidentified.08d15d63,"a"
	.incbin "baserom.gba", 0x00d15d63, 0x00000001
	.section .unidentified.08d16f1b,"a"
	.incbin "baserom.gba", 0x00d16f1b, 0x00000001
	.section .unidentified.08d17c69,"a"
	.incbin "baserom.gba", 0x00d17c69, 0x00000003
	.section .unidentified.08d18df2,"a"
	.incbin "baserom.gba", 0x00d18df2, 0x00000002
	.section .unidentified.08d1a8eb,"a"
	.incbin "baserom.gba", 0x00d1a8eb, 0x00000001
	.section .unidentified.08d1bcf7,"a"
	.incbin "baserom.gba", 0x00d1bcf7, 0x00000001
	.section .unidentified.08d1c24b,"a"
	.incbin "baserom.gba", 0x00d1c24b, 0x00000001
	.section .unidentified.08d1c885,"a"
	.incbin "baserom.gba", 0x00d1c885, 0x00000003
	.section .unidentified.08d1d7ef,"a"
	.incbin "baserom.gba", 0x00d1d7ef, 0x00000001
	.section .unidentified.08d1f89d,"a"
	.incbin "baserom.gba", 0x00d1f89d, 0x00000003
	.section .unidentified.08d21bd5,"a"
	.incbin "baserom.gba", 0x00d21bd5, 0x00000003
	.section .unidentified.08d24236,"a"
	.incbin "baserom.gba", 0x00d24236, 0x00000002
	.section .unidentified.08d243cb,"a"
	.incbin "baserom.gba", 0x00d243cb, 0x00000001
	.section .unidentified.08d25f7f,"a"
	.incbin "baserom.gba", 0x00d25f7f, 0x00000001
	.section .unidentified.08d260df,"a"
	.incbin "baserom.gba", 0x00d260df, 0x00000001
	.section .unidentified.08d28b83,"a"
	.incbin "baserom.gba", 0x00d28b83, 0x00000001
	.global Resource_Data4B3
Resource_Data4B3:
	.incbin "baserom.gba", 0x00d28b84, 0x000021b0
	.section .unidentified.08d2cbaa,"a"
	.incbin "baserom.gba", 0x00d2cbaa, 0x00000002
	.section .unidentified.08d2cceb,"a"
	.incbin "baserom.gba", 0x00d2cceb, 0x00000001
	.section .unidentified.08d2ea0f,"a"
	.incbin "baserom.gba", 0x00d2ea0f, 0x00000001
	.section .unidentified.08d3069b,"a"
	.incbin "baserom.gba", 0x00d3069b, 0x00000001
	.section .unidentified.08d32677,"a"
	.incbin "baserom.gba", 0x00d32677, 0x00000001
	.section .unidentified.08d33c63,"a"
	.incbin "baserom.gba", 0x00d33c63, 0x00000001
	.section .unidentified.08d33da1,"a"
	.incbin "baserom.gba", 0x00d33da1, 0x00000003
	.section .unidentified.08d35d75,"a"
	.incbin "baserom.gba", 0x00d35d75, 0x00000003
	.section .unidentified.08d35e69,"a"
	.incbin "baserom.gba", 0x00d35e69, 0x00000003
	.section .unidentified.08d36f49,"a"
	.incbin "baserom.gba", 0x00d36f49, 0x00000003
	.section .unidentified.08d38e23,"a"
	.incbin "baserom.gba", 0x00d38e23, 0x00000001
	.section .unidentified.08d39ddb,"a"
	.incbin "baserom.gba", 0x00d39ddb, 0x00000001
	.section .unidentified.08d3bd22,"a"
	.incbin "baserom.gba", 0x00d3bd22, 0x00000002
	.section .unidentified.08d3d92d,"a"
	.incbin "baserom.gba", 0x00d3d92d, 0x00000003
	.section .unidentified.08d3da79,"a"
	.incbin "baserom.gba", 0x00d3da79, 0x00000003
	.section .unidentified.08d3fb37,"a"
	.incbin "baserom.gba", 0x00d3fb37, 0x00000001
	.section .unidentified.08d43c26,"a"
	.incbin "baserom.gba", 0x00d43c26, 0x00000002
	.section .unidentified.08d479d8,"a"
	.global Resource_Data4C8
Resource_Data4C8:
	.incbin "baserom.gba", 0x00d479d8, 0x00001f0c
	.global Resource_Data4C9
Resource_Data4C9:
	.incbin "baserom.gba", 0x00d498e4, 0x00002364
	.global Resource_Data4CA
Resource_Data4CA:
	.incbin "baserom.gba", 0x00d4bc48, 0x0000202c
	.global Resource_Data4CB
Resource_Data4CB:
	.incbin "baserom.gba", 0x00d4dc74, 0x0000180c
	.section .unidentified.08d50bcd,"a"
	.incbin "baserom.gba", 0x00d50bcd, 0x00000003
	.section .unidentified.08d50cfa,"a"
	.incbin "baserom.gba", 0x00d50cfa, 0x00000002
	.section .unidentified.08d5264b,"a"
	.incbin "baserom.gba", 0x00d5264b, 0x00000001
	.section .unidentified.08d53d2f,"a"
	.incbin "baserom.gba", 0x00d53d2f, 0x00000001
	.section .unidentified.08d55549,"a"
	.incbin "baserom.gba", 0x00d55549, 0x00000003
	.section .unidentified.08d5618b,"a"
	.incbin "baserom.gba", 0x00d5618b, 0x00000001
	.section .unidentified.08d57c6d,"a"
	.incbin "baserom.gba", 0x00d57c6d, 0x00000003
	.section .unidentified.08d57d72,"a"
	.incbin "baserom.gba", 0x00d57d72, 0x00000002
	.section .unidentified.08d57eb2,"a"
	.incbin "baserom.gba", 0x00d57eb2, 0x00000002
	.section .unidentified.08d5987e,"a"
	.incbin "baserom.gba", 0x00d5987e, 0x00000002
	.section .unidentified.08d59975,"a"
	.incbin "baserom.gba", 0x00d59975, 0x00000003
	.section .unidentified.08d5c7c9,"a"
	.incbin "baserom.gba", 0x00d5c7c9, 0x00000003
	.section .unidentified.08d5c972,"a"
	.incbin "baserom.gba", 0x00d5c972, 0x00000002
	.section .unidentified.08d5f58e,"a"
	.incbin "baserom.gba", 0x00d5f58e, 0x00000002
	.section .unidentified.08d622c1,"a"
	.incbin "baserom.gba", 0x00d622c1, 0x00000003
	.section .unidentified.08d63fab,"a"
	.incbin "baserom.gba", 0x00d63fab, 0x00000001
	.section .unidentified.08d67825,"a"
	.incbin "baserom.gba", 0x00d67825, 0x00000003
	.section .unidentified.08d6797d,"a"
	.incbin "baserom.gba", 0x00d6797d, 0x00000003
	.section .unidentified.08d69ec1,"a"
	.incbin "baserom.gba", 0x00d69ec1, 0x00000003
	.section .unidentified.08d6dd86,"a"
	.incbin "baserom.gba", 0x00d6dd86, 0x00000002
	.section .unidentified.08d6feb6,"a"
	.incbin "baserom.gba", 0x00d6feb6, 0x00000002
	.section .unidentified.08d740c2,"a"
	.incbin "baserom.gba", 0x00d740c2, 0x00000002
	.section .unidentified.08d74296,"a"
	.incbin "baserom.gba", 0x00d74296, 0x00000002
	.section .unidentified.08d762d6,"a"
	.incbin "baserom.gba", 0x00d762d6, 0x00000002
	.section .unidentified.08d776cd,"a"
	.incbin "baserom.gba", 0x00d776cd, 0x00000003
	.section .unidentified.08d7820a,"a"
	.incbin "baserom.gba", 0x00d7820a, 0x00000002
	.section .unidentified.08d79375,"a"
	.incbin "baserom.gba", 0x00d79375, 0x00000003
	.section .unidentified.08d7a421,"a"
	.incbin "baserom.gba", 0x00d7a421, 0x00000003
	.section .unidentified.08d7a5cd,"a"
	.incbin "baserom.gba", 0x00d7a5cd, 0x00000003
	.section .unidentified.08d7c0da,"a"
	.incbin "baserom.gba", 0x00d7c0da, 0x00000002
	.section .unidentified.08d7daf7,"a"
	.incbin "baserom.gba", 0x00d7daf7, 0x00000001
	.section .unidentified.08d7e97a,"a"
	.incbin "baserom.gba", 0x00d7e97a, 0x00000002
	.section .unidentified.08d7ffc9,"a"
	.incbin "baserom.gba", 0x00d7ffc9, 0x00000003
	.section .unidentified.08d8132b,"a"
	.incbin "baserom.gba", 0x00d8132b, 0x00000001
	.section .unidentified.08d821df,"a"
	.incbin "baserom.gba", 0x00d821df, 0x00000001
	.section .unidentified.08d837cd,"a"
	.incbin "baserom.gba", 0x00d837cd, 0x00000003
	.section .unidentified.08d84d13,"a"
	.incbin "baserom.gba", 0x00d84d13, 0x00000001
	.section .unidentified.08d85add,"a"
	.incbin "baserom.gba", 0x00d85add, 0x00000003
	.section .unidentified.08d85c4b,"a"
	.incbin "baserom.gba", 0x00d85c4b, 0x00000001
	.section .unidentified.08d86c0a,"a"
	.incbin "baserom.gba", 0x00d86c0a, 0x00000002
	.section .unidentified.08d876d3,"a"
	.incbin "baserom.gba", 0x00d876d3, 0x00000001
	.section .unidentified.08d8826d,"a"
	.incbin "baserom.gba", 0x00d8826d, 0x00000003
	.section .unidentified.08d883bd,"a"
	.incbin "baserom.gba", 0x00d883bd, 0x00000003
	.section .unidentified.08d8c66b,"a"
	.incbin "baserom.gba", 0x00d8c66b, 0x00000001
	.section .unidentified.08d8e105,"a"
	.incbin "baserom.gba", 0x00d8e105, 0x00000003
	.section .unidentified.08d8fadb,"a"
	.incbin "baserom.gba", 0x00d8fadb, 0x00000001
	.section .unidentified.08d8fc5a,"a"
	.incbin "baserom.gba", 0x00d8fc5a, 0x00000002
	.section .unidentified.08d9388a,"a"
	.incbin "baserom.gba", 0x00d9388a, 0x00000002
	.section .unidentified.08d955e7,"a"
	.incbin "baserom.gba", 0x00d955e7, 0x00000001
	.section .unidentified.08d98c11,"a"
	.incbin "baserom.gba", 0x00d98c11, 0x00000003
	.section .unidentified.08d98d62,"a"
	.incbin "baserom.gba", 0x00d98d62, 0x00000002
	.section .unidentified.08d9e95e,"a"
	.incbin "baserom.gba", 0x00d9e95e, 0x00000002
	.section .unidentified.08da129b,"a"
	.incbin "baserom.gba", 0x00da129b, 0x00000001
	.section .unidentified.08da2bdd,"a"
	.incbin "baserom.gba", 0x00da2bdd, 0x00000003
	.section .unidentified.08da2d46,"a"
	.incbin "baserom.gba", 0x00da2d46, 0x00000002
	.section .unidentified.08da9b45,"a"
	.incbin "baserom.gba", 0x00da9b45, 0x00000003
	.section .unidentified.08daa123,"a"
	.incbin "baserom.gba", 0x00daa123, 0x00000001
	.section .unidentified.08dab02b,"a"
	.incbin "baserom.gba", 0x00dab02b, 0x00000001
	.section .unidentified.08dad873,"a"
	.incbin "baserom.gba", 0x00dad873, 0x00000001
	.section .unidentified.08daee92,"a"
	.incbin "baserom.gba", 0x00daee92, 0x00000002
	.section .unidentified.08db07f3,"a"
	.incbin "baserom.gba", 0x00db07f3, 0x00000001
	.section .unidentified.08db35db,"a"
	.incbin "baserom.gba", 0x00db35db, 0x00000001
	.section .unidentified.08db3756,"a"
	.incbin "baserom.gba", 0x00db3756, 0x00000002
	.section .unidentified.08db5e87,"a"
	.incbin "baserom.gba", 0x00db5e87, 0x00000001
	.section .unidentified.08db749f,"a"
	.incbin "baserom.gba", 0x00db749f, 0x00000001
	.section .unidentified.08db9d12,"a"
	.incbin "baserom.gba", 0x00db9d12, 0x00000002
	.section .unidentified.08dbd049,"a"
	.incbin "baserom.gba", 0x00dbd049, 0x00000003
	.section .unidentified.08dbe6aa,"a"
	.incbin "baserom.gba", 0x00dbe6aa, 0x00000002
	.section .unidentified.08dbff6a,"a"
	.incbin "baserom.gba", 0x00dbff6a, 0x00000002
	.section .unidentified.08dc18b9,"a"
	.incbin "baserom.gba", 0x00dc18b9, 0x00000003
	.section .unidentified.08dc291f,"a"
	.incbin "baserom.gba", 0x00dc291f, 0x00000001
	.section .unidentified.08dc2a87,"a"
	.incbin "baserom.gba", 0x00dc2a87, 0x00000001
	.section .unidentified.08dc50aa,"a"
	.incbin "baserom.gba", 0x00dc50aa, 0x00000002
	.section .unidentified.08dc51ed,"a"
	.incbin "baserom.gba", 0x00dc51ed, 0x00000003
	.section .unidentified.08dc6aaa,"a"
	.incbin "baserom.gba", 0x00dc6aaa, 0x00000002
	.section .unidentified.08dc9a0e,"a"
	.incbin "baserom.gba", 0x00dc9a0e, 0x00000002
	.section .unidentified.08dcc3a6,"a"
	.incbin "baserom.gba", 0x00dcc3a6, 0x00000002
	.section .unidentified.08dd0cfe,"a"
	.incbin "baserom.gba", 0x00dd0cfe, 0x00000002
	.section .unidentified.08dd3327,"a"
	.incbin "baserom.gba", 0x00dd3327, 0x00000001
	.section .unidentified.08dd34fb,"a"
	.incbin "baserom.gba", 0x00dd34fb, 0x00000001
	.section .unidentified.08dd681a,"a"
	.incbin "baserom.gba", 0x00dd681a, 0x00000002
	.section .unidentified.08dd69f1,"a"
	.incbin "baserom.gba", 0x00dd69f1, 0x00000003
	.section .unidentified.08dd9d65,"a"
	.incbin "baserom.gba", 0x00dd9d65, 0x00000003
	.section .unidentified.08ddb4b9,"a"
	.incbin "baserom.gba", 0x00ddb4b9, 0x00000003
	.section .unidentified.08ddc447,"a"
	.incbin "baserom.gba", 0x00ddc447, 0x00000001
	.section .unidentified.08ddddbd,"a"
	.incbin "baserom.gba", 0x00ddddbd, 0x00000003
	.section .unidentified.08dddef7,"a"
	.incbin "baserom.gba", 0x00dddef7, 0x00000001
	.section .unidentified.08ddfa39,"a"
	.incbin "baserom.gba", 0x00ddfa39, 0x00000003
	.section .unidentified.08de3011,"a"
	.incbin "baserom.gba", 0x00de3011, 0x00000003
	.section .unidentified.08de3c9b,"a"
	.incbin "baserom.gba", 0x00de3c9b, 0x00000001
	.section .unidentified.08de3ddb,"a"
	.incbin "baserom.gba", 0x00de3ddb, 0x00000001
	.section .unidentified.08de5b9f,"a"
	.incbin "baserom.gba", 0x00de5b9f, 0x00000001
	.section .unidentified.08de5d12,"a"
	.incbin "baserom.gba", 0x00de5d12, 0x00000002
	.section .unidentified.08de817a,"a"
	.incbin "baserom.gba", 0x00de817a, 0x00000002
	.section .unidentified.08de9b16,"a"
	.incbin "baserom.gba", 0x00de9b16, 0x00000002
	.section .unidentified.08deb94b,"a"
	.incbin "baserom.gba", 0x00deb94b, 0x00000001
	.section .unidentified.08dec402,"a"
	.incbin "baserom.gba", 0x00dec402, 0x00000002
	.section .unidentified.08dede1e,"a"
	.incbin "baserom.gba", 0x00dede1e, 0x00000002
	.section .unidentified.08df0aea,"a"
	.incbin "baserom.gba", 0x00df0aea, 0x00000002
	.section .unidentified.08df0cc5,"a"
	.incbin "baserom.gba", 0x00df0cc5, 0x00000003
	.section .unidentified.08df27fb,"a"
	.incbin "baserom.gba", 0x00df27fb, 0x00000001
	.section .unidentified.08df58c5,"a"
	.incbin "baserom.gba", 0x00df58c5, 0x00000003
	.section .unidentified.08df6a81,"a"
	.incbin "baserom.gba", 0x00df6a81, 0x00000003
	.section .unidentified.08df6c69,"a"
	.incbin "baserom.gba", 0x00df6c69, 0x00000003
	.section .unidentified.08df6dfb,"a"
	.incbin "baserom.gba", 0x00df6dfb, 0x00000001
	.section .unidentified.08df7b82,"a"
	.incbin "baserom.gba", 0x00df7b82, 0x00000002
	.section .unidentified.08df7d17,"a"
	.incbin "baserom.gba", 0x00df7d17, 0x00000001
	.section .unidentified.08df9dcb,"a"
	.incbin "baserom.gba", 0x00df9dcb, 0x00000001
	.section .unidentified.08dfb0e7,"a"
	.incbin "baserom.gba", 0x00dfb0e7, 0x00000001
	.section .unidentified.08dfc2b1,"a"
	.incbin "baserom.gba", 0x00dfc2b1, 0x00000003
	.section .unidentified.08dfe8c1,"a"
	.incbin "baserom.gba", 0x00dfe8c1, 0x00000003
	.section .unidentified.08e01e4e,"a"
	.incbin "baserom.gba", 0x00e01e4e, 0x00000002
	.section .unidentified.08e01f8f,"a"
	.incbin "baserom.gba", 0x00e01f8f, 0x00000001
	.section .unidentified.08e06317,"a"
	.incbin "baserom.gba", 0x00e06317, 0x00000001
	.section .unidentified.08e06a36,"a"
	.incbin "baserom.gba", 0x00e06a36, 0x00000002
	.section .unidentified.08e07df5,"a"
	.incbin "baserom.gba", 0x00e07df5, 0x00000003
	.section .unidentified.08e08e8f,"a"
	.incbin "baserom.gba", 0x00e08e8f, 0x00000001
	.section .unidentified.08e0bf0f,"a"
	.incbin "baserom.gba", 0x00e0bf0f, 0x00000001
	.section .unidentified.08e0cf8f,"a"
	.incbin "baserom.gba", 0x00e0cf8f, 0x00000001
	.section .unidentified.08e0e011,"a"
	.incbin "baserom.gba", 0x00e0e011, 0x00000003
	.section .unidentified.08e0eb82,"a"
	.incbin "baserom.gba", 0x00e0eb82, 0x00000002
	.section .unidentified.08e0fad3,"a"
	.incbin "baserom.gba", 0x00e0fad3, 0x00000001
	.section .unidentified.08e0fc51,"a"
	.incbin "baserom.gba", 0x00e0fc51, 0x00000003
	.section .unidentified.08e131ab,"a"
	.incbin "baserom.gba", 0x00e131ab, 0x00000001
	.section .unidentified.08e147de,"a"
	.incbin "baserom.gba", 0x00e147de, 0x00000002
	.section .unidentified.08e1491f,"a"
	.incbin "baserom.gba", 0x00e1491f, 0x00000001
	.section .unidentified.08e1726a,"a"
	.incbin "baserom.gba", 0x00e1726a, 0x00000002
	.section .unidentified.08e1fc12,"a"
	.incbin "baserom.gba", 0x00e1fc12, 0x00000002
	.global Resource_Data57B
Resource_Data57B:
	.incbin "baserom.gba", 0x00e1fc14, 0x00000fdc
	.section .unidentified.08e227bd,"a"
	.incbin "baserom.gba", 0x00e227bd, 0x00000003
	.section .unidentified.08e246fd,"a"
	.incbin "baserom.gba", 0x00e246fd, 0x00000003
	.section .unidentified.08e25b71,"a"
	.incbin "baserom.gba", 0x00e25b71, 0x00000003
	.section .unidentified.08e2782b,"a"
	.incbin "baserom.gba", 0x00e2782b, 0x00000001
	.section .unidentified.08e29133,"a"
	.incbin "baserom.gba", 0x00e29133, 0x00000001
	.section .unidentified.08e2aa79,"a"
	.incbin "baserom.gba", 0x00e2aa79, 0x00000003
	.section .unidentified.08e2c12b,"a"
	.incbin "baserom.gba", 0x00e2c12b, 0x00000001
	.section .unidentified.08e2d9d5,"a"
	.incbin "baserom.gba", 0x00e2d9d5, 0x00000003
	.section .unidentified.08e2ec7e,"a"
	.incbin "baserom.gba", 0x00e2ec7e, 0x00000002
	.section .unidentified.08e30453,"a"
	.incbin "baserom.gba", 0x00e30453, 0x00000001
	.section .unidentified.08e315e5,"a"
	.incbin "baserom.gba", 0x00e315e5, 0x00000003
	.section .unidentified.08e32bc3,"a"
	.incbin "baserom.gba", 0x00e32bc3, 0x00000001
	.section .unidentified.08e348da,"a"
	.incbin "baserom.gba", 0x00e348da, 0x00000002
	.section .unidentified.08e34a19,"a"
	.incbin "baserom.gba", 0x00e34a19, 0x00000003
	.section .unidentified.08e3693e,"a"
	.incbin "baserom.gba", 0x00e3693e, 0x00000002
	.section .unidentified.08e36a67,"a"
	.incbin "baserom.gba", 0x00e36a67, 0x00000001
	.section .unidentified.08e38c37,"a"
	.incbin "baserom.gba", 0x00e38c37, 0x00000001
	.section .unidentified.08e3b207,"a"
	.incbin "baserom.gba", 0x00e3b207, 0x00000001
	.section .unidentified.08e3d519,"a"
	.incbin "baserom.gba", 0x00e3d519, 0x00000003
	.section .unidentified.08e3dd8d,"a"
	.incbin "baserom.gba", 0x00e3dd8d, 0x00000003
	.section .unidentified.08e3decf,"a"
	.incbin "baserom.gba", 0x00e3decf, 0x00000001
	.section .unidentified.08e40f85,"a"
	.incbin "baserom.gba", 0x00e40f85, 0x00000003
	.section .unidentified.08e4266b,"a"
	.incbin "baserom.gba", 0x00e4266b, 0x00000001
	.section .unidentified.08e44233,"a"
	.incbin "baserom.gba", 0x00e44233, 0x00000001
	.section .unidentified.08e45dae,"a"
	.incbin "baserom.gba", 0x00e45dae, 0x00000002
	.section .unidentified.08e461f9,"a"
	.incbin "baserom.gba", 0x00e461f9, 0x00000003
	.section .unidentified.08e4835a,"a"
	.incbin "baserom.gba", 0x00e4835a, 0x00000002
	.section .unidentified.08e48473,"a"
	.incbin "baserom.gba", 0x00e48473, 0x00000001
	.section .unidentified.08e4929d,"a"
	.incbin "baserom.gba", 0x00e4929d, 0x00000003
	.section .unidentified.08e493ff,"a"
	.incbin "baserom.gba", 0x00e493ff, 0x00000001
	.section .unidentified.08e4dc67,"a"
	.incbin "baserom.gba", 0x00e4dc67, 0x00000001
	.section .unidentified.08e50325,"a"
	.incbin "baserom.gba", 0x00e50325, 0x00000003
	.section .unidentified.08e50903,"a"
	.incbin "baserom.gba", 0x00e50903, 0x00000001
	.section .unidentified.08e52896,"a"
	.incbin "baserom.gba", 0x00e52896, 0x00000002
	.section .unidentified.08e54076,"a"
	.incbin "baserom.gba", 0x00e54076, 0x00000002
	.section .unidentified.08e541da,"a"
	.incbin "baserom.gba", 0x00e541da, 0x00000002
	.section .unidentified.08e569d2,"a"
	.incbin "baserom.gba", 0x00e569d2, 0x00000002
	.section .unidentified.08e56b8b,"a"
	.incbin "baserom.gba", 0x00e56b8b, 0x00000001
	.section .unidentified.08e56d13,"a"
	.incbin "baserom.gba", 0x00e56d13, 0x00000001
	.section .unidentified.08e58535,"a"
	.incbin "baserom.gba", 0x00e58535, 0x00000003
	.section .unidentified.08e5911e,"a"
	.incbin "baserom.gba", 0x00e5911e, 0x00000002
	.section .unidentified.08e5a657,"a"
	.incbin "baserom.gba", 0x00e5a657, 0x00000001
	.section .unidentified.08e5a7f2,"a"
	.incbin "baserom.gba", 0x00e5a7f2, 0x00000002
	.section .unidentified.08e5ca9e,"a"
	.incbin "baserom.gba", 0x00e5ca9e, 0x00000002
	.section .unidentified.08e5ec65,"a"
	.incbin "baserom.gba", 0x00e5ec65, 0x00000003
	.section .unidentified.08e60363,"a"
	.incbin "baserom.gba", 0x00e60363, 0x00000001
	.section .unidentified.08e60e87,"a"
	.incbin "baserom.gba", 0x00e60e87, 0x00000001
	.section .unidentified.08e6365f,"a"
	.incbin "baserom.gba", 0x00e6365f, 0x00000001
	.section .unidentified.08e6450a,"a"
	.incbin "baserom.gba", 0x00e6450a, 0x00000002
	.section .unidentified.08e65d2a,"a"
	.incbin "baserom.gba", 0x00e65d2a, 0x00000002
	.section .unidentified.08e680d1,"a"
	.incbin "baserom.gba", 0x00e680d1, 0x00000003
	.section .unidentified.08e687ba,"a"
	.incbin "baserom.gba", 0x00e687ba, 0x00000002
	.section .unidentified.08e693a6,"a"
	.incbin "baserom.gba", 0x00e693a6, 0x00000002
	.global Resource_Data5BC
Resource_Data5BC:
	.incbin "baserom.gba", 0x00e693a8, 0x00000ac4
	.section .unidentified.08e6b41b,"a"
	.incbin "baserom.gba", 0x00e6b41b, 0x00000001
	.section .unidentified.08e6b5ce,"a"
	.incbin "baserom.gba", 0x00e6b5ce, 0x00000002
	.section .unidentified.08e6d80d,"a"
	.incbin "baserom.gba", 0x00e6d80d, 0x00000003
	.section .unidentified.08e6e333,"a"
	.incbin "baserom.gba", 0x00e6e333, 0x00000001
	.section .unidentified.08e7103d,"a"
	.incbin "baserom.gba", 0x00e7103d, 0x00000003
	.section .unidentified.08e727c5,"a"
	.incbin "baserom.gba", 0x00e727c5, 0x00000003
	.section .unidentified.08e758bf,"a"
	.incbin "baserom.gba", 0x00e758bf, 0x00000001
	.section .unidentified.08e7698d,"a"
	.incbin "baserom.gba", 0x00e7698d, 0x00000003
	.section .unidentified.08e77f49,"a"
	.incbin "baserom.gba", 0x00e77f49, 0x00000003
	.section .unidentified.08e78dc4,"a"
	.global Resource_Data5C9
Resource_Data5C9:
	.incbin "baserom.gba", 0x00e78dc4, 0x000007d0
	.section .unidentified.08e7a433,"a"
	.incbin "baserom.gba", 0x00e7a433, 0x00000001
	.section .unidentified.08e7b28b,"a"
	.incbin "baserom.gba", 0x00e7b28b, 0x00000001
	.section .unidentified.08e7c25d,"a"
	.incbin "baserom.gba", 0x00e7c25d, 0x00000003
	.section .unidentified.08e7c3d7,"a"
	.incbin "baserom.gba", 0x00e7c3d7, 0x00000001
	.section .unidentified.08e7c54e,"a"
	.incbin "baserom.gba", 0x00e7c54e, 0x00000002
	.section .unidentified.08e7c6e6,"a"
	.incbin "baserom.gba", 0x00e7c6e6, 0x00000002
	.section .unidentified.08e7c877,"a"
	.incbin "baserom.gba", 0x00e7c877, 0x00000001
	.section .unidentified.08e7eb23,"a"
	.incbin "baserom.gba", 0x00e7eb23, 0x00000001
	.section .unidentified.08e7ec3b,"a"
	.incbin "baserom.gba", 0x00e7ec3b, 0x00000001
	.section .unidentified.08e80a3d,"a"
	.incbin "baserom.gba", 0x00e80a3d, 0x00000003
	.section .unidentified.08e82502,"a"
	.incbin "baserom.gba", 0x00e82502, 0x00000002
	.section .unidentified.08e82c2a,"a"
	.incbin "baserom.gba", 0x00e82c2a, 0x00000002
	.section .unidentified.08e84026,"a"
	.incbin "baserom.gba", 0x00e84026, 0x00000002
	.section .unidentified.08e84efa,"a"
	.incbin "baserom.gba", 0x00e84efa, 0x00000002
	.section .unidentified.08e8503e,"a"
	.incbin "baserom.gba", 0x00e8503e, 0x00000002
	.section .unidentified.08e873ef,"a"
	.incbin "baserom.gba", 0x00e873ef, 0x00000001
	.section .unidentified.08e89c1d,"a"
	.incbin "baserom.gba", 0x00e89c1d, 0x00000003
	.section .unidentified.08e8a01a,"a"
	.incbin "baserom.gba", 0x00e8a01a, 0x00000002
	.section .unidentified.08e8b4ba,"a"
	.incbin "baserom.gba", 0x00e8b4ba, 0x00000002
	.section .unidentified.08e8b60e,"a"
	.incbin "baserom.gba", 0x00e8b60e, 0x00000002
	.section .unidentified.08e8ef5b,"a"
	.incbin "baserom.gba", 0x00e8ef5b, 0x00000001
	.section .unidentified.08e903c6,"a"
	.incbin "baserom.gba", 0x00e903c6, 0x00000002
	.section .unidentified.08e9189f,"a"
	.incbin "baserom.gba", 0x00e9189f, 0x00000001
	.section .unidentified.08e919f2,"a"
	.incbin "baserom.gba", 0x00e919f2, 0x00000002
	.section .unidentified.08e953ff,"a"
	.incbin "baserom.gba", 0x00e953ff, 0x00000001
	.section .unidentified.08e965d7,"a"
	.incbin "baserom.gba", 0x00e965d7, 0x00000001
	.section .unidentified.08e97de2,"a"
	.incbin "baserom.gba", 0x00e97de2, 0x00000002
	.section .unidentified.08e97f32,"a"
	.incbin "baserom.gba", 0x00e97f32, 0x00000002
	.section .unidentified.08e991b5,"a"
	.incbin "baserom.gba", 0x00e991b5, 0x00000003
	.section .unidentified.08e9a63f,"a"
	.incbin "baserom.gba", 0x00e9a63f, 0x00000001
	.section .unidentified.08e9a7a6,"a"
	.incbin "baserom.gba", 0x00e9a7a6, 0x00000002
	.section .unidentified.08e9d767,"a"
	.incbin "baserom.gba", 0x00e9d767, 0x00000001
	.section .unidentified.08e9e9d5,"a"
	.incbin "baserom.gba", 0x00e9e9d5, 0x00000003
	.section .unidentified.08ea1562,"a"
	.incbin "baserom.gba", 0x00ea1562, 0x00000002
	.section .unidentified.08ea1f49,"a"
	.incbin "baserom.gba", 0x00ea1f49, 0x00000003
	.section .unidentified.08ea28ea,"a"
	.incbin "baserom.gba", 0x00ea28ea, 0x00000002
	.section .unidentified.08ea2a5e,"a"
	.incbin "baserom.gba", 0x00ea2a5e, 0x00000002
	.section .unidentified.08ea456f,"a"
	.incbin "baserom.gba", 0x00ea456f, 0x00000001
	.section .unidentified.08ea4691,"a"
	.incbin "baserom.gba", 0x00ea4691, 0x00000003
	.section .unidentified.08ea718f,"a"
	.incbin "baserom.gba", 0x00ea718f, 0x00000001
	.section .unidentified.08ea7f86,"a"
	.incbin "baserom.gba", 0x00ea7f86, 0x00000002
	.section .unidentified.08ea9f25,"a"
	.incbin "baserom.gba", 0x00ea9f25, 0x00000003
	.section .unidentified.08eab255,"a"
	.incbin "baserom.gba", 0x00eab255, 0x00000003
	.section .unidentified.08eab3a9,"a"
	.incbin "baserom.gba", 0x00eab3a9, 0x00000003
	.section .unidentified.08eabf5b,"a"
	.incbin "baserom.gba", 0x00eabf5b, 0x00000001
	.section .unidentified.08ead735,"a"
	.incbin "baserom.gba", 0x00ead735, 0x00000003
	.section .unidentified.08eae895,"a"
	.incbin "baserom.gba", 0x00eae895, 0x00000003
	.section .unidentified.08eaf9b3,"a"
	.incbin "baserom.gba", 0x00eaf9b3, 0x00000001
	.section .unidentified.08eafbba,"a"
	.incbin "baserom.gba", 0x00eafbba, 0x00000002
	.section .unidentified.08eb08af,"a"
	.incbin "baserom.gba", 0x00eb08af, 0x00000001
	.section .unidentified.08eb3051,"a"
	.incbin "baserom.gba", 0x00eb3051, 0x00000003
	.section .unidentified.08eb47c6,"a"
	.incbin "baserom.gba", 0x00eb47c6, 0x00000002
	.section .unidentified.08eb5a83,"a"
	.incbin "baserom.gba", 0x00eb5a83, 0x00000001
	.section .unidentified.08eb62c6,"a"
	.incbin "baserom.gba", 0x00eb62c6, 0x00000002
	.global Resource_Data60D
Resource_Data60D:
	.incbin "baserom.gba", 0x00eb62c8, 0x0000060c
	.section .unidentified.08eb6e4b,"a"
	.incbin "baserom.gba", 0x00eb6e4b, 0x00000001
	.section .unidentified.08eb6f8b,"a"
	.incbin "baserom.gba", 0x00eb6f8b, 0x00000001
	.section .unidentified.08eb70cb,"a"
	.incbin "baserom.gba", 0x00eb70cb, 0x00000001
	.section .unidentified.08eb7c2d,"a"
	.incbin "baserom.gba", 0x00eb7c2d, 0x00000003
	.section .unidentified.08eba3d1,"a"
	.incbin "baserom.gba", 0x00eba3d1, 0x00000003
	.section .unidentified.08ebbb46,"a"
	.incbin "baserom.gba", 0x00ebbb46, 0x00000002
	.section .unidentified.08ebce03,"a"
	.incbin "baserom.gba", 0x00ebce03, 0x00000001
	.section .unidentified.08ebd646,"a"
	.incbin "baserom.gba", 0x00ebd646, 0x00000002
	.section .unidentified.08ebdbe5,"a"
	.incbin "baserom.gba", 0x00ebdbe5, 0x00000003
	.global Resource_Data61A
Resource_Data61A:
	.incbin "baserom.gba", 0x00ebdbe8, 0x0000000c
	.global Resource_Data61B
Resource_Data61B:
	.incbin "baserom.gba", 0x00ebdbf4, 0x00000150
	.global Resource_Data61C
Resource_Data61C:
	.incbin "baserom.gba", 0x00ebdd44, 0x00000140
	.global Resource_Data61D
Resource_Data61D:
	.incbin "baserom.gba", 0x00ebde84, 0x00000140
	.global Resource_Data61E
Resource_Data61E:
	.incbin "baserom.gba", 0x00ebdfc4, 0x00000140
	.section .unidentified.08ebe819,"a"
	.incbin "baserom.gba", 0x00ebe819, 0x00000003
	.global Resource_Data620
Resource_Data620:
	.incbin "baserom.gba", 0x00ebe81c, 0x0000000c
	.global Resource_Data621
Resource_Data621:
	.incbin "baserom.gba", 0x00ebe828, 0x00000150
	.global Resource_Data622
Resource_Data622:
	.incbin "baserom.gba", 0x00ebe978, 0x00000140
	.global Resource_Data623
Resource_Data623:
	.incbin "baserom.gba", 0x00ebeab8, 0x00000140
	.global Resource_Data624
Resource_Data624:
	.incbin "baserom.gba", 0x00ebebf8, 0x00000140
	.section .unidentified.08ebf8f9,"a"
	.incbin "baserom.gba", 0x00ebf8f9, 0x00000003
	.global Resource_Data626
Resource_Data626:
	.incbin "baserom.gba", 0x00ebf8fc, 0x0000000c
	.global Resource_Data627
Resource_Data627:
	.incbin "baserom.gba", 0x00ebf908, 0x00000150
	.global Resource_Data628
Resource_Data628:
	.incbin "baserom.gba", 0x00ebfa58, 0x00000140
	.global Resource_Data629
Resource_Data629:
	.incbin "baserom.gba", 0x00ebfb98, 0x00000140
	.global Resource_Data62A
Resource_Data62A:
	.incbin "baserom.gba", 0x00ebfcd8, 0x00000140
	.section .unidentified.08ec03e5,"a"
	.incbin "baserom.gba", 0x00ec03e5, 0x00000003
	.global Resource_Data62C
Resource_Data62C:
	.incbin "baserom.gba", 0x00ec03e8, 0x0000000c
	.global Resource_Data62D
Resource_Data62D:
	.incbin "baserom.gba", 0x00ec03f4, 0x00000150
	.global Resource_Data62E
Resource_Data62E:
	.incbin "baserom.gba", 0x00ec0544, 0x00000140
	.global Resource_Data62F
Resource_Data62F:
	.incbin "baserom.gba", 0x00ec0684, 0x00000140
	.global Resource_Data630
Resource_Data630:
	.incbin "baserom.gba", 0x00ec07c4, 0x00000140
	.section .unidentified.08ec1281,"a"
	.incbin "baserom.gba", 0x00ec1281, 0x00000003
	.global Resource_Data632
Resource_Data632:
	.incbin "baserom.gba", 0x00ec1284, 0x0000000c
	.global Resource_Data633
Resource_Data633:
	.incbin "baserom.gba", 0x00ec1290, 0x00000150
	.global Resource_Data634
Resource_Data634:
	.incbin "baserom.gba", 0x00ec13e0, 0x00000140
	.global Resource_Data635
Resource_Data635:
	.incbin "baserom.gba", 0x00ec1520, 0x00000140
	.global Resource_Data636
Resource_Data636:
	.incbin "baserom.gba", 0x00ec1660, 0x00000140
	.section .unidentified.08ec1bc9,"a"
	.incbin "baserom.gba", 0x00ec1bc9, 0x00000003
	.global Resource_Data638
Resource_Data638:
	.incbin "baserom.gba", 0x00ec1bcc, 0x0000000c
	.global Resource_Data639
Resource_Data639:
	.incbin "baserom.gba", 0x00ec1bd8, 0x00000150
	.global Resource_Data63A
Resource_Data63A:
	.incbin "baserom.gba", 0x00ec1d28, 0x00000140
	.global Resource_Data63B
Resource_Data63B:
	.incbin "baserom.gba", 0x00ec1e68, 0x00000140
	.global Resource_Data63C
Resource_Data63C:
	.incbin "baserom.gba", 0x00ec1fa8, 0x00000140
	.section .unidentified.08ec2711,"a"
	.incbin "baserom.gba", 0x00ec2711, 0x00000003
	.global Resource_Data63E
Resource_Data63E:
	.incbin "baserom.gba", 0x00ec2714, 0x0000000c
	.global Resource_Data63F
Resource_Data63F:
	.incbin "baserom.gba", 0x00ec2720, 0x00000150
	.global Resource_Data640
Resource_Data640:
	.incbin "baserom.gba", 0x00ec2870, 0x00000140
	.global Resource_Data641
Resource_Data641:
	.incbin "baserom.gba", 0x00ec29b0, 0x00000140
	.global Resource_Data642
Resource_Data642:
	.incbin "baserom.gba", 0x00ec2af0, 0x00000140
	.section .unidentified.08ec3359,"a"
	.incbin "baserom.gba", 0x00ec3359, 0x00000003
	.global Resource_Data644
Resource_Data644:
	.incbin "baserom.gba", 0x00ec335c, 0x0000000c
	.global Resource_Data645
Resource_Data645:
	.incbin "baserom.gba", 0x00ec3368, 0x00000150
	.global Resource_Data646
Resource_Data646:
	.incbin "baserom.gba", 0x00ec34b8, 0x00000140
	.global Resource_Data647
Resource_Data647:
	.incbin "baserom.gba", 0x00ec35f8, 0x00000140
	.global Resource_Data648
Resource_Data648:
	.incbin "baserom.gba", 0x00ec3738, 0x00000140
	.section .unidentified.08f79646,"a"
	.incbin "baserom.gba", 0x00f79646, 0x000869ba
