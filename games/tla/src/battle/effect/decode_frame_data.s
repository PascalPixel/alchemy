.syntax unified
	.section .data,"aw",%progbits
	.arm
	.4byte	0x00000000
	.4byte	BattleFx_DecodeFrameUnusedBase
	.4byte	BattleFx_DecodeFrameTranslationBase
	.global BattleFx_DecodeFrameCopyEnd
BattleFx_DecodeFrameCopyEnd:
	.4byte	0, 0, 0, 0, 0, 0, 0
	.4byte	0, 0, 0, 0, 0, 0, 0, 0
	.4byte	0, 0, 0, 0, 0, 0, 0, 0
	.4byte	0, 0, 0, 0, 0, 0, 0, 0
	.4byte	0, 0, 0, 0, 0, 0, 0, 0
	.4byte	0, 0, 0, 0, 0, 0, 0, 0
	.4byte	0, 0, 0, 0, 0, 0, 0, 0
	.4byte	0
