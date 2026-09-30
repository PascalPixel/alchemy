.syntax unified
	.thumb
	.section .text.x02008038,"ax",%progbits
	.global Func_02000038
	.thumb_func
Func_02000038:
	ldr r0, .L_0200803c
	bx lr
.L_0200803c:
	.4byte Data_0200064c
	.section .text.x02008040,"ax",%progbits
	.global Func_02000040
	.thumb_func
Func_02000040:
	movs r0, #0
	bx lr
	.section .text.x02008044,"ax",%progbits
	.global Func_02000044
	.thumb_func
Func_02000044:
	ldr r0, .L_02008048
	bx lr
.L_02008048:
	.4byte Data_0200067c
	.section .text.x0200804c,"ax",%progbits
	.global Func_0200004c
	.thumb_func
Func_0200004c:
	ldr r0, .L_02008050
	bx lr
.L_02008050:
	.4byte Data_02000680
	.section .text.x02008054,"ax",%progbits
	.global Func_02000054
	.thumb_func
Func_02000054:
	ldr r0, .L_02008058
	bx lr
.L_02008058:
	.4byte Data_02000698
	.section .text.x0200805c,"ax",%progbits
	.global Func_0200005c
	.thumb_func
Func_0200005c:
	push {r5, r6, lr}
	ldr r5, .L_020081e8
	movs r2, #241
	lsls r2, r2, #1
	adds r6, r5, r2
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #10
	bne .L_020080ba
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r5, r2
	ldr r0, [r3]
	bl Object_GetById
	movs r3, #0
	adds r0, #85
	strb r3, [r0]
	movs r0, #75
	bl Func_02000644
	movs r0, #0
	bl Func_0200031c
	movs r0, #120
	bl WaitFrames
	movs r5, #0
	b .L_02008098
.L_02008096:
	adds r5, #1
.L_02008098:
	movs r3, #209
	lsls r3, r3, #4
	adds r3, #255
	cmp r5, r3
	bgt .L_020080b0
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_020081ec
	ldr r3, [r3, #4]
	cmp r3, #0
	beq .L_02008096
.L_020080b0:
	ldr r0, .L_020081f0
	movs r1, #2
	bl Func_02000604
	b .L_020081e2
.L_020080ba:
	cmp r3, #8
	bne .L_020080f0
	movs r0, #68
	bl Func_02000644
	movs r0, #1
	bl Func_0200061c
	movs r0, #78
	bl Func_02000644
	movs r0, #60
	bl Blend_SetDarkenTarget16
	bl Blend_WaitForTransition
	movs r0, #240
	bl Battle_WaitMode0
	movs r0, #0
	bl Func_02000644
	ldr r0, .L_020081f0
	movs r1, #2
	bl Func_02000604
	b .L_020081e2
.L_020080f0:
	ldr r0, .L_020081f4
	bl Func_020005bc
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #1
	bne .L_02008162
.L_020080fe:
	movs r0, #0
	bl Func_02000644
	movs r0, #0
	bl Func_0200062c
	movs r0, #0
	bl Func_02000634
	bl Func_020005d4
	cmp r0, #0
	ble .L_02008158
	movs r0, #40
	bl Func_02000644
	movs r0, #1
	bl Func_0200063c
	movs r0, #1
	bl Func_02000624
	cmp r0, #0
	bne .L_02008158
	movs r0, #78
	bl Func_02000644
	movs r0, #30
	bl Blend_SetDarkenTarget16
	bl Blend_WaitForTransition
	movs r5, #0
	b .L_02008144
.L_02008142:
	adds r5, #1
.L_02008144:
	cmp r5, #119
	bgt .L_020080fe
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_020081ec
	ldr r3, [r3, #4]
	cmp r3, #0
	beq .L_02008142
	b .L_020080fe
.L_02008158:
	ldr r0, .L_020081f8
	movs r1, #1
	bl Func_02000604
	b .L_020081c6
.L_02008162:
	cmp r3, #2
	bne .L_02008192
	movs r0, #40
	bl Func_02000644
	movs r0, #0
	bl Func_0200063c
	movs r2, #147
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r5, r2
	adds r2, #1
	ldrb r0, [r3]
	adds r3, r5, r2
	ldrb r1, [r3]
	bl Func_020005dc
	bl Func_020005e4
	ldr r0, .L_020081fc
	movs r1, #99
	bl Func_02000604
.L_02008192:
	movs r2, #0
	ldrsh r3, [r6, r2]
	cmp r3, #3
	bne .L_020081c6
	movs r0, #75
	bl Func_02000644
	movs r0, #0
	bl Func_02000624
	movs r2, #147
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r5, r2
	adds r2, #1
	ldrb r0, [r3]
	adds r3, r5, r2
	ldrb r1, [r3]
	bl Func_020005dc
	bl Func_020005ec
	ldr r0, .L_02008200
	movs r1, #13
	bl Func_02000604
.L_020081c6:
	movs r0, #78
	bl Func_02000644
	movs r0, #30
	bl Blend_SetDarkenTarget16
	bl Blend_WaitForTransition
	movs r0, #60
	bl Battle_WaitMode0
	movs r0, #0
	bl Func_02000644
.L_020081e2:
	movs r0, #0
	pop {r5, r6, pc}
	.2byte 0x0000
.L_020081e8:
	.4byte gPartyState
.L_020081ec:
	.4byte gInput
.L_020081f0:
	.4byte 0x00000000
.L_020081f4:
	.4byte 0x0000000b
.L_020081f8:
	.4byte 0x00000001
.L_020081fc:
	.4byte 0x00000005
.L_02008200:
	.4byte 0x00000009
	.section .text.x02008204,"ax",%progbits
	.global Func_02000204
	.thumb_func
Func_02000204:
	movs r0, #0
	bx lr
	.section .text.x02008208,"ax",%progbits
	.global Func_02000208
	.thumb_func
Func_02000208:
	push {r5, r6, lr}
	movs r0, #164
	lsls r0, r0, #3
	bl Runtime_BumpAllocateAlternatePool
	ldr r6, .L_0200826c
	adds r5, r0, #0
	movs r2, #0
	ldrsh r3, [r6, r2]
	movs r2, #1
	negs r2, r2
	cmp r3, r2
	bne .L_02008228
	bl Resource_FindFreeEntry
	strh r0, [r6]
.L_02008228:
	ldr r0, .L_02008270
	bl Resource_GetTableEntry
	adds r1, r5, #0
	bl Func_02000594
	movs r3, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	adds r0, r5, #0
	ldr r1, .L_02008274
	adds r2, #8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r2, r5, #0
	movs r1, #160
	adds r2, #32
	movs r3, #0
	ldrsh r0, [r6, r3]
	lsls r1, r1, #3
	bl VramBlock_LoadCached
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #212
.L_0200825e:
	ldr r3, [r2, #8]
	cmp r3, #0
	blt .L_0200825e
	adds r0, r5, #0
	bl Sys_Free
	pop {r5, r6, pc}
.L_0200826c:
	.4byte Data_020006a4
.L_02008270:
	.4byte 0x00000025
.L_02008274:
	.4byte 0x050003e0
	.section .text.x02008278,"ax",%progbits
	.global Func_02000278
	.thumb_func
Func_02000278:
	push {r5, r6, r7, lr}
	ldr r3, .L_02008308
	ldr r2, .L_0200830c
	movs r1, #0
	ldrsh r3, [r3, r1]
	ldr r4, .L_02008310
	lsls r3, r3, #2
	adds r3, r3, r2
	ldrh r3, [r3, #2]
	sub sp, #4
	lsrs r5, r3, #5
	movs r7, #0
	adds r6, r4, #0
.L_02008292:
	movs r2, #18
	subs r2, r2, r7
	lsls r2, r2, #3
	movs r3, #232
	subs r3, r3, r2
	movs r2, #0
	movs r1, #136
	str r2, [r6]
	lsls r3, r3, #16
	movs r2, #132
	orrs r3, r1
	lsls r2, r2, #8
	orrs r3, r2
	str r3, [r6, #4]
	movs r3, #240
	lsls r3, r3, #8
	orrs r3, r5
	str r3, [r6, #8]
	ldr r3, .L_02008314
	adds r6, #12
	ldrh r3, [r3]
	lsls r3, r3, #16
	asrs r2, r3, #16
	lsrs r3, r3, #31
	adds r2, r2, r3
	asrs r2, r2, #1
	subs r1, r2, r7
	cmp r1, #0
	bge .L_020082ce
	movs r1, #0
.L_020082ce:
	cmp r1, #2
	bgt .L_020082e0
	ldr r3, .L_02008318
	movs r2, #1
	ldr r3, [r3]
	ands r3, r2
	cmp r3, #0
	beq .L_020082e0
	movs r1, #0
.L_020082e0:
	cmp r1, #0
	beq .L_020082f2
	adds r0, r4, #0
	movs r1, #255
	adds r4, #12
	str r4, [sp, #0]
	bl Func_020005ac
	ldr r4, [sp, #0]
.L_020082f2:
	adds r7, #1
	adds r5, #2
	cmp r7, #17
	ble .L_02008292
	ldr r2, .L_02008314
	add sp, #4
	ldrh r3, [r2]
	adds r3, #1
	strh r3, [r2]
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_02008308:
	.4byte Data_020006a4
.L_0200830c:
	.4byte ResourceTableEntries
.L_02008310:
	.4byte gOverlayArea + 0x6f0
.L_02008314:
	.4byte gOverlayArea + 0x6dc
.L_02008318:
	.4byte Data_0300122c
	.section .text.x0200831c,"ax",%progbits
	.global Func_0200031c
	.thumb_func
Func_0200031c:
	push {r5, r6, lr}
	bl Func_02000490
	movs r0, #30
	bl Battle_WaitMode0
	ldr r2, .L_02008354
	ldr r3, .L_02008350
	movs r0, #0
	strh r3, [r2]
	bl Func_02000208
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_02008358
	bl Scheduler_AddOrUpdateCallback
	ldr r0, .L_0200835c
	ldr r1, .L_02008360
	ldrh r3, [r1]
	adds r4, r3, #0
	strh r1, [r1]
	ldrh r2, [r0]
	cmp r2, #31
	bgt .L_02008384
	b .L_02008364
.L_02008350:
	.4byte 0x00000000
.L_02008354:
	.4byte gOverlayArea + 0x6dc
.L_02008358:
	.4byte Func_02000278
.L_0200835c:
	.4byte Data_020038e0
.L_02008360:
	.4byte 0x04000208
.L_02008364:
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r0
	strh r2, [r0]
	movs r2, #170
	adds r3, #4
	lsls r2, r2, #5
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_02008384:
	strh r4, [r1]
	ldrh r3, [r1]
	adds r4, r3, #0
	strh r1, [r1]
	ldrh r2, [r0]
	cmp r2, #31
	bgt .L_020083b6
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r0]
	movs r2, #188
	adds r3, r3, r0
	lsls r2, r2, #6
	adds r3, #4
	adds r2, #206
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_020083b6:
	strh r4, [r1]
	ldrh r3, [r1]
	adds r4, r3, #0
	strh r1, [r1]
	ldrh r2, [r0]
	cmp r2, #31
	bgt .L_020083e4
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r0
	adds r3, #4
	strh r2, [r0]
	movs r2, #16
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #84
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_020083e4:
	strh r4, [r1]
	ldrh r3, [r1]
	adds r4, r3, #0
	strh r1, [r1]
	ldrh r2, [r0]
	cmp r2, #31
	bgt .L_02008416
	lsls r3, r2, #1
	adds r3, r3, r2
	adds r2, #1
	lsls r3, r3, #2
	strh r2, [r0]
	movs r2, #128
	adds r3, r3, r0
	lsls r2, r2, #5
	adds r3, #4
	adds r2, #16
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #82
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_02008416:
	strh r4, [r1]
	movs r0, #120
	bl Battle_WaitMode0
	movs r5, #0
.L_02008420:
	ldr r1, .L_02008488
	ldr r0, .L_0200848c
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r3, [r1]
	cmp r3, #31
	bgt .L_02008452
	lsls r2, r3, #1
	adds r2, r2, r3
	lsls r2, r2, #2
	adds r3, #1
	adds r2, r2, r1
	strh r3, [r1]
	movs r3, #16
	adds r2, #4
	subs r3, r3, r5
	stmia r2!, {r3}
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #84
	stmia r2!, {r3}
	movs r3, #128
	lsls r3, r3, #10
	str r3, [r2]
.L_02008452:
	strh r4, [r0]
	movs r0, #3
	adds r5, #1
	bl WaitFrames
	cmp r5, #16
	ble .L_02008420
	movs r6, #192
	lsls r6, r6, #18
	ldr r1, [r6, #108]
	movs r3, #214
	lsls r3, r3, #1
	adds r2, r1, r3
	movs r5, #218
	movs r3, #0
	str r3, [r2]
	lsls r5, r5, #1
	movs r3, #1
	str r3, [r1, r5]
	bl Event_SetStatus1c6
	bl Event_WaitValue1c8Frames
	ldr r2, [r6, #108]
	movs r3, #60
	str r3, [r2, r5]
	pop {r5, r6, pc}
.L_02008488:
	.4byte Data_020038e0
.L_0200848c:
	.4byte 0x04000208
	.section .text.x02008490,"ax",%progbits
	.global Func_02000490
	.thumb_func
Func_02000490:
	push {r5, r6, lr}
	movs r0, #0
	ldr r5, .L_020084d8
	bl Blend_SetDarkenTarget16
	ldr r3, .L_020084d4
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #12
	strh r3, [r2]
	ldr r2, .L_020084dc
	movs r3, #0
	strh r3, [r2, #10]
	adds r0, r5, #0
	bl Resource_GetTableEntry
	movs r6, #128
	movs r3, #128
	movs r2, #132
	lsls r6, r6, #1
	lsls r3, r3, #19
	movs r1, #160
	lsls r2, r2, #24
	adds r6, #255
	adds r4, r0, #0
	adds r3, #212
	lsls r1, r1, #19
	adds r2, #112
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #224
	lsls r3, r3, #1
	adds r4, r4, r3
	b .L_020084e0
.L_020084d4:
	.4byte 0x00000681
.L_020084d8:
	.4byte 0x00000022
.L_020084dc:
	.4byte Data_03001120
.L_020084e0:
	adds r0, r4, #0
	ldr r1, .L_02008560
	bl Func_02000594
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_02008560
	ldr r1, .L_02008564
	ldr r2, .L_02008568
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r1, .L_0200856c
	movs r3, #208
	lsls r3, r3, #1
	movs r0, #0
.L_02008500:
	movs r4, #0
.L_02008502:
	adds r2, r3, #0
	movs r5, #128
	lsls r3, r2, #16
	lsls r5, r5, #9
	adds r3, r3, r5
	adds r4, #1
	strh r2, [r1]
	asrs r3, r3, #16
	adds r1, #2
	cmp r4, #29
	bls .L_02008502
	strh r6, [r1]
	adds r0, #1
	adds r1, #2
	strh r6, [r1]
	adds r1, #2
	cmp r0, #19
	bls .L_02008500
	ldr r2, .L_02008570
	movs r0, #0
.L_0200852a:
	movs r3, #0
	adds r0, #1
	strh r3, [r2, #2]
	strh r3, [r2]
	adds r2, #4
	cmp r0, #3
	bls .L_0200852a
	movs r3, #128
	movs r1, #128
	movs r2, #132
	lsls r3, r3, #19
	lsls r1, r1, #19
	lsls r2, r2, #24
	adds r3, #212
	ldr r0, .L_02008570
	adds r1, #16
	adds r2, #4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #32]
	movs r3, #160
	lsls r3, r3, #5
	strh r3, [r2, #20]
	pop {r5, r6, pc}
	.2byte 0x0000
.L_02008560:
	.4byte gMapCellBuffer
.L_02008564:
	.4byte 0x06006800
.L_02008568:
	.4byte 0x84002580
.L_0200856c:
	.4byte 0x06003000
.L_02008570:
	.4byte Data_03001120
	.section .rodata.x0200864c,"a",%progbits
	.global Data_0200064c
Data_0200064c:
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
	.global Data_0200067c
Data_0200067c:
	.4byte 0x000001ff
	.global Data_02000680
Data_02000680:
	.4byte 0x0000ffff
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_02000698
Data_02000698:
	.4byte 0xffffffff
	.4byte 0x00000000
	.4byte 0x00000000
	.global Data_020006a4
Data_020006a4:
	.2byte 0xffff
