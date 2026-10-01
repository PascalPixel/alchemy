.syntax unified
	.thumb
	.section .text.x020080c2,"ax",%progbits
	.2byte 0x0000
	.section .text.x020080c4,"ax",%progbits
	.global FuneKanpan_UpdateHoverGullA
	.thumb_func
FuneKanpan_UpdateHoverGullA:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	adds r6, r5, #0
	adds r6, #98
	ldrb r1, [r6]
	mov r8, r1
	cmp r1, #0
	bne .L_020080da
	b .L_0200821a
.L_020080da:
	adds r3, r1, #0
	subs r3, #1
	cmp r3, #7
	bls .L_020080e4
	b .L_020082b2
.L_020080e4:
	ldr r2, .L_020082d0
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_020080ec:
	.4byte .L_0200810c
	.4byte .L_020081d4
	.4byte .L_0200812c
	.4byte .L_020081d4
	.4byte .L_020081aa
	.4byte .L_020081d4
	.4byte .L_020081dc
	.4byte .L_02008214
.L_0200810c:
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r5, #48]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #52]
	movs r1, #134
	movs r2, #160
	movs r3, #173
	lsls r3, r3, #18
	adds r0, r5, #0
	lsls r1, r1, #17
	lsls r2, r2, #13
	bl Engine_ObjectSetPosition
	b .L_020081d4
.L_0200812c:
	movs r2, #128
	ldr r3, [r5, #56]
	lsls r2, r2, #24
	cmp r3, r2
	beq .L_02008138
	b .L_020082b2
.L_02008138:
	ldr r2, [r5, #60]
	cmp r2, r3
	beq .L_02008140
	b .L_020082b2
.L_02008140:
	ldr r3, [r5, #64]
	cmp r3, r2
	beq .L_02008148
	b .L_020082b2
.L_02008148:
	ldrb r3, [r6]
	movs r0, #146
	adds r3, #1
	strb r3, [r6]
	bl Engine_AudioPlayCue
	adds r3, r5, #0
	adds r3, #99
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0200816c
	movs r1, #208
	movs r0, #21
	lsls r1, r1, #8
	movs r2, #0
	bl Engine_ActorFaceDirection
	b .L_02008178
.L_0200816c:
	movs r1, #176
	movs r0, #21
	lsls r1, r1, #8
	movs r2, #0
	bl Engine_ActorFaceDirection
.L_02008178:
	bl Engine_RandomNext
	lsls r0, r0, #2
	lsrs r0, r0, #16
	cmp r0, #0
	beq .L_02008192
	movs r0, #21
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r0, #40]
	b .L_020082b2
.L_02008192:
	movs r0, #21
	ldr r1, .L_020082d4
	movs r2, #0
	bl Engine_ActorShowEmote
	movs r0, #21
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r0, #40]
	b .L_020082b2
.L_020081aa:
	adds r3, r5, #0
	adds r3, #99
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_020081c4
	movs r1, #141
	adds r0, r5, #0
	lsls r1, r1, #17
	movs r2, #0
	ldr r3, .L_020082d8
	bl Engine_ObjectSetPosition
	b .L_020081d4
.L_020081c4:
	movs r1, #254
	movs r3, #167
	adds r0, r5, #0
	lsls r1, r1, #16
	movs r2, #0
	lsls r3, r3, #18
	bl Engine_ObjectSetPosition
.L_020081d4:
	ldrb r3, [r6]
	adds r3, #1
	strb r3, [r6]
	b .L_020082b2
.L_020081dc:
	movs r1, #128
	ldr r3, [r5, #56]
	lsls r1, r1, #24
	cmp r3, r1
	bne .L_020082b2
	ldr r2, [r5, #60]
	cmp r2, r3
	bne .L_020082b2
	ldr r3, [r5, #64]
	cmp r3, r2
	bne .L_020082b2
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #48]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #52]
	adds r3, r5, #0
	movs r2, #0
	adds r3, #100
	strh r2, [r3]
	adds r3, #2
	strh r2, [r3]
	ldrb r3, [r6]
	adds r3, #1
	strb r3, [r6]
	str r2, [r5, #76]
	b .L_020082b2
.L_02008214:
	movs r3, #0
	strb r3, [r6]
	b .L_020082b2
.L_0200821a:
	adds r7, r5, #0
	adds r7, #100
	movs r2, #0
	ldrsh r3, [r7, r2]
	cmp r3, #0
	beq .L_02008240
	bl Engine_RandomNext
	ldr r3, [r5, #76]
	lsls r0, r0, #12
	lsrs r0, r0, #16
	ldr r1, .L_020082dc
	subs r3, r3, r0
	str r3, [r5, #76]
	cmp r3, r1
	bge .L_0200825a
	mov r2, r8
	strh r2, [r7]
	b .L_0200825a
.L_02008240:
	bl Engine_RandomNext
	ldr r3, [r5, #76]
	lsls r0, r0, #12
	lsrs r0, r0, #16
	movs r1, #128
	adds r3, r3, r0
	lsls r1, r1, #7
	str r3, [r5, #76]
	cmp r3, r1
	ble .L_0200825a
	movs r3, #1
	strh r3, [r7]
.L_0200825a:
	ldr r1, .L_020082e0
	ldr r2, [r5, #8]
	adds r3, r2, r1
	ldr r1, .L_020082e4
	cmp r3, r1
	bhi .L_0200826c
	ldr r3, [r5, #76]
	adds r3, r2, r3
	str r3, [r5, #8]
.L_0200826c:
	adds r7, r5, #0
	adds r7, #102
	movs r2, #0
	ldrsh r3, [r7, r2]
	cmp r3, #0
	beq .L_02008292
	bl Engine_RandomNext
	ldr r3, [r5, #12]
	lsls r0, r0, #15
	lsrs r0, r0, #16
	ldr r1, .L_020082e8
	subs r3, r3, r0
	adds r3, r3, r1
	str r3, [r5, #12]
	cmp r3, #0
	bge .L_020082b2
	movs r3, #0
	b .L_020082b0
.L_02008292:
	bl Engine_RandomNext
	ldr r3, [r5, #12]
	lsls r0, r0, #15
	lsrs r0, r0, #16
	movs r2, #128
	adds r3, r3, r0
	lsls r2, r2, #8
	movs r1, #128
	adds r3, r3, r2
	lsls r1, r1, #12
	str r3, [r5, #12]
	cmp r3, r1
	ble .L_020082b2
	movs r3, #1
.L_020082b0:
	strh r3, [r7]
.L_020082b2:
	bl Engine_RandomNext
	movs r3, #100
	muls r3, r0
	lsrs r3, r3, #16
	cmp r3, #0
	bne .L_020082c4
	movs r3, #1
	strb r3, [r6]
.L_020082c4:
	movs r0, #1
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r1}
	bx r1
.L_020082d0:
	.4byte .L_020080ec
.L_020082d4:
	.4byte 0x00000103
.L_020082d8:
	.4byte 0x02920000
.L_020082dc:
	.4byte 0xffffc000
.L_020082e0:
	.4byte 0xff07ffff
.L_020082e4:
	.4byte 0x002bfffe
.L_020082e8:
	.4byte 0xffff8000
	.section .text.x020082ec,"ax",%progbits
	.global FuneKanpan_UpdateHoverGullB
	.thumb_func
FuneKanpan_UpdateHoverGullB:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	adds r6, r5, #0
	adds r6, #98
	ldrb r1, [r6]
	mov r8, r1
	cmp r1, #0
	bne .L_02008302
	b .L_02008444
.L_02008302:
	adds r3, r1, #0
	subs r3, #1
	cmp r3, #7
	bls .L_0200830c
	b .L_020084dc
.L_0200830c:
	ldr r2, .L_020084fc
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_02008314:
	.4byte .L_02008334
	.4byte .L_020083fe
	.4byte .L_02008354
	.4byte .L_020083fe
	.4byte .L_020083d2
	.4byte .L_020083fe
	.4byte .L_02008406
	.4byte .L_0200843e
.L_02008334:
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r5, #48]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #52]
	movs r1, #128
	movs r2, #160
	movs r3, #160
	lsls r3, r3, #18
	adds r0, r5, #0
	lsls r1, r1, #17
	lsls r2, r2, #13
	bl Engine_ObjectSetPosition
	b .L_020083fe
.L_02008354:
	movs r2, #128
	ldr r3, [r5, #56]
	lsls r2, r2, #24
	cmp r3, r2
	beq .L_02008360
	b .L_020084dc
.L_02008360:
	ldr r2, [r5, #60]
	cmp r2, r3
	beq .L_02008368
	b .L_020084dc
.L_02008368:
	ldr r3, [r5, #64]
	cmp r3, r2
	beq .L_02008370
	b .L_020084dc
.L_02008370:
	ldrb r3, [r6]
	movs r0, #146
	adds r3, #1
	strb r3, [r6]
	bl Engine_AudioPlayCue
	adds r3, r5, #0
	adds r3, #99
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_02008394
	movs r1, #208
	movs r0, #22
	lsls r1, r1, #8
	movs r2, #0
	bl Engine_ActorFaceDirection
	b .L_020083a0
.L_02008394:
	movs r1, #176
	movs r0, #22
	lsls r1, r1, #8
	movs r2, #0
	bl Engine_ActorFaceDirection
.L_020083a0:
	bl Engine_RandomNext
	lsls r0, r0, #2
	lsrs r0, r0, #16
	cmp r0, #0
	beq .L_020083ba
	movs r0, #22
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r0, #40]
	b .L_020084dc
.L_020083ba:
	movs r0, #22
	ldr r1, .L_02008500
	movs r2, #0
	bl Engine_ActorShowEmote
	movs r0, #22
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r0, #40]
	b .L_020084dc
.L_020083d2:
	adds r3, r5, #0
	adds r3, #99
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_020083ee
	movs r1, #132
	movs r3, #150
	adds r0, r5, #0
	lsls r1, r1, #17
	movs r2, #0
	lsls r3, r3, #18
	bl Engine_ObjectSetPosition
	b .L_020083fe
.L_020083ee:
	movs r1, #242
	movs r3, #151
	adds r0, r5, #0
	lsls r1, r1, #16
	movs r2, #0
	lsls r3, r3, #18
	bl Engine_ObjectSetPosition
.L_020083fe:
	ldrb r3, [r6]
	adds r3, #1
	strb r3, [r6]
	b .L_020084dc
.L_02008406:
	movs r1, #128
	ldr r3, [r5, #56]
	lsls r1, r1, #24
	cmp r3, r1
	bne .L_020084dc
	ldr r2, [r5, #60]
	cmp r2, r3
	bne .L_020084dc
	ldr r3, [r5, #64]
	cmp r3, r2
	bne .L_020084dc
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #48]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #52]
	adds r3, r5, #0
	movs r2, #0
	adds r3, #100
	strh r2, [r3]
	adds r3, #2
	strh r2, [r3]
	ldrb r3, [r6]
	adds r3, #1
	strb r3, [r6]
	str r2, [r5, #76]
	b .L_020084dc
.L_0200843e:
	movs r3, #0
	strb r3, [r6]
	b .L_020084dc
.L_02008444:
	adds r7, r5, #0
	adds r7, #100
	movs r2, #0
	ldrsh r3, [r7, r2]
	cmp r3, #0
	beq .L_0200846a
	bl Engine_RandomNext
	ldr r3, [r5, #76]
	lsls r0, r0, #12
	lsrs r0, r0, #16
	ldr r1, .L_02008504
	subs r3, r3, r0
	str r3, [r5, #76]
	cmp r3, r1
	bge .L_02008484
	mov r2, r8
	strh r2, [r7]
	b .L_02008484
.L_0200846a:
	bl Engine_RandomNext
	ldr r3, [r5, #76]
	lsls r0, r0, #12
	lsrs r0, r0, #16
	movs r1, #128
	adds r3, r3, r0
	lsls r1, r1, #7
	str r3, [r5, #76]
	cmp r3, r1
	ble .L_02008484
	movs r3, #1
	strh r3, [r7]
.L_02008484:
	ldr r1, .L_02008508
	ldr r2, [r5, #8]
	adds r3, r2, r1
	ldr r1, .L_0200850c
	cmp r3, r1
	bhi .L_02008496
	ldr r3, [r5, #76]
	adds r3, r2, r3
	str r3, [r5, #8]
.L_02008496:
	adds r7, r5, #0
	adds r7, #102
	movs r2, #0
	ldrsh r3, [r7, r2]
	cmp r3, #0
	beq .L_020084bc
	bl Engine_RandomNext
	ldr r3, [r5, #12]
	lsls r0, r0, #15
	lsrs r0, r0, #16
	ldr r1, .L_02008510
	subs r3, r3, r0
	adds r3, r3, r1
	str r3, [r5, #12]
	cmp r3, #0
	bge .L_020084dc
	movs r3, #0
	b .L_020084da
.L_020084bc:
	bl Engine_RandomNext
	ldr r3, [r5, #12]
	lsls r0, r0, #15
	lsrs r0, r0, #16
	movs r2, #128
	adds r3, r3, r0
	lsls r2, r2, #8
	movs r1, #128
	adds r3, r3, r2
	lsls r1, r1, #12
	str r3, [r5, #12]
	cmp r3, r1
	ble .L_020084dc
	movs r3, #1
.L_020084da:
	strh r3, [r7]
.L_020084dc:
	bl Engine_RandomNext
	movs r3, #100
	muls r3, r0
	lsrs r3, r3, #16
	cmp r3, #0
	bne .L_020084ee
	movs r3, #1
	strb r3, [r6]
.L_020084ee:
	movs r0, #1
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
.L_020084fc:
	.4byte .L_02008314
.L_02008500:
	.4byte 0x00000103
.L_02008504:
	.4byte 0xffffc000
.L_02008508:
	.4byte 0xff17ffff
.L_0200850c:
	.4byte 0x0027fffe
.L_02008510:
	.4byte 0xffff8000
	.section .rodata.x0200c464,"a",%progbits
	.global FuneKanpan_PresentationActionsA
FuneKanpan_PresentationActionsA:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00ca0000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000010
	.global FuneKanpan_PresentationActionsB
FuneKanpan_PresentationActionsB:
	.4byte 0x00000000
	.4byte 0x00000014
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00a20000
	.4byte 0x00000000
	.4byte 0x029a0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000010
	.global FuneKanpan_RandomActorActions
FuneKanpan_RandomActorActions:
	.4byte 0x00000022
	.4byte SceneActor_UpdateScalePulse
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global FuneKanpan_DeckEventActions
FuneKanpan_DeckEventActions:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x008c0000
	.4byte 0x00000000
	.4byte 0x024a0000
	.4byte 0x00000001
	.4byte 0x0000001e
	.4byte 0x00000099
	.4byte 0x00000015
	.4byte 0x0000000d
	.4byte 0x00080000
	.4byte 0x00000003
	.4byte 0x00b80000
	.4byte 0x00000000
	.4byte 0x02560000
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x0000000d
	.4byte 0x00040000
	.4byte 0x00000022
	.4byte SceneState_ApplyArgMode1AndReturnZero
	.4byte 0x00000003
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02540000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x024a0000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000010
	.global FuneKanpan_LeadActionsA
FuneKanpan_LeadActionsA:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00e40000
	.4byte 0x00000000
	.4byte 0x02090000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x0000001c
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00cc0000
	.4byte 0x00000000
	.4byte 0x02090000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000b000
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x0000001c
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global FuneKanpan_LeadActionsB
FuneKanpan_LeadActionsB:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x02a00000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000046
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000b000
	.4byte 0x00000000
	.4byte 0x00000032
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00c40000
	.4byte 0x00000000
	.4byte 0x02a00000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00003000
	.4byte 0x00000000
	.4byte 0x00000046
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00c40000
	.4byte 0x00000000
	.4byte 0x027c0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00008000
	.4byte 0x00000000
	.4byte 0x00000032
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00005000
	.4byte 0x00000000
	.4byte 0x0000001e
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x027c0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x0000b000
	.4byte 0x00000000
	.4byte 0x00000046
	.4byte 0x00000015
	.4byte 0x00000003
	.4byte 0x00003000
	.4byte 0x00000000
	.4byte 0x00000028
	.4byte 0x0000000c
	.4byte 0x0000ffff
	.4byte 0x00000000
	.global FuneKanpan_CrewActionsA
FuneKanpan_CrewActionsA:
	.4byte 0x00000015
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000024
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000027
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000017
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte FuneKanpan_UpdateHoverGullA
	.global FuneKanpan_CrewActionsB
FuneKanpan_CrewActionsB:
	.4byte 0x00000015
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000024
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000027
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000017
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte FuneKanpan_UpdateHoverGullB
	.global FuneKanpan_CrewActionsC
FuneKanpan_CrewActionsC:
	.4byte 0x00000015
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000015
	.4byte 0x00000024
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte SceneActor_OscillateHeightBetweenLimits
	.global FuneKanpan_CrewActionsD
FuneKanpan_CrewActionsD:
	.4byte 0x00000022
	.4byte SceneActor_SetFacingFromSample
	.global FuneKanpan_CrewActionsE
FuneKanpan_CrewActionsE:
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000015
	.4byte 0x00000009
	.4byte 0x0000b333
	.4byte 0x00000015
	.4byte 0x0000000a
	.4byte 0x0000b333
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x80010000
	.4byte 0x00000016
	.4byte 0x00000009
	.4byte 0x00000f5c
	.4byte 0x00000016
	.4byte 0x0000000a
	.4byte 0x0000147a
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000c
	.4byte 0x00000018
	.4byte 0xc0010000
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.global FuneKanpan_SailorActions
FuneKanpan_SailorActions:
	.4byte 0x00000003
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x024a0000
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000010
	.global FuneKanpan_DeckActionsB
FuneKanpan_DeckActionsB:
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte FuneKanpan_UpdateFlyByForActor21
	.global FuneKanpan_DeckActionsA
FuneKanpan_DeckActionsA:
	.4byte 0x00000015
	.4byte 0x00000023
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte FuneKanpan_UpdateFlyByForActor22
	.global FuneKanpan_DeckActionsC
FuneKanpan_DeckActionsC:
	.4byte 0x00000022
	.4byte SceneActor_SetWord28RandomlyOneIn40
	.global FuneKanpan_RosterActions
FuneKanpan_RosterActions:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00520000
	.4byte 0x00000000
	.4byte 0x02900000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000010
	.global FuneKanpan_LeaveActions
FuneKanpan_LeaveActions:
	.4byte 0x0000001c
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00ba0000
	.4byte 0x00000000
	.4byte 0x025c0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x025c0000
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x024a0000
	.4byte 0x00000001
	.4byte 0x0000001c
	.4byte 0x00000001
	.4byte 0x00000002
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000010
	.global FuneKanpan_LeadActionsC
FuneKanpan_LeadActionsC:
	.4byte 0x00000015
	.4byte 0x00000027
	.4byte 0x00000000
	.4byte 0x00000022
	.4byte FuneKanpan_IdleTurn
	.global FuneKanpan_SceneTableA
FuneKanpan_SceneTableA:
	.4byte 0xffff0000
	.4byte 0x000000b3
	.4byte 0x4000028d
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x000000d8
	.4byte 0x40000258
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0002
	.4byte 0x000000e8
	.4byte 0x00000298
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0003
	.4byte 0x000000d8
	.4byte 0x40000328
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0004
	.4byte 0x000400d8
	.4byte 0x40000288
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0005
	.4byte 0xfffc0138
	.4byte 0x80000290
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0006
	.4byte 0xfffc0078
	.4byte 0x00000290
	.4byte 0x00410000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000a
	.4byte 0x000000e8
	.4byte 0x00000298
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000b
	.4byte 0x000000d4
	.4byte 0x00000209
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000c
	.4byte 0x000000c0
	.4byte 0x0000028a
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000d
	.4byte 0x000000e8
	.4byte 0x0000028e
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000e
	.4byte 0x000000d8
	.4byte 0x00000291
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff000f
	.4byte 0x000000d8
	.4byte 0x40000280
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0010
	.4byte 0x000000d8
	.4byte 0xc000021c
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0011
	.4byte 0x000000d8
	.4byte 0xc000021c
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0012
	.4byte 0x000000c4
	.4byte 0x4000028c
	.4byte 0x00410000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0013
	.4byte 0x000000d4
	.4byte 0x40000209
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global FuneKanpan_SceneTableB
FuneKanpan_SceneTableB:
	.4byte 0x001800d0
	.4byte 0x00e0027a
	.4byte 0x028a0028
	.4byte 0x0005ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global FuneKanpan_SceneTableC
FuneKanpan_SceneTableC:
	.4byte 0x0000006d
	.4byte 0x0010106f
	.4byte 0x0020306f
	.4byte 0x0030506f
	.4byte 0x0040206b
	.4byte 0x0050106e
	.4byte 0x00601070
	.4byte 0x0074a002
	.4byte 0x00a0a06e
	.4byte 0x00b0e06f
	.4byte 0x00c0f06f
	.4byte 0x00d1106f
	.4byte 0x00e1306f
	.4byte 0x00f46002
	.4byte 0x0101606f
	.4byte 0x0111806f
	.4byte 0x000001ff
	.global gFuneKanpanPlacements
gFuneKanpanPlacements:
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x01023000
	.4byte 0xffff0091
	.4byte 0x00000001
	.4byte 0x010a0000
	.4byte 0x00000000
	.4byte 0x02bf0000
	.4byte 0x00010000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gFuneKanpanPlacementsFlag911
gFuneKanpanPlacementsFlag911:
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x01023000
	.4byte 0xffff0091
	.4byte 0x00000001
	.4byte 0x00b70000
	.4byte 0x00000000
	.4byte 0x02d00000
	.4byte 0x0001d000
	.4byte 0xffff0092
	.4byte 0x00000001
	.4byte 0x01000000
	.4byte 0x00000000
	.4byte 0x02c80000
	.4byte 0x0002b000
	.4byte 0xffff002e
	.4byte 0x00000001
	.4byte 0x00c60000
	.4byte 0x00000000
	.4byte 0x02c20000
	.4byte 0x00015000
	.4byte 0xffff002f
	.4byte 0x00000001
	.4byte 0x00ac0000
	.4byte 0x00000000
	.4byte 0x02c20000
	.4byte 0x00000000
	.4byte 0xffff00ab
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00023000
	.4byte 0xffff0084
	.4byte 0x00000001
	.4byte 0x00e60000
	.4byte 0x00000000
	.4byte 0x025a0000
	.4byte 0x00003000
	.4byte 0xffff0094
	.4byte 0x00000001
	.4byte 0x00ec0000
	.4byte 0x00000000
	.4byte 0x026d0000
	.4byte 0x0000b000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00b60000
	.4byte 0x00000000
	.4byte 0x022c0000
	.4byte 0x00018000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gFuneKanpanPlacementsFlag927
gFuneKanpanPlacementsFlag927:
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02910000
	.4byte 0x00023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02910000
	.4byte 0x00023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02910000
	.4byte 0x00023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02910000
	.4byte 0x00023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x01023000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gFuneKanpanPlacementsFlag928
gFuneKanpanPlacementsFlag928:
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x01023000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global FuneKanpan_CrewScript
FuneKanpan_CrewScript:
	.4byte 0xffff002f
	.4byte 0x00000001
	.4byte 0x00c00000
	.4byte 0x00000000
	.4byte 0x02000000
	.4byte 0x0002b000
	.4byte 0xffff0043
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x027a0000
	.4byte 0x0000b000
	.4byte 0xffff0042
	.4byte 0x00000001
	.4byte 0x00d40000
	.4byte 0x00000000
	.4byte 0x02090000
	.4byte 0x0000b000
	.4byte 0xffff0092
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000d000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global FuneKanpan_ClosingCrewScript
FuneKanpan_ClosingCrewScript:
	.4byte 0xffff00ab
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00025000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global FuneKanpan_CrewScriptB
FuneKanpan_CrewScriptB:
	.4byte 0xffff00bd
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00bd
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff00bd
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global FuneKanpan_CrewScriptC
FuneKanpan_CrewScriptC:
	.4byte 0xffff00c4
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00c4
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00c4
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00c4
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00c4
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00c4
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00c6
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00d6
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global FuneKanpan_CrewScriptD
FuneKanpan_CrewScriptD:
	.4byte 0xffff00bd
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff00bd
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff00bd
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff00bd
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff00bd
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff00bd
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01024000
	.4byte 0xffff00c5
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0016
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global FuneKanpan_CrewScriptE
FuneKanpan_CrewScriptE:
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0002
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff0003
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00020000
	.4byte 0xffff002f
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0002b000
	.4byte 0xffff0043
	.4byte 0x00000001
	.4byte 0x00b20000
	.4byte 0x00000000
	.4byte 0x02820000
	.4byte 0x0000b000
	.4byte 0xffff0042
	.4byte 0x00000001
	.4byte 0x00b60000
	.4byte 0x00000000
	.4byte 0x02160000
	.4byte 0x0000b000
	.4byte 0xffff0092
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x0000d000
	.4byte 0xffff00c2
	.4byte 0x00000007
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x021c0000
	.4byte 0x00004000
	.4byte 0xffff0016
	.4byte 0x00000007
	.4byte 0x00d80000
	.4byte 0x00000000
	.4byte 0x02800000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gFuneKanpanPlacementsFlag93e
gFuneKanpanPlacementsFlag93e:
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x00023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x00840000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02780000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02980000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x01023000
	.4byte 0xffff00e1
	.4byte 0x00000001
	.4byte 0x012c0000
	.4byte 0x00000000
	.4byte 0x02d80000
	.4byte 0x01023000
	.4byte 0xffff002f
	.4byte 0x00000001
	.4byte 0x00bc0000
	.4byte 0x00000000
	.4byte 0x02000000
	.4byte 0x00003000
	.4byte 0xffff0091
	.4byte 0x00000001
	.4byte 0x00a20000
	.4byte 0x00000000
	.4byte 0x027a0000
	.4byte 0x00013000
	.4byte 0xffff0092
	.4byte 0x00000001
	.4byte 0x00a20000
	.4byte 0x00000000
	.4byte 0x02a40000
	.4byte 0x0001d000
	.4byte 0xffff00ab
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff00ab
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0084
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0094
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff009d
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff006c
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff006a
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0067
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0016
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gFuneKanpanEvents
gFuneKanpanEvents:
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte FuneKanpan_RunDeckStateEvent
	.ifdef TBS_EDITION_EN
	.4byte 0x00008602
	.else
	.ifdef TBS_EDITION_JA
	.4byte 0x00008602
	.else
	.4byte 0x00000002
	.endif
	.endif
	.4byte 0xffff0002
	.ifdef TBS_EDITION_JA
	.4byte FuneKanpan_RunDeckStateEvent
	.else
	.ifdef TBS_EDITION_EN
	.4byte FuneKanpan_RunDeckStateEvent
	.else
	.4byte FuneKanpan_RunDeckStateEventFromEntry
	.endif
	.endif
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte FuneKanpan_RunDeckStateEvent
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0x09230014
	.4byte MsgFuneKanpanThereAreALotOfPassengers
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte MsgFuneKanpanOhAtTheRiskOfSounding
	.4byte 0x00008d15
	.4byte 0x09230014
	.4byte MsgFuneKanpanTheReplacementShipFromTolbiShould
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte MsgFuneKanpanIHidTheAnchorCharmNo
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte MsgFuneKanpanWeApologizeForAnyTroubleThat
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte FieldScene_RunOpeningAuxiliarySequence
	.4byte 0x00000000
	.4byte 0xffff0018
	.4byte FieldScene_RunScene3afSequenceA
	.4byte 0x00000000
	.4byte 0x09220019
	.4byte MsgFuneKanpanIWonderWhatsGoingOnWeve
	.4byte 0x00000000
	.4byte 0x09250019
	.4byte MsgFuneKanpanThoseWarriorsGotTiredOfWaiting
	.4byte 0x00000000
	.4byte 0xffff0019
	.4byte MsgFuneKanpanIfWeJustPromiseToHelp
	.4byte 0x00000000
	.4byte 0x0922001a
	.4byte MsgFuneKanpanIWonderIfImTheOnly
	.4byte 0x00000000
	.4byte 0x0925001a
	.4byte MsgFuneKanpanIWonderIfThoseTwoAre
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte MsgFuneKanpanIllAgreeToAnythingEvenRowing
	.4byte 0x00000000
	.4byte 0x0922001b
	.4byte MsgFuneKanpanTheKaragolIsLikeAHumongous
	.4byte 0x00000000
	.4byte 0x0925001b
	.4byte MsgFuneKanpanHaHahThoseMercenariesAreGoing
	.4byte 0x00000000
	.4byte 0xffff001b
	.4byte MsgFuneKanpanThatSeaDogKajaIsA
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte MsgFuneKanpanThereIsNothingToFearAs
	.4byte 0x00008d15
	.4byte 0x09220015
	.4byte MsgFuneKanpanWhenAreWeGoingWhenAre
	.4byte 0x00008d15
	.4byte 0x09250015
	.4byte MsgFuneKanpanUsingForceToLaunchTheShip
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte MsgFuneKanpanAreYouSeriousAboutSettingSail
	.4byte 0x00008d15
	.4byte 0x09220018
	.4byte MsgFuneKanpanHmmmThisIsSoFrustratingWhy
	.4byte 0x00008d15
	.4byte 0x09250018
	.4byte MsgFuneKanpanTheyWereJustAboutToLaunch
	.4byte 0x00008d15
	.4byte 0xffff0018
	.4byte MsgFuneKanpanItProbablyWontMakeAnyDifference
	.4byte 0x00008d15
	.4byte 0x09220019
	.4byte MsgFuneKanpanTravelingByShipIsFasterThan
	.4byte 0x00008d15
	.4byte 0x09250019
	.4byte MsgFuneKanpanThoseThugsAreTooImpatientTrying
	.4byte 0x00008d15
	.4byte 0xffff0019
	.4byte MsgFuneKanpanUltimatelyTheyreJustBeingThreatenedInto
	.4byte 0x00008d15
	.4byte 0x0922001a
	.4byte MsgFuneKanpanMostOfThePeopleGoingTo
	.4byte 0x00008d15
	.4byte 0x0925001a
	.4byte MsgFuneKanpanHeeHeeIfWeCanGet
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte MsgFuneKanpanBesidesRowingIsAMansJob
	.4byte 0x00008d15
	.4byte 0x0922001b
	.4byte MsgFuneKanpanTheKaragolSeaIsPrettyFoggy
	.4byte 0x00008d15
	.4byte 0x0925001b
	.4byte MsgFuneKanpanThoseGuysSeemPrettySureOf
	.4byte 0x00008d15
	.4byte 0xffff001b
	.4byte MsgFuneKanpanUsHelpWithTheRowingGet
	.4byte 0x00000002
	.4byte 0x0920000a
	.4byte FieldScene_RunActorAndEffectPresentationSetup
	.4byte 0x00000002
	.4byte 0x0923000b
	.4byte FieldScene_RunScene3af_020010a0
	.4byte 0x00000002
	.4byte 0x0923000c
	.4byte FieldScene_RunScene3af_020011c8
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gFuneKanpanEventsFlag928
gFuneKanpanEventsFlag928:
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte FuneKanpan_RunDeckStateEvent
	.ifdef TBS_EDITION_EN
	.4byte 0x00008602
	.else
	.ifdef TBS_EDITION_JA
	.4byte 0x00008602
	.else
	.4byte 0x00000002
	.endif
	.endif
	.4byte 0xffff0002
	.ifdef TBS_EDITION_JA
	.4byte FuneKanpan_RunDeckStateEvent
	.else
	.ifdef TBS_EDITION_EN
	.4byte FuneKanpan_RunDeckStateEvent
	.else
	.4byte FuneKanpan_RunDeckStateEventFromEntry
	.endif
	.endif
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte FuneKanpan_RunDeckStateEvent
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte MsgFuneKanpanYouMustFeelAwfullyImportantOrdering
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte MsgFuneKanpanItLooksLikeAnotherOarsmanWas
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte MsgFuneKanpanIFeelSorryForTheOarsmen
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte MsgFuneKanpanAnOarsmanWasInjuredByA
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte MsgFuneKanpanWeHaveNoIdeaWhenThose
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte MsgFuneKanpanUhnnnWhenILookAtThe
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gFuneKanpanEventsFlag93e
gFuneKanpanEventsFlag93e:
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte FuneKanpan_RunDeckStateEvent
	.ifdef TBS_EDITION_EN
	.4byte 0x00008602
	.else
	.ifdef TBS_EDITION_JA
	.4byte 0x00008602
	.else
	.4byte 0x00000002
	.endif
	.endif
	.4byte 0xffff0002
	.ifdef TBS_EDITION_JA
	.4byte FuneKanpan_RunDeckStateEvent
	.else
	.ifdef TBS_EDITION_EN
	.4byte FuneKanpan_RunDeckStateEvent
	.else
	.4byte FuneKanpan_RunDeckStateEventFromEntry
	.endif
	.endif
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte FuneKanpan_RunDeckStateEvent
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte MsgFuneKanpanYouLookinForSeanAndOuranos
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte SceneDialogue_RunActor21Line
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte MsgFuneKanpanThisGroupThatCameOnOur
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte MsgFuneKanpanIWantSeanAndOuranosTo
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte MsgFuneKanpanImJustGoingToHangOut
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte MsgFuneKanpanIWonderWhatHappenedToThat
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global gFuneKanpanEventsFlag8a0
gFuneKanpanEventsFlag8a0:
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte FuneKanpan_RunDeckStateEvent
	.ifdef TBS_EDITION_EN
	.4byte 0x00008602
	.else
	.ifdef TBS_EDITION_JA
	.4byte 0x00008602
	.else
	.4byte 0x00000002
	.endif
	.endif
	.4byte 0xffff0002
	.ifdef TBS_EDITION_JA
	.4byte FuneKanpan_RunDeckStateEvent
	.else
	.ifdef TBS_EDITION_EN
	.4byte FuneKanpan_RunDeckStateEvent
	.else
	.4byte FuneKanpan_RunDeckStateEventFromEntry
	.endif
	.endif
	.4byte 0x0000c602
	.4byte 0xffff0003
	.4byte FuneKanpan_RunDeckStateEvent
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0xffff0017
	.4byte MsgFuneKanpanKajaAndTheCaptainAreProfessionals
	.4byte 0x00000000
	.4byte 0x09030016
	.4byte FieldScene_RunThreeActorEncounter
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte MsgFuneKanpanTheresNoTellingWhatKindOf
	.4byte 0x00000000
	.4byte 0x09030015
	.4byte FieldScene_RunThreeActorEncounter
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte MsgFuneKanpanWeveDoneAGoodJobOf
	.4byte 0x00008d15
	.4byte 0xffff0017
	.4byte MsgFuneKanpanIWantToBeASailor
	.4byte 0x00008d15
	.4byte 0x09030016
	.4byte MsgFuneKanpanIGiveUpWhatAreWe
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte MsgFuneKanpanThisIslandGivesMeTheCreeps
	.4byte 0x00008d15
	.4byte 0x09030015
	.4byte MsgFuneKanpanThatsEnoughBickering
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte MsgFuneKanpanWhatWeCanDoNowIs
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global FuneKanpan_FlagGroupEntries
FuneKanpan_FlagGroupEntries:
	.4byte 0x00000003
	.4byte 0x00000002
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0x00000004
	.4byte 0x00000004
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0x00000001
