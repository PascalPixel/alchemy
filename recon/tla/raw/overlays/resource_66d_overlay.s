.syntax unified
	.thumb
	.section .text.x0200804c,"ax",%progbits
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	push {lr}
	ldr r3, .L_02008078
	movs r2, #241
	lsls r2, r2, #1
	adds r3, r3, r2
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #10
	bne .L_02008062
	ldr r0, .L_0200807c
	b .L_02008076
.L_02008062:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008074
	ldr r0, .L_02008080
	b .L_02008076
.L_02008074:
	ldr r0, .L_02008084
.L_02008076:
	pop {pc}
.L_02008078:
	.4byte gPartyState
.L_0200807c:
	.4byte Data_020008c4
.L_02008080:
	.4byte Data_02000954
.L_02008084:
	.4byte Data_020006cc
	.section .text.x02008088,"ax",%progbits
	.global Func_02000088
	.thumb_func
Func_02000088:
	push {lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #208
	lsls r1, r1, #4
	adds r1, #56
	adds r2, r3, r1
	movs r3, #1
	strb r3, [r2]
	ldr r3, .L_020080b4
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #10
	beq .L_020080b0
	movs r3, #0
	strb r3, [r2]
.L_020080b0:
	pop {pc}
	.2byte 0x0000
.L_020080b4:
	.4byte gPartyState
	.section .text.x020080b8,"ax",%progbits
	.global Func_020000b8
	.thumb_func
Func_020000b8:
	push {lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020080cc
	ldr r0, .L_020080d0
	b .L_020080ce
.L_020080cc:
	ldr r0, .L_020080d4
.L_020080ce:
	pop {pc}
.L_020080d0:
	.4byte Data_02000cfc
.L_020080d4:
	.4byte Data_02000b4c
	.section .text.x020080d8,"ax",%progbits
	.global Func_020000d8
	.thumb_func
Func_020000d8:
	push {r5, lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #93
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008128
	ldr r5, .L_02008138
	adds r0, r5, #0
	bl Func_02000584
	movs r1, #0
	movs r0, #14
	bl UiText_OpenMessageAtObject
	bl Func_020005dc
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02008118
	adds r0, r5, #1
	bl Func_02000584
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #93
	bl GameFlag_SetBit
	b .L_0200811e
.L_02008118:
	adds r0, r5, #2
	bl Func_02000584
.L_0200811e:
	movs r0, #14
	movs r1, #0
	bl Func_02000594
	b .L_02008136
.L_02008128:
	ldr r0, .L_0200813c
	bl Func_02000584
	movs r0, #14
	movs r1, #0
	bl Func_02000594
.L_02008136:
	pop {r5, pc}
.L_02008138:
	.4byte 0x00001d8b
.L_0200813c:
	.4byte 0x00001d8e
	.section .text.x02008140,"ax",%progbits
	.global Func_02000140
	.thumb_func
Func_02000140:
	push {r5, lr}
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #94
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02008190
	ldr r5, .L_020081a0
	adds r0, r5, #0
	bl Func_02000584
	movs r1, #0
	movs r0, #18
	bl UiText_OpenMessageAtObject
	bl Func_020005dc
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_02008180
	adds r0, r5, #1
	bl Func_02000584
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #93
	bl GameFlag_SetBit
	b .L_02008186
.L_02008180:
	adds r0, r5, #2
	bl Func_02000584
.L_02008186:
	movs r0, #18
	movs r1, #0
	bl Func_02000594
	b .L_0200819e
.L_02008190:
	ldr r0, .L_020081a4
	bl Func_02000584
	movs r0, #14
	movs r1, #0
	bl Func_02000594
.L_0200819e:
	pop {r5, pc}
.L_020081a0:
	.4byte 0x00001d92
.L_020081a4:
	.4byte 0x00001d95
	.section .text.x020081a8,"ax",%progbits
	.global Func_020001a8
	.thumb_func
Func_020001a8:
	push {r5, lr}
	ldr r5, .L_020081ec
	adds r0, r5, #0
	bl Func_02000584
	movs r1, #0
	movs r0, #19
	bl UiText_OpenMessageAtObject
	bl Func_020005dc
	movs r1, #0
	bl Inventory_PromptAndSetObjectMode
	cmp r0, #0
	bne .L_020081d6
	movs r0, #10
	bl Battle_WaitMode0
	adds r0, r5, #1
	bl Func_02000584
	b .L_020081e2
.L_020081d6:
	movs r0, #20
	bl Battle_WaitMode0
	adds r0, r5, #2
	bl Func_02000584
.L_020081e2:
	movs r0, #19
	movs r1, #0
	bl Func_02000594
	pop {r5, pc}
.L_020081ec:
	.4byte 0x00001d96
	.section .text.x020081f0,"ax",%progbits
	.global Func_020001f0
	.thumb_func
Func_020001f0:
	push {r5, lr}
	ldr r3, .L_02008228
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	ldrh r3, [r0, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	ldr r2, .L_02008224
	ands r3, r2
	movs r2, #128
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_0200822c
	movs r0, #16
	adds r1, r5, #0
	bl Func_020005e4
	b .L_02008248
	.2byte 0x0000
.L_02008224:
	.4byte 0xffffc000
.L_02008228:
	.4byte gPartyState
.L_0200822c:
	bl Func_0200054c
	movs r0, #0
	bl Func_020005c4
	ldr r0, .L_0200824c
	bl Func_02000584
	adds r0, r5, #0
	movs r1, #0
	bl Func_02000594
	bl Func_02000554
.L_02008248:
	pop {r5, pc}
	.2byte 0x0000
.L_0200824c:
	.4byte 0x00001e04
	.section .text.x02008250,"ax",%progbits
	.global Func_02000250
	.thumb_func
Func_02000250:
	push {r5, lr}
	ldr r3, .L_02008288
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	ldrh r3, [r0, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	ldr r2, .L_02008284
	ands r3, r2
	movs r2, #128
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_0200828c
	movs r0, #16
	adds r1, r5, #0
	bl Func_020005e4
	b .L_020082a8
	.2byte 0x0000
.L_02008284:
	.4byte 0xffffc000
.L_02008288:
	.4byte gPartyState
.L_0200828c:
	bl Func_0200054c
	movs r0, #0
	bl Func_020005c4
	ldr r0, .L_020082ac
	bl Func_02000584
	adds r0, r5, #0
	movs r1, #0
	bl Func_02000594
	bl Func_02000554
.L_020082a8:
	pop {r5, pc}
	.2byte 0x0000
.L_020082ac:
	.4byte 0x00001e2c
	.section .text.x020082b0,"ax",%progbits
	.global Func_020002b0
	.thumb_func
Func_020002b0:
	push {r5, lr}
	ldr r3, .L_020082e4
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	ldrh r3, [r0, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	ldr r2, .L_020082e0
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_020082e8
	adds r0, r5, #0
	bl Func_020005ec
	b .L_02008304
.L_020082e0:
	.4byte 0xffffc000
.L_020082e4:
	.4byte gPartyState
.L_020082e8:
	bl Func_0200054c
	movs r0, #0
	bl Func_020005c4
	ldr r0, .L_02008308
	bl Func_02000584
	movs r0, #26
	movs r1, #0
	bl Func_02000594
	bl Func_02000554
.L_02008304:
	pop {r5, pc}
	.2byte 0x0000
.L_02008308:
	.4byte 0x00001e0b
	.section .text.x0200830c,"ax",%progbits
	.global Func_0200030c
	.thumb_func
Func_0200030c:
	push {r5, lr}
	ldr r3, .L_02008340
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	adds r5, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	ldrh r3, [r0, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	ldr r2, .L_0200833c
	ands r3, r2
	movs r2, #192
	lsls r3, r3, #16
	lsls r2, r2, #24
	cmp r3, r2
	bne .L_02008344
	adds r0, r5, #0
	bl Func_020005ec
	b .L_02008360
.L_0200833c:
	.4byte 0xffffc000
.L_02008340:
	.4byte gPartyState
.L_02008344:
	bl Func_0200054c
	movs r0, #0
	bl Func_020005c4
	ldr r0, .L_02008364
	bl Func_02000584
	movs r0, #26
	movs r1, #0
	bl Func_02000594
	bl Func_02000554
.L_02008360:
	pop {r5, pc}
	.2byte 0x0000
.L_02008364:
	.4byte 0x00001e33
	.section .text.x02008368,"ax",%progbits
	.global Func_02000368
	.thumb_func
Func_02000368:
	push {r5, r6, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #108]
	bl Func_0200054c
	movs r0, #0
	bl Func_020005c4
	movs r5, #8
.L_0200837c:
	adds r0, r5, #0
	bl Object_GetById
	cmp r0, #0
	beq .L_0200838e
	adds r2, r0, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
.L_0200838e:
	adds r5, #1
	cmp r5, #63
	bls .L_0200837c
	movs r3, #170
	lsls r3, r3, #1
	adds r6, r6, r3
	movs r3, #0
	ldrsh r5, [r6, r3]
	movs r0, #158
	bl Func_020005f4
	subs r5, #4
	ldr r0, .L_020083f8
	lsls r5, r5, #3
	adds r3, r5, #4
	ldrh r1, [r0, r3]
	adds r3, r3, r0
	ldrh r2, [r3, #2]
	ldr r0, [r0, r5]
	bl Func_02000534
	ldr r5, .L_020083fc
	movs r3, #133
	lsls r3, r3, #2
	adds r5, r5, r3
	movs r1, #128
	movs r2, #128
	lsls r2, r2, #7
	ldr r0, [r5]
	lsls r1, r1, #8
	bl ObjectMotion_SetSpeedParameters
	ldr r0, [r5]
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r1, #2
	ldr r0, [r5]
	bl Object_SetModeById
	movs r3, #0
	ldrsh r0, [r6, r3]
	bl Func_020005a4
	bl Event_ClearStatus1c6
	bl Event_WaitValue1c8Frames
	bl Func_02000554
	pop {r5, r6, pc}
.L_020083f8:
	.4byte Data_0200064c
.L_020083fc:
	.4byte gPartyState
	.section .text.x02008400,"ax",%progbits
	.global Func_02000400
	.thumb_func
Func_02000400:
	push {lr}
	bl Func_0200054c
	movs r0, #0
	bl Func_020005c4
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	bl Func_02000554
	pop {pc}
	.section .text.x0200841c,"ax",%progbits
	.global Func_0200041c
	.thumb_func
Func_0200041c:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #108]
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #49
	adds r1, r2, r3
	movs r3, #0
	strb r3, [r1]
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #50
	adds r1, r2, r3
	movs r3, #24
	strb r3, [r1]
	movs r3, #208
	lsls r3, r3, #4
	adds r3, #51
	adds r2, r2, r3
	movs r3, #25
	strb r3, [r2]
	bx lr
	.section .text.x02008448,"ax",%progbits
	.global Func_02000448
	.thumb_func
Func_02000448:
	push {lr}
	movs r0, #27
	movs r1, #0
	movs r2, #0
	bl Func_02000574
	movs r1, #208
	movs r2, #208
	movs r0, #65
	lsls r1, r1, #15
	lsls r2, r2, #15
	bl Func_020005bc
	pop {pc}
	.section .text.x02008464,"ax",%progbits
	.global Func_02000464
	.thumb_func
Func_02000464:
	push {lr}
	movs r0, #65
	movs r1, #0
	movs r2, #0
	bl Func_020005bc
	movs r1, #208
	movs r2, #208
	movs r0, #27
	lsls r1, r1, #15
	lsls r2, r2, #15
	bl Func_02000574
	pop {pc}
	.section .text.x02008480,"ax",%progbits
	.global Func_02000480
	.thumb_func
Func_02000480:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	subs r2, #172
	str r2, [r3]
	movs r0, #9
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #10
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #23
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #23
	bl Object_GetById
	ldr r3, .L_0200851c
	movs r5, #128
	str r3, [r0, #28]
	movs r0, #27
	bl Object_GetById
	lsls r5, r5, #8
	str r5, [r0, #24]
	movs r0, #27
	bl Object_GetById
	str r5, [r0, #28]
	bl Func_02000464
	movs r0, #129
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020084f0
	movs r0, #23
	movs r1, #0
	movs r2, #0
	bl Func_02000574
.L_020084f0:
	bl Func_020005cc
	movs r1, #128
	lsls r1, r1, #2
	movs r2, #24
	movs r3, #25
	adds r1, #2
	movs r0, #0
	bl Func_020005d4
	movs r0, #24
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26
	movs r0, #25
	bl Func_0200059c
	movs r0, #0
	pop {r5, pc}
	.2byte 0x0000
.L_0200851c:
	.4byte 0x00019999
	.section .text.x02008520,"ax",%progbits
	.global Func_02000520
	.thumb_func
Func_02000520:
	movs r0, #0
	bx lr
	.section .rodata.x020085fc,"a",%progbits
	.4byte Data_02000000 + 0x1a
	.4byte 0x0000ffff
.L_02008604:
	.4byte 0x00000027
	.4byte 0x00000002
	.4byte 0x00000011
.L_02008610:
	.4byte 0x00000027
	.4byte 0x00000000
	.4byte 0x00000016
	.4byte 0x0000001a
	.4byte 0x00000000
	.4byte 0x00000017
	.4byte 0x00000006
	.4byte 0x00180000
	.4byte 0x00000011
.L_02008634:
	.4byte 0x00000027
	.4byte 0x00000003
	.4byte 0x00000011
.L_02008640:
	.4byte 0x002a0008
	.4byte 0x00020001
	.4byte 0xffff0006
	.global Data_0200064c
Data_0200064c:
	.4byte .L_02008640
	.4byte 0x00330012
	.4byte .L_02008640
	.4byte 0x00220015
	.global gSceneEntrances
gSceneEntrances:
	.4byte 0xffff0000
	.4byte 0x000000e0
	.4byte 0x400000be
	.4byte 0xffff0000
	.4byte 0xffffffff
	.4byte 0x0000ffff
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global gSceneExits
gSceneExits:
	.4byte 0x00000069
	.4byte 0x1010106a
	.4byte 0xffffffff
	.4byte 0x1020206a
	.4byte 0xffffffff
	.4byte 0x1030306a
	.4byte 0xffffffff
	.4byte 0x1040406a
	.4byte 0xffffffff
	.4byte 0x1050506a
	.4byte 0xffffffff
	.4byte 0x10617002
	.4byte 0xffffffff
	.4byte 0x10718002
	.4byte 0xffffffff
	.4byte 0x000001ff
	.global Data_020006cc
Data_020006cc:
	.4byte 0xffff016d
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00f60000
	.4byte 0x00004000
	.4byte 0xffff0130
	.4byte .L_02008604
	.4byte 0x013c0000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0xffff0130
	.4byte .L_02008604
	.4byte 0x01740000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0xffff019a
	.4byte .L_02008610
	.4byte 0x013c0000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0xffff019a
	.4byte .L_02008610
	.4byte 0x01740000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00014000
	.4byte 0xffff0074
	.4byte 0x00000003
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00014000
	.4byte 0xffff0080
	.4byte 0x00000003
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x0000e000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00012000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x00012000
	.4byte 0xffff0088
	.4byte 0x00000002
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x0000e000
	.4byte 0xffff0093
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00016000
	.4byte 0xffff0070
	.4byte 0x00000002
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0xffff006e
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00010000
	.4byte 0xffff0133
	.4byte .L_02008634
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00014000
	.4byte 0xffff0148
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020008c4
Data_020008c4:
	.4byte 0xffff016c
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00f60000
	.4byte 0x00004000
	.4byte 0xffff0131
	.4byte 0x00000001
	.4byte 0x013c0000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0xffff0131
	.4byte 0x00000001
	.4byte 0x01740000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0xffff019a
	.4byte 0x00000001
	.4byte 0x013c0000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0xffff019a
	.4byte 0x00000001
	.4byte 0x01740000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02000954
Data_02000954:
	.4byte 0xffff016d
	.4byte 0x00000001
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0x00f60000
	.4byte 0x00004000
	.4byte 0xffff0130
	.4byte .L_02008604
	.4byte 0x013c0000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0xffff0130
	.4byte .L_02008604
	.4byte 0x01740000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0xffff019a
	.4byte .L_02008610
	.4byte 0x013c0000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0xffff019a
	.4byte .L_02008610
	.4byte 0x01740000
	.4byte 0x00000000
	.4byte 0x00840000
	.4byte 0x00004000
	.4byte 0xffff0073
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x01580000
	.4byte 0x00014000
	.4byte 0xffff0074
	.4byte 0x00000003
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x01280000
	.4byte 0x00000000
	.4byte 0xffff0072
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00980000
	.4byte 0x00014000
	.4byte 0xffff0080
	.4byte 0x00000003
	.4byte 0x00580000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x0000e000
	.4byte 0xffff0070
	.4byte 0x00000001
	.4byte 0x00f80000
	.4byte 0x00000000
	.4byte 0x00580000
	.4byte 0x00012000
	.4byte 0xffff0071
	.4byte 0x00000001
	.4byte 0x01700000
	.4byte 0x00000000
	.4byte 0x00e00000
	.4byte 0x00012000
	.4byte 0xffff0088
	.4byte 0x00000002
	.4byte 0x00c80000
	.4byte 0x00000000
	.4byte 0x00c80000
	.4byte 0x0000e000
	.4byte 0xffff0093
	.4byte 0x00000001
	.4byte 0x01380000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00016000
	.4byte 0xffff0070
	.4byte 0x00000002
	.4byte 0x00380000
	.4byte 0x00000000
	.4byte 0x00e80000
	.4byte 0x00000000
	.4byte 0xffff006e
	.4byte 0x00000001
	.4byte 0x00a80000
	.4byte 0x00000000
	.4byte 0x01680000
	.4byte 0x00010000
	.4byte 0xffff0133
	.4byte .L_02008634
	.4byte 0x00280000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00024000
	.4byte 0xffff0197
	.4byte 0x00000001
	.4byte 0x01680000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff01e9
	.4byte 0x00000001
	.4byte 0x01880000
	.4byte 0x00000000
	.4byte 0x01780000
	.4byte 0x00024000
	.4byte 0xffff00c9
	.4byte 0x00000001
	.4byte 0x01780000
	.4byte 0x00000000
	.4byte 0x00a80000
	.4byte 0x00014000
	.4byte 0xffff0148
	.4byte 0x00000001
	.4byte 0x00680000
	.4byte 0x00000000
	.4byte 0x00680000
	.4byte 0x00014000
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02000b4c
Data_02000b4c:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte Func_02000368
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte Func_02000368
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001d8a
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte Func_020000d8
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001d8f
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001d90
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001d91
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte Func_02000140
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte Func_020001a8
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00001d99
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x00001da2
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte Func_020001f0
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte Func_020002b0
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001d9a
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001d9b
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001d9c
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001d9d
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001d9e
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00001d9f
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001da0
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00001da1
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00001da3
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00001e05
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte 0x00001e0c
	.4byte 0x000001f3
	.4byte 0xffff00c8
	.4byte 0x00403041
	.4byte 0x10009a15
	.4byte 0xffff0008
	.4byte Func_02000088
	.4byte 0x00004e15
	.4byte Data_02010002 + 0x15
	.4byte Func_02000400
	.4byte 0x00008515
	.4byte Data_02020004 + 0x14
	.4byte 0x00000000
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte Func_02000448
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte Func_02000464
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02000cfc
Data_02000cfc:
	.4byte 0x00000001
	.4byte 0xffff0001
	.4byte 0x00000001
	.4byte 0x00000001
	.4byte 0xffff0002
	.4byte 0x00000002
	.4byte 0x00000001
	.4byte 0xffff0003
	.4byte 0x00000003
	.4byte 0x0000c602
	.4byte 0xffff0004
	.4byte Func_02000368
	.4byte 0x0000c602
	.4byte 0xffff0005
	.4byte Func_02000368
	.4byte 0x00000001
	.4byte 0xffff0006
	.4byte 0x00000006
	.4byte 0x00000001
	.4byte 0xffff0007
	.4byte 0x00000007
	.4byte 0x00000000
	.4byte 0xffff000d
	.4byte 0x00001e0d
	.4byte 0x00000000
	.4byte 0xffff000e
	.4byte 0x00001e0e
	.4byte 0x00000000
	.4byte 0xffff000f
	.4byte 0x00001e0f
	.4byte 0x00000000
	.4byte 0xffff0010
	.4byte 0x00001e10
	.4byte 0x00000000
	.4byte 0xffff0011
	.4byte 0x00001e11
	.4byte 0x00000000
	.4byte 0xffff0012
	.4byte 0x00001e12
	.4byte 0x00000000
	.4byte 0xffff0013
	.4byte 0x00001e13
	.4byte 0x00000000
	.4byte 0xffff0014
	.4byte 0x00001e14
	.4byte 0x00000000
	.4byte 0xffff0015
	.4byte 0x00001e1d
	.4byte 0x00000000
	.4byte 0xffff0016
	.4byte Func_02000250
	.4byte 0x00000000
	.4byte 0xffff001a
	.4byte Func_0200030c
	.4byte 0x00008d15
	.4byte 0xffff000d
	.4byte 0x00001e15
	.4byte 0x00008d15
	.4byte 0xffff000e
	.4byte 0x00001e16
	.4byte 0x00008d15
	.4byte 0xffff000f
	.4byte 0x00001e17
	.4byte 0x00008d15
	.4byte 0xffff0010
	.4byte 0x00001e18
	.4byte 0x00008d15
	.4byte 0xffff0011
	.4byte 0x00001e19
	.4byte 0x00008d15
	.4byte 0xffff0012
	.4byte 0x00001e1a
	.4byte 0x00008d15
	.4byte 0xffff0013
	.4byte 0x00001e1b
	.4byte 0x00008d15
	.4byte 0xffff0014
	.4byte 0x00001e1c
	.4byte 0x00008d15
	.4byte 0xffff0015
	.4byte 0x00001e1e
	.4byte 0x00008d15
	.4byte 0xffff0016
	.4byte 0x00001e2d
	.4byte 0x00008d15
	.4byte 0xffff001a
	.4byte 0x00001e34
	.4byte 0x000001f3
	.4byte 0xffff00c8
	.4byte 0x00403041
	.4byte 0x10009a15
	.4byte 0xffff0008
	.4byte Func_02000088
	.4byte 0x00004e15
	.4byte Data_02010002 + 0x15
	.4byte Func_02000400
	.4byte 0x00008515
	.4byte Data_02020004 + 0x14
	.4byte 0x00000000
	.4byte 0x50009085
	.4byte 0xffff0000
	.4byte Func_02000448
	.4byte 0x40009085
	.4byte 0xffff0000
	.4byte Func_02000464
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
