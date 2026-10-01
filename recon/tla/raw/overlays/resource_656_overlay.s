.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	push {lr}
	ldr r3, .L_02008060
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_02008064
	cmp r2, r3
	bne .L_02008050
	ldr r0, .L_02008068
	b .L_0200805c
.L_02008050:
	ldr r3, .L_0200806c
	cmp r2, r3
	bne .L_0200805a
	ldr r0, .L_02008070
	b .L_0200805c
.L_0200805a:
	ldr r0, .L_02008074
.L_0200805c:
	pop {pc}
	.2byte 0x0000
.L_02008060:
	.4byte gPartyState
.L_02008064:
	.4byte 0x00000028
.L_02008068:
	.4byte Data_02003084
.L_0200806c:
	.4byte 0x00000029
.L_02008070:
	.4byte Data_0200312c
.L_02008074:
	.4byte Data_02003054
	.section .text.x02008078,"ax",%progbits
	.global Func_02000078
	.thumb_func
Func_02000078:
	push {lr}
	ldr r3, .L_0200809c
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r2, [r3, r1]
	ldr r3, .L_020080a0
	cmp r2, r3
	bne .L_02008090
	ldr r0, .L_020080a4
	b .L_0200809a
.L_02008090:
	ldr r3, .L_020080a8
	movs r0, #0
	cmp r2, r3
	bne .L_0200809a
	ldr r0, .L_020080ac
.L_0200809a:
	pop {pc}
.L_0200809c:
	.4byte gPartyState
.L_020080a0:
	.4byte 0x00000028
.L_020080a4:
	.4byte Data_02003234
.L_020080a8:
	.4byte 0x00000029
.L_020080ac:
	.4byte Data_02003254
	.section .text.x020080b8,"ax",%progbits
	.global Func_020000b8
	.thumb_func
Func_020000b8:
	push {lr}
	ldr r2, .L_020081e8
	movs r0, #240
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r0, #0
	ldrsh r1, [r3, r0]
	ldr r3, .L_020081ec
	cmp r1, r3
	bne .L_02008168
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	subs r3, #3
	cmp r3, #27
	bls .L_020080de
	b .L_020081e4
.L_020080de:
	ldr r2, .L_020081f0
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_020080e8:
	.4byte .L_02008158
	.4byte .L_02008158
	.4byte .L_02008158
	.4byte .L_020081e4
	.4byte .L_020081e4
	.4byte .L_020081e4
	.4byte .L_020081e4
	.4byte .L_020081e4
	.4byte .L_020081e4
	.4byte .L_020081e4
	.4byte .L_020081e4
	.4byte .L_020081e4
	.4byte .L_02008160
	.4byte .L_02008160
	.4byte .L_02008160
	.4byte .L_02008160
	.4byte .L_020081e4
	.4byte .L_020081e4
	.4byte .L_020081e4
	.4byte .L_020081e4
	.4byte .L_020081e4
	.4byte .L_020081e4
	.4byte .L_0200815c
	.4byte .L_0200815c
	.4byte .L_0200815c
	.4byte .L_0200815c
	.4byte .L_02008164
	.4byte .L_02008164
.L_02008158:
	ldr r0, .L_020081f4
	b .L_020081e6
.L_0200815c:
	ldr r0, .L_020081f8
	b .L_020081e6
.L_02008160:
	ldr r0, .L_020081fc
	b .L_020081e6
.L_02008164:
	ldr r0, .L_02008200
	b .L_020081e6
.L_02008168:
	ldr r3, .L_02008204
	cmp r1, r3
	bne .L_020081e4
	movs r0, #241
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	subs r3, #1
	cmp r3, #19
	bhi .L_020081e0
	ldr r2, .L_02008208
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_02008188:
	.4byte .L_020081d8
	.4byte .L_020081d8
	.4byte .L_020081d8
	.4byte .L_020081d8
	.4byte .L_020081e0
	.4byte .L_020081dc
	.4byte .L_020081e0
	.4byte .L_020081e0
	.4byte .L_020081e0
	.4byte .L_020081dc
	.4byte .L_020081d8
	.4byte .L_020081d8
	.4byte .L_020081d8
	.4byte .L_020081d8
	.4byte .L_020081d8
	.4byte .L_020081d8
	.4byte .L_020081d8
	.4byte .L_020081d8
	.4byte .L_020081e0
	.4byte .L_020081d8
.L_020081d8:
	ldr r0, .L_0200820c
	b .L_020081e6
.L_020081dc:
	ldr r0, .L_02008210
	b .L_020081e6
.L_020081e0:
	ldr r0, .L_02008214
	b .L_020081e6
.L_020081e4:
	ldr r0, .L_02008218
.L_020081e6:
	pop {pc}
.L_020081e8:
	.4byte gPartyState
.L_020081ec:
	.4byte 0x00000028
.L_020081f0:
	.4byte .L_020080e8
.L_020081f4:
	.4byte Data_02003398
.L_020081f8:
	.4byte Data_0200b410
.L_020081fc:
	.4byte Data_02003560
.L_02008200:
	.4byte Data_02003590
.L_02008204:
	.4byte 0x00000029
.L_02008208:
	.4byte .L_02008188
.L_0200820c:
	.4byte Data_02003710
.L_02008210:
	.4byte Data_020037d0
.L_02008214:
	.4byte Data_02003848
.L_02008218:
	.4byte Data_02003380
	.section .text.x0200821c,"ax",%progbits
	.global Func_0200021c
	.thumb_func
Func_0200021c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	sub sp, #8
	mov r9, r3
	bl Func_02002c68
	movs r0, #0
	bl Func_02002da8
	movs r7, #0
	movs r5, #8
.L_0200823e:
	adds r0, r5, #0
	bl Object_GetById
	cmp r0, #0
	beq .L_02008250
	adds r2, r0, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
.L_02008250:
	adds r5, #1
	cmp r5, #63
	bls .L_0200823e
	ldr r0, .L_0200831c
	movs r5, #0
	movs r1, #0
.L_0200825c:
	movs r3, #170
	lsls r3, r3, #1
	add r3, r9
	movs r4, #0
	ldrsh r2, [r3, r4]
	ldrh r3, [r0, r1]
	cmp r2, r3
	bne .L_0200826e
	adds r7, r5, #0
.L_0200826e:
	adds r5, #1
	adds r1, #12
	cmp r5, #9
	bls .L_0200825c
	movs r0, #158
	bl Func_02002de0
	ldr r2, .L_02008320
	movs r3, #133
	mov r10, r2
	lsls r3, r3, #2
	add r10, r3
	mov r4, r10
	ldr r0, [r4]
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	lsls r5, r7, #1
	ldr r2, .L_0200831c
	adds r5, r5, r7
	lsls r5, r5, #2
	adds r6, r5, #0
	mov r8, r2
	adds r6, #8
	ldrh r2, [r2, r6]
	movs r1, #1
	add r6, r8
	movs r0, #2
	ldrh r3, [r6, #2]
	str r1, [sp, #0]
	str r0, [sp, #4]
	movs r1, #0
	movs r0, #0
	bl Func_02002c30
	ldrh r2, [r6, #2]
	mov r3, r8
	adds r5, #4
	ldr r0, [r3, r5]
	adds r2, #60
	ldrh r1, [r6]
	bl Func_02002c28
	mov r4, r10
	ldr r0, [r4]
	movs r1, #2
	bl Object_SetModeById
	mov r2, r10
	ldr r0, [r2]
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	mov r3, r10
	ldr r0, [r3]
	cmp r7, #12
	bne .L_020082f2
	movs r2, #4
	movs r1, #0
	negs r2, r2
	bl ObjectMotion_OffsetPositionAndResetMotion
	b .L_020082fc
.L_020082f2:
	movs r2, #4
	movs r1, #2
	negs r2, r2
	bl ObjectMotion_SnapHeadingAndOffset
.L_020082fc:
	movs r0, #4
	bl Battle_WaitMode0
	movs r3, #170
	lsls r3, r3, #1
	add r3, r9
	movs r4, #0
	ldrsh r0, [r3, r4]
	bl Func_02002d70
	add sp, #8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_0200831c:
	.4byte Data_02003904
.L_02008320:
	.4byte gPartyState
	.section .text.x02008324,"ax",%progbits
	.global Func_02000324
	.thumb_func
Func_02000324:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	sub sp, #8
	ldr r7, [r3, #108]
	bl Func_02002c68
	movs r0, #0
	bl Func_02002da8
	movs r6, #0
	movs r5, #8
.L_02008342:
	adds r0, r5, #0
	bl Object_GetById
	cmp r0, #0
	beq .L_02008354
	adds r2, r0, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
.L_02008354:
	adds r5, #1
	cmp r5, #63
	bls .L_02008342
	ldr r0, .L_02008430
	movs r5, #0
	movs r1, #0
.L_02008360:
	movs r2, #170
	lsls r2, r2, #1
	adds r3, r7, r2
	movs r4, #0
	ldrsh r2, [r3, r4]
	ldrh r3, [r0, r1]
	cmp r2, r3
	bne .L_02008372
	adds r6, r5, #0
.L_02008372:
	adds r5, #1
	adds r1, #12
	cmp r5, #3
	bls .L_02008360
	cmp r6, #1
	bhi .L_0200839c
	movs r0, #132
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200839c
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #228
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200839c
	adds r6, #4
.L_0200839c:
	movs r0, #158
	bl Func_02002de0
	ldr r2, .L_02008434
	movs r3, #133
	mov r10, r2
	lsls r3, r3, #2
	add r10, r3
	mov r4, r10
	ldr r0, [r4]
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	lsls r5, r6, #1
	ldr r2, .L_02008430
	adds r5, r5, r6
	lsls r5, r5, #2
	adds r6, r5, #0
	mov r8, r2
	adds r6, #8
	ldrh r2, [r2, r6]
	movs r1, #1
	add r6, r8
	movs r0, #2
	ldrh r3, [r6, #2]
	str r1, [sp, #0]
	str r0, [sp, #4]
	movs r1, #0
	movs r0, #0
	bl Func_02002c30
	ldrh r2, [r6, #2]
	mov r3, r8
	adds r5, #4
	ldr r0, [r3, r5]
	ldrh r1, [r6]
	adds r2, #60
	bl Func_02002c28
	mov r4, r10
	ldr r0, [r4]
	movs r1, #2
	bl Object_SetModeById
	mov r2, r10
	ldr r0, [r2]
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #8
	lsls r2, r2, #7
	bl ObjectMotion_SetSpeedParameters
	mov r3, r10
	movs r2, #4
	negs r2, r2
	ldr r0, [r3]
	movs r1, #2
	bl ObjectMotion_SnapHeadingAndOffset
	movs r0, #4
	bl Battle_WaitMode0
	movs r4, #170
	lsls r4, r4, #1
	adds r3, r7, r4
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Func_02002d70
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_02008430:
	.4byte Data_0200397c
.L_02008434:
	.4byte gPartyState
	.section .text.x02008438,"ax",%progbits
	.global Func_02000438
	.thumb_func
Func_02000438:
	push {r5, r6, r7, lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #130
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020084ea
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [r3, #108]
	ldr r3, .L_020084ec
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	ldr r0, [r6]
	bl Object_GetById
	adds r5, r0, #0
	bl Func_02002c68
	movs r0, #0
	bl Func_02002da8
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r2, r2
	negs r0, r0
	negs r1, r1
	bl Motion_CamBounds
	ldr r0, [r6]
	movs r1, #2
	bl Object_SetModeById
	movs r1, #128
	movs r2, #128
	ldr r0, [r6]
	lsls r2, r2, #7
	lsls r1, r1, #8
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r5, #8]
	asrs r2, r0, #16
	adds r3, r2, #0
	cmp r2, #0
	bge .L_0200849c
	adds r3, #15
.L_0200849c:
	asrs r3, r3, #4
	lsls r3, r3, #4
	subs r3, r2, r3
	movs r1, #8
	subs r1, r1, r3
	lsls r1, r1, #16
	ldr r2, [r5, #12]
	adds r1, r1, r0
	ldr r3, [r5, #16]
	adds r0, r5, #0
	bl Func_02002c18
	adds r0, r5, #0
	bl Func_02002c20
	movs r3, #192
	lsls r3, r3, #8
	strh r3, [r5, #6]
	ldr r1, .L_020084f0
	ldr r0, [r6]
	bl ObjectMotion_EnableActionAndSetCallback
	movs r0, #12
	bl WaitFrames
	movs r0, #123
	bl Func_02002de0
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r2, #170
	lsls r2, r2, #1
	adds r3, r7, r2
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl Func_02002d70
.L_020084ea:
	pop {r5, r6, r7, pc}
.L_020084ec:
	.4byte gPartyState
.L_020084f0:
	.4byte Data_02002de8
	.section .text.x020084f4,"ax",%progbits
	.global Func_020004f4
	.thumb_func
Func_020004f4:
	push {lr}
	ldr r2, .L_02008624
	movs r0, #240
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r0, #0
	ldrsh r1, [r3, r0]
	ldr r3, .L_02008628
	cmp r1, r3
	bne .L_020085a4
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	subs r3, #3
	cmp r3, #27
	bhi .L_020085a0
	ldr r2, .L_0200862c
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_02008520:
	.4byte .L_02008590
	.4byte .L_02008590
	.4byte .L_02008590
	.4byte .L_020085a0
	.4byte .L_020085a0
	.4byte .L_020085a0
	.4byte .L_020085a0
	.4byte .L_020085a0
	.4byte .L_020085a0
	.4byte .L_020085a0
	.4byte .L_020085a0
	.4byte .L_020085a0
	.4byte .L_02008598
	.4byte .L_02008598
	.4byte .L_02008598
	.4byte .L_02008598
	.4byte .L_020085a0
	.4byte .L_020085a0
	.4byte .L_020085a0
	.4byte .L_020085a0
	.4byte .L_020085a0
	.4byte .L_020085a0
	.4byte .L_02008594
	.4byte .L_02008594
	.4byte .L_02008594
	.4byte .L_02008594
	.4byte .L_0200859c
	.4byte .L_0200859c
.L_02008590:
	ldr r0, .L_02008630
	b .L_02008622
.L_02008594:
	ldr r0, .L_02008634
	b .L_02008622
.L_02008598:
	ldr r0, .L_02008638
	b .L_02008622
.L_0200859c:
	ldr r0, .L_0200863c
	b .L_02008622
.L_020085a0:
	ldr r0, .L_02008640
	b .L_02008622
.L_020085a4:
	ldr r3, .L_02008644
	cmp r1, r3
	bne .L_02008620
	movs r0, #241
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	subs r3, #1
	cmp r3, #19
	bhi .L_0200861c
	ldr r2, .L_02008648
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_020085c4:
	.4byte .L_02008614
	.4byte .L_02008614
	.4byte .L_02008614
	.4byte .L_02008614
	.4byte .L_0200861c
	.4byte .L_02008618
	.4byte .L_0200861c
	.4byte .L_0200861c
	.4byte .L_0200861c
	.4byte .L_02008618
	.4byte .L_02008614
	.4byte .L_02008614
	.4byte .L_02008614
	.4byte .L_02008614
	.4byte .L_02008614
	.4byte .L_02008614
	.4byte .L_02008614
	.4byte .L_02008614
	.4byte .L_0200861c
	.4byte .L_02008614
.L_02008614:
	ldr r0, .L_0200864c
	b .L_02008622
.L_02008618:
	ldr r0, .L_02008650
	b .L_02008622
.L_0200861c:
	ldr r0, .L_02008654
	b .L_02008622
.L_02008620:
	ldr r0, .L_02008658
.L_02008622:
	pop {pc}
.L_02008624:
	.4byte gPartyState
.L_02008628:
	.4byte 0x00000028
.L_0200862c:
	.4byte .L_02008520
.L_02008630:
	.4byte Data_02003a84
.L_02008634:
	.4byte Data_02003ae4
.L_02008638:
	.4byte Data_02003c10
.L_0200863c:
	.4byte Data_02003c7c
.L_02008640:
	.4byte Data_020039d0
.L_02008644:
	.4byte 0x00000029
.L_02008648:
	.4byte .L_020085c4
.L_0200864c:
	.4byte Data_02003d9c
.L_02008650:
	.4byte Data_02003dfc
.L_02008654:
	.4byte Data_02003ebc
.L_02008658:
	.4byte Data_020039c4
	.section .text.x0200865c,"ax",%progbits
	.global Func_0200065c
	.thumb_func
Func_0200065c:
	push {r5, lr}
	sub sp, #8
	bl Func_02002c68
	movs r0, #0
	bl Func_02002da8
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #131
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008758
	movs r0, #132
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020086ee
	movs r0, #232
	bl Func_02002de0
	movs r0, #15
	bl Object_GetById
	movs r1, #206
	movs r3, #144
	lsls r1, r1, #18
	ldr r2, .L_02008768
	lsls r3, r3, #15
	bl Object_SetPositionAndResetMotion
	movs r0, #15
	bl Object_GetById
	ldr r3, .L_02008768
	movs r1, #68
	str r3, [r0, #20]
	movs r3, #3
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #72
	movs r2, #50
	movs r3, #64
	bl Func_02002c30
	movs r3, #110
	str r3, [sp, #0]
	movs r5, #4
	movs r0, #106
	movs r1, #16
	movs r2, #3
	movs r3, #2
	str r5, [sp, #4]
	bl Func_02002c38
	movs r3, #50
	str r3, [sp, #0]
	movs r0, #50
	movs r1, #1
	movs r2, #3
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02002c38
	movs r0, #132
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
	b .L_02008760
.L_020086ee:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008704
	movs r0, #117
	bl Func_02002de0
	b .L_02008760
.L_02008704:
	movs r0, #232
	bl Func_02002de0
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_02002cd0
	movs r3, #3
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #68
	movs r1, #68
	movs r2, #50
	movs r3, #64
	bl Func_02002c30
	movs r3, #110
	str r3, [sp, #0]
	movs r5, #4
	movs r0, #102
	movs r1, #16
	movs r2, #3
	movs r3, #2
	str r5, [sp, #4]
	bl Func_02002c38
	movs r3, #50
	str r3, [sp, #0]
	movs r0, #50
	movs r1, #0
	movs r2, #3
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02002c38
	movs r0, #132
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	b .L_02008760
.L_02008758:
	ldr r0, .L_0200876c
	movs r1, #1
	bl UiText_ShowPositionedMessageAndWait
.L_02008760:
	bl Func_02002c70
	add sp, #8
	pop {r5, pc}
.L_02008768:
	.4byte 0xffe00000
.L_0200876c:
	.4byte 0x00002185
	.section .text.x02008770,"ax",%progbits
	.global Func_02000770
	.thumb_func
Func_02000770:
	push {lr}
	ldr r0, .L_0200877c
	bl Func_02002dd0
	pop {pc}
	.2byte 0x0000
.L_0200877c:
	.4byte Data_02003010
	.section .text.x02008780,"ax",%progbits
	.global Func_02000780
	.thumb_func
Func_02000780:
	push {lr}
	bl Func_02002dd8
	pop {pc}
	.section .text.x02008788,"ax",%progbits
	.global Func_02000788
	.thumb_func
Func_02000788:
	push {lr}
	ldr r0, .L_02008794
	bl Func_02002dd0
	pop {pc}
	.2byte 0x0000
.L_02008794:
	.4byte Data_02003010
	.section .text.x02008798,"ax",%progbits
	.global Func_02000798
	.thumb_func
Func_02000798:
	push {r5, r6, r7, lr}
	sub sp, #8
	bl Func_02002dd8
	movs r0, #8
	bl Object_GetById
	adds r6, r0, #0
	ldr r3, [r6, #8]
	asrs r5, r3, #20
	cmp r5, #16
	bne .L_02008814
	bl Func_02002c68
	movs r0, #0
	bl Func_02002da8
	movs r3, #46
	str r3, [sp, #4]
	movs r0, #16
	movs r3, #1
	movs r1, #47
	movs r2, #1
	str r5, [sp, #0]
	adds r7, r6, #0
	bl Func_02002c38
	movs r0, #1
	bl WaitFrames
	adds r7, #85
	movs r3, #3
	strb r3, [r7]
.L_020087da:
	movs r0, #1
	bl WaitFrames
	ldr r5, [r6, #40]
	cmp r5, #0
	bne .L_020087da
	movs r0, #188
	bl Func_02002de0
	movs r0, #10
	bl WaitFrames
	movs r0, #128
	lsls r0, r0, #2
	strb r5, [r7]
	bl GameFlag_SetBit
	movs r3, #16
	movs r2, #46
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #14
	movs r1, #46
	movs r2, #1
	movs r3, #1
	bl Func_02002c38
	bl Func_02002c70
.L_02008814:
	add sp, #8
	pop {r5, r6, r7, pc}
	.section .text.x02008818,"ax",%progbits
	.global Func_02000818
	.thumb_func
Func_02000818:
	push {lr}
	ldr r0, .L_02008824
	bl Func_02002dc0
	pop {pc}
	.2byte 0x0000
.L_02008824:
	.4byte Data_02003014
	.section .text.x02008828,"ax",%progbits
	.global Func_02000828
	.thumb_func
Func_02000828:
	push {lr}
	ldr r0, .L_02008834
	bl Func_02002dc0
	pop {pc}
	.2byte 0x0000
.L_02008834:
	.4byte Data_0200301e
	.section .text.x02008838,"ax",%progbits
	.global Func_02000838
	.thumb_func
Func_02000838:
	push {lr}
	sub sp, #8
	movs r3, #110
	movs r2, #4
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #1
	movs r2, #3
	movs r3, #1
	movs r0, #110
	bl Func_02002c38
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	bl GameFlag_SetBit
	add sp, #8
	pop {pc}
	.2byte 0x0000
	.section .text.x02008860,"ax",%progbits
	.global Func_02000860
	.thumb_func
Func_02000860:
	push {lr}
	ldr r0, .L_0200886c
	bl Func_02002dc0
	pop {pc}
	.2byte 0x0000
.L_0200886c:
	.4byte Data_02003034
	.section .text.x02008870,"ax",%progbits
	.global Func_02000870
	.thumb_func
Func_02000870:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #131
	sub sp, #8
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200888c
	b .L_02008ad8
.L_0200888c:
	ldr r3, .L_02008ae4
	movs r2, #133
	lsls r2, r2, #2
	adds r7, r3, r2
	ldr r0, [r7]
	bl Object_GetById
	adds r5, r0, #0
	bl Func_02002c68
	movs r0, #0
	bl Func_02002da8
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r7]
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #210
	ldr r0, [r7]
	lsls r1, r1, #2
	movs r2, #72
	bl ObjectMotion_SetPositionAndReset
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #50
	bl GameFlag_Test
	adds r5, #34
	mov r9, r0
	mov r10, r5
	cmp r0, #0
	beq .L_02008972
	movs r0, #243
	bl Func_02002de0
	movs r1, #129
	ldr r0, [r7]
	lsls r1, r1, #1
	bl Func_02002d48
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	movs r0, #0
	bl Func_02002c48
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_02002c48
	movs r5, #3
	movs r1, #68
	movs r0, #68
	movs r2, #50
	movs r3, #64
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02002c30
	movs r3, #2
	mov r2, r10
	strb r3, [r2]
	movs r0, #20
	bl Battle_WaitMode0
	ldr r0, [r7]
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r2, #204
	lsls r2, r2, #8
	ldr r0, [r7]
	ldr r1, .L_02008ae8
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #210
	lsls r1, r1, #2
	movs r2, #88
	ldr r0, [r7]
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #232
	bl Func_02002de0
	movs r2, #50
	movs r3, #64
	movs r0, #72
	movs r1, #68
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02002c30
	movs r0, #20
	bl Battle_WaitMode0
	movs r3, #1
	mov r2, r10
	strb r3, [r2]
	b .L_02008ad4
.L_02008972:
	movs r0, #9
	bl Object_GetById
	movs r1, #2
	adds r6, r0, #0
	movs r0, #9
	bl ObjectMotion_SetActionVariant
	ldr r1, .L_02008aec
	ldr r2, .L_02008af0
	movs r0, #9
	bl ObjectMotion_SetSpeedParameters
	movs r0, #9
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #254
	ands r3, r2
	movs r1, #206
	strb r3, [r0]
	lsls r1, r1, #2
	movs r2, #88
	movs r0, #9
	bl ObjectMotion_SetPositionAndReset
	movs r0, #1
	bl Battle_WaitMode0
	movs r0, #9
	bl Object_GetById
	adds r0, #90
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r0, #243
	bl Func_02002de0
	movs r1, #129
	ldr r0, [r7]
	lsls r1, r1, #1
	bl Func_02002d48
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	movs r0, #0
	bl Func_02002c48
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_02002c48
	movs r5, #3
	movs r1, #68
	movs r0, #68
	movs r2, #50
	movs r3, #64
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02002c30
	movs r3, #2
	mov r8, r3
	mov r2, r8
	mov r3, r10
	strb r2, [r3]
	movs r0, #20
	bl Battle_WaitMode0
	movs r1, #192
	ldr r0, [r7]
	lsls r1, r1, #7
	movs r2, #20
	bl ObjectMotion_ArmCallback
	ldr r0, [r7]
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r2, #204
	lsls r2, r2, #8
	ldr r0, [r7]
	ldr r1, .L_02008ae8
	adds r2, #204
	bl ObjectMotion_SetSpeedParameters
	movs r1, #210
	movs r2, #88
	ldr r0, [r7]
	lsls r1, r1, #2
	bl ObjectMotion_SetPositionAndCommit
	movs r1, #128
	ldr r0, [r7]
	lsls r1, r1, #8
	bl Func_02002d30
	ldr r0, [r7]
	movs r1, #9
	bl Object_LinkObjectAndSetCallback
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r6, #72]
	mov r2, r8
	adds r6, #34
	strb r2, [r6]
	movs r0, #9
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #208
	movs r0, #9
	lsls r1, r1, #2
	movs r2, #74
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #9
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #214
	movs r2, #88
	movs r0, #9
	lsls r1, r1, #2
	bl ObjectMotion_SetPositionAndCommit
	movs r1, #129
	lsls r1, r1, #1
	ldr r0, [r7]
	bl Func_02002d48
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #232
	bl Func_02002de0
	movs r1, #68
	movs r2, #50
	movs r3, #64
	movs r0, #72
	str r5, [sp, #0]
	str r5, [sp, #4]
	bl Func_02002c30
	movs r0, #20
	bl Battle_WaitMode0
	ldr r0, [r7]
	bl ObjectMotion_EnableActionAndResetMotion
	movs r3, #1
	mov r2, r10
	strb r3, [r2]
	mov r3, r9
	movs r0, #1
	strb r3, [r6]
	bl WaitFrames
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
.L_02008ad4:
	bl Func_02002c70
.L_02008ad8:
	add sp, #8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_02008ae4:
	.4byte gPartyState
.L_02008ae8:
	.4byte 0x00019999
.L_02008aec:
	.4byte 0x00026666
.L_02008af0:
	.4byte 0x00013333
	.section .text.x02008af4,"ax",%progbits
	.global Func_02000af4
	.thumb_func
Func_02000af4:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #131
	sub sp, #8
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008bda
	ldr r5, .L_02008be4
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	bl Object_GetById
	mov r8, r0
	bl Func_02002c68
	movs r0, #0
	bl Func_02002da8
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r5]
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #203
	movs r2, #72
	lsls r1, r1, #2
	ldr r0, [r5]
	bl ObjectMotion_SetPositionAndReset
	movs r0, #243
	bl Func_02002de0
	movs r1, #129
	ldr r0, [r5]
	lsls r1, r1, #1
	bl Func_02002d48
	movs r1, #128
	movs r2, #128
	lsls r1, r1, #10
	lsls r2, r2, #9
	movs r0, #0
	bl Func_02002c48
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #0
	movs r1, #0
	movs r2, #0
	bl Func_02002c48
	movs r6, #3
	movs r1, #68
	movs r0, #68
	movs r2, #50
	movs r3, #64
	str r6, [sp, #0]
	str r6, [sp, #4]
	bl Func_02002c30
	movs r3, #34
	add r8, r3
	mov r2, r8
	movs r3, #2
	strb r3, [r2]
	movs r0, #20
	bl Battle_WaitMode0
	ldr r0, [r5]
	movs r1, #4
	movs r2, #0
	bl ObjectMotion_Launch
	ldr r0, [r5]
	ldr r1, .L_02008be8
	ldr r2, .L_02008bec
	bl ObjectMotion_SetSpeedParameters
	movs r1, #203
	lsls r1, r1, #2
	movs r2, #88
	ldr r0, [r5]
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #232
	bl Func_02002de0
	movs r2, #50
	movs r3, #64
	movs r0, #72
	movs r1, #68
	str r6, [sp, #0]
	str r6, [sp, #4]
	bl Func_02002c30
	movs r0, #20
	bl Battle_WaitMode0
	movs r3, #1
	mov r2, r8
	strb r3, [r2]
.L_02008bda:
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008be4:
	.4byte gPartyState
.L_02008be8:
	.4byte 0x00033333
.L_02008bec:
	.4byte 0x00019999
	.section .text.x02008bf0,"ax",%progbits
	.global Func_02000bf0
	.thumb_func
Func_02000bf0:
	push {r5, r6, lr}
	adds r6, r0, #0
	bl Func_02002c68
	movs r0, #0
	bl Func_02002da8
	ldr r5, .L_02008c4c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r1, [r3]
	movs r2, #0
	adds r0, r6, #0
	bl ObjectMotion_SetAngleToward
	adds r0, r6, #0
	movs r1, #2
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	adds r0, r6, #0
	lsls r1, r1, #1
	bl Func_02002d48
	movs r0, #20
	bl Battle_WaitMode0
	movs r3, #166
	lsls r3, r3, #1
	adds r3, #255
	adds r5, r5, r3
	movs r3, #2
	strb r3, [r5]
	movs r0, #11
	movs r1, #0
	bl Func_02002d78
	movs r2, #190
	lsls r2, r2, #2
	adds r6, r6, r2
	adds r0, r6, #0
	bl GameFlag_SetBit
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008c4c:
	.4byte gPartyState
	.section .text.x02008c50,"ax",%progbits
	.global Func_02000c50
	.thumb_func
Func_02000c50:
	push {r5, r6, lr}
	adds r6, r0, #0
	bl Func_02002c68
	movs r0, #0
	bl Func_02002da8
	ldr r5, .L_02008cac
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r1, [r3]
	movs r2, #0
	adds r0, r6, #0
	bl ObjectMotion_SetAngleToward
	adds r0, r6, #0
	movs r1, #2
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	adds r0, r6, #0
	lsls r1, r1, #1
	bl Func_02002d48
	movs r0, #20
	bl Battle_WaitMode0
	movs r3, #166
	lsls r3, r3, #1
	adds r3, #255
	adds r5, r5, r3
	movs r3, #2
	strb r3, [r5]
	movs r0, #11
	movs r1, #0
	bl Func_02002d78
	movs r2, #253
	lsls r2, r2, #1
	adds r2, #255
	adds r6, r6, r2
	adds r0, r6, #0
	bl GameFlag_SetBit
	pop {r5, r6, pc}
.L_02008cac:
	.4byte gPartyState
	.section .text.x02008cb0,"ax",%progbits
	.global Func_02000cb0
	.thumb_func
Func_02000cb0:
	push {r5, r6, lr}
	adds r6, r0, #0
	bl Func_02002c68
	movs r0, #0
	bl Func_02002da8
	ldr r5, .L_02008d0c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r1, [r3]
	movs r2, #0
	adds r0, r6, #0
	bl ObjectMotion_SetAngleToward
	adds r0, r6, #0
	movs r1, #2
	bl ObjectMotion_SetVariantCallback
	movs r1, #129
	adds r0, r6, #0
	lsls r1, r1, #1
	bl Func_02002d48
	movs r0, #20
	bl Battle_WaitMode0
	movs r3, #166
	lsls r3, r3, #1
	adds r3, #255
	adds r5, r5, r3
	movs r3, #2
	strb r3, [r5]
	movs r0, #11
	movs r1, #0
	bl Func_02002d78
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #255
	adds r6, r6, r2
	adds r0, r6, #0
	bl GameFlag_SetBit
	pop {r5, r6, pc}
.L_02008d0c:
	.4byte gPartyState
	.section .text.x02008d10,"ax",%progbits
	.global Func_02000d10
	.thumb_func
Func_02000d10:
	push {r5, r6, lr}
	adds r5, r0, #0
	bl Object_GetById
	ldr r1, .L_02008d3c
	adds r6, r0, #0
	ldr r2, .L_02008d40
	adds r0, r5, #0
	bl ObjectMotion_SetSpeedParameters
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r6, #72]
	movs r3, #128
	lsls r3, r3, #12
	str r3, [r6, #40]
	adds r0, r5, #0
	movs r1, #120
	movs r2, #120
	bl ObjectMotion_ResetAndSetPosition
	pop {r5, r6, pc}
.L_02008d3c:
	.4byte 0x00033333
.L_02008d40:
	.4byte 0x00019999
	.section .text.x02008d44,"ax",%progbits
	.global Func_02000d44
	.thumb_func
Func_02000d44:
	push {lr}
	bl Func_02002c68
	movs r0, #0
	bl Func_02002da8
	movs r0, #0
	bl Func_02002de0
	movs r0, #9
	movs r1, #2
	bl ObjectMotion_SetVariantCallback
	movs r0, #10
	movs r1, #2
	bl ObjectMotion_SetVariantCallback
	movs r0, #11
	movs r1, #2
	bl ObjectMotion_SetVariantCallback
	movs r0, #12
	movs r1, #2
	bl ObjectMotion_SetVariantCallback
	movs r1, #2
	movs r0, #13
	bl Motion_SetVarCbAndRefresh
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #147
	bl Func_02002de0
	movs r0, #9
	bl Func_02000d10
	movs r0, #10
	bl Func_02000d10
	movs r0, #11
	bl Func_02000d10
	movs r0, #12
	bl Func_02000d10
	movs r0, #13
	bl Func_02000d10
	movs r0, #13
	bl ObjectMotion_CommitCurrentPositionAndActivate
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02002cd0
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_02002cd0
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02002cd0
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02002cd0
	movs r2, #0
	movs r1, #0
	movs r0, #13
	bl Func_02002cd0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #52
	bl Func_02002de0
	ldr r1, .L_02008e20
	movs r0, #8
	bl Object_SetActionCallbackAndRefreshById
	ldr r3, .L_02008e24
	movs r2, #166
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r3, r2
	movs r2, #2
	strb r2, [r3]
	ldr r0, .L_02008e28
	movs r1, #15
	bl Party_SetFields1eeAnd1f0
	movs r0, #11
	movs r1, #1
	bl Func_02002d78
	movs r0, #132
	lsls r0, r0, #2
	adds r0, #255
	bl GameFlag_SetBit
	pop {pc}
	.2byte 0x0000
.L_02008e20:
	.4byte Data_02002e9c
.L_02008e24:
	.4byte gPartyState
.L_02008e28:
	.4byte 0x00000029
	.section .text.x02008e2c,"ax",%progbits
	.global Func_02000e2c
	.thumb_func
Func_02000e2c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	sub sp, #8
	bl GameFlag_Test
	mov r8, r0
	cmp r0, #0
	bne .L_02008f44
	movs r0, #129
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008f44
	movs r0, #8
	bl Object_GetById
	adds r6, r0, #0
	ldr r3, [r6, #8]
	asrs r7, r3, #20
	cmp r7, #35
	bne .L_02008f44
	ldr r3, [r6, #16]
	asrs r5, r3, #20
	cmp r5, #9
	bne .L_02008f44
	ldr r3, .L_020090b4
	movs r1, #133
	lsls r1, r1, #2
	adds r1, r1, r3
	ldr r0, [r1]
	mov r9, r1
	bl Object_GetById
	mov r10, r0
	bl Func_02002c68
	movs r0, #0
	bl Func_02002da8
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #183
	bl Func_02002de0
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #35
	movs r1, #68
	movs r2, #35
	movs r3, #69
	bl Func_02002c30
	movs r0, #35
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r7, [sp, #0]
	str r5, [sp, #4]
	bl Func_02002c38
	adds r3, r6, #0
	adds r3, #85
	mov r2, r8
	strb r2, [r3]
	movs r1, #3
	movs r0, #8
	bl ObjectMotion_SetActionVariant
	mov r1, r10
	ldr r2, [r6, #16]
	ldr r3, [r1, #16]
	cmp r2, r3
	ble .L_02008eda
	mov r2, r9
	ldr r0, [r2]
	movs r1, #3
	bl ObjectMotion_SetActionVariant
.L_02008eda:
	movs r5, #0
.L_02008edc:
	ldr r3, [r6, #12]
	ldr r1, .L_020090b8
	movs r0, #1
	adds r3, r3, r1
	str r3, [r6, #12]
	adds r5, #1
	bl WaitFrames
	cmp r5, #127
	bls .L_02008edc
	movs r0, #8
	movs r1, #3
	bl Object_SetModeById
	movs r1, #142
	movs r2, #152
	lsls r1, r1, #18
	lsls r2, r2, #16
	movs r0, #10
	bl Func_02002cd0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_SetBit
	mov r1, r10
	ldr r2, [r6, #16]
	ldr r3, [r1, #16]
	cmp r2, r3
	ble .L_02008f40
	ldr r5, .L_020090b4
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
.L_02008f40:
	bl Func_02002c70
.L_02008f44:
	movs r0, #130
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	mov r8, r0
	cmp r0, #0
	bne .L_02009052
	movs r0, #131
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009052
	movs r0, #9
	bl Object_GetById
	adds r6, r0, #0
	ldr r3, [r6, #8]
	asrs r7, r3, #20
	cmp r7, #29
	bne .L_02009052
	ldr r3, [r6, #16]
	asrs r5, r3, #20
	cmp r5, #9
	bne .L_02009052
	ldr r3, .L_020090b4
	movs r1, #133
	lsls r1, r1, #2
	adds r1, r1, r3
	ldr r0, [r1]
	mov r9, r1
	bl Object_GetById
	mov r10, r0
	bl Func_02002c68
	movs r0, #0
	bl Func_02002da8
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #183
	bl Func_02002de0
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #29
	movs r1, #68
	movs r2, #29
	movs r3, #69
	bl Func_02002c30
	movs r0, #35
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r7, [sp, #0]
	str r5, [sp, #4]
	bl Func_02002c38
	adds r3, r6, #0
	adds r3, #85
	mov r2, r8
	strb r2, [r3]
	movs r1, #3
	movs r0, #9
	bl ObjectMotion_SetActionVariant
	mov r1, r10
	ldr r2, [r6, #16]
	ldr r3, [r1, #16]
	cmp r2, r3
	ble .L_02008fe8
	mov r2, r9
	ldr r0, [r2]
	movs r1, #3
	bl ObjectMotion_SetActionVariant
.L_02008fe8:
	movs r5, #0
.L_02008fea:
	ldr r3, [r6, #12]
	ldr r1, .L_020090b8
	movs r0, #1
	adds r3, r3, r1
	str r3, [r6, #12]
	adds r5, #1
	bl WaitFrames
	cmp r5, #127
	bls .L_02008fea
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	movs r1, #236
	movs r2, #152
	lsls r1, r1, #17
	lsls r2, r2, #16
	movs r0, #11
	bl Func_02002cd0
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #130
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	mov r1, r10
	ldr r2, [r6, #16]
	ldr r3, [r1, #16]
	cmp r2, r3
	ble .L_0200904e
	ldr r5, .L_020090b4
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	ldr r0, [r5]
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
.L_0200904e:
	bl Func_02002c70
.L_02009052:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020090a6
	movs r0, #130
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020090a6
	bl Func_02002c68
	movs r0, #0
	bl Func_02002da8
	movs r0, #188
	bl Func_02002de0
	movs r3, #3
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #124
	movs r2, #31
	movs r3, #64
	movs r0, #4
	bl Func_02002c30
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #130
	bl GameFlag_SetBit
	bl Func_02002c70
.L_020090a6:
	add sp, #8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020090b4:
	.4byte gPartyState
.L_020090b8:
	.4byte 0xffffe000
	.section .text.x020090bc,"ax",%progbits
	.global Func_020010bc
	.thumb_func
Func_020010bc:
	push {lr}
	ldr r0, .L_020090c8
	bl Func_02002dd0
	pop {pc}
	.2byte 0x0000
.L_020090c8:
	.4byte Data_0200304e
	.section .text.x020090cc,"ax",%progbits
	.global Func_020010cc
	.thumb_func
Func_020010cc:
	push {lr}
	bl Func_02002dd8
	bl Func_02000e2c
	pop {pc}
	.section .text.x020090d8,"ax",%progbits
	.global Func_020010d8
	.thumb_func
Func_020010d8:
	push {lr}
	movs r0, #106
	bl Func_02002de0
	movs r1, #2
	movs r0, #8
	bl Object_SetModeById
	movs r0, #129
	lsls r0, r0, #2
	bl GameFlag_SetBit
	bl Func_02000e2c
	pop {pc}
	.2byte 0x0000
	.section .text.x020090f8,"ax",%progbits
	.global Func_020010f8
	.thumb_func
Func_020010f8:
	push {lr}
	movs r0, #106
	bl Func_02002de0
	movs r1, #2
	movs r0, #9
	bl Object_SetModeById
	movs r0, #131
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	bl Func_02000e2c
	pop {pc}
	.section .text.x02009118,"ax",%progbits
	.global Func_02001118
	.thumb_func
Func_02001118:
	push {lr}
	movs r0, #10
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	pop {pc}
	.section .text.x02009128,"ax",%progbits
	.global Func_02001128
	.thumb_func
Func_02001128:
	push {lr}
	movs r0, #11
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	pop {pc}
	.section .text.x02009138,"ax",%progbits
	.global Func_02001138
	.thumb_func
Func_02001138:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	movs r0, #8
	sub sp, #8
	ldr r5, [r3, #108]
	bl Object_GetById
	adds r6, r0, #0
	bl Func_02002c68
	movs r0, #0
	bl Func_02002da8
	movs r3, #181
	lsls r3, r3, #1
	movs r0, #128
	adds r2, r5, r3
	lsls r0, r0, #2
	movs r3, #0
	strh r3, [r2]
	adds r0, #2
	bl GameFlag_ClearBit
	movs r0, #129
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #186
	bl Func_02002de0
	movs r5, #0
.L_0200917e:
	ldr r3, [r6, #12]
	movs r2, #128
	lsls r2, r2, #7
	adds r3, r3, r2
	str r3, [r6, #12]
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #63
	bls .L_0200917e
	movs r0, #8
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #4
	movs r1, #123
	movs r2, #35
	movs r3, #69
	bl Func_02002c30
	movs r3, #35
	movs r2, #9
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #0
	movs r2, #1
	movs r3, #1
	movs r0, #36
	bl Func_02002c38
	movs r0, #188
	bl Func_02002de0
	movs r3, #3
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #124
	movs r2, #31
	movs r3, #64
	movs r0, #7
	bl Func_02002c30
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #130
	bl GameFlag_ClearBit
	movs r0, #20
	bl Battle_WaitMode0
	bl Func_02002c70
	add sp, #8
	pop {r5, r6, pc}
	.section .text.x020091f4,"ax",%progbits
	.global Func_020011f4
	.thumb_func
Func_020011f4:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	movs r0, #9
	sub sp, #8
	ldr r5, [r3, #108]
	bl Object_GetById
	adds r6, r0, #0
	bl Func_02002c68
	movs r0, #0
	bl Func_02002da8
	movs r3, #181
	lsls r3, r3, #1
	movs r0, #130
	adds r2, r5, r3
	lsls r0, r0, #1
	movs r3, #0
	strh r3, [r2]
	adds r0, #255
	bl GameFlag_ClearBit
	movs r0, #131
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #186
	bl Func_02002de0
	movs r5, #0
.L_0200923c:
	ldr r3, [r6, #12]
	movs r2, #128
	lsls r2, r2, #7
	adds r3, r3, r2
	str r3, [r6, #12]
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #63
	bls .L_0200923c
	movs r0, #9
	movs r1, #2
	bl ObjectMotion_SetActionVariant
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #4
	movs r1, #123
	movs r2, #29
	movs r3, #69
	bl Func_02002c30
	movs r3, #29
	movs r2, #9
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #0
	movs r2, #1
	movs r3, #1
	movs r0, #36
	bl Func_02002c38
	movs r0, #188
	bl Func_02002de0
	movs r3, #3
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #124
	movs r2, #31
	movs r3, #64
	movs r0, #7
	bl Func_02002c30
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #130
	bl GameFlag_ClearBit
	movs r0, #20
	bl Battle_WaitMode0
	bl Func_02002c70
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x020092b4,"ax",%progbits
	.global Func_020012b4
	.thumb_func
Func_020012b4:
	push {r5, r6, lr}
	ldr r5, .L_02009360
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	sub sp, #8
	bl Object_GetById
	adds r6, r0, #0
	bl Func_02002c68
	movs r0, #0
	bl Func_02002da8
	movs r0, #128
	movs r1, #128
	movs r2, #248
	movs r3, #128
	lsls r3, r3, #17
	lsls r0, r0, #12
	lsls r1, r1, #13
	lsls r2, r2, #16
	bl Func_02002d68
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	ldr r6, .L_02009364
	ldr r0, [r5]
	adds r2, r2, r6
	bl Func_02002cd0
	movs r0, #64
	bl Object_GetById
	ldr r2, [r0, #16]
	ldr r1, [r0, #8]
	adds r2, r2, r6
	movs r0, #64
	bl Func_02002cd0
	movs r0, #64
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	movs r0, #10
	bl WaitFrames
	ldr r0, [r5]
	movs r1, #0
	bl Object_AttachWorkTargetToObject
	movs r0, #1
	bl WaitFrames
	movs r3, #6
	movs r2, #4
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r3, #2
	movs r2, #1
	movs r1, #0
	movs r0, #6
	bl Func_02002c38
	bl Func_02002c10
	movs r0, #1
	bl WaitFrames
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #228
	bl GameFlag_SetBit
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_02002d88
	bl Func_02002c70
	add sp, #8
	pop {r5, r6, pc}
.L_02009360:
	.4byte gPartyState
.L_02009364:
	.4byte 0xfed00000
	.section .text.x02009368,"ax",%progbits
	.global Func_02001368
	.thumb_func
Func_02001368:
	push {lr}
	bl Func_02002c68
	movs r0, #0
	bl Func_02002da8
	movs r0, #0
	bl Func_02002da8
	bl Func_02002c70
	pop {pc}
	.section .text.x02009380,"ax",%progbits
	.global Func_02001380
	.thumb_func
Func_02001380:
	push {r5, r6, lr}
	adds r5, r0, #0
	bl Object_GetById
	adds r6, r0, #0
	adds r0, r5, #0
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	movs r1, #1
	adds r0, r5, #0
	bl ObjectMotion_SetActionVariant
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #253
	ands r3, r2
	strb r3, [r1]
	adds r6, #98
	movs r3, #1
	strb r3, [r6]
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x020093b8,"ax",%progbits
	.global Func_020013b8
	.thumb_func
Func_020013b8:
	push {r5, r6, lr}
	adds r5, r0, #0
	bl Object_GetById
	adds r6, r0, #0
	adds r0, r5, #0
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	movs r1, #2
	adds r0, r5, #0
	bl ObjectMotion_SetActionVariant
	adds r1, r6, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #253
	ands r3, r2
	strb r3, [r1]
	adds r6, #98
	movs r3, #1
	strb r3, [r6]
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x020093f0,"ax",%progbits
	.global Func_020013f0
	.thumb_func
Func_020013f0:
	push {lr}
	movs r0, #15
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	movs r1, #3
	strb r3, [r0]
	movs r0, #15
	bl ObjectMotion_SetActionVariant
	movs r0, #15
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #253
	ands r3, r2
	strb r3, [r0]
	movs r0, #16
	bl Func_02001380
	movs r0, #17
	bl Func_02001380
	movs r0, #18
	bl Func_02001380
	movs r0, #19
	bl Func_02001380
	movs r0, #20
	bl Func_02001380
	pop {pc}
	.2byte 0x0000
	.section .text.x0200943c,"ax",%progbits
	.global Func_0200143c
	.thumb_func
Func_0200143c:
	push {lr}
	movs r0, #16
	bl Func_02001380
	movs r0, #17
	bl Func_02001380
	movs r0, #18
	bl Func_02001380
	movs r0, #19
	bl Func_02001380
	movs r0, #20
	bl Func_02001380
	movs r0, #22
	bl Func_02001380
	pop {pc}
	.section .text.x02009464,"ax",%progbits
	.global Func_02001464
	.thumb_func
Func_02001464:
	push {lr}
	movs r0, #10
	bl Func_020013b8
	movs r0, #11
	bl Func_020013b8
	pop {pc}
	.section .text.x02009474,"ax",%progbits
	.global Func_02001474
	.thumb_func
Func_02001474:
	push {r5, r6, lr}
	ldr r3, .L_02009558
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #8
	bl Object_GetById
	ldr r3, [r0, #8]
	movs r2, #6
	asrs r5, r3, #20
	ldr r3, [r0, #16]
	movs r1, #105
	asrs r6, r3, #20
	movs r3, #8
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #74
	movs r2, #4
	movs r3, #85
	bl Func_02002c30
	cmp r6, #25
	bne .L_020094c0
	cmp r5, #4
	bne .L_02009552
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #18
	movs r1, #126
	movs r2, #4
	movs r3, #85
	bl Func_02002c30
	b .L_02009552
.L_020094c0:
	cmp r6, #26
	bne .L_020094dc
	cmp r5, #7
	bne .L_02009552
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #18
	movs r1, #126
	movs r2, #7
	movs r3, #86
	bl Func_02002c30
	b .L_02009552
.L_020094dc:
	cmp r6, #27
	bne .L_02009536
	cmp r5, #4
	bne .L_020094f4
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #18
	movs r1, #126
	movs r2, #4
	b .L_0200952e
.L_020094f4:
	cmp r5, #6
	bne .L_02009508
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #18
	movs r1, #126
	movs r2, #6
	b .L_0200952e
.L_02009508:
	cmp r5, #7
	bne .L_0200951c
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #18
	movs r1, #126
	movs r2, #7
	b .L_0200952e
.L_0200951c:
	cmp r5, #9
	bne .L_02009552
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #18
	movs r1, #126
	movs r2, #9
.L_0200952e:
	movs r3, #87
	bl Func_02002c30
	b .L_02009552
.L_02009536:
	cmp r6, #29
	bne .L_02009552
	cmp r5, #6
	bne .L_02009552
	movs r3, #1
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #18
	movs r1, #126
	movs r2, #6
	movs r3, #89
	bl Func_02002c30
.L_02009552:
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02009558:
	.4byte gPartyState
	.section .text.x0200955c,"ax",%progbits
	.global Func_0200155c
	.thumb_func
Func_0200155c:
	push {r5, lr}
	movs r0, #128
	movs r3, #192
	lsls r0, r0, #2
	lsls r3, r3, #18
	adds r0, #2
	ldr r5, [r3, #108]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009594
	movs r0, #10
	bl Object_GetById
	ldr r3, [r0, #8]
	ldr r2, [r0, #16]
	orrs r3, r2
	cmp r3, #0
	bne .L_02009594
	movs r3, #181
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r3, #200
	strh r3, [r2]
	movs r0, #8
	movs r1, #1
	bl Object_SetModeById
.L_02009594:
	movs r0, #130
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020095c4
	movs r0, #11
	bl Object_GetById
	ldr r3, [r0, #8]
	ldr r2, [r0, #16]
	orrs r3, r2
	cmp r3, #0
	bne .L_020095c4
	movs r3, #181
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r3, #201
	strh r3, [r2]
	movs r0, #9
	movs r1, #1
	bl Object_SetModeById
.L_020095c4:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x020095c8,"ax",%progbits
	.global Func_020015c8
	.thumb_func
Func_020015c8:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	sub sp, #12
	movs r3, #202
	movs r7, #128
	movs r6, #140
	mov r5, sp
	lsls r3, r3, #18
	lsls r7, r7, #12
	lsls r6, r6, #16
	str r3, [r5]
	str r7, [r5, #4]
	str r6, [r5, #8]
	mov r8, r3
	bl Random16Far
	adds r1, r0, #0
	movs r0, #128
	adds r2, r5, #0
	lsls r0, r0, #14
	bl Vector_AddPolarOffsetFar
	movs r0, #128
	lsls r0, r0, #2
	ldr r1, [r5]
	ldr r2, [r5, #4]
	ldr r3, [r5, #8]
	adds r0, #162
	bl Func_02002c00
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02009642
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #48]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r5, #52]
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	adds r0, r5, #0
	movs r1, #0
	bl Func_02002bf0
	adds r0, r5, #0
	mov r1, r8
	adds r2, r7, #0
	adds r3, r6, #0
	bl Func_02002c18
	ldr r1, .L_0200964c
	adds r0, r5, #0
	bl Func_02002bf8
.L_02009642:
	add sp, #12
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200964c:
	.4byte Data_02003ef8
	.section .text.x02009650,"ax",%progbits
	.global Func_02001650
	.thumb_func
Func_02001650:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	bl Func_02002c68
	movs r0, #0
	bl Func_02002da8
	ldr r0, .L_02009928
	bl Func_02002d18
	movs r0, #78
	bl Func_02002de0
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	ldr r2, .L_0200992c
	movs r6, #133
	mov r8, r2
	lsls r6, r6, #2
	movs r1, #204
	movs r2, #204
	add r6, r8
	lsls r1, r1, #8
	lsls r2, r2, #7
	ldr r0, [r6]
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #202
	ldr r0, [r6]
	movs r2, #208
	lsls r1, r1, #2
	bl ObjectMotion_SetPositionAndReset
	movs r0, #204
	movs r1, #200
	lsls r0, r0, #8
	lsls r1, r1, #5
	adds r0, #204
	adds r1, #153
	bl Func_02002d58
	movs r0, #202
	movs r1, #1
	movs r2, #176
	movs r3, #1
	lsls r2, r2, #16
	lsls r0, r0, #18
	negs r1, r1
	bl Motion_CamBounds
	ldr r1, [r6]
	movs r0, #7
	bl Func_02002cd8
	ldr r1, [r6]
	movs r0, #6
	bl Func_02002cd8
	ldr r1, [r6]
	movs r0, #5
	bl Func_02002cd8
	ldr r1, [r6]
	movs r0, #8
	bl Func_02002cd8
	movs r0, #1
	bl WaitFrames
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #7
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #6
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #5
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	adds r2, #102
	movs r0, #8
	adds r1, #204
	bl ObjectMotion_SetSpeedParameters
	ldr r1, .L_02009930
	movs r0, #6
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_02009934
	movs r0, #5
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_02009938
	movs r0, #8
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_0200993c
	movs r0, #7
	bl Object_SetActionCallbackAndRefreshById
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #7
	movs r1, #0
	bl Func_02002d20
	movs r1, #128
	movs r0, #7
	lsls r1, r1, #7
	bl Func_02002d30
	movs r0, #7
	movs r1, #0
	bl Func_02002d20
	movs r0, #7
	movs r1, #3
	bl Object_SetModeById
	movs r0, #7
	movs r1, #0
	bl Func_02002d20
	ldr r0, [r6]
	movs r1, #3
	bl Motion_SetModeAndWaitAnimation
	movs r1, #202
	movs r2, #148
	movs r0, #7
	lsls r1, r1, #2
	bl ObjectMotion_SetPositionAndReset
	movs r0, #7
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r1, #1
	movs r0, #7
	bl ObjectMotion_SetActionVariant
	movs r0, #220
	bl Func_02002de0
	ldr r5, .L_02009940
	movs r1, #144
	lsls r1, r1, #3
	adds r0, r5, #0
	bl Scheduler_AddOrUpdateCallback
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #195
	lsls r0, r0, #1
	bl Func_02002de0
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #0
	bl Scheduler_RemoveCallbackFar
	movs r0, #251
	bl Func_02002de0
	movs r0, #172
	movs r2, #232
	movs r3, #136
	lsls r0, r0, #18
	movs r1, #0
	lsls r2, r2, #18
	lsls r3, r3, #18
	bl Func_02002d68
	movs r0, #202
	movs r1, #1
	movs r2, #224
	movs r3, #0
	lsls r0, r0, #18
	negs r1, r1
	lsls r2, r2, #17
	bl Motion_CamBounds
	movs r1, #202
	movs r2, #240
	ldr r0, [r6]
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_02002cd0
	movs r1, #202
	movs r2, #210
	movs r0, #7
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_02002cd0
	movs r1, #196
	movs r2, #234
	movs r0, #6
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_02002cd0
	movs r1, #208
	movs r2, #234
	movs r0, #5
	lsls r1, r1, #18
	lsls r2, r2, #17
	bl Func_02002cd0
	movs r1, #198
	movs r2, #244
	lsls r1, r1, #18
	lsls r2, r2, #17
	movs r0, #8
	bl Func_02002cd0
	bl Func_02002c10
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #246
	bl Func_02002de0
	movs r0, #80
	bl Battle_WaitMode0
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #85
	str r2, [r3]
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #242
	bl PartyInventory_Remove
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #131
	bl GameFlag_SetBit
	movs r0, #147
	lsls r0, r0, #4
	bl GameFlag_SetBit
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #49
	bl GameFlag_SetBit
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #51
	bl GameFlag_SetBit
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #52
	bl GameFlag_SetBit
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #53
	bl GameFlag_SetBit
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #54
	bl GameFlag_SetBit
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #55
	bl GameFlag_SetBit
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #56
	bl GameFlag_SetBit
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #57
	bl GameFlag_SetBit
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #58
	bl GameFlag_SetBit
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #59
	bl GameFlag_SetBit
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #60
	bl GameFlag_SetBit
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #62
	bl GameFlag_SetBit
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #50
	bl GameFlag_SetBit
	movs r0, #132
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_SetBit
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #118
	add r8, r3
	mov r2, r8
	movs r3, #1
	strh r3, [r2]
	movs r0, #90
	bl Func_02002d70
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_02009928:
	.4byte 0x00002168
.L_0200992c:
	.4byte gPartyState
.L_02009930:
	.4byte Data_02002f44
.L_02009934:
	.4byte Data_02002f88
.L_02009938:
	.4byte Data_02002fcc
.L_0200993c:
	.4byte Data_02002f1c
.L_02009940:
	.4byte Func_020015c8
	.section .text.x02009944,"ax",%progbits
	.global Func_02001944
	.thumb_func
Func_02001944:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	adds r6, r1, #0
	mov r8, r0
	adds r0, r6, #0
	bl Object_GetById
	adds r5, r0, #0
	bl Func_02002c68
	movs r0, #0
	bl Func_02002da8
	movs r3, #128
	lsls r3, r3, #5
	str r3, [r5, #24]
	str r3, [r5, #28]
	movs r0, #1
	bl WaitFrames
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #103
	bl Func_02002de0
	mov r0, r8
	ldr r1, .L_020099a4
	bl ObjectMotion_EnableActionAndSetCallback
	ldr r1, .L_020099a8
	adds r0, r6, #0
	bl Object_SetActionCallbackAndRefreshById
	movs r0, #20
	bl Battle_WaitMode0
	bl Func_02002c70
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020099a4:
	.4byte Data_02002e30
.L_020099a8:
	.4byte Data_02002e6c
	.section .text.x020099ac,"ax",%progbits
	.global Func_020019ac
	.thumb_func
Func_020019ac:
	push {lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #50
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020099dc
	bl Func_02002c68
	movs r0, #0
	bl Func_02002da8
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	movs r0, #20
	bl Battle_WaitMode0
	bl Func_02001a04
	bl Func_02002c70
.L_020099dc:
	pop {pc}
	.2byte 0x0000
	.section .text.x020099e0,"ax",%progbits
	.global Func_020019e0
	.thumb_func
Func_020019e0:
	push {lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #50
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009a02
	bl Func_02002c68
	movs r0, #0
	bl Func_02002da8
	bl Func_02001a04
	bl Func_02002c70
.L_02009a02:
	pop {pc}
	.section .text.x02009a04,"ax",%progbits
	.global Func_02001a04
	.thumb_func
Func_02001a04:
	push {r5, lr}
	movs r0, #9
	bl Object_GetById
	movs r1, #128
	movs r2, #0
	adds r5, r0, #0
	lsls r1, r1, #1
	movs r0, #9
	bl Func_02002d40
	movs r0, #9
	movs r1, #2
	bl Motion_SetVarCbAndRefresh
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r5, #72]
	movs r0, #9
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	movs r1, #204
	movs r2, #204
	lsls r1, r1, #8
	lsls r2, r2, #7
	movs r0, #9
	adds r1, #204
	adds r2, #102
	bl ObjectMotion_SetSpeedParameters
	movs r0, #9
	movs r1, #6
	movs r2, #0
	bl ObjectMotion_Launch
	movs r1, #208
	lsls r1, r1, #2
	movs r2, #88
	movs r0, #9
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #20
	bl Battle_WaitMode0
	movs r2, #204
	lsls r2, r2, #8
	adds r2, #204
	movs r0, #9
	ldr r1, .L_02009a98
	bl ObjectMotion_SetSpeedParameters
	movs r0, #9
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r1, #206
	movs r0, #9
	lsls r1, r1, #2
	movs r2, #72
	bl ObjectMotion_SetPositionAndCommit
	movs r1, #192
	movs r0, #9
	lsls r1, r1, #6
	movs r2, #40
	bl ObjectMotion_ArmCallback
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	pop {r5, pc}
.L_02009a98:
	.4byte 0x00019999
	.section .text.x02009a9c,"ax",%progbits
	.global Func_02001a9c
	.thumb_func
Func_02001a9c:
	push {lr}
	bl Func_02002c68
	movs r0, #0
	bl Func_02002da8
	movs r0, #164
	bl Func_02002de0
	movs r0, #120
	bl Battle_WaitMode0
	bl Func_02002c70
	pop {pc}
	.2byte 0x0000
	.section .text.x02009abc,"ax",%progbits
	.global Func_02001abc
	.thumb_func
Func_02001abc:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r0, #214
	lsls r0, r0, #1
	movs r2, #133
	adds r3, r3, r0
	lsls r2, r2, #1
	movs r0, #144
	adds r2, #255
	lsls r0, r0, #4
	str r2, [r3]
	adds r0, #180
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009ae8
	movs r0, #162
	lsls r0, r0, #1
	bl GameFlag_SetBit
.L_02009ae8:
	ldr r2, .L_02009c24
	movs r1, #240
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r0, #0
	ldrsh r1, [r3, r0]
	ldr r3, .L_02009c28
	cmp r1, r3
	bne .L_02009ba2
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	subs r3, #3
	cmp r3, #27
	bls .L_02009b0c
	b .L_02009c20
.L_02009b0c:
	ldr r2, .L_02009c2c
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_02009b14:
	.4byte .L_02009b84
	.4byte .L_02009b84
	.4byte .L_02009b84
	.4byte .L_02009b8a
	.4byte .L_02009b8a
	.4byte .L_02009b8a
	.4byte .L_02009c20
	.4byte .L_02009c20
	.4byte .L_02009c20
	.4byte .L_02009c20
	.4byte .L_02009c20
	.4byte .L_02009c20
	.4byte .L_02009b96
	.4byte .L_02009b96
	.4byte .L_02009b96
	.4byte .L_02009b96
	.4byte .L_02009c20
	.4byte .L_02009c20
	.4byte .L_02009c20
	.4byte .L_02009c20
	.4byte .L_02009c20
	.4byte .L_02009c20
	.4byte .L_02009b90
	.4byte .L_02009b90
	.4byte .L_02009b90
	.4byte .L_02009b90
	.4byte .L_02009b9c
	.4byte .L_02009b9c
.L_02009b84:
	bl Func_02001c88
	b .L_02009c20
.L_02009b8a:
	bl Func_020027e4
	b .L_02009c20
.L_02009b90:
	bl Func_02001e0c
	b .L_02009c20
.L_02009b96:
	bl Func_0200241c
	b .L_02009c20
.L_02009b9c:
	bl Func_02002498
	b .L_02009c20
.L_02009ba2:
	ldr r3, .L_02009c30
	cmp r1, r3
	bne .L_02009c20
	movs r0, #241
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	subs r3, #1
	cmp r3, #19
	bhi .L_02009c20
	ldr r2, .L_02009c34
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_02009bc0:
	.4byte .L_02009c10
	.4byte .L_02009c10
	.4byte .L_02009c10
	.4byte .L_02009c10
	.4byte .L_02009c20
	.4byte .L_02009c16
	.4byte .L_02009c20
	.4byte .L_02009c20
	.4byte .L_02009c1c
	.4byte .L_02009c16
	.4byte .L_02009c10
	.4byte .L_02009c10
	.4byte .L_02009c10
	.4byte .L_02009c10
	.4byte .L_02009c10
	.4byte .L_02009c10
	.4byte .L_02009c10
	.4byte .L_02009c10
	.4byte .L_02009c20
	.4byte .L_02009c10
.L_02009c10:
	bl Func_02002820
	b .L_02009c20
.L_02009c16:
	bl Func_02002970
	b .L_02009c20
.L_02009c1c:
	bl Func_02002b98
.L_02009c20:
	movs r0, #0
	pop {pc}
.L_02009c24:
	.4byte gPartyState
.L_02009c28:
	.4byte 0x00000028
.L_02009c2c:
	.4byte .L_02009b14
.L_02009c30:
	.4byte 0x00000029
.L_02009c34:
	.4byte .L_02009bc0
	.section .text.x02009c38,"ax",%progbits
	.global Func_02001c38
	.thumb_func
Func_02001c38:
	push {r5, lr}
	movs r5, #0
.L_02009c3c:
	movs r2, #192
	lsls r2, r2, #2
	adds r0, r5, r2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009c70
	ldr r3, .L_02009c84
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #86
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #1
	bne .L_02009c70
	movs r3, #147
	lsls r3, r3, #4
	adds r0, r5, r3
	bl GameFlag_SetBit
	movs r2, #136
	lsls r2, r2, #2
	adds r0, r5, r2
	bl GameFlag_SetBit
.L_02009c70:
	movs r3, #192
	lsls r3, r3, #2
	adds r0, r5, r3
	adds r5, #1
	bl GameFlag_ClearBit
	cmp r5, #15
	bls .L_02009c3c
	movs r0, #0
	pop {r5, pc}
.L_02009c84:
	.4byte gPartyState
	.section .text.x02009c88,"ax",%progbits
	.global Func_02001c88
	.thumb_func
Func_02001c88:
	push {r5, lr}
	movs r0, #147
	lsls r0, r0, #4
	sub sp, #8
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009d1a
	movs r0, #136
	lsls r0, r0, #2
	bl GameFlag_Test
	adds r5, r0, #0
	cmp r5, #0
	beq .L_02009cda
	movs r3, #6
	movs r2, #25
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #25
	movs r2, #1
	movs r3, #3
	movs r0, #0
	bl Func_02002c38
	movs r0, #10
	bl Object_GetById
	movs r1, #208
	movs r3, #212
	lsls r3, r3, #17
	lsls r1, r1, #15
	movs r2, #0
	bl Object_SetPositionAndResetMotion
	movs r0, #10
	bl Object_GetById
	movs r3, #0
	str r3, [r0, #20]
	b .L_02009d14
.L_02009cda:
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_02002cd0
	movs r3, #6
	movs r2, #25
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #25
	movs r2, #1
	movs r3, #3
	movs r0, #0
	bl Func_02002c38
	movs r0, #10
	bl Object_GetById
	movs r1, #208
	movs r3, #212
	lsls r1, r1, #15
	movs r2, #0
	lsls r3, r3, #17
	bl Object_SetPositionAndResetMotion
	movs r0, #10
	bl Object_GetById
	str r5, [r0, #20]
.L_02009d14:
	movs r0, #1
	bl WaitFrames
.L_02009d1a:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #49
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009d76
	movs r0, #145
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009d40
	movs r0, #9
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	b .L_02009d4a
.L_02009d40:
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02002cd0
.L_02009d4a:
	movs r0, #11
	bl Object_GetById
	movs r1, #208
	movs r3, #134
	lsls r1, r1, #15
	ldr r2, .L_02009dc0
	lsls r3, r3, #18
	bl Object_SetPositionAndResetMotion
	movs r0, #11
	bl Object_GetById
	ldr r3, .L_02009dc0
	movs r1, #1
	str r3, [r0, #20]
	movs r0, #11
	bl ObjectMotion_SetActionVariant
	movs r0, #1
	bl WaitFrames
.L_02009d76:
	ldr r0, .L_02009dc4
	bl Func_02002db8
	movs r0, #136
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009d9a
	movs r0, #8
	movs r1, #10
	bl Func_02001944
	movs r0, #136
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	b .L_02009dba
.L_02009d9a:
	movs r0, #145
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009dba
	movs r0, #9
	movs r1, #11
	bl Func_02001944
	movs r0, #145
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
.L_02009dba:
	add sp, #8
	pop {r5, pc}
	.2byte 0x0000
.L_02009dc0:
	.4byte 0xffe00000
.L_02009dc4:
	.4byte Data_02003014
	.section .text.x02009dc8,"ax",%progbits
	.global Func_02001dc8
	.thumb_func
Func_02001dc8:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	adds r5, r0, #0
	bl Object_GetById
	adds r6, r0, #0
	adds r0, r5, #0
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	movs r1, #0
	mov r8, r1
	strb r3, [r0]
	movs r1, #1
	adds r0, r5, #0
	bl ObjectMotion_SetActionVariant
	adds r3, r6, #0
	adds r3, #85
	mov r2, r8
	strb r2, [r3]
	ldr r3, .L_02009e08
	str r3, [r6, #12]
	str r3, [r6, #20]
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02009e08:
	.4byte 0xffe00000
	.section .text.x02009e0c,"ax",%progbits
	.global Func_02001e0c
	.thumb_func
Func_02001e0c:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_0200a100
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #8
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #32
	orrs r3, r5
	strb r3, [r0]
	ldr r0, .L_0200a104
	bl Func_02002dc8
	movs r0, #8
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r1, #1
	orrs r3, r5
	strb r3, [r0]
	movs r0, #8
	bl ObjectMotion_SetActionVariant
	movs r0, #9
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #50
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009edc
	movs r0, #15
	bl Object_GetById
	adds r5, r0, #0
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r0, #15
	bl Object_GetById
	movs r1, #206
	movs r3, #144
	lsls r3, r3, #15
	lsls r1, r1, #18
	ldr r2, .L_0200a108
	bl Object_SetPositionAndResetMotion
	ldr r3, .L_0200a108
	movs r0, #128
	lsls r0, r0, #2
	str r3, [r5, #20]
	adds r0, #18
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009eb0
	movs r3, #110
	movs r2, #4
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #110
	movs r1, #1
	movs r2, #3
	movs r3, #1
	bl Func_02002c38
	b .L_02009ec4
.L_02009eb0:
	movs r3, #110
	movs r2, #4
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #110
	movs r1, #0
	movs r2, #3
	movs r3, #1
	bl Func_02002c38
.L_02009ec4:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #34
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009edc
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02002cd0
.L_02009edc:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #131
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009fbc
	movs r0, #132
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009f36
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_02002cd0
	movs r3, #3
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #68
	movs r1, #68
	movs r2, #50
	movs r3, #64
	bl Func_02002c30
	movs r3, #110
	movs r5, #4
	str r3, [sp, #0]
	movs r0, #102
	movs r1, #16
	movs r2, #3
	movs r3, #2
	str r5, [sp, #4]
	bl Func_02002c38
	movs r3, #50
	str r3, [sp, #0]
	movs r0, #50
	movs r1, #0
	movs r2, #3
	movs r3, #1
	b .L_02009fa4
.L_02009f36:
	movs r0, #15
	bl Object_GetById
	movs r1, #206
	movs r3, #144
	lsls r1, r1, #18
	ldr r2, .L_0200a108
	lsls r3, r3, #15
	bl Object_SetPositionAndResetMotion
	movs r0, #15
	bl Object_GetById
	ldr r3, .L_0200a108
	movs r1, #68
	str r3, [r0, #20]
	movs r3, #3
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #72
	movs r2, #50
	movs r3, #64
	bl Func_02002c30
	movs r5, #4
	movs r0, #106
	movs r1, #16
	movs r2, #3
	movs r3, #2
	movs r6, #110
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02002c38
	movs r3, #50
	str r3, [sp, #0]
	movs r0, #50
	movs r1, #1
	movs r2, #3
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02002c38
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009fac
	movs r0, #110
	movs r1, #1
	movs r2, #3
	movs r3, #1
	str r6, [sp, #0]
.L_02009fa4:
	str r5, [sp, #4]
	bl Func_02002c38
	b .L_02009fbc
.L_02009fac:
	movs r0, #110
	movs r1, #0
	movs r2, #3
	movs r3, #1
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02002c38
.L_02009fbc:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #51
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a03a
	movs r3, #46
	str r3, [sp, #0]
	movs r5, #5
	movs r0, #42
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02002c38
	movs r3, #45
	str r3, [sp, #0]
	movs r0, #43
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02002c38
	movs r3, #47
	str r3, [sp, #0]
	movs r1, #0
	movs r2, #1
	movs r3, #1
	movs r0, #43
	str r5, [sp, #4]
	bl Func_02002c38
	movs r0, #16
	bl Object_GetById
	movs r1, #186
	movs r3, #176
	lsls r3, r3, #15
	lsls r1, r1, #18
	ldr r2, .L_0200a108
	bl Object_SetPositionAndResetMotion
	movs r0, #16
	bl Object_GetById
	ldr r3, .L_0200a108
	str r3, [r0, #20]
	movs r0, #146
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a040
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_02002cd0
	b .L_0200a040
.L_0200a03a:
	movs r0, #10
	bl Func_02001dc8
.L_0200a040:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #52
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a10c
	movs r0, #8
	bl Object_GetById
	movs r6, #47
	movs r5, #9
	adds r7, r0, #0
	movs r1, #0
	movs r0, #42
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02002c38
	movs r3, #46
	str r3, [sp, #0]
	movs r0, #43
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02002c38
	movs r3, #48
	str r3, [sp, #0]
	movs r0, #43
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02002c38
	movs r3, #10
	str r3, [sp, #4]
	movs r0, #43
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	bl Func_02002c38
	movs r3, #8
	str r3, [sp, #4]
	movs r1, #0
	movs r2, #1
	movs r3, #1
	movs r0, #43
	str r6, [sp, #0]
	bl Func_02002c38
	movs r0, #17
	bl Object_GetById
	movs r1, #190
	movs r3, #152
	lsls r3, r3, #16
	lsls r1, r1, #18
	ldr r2, .L_0200a108
	bl Object_SetPositionAndResetMotion
	movs r0, #17
	bl Object_GetById
	ldr r3, .L_0200a108
	str r3, [r0, #20]
	ldr r3, [r7, #8]
	asrs r3, r3, #20
	cmp r3, #47
	bne .L_0200a0e6
	ldr r3, [r7, #16]
	asrs r3, r3, #20
	cmp r3, #8
	bne .L_0200a0e6
	ldr r0, .L_0200a104
	bl Func_02002dc8
.L_0200a0e6:
	movs r0, #137
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a112
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02002cd0
	b .L_0200a112
	.2byte 0x0000
.L_0200a100:
	.4byte gPartyState
.L_0200a104:
	.4byte Data_02003010
.L_0200a108:
	.4byte 0xffe00000
.L_0200a10c:
	movs r0, #11
	bl Func_02001dc8
.L_0200a112:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #53
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a1de
	movs r0, #8
	bl Object_GetById
	ldr r3, [r0, #8]
	movs r6, #49
	asrs r3, r3, #20
	mov r8, r3
	ldr r3, [r0, #16]
	movs r5, #7
	movs r0, #42
	movs r1, #0
	movs r2, #1
	asrs r7, r3, #20
	movs r3, #1
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02002c38
	movs r3, #6
	str r3, [sp, #4]
	movs r0, #43
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	bl Func_02002c38
	movs r3, #50
	str r3, [sp, #0]
	movs r0, #43
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02002c38
	movs r3, #48
	str r3, [sp, #0]
	movs r0, #43
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02002c38
	movs r3, #8
	str r3, [sp, #4]
	movs r1, #0
	movs r2, #1
	movs r3, #1
	movs r0, #43
	str r6, [sp, #0]
	bl Func_02002c38
	movs r0, #18
	bl Object_GetById
	movs r1, #198
	movs r3, #240
	lsls r3, r3, #15
	lsls r1, r1, #18
	ldr r2, .L_0200a3e4
	bl Object_SetPositionAndResetMotion
	movs r0, #18
	bl Object_GetById
	ldr r3, .L_0200a3e4
	str r3, [r0, #20]
	mov r3, r8
	cmp r3, #48
	bne .L_0200a1b4
	cmp r7, #7
	beq .L_0200a1be
.L_0200a1b4:
	mov r2, r8
	cmp r2, #49
	bne .L_0200a1c4
	cmp r7, #8
	bne .L_0200a1c4
.L_0200a1be:
	ldr r0, .L_0200a3e8
	bl Func_02002dc8
.L_0200a1c4:
	movs r0, #147
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a1e4
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02002cd0
	b .L_0200a1e4
.L_0200a1de:
	movs r0, #12
	bl Func_02001dc8
.L_0200a1e4:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #54
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a23e
	movs r3, #51
	movs r2, #7
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #0
	movs r2, #1
	movs r3, #1
	movs r0, #42
	bl Func_02002c38
	movs r0, #19
	bl Object_GetById
	movs r1, #206
	movs r3, #240
	lsls r3, r3, #15
	lsls r1, r1, #18
	ldr r2, .L_0200a3e4
	bl Object_SetPositionAndResetMotion
	movs r0, #19
	bl Object_GetById
	ldr r3, .L_0200a3e4
	str r3, [r0, #20]
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #38
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a244
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_02002cd0
	b .L_0200a244
.L_0200a23e:
	movs r0, #13
	bl Func_02001dc8
.L_0200a244:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #55
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a2e6
	movs r6, #51
	movs r5, #9
	movs r0, #42
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02002c38
	movs r3, #8
	str r3, [sp, #4]
	movs r0, #43
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	bl Func_02002c38
	movs r3, #50
	str r3, [sp, #0]
	movs r0, #43
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02002c38
	movs r3, #52
	str r3, [sp, #0]
	movs r0, #43
	movs r1, #0
	movs r2, #1
	movs r3, #1
	str r5, [sp, #4]
	bl Func_02002c38
	movs r3, #10
	str r3, [sp, #4]
	movs r1, #0
	movs r2, #1
	movs r3, #1
	movs r0, #43
	str r6, [sp, #0]
	bl Func_02002c38
	movs r0, #20
	bl Object_GetById
	movs r1, #206
	movs r3, #152
	lsls r3, r3, #16
	lsls r1, r1, #18
	ldr r2, .L_0200a3e4
	bl Object_SetPositionAndResetMotion
	movs r0, #20
	bl Object_GetById
	ldr r3, .L_0200a3e4
	str r3, [r0, #20]
	movs r0, #148
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a2ec
	movs r0, #14
	movs r1, #0
	movs r2, #0
	bl Func_02002cd0
	b .L_0200a2ec
.L_0200a2e6:
	movs r0, #14
	bl Func_02001dc8
.L_0200a2ec:
	ldr r0, .L_0200a3ec
	bl Func_02002db8
	movs r1, #144
	ldr r0, .L_0200a3f0
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #34
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a31e
	movs r0, #9
	movs r1, #15
	bl Func_02001944
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #34
	bl GameFlag_ClearBit
	b .L_0200a3ba
.L_0200a31e:
	movs r0, #146
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a338
	movs r0, #10
	movs r1, #16
	bl Func_02001944
	movs r0, #146
	b .L_0200a36e
.L_0200a338:
	movs r0, #137
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a356
	movs r0, #11
	movs r1, #17
	bl Func_02001944
	movs r0, #137
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	b .L_0200a3ba
.L_0200a356:
	movs r0, #147
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a378
	movs r0, #12
	movs r1, #18
	bl Func_02001944
	movs r0, #147
.L_0200a36e:
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
	b .L_0200a3ba
.L_0200a378:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #38
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a39a
	movs r0, #13
	movs r1, #19
	bl Func_02001944
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #38
	bl GameFlag_ClearBit
	b .L_0200a3ba
.L_0200a39a:
	movs r0, #148
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a3ba
	movs r0, #14
	movs r1, #20
	bl Func_02001944
	movs r0, #148
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
.L_0200a3ba:
	ldr r3, .L_0200a3f4
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #26
	bne .L_0200a3da
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a3da
	bl Func_020019ac
.L_0200a3da:
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a3e4:
	.4byte 0xffe00000
.L_0200a3e8:
	.4byte Data_02003010
.L_0200a3ec:
	.4byte Data_0200301e
.L_0200a3f0:
	.4byte Func_020013f0
.L_0200a3f4:
	.4byte gPartyState
	.section .text.x0200a3f8,"ax",%progbits
	.global Func_020023f8
	.thumb_func
Func_020023f8:
	push {lr}
	ldr r3, .L_0200a418
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #80]
	movs r0, #8
	ldrb r1, [r3, #9]
	lsls r1, r1, #28
	lsrs r1, r1, #30
	bl ObjectMotion_SetActionVariant
	pop {pc}
.L_0200a418:
	.4byte gPartyState
	.section .text.x0200a41c,"ax",%progbits
	.global Func_0200241c
	.thumb_func
Func_0200241c:
	push {lr}
	movs r0, #8
	sub sp, #8
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a43e
	movs r3, #16
	movs r2, #46
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #14
	movs r1, #46
	movs r2, #1
	movs r3, #1
	bl Func_02002c38
.L_0200a43e:
	ldr r0, .L_0200a454
	bl Func_02002dc8
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200a458
	bl Scheduler_AddOrUpdateCallback
	add sp, #8
	pop {pc}
	.2byte 0x0000
.L_0200a454:
	.4byte Data_02003010
.L_0200a458:
	.4byte Func_020023f8
	.section .text.x0200a45c,"ax",%progbits
	.global Func_0200245c
	.thumb_func
Func_0200245c:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	adds r6, r0, #0
	bl Object_GetById
	mov r8, r0
	adds r0, r6, #0
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	movs r1, #1
	adds r0, r6, #0
	bl ObjectMotion_SetActionVariant
	mov r3, r8
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	mov r3, r8
	str r5, [r3, #12]
	str r5, [r3, #20]
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
	.2byte 0x0000
	.section .text.x0200a498,"ax",%progbits
	.global Func_02002498
	.thumb_func
Func_02002498:
	push {r5, lr}
	ldr r3, .L_0200a7d4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #8
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #32
	orrs r3, r5
	strb r3, [r0]
	ldr r0, .L_0200a7d8
	bl Func_02002dc8
	movs r0, #8
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r1, #1
	orrs r5, r3
	strb r5, [r0]
	movs r0, #8
	bl ObjectMotion_SetActionVariant
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #56
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a528
	movs r3, #48
	movs r2, #32
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #0
	movs r2, #1
	movs r3, #1
	movs r0, #44
	bl Func_02002c38
	movs r0, #16
	bl Object_GetById
	movs r1, #194
	movs r3, #130
	lsls r3, r3, #18
	lsls r1, r1, #18
	movs r2, #0
	bl Object_SetPositionAndResetMotion
	movs r0, #16
	bl Object_GetById
	movs r3, #0
	str r3, [r0, #20]
	movs r0, #138
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a52e
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02002cd0
	b .L_0200a52e
.L_0200a528:
	movs r0, #9
	bl Func_0200245c
.L_0200a52e:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #57
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a588
	movs r3, #53
	movs r2, #32
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #0
	movs r2, #1
	movs r3, #1
	movs r0, #44
	bl Func_02002c38
	movs r0, #17
	bl Object_GetById
	movs r1, #214
	movs r3, #130
	lsls r3, r3, #18
	lsls r1, r1, #18
	movs r2, #0
	bl Object_SetPositionAndResetMotion
	movs r0, #17
	bl Object_GetById
	movs r3, #0
	str r3, [r0, #20]
	movs r0, #149
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a58e
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_02002cd0
	b .L_0200a58e
.L_0200a588:
	movs r0, #10
	bl Func_0200245c
.L_0200a58e:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #58
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a5e8
	movs r3, #43
	movs r2, #37
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #0
	movs r2, #1
	movs r3, #1
	movs r0, #44
	bl Func_02002c38
	movs r0, #18
	bl Object_GetById
	movs r1, #174
	movs r3, #150
	lsls r3, r3, #18
	lsls r1, r1, #18
	movs r2, #0
	bl Object_SetPositionAndResetMotion
	movs r0, #18
	bl Object_GetById
	movs r3, #0
	str r3, [r0, #20]
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #42
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a5ee
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02002cd0
	b .L_0200a5ee
.L_0200a5e8:
	movs r0, #11
	bl Func_0200245c
.L_0200a5ee:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #59
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a648
	movs r3, #48
	movs r2, #37
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #0
	movs r2, #1
	movs r3, #1
	movs r0, #44
	bl Func_02002c38
	movs r0, #19
	bl Object_GetById
	movs r1, #194
	movs r3, #150
	lsls r3, r3, #18
	lsls r1, r1, #18
	movs r2, #0
	bl Object_SetPositionAndResetMotion
	movs r0, #19
	bl Object_GetById
	movs r3, #0
	str r3, [r0, #20]
	movs r0, #150
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a64e
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02002cd0
	b .L_0200a64e
.L_0200a648:
	movs r0, #12
	bl Func_0200245c
.L_0200a64e:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #60
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a6a6
	movs r3, #50
	movs r2, #37
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #0
	movs r2, #1
	movs r3, #1
	movs r0, #44
	bl Func_02002c38
	movs r0, #20
	bl Object_GetById
	movs r1, #202
	movs r3, #150
	lsls r3, r3, #18
	lsls r1, r1, #18
	movs r2, #0
	bl Object_SetPositionAndResetMotion
	movs r0, #20
	bl Object_GetById
	movs r3, #0
	str r3, [r0, #20]
	movs r0, #139
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a6ac
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_02002cd0
	b .L_0200a6ac
.L_0200a6a6:
	movs r0, #13
	bl Func_0200245c
.L_0200a6ac:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #62
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a706
	movs r3, #48
	movs r2, #43
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r1, #0
	movs r2, #1
	movs r3, #1
	movs r0, #44
	bl Func_02002c38
	movs r0, #22
	bl Object_GetById
	movs r1, #194
	movs r3, #174
	lsls r3, r3, #18
	lsls r1, r1, #18
	movs r2, #0
	bl Object_SetPositionAndResetMotion
	movs r0, #22
	bl Object_GetById
	movs r3, #0
	str r3, [r0, #20]
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #46
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a70c
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_02002cd0
	b .L_0200a70c
.L_0200a706:
	movs r0, #15
	bl Func_0200245c
.L_0200a70c:
	ldr r0, .L_0200a7dc
	bl Func_02002db8
	movs r1, #144
	ldr r0, .L_0200a7e0
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	movs r0, #138
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a734
	movs r0, #9
	movs r1, #16
	bl Func_02001944
	movs r0, #138
	b .L_0200a7a8
.L_0200a734:
	movs r0, #149
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a74e
	movs r0, #10
	movs r1, #17
	bl Func_02001944
	movs r0, #149
	b .L_0200a788
.L_0200a74e:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #42
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a770
	movs r0, #11
	movs r1, #18
	bl Func_02001944
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #42
	bl GameFlag_ClearBit
	b .L_0200a7d0
.L_0200a770:
	movs r0, #150
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a792
	movs r0, #12
	movs r1, #19
	bl Func_02001944
	movs r0, #150
.L_0200a788:
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
	b .L_0200a7d0
.L_0200a792:
	movs r0, #139
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a7b0
	movs r0, #13
	movs r1, #20
	bl Func_02001944
	movs r0, #139
.L_0200a7a8:
	lsls r0, r0, #2
	bl GameFlag_ClearBit
	b .L_0200a7d0
.L_0200a7b0:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #46
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a7d0
	movs r0, #15
	movs r1, #22
	bl Func_02001944
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #46
	bl GameFlag_ClearBit
.L_0200a7d0:
	add sp, #8
	pop {r5, pc}
.L_0200a7d4:
	.4byte gPartyState
.L_0200a7d8:
	.4byte Data_02003010
.L_0200a7dc:
	.4byte Data_02003034
.L_0200a7e0:
	.4byte Func_0200143c
	.section .text.x0200a7e4,"ax",%progbits
	.global Func_020027e4
	.thumb_func
Func_020027e4:
	push {lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #131
	sub sp, #8
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a81c
	movs r3, #3
	str r3, [sp, #0]
	str r3, [sp, #4]
	movs r0, #17
	movs r1, #125
	movs r2, #27
	movs r3, #63
	bl Func_02002c30
	movs r3, #25
	movs r2, #4
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #25
	movs r1, #0
	movs r2, #5
	movs r3, #3
	bl Func_02002c38
.L_0200a81c:
	add sp, #8
	pop {pc}
	.section .text.x0200a820,"ax",%progbits
	.global Func_02002820
	.thumb_func
Func_02002820:
	push {r5, r6, lr}
	ldr r3, .L_0200a95c
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	ldr r0, [r6]
	sub sp, #8
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	movs r1, #144
	strb r3, [r0]
	lsls r1, r1, #3
	ldr r0, .L_0200a960
	bl Scheduler_AddOrUpdateCallback
	movs r0, #132
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a8d2
	movs r0, #64
	bl Object_GetById
	adds r5, r0, #0
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r0, #64
	bl Object_GetById
	movs r1, #136
	movs r3, #196
	ldr r2, .L_0200a964
	lsls r1, r1, #16
	lsls r3, r3, #17
	bl Object_SetPositionAndResetMotion
	ldr r3, .L_0200a968
	movs r1, #3
	str r3, [r5, #20]
	movs r0, #64
	bl ObjectMotion_SetActionVariant
	movs r0, #64
	bl Object_GetById
	movs r1, #1
	bl Object_SetPartAttribute
	movs r0, #8
	movs r1, #0
	movs r2, #0
	bl Func_02002cd0
	movs r0, #9
	movs r1, #0
	movs r2, #0
	bl Func_02002cd0
	movs r0, #10
	movs r1, #0
	movs r2, #0
	bl Func_02002cd0
	movs r0, #11
	movs r1, #0
	movs r2, #0
	bl Func_02002cd0
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02002cd0
	movs r0, #13
	movs r1, #0
	movs r2, #0
	bl Func_02002cd0
	movs r0, #1
	bl WaitFrames
.L_0200a8d2:
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #228
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a93a
	movs r0, #64
	bl Object_GetById
	ldr r3, .L_0200a96c
	ldr r2, [r0, #16]
	ldr r1, [r0, #8]
	adds r2, r2, r3
	movs r0, #64
	bl Func_02002cd0
	movs r0, #64
	bl Object_GetById
	movs r1, #0
	bl Object_SetPartAttribute
	movs r3, #6
	movs r2, #4
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #6
	movs r1, #0
	movs r2, #1
	movs r3, #2
	bl Func_02002c38
	movs r0, #128
	movs r1, #128
	movs r2, #248
	movs r3, #128
	lsls r2, r2, #16
	lsls r3, r3, #17
	lsls r0, r0, #12
	lsls r1, r1, #13
	bl Func_02002d68
	ldr r0, [r6]
	movs r1, #0
	bl Object_AttachWorkTargetToObject
	bl Func_02002c10
	movs r0, #1
	bl WaitFrames
.L_0200a93a:
	movs r0, #152
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a956
	bl Func_02001a9c
	movs r0, #152
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_ClearBit
.L_0200a956:
	add sp, #8
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0200a95c:
	.4byte gPartyState
.L_0200a960:
	.4byte Func_02001474
.L_0200a964:
	.4byte 0xffe00000
.L_0200a968:
	.4byte 0xfff00000
.L_0200a96c:
	.4byte 0xfed00000
	.section .text.x0200a970,"ax",%progbits
	.global Func_02002970
	.thumb_func
Func_02002970:
	push {r5, lr}
	ldr r3, .L_0200ab80
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #8
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
	ldr r0, .L_0200ab84
	bl Func_02002dc8
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #130
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a9c8
	movs r3, #3
	movs r2, #2
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #4
	movs r1, #124
	movs r2, #31
	movs r3, #64
	bl Func_02002c30
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_SetBit
	movs r0, #130
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
.L_0200a9c8:
	movs r0, #8
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200aa46
	movs r0, #8
	bl Object_GetById
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	adds r5, r0, #0
	movs r1, #68
	movs r0, #35
	movs r2, #35
	movs r3, #69
	bl Func_02002c30
	movs r3, #35
	movs r2, #9
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #35
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl Func_02002c38
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r1, #142
	movs r3, #152
	ldr r2, .L_0200ab88
	lsls r3, r3, #16
	adds r0, r5, #0
	lsls r1, r1, #18
	bl Object_SetPositionAndResetMotion
	movs r1, #3
	movs r0, #8
	bl ObjectMotion_SetActionVariant
	movs r0, #129
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r1, #142
	movs r2, #152
	movs r0, #10
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02002cd0
.L_0200aa46:
	movs r0, #129
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200aa72
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200aa6a
	movs r0, #8
	movs r1, #3
	bl Object_SetModeById
	b .L_0200aa72
.L_0200aa6a:
	movs r0, #8
	movs r1, #2
	bl Object_SetModeById
.L_0200aa72:
	movs r0, #9
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200aa94
	movs r0, #130
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
.L_0200aa94:
	movs r0, #130
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200aafc
	movs r0, #9
	bl Object_GetById
	movs r3, #1
	str r3, [sp, #0]
	str r3, [sp, #4]
	adds r5, r0, #0
	movs r1, #68
	movs r0, #29
	movs r2, #29
	movs r3, #69
	bl Func_02002c30
	movs r3, #29
	movs r2, #9
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #35
	movs r1, #0
	movs r2, #1
	movs r3, #1
	bl Func_02002c38
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	ldr r3, .L_0200ab88
	movs r1, #3
	str r3, [r5, #12]
	movs r0, #9
	bl ObjectMotion_SetActionVariant
	movs r0, #131
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	movs r1, #236
	movs r2, #152
	movs r0, #11
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_02002cd0
.L_0200aafc:
	movs r0, #131
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200ab2a
	movs r0, #130
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200ab22
	movs r0, #9
	movs r1, #3
	bl Object_SetModeById
	b .L_0200ab2a
.L_0200ab22:
	movs r0, #9
	movs r1, #2
	bl Object_SetModeById
.L_0200ab2a:
	movs r0, #1
	bl WaitFrames
	ldr r0, .L_0200ab8c
	bl Func_02002db8
	movs r1, #144
	ldr r0, .L_0200ab90
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	movs r0, #132
	lsls r0, r0, #2
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200ab58
	movs r0, #10
	bl Object_GetById
	movs r1, #15
	bl Object_SetPartAttribute
.L_0200ab58:
	movs r0, #137
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200ab72
	movs r0, #11
	bl Object_GetById
	movs r1, #15
	bl Object_SetPartAttribute
.L_0200ab72:
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200ab94
	bl Scheduler_AddOrUpdateCallback
	add sp, #8
	pop {r5, pc}
.L_0200ab80:
	.4byte gPartyState
.L_0200ab84:
	.4byte Data_0200304e
.L_0200ab88:
	.4byte 0xfff00000
.L_0200ab8c:
	.4byte Data_02003014
.L_0200ab90:
	.4byte Func_02001464
.L_0200ab94:
	.4byte Func_0200155c
	.section .text.x0200ab98,"ax",%progbits
	.global Func_02002b98
	.thumb_func
Func_02002b98:
	push {lr}
	movs r0, #144
	lsls r0, r0, #4
	adds r0, #131
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200abac
	bl Func_02001650
.L_0200abac:
	pop {pc}
	.2byte 0x0000
	.section .rodata.x0200ade8,"a",%progbits
	.global Data_02002de8
Data_02002de8:
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000017
	.4byte 0x00000007
	.4byte 0xffff8000
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xfffffc00
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffffc00
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x00000018
	.4byte 0x00000000
	.4byte 0x00000011
	.global Data_02002e30
Data_02002e30:
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xfffffe00
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffffc00
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000003c
	.4byte 0x00000000
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02002e6c
Data_02002e6c:
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00000400
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00000400
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x0000003c
	.4byte 0x00000000
	.4byte 0x00000011
	.global Data_02002e9c
Data_02002e9c:
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0x00003000
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0x00003000
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xffffe800
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xffffe800
	.4byte 0x00000000
	.4byte 0x00000004
	.4byte 0x0000000d
	.4byte 0x00000008
	.4byte 0x00000000
	.4byte 0x00000011
	.4byte 0x00000017
	.4byte 0x00000009
	.4byte 0xfffffe00
	.4byte 0x00000017
	.4byte 0x0000000a
	.4byte 0xfffffe00
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x0000000d
	.4byte 0x000000a0
	.4byte 0x00000000
	.4byte 0x00000011
	.global Data_02002f1c
Data_02002f1c:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000011
	.global Data_02002f44
Data_02002f44:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x03100000
	.4byte 0x00000000
	.4byte 0x00c40000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000e000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.global Data_02002f88
Data_02002f88:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x03400000
	.4byte 0x00000000
	.4byte 0x00c40000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000a000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.global Data_02002fcc
Data_02002fcc:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000004
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x00d80000
	.4byte 0x00000001
	.4byte 0x00000027
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000016
	.4byte 0x00000003
	.4byte 0x0000d000
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000011
	.global Data_02003010
Data_02003010:
	.4byte 0xffff0008
	.global Data_02003014
Data_02003014:
	.4byte 0x0210000a
	.4byte 0x0211000b
	.2byte 0xffff
	.global Data_0200301e
Data_0200301e:
	.2byte 0x0010
	.4byte 0x00110213
	.4byte 0x00120214
	.4byte 0x00130215
	.4byte 0x00140216
	.4byte 0xffff0217
	.global Data_02003034
Data_02003034:
	.4byte 0x02180010
	.4byte 0x02190011
	.4byte 0x021a0012
	.4byte 0x021b0013
	.4byte 0x021c0014
	.4byte 0x021e0016
	.2byte 0xffff
	.global Data_0200304e
Data_0200304e:
	.2byte 0x0008
	.4byte 0xffff0009
	.global Data_02003054
Data_02003054:
	.4byte 0xffff0000
	.4byte 0x000000ac
	.4byte 0x40000095
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003084
Data_02003084:
	.4byte 0xffff0009
	.4byte 0x00000196
	.4byte 0xc0000178
	.4byte 0x01500000
	.4byte 0x02680108
	.4byte 0x000001a8
	.4byte 0xffff000a
	.4byte 0x00000226
	.4byte 0xc0000179
	.4byte 0x01500000
	.4byte 0x02680108
	.4byte 0x000001a8
	.4byte 0xffff000b
	.4byte 0x00000236
	.4byte 0xc0000379
	.4byte 0x01e80000
	.4byte 0x02d80308
	.4byte 0x000003a8
	.4byte 0xffff000c
	.4byte 0x00000288
	.4byte 0xc000037e
	.4byte 0x01e80000
	.4byte 0x02d80308
	.4byte 0x000003a8
	.4byte 0xffff000d
	.4byte 0x000001b8
	.4byte 0x000000d8
	.4byte 0x01800000
	.4byte 0x02700080
	.4byte 0x00000120
	.4byte 0xffff000e
	.4byte 0x00000237
	.4byte 0x400000d7
	.4byte 0x01800000
	.4byte 0x02700080
	.4byte 0x00000120
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_0200312c
Data_0200312c:
	.4byte 0xffff0000
	.4byte 0x00000048
	.4byte 0x40000048
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0xffff0001
	.4byte 0x00000048
	.4byte 0x40000048
	.4byte 0x00080000
	.4byte 0x00f80010
	.4byte 0x00000100
	.4byte 0xffff0002
	.4byte 0x000000a8
	.4byte 0x40000048
	.4byte 0x00080000
	.4byte 0x00f80010
	.4byte 0x00000100
	.4byte 0xffff0003
	.4byte 0x00000048
	.4byte 0xc00000d8
	.4byte 0x00080000
	.4byte 0x00f80010
	.4byte 0x00000100
	.4byte 0xffff0004
	.4byte 0x000000b8
	.4byte 0xc00000d8
	.4byte 0x00080000
	.4byte 0x00f80010
	.4byte 0x00000100
	.4byte 0xffff000b
	.4byte 0x00000048
	.4byte 0x40000178
	.4byte 0x00080000
	.4byte 0x00f80140
	.4byte 0x00000230
	.4byte 0xffff000c
	.4byte 0x000000a8
	.4byte 0x40000178
	.4byte 0x00080000
	.4byte 0x00f80140
	.4byte 0x00000230
	.4byte 0xffff000d
	.4byte 0x00000048
	.4byte 0xc0000200
	.4byte 0x00080000
	.4byte 0x00f80140
	.4byte 0x00000230
	.4byte 0xffff000e
	.4byte 0x000000b8
	.4byte 0xc0000200
	.4byte 0x00080000
	.4byte 0x00f80140
	.4byte 0x00000230
	.4byte 0xffff000f
	.4byte 0x00000078
	.4byte 0xc00001b8
	.4byte 0x00080000
	.4byte 0x00f80140
	.4byte 0x00000230
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003234
Data_02003234:
	.4byte 0xffec01d2
	.4byte 0x01da0244
	.4byte 0x024cfff4
	.4byte 0x0013ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003254
Data_02003254:
	.4byte 0x00100200
	.4byte 0x021000be
	.4byte 0x00ce0020
	.4byte 0x000affff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSceneExits
gSceneExits:
	.4byte 0x00000028
	.4byte 0x1010102b
	.4byte 0x00000907
	.4byte 0x101030c8
	.4byte 0x000009b4
	.4byte 0x101040ea
	.4byte 0xffffffff
	.4byte 0x00203028
	.4byte 0x00302028
	.4byte 0x00406028
	.4byte 0x0050d028
	.4byte 0x00604028
	.4byte 0x00713028
	.4byte 0x10801029
	.4byte 0x0000093f
	.4byte 0x1080b029
	.4byte 0x000009e4
	.4byte 0x10810029
	.4byte 0xffffffff
	.4byte 0x10902029
	.4byte 0x0000093f
	.4byte 0x1090c029
	.4byte 0x000009e4
	.4byte 0x10911029
	.4byte 0xffffffff
	.4byte 0x00a19028
	.4byte 0x00b1a028
	.4byte 0x00c1d028
	.4byte 0x00d05028
	.4byte 0x10e03029
	.4byte 0x0000093f
	.4byte 0x10e0d029
	.4byte 0x000009e4
	.4byte 0x10e12029
	.4byte 0xffffffff
	.4byte 0x10f04029
	.4byte 0x0000093f
	.4byte 0x10f0e029
	.4byte 0x000009e4
	.4byte 0x10f14029
	.4byte 0xffffffff
	.4byte 0x0101b028
	.4byte 0x0111c028
	.4byte 0x0121e028
	.4byte 0x0130a029
	.4byte 0x01407028
	.4byte 0x0190a028
	.4byte 0x01a0b028
	.4byte 0x01b10028
	.4byte 0x01c11028
	.4byte 0x01d0c028
	.4byte 0x01e12028
	.4byte 0x00000029
	.4byte 0x00108028
	.4byte 0x00209028
	.4byte 0x0030e028
	.4byte 0x0040f028
	.4byte 0x00607029
	.4byte 0x00706029
	.4byte 0x10809029
	.4byte 0x00000983
	.4byte 0x10813029
	.4byte 0xffffffff
	.4byte 0x00908029
	.4byte 0x00a14028
	.4byte 0x05a45002
	.4byte 0x000001ff
	.global Data_02003380
Data_02003380:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003398
Data_02003398:
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x01a80000
	.4byte 0x00004000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x02180000
	.4byte 0x00004000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0122
	.4byte 0x00000001
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
	.global Data_0200b410
Data_0200b410:
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00004000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x03580000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00004000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x02e80000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00004000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x02f80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00004000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x03180000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00004000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00004000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00004000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0122
	.4byte 0x00000001
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
	.global Data_02003560
Data_02003560:
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0x02e80000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003590
Data_02003590:
	.4byte 0xffff011f
	.4byte 0x00000001
	.4byte 0x03380000
	.4byte 0x00000000
	.4byte 0x02280000
	.4byte 0x00004000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00004000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x03580000
	.4byte 0x00000000
	.4byte 0x02080000
	.4byte 0x00004000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x02b80000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00004000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00004000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x03280000
	.4byte 0x00000000
	.4byte 0x02580000
	.4byte 0x00004000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00004000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x03080000
	.4byte 0x00000000
	.4byte 0x02b80000
	.4byte 0x00004000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0122
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00004000
	.4byte 0xffff0122
	.4byte 0x00000001
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
	.global Data_02003710
Data_02003710:
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x00780000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x00004000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x01004000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00780000
	.4byte 0x01004000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x01004000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x00980000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x01004000
	.4byte 0xffff00f8
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x01004000
	.4byte 0x00000007
	.4byte 0x00000001
	.4byte 0x00080000
	.4byte 0x00000000
	.4byte 0x00080000
	.4byte 0x00024000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020037d0
Data_020037d0:
	.4byte 0xffff013a
	.4byte 0x00000001
	.4byte 0x02480000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff013a
	.4byte 0x00000001
	.4byte 0x01d80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00024000
	.4byte 0xffff0122
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0122
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
	.global Data_02003848
Data_02003848:
	.4byte 0xffff0007
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0006
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0005
	.4byte 0x00000001
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00024000
	.4byte 0xffff0038
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
.L_0200b8c0:
	.4byte 0x007e0000
	.4byte 0x00020001
	.4byte 0x00010002
	.4byte 0x0001007e
	.4byte 0x00040002
	.4byte 0x007e0002
	.4byte 0x00020001
	.4byte 0xffff0004
.L_0200b8e0:
	.4byte 0x007e000a
	.4byte 0x00020001
	.4byte 0x000b0004
	.4byte 0x0001007e
	.4byte 0x00040002
	.4byte 0x0004ffff
	.4byte 0x0002007e
	.4byte 0x00020002
	.4byte 0x0000ffff
	.global Data_02003904
Data_02003904:
	.4byte 0x00000002
	.4byte .L_0200b8e0
	.4byte 0x00030004
	.4byte 0x0000000e
	.4byte .L_0200b8c0
	.4byte 0x000b0023
	.4byte 0x0000000f
	.4byte .L_0200b8c0
	.4byte 0x002d0006
	.4byte 0x00000010
	.4byte .L_0200b8c0
	.4byte 0x002d000b
	.4byte 0x00000011
	.4byte .L_0200b8c0
	.4byte 0x002d0015
	.4byte 0x00000012
	.4byte .L_0200b8c0
	.4byte 0x002e001a
	.4byte 0x00000014
	.4byte .L_0200b8c0
	.4byte 0x001c001d
	.4byte 0x00000019
	.4byte .L_0200b8c0
	.4byte 0x00020030
	.4byte 0x0000001a
	.4byte .L_0200b8c0
	.4byte 0x00020036
	.4byte 0x0000001d
	.4byte .L_0200b8c0
	.4byte 0x001d0029
	.global Data_0200397c
Data_0200397c:
	.4byte 0x00000001
	.4byte .L_0200b8c0
	.4byte 0x00020004
	.4byte 0x00000002
	.4byte .L_0200b8c0
	.4byte 0x0002000a
	.4byte 0x00000006
	.4byte .L_0200b8c0
	.4byte 0x0003001f
	.4byte 0x00000008
	.4byte .L_0200b8c0
	.4byte 0x00330029
	.4byte 0x0000000b
	.4byte .L_0200b8c0
	.4byte 0x00150004
	.4byte 0x0000000c
	.4byte .L_0200b8c0
	.4byte 0x0015000a
	.global Data_020039c4
Data_020039c4:
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020039d0
Data_020039d0:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte Func_0200021c
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
	.4byte 0x0000c602
	.4byte 0xffff000e
	.4byte Func_0200021c
	.4byte 0x0000c602
	.4byte 0xffff0014
	.4byte Func_0200021c
	.4byte 0x000000f3
	.4byte 0xffff00c8
	.4byte 0x00403063
	.4byte 0x000001c3
	.4byte 0xffff00c9
	.4byte 0x0040306c
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003a84
Data_02003a84:
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte Func_02000bf0
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte Func_02000bf0
	.4byte 0x00001815
	.4byte 0x0210000a
	.4byte Func_02000818
	.4byte 0x00001815
	.4byte 0x0211000b
	.4byte Func_02000818
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003ae4
Data_02003ae4:
	.4byte 0x0000c602
	.4byte 0xffff0019
	.4byte Func_0200021c
	.4byte 0x0000c602
	.4byte 0xffff001a
	.4byte Func_0200021c
	.4byte 0x00000001
	.4byte 0xffff001b
	.4byte 0x0000001b
	.4byte 0x00000001
	.4byte 0xffff001c
	.4byte 0x0000001c
	.4byte 0x00000000
	.4byte 0xffff0009
	.4byte Func_02000c50
	.4byte 0x00000000
	.4byte 0xffff000a
	.4byte Func_02000c50
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte Func_02000c50
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte Func_02000c50
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte Func_02000c50
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte Func_02000c50
	.4byte 0x00001815
	.4byte 0x0212000f
	.4byte Func_02000838
	.4byte 0x00001815
	.4byte 0x02130010
	.4byte Func_02000828
	.4byte 0x00001815
	.4byte 0x02140011
	.4byte Func_02000828
	.4byte 0x00001815
	.4byte 0x02150012
	.4byte Func_02000828
	.4byte 0x00001815
	.4byte 0x02160013
	.4byte Func_02000828
	.4byte 0x00001815
	.4byte 0x02170014
	.4byte Func_02000828
	.4byte 0x10008c15
	.4byte 0xffff0008
	.4byte Func_02000770
	.4byte 0x00008c15
	.4byte 0xffff0008
	.4byte Func_02000780
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte Func_02000770
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_02000780
	.4byte 0x00000003
	.4byte 0xffff0028
	.4byte Func_0200065c
	.4byte 0x00000002
	.4byte 0x02120029
	.4byte Func_02000870
	.4byte 0x00000002
	.4byte 0x0212002b
	.4byte Func_02000af4
	.4byte 0x00000002
	.4byte 0x0201002a
	.4byte Func_020019e0
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003c10
Data_02003c10:
	.4byte 0x0000c602
	.4byte 0xffff000f
	.4byte Func_0200021c
	.4byte 0x0000c602
	.4byte 0xffff0010
	.4byte Func_0200021c
	.4byte 0x0000c602
	.4byte 0xffff0011
	.4byte Func_0200021c
	.4byte 0x0000c602
	.4byte 0xffff0012
	.4byte Func_0200021c
	.4byte 0x10008c15
	.4byte 0xffff0008
	.4byte Func_02000788
	.4byte 0x00008c15
	.4byte 0xffff0008
	.4byte Func_02000798
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte Func_02000788
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_02000798
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003c7c
Data_02003c7c:
	.4byte 0x0000c602
	.4byte 0xffff001d
	.4byte Func_0200021c
	.4byte 0x00000001
	.4byte 0xffff001e
	.4byte 0x0000001e
	.4byte 0x00000400
	.4byte 0xffff0009
	.4byte Func_02000cb0
	.4byte 0x00004400
	.4byte 0xffff0009
	.4byte Func_02000cb0
	.4byte 0x00008400
	.4byte 0xffff000a
	.4byte Func_02000cb0
	.4byte 0x00004400
	.4byte 0xffff000a
	.4byte Func_02000cb0
	.4byte 0x0000c400
	.4byte 0xffff000a
	.4byte Func_02000cb0
	.4byte 0x00000000
	.4byte 0xffff000b
	.4byte Func_02000cb0
	.4byte 0x00000000
	.4byte 0xffff000c
	.4byte Func_02000cb0
	.4byte 0x0000c400
	.4byte 0xffff000d
	.4byte Func_02000cb0
	.4byte 0x00000400
	.4byte 0xffff000f
	.4byte Func_02000cb0
	.4byte 0x00008400
	.4byte 0xffff000f
	.4byte Func_02000cb0
	.4byte 0x0000c400
	.4byte 0xffff000f
	.4byte Func_02000cb0
	.4byte 0x00001815
	.4byte 0x02180010
	.4byte Func_02000860
	.4byte 0x00001815
	.4byte 0x02190011
	.4byte Func_02000860
	.4byte 0x00001815
	.4byte 0x021a0012
	.4byte Func_02000860
	.4byte 0x00001815
	.4byte 0x021b0013
	.4byte Func_02000860
	.4byte 0x00001815
	.4byte 0x021c0014
	.4byte Func_02000860
	.4byte 0x00001815
	.4byte 0x021e0016
	.4byte Func_02000860
	.4byte 0x10008c15
	.4byte 0xffff0008
	.4byte Func_02000770
	.4byte 0x00008c15
	.4byte 0xffff0008
	.4byte Func_02000780
	.4byte 0x00000008
	.4byte 0xffff0000
	.4byte Func_02000770
	.4byte 0x00000009
	.4byte 0xffff0000
	.4byte Func_02000780
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003d9c
Data_02003d9c:
	.4byte 0x0000c602
	.4byte 0xffff0001
	.4byte Func_02000324
	.4byte 0x0000c602
	.4byte 0xffff0002
	.4byte Func_02000324
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x00000001
	.4byte 0xffff0004
	.4byte 0x00000004
	.4byte 0x00000000
	.4byte 0xffff0008
	.4byte Func_02000d44
	.4byte 0x80008a05
	.4byte 0xffff00ff
	.4byte Func_02001368
	.4byte 0x50008a05
	.4byte 0xffff00ff
	.4byte Func_020012b4
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003dfc
Data_02003dfc:
	.4byte 0x0000c602
	.4byte 0xffff0006
	.4byte Func_02000438
	.4byte 0x10008c15
	.4byte 0x02020008
	.4byte Func_020010bc
	.4byte 0x00008c15
	.4byte 0x02020008
	.4byte Func_020010cc
	.4byte 0x10008c15
	.4byte 0x02030009
	.4byte Func_020010bc
	.4byte 0x00008c15
	.4byte 0x02030009
	.4byte Func_020010cc
	.4byte 0x00000008
	.4byte 0x09820000
	.4byte Func_020010bc
	.4byte 0x00000009
	.4byte 0x09820000
	.4byte Func_020010cc
	.4byte 0x00002115
	.4byte 0x02040008
	.4byte Func_020010d8
	.4byte 0x00002115
	.4byte 0x02050009
	.4byte Func_020010f8
	.4byte 0x50001815
	.4byte 0x0210000a
	.4byte Func_02001118
	.4byte 0x50001815
	.4byte 0x0211000b
	.4byte Func_02001128
	.4byte 0x00001815
	.4byte 0x0210000a
	.4byte Func_02000818
	.4byte 0x00001815
	.4byte 0x0211000b
	.4byte Func_02000818
	.4byte 0x00000006
	.4byte 0xffff00c8
	.4byte Func_02001138
	.4byte 0x00000006
	.4byte 0xffff00c9
	.4byte Func_020011f4
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003ebc
Data_02003ebc:
	.4byte 0x00000001
	.4byte 0xffff0005
	.4byte 0x00000005
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x0000c602
	.4byte 0xffff0008
	.4byte Func_02000438
	.4byte 0x00000001
	.4byte 0xffff0009
	.4byte 0x00000009
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02003ef8
Data_02003ef8:
	.4byte 0x00000000
	.4byte 0x0000000a
	.4byte 0x00000001
	.4byte 0x00000003
	.4byte 0x00000000
	.4byte 0x00000001
	.4byte 0x00000026
