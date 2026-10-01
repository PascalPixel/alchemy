.syntax unified
	.thumb
	.section .text.x0200a27c,"ax",%progbits
	.global Func_0200227c
	.thumb_func
Func_0200227c:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	movs r0, #0
	bl Object_GetById
	mov r10, r0
	bl Engine_EventBegin
	movs r0, #5
	movs r1, #1
	bl Engine_ActorEnableActionCallback
	movs r0, #9
	movs r1, #1
	bl Engine_ActorEnableActionCallback
	movs r0, #11
	movs r1, #1
	bl Engine_ActorEnableActionCallback
	movs r0, #10
	movs r1, #1
	bl Engine_ActorEnableActionCallback
	movs r0, #14
	movs r1, #1
	bl Engine_ActorEnableActionCallback
	movs r0, #13
	movs r1, #1
	bl Engine_ActorEnableActionCallback
	movs r2, #166
	movs r0, #5
	ldr r1, .L_0200a3e8
	lsls r2, r2, #17
	bl Engine_ActorSetPosition
	movs r2, #166
	movs r0, #9
	ldr r1, .L_0200a3ec
	lsls r2, r2, #17
	bl Engine_ActorSetPosition
	movs r2, #174
	movs r0, #11
	ldr r1, .L_0200a3f0
	lsls r2, r2, #17
	bl Engine_ActorSetPosition
	movs r2, #174
	movs r0, #10
	ldr r1, .L_0200a3f4
	lsls r2, r2, #17
	bl Engine_ActorSetPosition
	movs r1, #230
	movs r2, #180
	movs r0, #14
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Engine_ActorSetPosition
	movs r2, #153
	ldr r1, .L_0200a3f8
	lsls r2, r2, #17
	movs r0, #13
	bl Engine_ActorSetPosition
	movs r0, #5
	bl Object_GetById
	mov r1, r10
	str r1, [r0, #104]
	adds r2, r0, #0
	adds r2, #90
	ldrb r3, [r2]
	movs r6, #1
	orrs r3, r6
	strb r3, [r2]
	ldr r3, .L_0200a3fc
	movs r1, #0
	mov r8, r3
	mov r9, r1
	mov r1, r8
	bl Engine_ObjectSetScript
	movs r0, #9
	bl Object_GetById
	mov r1, r10
	str r1, [r0, #104]
	adds r2, r0, #0
	adds r2, #90
	ldrb r3, [r2]
	orrs r3, r6
	strb r3, [r2]
	mov r1, r8
	bl Engine_ObjectSetScript
	movs r0, #11
	bl Object_GetById
	mov r3, r10
	str r3, [r0, #104]
	adds r2, r0, #0
	adds r2, #90
	ldrb r3, [r2]
	orrs r3, r6
	strb r3, [r2]
	mov r1, r8
	bl Engine_ObjectSetScript
	movs r0, #10
	bl Object_GetById
	mov r1, r10
	str r1, [r0, #104]
	adds r2, r0, #0
	adds r2, #90
	ldrb r3, [r2]
	orrs r3, r6
	mov r1, r8
	strb r3, [r2]
	bl Engine_ObjectSetScript
	movs r0, #14
	bl Object_GetById
	mov r3, r10
	adds r5, r0, #0
	str r3, [r5, #104]
	adds r2, r5, #0
	adds r2, #90
	ldrb r3, [r2]
	orrs r3, r6
	strb r3, [r2]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #24]
	str r3, [r5, #28]
	movs r0, #11
	bl Object_GetById
	adds r0, #85
	ldrb r3, [r0]
	adds r2, r5, #0
	adds r2, #85
	mov r1, r9
	strb r3, [r2]
	adds r0, r5, #0
	str r1, [r5, #12]
	mov r1, r8
	bl Engine_ObjectSetScript
	movs r0, #13
	bl Object_GetById
	mov r3, r10
	str r3, [r0, #104]
	adds r2, r0, #0
	adds r2, #90
	ldrb r3, [r2]
	orrs r6, r3
	strb r6, [r2]
	mov r1, r8
	bl Engine_ObjectSetScript
	bl Engine_EventEnd
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_0200a3e8:
	.4byte 0x01db0000
.L_0200a3ec:
	.4byte 0x01eb0000
.L_0200a3f0:
	.4byte 0x01cb0000
.L_0200a3f4:
	.4byte 0x01fb0000
.L_0200a3f8:
	.4byte 0x01d70000
.L_0200a3fc:
	.4byte SoruStar_AngleScript
	.section .rodata.x0200cb1c,"a",%progbits
.L_0200cb1c:
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
.L_0200cb54:
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
.L_0200cb8c:
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
	.global gEffectScripts
gEffectScripts:
	.4byte .L_0200cb1c
	.4byte .L_0200cb54
	.4byte .L_0200cb8c
	.global SoruStar_AngleScript
SoruStar_AngleScript:
	.4byte 0x00000022
	.4byte UpdateOverlayObjectAngle
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global SoruStar_PresentItemScript
SoruStar_PresentItemScript:
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00008000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000c000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000c000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000e000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000e000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000f000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000f000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000010
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00016000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00016000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00018000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00018000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00012000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00012000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00016000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00016000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00018000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00018000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00012000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00012000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00010000
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00010000
	.4byte 0x00000000
	.4byte 0x00000002
	.4byte 0x00000010
	.global Placement_Scripts
Placement_Scripts:
	.4byte 0xffff0000
	.4byte 0x000001d8
	.4byte 0x40000142
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Placement_Messages
Placement_Messages:
	.4byte 0x00000011
	.4byte 0x0020a012
	.4byte 0x000001ff
	.global Placement_Actors
Placement_Actors:
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00034000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0xffff001e
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00034000
	.4byte 0xffff0020
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0021
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0022
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0023
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff002b
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x02690000
	.4byte 0x00000000
	.4byte 0x01980000
	.4byte 0x00000000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x01ad0000
	.4byte 0x00000000
	.4byte 0x00dd0000
	.4byte 0x00000000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x00710000
	.4byte 0x00000000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x00aa0000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x01340000
	.4byte 0x00000000
	.4byte 0x01ff0000
	.4byte 0x00000000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x018d0000
	.4byte 0x00000000
	.4byte 0x01a20000
	.4byte 0x00000000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x021e0000
	.4byte 0x00000000
	.4byte 0x010d0000
	.4byte 0x00000000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x01a50000
	.4byte 0x00000000
	.4byte 0x00750000
	.4byte 0x00000000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x01270000
	.4byte 0x00000000
	.4byte 0x00d20000
	.4byte 0x00000000
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x02e60000
	.4byte 0x00000000
	.4byte 0x01360000
	.4byte 0x00000000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Placement_Effects
Placement_Effects:
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte Sukureta_Talk
	.4byte 0x00000000
	.4byte 0xffff0005
	.4byte Jasmine_Talk
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte Saturos_Talk
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte Menardi_Talk
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte Garcia_Talk
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte Alex_Talk
	.4byte 0x00008d15
	.4byte 0xffff0009
	.4byte MsgSoruSukuretaIAmResponsible
	.4byte 0x00008d15
	.4byte 0xffff0005
	.4byte Jasmine_Talk
	.4byte 0x00008d15
	.4byte 0xffff000a
	.4byte MsgSoruSaturosBringTheFinalStar
	.4byte 0x00008d15
	.4byte 0xffff000b
	.4byte Menardi_Talk
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte MsgSoruAlexOnlyOneLeft
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte SceneDialogue_RunLine1072WithPair9And10
	.4byte 0x00000602
	.4byte 0xffff000a
	.4byte FieldScene_Forward72b4
	.4byte 0x00004602
	.4byte 0xffff000b
	.4byte FieldScene_Forward72b4
	.4byte 0x00000003
	.4byte 0x083c0002
	.4byte Scene_BagVenusStar
	.4byte 0x00000003
	.4byte 0x083d0003
	.4byte Scene_BagMercuryStar
	.4byte 0x00000003
	.4byte 0x083e0004
	.4byte Scene_HandOverStars
	.4byte 0x00000003
	.4byte 0x083f0005
	.4byte Scene_BagMarsStar
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global SoruStar_StarCells
SoruStar_StarCells:
	.4byte 0x0028003b
	.4byte 0x00040003
	.4byte 0x003e0006
	.4byte 0x00030028
	.4byte 0x00060004
	.4byte 0x00280041
	.4byte 0x00040003
	.4byte 0x00440006
	.4byte 0x00030028
	.4byte 0x00060004
	.4byte 0x00280047
	.4byte 0x00040003
	.4byte 0x004a0006
	.4byte 0x00030028
	.4byte 0x00060004
	.4byte 0x0028004d
	.4byte 0x00040003
	.4byte 0x00500006
	.4byte 0x00030028
	.4byte 0x00060004
	.4byte 0x00280053
	.4byte 0x00040003
	.4byte 0xffff0000
	.global Soru_RingDrift
Soru_RingDrift:
	.4byte 0x04040404
	.4byte 0x00040300
	.4byte 0x04000404
	.4byte 0x04040003
	.4byte 0x00060406
	.4byte 0x03000404
	.4byte 0x02010002
	.2byte 0x0200
	.global Soru_RingSwing
Soru_RingSwing:
	.2byte 0x0101
	.4byte 0x02000102
	.4byte 0x01020001
	.4byte 0x00010100
	.4byte 0x02010102
	.4byte 0x01020001
	.4byte 0x00010200
	.4byte 0x02000102
	.global Soru_RingDirection
Soru_RingDirection:
	.4byte 0x01010101
	.4byte 0x00010100
	.4byte 0x0100ff01
	.4byte 0x01ff00ff
	.4byte 0x00ff0101
	.4byte 0xff00ffff
	.4byte 0xffff00ff
	.4byte 0x0000ff00
	.global Soru_RingOffsetX
Soru_RingOffsetX:
	.4byte 0x00009999
	.4byte 0x0000cccc
	.4byte 0x0000b333
	.4byte 0x00009999
	.4byte 0x0000cccc
	.4byte 0x00009999
	.4byte 0x0000b333
	.4byte 0x00009999
	.4byte 0x00009999
	.4byte 0x0000b333
	.global Soru_RingOffsetZ
Soru_RingOffsetZ:
	.4byte 0x0000028f
	.4byte 0x000001ca
	.4byte 0x0000028f
	.4byte 0x000001ca
	.4byte 0x0000028f
	.4byte 0x000001ca
	.4byte 0x0000028f
	.4byte 0x0000028f
	.4byte 0x0000020c
	.4byte 0x0000028f
