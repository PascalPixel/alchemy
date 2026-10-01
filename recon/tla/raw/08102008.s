.syntax unified
	.thumb
	.global Func_08102008
	.thumb_func
Func_08102008:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #112
	str r0, [sp, #84]
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	movs r0, #192
	str r3, [sp, #80]
	lsls r0, r0, #1
	adds r3, r3, r0
	ldr r3, [r3]
	ldr r1, [sp, #80]
	mov r10, r3
	ldr r3, [sp, #80]
	movs r2, #4
	adds r3, #240
	ldr r3, [r3]
	ldr r7, [sp, #80]
	str r3, [sp, #76]
	ldr r3, [sp, #84]
	ldr r1, [r1, #52]
	lsls r3, r3, #1
	str r1, [sp, #72]
	str r2, [sp, #68]
	str r3, [sp, #56]
	movs r4, #180
	lsls r4, r4, #1
	adds r3, r3, r4
	ldrh r5, [r7, r3]
	movs r1, #10
	adds r0, r5, #0
	bl __umodsi3
	lsls r0, r0, #16
	lsrs r0, r0, #16
	movs r1, #10
	mov r11, r0
	adds r0, r5, #0
	bl __udivsi3
	mov r2, sp
	adds r2, #104
	lsls r0, r0, #16
	lsrs r0, r0, #16
	str r2, [sp, #12]
	movs r1, #1
	str r0, [sp, #52]
	negs r1, r1
	movs r0, #0
	ldr r4, [sp, #12]
	str r0, [sp, #48]
	str r0, [sp, #44]
	str r0, [sp, #40]
	str r0, [sp, #36]
	str r0, [sp, #32]
	str r0, [sp, #28]
	str r0, [sp, #24]
	str r0, [sp, #20]
	str r1, [sp, #60]
	mov r3, sp
	movs r2, #0
	adds r3, #111
	mov r12, r4
.L_08102094:
	strb r2, [r3]
	subs r3, #1
	cmp r3, r12
	bge .L_08102094
	ldr r7, [sp, #84]
	cmp r7, #0
	bne .L_08102116
	ldr r0, [sp, #52]
	ldr r3, [sp, #80]
	movs r4, #139
	str r0, [sp, #48]
	lsls r4, r4, #1
	adds r4, #255
	adds r2, r3, r4
	ldrb r3, [r2]
	movs r1, #0
	mov r9, r1
	cmp r7, r3
	bge .L_081020dc
	ldr r1, [sp, #12]
	adds r0, r2, #0
	mov r2, r10
	movs r4, #4
	adds r2, #160
.L_081020c4:
	ldrb r3, [r2]
	adds r2, #1
	lsls r3, r3, #24
	cmp r3, #0
	bne .L_081020d0
	strb r4, [r1]
.L_081020d0:
	ldrb r3, [r0]
	movs r7, #1
	add r9, r7
	adds r1, #1
	cmp r9, r3
	blt .L_081020c4
.L_081020dc:
	ldr r1, [sp, #80]
	movs r2, #139
	lsls r2, r2, #1
	adds r2, #255
	adds r3, r1, r2
	ldrb r3, [r3]
	movs r0, #0
	mov r9, r0
	cmp r9, r3
	bge .L_08102180
	adds r5, r1, r2
	adds r6, r3, #0
.L_081020f4:
	ldr r4, [sp, #12]
	mov r7, r11
	ldrsb r3, [r4, r7]
	cmp r3, #4
	bne .L_0810210c
	movs r0, #1
	add r11, r0
	mov r0, r11
	ldrb r1, [r5]
	bl Func_08100e28
	mov r11, r0
.L_0810210c:
	movs r1, #1
	add r9, r1
	cmp r9, r6
	blt .L_081020f4
	b .L_08102180
.L_08102116:
	ldr r2, [sp, #80]
	add r5, sp, #88
	movs r1, #28
	ldrsb r1, [r2, r1]
	adds r0, r5, #0
	bl Func_08104928
	movs r7, #139
	ldr r4, [sp, #80]
	lsls r7, r7, #1
	adds r7, #255
	movs r3, #0
	adds r2, r4, r7
	mov r9, r3
	ldrb r3, [r2]
	cmp r9, r3
	bge .L_08102180
	ldr r0, [sp, #12]
	mov r1, r10
	adds r4, r2, #0
	adds r1, #160
	adds r2, r0, #0
	movs r6, #7
.L_08102144:
	ldr r7, [sp, #80]
	movs r3, #28
	ldrsb r3, [r7, r3]
	cmp r9, r3
	bne .L_08102152
	strb r6, [r2]
	b .L_08102170
.L_08102152:
	mov r7, r9
	ldrb r3, [r5, r7]
	cmp r3, #0
	beq .L_08102160
	movs r3, #0
	strb r3, [r2]
	b .L_08102170
.L_08102160:
	movs r3, #3
	strb r3, [r2]
	movs r3, #0
	ldrsb r3, [r1, r3]
	cmp r3, #0
	bne .L_08102170
	movs r3, #7
	strb r3, [r0]
.L_08102170:
	movs r3, #1
	add r9, r3
	ldrb r3, [r4]
	adds r0, #1
	adds r1, #1
	adds r2, #1
	cmp r9, r3
	blt .L_08102144
.L_08102180:
	ldr r4, [sp, #80]
	movs r3, #1
	ldr r2, [r4, #20]
	strb r3, [r2, #5]
.L_08102188:
	ldr r7, [sp, #68]
	cmp r7, #0
	bne .L_08102190
	b .L_081025c0
.L_08102190:
	movs r0, #1
	negs r0, r0
	str r0, [sp, #60]
	ldr r1, [sp, #12]
	mov r3, r11
	ldrb r2, [r1, r3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	bne .L_081021aa
	ldr r4, [sp, #52]
	str r4, [sp, #60]
	b .L_081021ca
.L_081021aa:
	mov r3, r11
	cmp r3, #0
	bge .L_081021b2
	adds r3, #3
.L_081021b2:
	asrs r3, r3, #2
	lsls r3, r3, #2
	mov r7, r11
	subs r3, r7, r3
	lsls r0, r3, #3
	subs r0, r0, r3
	lsls r0, r0, #3
	subs r0, #8
	movs r1, #52
	bl Func_080f8a44
	b .L_081021ec
.L_081021ca:
	mov r3, r11
	cmp r3, #0
	bge .L_081021d2
	adds r3, #3
.L_081021d2:
	asrs r3, r3, #2
	mov r0, r11
	lsls r3, r3, #2
	subs r3, r0, r3
	ldr r2, [sp, #52]
	lsls r0, r3, #3
	subs r0, r0, r3
	lsls r0, r0, #3
	lsls r1, r2, #3
	subs r0, #8
	adds r1, #60
	bl Func_080f8a44
.L_081021ec:
	ldr r4, [sp, #68]
	lsrs r3, r4, #2
	cmp r3, #0
	bne .L_081021f6
	b .L_0810230a
.L_081021f6:
	mov r3, r11
	cmp r3, #0
	bge .L_081021fe
	adds r3, #3
.L_081021fe:
	asrs r3, r3, #2
	lsls r7, r3, #2
	adds r0, r7, #0
	bl Func_08104ef8
	mov r0, r10
	adds r1, r7, #0
	bl Func_08101ac8
	ldr r0, [sp, #84]
	cmp r0, #1
	bne .L_081022f8
	ldr r1, [sp, #80]
	movs r2, #180
	lsls r2, r2, #1
	adds r3, r1, r2
	ldrh r5, [r3]
	movs r1, #10
	adds r0, r5, #0
	bl __umodsi3
	lsls r0, r0, #16
	lsrs r6, r0, #16
	movs r1, #10
	adds r0, r5, #0
	bl __udivsi3
	lsls r0, r0, #16
	lsrs r2, r0, #16
	adds r3, r6, #0
	cmp r6, #0
	bge .L_08102240
	adds r3, r6, #3
.L_08102240:
	asrs r3, r3, #2
	lsls r3, r3, #2
	cmp r3, r7
	bne .L_0810227c
	adds r3, r6, #0
	mov r4, r10
	adds r3, #160
	ldrsb r5, [r4, r3]
	ldr r0, [sp, #72]
	adds r1, r6, #0
	movs r3, #1
	bl Func_08101c40
	subs r3, r6, r7
	lsls r1, r3, #3
	subs r1, r1, r3
	adds r1, #1
	movs r3, #6
	ldr r0, [sp, #72]
	movs r2, #2
	str r3, [sp, #4]
	str r5, [sp, #0]
	bl Func_08101d34
	ldr r0, [sp, #80]
	movs r1, #142
	lsls r1, r1, #2
	adds r2, r0, r1
	movs r3, #54
	b .L_08102286
.L_0810227c:
	ldr r3, [sp, #80]
	movs r4, #142
	lsls r4, r4, #2
	adds r2, r3, r4
	movs r3, #200
.L_08102286:
	strh r3, [r2]
	ldr r5, [sp, #80]
	movs r0, #0
	mov r9, r0
	movs r6, #8
	adds r5, #248
.L_08102292:
	ldmia r5!, {r3}
	cmp r3, #0
	beq .L_081022ec
	ldr r4, [sp, #80]
	mov r1, r9
	movs r3, #28
	ldrsb r3, [r4, r3]
	adds r2, r7, r1
	cmp r2, r3
	bne .L_081022be
	movs r0, #182
	lsls r0, r0, #1
	adds r3, r4, r0
	ldrh r2, [r3]
	ldr r3, .L_081022d0
	ands r2, r3
	cmp r2, #0
	beq .L_081022ba
	ldr r0, .L_081022d4
	b .L_081022e2
.L_081022ba:
	ldr r0, .L_081022d8
	b .L_081022e2
.L_081022be:
	ldr r1, [sp, #12]
	movs r3, #2
	ldrb r2, [r1, r2]
	ands r3, r2
	cmp r3, #0
	beq .L_081022e0
	ldr r0, .L_081022dc
	b .L_081022e2
	.2byte 0x0000
.L_081022d0:
	.4byte 0x00008000
.L_081022d4:
	.4byte 0x000010e1
.L_081022d8:
	.4byte 0x000010e0
.L_081022dc:
	.4byte 0x000010df
.L_081022e0:
	ldr r0, .L_0810249c
.L_081022e2:
	ldr r1, [sp, #72]
	adds r2, r6, #0
	movs r3, #8
	bl UiText_DrawCharacterAtOffsetFar
.L_081022ec:
	movs r2, #1
	add r9, r2
	mov r3, r9
	adds r6, #56
	cmp r3, #3
	ble .L_08102292
.L_081022f8:
	ldr r4, [sp, #80]
	movs r7, #139
	lsls r7, r7, #1
	adds r7, #255
	adds r3, r4, r7
	ldrb r1, [r3]
	mov r0, r11
	bl Func_08104d14
.L_0810230a:
	ldr r0, [sp, #12]
	mov r1, r11
	ldrb r2, [r0, r1]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	bne .L_08102322
	ldr r0, [sp, #72]
	ldr r2, [sp, #52]
	movs r3, #1
	bl Func_08101c40
.L_08102322:
	ldr r2, [sp, #60]
	movs r5, #1
	negs r5, r5
	cmp r2, r5
	beq .L_0810233e
	mov r4, r11
	lsls r3, r4, #2
	add r3, r11
	lsls r3, r3, #1
	adds r3, r3, r2
	lsls r3, r3, #1
	mov r7, r10
	ldrh r3, [r7, r3]
	str r3, [sp, #44]
.L_0810233e:
	ldr r0, [sp, #44]
	movs r3, #240
	lsls r3, r3, #4
	ands r3, r0
	lsrs r3, r3, #8
	str r3, [sp, #32]
	movs r1, #31
	movs r3, #224
	ands r3, r0
	ands r0, r1
	lsrs r3, r3, #5
	str r0, [sp, #24]
	ldr r0, [sp, #44]
	str r3, [sp, #28]
	bl Func_08101a04
	ldr r2, [sp, #60]
	str r0, [sp, #20]
	cmp r2, r5
	bne .L_08102372
	ldr r0, [sp, #84]
	movs r1, #0
	movs r2, #200
	bl Func_08105300
	b .L_081023de
.L_08102372:
	ldr r3, [sp, #20]
	movs r6, #128
	movs r5, #1
	lsls r6, r6, #7
	cmp r3, #0
	beq .L_08102390
	cmp r3, #1
	bne .L_0810238a
	movs r6, #128
	movs r5, #2
	lsls r6, r6, #7
	b .L_08102390
.L_0810238a:
	movs r6, #128
	movs r5, #1
	lsls r6, r6, #8
.L_08102390:
	mov r2, r11
	cmp r2, #0
	bge .L_08102398
	adds r2, #3
.L_08102398:
	asrs r2, r2, #2
	lsls r2, r2, #2
	mov r4, r11
	subs r2, r4, r2
	lsls r3, r2, #3
	subs r3, r3, r2
	ldr r7, [sp, #84]
	lsls r3, r3, #3
	adds r1, r3, #0
	adds r1, #48
	movs r2, #62
	cmp r7, #0
	beq .L_081023b4
	movs r2, #54
.L_081023b4:
	ldr r0, [sp, #84]
	bl Func_08105300
	adds r1, r6, #0
	ldr r0, [sp, #84]
	bl Func_0810532c
	ldr r0, [sp, #68]
	movs r3, #2
	ands r3, r0
	cmp r3, #0
	beq .L_081023d4
	ldr r0, [sp, #84]
	movs r1, #0
	bl Func_08105350
.L_081023d4:
	ldr r0, [sp, #84]
	ldr r1, [sp, #28]
	adds r2, r5, #0
	bl Func_081052ac
.L_081023de:
	ldr r1, [sp, #80]
	mov r2, r11
	movs r4, #129
	lsls r4, r4, #2
	lsls r3, r2, #1
	adds r3, r3, r4
	ldrh r0, [r1, r3]
	ldr r6, [r1, #16]
	bl Owner_GetState
	movs r7, #42
	adds r5, r0, #0
	adds r0, r6, #0
	bl RenderOutput_RedrawSavedRectFar
	adds r7, #255
	adds r0, r5, #0
	adds r1, r6, #0
	movs r2, #0
	movs r3, #0
	bl UiText_DrawStringAtOffsetFar
	adds r3, r5, r7
	ldrb r0, [r3]
	ldr r3, .L_081024a0
	adds r1, r6, #0
	adds r0, r0, r3
	movs r2, #0
	movs r3, #8
	bl UiText_DrawCharacterAtOffsetFar
	ldr r0, .L_081024a4
	adds r1, r6, #0
	movs r2, #48
	movs r3, #0
	bl UiText_DrawStringAtOffsetFar
	movs r3, #0
	ldrb r0, [r5, #15]
	movs r1, #2
	str r3, [sp, #0]
	adds r2, r6, #0
	movs r3, #72
	bl UiText_DrawNumberInWindowFar
	ldr r0, [sp, #84]
	cmp r0, #0
	bne .L_0810244a
	ldr r0, .L_081024a8
	adds r1, r6, #0
	movs r2, #0
	movs r3, #16
	bl UiText_DrawCharacterAtOffsetFar
.L_0810244a:
	ldr r0, [sp, #76]
	bl RenderOutput_RedrawSavedRectFar
	ldr r1, [sp, #84]
	cmp r1, #0
	bne .L_081024e0
	ldr r2, [sp, #60]
	movs r3, #1
	negs r3, r3
	cmp r2, r3
	beq .L_081024e0
	ldr r4, [sp, #40]
	cmp r4, #0
	beq .L_081024b8
	ldr r7, [sp, #36]
	cmp r7, #0
	bne .L_0810247a
	ldr r0, .L_081024ac
	ldr r1, [sp, #76]
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
	b .L_08102486
.L_0810247a:
	ldr r0, .L_081024b0
	ldr r1, [sp, #76]
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
.L_08102486:
	ldr r0, [sp, #20]
	cmp r0, #2
	bne .L_081024e0
	ldr r0, .L_081024b4
	ldr r1, [sp, #76]
	movs r2, #0
	movs r3, #16
	bl UiText_DrawCharacterAtOffsetFar
	b .L_081024e0
	.2byte 0x0000
.L_0810249c:
	.4byte 0x000010e2
.L_081024a0:
	.4byte 0x00000b63
.L_081024a4:
	.4byte Data_081059d4
.L_081024a8:
	.4byte 0x000010da
.L_081024ac:
	.4byte 0x000010c9
.L_081024b0:
	.4byte 0x000010ca
.L_081024b4:
	.4byte 0x000010cf
.L_081024b8:
	ldr r5, .L_08102844
	ldr r1, [sp, #76]
	adds r0, r5, #0
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
	ldr r1, [sp, #20]
	adds r0, r5, #3
	cmp r1, #0
	beq .L_081024d6
	adds r0, r5, #2
	cmp r1, #1
	beq .L_081024d6
	ldr r0, .L_08102848
.L_081024d6:
	ldr r1, [sp, #76]
	movs r2, #0
	movs r3, #16
	bl UiText_DrawCharacterAtOffsetFar
.L_081024e0:
	movs r0, #1
	bl WaitFrames
	ldr r2, [sp, #84]
	cmp r2, #1
	bne .L_08102572
	ldr r4, [sp, #80]
	movs r7, #128
	lsls r7, r7, #2
	adds r7, #22
	adds r3, r4, r7
	ldrb r0, [r3]
	movs r1, #1
	bl UiText_DrawQuantity
	ldr r4, .L_0810284c
	ldr r1, [sp, #76]
	adds r0, r4, #0
	movs r2, #0
	movs r3, #0
	str r4, [sp, #8]
	bl UiText_DrawCharacterAtOffsetFar
	ldr r0, [sp, #80]
	movs r1, #182
	lsls r1, r1, #1
	adds r6, r0, r1
	ldrh r2, [r6]
	movs r5, #224
	adds r3, r5, #0
	ands r3, r2
	lsrs r3, r3, #5
	lsls r0, r3, #2
	adds r0, r0, r3
	movs r3, #31
	ands r3, r2
	lsls r0, r0, #2
	movs r2, #150
	adds r0, r0, r3
	lsls r2, r2, #1
	adds r0, r0, r2
	movs r1, #4
	bl UiText_DrawQuantity
	ldrh r3, [r6]
	ldr r0, [sp, #76]
	ands r5, r3
	movs r3, #160
	lsls r3, r3, #7
	adds r3, #1
	lsrs r5, r5, #5
	adds r5, r5, r3
	adds r1, r5, #0
	movs r3, #0
	movs r2, #6
	str r3, [sp, #0]
	bl UiWindow_SetTilemapEntryFar
	ldr r4, [sp, #8]
	ldr r1, [sp, #76]
	adds r0, r4, #1
	movs r2, #56
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
	ldr r4, [sp, #8]
	ldr r1, [sp, #76]
	adds r4, #2
	adds r0, r4, #0
	movs r2, #0
	movs r3, #8
	bl UiText_DrawCharacterAtOffsetFar
.L_08102572:
	movs r0, #1
	bl WaitFrames
	ldr r0, [sp, #72]
	bl RenderOutput_ClearListFar
	ldr r4, [sp, #60]
	movs r7, #1
	negs r7, r7
	cmp r4, r7
	beq .L_081025b2
	movs r3, #104
	str r3, [sp, #0]
	ldr r0, [sp, #72]
	movs r1, #0
	movs r2, #96
	movs r3, #224
	bl UiWindow_ClearInteriorTilesFar
	ldr r1, [sp, #28]
	ldr r2, [sp, #24]
	lsls r0, r1, #2
	adds r0, r0, r1
	ldr r3, .L_08102850
	lsls r0, r0, #2
	adds r0, r0, r2
	adds r0, r0, r3
	ldr r1, [sp, #72]
	movs r2, #0
	movs r3, #96
	bl UiText_DrawCharacterAtOffsetFar
.L_081025b2:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #60]
	movs r3, #1
	strb r3, [r2, #3]
	movs r3, #0
	str r3, [sp, #68]
.L_081025c0:
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_08102854
	movs r2, #128
	ldr r3, [r1]
	lsls r2, r2, #1
	ands r3, r2
	cmp r3, #0
	beq .L_081025dc
	ldr r3, [r1, #28]
	ands r3, r2
	cmp r3, #0
	beq .L_081025ec
.L_081025dc:
	ldr r4, [sp, #40]
	cmp r4, #0
	beq .L_081025e6
	movs r7, #1
	str r7, [sp, #68]
.L_081025e6:
	movs r0, #0
	str r0, [sp, #40]
	str r0, [sp, #36]
.L_081025ec:
	ldr r2, [r1, #12]
	str r2, [sp, #16]
	ldr r4, [r1, #4]
	movs r1, #132
	lsls r1, r1, #6
	adds r1, #44
	add r1, r10
	ldr r3, [r1]
	cmp r3, #0
	bne .L_08102602
	b .L_08102b06
.L_08102602:
	movs r4, #0
	movs r3, #132
	str r4, [sp, #16]
	lsls r3, r3, #6
	adds r3, #40
	add r3, r10
	ldr r2, [r3]
	adds r2, #1
	str r2, [r3]
	ldr r3, [r1]
	subs r3, #1
	cmp r3, #27
	bls .L_0810261e
	b .L_08102b06
.L_0810261e:
	ldr r2, .L_08102858
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_08102628:
	.4byte .L_08102788
	.4byte .L_081027f4
	.4byte .L_0810281a
	.4byte .L_081027f4
	.4byte .L_08102b06
	.4byte .L_08102860
	.4byte .L_08102860
	.4byte .L_081027f4
	.4byte .L_081027f4
	.4byte .L_08102b06
	.4byte .L_08102b06
	.4byte .L_08102b06
	.4byte .L_081028ce
	.4byte .L_081028f4
	.4byte .L_08102918
	.4byte .L_081028f4
	.4byte .L_081028f4
	.4byte .L_08102a98
	.4byte .L_08102b06
	.4byte .L_08102a98
	.4byte .L_08102abe
	.4byte .L_081027f4
	.4byte .L_08102b06
	.4byte .L_08102ae2
	.4byte .L_08102b06
	.4byte .L_08102b06
	.4byte .L_081026c4
	.4byte .L_08102698
.L_08102698:
	ldr r1, .L_08102854
	movs r3, #1
	ldr r2, [r1, #4]
	ands r2, r3
	cmp r2, #0
	bne .L_081026be
	adds r6, r1, #0
	movs r5, #1
.L_081026a8:
	movs r0, #150
	movs r1, #26
	bl Func_080f8a44
	movs r0, #1
	bl WaitFrames
	ldr r3, [r6, #4]
	ands r3, r5
	cmp r3, #0
	beq .L_081026a8
.L_081026be:
	movs r4, #2
	str r4, [sp, #16]
	b .L_08102b06
.L_081026c4:
	movs r3, #132
	lsls r3, r3, #6
	adds r3, #40
	add r3, r10
	ldr r3, [r3]
	cmp r3, #60
	beq .L_081026d4
	b .L_08102b06
.L_081026d4:
	bl Func_08100e5c
	movs r2, #9
	movs r3, #1
	adds r0, #12
	movs r1, #9
	bl UiText_OpenMessageWindowFar
	ldr r2, .L_0810285c
	movs r3, #139
	lsls r3, r3, #2
	adds r2, r2, r3
	adds r5, r0, #0
	movs r3, #1
	strb r3, [r2]
	b .L_081026fa
.L_081026f4:
	movs r0, #1
	bl WaitFrames
.L_081026fa:
	bl UiWork_IsCompleteFar
	cmp r0, #0
	beq .L_081026f4
	movs r1, #1
	adds r0, r5, #0
	bl UiWork_FinalizeFar
	mov r0, r10
	bl Func_08101c0c
	movs r0, #1
	bl WaitFrames
	bl Func_08100e5c
	movs r2, #9
	movs r3, #1
	adds r0, #13
	movs r1, #9
	bl UiText_OpenMessageWindowFar
	ldr r2, .L_0810285c
	movs r4, #139
	lsls r4, r4, #2
	adds r2, r2, r4
	movs r3, #1
	adds r5, r0, #0
	strb r3, [r2]
	b .L_0810273c
.L_08102736:
	movs r0, #1
	bl WaitFrames
.L_0810273c:
	bl UiWork_IsCompleteFar
	cmp r0, #0
	beq .L_08102736
	movs r1, #1
	adds r0, r5, #0
	bl UiWork_FinalizeFar
	mov r0, r10
	bl Func_08101c0c
	movs r2, #132
	lsls r2, r2, #6
	adds r2, #40
	movs r3, #0
	add r2, r10
	str r3, [r2]
	bl BattlePlacement_UpdateTimedEntriesFar
	bl BattlePlacement_UpdateTimedEntriesFar
	bl BattlePlacement_UpdateTimedEntriesFar
	movs r1, #0
	movs r2, #7
	movs r0, #4
	bl Djinn_DeactivateFar
	movs r1, #0
	movs r2, #7
	movs r0, #4
	bl Trade_AddOfferFar
	movs r0, #4
	bl Owner_RecalculateStatsFar
	movs r7, #2
	b .L_08102b02
.L_08102788:
	movs r3, #132
	lsls r3, r3, #6
	adds r3, #40
	add r3, r10
	ldr r3, [r3]
	cmp r3, #60
	beq .L_08102798
	b .L_08102b06
.L_08102798:
	str r4, [sp, #8]
	bl Func_08100e5c
	movs r1, #9
	movs r2, #9
	movs r3, #1
	bl UiText_OpenMessageWindowFar
	ldr r2, .L_0810285c
	adds r5, r0, #0
	movs r0, #139
	lsls r0, r0, #2
	adds r2, r2, r0
	movs r3, #1
	strb r3, [r2]
	b .L_081027c0
.L_081027b8:
	movs r0, #1
	str r4, [sp, #8]
	bl WaitFrames
.L_081027c0:
	ldr r4, [sp, #8]
	str r4, [sp, #8]
	bl UiWork_IsCompleteFar
	ldr r4, [sp, #8]
	cmp r0, #0
	beq .L_081027b8
	adds r0, r5, #0
	movs r1, #1
	bl UiWork_FinalizeFar
	mov r0, r10
	bl Func_08101c0c
	movs r2, #132
	lsls r2, r2, #6
	adds r2, #40
	add r2, r10
	movs r3, #0
	str r3, [r2]
	movs r2, #132
	lsls r2, r2, #6
	adds r2, #44
	add r2, r10
	movs r3, #2
	b .L_08102a92
.L_081027f4:
	movs r2, #132
	lsls r2, r2, #6
	adds r2, #40
	add r2, r10
	ldr r3, [r2]
	cmp r3, #90
	beq .L_08102804
	b .L_08102b06
.L_08102804:
	movs r3, #0
	movs r1, #1
	str r1, [sp, #16]
	str r3, [r2]
	movs r2, #132
	lsls r2, r2, #6
	adds r2, #44
	add r2, r10
	ldr r3, [r2]
	movs r4, #1
	b .L_08102912
.L_0810281a:
	movs r2, #132
	lsls r2, r2, #6
	adds r2, #40
	add r2, r10
	ldr r3, [r2]
	cmp r3, #90
	beq .L_0810282a
	b .L_08102b06
.L_0810282a:
	movs r3, #16
	str r3, [sp, #16]
	movs r3, #0
	str r3, [r2]
	movs r2, #132
	lsls r2, r2, #6
	adds r2, #44
	add r2, r10
	movs r3, #4
	movs r4, #16
	str r3, [r2]
	b .L_08102b06
	.2byte 0x0000
.L_08102844:
	.4byte 0x000010cb
.L_08102848:
	.4byte 0x000010cf
.L_0810284c:
	.4byte 0x000010e3
.L_08102850:
	.4byte 0x000009b1
.L_08102854:
	.4byte gInput
.L_08102858:
	.4byte .L_08102628
.L_0810285c:
	.4byte gPartyState
.L_08102860:
	movs r3, #132
	lsls r3, r3, #6
	adds r3, #40
	add r3, r10
	ldr r3, [r3]
	cmp r3, #60
	beq .L_08102870
	b .L_08102b06
.L_08102870:
	str r4, [sp, #8]
	bl Func_08100e5c
	movs r1, #9
	movs r2, #9
	movs r3, #1
	adds r0, #1
	bl UiText_OpenMessageWindowFar
	ldr r2, .L_08102bc8
	movs r7, #139
	lsls r7, r7, #2
	adds r2, r2, r7
	movs r3, #1
	adds r5, r0, #0
	strb r3, [r2]
	b .L_0810289a
.L_08102892:
	movs r0, #1
	str r4, [sp, #8]
	bl WaitFrames
.L_0810289a:
	ldr r4, [sp, #8]
	str r4, [sp, #8]
	bl UiWork_IsCompleteFar
	ldr r4, [sp, #8]
	cmp r0, #0
	beq .L_08102892
	adds r0, r5, #0
	movs r1, #1
	bl UiWork_FinalizeFar
	mov r0, r10
	bl Func_08101c0c
	movs r2, #132
	lsls r2, r2, #6
	adds r2, #40
	add r2, r10
	movs r3, #0
	str r3, [r2]
	movs r2, #132
	lsls r2, r2, #6
	adds r2, #44
	add r2, r10
	movs r3, #8
	b .L_08102a92
.L_081028ce:
	movs r2, #132
	lsls r2, r2, #6
	adds r2, #40
	add r2, r10
	ldr r3, [r2]
	cmp r3, #40
	beq .L_081028de
	b .L_08102b06
.L_081028de:
	movs r3, #0
	movs r0, #2
	str r0, [sp, #16]
	str r3, [r2]
	movs r2, #132
	lsls r2, r2, #6
	adds r2, #44
	add r2, r10
	ldr r3, [r2]
	movs r4, #2
	b .L_08102912
.L_081028f4:
	movs r2, #132
	lsls r2, r2, #6
	adds r2, #40
	add r2, r10
	ldr r3, [r2]
	cmp r3, #40
	beq .L_08102904
	b .L_08102b06
.L_08102904:
	movs r3, #0
	str r3, [r2]
	movs r2, #132
	lsls r2, r2, #6
	adds r2, #44
	add r2, r10
	ldr r3, [r2]
.L_08102912:
	adds r3, #1
	str r3, [r2]
	b .L_08102b06
.L_08102918:
	movs r3, #132
	lsls r3, r3, #6
	adds r3, #40
	add r3, r10
	ldr r3, [r3]
	cmp r3, #60
	beq .L_08102928
	b .L_08102b06
.L_08102928:
	ldr r2, .L_08102bc8
	movs r1, #139
	lsls r1, r1, #2
	adds r2, r2, r1
	movs r3, #1
	strb r3, [r2]
	str r4, [sp, #8]
	bl Func_08100e5c
	movs r2, #9
	movs r3, #1
	movs r1, #9
	adds r0, #4
	bl UiText_OpenMessageWindowFar
	movs r1, #146
	adds r5, r0, #0
	movs r0, #2
	bl Func_080f8ab4
	b .L_0810295a
.L_08102952:
	movs r0, #1
	str r4, [sp, #8]
	bl WaitFrames
.L_0810295a:
	ldr r4, [sp, #8]
	str r4, [sp, #8]
	bl UiWork_IsCompleteFar
	ldr r4, [sp, #8]
	cmp r0, #0
	beq .L_08102952
	ldr r1, .L_08102bcc
	movs r3, #1
	ldr r2, [r1, #4]
	ands r2, r3
	cmp r2, #0
	bne .L_08102992
	adds r7, r1, #0
	movs r6, #1
.L_08102978:
	movs r0, #2
	movs r1, #146
	str r4, [sp, #8]
	bl Func_080f8a44
	movs r0, #1
	bl WaitFrames
	ldr r3, [r7, #4]
	ldr r4, [sp, #8]
	ands r3, r6
	cmp r3, #0
	beq .L_08102978
.L_08102992:
	movs r1, #1
	adds r0, r5, #0
	str r4, [sp, #8]
	bl UiWork_FinalizeFar
	mov r0, r10
	bl Func_08101c0c
	movs r0, #1
	bl WaitFrames
	bl Func_08100e5c
	movs r1, #9
	movs r2, #9
	movs r3, #1
	adds r0, #5
	bl UiText_OpenMessageWindowFar
	adds r5, r0, #0
	b .L_081029c4
.L_081029bc:
	movs r0, #1
	str r4, [sp, #8]
	bl WaitFrames
.L_081029c4:
	ldr r4, [sp, #8]
	str r4, [sp, #8]
	bl UiWork_IsCompleteFar
	ldr r4, [sp, #8]
	cmp r0, #0
	beq .L_081029bc
	ldr r1, .L_08102bcc
	movs r3, #1
	ldr r2, [r1, #4]
	ands r2, r3
	cmp r2, #0
	bne .L_081029fc
	adds r7, r1, #0
	movs r6, #1
.L_081029e2:
	movs r0, #2
	movs r1, #146
	str r4, [sp, #8]
	bl Func_080f8a44
	movs r0, #1
	bl WaitFrames
	ldr r3, [r7, #4]
	ldr r4, [sp, #8]
	ands r3, r6
	cmp r3, #0
	beq .L_081029e2
.L_081029fc:
	movs r1, #1
	adds r0, r5, #0
	str r4, [sp, #8]
	bl UiWork_FinalizeFar
	mov r0, r10
	bl Func_08101c0c
	movs r0, #1
	bl WaitFrames
	bl Func_08100e5c
	movs r1, #9
	movs r2, #9
	movs r3, #1
	adds r0, #6
	bl UiText_OpenMessageWindowFar
	adds r5, r0, #0
	b .L_08102a2e
.L_08102a26:
	movs r0, #1
	str r4, [sp, #8]
	bl WaitFrames
.L_08102a2e:
	ldr r4, [sp, #8]
	str r4, [sp, #8]
	bl UiWork_IsCompleteFar
	ldr r4, [sp, #8]
	cmp r0, #0
	beq .L_08102a26
	ldr r1, .L_08102bcc
	movs r3, #1
	ldr r2, [r1, #4]
	ands r2, r3
	cmp r2, #0
	bne .L_08102a66
	adds r7, r1, #0
	movs r6, #1
.L_08102a4c:
	movs r0, #2
	movs r1, #146
	str r4, [sp, #8]
	bl Func_080f8a44
	movs r0, #1
	bl WaitFrames
	ldr r3, [r7, #4]
	ldr r4, [sp, #8]
	ands r3, r6
	cmp r3, #0
	beq .L_08102a4c
.L_08102a66:
	movs r1, #1
	adds r0, r5, #0
	str r4, [sp, #8]
	bl UiWork_FinalizeFar
	mov r0, r10
	bl Func_08101c0c
	movs r0, #1
	bl WaitFrames
	movs r2, #132
	lsls r2, r2, #6
	adds r2, #40
	add r2, r10
	movs r3, #0
	str r3, [r2]
	movs r2, #132
	lsls r2, r2, #6
	adds r2, #44
	add r2, r10
	movs r3, #16
.L_08102a92:
	str r3, [r2]
	ldr r4, [sp, #8]
	b .L_08102b06
.L_08102a98:
	movs r2, #132
	lsls r2, r2, #6
	adds r2, #40
	add r2, r10
	ldr r3, [r2]
	cmp r3, #90
	bne .L_08102b06
	movs r3, #1
	str r3, [sp, #16]
	movs r3, #0
	str r3, [r2]
	movs r2, #132
	lsls r2, r2, #6
	adds r2, #44
	add r2, r10
	movs r3, #21
	movs r4, #1
	str r3, [r2]
	b .L_08102b06
.L_08102abe:
	movs r2, #132
	lsls r2, r2, #6
	adds r2, #40
	add r2, r10
	ldr r3, [r2]
	cmp r3, #90
	bne .L_08102b06
	movs r3, #0
	movs r4, #32
	str r4, [sp, #16]
	str r3, [r2]
	movs r2, #132
	lsls r2, r2, #6
	adds r2, #44
	add r2, r10
	movs r3, #22
	str r3, [r2]
	b .L_08102b06
.L_08102ae2:
	movs r2, #132
	lsls r2, r2, #6
	adds r2, #40
	add r2, r10
	ldr r3, [r2]
	cmp r3, #60
	bne .L_08102b06
	movs r3, #0
	str r3, [r2]
	movs r2, #132
	lsls r2, r2, #6
	adds r2, #44
	add r2, r10
	movs r3, #25
	movs r7, #2
	str r3, [r2]
.L_08102b02:
	str r7, [sp, #16]
	movs r4, #2
.L_08102b06:
	ldr r0, [sp, #84]
	cmp r0, #0
	bne .L_08102bd0
	movs r3, #128
	lsls r3, r3, #1
	ands r3, r4
	cmp r3, #0
	beq .L_08102bac
	ldr r1, [sp, #60]
	movs r2, #1
	negs r2, r2
	cmp r1, r2
	bne .L_08102b2a
	movs r0, #114
	bl Audio_PlayCue
	bl .L_08102188
.L_08102b2a:
	movs r3, #1
	str r3, [sp, #40]
	ldr r7, [sp, #84]
	ldr r3, .L_08102bcc
	str r7, [r3, #28]
	ldr r0, [sp, #20]
	cmp r0, #0
	beq .L_08102b4c
	cmp r0, #1
	beq .L_08102b6a
	movs r0, #114
	bl Audio_PlayCue
	movs r1, #1
	str r1, [sp, #68]
	bl .L_08102188
.L_08102b4c:
	movs r0, #139
	str r4, [sp, #8]
	bl Audio_PlayCue
	ldr r1, [sp, #28]
	ldr r2, [sp, #24]
	ldr r0, [sp, #32]
	bl Djinn_ActivateFar
	ldr r0, [sp, #32]
	ldr r1, [sp, #28]
	ldr r2, [sp, #24]
	bl Trade_RemoveOfferFar
	b .L_08102b86
.L_08102b6a:
	movs r0, #175
	str r4, [sp, #8]
	bl Audio_PlayCue
	ldr r1, [sp, #28]
	ldr r2, [sp, #24]
	ldr r0, [sp, #32]
	bl Djinn_DeactivateFar
	ldr r0, [sp, #32]
	ldr r1, [sp, #28]
	ldr r2, [sp, #24]
	bl Trade_AddOfferFar
.L_08102b86:
	ldr r4, [sp, #8]
	ldr r0, [sp, #32]
	str r4, [sp, #8]
	bl Owner_RecalculateStatsFar
	mov r1, r11
	ldr r4, [sp, #8]
	cmp r1, #0
	bge .L_08102b9a
	adds r1, #3
.L_08102b9a:
	asrs r1, r1, #2
	lsls r1, r1, #2
	mov r0, r10
	str r4, [sp, #8]
	bl Func_08101ac8
	movs r2, #1
	str r2, [sp, #68]
	ldr r4, [sp, #8]
.L_08102bac:
	ldr r3, [sp, #84]
	cmp r3, #0
	bne .L_08102bd0
	movs r3, #128
	lsls r3, r3, #2
	ands r3, r4
	cmp r3, #0
	beq .L_08102bd0
	movs r0, #112
	movs r5, #7
	bl Audio_PlayCue
	b .L_08102fd4
	.2byte 0x0000
.L_08102bc8:
	.4byte gPartyState
.L_08102bcc:
	.4byte gInput
.L_08102bd0:
	movs r3, #1
	ands r3, r4
	cmp r3, #0
	bne .L_08102be8
	ldr r7, [sp, #84]
	cmp r7, #1
	bne .L_08102c6c
	movs r3, #128
	lsls r3, r3, #1
	ands r3, r4
	cmp r3, #0
	beq .L_08102c6c
.L_08102be8:
	ldr r0, [sp, #60]
	movs r1, #1
	negs r1, r1
	cmp r0, r1
	beq .L_08102c24
	ldr r2, [sp, #20]
	cmp r2, #2
	bne .L_08102c24
	movs r0, #114
	bl Audio_PlayCue
	ldr r0, [sp, #72]
	bl RenderOutput_ClearListFar
	movs r3, #104
	str r3, [sp, #0]
	ldr r0, [sp, #72]
	movs r1, #0
	movs r2, #96
	movs r3, #224
	bl UiWindow_ClearInteriorTilesFar
	ldr r0, .L_08102f1c
	ldr r1, [sp, #72]
	movs r2, #0
	movs r3, #96
	bl UiText_DrawCharacterAtOffsetFar
	bl .L_08102188
.L_08102c24:
	ldr r3, [sp, #84]
	movs r5, #1
	cmp r3, #1
	bne .L_08102c64
	ldr r4, [sp, #80]
	movs r3, #28
	ldrsb r3, [r4, r3]
	cmp r11, r3
	bne .L_08102c4e
	movs r7, #182
	lsls r7, r7, #1
	adds r3, r4, r7
	ldrh r2, [r3]
	movs r3, #128
	lsls r3, r3, #8
	ands r3, r2
	movs r5, #2
	cmp r3, #0
	bne .L_08102c64
	movs r5, #1
	b .L_08102c64
.L_08102c4e:
	ldr r0, [sp, #12]
	mov r1, r11
	ldrb r3, [r0, r1]
	ldr r2, [sp, #84]
	ands r2, r3
	negs r3, r2
	orrs r3, r2
	lsrs r3, r3, #31
	adds r5, r3, #0
	movs r3, #4
	subs r5, r3, r5
.L_08102c64:
	movs r0, #112
	bl Audio_PlayCue
	b .L_08102fd4
.L_08102c6c:
	movs r3, #8
	ands r3, r4
	cmp r3, #0
	beq .L_08102c7a
	movs r0, #113
	movs r5, #2
	b .L_08102c86
.L_08102c7a:
	movs r3, #2
	ands r3, r4
	cmp r3, #0
	beq .L_08102c8e
	movs r0, #113
	movs r5, #1
.L_08102c86:
	bl Audio_PlayCue
	negs r5, r5
	b .L_08102fd4
.L_08102c8e:
	ldr r2, [sp, #84]
	cmp r2, #0
	beq .L_08102c96
	b .L_08102db4
.L_08102c96:
	movs r3, #4
	ands r3, r4
	cmp r3, #0
	bne .L_08102ca0
	b .L_08102db4
.L_08102ca0:
	ldr r3, [sp, #40]
	cmp r3, #0
	beq .L_08102d9e
	ldr r4, [sp, #36]
	movs r3, #1
	eors r4, r3
	str r4, [sp, #36]
	cmp r4, #0
	beq .L_08102cba
	movs r0, #139
	bl Audio_PlayCue
	b .L_08102cc0
.L_08102cba:
	movs r0, #175
	bl Audio_PlayCue
.L_08102cc0:
	ldr r0, [sp, #80]
	movs r1, #139
	lsls r1, r1, #1
	adds r1, #255
	adds r3, r0, r1
	ldrb r3, [r3]
	movs r7, #0
	mov r9, r7
	cmp r9, r3
	bge .L_08102d86
	movs r2, #160
	add r2, r10
	mov r8, r2
.L_08102cda:
	movs r3, #0
	str r3, [sp, #64]
	mov r4, r8
	movs r3, #0
	ldrsb r3, [r4, r3]
	movs r7, #0
	cmp r7, r3
	bge .L_08102d70
	mov r0, r9
	lsls r3, r0, #2
	add r3, r9
	lsls r3, r3, #2
	mov r1, r10
	adds r4, r3, r1
.L_08102cf6:
	ldrh r0, [r4]
	movs r3, #240
	lsls r3, r3, #4
	ands r3, r0
	lsrs r6, r3, #8
	movs r3, #224
	adds r4, #2
	ands r3, r0
	movs r7, #31
	str r4, [sp, #8]
	lsrs r5, r3, #5
	ands r7, r0
	bl Func_08101a04
	ldr r4, [sp, #8]
	cmp r0, #0
	beq .L_08102d1e
	cmp r0, #1
	beq .L_08102d3c
	b .L_08102d60
.L_08102d1e:
	ldr r2, [sp, #36]
	cmp r2, #0
	beq .L_08102d60
	adds r1, r5, #0
	adds r2, r7, #0
	adds r0, r6, #0
	str r4, [sp, #8]
	bl Djinn_ActivateFar
	adds r1, r5, #0
	adds r2, r7, #0
	adds r0, r6, #0
	bl Trade_RemoveOfferFar
	b .L_08102d58
.L_08102d3c:
	ldr r3, [sp, #36]
	cmp r3, #0
	bne .L_08102d60
	adds r1, r5, #0
	adds r2, r7, #0
	adds r0, r6, #0
	str r4, [sp, #8]
	bl Djinn_DeactivateFar
	adds r1, r5, #0
	adds r2, r7, #0
	adds r0, r6, #0
	bl Trade_AddOfferFar
.L_08102d58:
	adds r0, r6, #0
	bl Owner_RecalculateStatsFar
	ldr r4, [sp, #8]
.L_08102d60:
	ldr r7, [sp, #64]
	mov r0, r8
	adds r7, #1
	str r7, [sp, #64]
	movs r3, #0
	ldrsb r3, [r0, r3]
	cmp r7, r3
	blt .L_08102cf6
.L_08102d70:
	ldr r2, [sp, #80]
	movs r4, #139
	lsls r4, r4, #1
	adds r4, #255
	adds r3, r2, r4
	ldrb r3, [r3]
	movs r1, #1
	add r9, r1
	add r8, r1
	cmp r9, r3
	blt .L_08102cda
.L_08102d86:
	mov r1, r11
	cmp r1, #0
	bge .L_08102d8e
	adds r1, #3
.L_08102d8e:
	asrs r1, r1, #2
	lsls r1, r1, #2
	mov r0, r10
	bl Func_08101ac8
	movs r7, #1
	str r7, [sp, #68]
	b .L_08102db4
.L_08102d9e:
	ldr r0, [sp, #72]
	mov r1, r11
	ldr r2, [sp, #52]
	movs r3, #0
	bl Func_08101c40
	movs r0, #112
	movs r5, #10
	bl Audio_PlayCue
	b .L_08102fd4
.L_08102db4:
	ldr r0, [sp, #16]
	movs r3, #64
	ands r3, r0
	cmp r3, #0
	beq .L_08102e4c
	ldr r1, [sp, #12]
	mov r3, r11
	ldrb r2, [r1, r3]
	movs r3, #4
	ands r3, r2
	cmp r3, #0
	beq .L_08102dd0
	bl .L_08102188
.L_08102dd0:
	mov r5, r11
	adds r5, #160
	mov r4, r10
	ldrsb r3, [r4, r5]
	cmp r3, #0
	bne .L_08102de0
	bl .L_08102188
.L_08102de0:
	ldr r0, [sp, #72]
	mov r1, r11
	ldr r2, [sp, #52]
	movs r3, #0
	bl Func_08101c40
	movs r0, #111
	bl Audio_PlayCue
	ldr r7, [sp, #12]
	mov r0, r11
	ldrb r2, [r7, r0]
	movs r3, #1
	ands r3, r2
	movs r1, #1
	cmp r3, #0
	beq .L_08102e10
	movs r3, #2
	negs r3, r3
	ands r3, r2
	movs r1, #0
	strb r3, [r7, r0]
	str r1, [sp, #52]
	b .L_08102e30
.L_08102e10:
	ldr r3, [sp, #52]
	cmp r3, #0
	bne .L_08102e30
	movs r3, #2
	ands r3, r2
	cmp r3, #0
	beq .L_08102e30
	ldr r4, [sp, #12]
	adds r3, r2, #0
	orrs r3, r1
	mov r7, r11
	movs r0, #2
	strb r3, [r4, r7]
	str r0, [sp, #68]
	bl .L_08102188
.L_08102e30:
	ldr r1, [sp, #52]
	mov r2, r10
	subs r1, #1
	str r1, [sp, #52]
	ldr r0, [sp, #52]
	ldrsb r1, [r2, r5]
	bl Func_08100e28
	movs r3, #2
	str r0, [sp, #52]
	str r0, [sp, #48]
	str r3, [sp, #68]
	bl .L_08102188
.L_08102e4c:
	ldr r4, [sp, #16]
	movs r3, #128
	ands r3, r4
	cmp r3, #0
	beq .L_08102edc
	ldr r7, [sp, #12]
	mov r0, r11
	ldrb r2, [r7, r0]
	movs r3, #4
	ands r3, r2
	cmp r3, #0
	beq .L_08102e68
	bl .L_08102188
.L_08102e68:
	mov r5, r11
	adds r5, #160
	mov r1, r10
	ldrsb r3, [r1, r5]
	cmp r3, #0
	bne .L_08102e78
	bl .L_08102188
.L_08102e78:
	ldr r0, [sp, #72]
	ldr r2, [sp, #52]
	mov r1, r11
	movs r3, #0
	bl Func_08101c40
	movs r0, #111
	bl Audio_PlayCue
	ldr r2, [sp, #52]
	mov r3, r10
	adds r2, #1
	str r2, [sp, #52]
	adds r0, r2, #0
	ldrsb r1, [r3, r5]
	bl Func_08100e28
	mov r4, r11
	str r0, [sp, #52]
	ldrb r2, [r7, r4]
	movs r3, #1
	ands r3, r2
	movs r1, #1
	cmp r3, #0
	beq .L_08102eb8
	movs r3, #2
	negs r3, r3
	ands r3, r2
	strb r3, [r7, r4]
	movs r7, #0
	str r7, [sp, #52]
	b .L_08102ed0
.L_08102eb8:
	ldr r0, [sp, #52]
	cmp r0, #0
	bne .L_08102ed0
	movs r3, #2
	ands r3, r2
	cmp r3, #0
	beq .L_08102ed0
	adds r3, r2, #0
	orrs r3, r1
	ldr r1, [sp, #12]
	mov r2, r11
	strb r3, [r1, r2]
.L_08102ed0:
	ldr r3, [sp, #52]
	movs r4, #2
	str r3, [sp, #48]
	str r4, [sp, #68]
	bl .L_08102188
.L_08102edc:
	ldr r7, [sp, #16]
	movs r3, #48
	ands r3, r7
	cmp r3, #0
	bne .L_08102eea
	bl .L_08102188
.L_08102eea:
	movs r0, #111
	bl Audio_PlayCue
	ldr r0, [sp, #12]
	mov r1, r11
	ldrb r2, [r0, r1]
	movs r3, #4
	ands r3, r2
	mov r8, r11
	cmp r3, #0
	bne .L_08102f0a
	ldr r0, [sp, #72]
	ldr r2, [sp, #52]
	movs r3, #0
	bl Func_08101c40
.L_08102f0a:
	ldr r2, [sp, #16]
	movs r3, #32
	ands r3, r2
	cmp r3, #0
	beq .L_08102f20
	movs r3, #1
	negs r3, r3
	add r11, r3
	b .L_08102f24
.L_08102f1c:
	.4byte 0x000010ef
.L_08102f20:
	movs r4, #1
	add r11, r4
.L_08102f24:
	mov r7, r11
	cmp r7, #0
	bge .L_08102f2e
	movs r0, #0
	mov r11, r0
.L_08102f2e:
	ldr r1, [sp, #80]
	movs r3, #139
	lsls r3, r3, #1
	adds r3, #255
	adds r2, r1, r3
	ldrb r1, [r2]
	subs r3, r1, #1
	cmp r11, r3
	ble .L_08102f42
	mov r11, r3
.L_08102f42:
	ldr r4, [sp, #84]
	cmp r4, #0
	bne .L_08102f8c
	adds r0, r2, #0
	ldr r2, [sp, #16]
	mov r12, r1
	movs r1, #32
	ands r1, r2
	ldr r2, [sp, #12]
	movs r7, #0
	mov r9, r7
	add r2, r11
.L_08102f5a:
	cmp r9, r12
	bge .L_08102f8c
	movs r3, #0
	ldrsb r3, [r2, r3]
	cmp r3, #4
	bne .L_08102f8c
	cmp r1, #0
	beq .L_08102f74
	movs r3, #1
	negs r3, r3
	subs r2, #1
	add r11, r3
	b .L_08102f7a
.L_08102f74:
	movs r4, #1
	adds r2, #1
	add r11, r4
.L_08102f7a:
	mov r7, r11
	cmp r7, #0
	blt .L_08102f8a
	ldrb r3, [r0]
	movs r4, #1
	add r9, r4
	cmp r11, r3
	blt .L_08102f5a
.L_08102f8a:
	mov r11, r8
.L_08102f8c:
	ldr r7, [sp, #48]
	mov r3, r11
	str r7, [sp, #52]
	adds r3, #160
	mov r0, r10
	ldrsb r5, [r0, r3]
	cmp r5, #0
	bne .L_08102f9e
	movs r5, #1
.L_08102f9e:
	ldr r0, [sp, #52]
	adds r1, r5, #0
	bl Func_08100e28
	movs r1, #2
	mov r3, r8
	str r0, [sp, #52]
	str r1, [sp, #68]
	cmp r3, #0
	bge .L_08102fb4
	adds r3, #3
.L_08102fb4:
	asrs r3, r3, #2
	lsls r2, r3, #2
	mov r3, r11
	cmp r3, #0
	bge .L_08102fc0
	adds r3, #3
.L_08102fc0:
	asrs r3, r3, #2
	lsls r3, r3, #2
	cmp r2, r3
	bne .L_08102fcc
	bl .L_08102188
.L_08102fcc:
	movs r2, #6
	str r2, [sp, #68]
	bl .L_08102188
.L_08102fd4:
	ldr r3, [sp, #84]
	ldr r4, [sp, #80]
	adds r3, #28
	mov r7, r11
	strb r7, [r4, r3]
	ldr r0, [sp, #60]
	movs r1, #1
	negs r1, r1
	cmp r0, r1
	beq .L_08103032
	ldr r2, [sp, #56]
	movs r3, #182
	lsls r3, r3, #1
	mov r4, r11
	adds r1, r2, r3
	lsls r3, r4, #2
	add r3, r11
	lsls r3, r3, #1
	adds r3, r3, r0
	lsls r3, r3, #1
	mov r7, r10
	ldrh r2, [r7, r3]
	ldr r0, [sp, #80]
	movs r4, #150
	strh r2, [r0, r1]
	ldr r3, [sp, #84]
	lsls r4, r4, #2
	adds r1, r3, r4
	movs r3, #31
	ands r3, r2
	strb r3, [r0, r1]
	ldr r0, [sp, #80]
	movs r3, #224
	ands r3, r2
	adds r0, #2
	lsrs r3, r3, #5
	strb r3, [r0, r1]
	movs r3, #240
	ldr r7, [sp, #84]
	lsls r3, r3, #4
	ands r3, r2
	movs r0, #151
	ldr r2, [sp, #80]
	lsls r0, r0, #2
	adds r1, r7, r0
	lsrs r3, r3, #8
	strb r3, [r2, r1]
.L_08103032:
	ldr r0, [sp, #52]
	ldr r4, [sp, #56]
	lsls r2, r0, #2
	ldr r1, [sp, #80]
	adds r2, r2, r0
	movs r7, #180
	lsls r7, r7, #1
	lsls r2, r2, #1
	adds r3, r4, r7
	add r2, r11
	strh r2, [r1, r3]
	movs r1, #128
	ldr r0, [sp, #84]
	lsls r1, r1, #7
	bl Func_0810532c
	adds r0, r5, #0
	add sp, #112
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
