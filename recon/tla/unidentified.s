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
	.global Resource_Data027
Resource_Data027:
	.incbin "baserom.gba", 0x006ce7d4, 0x000029f0
	.global Resource_Data028
Resource_Data028:
	.incbin "baserom.gba", 0x006d11c4, 0x00004444
	.global Resource_Data029
Resource_Data029:
	.incbin "baserom.gba", 0x006d5608, 0x00004700
	.global Resource_Data02A
Resource_Data02A:
	.incbin "baserom.gba", 0x006d9d08, 0x0000411c
	.global Resource_Data02B
Resource_Data02B:
	.incbin "baserom.gba", 0x006dde24, 0x00004218
	.global Resource_Data02C
Resource_Data02C:
	.incbin "baserom.gba", 0x006e203c, 0x00003d3c
	.global Resource_Data02D
Resource_Data02D:
	.incbin "baserom.gba", 0x006e5d78, 0x000035f0
	.global Resource_Data02E
Resource_Data02E:
	.incbin "baserom.gba", 0x006e9368, 0x00003e58
	.global Resource_Data02F
Resource_Data02F:
	.incbin "baserom.gba", 0x006ed1c0, 0x00003b74
	.global Resource_Data030
Resource_Data030:
	.incbin "baserom.gba", 0x006f0d34, 0x000040d8
	.global Resource_Data031
Resource_Data031:
	.incbin "baserom.gba", 0x006f4e0c, 0x00003f7c
	.global Resource_Data032
Resource_Data032:
	.incbin "baserom.gba", 0x006f8d88, 0x00004400
	.global Resource_Data033
Resource_Data033:
	.incbin "baserom.gba", 0x006fd188, 0x0000400c
	.global Resource_Data034
Resource_Data034:
	.incbin "baserom.gba", 0x00701194, 0x00004328
	.global Resource_Data035
Resource_Data035:
	.incbin "baserom.gba", 0x007054bc, 0x00003a28
	.global Resource_Data036
Resource_Data036:
	.incbin "baserom.gba", 0x00708ee4, 0x00004078
	.global Resource_Data037
Resource_Data037:
	.incbin "baserom.gba", 0x0070cf5c, 0x00003840
	.global Resource_Data038
Resource_Data038:
	.incbin "baserom.gba", 0x0071079c, 0x00004a38
	.global Resource_Data039
Resource_Data039:
	.incbin "baserom.gba", 0x007151d4, 0x000049fc
	.global Resource_Data03A
Resource_Data03A:
	.incbin "baserom.gba", 0x00719bd0, 0x00003660
	.global Resource_Data03B
Resource_Data03B:
	.incbin "baserom.gba", 0x0071d230, 0x00003b4c
	.global Resource_Data03C
Resource_Data03C:
	.incbin "baserom.gba", 0x00720d7c, 0x00003e88
	.global Resource_Data03D
Resource_Data03D:
	.incbin "baserom.gba", 0x00724c04, 0x00004034
	.global Resource_Data03E
Resource_Data03E:
	.incbin "baserom.gba", 0x00728c38, 0x00004a90
	.global Resource_Data03F
Resource_Data03F:
	.incbin "baserom.gba", 0x0072d6c8, 0x000042b4
	.global Resource_Data040
Resource_Data040:
	.incbin "baserom.gba", 0x0073197c, 0x00003980
	.global Resource_Data041
Resource_Data041:
	.incbin "baserom.gba", 0x007352fc, 0x00004210
	.global Resource_Data042
Resource_Data042:
	.incbin "baserom.gba", 0x0073950c, 0x00004374
	.global Resource_Data043
Resource_Data043:
	.incbin "baserom.gba", 0x0073d880, 0x000042b8
	.global Resource_Data044
Resource_Data044:
	.incbin "baserom.gba", 0x00741b38, 0x0000471c
	.global Resource_Data045
Resource_Data045:
	.incbin "baserom.gba", 0x00746254, 0x00003484
	.global Resource_Data046
Resource_Data046:
	.incbin "baserom.gba", 0x007496d8, 0x00003ba0
	.global Resource_Data047
Resource_Data047:
	.incbin "baserom.gba", 0x0074d278, 0x000033d0
	.global Resource_Data048
Resource_Data048:
	.incbin "baserom.gba", 0x00750648, 0x000053e4
	.global Resource_Data049
Resource_Data049:
	.incbin "baserom.gba", 0x00755a2c, 0x000041fc
	.global Resource_Data04A
Resource_Data04A:
	.incbin "baserom.gba", 0x00759c28, 0x000042d4
	.global Resource_Data04B
Resource_Data04B:
	.incbin "baserom.gba", 0x0075defc, 0x00004668
	.global Resource_Data04C
Resource_Data04C:
	.incbin "baserom.gba", 0x00762564, 0x00003a68
	.global Resource_Data04D
Resource_Data04D:
	.incbin "baserom.gba", 0x00765fcc, 0x000039f0
	.global Resource_Data04E
Resource_Data04E:
	.incbin "baserom.gba", 0x007699bc, 0x000042d8
	.global Resource_Data04F
Resource_Data04F:
	.incbin "baserom.gba", 0x0076dc94, 0x00004434
	.global Resource_Data050
Resource_Data050:
	.incbin "baserom.gba", 0x007720c8, 0x00004510
	.global Resource_Data051
Resource_Data051:
	.incbin "baserom.gba", 0x007765d8, 0x0000394c
	.global Resource_Data052
Resource_Data052:
	.incbin "baserom.gba", 0x00779f24, 0x00003f30
	.global Resource_Data053
Resource_Data053:
	.incbin "baserom.gba", 0x0077de54, 0x00003a24
	.global Resource_Data054
Resource_Data054:
	.incbin "baserom.gba", 0x00781878, 0x00004120
	.global Resource_Data055
Resource_Data055:
	.incbin "baserom.gba", 0x00785998, 0x00004200
	.global Resource_Data056
Resource_Data056:
	.incbin "baserom.gba", 0x00789b98, 0x000041e4
	.global Resource_Data057
Resource_Data057:
	.incbin "baserom.gba", 0x0078dd7c, 0x00003f88
	.global Resource_Data058
Resource_Data058:
	.incbin "baserom.gba", 0x00791d04, 0x0000484c
	.global Resource_Data059
Resource_Data059:
	.incbin "baserom.gba", 0x00796550, 0x000042b8
	.global Resource_Data05A
Resource_Data05A:
	.incbin "baserom.gba", 0x0079a808, 0x000047c8
	.global Resource_Data05B
Resource_Data05B:
	.incbin "baserom.gba", 0x0079efd0, 0x00003ebc
	.global Resource_Data05C
Resource_Data05C:
	.incbin "baserom.gba", 0x007a2e8c, 0x00003dfc
	.global Resource_Data05D
Resource_Data05D:
	.incbin "baserom.gba", 0x007a6c88, 0x000044d0
	.global Resource_Data05E
Resource_Data05E:
	.incbin "baserom.gba", 0x007ab158, 0x00004280
	.global Resource_Data05F
Resource_Data05F:
	.incbin "baserom.gba", 0x007af3d8, 0x0000375c
	.global Resource_Data060
Resource_Data060:
	.incbin "baserom.gba", 0x007b2b34, 0x0000462c
	.global Resource_Data061
Resource_Data061:
	.incbin "baserom.gba", 0x007b7160, 0x00003fc8
	.global Resource_Data062
Resource_Data062:
	.incbin "baserom.gba", 0x007bb128, 0x00003cb4
	.global Resource_Data063
Resource_Data063:
	.incbin "baserom.gba", 0x007beddc, 0x00003bfc
	.global Resource_Data064
Resource_Data064:
	.incbin "baserom.gba", 0x007c29d8, 0x000039b0
	.global Resource_Data065
Resource_Data065:
	.incbin "baserom.gba", 0x007c6388, 0x00004450
	.global Resource_Data066
Resource_Data066:
	.incbin "baserom.gba", 0x007ca7d8, 0x00003bf8
	.global Resource_Data067
Resource_Data067:
	.incbin "baserom.gba", 0x007ce3d0, 0x00003fac
	.global Resource_Data068
Resource_Data068:
	.incbin "baserom.gba", 0x007d237c, 0x00003f24
	.global Resource_Data069
Resource_Data069:
	.incbin "baserom.gba", 0x007d62a0, 0x00003d90
	.global Resource_Data06A
Resource_Data06A:
	.incbin "baserom.gba", 0x007da030, 0x00003c50
	.global Resource_Data06B
Resource_Data06B:
	.incbin "baserom.gba", 0x007ddc80, 0x000042f4
	.global Resource_Data06C
Resource_Data06C:
	.incbin "baserom.gba", 0x007e1f74, 0x0000428c
	.global Resource_Data06D
Resource_Data06D:
	.incbin "baserom.gba", 0x007e6200, 0x00004168
	.global Resource_Data06E
Resource_Data06E:
	.incbin "baserom.gba", 0x007ea368, 0x0000414c
	.global Resource_Data06F
Resource_Data06F:
	.incbin "baserom.gba", 0x007ee4b4, 0x0000439c
	.global Resource_Data070
Resource_Data070:
	.incbin "baserom.gba", 0x007f2850, 0x00003b58
	.global Resource_Data071
Resource_Data071:
	.incbin "baserom.gba", 0x007f63a8, 0x00002f5c
	.global Resource_Data072
Resource_Data072:
	.incbin "baserom.gba", 0x007f9304, 0x00002520
	.global Resource_Data073
Resource_Data073:
	.incbin "baserom.gba", 0x007fb824, 0x000033fc
	.global Resource_Data074
Resource_Data074:
	.incbin "baserom.gba", 0x007fec20, 0x000039ac
	.global Resource_Data075
Resource_Data075:
	.incbin "baserom.gba", 0x008025cc, 0x00005180
	.global Resource_Data076
Resource_Data076:
	.incbin "baserom.gba", 0x0080774c, 0x000028a4
	.global Resource_Data077
Resource_Data077:
	.incbin "baserom.gba", 0x00809ff0, 0x00003d18
	.global Resource_Data078
Resource_Data078:
	.incbin "baserom.gba", 0x0080dd08, 0x000024b8
	.global Resource_Data079
Resource_Data079:
	.incbin "baserom.gba", 0x008101c0, 0x00007c08
	.global Resource_Data07A
Resource_Data07A:
	.incbin "baserom.gba", 0x00817dc8, 0x00007ea8
	.global Resource_Data07B
Resource_Data07B:
	.incbin "baserom.gba", 0x0081fc70, 0x00009444
	.global Resource_Data07C
Resource_Data07C:
	.incbin "baserom.gba", 0x008290b4, 0x0000921c
	.global Resource_Data07D
Resource_Data07D:
	.incbin "baserom.gba", 0x008322d0, 0x000060e4
	.global Resource_Data07E
Resource_Data07E:
	.incbin "baserom.gba", 0x008383b4, 0x00007844
	.global Resource_Data07F
Resource_Data07F:
	.incbin "baserom.gba", 0x0083fbf8, 0x00005e54
	.global Resource_Data080
Resource_Data080:
	.incbin "baserom.gba", 0x00845a4c, 0x00005d20
	.global Resource_Data081
Resource_Data081:
	.incbin "baserom.gba", 0x0084b76c, 0x00000784
	.global Resource_Data082
Resource_Data082:
	.incbin "baserom.gba", 0x0084bef0, 0x00001334
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
	.global Resource_Data0B6
Resource_Data0B6:
	.incbin "baserom.gba", 0x0087ddcc, 0x00000638
	.global Resource_Data0B7
Resource_Data0B7:
	.incbin "baserom.gba", 0x0087e404, 0x0000216c
	.global Resource_Data0B8
Resource_Data0B8:
	.incbin "baserom.gba", 0x00880570, 0x00000084
	.global Resource_Data0B9
Resource_Data0B9:
	.incbin "baserom.gba", 0x008805f4, 0x00000084
	.global Resource_Data0BA
Resource_Data0BA:
	.incbin "baserom.gba", 0x00880678, 0x0000024c
	.global Resource_Data0BB
Resource_Data0BB:
	.incbin "baserom.gba", 0x008808c4, 0x00000184
	.global Resource_Data0BC
Resource_Data0BC:
	.incbin "baserom.gba", 0x00880a48, 0x000008bc
	.global Resource_Data0BD
Resource_Data0BD:
	.incbin "baserom.gba", 0x00881304, 0x00001be4
	.global Resource_Data0BE
Resource_Data0BE:
	.incbin "baserom.gba", 0x00882ee8, 0x00001b5c
	.global Resource_Data0BF
Resource_Data0BF:
	.incbin "baserom.gba", 0x00884a44, 0x00000440
	.global Resource_Data0C0
Resource_Data0C0:
	.incbin "baserom.gba", 0x00884e84, 0x00000414
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
	.global Resource_Data0C5
Resource_Data0C5:
	.incbin "baserom.gba", 0x00886ad8, 0x0000250c
	.global Resource_Data0C6
Resource_Data0C6:
	.incbin "baserom.gba", 0x00888fe4, 0x000009bc
	.global Resource_Data0C7
Resource_Data0C7:
	.incbin "baserom.gba", 0x008899a0, 0x00001144
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
	.global Resource_Data0CC
Resource_Data0CC:
	.incbin "baserom.gba", 0x0088bf78, 0x0000157c
	.global Resource_Data0CD
Resource_Data0CD:
	.incbin "baserom.gba", 0x0088d4f4, 0x0000032c
	.global Resource_Data0CE
Resource_Data0CE:
	.incbin "baserom.gba", 0x0088d820, 0x00000528
	.global Resource_Data0CF
Resource_Data0CF:
	.incbin "baserom.gba", 0x0088dd48, 0x00000530
	.global Resource_Data0D0
Resource_Data0D0:
	.incbin "baserom.gba", 0x0088e278, 0x000005f0
	.global Resource_Data0D1
Resource_Data0D1:
	.incbin "baserom.gba", 0x0088e868, 0x0000078c
	.global Resource_Data0D2
Resource_Data0D2:
	.incbin "baserom.gba", 0x0088eff4, 0x00000200
	.global Resource_Data0D3
Resource_Data0D3:
	.incbin "baserom.gba", 0x0088f1f4, 0x00000420
	.global Resource_Data0D4
Resource_Data0D4:
	.incbin "baserom.gba", 0x0088f614, 0x0000062c
	.global Resource_Data0D5
Resource_Data0D5:
	.incbin "baserom.gba", 0x0088fc40, 0x000001fc
	.global Resource_Data0D6
Resource_Data0D6:
	.incbin "baserom.gba", 0x0088fe3c, 0x0000020c
	.global Resource_Data0D7
Resource_Data0D7:
	.incbin "baserom.gba", 0x00890048, 0x0000025c
	.global Resource_Data0D8
Resource_Data0D8:
	.incbin "baserom.gba", 0x008902a4, 0x0000057c
	.global Resource_Data0D9
Resource_Data0D9:
	.incbin "baserom.gba", 0x00890820, 0x000002ac
	.global Resource_Data0DA
Resource_Data0DA:
	.incbin "baserom.gba", 0x00890acc, 0x0000095c
	.global Resource_Data0DB
Resource_Data0DB:
	.incbin "baserom.gba", 0x00891428, 0x000006c0
	.global Resource_Data0DC
Resource_Data0DC:
	.incbin "baserom.gba", 0x00891ae8, 0x00002ab0
	.global Resource_Data0DD
Resource_Data0DD:
	.incbin "baserom.gba", 0x00894598, 0x000011cc
	.global Resource_Data0DE
Resource_Data0DE:
	.incbin "baserom.gba", 0x00895764, 0x00000660
	.global Resource_Data0DF
Resource_Data0DF:
	.incbin "baserom.gba", 0x00895dc4, 0x0000103c
	.global Resource_Data0E0
Resource_Data0E0:
	.incbin "baserom.gba", 0x00896e00, 0x00000654
	.global Resource_Data0E1
Resource_Data0E1:
	.incbin "baserom.gba", 0x00897454, 0x00000680
	.global Resource_Data0E2
Resource_Data0E2:
	.incbin "baserom.gba", 0x00897ad4, 0x00000620
	.global Resource_Data0E3
Resource_Data0E3:
	.incbin "baserom.gba", 0x008980f4, 0x00000ce4
	.global Resource_Data0E4
Resource_Data0E4:
	.incbin "baserom.gba", 0x00898dd8, 0x000006e8
	.global Resource_Data0E5
Resource_Data0E5:
	.incbin "baserom.gba", 0x008994c0, 0x00000624
	.global Resource_Data0E6
Resource_Data0E6:
	.incbin "baserom.gba", 0x00899ae4, 0x00000a20
	.global Resource_Data0E7
Resource_Data0E7:
	.incbin "baserom.gba", 0x0089a504, 0x000003d8
	.global Resource_Data0E8
Resource_Data0E8:
	.incbin "baserom.gba", 0x0089a8dc, 0x000006b0
	.global Resource_Data0E9
Resource_Data0E9:
	.incbin "baserom.gba", 0x0089af8c, 0x000002cc
	.global Resource_Data0EA
Resource_Data0EA:
	.incbin "baserom.gba", 0x0089b258, 0x00000d3c
	.global Resource_Data0EB
Resource_Data0EB:
	.incbin "baserom.gba", 0x0089bf94, 0x000008b4
	.global Resource_Data0EC
Resource_Data0EC:
	.incbin "baserom.gba", 0x0089c848, 0x00000988
	.global Resource_Data0ED
Resource_Data0ED:
	.incbin "baserom.gba", 0x0089d1d0, 0x00000948
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
	.global Resource_Data0F4
Resource_Data0F4:
	.incbin "baserom.gba", 0x008a4720, 0x00000534
	.global Resource_Data0F5
Resource_Data0F5:
	.incbin "baserom.gba", 0x008a4c54, 0x000011d8
	.global Resource_Data0F6
Resource_Data0F6:
	.incbin "baserom.gba", 0x008a5e2c, 0x000004f0
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
	.global Resource_Data0FE
Resource_Data0FE:
	.incbin "baserom.gba", 0x008a8b18, 0x00000644
	.global Resource_Data0FF
Resource_Data0FF:
	.incbin "baserom.gba", 0x008a915c, 0x000000a8
	.global Resource_Data100
Resource_Data100:
	.incbin "baserom.gba", 0x008a9204, 0x00000318
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
	.global Resource_Data105
Resource_Data105:
	.incbin "baserom.gba", 0x008aadec, 0x00000c74
	.global Resource_Data106
Resource_Data106:
	.incbin "baserom.gba", 0x008aba60, 0x00000848
	.global Resource_Data107
Resource_Data107:
	.incbin "baserom.gba", 0x008ac2a8, 0x00001200
	.global Resource_Data108
Resource_Data108:
	.incbin "baserom.gba", 0x008ad4a8, 0x00000084
	.global Resource_Data109
Resource_Data109:
	.incbin "baserom.gba", 0x008ad52c, 0x00000084
	.global Resource_Data10A
Resource_Data10A:
	.incbin "baserom.gba", 0x008ad5b0, 0x00000084
	.global Resource_Data10B
Resource_Data10B:
	.incbin "baserom.gba", 0x008ad634, 0x00000d34
	.global Resource_Data10C
Resource_Data10C:
	.incbin "baserom.gba", 0x008ae368, 0x00000adc
	.global Resource_Data10D
Resource_Data10D:
	.incbin "baserom.gba", 0x008aee44, 0x000006c4
	.global Resource_Data10E
Resource_Data10E:
	.incbin "baserom.gba", 0x008af508, 0x000002e8
	.global Resource_Data10F
Resource_Data10F:
	.incbin "baserom.gba", 0x008af7f0, 0x0000057c
	.global Resource_Data110
Resource_Data110:
	.incbin "baserom.gba", 0x008afd6c, 0x000006e8
	.global Resource_Data111
Resource_Data111:
	.incbin "baserom.gba", 0x008b0454, 0x00000700
	.global Resource_Data112
Resource_Data112:
	.incbin "baserom.gba", 0x008b0b54, 0x000003ec
	.global Resource_Data113
Resource_Data113:
	.incbin "baserom.gba", 0x008b0f40, 0x000003d0
	.global Resource_Data114
Resource_Data114:
	.incbin "baserom.gba", 0x008b1310, 0x000008a0
	.global Resource_Data115
Resource_Data115:
	.incbin "baserom.gba", 0x008b1bb0, 0x000004a4
	.global Resource_Data116
Resource_Data116:
	.incbin "baserom.gba", 0x008b2054, 0x00000534
	.global Resource_Data117
Resource_Data117:
	.incbin "baserom.gba", 0x008b2588, 0x00000c84
	.global Resource_Data118
Resource_Data118:
	.incbin "baserom.gba", 0x008b320c, 0x00000458
	.global Resource_Data119
Resource_Data119:
	.incbin "baserom.gba", 0x008b3664, 0x0000106c
	.global Resource_Data11A
Resource_Data11A:
	.incbin "baserom.gba", 0x008b46d0, 0x00000fc8
	.global Resource_Data11B
Resource_Data11B:
	.incbin "baserom.gba", 0x008b5698, 0x0000025c
	.global Resource_Data11C
Resource_Data11C:
	.incbin "baserom.gba", 0x008b58f4, 0x0000175c
	.global Resource_Data11D
Resource_Data11D:
	.incbin "baserom.gba", 0x008b7050, 0x00000300
	.global Resource_Data11E
Resource_Data11E:
	.incbin "baserom.gba", 0x008b7350, 0x000001b4
	.global Resource_Data11F
Resource_Data11F:
	.incbin "baserom.gba", 0x008b7504, 0x00000398
	.global Resource_Data120
Resource_Data120:
	.incbin "baserom.gba", 0x008b789c, 0x00001d88
	.global Resource_Data121
Resource_Data121:
	.incbin "baserom.gba", 0x008b9624, 0x00000278
	.global Resource_Data122
Resource_Data122:
	.incbin "baserom.gba", 0x008b989c, 0x00000084
	.global Resource_Data123
Resource_Data123:
	.incbin "baserom.gba", 0x008b9920, 0x00000444
	.global Resource_Data124
Resource_Data124:
	.incbin "baserom.gba", 0x008b9d64, 0x00001bd4
	.global Resource_Data125
Resource_Data125:
	.incbin "baserom.gba", 0x008bb938, 0x00001c00
	.global Resource_Data126
Resource_Data126:
	.incbin "baserom.gba", 0x008bd538, 0x00000220
	.global Resource_Data127
Resource_Data127:
	.incbin "baserom.gba", 0x008bd758, 0x0000043c
	.global Resource_Data128
Resource_Data128:
	.incbin "baserom.gba", 0x008bdb94, 0x00000114
	.global Resource_Data129
Resource_Data129:
	.incbin "baserom.gba", 0x008bdca8, 0x000005e0
	.global Resource_Data12A
Resource_Data12A:
	.incbin "baserom.gba", 0x008be288, 0x000004a0
	.global Resource_Data12B
Resource_Data12B:
	.incbin "baserom.gba", 0x008be728, 0x00000d14
	.global Resource_Data12C
Resource_Data12C:
	.incbin "baserom.gba", 0x008bf43c, 0x00000540
	.global Resource_Data12D
Resource_Data12D:
	.incbin "baserom.gba", 0x008bf97c, 0x00000840
	.global Resource_Data12E
Resource_Data12E:
	.incbin "baserom.gba", 0x008c01bc, 0x00000390
	.global Resource_Data12F
Resource_Data12F:
	.incbin "baserom.gba", 0x008c054c, 0x00001258
	.global Resource_Data130
Resource_Data130:
	.incbin "baserom.gba", 0x008c17a4, 0x00000af0
	.global Resource_Data131
Resource_Data131:
	.incbin "baserom.gba", 0x008c2294, 0x00000dd8
	.global Resource_Data132
Resource_Data132:
	.incbin "baserom.gba", 0x008c306c, 0x0000082c
	.global Resource_Data133
Resource_Data133:
	.incbin "baserom.gba", 0x008c3898, 0x000003f0
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
	.global Resource_Data138
Resource_Data138:
	.incbin "baserom.gba", 0x008c4d58, 0x00000084
	.global Resource_Data139
Resource_Data139:
	.incbin "baserom.gba", 0x008c4ddc, 0x00000514
	.global Resource_Data13A
Resource_Data13A:
	.incbin "baserom.gba", 0x008c52f0, 0x000003dc
	.global Resource_Data13B
Resource_Data13B:
	.incbin "baserom.gba", 0x008c56cc, 0x000003ac
	.global Resource_Data13C
Resource_Data13C:
	.incbin "baserom.gba", 0x008c5a78, 0x00000ad4
	.global Resource_Data13D
Resource_Data13D:
	.incbin "baserom.gba", 0x008c654c, 0x00000084
	.global Resource_Data13E
Resource_Data13E:
	.incbin "baserom.gba", 0x008c65d0, 0x0000144c
	.global Resource_Data13F
Resource_Data13F:
	.incbin "baserom.gba", 0x008c7a1c, 0x00000d9c
	.global Resource_Data140
Resource_Data140:
	.incbin "baserom.gba", 0x008c87b8, 0x0000021c
	.global Resource_Data141
Resource_Data141:
	.incbin "baserom.gba", 0x008c89d4, 0x000002fc
	.global Resource_Data142
Resource_Data142:
	.incbin "baserom.gba", 0x008c8cd0, 0x000003a0
	.global Resource_Data143
Resource_Data143:
	.incbin "baserom.gba", 0x008c9070, 0x000016b0
	.global Resource_Data144
Resource_Data144:
	.incbin "baserom.gba", 0x008ca720, 0x00000760
	.global Resource_Data145
Resource_Data145:
	.incbin "baserom.gba", 0x008cae80, 0x00000dcc
	.global Resource_Data146
Resource_Data146:
	.incbin "baserom.gba", 0x008cbc4c, 0x00000c74
	.global Resource_Data147
Resource_Data147:
	.incbin "baserom.gba", 0x008cc8c0, 0x00000084
	.global Resource_Data148
Resource_Data148:
	.incbin "baserom.gba", 0x008cc944, 0x00000084
	.global Resource_Data149
Resource_Data149:
	.incbin "baserom.gba", 0x008cc9c8, 0x00000084
	.global Resource_Data14A
Resource_Data14A:
	.incbin "baserom.gba", 0x008cca4c, 0x00000084
	.global Resource_Data14B
Resource_Data14B:
	.incbin "baserom.gba", 0x008ccad0, 0x000003e8
	.global Resource_Data14C
Resource_Data14C:
	.incbin "baserom.gba", 0x008cceb8, 0x00000558
	.global Resource_Data14D
Resource_Data14D:
	.incbin "baserom.gba", 0x008cd410, 0x00000c18
	.global Resource_Data14E
Resource_Data14E:
	.incbin "baserom.gba", 0x008ce028, 0x00000cd4
	.global Resource_Data14F
Resource_Data14F:
	.incbin "baserom.gba", 0x008cecfc, 0x000008a0
	.global Resource_Data150
Resource_Data150:
	.incbin "baserom.gba", 0x008cf59c, 0x000003dc
	.global Resource_Data151
Resource_Data151:
	.incbin "baserom.gba", 0x008cf978, 0x0000028c
	.global Resource_Data152
Resource_Data152:
	.incbin "baserom.gba", 0x008cfc04, 0x000003a4
	.global Resource_Data153
Resource_Data153:
	.incbin "baserom.gba", 0x008cffa8, 0x0000025c
	.global Resource_Data154
Resource_Data154:
	.incbin "baserom.gba", 0x008d0204, 0x000003b8
	.global Resource_Data155
Resource_Data155:
	.incbin "baserom.gba", 0x008d05bc, 0x0000026c
	.global Resource_Data156
Resource_Data156:
	.incbin "baserom.gba", 0x008d0828, 0x00000440
	.global Resource_Data157
Resource_Data157:
	.incbin "baserom.gba", 0x008d0c68, 0x000002b4
	.global Resource_Data158
Resource_Data158:
	.incbin "baserom.gba", 0x008d0f1c, 0x00000b88
	.global Resource_Data159
Resource_Data159:
	.incbin "baserom.gba", 0x008d1aa4, 0x000015c4
	.global Resource_Data15A
Resource_Data15A:
	.incbin "baserom.gba", 0x008d3068, 0x00000a6c
	.global Resource_Data15B
Resource_Data15B:
	.incbin "baserom.gba", 0x008d3ad4, 0x000003c4
	.global Resource_Data15C
Resource_Data15C:
	.incbin "baserom.gba", 0x008d3e98, 0x00000a14
	.global Resource_Data15D
Resource_Data15D:
	.incbin "baserom.gba", 0x008d48ac, 0x00000384
	.global Resource_Data15E
Resource_Data15E:
	.incbin "baserom.gba", 0x008d4c30, 0x00000cb0
	.global Resource_Data15F
Resource_Data15F:
	.incbin "baserom.gba", 0x008d58e0, 0x00000b48
	.global Resource_Data160
Resource_Data160:
	.incbin "baserom.gba", 0x008d6428, 0x00000198
	.global Resource_Data161
Resource_Data161:
	.incbin "baserom.gba", 0x008d65c0, 0x0000088c
	.global Resource_Data162
Resource_Data162:
	.incbin "baserom.gba", 0x008d6e4c, 0x00000084
	.global Resource_Data163
Resource_Data163:
	.incbin "baserom.gba", 0x008d6ed0, 0x00000084
	.global Resource_Data164
Resource_Data164:
	.incbin "baserom.gba", 0x008d6f54, 0x00000084
	.global Resource_Data165
Resource_Data165:
	.incbin "baserom.gba", 0x008d6fd8, 0x00000084
	.global Resource_Data166
Resource_Data166:
	.incbin "baserom.gba", 0x008d705c, 0x00000084
	.global Resource_Data167
Resource_Data167:
	.incbin "baserom.gba", 0x008d70e0, 0x00000084
	.global Resource_Data168
Resource_Data168:
	.incbin "baserom.gba", 0x008d7164, 0x00000084
	.global Resource_Data169
Resource_Data169:
	.incbin "baserom.gba", 0x008d71e8, 0x00000084
	.global Resource_Data16A
Resource_Data16A:
	.incbin "baserom.gba", 0x008d726c, 0x000007b0
	.global Resource_Data16B
Resource_Data16B:
	.incbin "baserom.gba", 0x008d7a1c, 0x00000518
	.global Resource_Data16C
Resource_Data16C:
	.incbin "baserom.gba", 0x008d7f34, 0x00000624
	.global Resource_Data16D
Resource_Data16D:
	.incbin "baserom.gba", 0x008d8558, 0x00000424
	.global Resource_Data16E
Resource_Data16E:
	.incbin "baserom.gba", 0x008d897c, 0x00000298
	.global Resource_Data16F
Resource_Data16F:
	.incbin "baserom.gba", 0x008d8c14, 0x00001500
	.global Resource_Data170
Resource_Data170:
	.incbin "baserom.gba", 0x008da114, 0x0000049c
	.global Resource_Data171
Resource_Data171:
	.incbin "baserom.gba", 0x008da5b0, 0x0000198c
	.global Resource_Data172
Resource_Data172:
	.incbin "baserom.gba", 0x008dbf3c, 0x000003ec
	.global Resource_Data173
Resource_Data173:
	.incbin "baserom.gba", 0x008dc328, 0x000012e8
	.global Resource_Data174
Resource_Data174:
	.incbin "baserom.gba", 0x008dd610, 0x00000c8c
	.global Resource_Data175
Resource_Data175:
	.incbin "baserom.gba", 0x008de29c, 0x000004d0
	.global Resource_Data176
Resource_Data176:
	.incbin "baserom.gba", 0x008de76c, 0x00000694
	.global Resource_Data177
Resource_Data177:
	.incbin "baserom.gba", 0x008dee00, 0x00000a34
	.global Resource_Data178
Resource_Data178:
	.incbin "baserom.gba", 0x008df834, 0x00000bfc
	.global Resource_Data179
Resource_Data179:
	.incbin "baserom.gba", 0x008e0430, 0x000007d8
	.global Resource_Data17A
Resource_Data17A:
	.incbin "baserom.gba", 0x008e0c08, 0x00000ad0
	.global Resource_Data17B
Resource_Data17B:
	.incbin "baserom.gba", 0x008e16d8, 0x00000b3c
	.global Resource_Data17C
Resource_Data17C:
	.incbin "baserom.gba", 0x008e2214, 0x00000640
	.global Resource_Data17D
Resource_Data17D:
	.incbin "baserom.gba", 0x008e2854, 0x00001588
	.global Resource_Data17E
Resource_Data17E:
	.incbin "baserom.gba", 0x008e3ddc, 0x00000064
	.global Resource_Data17F
Resource_Data17F:
	.incbin "baserom.gba", 0x008e3e40, 0x0000036c
	.global Resource_Data180
Resource_Data180:
	.incbin "baserom.gba", 0x008e41ac, 0x000005a0
	.global Resource_Data181
Resource_Data181:
	.incbin "baserom.gba", 0x008e474c, 0x00000204
	.global Resource_Data182
Resource_Data182:
	.incbin "baserom.gba", 0x008e4950, 0x0000033c
	.global Resource_Data183
Resource_Data183:
	.incbin "baserom.gba", 0x008e4c8c, 0x00001018
	.global Resource_Data184
Resource_Data184:
	.incbin "baserom.gba", 0x008e5ca4, 0x0000166c
	.global Resource_Data185
Resource_Data185:
	.incbin "baserom.gba", 0x008e7310, 0x00000da0
	.global Resource_Data186
Resource_Data186:
	.incbin "baserom.gba", 0x008e80b0, 0x00001200
	.global Resource_Data187
Resource_Data187:
	.incbin "baserom.gba", 0x008e92b0, 0x000007a4
	.global Resource_Data188
Resource_Data188:
	.incbin "baserom.gba", 0x008e9a54, 0x00001014
	.global Resource_Data189
Resource_Data189:
	.incbin "baserom.gba", 0x008eaa68, 0x0000037c
	.global Resource_Data18A
Resource_Data18A:
	.incbin "baserom.gba", 0x008eade4, 0x00000430
	.global Resource_Data18B
Resource_Data18B:
	.incbin "baserom.gba", 0x008eb214, 0x000010cc
	.global Resource_Data18C
Resource_Data18C:
	.incbin "baserom.gba", 0x008ec2e0, 0x00000084
	.global Resource_Data18D
Resource_Data18D:
	.incbin "baserom.gba", 0x008ec364, 0x000004e8
	.global Resource_Data18E
Resource_Data18E:
	.incbin "baserom.gba", 0x008ec84c, 0x00000084
	.global Resource_Data18F
Resource_Data18F:
	.incbin "baserom.gba", 0x008ec8d0, 0x00000084
	.global Resource_Data190
Resource_Data190:
	.incbin "baserom.gba", 0x008ec954, 0x000006b8
	.global Resource_Data191
Resource_Data191:
	.incbin "baserom.gba", 0x008ed00c, 0x00000914
	.global Resource_Data192
Resource_Data192:
	.incbin "baserom.gba", 0x008ed920, 0x00001b34
	.global Resource_Data193
Resource_Data193:
	.incbin "baserom.gba", 0x008ef454, 0x00001050
	.global Resource_Data194
Resource_Data194:
	.incbin "baserom.gba", 0x008f04a4, 0x000010a8
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
	.global Resource_Data19A
Resource_Data19A:
	.incbin "baserom.gba", 0x0093c39c, 0x000001f0
	.global Resource_Data19B
Resource_Data19B:
	.incbin "baserom.gba", 0x0093c58c, 0x00000154
	.global Resource_Data19C
Resource_Data19C:
	.incbin "baserom.gba", 0x0093c6e0, 0x000004b8
	.global Resource_Data19D
Resource_Data19D:
	.incbin "baserom.gba", 0x0093cb98, 0x0000176c
	.global Resource_Data19E
Resource_Data19E:
	.incbin "baserom.gba", 0x0093e304, 0x00001538
	.global Resource_Data19F
Resource_Data19F:
	.incbin "baserom.gba", 0x0093f83c, 0x00000dd0
	.global Resource_Data1A0
Resource_Data1A0:
	.incbin "baserom.gba", 0x0094060c, 0x00001084
	.global Resource_Data1A1
Resource_Data1A1:
	.incbin "baserom.gba", 0x00941690, 0x000011d4
	.global Resource_Data1A2
Resource_Data1A2:
	.incbin "baserom.gba", 0x00942864, 0x00000f40
	.global Resource_Data1A3
Resource_Data1A3:
	.incbin "baserom.gba", 0x009437a4, 0x000001d8
	.global Resource_Data1A4
Resource_Data1A4:
	.incbin "baserom.gba", 0x0094397c, 0x000001e4
	.global Resource_Data1A5
Resource_Data1A5:
	.incbin "baserom.gba", 0x00943b60, 0x000003c0
	.global Resource_Data1A6
Resource_Data1A6:
	.incbin "baserom.gba", 0x00943f20, 0x000015b8
	.global Resource_Data1A7
Resource_Data1A7:
	.incbin "baserom.gba", 0x009454d8, 0x000010c4
	.global Resource_Data1A8
Resource_Data1A8:
	.incbin "baserom.gba", 0x0094659c, 0x00000a08
	.global Resource_Data1A9
Resource_Data1A9:
	.incbin "baserom.gba", 0x00946fa4, 0x00000bb8
	.global Resource_Data1AA
Resource_Data1AA:
	.incbin "baserom.gba", 0x00947b5c, 0x000011d4
	.global Resource_Data1AB
Resource_Data1AB:
	.incbin "baserom.gba", 0x00948d30, 0x00000f94
	.global Resource_Data1AC
Resource_Data1AC:
	.incbin "baserom.gba", 0x00949cc4, 0x000001d0
	.global Resource_Data1AD
Resource_Data1AD:
	.incbin "baserom.gba", 0x00949e94, 0x00000008
	.global Resource_Data1AE
Resource_Data1AE:
	.incbin "baserom.gba", 0x00949e9c, 0x00000460
	.global Resource_Data1AF
Resource_Data1AF:
	.incbin "baserom.gba", 0x0094a2fc, 0x000015f4
	.global Resource_Data1B0
Resource_Data1B0:
	.incbin "baserom.gba", 0x0094b8f0, 0x00001470
	.global Resource_Data1B1
Resource_Data1B1:
	.incbin "baserom.gba", 0x0094cd60, 0x000008f4
	.global Resource_Data1B2
Resource_Data1B2:
	.incbin "baserom.gba", 0x0094d654, 0x00000928
	.global Resource_Data1B3
Resource_Data1B3:
	.incbin "baserom.gba", 0x0094df7c, 0x00000ef0
	.global Resource_Data1B4
Resource_Data1B4:
	.incbin "baserom.gba", 0x0094ee6c, 0x00000bec
	.global Resource_Data1B5
Resource_Data1B5:
	.incbin "baserom.gba", 0x0094fa58, 0x000001dc
	.global Resource_Data1B6
Resource_Data1B6:
	.incbin "baserom.gba", 0x0094fc34, 0x000001f0
	.global Resource_Data1B7
Resource_Data1B7:
	.incbin "baserom.gba", 0x0094fe24, 0x000002d8
	.global Resource_Data1B8
Resource_Data1B8:
	.incbin "baserom.gba", 0x009500fc, 0x00000898
	.global Resource_Data1B9
Resource_Data1B9:
	.incbin "baserom.gba", 0x00950994, 0x000014fc
	.global Resource_Data1BA
Resource_Data1BA:
	.incbin "baserom.gba", 0x00951e90, 0x00000f40
	.global Resource_Data1BB
Resource_Data1BB:
	.incbin "baserom.gba", 0x00952dd0, 0x000005d0
	.global Resource_Data1BC
Resource_Data1BC:
	.incbin "baserom.gba", 0x009533a0, 0x000004a0
	.global Resource_Data1BD
Resource_Data1BD:
	.incbin "baserom.gba", 0x00953840, 0x00000354
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
	.global Resource_Data203
Resource_Data203:
	.incbin "baserom.gba", 0x0096b4f0, 0x00005238
	.global Resource_Data204
Resource_Data204:
	.incbin "baserom.gba", 0x00970728, 0x00002f4c
	.global Resource_Data205
Resource_Data205:
	.incbin "baserom.gba", 0x00973674, 0x00003c24
	.global Resource_Data206
Resource_Data206:
	.incbin "baserom.gba", 0x00977298, 0x0000224c
	.global Resource_Data207
Resource_Data207:
	.incbin "baserom.gba", 0x009794e4, 0x0000559c
	.global Resource_Data208
Resource_Data208:
	.incbin "baserom.gba", 0x0097ea80, 0x00002c54
	.global Resource_Data209
Resource_Data209:
	.incbin "baserom.gba", 0x009816d4, 0x00000c24
	.global Resource_Data20A
Resource_Data20A:
	.incbin "baserom.gba", 0x009822f8, 0x00005e6c
	.global Resource_Data20B
Resource_Data20B:
	.incbin "baserom.gba", 0x00988164, 0x000010d4
	.global Resource_Data20C
Resource_Data20C:
	.incbin "baserom.gba", 0x00989238, 0x0000106c
	.global Resource_Data20D
Resource_Data20D:
	.incbin "baserom.gba", 0x0098a2a4, 0x00002a30
	.global Resource_Data20E
Resource_Data20E:
	.incbin "baserom.gba", 0x0098ccd4, 0x00002c28
	.global Resource_Data20F
Resource_Data20F:
	.incbin "baserom.gba", 0x0098f8fc, 0x00003144
	.global Resource_Data210
Resource_Data210:
	.incbin "baserom.gba", 0x00992a40, 0x000020a0
	.global Resource_Data211
Resource_Data211:
	.incbin "baserom.gba", 0x00994ae0, 0x00002320
	.global Resource_Data212
Resource_Data212:
	.incbin "baserom.gba", 0x00996e00, 0x00002c78
	.global Resource_Data213
Resource_Data213:
	.incbin "baserom.gba", 0x00999a78, 0x00001efc
	.global Resource_Data214
Resource_Data214:
	.incbin "baserom.gba", 0x0099b974, 0x000039f8
	.global Resource_Data215
Resource_Data215:
	.incbin "baserom.gba", 0x0099f36c, 0x00004b4c
	.global Resource_Data216
Resource_Data216:
	.incbin "baserom.gba", 0x009a3eb8, 0x00002470
	.global Resource_Data217
Resource_Data217:
	.incbin "baserom.gba", 0x009a6328, 0x00003800
	.global Resource_Data218
Resource_Data218:
	.incbin "baserom.gba", 0x009a9b28, 0x00003010
	.global Resource_Data219
Resource_Data219:
	.incbin "baserom.gba", 0x009acb38, 0x00002b7c
	.global Resource_Data21A
Resource_Data21A:
	.incbin "baserom.gba", 0x009af6b4, 0x00003134
	.global Resource_Data21B
Resource_Data21B:
	.incbin "baserom.gba", 0x009b27e8, 0x00003908
	.global Resource_Data21C
Resource_Data21C:
	.incbin "baserom.gba", 0x009b60f0, 0x00002af4
	.global Resource_Data21D
Resource_Data21D:
	.incbin "baserom.gba", 0x009b8be4, 0x00001af0
	.global Resource_Data21E
Resource_Data21E:
	.incbin "baserom.gba", 0x009ba6d4, 0x00002914
	.global Resource_Data21F
Resource_Data21F:
	.incbin "baserom.gba", 0x009bcfe8, 0x00003440
	.global Resource_Data220
Resource_Data220:
	.incbin "baserom.gba", 0x009c0428, 0x000049a8
	.global Resource_Data221
Resource_Data221:
	.incbin "baserom.gba", 0x009c4dd0, 0x00000f9c
	.global Resource_Data222
Resource_Data222:
	.incbin "baserom.gba", 0x009c5d6c, 0x0000272c
	.global Resource_Data223
Resource_Data223:
	.incbin "baserom.gba", 0x009c8498, 0x00001714
	.global Resource_Data224
Resource_Data224:
	.incbin "baserom.gba", 0x009c9bac, 0x000041f4
	.global Resource_Data225
Resource_Data225:
	.incbin "baserom.gba", 0x009cdda0, 0x00004624
	.global Resource_Data226
Resource_Data226:
	.incbin "baserom.gba", 0x009d23c4, 0x000042c8
	.global Resource_Data227
Resource_Data227:
	.incbin "baserom.gba", 0x009d668c, 0x0000562c
	.global Resource_Data228
Resource_Data228:
	.incbin "baserom.gba", 0x009dbcb8, 0x00002210
	.global Resource_Data229
Resource_Data229:
	.incbin "baserom.gba", 0x009ddec8, 0x000017e4
	.global Resource_Data22A
Resource_Data22A:
	.incbin "baserom.gba", 0x009df6ac, 0x00002d98
	.global Resource_Data22B
Resource_Data22B:
	.incbin "baserom.gba", 0x009e2444, 0x00001048
	.global Resource_Data22C
Resource_Data22C:
	.incbin "baserom.gba", 0x009e348c, 0x00003be4
	.global Resource_Data22D
Resource_Data22D:
	.incbin "baserom.gba", 0x009e7070, 0x000049ac
	.global Resource_Data22E
Resource_Data22E:
	.incbin "baserom.gba", 0x009eba1c, 0x00001dfc
	.global Resource_Data22F
Resource_Data22F:
	.incbin "baserom.gba", 0x009ed818, 0x000013c4
	.global Resource_Data230
Resource_Data230:
	.incbin "baserom.gba", 0x009eebdc, 0x000041a0
	.global Resource_Data231
Resource_Data231:
	.incbin "baserom.gba", 0x009f2d7c, 0x00002d64
	.global Resource_Data232
Resource_Data232:
	.incbin "baserom.gba", 0x009f5ae0, 0x00002300
	.global Resource_Data233
Resource_Data233:
	.incbin "baserom.gba", 0x009f7de0, 0x000041c0
	.global Resource_Data234
Resource_Data234:
	.incbin "baserom.gba", 0x009fbfa0, 0x0000108c
	.global Resource_Data235
Resource_Data235:
	.incbin "baserom.gba", 0x009fd02c, 0x00002308
	.global Resource_Data236
Resource_Data236:
	.incbin "baserom.gba", 0x009ff334, 0x000030f8
	.global Resource_Data237
Resource_Data237:
	.incbin "baserom.gba", 0x00a0242c, 0x000021b4
	.global Resource_Data238
Resource_Data238:
	.incbin "baserom.gba", 0x00a045e0, 0x000018f4
	.global Resource_Data239
Resource_Data239:
	.incbin "baserom.gba", 0x00a05ed4, 0x00002874
	.global Resource_Data23A
Resource_Data23A:
	.incbin "baserom.gba", 0x00a08748, 0x00003224
	.global Resource_Data23B
Resource_Data23B:
	.incbin "baserom.gba", 0x00a0b96c, 0x00001704
	.global Resource_Data23C
Resource_Data23C:
	.incbin "baserom.gba", 0x00a0d070, 0x00002c9c
	.global Resource_Data23D
Resource_Data23D:
	.incbin "baserom.gba", 0x00a0fd0c, 0x0000193c
	.global Resource_Data23E
Resource_Data23E:
	.incbin "baserom.gba", 0x00a11648, 0x00002610
	.global Resource_Data23F
Resource_Data23F:
	.incbin "baserom.gba", 0x00a13c58, 0x00004b04
	.global Resource_Data240
Resource_Data240:
	.incbin "baserom.gba", 0x00a1875c, 0x00004490
	.global Resource_Data241
Resource_Data241:
	.incbin "baserom.gba", 0x00a1cbec, 0x0000364c
	.global Resource_Data242
Resource_Data242:
	.incbin "baserom.gba", 0x00a20238, 0x00003094
	.global Resource_Data243
Resource_Data243:
	.incbin "baserom.gba", 0x00a232cc, 0x0000286c
	.global Resource_Data244
Resource_Data244:
	.incbin "baserom.gba", 0x00a25b38, 0x00000ca8
	.global Resource_Data245
Resource_Data245:
	.incbin "baserom.gba", 0x00a267e0, 0x00000e48
	.global Resource_Data246
Resource_Data246:
	.incbin "baserom.gba", 0x00a27628, 0x00000e18
	.global Resource_Data247
Resource_Data247:
	.incbin "baserom.gba", 0x00a28440, 0x00000cec
	.global Resource_Data248
Resource_Data248:
	.incbin "baserom.gba", 0x00a2912c, 0x00002b48
	.global Resource_Data249
Resource_Data249:
	.incbin "baserom.gba", 0x00a2bc74, 0x00002324
	.global Resource_Data24A
Resource_Data24A:
	.incbin "baserom.gba", 0x00a2df98, 0x000035f4
	.global Resource_Data24B
Resource_Data24B:
	.incbin "baserom.gba", 0x00a3158c, 0x0000105c
	.global Resource_Data24C
Resource_Data24C:
	.incbin "baserom.gba", 0x00a325e8, 0x00000d0c
	.global Resource_Data24D
Resource_Data24D:
	.incbin "baserom.gba", 0x00a332f4, 0x00000a84
	.global Resource_Data24E
Resource_Data24E:
	.incbin "baserom.gba", 0x00a33d78, 0x000007e0
	.global Resource_Data24F
Resource_Data24F:
	.incbin "baserom.gba", 0x00a34558, 0x00000f30
	.global Resource_Data250
Resource_Data250:
	.incbin "baserom.gba", 0x00a35488, 0x00000c70
	.global Resource_Data251
Resource_Data251:
	.incbin "baserom.gba", 0x00a360f8, 0x00000c18
	.global Resource_Data252
Resource_Data252:
	.incbin "baserom.gba", 0x00a36d10, 0x00000774
	.global Resource_Data253
Resource_Data253:
	.incbin "baserom.gba", 0x00a37484, 0x00000c18
	.global Resource_Data254
Resource_Data254:
	.incbin "baserom.gba", 0x00a3809c, 0x00000c78
	.global Resource_Data255
Resource_Data255:
	.incbin "baserom.gba", 0x00a38d14, 0x000009d0
	.global Resource_Data256
Resource_Data256:
	.incbin "baserom.gba", 0x00a396e4, 0x00000c1c
	.global Resource_Data257
Resource_Data257:
	.incbin "baserom.gba", 0x00a3a300, 0x000026b4
	.global Resource_Data258
Resource_Data258:
	.incbin "baserom.gba", 0x00a3c9b4, 0x00002714
	.global Resource_Data259
Resource_Data259:
	.incbin "baserom.gba", 0x00a3f0c8, 0x00004f54
	.global Resource_Data25A
Resource_Data25A:
	.incbin "baserom.gba", 0x00a4401c, 0x000018f0
	.global Resource_Data25B
Resource_Data25B:
	.incbin "baserom.gba", 0x00a4590c, 0x00002a3c
	.global Resource_Data25C
Resource_Data25C:
	.incbin "baserom.gba", 0x00a48348, 0x00000bec
	.global Resource_Data25D
Resource_Data25D:
	.incbin "baserom.gba", 0x00a48f34, 0x00004bb4
	.global Resource_Data25E
Resource_Data25E:
	.incbin "baserom.gba", 0x00a4dae8, 0x0000236c
	.global Resource_Data25F
Resource_Data25F:
	.incbin "baserom.gba", 0x00a4fe54, 0x000019a0
	.global Resource_Data260
Resource_Data260:
	.incbin "baserom.gba", 0x00a517f4, 0x00003bfc
	.global Resource_Data261
Resource_Data261:
	.incbin "baserom.gba", 0x00a553f0, 0x00000cc8
	.global Resource_Data262
Resource_Data262:
	.incbin "baserom.gba", 0x00a560b8, 0x00003410
	.global Resource_Data263
Resource_Data263:
	.incbin "baserom.gba", 0x00a594c8, 0x000040c8
	.global Resource_Data264
Resource_Data264:
	.incbin "baserom.gba", 0x00a5d590, 0x000036c8
	.global Resource_Data265
Resource_Data265:
	.incbin "baserom.gba", 0x00a60c58, 0x00004228
	.global Resource_Data266
Resource_Data266:
	.incbin "baserom.gba", 0x00a64e80, 0x00003da8
	.global Resource_Data267
Resource_Data267:
	.incbin "baserom.gba", 0x00a68c28, 0x00002b1c
	.global Resource_Data268
Resource_Data268:
	.incbin "baserom.gba", 0x00a6b744, 0x000047f4
	.global Resource_Data269
Resource_Data269:
	.incbin "baserom.gba", 0x00a6ff38, 0x00004f80
	.global Resource_Data26A
Resource_Data26A:
	.incbin "baserom.gba", 0x00a74eb8, 0x0000279c
	.global Resource_Data26B
Resource_Data26B:
	.incbin "baserom.gba", 0x00a77654, 0x00001ec8
	.global Resource_Data26C
Resource_Data26C:
	.incbin "baserom.gba", 0x00a7951c, 0x00000420
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
	.global Resource_Data272
Resource_Data272:
	.incbin "baserom.gba", 0x00a79e58, 0x00001260
	.global Resource_Data273
Resource_Data273:
	.incbin "baserom.gba", 0x00a7b0b8, 0x000001d0
	.global Resource_Data274
Resource_Data274:
	.incbin "baserom.gba", 0x00a7b288, 0x000020a0
	.global Resource_Data275
Resource_Data275:
	.incbin "baserom.gba", 0x00a7d328, 0x00000f8c
	.global Resource_Data276
Resource_Data276:
	.incbin "baserom.gba", 0x00a7e2b4, 0x000022d8
	.global Resource_Data277
Resource_Data277:
	.incbin "baserom.gba", 0x00a8058c, 0x00001254
	.global Resource_Data278
Resource_Data278:
	.incbin "baserom.gba", 0x00a817e0, 0x0000461c
	.global Resource_Data279
Resource_Data279:
	.incbin "baserom.gba", 0x00a85dfc, 0x00000108
	.global Resource_Data27A
Resource_Data27A:
	.incbin "baserom.gba", 0x00a85f04, 0x000011ec
	.global Resource_Data27B
Resource_Data27B:
	.incbin "baserom.gba", 0x00a870f0, 0x00001c88
	.global Resource_Data27C
Resource_Data27C:
	.incbin "baserom.gba", 0x00a88d78, 0x0000050c
	.global Resource_Data27D
Resource_Data27D:
	.incbin "baserom.gba", 0x00a89284, 0x00000140
	.global Resource_Data27E
Resource_Data27E:
	.incbin "baserom.gba", 0x00a893c4, 0x00001c84
	.global Resource_Data27F
Resource_Data27F:
	.incbin "baserom.gba", 0x00a8b048, 0x00000170
	.global Resource_Data280
Resource_Data280:
	.incbin "baserom.gba", 0x00a8b1b8, 0x00002868
	.global Resource_Data281
Resource_Data281:
	.incbin "baserom.gba", 0x00a8da20, 0x000027c8
	.global Resource_Data282
Resource_Data282:
	.incbin "baserom.gba", 0x00a901e8, 0x00001520
	.global Resource_Data283
Resource_Data283:
	.incbin "baserom.gba", 0x00a91708, 0x00001944
	.global Resource_Data284
Resource_Data284:
	.incbin "baserom.gba", 0x00a9304c, 0x00001ab0
	.global Resource_Data285
Resource_Data285:
	.incbin "baserom.gba", 0x00a94afc, 0x00000174
	.global Resource_Data286
Resource_Data286:
	.incbin "baserom.gba", 0x00a94c70, 0x00002de4
	.global Resource_Data287
Resource_Data287:
	.incbin "baserom.gba", 0x00a97a54, 0x000028a8
	.global Resource_Data288
Resource_Data288:
	.incbin "baserom.gba", 0x00a9a2fc, 0x0000295c
	.global Resource_Data289
Resource_Data289:
	.incbin "baserom.gba", 0x00a9cc58, 0x00001b3c
	.global Resource_Data28A
Resource_Data28A:
	.incbin "baserom.gba", 0x00a9e794, 0x00001838
	.global Resource_Data28B
Resource_Data28B:
	.incbin "baserom.gba", 0x00a9ffcc, 0x00001988
	.global Resource_Data28C
Resource_Data28C:
	.incbin "baserom.gba", 0x00aa1954, 0x000001b8
	.global Resource_Data28D
Resource_Data28D:
	.incbin "baserom.gba", 0x00aa1b0c, 0x00002afc
	.global Resource_Data28E
Resource_Data28E:
	.incbin "baserom.gba", 0x00aa4608, 0x000020e0
	.global Resource_Data28F
Resource_Data28F:
	.incbin "baserom.gba", 0x00aa66e8, 0x0000224c
	.global Resource_Data290
Resource_Data290:
	.incbin "baserom.gba", 0x00aa8934, 0x00000140
	.global Resource_Data291
Resource_Data291:
	.incbin "baserom.gba", 0x00aa8a74, 0x000016d4
	.global Resource_Data292
Resource_Data292:
	.incbin "baserom.gba", 0x00aaa148, 0x00000100
	.global Resource_Data293
Resource_Data293:
	.incbin "baserom.gba", 0x00aaa248, 0x00000274
	.global Resource_Data294
Resource_Data294:
	.incbin "baserom.gba", 0x00aaa4bc, 0x00001c80
	.global Resource_Data295
Resource_Data295:
	.incbin "baserom.gba", 0x00aac13c, 0x000010f8
	.global Resource_Data296
Resource_Data296:
	.incbin "baserom.gba", 0x00aad234, 0x000008e0
	.global Resource_Data297
Resource_Data297:
	.incbin "baserom.gba", 0x00aadb14, 0x00002898
	.global Resource_Data298
Resource_Data298:
	.incbin "baserom.gba", 0x00ab03ac, 0x00000198
	.global Resource_Data299
Resource_Data299:
	.incbin "baserom.gba", 0x00ab0544, 0x00002b44
	.global Resource_Data29A
Resource_Data29A:
	.incbin "baserom.gba", 0x00ab3088, 0x0000270c
	.global Resource_Data29B
Resource_Data29B:
	.incbin "baserom.gba", 0x00ab5794, 0x00002944
	.global Resource_Data29C
Resource_Data29C:
	.incbin "baserom.gba", 0x00ab80d8, 0x000014d4
	.global Resource_Data29D
Resource_Data29D:
	.incbin "baserom.gba", 0x00ab95ac, 0x000023f8
	.global Resource_Data29E
Resource_Data29E:
	.incbin "baserom.gba", 0x00abb9a4, 0x000029d4
	.global Resource_Data29F
Resource_Data29F:
	.incbin "baserom.gba", 0x00abe378, 0x00000178
	.global Resource_Data2A0
Resource_Data2A0:
	.incbin "baserom.gba", 0x00abe4f0, 0x0000187c
	.global Resource_Data2A1
Resource_Data2A1:
	.incbin "baserom.gba", 0x00abfd6c, 0x000012b4
	.global Resource_Data2A2
Resource_Data2A2:
	.incbin "baserom.gba", 0x00ac1020, 0x00001098
	.global Resource_Data2A3
Resource_Data2A3:
	.incbin "baserom.gba", 0x00ac20b8, 0x00001250
	.global Resource_Data2A4
Resource_Data2A4:
	.incbin "baserom.gba", 0x00ac3308, 0x00000f50
	.global Resource_Data2A5
Resource_Data2A5:
	.incbin "baserom.gba", 0x00ac4258, 0x00000198
	.global Resource_Data2A6
Resource_Data2A6:
	.incbin "baserom.gba", 0x00ac43f0, 0x00002e00
	.global Resource_Data2A7
Resource_Data2A7:
	.incbin "baserom.gba", 0x00ac71f0, 0x000017b4
	.global Resource_Data2A8
Resource_Data2A8:
	.incbin "baserom.gba", 0x00ac89a4, 0x00000f9c
	.global Resource_Data2A9
Resource_Data2A9:
	.incbin "baserom.gba", 0x00ac9940, 0x000017cc
	.global Resource_Data2AA
Resource_Data2AA:
	.incbin "baserom.gba", 0x00acb10c, 0x00000e94
	.global Resource_Data2AB
Resource_Data2AB:
	.incbin "baserom.gba", 0x00acbfa0, 0x000001a8
	.global Resource_Data2AC
Resource_Data2AC:
	.incbin "baserom.gba", 0x00acc148, 0x00002e78
	.global Resource_Data2AD
Resource_Data2AD:
	.incbin "baserom.gba", 0x00acefc0, 0x00000140
	.global Resource_Data2AE
Resource_Data2AE:
	.incbin "baserom.gba", 0x00acf100, 0x00000ca8
	.global Resource_Data2AF
Resource_Data2AF:
	.incbin "baserom.gba", 0x00acfda8, 0x000015d0
	.global Resource_Data2B0
Resource_Data2B0:
	.incbin "baserom.gba", 0x00ad1378, 0x00001454
	.global Resource_Data2B1
Resource_Data2B1:
	.incbin "baserom.gba", 0x00ad27cc, 0x00000ad0
	.global Resource_Data2B2
Resource_Data2B2:
	.incbin "baserom.gba", 0x00ad329c, 0x00000b40
	.global Resource_Data2B3
Resource_Data2B3:
	.incbin "baserom.gba", 0x00ad3ddc, 0x0000176c
	.global Resource_Data2B4
Resource_Data2B4:
	.incbin "baserom.gba", 0x00ad5548, 0x000012e8
	.global Resource_Data2B5
Resource_Data2B5:
	.incbin "baserom.gba", 0x00ad6830, 0x00000e8c
	.global Resource_Data2B6
Resource_Data2B6:
	.incbin "baserom.gba", 0x00ad76bc, 0x00001b0c
	.global Resource_Data2B7
Resource_Data2B7:
	.incbin "baserom.gba", 0x00ad91c8, 0x0000017c
	.global Resource_Data2B8
Resource_Data2B8:
	.incbin "baserom.gba", 0x00ad9344, 0x00001ae4
	.global Resource_Data2B9
Resource_Data2B9:
	.incbin "baserom.gba", 0x00adae28, 0x000014ec
	.global Resource_Data2BA
Resource_Data2BA:
	.incbin "baserom.gba", 0x00adc314, 0x000000f0
	.global Resource_Data2BB
Resource_Data2BB:
	.incbin "baserom.gba", 0x00adc404, 0x000026d4
	.global Resource_Data2BC
Resource_Data2BC:
	.incbin "baserom.gba", 0x00adead8, 0x000017dc
	.global Resource_Data2BD
Resource_Data2BD:
	.incbin "baserom.gba", 0x00ae02b4, 0x00002288
	.global Resource_Data2BE
Resource_Data2BE:
	.incbin "baserom.gba", 0x00ae253c, 0x000004d4
	.global Resource_Data2BF
Resource_Data2BF:
	.incbin "baserom.gba", 0x00ae2a10, 0x00001148
	.global Resource_Data2C0
Resource_Data2C0:
	.incbin "baserom.gba", 0x00ae3b58, 0x000001b0
	.global Resource_Data2C1
Resource_Data2C1:
	.incbin "baserom.gba", 0x00ae3d08, 0x00002e94
	.global Resource_Data2C2
Resource_Data2C2:
	.incbin "baserom.gba", 0x00ae6b9c, 0x00000f5c
	.global Resource_Data2C3
Resource_Data2C3:
	.incbin "baserom.gba", 0x00ae7af8, 0x00000b3c
	.global Resource_Data2C4
Resource_Data2C4:
	.incbin "baserom.gba", 0x00ae8634, 0x00001140
	.global Resource_Data2C5
Resource_Data2C5:
	.incbin "baserom.gba", 0x00ae9774, 0x00000e74
	.global Resource_Data2C6
Resource_Data2C6:
	.incbin "baserom.gba", 0x00aea5e8, 0x00000f58
	.global Resource_Data2C7
Resource_Data2C7:
	.incbin "baserom.gba", 0x00aeb540, 0x00000edc
	.global Resource_Data2C8
Resource_Data2C8:
	.incbin "baserom.gba", 0x00aec41c, 0x00000194
	.global Resource_Data2C9
Resource_Data2C9:
	.incbin "baserom.gba", 0x00aec5b0, 0x00000f80
	.global Resource_Data2CA
Resource_Data2CA:
	.incbin "baserom.gba", 0x00aed530, 0x00000ac4
	.global Resource_Data2CB
Resource_Data2CB:
	.incbin "baserom.gba", 0x00aedff4, 0x00001a00
	.global Resource_Data2CC
Resource_Data2CC:
	.incbin "baserom.gba", 0x00aef9f4, 0x00000120
	.global Resource_Data2CD
Resource_Data2CD:
	.incbin "baserom.gba", 0x00aefb14, 0x000023a0
	.global Resource_Data2CE
Resource_Data2CE:
	.incbin "baserom.gba", 0x00af1eb4, 0x0000124c
	.global Resource_Data2CF
Resource_Data2CF:
	.incbin "baserom.gba", 0x00af3100, 0x00002390
	.global Resource_Data2D0
Resource_Data2D0:
	.incbin "baserom.gba", 0x00af5490, 0x00001900
	.global Resource_Data2D1
Resource_Data2D1:
	.incbin "baserom.gba", 0x00af6d90, 0x00000e80
	.global Resource_Data2D2
Resource_Data2D2:
	.incbin "baserom.gba", 0x00af7c10, 0x00000170
	.global Resource_Data2D3
Resource_Data2D3:
	.incbin "baserom.gba", 0x00af7d80, 0x0000259c
	.global Resource_Data2D4
Resource_Data2D4:
	.incbin "baserom.gba", 0x00afa31c, 0x000027a0
	.global Resource_Data2D5
Resource_Data2D5:
	.incbin "baserom.gba", 0x00afcabc, 0x00001e88
	.global Resource_Data2D6
Resource_Data2D6:
	.incbin "baserom.gba", 0x00afe944, 0x000006c4
	.global Resource_Data2D7
Resource_Data2D7:
	.incbin "baserom.gba", 0x00aff008, 0x00002a98
	.global Resource_Data2D8
Resource_Data2D8:
	.incbin "baserom.gba", 0x00b01aa0, 0x00000150
	.global Resource_Data2D9
Resource_Data2D9:
	.incbin "baserom.gba", 0x00b01bf0, 0x00001434
	.global Resource_Data2DA
Resource_Data2DA:
	.incbin "baserom.gba", 0x00b03024, 0x00000fdc
	.global Resource_Data2DB
Resource_Data2DB:
	.incbin "baserom.gba", 0x00b04000, 0x00002180
	.global Resource_Data2DC
Resource_Data2DC:
	.incbin "baserom.gba", 0x00b06180, 0x00000140
	.global Resource_Data2DD
Resource_Data2DD:
	.incbin "baserom.gba", 0x00b062c0, 0x00002aa0
	.global Resource_Data2DE
Resource_Data2DE:
	.incbin "baserom.gba", 0x00b08d60, 0x00000150
	.global Resource_Data2DF
Resource_Data2DF:
	.incbin "baserom.gba", 0x00b08eb0, 0x00001434
	.global Resource_Data2E0
Resource_Data2E0:
	.incbin "baserom.gba", 0x00b0a2e4, 0x00000fdc
	.global Resource_Data2E1
Resource_Data2E1:
	.incbin "baserom.gba", 0x00b0b2c0, 0x00002180
	.global Resource_Data2E2
Resource_Data2E2:
	.incbin "baserom.gba", 0x00b0d440, 0x00000140
	.global Resource_Data2E3
Resource_Data2E3:
	.incbin "baserom.gba", 0x00b0d580, 0x00003630
	.global Resource_Data2E4
Resource_Data2E4:
	.incbin "baserom.gba", 0x00b10bb0, 0x00000154
	.global Resource_Data2E5
Resource_Data2E5:
	.incbin "baserom.gba", 0x00b10d04, 0x00001434
	.global Resource_Data2E6
Resource_Data2E6:
	.incbin "baserom.gba", 0x00b12138, 0x00000fdc
	.global Resource_Data2E7
Resource_Data2E7:
	.incbin "baserom.gba", 0x00b13114, 0x00002180
	.global Resource_Data2E8
Resource_Data2E8:
	.incbin "baserom.gba", 0x00b15294, 0x00000140
	.global Resource_Data2E9
Resource_Data2E9:
	.incbin "baserom.gba", 0x00b153d4, 0x000014b0
	.global Resource_Data2EA
Resource_Data2EA:
	.incbin "baserom.gba", 0x00b16884, 0x0000010c
	.global Resource_Data2EB
Resource_Data2EB:
	.incbin "baserom.gba", 0x00b16990, 0x00001b58
	.global Resource_Data2EC
Resource_Data2EC:
	.incbin "baserom.gba", 0x00b184e8, 0x000014ac
	.global Resource_Data2ED
Resource_Data2ED:
	.incbin "baserom.gba", 0x00b19994, 0x000001f8
	.global Resource_Data2EE
Resource_Data2EE:
	.incbin "baserom.gba", 0x00b19b8c, 0x000001c0
	.global Resource_Data2EF
Resource_Data2EF:
	.incbin "baserom.gba", 0x00b19d4c, 0x00000d9c
	.global Resource_Data2F0
Resource_Data2F0:
	.incbin "baserom.gba", 0x00b1aae8, 0x00000100
	.global Resource_Data2F1
Resource_Data2F1:
	.incbin "baserom.gba", 0x00b1abe8, 0x00001b58
	.global Resource_Data2F2
Resource_Data2F2:
	.incbin "baserom.gba", 0x00b1c740, 0x00001660
	.global Resource_Data2F3
Resource_Data2F3:
	.incbin "baserom.gba", 0x00b1dda0, 0x000001f8
	.global Resource_Data2F4
Resource_Data2F4:
	.incbin "baserom.gba", 0x00b1df98, 0x000001c0
	.global Resource_Data2F5
Resource_Data2F5:
	.incbin "baserom.gba", 0x00b1e158, 0x00000eb8
	.global Resource_Data2F6
Resource_Data2F6:
	.incbin "baserom.gba", 0x00b1f010, 0x00000104
	.global Resource_Data2F7
Resource_Data2F7:
	.incbin "baserom.gba", 0x00b1f114, 0x00001c1c
	.global Resource_Data2F8
Resource_Data2F8:
	.incbin "baserom.gba", 0x00b20d30, 0x000014ac
	.global Resource_Data2F9
Resource_Data2F9:
	.incbin "baserom.gba", 0x00b221dc, 0x000001f8
	.global Resource_Data2FA
Resource_Data2FA:
	.incbin "baserom.gba", 0x00b223d4, 0x000001c0
	.global Resource_Data2FB
Resource_Data2FB:
	.incbin "baserom.gba", 0x00b22594, 0x00000a78
	.global Resource_Data2FC
Resource_Data2FC:
	.incbin "baserom.gba", 0x00b2300c, 0x00000108
	.global Resource_Data2FD
Resource_Data2FD:
	.incbin "baserom.gba", 0x00b23114, 0x00001cc0
	.global Resource_Data2FE
Resource_Data2FE:
	.incbin "baserom.gba", 0x00b24dd4, 0x000014ac
	.global Resource_Data2FF
Resource_Data2FF:
	.incbin "baserom.gba", 0x00b26280, 0x000002e0
	.global Resource_Data300
Resource_Data300:
	.incbin "baserom.gba", 0x00b26560, 0x000002b8
	.global Resource_Data301
Resource_Data301:
	.incbin "baserom.gba", 0x00b26818, 0x000019c0
	.global Resource_Data302
Resource_Data302:
	.incbin "baserom.gba", 0x00b281d8, 0x00000150
	.global Resource_Data303
Resource_Data303:
	.incbin "baserom.gba", 0x00b28328, 0x0000288c
	.global Resource_Data304
Resource_Data304:
	.incbin "baserom.gba", 0x00b2abb4, 0x000027f0
	.global Resource_Data305
Resource_Data305:
	.incbin "baserom.gba", 0x00b2d3a4, 0x00000fa0
	.global Resource_Data306
Resource_Data306:
	.incbin "baserom.gba", 0x00b2e344, 0x00002154
	.global Resource_Data307
Resource_Data307:
	.incbin "baserom.gba", 0x00b30498, 0x0000368c
	.global Resource_Data308
Resource_Data308:
	.incbin "baserom.gba", 0x00b33b24, 0x000001b4
	.global Resource_Data309
Resource_Data309:
	.incbin "baserom.gba", 0x00b33cd8, 0x00001be0
	.global Resource_Data30A
Resource_Data30A:
	.incbin "baserom.gba", 0x00b358b8, 0x00001e20
	.global Resource_Data30B
Resource_Data30B:
	.incbin "baserom.gba", 0x00b376d8, 0x00001adc
	.global Resource_Data30C
Resource_Data30C:
	.incbin "baserom.gba", 0x00b391b4, 0x000015e8
	.global Resource_Data30D
Resource_Data30D:
	.incbin "baserom.gba", 0x00b3a79c, 0x000025e8
	.global Resource_Data30E
Resource_Data30E:
	.incbin "baserom.gba", 0x00b3cd84, 0x00000178
	.global Resource_Data30F
Resource_Data30F:
	.incbin "baserom.gba", 0x00b3cefc, 0x000014c0
	.global Resource_Data310
Resource_Data310:
	.incbin "baserom.gba", 0x00b3e3bc, 0x00001804
	.global Resource_Data311
Resource_Data311:
	.incbin "baserom.gba", 0x00b3fbc0, 0x00001978
	.global Resource_Data312
Resource_Data312:
	.incbin "baserom.gba", 0x00b41538, 0x00000f20
	.global Resource_Data313
Resource_Data313:
	.incbin "baserom.gba", 0x00b42458, 0x00001aac
	.global Resource_Data314
Resource_Data314:
	.incbin "baserom.gba", 0x00b43f04, 0x0000010c
	.global Resource_Data315
Resource_Data315:
	.incbin "baserom.gba", 0x00b44010, 0x00002130
	.global Resource_Data316
Resource_Data316:
	.incbin "baserom.gba", 0x00b46140, 0x00001bd8
	.global Resource_Data317
Resource_Data317:
	.incbin "baserom.gba", 0x00b47d18, 0x00000610
	.global Resource_Data318
Resource_Data318:
	.incbin "baserom.gba", 0x00b48328, 0x00000140
	.global Resource_Data319
Resource_Data319:
	.incbin "baserom.gba", 0x00b48468, 0x000006a4
	.global Resource_Data31A
Resource_Data31A:
	.incbin "baserom.gba", 0x00b48b0c, 0x00000100
	.global Resource_Data31B
Resource_Data31B:
	.incbin "baserom.gba", 0x00b48c0c, 0x00001f5c
	.global Resource_Data31C
Resource_Data31C:
	.incbin "baserom.gba", 0x00b4ab68, 0x00001860
	.global Resource_Data31D
Resource_Data31D:
	.incbin "baserom.gba", 0x00b4c3c8, 0x00000610
	.global Resource_Data31E
Resource_Data31E:
	.incbin "baserom.gba", 0x00b4c9d8, 0x00000140
	.global Resource_Data31F
Resource_Data31F:
	.incbin "baserom.gba", 0x00b4cb18, 0x00000e7c
	.global Resource_Data320
Resource_Data320:
	.incbin "baserom.gba", 0x00b4d994, 0x00000118
	.global Resource_Data321
Resource_Data321:
	.incbin "baserom.gba", 0x00b4daac, 0x000022d4
	.global Resource_Data322
Resource_Data322:
	.incbin "baserom.gba", 0x00b4fd80, 0x00001c54
	.global Resource_Data323
Resource_Data323:
	.incbin "baserom.gba", 0x00b519d4, 0x00000644
	.global Resource_Data324
Resource_Data324:
	.incbin "baserom.gba", 0x00b52018, 0x00000140
	.global Resource_Data325
Resource_Data325:
	.incbin "baserom.gba", 0x00b52158, 0x00000a9c
	.global Resource_Data326
Resource_Data326:
	.incbin "baserom.gba", 0x00b52bf4, 0x0000016c
	.global Resource_Data327
Resource_Data327:
	.incbin "baserom.gba", 0x00b52d60, 0x000025b0
	.global Resource_Data328
Resource_Data328:
	.incbin "baserom.gba", 0x00b55310, 0x00001cc0
	.global Resource_Data329
Resource_Data329:
	.incbin "baserom.gba", 0x00b56fd0, 0x00001acc
	.global Resource_Data32A
Resource_Data32A:
	.incbin "baserom.gba", 0x00b58a9c, 0x00000424
	.global Resource_Data32B
Resource_Data32B:
	.incbin "baserom.gba", 0x00b58ec0, 0x00000cb0
	.global Resource_Data32C
Resource_Data32C:
	.incbin "baserom.gba", 0x00b59b70, 0x00000168
	.global Resource_Data32D
Resource_Data32D:
	.incbin "baserom.gba", 0x00b59cd8, 0x000010a0
	.global Resource_Data32E
Resource_Data32E:
	.incbin "baserom.gba", 0x00b5ad78, 0x00000178
	.global Resource_Data32F
Resource_Data32F:
	.incbin "baserom.gba", 0x00b5aef0, 0x00000be0
	.global Resource_Data330
Resource_Data330:
	.incbin "baserom.gba", 0x00b5bad0, 0x0000016c
	.global Resource_Data331
Resource_Data331:
	.incbin "baserom.gba", 0x00b5bc3c, 0x00002e50
	.global Resource_Data332
Resource_Data332:
	.incbin "baserom.gba", 0x00b5ea8c, 0x000027c0
	.global Resource_Data333
Resource_Data333:
	.incbin "baserom.gba", 0x00b6124c, 0x00002534
	.global Resource_Data334
Resource_Data334:
	.incbin "baserom.gba", 0x00b63780, 0x000019d8
	.global Resource_Data335
Resource_Data335:
	.incbin "baserom.gba", 0x00b65158, 0x000021a4
	.global Resource_Data336
Resource_Data336:
	.incbin "baserom.gba", 0x00b672fc, 0x00000104
	.global Resource_Data337
Resource_Data337:
	.incbin "baserom.gba", 0x00b67400, 0x00001a2c
	.global Resource_Data338
Resource_Data338:
	.incbin "baserom.gba", 0x00b68e2c, 0x00002338
	.global Resource_Data339
Resource_Data339:
	.incbin "baserom.gba", 0x00b6b164, 0x000011b0
	.global Resource_Data33A
Resource_Data33A:
	.incbin "baserom.gba", 0x00b6c314, 0x00001924
	.global Resource_Data33B
Resource_Data33B:
	.incbin "baserom.gba", 0x00b6dc38, 0x00001d30
	.global Resource_Data33C
Resource_Data33C:
	.incbin "baserom.gba", 0x00b6f968, 0x0000015c
	.global Resource_Data33D
Resource_Data33D:
	.incbin "baserom.gba", 0x00b6fac4, 0x000021e0
	.global Resource_Data33E
Resource_Data33E:
	.incbin "baserom.gba", 0x00b71ca4, 0x000027f4
	.global Resource_Data33F
Resource_Data33F:
	.incbin "baserom.gba", 0x00b74498, 0x000011d4
	.global Resource_Data340
Resource_Data340:
	.incbin "baserom.gba", 0x00b7566c, 0x000000ec
	.global Resource_Data341
Resource_Data341:
	.incbin "baserom.gba", 0x00b75758, 0x0000274c
	.global Resource_Data342
Resource_Data342:
	.incbin "baserom.gba", 0x00b77ea4, 0x00000aec
	.global Resource_Data343
Resource_Data343:
	.incbin "baserom.gba", 0x00b78990, 0x00000678
	.global Resource_Data344
Resource_Data344:
	.incbin "baserom.gba", 0x00b79008, 0x00001948
	.global Resource_Data345
Resource_Data345:
	.incbin "baserom.gba", 0x00b7a950, 0x00002478
	.global Resource_Data346
Resource_Data346:
	.incbin "baserom.gba", 0x00b7cdc8, 0x000001a4
	.global Resource_Data347
Resource_Data347:
	.incbin "baserom.gba", 0x00b7cf6c, 0x000020f0
	.global Resource_Data348
Resource_Data348:
	.incbin "baserom.gba", 0x00b7f05c, 0x00001a1c
	.global Resource_Data349
Resource_Data349:
	.incbin "baserom.gba", 0x00b80a78, 0x0000110c
	.global Resource_Data34A
Resource_Data34A:
	.incbin "baserom.gba", 0x00b81b84, 0x00001064
	.global Resource_Data34B
Resource_Data34B:
	.incbin "baserom.gba", 0x00b82be8, 0x00001fb4
	.global Resource_Data34C
Resource_Data34C:
	.incbin "baserom.gba", 0x00b84b9c, 0x00000188
	.global Resource_Data34D
Resource_Data34D:
	.incbin "baserom.gba", 0x00b84d24, 0x000011f8
	.global Resource_Data34E
Resource_Data34E:
	.incbin "baserom.gba", 0x00b85f1c, 0x00000ec4
	.global Resource_Data34F
Resource_Data34F:
	.incbin "baserom.gba", 0x00b86de0, 0x00001180
	.global Resource_Data350
Resource_Data350:
	.incbin "baserom.gba", 0x00b87f60, 0x00000f20
	.global Resource_Data351
Resource_Data351:
	.incbin "baserom.gba", 0x00b88e80, 0x000018e0
	.global Resource_Data352
Resource_Data352:
	.incbin "baserom.gba", 0x00b8a760, 0x00000174
	.global Resource_Data353
Resource_Data353:
	.incbin "baserom.gba", 0x00b8a8d4, 0x00001348
	.global Resource_Data354
Resource_Data354:
	.incbin "baserom.gba", 0x00b8bc1c, 0x00000184
	.global Resource_Data355
Resource_Data355:
	.incbin "baserom.gba", 0x00b8bda0, 0x000011b8
	.global Resource_Data356
Resource_Data356:
	.incbin "baserom.gba", 0x00b8cf58, 0x0000104c
	.global Resource_Data357
Resource_Data357:
	.incbin "baserom.gba", 0x00b8dfa4, 0x000000f4
	.global Resource_Data358
Resource_Data358:
	.incbin "baserom.gba", 0x00b8e098, 0x000015d4
	.global Resource_Data359
Resource_Data359:
	.incbin "baserom.gba", 0x00b8f66c, 0x000000e0
	.global Resource_Data35A
Resource_Data35A:
	.incbin "baserom.gba", 0x00b8f74c, 0x00000140
	.global Resource_Data35B
Resource_Data35B:
	.incbin "baserom.gba", 0x00b8f88c, 0x000020e4
	.global Resource_Data35C
Resource_Data35C:
	.incbin "baserom.gba", 0x00b91970, 0x000001c4
	.global Resource_Data35D
Resource_Data35D:
	.incbin "baserom.gba", 0x00b91b34, 0x00000170
	.global Resource_Data35E
Resource_Data35E:
	.incbin "baserom.gba", 0x00b91ca4, 0x00001f54
	.global Resource_Data35F
Resource_Data35F:
	.incbin "baserom.gba", 0x00b93bf8, 0x00000130
	.global Resource_Data360
Resource_Data360:
	.incbin "baserom.gba", 0x00b93d28, 0x00002ab8
	.global Resource_Data361
Resource_Data361:
	.incbin "baserom.gba", 0x00b967e0, 0x00001b58
	.global Resource_Data362
Resource_Data362:
	.incbin "baserom.gba", 0x00b98338, 0x00000f48
	.global Resource_Data363
Resource_Data363:
	.incbin "baserom.gba", 0x00b99280, 0x00001a08
	.global Resource_Data364
Resource_Data364:
	.incbin "baserom.gba", 0x00b9ac88, 0x00001924
	.global Resource_Data365
Resource_Data365:
	.incbin "baserom.gba", 0x00b9c5ac, 0x00001ad0
	.global Resource_Data366
Resource_Data366:
	.incbin "baserom.gba", 0x00b9e07c, 0x00001474
	.global Resource_Data367
Resource_Data367:
	.incbin "baserom.gba", 0x00b9f4f0, 0x000016c4
	.global Resource_Data368
Resource_Data368:
	.incbin "baserom.gba", 0x00ba0bb4, 0x00001524
	.global Resource_Data369
Resource_Data369:
	.incbin "baserom.gba", 0x00ba20d8, 0x00001140
	.global Resource_Data36A
Resource_Data36A:
	.incbin "baserom.gba", 0x00ba3218, 0x00002438
	.global Resource_Data36B
Resource_Data36B:
	.incbin "baserom.gba", 0x00ba5650, 0x00000cb4
	.global Resource_Data36C
Resource_Data36C:
	.incbin "baserom.gba", 0x00ba6304, 0x00000cb0
	.global Resource_Data36D
Resource_Data36D:
	.incbin "baserom.gba", 0x00ba6fb4, 0x00000d80
	.global Resource_Data36E
Resource_Data36E:
	.incbin "baserom.gba", 0x00ba7d34, 0x0000130c
	.global Resource_Data36F
Resource_Data36F:
	.incbin "baserom.gba", 0x00ba9040, 0x0000199c
	.global Resource_Data370
Resource_Data370:
	.incbin "baserom.gba", 0x00baa9dc, 0x00000150
	.global Resource_Data371
Resource_Data371:
	.incbin "baserom.gba", 0x00baab2c, 0x00002b0c
	.global Resource_Data372
Resource_Data372:
	.incbin "baserom.gba", 0x00bad638, 0x00001ab8
	.global Resource_Data373
Resource_Data373:
	.incbin "baserom.gba", 0x00baf0f0, 0x00000150
	.global Resource_Data374
Resource_Data374:
	.incbin "baserom.gba", 0x00baf240, 0x00001a08
	.global Resource_Data375
Resource_Data375:
	.incbin "baserom.gba", 0x00bb0c48, 0x00001d1c
	.global Resource_Data376
Resource_Data376:
	.incbin "baserom.gba", 0x00bb2964, 0x00000150
	.global Resource_Data377
Resource_Data377:
	.incbin "baserom.gba", 0x00bb2ab4, 0x00001a08
	.global Resource_Data378
Resource_Data378:
	.incbin "baserom.gba", 0x00bb44bc, 0x00003d88
	.global Resource_Data379
Resource_Data379:
	.incbin "baserom.gba", 0x00bb8244, 0x00000194
	.global Resource_Data37A
Resource_Data37A:
	.incbin "baserom.gba", 0x00bb83d8, 0x00002968
	.global Resource_Data37B
Resource_Data37B:
	.incbin "baserom.gba", 0x00bbad40, 0x000026a0
	.global Resource_Data37C
Resource_Data37C:
	.incbin "baserom.gba", 0x00bbd3e0, 0x00002684
	.global Resource_Data37D
Resource_Data37D:
	.incbin "baserom.gba", 0x00bbfa64, 0x00000334
	.global Resource_Data37E
Resource_Data37E:
	.incbin "baserom.gba", 0x00bbfd98, 0x00001c8c
	.global Resource_Data37F
Resource_Data37F:
	.incbin "baserom.gba", 0x00bc1a24, 0x000022ac
	.global Resource_Data380
Resource_Data380:
	.incbin "baserom.gba", 0x00bc3cd0, 0x00002520
	.global Resource_Data381
Resource_Data381:
	.incbin "baserom.gba", 0x00bc61f0, 0x0000014c
	.global Resource_Data382
Resource_Data382:
	.incbin "baserom.gba", 0x00bc633c, 0x00001e84
	.global Resource_Data383
Resource_Data383:
	.incbin "baserom.gba", 0x00bc81c0, 0x000015a8
	.global Resource_Data384
Resource_Data384:
	.incbin "baserom.gba", 0x00bc9768, 0x00001fac
	.global Resource_Data385
Resource_Data385:
	.incbin "baserom.gba", 0x00bcb714, 0x00000f70
	.global Resource_Data386
Resource_Data386:
	.incbin "baserom.gba", 0x00bcc684, 0x00001484
	.global Resource_Data387
Resource_Data387:
	.incbin "baserom.gba", 0x00bcdb08, 0x0000174c
	.global Resource_Data388
Resource_Data388:
	.incbin "baserom.gba", 0x00bcf254, 0x00001868
	.global Resource_Data389
Resource_Data389:
	.incbin "baserom.gba", 0x00bd0abc, 0x000010a4
	.global Resource_Data38A
Resource_Data38A:
	.incbin "baserom.gba", 0x00bd1b60, 0x00001924
	.global Resource_Data38B
Resource_Data38B:
	.incbin "baserom.gba", 0x00bd3484, 0x00001470
	.global Resource_Data38C
Resource_Data38C:
	.incbin "baserom.gba", 0x00bd48f4, 0x0000178c
	.global Resource_Data38D
Resource_Data38D:
	.incbin "baserom.gba", 0x00bd6080, 0x00001070
	.global Resource_Data38E
Resource_Data38E:
	.incbin "baserom.gba", 0x00bd70f0, 0x00000b9c
	.global Resource_Data38F
Resource_Data38F:
	.incbin "baserom.gba", 0x00bd7c8c, 0x000000bc
	.global Resource_Data390
Resource_Data390:
	.incbin "baserom.gba", 0x00bd7d48, 0x00000d40
	.global Resource_Data391
Resource_Data391:
	.incbin "baserom.gba", 0x00bd8a88, 0x0000089c
	.global Resource_Data392
Resource_Data392:
	.incbin "baserom.gba", 0x00bd9324, 0x00000140
	.global Resource_Data393
Resource_Data393:
	.incbin "baserom.gba", 0x00bd9464, 0x00000c18
	.global Resource_Data394
Resource_Data394:
	.incbin "baserom.gba", 0x00bda07c, 0x00001ebc
	.global Resource_Data395
Resource_Data395:
	.incbin "baserom.gba", 0x00bdbf38, 0x0000011c
	.global Resource_Data396
Resource_Data396:
	.incbin "baserom.gba", 0x00bdc054, 0x00002880
	.global Resource_Data397
Resource_Data397:
	.incbin "baserom.gba", 0x00bde8d4, 0x000001a4
	.global Resource_Data398
Resource_Data398:
	.incbin "baserom.gba", 0x00bdea78, 0x000015e8
	.global Resource_Data399
Resource_Data399:
	.incbin "baserom.gba", 0x00be0060, 0x00002760
	.global Resource_Data39A
Resource_Data39A:
	.incbin "baserom.gba", 0x00be27c0, 0x000017d0
	.global Resource_Data39B
Resource_Data39B:
	.incbin "baserom.gba", 0x00be3f90, 0x00000148
	.global Resource_Data39C
Resource_Data39C:
	.incbin "baserom.gba", 0x00be40d8, 0x000026b0
	.global Resource_Data39D
Resource_Data39D:
	.incbin "baserom.gba", 0x00be6788, 0x000027a4
	.global Resource_Data39E
Resource_Data39E:
	.incbin "baserom.gba", 0x00be8f2c, 0x00000d8c
	.global Resource_Data39F
Resource_Data39F:
	.incbin "baserom.gba", 0x00be9cb8, 0x00001944
	.global Resource_Data3A0
Resource_Data3A0:
	.incbin "baserom.gba", 0x00beb5fc, 0x000014fc
	.global Resource_Data3A1
Resource_Data3A1:
	.incbin "baserom.gba", 0x00becaf8, 0x000001c8
	.global Resource_Data3A2
Resource_Data3A2:
	.incbin "baserom.gba", 0x00beccc0, 0x00002460
	.global Resource_Data3A3
Resource_Data3A3:
	.incbin "baserom.gba", 0x00bef120, 0x00001cec
	.global Resource_Data3A4
Resource_Data3A4:
	.incbin "baserom.gba", 0x00bf0e0c, 0x00001700
	.global Resource_Data3A5
Resource_Data3A5:
	.incbin "baserom.gba", 0x00bf250c, 0x0000157c
	.global Resource_Data3A6
Resource_Data3A6:
	.incbin "baserom.gba", 0x00bf3a88, 0x000015d4
	.global Resource_Data3A7
Resource_Data3A7:
	.incbin "baserom.gba", 0x00bf505c, 0x0000019c
	.global Resource_Data3A8
Resource_Data3A8:
	.incbin "baserom.gba", 0x00bf51f8, 0x00002c8c
	.global Resource_Data3A9
Resource_Data3A9:
	.incbin "baserom.gba", 0x00bf7e84, 0x0000104c
	.global Resource_Data3AA
Resource_Data3AA:
	.incbin "baserom.gba", 0x00bf8ed0, 0x000010fc
	.global Resource_Data3AB
Resource_Data3AB:
	.incbin "baserom.gba", 0x00bf9fcc, 0x000017d4
	.global Resource_Data3AC
Resource_Data3AC:
	.incbin "baserom.gba", 0x00bfb7a0, 0x00001328
	.global Resource_Data3AD
Resource_Data3AD:
	.incbin "baserom.gba", 0x00bfcac8, 0x0000019c
	.global Resource_Data3AE
Resource_Data3AE:
	.incbin "baserom.gba", 0x00bfcc64, 0x00002534
	.global Resource_Data3AF
Resource_Data3AF:
	.incbin "baserom.gba", 0x00bff198, 0x00000134
	.global Resource_Data3B0
Resource_Data3B0:
	.incbin "baserom.gba", 0x00bff2cc, 0x0000276c
	.global Resource_Data3B1
Resource_Data3B1:
	.incbin "baserom.gba", 0x00c01a38, 0x00001dd4
	.global Resource_Data3B2
Resource_Data3B2:
	.incbin "baserom.gba", 0x00c0380c, 0x00001200
	.global Resource_Data3B3
Resource_Data3B3:
	.incbin "baserom.gba", 0x00c04a0c, 0x00000170
	.global Resource_Data3B4
Resource_Data3B4:
	.incbin "baserom.gba", 0x00c04b7c, 0x00002574
	.global Resource_Data3B5
Resource_Data3B5:
	.incbin "baserom.gba", 0x00c070f0, 0x000001e0
	.global Resource_Data3B6
Resource_Data3B6:
	.incbin "baserom.gba", 0x00c072d0, 0x00002584
	.global Resource_Data3B7
Resource_Data3B7:
	.incbin "baserom.gba", 0x00c09854, 0x0000196c
	.global Resource_Data3B8
Resource_Data3B8:
	.incbin "baserom.gba", 0x00c0b1c0, 0x00001908
	.global Resource_Data3B9
Resource_Data3B9:
	.incbin "baserom.gba", 0x00c0cac8, 0x00000f20
	.global Resource_Data3BA
Resource_Data3BA:
	.incbin "baserom.gba", 0x00c0d9e8, 0x000042bc
	.global Resource_Data3BB
Resource_Data3BB:
	.incbin "baserom.gba", 0x00c11ca4, 0x00000124
	.global Resource_Data3BC
Resource_Data3BC:
	.incbin "baserom.gba", 0x00c11dc8, 0x00001fd8
	.global Resource_Data3BD
Resource_Data3BD:
	.incbin "baserom.gba", 0x00c13da0, 0x000013d0
	.global Resource_Data3BE
Resource_Data3BE:
	.incbin "baserom.gba", 0x00c15170, 0x00000df4
	.global Resource_Data3BF
Resource_Data3BF:
	.incbin "baserom.gba", 0x00c15f64, 0x00000d78
	.global Resource_Data3C0
Resource_Data3C0:
	.incbin "baserom.gba", 0x00c16cdc, 0x000023f4
	.global Resource_Data3C1
Resource_Data3C1:
	.incbin "baserom.gba", 0x00c190d0, 0x000034a4
	.global Resource_Data3C2
Resource_Data3C2:
	.incbin "baserom.gba", 0x00c1c574, 0x00000138
	.global Resource_Data3C3
Resource_Data3C3:
	.incbin "baserom.gba", 0x00c1c6ac, 0x00002758
	.global Resource_Data3C4
Resource_Data3C4:
	.incbin "baserom.gba", 0x00c1ee04, 0x00001c0c
	.global Resource_Data3C5
Resource_Data3C5:
	.incbin "baserom.gba", 0x00c20a10, 0x00000e58
	.global Resource_Data3C6
Resource_Data3C6:
	.incbin "baserom.gba", 0x00c21868, 0x00000f40
	.global Resource_Data3C7
Resource_Data3C7:
	.incbin "baserom.gba", 0x00c227a8, 0x000028a8
	.global Resource_Data3C8
Resource_Data3C8:
	.incbin "baserom.gba", 0x00c25050, 0x00000134
	.global Resource_Data3C9
Resource_Data3C9:
	.incbin "baserom.gba", 0x00c25184, 0x00000a04
	.global Resource_Data3CA
Resource_Data3CA:
	.incbin "baserom.gba", 0x00c25b88, 0x00000128
	.global Resource_Data3CB
Resource_Data3CB:
	.incbin "baserom.gba", 0x00c25cb0, 0x00001988
	.global Resource_Data3CC
Resource_Data3CC:
	.incbin "baserom.gba", 0x00c27638, 0x00001490
	.global Resource_Data3CD
Resource_Data3CD:
	.incbin "baserom.gba", 0x00c28ac8, 0x00000468
	.global Resource_Data3CE
Resource_Data3CE:
	.incbin "baserom.gba", 0x00c28f30, 0x000001c0
	.global Resource_Data3CF
Resource_Data3CF:
	.incbin "baserom.gba", 0x00c290f0, 0x00002808
	.global Resource_Data3D0
Resource_Data3D0:
	.incbin "baserom.gba", 0x00c2b8f8, 0x00000158
	.global Resource_Data3D1
Resource_Data3D1:
	.incbin "baserom.gba", 0x00c2ba50, 0x00002948
	.global Resource_Data3D2
Resource_Data3D2:
	.incbin "baserom.gba", 0x00c2e398, 0x00002224
	.global Resource_Data3D3
Resource_Data3D3:
	.incbin "baserom.gba", 0x00c305bc, 0x000011e8
	.global Resource_Data3D4
Resource_Data3D4:
	.incbin "baserom.gba", 0x00c317a4, 0x0000030c
	.global Resource_Data3D5
Resource_Data3D5:
	.incbin "baserom.gba", 0x00c31ab0, 0x00002144
	.global Resource_Data3D6
Resource_Data3D6:
	.incbin "baserom.gba", 0x00c33bf4, 0x00000120
	.global Resource_Data3D7
Resource_Data3D7:
	.incbin "baserom.gba", 0x00c33d14, 0x000028ac
	.global Resource_Data3D8
Resource_Data3D8:
	.incbin "baserom.gba", 0x00c365c0, 0x00001f94
	.global Resource_Data3D9
Resource_Data3D9:
	.incbin "baserom.gba", 0x00c38554, 0x00001020
	.global Resource_Data3DA
Resource_Data3DA:
	.incbin "baserom.gba", 0x00c39574, 0x00000140
	.global Resource_Data3DB
Resource_Data3DB:
	.incbin "baserom.gba", 0x00c396b4, 0x00002ab8
	.global Resource_Data3DC
Resource_Data3DC:
	.incbin "baserom.gba", 0x00c3c16c, 0x00000168
	.global Resource_Data3DD
Resource_Data3DD:
	.incbin "baserom.gba", 0x00c3c2d4, 0x00002484
	.global Resource_Data3DE
Resource_Data3DE:
	.incbin "baserom.gba", 0x00c3e758, 0x000020e8
	.global Resource_Data3DF
Resource_Data3DF:
	.incbin "baserom.gba", 0x00c40840, 0x000014c0
	.global Resource_Data3E0
Resource_Data3E0:
	.incbin "baserom.gba", 0x00c41d00, 0x000003a4
	.global Resource_Data3E1
Resource_Data3E1:
	.incbin "baserom.gba", 0x00c420a4, 0x00002564
	.global Resource_Data3E2
Resource_Data3E2:
	.incbin "baserom.gba", 0x00c44608, 0x00000168
	.global Resource_Data3E3
Resource_Data3E3:
	.incbin "baserom.gba", 0x00c44770, 0x000027f8
	.global Resource_Data3E4
Resource_Data3E4:
	.incbin "baserom.gba", 0x00c46f68, 0x00002034
	.global Resource_Data3E5
Resource_Data3E5:
	.incbin "baserom.gba", 0x00c48f9c, 0x00001078
	.global Resource_Data3E6
Resource_Data3E6:
	.incbin "baserom.gba", 0x00c4a014, 0x00000bb0
	.global Resource_Data3E7
Resource_Data3E7:
	.incbin "baserom.gba", 0x00c4abc4, 0x0000283c
	.global Resource_Data3E8
Resource_Data3E8:
	.incbin "baserom.gba", 0x00c4d400, 0x000001e4
	.global Resource_Data3E9
Resource_Data3E9:
	.incbin "baserom.gba", 0x00c4d5e4, 0x00000b6c
	.global Resource_Data3EA
Resource_Data3EA:
	.incbin "baserom.gba", 0x00c4e150, 0x000000e4
	.global Resource_Data3EB
Resource_Data3EB:
	.incbin "baserom.gba", 0x00c4e234, 0x0000194c
	.global Resource_Data3EC
Resource_Data3EC:
	.incbin "baserom.gba", 0x00c4fb80, 0x00000c2c
	.global Resource_Data3ED
Resource_Data3ED:
	.incbin "baserom.gba", 0x00c507ac, 0x00000bf8
	.global Resource_Data3EE
Resource_Data3EE:
	.incbin "baserom.gba", 0x00c513a4, 0x00000140
	.global Resource_Data3EF
Resource_Data3EF:
	.incbin "baserom.gba", 0x00c514e4, 0x000013d0
	.global Resource_Data3F0
Resource_Data3F0:
	.incbin "baserom.gba", 0x00c528b4, 0x000000e4
	.global Resource_Data3F1
Resource_Data3F1:
	.incbin "baserom.gba", 0x00c52998, 0x00000b70
	.global Resource_Data3F2
Resource_Data3F2:
	.incbin "baserom.gba", 0x00c53508, 0x000000e4
	.global Resource_Data3F3
Resource_Data3F3:
	.incbin "baserom.gba", 0x00c535ec, 0x00000d64
	.global Resource_Data3F4
Resource_Data3F4:
	.incbin "baserom.gba", 0x00c54350, 0x000000e4
	.global Resource_Data3F5
Resource_Data3F5:
	.incbin "baserom.gba", 0x00c54434, 0x00000808
	.global Resource_Data3F6
Resource_Data3F6:
	.incbin "baserom.gba", 0x00c54c3c, 0x000000e4
	.global Resource_Data3F7
Resource_Data3F7:
	.incbin "baserom.gba", 0x00c54d20, 0x00000a60
	.global Resource_Data3F8
Resource_Data3F8:
	.incbin "baserom.gba", 0x00c55780, 0x00000100
	.global Resource_Data3F9
Resource_Data3F9:
	.incbin "baserom.gba", 0x00c55880, 0x00000880
	.global Resource_Data3FA
Resource_Data3FA:
	.incbin "baserom.gba", 0x00c56100, 0x000000fc
	.global Resource_Data3FB
Resource_Data3FB:
	.incbin "baserom.gba", 0x00c561fc, 0x00001e58
	.global Resource_Data3FC
Resource_Data3FC:
	.incbin "baserom.gba", 0x00c58054, 0x00000e10
	.global Resource_Data3FD
Resource_Data3FD:
	.incbin "baserom.gba", 0x00c58e64, 0x00000748
	.global Resource_Data3FE
Resource_Data3FE:
	.incbin "baserom.gba", 0x00c595ac, 0x000001f0
	.global Resource_Data3FF
Resource_Data3FF:
	.incbin "baserom.gba", 0x00c5979c, 0x00000894
	.global Resource_Data400
Resource_Data400:
	.incbin "baserom.gba", 0x00c5a030, 0x000000c8
	.global Resource_Data401
Resource_Data401:
	.incbin "baserom.gba", 0x00c5a0f8, 0x00000f18
	.global Resource_Data402
Resource_Data402:
	.incbin "baserom.gba", 0x00c5b010, 0x000000c4
	.global Resource_Data403
Resource_Data403:
	.incbin "baserom.gba", 0x00c5b0d4, 0x00001878
	.global Resource_Data404
Resource_Data404:
	.incbin "baserom.gba", 0x00c5c94c, 0x000008f8
	.global Resource_Data405
Resource_Data405:
	.incbin "baserom.gba", 0x00c5d244, 0x00000294
	.global Resource_Data406
Resource_Data406:
	.incbin "baserom.gba", 0x00c5d4d8, 0x00000140
	.global Resource_Data407
Resource_Data407:
	.incbin "baserom.gba", 0x00c5d618, 0x000006d0
	.global Resource_Data408
Resource_Data408:
	.incbin "baserom.gba", 0x00c5dce8, 0x000000d4
	.global Resource_Data409
Resource_Data409:
	.incbin "baserom.gba", 0x00c5ddbc, 0x00000878
	.global Resource_Data40A
Resource_Data40A:
	.incbin "baserom.gba", 0x00c5e634, 0x000000dc
	.global Resource_Data40B
Resource_Data40B:
	.incbin "baserom.gba", 0x00c5e710, 0x000003d4
	.global Resource_Data40C
Resource_Data40C:
	.incbin "baserom.gba", 0x00c5eae4, 0x0000147c
	.global Resource_Data40D
Resource_Data40D:
	.incbin "baserom.gba", 0x00c5ff60, 0x000006d4
	.global Resource_Data40E
Resource_Data40E:
	.incbin "baserom.gba", 0x00c60634, 0x00000140
	.global Resource_Data40F
Resource_Data40F:
	.incbin "baserom.gba", 0x00c60774, 0x00000c98
	.global Resource_Data410
Resource_Data410:
	.incbin "baserom.gba", 0x00c6140c, 0x000001a4
	.global Resource_Data411
Resource_Data411:
	.incbin "baserom.gba", 0x00c615b0, 0x00002ec4
	.global Resource_Data412
Resource_Data412:
	.incbin "baserom.gba", 0x00c64474, 0x00001980
	.global Resource_Data413
Resource_Data413:
	.incbin "baserom.gba", 0x00c65df4, 0x0000104c
	.global Resource_Data414
Resource_Data414:
	.incbin "baserom.gba", 0x00c66e40, 0x00001848
	.global Resource_Data415
Resource_Data415:
	.incbin "baserom.gba", 0x00c68688, 0x00000e28
	.global Resource_Data416
Resource_Data416:
	.incbin "baserom.gba", 0x00c694b0, 0x00000d88
	.global Resource_Data417
Resource_Data417:
	.incbin "baserom.gba", 0x00c6a238, 0x00000e64
	.global Resource_Data418
Resource_Data418:
	.incbin "baserom.gba", 0x00c6b09c, 0x000013e0
	.global Resource_Data419
Resource_Data419:
	.incbin "baserom.gba", 0x00c6c47c, 0x00002108
	.global Resource_Data41A
Resource_Data41A:
	.incbin "baserom.gba", 0x00c6e584, 0x00000138
	.global Resource_Data41B
Resource_Data41B:
	.incbin "baserom.gba", 0x00c6e6bc, 0x00001c88
	.global Resource_Data41C
Resource_Data41C:
	.incbin "baserom.gba", 0x00c70344, 0x0000235c
	.global Resource_Data41D
Resource_Data41D:
	.incbin "baserom.gba", 0x00c726a0, 0x00001dfc
	.global Resource_Data41E
Resource_Data41E:
	.incbin "baserom.gba", 0x00c7449c, 0x000018dc
	.global Resource_Data41F
Resource_Data41F:
	.incbin "baserom.gba", 0x00c75d78, 0x0000273c
	.global Resource_Data420
Resource_Data420:
	.incbin "baserom.gba", 0x00c784b4, 0x000001c8
	.global Resource_Data421
Resource_Data421:
	.incbin "baserom.gba", 0x00c7867c, 0x0000204c
	.global Resource_Data422
Resource_Data422:
	.incbin "baserom.gba", 0x00c7a6c8, 0x00001b38
	.global Resource_Data423
Resource_Data423:
	.incbin "baserom.gba", 0x00c7c200, 0x00000174
	.global Resource_Data424
Resource_Data424:
	.incbin "baserom.gba", 0x00c7c374, 0x00002a38
	.global Resource_Data425
Resource_Data425:
	.incbin "baserom.gba", 0x00c7edac, 0x000028d0
	.global Resource_Data426
Resource_Data426:
	.incbin "baserom.gba", 0x00c8167c, 0x00002078
	.global Resource_Data427
Resource_Data427:
	.incbin "baserom.gba", 0x00c836f4, 0x000027d4
	.global Resource_Data428
Resource_Data428:
	.incbin "baserom.gba", 0x00c85ec8, 0x00001000
	.global Resource_Data429
Resource_Data429:
	.incbin "baserom.gba", 0x00c86ec8, 0x00000118
	.global Resource_Data42A
Resource_Data42A:
	.incbin "baserom.gba", 0x00c86fe0, 0x00001418
	.global Resource_Data42B
Resource_Data42B:
	.incbin "baserom.gba", 0x00c883f8, 0x00000190
	.global Resource_Data42C
Resource_Data42C:
	.incbin "baserom.gba", 0x00c88588, 0x00001a5c
	.global Resource_Data42D
Resource_Data42D:
	.incbin "baserom.gba", 0x00c89fe4, 0x0000013c
	.global Resource_Data42E
Resource_Data42E:
	.incbin "baserom.gba", 0x00c8a120, 0x00001508
	.global Resource_Data42F
Resource_Data42F:
	.incbin "baserom.gba", 0x00c8b628, 0x00001ca4
	.global Resource_Data430
Resource_Data430:
	.incbin "baserom.gba", 0x00c8d2cc, 0x000015e4
	.global Resource_Data431
Resource_Data431:
	.incbin "baserom.gba", 0x00c8e8b0, 0x00001448
	.global Resource_Data432
Resource_Data432:
	.incbin "baserom.gba", 0x00c8fcf8, 0x00002cf8
	.global Resource_Data433
Resource_Data433:
	.incbin "baserom.gba", 0x00c929f0, 0x00000188
	.global Resource_Data434
Resource_Data434:
	.incbin "baserom.gba", 0x00c92b78, 0x000022ac
	.global Resource_Data435
Resource_Data435:
	.incbin "baserom.gba", 0x00c94e24, 0x000001dc
	.global Resource_Data436
Resource_Data436:
	.incbin "baserom.gba", 0x00c95000, 0x00001624
	.global Resource_Data437
Resource_Data437:
	.incbin "baserom.gba", 0x00c96624, 0x00001af0
	.global Resource_Data438
Resource_Data438:
	.incbin "baserom.gba", 0x00c98114, 0x00001484
	.global Resource_Data439
Resource_Data439:
	.incbin "baserom.gba", 0x00c99598, 0x00001050
	.global Resource_Data43A
Resource_Data43A:
	.incbin "baserom.gba", 0x00c9a5e8, 0x00001e64
	.global Resource_Data43B
Resource_Data43B:
	.incbin "baserom.gba", 0x00c9c44c, 0x0000019c
	.global Resource_Data43C
Resource_Data43C:
	.incbin "baserom.gba", 0x00c9c5e8, 0x0000015c
	.global Resource_Data43D
Resource_Data43D:
	.incbin "baserom.gba", 0x00c9c744, 0x00000f80
	.global Resource_Data43E
Resource_Data43E:
	.incbin "baserom.gba", 0x00c9d6c4, 0x00000118
	.global Resource_Data43F
Resource_Data43F:
	.incbin "baserom.gba", 0x00c9d7dc, 0x00002094
	.global Resource_Data440
Resource_Data440:
	.incbin "baserom.gba", 0x00c9f870, 0x000017e8
	.global Resource_Data441
Resource_Data441:
	.incbin "baserom.gba", 0x00ca1058, 0x00000698
	.global Resource_Data442
Resource_Data442:
	.incbin "baserom.gba", 0x00ca16f0, 0x00000430
	.global Resource_Data443
Resource_Data443:
	.incbin "baserom.gba", 0x00ca1b20, 0x000020f8
	.global Resource_Data444
Resource_Data444:
	.incbin "baserom.gba", 0x00ca3c18, 0x0000010c
	.global Resource_Data445
Resource_Data445:
	.incbin "baserom.gba", 0x00ca3d24, 0x00002094
	.global Resource_Data446
Resource_Data446:
	.incbin "baserom.gba", 0x00ca5db8, 0x000017e8
	.global Resource_Data447
Resource_Data447:
	.incbin "baserom.gba", 0x00ca75a0, 0x000006c0
	.global Resource_Data448
Resource_Data448:
	.incbin "baserom.gba", 0x00ca7c60, 0x0000162c
	.global Resource_Data449
Resource_Data449:
	.incbin "baserom.gba", 0x00ca928c, 0x00000aec
	.global Resource_Data44A
Resource_Data44A:
	.incbin "baserom.gba", 0x00ca9d78, 0x00000170
	.global Resource_Data44B
Resource_Data44B:
	.incbin "baserom.gba", 0x00ca9ee8, 0x00002f18
	.global Resource_Data44C
Resource_Data44C:
	.incbin "baserom.gba", 0x00cace00, 0x00000eb4
	.global Resource_Data44D
Resource_Data44D:
	.incbin "baserom.gba", 0x00cadcb4, 0x000010d8
	.global Resource_Data44E
Resource_Data44E:
	.incbin "baserom.gba", 0x00caed8c, 0x0000196c
	.global Resource_Data44F
Resource_Data44F:
	.incbin "baserom.gba", 0x00cb06f8, 0x00000be8
	.global Resource_Data450
Resource_Data450:
	.incbin "baserom.gba", 0x00cb12e0, 0x00000dc8
	.global Resource_Data451
Resource_Data451:
	.incbin "baserom.gba", 0x00cb20a8, 0x00000ea8
	.global Resource_Data452
Resource_Data452:
	.incbin "baserom.gba", 0x00cb2f50, 0x000012a8
	.global Resource_Data453
Resource_Data453:
	.incbin "baserom.gba", 0x00cb41f8, 0x00001130
	.global Resource_Data454
Resource_Data454:
	.incbin "baserom.gba", 0x00cb5328, 0x00000ebc
	.global Resource_Data455
Resource_Data455:
	.incbin "baserom.gba", 0x00cb61e4, 0x000001b0
	.global Resource_Data456
Resource_Data456:
	.incbin "baserom.gba", 0x00cb6394, 0x0000152c
	.global Resource_Data457
Resource_Data457:
	.incbin "baserom.gba", 0x00cb78c0, 0x00001100
	.global Resource_Data458
Resource_Data458:
	.incbin "baserom.gba", 0x00cb89c0, 0x0000234c
	.global Resource_Data459
Resource_Data459:
	.incbin "baserom.gba", 0x00cbad0c, 0x00000158
	.global Resource_Data45A
Resource_Data45A:
	.incbin "baserom.gba", 0x00cbae64, 0x00001b14
	.global Resource_Data45B
Resource_Data45B:
	.incbin "baserom.gba", 0x00cbc978, 0x00001ab8
	.global Resource_Data45C
Resource_Data45C:
	.incbin "baserom.gba", 0x00cbe430, 0x00001884
	.global Resource_Data45D
Resource_Data45D:
	.incbin "baserom.gba", 0x00cbfcb4, 0x00001d28
	.global Resource_Data45E
Resource_Data45E:
	.incbin "baserom.gba", 0x00cc19dc, 0x00001790
	.global Resource_Data45F
Resource_Data45F:
	.incbin "baserom.gba", 0x00cc316c, 0x00000154
	.global Resource_Data460
Resource_Data460:
	.incbin "baserom.gba", 0x00cc32c0, 0x00001e3c
	.global Resource_Data461
Resource_Data461:
	.incbin "baserom.gba", 0x00cc50fc, 0x00001b60
	.global Resource_Data462
Resource_Data462:
	.incbin "baserom.gba", 0x00cc6c5c, 0x00001cc8
	.global Resource_Data463
Resource_Data463:
	.incbin "baserom.gba", 0x00cc8924, 0x00001960
	.global Resource_Data464
Resource_Data464:
	.incbin "baserom.gba", 0x00cca284, 0x00002408
	.global Resource_Data465
Resource_Data465:
	.incbin "baserom.gba", 0x00ccc68c, 0x000001c8
	.global Resource_Data466
Resource_Data466:
	.incbin "baserom.gba", 0x00ccc854, 0x0000198c
	.global Resource_Data467
Resource_Data467:
	.incbin "baserom.gba", 0x00cce1e0, 0x0000112c
	.global Resource_Data468
Resource_Data468:
	.incbin "baserom.gba", 0x00ccf30c, 0x00000bb0
	.global Resource_Data469
Resource_Data469:
	.incbin "baserom.gba", 0x00ccfebc, 0x0000103c
	.global Resource_Data46A
Resource_Data46A:
	.incbin "baserom.gba", 0x00cd0ef8, 0x0000098c
	.global Resource_Data46B
Resource_Data46B:
	.incbin "baserom.gba", 0x00cd1884, 0x000000e4
	.global Resource_Data46C
Resource_Data46C:
	.incbin "baserom.gba", 0x00cd1968, 0x00001a24
	.global Resource_Data46D
Resource_Data46D:
	.incbin "baserom.gba", 0x00cd338c, 0x000008f8
	.global Resource_Data46E
Resource_Data46E:
	.incbin "baserom.gba", 0x00cd3c84, 0x00000140
	.global Resource_Data46F
Resource_Data46F:
	.incbin "baserom.gba", 0x00cd3dc4, 0x00000140
	.global Resource_Data470
Resource_Data470:
	.incbin "baserom.gba", 0x00cd3f04, 0x00003678
	.global Resource_Data471
Resource_Data471:
	.incbin "baserom.gba", 0x00cd757c, 0x00000164
	.global Resource_Data472
Resource_Data472:
	.incbin "baserom.gba", 0x00cd76e0, 0x000026e0
	.global Resource_Data473
Resource_Data473:
	.incbin "baserom.gba", 0x00cd9dc0, 0x00001d20
	.global Resource_Data474
Resource_Data474:
	.incbin "baserom.gba", 0x00cdbae0, 0x00001190
	.global Resource_Data475
Resource_Data475:
	.incbin "baserom.gba", 0x00cdcc70, 0x00001574
	.global Resource_Data476
Resource_Data476:
	.incbin "baserom.gba", 0x00cde1e4, 0x00002850
	.global Resource_Data477
Resource_Data477:
	.incbin "baserom.gba", 0x00ce0a34, 0x000018ac
	.global Resource_Data478
Resource_Data478:
	.incbin "baserom.gba", 0x00ce22e0, 0x00000108
	.global Resource_Data479
Resource_Data479:
	.incbin "baserom.gba", 0x00ce23e8, 0x0000217c
	.global Resource_Data47A
Resource_Data47A:
	.incbin "baserom.gba", 0x00ce4564, 0x00001be4
	.global Resource_Data47B
Resource_Data47B:
	.incbin "baserom.gba", 0x00ce6148, 0x00001cf8
	.global Resource_Data47C
Resource_Data47C:
	.incbin "baserom.gba", 0x00ce7e40, 0x00001540
	.global Resource_Data47D
Resource_Data47D:
	.incbin "baserom.gba", 0x00ce9380, 0x00001810
	.global Resource_Data47E
Resource_Data47E:
	.incbin "baserom.gba", 0x00ceab90, 0x00002048
	.global Resource_Data47F
Resource_Data47F:
	.incbin "baserom.gba", 0x00cecbd8, 0x000017c8
	.global Resource_Data480
Resource_Data480:
	.incbin "baserom.gba", 0x00cee3a0, 0x000014dc
	.global Resource_Data481
Resource_Data481:
	.incbin "baserom.gba", 0x00cef87c, 0x000019fc
	.global Resource_Data482
Resource_Data482:
	.incbin "baserom.gba", 0x00cf1278, 0x00000b90
	.global Resource_Data483
Resource_Data483:
	.incbin "baserom.gba", 0x00cf1e08, 0x000012e8
	.global Resource_Data484
Resource_Data484:
	.incbin "baserom.gba", 0x00cf30f0, 0x00001330
	.global Resource_Data485
Resource_Data485:
	.incbin "baserom.gba", 0x00cf4420, 0x00001044
	.global Resource_Data486
Resource_Data486:
	.incbin "baserom.gba", 0x00cf5464, 0x00000128
	.global Resource_Data487
Resource_Data487:
	.incbin "baserom.gba", 0x00cf558c, 0x00002210
	.global Resource_Data488
Resource_Data488:
	.incbin "baserom.gba", 0x00cf779c, 0x00000190
	.global Resource_Data489
Resource_Data489:
	.incbin "baserom.gba", 0x00cf792c, 0x00002a24
	.global Resource_Data48A
Resource_Data48A:
	.incbin "baserom.gba", 0x00cfa350, 0x000028e8
	.global Resource_Data48B
Resource_Data48B:
	.incbin "baserom.gba", 0x00cfcc38, 0x000021a8
	.global Resource_Data48C
Resource_Data48C:
	.incbin "baserom.gba", 0x00cfede0, 0x000027d4
	.global Resource_Data48D
Resource_Data48D:
	.incbin "baserom.gba", 0x00d015b4, 0x0000275c
	.global Resource_Data48E
Resource_Data48E:
	.incbin "baserom.gba", 0x00d03d10, 0x000001b4
	.global Resource_Data48F
Resource_Data48F:
	.incbin "baserom.gba", 0x00d03ec4, 0x00001704
	.global Resource_Data490
Resource_Data490:
	.incbin "baserom.gba", 0x00d055c8, 0x000013c8
	.global Resource_Data491
Resource_Data491:
	.incbin "baserom.gba", 0x00d06990, 0x00000c90
	.global Resource_Data492
Resource_Data492:
	.incbin "baserom.gba", 0x00d07620, 0x0000134c
	.global Resource_Data493
Resource_Data493:
	.incbin "baserom.gba", 0x00d0896c, 0x00001a48
	.global Resource_Data494
Resource_Data494:
	.incbin "baserom.gba", 0x00d0a3b4, 0x00000124
	.global Resource_Data495
Resource_Data495:
	.incbin "baserom.gba", 0x00d0a4d8, 0x000004d4
	.global Resource_Data496
Resource_Data496:
	.incbin "baserom.gba", 0x00d0a9ac, 0x00002094
	.global Resource_Data497
Resource_Data497:
	.incbin "baserom.gba", 0x00d0ca40, 0x00000150
	.global Resource_Data498
Resource_Data498:
	.incbin "baserom.gba", 0x00d0cb90, 0x000004d4
	.global Resource_Data499
Resource_Data499:
	.incbin "baserom.gba", 0x00d0d064, 0x00002534
	.global Resource_Data49A
Resource_Data49A:
	.incbin "baserom.gba", 0x00d0f598, 0x00000188
	.global Resource_Data49B
Resource_Data49B:
	.incbin "baserom.gba", 0x00d0f720, 0x000019b4
	.global Resource_Data49C
Resource_Data49C:
	.incbin "baserom.gba", 0x00d110d4, 0x0000125c
	.global Resource_Data49D
Resource_Data49D:
	.incbin "baserom.gba", 0x00d12330, 0x00001c94
	.global Resource_Data49E
Resource_Data49E:
	.incbin "baserom.gba", 0x00d13fc4, 0x00000134
	.global Resource_Data49F
Resource_Data49F:
	.incbin "baserom.gba", 0x00d140f8, 0x00001c6c
	.global Resource_Data4A0
Resource_Data4A0:
	.incbin "baserom.gba", 0x00d15d64, 0x000011b8
	.global Resource_Data4A1
Resource_Data4A1:
	.incbin "baserom.gba", 0x00d16f1c, 0x00000714
	.global Resource_Data4A2
Resource_Data4A2:
	.incbin "baserom.gba", 0x00d17630, 0x0000063c
	.global Resource_Data4A3
Resource_Data4A3:
	.incbin "baserom.gba", 0x00d17c6c, 0x00001068
	.global Resource_Data4A4
Resource_Data4A4:
	.incbin "baserom.gba", 0x00d18cd4, 0x00000120
	.global Resource_Data4A5
Resource_Data4A5:
	.incbin "baserom.gba", 0x00d18df4, 0x00001af8
	.global Resource_Data4A6
Resource_Data4A6:
	.incbin "baserom.gba", 0x00d1a8ec, 0x0000140c
	.global Resource_Data4A7
Resource_Data4A7:
	.incbin "baserom.gba", 0x00d1bcf8, 0x00000554
	.global Resource_Data4A8
Resource_Data4A8:
	.incbin "baserom.gba", 0x00d1c24c, 0x0000063c
	.global Resource_Data4A9
Resource_Data4A9:
	.incbin "baserom.gba", 0x00d1c888, 0x00000f68
	.global Resource_Data4AA
Resource_Data4AA:
	.incbin "baserom.gba", 0x00d1d7f0, 0x00000e70
	.global Resource_Data4AB
Resource_Data4AB:
	.incbin "baserom.gba", 0x00d1e660, 0x00001240
	.global Resource_Data4AC
Resource_Data4AC:
	.incbin "baserom.gba", 0x00d1f8a0, 0x000021c0
	.global Resource_Data4AD
Resource_Data4AD:
	.incbin "baserom.gba", 0x00d21a60, 0x00000178
	.global Resource_Data4AE
Resource_Data4AE:
	.incbin "baserom.gba", 0x00d21bd8, 0x00002660
	.global Resource_Data4AF
Resource_Data4AF:
	.incbin "baserom.gba", 0x00d24238, 0x00000194
	.global Resource_Data4B0
Resource_Data4B0:
	.incbin "baserom.gba", 0x00d243cc, 0x00001bb4
	.global Resource_Data4B1
Resource_Data4B1:
	.incbin "baserom.gba", 0x00d25f80, 0x00000160
	.global Resource_Data4B2
Resource_Data4B2:
	.incbin "baserom.gba", 0x00d260e0, 0x00002aa4
	.global Resource_Data4B3
Resource_Data4B3:
	.incbin "baserom.gba", 0x00d28b84, 0x000021b0
	.global Resource_Data4B4
Resource_Data4B4:
	.incbin "baserom.gba", 0x00d2ad34, 0x00001e78
	.global Resource_Data4B5
Resource_Data4B5:
	.incbin "baserom.gba", 0x00d2cbac, 0x00000140
	.global Resource_Data4B6
Resource_Data4B6:
	.incbin "baserom.gba", 0x00d2ccec, 0x00001d24
	.global Resource_Data4B7
Resource_Data4B7:
	.incbin "baserom.gba", 0x00d2ea10, 0x00001c8c
	.global Resource_Data4B8
Resource_Data4B8:
	.incbin "baserom.gba", 0x00d3069c, 0x00001fdc
	.global Resource_Data4B9
Resource_Data4B9:
	.incbin "baserom.gba", 0x00d32678, 0x000015ec
	.global Resource_Data4BA
Resource_Data4BA:
	.incbin "baserom.gba", 0x00d33c64, 0x00000140
	.global Resource_Data4BB
Resource_Data4BB:
	.incbin "baserom.gba", 0x00d33da4, 0x00001fd4
	.global Resource_Data4BC
Resource_Data4BC:
	.incbin "baserom.gba", 0x00d35d78, 0x000000f4
	.global Resource_Data4BD
Resource_Data4BD:
	.incbin "baserom.gba", 0x00d35e6c, 0x000010e0
	.global Resource_Data4BE
Resource_Data4BE:
	.incbin "baserom.gba", 0x00d36f4c, 0x00001ed8
	.global Resource_Data4BF
Resource_Data4BF:
	.incbin "baserom.gba", 0x00d38e24, 0x00000fb8
	.global Resource_Data4C0
Resource_Data4C0:
	.incbin "baserom.gba", 0x00d39ddc, 0x00001f48
	.global Resource_Data4C1
Resource_Data4C1:
	.incbin "baserom.gba", 0x00d3bd24, 0x00001c0c
	.global Resource_Data4C2
Resource_Data4C2:
	.incbin "baserom.gba", 0x00d3d930, 0x0000014c
	.global Resource_Data4C3
Resource_Data4C3:
	.incbin "baserom.gba", 0x00d3da7c, 0x000020bc
	.global Resource_Data4C4
Resource_Data4C4:
	.incbin "baserom.gba", 0x00d3fb38, 0x00002b2c
	.global Resource_Data4C5
Resource_Data4C5:
	.incbin "baserom.gba", 0x00d42664, 0x000015c4
	.global Resource_Data4C6
Resource_Data4C6:
	.incbin "baserom.gba", 0x00d43c28, 0x00002690
	.global Resource_Data4C7
Resource_Data4C7:
	.incbin "baserom.gba", 0x00d462b8, 0x00001720
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
	.global Resource_Data4CC
Resource_Data4CC:
	.incbin "baserom.gba", 0x00d4f480, 0x00001750
	.global Resource_Data4CD
Resource_Data4CD:
	.incbin "baserom.gba", 0x00d50bd0, 0x0000012c
	.global Resource_Data4CE
Resource_Data4CE:
	.incbin "baserom.gba", 0x00d50cfc, 0x00001950
	.global Resource_Data4CF
Resource_Data4CF:
	.incbin "baserom.gba", 0x00d5264c, 0x000016e4
	.global Resource_Data4D0
Resource_Data4D0:
	.incbin "baserom.gba", 0x00d53d30, 0x0000181c
	.global Resource_Data4D1
Resource_Data4D1:
	.incbin "baserom.gba", 0x00d5554c, 0x00000c40
	.global Resource_Data4D2
Resource_Data4D2:
	.incbin "baserom.gba", 0x00d5618c, 0x00001ae4
	.global Resource_Data4D3
Resource_Data4D3:
	.incbin "baserom.gba", 0x00d57c70, 0x00000104
	.global Resource_Data4D4
Resource_Data4D4:
	.incbin "baserom.gba", 0x00d57d74, 0x00000140
	.global Resource_Data4D5
Resource_Data4D5:
	.incbin "baserom.gba", 0x00d57eb4, 0x00000148
	.global Resource_Data4D6
Resource_Data4D6:
	.incbin "baserom.gba", 0x00d57ffc, 0x00001884
	.global Resource_Data4D7
Resource_Data4D7:
	.incbin "baserom.gba", 0x00d59880, 0x000000f8
	.global Resource_Data4D8
Resource_Data4D8:
	.incbin "baserom.gba", 0x00d59978, 0x00002e54
	.global Resource_Data4D9
Resource_Data4D9:
	.incbin "baserom.gba", 0x00d5c7cc, 0x000001a8
	.global Resource_Data4DA
Resource_Data4DA:
	.incbin "baserom.gba", 0x00d5c974, 0x00002c1c
	.global Resource_Data4DB
Resource_Data4DB:
	.incbin "baserom.gba", 0x00d5f590, 0x00002d34
	.global Resource_Data4DC
Resource_Data4DC:
	.incbin "baserom.gba", 0x00d622c4, 0x00001ce8
	.global Resource_Data4DD
Resource_Data4DD:
	.incbin "baserom.gba", 0x00d63fac, 0x00001814
	.global Resource_Data4DE
Resource_Data4DE:
	.incbin "baserom.gba", 0x00d657c0, 0x00002068
	.global Resource_Data4DF
Resource_Data4DF:
	.incbin "baserom.gba", 0x00d67828, 0x00000158
	.global Resource_Data4E0
Resource_Data4E0:
	.incbin "baserom.gba", 0x00d67980, 0x00002378
	.global Resource_Data4E1
Resource_Data4E1:
	.incbin "baserom.gba", 0x00d69cf8, 0x000001cc
	.global Resource_Data4E2
Resource_Data4E2:
	.incbin "baserom.gba", 0x00d69ec4, 0x000025f4
	.global Resource_Data4E3
Resource_Data4E3:
	.incbin "baserom.gba", 0x00d6c4b8, 0x000018d0
	.global Resource_Data4E4
Resource_Data4E4:
	.incbin "baserom.gba", 0x00d6dd88, 0x00002130
	.global Resource_Data4E5
Resource_Data4E5:
	.incbin "baserom.gba", 0x00d6feb8, 0x000019d8
	.global Resource_Data4E6
Resource_Data4E6:
	.incbin "baserom.gba", 0x00d71890, 0x00002834
	.global Resource_Data4E7
Resource_Data4E7:
	.incbin "baserom.gba", 0x00d740c4, 0x000001d4
	.global Resource_Data4E8
Resource_Data4E8:
	.incbin "baserom.gba", 0x00d74298, 0x00002040
	.global Resource_Data4E9
Resource_Data4E9:
	.incbin "baserom.gba", 0x00d762d8, 0x000013f8
	.global Resource_Data4EA
Resource_Data4EA:
	.incbin "baserom.gba", 0x00d776d0, 0x00000b3c
	.global Resource_Data4EB
Resource_Data4EB:
	.incbin "baserom.gba", 0x00d7820c, 0x0000116c
	.global Resource_Data4EC
Resource_Data4EC:
	.incbin "baserom.gba", 0x00d79378, 0x000010ac
	.global Resource_Data4ED
Resource_Data4ED:
	.incbin "baserom.gba", 0x00d7a424, 0x000001ac
	.global Resource_Data4EE
Resource_Data4EE:
	.incbin "baserom.gba", 0x00d7a5d0, 0x00001b0c
	.global Resource_Data4EF
Resource_Data4EF:
	.incbin "baserom.gba", 0x00d7c0dc, 0x00000ba4
	.global Resource_Data4F0
Resource_Data4F0:
	.incbin "baserom.gba", 0x00d7cc80, 0x00000e78
	.global Resource_Data4F1
Resource_Data4F1:
	.incbin "baserom.gba", 0x00d7daf8, 0x00000e84
	.global Resource_Data4F2
Resource_Data4F2:
	.incbin "baserom.gba", 0x00d7e97c, 0x00001650
	.global Resource_Data4F3
Resource_Data4F3:
	.incbin "baserom.gba", 0x00d7ffcc, 0x00000170
	.global Resource_Data4F4
Resource_Data4F4:
	.incbin "baserom.gba", 0x00d8013c, 0x000011f0
	.global Resource_Data4F5
Resource_Data4F5:
	.incbin "baserom.gba", 0x00d8132c, 0x00000160
	.global Resource_Data4F6
Resource_Data4F6:
	.incbin "baserom.gba", 0x00d8148c, 0x00000d54
	.global Resource_Data4F7
Resource_Data4F7:
	.incbin "baserom.gba", 0x00d821e0, 0x00000160
	.global Resource_Data4F8
Resource_Data4F8:
	.incbin "baserom.gba", 0x00d82340, 0x00001490
	.global Resource_Data4F9
Resource_Data4F9:
	.incbin "baserom.gba", 0x00d837d0, 0x00000160
	.global Resource_Data4FA
Resource_Data4FA:
	.incbin "baserom.gba", 0x00d83930, 0x000013e4
	.global Resource_Data4FB
Resource_Data4FB:
	.incbin "baserom.gba", 0x00d84d14, 0x00000160
	.global Resource_Data4FC
Resource_Data4FC:
	.incbin "baserom.gba", 0x00d84e74, 0x00000c6c
	.global Resource_Data4FD
Resource_Data4FD:
	.incbin "baserom.gba", 0x00d85ae0, 0x0000016c
	.global Resource_Data4FE
Resource_Data4FE:
	.incbin "baserom.gba", 0x00d85c4c, 0x00000fc0
	.global Resource_Data4FF
Resource_Data4FF:
	.incbin "baserom.gba", 0x00d86c0c, 0x00000ac8
	.global Resource_Data500
Resource_Data500:
	.incbin "baserom.gba", 0x00d876d4, 0x00000124
	.global Resource_Data501
Resource_Data501:
	.incbin "baserom.gba", 0x00d877f8, 0x00000a78
	.global Resource_Data502
Resource_Data502:
	.incbin "baserom.gba", 0x00d88270, 0x00000150
	.global Resource_Data503
Resource_Data503:
	.incbin "baserom.gba", 0x00d883c0, 0x000026dc
	.global Resource_Data504
Resource_Data504:
	.incbin "baserom.gba", 0x00d8aa9c, 0x00001bd0
	.global Resource_Data505
Resource_Data505:
	.incbin "baserom.gba", 0x00d8c66c, 0x00000f70
	.global Resource_Data506
Resource_Data506:
	.incbin "baserom.gba", 0x00d8d5dc, 0x00000b2c
	.global Resource_Data507
Resource_Data507:
	.incbin "baserom.gba", 0x00d8e108, 0x000019d4
	.global Resource_Data508
Resource_Data508:
	.incbin "baserom.gba", 0x00d8fadc, 0x00000180
	.global Resource_Data509
Resource_Data509:
	.incbin "baserom.gba", 0x00d8fc5c, 0x00001e2c
	.global Resource_Data50A
Resource_Data50A:
	.incbin "baserom.gba", 0x00d91a88, 0x00001e04
	.global Resource_Data50B
Resource_Data50B:
	.incbin "baserom.gba", 0x00d9388c, 0x00001d5c
	.global Resource_Data50C
Resource_Data50C:
	.incbin "baserom.gba", 0x00d955e8, 0x00001af0
	.global Resource_Data50D
Resource_Data50D:
	.incbin "baserom.gba", 0x00d970d8, 0x00001b3c
	.global Resource_Data50E
Resource_Data50E:
	.incbin "baserom.gba", 0x00d98c14, 0x00000150
	.global Resource_Data50F
Resource_Data50F:
	.incbin "baserom.gba", 0x00d98d64, 0x00001bc0
	.global Resource_Data510
Resource_Data510:
	.incbin "baserom.gba", 0x00d9a924, 0x0000015c
	.global Resource_Data511
Resource_Data511:
	.incbin "baserom.gba", 0x00d9aa80, 0x00002544
	.global Resource_Data512
Resource_Data512:
	.incbin "baserom.gba", 0x00d9cfc4, 0x0000199c
	.global Resource_Data513
Resource_Data513:
	.incbin "baserom.gba", 0x00d9e960, 0x00001db8
	.global Resource_Data514
Resource_Data514:
	.incbin "baserom.gba", 0x00da0718, 0x00000b84
	.global Resource_Data515
Resource_Data515:
	.incbin "baserom.gba", 0x00da129c, 0x00001944
	.global Resource_Data516
Resource_Data516:
	.incbin "baserom.gba", 0x00da2be0, 0x00000168
	.global Resource_Data517
Resource_Data517:
	.incbin "baserom.gba", 0x00da2d48, 0x00002690
	.global Resource_Data518
Resource_Data518:
	.incbin "baserom.gba", 0x00da53d8, 0x000020b0
	.global Resource_Data519
Resource_Data519:
	.incbin "baserom.gba", 0x00da7488, 0x000026c0
	.global Resource_Data51A
Resource_Data51A:
	.incbin "baserom.gba", 0x00da9b48, 0x000005dc
	.global Resource_Data51B
Resource_Data51B:
	.incbin "baserom.gba", 0x00daa124, 0x00000f08
	.global Resource_Data51C
Resource_Data51C:
	.incbin "baserom.gba", 0x00dab02c, 0x000001d0
	.global Resource_Data51D
Resource_Data51D:
	.incbin "baserom.gba", 0x00dab1fc, 0x00002678
	.global Resource_Data51E
Resource_Data51E:
	.incbin "baserom.gba", 0x00dad874, 0x00001620
	.global Resource_Data51F
Resource_Data51F:
	.incbin "baserom.gba", 0x00daee94, 0x00001960
	.global Resource_Data520
Resource_Data520:
	.incbin "baserom.gba", 0x00db07f4, 0x00001554
	.global Resource_Data521
Resource_Data521:
	.incbin "baserom.gba", 0x00db1d48, 0x00001894
	.global Resource_Data522
Resource_Data522:
	.incbin "baserom.gba", 0x00db35dc, 0x0000017c
	.global Resource_Data523
Resource_Data523:
	.incbin "baserom.gba", 0x00db3758, 0x00002730
	.global Resource_Data524
Resource_Data524:
	.incbin "baserom.gba", 0x00db5e88, 0x00001618
	.global Resource_Data525
Resource_Data525:
	.incbin "baserom.gba", 0x00db74a0, 0x00001320
	.global Resource_Data526
Resource_Data526:
	.incbin "baserom.gba", 0x00db87c0, 0x00000158
	.global Resource_Data527
Resource_Data527:
	.incbin "baserom.gba", 0x00db8918, 0x000013fc
	.global Resource_Data528
Resource_Data528:
	.incbin "baserom.gba", 0x00db9d14, 0x00001918
	.global Resource_Data529
Resource_Data529:
	.incbin "baserom.gba", 0x00dbb62c, 0x00001a20
	.global Resource_Data52A
Resource_Data52A:
	.incbin "baserom.gba", 0x00dbd04c, 0x00001660
	.global Resource_Data52B
Resource_Data52B:
	.incbin "baserom.gba", 0x00dbe6ac, 0x000018c0
	.global Resource_Data52C
Resource_Data52C:
	.incbin "baserom.gba", 0x00dbff6c, 0x00001950
	.global Resource_Data52D
Resource_Data52D:
	.incbin "baserom.gba", 0x00dc18bc, 0x00001064
	.global Resource_Data52E
Resource_Data52E:
	.incbin "baserom.gba", 0x00dc2920, 0x00000168
	.global Resource_Data52F
Resource_Data52F:
	.incbin "baserom.gba", 0x00dc2a88, 0x00002624
	.global Resource_Data530
Resource_Data530:
	.incbin "baserom.gba", 0x00dc50ac, 0x00000144
	.global Resource_Data531
Resource_Data531:
	.incbin "baserom.gba", 0x00dc51f0, 0x000018bc
	.global Resource_Data532
Resource_Data532:
	.incbin "baserom.gba", 0x00dc6aac, 0x00002f64
	.global Resource_Data533
Resource_Data533:
	.incbin "baserom.gba", 0x00dc9a10, 0x00000178
	.global Resource_Data534
Resource_Data534:
	.incbin "baserom.gba", 0x00dc9b88, 0x00002820
	.global Resource_Data535
Resource_Data535:
	.incbin "baserom.gba", 0x00dcc3a8, 0x00002664
	.global Resource_Data536
Resource_Data536:
	.incbin "baserom.gba", 0x00dcea0c, 0x000010e4
	.global Resource_Data537
Resource_Data537:
	.incbin "baserom.gba", 0x00dcfaf0, 0x00001210
	.global Resource_Data538
Resource_Data538:
	.incbin "baserom.gba", 0x00dd0d00, 0x00002494
	.global Resource_Data539
Resource_Data539:
	.incbin "baserom.gba", 0x00dd3194, 0x00000194
	.global Resource_Data53A
Resource_Data53A:
	.incbin "baserom.gba", 0x00dd3328, 0x000001d4
	.global Resource_Data53B
Resource_Data53B:
	.incbin "baserom.gba", 0x00dd34fc, 0x00003320
	.global Resource_Data53C
Resource_Data53C:
	.incbin "baserom.gba", 0x00dd681c, 0x000001d8
	.global Resource_Data53D
Resource_Data53D:
	.incbin "baserom.gba", 0x00dd69f4, 0x00001cf8
	.global Resource_Data53E
Resource_Data53E:
	.incbin "baserom.gba", 0x00dd86ec, 0x0000167c
	.global Resource_Data53F
Resource_Data53F:
	.incbin "baserom.gba", 0x00dd9d68, 0x00001754
	.global Resource_Data540
Resource_Data540:
	.incbin "baserom.gba", 0x00ddb4bc, 0x00000dc0
	.global Resource_Data541
Resource_Data541:
	.incbin "baserom.gba", 0x00ddc27c, 0x000001cc
	.global Resource_Data542
Resource_Data542:
	.incbin "baserom.gba", 0x00ddc448, 0x00001978
	.global Resource_Data543
Resource_Data543:
	.incbin "baserom.gba", 0x00ddddc0, 0x00000138
	.global Resource_Data544
Resource_Data544:
	.incbin "baserom.gba", 0x00dddef8, 0x00001a0c
	.global Resource_Data545
Resource_Data545:
	.incbin "baserom.gba", 0x00ddf904, 0x00000138
	.global Resource_Data546
Resource_Data546:
	.incbin "baserom.gba", 0x00ddfa3c, 0x00002270
	.global Resource_Data547
Resource_Data547:
	.incbin "baserom.gba", 0x00de1cac, 0x00001368
	.global Resource_Data548
Resource_Data548:
	.incbin "baserom.gba", 0x00de3014, 0x00000c88
	.global Resource_Data549
Resource_Data549:
	.incbin "baserom.gba", 0x00de3c9c, 0x00000140
	.global Resource_Data54A
Resource_Data54A:
	.incbin "baserom.gba", 0x00de3ddc, 0x00001dc4
	.global Resource_Data54B
Resource_Data54B:
	.incbin "baserom.gba", 0x00de5ba0, 0x00000174
	.global Resource_Data54C
Resource_Data54C:
	.incbin "baserom.gba", 0x00de5d14, 0x00002468
	.global Resource_Data54D
Resource_Data54D:
	.incbin "baserom.gba", 0x00de817c, 0x0000199c
	.global Resource_Data54E
Resource_Data54E:
	.incbin "baserom.gba", 0x00de9b18, 0x00001e34
	.global Resource_Data54F
Resource_Data54F:
	.incbin "baserom.gba", 0x00deb94c, 0x00000ab8
	.global Resource_Data550
Resource_Data550:
	.incbin "baserom.gba", 0x00dec404, 0x000018c8
	.global Resource_Data551
Resource_Data551:
	.incbin "baserom.gba", 0x00dedccc, 0x00000154
	.global Resource_Data552
Resource_Data552:
	.incbin "baserom.gba", 0x00dede20, 0x00002ccc
	.global Resource_Data553
Resource_Data553:
	.incbin "baserom.gba", 0x00df0aec, 0x000001dc
	.global Resource_Data554
Resource_Data554:
	.incbin "baserom.gba", 0x00df0cc8, 0x00001b34
	.global Resource_Data555
Resource_Data555:
	.incbin "baserom.gba", 0x00df27fc, 0x000016c8
	.global Resource_Data556
Resource_Data556:
	.incbin "baserom.gba", 0x00df3ec4, 0x00001a04
	.global Resource_Data557
Resource_Data557:
	.incbin "baserom.gba", 0x00df58c8, 0x000011bc
	.global Resource_Data558
Resource_Data558:
	.incbin "baserom.gba", 0x00df6a84, 0x000001e8
	.global Resource_Data559
Resource_Data559:
	.incbin "baserom.gba", 0x00df6c6c, 0x00000190
	.global Resource_Data55A
Resource_Data55A:
	.incbin "baserom.gba", 0x00df6dfc, 0x00000d88
	.global Resource_Data55B
Resource_Data55B:
	.incbin "baserom.gba", 0x00df7b84, 0x00000194
	.global Resource_Data55C
Resource_Data55C:
	.incbin "baserom.gba", 0x00df7d18, 0x000020b4
	.global Resource_Data55D
Resource_Data55D:
	.incbin "baserom.gba", 0x00df9dcc, 0x0000131c
	.global Resource_Data55E
Resource_Data55E:
	.incbin "baserom.gba", 0x00dfb0e8, 0x000011cc
	.global Resource_Data55F
Resource_Data55F:
	.incbin "baserom.gba", 0x00dfc2b4, 0x00001558
	.global Resource_Data560
Resource_Data560:
	.incbin "baserom.gba", 0x00dfd80c, 0x000010b8
	.global Resource_Data561
Resource_Data561:
	.incbin "baserom.gba", 0x00dfe8c4, 0x000011c4
	.global Resource_Data562
Resource_Data562:
	.incbin "baserom.gba", 0x00dffa88, 0x000023c8
	.global Resource_Data563
Resource_Data563:
	.incbin "baserom.gba", 0x00e01e50, 0x00000140
	.global Resource_Data564
Resource_Data564:
	.incbin "baserom.gba", 0x00e01f90, 0x000028bc
	.global Resource_Data565
Resource_Data565:
	.incbin "baserom.gba", 0x00e0484c, 0x00001acc
	.global Resource_Data566
Resource_Data566:
	.incbin "baserom.gba", 0x00e06318, 0x00000720
	.global Resource_Data567
Resource_Data567:
	.incbin "baserom.gba", 0x00e06a38, 0x000013c0
	.global Resource_Data568
Resource_Data568:
	.incbin "baserom.gba", 0x00e07df8, 0x00001098
	.global Resource_Data569
Resource_Data569:
	.incbin "baserom.gba", 0x00e08e90, 0x00000f64
	.global Resource_Data56A
Resource_Data56A:
	.incbin "baserom.gba", 0x00e09df4, 0x00000144
	.global Resource_Data56B
Resource_Data56B:
	.incbin "baserom.gba", 0x00e09f38, 0x00001fd8
	.global Resource_Data56C
Resource_Data56C:
	.incbin "baserom.gba", 0x00e0bf10, 0x00001080
	.global Resource_Data56D
Resource_Data56D:
	.incbin "baserom.gba", 0x00e0cf90, 0x00001084
	.global Resource_Data56E
Resource_Data56E:
	.incbin "baserom.gba", 0x00e0e014, 0x00000b70
	.global Resource_Data56F
Resource_Data56F:
	.incbin "baserom.gba", 0x00e0eb84, 0x00000f50
	.global Resource_Data570
Resource_Data570:
	.incbin "baserom.gba", 0x00e0fad4, 0x00000180
	.global Resource_Data571
Resource_Data571:
	.incbin "baserom.gba", 0x00e0fc54, 0x0000167c
	.global Resource_Data572
Resource_Data572:
	.incbin "baserom.gba", 0x00e112d0, 0x00001edc
	.global Resource_Data573
Resource_Data573:
	.incbin "baserom.gba", 0x00e131ac, 0x00001634
	.global Resource_Data574
Resource_Data574:
	.incbin "baserom.gba", 0x00e147e0, 0x00000140
	.global Resource_Data575
Resource_Data575:
	.incbin "baserom.gba", 0x00e14920, 0x0000294c
	.global Resource_Data576
Resource_Data576:
	.incbin "baserom.gba", 0x00e1726c, 0x0000011c
	.global Resource_Data577
Resource_Data577:
	.incbin "baserom.gba", 0x00e17388, 0x00002efc
	.global Resource_Data578
Resource_Data578:
	.incbin "baserom.gba", 0x00e1a284, 0x00002a48
	.global Resource_Data579
Resource_Data579:
	.incbin "baserom.gba", 0x00e1cccc, 0x00002484
	.global Resource_Data57A
Resource_Data57A:
	.incbin "baserom.gba", 0x00e1f150, 0x00000ac4
	.global Resource_Data57B
Resource_Data57B:
	.incbin "baserom.gba", 0x00e1fc14, 0x00000fdc
	.global Resource_Data57C
Resource_Data57C:
	.incbin "baserom.gba", 0x00e20bf0, 0x00000118
	.global Resource_Data57D
Resource_Data57D:
	.incbin "baserom.gba", 0x00e20d08, 0x00000850
	.global Resource_Data57E
Resource_Data57E:
	.incbin "baserom.gba", 0x00e21558, 0x000010ec
	.global Resource_Data57F
Resource_Data57F:
	.incbin "baserom.gba", 0x00e22644, 0x0000017c
	.global Resource_Data580
Resource_Data580:
	.incbin "baserom.gba", 0x00e227c0, 0x00001f40
	.global Resource_Data581
Resource_Data581:
	.incbin "baserom.gba", 0x00e24700, 0x00001474
	.global Resource_Data582
Resource_Data582:
	.incbin "baserom.gba", 0x00e25b74, 0x00001cb8
	.global Resource_Data583
Resource_Data583:
	.incbin "baserom.gba", 0x00e2782c, 0x0000054c
	.global Resource_Data584
Resource_Data584:
	.incbin "baserom.gba", 0x00e27d78, 0x000013bc
	.global Resource_Data585
Resource_Data585:
	.incbin "baserom.gba", 0x00e29134, 0x00001948
	.global Resource_Data586
Resource_Data586:
	.incbin "baserom.gba", 0x00e2aa7c, 0x000016b0
	.global Resource_Data587
Resource_Data587:
	.incbin "baserom.gba", 0x00e2c12c, 0x000018ac
	.global Resource_Data588
Resource_Data588:
	.incbin "baserom.gba", 0x00e2d9d8, 0x000012a8
	.global Resource_Data589
Resource_Data589:
	.incbin "baserom.gba", 0x00e2ec80, 0x000017d4
	.global Resource_Data58A
Resource_Data58A:
	.incbin "baserom.gba", 0x00e30454, 0x00001194
	.global Resource_Data58B
Resource_Data58B:
	.incbin "baserom.gba", 0x00e315e8, 0x000015dc
	.global Resource_Data58C
Resource_Data58C:
	.incbin "baserom.gba", 0x00e32bc4, 0x00001d18
	.global Resource_Data58D
Resource_Data58D:
	.incbin "baserom.gba", 0x00e348dc, 0x00000140
	.global Resource_Data58E
Resource_Data58E:
	.incbin "baserom.gba", 0x00e34a1c, 0x00001f24
	.global Resource_Data58F
Resource_Data58F:
	.incbin "baserom.gba", 0x00e36940, 0x00000128
	.global Resource_Data590
Resource_Data590:
	.incbin "baserom.gba", 0x00e36a68, 0x000020b0
	.global Resource_Data591
Resource_Data591:
	.incbin "baserom.gba", 0x00e38b18, 0x00000120
	.global Resource_Data592
Resource_Data592:
	.incbin "baserom.gba", 0x00e38c38, 0x000025d0
	.global Resource_Data593
Resource_Data593:
	.incbin "baserom.gba", 0x00e3b208, 0x00002314
	.global Resource_Data594
Resource_Data594:
	.incbin "baserom.gba", 0x00e3d51c, 0x00000874
	.global Resource_Data595
Resource_Data595:
	.incbin "baserom.gba", 0x00e3dd90, 0x00000140
	.global Resource_Data596
Resource_Data596:
	.incbin "baserom.gba", 0x00e3ded0, 0x000030b8
	.global Resource_Data597
Resource_Data597:
	.incbin "baserom.gba", 0x00e40f88, 0x000001e8
	.global Resource_Data598
Resource_Data598:
	.incbin "baserom.gba", 0x00e41170, 0x000014fc
	.global Resource_Data599
Resource_Data599:
	.incbin "baserom.gba", 0x00e4266c, 0x00001bc8
	.global Resource_Data59A
Resource_Data59A:
	.incbin "baserom.gba", 0x00e44234, 0x00001b7c
	.global Resource_Data59B
Resource_Data59B:
	.incbin "baserom.gba", 0x00e45db0, 0x0000044c
	.global Resource_Data59C
Resource_Data59C:
	.incbin "baserom.gba", 0x00e461fc, 0x00002160
	.global Resource_Data59D
Resource_Data59D:
	.incbin "baserom.gba", 0x00e4835c, 0x00000118
	.global Resource_Data59E
Resource_Data59E:
	.incbin "baserom.gba", 0x00e48474, 0x00000e2c
	.global Resource_Data59F
Resource_Data59F:
	.incbin "baserom.gba", 0x00e492a0, 0x00000160
	.global Resource_Data5A0
Resource_Data5A0:
	.incbin "baserom.gba", 0x00e49400, 0x000026f4
	.global Resource_Data5A1
Resource_Data5A1:
	.incbin "baserom.gba", 0x00e4baf4, 0x00002174
	.global Resource_Data5A2
Resource_Data5A2:
	.incbin "baserom.gba", 0x00e4dc68, 0x000026c0
	.global Resource_Data5A3
Resource_Data5A3:
	.incbin "baserom.gba", 0x00e50328, 0x000005dc
	.global Resource_Data5A4
Resource_Data5A4:
	.incbin "baserom.gba", 0x00e50904, 0x00001e30
	.global Resource_Data5A5
Resource_Data5A5:
	.incbin "baserom.gba", 0x00e52734, 0x00000164
	.global Resource_Data5A6
Resource_Data5A6:
	.incbin "baserom.gba", 0x00e52898, 0x000017e0
	.global Resource_Data5A7
Resource_Data5A7:
	.incbin "baserom.gba", 0x00e54078, 0x00000164
	.global Resource_Data5A8
Resource_Data5A8:
	.incbin "baserom.gba", 0x00e541dc, 0x000027f8
	.global Resource_Data5A9
Resource_Data5A9:
	.incbin "baserom.gba", 0x00e569d4, 0x000001b8
	.global Resource_Data5AA
Resource_Data5AA:
	.incbin "baserom.gba", 0x00e56b8c, 0x00000188
	.global Resource_Data5AB
Resource_Data5AB:
	.incbin "baserom.gba", 0x00e56d14, 0x00001824
	.global Resource_Data5AC
Resource_Data5AC:
	.incbin "baserom.gba", 0x00e58538, 0x00000124
	.global Resource_Data5AD
Resource_Data5AD:
	.incbin "baserom.gba", 0x00e5865c, 0x00000ac4
	.global Resource_Data5AE
Resource_Data5AE:
	.incbin "baserom.gba", 0x00e59120, 0x00001538
	.global Resource_Data5AF
Resource_Data5AF:
	.incbin "baserom.gba", 0x00e5a658, 0x0000019c
	.global Resource_Data5B0
Resource_Data5B0:
	.incbin "baserom.gba", 0x00e5a7f4, 0x000022ac
	.global Resource_Data5B1
Resource_Data5B1:
	.incbin "baserom.gba", 0x00e5caa0, 0x000021c8
	.global Resource_Data5B2
Resource_Data5B2:
	.incbin "baserom.gba", 0x00e5ec68, 0x000016fc
	.global Resource_Data5B3
Resource_Data5B3:
	.incbin "baserom.gba", 0x00e60364, 0x00000b24
	.global Resource_Data5B4
Resource_Data5B4:
	.incbin "baserom.gba", 0x00e60e88, 0x00001298
	.global Resource_Data5B5
Resource_Data5B5:
	.incbin "baserom.gba", 0x00e62120, 0x00001540
	.global Resource_Data5B6
Resource_Data5B6:
	.incbin "baserom.gba", 0x00e63660, 0x00000eac
	.global Resource_Data5B7
Resource_Data5B7:
	.incbin "baserom.gba", 0x00e6450c, 0x000016b0
	.global Resource_Data5B8
Resource_Data5B8:
	.incbin "baserom.gba", 0x00e65bbc, 0x00000170
	.global Resource_Data5B9
Resource_Data5B9:
	.incbin "baserom.gba", 0x00e65d2c, 0x000023a8
	.global Resource_Data5BA
Resource_Data5BA:
	.incbin "baserom.gba", 0x00e680d4, 0x000006e8
	.global Resource_Data5BB
Resource_Data5BB:
	.incbin "baserom.gba", 0x00e687bc, 0x00000bec
	.global Resource_Data5BC
Resource_Data5BC:
	.incbin "baserom.gba", 0x00e693a8, 0x00000ac4
	.global Resource_Data5BD
Resource_Data5BD:
	.incbin "baserom.gba", 0x00e69e6c, 0x000015b0
	.global Resource_Data5BE
Resource_Data5BE:
	.incbin "baserom.gba", 0x00e6b41c, 0x000001b4
	.global Resource_Data5BF
Resource_Data5BF:
	.incbin "baserom.gba", 0x00e6b5d0, 0x00002240
	.global Resource_Data5C0
Resource_Data5C0:
	.incbin "baserom.gba", 0x00e6d810, 0x00000b24
	.global Resource_Data5C1
Resource_Data5C1:
	.incbin "baserom.gba", 0x00e6e334, 0x0000129c
	.global Resource_Data5C2
Resource_Data5C2:
	.incbin "baserom.gba", 0x00e6f5d0, 0x00001a70
	.global Resource_Data5C3
Resource_Data5C3:
	.incbin "baserom.gba", 0x00e71040, 0x00001788
	.global Resource_Data5C4
Resource_Data5C4:
	.incbin "baserom.gba", 0x00e727c8, 0x00001a7c
	.global Resource_Data5C5
Resource_Data5C5:
	.incbin "baserom.gba", 0x00e74244, 0x0000167c
	.global Resource_Data5C6
Resource_Data5C6:
	.incbin "baserom.gba", 0x00e758c0, 0x000010d0
	.global Resource_Data5C7
Resource_Data5C7:
	.incbin "baserom.gba", 0x00e76990, 0x000015bc
	.global Resource_Data5C8
Resource_Data5C8:
	.incbin "baserom.gba", 0x00e77f4c, 0x00000e78
	.global Resource_Data5C9
Resource_Data5C9:
	.incbin "baserom.gba", 0x00e78dc4, 0x000007d0
	.global Resource_Data5CA
Resource_Data5CA:
	.incbin "baserom.gba", 0x00e79594, 0x00000ea0
	.global Resource_Data5CB
Resource_Data5CB:
	.incbin "baserom.gba", 0x00e7a434, 0x00000e58
	.global Resource_Data5CC
Resource_Data5CC:
	.incbin "baserom.gba", 0x00e7b28c, 0x00000ea8
	.global Resource_Data5CD
Resource_Data5CD:
	.incbin "baserom.gba", 0x00e7c134, 0x0000012c
	.global Resource_Data5CE
Resource_Data5CE:
	.incbin "baserom.gba", 0x00e7c260, 0x00000178
	.global Resource_Data5CF
Resource_Data5CF:
	.incbin "baserom.gba", 0x00e7c3d8, 0x00000178
	.global Resource_Data5D0
Resource_Data5D0:
	.incbin "baserom.gba", 0x00e7c550, 0x00000198
	.global Resource_Data5D1
Resource_Data5D1:
	.incbin "baserom.gba", 0x00e7c6e8, 0x00000190
	.global Resource_Data5D2
Resource_Data5D2:
	.incbin "baserom.gba", 0x00e7c878, 0x000022ac
	.global Resource_Data5D3
Resource_Data5D3:
	.incbin "baserom.gba", 0x00e7eb24, 0x00000118
	.global Resource_Data5D4
Resource_Data5D4:
	.incbin "baserom.gba", 0x00e7ec3c, 0x00001e04
	.global Resource_Data5D5
Resource_Data5D5:
	.incbin "baserom.gba", 0x00e80a40, 0x00001ac4
	.global Resource_Data5D6
Resource_Data5D6:
	.incbin "baserom.gba", 0x00e82504, 0x00000728
	.global Resource_Data5D7
Resource_Data5D7:
	.incbin "baserom.gba", 0x00e82c2c, 0x000013fc
	.global Resource_Data5D8
Resource_Data5D8:
	.incbin "baserom.gba", 0x00e84028, 0x00000ed4
	.global Resource_Data5D9
Resource_Data5D9:
	.incbin "baserom.gba", 0x00e84efc, 0x00000144
	.global Resource_Data5DA
Resource_Data5DA:
	.incbin "baserom.gba", 0x00e85040, 0x000023b0
	.global Resource_Data5DB
Resource_Data5DB:
	.incbin "baserom.gba", 0x00e873f0, 0x00001c48
	.global Resource_Data5DC
Resource_Data5DC:
	.incbin "baserom.gba", 0x00e89038, 0x00000be8
	.global Resource_Data5DD
Resource_Data5DD:
	.incbin "baserom.gba", 0x00e89c20, 0x000003fc
	.global Resource_Data5DE
Resource_Data5DE:
	.incbin "baserom.gba", 0x00e8a01c, 0x000014a0
	.global Resource_Data5DF
Resource_Data5DF:
	.incbin "baserom.gba", 0x00e8b4bc, 0x00000154
	.global Resource_Data5E0
Resource_Data5E0:
	.incbin "baserom.gba", 0x00e8b610, 0x000022e0
	.global Resource_Data5E1
Resource_Data5E1:
	.incbin "baserom.gba", 0x00e8d8f0, 0x0000166c
	.global Resource_Data5E2
Resource_Data5E2:
	.incbin "baserom.gba", 0x00e8ef5c, 0x00000cf4
	.global Resource_Data5E3
Resource_Data5E3:
	.incbin "baserom.gba", 0x00e8fc50, 0x00000778
	.global Resource_Data5E4
Resource_Data5E4:
	.incbin "baserom.gba", 0x00e903c8, 0x000014d8
	.global Resource_Data5E5
Resource_Data5E5:
	.incbin "baserom.gba", 0x00e918a0, 0x00000154
	.global Resource_Data5E6
Resource_Data5E6:
	.incbin "baserom.gba", 0x00e919f4, 0x000022e0
	.global Resource_Data5E7
Resource_Data5E7:
	.incbin "baserom.gba", 0x00e93cd4, 0x0000172c
	.global Resource_Data5E8
Resource_Data5E8:
	.incbin "baserom.gba", 0x00e95400, 0x00000cf4
	.global Resource_Data5E9
Resource_Data5E9:
	.incbin "baserom.gba", 0x00e960f4, 0x000004e4
	.global Resource_Data5EA
Resource_Data5EA:
	.incbin "baserom.gba", 0x00e965d8, 0x0000180c
	.global Resource_Data5EB
Resource_Data5EB:
	.incbin "baserom.gba", 0x00e97de4, 0x00000150
	.global Resource_Data5EC
Resource_Data5EC:
	.incbin "baserom.gba", 0x00e97f34, 0x00001284
	.global Resource_Data5ED
Resource_Data5ED:
	.incbin "baserom.gba", 0x00e991b8, 0x00001488
	.global Resource_Data5EE
Resource_Data5EE:
	.incbin "baserom.gba", 0x00e9a640, 0x00000168
	.global Resource_Data5EF
Resource_Data5EF:
	.incbin "baserom.gba", 0x00e9a7a8, 0x00002fc0
	.global Resource_Data5F0
Resource_Data5F0:
	.incbin "baserom.gba", 0x00e9d768, 0x00001270
	.global Resource_Data5F1
Resource_Data5F1:
	.incbin "baserom.gba", 0x00e9e9d8, 0x000010c8
	.global Resource_Data5F2
Resource_Data5F2:
	.incbin "baserom.gba", 0x00e9faa0, 0x00001ac4
	.global Resource_Data5F3
Resource_Data5F3:
	.incbin "baserom.gba", 0x00ea1564, 0x000009e8
	.global Resource_Data5F4
Resource_Data5F4:
	.incbin "baserom.gba", 0x00ea1f4c, 0x00000164
	.global Resource_Data5F5
Resource_Data5F5:
	.incbin "baserom.gba", 0x00ea20b0, 0x0000083c
	.global Resource_Data5F6
Resource_Data5F6:
	.incbin "baserom.gba", 0x00ea28ec, 0x00000174
	.global Resource_Data5F7
Resource_Data5F7:
	.incbin "baserom.gba", 0x00ea2a60, 0x00000af4
	.global Resource_Data5F8
Resource_Data5F8:
	.incbin "baserom.gba", 0x00ea3554, 0x00000114
	.global Resource_Data5F9
Resource_Data5F9:
	.incbin "baserom.gba", 0x00ea3668, 0x00000f08
	.global Resource_Data5FA
Resource_Data5FA:
	.incbin "baserom.gba", 0x00ea4570, 0x00000124
	.global Resource_Data5FB
Resource_Data5FB:
	.incbin "baserom.gba", 0x00ea4694, 0x00002afc
	.global Resource_Data5FC
Resource_Data5FC:
	.incbin "baserom.gba", 0x00ea7190, 0x00000df8
	.global Resource_Data5FD
Resource_Data5FD:
	.incbin "baserom.gba", 0x00ea7f88, 0x00000764
	.global Resource_Data5FE
Resource_Data5FE:
	.incbin "baserom.gba", 0x00ea86ec, 0x0000183c
	.global Resource_Data5FF
Resource_Data5FF:
	.incbin "baserom.gba", 0x00ea9f28, 0x00001330
	.global Resource_Data600
Resource_Data600:
	.incbin "baserom.gba", 0x00eab258, 0x00000154
	.global Resource_Data601
Resource_Data601:
	.incbin "baserom.gba", 0x00eab3ac, 0x00000bb0
	.global Resource_Data602
Resource_Data602:
	.incbin "baserom.gba", 0x00eabf5c, 0x0000015c
	.global Resource_Data603
Resource_Data603:
	.incbin "baserom.gba", 0x00eac0b8, 0x00001680
	.global Resource_Data604
Resource_Data604:
	.incbin "baserom.gba", 0x00ead738, 0x00001160
	.global Resource_Data605
Resource_Data605:
	.incbin "baserom.gba", 0x00eae898, 0x0000111c
	.global Resource_Data606
Resource_Data606:
	.incbin "baserom.gba", 0x00eaf9b4, 0x00000208
	.global Resource_Data607
Resource_Data607:
	.incbin "baserom.gba", 0x00eafbbc, 0x00000cf4
	.global Resource_Data608
Resource_Data608:
	.incbin "baserom.gba", 0x00eb08b0, 0x00000150
	.global Resource_Data609
Resource_Data609:
	.incbin "baserom.gba", 0x00eb0a00, 0x00002654
	.global Resource_Data60A
Resource_Data60A:
	.incbin "baserom.gba", 0x00eb3054, 0x00001774
	.global Resource_Data60B
Resource_Data60B:
	.incbin "baserom.gba", 0x00eb47c8, 0x000012bc
	.global Resource_Data60C
Resource_Data60C:
	.incbin "baserom.gba", 0x00eb5a84, 0x00000844
	.global Resource_Data60D
Resource_Data60D:
	.incbin "baserom.gba", 0x00eb62c8, 0x0000060c
	.global Resource_Data60E
Resource_Data60E:
	.incbin "baserom.gba", 0x00eb68d4, 0x00000028
	.global Resource_Data60F
Resource_Data60F:
	.incbin "baserom.gba", 0x00eb68fc, 0x00000410
	.global Resource_Data610
Resource_Data610:
	.incbin "baserom.gba", 0x00eb6d0c, 0x00000140
	.global Resource_Data611
Resource_Data611:
	.incbin "baserom.gba", 0x00eb6e4c, 0x00000140
	.global Resource_Data612
Resource_Data612:
	.incbin "baserom.gba", 0x00eb6f8c, 0x00000140
	.global Resource_Data613
Resource_Data613:
	.incbin "baserom.gba", 0x00eb70cc, 0x00000b64
	.global Resource_Data614
Resource_Data614:
	.incbin "baserom.gba", 0x00eb7c30, 0x00000150
	.global Resource_Data615
Resource_Data615:
	.incbin "baserom.gba", 0x00eb7d80, 0x00002654
	.global Resource_Data616
Resource_Data616:
	.incbin "baserom.gba", 0x00eba3d4, 0x00001774
	.global Resource_Data617
Resource_Data617:
	.incbin "baserom.gba", 0x00ebbb48, 0x000012bc
	.global Resource_Data618
Resource_Data618:
	.incbin "baserom.gba", 0x00ebce04, 0x00000844
	.global Resource_Data619
Resource_Data619:
	.incbin "baserom.gba", 0x00ebd648, 0x000005a0
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
	.global Resource_Data61F
Resource_Data61F:
	.incbin "baserom.gba", 0x00ebe104, 0x00000718
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
	.global Resource_Data625
Resource_Data625:
	.incbin "baserom.gba", 0x00ebed38, 0x00000bc4
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
	.global Resource_Data62B
Resource_Data62B:
	.incbin "baserom.gba", 0x00ebfe18, 0x000005d0
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
	.global Resource_Data631
Resource_Data631:
	.incbin "baserom.gba", 0x00ec0904, 0x00000980
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
	.global Resource_Data637
Resource_Data637:
	.incbin "baserom.gba", 0x00ec17a0, 0x0000042c
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
	.global Resource_Data63D
Resource_Data63D:
	.incbin "baserom.gba", 0x00ec20e8, 0x0000062c
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
	.global Resource_Data643
Resource_Data643:
	.incbin "baserom.gba", 0x00ec2c30, 0x0000072c
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
