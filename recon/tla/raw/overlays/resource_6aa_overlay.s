.syntax unified
	.thumb
	.section .text.x020081f6,"ax",%progbits
	.2byte 0x0000
	.section .text.x020081f8,"ax",%progbits
	.global Func_020001f8
	.thumb_func
Func_020001f8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	ldr r3, [r5, #8]
	movs r0, #0
	asrs r3, r3, #20
	mov r9, r3
	ldr r3, [r5, #16]
	mov r1, r9
	asrs r3, r3, #20
	mov r10, r3
	mov r2, r10
	bl Func_0200012c
	mov r1, r9
	mov r2, r10
	mov r8, r0
	movs r0, #2
	bl Func_0200012c
	movs r2, #34
	adds r2, r2, r5
	adds r6, r0, #0
	mov r11, r2
	ldrb r0, [r2]
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	bl Map_GetTerrainHeight
	adds r3, r5, #0
	adds r3, #100
	asrs r7, r0, #19
	movs r2, #0
	ldrsh r0, [r3, r2]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02008268
	ldr r3, .L_020082b8
	mov r2, r8
	ands r6, r3
	movs r3, #129
	negs r3, r3
	ands r2, r3
	ldr r3, [r5, #20]
	mov r8, r2
	asrs r3, r3, #19
	cmp r3, r7
	beq .L_02008282
	subs r7, #4
	b .L_02008282
.L_02008268:
	movs r3, #255
	ands r6, r3
	lsls r3, r3, #8
	mov r2, r8
	orrs r6, r3
	movs r3, #128
	orrs r2, r3
	adds r0, r5, #0
	movs r1, #2
	mov r8, r2
	adds r7, #4
	bl Object_SetSpritePriority
.L_02008282:
	mov r1, r9
	mov r2, r10
	mov r3, r8
	movs r0, #0
	bl Func_02000168
	mov r1, r9
	mov r2, r10
	adds r3, r6, #0
	movs r0, #2
	bl Func_02000168
	mov r3, r9
	mov r2, r10
	lsls r0, r3, #20
	mov r3, r11
	lsls r1, r2, #20
	ldrb r2, [r3]
	adds r3, r7, #0
	bl Func_02004c80
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_020082b8:
	.4byte 0xffff00ff
	.section .text.x020082bc,"ax",%progbits
	.global Func_020002bc
	.thumb_func
Func_020002bc:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r2, #255
	ldrh r3, [r0]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	beq .L_02008330
	adds r7, r0, #0
.L_020082d0:
	ldrh r0, [r7]
	bl Object_GetById
	movs r3, #4
	ldrsh r2, [r7, r3]
	movs r1, #0
	mov r8, r2
	adds r6, r0, #0
	movs r3, #2
	ldrsh r5, [r7, r3]
	bl ObjectDispatch_SetSingleChildFlags
	mov r2, r8
	lsls r0, r2, #16
	lsrs r0, r0, #16
	bl GameFlag_Test
	lsls r5, r5, #16
	lsrs r5, r5, #16
	adds r5, r5, r0
	adds r1, r5, #0
	adds r0, r6, #0
	bl Func_02004bc0
	adds r2, r6, #0
	adds r2, #85
	movs r3, #0
	strb r3, [r2]
	adds r1, r6, #0
	adds r1, #89
	ldrb r2, [r1]
	movs r3, #254
	ands r3, r2
	strb r3, [r1]
	adds r3, r6, #0
	adds r3, #100
	mov r2, r8
	strh r2, [r3]
	adds r0, r6, #0
	adds r7, #6
	bl Func_020001f8
	movs r2, #255
	ldrh r3, [r7]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_020082d0
.L_02008330:
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x02008338,"ax",%progbits
	.global Func_02000338
	.thumb_func
Func_02000338:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r3, [r3]
	mov r9, r0
	movs r2, #26
	ldrsh r0, [r3, r2]
	sub sp, #56
	adds r6, r1, #0
	mov r10, r3
	bl Object_GetById
	adds r7, r0, #0
	cmp r6, #7
	bgt .L_020083cc
	add r5, sp, #16
	movs r3, #0
	mov r8, r3
	str r3, [r5]
	movs r3, #24
	adds r3, #255
	strh r3, [r5, #24]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #12]
	str r3, [r5, #8]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r5, #20]
	str r3, [r5, #16]
	bl Random16Far
	ldr r3, .L_020083c0
	ands r0, r3
	lsls r0, r0, #12
	strh r0, [r5, #32]
	bl Random16Far
	ldr r1, [r7, #12]
	lsls r2, r6, #1
	adds r2, r2, r6
	lsls r2, r2, #16
	movs r4, #128
	subs r1, r1, r2
	lsls r4, r4, #13
	movs r3, #15
	ands r3, r0
	adds r1, r1, r4
	mov r4, r8
	ldr r2, [r7, #16]
	ldr r0, [r7, #8]
	subs r3, #8
	str r4, [sp, #0]
	str r4, [sp, #4]
	movs r4, #180
	lsls r3, r3, #13
	lsls r4, r4, #15
	str r4, [sp, #8]
	str r5, [sp, #12]
	bl Func_02003b30
	b .L_020083c4
	.2byte 0x0000
.L_020083c0:
	.4byte 0x0000000f
.L_020083c4:
	ldr r3, [r7, #28]
	ldr r2, .L_02008408
	adds r3, r3, r2
	str r3, [r7, #28]
.L_020083cc:
	mov r3, r9
	cmp r3, #1
	bne .L_020083fa
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #28]
	adds r3, r7, #0
	adds r3, #100
	movs r4, #0
	ldrsh r0, [r3, r4]
	bl GameFlag_SetBit
	mov r3, r10
	movs r2, #26
	ldrsh r0, [r3, r2]
	bl Object_GetById
	bl Func_020001f8
	movs r3, #0
	str r3, [r7, #16]
	str r3, [r7, #12]
	str r3, [r7, #8]
.L_020083fa:
	add sp, #56
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008408:
	.4byte 0xffffe100
	.section .text.x02008414,"ax",%progbits
	.balign 4
	.global Func_02000414
	.thumb_func
Func_02000414:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	bl Object_GetById
	movs r1, #5
	mov r9, r0
	mov r11, r1
.L_0200842c:
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	mov r2, r9
	ldr r2, [r2, #8]
	lsls r5, r5, #4
	mov r8, r2
	add r8, r5
	lsls r0, r0, #4
	mov r3, r8
	subs r3, r3, r0
	mov r8, r3
	bl Random16Far
	adds r6, r0, #0
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	mov r1, r9
	ldr r2, [r1, #12]
	ldr r3, [r1, #16]
	lsls r6, r6, #3
	adds r6, r6, r2
	lsls r5, r5, #4
	movs r2, #128
	lsls r0, r0, #4
	lsls r2, r2, #11
	adds r3, r3, r5
	subs r3, r3, r0
	adds r6, r6, r2
	movs r0, #70
	adds r0, #255
	mov r1, r8
	adds r2, r6, #0
	bl Func_02004bd8
	adds r7, r0, #0
	cmp r7, #0
	beq .L_020084fc
	bl Random16Far
	mov r8, r0
	bl Random16Far
	adds r6, r0, #0
	bl Random16Far
	movs r3, #128
	adds r5, r0, #0
	lsls r3, r3, #7
	adds r0, r7, #0
	ldr r1, .L_02008514
	lsrs r5, r5, #1
	adds r5, r5, r3
	bl Func_02004bd0
	adds r0, r7, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	mov r1, r8
	movs r2, #128
	lsls r2, r2, #10
	lsls r3, r1, #2
	adds r3, r3, r2
	str r3, [r7, #40]
	mov r0, r8
	bl Math_Cosine
	ldr r3, .L_02008518
	lsls r6, r6, #3
	adds r1, r0, #0
	mov r10, r3
	adds r0, r6, #0
	mov lr, r10
	.2byte 0xf800
	str r0, [r7, #44]
	mov r0, r8
	bl Math_Sine
	adds r1, r0, #0
	adds r0, r6, #0
	mov lr, r10
	.2byte 0xf800
	movs r3, #0
	str r3, [r7, #52]
	movs r3, #168
	lsls r3, r3, #7
	adds r3, #122
	str r3, [r7, #72]
	movs r3, #128
	lsls r3, r3, #8
	str r0, [r7, #36]
	str r5, [r7, #24]
	str r5, [r7, #28]
	str r3, [r7, #68]
	adds r0, r7, #0
	movs r1, #1
	bl Animation_ApplyChildValues
.L_020084fc:
	movs r1, #1
	negs r1, r1
	add r11, r1
	mov r2, r11
	cmp r2, #0
	bge .L_0200842c
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_02008514:
	.4byte Data_02005394
.L_02008518:
	.4byte IwramMulQ16
	.section .text.x0200896c,"ax",%progbits
	.balign 4
	.global Func_0200096c
	.thumb_func
Func_0200096c:
	push {r5, lr}
	adds r0, r1, #0
	bl Object_GetById
	adds r5, r0, #0
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #11
	bne .L_02008998
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r5, #48]
	movs r3, #128
	lsls r3, r3, #6
	str r3, [r5, #52]
	movs r1, #152
	movs r3, #198
	ldr r2, [r5, #12]
	lsls r1, r1, #16
	lsls r3, r3, #18
	bl Func_02004bf8
.L_02008998:
	ldr r3, [r5, #8]
	asrs r3, r3, #20
	cmp r3, #10
	bne .L_020089bc
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r5, #48]
	movs r3, #128
	lsls r3, r3, #6
	str r3, [r5, #52]
	movs r1, #200
	movs r3, #198
	ldr r2, [r5, #12]
	adds r0, r5, #0
	lsls r1, r1, #16
	lsls r3, r3, #18
	bl Func_02004bf8
.L_020089bc:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x02008d18,"ax",%progbits
	.balign 4
	.global Func_02000d18
	.thumb_func
Func_02000d18:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	ldr r3, .L_02008dd8
	adds r2, r1, #0
	adds r2, #228
	ldr r6, [r2]
	ldr r2, [r2, #4]
	ands r6, r3
	ands r2, r3
	ldr r3, [r1]
	mov r8, r2
	ldr r3, [r3, #4]
	ldr r2, .L_02008ddc
	mov r10, r3
	ldr r3, .L_02008de0
	ldrh r3, [r3]
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	movs r2, #133
	lsrs r7, r3, #5
	ldr r3, .L_02008de4
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r5, r0, #0
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	asrs r3, r1, #20
	lsls r3, r3, #20
	movs r0, #0
	subs r6, r3, r6
	bl Map_GetTerrainHeight
	mov r3, r10
	subs r0, r0, r3
	ldr r3, [r5, #16]
	mov r2, r8
	asrs r3, r3, #20
	lsls r3, r3, #20
	subs r3, r3, r2
	mov r2, r10
	subs r3, r3, r2
	subs r4, r3, r0
	asrs r2, r4, #16
	adds r0, r0, r3
	asrs r6, r6, #16
	adds r4, r2, #0
	asrs r0, r0, #16
	adds r3, r6, #0
	movs r2, #167
	adds r1, r0, #0
	adds r3, #15
	lsls r2, r2, #1
	adds r4, #16
	adds r1, #58
	cmp r3, r2
	bhi .L_02008dd0
	movs r3, #15
	negs r3, r3
	cmp r4, r3
	blt .L_02008dd0
	cmp r4, #239
	bgt .L_02008dd0
	movs r3, #128
	lsls r3, r3, #1
	ldr r2, .L_02008de8
	adds r3, #255
	ands r6, r3
	movs r3, #255
	ands r4, r3
	movs r3, #0
	stmia r2!, {r3}
	lsls r3, r6, #16
	orrs r4, r3
	ldr r3, .L_02008dec
	ldr r0, .L_02008de8
	orrs r4, r3
	stmia r2!, {r4}
	movs r3, #128
	lsls r3, r3, #3
	orrs r7, r3
	str r7, [r2]
	bl Func_02004b80
.L_02008dd0:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_02008dd8:
	.4byte 0xffff0000
.L_02008ddc:
	.4byte ResourceTableEntries
.L_02008de0:
	.4byte Data_020068a2
.L_02008de4:
	.4byte gPartyState
.L_02008de8:
	.4byte Data_020068a4
.L_02008dec:
	.4byte 0x80008800
	.section .text.x02008df0,"ax",%progbits
	.global Func_02000df0
	.thumb_func
Func_02000df0:
	push {r5, r6, lr}
	movs r0, #128
	lsls r0, r0, #1
	bl MemAlloc
	adds r6, r0, #0
	adds r1, r6, #0
	movs r2, #63
.L_02008e00:
	ldr r3, .L_02008e30
	subs r2, #1
	stmia r1!, {r3}
	cmp r2, #0
	bge .L_02008e00
	ldr r5, .L_02008e34
	bl Resource_FindFreeEntry
	strh r0, [r5]
	movs r1, #128
	lsls r1, r1, #1
	adds r2, r6, #0
	ldrh r0, [r5]
	bl VramBlock_LoadCached
	adds r0, r6, #0
	bl MemFree
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02008e38
	bl Scheduler_AddOrUpdateCallback
	pop {r5, r6, pc}
.L_02008e30:
	.4byte 0x11111111
.L_02008e34:
	.4byte Data_020068a2
.L_02008e38:
	.4byte Func_02000d18
	.section .text.x02008e3c,"ax",%progbits
	.global Func_02000e3c
	.thumb_func
Func_02000e3c:
	push {r5, r6, r7, lr}
	ldr r5, .L_02008ecc
	movs r1, #133
	lsls r1, r1, #2
	adds r5, r5, r1
	adds r7, r0, #0
	ldr r0, [r5]
	bl Object_GetById
	adds r6, r0, #0
	bl Func_02004ca8
	movs r0, #0
	bl Func_02004da0
	ldr r0, [r5]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r3, #0
	movs r1, #128
	str r3, [r6, #108]
	movs r2, #0
	lsls r1, r1, #1
	ldr r0, [r5]
	bl Func_02004d00
	adds r0, r6, #0
	movs r1, #27
	bl Func_02004bc0
	movs r1, #48
	adds r0, r6, #0
	bl ObjectDispatch_ApplyValueToChildren
	movs r0, #30
	bl WaitFrames
	movs r3, #128
	ldr r2, .L_02008ec8
	lsls r3, r3, #7
	strh r3, [r6, #6]
	adds r3, r6, #0
	adds r3, #85
	strb r2, [r3]
	ldr r2, .L_02008ed0
	ldr r3, [r6, #8]
	asrs r4, r3, #20
	ldr r3, [r6, #16]
	asrs r0, r3, #20
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #32]
	cmp r1, #0
	beq .L_02008ed4
	adds r3, r6, #0
	adds r3, #34
	ldrb r3, [r3]
	movs r2, #3
	ands r2, r3
	lsls r3, r2, #3
	subs r3, r3, r2
	movs r2, #156
	lsls r2, r2, #1
	lsls r3, r3, #3
	adds r3, r3, r2
	ldr r2, [r1, r3]
	b .L_02008ed4
.L_02008ec8:
	.4byte 0x00000000
.L_02008ecc:
	.4byte gPartyState
.L_02008ed0:
	.4byte gMapCellBuffer
.L_02008ed4:
	lsls r3, r0, #7
	adds r3, r4, r3
	lsls r3, r3, #2
	movs r1, #128
	adds r3, r2, r3
	lsls r1, r1, #2
	adds r2, r3, r1
	ldrb r2, [r2, #3]
	movs r3, #64
	ands r3, r2
	cmp r3, #0
	bne .L_02008f0a
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
	ldr r2, .L_02008f38
	orrs r3, r2
	strh r3, [r1]
	bl Func_02000df0
.L_02008f0a:
	ldr r3, .L_02008f3c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	movs r1, #28
	bl Object_SetModeById
	movs r0, #16
	bl Battle_WaitMode0
	movs r5, #0
.L_02008f22:
	cmp r5, #5
	bne .L_02008f2c
	movs r0, #204
	bl Func_02004de0
.L_02008f2c:
	ldr r3, [r6, #24]
	ldr r1, .L_02008f40
	ldr r2, .L_02008f44
	adds r3, r3, r1
	str r3, [r6, #24]
	b .L_02008f48
.L_02008f38:
	.4byte 0x00008000
.L_02008f3c:
	.4byte gPartyState
.L_02008f40:
	.4byte 0xfffffc00
.L_02008f44:
	.4byte 0xfffffd00
.L_02008f48:
	ldr r3, [r6, #28]
	ldr r1, .L_02008f9c
	adds r3, r3, r2
	str r3, [r6, #28]
	ldr r3, [r6, #12]
	movs r0, #1
	adds r3, r3, r1
	str r3, [r6, #12]
	adds r5, #1
	bl WaitFrames
	cmp r5, #39
	ble .L_02008f22
	ldr r3, .L_02008fa0
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r3, #0
	adds r0, #84
	strb r3, [r0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	movs r1, #214
	movs r2, #133
	lsls r1, r1, #1
	lsls r2, r2, #1
	adds r3, r3, r1
	adds r2, #255
	str r2, [r3]
	bl Event_FinishScreenTransition
	bl Event_WaitTransitionFrames
	adds r0, r7, #0
	bl Func_02004d30
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008f9c:
	.4byte 0xffff6667
.L_02008fa0:
	.4byte gPartyState
	.section .text.x02008fe0,"ax",%progbits
	.balign 4
	.global Func_02000fe0
	.thumb_func
Func_02000fe0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_02009098
	movs r2, #133
	lsls r2, r2, #2
	adds r2, r2, r3
	mov r10, r0
	ldr r0, [r2]
	mov r8, r2
	bl Object_GetById
	adds r6, r0, #0
	ldr r3, [r6, #8]
	ldr r5, [r6, #16]
	asrs r7, r3, #20
	movs r0, #208
	lsls r0, r0, #2
	adds r1, r7, #0
	bl GameFlag_SetByte
	asrs r5, r5, #20
	movs r0, #210
	adds r1, r5, #0
	lsls r0, r0, #2
	bl GameFlag_SetByte
	bl Func_02004ca8
	movs r0, #0
	bl Func_02004da0
	mov r3, r8
	ldr r0, [r3]
	bl Object_GetById
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r3, #0
	mov r2, r8
	movs r1, #128
	str r3, [r6, #108]
	ldr r0, [r2]
	lsls r1, r1, #1
	movs r2, #0
	bl Func_02004d00
	adds r0, r6, #0
	movs r1, #27
	bl Func_02004bc0
	adds r0, r6, #0
	movs r1, #48
	bl ObjectDispatch_ApplyValueToChildren
	movs r0, #30
	bl WaitFrames
	movs r3, #128
	ldr r2, .L_02009090
	lsls r3, r3, #7
	strh r3, [r6, #6]
	adds r3, r6, #0
	adds r3, #85
	strb r2, [r3]
	mov r3, r10
	ldrb r2, [r3, #3]
	movs r3, #64
	ands r3, r2
	cmp r3, #0
	bne .L_020090a0
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
	ldr r2, .L_02009094
	orrs r3, r2
	strh r3, [r1]
	b .L_0200909c
	.2byte 0x0000
.L_02009090:
	.4byte 0x00000000
.L_02009094:
	.4byte 0x00008000
.L_02009098:
	.4byte gPartyState
.L_0200909c:
	bl Func_02000df0
.L_020090a0:
	mov r2, r8
	ldr r0, [r2]
	movs r1, #28
	bl Object_SetModeById
	movs r0, #16
	bl Battle_WaitMode0
	movs r7, #0
.L_020090b2:
	cmp r7, #5
	bne .L_020090bc
	movs r0, #204
	bl Func_02004de0
.L_020090bc:
	ldr r3, [r6, #24]
	ldr r2, .L_0200911c
	movs r0, #1
	adds r3, r3, r2
	str r3, [r6, #24]
	ldr r2, .L_02009120
	ldr r3, [r6, #28]
	adds r7, #1
	adds r3, r3, r2
	str r3, [r6, #28]
	ldr r2, .L_02009124
	ldr r3, [r6, #12]
	adds r3, r3, r2
	str r3, [r6, #12]
	bl WaitFrames
	cmp r7, #39
	ble .L_020090b2
	ldr r3, .L_02009128
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r3, #0
	adds r0, #84
	strb r3, [r0]
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
	movs r0, #11
	bl Func_02004d30
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200911c:
	.4byte 0xfffffc00
.L_02009120:
	.4byte 0xfffffd00
.L_02009124:
	.4byte 0xffff6667
.L_02009128:
	.4byte gPartyState
	.section .text.x02009254,"ax",%progbits
	.balign 4
	.global Func_02001254
	.thumb_func
Func_02001254:
	push {r5, lr}
	ldr r3, .L_0200927c
	ldr r5, .L_02009280
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #59
	bgt .L_02009284
	movs r2, #0
	ldrsh r3, [r5, r2]
	cmp r3, #0
	bne .L_02009298
	movs r0, #0
	movs r1, #3
	bl Func_0200246c
	ldr r3, .L_02009278
	b .L_02009296
	.2byte 0x0000
.L_02009278:
	.4byte 0x00000001
.L_0200927c:
	.4byte Data_020068a0
.L_02009280:
	.4byte Data_02006868
.L_02009284:
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, #1
	bne .L_02009298
	movs r0, #0
	movs r1, #67
	bl Func_0200246c
	ldr r3, .L_020092b0
.L_02009296:
	strh r3, [r5]
.L_02009298:
	ldr r2, .L_020092b4
	movs r1, #150
	ldrh r3, [r2]
	lsls r1, r1, #17
	adds r3, #1
	strh r3, [r2]
	lsls r3, r3, #16
	cmp r3, r1
	ble .L_020092b8
	ldr r3, .L_020092b0
	strh r3, [r2]
	b .L_020092b8
.L_020092b0:
	.4byte 0x00000000
.L_020092b4:
	.4byte Data_020068a0
.L_020092b8:
	pop {r5, pc}
	.2byte 0x0000
	.section .text.x020092e4,"ax",%progbits
	.balign 4
	.global Func_020012e4
	.thumb_func
Func_020012e4:
	push {lr}
	ldr r2, [r0, #80]
	movs r1, #128
	ldrh r3, [r2, #18]
	lsls r1, r1, #4
	adds r3, r3, r1
	adds r1, r0, #0
	adds r1, #100
	strh r3, [r2, #18]
	ldrh r3, [r1]
	movs r2, #128
	adds r3, #1
	strh r3, [r1]
	lsls r2, r2, #12
	lsls r3, r3, #16
	movs r4, #0
	cmp r3, r2
	bne .L_0200930e
	movs r3, #128
	lsls r3, r3, #11
	str r3, [r0, #40]
.L_0200930e:
	movs r2, #0
	ldrsh r3, [r1, r2]
	cmp r3, #9
	ble .L_0200932e
	ldr r3, [r0, #8]
	movs r2, #128
	lsls r2, r2, #11
	adds r3, r3, r2
	str r3, [r0, #8]
	ldr r2, .L_02009340
	ldr r3, [r0, #24]
	adds r3, r3, r2
	str r3, [r0, #24]
	ldr r3, [r0, #28]
	adds r3, r3, r2
	str r3, [r0, #28]
.L_0200932e:
	movs r2, #0
	ldrsh r3, [r1, r2]
	cmp r3, #32
	bne .L_0200933c
	str r4, [r0, #8]
	str r4, [r0, #16]
	str r4, [r0, #108]
.L_0200933c:
	pop {pc}
	.2byte 0x0000
.L_02009340:
	.4byte 0xfffff800
	.section .text.x0200938c,"ax",%progbits
	.balign 4
	.global Func_0200138c
	.thumb_func
Func_0200138c:
	.ifdef TLA_EDITION_JA
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r2, .L_ja_02009730
	movs r0, #240
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r0, #0
	ldrsh r1, [r3, r0]
	movs r0, #214
	mov r8, r1
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r2, #0
	ldrsh r7, [r3, r2]
	ldr r2, .L_ja_02009734
	movs r3, #0
	str r3, [r2, #0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	lsls r0, r0, #1
	movs r2, #129
	adds r3, r3, r0
	lsls r2, r2, #2
	str r2, [r3, #0]
	subs r0, #154
	sub sp, #8
	bl GameFlag_SetBit
	bl Func_02002270
	ldr r3, .L_ja_02009738
	cmp r8, r3
	bne .L_ja_020093dc
	movs r0, #1
	bl Func_02004dd8
	b .L_ja_020098c6
.L_ja_020093dc:
	ldr r3, .L_ja_0200973c
	cmp r8, r3
	bne .L_ja_02009454
	movs r0, #0
	bl Func_02004d88
	movs r0, #11
	movs r1, #3
	bl ObjectMotion_SetActionVariant
	movs r0, #10
	bl Func_02001234
	movs r0, #163
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_02009428
	movs r3, #53
	movs r5, #3
	str r3, [sp, #4]
	movs r0, #35
	movs r1, #41
	movs r2, #24
	movs r3, #10
	str r5, [sp, #0]
	bl Func_02004c30
	movs r3, #117
	str r3, [sp, #4]
	movs r0, #35
	movs r1, #105
	movs r2, #24
	movs r3, #10
	str r5, [sp, #0]
	bl Func_02004c30
.L_ja_02009428:
	ldr r0, .L_ja_02009740
	bl Func_020002bc
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #62
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_0200943e
	b .L_ja_020098c6
.L_ja_0200943e:
	movs r3, #39
	movs r2, #85
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #50
	movs r1, #125
	movs r2, #3
	movs r3, #3
	bl Func_02004c30
	b .L_ja_020098c6
.L_ja_02009454:
	ldr r3, .L_ja_02009744
	cmp r8, r3
	beq .L_ja_0200945c
	b .L_ja_02009688
.L_ja_0200945c:
	movs r0, #0
	bl Func_02004d88
	ldr r0, .L_ja_02009748
	ldr r1, .L_ja_0200974c
	movs r2, #0
	movs r3, #88
	bl Func_020022e4
	ldr r0, .L_ja_02009750
	bl Func_020002bc
	movs r0, #148
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_020094b6
	movs r3, #47
	movs r2, #100
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #47
	movs r1, #94
	movs r2, #10
	movs r3, #6
	bl Func_02004c30
	movs r3, #46
	movs r2, #40
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #110
	movs r1, #40
	movs r2, #12
	movs r3, #4
	bl Func_02004c28
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_02004cd8
	b .L_ja_020094e8
.L_ja_020094b6:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #62
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_020094de
	movs r0, #140
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #18
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0, #0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0, #0]
	b .L_ja_020094e8
.L_ja_020094de:
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_02004cd8
.L_ja_020094e8:
	movs r0, #20
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0, #0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0, #0]
	movs r1, #2
	movs r0, #20
	bl ObjectMotion_SetActionVariant
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #71
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_02009544
	movs r3, #100
	str r3, [sp, #4]
	movs r5, #28
	movs r0, #28
	movs r1, #92
	movs r2, #8
	movs r3, #8
	str r5, [sp, #0]
	bl Func_02004c30
	movs r3, #38
	str r3, [sp, #4]
	movs r0, #92
	movs r1, #38
	movs r2, #8
	movs r3, #8
	str r5, [sp, #0]
	bl Func_02004c28
	bl Func_02000798
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl Func_02004cd8
	b .L_ja_0200956c
.L_ja_02009544:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #72
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_02009562
	bl Func_02000798
	movs r0, #153
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	b .L_ja_0200956c
.L_ja_02009562:
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl Func_02004cd8
.L_ja_0200956c:
	movs r0, #149
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_02009594
	movs r1, #164
	movs r2, #240
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #15
	bl Func_02004cd8
	movs r0, #8
	bl Object_GetById
	movs r3, #1
	adds r0, #98
	strb r3, [r0, #0]
.L_ja_02009594:
	movs r0, #163
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_020095ac
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl Func_02004cd8
	b .L_ja_020095b6
.L_ja_020095ac:
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_02004cd8
.L_ja_020095b6:
	cmp r7, #20
	beq .L_ja_020095bc
	b .L_ja_020098c6
.L_ja_020095bc:
	bl Func_02004ca8
	movs r0, #0
	bl Func_02004da0
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r0, r0
	negs r1, r1
	negs r2, r2
	bl Motion_CamBounds
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl Func_02004c50
	ldr r3, .L_ja_02009730
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3, #0]
	movs r1, #0
	movs r2, #0
	bl Func_02004cd8
	movs r1, #0
	movs r2, #0
	movs r0, #18
	bl Func_02004cd8
	movs r0, #18
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0, #0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0, #0]
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #18
	bl Object_GetById
	adds r3, r0, #0
	adds r3, #100
	adds r0, #102
	movs r2, #0
	ldrsh r1, [r3, r2]
	movs r3, #0
	ldrsh r2, [r0, r3]
	lsls r1, r1, #16
	lsls r2, r2, #16
	movs r0, #18
	bl Func_02004cd8
	movs r0, #159
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02004de0
	movs r5, #2
.L_ja_02009644:
	movs r0, #18
	bl Func_02000414
	subs r5, #1
	movs r0, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_ja_02009644
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #62
	bl GameFlag_SetBit
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r1, r1
	adds r2, #102
	negs r0, r0
	bl Func_02004c50
	movs r0, #120
	bl Battle_WaitMode0
	movs r0, #20
	bl Func_02004d30
	movs r0, #10
	adds r0, #255
	bl GameFlag_SetBit
	b .L_ja_020098c6
.L_ja_02009688:
	ldr r3, .L_ja_02009754
	cmp r8, r3
	bne .L_ja_020096b8
	movs r0, #0
	bl Func_02004d88
	cmp r7, #10
	beq .L_ja_0200969c
	cmp r7, #1
	bne .L_ja_020096b0
.L_ja_0200969c:
	ldr r0, .L_ja_02009758
	ldr r1, .L_ja_0200975c
	ldr r2, .L_ja_02009760
	movs r3, #88
	bl Func_020022e4
	movs r0, #0
	movs r1, #1
	bl Func_0200248c
.L_ja_020096b0:
	ldr r0, .L_ja_02009764
	bl Func_020002bc
	b .L_ja_020098c6
.L_ja_020096b8:
	ldr r3, .L_ja_02009768
	cmp r8, r3
	bne .L_ja_0200978c
	movs r0, #0
	bl Func_02004d88
	movs r0, #12
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0, #0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0, #0]
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #77
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_02009714
	movs r3, #101
	str r3, [sp, #4]
	movs r5, #12
	movs r0, #12
	movs r1, #95
	movs r2, #8
	movs r3, #6
	str r5, [sp, #0]
	bl Func_02004c30
	movs r3, #40
	str r3, [sp, #4]
	movs r0, #76
	movs r1, #40
	movs r2, #8
	movs r3, #5
	str r5, [sp, #0]
	bl Func_02004c28
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02004cd8
	b .L_ja_02009776
.L_ja_02009714:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #76
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_0200976c
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #50
	bl GameFlag_SetBit
	b .L_ja_02009776
	.2byte 0x0000
.L_ja_02009730:
	.4byte gPartyState
.L_ja_02009734:
	.4byte Data_02006878
.L_ja_02009738:
	.4byte 0x00000117
.L_ja_0200973c:
	.4byte 0x00000118
.L_ja_02009740:
	.4byte Data_020053d8
.L_ja_02009744:
	.4byte 0x00000119
.L_ja_02009748:
	.4byte Data_020053e6
.L_ja_0200974c:
	.4byte Data_020053f8
.L_ja_02009750:
	.4byte Data_020053fe
.L_ja_02009754:
	.4byte 0x0000011a
.L_ja_02009758:
	.4byte Data_0200540c
.L_ja_0200975c:
	.4byte Data_02005412
.L_ja_02009760:
	.4byte Func_020019a4
.L_ja_02009764:
	.4byte Data_02005418
.L_ja_02009768:
	.4byte 0x0000011b
.L_ja_0200976c:
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02004cd8
.L_ja_02009776:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #73
	bl GameFlag_Test
	cmp r0, #0
	bne .L_ja_02009786
	b .L_ja_020098c6
.L_ja_02009786:
	bl Func_02000c1c
	b .L_ja_020098c6
.L_ja_0200978c:
	ldr r3, .L_ja_020098f0
	cmp r8, r3
	beq .L_ja_02009794
	b .L_ja_020098ba
.L_ja_02009794:
	movs r0, #0
	bl Func_02004d88
	cmp r7, #2
	beq .L_ja_020097aa
	cmp r7, #4
	beq .L_ja_020097aa
	cmp r7, #6
	beq .L_ja_020097aa
	cmp r7, #13
	bne .L_ja_020097be
.L_ja_020097aa:
	ldr r0, .L_ja_020098f4
	ldr r1, .L_ja_020098f8
	ldr r2, .L_ja_020098fc
	movs r3, #88
	bl Func_020022e4
	movs r0, #0
	movs r1, #1
	bl Func_0200248c
.L_ja_020097be:
	subs r3, r7, #7
	cmp r3, #1
	bls .L_ja_020097c8
	cmp r7, #11
	bne .L_ja_0200988c
.L_ja_020097c8:
	ldr r0, .L_ja_02009900
	ldr r1, .L_ja_02009904
	ldr r2, .L_ja_02009908
	movs r3, #88
	bl Func_020022e4
	movs r0, #0
	movs r1, #1
	bl Func_0200248c
	cmp r7, #11
	bne .L_ja_02009802
	ldr r1, .L_ja_0200990c
	movs r0, #11
	bl ObjectMotion_EnableActionAndSetCallback
	movs r1, #3
	movs r0, #0
	bl Func_0200246c
	movs r1, #144
	ldr r0, .L_ja_02009910
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	movs r0, #126
	adds r0, #255
	bl GameFlag_SetBit
.L_ja_02009802:
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_ja_02009914
	bl Scheduler_AddOrUpdateCallback
	movs r0, #11
	bl Object_GetById
	ldr r3, .L_ja_02009918
	str r3, [r0, #108]
	movs r0, #1
	bl Func_020022d8
	ldr r0, .L_ja_0200991c
	bl Func_020002bc
	movs r0, #20
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #13
	bl Object_GetById
	ldr r5, .L_ja_02009920
	str r5, [r0, #108]
	movs r0, #14
	bl Object_GetById
	str r5, [r0, #108]
	movs r0, #15
	bl Object_GetById
	str r5, [r0, #108]
	movs r0, #16
	bl Object_GetById
	str r5, [r0, #108]
	movs r0, #17
	bl Object_GetById
	str r5, [r0, #108]
	movs r0, #18
	bl Object_GetById
	str r5, [r0, #108]
	movs r0, #13
	bl Object_GetById
	str r6, [r0, #104]
	movs r0, #14
	bl Object_GetById
	str r6, [r0, #104]
	movs r0, #15
	bl Object_GetById
	str r6, [r0, #104]
	movs r0, #16
	bl Object_GetById
	str r6, [r0, #104]
	movs r0, #17
	bl Object_GetById
	str r6, [r0, #104]
	movs r0, #18
	bl Object_GetById
	str r6, [r0, #104]
.L_ja_0200988c:
	movs r0, #9
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0, #0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0, #0]
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #78
	bl GameFlag_Test
	cmp r0, #0
	beq .L_ja_020098c6
	movs r1, #176
	movs r2, #174
	movs r0, #8
	lsls r1, r1, #15
	lsls r2, r2, #18
	bl Func_02004cd8
	b .L_ja_020098c6
.L_ja_020098ba:
	ldr r3, .L_ja_02009924
	cmp r8, r3
	bne .L_ja_020098c6
	movs r0, #0
	bl Func_02004d88
.L_ja_020098c6:
	ldr r3, .L_ja_020098f0
	cmp r8, r3
	bne .L_ja_020098d8
	cmp r7, #13
	bne .L_ja_020098d8
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_ClearBit
.L_ja_020098d8:
	ldr r0, .L_ja_02009928
	ldr r1, .L_ja_0200992c
	ldr r2, .L_ja_02009930
	ldr r3, .L_ja_02009934
	bl Func_020045f8
	movs r0, #0
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_ja_020098f0:
	.4byte 0x0000011c
.L_ja_020098f4:
	.4byte Data_02005438
.L_ja_020098f8:
	.4byte Data_0200543e
.L_ja_020098fc:
	.4byte Func_0200196c
.L_ja_02009900:
	.4byte Data_02005448
.L_ja_02009904:
	.4byte Data_0200544e
.L_ja_02009908:
	.4byte Func_02001988
.L_ja_0200990c:
	.4byte Data_02005314
.L_ja_02009910:
	.4byte Func_02001254
.L_ja_02009914:
	.4byte Func_02001378
.L_ja_02009918:
	.4byte Func_020012bc
.L_ja_0200991c:
	.4byte Data_02005454
.L_ja_02009920:
	.4byte Func_02001344
.L_ja_02009924:
	.4byte 0x0000011d
.L_ja_02009928:
	.4byte Data_0200586c
.L_ja_0200992c:
	.4byte Data_0200587c
.L_ja_02009930:
	.4byte Data_020058a8
.L_ja_02009934:
	.4byte Data_020058d4
	.else
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	ldr r2, .L_02009728
	movs r0, #240
	lsls r0, r0, #1
	adds r3, r2, r0
	movs r0, #0
	ldrsh r1, [r3, r0]
	movs r0, #214
	mov r8, r1
	movs r1, #241
	lsls r1, r1, #1
	adds r3, r2, r1
	movs r2, #0
	ldrsh r7, [r3, r2]
	ldr r2, .L_0200972c
	movs r3, #0
	str r3, [r2]
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	lsls r0, r0, #1
	movs r2, #129
	adds r3, r3, r0
	lsls r2, r2, #2
	str r2, [r3]
	subs r0, #154
	sub sp, #8
	bl GameFlag_SetBit
	bl Func_02002270
	ldr r3, .L_02009730
	cmp r8, r3
	bne .L_020093dc
	movs r0, #1
	bl Func_02004dd8
	b .L_020098be
.L_020093dc:
	ldr r3, .L_02009734
	cmp r8, r3
	bne .L_0200944c
	movs r0, #0
	bl Func_02004d88
	movs r0, #10
	bl Func_02001234
	movs r0, #163
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009420
	movs r3, #53
	movs r5, #3
	str r3, [sp, #4]
	movs r0, #35
	movs r1, #41
	movs r2, #24
	movs r3, #10
	str r5, [sp, #0]
	bl Func_02004c30
	movs r3, #117
	str r3, [sp, #4]
	movs r0, #35
	movs r1, #105
	movs r2, #24
	movs r3, #10
	str r5, [sp, #0]
	bl Func_02004c30
.L_02009420:
	ldr r0, .L_02009738
	bl Func_020002bc
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #62
	bl GameFlag_Test
	cmp r0, #0
	bne .L_02009436
	b .L_020098be
.L_02009436:
	movs r3, #39
	movs r2, #85
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #50
	movs r1, #125
	movs r2, #3
	movs r3, #3
	bl Func_02004c30
	b .L_020098be
.L_0200944c:
	ldr r3, .L_0200973c
	cmp r8, r3
	beq .L_02009454
	b .L_02009680
.L_02009454:
	movs r0, #0
	bl Func_02004d88
	ldr r0, .L_02009740
	ldr r1, .L_02009744
	movs r2, #0
	movs r3, #88
	bl Func_020022e4
	ldr r0, .L_02009748
	bl Func_020002bc
	movs r0, #148
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020094ae
	movs r3, #47
	movs r2, #100
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #47
	movs r1, #94
	movs r2, #10
	movs r3, #6
	bl Func_02004c30
	movs r3, #46
	movs r2, #40
	str r3, [sp, #0]
	str r2, [sp, #4]
	movs r0, #110
	movs r1, #40
	movs r2, #12
	movs r3, #4
	bl Func_02004c28
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_02004cd8
	b .L_020094e0
.L_020094ae:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #62
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020094d6
	movs r0, #140
	lsls r0, r0, #2
	bl GameFlag_SetBit
	movs r0, #18
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	b .L_020094e0
.L_020094d6:
	movs r0, #18
	movs r1, #0
	movs r2, #0
	bl Func_02004cd8
.L_020094e0:
	movs r0, #20
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	movs r1, #2
	movs r0, #20
	bl ObjectMotion_SetActionVariant
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #71
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200953c
	movs r3, #100
	str r3, [sp, #4]
	movs r5, #28
	movs r0, #28
	movs r1, #92
	movs r2, #8
	movs r3, #8
	str r5, [sp, #0]
	bl Func_02004c30
	movs r3, #38
	str r3, [sp, #4]
	movs r0, #92
	movs r1, #38
	movs r2, #8
	movs r3, #8
	str r5, [sp, #0]
	bl Func_02004c28
	bl Func_02000798
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl Func_02004cd8
	b .L_02009564
.L_0200953c:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #72
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200955a
	bl Func_02000798
	movs r0, #153
	lsls r0, r0, #1
	adds r0, #255
	bl GameFlag_SetBit
	b .L_02009564
.L_0200955a:
	movs r0, #20
	movs r1, #0
	movs r2, #0
	bl Func_02004cd8
.L_02009564:
	movs r0, #149
	lsls r0, r0, #4
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200958c
	movs r1, #164
	movs r2, #240
	movs r0, #8
	lsls r1, r1, #17
	lsls r2, r2, #15
	bl Func_02004cd8
	movs r0, #8
	bl Object_GetById
	movs r3, #1
	adds r0, #98
	strb r3, [r0]
.L_0200958c:
	movs r0, #163
	lsls r0, r0, #4
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020095a4
	movs r0, #19
	movs r1, #0
	movs r2, #0
	bl Func_02004cd8
	b .L_020095ae
.L_020095a4:
	movs r0, #15
	movs r1, #0
	movs r2, #0
	bl Func_02004cd8
.L_020095ae:
	cmp r7, #20
	beq .L_020095b4
	b .L_020098be
.L_020095b4:
	bl Func_02004ca8
	movs r0, #0
	bl Func_02004da0
	movs r0, #1
	movs r1, #1
	movs r2, #1
	movs r3, #0
	negs r0, r0
	negs r1, r1
	negs r2, r2
	bl Motion_CamBounds
	movs r1, #128
	movs r2, #128
	movs r0, #0
	lsls r1, r1, #10
	lsls r2, r2, #9
	bl Func_02004c50
	ldr r3, .L_02009728
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	movs r1, #0
	movs r2, #0
	bl Func_02004cd8
	movs r1, #0
	movs r2, #0
	movs r0, #18
	bl Func_02004cd8
	movs r0, #18
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	bl Event_StartScreenTransition
	bl Event_WaitTransitionFrames
	movs r0, #18
	bl Object_GetById
	adds r3, r0, #0
	adds r3, #100
	adds r0, #102
	movs r2, #0
	ldrsh r1, [r3, r2]
	movs r3, #0
	ldrsh r2, [r0, r3]
	lsls r1, r1, #16
	lsls r2, r2, #16
	movs r0, #18
	bl Func_02004cd8
	movs r0, #159
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02004de0
	movs r5, #2
.L_0200963c:
	movs r0, #18
	bl Func_02000414
	subs r5, #1
	movs r0, #1
	bl WaitFrames
	cmp r5, #0
	bge .L_0200963c
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #62
	bl GameFlag_SetBit
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r1, r1
	adds r2, #102
	negs r0, r0
	bl Func_02004c50
	movs r0, #120
	bl Battle_WaitMode0
	movs r0, #20
	bl Func_02004d30
	movs r0, #10
	adds r0, #255
	bl GameFlag_SetBit
	b .L_020098be
.L_02009680:
	ldr r3, .L_0200974c
	cmp r8, r3
	bne .L_020096b0
	movs r0, #0
	bl Func_02004d88
	cmp r7, #10
	beq .L_02009694
	cmp r7, #1
	bne .L_020096a8
.L_02009694:
	ldr r0, .L_02009750
	ldr r1, .L_02009754
	ldr r2, .L_02009758
	movs r3, #88
	bl Func_020022e4
	movs r0, #0
	movs r1, #1
	bl Func_0200248c
.L_020096a8:
	ldr r0, .L_0200975c
	bl Func_020002bc
	b .L_020098be
.L_020096b0:
	ldr r3, .L_02009760
	cmp r8, r3
	bne .L_02009784
	movs r0, #0
	bl Func_02004d88
	movs r0, #12
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #77
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200970c
	movs r3, #101
	str r3, [sp, #4]
	movs r5, #12
	movs r0, #12
	movs r1, #95
	movs r2, #8
	movs r3, #6
	str r5, [sp, #0]
	bl Func_02004c30
	movs r3, #40
	str r3, [sp, #4]
	movs r0, #76
	movs r1, #40
	movs r2, #8
	movs r3, #5
	str r5, [sp, #0]
	bl Func_02004c28
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02004cd8
	b .L_0200976e
.L_0200970c:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #76
	bl GameFlag_Test
	cmp r0, #0
	beq .L_02009764
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #50
	bl GameFlag_SetBit
	b .L_0200976e
	.2byte 0x0000
.L_02009728:
	.4byte gPartyState
.L_0200972c:
	.4byte Data_02006878
.L_02009730:
	.4byte 0x00000117
.L_02009734:
	.4byte 0x00000118
.L_02009738:
	.4byte Data_020053d8
.L_0200973c:
	.4byte 0x00000119
.L_02009740:
	.4byte Data_020053e6
.L_02009744:
	.4byte Data_020053f8
.L_02009748:
	.4byte Data_020053fe
.L_0200974c:
	.4byte 0x0000011a
.L_02009750:
	.4byte Data_0200540c
.L_02009754:
	.4byte Data_02005412
.L_02009758:
	.4byte Func_020019a4
.L_0200975c:
	.4byte Data_02005418
.L_02009760:
	.4byte 0x0000011b
.L_02009764:
	movs r0, #12
	movs r1, #0
	movs r2, #0
	bl Func_02004cd8
.L_0200976e:
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #73
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200977e
	b .L_020098be
.L_0200977e:
	bl Func_02000c1c
	b .L_020098be
.L_02009784:
	ldr r3, .L_020098e8
	cmp r8, r3
	beq .L_0200978c
	b .L_020098b2
.L_0200978c:
	movs r0, #0
	bl Func_02004d88
	cmp r7, #2
	beq .L_020097a2
	cmp r7, #4
	beq .L_020097a2
	cmp r7, #6
	beq .L_020097a2
	cmp r7, #13
	bne .L_020097b6
.L_020097a2:
	ldr r0, .L_020098ec
	ldr r1, .L_020098f0
	ldr r2, .L_020098f4
	movs r3, #88
	bl Func_020022e4
	movs r0, #0
	movs r1, #1
	bl Func_0200248c
.L_020097b6:
	subs r3, r7, #7
	cmp r3, #1
	bls .L_020097c0
	cmp r7, #11
	bne .L_02009884
.L_020097c0:
	ldr r0, .L_020098f8
	ldr r1, .L_020098fc
	ldr r2, .L_02009900
	movs r3, #88
	bl Func_020022e4
	movs r0, #0
	movs r1, #1
	bl Func_0200248c
	cmp r7, #11
	bne .L_020097fa
	ldr r1, .L_02009904
	movs r0, #11
	bl ObjectMotion_EnableActionAndSetCallback
	movs r1, #3
	movs r0, #0
	bl Func_0200246c
	movs r1, #144
	ldr r0, .L_02009908
	lsls r1, r1, #3
	bl Scheduler_AddOrUpdateCallback
	movs r0, #126
	adds r0, #255
	bl GameFlag_SetBit
.L_020097fa:
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200990c
	bl Scheduler_AddOrUpdateCallback
	movs r0, #11
	bl Object_GetById
	ldr r3, .L_02009910
	str r3, [r0, #108]
	movs r0, #1
	bl Func_020022d8
	ldr r0, .L_02009914
	bl Func_020002bc
	movs r0, #20
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #13
	bl Object_GetById
	ldr r5, .L_02009918
	str r5, [r0, #108]
	movs r0, #14
	bl Object_GetById
	str r5, [r0, #108]
	movs r0, #15
	bl Object_GetById
	str r5, [r0, #108]
	movs r0, #16
	bl Object_GetById
	str r5, [r0, #108]
	movs r0, #17
	bl Object_GetById
	str r5, [r0, #108]
	movs r0, #18
	bl Object_GetById
	str r5, [r0, #108]
	movs r0, #13
	bl Object_GetById
	str r6, [r0, #104]
	movs r0, #14
	bl Object_GetById
	str r6, [r0, #104]
	movs r0, #15
	bl Object_GetById
	str r6, [r0, #104]
	movs r0, #16
	bl Object_GetById
	str r6, [r0, #104]
	movs r0, #17
	bl Object_GetById
	str r6, [r0, #104]
	movs r0, #18
	bl Object_GetById
	str r6, [r0, #104]
.L_02009884:
	movs r0, #9
	bl Object_GetById
	adds r0, #35
	ldrb r2, [r0]
	movs r3, #2
	orrs r3, r2
	strb r3, [r0]
	movs r0, #160
	lsls r0, r0, #4
	adds r0, #78
	bl GameFlag_Test
	cmp r0, #0
	beq .L_020098be
	movs r1, #176
	movs r2, #174
	movs r0, #8
	lsls r1, r1, #15
	lsls r2, r2, #18
	bl Func_02004cd8
	b .L_020098be
.L_020098b2:
	ldr r3, .L_0200991c
	cmp r8, r3
	bne .L_020098be
	movs r0, #0
	bl Func_02004d88
.L_020098be:
	ldr r3, .L_020098e8
	cmp r8, r3
	bne .L_020098d0
	cmp r7, #13
	bne .L_020098d0
	movs r0, #130
	lsls r0, r0, #1
	bl GameFlag_ClearBit
.L_020098d0:
	ldr r0, .L_02009920
	ldr r1, .L_02009924
	ldr r2, .L_02009928
	ldr r3, .L_0200992c
	bl Func_020045f8
	movs r0, #0
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_020098e8:
	.4byte 0x0000011c
.L_020098ec:
	.4byte Data_02005438
.L_020098f0:
	.4byte Data_0200543e
.L_020098f4:
	.4byte Func_0200196c
.L_020098f8:
	.4byte Data_02005448
.L_020098fc:
	.4byte Data_0200544e
.L_02009900:
	.4byte Func_02001988
.L_02009904:
	.4byte Data_02005314
.L_02009908:
	.4byte Func_02001254
.L_0200990c:
	.4byte Func_02001378
.L_02009910:
	.4byte Func_020012bc
.L_02009914:
	.4byte Data_02005454
.L_02009918:
	.4byte Func_02001344
.L_0200991c:
	.4byte 0x0000011d
.L_02009920:
	.4byte Data_0200586c
.L_02009924:
	.4byte Data_0200587c
.L_02009928:
	.4byte Data_020058a8
.L_0200992c:
	.4byte Data_020058d4
	.endif
	.section .text.x02009ae0,"ax",%progbits
	.balign 4
	.global Func_02001ae0
	.thumb_func
Func_02001ae0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_02009c3c
	ldr r2, .L_02009c40
	mov r10, r3
	movs r3, #133
	lsls r3, r3, #2
	add r3, r10
	adds r6, r0, #0
	ldr r0, [r3]
	sub sp, #4
	mov r8, r2
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	ldr r5, [r6, #68]
	mov r9, r3
	ldr r3, [r6, #8]
	ldr r2, [r6, #72]
	adds r3, r3, r5
	str r3, [r6, #8]
	ldr r3, [r6, #12]
	ldr r7, [r6, #76]
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r6, #16]
	adds r4, r0, #0
	adds r3, r3, r7
	adds r0, r5, #0
	movs r1, #18
	str r3, [r6, #16]
	str r4, [sp, #0]
	bl __divsi3
	subs r5, r5, r0
	str r5, [r6, #68]
	adds r3, r7, #0
	ldr r4, [sp, #0]
	cmp r7, #0
	bge .L_02009b3c
	adds r3, #15
.L_02009b3c:
	asrs r3, r3, #4
	subs r3, r7, r3
	str r3, [r6, #76]
	ldr r2, [r6, #48]
	ldr r3, [r6, #24]
	ldr r1, [r6, #80]
	adds r3, r3, r2
	str r3, [r6, #24]
	ldr r2, [r6, #52]
	ldr r3, [r6, #28]
	adds r3, r3, r2
	str r3, [r6, #28]
	adds r2, r6, #0
	adds r2, #100
	ldrh r3, [r1, #18]
	ldrh r2, [r2]
	adds r3, r3, r2
	strh r3, [r1, #18]
	ldr r2, [r4, #12]
	ldr r3, [r4, #20]
	cmp r2, r3
	bne .L_02009bce
	adds r0, r6, #0
	adds r1, r4, #0
	bl Func_02001a1c
	cmp r0, #0
	beq .L_02009bce
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #18
	add r3, r10
	ldrb r2, [r3]
	cmp r2, #0
	bne .L_02009bce
	ldr r1, [r6, #76]
	cmp r1, #0
	beq .L_02009ba2
	mov r3, r8
	adds r3, #104
	strh r2, [r3]
	mov r2, r8
	adds r2, #106
	cmp r1, #0
	ble .L_02009b9a
	movs r3, #1
	b .L_02009ba0
.L_02009b9a:
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
.L_02009ba0:
	strh r3, [r2]
.L_02009ba2:
	adds r2, r6, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	movs r1, #0
	ldr r2, [r6, #76]
	ldr r3, [r6, #16]
	str r1, [r6, #76]
	subs r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #72]
	asrs r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #72]
	mov r2, r8
	movs r3, #1
	strh r3, [r2, #4]
	ldrh r2, [r2, #10]
	movs r3, #170
	lsls r3, r3, #1
	add r3, r9
	strh r2, [r3]
.L_02009bce:
	movs r3, #84
	mov r2, r8
	ldrh r0, [r2, r3]
	movs r7, #0
	cmp r0, #0
	beq .L_02009c2e
	mov r5, r8
	adds r5, #84
.L_02009bde:
	bl Object_GetById
	adds r4, r0, #0
	ldr r2, [r4, #12]
	ldr r3, [r4, #20]
	cmp r2, r3
	bne .L_02009c20
	adds r0, r6, #0
	adds r1, r4, #0
	bl Func_02001a80
	cmp r0, #0
	beq .L_02009c20
	ldrh r1, [r5, #2]
	cmp r1, #0
	bne .L_02009c1a
	adds r2, r6, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	ldr r2, [r6, #76]
	ldr r3, [r6, #16]
	str r1, [r6, #76]
	subs r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #72]
	asrs r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #72]
	b .L_02009c20
.L_02009c1a:
	movs r3, #1
	mov r2, r8
	strh r3, [r2, #6]
.L_02009c20:
	adds r7, #1
	adds r5, #4
	cmp r7, #3
	bgt .L_02009c2e
	ldrh r0, [r5]
	cmp r0, #0
	bne .L_02009bde
.L_02009c2e:
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02009c3c:
	.4byte gPartyState
.L_02009c40:
	.4byte Data_020023c4 + 0x188
	.section .text.x02009cbc,"ax",%progbits
	.balign 4
	.global Func_02001cbc
	.thumb_func
Func_02001cbc:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	ldr r3, .L_02009e14
	ldr r2, .L_02009e18
	mov r10, r3
	movs r3, #133
	lsls r3, r3, #2
	add r3, r10
	adds r6, r0, #0
	ldr r0, [r3]
	sub sp, #4
	mov r8, r2
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	ldr r5, [r6, #68]
	mov r9, r3
	ldr r3, [r6, #16]
	ldr r2, [r6, #72]
	adds r3, r3, r5
	str r3, [r6, #16]
	ldr r3, [r6, #12]
	ldr r7, [r6, #76]
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r6, #8]
	adds r4, r0, #0
	adds r3, r3, r7
	adds r0, r5, #0
	movs r1, #18
	str r3, [r6, #8]
	str r4, [sp, #0]
	bl __divsi3
	subs r5, r5, r0
	str r5, [r6, #68]
	adds r3, r7, #0
	ldr r4, [sp, #0]
	cmp r7, #0
	bge .L_02009d18
	adds r3, #15
.L_02009d18:
	asrs r3, r3, #4
	subs r3, r7, r3
	str r3, [r6, #76]
	ldr r2, [r6, #48]
	ldr r3, [r6, #24]
	ldr r1, [r6, #80]
	adds r3, r3, r2
	str r3, [r6, #24]
	ldr r2, [r6, #52]
	ldr r3, [r6, #28]
	adds r3, r3, r2
	str r3, [r6, #28]
	adds r2, r6, #0
	adds r2, #100
	ldrh r3, [r1, #18]
	ldrh r2, [r2]
	adds r3, r3, r2
	strh r3, [r1, #18]
	ldr r2, [r4, #12]
	ldr r3, [r4, #20]
	cmp r2, r3
	bne .L_02009da8
	adds r0, r6, #0
	adds r1, r4, #0
	bl Func_02001a1c
	cmp r0, #0
	beq .L_02009da8
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #18
	add r3, r10
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_02009da8
	ldr r2, [r6, #76]
	cmp r2, #0
	beq .L_02009d7c
	mov r1, r8
	adds r1, #106
	strh r3, [r1]
	subs r1, #2
	cmp r2, #0
	ble .L_02009d74
	movs r3, #1
	b .L_02009d7a
.L_02009d74:
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
.L_02009d7a:
	strh r3, [r1]
.L_02009d7c:
	adds r2, r6, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	movs r1, #0
	ldr r3, [r6, #16]
	ldr r2, [r6, #68]
	str r1, [r6, #76]
	subs r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #72]
	asrs r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #72]
	mov r2, r8
	movs r3, #1
	strh r3, [r2, #4]
	ldrh r2, [r2, #10]
	movs r3, #170
	lsls r3, r3, #1
	add r3, r9
	strh r2, [r3]
.L_02009da8:
	movs r3, #84
	mov r2, r8
	ldrh r0, [r2, r3]
	movs r7, #0
	cmp r0, #0
	beq .L_02009e08
	mov r5, r8
	adds r5, #84
.L_02009db8:
	bl Object_GetById
	adds r4, r0, #0
	ldr r2, [r4, #12]
	ldr r3, [r4, #20]
	cmp r2, r3
	bne .L_02009dfa
	adds r0, r6, #0
	adds r1, r4, #0
	bl Func_02001a80
	cmp r0, #0
	beq .L_02009dfa
	ldrh r1, [r5, #2]
	cmp r1, #0
	bne .L_02009df4
	adds r2, r6, #0
	adds r2, #35
	movs r3, #2
	strb r3, [r2]
	ldr r2, [r6, #76]
	ldr r3, [r6, #16]
	str r1, [r6, #76]
	subs r3, r3, r2
	str r3, [r6, #16]
	ldr r3, [r6, #72]
	asrs r2, r2, #2
	adds r3, r3, r2
	str r3, [r6, #72]
	b .L_02009dfa
.L_02009df4:
	movs r3, #1
	mov r2, r8
	strh r3, [r2, #6]
.L_02009dfa:
	adds r7, #1
	adds r5, #4
	cmp r7, #3
	bgt .L_02009e08
	ldrh r0, [r5]
	cmp r0, #0
	bne .L_02009db8
.L_02009e08:
	add sp, #4
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_02009e14:
	.4byte gPartyState
.L_02009e18:
	.4byte Data_020023c4 + 0x188
	.section .text.x02009e1c,"ax",%progbits
	.global Func_02001e1c
	.thumb_func
Func_02001e1c:
	push {r5, r6, lr}
	adds r5, r0, #0
	adds r3, r5, #0
	movs r6, #0
	adds r3, #85
	strb r6, [r3]
	adds r3, #4
	strb r6, [r3]
	movs r3, #3
	ldr r0, [r5, #80]
	ands r1, r3
	ldrb r2, [r0, #9]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	lsls r1, r1, #2
	orrs r3, r1
	strb r3, [r0, #9]
	movs r1, #0
	adds r0, r5, #0
	bl ObjectDispatch_SetSingleChildFlags
	ldr r1, .L_02009e5c
	adds r0, r5, #0
	bl Func_02004bd0
	adds r3, r5, #0
	adds r3, #100
	strh r6, [r3]
	str r6, [r5, #48]
	str r6, [r5, #52]
	pop {r5, r6, pc}
.L_02009e5c:
	.4byte Data_020056a0
	.section .text.x0200a0ec,"ax",%progbits
	.balign 4
	.global Func_020020ec
	.thumb_func
Func_020020ec:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r0, .L_0200a268
	sub sp, #8
	mov r8, r0
	movs r0, #192
	lsls r0, r0, #18
	ldr r1, [r0, #32]
	mov r7, r8
	adds r2, r1, #0
	adds r2, #228
	ldr r3, [r2]
	ldr r2, [r2, #4]
	mov r9, r3
	ldr r3, .L_0200a26c
	mov r4, r9
	ands r4, r3
	ands r2, r3
	ldr r3, [r1]
	mov r9, r4
	ldr r3, [r3, #4]
	adds r7, #20
	str r3, [sp, #4]
	mov r10, r2
	ldr r0, [r0, #108]
	str r0, [sp, #0]
	mov r0, r8
	movs r4, #6
	ldrsh r3, [r0, r4]
	cmp r3, #0
	beq .L_0200a146
	movs r1, #8
	ldrsh r3, [r0, r1]
	cmp r3, #0
	bne .L_0200a146
	ldr r3, [r0, #16]
	cmp r3, #0
	beq .L_0200a146
	mov lr, r3
	.2byte 0xf800
.L_0200a146:
	mov r2, r8
	ldrh r3, [r2, #6]
	mov r4, r8
	movs r2, #0
	mov r0, r8
	strh r3, [r4, #8]
	strh r2, [r0, #6]
	movs r1, #3
	mov r11, r1
.L_0200a158:
	movs r2, #0
	ldrsh r3, [r7, r2]
	cmp r3, #0
	beq .L_0200a248
	ldr r5, [r7, #8]
	cmp r5, #0
	beq .L_0200a248
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_Test
	ldr r4, [sp, #0]
	movs r1, #179
	lsls r1, r1, #1
	adds r3, r4, r1
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_0200a182
	movs r3, #1
	orrs r0, r3
.L_0200a182:
	adds r6, r5, #0
	adds r6, #91
	strb r0, [r6]
	mov r0, r8
	movs r4, #14
	ldrsh r3, [r0, r4]
	cmp r3, #0
	beq .L_0200a19c
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_Test
	strb r0, [r6]
.L_0200a19c:
	ldr r2, [r5, #8]
	mov r1, r9
	ldr r3, [r5, #16]
	subs r2, r2, r1
	ldr r1, [r5, #12]
	mov r4, r10
	subs r3, r3, r4
	subs r1, r3, r1
	movs r0, #6
	ldrsh r4, [r7, r0]
	asrs r3, r1, #16
	adds r1, r3, #0
	asrs r2, r2, #16
	subs r1, #8
	cmp r4, #0
	bne .L_0200a1d2
	adds r3, r2, #7
	movs r2, #167
	lsls r2, r2, #1
	cmp r3, r2
	bhi .L_0200a248
	movs r3, #48
	negs r3, r3
	cmp r1, r3
	ble .L_0200a248
	cmp r1, #239
	bgt .L_0200a248
.L_0200a1d2:
	movs r0, #2
	ldrsh r3, [r7, r0]
	ldrh r1, [r7, #2]
	cmp r3, #0
	bgt .L_0200a244
	ldrh r3, [r7, #4]
	movs r1, #240
	ands r1, r3
	cmp r1, #32
	beq .L_0200a218
	cmp r1, #32
	bgt .L_0200a1f4
	cmp r1, #0
	beq .L_0200a234
	cmp r1, #16
	beq .L_0200a226
	b .L_0200a240
.L_0200a1f4:
	cmp r1, #48
	beq .L_0200a20a
	cmp r1, #128
	bne .L_0200a240
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	adds r0, #8
	adds r2, r4, #0
	bl Func_0200206c
	b .L_0200a240
.L_0200a20a:
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	adds r0, #8
	adds r2, r4, #0
	bl Func_02001e60
	b .L_0200a240
.L_0200a218:
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	adds r0, #8
	adds r2, r4, #0
	bl Func_02001ef0
	b .L_0200a240
.L_0200a226:
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	adds r0, #8
	adds r2, r4, #0
	bl Func_02001ea8
	b .L_0200a240
.L_0200a234:
	ldr r0, [r7, #8]
	ldr r1, [r7, #12]
	adds r0, #8
	adds r2, r4, #0
	bl Func_02001c74
.L_0200a240:
	movs r3, #8
	b .L_0200a246
.L_0200a244:
	subs r3, r1, #1
.L_0200a246:
	strh r3, [r7, #2]
.L_0200a248:
	movs r1, #1
	negs r1, r1
	add r11, r1
	mov r2, r11
	adds r7, #16
	cmp r2, #0
	blt .L_0200a258
	b .L_0200a158
.L_0200a258:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a268:
	.4byte Data_020023c4 + 0x188
.L_0200a26c:
	.4byte 0xffff0000
	.section .text.x0200a2e4,"ax",%progbits
	.balign 4
	.global Func_020022e4
	.thumb_func
Func_020022e4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r8, r1
	ldr r1, .L_0200a410
	sub sp, #16
	adds r6, r0, #0
	movs r0, #10
	str r1, [sp, #4]
	adds r0, #255
	adds r1, #20
	str r2, [sp, #12]
	str r3, [sp, #8]
	mov r9, r1
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200a3c8
	ldrh r3, [r6]
	movs r2, #0
	mov r11, r2
	mov r10, r3
	adds r6, #2
	cmp r3, #0
	ble .L_0200a386
.L_0200a31e:
	ldrh r7, [r6]
	movs r1, #15
	ands r1, r7
	movs r3, #240
	mov r0, r10
	str r1, [sp, #0]
	ands r7, r3
	bl Object_GetById
	adds r5, r0, #0
	adds r6, #2
	cmp r5, #0
	beq .L_0200a372
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	adds r3, r5, #0
	movs r2, #128
	adds r3, #98
	movs r1, #1
	ands r2, r7
	strb r1, [r3]
	cmp r2, #0
	bne .L_0200a352
	subs r3, #9
	strb r2, [r3]
.L_0200a352:
	mov r2, r9
	mov r3, r9
	strh r1, [r2]
	mov r0, r10
	strh r7, [r3, #4]
	bl Object_GetById
	mov r1, r9
	str r0, [r1, #8]
	ldr r2, [sp, #0]
	lsls r3, r2, #16
	str r3, [r1, #12]
	mov r3, r11
	strh r3, [r1, #2]
	movs r2, #16
	add r9, r2
.L_0200a372:
	movs r3, #1
	add r11, r3
	mov r1, r11
	cmp r1, #3
	bgt .L_0200a386
	ldrh r2, [r6]
	adds r6, #2
	mov r10, r2
	cmp r2, #0
	bgt .L_0200a31e
.L_0200a386:
	mov r3, r8
	cmp r3, #0
	beq .L_0200a3c8
	movs r1, #0
	ldrh r2, [r3]
	mov r11, r1
	ldr r1, [sp, #4]
	movs r3, #2
	add r8, r3
	movs r3, #84
	strh r2, [r1, r3]
	cmp r2, #0
	ble .L_0200a3c8
	adds r2, r1, #0
	adds r2, #84
.L_0200a3a4:
	mov r1, r8
	ldrh r3, [r1]
	movs r1, #1
	strh r3, [r2, #2]
	add r11, r1
	movs r3, #2
	add r8, r3
	mov r3, r11
	adds r2, #4
	cmp r3, #3
	bgt .L_0200a3c8
	mov r1, r8
	ldrh r3, [r1]
	movs r1, #2
	add r8, r1
	strh r3, [r2]
	cmp r3, #0
	bgt .L_0200a3a4
.L_0200a3c8:
	ldr r2, [sp, #12]
	ldr r3, [sp, #4]
	add r1, sp, #8
	str r2, [r3, #16]
	ldrh r1, [r1]
	ldr r2, [sp, #4]
	strh r1, [r2, #10]
	movs r1, #128
	lsls r1, r1, #19
	adds r1, #80
	ldrh r3, [r1]
	cmp r3, #0
	bne .L_0200a3f8
	movs r3, #192
	movs r2, #128
	lsls r3, r3, #4
	lsls r2, r2, #19
	adds r3, #8
	adds r2, #82
	strh r3, [r2]
	movs r3, #252
	lsls r3, r3, #6
	adds r3, #16
	strh r3, [r1]
.L_0200a3f8:
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_0200a414
	bl Scheduler_AddOrUpdateCallback
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200a410:
	.4byte Data_020023c4 + 0x188
.L_0200a414:
	.4byte Func_020020ec
	.section .text.x0200a4c0,"ax",%progbits
	.balign 4
	.global Func_020024c0
	.thumb_func
Func_020024c0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	sub sp, #16
	ldr r6, [r3, #108]
	bl Func_02004dc0
	mov r8, r0
	bl Object_GetById
	bl Party_CountActiveOwners
	movs r5, #0
	adds r7, r0, #0
	cmp r5, r7
	bge .L_0200a502
.L_0200a4e6:
	ldr r2, .L_0200a5a8
	movs r1, #134
	lsls r1, r1, #2
	adds r3, r5, r1
	ldrb r0, [r2, r3]
	bl UnitGet
	ldrh r3, [r0, #56]
	lsls r2, r5, #1
	mov r1, sp
	adds r5, #1
	strh r3, [r1, r2]
	cmp r5, r7
	blt .L_0200a4e6
.L_0200a502:
	movs r0, #10
	negs r0, r0
	movs r1, #0
	bl Func_02004da8
	movs r2, #182
	lsls r2, r2, #1
	movs r4, #183
	adds r3, r6, r2
	lsls r4, r4, #1
	movs r2, #0
	strh r2, [r3]
	movs r1, #129
	adds r3, r6, r4
	strh r2, [r3]
	mov r0, r8
	lsls r1, r1, #1
	movs r5, #0
	bl Func_02004d08
	cmp r5, r7
	bge .L_0200a59c
.L_0200a52e:
	ldr r1, .L_0200a5a8
	movs r2, #134
	lsls r2, r2, #2
	adds r2, r2, r5
	ldrb r0, [r1, r2]
	mov r10, r1
	mov r8, r2
	bl UnitGet
	movs r4, #56
	ldrsh r3, [r0, r4]
	cmp r3, #0
	ble .L_0200a556
	movs r1, #183
	lsls r1, r1, #1
	adds r2, r6, r1
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	b .L_0200a596
.L_0200a556:
	mov r3, sp
	lsls r2, r5, #1
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_0200a596
	movs r2, #182
	lsls r2, r2, #1
	adds r1, r6, r2
	ldrh r3, [r1]
	movs r4, #184
	adds r2, r3, #1
	lsls r3, r3, #16
	lsls r4, r4, #1
	asrs r3, r3, #15
	strh r2, [r1]
	adds r3, r3, r4
	mov r1, r10
	mov r4, r8
	ldrb r2, [r1, r4]
	movs r1, #181
	strh r2, [r6, r3]
	movs r3, #255
	lsls r1, r1, #1
	lsls r3, r3, #8
	adds r2, r6, r1
	adds r3, #255
	strh r3, [r2]
	movs r3, #50
	adds r3, #255
	adds r2, r0, r3
	movs r3, #0
	strb r3, [r2]
.L_0200a596:
	adds r5, #1
	cmp r5, r7
	blt .L_0200a52e
.L_0200a59c:
	add sp, #16
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a5a8:
	.4byte gPartyState
	.section .text.x0200a5ac,"ax",%progbits
	.global Func_020025ac
	.thumb_func
Func_020025ac:
	push {r5, r6, r7, lr}
	ldr r3, .L_0200a610
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	adds r6, r0, #0
	ldr r0, [r3]
	bl Object_GetById
	ldr r5, [r6, #68]
	ldr r3, [r6, #8]
	ldr r2, [r6, #72]
	adds r3, r3, r5
	str r3, [r6, #8]
	ldr r3, [r6, #12]
	ldr r7, [r6, #76]
	adds r3, r3, r2
	str r3, [r6, #12]
	ldr r3, [r6, #16]
	adds r0, r5, #0
	adds r3, r3, r7
	movs r1, #18
	str r3, [r6, #16]
	bl __divsi3
	subs r5, r5, r0
	str r5, [r6, #68]
	adds r3, r7, #0
	cmp r7, #0
	bge .L_0200a5ea
	adds r3, #15
.L_0200a5ea:
	asrs r3, r3, #4
	subs r3, r7, r3
	str r3, [r6, #76]
	ldr r2, [r6, #48]
	ldr r3, [r6, #24]
	movs r1, #128
	adds r3, r3, r2
	str r3, [r6, #24]
	ldr r2, [r6, #52]
	ldr r3, [r6, #28]
	lsls r1, r1, #5
	adds r3, r3, r2
	str r3, [r6, #28]
	ldr r2, [r6, #80]
	ldrh r3, [r2, #18]
	adds r3, r3, r1
	strh r3, [r2, #18]
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200a610:
	.4byte gPartyState
	.section .text.x0200a63c,"ax",%progbits
	.balign 4
	.global Func_0200263c
	.thumb_func
Func_0200263c:
	.ifdef TLA_EDITION_JA
	push {r5, r6, r7, lr}
	mov r7, sl
	mov r6, r8
	push {r6, r7}
	ldr r5, .L_ja_0200a7d0
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	adds r3, r5, r0
	ldrb r3, [r3, #0]
	sub sp, #12
	cmp r3, #0
	beq .L_ja_0200a660
	b .L_ja_0200a7c0
.L_ja_0200a660:
	movs r0, #10
	movs r1, #0
	negs r0, r0
	bl Func_020024c0
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r5, r1
	ldr r0, [r3, #0]
	bl Object_GetById
	ldr r2, .L_ja_0200a7d4
	adds r5, r0, #0
	mov sl, r2
	bl Func_02004ca8
	movs r0, #0
	bl Func_02004da0
	ldr r3, .L_ja_0200a7d8
	adds r0, r5, #0
	str r3, [r5, #108]
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r5, #6]
	movs r1, #49
	bl Func_02004bc0
.L_ja_0200a698:
	adds r3, r5, #0
	adds r3, #34
	ldr r1, [r5, #8]
	ldrb r0, [r3, #0]
	ldr r2, [r5, #16]
	bl Func_02004c18
	mov r6, sl
	mov r7, sl
	adds r6, #104
	adds r7, #106
	mov r1, sp
	cmp r0, #7
	bne .L_ja_0200a710
	movs r0, #0
	ldrsh r3, [r6, r0]
	movs r2, #0
	lsls r3, r3, #17
	str r3, [r5, #36]
	mov r8, r2
	movs r1, #0
	ldrsh r3, [r7, r1]
	lsls r3, r3, #17
	str r3, [r5, #44]
	movs r3, #128
	lsls r3, r3, #5
	str r3, [r5, #52]
.L_ja_0200a6ce:
	movs r3, #0
	ldrsh r2, [r6, r3]
	ldr r3, [r5, #8]
	lsls r2, r2, #19
	mov r1, sp
	adds r3, r3, r2
	str r3, [r1, #0]
	ldr r3, [r5, #12]
	str r3, [r1, #4]
	movs r0, #0
	ldrsh r2, [r7, r0]
	ldr r3, [r5, #16]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1, #8]
	adds r0, r5, #0
	bl Func_02004c38
	cmp r0, #0
	beq .L_ja_0200a6fe
	movs r3, #0
	str r3, [r5, #36]
	str r3, [r5, #44]
	b .L_ja_0200a7b4
.L_ja_0200a6fe:
	movs r0, #1
	bl WaitFrames
	movs r1, #1
	add r8, r1
	mov r2, r8
	cmp r2, #9
	ble .L_ja_0200a6ce
	b .L_ja_0200a7b4
.L_ja_0200a710:
	movs r3, #0
	ldrsh r2, [r6, r3]
	ldr r3, [r5, #8]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1, #0]
	ldr r3, [r5, #12]
	str r3, [r1, #4]
	movs r0, #0
	ldrsh r2, [r7, r0]
	ldr r3, [r5, #16]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1, #8]
	adds r0, r5, #0
	bl Func_02004c38
	cmp r0, #0
	bgt .L_ja_0200a7b4
	cmp r0, #0
	bge .L_ja_0200a756
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r5, #6]
	ldr r3, .L_ja_0200a7d0
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	movs r2, #1
	ldr r0, [r3, #0]
	movs r1, #6
	negs r2, r2
	bl Func_02004ce8
	b .L_ja_0200a7b4
.L_ja_0200a756:
	movs r2, #0
	ldrsh r3, [r6, r2]
	ldr r2, [r5, #8]
	lsls r3, r3, #17
	adds r1, r2, r3
	str r1, [r5, #8]
	ldr r2, [r5, #16]
	movs r0, #0
	ldrsh r3, [r7, r0]
	ldr r7, .L_ja_0200a7dc
	lsls r3, r3, #17
	ldr r0, .L_ja_0200a7e0
	adds r6, r2, r3
	adds r3, r1, #0
	ands r3, r7
	movs r4, #128
	adds r2, r3, r0
	lsls r4, r4, #9
	str r6, [r5, #16]
	cmp r2, r4
	ble .L_ja_0200a782
	adds r2, r4, #0
.L_ja_0200a782:
	ldr r0, .L_ja_0200a7e4
	cmp r2, r0
	bge .L_ja_0200a78a
	adds r2, r0, #0
.L_ja_0200a78a:
	subs r3, r1, r2
	ldr r1, .L_ja_0200a7e0
	str r3, [r5, #8]
	adds r3, r6, #0
	ands r3, r7
	adds r2, r3, r1
	cmp r2, r4
	ble .L_ja_0200a79c
	adds r2, r4, #0
.L_ja_0200a79c:
	cmp r2, r0
	bge .L_ja_0200a7a2
	adds r2, r0, #0
.L_ja_0200a7a2:
	subs r3, r6, r2
	str r3, [r5, #16]
	mov r2, sl
	movs r3, #0
	strh r3, [r2, #4]
	movs r0, #1
	bl WaitFrames
	b .L_ja_0200a698
.L_ja_0200a7b4:
	movs r3, #0
	str r3, [r5, #108]
	adds r0, r5, #0
	movs r1, #0
	bl Animation_ApplyChildValues
.L_ja_0200a7c0:
	mov r0, sl
	movs r3, #0
	strh r3, [r0, #4]
	add sp, #12
	pop {r3, r5}
	mov r8, r3
	mov sl, r5
	pop {r5, r6, r7, pc}
.L_ja_0200a7d0:
	.4byte gPartyState
.L_ja_0200a7d4:
	.4byte 0x0200254c
.L_ja_0200a7d8:
	.4byte Func_02002614
.L_ja_0200a7dc:
	.4byte 0x000fffff
.L_ja_0200a7e0:
	.4byte 0xfff80000
.L_ja_0200a7e4:
	.4byte 0xffff0000
	.else
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r5, .L_0200a830
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #18
	adds r3, r5, r0
	ldrb r3, [r3]
	sub sp, #16
	cmp r3, #0
	beq .L_0200a65e
	b .L_0200a81c
.L_0200a65e:
	movs r0, #10
	movs r1, #0
	negs r0, r0
	bl Func_020024c0
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r5, r1
	ldr r0, [r3]
	bl Object_GetById
	ldr r2, .L_0200a834
	adds r6, r0, #0
	str r2, [sp, #0]
	bl Func_02004ca8
	movs r0, #0
	bl Func_02004da0
	ldr r3, .L_0200a838
	adds r0, r6, #0
	str r3, [r6, #108]
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r6, #6]
	movs r1, #49
	bl Func_02004bc0
.L_0200a696:
	adds r3, r6, #0
	adds r3, #34
	ldrb r0, [r3]
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	bl Func_02004c18
	ldr r3, [sp, #0]
	ldr r1, [sp, #0]
	adds r3, #104
	adds r1, #106
	mov r9, r1
	mov r10, r3
	add r1, sp, #4
	cmp r0, #7
	bne .L_0200a712
	movs r2, #0
	ldrsh r3, [r3, r2]
	mov r1, r9
	lsls r3, r3, #17
	str r3, [r6, #36]
	movs r5, #0
	movs r0, #0
	ldrsh r3, [r1, r0]
	lsls r3, r3, #17
	str r3, [r6, #44]
	movs r3, #128
	lsls r3, r3, #5
	str r3, [r6, #52]
.L_0200a6d0:
	mov r0, r10
	movs r3, #0
	ldrsh r2, [r0, r3]
	ldr r3, [r6, #8]
	lsls r2, r2, #19
	add r1, sp, #4
	adds r3, r3, r2
	str r3, [r1]
	mov r0, r9
	ldr r3, [r6, #12]
	str r3, [r1, #4]
	movs r3, #0
	ldrsh r2, [r0, r3]
	ldr r3, [r6, #16]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1, #8]
	adds r0, r6, #0
	bl Func_02004c38
	cmp r0, #0
	beq .L_0200a704
	movs r3, #0
	str r3, [r6, #36]
	str r3, [r6, #44]
	b .L_0200a810
.L_0200a704:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #9
	ble .L_0200a6d0
	b .L_0200a810
.L_0200a712:
	mov r0, r10
	movs r3, #0
	ldrsh r2, [r0, r3]
	ldr r3, [r6, #8]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1]
	mov r0, r9
	ldr r3, [r6, #12]
	str r3, [r1, #4]
	movs r3, #0
	ldrsh r2, [r0, r3]
	ldr r3, [r6, #16]
	lsls r2, r2, #19
	adds r3, r3, r2
	str r3, [r1, #8]
	adds r0, r6, #0
	bl Func_02004c38
	cmp r0, #0
	bgt .L_0200a810
	cmp r0, #0
	bge .L_0200a75c
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r6, #6]
	ldr r3, .L_0200a830
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	movs r2, #1
	ldr r0, [r3]
	movs r1, #6
	negs r2, r2
	bl Func_02004ce8
	b .L_0200a810
.L_0200a75c:
	ldrh r3, [r6, #32]
	movs r2, #0
	subs r3, #2
	mov r11, r3
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #20]
	mov r8, r2
	adds r7, r5, #0
	adds r7, #89
.L_0200a770:
	ldr r3, [r5]
	cmp r3, #0
	beq .L_0200a7a0
	ldrb r2, [r7]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0200a7a0
	cmp r5, r6
	beq .L_0200a7a0
	ldrh r3, [r5, #32]
	adds r0, r5, #0
	adds r0, #8
	subs r3, #2
	mov r1, r11
	add r2, sp, #4
	bl Func_02004c78
	cmp r0, #0
	blt .L_0200a7a0
	movs r0, #1
	bl WaitFrames
	b .L_0200a810
.L_0200a7a0:
	movs r3, #1
	add r8, r3
	mov r0, r8
	adds r7, #128
	adds r5, #128
	cmp r0, #63
	ble .L_0200a770
	mov r2, r10
	movs r1, #0
	ldrsh r3, [r2, r1]
	ldr r2, [r6, #8]
	lsls r3, r3, #17
	adds r1, r2, r3
	str r1, [r6, #8]
	mov r2, r9
	movs r0, #0
	ldrsh r3, [r2, r0]
	ldr r2, [r6, #16]
	ldr r7, .L_0200a83c
	lsls r3, r3, #17
	ldr r0, .L_0200a840
	adds r5, r2, r3
	adds r3, r1, #0
	ands r3, r7
	movs r4, #128
	adds r2, r3, r0
	lsls r4, r4, #9
	str r5, [r6, #16]
	cmp r2, r4
	ble .L_0200a7de
	adds r2, r4, #0
.L_0200a7de:
	ldr r0, .L_0200a844
	cmp r2, r0
	bge .L_0200a7e6
	adds r2, r0, #0
.L_0200a7e6:
	subs r3, r1, r2
	ldr r1, .L_0200a840
	str r3, [r6, #8]
	adds r3, r5, #0
	ands r3, r7
	adds r2, r3, r1
	cmp r2, r4
	ble .L_0200a7f8
	adds r2, r4, #0
.L_0200a7f8:
	cmp r2, r0
	bge .L_0200a7fe
	adds r2, r0, #0
.L_0200a7fe:
	subs r3, r5, r2
	str r3, [r6, #16]
	ldr r2, [sp, #0]
	movs r3, #0
	strh r3, [r2, #4]
	movs r0, #1
	bl WaitFrames
	b .L_0200a696
.L_0200a810:
	movs r3, #0
	str r3, [r6, #108]
	adds r0, r6, #0
	movs r1, #0
	bl Animation_ApplyChildValues
.L_0200a81c:
	ldr r0, [sp, #0]
	movs r3, #0
	strh r3, [r0, #4]
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200a830:
	.4byte gPartyState
.L_0200a834:
	.4byte Data_020023c4 + 0x188
.L_0200a838:
	.4byte Func_02002614
.L_0200a83c:
	.4byte 0x000fffff
.L_0200a840:
	.4byte 0xfff80000
.L_0200a844:
	.4byte 0xffff0000
	.endif
	.section .text.x0200a860,"ax",%progbits
	.balign 4
	.global Func_02002860
	.thumb_func
Func_02002860:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #108]
	movs r1, #217
	lsls r1, r1, #1
	adds r6, r5, r1
	ldrh r3, [r6]
	sub sp, #12
	cmp r3, #0
	bne .L_0200a88c
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r5, r2
	adds r1, #2
	ldr r0, [r3]
	adds r3, r5, r1
	ldr r1, [r3]
	bl Func_02004d50
	movs r3, #1
	strh r3, [r6]
.L_0200a88c:
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	movs r2, #179
	lsls r2, r2, #1
	movs r1, #173
	adds r3, r5, r2
	lsls r1, r1, #1
	movs r2, #0
	strh r2, [r3]
	adds r3, r5, r1
	adds r1, #4
	strh r2, [r3]
	adds r3, r5, r1
	subs r1, #2
	strh r2, [r3]
	adds r3, r5, r1
	adds r1, #8
	strh r2, [r3]
	movs r0, #10
	adds r3, r5, r1
	strh r2, [r3]
	movs r1, #0
	negs r0, r0
	bl Func_020024c0
	movs r0, #224
	movs r1, #224
	lsls r1, r1, #8
	lsls r0, r0, #11
	bl Func_02004d10
	ldr r3, .L_0200a990
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	adds r6, r0, #0
	movs r0, #131
	lsls r0, r0, #1
	ldr r7, .L_0200a994
	bl GameFlag_SetBit
	bl Func_02004ca8
	movs r0, #0
	bl Func_02004da0
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_ClearBit
	ldr r3, .L_0200a998
	movs r1, #49
	str r3, [r6, #108]
	movs r3, #128
	lsls r3, r3, #7
	strh r3, [r6, #6]
	adds r0, r6, #0
	bl Func_02004bc0
	ldr r3, [r7, #108]
	adds r3, #100
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	bne .L_0200a978
.L_0200a918:
	ldr r1, [r6, #8]
	ldr r2, [r6, #16]
	movs r0, #0
	bl Func_02004c18
	ldr r1, [r7, #108]
	cmp r0, #7
	beq .L_0200a938
	ldr r2, [r1, #112]
	ldr r3, [r6, #8]
	adds r3, r3, r2
	str r3, [r6, #8]
	ldr r3, [r6, #16]
	ldr r2, [r1, #120]
	adds r3, r3, r2
	b .L_0200a960
.L_0200a938:
	ldr r3, [r1, #112]
	cmp r3, #0
	beq .L_0200a94c
	ldr r3, [r6, #8]
	ldr r2, .L_0200a99c
	ands r3, r2
	movs r2, #128
	lsls r2, r2, #12
	adds r3, r3, r2
	str r3, [r6, #8]
.L_0200a94c:
	ldr r3, [r7, #108]
	ldr r3, [r3, #120]
	cmp r3, #0
	beq .L_0200a962
	ldr r3, [r6, #16]
	ldr r2, .L_0200a99c
	movs r1, #128
	ands r3, r2
	lsls r1, r1, #12
	adds r3, r3, r1
.L_0200a960:
	str r3, [r6, #16]
.L_0200a962:
	movs r3, #0
	strh r3, [r7, #4]
	movs r0, #1
	bl WaitFrames
	ldr r3, [r7, #108]
	adds r3, #100
	movs r2, #0
	ldrsh r3, [r3, r2]
	cmp r3, #0
	beq .L_0200a918
.L_0200a978:
	movs r5, #0
	movs r0, #30
	bl Battle_WaitMode0
	adds r0, r6, #0
	str r5, [r6, #108]
	movs r1, #0
	bl Animation_ApplyChildValues
	strh r5, [r7, #4]
	add sp, #12
	pop {r5, r6, r7, pc}
.L_0200a990:
	.4byte gPartyState
.L_0200a994:
	.4byte Data_020023c4 + 0x188
.L_0200a998:
	.4byte Func_02002614
.L_0200a99c:
	.4byte 0xfff00000
	.section .text.x0200a9c8,"ax",%progbits
	.balign 4
	.global Func_020029c8
	.thumb_func
Func_020029c8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r1, .L_0200ab30
	sub sp, #4
	ldr r3, [r1, #112]
	mov r11, r0
	cmp r3, #0
	bne .L_0200a9e4
	b .L_0200ab3c
.L_0200a9e4:
	movs r2, #0
	str r2, [sp, #0]
.L_0200a9e8:
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	mov r3, r11
	ldr r3, [r3, #8]
	lsls r5, r5, #4
	mov r8, r3
	add r8, r5
	lsls r0, r0, #4
	mov r1, r8
	subs r1, r1, r0
	mov r8, r1
	bl Random16Far
	adds r6, r0, #0
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	mov r2, r11
	ldr r3, [r2, #16]
	ldr r2, [r2, #12]
	lsls r5, r5, #4
	lsls r6, r6, #3
	movs r1, #128
	lsls r0, r0, #4
	adds r6, r6, r2
	lsls r1, r1, #11
	adds r3, r3, r5
	subs r3, r3, r0
	adds r6, r6, r1
	movs r0, #234
	adds r0, #255
	mov r1, r8
	adds r2, r6, #0
	bl Func_02004bd8
	adds r7, r0, #0
	cmp r7, #0
	beq .L_0200ab1c
	bl Random16Far
	mov r10, r0
	bl Random16Far
	adds r6, r0, #0
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	movs r2, #128
	lsls r2, r2, #8
	adds r5, r5, r0
	ldr r1, .L_0200ab34
	adds r0, r7, #0
	mov r9, r2
	bl Func_02004bd0
	adds r0, r7, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	mov r1, r10
	movs r2, #128
	lsls r2, r2, #10
	lsls r3, r1, #2
	adds r3, r3, r2
	str r3, [r7, #40]
	mov r0, r10
	bl Math_Cosine
	ldr r3, .L_0200ab38
	lsls r6, r6, #3
	mov r8, r3
	adds r1, r0, #0
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	str r0, [r7, #44]
	mov r0, r10
	bl Math_Sine
	adds r1, r0, #0
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	ldr r2, .L_0200ab30
	movs r3, #168
	lsls r3, r3, #7
	adds r3, #122
	str r3, [r7, #72]
	lsrs r5, r5, #2
	ldr r3, [r2, #112]
	add r5, r9
	movs r6, #0
	mov r1, r9
	str r5, [r7, #24]
	str r5, [r7, #28]
	str r1, [r7, #68]
	ldr r5, [r7, #80]
	str r0, [r7, #36]
	str r6, [r7, #52]
	ldr r3, [r3, #80]
	ldrb r0, [r5, #16]
	mov r8, r3
	bl Resource_ResetEntry
	ldrb r3, [r5, #17]
	ldr r1, .L_0200ab30
	movs r2, #1
	orrs r3, r2
	strb r3, [r5, #17]
	ldrh r3, [r1, #12]
	ldr r0, [r5, #40]
	strb r3, [r5, #16]
	bl ResourceMetadata_ClearRecord
	str r6, [r5, #40]
	strb r6, [r5, #27]
	mov r2, r8
	ldrb r3, [r2, #20]
	ldrb r0, [r5, #5]
	strb r3, [r5, #20]
	ldrb r3, [r2, #21]
	strb r3, [r5, #21]
	ldrb r1, [r2, #5]
	movs r2, #63
	adds r3, r2, #0
	lsrs r1, r1, #6
	lsls r1, r1, #6
	ands r3, r0
	orrs r3, r1
	strb r3, [r5, #5]
	mov r1, r8
	ldrb r3, [r1, #7]
	ldrb r1, [r5, #7]
	lsrs r3, r3, #6
	lsls r3, r3, #6
	ands r2, r1
	orrs r2, r3
	strb r2, [r5, #7]
	mov r3, r8
	ldrh r2, [r3, #8]
	ldr r1, .L_0200ab2c
	ldrh r3, [r5, #8]
	lsls r2, r2, #22
	lsrs r2, r2, #22
	ands r3, r1
	orrs r3, r2
	strh r3, [r5, #8]
.L_0200ab1c:
	ldr r1, [sp, #0]
	subs r1, #1
	str r1, [sp, #0]
	cmp r1, #0
	blt .L_0200ab28
	b .L_0200a9e8
.L_0200ab28:
	b .L_0200ab3c
	.2byte 0x0000
.L_0200ab2c:
	.4byte 0xfffffc00
.L_0200ab30:
	.4byte Data_020023c4 + 0x188
.L_0200ab34:
	.4byte Data_020056d0
.L_0200ab38:
	.4byte IwramMulQ16
.L_0200ab3c:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200ab4c,"ax",%progbits
	.global Func_02002b4c
	.thumb_func
Func_02002b4c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #16
	str r0, [sp, #12]
	str r1, [sp, #8]
	str r2, [sp, #4]
	str r3, [sp, #0]
	movs r3, #1
	mov r11, r3
.L_0200ab68:
	movs r0, #70
	adds r0, #255
	ldr r1, [sp, #12]
	ldr r2, [sp, #8]
	ldr r3, [sp, #4]
	bl Func_02004bd8
	adds r7, r0, #0
	cmp r7, #0
	beq .L_0200ac0a
	bl Random16Far
	adds r5, r0, #0
	bl Random16Far
	ldr r3, [sp, #0]
	lsrs r5, r5, #4
	adds r5, r3, r5
	lsrs r0, r0, #4
	movs r3, #128
	subs r5, r5, r0
	lsls r3, r3, #7
	mov r10, r3
	adds r3, r5, #0
	add r3, r10
	mov r9, r3
	bl Random16Far
	mov r8, r0
	mov r3, r8
	lsls r3, r3, #3
	mov r8, r3
	bl Random16Far
	ldr r1, .L_0200ac24
	adds r6, r0, #0
	adds r0, r7, #0
	bl Func_02004bd0
	movs r1, #0
	adds r0, r7, #0
	bl ObjectDispatch_SetSingleChildFlags
	movs r3, #160
	lsls r3, r3, #9
	adds r5, r5, r3
	str r5, [r7, #40]
	mov r0, r9
	bl Math_Cosine
	ldr r5, .L_0200ac28
	adds r1, r0, #0
	mov r0, r8
	mov lr, r5
	.2byte 0xf800
	str r0, [r7, #44]
	mov r0, r9
	bl Math_Sine
	adds r1, r0, #0
	mov r0, r8
	mov lr, r5
	.2byte 0xf800
	movs r3, #0
	str r3, [r7, #52]
	movs r3, #168
	lsls r3, r3, #7
	adds r3, #122
	lsrs r6, r6, #1
	str r3, [r7, #72]
	movs r3, #128
	add r6, r10
	lsls r3, r3, #8
	str r0, [r7, #36]
	str r6, [r7, #24]
	str r6, [r7, #28]
	str r3, [r7, #68]
	adds r0, r7, #0
	movs r1, #1
	bl Animation_ApplyChildValues
.L_0200ac0a:
	movs r3, #1
	negs r3, r3
	add r11, r3
	mov r3, r11
	cmp r3, #0
	bge .L_0200ab68
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200ac24:
	.4byte Data_02005714
.L_0200ac28:
	.4byte IwramMulQ16
	.section .text.x0200ac40,"ax",%progbits
	.balign 4
	.global Func_02002c40
	.thumb_func
Func_02002c40:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	movs r3, #192
	movs r0, #100
	lsls r3, r3, #18
	adds r0, r0, r5
	ldr r6, [r3, #108]
	movs r1, #0
	ldrsh r3, [r0, r1]
	sub sp, #56
	mov r8, r0
	cmp r3, #0
	beq .L_0200ac68
	b .L_0200aef4
.L_0200ac68:
	movs r0, #131
	lsls r0, r0, #1
	bl GameFlag_Test
	movs r2, #179
	lsls r2, r2, #1
	adds r3, r6, r2
	movs r4, #0
	ldrsh r3, [r3, r4]
	cmp r3, #0
	beq .L_0200ac82
	movs r3, #1
	orrs r0, r3
.L_0200ac82:
	adds r3, r5, #0
	adds r3, #91
	strb r0, [r3]
	add r7, sp, #44
	ldr r3, [r5, #8]
	movs r0, #128
	str r3, [r7]
	lsls r0, r0, #12
	ldr r3, [r5, #12]
	adds r2, r7, #0
	str r3, [r7, #4]
	ldr r3, [r5, #16]
	str r3, [r7, #8]
	ldrh r1, [r5, #6]
	bl Vector_AddPolarOffsetFar
	ldr r1, [r7]
	ldr r2, [r7, #8]
	movs r0, #0
	bl Func_02004c18
	cmp r0, #7
	bne .L_0200acf8
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r5, #64]
	str r3, [r5, #60]
	str r3, [r5, #56]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r5, #52]
	mov r0, r8
	movs r3, #1
	strh r3, [r0]
	movs r0, #145
	bl Func_02004de0
	movs r0, #160
	lsls r0, r0, #11
	movs r2, #128
	adds r1, r0, #0
	lsls r2, r2, #9
	bl Func_02004c50
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	negs r0, r0
	negs r1, r1
	adds r2, #102
	bl Func_02004c50
	ldr r3, [r5, #104]
	cmp r3, #0
	beq .L_0200acf8
	adds r0, r5, #0
	mov lr, r3
	.2byte 0xf800
.L_0200acf8:
	movs r3, #192
	lsls r3, r3, #18
	ldr r0, [r3, #32]
	ldr r3, [r5, #8]
	ldr r1, [r5, #16]
	asrs r3, r3, #20
	str r3, [sp, #32]
	movs r3, #184
	lsls r3, r3, #1
	ldr r4, [sp, #32]
	asrs r1, r1, #20
	adds r2, r0, r3
	ldr r2, [r2]
	lsls r3, r1, #7
	adds r3, r4, r3
	lsls r3, r3, #2
	adds r2, r2, r3
	str r2, [sp, #28]
	movs r4, #212
	lsls r4, r4, #1
	adds r2, r0, r4
	ldr r2, [r2]
	subs r4, #92
	adds r2, r2, r3
	str r2, [sp, #24]
	movs r2, #164
	lsls r2, r2, #1
	adds r3, r0, r2
	ldr r3, [r3]
	asrs r3, r3, #20
	str r3, [sp, #20]
	adds r3, r0, r4
	adds r4, #52
	ldr r2, [r3]
	adds r3, r0, r4
	ldr r3, [r3]
	adds r4, #4
	asrs r3, r3, #20
	str r3, [sp, #16]
	adds r3, r0, r4
	ldr r3, [r3]
	movs r0, #1
	asrs r3, r3, #20
	adds r3, r1, r3
	subs r3, #2
	asrs r2, r2, #20
	negs r0, r0
	str r3, [sp, #8]
	str r0, [sp, #40]
	subs r3, r1, #1
	adds r1, r1, r2
	subs r1, #1
	mov r8, r3
	mov r11, r1
.L_0200ad64:
	ldr r0, [sp, #32]
	ldr r1, [sp, #16]
	ldr r2, [sp, #20]
	movs r4, #1
	adds r3, r0, r1
	subs r3, #1
	negs r4, r4
	mov r9, r3
	str r4, [sp, #36]
	adds r3, r0, r2
	adds r6, r0, #0
	subs r3, #1
	subs r6, #1
	mov r10, r3
.L_0200ad80:
	ldr r4, [sp, #40]
	ldr r0, [sp, #36]
	lsls r3, r4, #7
	adds r3, r3, r0
	lsls r3, r3, #2
	ldr r1, [sp, #28]
	str r3, [sp, #12]
	adds r2, r3, r1
	ldrb r3, [r2, #2]
	cmp r3, #77
	bne .L_0200ade0
	movs r3, #0
	strb r3, [r2, #2]
	mov r2, r10
	mov r3, r11
	str r2, [sp, #0]
	str r3, [sp, #4]
	movs r0, #64
	movs r1, #64
	movs r2, #1
	movs r3, #1
	bl Func_02004c30
	mov r4, r8
	movs r1, #64
	movs r2, #1
	movs r3, #1
	movs r0, #64
	str r4, [sp, #4]
	str r6, [sp, #0]
	bl Func_02004c28
	movs r0, #159
	lsls r0, r0, #1
	adds r0, #255
	bl Func_02004de0
	mov r3, r8
	movs r4, #128
	lsls r4, r4, #12
	lsls r2, r3, #20
	lsls r0, r6, #20
	adds r0, r0, r4
	ldr r1, [r5, #12]
	ldrh r3, [r5, #6]
	adds r2, r2, r4
	bl Func_02002b4c
.L_0200ade0:
	ldr r4, [sp, #12]
	ldr r0, [sp, #24]
	adds r2, r4, r0
	ldrb r3, [r2, #2]
	cmp r3, #77
	bne .L_0200ae34
	movs r3, #0
	strb r3, [r2, #2]
	ldr r2, [sp, #8]
	mov r1, r9
	str r1, [sp, #0]
	str r2, [sp, #4]
	movs r0, #64
	movs r1, #64
	movs r2, #1
	movs r3, #2
	bl Func_02004c30
	mov r3, r8
	str r3, [sp, #4]
	movs r1, #64
	movs r2, #1
	movs r3, #1
	movs r0, #64
	str r6, [sp, #0]
	bl Func_02004c28
	movs r0, #143
	lsls r0, r0, #2
	bl Func_02004de0
	mov r3, r8
	movs r4, #128
	lsls r4, r4, #12
	lsls r2, r3, #20
	lsls r0, r6, #20
	adds r0, r0, r4
	ldr r1, [r5, #12]
	ldrh r3, [r5, #6]
	adds r2, r2, r4
	bl Func_02002b4c
.L_0200ae34:
	ldr r0, [sp, #36]
	movs r4, #1
	adds r0, #1
	add r9, r4
	adds r6, #1
	add r10, r4
	str r0, [sp, #36]
	cmp r0, #1
	ble .L_0200ad80
	ldr r1, [sp, #8]
	ldr r2, [sp, #40]
	adds r1, #1
	adds r2, #1
	str r1, [sp, #8]
	add r8, r4
	add r11, r4
	str r2, [sp, #40]
	cmp r2, #1
	ble .L_0200ad64
	ldr r3, [r5, #24]
	movs r4, #128
	lsls r4, r4, #9
	cmp r3, r4
	bge .L_0200ae72
	movs r2, #128
	lsls r2, r2, #5
	adds r3, r3, r2
	str r3, [r5, #24]
	ldr r3, [r5, #28]
	adds r3, r3, r2
	str r3, [r5, #28]
.L_0200ae72:
	ldr r1, [r7]
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	adds r0, r5, #0
	bl Func_02004bf8
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [r3, #108]
	ldr r3, .L_0200af38
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	ldr r6, .L_0200af3c
	bl Object_GetById
	ldr r1, [r5, #8]
	ldr r3, [r0, #8]
	subs r2, r1, r3
	cmp r2, #0
	blt .L_0200aea8
	movs r1, #160
	lsls r1, r1, #13
	cmp r2, r1
	blt .L_0200aeb2
	b .L_0200af28
.L_0200aea8:
	movs r2, #160
	subs r3, r3, r1
	lsls r2, r2, #13
	cmp r3, r2
	bge .L_0200af28
.L_0200aeb2:
	ldr r3, [r5, #12]
	ldr r2, [r0, #12]
	ldr r4, .L_0200af40
	ldr r1, .L_0200af44
	subs r3, r3, r2
	adds r3, r3, r4
	cmp r3, r1
	bhi .L_0200af28
	ldr r3, [r5, #16]
	ldr r0, [r0, #16]
	subs r2, r3, r0
	cmp r2, #0
	blt .L_0200aed6
	movs r3, #160
	lsls r3, r3, #13
	cmp r2, r3
	blt .L_0200aee0
	b .L_0200af28
.L_0200aed6:
	movs r4, #160
	subs r3, r0, r3
	lsls r4, r4, #13
	cmp r3, r4
	bge .L_0200af28
.L_0200aee0:
	movs r3, #2
	strh r3, [r6, #4]
	ldrh r3, [r6, #10]
	movs r0, #170
	lsls r0, r0, #1
	adds r3, #2
	adds r2, r7, r0
	str r5, [r6, #108]
	strh r3, [r2]
	b .L_0200af28
.L_0200aef4:
	cmp r3, #1
	bne .L_0200af28
	adds r3, r5, #0
	adds r3, #91
	movs r2, #0
	strb r2, [r3]
	ldr r3, [r5, #24]
	cmp r3, #0
	ble .L_0200af1a
	ldr r2, .L_0200af48
	adds r0, r5, #0
	adds r3, r3, r2
	str r3, [r5, #24]
	ldr r3, [r5, #28]
	adds r3, r3, r2
	str r3, [r5, #28]
	bl Func_020029c8
	b .L_0200af28
.L_0200af1a:
	str r2, [r5, #16]
	str r2, [r5, #12]
	str r2, [r5, #8]
	str r2, [r5, #44]
	str r2, [r5, #40]
	str r2, [r5, #36]
	str r2, [r5, #108]
.L_0200af28:
	add sp, #56
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200af38:
	.4byte gPartyState
.L_0200af3c:
	.4byte Data_020023c4 + 0x188
.L_0200af40:
	.4byte 0x0007ffff
.L_0200af44:
	.4byte 0x001ffffe
.L_0200af48:
	.4byte 0xfffff000
	.section .text.x0200afc4,"ax",%progbits
	.balign 4
	.global Func_02002fc4
	.thumb_func
Func_02002fc4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r11, r0
	mov r3, r11
	adds r3, #98
	ldrb r0, [r3]
	sub sp, #12
	bl Object_GetById
	mov r3, r11
	adds r7, r0, #0
	adds r3, #102
	movs r2, #0
	ldrsh r6, [r3, r2]
	ldr r3, [r7, #80]
	mov r2, r11
	mov r9, r3
	ldr r3, [r2, #8]
	mov r5, sp
	str r3, [r5]
	movs r0, #128
	ldr r3, [r2, #12]
	ldr r2, .L_0200b054
	adds r1, r6, #0
	adds r3, r3, r2
	str r3, [r5, #4]
	mov r2, r11
	ldr r3, [r2, #16]
	lsls r0, r0, #14
	adds r2, r5, #0
	str r3, [r5, #8]
	bl Vector_AddPolarOffsetFar
	ldr r3, .L_0200b050
	movs r2, #0
	mov r10, r3
	adds r3, r7, #0
	mov r8, r2
	adds r3, #85
	mov r2, r10
	strh r6, [r7, #6]
	strb r2, [r3]
	adds r0, r7, #0
	ldr r1, [r5]
	ldr r2, [r5, #4]
	ldr r3, [r5, #8]
	bl Object_SetPositionAndResetMotion
	ldr r3, .L_0200b058
	mov r2, r10
	str r3, [r7, #108]
	adds r3, r7, #0
	adds r3, #90
	strb r2, [r3]
	movs r3, #128
	lsls r3, r3, #12
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #6
	str r3, [r7, #52]
	ldr r3, .L_0200b05c
	mov r2, r9
	adds r6, r6, r3
	b .L_0200b060
	.2byte 0x0000
.L_0200b050:
	.4byte 0x00000000
.L_0200b054:
	.4byte 0xfff40000
.L_0200b058:
	.4byte Func_02002c40
.L_0200b05c:
	.4byte 0xffffc000
.L_0200b060:
	mov r3, r8
	strh r6, [r2, #18]
	str r3, [r7, #24]
	str r3, [r7, #28]
	adds r3, r7, #0
	mov r2, r8
	adds r3, #100
	strh r2, [r3]
	mov r2, r11
	ldr r3, [r2, #104]
	movs r0, #104
	str r3, [r7, #104]
	bl Func_02004de0
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200b08c,"ax",%progbits
	.global Func_0200308c
	.thumb_func
Func_0200308c:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	adds r7, r5, #0
	adds r7, #100
	movs r3, #0
	ldrsh r6, [r7, r3]
	cmp r6, #0
	bne .L_0200b0c4
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	movs r0, #0
	bl Map_GetTerrainHeight
	movs r3, #128
	lsls r3, r3, #13
	adds r0, r0, r3
	movs r3, #128
	lsls r3, r3, #9
	str r0, [r5, #12]
	movs r1, #1
	adds r0, r5, #0
	str r6, [r5, #24]
	str r6, [r5, #28]
	str r3, [r5, #48]
	str r3, [r5, #52]
	bl Func_02004c68
	b .L_0200b12c
.L_0200b0c4:
	cmp r6, #30
	bgt .L_0200b0dc
	cmp r6, #30
	bne .L_0200b12c
	movs r0, #136
	bl Func_02004de0
	ldr r0, [r5, #104]
	movs r1, #2
	bl Func_02004bc0
	b .L_0200b12c
.L_0200b0dc:
	cmp r6, #60
	bgt .L_0200b104
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #24]
	str r3, [r5, #28]
	ldrh r3, [r7]
	ldr r0, [r5, #104]
	lsls r3, r3, #16
	asrs r2, r3, #16
	lsrs r3, r3, #31
	adds r2, r2, r3
	ldr r3, .L_0200b128
	asrs r2, r2, #1
	ands r2, r3
	lsls r1, r2, #3
	subs r1, r1, r2
	bl Animation_ApplyChildValues
	b .L_0200b12c
.L_0200b104:
	movs r1, #1
	ldr r0, [r5, #104]
	bl Func_02004bc0
	movs r0, #184
	bl Func_02004de0
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	movs r0, #0
	bl Map_GetTerrainHeight
	movs r3, #0
	str r0, [r5, #12]
	str r3, [r5, #104]
	movs r0, #0
	b .L_0200b134
	.2byte 0x0000
.L_0200b128:
	.4byte 0x00000001
.L_0200b12c:
	ldrh r3, [r7]
	movs r0, #1
	adds r3, #1
	strh r3, [r7]
.L_0200b134:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200b138,"ax",%progbits
	.global Func_02003138
	.thumb_func
Func_02003138:
	push {r5, r6, r7, lr}
	adds r5, r0, #0
	adds r7, r5, #0
	adds r7, #100
	movs r3, #0
	ldrsh r6, [r7, r3]
	cmp r6, #0
	bne .L_0200b182
	movs r0, #136
	bl Func_02004de0
	ldr r0, [r5, #104]
	movs r1, #2
	bl Func_02004bc0
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	movs r0, #0
	bl Map_GetTerrainHeight
	movs r3, #128
	lsls r3, r3, #13
	adds r0, r0, r3
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #48]
	movs r3, #128
	str r0, [r5, #12]
	lsls r3, r3, #8
	adds r0, r5, #0
	movs r1, #1
	str r6, [r5, #24]
	str r6, [r5, #28]
	str r3, [r5, #52]
	bl Func_02004c68
	b .L_0200b1d0
.L_0200b182:
	cmp r6, #32
	bgt .L_0200b1aa
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #24]
	str r3, [r5, #28]
	ldrh r3, [r7]
	ldr r0, [r5, #104]
	lsls r3, r3, #16
	asrs r2, r3, #16
	lsrs r3, r3, #31
	adds r2, r2, r3
	ldr r3, .L_0200b1cc
	asrs r2, r2, #1
	ands r2, r3
	lsls r1, r2, #3
	subs r1, r1, r2
	bl Animation_ApplyChildValues
	b .L_0200b1d0
.L_0200b1aa:
	movs r1, #1
	ldr r0, [r5, #104]
	bl Func_02004bc0
	movs r0, #184
	bl Func_02004de0
	ldr r1, [r5, #8]
	ldr r2, [r5, #16]
	movs r0, #0
	bl Map_GetTerrainHeight
	movs r3, #0
	str r0, [r5, #12]
	str r3, [r5, #104]
	movs r0, #0
	b .L_0200b1d8
.L_0200b1cc:
	.4byte 0x00000001
.L_0200b1d0:
	ldrh r3, [r7]
	movs r0, #1
	adds r3, #1
	strh r3, [r7]
.L_0200b1d8:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200b2b8,"ax",%progbits
	.balign 4
	.global Func_020032b8
	.thumb_func
Func_020032b8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_0200b378
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	sub sp, #40
	bl ObjectTable_Get
	adds r7, r0, #0
.L_0200b2d8:
	bl Func_02003284
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r7, #52]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r7, #56]
	str r3, [r7, #64]
	movs r3, #0
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r1, .L_0200b37c
	ldr r3, [r7, #8]
	add r2, sp, #28
	mov r10, r2
	movs r2, #128
	lsls r2, r2, #12
	ands r3, r1
	adds r3, r3, r2
	mov r0, r10
	str r3, [sp, #12]
	str r3, [r0]
	ldr r3, [r7, #12]
	str r3, [r0, #4]
	ldr r3, [r7, #16]
	ands r3, r1
	adds r3, r3, r2
	str r3, [sp, #8]
	str r3, [r0, #8]
	ldr r2, [sp, #12]
	str r3, [sp, #16]
	adds r3, r7, #0
	adds r3, #34
	str r2, [sp, #20]
	str r3, [sp, #4]
	adds r1, r2, #0
	ldrb r0, [r3]
	ldr r2, [sp, #16]
	bl Func_02004c10
	str r0, [sp, #24]
	ldr r2, [sp, #20]
	ldr r1, [r7, #8]
	ldr r0, [sp, #16]
	ldr r3, .L_0200b380
	ldr r6, [r7, #16]
	subs r1, r2, r1
	subs r6, r0, r6
	mov r8, r3
	adds r0, r1, #0
	mov lr, r8
	.2byte 0xf800
	adds r1, r6, #0
	adds r5, r0, #0
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	adds r5, r5, r0
	movs r0, #128
	lsls r0, r0, #11
	cmp r5, r0
	bge .L_0200b390
	ldr r3, .L_0200b384
	movs r2, #15
	ldr r3, [r3]
	ldr r1, .L_0200b388
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r2, [r1, r3]
	mov r9, r2
	lsls r3, r2, #16
	ldr r2, .L_0200b38c
	cmp r3, r2
	bne .L_0200b3bc
	b .L_0200b552
.L_0200b378:
	.4byte gPartyState
.L_0200b37c:
	.4byte 0xfff00000
.L_0200b380:
	.4byte IwramMulQ16
.L_0200b384:
	.4byte gInput
.L_0200b388:
	.4byte Data_02005798
.L_0200b38c:
	.4byte 0xffff0000
.L_0200b390:
	ldr r3, [sp, #16]
	ldr r2, [sp, #20]
	ldr r0, [r7, #16]
	ldr r1, [r7, #8]
	subs r0, r3, r0
	subs r1, r2, r1
	bl ArcTan2
	lsls r0, r0, #16
	asrs r0, r0, #16
	movs r3, #128
	ldr r2, .L_0200b3b8
	mov r9, r0
	lsls r3, r3, #6
	add r3, r9
	ands r3, r2
	lsls r3, r3, #16
	asrs r3, r3, #16
	mov r9, r3
	b .L_0200b3bc
.L_0200b3b8:
	.4byte 0xffffc000
.L_0200b3bc:
	movs r0, #128
	add r2, sp, #28
	lsls r0, r0, #13
	mov r1, r9
	bl Vector_AddPolarOffsetFar
	ldr r3, [sp, #4]
	mov r2, r10
	ldr r1, [r2]
	ldrb r0, [r3]
	ldr r2, [r2, #8]
	bl Func_02004c10
	mov r11, r0
	cmp r0, #255
	beq .L_0200b43e
	ldr r3, [sp, #4]
	mov r2, r10
	ldrb r0, [r3]
	ldr r1, [r2]
	ldr r2, [r2, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt .L_0200b43e
	ldr r0, [sp, #12]
	mov r2, r10
	str r0, [r2]
	ldr r3, [sp, #8]
	adds r0, r7, #0
	str r3, [r2, #8]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	adds r2, r7, #0
	str r3, [r7, #52]
	adds r2, #100
	movs r3, #0
	strh r3, [r2]
	ldr r1, [sp, #12]
	ldr r3, [sp, #8]
	ldr r2, [r7, #12]
	bl Func_02004bf8
	adds r0, r7, #0
	movs r1, #2
	bl Func_02004bc0
	adds r0, r7, #0
	movs r1, #48
	bl ObjectDispatch_ApplyValueToChildren
	adds r0, r7, #0
	bl Func_02004c00
	ldr r3, .L_0200b560
	str r3, [r7, #108]
	b .L_0200b4e8
.L_0200b43e:
	movs r3, #0
	mov r0, r9
	strh r0, [r7, #6]
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r2, [sp, #12]
	str r2, [r7, #8]
	ldr r3, [sp, #8]
	str r3, [r7, #16]
	b .L_0200b534
.L_0200b452:
	ldr r2, [sp, #4]
	ldr r1, [r6]
	ldrb r0, [r2]
	ldr r2, [r6, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt .L_0200b508
	ldrh r3, [r7, #32]
	movs r2, #89
	subs r3, #2
	str r3, [sp, #0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #20]
	movs r0, #0
	adds r2, r2, r5
	mov r10, r0
	mov r8, r2
.L_0200b480:
	ldr r3, [r5]
	cmp r3, #0
	beq .L_0200b4aa
	mov r3, r8
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0200b4aa
	cmp r5, r7
	beq .L_0200b4aa
	ldrh r3, [r5, #32]
	adds r0, r5, #0
	adds r0, #8
	subs r3, #2
	ldr r1, [sp, #0]
	add r2, sp, #28
	bl Func_02004c78
	cmp r0, #0
	bge .L_0200b508
.L_0200b4aa:
	movs r0, #1
	add r10, r0
	movs r2, #128
	mov r3, r10
	add r8, r2
	adds r5, #128
	cmp r3, #63
	ble .L_0200b480
	ldr r0, [r6]
	movs r3, #128
	str r0, [sp, #20]
	lsls r3, r3, #10
	ldr r2, [r6, #8]
	adds r0, r7, #0
	str r2, [sp, #16]
	str r3, [r7, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	str r3, [r7, #52]
	ldr r3, [r6, #8]
	ldr r1, [r6]
	ldr r2, [r6, #4]
	bl Func_02004bf8
	adds r0, r7, #0
	bl Func_02004c00
	ldr r3, [sp, #24]
	cmp r11, r3
	bne .L_0200b52e
.L_0200b4e8:
	movs r0, #128
	add r2, sp, #28
	lsls r0, r0, #13
	mov r1, r9
	bl Vector_AddPolarOffsetFar
	ldr r2, [sp, #4]
	add r6, sp, #28
	ldrb r0, [r2]
	ldr r1, [r6]
	ldr r2, [r6, #8]
	bl Func_02004c10
	mov r11, r0
	cmp r0, #255
	bne .L_0200b452
.L_0200b508:
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #52]
	ldr r2, [r7, #12]
	adds r0, r7, #0
	ldr r1, [sp, #20]
	ldr r3, [sp, #16]
	bl Func_02004bf8
	adds r0, r7, #0
	bl Func_02004c00
	movs r0, #2
	bl WaitFrames
	b .L_0200b2d8
.L_0200b52e:
	movs r0, #10
	bl WaitFrames
.L_0200b534:
	movs r3, #0
	str r3, [r7, #108]
	adds r1, r7, #0
	adds r1, #90
	ldrb r2, [r1]
	movs r3, #1
	orrs r3, r2
	strb r3, [r1]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #52]
	adds r0, r7, #0
	movs r1, #1
	bl Func_02004bc0
.L_0200b552:
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200b560:
	.4byte Func_020031f8
	.section .text.x0200b564,"ax",%progbits
	.global Func_02003564
	.thumb_func
Func_02003564:
	.ifdef TLA_EDITION_JA
	push {r5, r6, r7, lr}
	mov r7, fp
	mov r6, sl
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_ja_0200b564
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3, #0]
	sub sp, #40
	bl ObjectTable_Get
	ldr r2, .L_ja_0200b568
	ldr r3, .L_ja_0200b560
	adds r7, r0, #0
	strh r3, [r2, #0]
.L_ja_0200b52a:
	bl Func_02003284
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r7, #52]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r7, #56]
	str r3, [r7, #64]
	movs r3, #0
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r1, .L_ja_0200b56c
	ldr r3, [r7, #8]
	add r2, sp, #28
	mov sl, r2
	movs r2, #128
	lsls r2, r2, #12
	ands r3, r1
	adds r3, r3, r2
	mov r0, sl
	str r3, [sp, #12]
	str r3, [r0, #0]
	b .L_ja_0200b570
.L_ja_0200b560:
	.4byte 0x00000000
.L_ja_0200b564:
	.4byte gPartyState
.L_ja_0200b568:
	.4byte Data_020068b0
.L_ja_0200b56c:
	.4byte 0xfff00000
.L_ja_0200b570:
	ldr r3, [r7, #12]
	str r3, [r0, #4]
	ldr r3, [r7, #16]
	ands r3, r1
	adds r3, r3, r2
	str r3, [sp, #8]
	str r3, [r0, #8]
	ldr r2, [sp, #12]
	str r3, [sp, #16]
	adds r3, r7, #0
	adds r3, #34
	str r2, [sp, #20]
	str r3, [sp, #4]
	adds r1, r2, #0
	ldrb r0, [r3, #0]
	ldr r2, [sp, #16]
	bl Func_02004c10
	str r0, [sp, #24]
	ldr r2, [sp, #20]
	ldr r1, [r7, #8]
	ldr r0, [sp, #16]
	ldr r3, .L_ja_0200b5dc
	ldr r6, [r7, #16]
	subs r1, r2, r1
	subs r6, r0, r6
	mov r8, r3
	adds r0, r1, #0
	mov lr, r8
	.2byte 0xf800
	adds r1, r6, #0
	adds r5, r0, #0
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	adds r5, r5, r0
	movs r0, #128
	lsls r0, r0, #11
	cmp r5, r0
	bge .L_ja_0200b5ec
	ldr r3, .L_ja_0200b5e0
	movs r2, #15
	ldr r3, [r3, #0]
	ldr r1, .L_ja_0200b5e4
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r2, [r1, r3]
	mov r9, r2
	lsls r3, r2, #16
	ldr r2, .L_ja_0200b5e8
	cmp r3, r2
	bne .L_ja_0200b618
	b .L_ja_0200b7e0
.L_ja_0200b5dc:
	.4byte IwramMulQ16
.L_ja_0200b5e0:
	.4byte gInput
.L_ja_0200b5e4:
	.4byte Data_02005798
.L_ja_0200b5e8:
	.4byte 0xffff0000
.L_ja_0200b5ec:
	ldr r3, [sp, #16]
	ldr r2, [sp, #20]
	ldr r0, [r7, #16]
	ldr r1, [r7, #8]
	subs r0, r3, r0
	subs r1, r2, r1
	bl ArcTan2
	lsls r0, r0, #16
	asrs r0, r0, #16
	movs r3, #128
	ldr r2, .L_ja_0200b614
	mov r9, r0
	lsls r3, r3, #6
	add r3, r9
	ands r3, r2
	lsls r3, r3, #16
	asrs r3, r3, #16
	mov r9, r3
	b .L_ja_0200b618
.L_ja_0200b614:
	.4byte 0xffffc000
.L_ja_0200b618:
	movs r0, #128
	add r2, sp, #28
	lsls r0, r0, #13
	mov r1, r9
	bl Vector_AddPolarOffsetFar
	ldr r3, [sp, #4]
	mov r2, sl
	ldr r1, [r2, #0]
	ldrb r0, [r3, #0]
	ldr r2, [r2, #8]
	bl Func_02004c10
	mov fp, r0
	cmp r0, #255
	beq .L_ja_0200b69a
	ldr r3, [sp, #4]
	mov r2, sl
	ldrb r0, [r3, #0]
	ldr r1, [r2, #0]
	ldr r2, [r2, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt .L_ja_0200b69a
	ldr r0, [sp, #12]
	mov r2, sl
	str r0, [r2, #0]
	ldr r3, [sp, #8]
	adds r0, r7, #0
	str r3, [r2, #8]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	adds r2, r7, #0
	str r3, [r7, #52]
	adds r2, #100
	movs r3, #0
	strh r3, [r2, #0]
	ldr r1, [sp, #12]
	ldr r3, [sp, #8]
	ldr r2, [r7, #12]
	bl Func_02004bf8
	adds r0, r7, #0
	movs r1, #2
	bl Func_02004bc0
	adds r0, r7, #0
	movs r1, #48
	bl ObjectDispatch_ApplyValueToChildren
	adds r0, r7, #0
	bl Func_02004c00
	ldr r3, .L_ja_0200b7f0
	str r3, [r7, #108]
	b .L_ja_0200b776
.L_ja_0200b69a:
	movs r3, #0
	mov r0, r9
	strh r0, [r7, #6]
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r2, [sp, #12]
	str r2, [r7, #8]
	ldr r3, [sp, #8]
	str r3, [r7, #16]
	b .L_ja_0200b7c2
.L_ja_0200b6ae:
	ldr r2, [sp, #4]
	ldr r1, [r6, #0]
	ldrb r0, [r2, #0]
	ldr r2, [r6, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt .L_ja_0200b796
	ldr r3, .L_ja_0200b7f4
	ldrh r3, [r3, #0]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_ja_0200b796
	ldrh r3, [r7, #32]
	movs r2, #89
	subs r3, #2
	str r3, [sp, #0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #20]
	movs r0, #0
	adds r2, r2, r5
	mov sl, r0
	mov r8, r2
.L_ja_0200b6e6:
	ldr r3, [r5, #0]
	cmp r3, #0
	beq .L_ja_0200b710
	mov r3, r8
	ldrb r2, [r3, #0]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_ja_0200b710
	cmp r5, r7
	beq .L_ja_0200b710
	ldrh r3, [r5, #32]
	adds r0, r5, #0
	adds r0, #8
	subs r3, #2
	ldr r1, [sp, #0]
	add r2, sp, #28
	bl Func_02004c78
	cmp r0, #0
	bge .L_ja_0200b796
.L_ja_0200b710:
	movs r0, #1
	add sl, r0
	movs r2, #128
	mov r3, sl
	add r8, r2
	adds r5, #128
	cmp r3, #63
	ble .L_ja_0200b6e6
	ldr r0, [r6, #0]
	movs r3, #128
	str r0, [sp, #20]
	lsls r3, r3, #10
	ldr r2, [r6, #8]
	adds r0, r7, #0
	str r2, [sp, #16]
	str r3, [r7, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	str r3, [r7, #52]
	movs r5, #0
	ldr r1, [r6, #0]
	ldr r2, [r6, #4]
	ldr r3, [r6, #8]
	bl Func_02004bf8
	b .L_ja_0200b74e
.L_ja_0200b746:
	movs r0, #1
	bl WaitFrames
	adds r5, #1
.L_ja_0200b74e:
	cmp r5, #179
	bgt .L_ja_0200b766
	adds r0, r7, #0
	bl Func_02004c70
	cmp r0, #0
	bne .L_ja_0200b766
	ldr r3, .L_ja_0200b7f4
	ldrh r3, [r3, #0]
	lsls r3, r3, #16
	cmp r3, #0
	beq .L_ja_0200b746
.L_ja_0200b766:
	ldr r3, .L_ja_0200b7f4
	ldrh r3, [r3, #0]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_ja_0200b7c2
	ldr r3, [sp, #24]
	cmp fp, r3
	bne .L_ja_0200b7bc
.L_ja_0200b776:
	movs r0, #128
	add r2, sp, #28
	lsls r0, r0, #13
	mov r1, r9
	bl Vector_AddPolarOffsetFar
	ldr r2, [sp, #4]
	add r6, sp, #28
	ldrb r0, [r2, #0]
	ldr r1, [r6, #0]
	ldr r2, [r6, #8]
	bl Func_02004c10
	mov fp, r0
	cmp r0, #255
	bne .L_ja_0200b6ae
.L_ja_0200b796:
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #52]
	ldr r2, [r7, #12]
	adds r0, r7, #0
	ldr r1, [sp, #20]
	ldr r3, [sp, #16]
	bl Func_02004bf8
	adds r0, r7, #0
	bl Func_02004c00
	movs r0, #2
	bl WaitFrames
	b .L_ja_0200b52a
.L_ja_0200b7bc:
	movs r0, #10
	bl WaitFrames
.L_ja_0200b7c2:
	movs r3, #0
	str r3, [r7, #108]
	adds r1, r7, #0
	adds r1, #90
	ldrb r2, [r1, #0]
	movs r3, #1
	orrs r3, r2
	strb r3, [r1, #0]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #52]
	adds r0, r7, #0
	movs r1, #1
	bl Func_02004bc0
.L_ja_0200b7e0:
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov sl, r6
	mov fp, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_ja_0200b7f0:
	.4byte Func_020031f8
.L_ja_0200b7f4:
	.4byte Data_020068b0
	.else
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_0200b5c4
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	ldr r0, [r3]
	sub sp, #40
	bl ObjectTable_Get
	ldr r2, .L_0200b5c8
	ldr r3, .L_0200b5c0
	adds r7, r0, #0
	strh r3, [r2]
.L_0200b58a:
	bl Func_02003284
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r7, #52]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r7, #56]
	str r3, [r7, #64]
	movs r3, #0
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r1, .L_0200b5cc
	ldr r3, [r7, #8]
	add r2, sp, #28
	mov r10, r2
	movs r2, #128
	lsls r2, r2, #12
	ands r3, r1
	adds r3, r3, r2
	mov r0, r10
	str r3, [sp, #12]
	str r3, [r0]
	b .L_0200b5d0
.L_0200b5c0:
	.4byte 0x00000000
.L_0200b5c4:
	.4byte gPartyState
.L_0200b5c8:
	.4byte Data_020068b0
.L_0200b5cc:
	.4byte 0xfff00000
.L_0200b5d0:
	ldr r3, [r7, #12]
	str r3, [r0, #4]
	ldr r3, [r7, #16]
	ands r3, r1
	adds r3, r3, r2
	str r3, [sp, #8]
	str r3, [r0, #8]
	ldr r2, [sp, #12]
	str r3, [sp, #16]
	adds r3, r7, #0
	adds r3, #34
	str r2, [sp, #20]
	str r3, [sp, #4]
	adds r1, r2, #0
	ldrb r0, [r3]
	ldr r2, [sp, #16]
	bl Func_02004c10
	str r0, [sp, #24]
	ldr r2, [sp, #20]
	ldr r1, [r7, #8]
	ldr r0, [sp, #16]
	ldr r3, .L_0200b63c
	ldr r6, [r7, #16]
	subs r1, r2, r1
	subs r6, r0, r6
	mov r8, r3
	adds r0, r1, #0
	mov lr, r8
	.2byte 0xf800
	adds r1, r6, #0
	adds r5, r0, #0
	adds r0, r6, #0
	mov lr, r8
	.2byte 0xf800
	adds r5, r5, r0
	movs r0, #128
	lsls r0, r0, #11
	cmp r5, r0
	bge .L_0200b64c
	ldr r3, .L_0200b640
	movs r2, #15
	ldr r3, [r3]
	ldr r1, .L_0200b644
	lsrs r3, r3, #4
	ands r3, r2
	lsls r3, r3, #1
	ldrsh r2, [r1, r3]
	mov r9, r2
	lsls r3, r2, #16
	ldr r2, .L_0200b648
	cmp r3, r2
	bne .L_0200b678
	b .L_0200b842
.L_0200b63c:
	.4byte IwramMulQ16
.L_0200b640:
	.4byte gInput
.L_0200b644:
	.4byte Data_02005798
.L_0200b648:
	.4byte 0xffff0000
.L_0200b64c:
	ldr r3, [sp, #16]
	ldr r2, [sp, #20]
	ldr r0, [r7, #16]
	ldr r1, [r7, #8]
	subs r0, r3, r0
	subs r1, r2, r1
	bl ArcTan2
	lsls r0, r0, #16
	asrs r0, r0, #16
	movs r3, #128
	ldr r2, .L_0200b674
	mov r9, r0
	lsls r3, r3, #6
	add r3, r9
	ands r3, r2
	lsls r3, r3, #16
	asrs r3, r3, #16
	mov r9, r3
	b .L_0200b678
.L_0200b674:
	.4byte 0xffffc000
.L_0200b678:
	movs r0, #128
	add r2, sp, #28
	lsls r0, r0, #13
	mov r1, r9
	bl Vector_AddPolarOffsetFar
	ldr r3, [sp, #4]
	mov r2, r10
	ldr r1, [r2]
	ldrb r0, [r3]
	ldr r2, [r2, #8]
	bl Func_02004c10
	mov r11, r0
	cmp r0, #255
	beq .L_0200b6f2
	ldr r3, [sp, #4]
	mov r2, r10
	ldrb r0, [r3]
	ldr r1, [r2]
	ldr r2, [r2, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt .L_0200b6f2
	ldr r0, [sp, #12]
	mov r2, r10
	str r0, [r2]
	ldr r3, [sp, #8]
	adds r0, r7, #0
	str r3, [r2, #8]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	adds r2, r7, #0
	str r3, [r7, #52]
	adds r2, #100
	movs r3, #0
	strh r3, [r2]
	ldr r1, [sp, #12]
	ldr r2, [r7, #12]
	ldr r3, [sp, #8]
	bl Func_02004bf8
	adds r0, r7, #0
	movs r1, #2
	bl Func_02004bc0
	adds r0, r7, #0
	movs r1, #48
	bl ObjectDispatch_ApplyValueToChildren
	movs r5, #0
	b .L_0200b71a
.L_0200b6f2:
	movs r3, #0
	mov r0, r9
	strh r0, [r7, #6]
	str r3, [r7, #36]
	str r3, [r7, #44]
	ldr r2, [sp, #12]
	str r2, [r7, #8]
	ldr r3, [sp, #8]
	str r3, [r7, #16]
	b .L_0200b842
.L_0200b706:
	ldr r3, .L_0200b870
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	beq .L_0200b712
	b .L_0200b842
.L_0200b712:
	movs r0, #1
	bl WaitFrames
	adds r5, #1
.L_0200b71a:
	cmp r5, #179
	bgt .L_0200b728
	adds r0, r7, #0
	bl Func_02004c70
	cmp r0, #0
	beq .L_0200b706
.L_0200b728:
	ldr r3, .L_0200b874
	str r3, [r7, #108]
	b .L_0200b7f6
.L_0200b72e:
	ldr r2, [sp, #4]
	ldr r1, [r6]
	ldrb r0, [r2]
	ldr r2, [r6, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r7, #12]
	subs r0, r0, r3
	movs r3, #128
	lsls r3, r3, #12
	cmp r0, r3
	bgt .L_0200b816
	ldr r3, .L_0200b870
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_0200b842
	ldrh r3, [r7, #32]
	movs r2, #89
	subs r3, #2
	str r3, [sp, #0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #20]
	movs r0, #0
	adds r2, r2, r5
	mov r10, r0
	mov r8, r2
.L_0200b766:
	ldr r3, [r5]
	cmp r3, #0
	beq .L_0200b790
	mov r3, r8
	ldrb r2, [r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0200b790
	cmp r5, r7
	beq .L_0200b790
	ldrh r3, [r5, #32]
	adds r0, r5, #0
	adds r0, #8
	subs r3, #2
	ldr r1, [sp, #0]
	add r2, sp, #28
	bl Func_02004c78
	cmp r0, #0
	bge .L_0200b816
.L_0200b790:
	movs r0, #1
	add r10, r0
	movs r2, #128
	mov r3, r10
	add r8, r2
	adds r5, #128
	cmp r3, #63
	ble .L_0200b766
	ldr r0, [r6]
	movs r3, #128
	str r0, [sp, #20]
	lsls r3, r3, #10
	ldr r2, [r6, #8]
	adds r0, r7, #0
	str r2, [sp, #16]
	str r3, [r7, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	str r3, [r7, #52]
	movs r5, #0
	ldr r1, [r6]
	ldr r2, [r6, #4]
	ldr r3, [r6, #8]
	bl Func_02004bf8
	b .L_0200b7ce
.L_0200b7c6:
	movs r0, #1
	bl WaitFrames
	adds r5, #1
.L_0200b7ce:
	cmp r5, #179
	bgt .L_0200b7e6
	adds r0, r7, #0
	bl Func_02004c70
	cmp r0, #0
	bne .L_0200b7e6
	ldr r3, .L_0200b870
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	beq .L_0200b7c6
.L_0200b7e6:
	ldr r3, .L_0200b870
	ldrh r3, [r3]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_0200b842
	ldr r3, [sp, #24]
	cmp r11, r3
	bne .L_0200b83c
.L_0200b7f6:
	movs r0, #128
	add r2, sp, #28
	lsls r0, r0, #13
	mov r1, r9
	bl Vector_AddPolarOffsetFar
	ldr r2, [sp, #4]
	add r6, sp, #28
	ldrb r0, [r2]
	ldr r1, [r6]
	ldr r2, [r6, #8]
	bl Func_02004c10
	mov r11, r0
	cmp r0, #255
	bne .L_0200b72e
.L_0200b816:
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r7, #48]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r7, #52]
	ldr r2, [r7, #12]
	adds r0, r7, #0
	ldr r1, [sp, #20]
	ldr r3, [sp, #16]
	bl Func_02004bf8
	adds r0, r7, #0
	bl Func_02004c00
	movs r0, #2
	bl WaitFrames
	b .L_0200b58a
.L_0200b83c:
	movs r0, #10
	bl WaitFrames
.L_0200b842:
	movs r3, #0
	str r3, [r7, #108]
	adds r1, r7, #0
	adds r1, #90
	ldrb r2, [r1]
	movs r3, #1
	orrs r3, r2
	strb r3, [r1]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r7, #52]
	adds r0, r7, #0
	movs r1, #1
	bl Func_02004bc0
	add sp, #40
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200b870:
	.4byte Data_020068b0
.L_0200b874:
	.4byte Func_020031f8
	.endif
	.section .text.x0200b884,"ax",%progbits
	.balign 4
	.global Func_02003884
	.thumb_func
Func_02003884:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_0200b8f0
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	sub sp, #32
	bl ObjectTable_Get
	adds r5, r0, #0
	ldrh r3, [r5, #6]
	movs r2, #128
	lsls r2, r2, #6
	adds r3, r3, r2
	ldr r2, .L_0200b8ec
	ands r3, r2
	lsls r3, r3, #16
	asrs r3, r3, #16
	str r3, [sp, #16]
.L_0200b8b6:
	bl Func_02003284
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #48]
	movs r3, #128
	lsls r3, r3, #8
	str r3, [r5, #52]
	movs r3, #128
	lsls r3, r3, #24
	str r3, [r5, #56]
	str r3, [r5, #64]
	movs r3, #0
	str r3, [r5, #36]
	str r3, [r5, #44]
	ldr r2, .L_0200b8f4
	ldr r3, [r5, #8]
	movs r1, #128
	lsls r1, r1, #12
	ands r3, r2
	mov r9, r1
	add r6, sp, #20
	add r3, r9
	str r3, [r6]
	mov r8, r3
	b .L_0200b8f8
	.2byte 0x0000
.L_0200b8ec:
	.4byte 0xffffc000
.L_0200b8f0:
	.4byte gPartyState
.L_0200b8f4:
	.4byte 0xfff00000
.L_0200b8f8:
	ldr r3, [r5, #12]
	str r3, [r6, #4]
	ldr r3, [r5, #16]
	ands r3, r2
	adds r7, r3, r1
	mov r2, r8
	str r7, [r6, #8]
	str r2, [sp, #8]
	str r7, [sp, #4]
	movs r3, #34
	adds r3, r3, r5
	ldrb r0, [r3]
	adds r1, r2, #0
	adds r2, r7, #0
	mov r11, r3
	bl Func_02004c10
	str r0, [sp, #12]
	movs r0, #128
	ldr r1, [sp, #16]
	lsls r0, r0, #13
	adds r2, r6, #0
	bl Vector_AddPolarOffsetFar
	mov r1, r11
	ldrb r0, [r1]
	ldr r2, [r6, #8]
	ldr r1, [r6]
	bl Func_02004c10
	mov r10, r0
	cmp r0, #255
	beq .L_0200b98c
	mov r2, r11
	ldrb r0, [r2]
	ldr r1, [r6]
	ldr r2, [r6, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r5, #12]
	subs r0, r0, r3
	cmp r0, r9
	bgt .L_0200b98c
	ldr r3, [sp, #8]
	ldr r2, .L_0200b984
	str r3, [r6]
	ldr r1, [sp, #4]
	movs r3, #128
	lsls r3, r3, #10
	str r1, [r6, #8]
	str r3, [r5, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	str r3, [r5, #52]
	adds r3, r5, #0
	adds r3, #100
	strh r2, [r3]
	adds r0, r5, #0
	movs r1, #2
	bl Func_02004bc0
	adds r0, r5, #0
	movs r1, #48
	bl ObjectDispatch_ApplyValueToChildren
	ldr r3, .L_0200b988
	str r3, [r5, #108]
	b .L_0200ba36
	.2byte 0x0000
.L_0200b984:
	.4byte 0x00000000
.L_0200b988:
	.4byte Func_020031f8
.L_0200b98c:
	add r1, sp, #16
	ldrh r1, [r1]
	movs r3, #0
	mov r2, r8
	strh r1, [r5, #6]
	str r3, [r5, #36]
	str r3, [r5, #44]
	str r2, [r5, #8]
	str r7, [r5, #16]
	b .L_0200ba82
.L_0200b9a0:
	mov r3, r11
	ldrb r0, [r3]
	ldr r1, [r7]
	ldr r2, [r7, #8]
	bl Map_GetTerrainHeight
	ldr r3, [r5, #12]
	movs r1, #128
	subs r0, r0, r3
	lsls r1, r1, #12
	cmp r0, r1
	bgt .L_0200ba56
	ldrh r3, [r5, #32]
	movs r2, #0
	subs r3, #2
	str r3, [sp, #0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #20]
	movs r3, #89
	adds r3, r3, r6
	mov r9, r2
	mov r8, r3
.L_0200b9ce:
	ldr r3, [r6]
	cmp r3, #0
	beq .L_0200b9f8
	mov r1, r8
	ldrb r2, [r1]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_0200b9f8
	cmp r6, r5
	beq .L_0200b9f8
	ldrh r3, [r6, #32]
	adds r0, r6, #0
	adds r0, #8
	subs r3, #2
	ldr r1, [sp, #0]
	add r2, sp, #20
	bl Func_02004c78
	cmp r0, #0
	bge .L_0200ba56
.L_0200b9f8:
	movs r2, #1
	add r9, r2
	movs r3, #128
	mov r1, r9
	add r8, r3
	adds r6, #128
	cmp r1, #63
	ble .L_0200b9ce
	ldr r2, [r7]
	adds r0, r5, #0
	str r2, [sp, #8]
	ldr r3, [r7, #8]
	str r3, [sp, #4]
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #48]
	movs r3, #200
	lsls r3, r3, #5
	adds r3, #153
	str r3, [r5, #52]
	ldr r1, [r7]
	ldr r2, [r7, #4]
	ldr r3, [r7, #8]
	bl Func_02004bf8
	adds r0, r5, #0
	bl Func_02004c00
	ldr r1, [sp, #12]
	cmp r10, r1
	bne .L_0200ba7c
.L_0200ba36:
	movs r0, #128
	ldr r1, [sp, #16]
	add r2, sp, #20
	lsls r0, r0, #13
	bl Vector_AddPolarOffsetFar
	mov r2, r11
	add r7, sp, #20
	ldrb r0, [r2]
	ldr r1, [r7]
	ldr r2, [r7, #8]
	bl Func_02004c10
	mov r10, r0
	cmp r0, #255
	bne .L_0200b9a0
.L_0200ba56:
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r5, #48]
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r5, #52]
	ldr r2, [r5, #12]
	adds r0, r5, #0
	ldr r1, [sp, #8]
	ldr r3, [sp, #4]
	bl Func_02004bf8
	adds r0, r5, #0
	bl Func_02004c00
	movs r0, #2
	bl WaitFrames
	b .L_0200b8b6
.L_0200ba7c:
	movs r0, #10
	bl WaitFrames
.L_0200ba82:
	movs r3, #0
	str r3, [r5, #108]
	adds r1, r5, #0
	adds r1, #90
	ldrb r2, [r1]
	movs r3, #1
	orrs r3, r2
	strb r3, [r1]
	movs r3, #128
	lsls r3, r3, #7
	str r3, [r5, #52]
	adds r0, r5, #0
	movs r1, #1
	bl Func_02004bc0
	add sp, #32
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200baf6,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200bb2e,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200bb30,"ax",%progbits
	.global Func_02003b30
	.thumb_func
Func_02003b30:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r11, r3
	ldr r3, .L_0200bce8
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
	beq .L_0200bb78
	cmp r7, #0
	beq .L_0200bb78
	movs r2, #24
	ldrsh r0, [r7, r2]
	adds r1, r5, #0
	adds r2, r6, #0
	b .L_0200bb80
.L_0200bb78:
	movs r0, #30
	adds r2, r6, #0
	adds r0, #255
	adds r1, r5, #0
.L_0200bb80:
	mov r3, r10
	bl Func_02004bd8
	adds r6, r0, #0
	cmp r6, #0
	bne .L_0200bb8e
	b .L_0200bcda
.L_0200bb8e:
	ldr r3, [r6, #80]
	mov r1, r8
	movs r5, #15
	adds r1, #1
	ands r1, r5
	adds r0, r6, #0
	str r3, [sp, #0]
	bl Func_02004bc0
	ldr r2, .L_0200bcec
	mov r3, r8
	ands r3, r5
	lsls r3, r3, #2
	ldr r1, [r2, r3]
	adds r0, r6, #0
	mov r10, r3
	bl Func_02004bd0
	adds r3, r6, #0
	movs r5, #0
	adds r3, #85
	strb r5, [r3]
	adds r0, r6, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildFlags
	ldr r3, .L_0200bcf0
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
	bl Object_SetSpritePriorityB
	movs r2, #100
	adds r2, r2, r6
	mov r9, r2
	mov r3, r9
	str r5, [r6, #48]
	str r5, [r6, #52]
	strh r5, [r3]
	ldr r3, .L_0200bcf4
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_0200bcda
	cmp r7, #0
	beq .L_0200bcda
	movs r3, #128
	lsls r3, r3, #9
	ands r3, r1
	cmp r3, #0
	beq .L_0200bc10
	ldr r1, [r7, #4]
	adds r0, r6, #0
	bl Object_SetPartAttribute
.L_0200bc10:
	movs r3, #128
	lsls r3, r3, #10
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_0200bc30
	adds r1, r6, #0
	adds r1, #35
	ldrb r3, [r1]
	movs r2, #254
	ands r2, r3
	strb r2, [r1]
	ldr r1, [r7]
	adds r0, r6, #0
	bl Object_SetSpritePriorityB
.L_0200bc30:
	movs r2, #128
	lsls r2, r2, #12
	mov r3, r8
	ands r2, r3
	cmp r2, #0
	beq .L_0200bc44
	ldr r3, [r7, #8]
	str r3, [r6, #24]
	ldr r3, [r7, #12]
	str r3, [r6, #28]
.L_0200bc44:
	movs r3, #128
	lsls r3, r3, #11
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_0200bc8a
	ldr r3, .L_0200bcec
	mov r1, r10
	ldr r5, [r3, r1]
	ldr r3, [r7, #16]
	ldr r1, [r5, #12]
	cmp r2, #0
	beq .L_0200bc72
	ldr r0, [r6, #24]
	subs r0, r3, r0
	bl __divsi3
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, [r6, #28]
	ldr r1, [r5, #12]
	subs r0, r0, r3
	b .L_0200bc84
.L_0200bc72:
	ldr r2, .L_0200bcf4
	adds r0, r3, r2
	bl __divsi3
	str r0, [r6, #48]
	ldr r0, [r7, #20]
	ldr r3, .L_0200bcf4
	ldr r1, [r5, #12]
	adds r0, r0, r3
.L_0200bc84:
	bl __divsi3
	str r0, [r6, #52]
.L_0200bc8a:
	movs r3, #128
	lsls r3, r3, #14
	mov r1, r8
	ands r3, r1
	cmp r3, #0
	beq .L_0200bca6
	adds r0, r6, #0
	movs r1, #1
	bl Func_02004bc0
	ldr r1, [r7, #28]
	adds r0, r6, #0
	bl Func_02004bd0
.L_0200bca6:
	movs r3, #128
	lsls r3, r3, #15
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_0200bcb8
	ldrh r3, [r7, #32]
	ldr r1, [sp, #0]
	strh r3, [r1, #18]
.L_0200bcb8:
	movs r3, #128
	lsls r3, r3, #16
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_0200bcca
	ldrh r3, [r7, #34]
	mov r1, r9
	strh r3, [r1]
.L_0200bcca:
	movs r3, #128
	lsls r3, r3, #17
	mov r2, r8
	ands r3, r2
	cmp r3, #0
	beq .L_0200bcda
	ldr r3, [r7, #36]
	str r3, [r6, #108]
.L_0200bcda:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200bce8:
	.4byte gPartyState
.L_0200bcec:
	.4byte Data_0200686c
.L_0200bcf0:
	.4byte BattleFx_UpdateObjectMotionScaleAndLinkedAngle
.L_0200bcf4:
	.4byte 0xffff0000
	.section .text.x0200bcf8,"ax",%progbits
	.global Func_02003cf8
	.thumb_func
Func_02003cf8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r4, .L_0200be10
	movs r1, #1
	movs r0, #12
	ldrsh r3, [r4, r0]
	negs r1, r1
	sub sp, #4
	cmp r3, r1
	beq .L_0200be04
	lsls r3, r3, #3
	adds r3, r3, r4
	adds r3, #32
	mov r8, r3
	ldr r3, .L_0200be14
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
	bne .L_0200bd44
	ldr r3, [r0, #16]
	movs r5, #2
	ldrsh r2, [r1, r5]
	asrs r3, r3, #20
	cmp r3, r2
	beq .L_0200bd4c
.L_0200bd44:
	movs r3, #255
	lsls r3, r3, #8
	adds r3, #255
	strh r3, [r4, #12]
.L_0200bd4c:
	movs r0, #12
	ldrsh r3, [r4, r0]
	movs r2, #1
	negs r2, r2
	ldr r1, .L_0200be18
	cmp r3, r2
	beq .L_0200be04
	movs r5, #14
	ldrsh r3, [r4, r5]
	cmp r3, #0
	beq .L_0200be04
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
	ldr r2, .L_0200be1c
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
	bhi .L_0200be04
	movs r0, #15
	negs r0, r0
	cmp r2, r0
	blt .L_0200be04
	cmp r2, #239
	bgt .L_0200be04
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
	ldr r3, .L_0200be20
	adds r0, r4, #0
	orrs r2, r3
	movs r3, #128
	str r2, [r4, #24]
	lsls r3, r3, #3
	mov r2, r10
	orrs r2, r3
	str r2, [r4, #28]
	adds r0, #20
	bl Func_02004b80
.L_0200be04:
	add sp, #4
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200be10:
	.4byte Data_020068b4
.L_0200be14:
	.4byte gPartyState
.L_0200be18:
	.4byte 0xffff0000
.L_0200be1c:
	.4byte ResourceTableEntries
.L_0200be20:
	.4byte 0x80008800
	.section .text.x0200be24,"ax",%progbits
	.global Func_02003e24
	.thumb_func
Func_02003e24:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #48
	str r0, [sp, #44]
	ldr r0, .L_0200c054
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
	ldr r3, .L_0200c058
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
	bge .L_0200bfa8
.L_0200bed8:
	ldr r1, [sp, #12]
	ldr r2, [sp, #36]
	ldr r5, [sp, #24]
	lsls r3, r1, #9
	adds r2, r2, r3
	movs r3, #0
	mov r11, r2
	str r3, [sp, #16]
	cmp r3, r5
	bge .L_0200bf9c
.L_0200beec:
	mov r0, r11
	ldrb r5, [r0, #2]
	cmp r5, #0
	beq .L_0200bf8c
	ldr r1, [sp, #44]
	cmp r5, r1
	bcc .L_0200bf8c
	adds r1, #1
	mov r10, r1
	cmp r5, r10
	bhi .L_0200bf8c
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
	bne .L_0200bf40
	cmp r5, r10
	bne .L_0200bf7e
	mov r3, r9
	movs r2, #4
	ldrsh r0, [r3, r2]
	bl GameFlag_SetBit
	b .L_0200bf7e
.L_0200bf40:
	mov r1, r9
	movs r5, #4
	ldrsh r0, [r1, r5]
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200bf7e
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
	bl Func_02004c20
.L_0200bf7e:
	mov r0, r8
	ldrh r3, [r0, #10]
	mov r1, r8
	adds r3, #1
	strh r3, [r1, #10]
	movs r5, #8
	add r9, r5
.L_0200bf8c:
	ldr r2, [sp, #16]
	ldr r5, [sp, #24]
	adds r2, #1
	movs r3, #4
	str r2, [sp, #16]
	add r11, r3
	cmp r2, r5
	blt .L_0200beec
.L_0200bf9c:
	ldr r0, [sp, #12]
	ldr r1, [sp, #20]
	adds r0, #1
	str r0, [sp, #12]
	cmp r0, r1
	blt .L_0200bed8
.L_0200bfa8:
	movs r0, #10
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0200c000
	ldr r3, .L_0200c05c
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
	bge .L_0200c000
.L_0200bfda:
	mov r0, r9
	movs r5, #0
	ldrsh r3, [r0, r5]
	cmp r3, r4
	bne .L_0200bff0
	movs r5, #2
	ldrsh r3, [r0, r5]
	cmp r3, r1
	bne .L_0200bff0
	mov r0, r8
	strh r2, [r0, #12]
.L_0200bff0:
	movs r3, #8
	mov r0, r8
	add r9, r3
	movs r5, #10
	ldrsh r3, [r0, r5]
	adds r2, #1
	cmp r2, r3
	blt .L_0200bfda
.L_0200c000:
	movs r0, #128
	lsls r0, r0, #1
	bl MemAlloc
	adds r5, r0, #0
	adds r1, r5, #0
	movs r2, #63
.L_0200c00e:
	ldr r3, .L_0200c060
	subs r2, #1
	stmia r1!, {r3}
	cmp r2, #0
	bge .L_0200c00e
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
	ldr r0, .L_0200c064
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
.L_0200c054:
	.4byte Data_020068b4
.L_0200c058:
	.4byte IwramClearWords
.L_0200c05c:
	.4byte gPartyState
.L_0200c060:
	.4byte 0x11111111
.L_0200c064:
	.4byte Func_02003cf8
	.section .text.x0200c0f0,"ax",%progbits
	.global Func_020040f0
	.thumb_func
Func_020040f0:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	ldr r3, .L_0200c15c
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
	bl Func_02004068
	movs r0, #161
	bl Func_02004de0
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
	bl Func_02004c20
	movs r0, #12
	bl Battle_WaitMode0
	add sp, #8
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_0200c15c:
	.4byte gPartyState
	.section .text.x0200c160,"ax",%progbits
	.global Func_02004160
	.thumb_func
Func_02004160:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r1, .L_0200c210
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
	bl Func_02004068
	movs r0, #229
	bl Func_02004de0
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
	bl Func_02004c20
	movs r0, #12
	bl Battle_WaitMode0
	movs r3, #128
	ldr r2, .L_0200c208
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
	ldr r2, .L_0200c20c
	movs r7, #0
	orrs r3, r2
	strh r3, [r1]
	mov r2, r10
	b .L_0200c214
	.2byte 0x0000
.L_0200c208:
	.4byte 0x00000000
.L_0200c20c:
	.4byte 0x00008000
.L_0200c210:
	.4byte gPartyState
.L_0200c214:
	movs r3, #1
	mov r1, r8
	strh r3, [r1, #14]
	ldr r0, [r2]
	movs r1, #28
	bl Object_SetModeById
	movs r0, #16
	bl Battle_WaitMode0
.L_0200c228:
	cmp r7, #5
	bne .L_0200c232
	movs r0, #204
	bl Func_02004de0
.L_0200c232:
	ldr r3, [r6, #24]
	ldr r1, .L_0200c290
	ldr r2, .L_0200c294
	adds r3, r3, r1
	str r3, [r6, #24]
	ldr r3, [r6, #28]
	ldr r1, .L_0200c298
	adds r3, r3, r2
	str r3, [r6, #28]
	ldr r3, [r6, #12]
	movs r0, #1
	adds r3, r3, r1
	str r3, [r6, #12]
	adds r7, #1
	bl WaitFrames
	cmp r7, #39
	ble .L_0200c228
	ldr r3, .L_0200c29c
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
.L_0200c290:
	.4byte 0xfffffc00
.L_0200c294:
	.4byte 0xfffffd00
.L_0200c298:
	.4byte 0xffff6667
.L_0200c29c:
	.4byte gPartyState
	.section .text.x0200c2f6,"ax",%progbits
	.2byte 0x0000
	.section .text.x0200c2f8,"ax",%progbits
	.global Func_020042f8
	.thumb_func
Func_020042f8:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200c47c
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #68
	bl Object_GetById
	adds r7, r0, #0
	bl Func_02004ca8
	movs r0, #0
	bl Func_02004da0
	movs r0, #1
	movs r1, #1
	movs r2, #1
	negs r1, r1
	negs r2, r2
	movs r3, #0
	negs r0, r0
	bl Motion_CamBounds
	bl Func_02004bf0
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
	bl Func_02004de0
	movs r3, #3
	strb r3, [r5]
	movs r0, #24
	bl Battle_WaitMode0
	add r2, sp, #28
	movs r3, #7
	str r3, [r2, #4]
	ldr r3, .L_0200c480
	mov r8, r2
	str r3, [r2, #36]
	movs r3, #163
	lsls r3, r3, #8
	adds r3, #215
	str r3, [r2, #8]
	str r3, [r2, #12]
	movs r3, #0
	mov r10, r3
.L_0200c392:
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
	ldr r3, .L_0200c484
	adds r2, r2, r3
	str r2, [r6]
	bl Random16Far
	lsls r3, r0, #1
	ldr r5, [r6, #8]
	adds r3, r3, r0
	ldr r4, .L_0200c488
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
	ldr r4, .L_0200c48c
	str r5, [sp, #4]
	str r4, [sp, #8]
	mov r4, r8
	str r4, [sp, #12]
	bl Func_02003b30
	movs r2, #1
	add r10, r2
	mov r3, r10
	cmp r3, #16
	bls .L_0200c392
	movs r0, #188
	bl Func_02004de0
	ldr r5, .L_0200c47c
	movs r4, #133
	lsls r4, r4, #2
	adds r5, r5, r4
	movs r1, #2
	ldr r0, [r5]
	adds r1, #255
	bl Func_02004d08
	ldr r0, [r5]
	movs r1, #49
	bl Object_SetModeById
	movs r0, #160
	movs r1, #160
	movs r2, #128
	lsls r0, r0, #11
	lsls r1, r1, #11
	lsls r2, r2, #9
	bl Func_02004c50
	movs r2, #230
	movs r0, #1
	movs r1, #1
	lsls r2, r2, #8
	adds r2, #102
	negs r0, r0
	negs r1, r1
	bl Func_02004c50
	bl Func_02004c58
	movs r1, #128
	ldr r0, [r5]
	lsls r1, r1, #1
	bl Func_02004d08
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
	bl Func_02004cb0
	add sp, #68
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
.L_0200c47c:
	.4byte gPartyState
.L_0200c480:
	.4byte Effect_AdvanceMotion
.L_0200c484:
	.4byte 0xffffa000
.L_0200c488:
	.4byte 0xffffd000
.L_0200c48c:
	.4byte 0x01090001
	.section .text.x0200c490,"ax",%progbits
	.global Func_02004490
	.thumb_func
Func_02004490:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_0200c538
	movs r1, #133
	lsls r1, r1, #2
	adds r3, r3, r1
	ldr r0, [r3]
	bl Object_GetById
	ldr r3, [r0, #8]
	ldr r6, .L_0200c53c
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
	bge .L_0200c52c
.L_0200c4c4:
	movs r1, #0
	ldrsh r3, [r5, r1]
	cmp r3, r8
	bne .L_0200c520
	movs r1, #2
	ldrsh r3, [r5, r1]
	cmp r3, r10
	bne .L_0200c520
	movs r2, #4
	ldrsh r0, [r5, r2]
	bl GameFlag_Test
	cmp r0, #0
	bne .L_0200c4f4
	adds r0, r6, #0
	adds r1, r5, #0
	bl Func_020040f0
	movs r3, #4
	ldrsh r0, [r5, r3]
	bl GameFlag_SetBit
	strh r7, [r6, #12]
	b .L_0200c52c
.L_0200c4f4:
	movs r1, #12
	ldrsh r3, [r6, r1]
	cmp r7, r3
	beq .L_0200c52c
	adds r0, r6, #0
	adds r1, r5, #0
	strh r7, [r6, #12]
	bl Func_02004160
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
	b .L_0200c52e
.L_0200c520:
	lsls r3, r2, #16
	adds r7, #1
	asrs r3, r3, #16
	adds r5, #8
	cmp r7, r3
	blt .L_0200c4c4
.L_0200c52c:
	movs r0, #0
.L_0200c52e:
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0200c538:
	.4byte gPartyState
.L_0200c53c:
	.4byte Data_020068b4
	.section .text.x0200c540,"ax",%progbits
	.global Func_02004540
	.thumb_func
Func_02004540:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #4
	ldr r3, .L_0200c5f0
	str r2, [sp, #0]
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	mov r9, r0
	ldr r0, [r3]
	mov r11, r1
	bl Object_GetById
	movs r3, #192
	ldr r5, .L_0200c5f4
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
	bne .L_0200c58e
	cmp r0, #0
	beq .L_0200c5e2
.L_0200c58e:
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
	bl Func_02004bf0
	bl Func_020042f8
	movs r2, #192
	lsls r2, r2, #18
	ldr r3, [r2, #108]
	movs r2, #214
	lsls r2, r2, #1
	adds r3, r3, r2
	ldr r2, [sp, #0]
	str r2, [r3]
.L_0200c5e2:
	add sp, #4
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0200c5f0:
	.4byte gPartyState
.L_0200c5f4:
	.4byte Data_020068b4
	.section .text.x0200c5f8,"ax",%progbits
	.global Func_020045f8
	.thumb_func
Func_020045f8:
	push {r5, r6, lr}
	adds r4, r1, #0
	movs r1, #192
	lsls r1, r1, #18
	adds r1, #128
	ldr r6, [r1]
	ldr r1, .L_0200c69c
	adds r5, r0, #0
	str r5, [r1]
	ldr r1, .L_0200c6a0
	str r4, [r1]
	ldr r1, .L_0200c6a4
	str r2, [r1]
	ldr r2, .L_0200c6a8
	str r3, [r2]
	movs r2, #255
	ldrh r3, [r5]
	b .L_0200c642
.L_0200c61c:
	ldrh r0, [r4]
	adds r4, #2
	ldrh r2, [r4]
	adds r4, #2
	ldrh r1, [r5]
	ldrh r3, [r4]
	adds r5, #2
	adds r4, #2
	lsls r3, r3, #10
	lsls r2, r2, #5
	orrs r3, r2
	movs r2, #160
	lsls r2, r2, #19
	lsls r1, r1, #1
	orrs r3, r0
	adds r1, r1, r2
	strh r3, [r1]
	ldrh r3, [r5]
	movs r2, #255
.L_0200c642:
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	beq .L_0200c650
	ldrh r3, [r4]
	cmp r3, r2
	bne .L_0200c61c
.L_0200c650:
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	movs r0, #160
	lsls r2, r2, #24
	adds r3, #212
	lsls r0, r0, #19
	adds r1, r6, #0
	adds r2, #112
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #224
	lsls r2, r2, #1
	adds r1, r6, r2
	movs r2, #132
	lsls r2, r2, #24
	ldr r0, .L_0200c6ac
	adds r2, #112
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #128
	lsls r0, r0, #9
	movs r1, #0
	bl Func_02004d58
	ldr r3, .L_0200c6b0
	movs r2, #128
	lsls r2, r2, #2
	adds r2, #194
	adds r3, r3, r2
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #2
	bne .L_0200c69a
	bl Func_020048e8
.L_0200c69a:
	pop {r5, r6, pc}
.L_0200c69c:
	.4byte Data_0200e88c
.L_0200c6a0:
	.4byte Data_02006890
.L_0200c6a4:
	.4byte Data_02006894
.L_0200c6a8:
	.4byte Data_02006880
.L_0200c6ac:
	.4byte 0x05000200
.L_0200c6b0:
	.4byte gPartyState
	.section .text.x0200c794,"ax",%progbits
	.balign 4
	.global Func_02004794
	.thumb_func
Func_02004794:
	push {lr}
	ldr r1, .L_0200c7ac
	ldrh r3, [r0]
	movs r2, #0
	cmp r3, r1
	beq .L_0200c7b0
.L_0200c7a0:
	adds r0, #2
	ldrh r3, [r0]
	adds r2, #1
	cmp r3, r1
	bne .L_0200c7a0
	b .L_0200c7b0
.L_0200c7ac:
	.4byte 0x0000ffff
.L_0200c7b0:
	subs r2, #1
	adds r0, r2, #0
	pop {pc}
	.2byte 0x0000
	.section .text.x0200c7b8,"ax",%progbits
	.global Func_020047b8
	.thumb_func
Func_020047b8:
	push {r5, r6, r7, lr}
	ldr r1, .L_0200c868
	movs r4, #0
	ldrh r2, [r1]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_0200c7f2
	movs r0, #255
	lsls r0, r0, #8
	adds r0, #255
	adds r3, r2, r0
	strh r3, [r1]
	lsls r3, r3, #16
	cmp r3, #0
	bne .L_0200c7f2
	ldr r0, .L_0200c86c
	movs r4, #1
	ldrh r2, [r0]
	strh r2, [r1]
	movs r1, #128
	lsls r3, r2, #16
	lsls r1, r1, #10
	cmp r3, r1
	bls .L_0200c7f2
	movs r1, #255
	lsls r1, r1, #8
	adds r1, #255
	adds r3, r2, r1
	strh r3, [r0]
.L_0200c7f2:
	cmp r4, #0
	bne .L_0200c7f8
	b .L_0200c8e4
.L_0200c7f8:
	ldr r3, .L_0200c870
	ldr r6, .L_0200c874
	ldr r1, [r3]
	ldrh r3, [r6]
	movs r5, #0
	cmp r5, r3
	bcs .L_0200c846
	ldr r3, .L_0200c878
	ldr r2, .L_0200c87c
	ldr r7, [r3]
	mov lr, r2
	mov r12, r6
.L_0200c810:
	mov r3, lr
	ldrh r2, [r3]
	ldrh r3, [r6]
	movs r0, #160
	muls r3, r2
	adds r3, r3, r5
	lsls r3, r3, #1
	ldrh r3, [r3, r7]
	lsls r0, r0, #19
	lsls r3, r3, #1
	adds r4, r3, r0
	ldrh r0, [r1]
	adds r1, #2
	ldrh r2, [r1]
	adds r1, #2
	ldrh r3, [r1]
	adds r1, #2
	lsls r3, r3, #10
	lsls r2, r2, #5
	orrs r3, r2
	orrs r3, r0
	strh r3, [r4]
	adds r5, #1
	mov r2, r12
	ldrh r3, [r2]
	cmp r5, r3
	bcc .L_0200c810
.L_0200c846:
	ldr r3, .L_0200c874
	movs r0, #160
	ldrh r1, [r3]
	ldr r3, .L_0200c880
	lsls r2, r1, #1
	ldr r3, [r3]
	lsls r0, r0, #19
	ldrh r3, [r2, r3]
	adds r2, r2, r1
	lsls r3, r3, #1
	adds r4, r3, r0
	ldr r3, .L_0200c884
	ldrh r3, [r3]
	cmp r3, #0
	beq .L_0200c88c
	ldr r3, .L_0200c888
	b .L_0200c88e
.L_0200c868:
	.4byte Data_02006884
.L_0200c86c:
	.4byte Data_0200e888
.L_0200c870:
	.4byte Data_02006894
.L_0200c874:
	.4byte Data_02006898
.L_0200c878:
	.4byte Data_02006880
.L_0200c87c:
	.4byte Data_0200689c
.L_0200c880:
	.4byte Data_0200e88c
.L_0200c884:
	.4byte Data_0200687c
.L_0200c888:
	.4byte Data_02006890
.L_0200c88c:
	ldr r3, .L_0200c8d4
.L_0200c88e:
	lsls r2, r2, #1
	ldr r3, [r3]
	adds r1, r3, r2
	ldrh r0, [r1]
	adds r1, #2
	ldrh r2, [r1]
	ldrh r3, [r1, #2]
	lsls r3, r3, #10
	lsls r2, r2, #5
	orrs r3, r2
	orrs r3, r0
	strh r3, [r4]
	ldr r1, .L_0200c8d8
	ldr r2, .L_0200c8cc
	ldrh r3, [r1]
	eors r3, r2
	strh r3, [r1]
	ldr r1, .L_0200c8dc
	ldr r2, .L_0200c8e0
	ldrh r3, [r1]
	adds r3, #1
	strh r3, [r1]
	lsls r3, r3, #16
	ldrh r2, [r2]
	lsrs r3, r3, #16
	cmp r3, r2
	bne .L_0200c8e4
	ldr r3, .L_0200c8d0
	strh r3, [r1]
	b .L_0200c8e4
	.2byte 0x0000
.L_0200c8cc:
	.4byte 0x00000001
.L_0200c8d0:
	.4byte 0x00000000
.L_0200c8d4:
	.4byte Data_02006894
.L_0200c8d8:
	.4byte Data_0200687c
.L_0200c8dc:
	.4byte Data_0200689c
.L_0200c8e0:
	.4byte Data_02006898
.L_0200c8e4:
	pop {r5, r6, r7, pc}
	.2byte 0x0000
	.section .text.x0200c970,"ax",%progbits
	.global Func_02004970
	.thumb_func
Func_02004970:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r2, #255
	ldrh r3, [r0]
	lsls r2, r2, #8
	adds r2, #255
	sub sp, #4
	cmp r3, r2
	beq .L_0200c9d4
	adds r7, r0, #0
.L_0200c986:
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
	bl Func_02004a60
	movs r2, #255
	ldrh r3, [r7]
	lsls r2, r2, #8
	adds r2, #255
	cmp r3, r2
	bne .L_0200c986
.L_0200c9d4:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
	.section .text.x0200c9dc,"ax",%progbits
	.global Func_020049dc
	.thumb_func
Func_020049dc:
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
	b .L_0200ca48
.L_0200c9f8:
	ldrh r3, [r5]
	movs r1, #26
	ldrsh r7, [r2, r1]
	cmp r7, r3
	bne .L_0200ca44
	adds r0, r7, #0
	bl Object_GetById
	adds r5, #2
	ldrh r2, [r5]
	mov r3, r10
	adds r6, r0, #0
	mov r8, r2
	ldrh r5, [r5, #2]
	cmp r3, #7
	bgt .L_0200ca20
	ldr r3, [r6, #28]
	ldr r1, .L_0200ca5c
	adds r3, r3, r1
	str r3, [r6, #28]
.L_0200ca20:
	mov r2, r9
	cmp r2, #1
	bne .L_0200ca52
	adds r0, r5, #0
	bl GameFlag_SetBit
	adds r3, r6, #0
	adds r3, #34
	ldrb r3, [r3]
	adds r0, r7, #0
	mov r1, r8
	adds r2, r5, #0
	bl Func_02004a60
	movs r3, #128
	lsls r3, r3, #9
	str r3, [r6, #28]
	b .L_0200ca52
.L_0200ca44:
	adds r5, #6
	movs r1, #255
.L_0200ca48:
	ldrh r3, [r5]
	lsls r1, r1, #8
	adds r1, #255
	cmp r3, r1
	bne .L_0200c9f8
.L_0200ca52:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
.L_0200ca5c:
	.4byte 0xffffe100
	.section .bss,"aw",%nobits
	.global Data_02006878
Data_02006878:
	.space 0x00000004
	.global Data_0200687c
Data_0200687c:
	.space 0x00000004
	.global Data_02006880
Data_02006880:
	.space 0x00000004
	.global Data_02006884
Data_02006884:
	.space 0x00000004
	.global Data_0200e888
Data_0200e888:
	.space 0x00000004
	.global Data_0200e88c
Data_0200e88c:
	.space 0x00000004
	.global Data_02006890
Data_02006890:
	.space 0x00000004
	.global Data_02006894
Data_02006894:
	.space 0x00000004
	.global Data_02006898
Data_02006898:
	.space 0x00000004
	.global Data_0200689c
Data_0200689c:
	.space 0x00000004
	.global Data_020068a0
Data_020068a0:
	.space 0x00000002
	.global Data_020068a2
Data_020068a2:
	.space 0x00000002
	.global Data_020068a4
Data_020068a4:
	.space 0x0000000c
	.global Data_020068b0
Data_020068b0:
	.space 0x00000004
	.global Data_020068b4
Data_020068b4:
