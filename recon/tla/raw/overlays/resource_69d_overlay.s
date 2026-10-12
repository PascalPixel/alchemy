.syntax unified
	.thumb
	.section .text.x02008054,"ax",%progbits
	.global Func_02000054
	.thumb_func
Func_02000054:
	push {r5, r6, lr}
	sub sp, #8
	movs r5, #19
	movs r6, #10
	movs r0, #17
	movs r1, #10
	movs r2, #1
	movs r3, #4
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02001e00
	movs r0, #17
	movs r1, #10
	movs r2, #1
	movs r3, #1
	str r5, [sp, #0]
	str r6, [sp, #4]
	bl Func_02001df8
	movs r0, #152
	movs r1, #160
	lsls r1, r1, #16
	movs r2, #0
	movs r3, #254
	lsls r0, r0, #17
	bl Func_02001ef0
	movs r1, #252
	movs r2, #168
	movs r0, #14
	lsls r1, r1, #17
	lsls r2, r2, #16
	bl Func_02001e70
	movs r0, #160
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020080bc
	movs r0, #14
	bl Func_02000724
	movs r0, #248
	movs r1, #160
	lsls r0, r0, #17
	lsls r1, r1, #16
	movs r2, #0
	movs r3, #254
	bl Func_02001ef0
.L_020080bc:
	add sp, #8
	pop {r5, r6, pc}
	.section .text.x02008244,"ax",%progbits
	.global Func_02000244
	.thumb_func
Func_02000244:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	adds r6, r0, #0
	movs r1, #100
	adds r1, r1, r6
	movs r3, #0
	ldrsh r2, [r1, r3]
	adds r7, r6, #0
	mov r10, r2
	adds r7, #102
	mov r0, r10
	mov r8, r1
	movs r1, #0
	ldrsh r5, [r7, r1]
	bl Math_Cosine
	ldr r1, [r6, #48]
	ldr r3, .L_020082f8
	mov lr, r3
	.2byte 0xf800
	lsls r3, r5, #1
	ldr r1, .L_020082fc
	adds r3, r3, r5
	lsls r3, r3, #16
	adds r3, r3, r1
	movs r2, #232
	lsls r2, r2, #16
	str r3, [r6, #12]
	movs r3, #130
	adds r0, r0, r2
	lsls r3, r3, #18
	str r0, [r6, #8]
	str r3, [r6, #16]
	movs r2, #0
	ldrsh r3, [r7, r2]
	cmp r3, #1
	bne .L_0200829a
	adds r0, r6, #0
	movs r1, #7
	bl Func_02001da8
.L_0200829a:
	movs r1, #0
	ldrsh r3, [r7, r1]
	cmp r3, #38
	bne .L_020082aa
	adds r0, r6, #0
	movs r1, #2
	bl Func_02001da8
.L_020082aa:
	ldr r3, [r6, #12]
	cmp r3, #0
	ble .L_020082b8
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r7]
.L_020082b8:
	mov r0, r10
	bl Math_Sine
	ldr r1, [r6, #80]
	cmp r0, #0
	bge .L_020082d4
	ldrb r3, [r1, #9]
	movs r2, #13
	negs r2, r2
	ands r2, r3
	movs r3, #4
	orrs r2, r3
	strb r2, [r1, #9]
	b .L_020082dc
.L_020082d4:
	ldrb r3, [r1, #9]
	movs r2, #12
	orrs r3, r2
	strb r3, [r1, #9]
.L_020082dc:
	mov r1, r8
	ldr r3, [r6, #52]
	ldrh r2, [r1]
	adds r2, r2, r3
	mov r3, r8
	strh r2, [r3]
	ldrh r3, [r7]
	adds r3, #1
	strh r3, [r7]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020082f8:
	.4byte IwramMulQ16
.L_020082fc:
	.4byte 0xff880000
	.section .text.x02008300,"ax",%progbits
	.global Func_02000300
	.thumb_func
Func_02000300:
	push {r5, r6, r7, lr}
	movs r7, #0
	movs r6, #0
.L_02008306:
	movs r0, #168
	movs r1, #0
	movs r2, #0
	movs r3, #0
	lsls r0, r0, #2
	bl Func_02001dc0
	adds r5, r0, #0
	adds r2, r5, #0
	movs r3, #0
	adds r2, #85
	strb r3, [r2]
	subs r2, #50
	strb r3, [r2]
	bl Random16Far
	movs r3, #192
	lsls r3, r3, #11
	lsls r0, r0, #2
	adds r0, r0, r3
	str r0, [r5, #48]
	bl Random16Far
	movs r3, #128
	lsls r0, r0, #11
	lsls r3, r3, #4
	lsrs r0, r0, #16
	adds r0, r0, r3
	ldr r3, .L_02008380
	str r0, [r5, #52]
	str r3, [r5, #108]
	bl Random16Far
	adds r3, r5, #0
	adds r3, #100
	strh r0, [r3]
	adds r3, #2
	strh r6, [r3]
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	adds r0, r5, #0
	movs r1, #2
	bl Object_SetPartAttribute
	ldr r1, [r5, #80]
	movs r0, #13
	ldrb r3, [r1, #9]
	negs r0, r0
	adds r2, r0, #0
	ands r3, r2
	movs r2, #4
	orrs r3, r2
	adds r7, #1
	strb r3, [r1, #9]
	adds r6, #5
	cmp r7, #7
	bne .L_02008306
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008380:
	.4byte Func_02000244
	.section .text.x02008384,"ax",%progbits
	.global Func_02000384
	.thumb_func
Func_02000384:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	lsls r1, r1, #1
	movs r2, #129
	adds r3, r3, r1
	lsls r2, r2, #2
	str r2, [r3]
	bl Func_020000c0
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #7
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020083b4
	bl Func_0200017c
.L_020083b4:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #8
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020083c6
	bl Func_020001ac
.L_020083c6:
	bl Func_02000300
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	mov r10, r0
	cmp r0, #0
	bne .L_020084c2
	ldr r2, .L_020084f4
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #11
	bne .L_020084c2
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r2, r3
	ldr r0, [r5]
	bl Object_GetById
	adds r6, r0, #0
	adds r7, r6, #0
	adds r7, #35
	ldrb r2, [r7]
	movs r3, #191
	ands r3, r2
	strb r3, [r7]
	bl Func_02001e48
	movs r0, #0
	bl Func_02001ed0
	ldr r0, [r5]
	movs r1, #1
	bl ObjectMotion_SetActionVariant
	ldr r0, [r5]
	movs r1, #2
	bl Object_SetModeById
	adds r0, r6, #0
	movs r1, #64
	bl ObjectDispatch_ApplyValueToChildren
	ldr r3, .L_020084f8
	movs r1, #85
	adds r1, r1, r6
	mov r2, r10
	strb r2, [r1]
	str r3, [r6, #20]
	str r3, [r6, #12]
	mov r8, r1
	bl Event_StartScreenTransition
	movs r0, #10
	bl Battle_WaitMode0
	ldr r3, [r6, #12]
	movs r5, #0
	cmp r3, #0
	bge .L_0200846c
.L_02008446:
	movs r1, #192
	lsls r1, r1, #10
	adds r3, r3, r1
	str r3, [r6, #12]
	str r3, [r6, #20]
	ldrh r3, [r6, #6]
	movs r2, #192
	lsls r2, r2, #5
	adds r3, r3, r2
	strh r3, [r6, #6]
	movs r0, #2
	adds r5, #1
	bl WaitFrames
	cmp r5, #89
	bgt .L_0200846c
	ldr r3, [r6, #12]
	cmp r3, #0
	blt .L_02008446
.L_0200846c:
	movs r3, #0
	str r3, [r6, #12]
	str r3, [r6, #20]
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r6, #6]
	mov r1, r8
	movs r3, #3
	strb r3, [r1]
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r6, #40]
	movs r3, #128
	lsls r3, r3, #8
	ldr r5, .L_020084f4
	str r3, [r6, #72]
	movs r2, #133
	lsls r2, r2, #2
	adds r5, r5, r2
	movs r1, #1
	ldr r0, [r5]
	bl Object_SetModeById
	movs r0, #40
	bl Battle_WaitMode0
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r6, #72]
	ldrb r2, [r7]
	movs r3, #64
	orrs r3, r2
	strb r3, [r7]
	ldr r0, [r5]
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r0]
	bl Func_02001e50
.L_020084c2:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	bne .L_020084ea
	ldr r2, .L_020084f4
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #12
	bne .L_020084ea
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r2, r1
	ldr r0, [r3]
	bl Func_02001f00
.L_020084ea:
	movs r0, #0
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_020084f4:
	.4byte gPartyState
.L_020084f8:
	.4byte 0xff880000
	.section .text.x020084fc,"ax",%progbits
	.global Func_020004fc
	.thumb_func
Func_020004fc:
	push {lr}
	movs r0, #0
	bl Scene_SetArrivalFlags
	movs r0, #0
	bl Func_020006a0
	ldr r3, .L_020085a8
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
	movs r0, #160
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008576
	movs r0, #8
	bl Func_02000724
	movs r0, #9
	bl Func_02000724
	movs r0, #10
	bl Func_02000724
	movs r0, #11
	bl Func_02000724
	movs r0, #12
	bl Func_02000724
	movs r0, #13
	bl Func_02000724
	movs r0, #16
	bl Func_02000724
	movs r0, #17
	bl Func_02000724
	movs r0, #18
	bl Func_02000724
	movs r0, #19
	bl Func_02000724
	movs r0, #19
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #10
	str r3, [r0, #72]
.L_02008576:
	movs r0, #19
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #6
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020085a4
	movs r1, #168
	movs r2, #130
	movs r0, #15
	lsls r1, r1, #16
	lsls r2, r2, #18
	bl Func_02001e70
	movs r0, #15
	bl Func_02000724
.L_020085a4:
	movs r0, #0
	pop {pc}
.L_020085a8:
	.4byte gPartyState
	.section .text.x020085d6,"ax",%progbits
	.2byte 0x0000
	.section .text.x020085e4,"ax",%progbits
	.global Func_020005e4
	.thumb_func
Func_020005e4:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r3, .L_02008690
	sub sp, #32
	ldr r0, [r3]
	cmp r0, #0
	bge .L_020085f6
	adds r0, #3
.L_020085f6:
	asrs r0, r0, #2
	movs r1, #5
	bl __modsi3
	ldr r3, .L_02008694
	mov r8, r0
	ldr r3, [r3]
	cmp r3, #0
	beq .L_0200864a
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
	beq .L_0200862a
	movs r3, #192
	lsls r3, r3, #2
	adds r3, #255
	ands r3, r1
	cmp r3, #153
	bne .L_02008686
.L_0200862a:
	movs r1, #192
	lsls r1, r1, #4
	adds r1, #164
	adds r3, r2, r1
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	bne .L_02008686
	movs r0, #175
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_02008686
.L_0200864a:
	movs r5, #0
	movs r6, #4
.L_0200864e:
	mov r2, r8
	adds r0, r2, r5
	movs r1, #5
	mov r7, sp
	bl __modsi3
	ldr r3, .L_02008698
	lsls r0, r0, #1
	ldrh r3, [r3, r6]
	adds r5, #1
	strh r3, [r7, r0]
	adds r6, #2
	cmp r5, #4
	ble .L_0200864e
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r7, #0
	ldr r1, .L_0200869c
	adds r2, #2
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, .L_02008690
	ldr r3, [r2]
	adds r3, #1
	str r3, [r2]
.L_02008686:
	add sp, #32
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008690:
	.4byte Data_020026f0
.L_02008694:
	.4byte Data_020026f4
.L_02008698:
	.4byte Data_0200271c
.L_0200869c:
	.4byte 0x05000184
	.section .text.x02008778,"ax",%progbits
	.global Func_02000778
	.thumb_func
Func_02000778:
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
	ldr r3, .L_02008884
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r6, r0, #0
	ldr r7, [r6, #104]
	bl Func_02001e48
	movs r0, #0
	bl Func_02001ed0
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
	beq .L_02008818
.L_020087d2:
	ldr r3, [r7, #8]
	ldr r2, .L_02008888
	str r3, [r6, #8]
	ldr r3, [r7, #12]
	adds r3, r3, r5
	str r3, [r6, #12]
	ldr r3, [r7, #16]
	str r3, [r6, #16]
	cmp r5, r2
	bgt .L_020087ee
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	adds r5, r5, r3
.L_020087ee:
	ldr r3, .L_0200888c
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
	bne .L_020087d2
.L_02008818:
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
	ldr r0, .L_02008880
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
	b .L_02008890
.L_02008880:
	.4byte 0x00000001
.L_02008884:
	.4byte gPartyState
.L_02008888:
	.4byte 0x0003ffff
.L_0200888c:
	.4byte gFrameCount
.L_02008890:
	bl Motion_CamBounds
	bl Func_02001ea8
	movs r3, #128
	adds r7, r0, #0
	lsls r3, r3, #12
	str r3, [r7, #48]
	movs r3, #128
	ldr r5, .L_020088d8
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
	bl Func_02001dd8
	ldr r3, [r6, #20]
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	ldr r2, [r6, #12]
	movs r5, #0
	cmp r2, r3
	ble .L_020088f6
	b .L_020088dc
	.2byte 0x0000
.L_020088d8:
	.4byte 0x00000000
.L_020088dc:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #59
	bgt .L_020088f6
	ldr r3, [r6, #20]
	movs r2, #128
	lsls r2, r2, #13
	adds r3, r3, r2
	ldr r2, [r6, #12]
	cmp r2, r3
	bgt .L_020088dc
.L_020088f6:
	movs r0, #127
	bl Func_02001f08
	ldr r3, [r6, #40]
	movs r5, #0
	cmp r3, #0
	beq .L_02008916
.L_02008904:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #59
	bgt .L_02008916
	ldr r3, [r6, #40]
	cmp r3, #0
	bne .L_02008904
.L_02008916:
	adds r0, r7, #0
	bl Func_02001de0
	ldr r5, .L_02008960
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
	bl Func_02001e50
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_02008960:
	.4byte gPartyState
	.section .text.x02008990,"ax",%progbits
	.global Func_02000990
	.thumb_func
Func_02000990:
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
	ldr r3, .L_020089f8
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
	bne .L_020089ee
	movs r3, #173
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_020089ee
	movs r3, #175
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	bne .L_020089ee
	movs r3, #180
	lsls r3, r3, #1
	add r3, r10
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_020089fc
.L_020089ee:
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_ApplyValueToChildren
	b .L_02008b3a
.L_020089f8:
	.4byte gPartyState
.L_020089fc:
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
	bne .L_02008a1c
	movs r0, #231
	bl Func_02001f08
.L_02008a1c:
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
	bl Func_02001ee8
	cmp r0, #255
	beq .L_02008b1e
	ldr r3, [r6, #8]
	mov r5, sp
	str r3, [r5]
	adds r0, r5, #0
	ldr r3, [r6, #12]
	str r3, [r5, #4]
	ldr r3, [r6, #16]
	str r3, [r5, #8]
	bl Func_02001ed8
	ldr r5, [r5]
	movs r3, #136
	lsls r3, r3, #17
	cmp r5, r3
	bgt .L_02008b1e
	ldr r2, .L_02008b0c
	cmp r5, r2
	blt .L_02008b1e
	movs r3, #98
	adds r3, r3, r6
	mov r9, r3
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_02008ae4
	ldr r2, [r7, #12]
	ldr r3, [r6, #12]
	subs r5, r2, r3
	cmp r5, #0
	bge .L_02008a7c
	subs r5, r3, r2
.L_02008a7c:
	adds r0, r7, #0
	adds r1, r6, #0
	movs r2, #0
	adds r0, #8
	adds r1, #8
	mov r8, r2
	bl Func_02000964
	cmp r0, #12
	bgt .L_02008a9c
	movs r3, #192
	lsls r3, r3, #12
	cmp r5, r3
	bge .L_02008a9c
	movs r2, #1
	mov r8, r2
.L_02008a9c:
	mov r3, r8
	cmp r3, #0
	beq .L_02008ae4
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008ae4
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
	ldr r3, .L_02008b10
	movs r2, #128
	ldr r0, .L_02008b08
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
.L_02008ae4:
	ldrh r0, [r6, #6]
	bl Math_Cosine
	ldr r1, [r6, #48]
	ldr r5, .L_02008b14
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
	b .L_02008b18
.L_02008b08:
	.4byte 0x00000000
.L_02008b0c:
	.4byte 0xffe00000
.L_02008b10:
	.4byte gPartyState
.L_02008b14:
	.4byte IwramMulQ16
.L_02008b18:
	adds r3, r3, r0
	str r3, [r6, #16]
	b .L_02008b3a
.L_02008b1e:
	adds r3, r6, #0
	adds r3, #99
	movs r5, #0
	strb r5, [r3]
	ldr r1, .L_02008b48
	adds r0, r6, #0
	str r5, [r6, #108]
	bl Func_02001db8
	movs r0, #228
	bl Func_02001f08
	ldr r3, .L_02008b4c
	str r5, [r3]
.L_02008b3a:
	add sp, #12
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008b48:
	.4byte Data_020026f8
.L_02008b4c:
	.4byte Data_02002718
	.section .text.x02008b50,"ax",%progbits
	.global Func_02000b50
	.thumb_func
Func_02000b50:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	movs r0, #222
	sub sp, #68
	bl Func_02001f08
	ldrh r0, [r5, #6]
	bl Math_Cosine
	adds r1, r0, #0
	movs r0, #128
	ldr r6, .L_02008c04
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
	bl Func_02001dc0
	movs r1, #2
	adds r7, r0, #0
	bl Func_02001da8
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
	ldr r2, .L_02008c00
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
	ldr r3, .L_02008c08
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
	ldr r3, .L_02008c0c
	ldr r1, [r5, #12]
	adds r2, r2, r3
	movs r3, #192
	lsls r3, r3, #10
	str r3, [sp, #8]
	b .L_02008c10
.L_02008c00:
	.4byte 0x00000000
.L_02008c04:
	.4byte IwramMulQ16
.L_02008c08:
	.4byte Func_02000990
.L_02008c0c:
	.4byte 0xfffa0000
.L_02008c10:
	movs r3, #0
	str r6, [sp, #0]
	str r6, [sp, #4]
	str r4, [sp, #12]
	bl Func_02001258
	adds r0, r7, #0
	add sp, #68
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008c28,"ax",%progbits
	.global Func_02000c28
	.thumb_func
Func_02000c28:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_02008cc4
	sub sp, #56
	ldr r7, [r3]
	movs r3, #7
	ands r7, r3
	mov r10, r0
	cmp r7, #0
	bne .L_02008cb6
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
	bl Func_02001258
.L_02008cb6:
	movs r0, #0
	add sp, #56
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008cc4:
	.4byte gFrameCount
	.section .text.x02008cc8,"ax",%progbits
	.global Func_02000cc8
	.thumb_func
Func_02000cc8:
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
	bl Func_02001e70
	movs r1, #128
	movs r2, #248
	movs r0, #23
	lsls r1, r1, #18
	lsls r2, r2, #16
	bl Func_02001e70
	movs r1, #236
	movs r2, #128
	movs r0, #9
	lsls r1, r1, #17
	lsls r2, r2, #17
	bl Func_02001e70
	movs r1, #138
	movs r2, #128
	lsls r2, r2, #17
	movs r0, #10
	lsls r1, r1, #18
	bl Func_02001e70
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
	beq .L_02008da4
	ldr r3, [r6, #12]
	ldr r2, .L_02008e3c
	movs r0, #9
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r2, .L_02008e40
	ldr r3, [r5, #12]
	adds r3, r3, r2
	str r3, [r5, #12]
	ldr r2, .L_02008e44
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
.L_02008da4:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #11
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008dee
	ldr r3, [r7, #12]
	ldr r2, .L_02008e3c
	movs r0, #10
	adds r3, r3, r2
	str r3, [r7, #12]
	ldr r2, .L_02008e40
	ldr r3, [r5, #12]
	adds r3, r3, r2
	str r3, [r5, #12]
	ldr r2, .L_02008e48
	ldr r3, [r5, #8]
	adds r3, r3, r2
	str r3, [r5, #8]
	mov r2, r8
	ldrh r3, [r2, #18]
	ldr r2, .L_02008e4c
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
.L_02008dee:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #10
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008e2e
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #11
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008e2e
	ldr r3, [r6, #12]
	ldr r2, .L_02008e50
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r7, #12]
	adds r3, r3, r2
	str r3, [r7, #12]
	ldr r2, .L_02008e54
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
.L_02008e2e:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008e3c:
	.4byte 0x00066640
.L_02008e40:
	.4byte 0x0001eb80
.L_02008e44:
	.4byte 0xfffd70c0
.L_02008e48:
	.4byte 0x00028f40
.L_02008e4c:
	.4byte 0xfffff800
.L_02008e50:
	.4byte 0x00199900
.L_02008e54:
	.4byte 0x001b8480
	.section .text.x02008e58,"ax",%progbits
	.global Func_02000e58
	.thumb_func
Func_02000e58:
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
	ldr r3, .L_02008fec
	adds r2, r1, #0
	adds r2, #228
	ldr r0, [r2]
	ldr r2, [r2, #4]
	ands r0, r3
	ands r2, r3
	ldr r3, .L_02008ff0
	mov r10, r0
	movs r0, #0
	ldrsh r3, [r3, r0]
	mov r8, r2
	ldr r2, .L_02008ff4
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	sub sp, #8
	lsrs r3, r3, #5
	str r3, [sp, #4]
	ldr r6, .L_02008ff8
	ldr r3, [r1]
	movs r1, #0
	ldr r3, [r3, #4]
	mov r9, r1
	str r3, [sp, #0]
	ldr r3, .L_02008ffc
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r9, r3
	blt .L_02008eaa
	b .L_02008fde
.L_02008eaa:
	ldr r2, .L_02009000
	mov r0, r9
	lsls r3, r0, #2
	ldr r5, [r2, r3]
	cmp r5, #0
	bne .L_02008eb8
	b .L_02008fce
.L_02008eb8:
	ldr r3, [r5, #8]
	cmp r3, #0
	bne .L_02008ec0
	b .L_02008fce
.L_02008ec0:
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
	ldr r3, .L_02009004
	movs r1, #0
	ldrsh r2, [r3, r1]
	adds r3, r5, #0
	mov r12, r2
	asrs r1, r0, #16
	mov r0, r12
	adds r3, #100
	asrs r2, r4, #16
	cmp r0, #0
	bne .L_02008f36
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
	bhi .L_02008fce
	movs r2, #16
	negs r2, r2
	cmp r4, r2
	ble .L_02008fce
	cmp r4, #239
	bgt .L_02008fce
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
	ldr r3, .L_02009008
	b .L_02008f72
.L_02008f36:
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
	bhi .L_02008fce
	movs r2, #64
	negs r2, r2
	cmp r4, r2
	ble .L_02008fce
	cmp r4, #175
	bgt .L_02008fce
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
	ldr r3, .L_0200900c
.L_02008f72:
	movs r2, #128
	orrs r4, r3
	stmia r1!, {r4}
	ldr r0, [sp, #4]
	lsls r3, r7, #3
	adds r3, r0, r3
	lsls r2, r2, #4
	orrs r3, r2
	str r3, [r1]
	ldr r3, .L_02009010
	movs r0, #1
	ldrh r2, [r3]
	movs r1, #0
	ldrsh r3, [r3, r1]
	negs r0, r0
	cmp r3, r0
	bne .L_02008fb0
	adds r0, r5, #0
	bl Func_02001ee0
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
	b .L_02008fc4
.L_02008fb0:
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
.L_02008fc4:
	adds r0, r6, #0
	mov r1, r11
	bl Func_02001d80
	adds r6, #12
.L_02008fce:
	ldr r3, .L_02008ffc
	movs r1, #1
	movs r2, #0
	ldrsh r3, [r3, r2]
	add r9, r1
	cmp r9, r3
	bge .L_02008fde
	b .L_02008eaa
.L_02008fde:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008fec:
	.4byte 0xffff0000
.L_02008ff0:
	.4byte Data_0200275c
.L_02008ff4:
	.4byte ResourceTableEntries
.L_02008ff8:
	.4byte Data_020027a0
.L_02008ffc:
	.4byte Data_0200275e
.L_02009000:
	.4byte Data_02002760
.L_02009004:
	.4byte Data_02002860
.L_02009008:
	.4byte 0x40002000
.L_0200900c:
	.4byte 0xc000a000
.L_02009010:
	.4byte Data_02002862
	.section .text.x0200921e,"ax",%progbits
	.2byte 0x0000
	.section .text.x02009256,"ax",%progbits
	.2byte 0x0000
	.section .text.x02009258,"ax",%progbits
	.global Func_02001258
	.thumb_func
Func_02001258:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r11, r3
	ldr r3, .L_02009410
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
	beq .L_020092a0
	cmp r7, #0
	beq .L_020092a0
	movs r2, #24
	ldrsh r0, [r7, r2]
	adds r1, r5, #0
	adds r2, r6, #0
	b .L_020092a8
.L_020092a0:
	movs r0, #30
	adds r2, r6, #0
	adds r0, #255
	adds r1, r5, #0
.L_020092a8:
	mov r3, r10
	bl Func_02001dc0
	adds r6, r0, #0
	cmp r6, #0
	bne .L_020092b6
	b .L_02009402
.L_020092b6:
	ldr r3, [r6, #80]
	mov r1, r8
	movs r5, #15
	adds r1, #1
	ands r1, r5
	adds r0, r6, #0
	str r3, [sp, #0]
	bl Func_02001da8
	ldr r2, .L_02009414
	mov r3, r8
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r10, r3
	bl Func_02001db8
	adds r3, r6, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	ldr r3, .L_02009418
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
	ldr r3, .L_0200941c
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_02009402
	cmp r7, #0
	beq .L_02009402
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r1
	cmp r3, #0
	beq .L_02009338
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl Object_SetPartAttribute
.L_02009338:
	movs r3, #128
	lsls r3, r3, #10
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_02009358
	adds r1, r6, #0
	adds r1, #35
	ldrb r3, [r1]
	movs r2, #254
	ands r2, r3
	strb r2, [r1]
	ldr r1, [r7]
	adds r0, r6, #0
	bl Object_SetSpritePriority
.L_02009358:
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r8
	ands r2, r3
	cmp r2, #0
	beq .L_0200936c
	ldr r3, [r7, #8]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
.L_0200936c:
	movs r3, #128
	lsls r3, r3, #11
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_020093b2
	ldr r3, .L_02009414
	mov r1, r10
	ldr r5, [r3, r1]
	ldr r3, [r7, #16]
	ldr r1, [r5, #12]
	cmp r2, #0
	beq .L_0200939a
	ldr r0, [r6, #24]
	subs r0, r3, r0
	bl __divsi3
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_020093ac
.L_0200939a:
	ldr r2, .L_0200941c
	adds r0, r3, r2
	bl __divsi3
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, .L_0200941c
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_020093ac:
	bl __divsi3
	str r0, [r6, #52]
.L_020093b2:
	movs r3, #128
	lsls r3, r3, #14
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_020093ce
	adds r0, r6, #0
	movs r1, #1
	bl Func_02001da8
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_02001db8
.L_020093ce:
	movs r3, #128
	lsls r3, r3, #15
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_020093e0
	ldrh r3, [r7, #32]
	ldr r1, [sp, #0]
	strh r3, [r1, #18]
.L_020093e0:
	movs r3, #128
	lsls r3, r3, #16
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_020093f2
	ldrh r3, [r7, #34]
	mov r1, r9
	strh r3, [r1]
.L_020093f2:
	movs r3, #128
	lsls r3, r3, #17
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_02009402
	ldr r3, [r7, #36]
	str r3, [r6, #108]
.L_02009402:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02009410:
	.4byte gPartyState
.L_02009414:
	.4byte Data_0200270c
.L_02009418:
	.4byte BattleFx_UpdateObjectMotionScaleAndLinkedAngle
.L_0200941c:
	.4byte 0xffff0000
	.section .text.x02009420,"ax",%progbits
	.global Func_02001420
	.thumb_func
Func_02001420:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r4, .L_02009538
	movs r1, #1
	movs r0, #12
	ldrsh r3, [r4, r0]
	negs r1, r1
	sub sp, #4
	cmp r3, r1
	beq .L_0200952c
	lsls r3, r3, #3
	adds r3, r3, r4
	adds r3, #32
	mov r8, r3
	ldr r3, .L_0200953c
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
	bne .L_0200946c
	ldr r3, [r0, #16]
	movs r5, #2
	ldrsh r2, [r1, r5]
	asrs r3, r3, #20
	cmp r3, r2
	beq .L_02009474
.L_0200946c:
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r4, #12]
.L_02009474:
	movs r0, #12
	ldrsh r3, [r4, r0]
	movs r2, #1
	negs r2, r2
	ldr r1, .L_02009540
	cmp r3, r2
	beq .L_0200952c
	movs r5, #14
	ldrsh r3, [r4, r5]
	cmp r3, #0
	beq .L_0200952c
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
	ldr r2, .L_02009544
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
	bhi .L_0200952c
	movs r0, #15
	negs r0, r0
	cmp r2, r0
	blt .L_0200952c
	cmp r2, #239
	bgt .L_0200952c
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
	ldr r3, .L_02009548
	adds r0, r4, #0
	orrs r2, r3
	movs r3, #128
	str r2, [r4, #24]
	lsls r3, r3, #3
	mov r2, r10
	orrs r2, r3
	str r2, [r4, #28]
	adds r0, #20
	bl Func_02001d80
.L_0200952c:
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009538:
	.4byte Data_02002864
.L_0200953c:
	.4byte gPartyState
.L_02009540:
	.4byte 0xffff0000
.L_02009544:
	.4byte ResourceTableEntries
.L_02009548:
	.4byte 0x80008800
	.section .text.x0200954c,"ax",%progbits
	.global Func_0200154c
	.thumb_func
Func_0200154c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #48
	str r0, [sp, #44]
	ldr r0, .L_0200977c
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
	ldr r3, .L_02009780
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
	bge .L_020096d0
.L_02009600:
	ldr r1, [sp, #12]
	ldr r2, [sp, #36]
	ldr r5, [sp, #24]
	lsls r3, r1, #9
	adds r2, r2, r3
	movs r3, #0
	mov r11, r2
	str r3, [sp, #16]
	cmp r3, r5
	bge .L_020096c4
.L_02009614:
	mov r0, r11
	ldrb r5, [r0, #2]
	cmp r5, #0
	beq .L_020096b4
	ldr r1, [sp, #44]
	cmp r5, r1
	bcc .L_020096b4
	adds r1, #1
	mov r10, r1
	cmp r5, r10
	bhi .L_020096b4
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
	bne .L_02009668
	cmp r5, r10
	bne .L_020096a6
	mov r3, r9
	movs r2, #4
	ldrsh r0, [r3, r2]
	bl GameFlag_SetBit
	b .L_020096a6
.L_02009668:
	mov r1, r9
	movs r5, #4
	ldrsh r0, [r1, r5]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020096a6
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
	bl Func_02001df0
.L_020096a6:
	mov r0, r8
	ldrh r3, [r0, #10]
	mov r1, r8
	adds r3, #1
	strh r3, [r1, #10]
	movs r5, #8
	add r9, r5
.L_020096b4:
	ldr r2, [sp, #16]
	ldr r5, [sp, #24]
	adds r2, #1
	movs r3, #4
	str r2, [sp, #16]
	add r11, r3
	cmp r2, r5
	blt .L_02009614
.L_020096c4:
	ldr r0, [sp, #12]
	ldr r1, [sp, #20]
	adds r0, #1
	str r0, [sp, #12]
	cmp r0, r1
	blt .L_02009600
.L_020096d0:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009728
	ldr r3, .L_02009784
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
	bge .L_02009728
.L_02009702:
	mov r0, r9
	movs r5, #0
	ldrsh r3, [r0, r5]
	cmp r3, r4
	bne .L_02009718
	movs r5, #2
	ldrsh r3, [r0, r5]
	cmp r3, r1
	bne .L_02009718
	mov r0, r8
	strh r2, [r0, #12]
.L_02009718:
	movs r3, #8
	mov r0, r8
	add r9, r3
	movs r5, #10
	ldrsh r3, [r0, r5]
	adds r2, #1
	cmp r2, r3
	blt .L_02009702
.L_02009728:
	movs r0, #128
	lsls r0, r0, #1
	bl MemAlloc
	adds r5, r0, #0
	adds r1, r5, #0
	movs r2, #63
.L_02009736:
	ldr r3, .L_02009788
	subs r2, #1
	stmia r1!, {r3}
	cmp r2, #0
	bge .L_02009736
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
	ldr r0, .L_0200978c
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
.L_0200977c:
	.4byte Data_02002864
.L_02009780:
	.4byte IwramClearWords
.L_02009784:
	.4byte gPartyState
.L_02009788:
	.4byte 0x11111111
.L_0200978c:
	.4byte Func_02001420
	.section .text.x02009790,"ax",%progbits
	.global Func_02001790
	.thumb_func
Func_02001790:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r8
	push {r5, r6}
	ldr r1, .L_02009810
	movs r2, #133
	mov r8, r1
	lsls r2, r2, #2
	add r8, r2
	mov r3, r8
	ldr r0, [r3]
	bl Object_GetById
	mov r1, r8
	ldr r5, [r0, #8]
	ldr r6, [r0, #16]
	mov r10, r0
	movs r2, #128
	ldr r0, [r1]
	movs r1, #128
	lsls r1, r1, #11
	lsls r2, r2, #10
	bl ObjectMotion_SetSpeedParameters
	asrs r5, r5, #20
	mov r2, r8
	asrs r6, r6, #20
	ldr r0, [r2]
	lsls r1, r5, #4
	lsls r2, r6, #4
	adds r1, #8
	adds r2, #8
	bl ObjectMotion_SetPositionAndCommit
	movs r0, #1
	bl WaitFrames
	movs r3, #128
	lsls r3, r3, #12
	lsls r5, r5, #20
	lsls r6, r6, #20
	adds r5, r5, r3
	mov r1, r10
	adds r6, r6, r3
	ldr r2, [r1, #12]
	adds r3, r6, #0
	adds r1, r5, #0
	mov r0, r10
	bl Object_SetPositionAndResetMotion
	movs r0, #4
	bl Battle_WaitMode0
	bl Func_02001ea8
	ldr r2, .L_02009814
	ldr r3, [r0, #12]
	adds r3, r3, r2
	str r3, [r0, #12]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02009810:
	.4byte gPartyState
.L_02009814:
	.4byte 0xfff80000
	.section .text.x02009818,"ax",%progbits
	.global Func_02001818
	.thumb_func
Func_02001818:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	ldr r3, .L_02009884
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
	bl Func_02001790
	movs r0, #161
	bl Func_02001f08
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
	bl Func_02001df0
	movs r0, #12
	bl Battle_WaitMode0
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_02009884:
	.4byte gPartyState
	.section .text.x02009888,"ax",%progbits
	.global Func_02001888
	.thumb_func
Func_02001888:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r1, .L_02009938
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
	bl Func_02001790
	movs r0, #229
	bl Func_02001f08
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
	bl Func_02001df0
	movs r0, #12
	bl Battle_WaitMode0
	movs r3, #128
	ldr r2, .L_02009930
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
	ldr r2, .L_02009934
	movs r7, #0
	orrs r3, r2
	strh r3, [r1]
	mov r2, r10
	b .L_0200993c
	.2byte 0x0000
.L_02009930:
	.4byte 0x00000000
.L_02009934:
	.4byte 0x00008000
.L_02009938:
	.4byte gPartyState
.L_0200993c:
	movs r3, #1
	mov r1, r8
	strh r3, [r1, #14]
	ldr r0, [r2]
	movs r1, #28
	bl Object_SetModeById
	movs r0, #16
	bl Battle_WaitMode0
.L_02009950:
	cmp r7, #5
	bne .L_0200995a
	movs r0, #204
	bl Func_02001f08
.L_0200995a:
	ldr r3, [r6, #24]
	ldr r1, .L_020099b8
	ldr r2, .L_020099bc
	adds r3, r3, r1
	str r3, [r6, #24]
	ldr r3, [r6, #28]
	ldr r1, .L_020099c0
	adds r3, r3, r2
	str r3, [r6, #28]
	ldr r3, [r6, #12]
	movs r0, #1
	adds r3, r3, r1
	str r3, [r6, #12]
	adds r7, #1
	bl WaitFrames
	cmp r7, #39
	ble .L_02009950
	ldr r3, .L_020099c4
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
.L_020099b8:
	.4byte 0xfffffc00
.L_020099bc:
	.4byte 0xfffffd00
.L_020099c0:
	.4byte 0xffff6667
.L_020099c4:
	.4byte gPartyState
	.section .text.x02009a1e,"ax",%progbits
	.2byte 0x0000
	.section .text.x02009a20,"ax",%progbits
	.global Func_02001a20
	.thumb_func
Func_02001a20:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_02009ba4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #68
	bl Object_GetById
	adds r7, r0, #0
	bl Func_02001e48
	movs r0, #0
	bl Func_02001ed0
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Motion_CamBounds
	bl Func_02001dd0
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
	bl Func_02001f08
	movs r3, #3
	strb r3, [r5]
	movs r0, #24
	bl Battle_WaitMode0
	add r2, sp, #28
	movs r3, #7
	str r3, [r2, #4]
	ldr r3, .L_02009ba8
	mov r8, r2
	str r3, [r2, #36]
	movs r3, #163
	lsls r3, r3, #8
	adds r3, #215
	str r3, [r2, #8]
	str r3, [r2, #12]
	movs r3, #0
	mov r10, r3
.L_02009aba:
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
	ldr r3, .L_02009bac
	adds r2, r2, r3
	str r2, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r5, [r6, #8]
	adds r3, r3, r0
	ldr r4, .L_02009bb0
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
	ldr r4, .L_02009bb4
	str r5, [sp, #4]
	str r4, [sp, #8]
	mov r4, r8
	str r4, [sp, #12]
	bl Func_02001258
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #16
	bls .L_02009aba
	movs r0, #188
	bl Func_02001f08
	ldr r5, .L_02009ba4
	movs r4, #133
	lsls r4, r4, #2
	adds r5, r5, r4
	movs r1, #2
	ldr r0, [r5]
	adds r1, #255
	bl Func_02001e90
	ldr r0, [r5]
	movs r1, #49
	bl Object_SetModeById
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02001e10
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	adds r2, #102
	negs r0, r0
	negs r1, r1
	bl Func_02001e10
	bl Func_02001e18
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #1
	bl Func_02001e90
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
	bl Func_02001e50
	add sp, #68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_02009ba4:
	.4byte gPartyState
.L_02009ba8:
	.4byte Effect_AdvanceMotion
.L_02009bac:
	.4byte 0xffffa000
.L_02009bb0:
	.4byte 0xffffd000
.L_02009bb4:
	.4byte 0x01090001
	.section .text.x02009bb8,"ax",%progbits
	.global Func_02001bb8
	.thumb_func
Func_02001bb8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_02009c60
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	ldr r6, .L_02009c64
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
	bge .L_02009c54
.L_02009bec:
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, r8
	bne .L_02009c48
	movs r1, #2
	ldrsh r3, [r5, r1]
	cmp r3, r10
	bne .L_02009c48
	movs r2, #4
	ldrsh r0, [r5, r2]
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009c1c
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_02001818
	movs r3, #4
	ldrsh r0, [r5, r3]
	bl GameFlag_SetBit
	strh r7, [r6, #12]
	b .L_02009c54
.L_02009c1c:
	movs r1, #12
	ldrsh r3, [r6, r1]
	cmp r7, r3
	beq .L_02009c54
	adds r0, r6, #0
	adds r1, r5, #0
	strh r7, [r6, #12]
	bl Func_02001888
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
	b .L_02009c56
.L_02009c48:
	lsls r3, r2, #16
	adds r7, #1
	asrs r3, r3, #16
	adds r5, #8
	cmp r7, r3
	blt .L_02009bec
.L_02009c54:
	movs r0, #0
.L_02009c56:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009c60:
	.4byte gPartyState
.L_02009c64:
	.4byte Data_02002864
	.section .text.x02009c68,"ax",%progbits
	.global Func_02001c68
	.thumb_func
Func_02001c68:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #4
	ldr r3, .L_02009d18
	str r2, [sp, #0]
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	mov r9, r0
	ldr r0, [r3]
	mov r11, r1
	bl Object_GetById
	movs r3, #192
	ldr r5, .L_02009d1c
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
	bne .L_02009cb6
	cmp r0, #0
	beq .L_02009d0a
.L_02009cb6:
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
	bl Func_02001dd0
	bl Func_02001a20
	movs r2, #192
	lsls r2, r2, #18
	ldr r3, [r2, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r2, [sp, #0]
	str r2, [r3]
.L_02009d0a:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02009d18:
	.4byte gPartyState
.L_02009d1c:
	.4byte Data_02002864
	.section .bss,"aw",%nobits
	.global Data_02002718
Data_02002718:
	.space 0x00000004
	.global Data_0200271c
Data_0200271c:
	.space 0x00000020
	.global Data_0200273c
Data_0200273c:
	.space 0x00000004
	.global Data_02002740
Data_02002740:
	.space 0x0000001c
	.global Data_0200275c
Data_0200275c:
	.space 0x00000002
	.global Data_0200275e
Data_0200275e:
	.space 0x00000002
	.global Data_02002760
Data_02002760:
	.space 0x00000040
	.global Data_020027a0
Data_020027a0:
	.space 0x000000c0
	.global Data_02002860
Data_02002860:
	.space 0x00000002
	.global Data_02002862
Data_02002862:
	.space 0x00000002
	.global Data_02002864
Data_02002864:
