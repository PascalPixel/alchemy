.syntax unified
	.thumb
	.section .text.x0200b4bc,"ax",%progbits
	.global MakyuriChojo_FlickerActorEight
	.thumb_func
MakyuriChojo_FlickerActorEight:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r0, #8
	bl Object_GetById
	ldr r3, .L_0200b534
	mov r10, r0
	ldr r5, [r3]
	bl Engine_RandomNext
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #4
	lsrs r3, r3, #16
	lsls r3, r3, #16
	adds r5, #232
	mov r8, r3
	movs r0, #2
	ldrsh r3, [r5, r0]
	cmp r3, #129
	bgt .L_0200b540
	ldr r3, .L_0200b538
	ldr r3, [r3]
	movs r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0200b512
	movs r1, #152
	movs r2, #144
	lsls r1, r1, #17
	lsls r2, r2, #16
	movs r0, #8
	bl Engine_ActorSetPosition
	movs r0, #8
	bl Object_GetById
	movs r5, #128
	lsls r5, r5, #9
	b .L_0200b528
.L_0200b512:
	movs r1, #152
	movs r2, #151
	lsls r1, r1, #17
	lsls r2, r2, #16
	movs r0, #8
	bl Engine_ActorSetPosition
	movs r0, #8
	bl Object_GetById
	ldr r5, .L_0200b53c
.L_0200b528:
	str r5, [r0, #24]
	movs r0, #8
	bl Object_GetById
	str r5, [r0, #28]
	b .L_0200b54e
.L_0200b534:
	.4byte gMapWork
.L_0200b538:
	.4byte gFrameCount
.L_0200b53c:
	.4byte 0x00014ccc
.L_0200b540:
	movs r1, #128
	movs r2, #128
	movs r0, #8
	lsls r1, r1, #12
	lsls r2, r2, #12
	bl Engine_ActorSetPosition
.L_0200b54e:
	mov r1, r10
	cmp r1, #0
	beq .L_0200b618
	ldr r3, .L_0200b5f8
	ldr r6, [r3]
	movs r3, #15
	ands r6, r3
	cmp r6, #0
	bne .L_0200b618
	mov r0, r10
	ldr r2, [r0, #12]
	ldr r1, [r1, #8]
	movs r3, #128
	lsls r3, r3, #12
	add r2, r8
	adds r1, r1, r3
	adds r2, r2, r3
	ldr r3, [r0, #16]
	movs r0, #142
	lsls r0, r0, #1
	bl CreateOverlayObject
	movs r1, #192
	lsls r1, r1, #11
	adds r7, r0, #0
	mov r0, r8
	bl IwramSignedDivideEntry
	mov r8, r0
	mov r1, r8
	lsls r1, r1, #16
	mov r8, r1
	cmp r7, #0
	beq .L_0200b618
	ldr r1, .L_0200b5fc
	adds r0, r7, #0
	ldr r5, [r7, #80]
	bl Engine_ObjectSetScript
	movs r1, #3
	adds r0, r7, #0
	bl ObjectGroup_SetChildValue
	adds r3, r7, #0
	adds r3, #85
	strb r6, [r3]
	bl Engine_RandomNext
	ldr r3, .L_0200b600
	adds r2, r7, #0
	ands r3, r0
	adds r2, #100
	ldr r0, .L_0200b5f4
	strh r3, [r2]
	adds r3, r7, #0
	mov r9, r0
	adds r3, #102
	ldr r0, .L_0200b604
	strh r6, [r3]
	mov r2, r8
	ldr r3, .L_0200b608
	mov r1, r10
	ands r0, r2
	str r1, [r7, #104]
	str r3, [r7, #108]
	asrs r0, r0, #4
	bl Engine_MathSin
	lsls r3, r0, #1
	adds r3, r3, r0
	lsls r3, r3, #3
	asrs r3, r3, #16
	str r3, [r7, #48]
	adds r3, r5, #0
	adds r3, #38
	mov r0, r9
	strb r0, [r3]
	mov r1, r10
	ldr r3, [r1, #80]
	ldrb r3, [r3, #9]
	movs r2, #12
	ands r2, r3
	b .L_0200b60c
.L_0200b5f4:
	.4byte 0x00000000
.L_0200b5f8:
	.4byte gFrameCount
.L_0200b5fc:
	.4byte Data_02003c54
.L_0200b600:
	.4byte 0x0ffff000
.L_0200b604:
	.4byte 0x000fffff
.L_0200b608:
	.4byte SceneEffect_UpdateArcPosition
.L_0200b60c:
	ldrb r1, [r5, #9]
	movs r3, #13
	negs r3, r3
	ands r3, r1
	orrs r3, r2
	strb r3, [r5, #9]
.L_0200b618:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
	.section .rodata.x0200b884,"a",%progbits
.L_0200b884:
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
.L_0200b8bc:
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
.L_0200b8f4:
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
	.4byte .L_0200b884
	.4byte .L_0200b8bc
	.4byte .L_0200b8f4
	.global MakyuriChojo_ScriptTable
MakyuriChojo_ScriptTable:
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000008
	.4byte 0x40000008
	.4byte 0x00000000
	.4byte 0x02600000
	.4byte 0x000001a8
	.4byte 0xffff0002
	.4byte 0x00000008
	.4byte 0x40000008
	.4byte 0x00000000
	.4byte 0x02600000
	.4byte 0x000001a8
	.4byte 0xffff0005
	.4byte 0x000001f8
	.4byte 0x40000098
	.4byte 0x00000000
	.4byte 0x02600000
	.4byte 0x000001a8
	.4byte 0xffff0033
	.4byte 0x000001f8
	.4byte 0x400000a8
	.4byte 0x00000000
	.4byte 0x02600000
	.4byte 0x000001a8
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global MakyuriChojo_MessageTable
MakyuriChojo_MessageTable:
	.4byte 0x0000003a
	.4byte 0x0010f039
	.4byte 0x000001ff
	.global MakyuriChojo_ActorTable
MakyuriChojo_ActorTable:
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00026000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00022000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0x02530005
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00020000
	.4byte 0xffff00f4
	.4byte 0x00000007
	.4byte 0x01300000
	.4byte 0x00280000
	.4byte 0x00900000
	.4byte 0x00024000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x00024000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x01024000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x01180000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x01024000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x01480000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x01024000
	.4byte 0xffff00e0
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x00880000
	.4byte 0x01024000
	.4byte 0xffff00f2
	.4byte 0x00000007
	.4byte 0x01b80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00024000
	.4byte 0xffff00f2
	.4byte 0x00000007
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x01024000
	.4byte 0xffff00f2
	.4byte 0x00000007
	.4byte 0x01f80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x01024000
	.4byte 0xffff00f2
	.4byte 0x00000007
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x01024000
	.4byte 0xffff00f2
	.4byte 0x00000007
	.4byte 0x00880000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x01024000
	.4byte 0xffff00f2
	.4byte 0x00000007
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x01024000
	.4byte 0x0253001e
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00020000
	.4byte 0x02530023
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00a00000
	.4byte 0x00020000
	.4byte 0xffff0020
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0x02530021
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x00020000
	.4byte 0xffff002b
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00020000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global MakyuriChojo_EventTable
MakyuriChojo_EventTable:
	.4byte 0x00000002
	.4byte 0x08800005
	.4byte RunScene58Sequence
	.4byte 0x00000002
	.4byte 0x02510006
	.4byte FieldScene_RunScene39d_02002ddc
	.4byte 0x00000202
	.4byte 0xffff000b
	.4byte SceneActor_InitializeMotion
	.4byte 0x00000002
	.4byte 0xffff000b
	.4byte SceneActor_SetMode55OnSevenRecords
	.4byte 0x00000202
	.4byte 0xffff000a
	.4byte SceneActor_InitializeMotion
	.4byte 0x00000002
	.4byte 0x0250000a
	.4byte FieldScene_RunScene39d_020009fc
	.4byte 0x00000003
	.4byte 0x03500064
	.4byte 0x00300000
	.4byte 0x00000002
	.4byte 0xffff000c
	.4byte FieldScene_RunScene39d_02002eb8
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global MakyuriChojo_NearestActor
MakyuriChojo_NearestActor:
	.4byte 0x00000000
	.global Data_02003c54
Data_02003c54:
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x00016666
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x00016666
	.4byte 0x0000001c
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x0000001b
