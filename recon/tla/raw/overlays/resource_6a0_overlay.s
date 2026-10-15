.syntax unified
	.thumb
	.section .text.x02008194,"ax",%progbits
	.balign 4
	.global Func_02000194
	.thumb_func
Func_02000194:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r0, #18
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #19
	bl Object_GetById
	ldr r3, [r5, #8]
	movs r7, #1
	asrs r6, r3, #20
	ldr r3, [r5, #16]
	mov r8, r0
	negs r7, r7
	asrs r5, r3, #20
	cmp r6, #35
	bne .L_020081d6
	cmp r5, #15
	bne .L_020081d6
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #17
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020081d6
	movs r0, #161
	lsls r0, r0, #4
	bl GameFlag_SetBit
	movs r7, #0
.L_020081d6:
	cmp r6, #31
	bne .L_020081f8
	cmp r5, #16
	bne .L_020081f8
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #19
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020081f8
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #18
	bl GameFlag_SetBit
	movs r7, #2
.L_020081f8:
	cmp r6, #33
	bne .L_0200821a
	cmp r5, #10
	bne .L_0200821a
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #21
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200821a
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #20
	bl GameFlag_SetBit
	movs r7, #4
.L_0200821a:
	mov r2, r8
	ldr r3, [r2, #8]
	asrs r6, r3, #20
	ldr r3, [r2, #16]
	asrs r5, r3, #20
	cmp r6, #35
	bne .L_02008244
	cmp r5, #15
	bne .L_02008244
	movs r0, #161
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008244
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #17
	bl GameFlag_SetBit
	movs r7, #1
.L_02008244:
	cmp r6, #31
	bne .L_02008266
	cmp r5, #16
	bne .L_02008266
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #18
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008266
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #19
	bl GameFlag_SetBit
	movs r7, #3
.L_02008266:
	cmp r6, #33
	bne .L_02008288
	cmp r5, #10
	bne .L_02008288
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #20
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008288
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #21
	bl GameFlag_SetBit
	movs r7, #5
.L_02008288:
	adds r0, r7, #0
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.section .text.x020085a0,"ax",%progbits
	.global Func_020005a0
	.thumb_func
Func_020005a0:
	push {r5, lr}
	ldr r3, .L_020085e4
	adds r5, r0, #0
	adds r4, r3, #0
	movs r2, #0
	ldrsh r3, [r4, r2]
	movs r2, #1
	negs r2, r2
	cmp r3, r2
	beq .L_020085de
	movs r0, #2
	movs r1, #0
.L_020085b8:
	ldrsh r2, [r4, r1]
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_020085d0
	ldrsh r2, [r0, r4]
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r2, r3
	bne .L_020085d0
	movs r0, #1
	b .L_020085e0
.L_020085d0:
	adds r1, #4
	ldrsh r3, [r4, r1]
	movs r2, #1
	negs r2, r2
	adds r0, #4
	cmp r3, r2
	bne .L_020085b8
.L_020085de:
	movs r0, #0
.L_020085e0:
	pop {r5, pc}
	.2byte 0x0000
.L_020085e4:
	.4byte gData_02002c64
	.section .text.x020085e8,"ax",%progbits
	.global Func_020005e8
	.thumb_func
Func_020005e8:
	push {r5, r6, r7, lr}
	adds r6, r0, #0
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #26
	sub sp, #8
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020086a2
	movs r3, #9
	str r3, [sp, #4]
	movs r5, #72
	movs r0, #80
	movs r1, #46
	movs r2, #1
	movs r3, #7
	str r5, [sp, #0]
	bl Func_02002b2c
	movs r3, #16
	str r3, [sp, #4]
	movs r0, #80
	movs r1, #53
	movs r2, #8
	movs r3, #5
	str r5, [sp, #0]
	bl Func_02002b2c
	movs r3, #8
	movs r2, #74
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #78
	movs r1, #47
	movs r2, #1
	movs r3, #1
	bl Func_02002b2c
	movs r0, #128
	movs r1, #192
	lsls r0, r0, #16
	lsls r1, r1, #16
	movs r2, #0
	movs r3, #4
	bl Func_02002b54
	cmp r6, #1
	bne .L_02008682
	movs r7, #0
.L_0200864c:
	movs r6, #0
.L_0200864e:
	adds r0, r6, #0
	adds r0, #10
	bl Object_GetById
	adds r5, r0, #0
	bl Func_020005a0
	cmp r0, #0
	beq .L_02008670
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	ldr r2, .L_02008770
	ldr r3, [r5, #12]
	adds r3, r3, r2
	str r3, [r5, #12]
.L_02008670:
	adds r6, #1
	cmp r6, #5
	ble .L_0200864e
	movs r0, #1
	adds r7, #1
	bl WaitFrames
	cmp r7, #15
	ble .L_0200864c
.L_02008682:
	movs r6, #0
.L_02008684:
	adds r0, r6, #0
	adds r0, #10
	bl Object_GetById
	adds r5, r0, #0
	adds r2, r5, #0
	movs r3, #3
	adds r2, #85
	strb r3, [r2]
	adds r6, #1
	ldr r3, [r5, #20]
	str r3, [r5, #12]
	cmp r6, #5
	ble .L_02008684
	b .L_0200876a
.L_020086a2:
	movs r3, #9
	str r3, [sp, #4]
	movs r5, #72
	movs r0, #72
	movs r1, #46
	movs r2, #1
	movs r3, #7
	str r5, [sp, #0]
	bl Func_02002b2c
	movs r3, #16
	str r3, [sp, #4]
	movs r0, #72
	movs r1, #53
	movs r2, #8
	movs r3, #5
	str r5, [sp, #0]
	bl Func_02002b2c
	movs r3, #8
	movs r2, #74
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #70
	movs r1, #47
	movs r2, #1
	movs r3, #1
	bl Func_02002b2c
	movs r0, #128
	movs r1, #192
	lsls r1, r1, #16
	lsls r0, r0, #16
	movs r2, #0
	movs r3, #6
	bl Func_02002b54
	ldr r3, .L_02008774
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl ObjectMotion_EnableActionAndResetMotion
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl Func_02002ba4
	cmp r6, #1
	bne .L_02008742
	movs r7, #0
.L_0200870a:
	movs r6, #0
.L_0200870c:
	adds r0, r6, #0
	adds r0, #10
	bl Object_GetById
	adds r5, r0, #0
	bl Func_020005a0
	cmp r0, #0
	beq .L_02008730
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r2, #128
	ldr r3, [r5, #12]
	lsls r2, r2, #10
	adds r3, r3, r2
	str r3, [r5, #12]
.L_02008730:
	adds r6, #1
	cmp r6, #5
	ble .L_0200870c
	movs r0, #1
	adds r7, #1
	bl WaitFrames
	cmp r7, #15
	ble .L_0200870a
.L_02008742:
	movs r6, #0
.L_02008744:
	adds r0, r6, #0
	adds r0, #10
	bl Object_GetById
	adds r5, r0, #0
	bl Func_020005a0
	cmp r0, #0
	beq .L_02008764
	adds r2, r5, #0
	movs r3, #4
	adds r2, #85
	strb r3, [r2]
	movs r3, #130
	lsls r3, r3, #14
	str r3, [r5, #12]
.L_02008764:
	adds r6, #1
	cmp r6, #5
	ble .L_02008744
.L_0200876a:
	add sp, #8
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008770:
	.4byte 0xfffe0000
.L_02008774:
	.4byte gPartyState
	.section .text.x020087a4,"ax",%progbits
	.balign 4
	.global Func_020007a4
	.thumb_func
Func_020007a4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r7, r1, #0
	adds r0, r7, #0
	bl Object_GetById
	adds r5, r0, #0
	adds r3, r5, #0
	adds r3, #34
	ldrb r0, [r3]
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	bl Map_GetTerrainHeight
	ldr r3, [r5, #20]
	cmp r3, r0
	beq .L_02008806
	movs r2, #85
	adds r2, r2, r5
	movs r3, #3
	strb r3, [r2]
	movs r0, #2
	mov r8, r2
	bl WaitFrames
	ldr r2, [r5, #12]
	ldr r3, [r5, #20]
	movs r6, #0
	b .L_020087f0
.L_020087e0:
	movs r0, #1
	adds r6, #1
	bl WaitFrames
	cmp r6, #29
	bgt .L_020087fa
	ldr r2, [r5, #12]
	ldr r3, [r5, #20]
.L_020087f0:
	cmp r2, r3
	bgt .L_020087e0
	ldr r3, [r5, #40]
	cmp r3, #0
	bne .L_020087e0
.L_020087fa:
	movs r0, #188
	bl Func_02002c5c
	movs r3, #0
	mov r2, r8
	strb r3, [r2]
.L_02008806:
	adds r3, r7, #0
	subs r3, #18
	cmp r3, #1
	bls .L_02008810
	b .L_0200892a
.L_02008810:
	bl Func_02000194
	movs r3, #1
	adds r5, r0, #0
	negs r3, r3
	cmp r5, r3
	bne .L_02008820
	b .L_020089ce
.L_02008820:
	adds r0, r7, #0
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #30
	bl Battle_WaitMode0
	bl Func_020002f0
	cmp r5, #0
	bne .L_02008840
	movs r0, #167
	bl Func_02002c5c
.L_02008840:
	cmp r5, #1
	bne .L_0200884a
	movs r0, #167
	bl Func_02002c5c
.L_0200884a:
	cmp r5, #3
	bne .L_02008860
	movs r0, #161
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008860
	movs r0, #167
	bl Func_02002c5c
.L_02008860:
	cmp r5, #4
	bne .L_02008878
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #17
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008878
	movs r0, #167
	bl Func_02002c5c
.L_02008878:
	ldr r3, .L_020089d4
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r3, r2
	ldr r0, [r6]
	bl ObjectMotion_EnableActionAndResetMotion
	adds r0, r7, #0
	movs r1, #0
	movs r2, #0
	bl Func_02002ba4
	bl Func_02000290
	cmp r0, #2
	beq .L_0200889a
	b .L_020089ce
.L_0200889a:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #22
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020088aa
	b .L_020089ce
.L_020088aa:
	movs r0, #17
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #22
	bl GameFlag_ClearBit
	movs r0, #10
	bl Battle_WaitMode0
	bl Func_02002b6c
	movs r0, #0
	bl Func_02002c0c
	movs r0, #166
	movs r1, #1
	movs r2, #136
	lsls r2, r2, #16
	movs r3, #1
	negs r1, r1
	lsls r0, r0, #18
	bl Motion_CamBounds
	bl Func_02002bd4
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #151
	bl Func_02002c5c
	adds r0, r5, #0
	movs r1, #3
	bl Func_02002ad4
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #253
	ands r3, r2
	strb r3, [r1]
	adds r3, r5, #0
	adds r3, #34
	ldrb r2, [r3]
	ldr r1, [r5, #16]
	movs r3, #255
	ldr r0, [r5, #8]
	bl Func_02002c3c
	movs r0, #30
	bl Battle_WaitMode0
	ldr r0, [r6]
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	bl Func_02002bd4
	bl Func_02002b74
	b .L_020089ce
.L_0200892a:
	cmp r7, #20
	bne .L_020089ce
	ldr r0, [r5, #8]
	asrs r3, r0, #20
	cmp r3, #8
	bne .L_020089b8
	ldr r3, [r5, #16]
	asrs r3, r3, #20
	cmp r3, #12
	bne .L_020089b8
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #26
	bl GameFlag_SetBit
	movs r0, #20
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #30
	bl Battle_WaitMode0
	movs r0, #167
	bl Func_02002c5c
	movs r0, #1
	bl Func_020005e8
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #23
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020089ce
	movs r0, #9
	bl Object_GetById
	adds r5, r0, #0
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #23
	bl GameFlag_ClearBit
	movs r0, #20
	bl Battle_WaitMode0
	movs r0, #151
	bl Func_02002c5c
	adds r0, r5, #0
	movs r1, #3
	bl Func_02002ad4
	adds r1, r5, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #253
	ands r3, r2
	strb r3, [r1]
	adds r3, r5, #0
	adds r3, #34
	ldrb r2, [r3]
	ldr r0, [r5, #8]
	ldr r1, [r5, #16]
	movs r3, #255
	bl Func_02002c3c
	b .L_020089ce
.L_020089b8:
	ldr r1, [r5, #16]
	movs r2, #0
	movs r3, #8
	bl Func_02002b54
	ldr r0, [r5, #8]
	ldr r1, [r5, #16]
	movs r2, #0
	movs r3, #128
	bl Func_02002c4c
.L_020089ce:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_020089d4:
	.4byte gPartyState
	.section .text.x02008a74,"ax",%progbits
	.global Func_02000a74
	.thumb_func
Func_02000a74:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	movs r0, #161
	lsls r0, r0, #4
	sub sp, #4
	bl GameFlag_Test
	adds r5, r0, #0
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #18
	bl GameFlag_Test
	mov r9, r0
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #20
	bl GameFlag_Test
	mov r10, r0
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #17
	bl GameFlag_Test
	mov r8, r0
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #19
	bl GameFlag_Test
	adds r6, r0, #0
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #21
	bl GameFlag_Test
	add r5, r9
	add r5, r10
	add r5, r8
	adds r5, r5, r6
	cmn r5, r0
	bne .L_02008ad4
	b .L_02008c6c
.L_02008ad4:
	movs r0, #148
	movs r1, #1
	movs r2, #248
	lsls r0, r0, #18
	negs r1, r1
	lsls r2, r2, #16
	movs r3, #1
	bl Motion_CamBounds
	bl Func_02002bd4
	movs r0, #10
	bl Battle_WaitMode0
	movs r0, #161
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008b0e
	movs r2, #0
	movs r3, #16
	str r2, [sp, #0]
	negs r3, r3
	movs r0, #18
	movs r1, #35
	movs r2, #15
	bl Func_020009f8
.L_02008b0e:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #17
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008b2e
	movs r2, #0
	movs r3, #16
	str r2, [sp, #0]
	negs r3, r3
	movs r0, #19
	movs r1, #35
	movs r2, #15
	bl Func_020009f8
.L_02008b2e:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #18
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008b4c
	movs r3, #0
	str r3, [sp, #0]
	movs r0, #18
	movs r1, #31
	movs r2, #16
	movs r3, #16
	bl Func_020009f8
.L_02008b4c:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #19
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008b6a
	movs r3, #0
	str r3, [sp, #0]
	movs r0, #19
	movs r1, #31
	movs r2, #16
	movs r3, #16
	bl Func_020009f8
.L_02008b6a:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #20
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008b8a
	movs r2, #0
	movs r3, #16
	str r2, [sp, #0]
	negs r3, r3
	movs r0, #18
	movs r1, #33
	movs r2, #10
	bl Func_020009f8
.L_02008b8a:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #21
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008baa
	movs r2, #0
	movs r3, #16
	str r2, [sp, #0]
	negs r3, r3
	movs r0, #19
	movs r1, #33
	movs r2, #10
	bl Func_020009f8
.L_02008baa:
	movs r0, #161
	lsls r0, r0, #4
	bl GameFlag_ClearBit
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #17
	bl GameFlag_ClearBit
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #18
	bl GameFlag_ClearBit
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #19
	bl GameFlag_ClearBit
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #20
	bl GameFlag_ClearBit
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #21
	bl GameFlag_ClearBit
	bl Func_020002f0
	movs r0, #2
	bl WaitFrames
	movs r0, #18
	bl Object_GetById
	adds r5, r0, #0
	ldr r2, [r5, #12]
	ldr r3, [r5, #20]
	movs r6, #0
	b .L_02008c0e
.L_02008bfe:
	movs r0, #1
	adds r6, #1
	bl WaitFrames
	cmp r6, #29
	bgt .L_02008c18
	ldr r2, [r5, #12]
	ldr r3, [r5, #20]
.L_02008c0e:
	cmp r2, r3
	bgt .L_02008bfe
	ldr r3, [r5, #40]
	cmp r3, #0
	bne .L_02008bfe
.L_02008c18:
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r0, #19
	bl Object_GetById
	adds r5, r0, #0
	ldr r2, [r5, #12]
	ldr r3, [r5, #20]
	movs r6, #0
	b .L_02008c40
.L_02008c30:
	movs r0, #1
	adds r6, #1
	bl WaitFrames
	cmp r6, #29
	bgt .L_02008c4a
	ldr r2, [r5, #12]
	ldr r3, [r5, #20]
.L_02008c40:
	cmp r2, r3
	bgt .L_02008c30
	ldr r3, [r5, #40]
	cmp r3, #0
	bne .L_02008c30
.L_02008c4a:
	adds r2, r5, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	movs r0, #30
	bl Battle_WaitMode0
	ldr r3, .L_02008c78
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #1
	bl Object_AttachWorkTargetToObject
	bl Func_02002bd4
.L_02008c6c:
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, pc}
.L_02008c78:
	.4byte gPartyState
	.section .text.x02008cec,"ax",%progbits
	.balign 4
	.global Func_02000cec
	.thumb_func
Func_02000cec:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_02008e1c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #12
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #18
	bl Object_GetById
	adds r7, r0, #0
	movs r0, #19
	bl Object_GetById
	ldrh r1, [r6, #6]
	movs r3, #0
	movs r2, #128
	lsls r2, r2, #6
	mov r10, r3
	movs r3, #192
	adds r1, r1, r2
	lsls r3, r3, #8
	ands r1, r3
	ldr r3, [r6, #8]
	mov r5, sp
	str r3, [r5]
	mov r8, r0
	ldr r3, [r6, #12]
	movs r0, #128
	str r3, [r5, #4]
	adds r2, r5, #0
	ldr r3, [r6, #16]
	lsls r0, r0, #13
	str r3, [r5, #8]
	bl Vector_AddPolarOffsetFar
	ldr r3, [r5]
	asrs r6, r3, #20
	ldr r3, [r5, #8]
	asrs r5, r3, #20
	ldr r3, [r7, #12]
	cmp r3, #0
	bne .L_02008d62
	ldr r3, [r7, #8]
	asrs r3, r3, #20
	cmp r6, r3
	bne .L_02008d62
	ldr r3, [r7, #16]
	asrs r3, r3, #20
	cmp r5, r3
	bne .L_02008d62
	movs r3, #1
	mov r10, r3
.L_02008d62:
	mov r2, r8
	ldr r3, [r2, #12]
	cmp r3, #0
	bne .L_02008d7e
	ldr r3, [r2, #8]
	asrs r3, r3, #20
	cmp r6, r3
	bne .L_02008d7e
	ldr r3, [r2, #16]
	asrs r3, r3, #20
	cmp r5, r3
	bne .L_02008d7e
	movs r3, #1
	mov r10, r3
.L_02008d7e:
	mov r2, r10
	cmp r2, #0
	bne .L_02008e06
	cmp r6, #35
	bne .L_02008daa
	cmp r5, #15
	bne .L_02008daa
	movs r0, #161
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008da6
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #17
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008daa
.L_02008da6:
	movs r3, #2
	mov r10, r3
.L_02008daa:
	cmp r6, #31
	bne .L_02008dd2
	cmp r5, #16
	bne .L_02008dd2
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #18
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008dce
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #19
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008dd2
.L_02008dce:
	movs r2, #2
	mov r10, r2
.L_02008dd2:
	cmp r6, #33
	bne .L_02008dfa
	cmp r5, #10
	bne .L_02008dfa
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #20
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008df6
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #21
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008dfa
.L_02008df6:
	movs r3, #2
	mov r10, r3
.L_02008dfa:
	mov r2, r10
	cmp r2, #0
	bne .L_02008e06
	bl Func_02002bec
	b .L_02008e10
.L_02008e06:
	mov r3, r10
	cmp r3, #1
	bne .L_02008e10
	bl Func_02002c24
.L_02008e10:
	add sp, #12
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008e1c:
	.4byte gPartyState
	.section .text.x02008e30,"ax",%progbits
	.global Func_02000e30
	.thumb_func
Func_02000e30:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	lsls r1, r1, #1
	movs r2, #129
	adds r3, r3, r1
	lsls r2, r2, #2
	str r2, [r3]
	ldr r2, .L_02008fb8
	adds r1, #54
	adds r3, r2, r1
	ldrh r3, [r3]
	movs r1, #128
	subs r3, #3
	lsls r3, r3, #16
	lsls r1, r1, #9
	cmp r3, r1
	bhi .L_02008e6e
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r2, r1
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #32
	orrs r3, r2
	strb r3, [r0]
.L_02008e6e:
	movs r0, #8
	bl Object_GetById
	movs r5, #192
	lsls r5, r5, #8
	str r5, [r0, #24]
	movs r0, #8
	bl Object_GetById
	str r5, [r0, #28]
	ldr r0, .L_02008fbc
	bl Func_020019dc
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #27
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008eaa
	movs r1, #136
	movs r2, #150
	movs r0, #16
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl Func_02002ba4
	movs r0, #0
	bl Func_02000060
.L_02008eaa:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #2
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008ebe
	movs r0, #0
	bl Func_02000060
.L_02008ebe:
	bl Func_02001db0
	movs r0, #2
	bl Func_02001ee8
	movs r0, #18
	movs r1, #2
	bl Func_02001eb8
	movs r0, #19
	movs r1, #1
	bl Func_02001eb8
	movs r0, #20
	movs r1, #0
	bl Func_02001eb8
	movs r0, #161
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008f08
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #18
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008f08
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #20
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008f12
.L_02008f08:
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_02002ba4
.L_02008f12:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #17
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008f3c
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #19
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008f3c
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #21
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008f46
.L_02008f3c:
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl Func_02002ba4
.L_02008f46:
	ldr r3, .L_02008fb8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #64
	orrs r3, r2
	strb r3, [r0]
	movs r0, #10
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	movs r5, #128
	orrs r3, r5
	strb r3, [r0]
	movs r0, #11
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #12
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #13
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #14
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r3, r5
	strb r3, [r0]
	movs r0, #15
	bl Object_GetById
	adds r0, #35
	ldrb r3, [r0]
	orrs r5, r3
	strb r5, [r0]
	movs r0, #0
	pop {r5, pc}
.L_02008fb8:
	.4byte gPartyState
.L_02008fbc:
	.4byte gData_020031c4
	.section .text.x0200915a,"ax",%progbits
	.2byte 0x0000
	.section .text.x02009168,"ax",%progbits
	.global Func_02001168
	.thumb_func
Func_02001168:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_02009214
	sub sp, #32
	ldr r0, [r3]
	cmp r0, #0
	bge .L_0200917a
	adds r0, #3
.L_0200917a:
	asrs r0, r0, #2
	movs r1, #5
	bl __modsi3
	ldr r3, .L_02009218
	mov r8, r0
	ldr r3, [r3]
	cmp r3, #0
	beq .L_020091ce
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r0, #179
	lsls r0, r0, #1
	adds r3, r2, r0
	ldrh r1, [r3]
	movs r0, #0
	ldrsh r3, [r3, r0]
	cmp r3, #0
	beq .L_020091ae
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #255
	ands r3, r1
	cmp r3, #153
	bne .L_0200920a
.L_020091ae:
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #164
	adds r3, r2, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_0200920a
	movs r0, #175
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_0200920a
.L_020091ce:
	movs r5, #0
	movs r6, #4
.L_020091d2:
	mov r2, r8
	adds r0, r2, r5
	movs r1, #5
	mov r7, sp
	bl __modsi3
	ldr r3, .L_0200921c
	lsls r0, r0, #1
	ldrh r3, [r3, r6]
	adds r5, #1
	strh r3, [r7, r0]
	adds r6, #2
	cmp r5, #4
	ble .L_020091d2
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r7, #0
	ldr r1, .L_02009220
	adds r2, #2
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, .L_02009214
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
.L_0200920a:
	add sp, #32
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009214:
	.4byte gData_02003590
.L_02009218:
	.4byte gData_02003594
.L_0200921c:
	.4byte gData_020035bc
.L_02009220:
	.4byte 0x05000184
	.section .text.x020092fc,"ax",%progbits
	.global Func_020012fc
	.thumb_func
Func_020012fc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #133
	mov r10, r3
	ldr r3, .L_02009408
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r6, r0, #0
	ldr r7, [r6, #104]
	bl Func_02002b6c
	movs r0, #0
	bl Func_02002c0c
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #70
	add r3, r10
	movs r5, #0
	strh r5, [r3]
	movs r3, #85
	adds r3, r3, r6
	mov r9, r3
	mov r2, r9
	movs r3, #4
	strb r3, [r2]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r3, #99
	adds r3, r3, r7
	mov r8, r3
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0200939c
.L_02009356:
	ldr r3, [r7, #8]
	ldr r2, .L_0200940c
	str r3, [r6, #8]
	ldr r3, [r7, #12]
	adds r3, r3, r5
	str r3, [r6, #12]
	ldr r3, [r7, #16]
	str r3, [r6, #16]
	cmp r5, r2
	bgt .L_02009372
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	adds r5, r5, r3
.L_02009372:
	ldr r3, .L_02009410
	adds r1, r6, #0
	ldr r2, [r3]
	ldrb r3, [r3]
	adds r1, #35
	lsls r3, r3, #12
	strh r3, [r6, #6]
	movs r3, #1
	ands r2, r3
	movs r3, #2
	lsls r3, r2
	ldrb r2, [r1]
	movs r0, #1
	eors r3, r2
	strb r3, [r1]
	bl WaitFrames
	mov r2, r8
	ldrb r3, [r2]
	cmp r3, #0
	bne .L_02009356
.L_0200939c:
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #68
	movs r2, #1
	add r3, r10
	strh r2, [r3]
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #70
	add r3, r10
	strh r2, [r3]
	ldr r3, [r7, #8]
	ldrh r1, [r7, #6]
	subs r2, #3
	asrs r3, r3, #19
	ands r3, r2
	asrs r1, r1, #13
	adds r3, r3, r1
	subs r3, #1
	lsls r3, r3, #19
	str r3, [r6, #8]
	ldr r3, [r7, #16]
	ldr r0, .L_02009404
	asrs r3, r3, #19
	ands r3, r2
	movs r2, #2
	ands r1, r2
	subs r3, r3, r1
	adds r3, #1
	lsls r3, r3, #19
	str r3, [r6, #16]
	movs r3, #192
	lsls r3, r3, #11
	str r3, [r6, #40]
	adds r3, r6, #0
	adds r3, #35
	strb r0, [r3]
	mov r2, r9
	movs r3, #3
	strb r3, [r2]
	adds r0, r6, #0
	movs r1, #1
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	b .L_02009414
.L_02009404:
	.4byte 0x00000001
.L_02009408:
	.4byte gPartyState
.L_0200940c:
	.4byte 0x0003ffff
.L_02009410:
	.4byte gFrameCount
.L_02009414:
	bl Motion_CamBounds
	bl Func_02002bdc
	movs r3, #128
	adds r7, r0, #0
	lsls r3, r3, #12
	str r3, [r7, #48]
	movs r3, #128
	ldr r5, .L_0200945c
	lsls r3, r3, #9
	str r3, [r7, #52]
	adds r3, r7, #0
	adds r3, #85
	strb r5, [r3]
	movs r0, #0
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	bl Map_GetTerrainHeight
	ldr r3, [r6, #16]
	adds r2, r0, #0
	ldr r1, [r6, #8]
	adds r0, r7, #0
	bl Func_02002b04
	ldr r3, [r6, #20]
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	ldr r2, [r6, #12]
	movs r5, #0
	cmp r2, r3
	ble .L_0200947a
	b .L_02009460
	.2byte 0x0000
.L_0200945c:
	.4byte 0x00000000
.L_02009460:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #59
	bgt .L_0200947a
	ldr r3, [r6, #20]
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	ldr r2, [r6, #12]
	cmp r2, r3
	bgt .L_02009460
.L_0200947a:
	movs r0, #127
	bl Func_02002c5c
	ldr r3, [r6, #40]
	movs r5, #0
	cmp r3, #0
	beq .L_0200949a
.L_02009488:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #59
	bgt .L_0200949a
	ldr r3, [r6, #40]
	cmp r3, #0
	bne .L_02009488
.L_0200949a:
	adds r0, r7, #0
	bl Func_02002b0c
	ldr r5, .L_020094e4
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	movs r1, #1
	ldr r0, [r5]
	bl Object_AttachWorkTargetToObject
	movs r2, #208
	lsls r2, r2, #4
	adds r2, #70
	add r2, r10
	movs r3, #1
	strh r3, [r2]
	movs r3, #170
	lsls r3, r3, #1
	movs r6, #0
	add r3, r10
	strh r6, [r3]
	bl Func_02002b74
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_020094e4:
	.4byte gPartyState
	.section .text.x02009514,"ax",%progbits
	.global Func_02001514
	.thumb_func
Func_02001514:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #133
	mov r10, r3
	ldr r3, .L_0200957c
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r6, r0, #0
	ldr r0, [r3]
	sub sp, #12
	bl Object_GetById
	movs r3, #179
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	adds r7, r0, #0
	cmp r3, #0
	bne .L_02009572
	movs r3, #173
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_02009572
	movs r3, #175
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_02009572
	movs r3, #180
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_02009580
.L_02009572:
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_ApplyValueToChildren
	b .L_020096be
.L_0200957c:
	.4byte gPartyState
.L_02009580:
	adds r0, r6, #0
	movs r1, #16
	bl ObjectDispatch_ApplyValueToChildren
	adds r3, r6, #0
	adds r3, #100
	ldrh r2, [r3]
	adds r2, #1
	strh r2, [r3]
	movs r3, #31
	ands r3, r2
	cmp r3, #31
	bne .L_020095a0
	movs r0, #231
	bl Func_02002c5c
.L_020095a0:
	ldr r3, [r7, #80]
	ldr r0, [r6, #80]
	ldrb r3, [r3, #9]
	ldrb r1, [r0, #9]
	movs r2, #12
	ands r2, r3
	movs r3, #13
	negs r3, r3
	ands r3, r1
	orrs r3, r2
	strb r3, [r0, #9]
	movs r2, #2
	ldr r0, [r6, #8]
	ldr r1, [r6, #16]
	bl Func_02002c34
	cmp r0, #255
	beq .L_020096a2
	ldr r3, [r6, #8]
	mov r5, sp
	str r3, [r5]
	adds r0, r5, #0
	ldr r3, [r6, #12]
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	bl Func_02002c14
	ldr r5, [r5]
	movs r3, #136
	lsls r3, r3, #17
	cmp r5, r3
	bgt .L_020096a2
	ldr r2, .L_02009690
	cmp r5, r2
	blt .L_020096a2
	movs r3, #98
	adds r3, r3, r6
	mov r9, r3
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_02009668
	ldr r2, [r7, #12]
	ldr r3, [r6, #12]
	subs r5, r2, r3
	cmp r5, #0
	bge .L_02009600
	subs r5, r3, r2
.L_02009600:
	adds r0, r7, #0
	adds r1, r6, #0
	movs r2, #0
	adds r0, #8
	adds r1, #8
	mov r8, r2
	bl Func_020014e8
	cmp r0, #12
	bgt .L_02009620
	movs r3, #192
	lsls r3, r3, #12
	cmp r5, r3
	bge .L_02009620
	movs r2, #1
	mov r8, r2
.L_02009620:
	mov r3, r8
	cmp r3, #0
	beq .L_02009668
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009668
	ldrh r3, [r6, #6]
	str r6, [r7, #104]
	strh r3, [r7, #6]
	adds r1, r7, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	movs r2, #181
	lsls r2, r2, #1
	strb r3, [r1]
	add r2, r10
	movs r3, #200
	strh r3, [r2]
	ldr r3, .L_02009694
	movs r2, #128
	ldr r0, .L_0200968c
	lsls r2, r2, #2
	adds r2, #18
	adds r3, r3, r2
	strb r0, [r3]
	mov r2, r9
	movs r3, #1
	strb r3, [r2]
	adds r2, r6, #0
	adds r2, #99
	strb r3, [r2]
.L_02009668:
	ldrh r0, [r6, #6]
	bl Math_Cosine
	ldr r1, [r6, #48]
	ldr r5, .L_02009698
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r6, #8]
	adds r3, r3, r0
	ldrh r0, [r6, #6]
	str r3, [r6, #8]
	bl Math_Sine
	ldr r1, [r6, #48]
	mov lr, r5
	.2byte 0xf800
	ldr r3, [r6, #16]
	b .L_0200969c
.L_0200968c:
	.4byte 0x00000000
.L_02009690:
	.4byte 0xffe00000
.L_02009694:
	.4byte gPartyState
.L_02009698:
	.4byte IwramMulQ16
.L_0200969c:
	adds r3, r3, r0
	str r3, [r6, #16]
	b .L_020096be
.L_020096a2:
	adds r3, r6, #0
	adds r3, #99
	movs r5, #0
	strb r5, [r3]
	ldr r1, .L_020096cc
	adds r0, r6, #0
	str r5, [r6, #108]
	bl Func_02002ae4
	movs r0, #228
	bl Func_02002c5c
	ldr r3, .L_020096d0
	str r5, [r3]
.L_020096be:
	add sp, #12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020096cc:
	.4byte gData_02003598
.L_020096d0:
	.4byte Data_020035b8
	.section .text.x020096d4,"ax",%progbits
	.global Func_020016d4
	.thumb_func
Func_020016d4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	movs r0, #222
	sub sp, #68
	bl Func_02002c5c
	ldrh r0, [r5, #6]
	bl Math_Cosine
	adds r1, r0, #0
	movs r0, #128
	ldr r6, .L_02009788
	lsls r0, r0, #12
	mov lr, r6
	.2byte 0xf800
	ldr r3, [r5, #8]
	add r2, sp, #56
	adds r3, r3, r0
	str r3, [r2]
	mov r8, r2
	ldrh r0, [r5, #6]
	bl Math_Sine
	adds r1, r0, #0
	movs r0, #128
	lsls r0, r0, #12
	mov lr, r6
	.2byte 0xf800
	ldr r3, [r5, #16]
	mov r2, r8
	adds r3, r3, r0
	str r3, [r2, #8]
	movs r0, #140
	ldr r1, [r2]
	lsls r0, r0, #1
	ldr r2, [r5, #12]
	bl Func_02002aec
	movs r1, #2
	adds r7, r0, #0
	bl Func_02002ad4
	adds r0, r7, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	adds r3, r7, #0
	movs r6, #0
	adds r3, #85
	strb r6, [r3]
	adds r1, r7, #0
	adds r1, #35
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	ldr r2, .L_02009784
	ldrh r3, [r5, #6]
	add r4, sp, #16
	strh r3, [r7, #6]
	adds r3, r7, #0
	adds r3, #100
	strh r6, [r3]
	subs r3, #2
	strb r2, [r3]
	adds r3, #1
	strb r2, [r3]
	ldr r3, .L_0200978c
	str r3, [r7, #108]
	movs r3, #192
	lsls r3, r3, #9
	str r3, [r7, #48]
	movs r3, #1
	str r3, [r4]
	movs r3, #7
	str r3, [r4, #4]
	mov r3, r8
	ldr r0, [r3]
	ldr r2, [r3, #8]
	ldr r3, .L_02009790
	ldr r1, [r5, #12]
	adds r2, r2, r3
	movs r3, #192
	lsls r3, r3, #10
	str r3, [sp, #8]
	b .L_02009794
.L_02009784:
	.4byte 0x00000000
.L_02009788:
	.4byte IwramMulQ16
.L_0200978c:
	.4byte Func_02001514
.L_02009790:
	.4byte 0xfffa0000
.L_02009794:
	movs r3, #0
	str r6, [sp, #0]
	str r6, [sp, #4]
	str r4, [sp, #12]
	bl Func_02001f74
	adds r0, r7, #0
	add sp, #68
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x020097ac,"ax",%progbits
	.global Func_020017ac
	.thumb_func
Func_020017ac:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_02009848
	sub sp, #56
	ldr r7, [r3]
	movs r3, #7
	ands r7, r3
	mov r10, r0
	cmp r7, #0
	bne .L_0200983a
	add r6, sp, #16
	movs r3, #3
	str r3, [r6]
	movs r3, #179
	lsls r3, r3, #8
	adds r3, #51
	str r3, [r6, #8]
	str r3, [r6, #12]
	movs r3, #14
	str r3, [r6, #4]
	bl Random16Far
	mov r2, r10
	lsls r3, r0, #3
	ldr r2, [r2, #8]
	adds r3, r3, r0
	lsrs r3, r3, #16
	subs r3, #4
	lsls r3, r3, #16
	mov r8, r2
	add r8, r3
	bl Random16Far
	lsls r3, r0, #2
	adds r3, r3, r0
	lsls r3, r3, #2
	lsrs r3, r3, #16
	movs r2, #32
	subs r2, r2, r3
	mov r3, r10
	ldr r5, [r3, #12]
	lsls r2, r2, #16
	adds r5, r5, r2
	bl Random16Far
	adds r3, r0, #0
	lsls r0, r3, #2
	adds r0, r0, r3
	lsrs r0, r0, #16
	movs r2, #160
	lsls r2, r2, #11
	lsls r0, r0, #16
	adds r0, r0, r2
	movs r1, #10
	bl __divsi3
	mov r3, r10
	ldr r2, [r3, #16]
	movs r3, #176
	lsls r3, r3, #12
	str r0, [sp, #0]
	str r3, [sp, #8]
	mov r0, r8
	adds r1, r5, #0
	movs r3, #0
	str r7, [sp, #4]
	str r6, [sp, #12]
	bl Func_02001f74
.L_0200983a:
	movs r0, #0
	add sp, #56
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009848:
	.4byte gFrameCount
	.section .text.x0200984c,"ax",%progbits
	.global Func_0200184c
	.thumb_func
Func_0200184c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r0, #9
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #10
	bl Object_GetById
	adds r7, r0, #0
	movs r0, #23
	bl Object_GetById
	adds r5, r0, #0
	ldr r2, [r5, #80]
	movs r1, #128
	mov r8, r2
	movs r2, #248
	movs r0, #24
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02002ba4
	movs r1, #128
	movs r2, #248
	movs r0, #23
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02002ba4
	movs r1, #236
	movs r2, #128
	movs r0, #9
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02002ba4
	movs r1, #138
	movs r2, #128
	lsls r2, r2, #17
	movs r0, #10
	lsls r1, r1, #18
	bl Func_02002ba4
	movs r0, #24
	bl Object_GetById
	movs r3, #85
	movs r2, #0
	adds r3, r3, r5
	str r2, [r0, #24]
	strb r2, [r3]
	mov r11, r3
	ldr r3, [r5, #20]
	movs r0, #160
	str r3, [r5, #12]
	movs r3, #85
	adds r3, r3, r6
	strb r2, [r3]
	mov r9, r3
	ldr r3, [r6, #20]
	lsls r0, r0, #4
	str r3, [r6, #12]
	movs r3, #85
	adds r3, r3, r7
	strb r2, [r3]
	mov r10, r3
	ldr r3, [r7, #20]
	adds r0, #10
	str r3, [r7, #12]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009928
	ldr r3, [r6, #12]
	ldr r2, .L_020099c0
	movs r0, #9
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r2, .L_020099c4
	ldr r3, [r5, #12]
	adds r3, r3, r2
	str r3, [r5, #12]
	ldr r2, .L_020099c8
	ldr r3, [r5, #8]
	adds r3, r3, r2
	str r3, [r5, #8]
	mov r2, r8
	ldrh r3, [r2, #18]
	movs r2, #128
	lsls r2, r2, #4
	adds r3, r3, r2
	mov r2, r8
	strh r3, [r2, #18]
	bl Object_GetById
	movs r1, #4
	bl Animation_ApplyChildValues
	movs r0, #11
	bl Object_GetById
	movs r1, #4
	bl Animation_ApplyChildValues
.L_02009928:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #11
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009972
	ldr r3, [r7, #12]
	ldr r2, .L_020099c0
	movs r0, #10
	adds r3, r3, r2
	str r3, [r7, #12]
	ldr r2, .L_020099c4
	ldr r3, [r5, #12]
	adds r3, r3, r2
	str r3, [r5, #12]
	ldr r2, .L_020099cc
	ldr r3, [r5, #8]
	adds r3, r3, r2
	str r3, [r5, #8]
	mov r2, r8
	ldrh r3, [r2, #18]
	ldr r2, .L_020099d0
	adds r3, r3, r2
	mov r2, r8
	strh r3, [r2, #18]
	bl Object_GetById
	movs r1, #4
	bl Animation_ApplyChildValues
	movs r0, #12
	bl Object_GetById
	movs r1, #4
	bl Animation_ApplyChildValues
.L_02009972:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #10
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020099b2
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #11
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020099b2
	ldr r3, [r6, #12]
	ldr r2, .L_020099d4
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r7, #12]
	adds r3, r3, r2
	str r3, [r7, #12]
	ldr r2, .L_020099d8
	ldr r3, [r5, #12]
	adds r3, r3, r2
	str r3, [r5, #12]
	mov r2, r9
	movs r3, #4
	strb r3, [r2]
	mov r2, r10
	strb r3, [r2]
	mov r2, r11
	strb r3, [r2]
.L_020099b2:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020099c0:
	.4byte 0x00066640
.L_020099c4:
	.4byte 0x0001eb80
.L_020099c8:
	.4byte 0xfffd70c0
.L_020099cc:
	.4byte 0x00028f40
.L_020099d0:
	.4byte 0xfffff800
.L_020099d4:
	.4byte 0x00199900
.L_020099d8:
	.4byte 0x001b8480
	.section .text.x020099dc,"ax",%progbits
	.global Func_020019dc
	.thumb_func
Func_020019dc:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r2, #255
	ldrh r3, [r0]
	lsls r2, r2, #8
	adds r2, #255
	sub sp, #4
	cmp r3, r2
	beq .L_02009a40
	adds r7, r0, #0
.L_020099f2:
	ldrh r3, [r7]
	mov r8, r3
	mov r0, r8
	bl Object_GetById
	adds r5, r0, #0
	adds r1, r5, #0
	adds r1, #89
	movs r2, #2
	ldrsh r6, [r7, r2]
	ldrb r2, [r1]
	movs r3, #4
	ldrsh r4, [r7, r3]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	movs r1, #0
	str r4, [sp, #0]
	bl ObjectDispatch_SetSingleChildFlags
	ldr r4, [sp, #0]
	lsls r6, r6, #16
	lsls r4, r4, #16
	lsrs r4, r4, #16
	lsrs r6, r6, #16
	adds r5, #34
	ldrb r3, [r5]
	adds r2, r4, #0
	mov r0, r8
	adds r1, r6, #0
	adds r7, #6
	bl Func_02001acc
	movs r2, #255
	ldrh r3, [r7]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_020099f2
.L_02009a40:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.section .text.x02009a48,"ax",%progbits
	.global Func_02001a48
	.thumb_func
Func_02001a48:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	adds r5, r0, #0
	mov r9, r1
	mov r10, r2
	movs r1, #255
	ldr r2, [r3]
	b .L_02009ab4
.L_02009a64:
	ldrh r3, [r5]
	movs r1, #26
	ldrsh r7, [r2, r1]
	cmp r7, r3
	bne .L_02009ab0
	adds r0, r7, #0
	bl Object_GetById
	adds r5, #2
	ldrh r2, [r5]
	mov r3, r10
	adds r6, r0, #0
	mov r8, r2
	ldrh r5, [r5, #2]
	cmp r3, #7
	bgt .L_02009a8c
	ldr r3, [r6, #28]
	ldr r1, .L_02009ac8
	adds r3, r3, r1
	str r3, [r6, #28]
.L_02009a8c:
	mov r2, r9
	cmp r2, #1
	bne .L_02009abe
	adds r0, r5, #0
	bl GameFlag_SetBit
	adds r3, r6, #0
	adds r3, #34
	ldrb r3, [r3]
	adds r0, r7, #0
	mov r1, r8
	adds r2, r5, #0
	bl Func_02001acc
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r6, #28]
	b .L_02009abe
.L_02009ab0:
	adds r5, #6
	movs r1, #255
.L_02009ab4:
	ldrh r3, [r5]
	lsls r1, r1, #8
	adds r1, #255
	cmp r3, r1
	bne .L_02009a64
.L_02009abe:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_02009ac8:
	.4byte 0xffffe100
	.section .text.x02009b74,"ax",%progbits
	.global Func_02001b74
	.thumb_func
Func_02001b74:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	ldr r3, .L_02009d08
	adds r2, r1, #0
	adds r2, #228
	ldr r0, [r2]
	ldr r2, [r2, #4]
	ands r0, r3
	ands r2, r3
	ldr r3, .L_02009d0c
	mov r10, r0
	movs r0, #0
	ldrsh r3, [r3, r0]
	mov r8, r2
	ldr r2, .L_02009d10
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	sub sp, #8
	lsrs r3, r3, #5
	str r3, [sp, #4]
	ldr r6, .L_02009d14
	ldr r3, [r1]
	movs r1, #0
	ldr r3, [r3, #4]
	mov r9, r1
	str r3, [sp, #0]
	ldr r3, .L_02009d18
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r9, r3
	blt .L_02009bc6
	b .L_02009cfa
.L_02009bc6:
	ldr r2, .L_02009d1c
	mov r0, r9
	lsls r3, r0, #2
	ldr r5, [r2, r3]
	cmp r5, #0
	bne .L_02009bd4
	b .L_02009cea
.L_02009bd4:
	ldr r3, [r5, #8]
	cmp r3, #0
	bne .L_02009bdc
	b .L_02009cea
.L_02009bdc:
	mov r1, r10
	subs r0, r3, r1
	ldr r2, [sp, #0]
	ldr r3, [r5, #12]
	movs r1, #128
	subs r3, r3, r2
	ldr r2, [r5, #16]
	lsls r1, r1, #12
	adds r3, r3, r1
	mov r1, r8
	subs r2, r2, r1
	ldr r1, [sp, #0]
	subs r2, r2, r1
	subs r4, r2, r3
	adds r3, r3, r2
	asrs r3, r3, #16
	adds r3, #58
	mov r11, r3
	ldr r3, .L_02009d20
	movs r1, #0
	ldrsh r2, [r3, r1]
	adds r3, r5, #0
	mov r12, r2
	asrs r1, r0, #16
	mov r0, r12
	adds r3, #100
	asrs r2, r4, #16
	cmp r0, #0
	bne .L_02009c52
	movs r0, #0
	ldrsh r7, [r3, r0]
	adds r0, r1, #0
	adds r3, r1, #7
	movs r1, #167
	adds r4, r2, #0
	lsls r1, r1, #1
	subs r0, #8
	subs r4, #16
	cmp r3, r1
	bhi .L_02009cea
	movs r2, #16
	negs r2, r2
	cmp r4, r2
	ble .L_02009cea
	cmp r4, #239
	bgt .L_02009cea
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r0, r3
	movs r3, #255
	adds r1, r6, #0
	ands r4, r3
	mov r3, r12
	stmia r1!, {r3}
	lsls r3, r0, #16
	orrs r4, r3
	ldr r3, .L_02009d24
	b .L_02009c8e
.L_02009c52:
	movs r0, #0
	ldrsh r7, [r3, r0]
	adds r0, r1, #0
	adds r3, r1, #0
	movs r1, #175
	adds r4, r2, #0
	adds r3, #23
	lsls r1, r1, #1
	subs r0, #8
	subs r4, #64
	cmp r3, r1
	bhi .L_02009cea
	movs r2, #64
	negs r2, r2
	cmp r4, r2
	ble .L_02009cea
	cmp r4, #175
	bgt .L_02009cea
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r0, r3
	movs r3, #255
	adds r1, r6, #0
	ands r4, r3
	movs r3, #0
	stmia r1!, {r3}
	lsls r3, r0, #16
	orrs r4, r3
	ldr r3, .L_02009d28
.L_02009c8e:
	movs r2, #128
	orrs r4, r3
	stmia r1!, {r4}
	ldr r0, [sp, #4]
	lsls r3, r7, #3
	adds r3, r0, r3
	lsls r2, r2, #4
	orrs r3, r2
	str r3, [r1]
	ldr r3, .L_02009d2c
	movs r0, #1
	ldrh r2, [r3]
	movs r1, #0
	ldrsh r3, [r3, r1]
	negs r0, r0
	cmp r3, r0
	bne .L_02009ccc
	adds r0, r5, #0
	bl Func_02002c2c
	movs r3, #3
	ands r0, r3
	movs r1, #13
	ldrb r3, [r6, #9]
	negs r1, r1
	adds r2, r1, #0
	lsls r0, r0, #2
	ands r3, r2
	orrs r3, r0
	strb r3, [r6, #9]
	b .L_02009ce0
.L_02009ccc:
	movs r3, #3
	ands r3, r2
	movs r0, #13
	ldrb r2, [r6, #9]
	negs r0, r0
	adds r1, r0, #0
	lsls r3, r3, #2
	ands r2, r1
	orrs r2, r3
	strb r2, [r6, #9]
.L_02009ce0:
	adds r0, r6, #0
	mov r1, r11
	bl Func_02002aa4
	adds r6, #12
.L_02009cea:
	ldr r3, .L_02009d18
	movs r1, #1
	movs r2, #0
	ldrsh r3, [r3, r2]
	add r9, r1
	cmp r9, r3
	bge .L_02009cfa
	b .L_02009bc6
.L_02009cfa:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02009d08:
	.4byte 0xffff0000
.L_02009d0c:
	.4byte gData_020035fc
.L_02009d10:
	.4byte gResourceTableEntries
.L_02009d14:
	.4byte Data_02003640
.L_02009d18:
	.4byte gData_020035fe
.L_02009d1c:
	.4byte gData_02003600
.L_02009d20:
	.4byte gData_02003700
.L_02009d24:
	.4byte 0x40002000
.L_02009d28:
	.4byte 0xc000a000
.L_02009d2c:
	.4byte gData_02003702
	.section .text.x02009f3a,"ax",%progbits
	.2byte 0x0000
	.section .text.x02009f72,"ax",%progbits
	.2byte 0x0000
	.section .text.x02009f74,"ax",%progbits
	.global Func_02001f74
	.thumb_func
Func_02001f74:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r11, r3
	ldr r3, .L_0200a12c
	sub sp, #4
	mov r10, r2
	movs r2, #133
	lsls r2, r2, #2
	adds r6, r1, #0
	ldr r1, [sp, #44]
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	mov r8, r1
	ldr r7, [sp, #48]
	bl Object_GetById
	movs r3, #128
	lsls r3, r3, #13
	mov r1, r8
	ands r3, r1
	mov r9, r0
	cmp r3, #0
	beq .L_02009fbc
	cmp r7, #0
	beq .L_02009fbc
	movs r2, #24
	ldrsh r0, [r7, r2]
	adds r1, r5, #0
	adds r2, r6, #0
	b .L_02009fc4
.L_02009fbc:
	movs r0, #30
	adds r2, r6, #0
	adds r0, #255
	adds r1, r5, #0
.L_02009fc4:
	mov r3, r10
	bl Func_02002aec
	adds r6, r0, #0
	cmp r6, #0
	bne .L_02009fd2
	b .L_0200a11e
.L_02009fd2:
	ldr r3, [r6, #80]
	mov r1, r8
	movs r5, #15
	adds r1, #1
	ands r1, r5
	adds r0, r6, #0
	str r3, [sp, #0]
	bl Func_02002ad4
	ldr r2, .L_0200a130
	mov r3, r8
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r10, r3
	bl Func_02002ae4
	adds r3, r6, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	ldr r3, .L_0200a134
	mov r1, r9
	str r3, [r6, #108]
	mov r3, r11
	str r3, [r6, #68]
	ldr r3, [sp, #36]
	adds r0, r6, #0
	str r3, [r6, #72]
	ldr r3, [sp, #40]
	str r3, [r6, #76]
	ldr r3, [r1, #80]
	ldrb r1, [r3, #9]
	lsls r1, r1, #28
	lsrs r1, r1, #30
	bl Object_SetSpritePriority
	movs r2, #100
	adds r2, r2, r6
	mov r9, r2
	mov r3, r9
	str r5, [r6, #48]
	str r5, [r6, #52]
	strh r5, [r3]
	ldr r3, .L_0200a138
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_0200a11e
	cmp r7, #0
	beq .L_0200a11e
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r1
	cmp r3, #0
	beq .L_0200a054
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl Object_SetPartAttribute
.L_0200a054:
	movs r3, #128
	lsls r3, r3, #10
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_0200a074
	adds r1, r6, #0
	adds r1, #35
	ldrb r3, [r1]
	movs r2, #254
	ands r2, r3
	strb r2, [r1]
	ldr r1, [r7]
	adds r0, r6, #0
	bl Object_SetSpritePriority
.L_0200a074:
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r8
	ands r2, r3
	cmp r2, #0
	beq .L_0200a088
	ldr r3, [r7, #8]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
.L_0200a088:
	movs r3, #128
	lsls r3, r3, #11
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_0200a0ce
	ldr r3, .L_0200a130
	mov r1, r10
	ldr r5, [r3, r1]
	ldr r3, [r7, #16]
	ldr r1, [r5, #12]
	cmp r2, #0
	beq .L_0200a0b6
	ldr r0, [r6, #24]
	subs r0, r3, r0
	bl __divsi3
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_0200a0c8
.L_0200a0b6:
	ldr r2, .L_0200a138
	adds r0, r3, r2
	bl __divsi3
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, .L_0200a138
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_0200a0c8:
	bl __divsi3
	str r0, [r6, #52]
.L_0200a0ce:
	movs r3, #128
	lsls r3, r3, #14
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_0200a0ea
	adds r0, r6, #0
	movs r1, #1
	bl Func_02002ad4
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_02002ae4
.L_0200a0ea:
	movs r3, #128
	lsls r3, r3, #15
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_0200a0fc
	ldrh r3, [r7, #32]
	ldr r1, [sp, #0]
	strh r3, [r1, #18]
.L_0200a0fc:
	movs r3, #128
	lsls r3, r3, #16
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_0200a10e
	ldrh r3, [r7, #34]
	mov r1, r9
	strh r3, [r1]
.L_0200a10e:
	movs r3, #128
	lsls r3, r3, #17
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_0200a11e
	ldr r3, [r7, #36]
	str r3, [r6, #108]
.L_0200a11e:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200a12c:
	.4byte gPartyState
.L_0200a130:
	.4byte gData_020035ac
.L_0200a134:
	.4byte BattleFx_UpdateObjectMotionScaleAndLinkedAngle
.L_0200a138:
	.4byte 0xffff0000
	.section .text.x0200a13c,"ax",%progbits
	.global Func_0200213c
	.thumb_func
Func_0200213c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r4, .L_0200a254
	movs r1, #1
	movs r0, #12
	ldrsh r3, [r4, r0]
	negs r1, r1
	sub sp, #4
	cmp r3, r1
	beq .L_0200a248
	lsls r3, r3, #3
	adds r3, r3, r4
	adds r3, #32
	mov r8, r3
	ldr r3, .L_0200a258
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	str r4, [sp, #0]
	bl Object_GetById
	mov r1, r8
	ldr r3, [r0, #8]
	movs r5, #0
	ldrsh r2, [r1, r5]
	asrs r3, r3, #20
	ldr r4, [sp, #0]
	cmp r3, r2
	bne .L_0200a188
	ldr r3, [r0, #16]
	movs r5, #2
	ldrsh r2, [r1, r5]
	asrs r3, r3, #20
	cmp r3, r2
	beq .L_0200a190
.L_0200a188:
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r4, #12]
.L_0200a190:
	movs r0, #12
	ldrsh r3, [r4, r0]
	movs r2, #1
	negs r2, r2
	ldr r1, .L_0200a25c
	cmp r3, r2
	beq .L_0200a248
	movs r5, #14
	ldrsh r3, [r4, r5]
	cmp r3, #0
	beq .L_0200a248
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #32]
	str r4, [sp, #0]
	adds r3, r2, #0
	adds r3, #228
	ldr r0, [r3]
	ldr r5, [r3, #4]
	ldr r3, [r2]
	ands r0, r1
	ands r5, r1
	ldr r6, [r3, #4]
	movs r1, #16
	ldrsh r3, [r4, r1]
	ldr r2, .L_0200a260
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	lsrs r3, r3, #5
	mov r10, r3
	mov r3, r8
	movs r2, #0
	ldrsh r1, [r3, r2]
	lsls r1, r1, #20
	subs r7, r1, r0
	movs r0, #2
	ldrsh r2, [r3, r0]
	movs r0, #0
	lsls r2, r2, #20
	bl Map_GetTerrainHeight
	mov r2, r8
	movs r1, #2
	ldrsh r3, [r2, r1]
	subs r0, r0, r6
	lsls r3, r3, #20
	subs r3, r3, r5
	subs r3, r3, r6
	subs r2, r3, r0
	asrs r7, r7, #16
	adds r0, r0, r3
	asrs r0, r0, #16
	adds r3, r7, #0
	movs r5, #167
	asrs r2, r2, #16
	adds r1, r0, #0
	adds r3, #15
	lsls r5, r5, #1
	adds r2, #14
	adds r1, #58
	ldr r4, [sp, #0]
	cmp r3, r5
	bhi .L_0200a248
	movs r0, #15
	negs r0, r0
	cmp r2, r0
	blt .L_0200a248
	cmp r2, #239
	bgt .L_0200a248
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r7, r3
	movs r3, #255
	ands r2, r3
	movs r3, #0
	str r3, [r4, #20]
	lsls r3, r7, #16
	orrs r2, r3
	ldr r3, .L_0200a264
	adds r0, r4, #0
	orrs r2, r3
	movs r3, #128
	str r2, [r4, #24]
	lsls r3, r3, #3
	mov r2, r10
	orrs r2, r3
	str r2, [r4, #28]
	adds r0, #20
	bl Func_02002aa4
.L_0200a248:
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a254:
	.4byte gData_02003704
.L_0200a258:
	.4byte gPartyState
.L_0200a25c:
	.4byte 0xffff0000
.L_0200a260:
	.4byte gResourceTableEntries
.L_0200a264:
	.4byte 0x80008800
	.section .text.x0200a268,"ax",%progbits
	.global Func_02002268
	.thumb_func
Func_02002268:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #48
	str r0, [sp, #44]
	ldr r0, .L_0200a498
	str r1, [sp, #40]
	mov r8, r0
	movs r1, #32
	add r1, r8
	mov r9, r1
	mov r12, r9
	adds r5, r2, #0
	mov r2, r12
	adds r6, r3, #0
	str r2, [sp, #8]
	ldr r3, .L_0200a49c
	movs r1, #4
	ldr r7, [sp, #80]
	mov lr, r3
	.2byte 0xf800
	add r0, sp, #44
	ldrh r0, [r0]
	mov r1, r8
	strh r0, [r1, #4]
	add r1, sp, #40
	ldrh r1, [r1]
	mov r3, r8
	strh r1, [r3]
	strh r5, [r3, #2]
	movs r3, #255
	lsls r3, r3, #8
	mov r5, r8
	mov r0, r8
	adds r3, #255
	mov r1, r8
	strh r6, [r5, #6]
	movs r2, #0
	strh r7, [r0, #8]
	strh r3, [r1, #12]
	mov r3, r8
	strh r2, [r3, #10]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #32]
	movs r2, #132
	mov r12, r3
	lsls r2, r2, #1
	mov r1, r12
	add r2, r12
	adds r1, #236
	ldr r0, [r1]
	ldr r3, [r2, #8]
	ldr r5, [r2, #48]
	adds r3, r3, r0
	asrs r3, r3, #20
	str r3, [sp, #32]
	adds r1, #4
	ldr r3, [r2, #12]
	ldr r2, [r1]
	adds r3, r3, r2
	asrs r3, r3, #20
	str r3, [sp, #28]
	mov r3, r12
	adds r3, #244
	ldr r3, [r3]
	subs r3, r3, r0
	asrs r3, r3, #20
	str r3, [sp, #24]
	mov r3, r12
	adds r3, #248
	ldr r3, [r3]
	asrs r0, r0, #20
	subs r3, r3, r2
	asrs r2, r2, #20
	lsls r2, r2, #7
	adds r2, r2, r0
	lsls r2, r2, #2
	asrs r3, r3, #20
	adds r5, r5, r2
	movs r0, #0
	str r3, [sp, #20]
	str r5, [sp, #36]
	str r0, [sp, #12]
	cmp r0, r3
	bge .L_0200a3ec
.L_0200a31c:
	ldr r1, [sp, #12]
	ldr r2, [sp, #36]
	ldr r5, [sp, #24]
	lsls r3, r1, #9
	adds r2, r2, r3
	movs r3, #0
	mov r11, r2
	str r3, [sp, #16]
	cmp r3, r5
	bge .L_0200a3e0
.L_0200a330:
	mov r0, r11
	ldrb r5, [r0, #2]
	cmp r5, #0
	beq .L_0200a3d0
	ldr r1, [sp, #44]
	cmp r5, r1
	bcc .L_0200a3d0
	adds r1, #1
	mov r10, r1
	cmp r5, r10
	bhi .L_0200a3d0
	ldr r2, [sp, #16]
	ldr r3, [sp, #32]
	mov r0, r9
	adds r7, r2, r3
	strh r7, [r0]
	ldr r1, [sp, #12]
	ldr r2, [sp, #28]
	add r0, sp, #40
	ldrh r0, [r0]
	adds r6, r1, r2
	mov r3, r9
	mov r1, r9
	strh r6, [r3, #2]
	strh r0, [r1, #4]
	ldr r1, [sp, #40]
	movs r0, #10
	adds r1, #1
	adds r0, #255
	str r1, [sp, #40]
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a384
	cmp r5, r10
	bne .L_0200a3c2
	mov r3, r9
	movs r2, #4
	ldrsh r0, [r3, r2]
	bl GameFlag_SetBit
	b .L_0200a3c2
.L_0200a384:
	mov r1, r9
	movs r5, #4
	ldrsh r0, [r1, r5]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a3c2
	mov r2, r8
	ldrh r4, [r2, #6]
	ldrh r5, [r2, #8]
	movs r3, #8
	ldrsh r1, [r2, r3]
	movs r3, #6
	ldrsh r0, [r2, r3]
	movs r2, #64
	adds r3, r2, #0
	ands r3, r4
	ands r2, r5
	lsls r3, r3, #16
	lsls r2, r2, #16
	asrs r3, r3, #16
	asrs r2, r2, #16
	orrs r7, r3
	orrs r6, r2
	adds r1, #1
	movs r2, #1
	movs r3, #1
	str r7, [sp, #0]
	str r6, [sp, #4]
	bl Func_02002b1c
.L_0200a3c2:
	mov r0, r8
	ldrh r3, [r0, #10]
	mov r1, r8
	adds r3, #1
	strh r3, [r1, #10]
	movs r5, #8
	add r9, r5
.L_0200a3d0:
	ldr r2, [sp, #16]
	ldr r5, [sp, #24]
	adds r2, #1
	movs r3, #4
	str r2, [sp, #16]
	add r11, r3
	cmp r2, r5
	blt .L_0200a330
.L_0200a3e0:
	ldr r0, [sp, #12]
	ldr r1, [sp, #20]
	adds r0, #1
	str r0, [sp, #12]
	cmp r0, r1
	blt .L_0200a31c
.L_0200a3ec:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200a444
	ldr r3, .L_0200a4a0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	movs r2, #0
	asrs r4, r3, #20
	ldr r3, [r0, #16]
	mov r0, r8
	asrs r1, r3, #20
	ldr r3, [sp, #8]
	mov r9, r3
	movs r5, #10
	ldrsh r3, [r0, r5]
	cmp r2, r3
	bge .L_0200a444
.L_0200a41e:
	mov r0, r9
	movs r5, #0
	ldrsh r3, [r0, r5]
	cmp r3, r4
	bne .L_0200a434
	movs r5, #2
	ldrsh r3, [r0, r5]
	cmp r3, r1
	bne .L_0200a434
	mov r0, r8
	strh r2, [r0, #12]
.L_0200a434:
	movs r3, #8
	mov r0, r8
	add r9, r3
	movs r5, #10
	ldrsh r3, [r0, r5]
	adds r2, #1
	cmp r2, r3
	blt .L_0200a41e
.L_0200a444:
	movs r0, #128
	lsls r0, r0, #1
	bl MemAlloc
	adds r5, r0, #0
	adds r1, r5, #0
	movs r2, #63
.L_0200a452:
	ldr r3, .L_0200a4a4
	subs r2, #1
	stmia r1!, {r3}
	cmp r2, #0
	bge .L_0200a452
	bl Resource_FindFreeEntry
	mov r1, r8
	strh r0, [r1, #16]
	lsls r0, r0, #16
	movs r1, #128
	adds r2, r5, #0
	lsls r1, r1, #1
	asrs r0, r0, #16
	bl VramBlock_LoadCached
	adds r0, r5, #0
	bl MemFree
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200a4a8
	bl Scheduler_AddOrUpdateCallback
	mov r3, r8
	movs r2, #10
	ldrsh r0, [r3, r2]
	add sp, #48
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a498:
	.4byte gData_02003704
.L_0200a49c:
	.4byte IwramClearWords
.L_0200a4a0:
	.4byte gPartyState
.L_0200a4a4:
	.4byte 0x11111111
.L_0200a4a8:
	.4byte Func_0200213c
	.section .text.x0200a534,"ax",%progbits
	.global Func_02002534
	.thumb_func
Func_02002534:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	ldr r3, .L_0200a5a0
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	mov r8, r0
	ldr r0, [r3]
	sub sp, #8
	bl Object_GetById
	mov r2, r8
	ldrh r1, [r2, #6]
	movs r2, #64
	ldr r6, [r0, #8]
	adds r3, r2, #0
	ands r3, r1
	lsls r3, r3, #16
	mov r1, r8
	asrs r3, r3, #16
	asrs r6, r6, #20
	orrs r6, r3
	ldrh r3, [r1, #8]
	ldr r5, [r0, #16]
	ands r2, r3
	lsls r2, r2, #16
	asrs r2, r2, #16
	asrs r5, r5, #20
	orrs r5, r2
	bl Func_020024ac
	movs r0, #161
	bl Func_02002c5c
	mov r3, r8
	movs r2, #8
	ldrsh r1, [r3, r2]
	movs r2, #6
	ldrsh r0, [r3, r2]
	adds r1, #1
	movs r2, #1
	movs r3, #1
	str r6, [sp, #0]
	str r5, [sp, #4]
	bl Func_02002b1c
	movs r0, #12
	bl Battle_WaitMode0
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_0200a5a0:
	.4byte gPartyState
	.section .text.x0200a5a4,"ax",%progbits
	.global Func_020025a4
	.thumb_func
Func_020025a4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r1, .L_0200a654
	movs r2, #133
	lsls r2, r2, #2
	adds r1, r1, r2
	mov r8, r0
	ldr r0, [r1]
	sub sp, #8
	mov r10, r1
	bl Object_GetById
	adds r6, r0, #0
	ldr r3, [r6, #8]
	movs r2, #64
	asrs r7, r3, #20
	mov r3, r8
	ldrh r1, [r3, #6]
	adds r3, r2, #0
	ands r3, r1
	lsls r3, r3, #16
	mov r1, r8
	asrs r3, r3, #16
	orrs r7, r3
	ldrh r3, [r1, #8]
	ldr r5, [r6, #16]
	ands r2, r3
	lsls r2, r2, #16
	asrs r2, r2, #16
	asrs r5, r5, #20
	orrs r5, r2
	bl Func_020024ac
	movs r0, #229
	bl Func_02002c5c
	mov r3, r8
	movs r2, #8
	ldrsh r1, [r3, r2]
	movs r2, #6
	ldrsh r0, [r3, r2]
	adds r1, #2
	movs r2, #1
	movs r3, #1
	str r7, [sp, #0]
	str r5, [sp, #4]
	bl Func_02002b1c
	movs r0, #12
	bl Battle_WaitMode0
	movs r3, #128
	ldr r2, .L_0200a64c
	lsls r3, r3, #7
	strh r3, [r6, #6]
	adds r3, r6, #0
	adds r3, #85
	strb r2, [r3]
	mov r3, r10
	ldr r0, [r3]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r2, #226
	movs r3, #128
	lsls r2, r2, #4
	lsls r3, r3, #19
	adds r2, #255
	adds r3, #74
	strh r2, [r3]
	movs r1, #128
	lsls r1, r1, #19
	ldrh r3, [r1]
	ldr r2, .L_0200a650
	movs r7, #0
	orrs r3, r2
	strh r3, [r1]
	mov r2, r10
	b .L_0200a658
	.2byte 0x0000
.L_0200a64c:
	.4byte 0x00000000
.L_0200a650:
	.4byte 0x00008000
.L_0200a654:
	.4byte gPartyState
.L_0200a658:
	movs r3, #1
	mov r1, r8
	strh r3, [r1, #14]
	ldr r0, [r2]
	movs r1, #28
	bl Object_SetModeById
	movs r0, #16
	bl Battle_WaitMode0
.L_0200a66c:
	cmp r7, #5
	bne .L_0200a676
	movs r0, #204
	bl Func_02002c5c
.L_0200a676:
	ldr r3, [r6, #24]
	ldr r1, .L_0200a6d4
	ldr r2, .L_0200a6d8
	adds r3, r3, r1
	str r3, [r6, #24]
	ldr r3, [r6, #28]
	ldr r1, .L_0200a6dc
	adds r3, r3, r2
	str r3, [r6, #28]
	ldr r3, [r6, #12]
	movs r0, #1
	adds r3, r3, r1
	str r3, [r6, #12]
	adds r7, #1
	bl WaitFrames
	cmp r7, #39
	ble .L_0200a66c
	ldr r3, .L_0200a6e0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r3, #0
	adds r0, #84
	strb r3, [r0]
	mov r1, r8
	strh r3, [r1, #14]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	adds r2, #93
	str r2, [r3]
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200a6d4:
	.4byte 0xfffffc00
.L_0200a6d8:
	.4byte 0xfffffd00
.L_0200a6dc:
	.4byte 0xffff6667
.L_0200a6e0:
	.4byte gPartyState
	.section .text.x0200a73a,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200a73c,"ax",%progbits
	.global Func_0200273c
	.thumb_func
Func_0200273c:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200a8c0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #68
	bl Object_GetById
	adds r7, r0, #0
	bl Func_02002b6c
	movs r0, #0
	bl Func_02002c0c
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Motion_CamBounds
	bl Func_02002afc
	movs r0, #1
	bl WaitFrames
	movs r3, #130
	lsls r3, r3, #16
	str r3, [r7, #12]
	movs r3, #128
	lsls r3, r3, #8
	adds r5, r7, #0
	str r3, [r7, #72]
	adds r5, #85
	movs r3, #0
	str r3, [r7, #68]
	strb r3, [r5]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r4, #214
	lsls r4, r4, #1
	movs r2, #128
	adds r3, r3, r4
	lsls r2, r2, #1
	str r2, [r3]
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #204
	bl Func_02002c5c
	movs r3, #3
	strb r3, [r5]
	movs r0, #24
	bl Battle_WaitMode0
	add r2, sp, #28
	movs r3, #7
	str r3, [r2, #4]
	ldr r3, .L_0200a8c4
	mov r8, r2
	str r3, [r2, #36]
	movs r3, #163
	lsls r3, r3, #8
	adds r3, #215
	str r3, [r2, #8]
	str r3, [r2, #12]
	movs r3, #0
	mov r10, r3
.L_0200a7d6:
	mov r4, r10
	lsls r5, r4, #12
	adds r0, r5, #0
	bl Math_Cosine
	add r6, sp, #16
	movs r3, #0
	str r0, [r6]
	adds r0, r5, #0
	str r3, [r6, #4]
	bl Math_Sine
	ldr r3, [r6]
	str r0, [r6, #8]
	asrs r2, r3, #1
	adds r3, r3, r2
	str r3, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r2, [r6]
	adds r3, r3, r0
	lsls r3, r3, #14
	lsrs r3, r3, #16
	adds r2, r2, r3
	ldr r3, .L_0200a8c8
	adds r2, r2, r3
	str r2, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r5, [r6, #8]
	adds r3, r3, r0
	ldr r4, .L_0200a8cc
	lsls r3, r3, #13
	lsrs r3, r3, #16
	adds r5, r5, r3
	adds r5, r5, r4
	ldr r4, [r6, #4]
	str r5, [r6, #8]
	ldr r2, [r7, #16]
	ldr r3, [r6]
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	str r4, [sp, #0]
	ldr r4, .L_0200a8d0
	str r5, [sp, #4]
	str r4, [sp, #8]
	mov r4, r8
	str r4, [sp, #12]
	bl Func_02001f74
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #16
	bls .L_0200a7d6
	movs r0, #188
	bl Func_02002c5c
	ldr r5, .L_0200a8c0
	movs r4, #133
	lsls r4, r4, #2
	adds r5, r5, r4
	movs r1, #2
	ldr r0, [r5]
	adds r1, #255
	bl Func_02002bbc
	ldr r0, [r5]
	movs r1, #49
	bl Object_SetModeById
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02002b3c
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	adds r2, #102
	negs r0, r0
	negs r1, r1
	bl Func_02002b3c
	bl Func_02002b44
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #1
	bl Func_02002bbc
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #72]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #68]
	movs r0, #10
	bl Battle_WaitMode0
	ldr r0, [r5]
	movs r1, #1
	bl Object_SetModeById
	bl Func_02002b74
	add sp, #68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200a8c0:
	.4byte gPartyState
.L_0200a8c4:
	.4byte Effect_AdvanceMotion
.L_0200a8c8:
	.4byte 0xffffa000
.L_0200a8cc:
	.4byte 0xffffd000
.L_0200a8d0:
	.4byte 0x01090001
	.section .text.x0200a8d4,"ax",%progbits
	.global Func_020028d4
	.thumb_func
Func_020028d4:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200a97c
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	ldr r6, .L_0200a980
	asrs r3, r3, #20
	mov r8, r3
	ldr r3, [r0, #16]
	adds r5, r6, #0
	asrs r3, r3, #20
	mov r10, r3
	movs r1, #10
	ldrsh r3, [r6, r1]
	movs r7, #0
	adds r5, #32
	ldrh r2, [r6, #10]
	cmp r7, r3
	bge .L_0200a970
.L_0200a908:
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, r8
	bne .L_0200a964
	movs r1, #2
	ldrsh r3, [r5, r1]
	cmp r3, r10
	bne .L_0200a964
	movs r2, #4
	ldrsh r0, [r5, r2]
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a938
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_02002534
	movs r3, #4
	ldrsh r0, [r5, r3]
	bl GameFlag_SetBit
	strh r7, [r6, #12]
	b .L_0200a970
.L_0200a938:
	movs r1, #12
	ldrsh r3, [r6, r1]
	cmp r7, r3
	beq .L_0200a970
	adds r0, r6, #0
	adds r1, r5, #0
	strh r7, [r6, #12]
	bl Func_020025a4
	movs r2, #2
	ldrsh r0, [r6, r2]
	mov r1, r8
	bl GameFlag_SetByte
	movs r3, #2
	ldrsh r0, [r6, r3]
	mov r1, r10
	adds r0, #8
	bl GameFlag_SetByte
	movs r0, #1
	b .L_0200a972
.L_0200a964:
	lsls r3, r2, #16
	adds r7, #1
	asrs r3, r3, #16
	adds r5, #8
	cmp r7, r3
	blt .L_0200a908
.L_0200a970:
	movs r0, #0
.L_0200a972:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a97c:
	.4byte gPartyState
.L_0200a980:
	.4byte gData_02003704
	.section .text.x0200a984,"ax",%progbits
	.global Func_02002984
	.thumb_func
Func_02002984:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #4
	ldr r3, .L_0200aa34
	str r2, [sp, #0]
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	mov r9, r0
	ldr r0, [r3]
	mov r11, r1
	bl Object_GetById
	movs r3, #192
	ldr r5, .L_0200aa38
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	adds r6, r0, #0
	movs r2, #2
	ldrsh r0, [r5, r2]
	mov r10, r3
	bl GameFlag_GetByte
	adds r7, r0, #0
	movs r3, #2
	ldrsh r0, [r5, r3]
	adds r0, #8
	bl GameFlag_GetByte
	mov r8, r0
	cmp r7, #0
	bne .L_0200a9d2
	cmp r0, #0
	beq .L_0200aa26
.L_0200a9d2:
	movs r2, #2
	ldrsh r0, [r5, r2]
	movs r1, #0
	bl GameFlag_SetByte
	movs r3, #2
	ldrsh r0, [r5, r3]
	movs r1, #0
	adds r0, #8
	bl GameFlag_SetByte
	mov r3, r9
	adds r2, r7, r3
	mov r3, r8
	movs r1, #128
	add r3, r11
	lsls r1, r1, #12
	lsls r3, r3, #20
	adds r3, r3, r1
	str r3, [r6, #16]
	movs r3, #230
	lsls r3, r3, #1
	add r3, r10
	lsls r2, r2, #20
	adds r2, r2, r1
	ldr r1, [r3]
	str r2, [r6, #8]
	str r2, [r1, #8]
	ldr r3, [r6, #16]
	str r3, [r1, #16]
	bl Func_02002afc
	bl Func_0200273c
	movs r2, #192
	lsls r2, r2, #18
	ldr r3, [r2, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r2, [sp, #0]
	str r2, [r3]
.L_0200aa26:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200aa34:
	.4byte gPartyState
.L_0200aa38:
	.4byte gData_02003704
	.section .bss,"aw",%nobits
	.global Data_020035b8
Data_020035b8:
	.space 0x00000004
	.global gData_020035bc
gData_020035bc:
	.space 0x00000020
	.global gData_020035dc
gData_020035dc:
	.space 0x00000004
	.global gData_020035e0
gData_020035e0:
	.space 0x0000001c
	.global gData_020035fc
gData_020035fc:
	.space 0x00000002
	.global gData_020035fe
gData_020035fe:
	.space 0x00000002
	.global gData_02003600
gData_02003600:
	.space 0x00000040
	.global Data_02003640
Data_02003640:
	.space 0x000000c0
	.global gData_02003700
gData_02003700:
	.space 0x00000002
	.global gData_02003702
gData_02003702:
	.space 0x00000002
	.global gData_02003704
gData_02003704:
