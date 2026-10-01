.syntax unified
	.thumb
	.section .text.x020090c2,"ax",%progbits
	.2byte 0x0000
	.section .text.x020090c4,"ax",%progbits
	.global Func_020010c4
	.thumb_func
Func_020010c4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r0, #0
	sub sp, #56
	bl Object_GetById
	adds r6, r0, #0
	bl Engine_EventBegin
	movs r1, #6
	adds r0, r6, #0
	bl Object_SetMode
	movs r0, #0
	bl ObjectMotion_WaitForAnimationChange
	adds r0, r6, #0
	movs r1, #1
	bl Object_SetMode
	movs r1, #0
	adds r0, r6, #0
	bl Engine_ActorSetSpriteFlags
	movs r0, #85
	adds r0, r0, r6
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	mov r10, r0
	movs r0, #152
	bl Audio_PlayCue
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r6, #40]
	movs r0, #192
	ldr r3, [r6, #16]
	lsls r0, r0, #12
	adds r3, r3, r0
	ldr r1, [r6, #8]
	ldr r2, [r6, #12]
	adds r0, r6, #0
	bl Object_SetPosition
	movs r0, #6
	bl WaitFrames
	add r3, sp, #16
	mov r8, r3
	ldr r3, .L_020091cc
	mov r2, r10
	mov r0, r8
	movs r5, #0
	strb r5, [r2]
	str r3, [r0, #36]
	movs r0, #127
	bl Audio_PlayCue
	movs r7, #0
.L_02009142:
	ldr r3, [r6, #12]
	ldr r2, .L_020091d0
	adds r3, r3, r2
	str r3, [r6, #12]
	str r3, [r6, #60]
	movs r0, #1
	bl WaitFrames
	movs r3, #1
	ands r3, r7
	cmp r3, #0
	beq .L_020091a6
	bl Engine_RandomNext
	movs r1, #10
	bl __umodsi3
	ldr r3, .L_020091d4
	subs r0, #5
	adds r5, r0, #0
	muls r5, r3
	bl Engine_RandomNext
	movs r1, #10
	bl __umodsi3
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #2
	adds r3, r3, r0
	lsls r4, r3, #6
	subs r4, r4, r3
	lsls r4, r4, #3
	adds r4, r4, r0
	ldr r3, .L_020091d8
	negs r4, r4
	adds r4, r4, r3
	movs r3, #0
	ldr r0, [r6, #8]
	ldr r1, [r6, #12]
	ldr r2, [r6, #16]
	str r3, [sp, #0]
	ldr r3, .L_020091dc
	str r3, [sp, #8]
	mov r3, r8
	str r3, [sp, #12]
	adds r3, r5, #0
	str r4, [sp, #4]
	bl Effect_Spawn
.L_020091a6:
	adds r7, #1
	cmp r7, #7
	bls .L_02009142
	adds r0, r6, #0
	movs r1, #1
	bl Engine_ActorSetSpriteFlags
	movs r3, #3
	mov r0, r10
	strb r3, [r0]
	bl Engine_EventEnd
	add sp, #56
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_020091cc:
	.4byte Effect_AdvanceMotion
.L_020091d0:
	.4byte 0xfffe0000
.L_020091d4:
	.4byte 0x00003332
.L_020091d8:
	.4byte 0xffff8003
.L_020091dc:
	.4byte 0x01000001
	.section .rodata.x0200b1f0,"a",%progbits
	.global StagedActor_DirectionSteps
StagedActor_DirectionSteps:
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00100000
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0x00000010
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x0000fff0
	.4byte 0x00100000
	.global StagedActor_FootprintKinds
StagedActor_FootprintKinds:
	.4byte 0x000000cf
	.4byte 0x000000cd
	.4byte 0x000000e4
	.4byte 0x000000e5
	.4byte 0x0000012a
	.4byte 0x00000129
	.global StagedActor_FootprintBounds
StagedActor_FootprintBounds:
	.4byte 0xffffffe0
	.4byte 0xfffffff8
	.4byte 0x00000020
	.4byte 0x00000008
	.4byte 0xfffffff8
	.4byte 0xffffffe0
	.4byte 0x00000008
	.4byte 0x00000020
	.4byte 0xffffffe0
	.4byte 0xfffffff0
	.4byte 0x00000020
	.4byte 0x00000000
	.4byte 0xfffffff8
	.4byte 0xffffffe0
	.4byte 0x00000008
	.4byte 0x00000020
	.4byte 0xffffffe0
	.4byte 0xfffffff8
	.4byte 0x00000020
	.4byte 0x00000008
	.4byte 0xfffffff8
	.4byte 0xffffffe0
	.4byte 0x00000008
	.4byte 0x00000020
.L_0200b2a8:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000016
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
.L_0200b2e0:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000002c
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
.L_0200b318:
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x0000007e
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000001b
	.global Data_0200b350
Data_0200b350:
	.4byte 0xfff00000
	.4byte 0xfff00000
	.4byte 0xffe00000
	.4byte 0xffd00000
	.4byte 0xffc00000
	.4byte 0xffb00000
	.4byte 0xffb00000
	.global gEffectScripts
gEffectScripts:
	.4byte .L_0200b2a8
	.4byte .L_0200b2e0
	.4byte .L_0200b318
.L_0200b378:
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00009999
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00009999
	.4byte 0x00000015
	.4byte 0x0000001e
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte SceneState_ApplyArgMode0AndReturnZero
	.4byte 0x00000010
.L_0200b3a8:
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00019999
	.4byte 0x00000010
	.4byte 0x00000000
	.4byte 0x00000023
	.4byte 0x00000015
	.4byte 0x00000026
	.4byte BabiChika_UpdateFlickerEffect
	.4byte 0x00000000
	.4byte 0x00000082
	.4byte 0x00000015
	.4byte 0x00000026
	.4byte 0x00000000
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000010
	.global Data_0200b3ec
Data_0200b3ec:
	.4byte 0x003b001c
	.4byte 0x00020001
	.4byte 0x001a0004
	.4byte 0x0001003b
	.4byte 0x00040002
	.4byte 0x003b0018
	.4byte 0x00020001
	.4byte 0xffff0004
	.global Data_0200b40c
Data_0200b40c:
	.4byte 0x003b001a
	.4byte 0x00020001
	.4byte 0x001c0004
	.4byte 0x0001003b
	.4byte 0x00040002
	.4byte 0x003b001e
	.4byte 0x00020001
	.4byte 0xffff0004
	.global gBabiChikaEntrancesOther
gBabiChikaEntrancesOther:
	.4byte 0xffff0000
	.4byte 0x000002e8
	.4byte 0x40000068
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x000001b8
	.4byte 0x40000228
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gBabiChikaEntrances1
gBabiChikaEntrances1:
	.4byte 0xffff0000
	.4byte 0x000002e8
	.4byte 0x40000068
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000068
	.4byte 0x40000098
	.4byte 0x00300000
	.4byte 0x01200030
	.4byte 0x00000170
	.4byte 0xffff0002
	.4byte 0x000000c8
	.4byte 0xc0000148
	.4byte 0x00300000
	.4byte 0x01200030
	.4byte 0x00000170
	.4byte 0xffff0003
	.4byte 0x000000b8
	.4byte 0x40000228
	.4byte 0x00400000
	.4byte 0x013001d0
	.4byte 0x00000330
	.4byte 0xffff0004
	.4byte 0x000000a8
	.4byte 0xc0000330
	.4byte 0x00400000
	.4byte 0x013001d0
	.4byte 0x00000330
	.4byte 0xffff0005
	.4byte 0x00000208
	.4byte 0x40000068
	.4byte 0x01500000
	.4byte 0x02400030
	.4byte 0x00000120
	.4byte 0xffff0006
	.4byte 0x00000188
	.4byte 0xc00000c0
	.4byte 0x01500000
	.4byte 0x02400030
	.4byte 0x00000120
	.4byte 0xffff0007
	.4byte 0x00000208
	.4byte 0xc0000100
	.4byte 0x01500000
	.4byte 0x02400030
	.4byte 0x00000120
	.4byte 0xffff0008
	.4byte 0x00000238
	.4byte 0x400001b8
	.4byte 0x01800000
	.4byte 0x02700160
	.4byte 0x00000280
	.4byte 0xffff0009
	.4byte 0x000001b8
	.4byte 0xc0000288
	.4byte 0x01800000
	.4byte 0x02700160
	.4byte 0x00000280
	.4byte 0xffff000a
	.4byte 0x000002e8
	.4byte 0x40000068
	.4byte 0x02700000
	.4byte 0x03600028
	.4byte 0x00000120
	.4byte 0xffff000b
	.4byte 0x000002a8
	.4byte 0xc0000100
	.4byte 0x02700000
	.4byte 0x03600028
	.4byte 0x00000120
	.4byte 0xffff000c
	.4byte 0x000001f8
	.4byte 0x40000318
	.4byte 0x01700000
	.4byte 0x026002d0
	.4byte 0x000003d0
	.4byte 0xffff000d
	.4byte 0x00000238
	.4byte 0x40000368
	.4byte 0x01700000
	.4byte 0x026002d0
	.4byte 0x000003d0
	.4byte 0xffff000e
	.4byte 0x00000308
	.4byte 0xc00001b0
	.4byte 0x02b00000
	.4byte 0x03a00170
	.4byte 0x00000280
	.4byte 0xffff000f
	.4byte 0x00000348
	.4byte 0xc0000260
	.4byte 0x02b00000
	.4byte 0x03a00170
	.4byte 0x00000280
	.4byte 0xffff0010
	.4byte 0x000002e8
	.4byte 0x40000318
	.4byte 0x02b00000
	.4byte 0x03a002e0
	.4byte 0x000003d0
	.4byte 0xffff0011
	.4byte 0x000002e8
	.4byte 0xc00003b0
	.4byte 0x02b00000
	.4byte 0x03a002e0
	.4byte 0x000003d0
	.4byte 0xffff0012
	.4byte 0x00000368
	.4byte 0xc0000370
	.4byte 0x02b00000
	.4byte 0x03a002e0
	.4byte 0x000003d0
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gBabiChikaEntrances2
gBabiChikaEntrances2:
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000278
	.4byte 0x40000158
	.4byte 0x02500000
	.4byte 0x03700040
	.4byte 0x00000190
	.4byte 0xffff0002
	.4byte 0x00000348
	.4byte 0x40000078
	.4byte 0x02500000
	.4byte 0x03700040
	.4byte 0x00000190
	.4byte 0xffff0003
	.4byte 0x00000338
	.4byte 0x40000158
	.4byte 0x02500000
	.4byte 0x03700040
	.4byte 0x00000190
	.4byte 0xffff0004
	.4byte 0x000000d8
	.4byte 0x40000068
	.4byte 0x00200000
	.4byte 0x01100030
	.4byte 0x00000140
	.4byte 0xffff0005
	.4byte 0x000000d8
	.4byte 0xc0000128
	.4byte 0x00200000
	.4byte 0x01100030
	.4byte 0x00000140
	.4byte 0xffff0006
	.4byte 0x00000168
	.4byte 0x40000068
	.4byte 0x01300000
	.4byte 0x02200030
	.4byte 0x00000160
	.4byte 0xffff0007
	.4byte 0x000001e8
	.4byte 0xc0000148
	.4byte 0x01300000
	.4byte 0x02200030
	.4byte 0x00000160
	.4byte 0xffff0008
	.4byte 0x00000078
	.4byte 0xc0000238
	.4byte 0x00500000
	.4byte 0x01c00190
	.4byte 0x00000250
	.4byte 0xffff0009
	.4byte 0x000000d8
	.4byte 0x400001c8
	.4byte 0x00500000
	.4byte 0x01c00190
	.4byte 0x00000250
	.4byte 0xffff000a
	.4byte 0x00000138
	.4byte 0xc0000238
	.4byte 0x00500000
	.4byte 0x01c00190
	.4byte 0x00000250
	.4byte 0xffff000b
	.4byte 0x00000198
	.4byte 0x400001c8
	.4byte 0x00500000
	.4byte 0x01c00190
	.4byte 0x00000250
	.4byte 0xffff000c
	.4byte 0x00000088
	.4byte 0x400002c8
	.4byte 0x00100000
	.4byte 0x01000290
	.4byte 0x00000370
	.4byte 0xffff000d
	.4byte 0x00000188
	.4byte 0x400002c8
	.4byte 0x01200000
	.4byte 0x02300290
	.4byte 0x00000360
	.4byte 0xffff000e
	.4byte 0x00000208
	.4byte 0x40000308
	.4byte 0x01200000
	.4byte 0x02300290
	.4byte 0x00000360
	.4byte 0xffff000f
	.4byte 0x00000288
	.4byte 0xc00003b8
	.4byte 0x02600000
	.4byte 0x03500330
	.4byte 0x000003d0
	.4byte 0xffff0010
	.4byte 0x00000308
	.4byte 0x40000388
	.4byte 0x02600000
	.4byte 0x03500330
	.4byte 0x000003d0
	.4byte 0xffff0011
	.4byte 0x000002b8
	.4byte 0x400002a8
	.4byte 0x02700000
	.4byte 0x03600250
	.4byte 0x00000300
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gBabiChikaRegions2
gBabiChikaRegions2:
	.4byte 0x00100270
	.4byte 0x02800150
	.4byte 0x01600020
	.4byte 0x000fffff
	.4byte 0x00100330
	.4byte 0x03400150
	.4byte 0x01600020
	.4byte 0x0010ffff
	.4byte 0x00100340
	.4byte 0x03500070
	.4byte 0x00800020
	.4byte 0x0011ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200b85c
Data_0200b85c:
	.4byte 0x000000ac
	.4byte 0x001010ae
	.4byte 0x002030ac
	.4byte 0x003020ac
	.4byte 0x004050ac
	.4byte 0x005040ac
	.4byte 0x006080ac
	.4byte 0x0070a0ac
	.4byte 0x008060ac
	.4byte 0x0090c0ac
	.4byte 0x00a070ac
	.4byte 0x00b100ac
	.4byte 0x00c090ac
	.4byte 0x00d0e0ac
	.4byte 0x00e0d0ac
	.4byte 0x00f040ad
	.4byte 0x0100b0ac
	.4byte 0x011060ad
	.4byte 0x012110ad
	.4byte 0x000000ad
	.4byte 0x0010f0ac
	.4byte 0x002090ad
	.4byte 0x003110ac
	.4byte 0x0040b0ad
	.4byte 0x005050ad
	.4byte 0x006070ad
	.4byte 0x0070c0ad
	.4byte 0x0080d0ad
	.4byte 0x009080ad
	.4byte 0x00a0a0ad
	.4byte 0x00b0f0ad
	.4byte 0x00c120ac
	.4byte 0x00d0e0ad
	.4byte 0x00e120b7
	.4byte 0x00f040b0
	.4byte 0x010050b0
	.4byte 0x011060b0
	.4byte 0x000001ff
	.global Data_0200b8f4
Data_0200b8f4:
	.4byte 0x000000fd
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x00024000
	.4byte 0x000000fd
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02680000
	.4byte 0x01024000
	.4byte 0x000000fd
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x00024000
	.4byte 0x000000fd
	.4byte 0x00000001
	.4byte 0x02180000
	.4byte 0x00000000
	.4byte 0x01e80000
	.4byte 0x01024000
	.4byte 0x000000fe
	.4byte 0x00000001
	.4byte 0x01e80000
	.4byte 0x00000000
	.4byte 0x03480000
	.4byte 0x00024000
	.4byte 0x000000fe
	.4byte 0x00000001
	.4byte 0x02080000
	.4byte 0x00000000
	.4byte 0x03480000
	.4byte 0x01024000
	.4byte 0x000000fe
	.4byte 0x00000001
	.4byte 0x01a80000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x01024000
	.4byte 0x000000f8
	.4byte .L_0200b3a8
	.4byte 0x01a00000
	.4byte 0x00000000
	.4byte 0x031a0000
	.4byte 0x0000c000
	.4byte 0x000000f8
	.4byte .L_0200b3a8
	.4byte 0x01b00000
	.4byte 0x00000000
	.4byte 0x031a0000
	.4byte 0x0000c000
	.4byte 0x097000f8
	.4byte .L_0200b3a8
	.4byte 0x02380000
	.4byte 0x00000000
	.4byte 0x03580000
	.4byte 0x0000c000
	.4byte 0x000000ff
	.4byte 0x00000001
	.4byte 0x02a80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x0002c000
	.4byte 0x000000ff
	.4byte 0x00000001
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x0002c000
	.4byte 0x000000e3
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x0002c000
	.4byte 0x000000e3
	.4byte 0x00000001
	.4byte 0x02d80000
	.4byte 0x00000000
	.4byte 0x00b80000
	.4byte 0x0002c000
	.4byte 0x00000114
	.4byte .L_0200b378
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200ba74
Data_0200ba74:
	.4byte 0x000000ff
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x0002c000
	.4byte 0x000000e3
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x00f80000
	.4byte 0x0002c000
	.4byte 0x00000101
	.4byte 0x00000007
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x0002c000
	.4byte 0x00000101
	.4byte 0x00000007
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x0102c000
	.4byte 0x00000100
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x0002c000
	.4byte 0x00000100
	.4byte 0x00000001
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x0102c000
	.4byte 0x00000100
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x0102c000
	.4byte 0x00000102
	.4byte 0x00000001
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x03180000
	.4byte 0x00024000
	.4byte 0x00000102
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00024000
	.4byte 0x000000fd
	.4byte 0x00000001
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0x000000fd
	.4byte 0x00000001
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x01380000
	.4byte 0x00024000
	.4byte 0x000001f4
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00024000
	.4byte 0x000001f4
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x02f80000
	.4byte 0x00024000
	.4byte 0x000001f4
	.4byte 0x00000001
	.4byte 0x01580000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00024000
	.4byte 0x000001f4
	.4byte 0x00000001
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x03280000
	.4byte 0x00024000
	.4byte 0x000001f4
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x03380000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200bc0c
Data_0200bc0c:
	.4byte 0x00000021
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000001
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000001
	.4byte 0xffff000f
	.4byte 0x0000000f
	.4byte 0x00000001
	.4byte 0xffff0010
	.4byte 0x00000010
	.4byte 0x00000001
	.4byte 0xffff0011
	.4byte 0x00000011
	.4byte 0x00000001
	.4byte 0xffff0012
	.4byte 0x00000012
	.4byte 0x00004602
	.4byte 0xffff0017
	.4byte FieldScene_RunStepWith6
	.4byte 0x00008602
	.4byte 0xffff0017
	.4byte SceneActor_ApplyPointLeftOfActorZero
	.4byte 0x00000202
	.4byte 0xffff0018
	.4byte SceneState_RunRect73x38Step
	.4byte 0x00004602
	.4byte 0xffff0019
	.4byte Func_020010c4
	.4byte 0x00000202
	.4byte 0xffff001a
	.4byte FieldScene_RunLayoutAt93By30
	.4byte 0x00004602
	.4byte 0xffff001b
	.4byte FieldScene_RunStepWith6
	.4byte 0x00000602
	.4byte 0xffff001b
	.4byte SceneActor_PassPointTwoRightOfActorZero
	.4byte 0x00008602
	.4byte 0xffff0032
	.4byte SceneState_RunUnlessActorZeroAtTile32x50
	.4byte 0x00000602
	.4byte 0xffff0032
	.4byte SceneActor_PassPointTwoRightOfActorZero
	.4byte 0x0000c602
	.4byte 0xffff0032
	.4byte SceneState_RunUnlessActorZeroAt30_52
	.4byte 0x00004602
	.4byte 0xffff0032
	.4byte SceneActor_PassRaisedPointOfActorZero
	.4byte 0x00004602
	.4byte 0xffff001c
	.4byte SceneActor_CheckTwoUnitsAboveActorZero
	.4byte 0x00000202
	.4byte 0xffff001c
	.4byte SceneState_ApplyTwoRectsAndRunThree
	.4byte 0x00000202
	.4byte 0x0971001d
	.4byte FieldScene_RunFourCallSequenceB
	.4byte 0x00000602
	.4byte 0x0200001d
	.4byte SceneActor_PassPointTwoRightOfActorZero
	.4byte 0x00004602
	.4byte 0x0200001d
	.4byte SceneActor_PassRaisedPointOfActorZero
	.4byte 0x0000c602
	.4byte 0x0200001d
	.4byte SceneActor_PassActorZeroOffsetPoint
	.4byte 0x00008602
	.4byte 0x0200001f
	.4byte SceneActor_ApplyPointLeftOfActorZero
	.4byte 0x00000602
	.4byte 0xffff001f
	.4byte SceneActor_PassPointTwoRightOfActorZero
	.4byte 0x00000202
	.4byte 0x0972001e
	.4byte FieldScene_RunFourStepSequenceA
	.4byte 0x00008602
	.4byte 0x0201001e
	.4byte SceneActor_ApplyPointLeftOfActorZero
	.4byte 0x00004602
	.4byte 0x0201001e
	.4byte SceneActor_PassRaisedPointOfActorZero
	.4byte 0x0000c602
	.4byte 0x0201001e
	.4byte SceneActor_PassActorZeroOffsetPoint
	.4byte 0x00000602
	.4byte 0x02020020
	.4byte FieldScene_RunFourStepSequenceA
	.4byte 0x00000602
	.4byte 0x02010020
	.4byte SceneActor_PassPointTwoRightOfActorZero
	.4byte 0x00008602
	.4byte 0xffff0020
	.4byte SceneActor_ApplyPointLeftOfActorZero
	.4byte 0x00000003
	.4byte 0xffff0028
	.4byte SceneDialogue_RunFlag982Or983Dialogue
	.4byte 0x00008c15
	.4byte 0xffff0008
	.4byte FieldScene_PlaceAndPinSlots8And9
	.4byte 0x00008c15
	.4byte 0xffff0009
	.4byte FieldScene_PlaceAndPinSlots8And9
	.4byte 0x00008c15
	.4byte 0xffff000a
	.4byte FieldScene_PlaceAndPinSlots10And11
	.4byte 0x00008c15
	.4byte 0xffff000b
	.4byte FieldScene_PlaceAndPinSlots10And11
	.4byte 0x00008c15
	.4byte 0xffff000c
	.4byte FieldScene_RunScene3c4_02002480
	.4byte 0x00008c15
	.4byte 0xffff000d
	.4byte FieldScene_RunScene3c4_02002480
	.4byte 0x00008c15
	.4byte 0xffff000e
	.4byte FieldScene_RunScene3c4_02002480
	.4byte 0x00004e15
	.4byte 0x0200000f
	.4byte SceneActor_MirrorFlag201IntoSlot14
	.4byte 0x00004e15
	.4byte 0x02010010
	.4byte SceneActor_SetActor14Field98ByFlag200
	.4byte 0x00004e15
	.4byte 0x09700011
	.4byte SceneState_ApplyFlag970
	.4byte 0x00008c15
	.4byte 0x09710012
	.4byte FieldScene_RunSupplementalSequenceTwo
	.4byte 0x00001815
	.4byte 0x02000014
	.4byte ActorPresentation_ConfigureActorTwentyAndFlag200
	.4byte 0x00008c15
	.4byte 0x09720013
	.4byte FieldScene_RunSupplementalSequenceOne
	.4byte 0x00001815
	.4byte 0x02010015
	.4byte SceneActor_ConfigureSlot21AndSetFlag201
	.4byte 0x10002115
	.4byte 0x02020013
	.4byte FieldScene_SetActor19TableB3B8
	.4byte 0x00002115
	.4byte 0x02020013
	.4byte SceneState_SetValue202ThenCall
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200bef4
Data_0200bef4:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0xffff0008
	.4byte 0x00000008
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0x00000001
	.4byte 0xffff000a
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0xffff000b
	.4byte 0x0000000b
	.4byte 0x00000001
	.4byte 0xffff000c
	.4byte 0x0000000c
	.4byte 0x00000001
	.4byte 0xffff000d
	.4byte 0x0000000d
	.4byte 0x00000021
	.4byte 0xffff000e
	.4byte 0x0000000e
	.4byte 0x00000202
	.4byte 0xffff0019
	.4byte FieldScene_RunFourStepSequenceB
	.4byte 0x00004602
	.4byte 0xffff001a
	.4byte FieldScene_RunStepWith6
	.4byte 0x00008602
	.4byte 0xffff001a
	.4byte SceneActor_ApplyPointLeftOfActorZero
	.4byte 0x00004602
	.4byte 0xffff001b
	.4byte Func_020010c4
	.4byte 0x00000202
	.4byte 0xffff001c
	.4byte FieldScene_RunThreeStepSequence
	.4byte 0x00000602
	.4byte 0xffff001e
	.4byte SceneActor_PassPointTwoRightOfActorZero
	.4byte 0x00008602
	.4byte 0xffff001e
	.4byte SceneActor_ApplyPointLeftOfActorZero
	.4byte 0x00000202
	.4byte 0xffff001e
	.4byte FieldScene_RunThreeStepSequence
	.4byte 0x00000602
	.4byte 0xffff001d
	.4byte FieldScene_RunFourCallSequence
	.4byte 0x00004602
	.4byte 0xffff001d
	.4byte SceneActor_PassRaisedPointOfActorZero
	.4byte 0x0000c602
	.4byte 0xffff001d
	.4byte SceneActor_PassActorZeroOffsetPoint
	.4byte 0x00000202
	.4byte 0xffff0029
	.4byte FieldScene_RunLayoutAt83By45
	.4byte 0x00000003
	.4byte 0xffff0014
	.4byte FieldScene_RunScriptedStep953
	.4byte 0x00000003
	.4byte 0xffff002a
	.4byte SceneState_SetValue268bInScene
	.4byte 0x00000013
	.4byte 0x0f320064
	.4byte 0x00100071
	.4byte 0x00000013
	.4byte 0x0f330065
	.4byte 0x00100054
	.4byte 0x00008c15
	.4byte 0x02040008
	.4byte FieldScene_RunTwoStepSequence
	.4byte 0x10002115
	.4byte 0x02030008
	.4byte SceneActor_InstallSlotNineHandler
	.4byte 0x00001815
	.4byte 0x02040009
	.4byte SceneActor_SetupSlotNineAndInstallHandler
	.4byte 0x00008c15
	.4byte 0xffff000c
	.4byte 0x00000000
	.4byte 0x00008c15
	.4byte 0xffff000d
	.4byte 0x00000000
	.4byte 0x00008c15
	.4byte 0xffff000e
	.4byte 0x00000000
	.4byte 0x00009315
	.4byte 0xffff000c
	.4byte FieldScene_RunMiddleSequence
	.4byte 0x00009315
	.4byte 0xffff000d
	.4byte FieldScene_RunMiddleSequence
	.4byte 0x00009315
	.4byte 0xffff000e
	.4byte FieldScene_RunMiddleSequence
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte FieldScene_RunLateSequenceSecond
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte FieldScene_RunLateSequenceHead
	.4byte 0x00008c15
	.4byte 0xffff0011
	.4byte SceneState_SetSlot17And18Selectors
	.4byte 0x00008c15
	.4byte 0xffff0012
	.4byte SceneState_SetSlot17And18Selectors
	.4byte 0x00008c15
	.4byte 0xffff0013
	.4byte FieldScene_RunScene3c4SequenceA
	.4byte 0x00008c15
	.4byte 0xffff0014
	.4byte FieldScene_RunScene3c4SequenceA
	.4byte 0x00008c15
	.4byte 0xffff0015
	.4byte FieldScene_RunScene3c4SequenceA
	.4byte 0x00008c15
	.4byte 0xffff0016
	.4byte FieldScene_RunScene3c4SequenceA
	.4byte 0x00008c15
	.4byte 0xffff0017
	.4byte FieldScene_RunScene3c4SequenceA
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x0000268c
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x0000268d
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x0000268e
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x0000268f
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte 0x00002690
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
