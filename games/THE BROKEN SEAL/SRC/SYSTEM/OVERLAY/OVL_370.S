	.include "games/COMMON/INCLUDE/GAME/ED_ASM.H"
.syntax unified
	.thumb
	.section .rodata.x020094d4,"a",%progbits
	.global Clear_CodeSequence
Clear_CodeSequence:
	.4byte 0x00040004
	.4byte 0x00000004
	.global Clear_ExtraCodeSequence
Clear_ExtraCodeSequence:
	.4byte 0x00800040
	.4byte 0x00800040
	.4byte 0x00100020
	.4byte 0x00100020
	.4byte 0x00100040
	.4byte 0x00200080
	.4byte 0x00040040
	.4byte 0x00000000
	.4byte 0x8fc22100
	.4byte 0x250d09f0
	.4byte 0x3f08b7c2
	.4byte 0xf0c97c32
	.4byte 0x0ca7c327
	.4byte 0x907c32bf
	.4byte 0x5fc3d5f8
	.4byte 0xfc3d9f0f
	.4byte 0xb9ddf0f6
	.4byte 0x1f0c3fc2
	.4byte 0xf0c4fc31
	.4byte 0xd45fc315
	.4byte 0xfc7c32f5
	.4byte 0x78652df0
	.4byte 0x6d4b4f21
	.4byte 0x0ccfc331
	.4byte 0xf7ef535f
	.4byte 0xc3e304f0
	.4byte 0x5e7f0f97
	.4byte 0x6df51936
	.4byte 0x0751d1d4
	.4byte 0xc79c1b9b
	.4byte 0xc339f0cd
	.4byte 0x33df0cef
	.4byte 0xe9bd4ffc
	.4byte 0xd1d7ac75
	.4byte 0x863e025e
	.4byte 0x7fa5ab0f
	.4byte 0x1a7efff8
	.4byte 0x87821ebc
	.4byte 0x78be1e1f
	.4byte 0x93e1e3f8
	.4byte 0xb97bc1cb
	.4byte 0x7ebe1f9a
	.4byte 0x031efbf8
	.4byte 0x9f1e98b6
	.4byte 0xe1e59f81
	.4byte 0x1e7f879b
	.4byte 0xe9f87a3e
	.4byte 0x68ebf37a
	.4byte 0x0fbf63a8
	.4byte 0x9f9f83c1
	.4byte 0x7fc1e6f8
	.4byte 0xe60cc1f3
	.4byte 0x65f0d8c1
	.4byte 0x9f0d9fc3
	.4byte 0xc95afc36
	.4byte 0xe5fc3950
	.4byte 0x6fc399f0
	.4byte 0x6a59df0e
	.4byte 0x351f0d3c
	.4byte 0x55f0d4fc
	.4byte 0x09f55fc3
	.4byte 0xb41e6208
	.4byte 0x7c36ff0d
	.4byte 0xc373f0dc
	.4byte 0x39fbd5d7
	.4byte 0xa3f0e87c
	.4byte 0x7f0e97c3
	.4byte 0xd564025a
	.4byte 0x574755b7
	.4byte 0x676f41e7
	.4byte 0x37707982
	.4byte 0x7bf0de7c
	.4byte 0xff0df7c3
	.4byte 0x1d6a6f57
	.4byte 0xd6b475ab
	.4byte 0xe027d5fe
	.4byte 0x1da27e1f
	.4byte 0x083c79ce
	.4byte 0xf8707e1c
	.4byte 0x870fe1c2
	.4byte 0x5e072c4f
	.4byte 0x7e1d8f87
	.4byte 0xe1daf876
	.4byte 0xd410fb6f
	.4byte 0xc1a818f2
	.4byte 0xbe1c583c
	.4byte 0xe1c7f871
	.4byte 0xac9f8723
	.4byte 0xa68eb72f
	.4byte 0x14fb763a
	.4byte 0x90ed8394
	.4byte 0x270fc791
	.4byte 0x09f0a7c2
	.4byte 0xfbc22b0d
	.4byte 0x00000000
	.global Clear_ScriptTable
Clear_ScriptTable:
	.4byte 0xffff0000
	.4byte 0x00000000
	.4byte 0x40000000
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Clear_MessageTable
Clear_MessageTable:
	.4byte 0x000001ff
	.global Clear_ActorTable
Clear_ActorTable:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Clear_EffectTable
Clear_EffectTable:
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Clear_BlendFrame
Clear_BlendFrame:
	.2byte 0x0000
	.global Clear_CodeUnlocked
Clear_CodeUnlocked:
	.2byte 0x0000
	.global Clear_ExtraCodeUnlocked
Clear_ExtraCodeUnlocked:
	.2byte 0x0000
	.global Clear_CodeProgress
Clear_CodeProgress:
	.2byte 0x0000
	.global Clear_ExtraCodeProgress
Clear_ExtraCodeProgress:
	.2byte 0x0000
	.global Clear_CodeHeld
Clear_CodeHeld:
	.2byte 0x0000
	.global Clear_ExtraCodeHeld
Clear_ExtraCodeHeld:
	.4byte 0x00000000
	.global Clear_PasswordMembers
Clear_PasswordMembers:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000003
	.global Clear_PasswordFlags
Clear_PasswordFlags:
	.4byte 0x09510941
	.4byte 0x08d108b3
	.4byte 0x0868081e
	.global Clear_PasswordKeyItems
Clear_PasswordKeyItems:
	.4byte 0x00c900c8
	.4byte 0x00cb00ca
	.4byte 0x00cd00cc
	.4byte 0x00cf00ce
	.global Clear_PasswordStackItems
Clear_PasswordStackItems:
	.4byte 0x00b500b4
	.4byte 0x00b700b6
	.4byte 0x00bb00ba
	.4byte 0x00bd00bc
	.4byte 0x00c000bf
	.4byte 0x00c200c1
	.4byte 0x00c400c3
	.4byte 0x00e300e2
	.4byte 0x00e500e4
	.4byte 0x00ee00ec
	.4byte 0x00f000ef
	.2byte 0x00f1
