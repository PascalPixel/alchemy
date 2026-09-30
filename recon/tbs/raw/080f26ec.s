.syntax unified
	.thumb
	.global Func_080f26ec
	.thumb_func
Func_080f26ec:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #20
	str r0, [sp, #12]
	movs r0, #0
	str r0, [sp, #8]
	ldr r5, .L_080f284c
	ldrb r3, [r5]
	movs r1, #224
	movs r0, #43
	str r3, [sp, #4]
	bl Runtime_AllocateHeapBlock
	mov r8, r0
	bl Bg0_ClearTilemap
	bl Resource_InitializeTable
	movs r0, #1
	bl WaitFrames
	bl Scheduler_ResetTaskTable
	add r1, sp, #8
	ldrb r1, [r1]
	ldr r3, .L_080f2850
	adds r2, r1, #0
	strb r1, [r3]
	strb r2, [r5]
	bl Unnamed_080f24a0
	bl TitlePalette_InitializeBuffers
	movs r1, #0
	movs r0, #2
	bl Graphics_TransformSmallPalette
	ldr r1, .L_080f2854
	ldr r0, .L_080f2858
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_080f276e
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r1
	strh r2, [r1]
	ldr r2, .L_080f285c
	adds r3, #4
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_080f276e:
	strh r4, [r0]
	movs r0, #60
	bl Graphics_UpdatePaletteInterpolation
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_080f2860
	bl Scheduler_AddOrUpdateCallback
	ldr r0, .L_080f2864
	movs r1, #18
	movs r3, #1
	add r1, sp
	mov r9, r3
	mov r10, r0
	mov r11, r1
.L_080f278e:
	mov r2, r8
	ldr r1, [r2, #8]
	adds r3, r1, #0
	subs r3, #21
	cmp r3, #217
	bhi .L_080f27b2
	ldr r3, .L_080f2868
	ldr r3, [r3]
	movs r2, #9
	ands r3, r2
	cmp r3, #0
	beq .L_080f27b2
	mov r3, r9
	mov r0, r8
	str r3, [r0, #16]
	movs r3, #239
	str r3, [r0, #8]
	movs r1, #239
.L_080f27b2:
	movs r2, #139
	adds r3, r1, #1
	lsls r2, r2, #1
	mov r1, r8
	str r3, [r1, #8]
	cmp r3, r2
	ble .L_080f27c2
	b .L_080f28fa
.L_080f27c2:
	ldr r5, [r1, #12]
	movs r1, #3
	adds r0, r5, #0
	bl Math_Mod
	cmp r0, #0
	bne .L_080f280a
	mov r0, r10
	ldrh r3, [r0, #6]
	ldr r1, .L_080f286c
	mov r2, r10
	adds r3, r3, r1
	strh r3, [r2, #6]
	ldrh r3, [r2, #6]
	movs r2, #7
	ands r3, r2
	cmp r3, #0
	bne .L_080f280a
	mov r0, r8
	ldr r3, [r0]
	lsls r2, r3, #4
	subs r2, r2, r3
	ldr r0, .L_080f2870
	ldr r1, .L_080f2874
	lsls r2, r2, #6
	subs r0, r0, r2
	subs r1, r1, r2
	ldr r3, .L_080f2878
	ldr r2, .L_080f287c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	mov r1, r8
	ldr r3, [r1]
	adds r3, #1
	str r3, [r1]
	ldr r5, [r1, #12]
.L_080f280a:
	mov r3, r9
	ands r3, r5
	cmp r3, #0
	bne .L_080f28ec
	mov r2, r10
	ldrh r3, [r2, #10]
	ldr r0, .L_080f286c
	mov r1, r10
	adds r3, r3, r0
	strh r3, [r1, #10]
	ldrh r2, [r1, #10]
	movs r3, #7
	ands r2, r3
	cmp r2, #0
	bne .L_080f28ec
	mov r0, r8
	ldr r3, [r0, #4]
	lsls r1, r3, #3
	cmp r1, #24
	bgt .L_080f288c
	lsls r1, r3, #4
	subs r1, r1, r3
	ldr r2, .L_080f2880
	ldr r0, .L_080f2884
	lsls r1, r1, #7
	subs r0, r0, r1
	ldr r3, .L_080f2878
	subs r1, r2, r1
	ldr r2, .L_080f2888
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	b .L_080f28e4
	.2byte 0x0000
.L_080f284c:
	.4byte Data_03001f58
.L_080f2850:
	.4byte gOamCopyEnabled
.L_080f2854:
	.4byte gIoWriteQueue
.L_080f2858:
	.4byte 0x04000208
.L_080f285c:
	.4byte 0x0000f740
.L_080f2860:
	.4byte Func_080f2028
.L_080f2864:
	.4byte gBgScroll
.L_080f2868:
	.4byte gKeyState
.L_080f286c:
	.4byte 0x0000ffff
.L_080f2870:
	.4byte gMapCellBuffer + 0x2580
.L_080f2874:
	.4byte 0x06004b00
.L_080f2878:
	.4byte 0x040000d4
.L_080f287c:
	.4byte 0x800001e0
.L_080f2880:
	.4byte 0x0600e4c0
.L_080f2884:
	.4byte gActorSpriteSlots + 0x19c0
.L_080f2888:
	.4byte 0x800003c0
.L_080f288c:
	mov r3, r11
	movs r0, #160
	strh r2, [r3]
	subs r0, r0, r1
	movs r1, #160
	bl Math_Mod
	lsls r1, r0, #4
	subs r1, r1, r0
	ldr r2, .L_080f28d4
	lsls r1, r1, #4
	adds r1, r1, r2
	ldr r3, .L_080f28d8
	mov r0, r11
	ldr r2, .L_080f28dc
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r1, .L_080f28e0
	ldr r3, .L_080f28d0
	movs r7, #0
.L_080f28b4:
	movs r6, #0
.L_080f28b6:
	adds r6, #1
	strh r3, [r1]
	adds r1, #2
	cmp r6, #29
	bls .L_080f28b6
	strh r3, [r1]
	adds r7, #1
	adds r1, #2
	strh r3, [r1]
	adds r1, #2
	cmp r7, #4
	bls .L_080f28b4
	b .L_080f28e4
.L_080f28d0:
	.4byte 0x0000013b
.L_080f28d4:
	.4byte 0x06004ec0
.L_080f28d8:
	.4byte 0x040000d4
.L_080f28dc:
	.4byte 0x810003c0
.L_080f28e0:
	.4byte 0x0600f6c0
.L_080f28e4:
	mov r0, r8
	ldr r3, [r0, #4]
	adds r3, #1
	str r3, [r0, #4]
.L_080f28ec:
	mov r1, r8
	ldr r3, [r1, #8]
	cmp r3, #239
	bne .L_080f29bc
	mov r2, r9
	str r2, [r1, #16]
	b .L_080f29bc
.L_080f28fa:
	ldr r0, .L_080f2918
	cmp r3, r0
	bne .L_080f2908
	movs r3, #2
	mov r1, r8
	str r3, [r1, #16]
	b .L_080f29bc
.L_080f2908:
	ldr r2, .L_080f291c
	cmp r3, r2
	bne .L_080f2920
	movs r3, #0
	mov r0, r8
	str r3, [r0, #16]
	b .L_080f29bc
	.2byte 0x0000
.L_080f2918:
	.4byte 0x00000119
.L_080f291c:
	.4byte 0x00000121
.L_080f2920:
	movs r1, #140
	lsls r1, r1, #1
	cmp r3, r1
	bne .L_080f29bc
	movs r0, #1
	bl Blend_SetBrightenTarget16
	ldr r0, .L_080f2984
	bl Scheduler_RemoveCallback
	ldr r3, .L_080f2988
	mov r2, r9
	strb r2, [r3]
	movs r0, #1
	bl WaitFrames
	ldr r2, .L_080f298c
	ldr r3, .L_080f2978
	strh r3, [r2]
	ldr r3, .L_080f297c
	subs r2, #12
	strh r3, [r2]
	ldr r3, .L_080f2980
	mov r0, r10
	strh r3, [r0, #10]
	ldr r0, .L_080f2990
	bl Resource_GetTableEntry
	movs r1, #160
	adds r4, r0, #0
	ldr r3, .L_080f2994
	lsls r1, r1, #19
	ldr r2, .L_080f2998
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #128
	lsls r2, r2, #2
	ldr r5, .L_080f299c
	movs r3, #160
	adds r4, r4, r2
	lsls r3, r3, #19
	strh r1, [r3]
	adds r0, r4, #0
	b .L_080f29a0
.L_080f2978:
	.4byte 0x00000681
.L_080f297c:
	.4byte 0x00001440
.L_080f2980:
	.4byte 0x00000000
.L_080f2984:
	.4byte Func_080f2028
.L_080f2988:
	.4byte gOamCopyEnabled
.L_080f298c:
	.4byte 0x0400000c
.L_080f2990:
	.4byte 0x00000016
.L_080f2994:
	.4byte 0x040000d4
.L_080f2998:
	.4byte 0x84000078
.L_080f299c:
	.4byte gMapCellBuffer
.L_080f29a0:
	adds r1, r5, #0
	bl Resource_DecodeByteLz
	ldr r3, .L_080f2a70
	adds r0, r5, #0
	ldr r1, .L_080f2a74
	ldr r2, .L_080f2a78
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #128
	ldr r1, .L_080f2a7c
	lsls r3, r3, #1
	movs r7, #0
	b .L_080f29ea
.L_080f29bc:
	movs r0, #1
	bl WaitFrames
	b .L_080f278e
.L_080f29c4:
	movs r6, #0
.L_080f29c6:
	adds r2, r3, #0
	movs r0, #128
	lsls r3, r2, #16
	lsls r0, r0, #9
	adds r3, r3, r0
	adds r6, #1
	strh r2, [r1]
	asrs r3, r3, #16
	adds r1, #2
	cmp r6, #29
	bls .L_080f29c6
	ldr r2, .L_080f2a80
	ldr r0, .L_080f2a80
	strh r2, [r1]
	adds r1, #2
	strh r0, [r1]
	adds r7, #1
	adds r1, #2
.L_080f29ea:
	cmp r7, #19
	bls .L_080f29c4
	bl Ui_LoadWindowGraphics
	bl Bg0_ClearTilemap
	ldr r1, [sp, #12]
	cmp r1, #0
	beq .L_080f2a40
	movs r1, #128
	lsls r1, r1, #3
	movs r0, #14
	bl Runtime_AllocateBlock
	adds r6, r0, #0
	adds r1, r6, #0
	ldr r0, .L_080f2a84
	bl Resource_DecodeByteLz
	mov r5, r8
	adds r5, #128
	movs r7, #0
.L_080f2a16:
	bl Resource_FindFreeEntry
	lsls r2, r7, #8
	lsrs r2, r2, #1
	adds r2, r6, r2
	movs r1, #128
	bl VramBlock_LoadCached
	adds r2, r5, #0
	movs r3, #0
	stmia r2!, {r3}
	ldr r3, .L_080f2a88
	stmia r2!, {r3}
	adds r7, #1
	adds r5, #12
	str r0, [r2]
	cmp r7, #4
	bls .L_080f2a16
	movs r0, #14
	bl Runtime_ReleaseHeapBlock
.L_080f2a40:
	movs r0, #30
	bl Blend_SetBrightenTarget0
	bl Blend_WaitForTransition
	ldr r3, .L_080f2a6c
	movs r2, #128
	lsls r2, r2, #19
	strh r3, [r2]
	ldr r3, [sp, #12]
	movs r2, #150
	lsls r2, r2, #1
	mov r9, r2
	cmp r3, #0
	beq .L_080f2a8c
	movs r0, #225
	lsls r0, r0, #4
	mov r9, r0
	b .L_080f2a8c
.L_080f2a66:
	movs r1, #1
	str r1, [sp, #8]
	b .L_080f2b1c
.L_080f2a6c:
	.4byte 0x00001540
.L_080f2a70:
	.4byte 0x040000d4
.L_080f2a74:
	.4byte 0x06004000
.L_080f2a78:
	.4byte 0x80004b00
.L_080f2a7c:
	.4byte 0x06003000
.L_080f2a80:
	.4byte 0x000001ff
.L_080f2a84:
	.4byte Data_080f38bc
.L_080f2a88:
	.4byte 0x40004000
.L_080f2a8c:
	movs r7, #0
	cmp r7, r9
	bcs .L_080f2b1c
	ldr r2, .L_080f2ae4
	mov r11, r2
.L_080f2a96:
	ldr r3, [sp, #12]
	cmp r3, #0
	beq .L_080f2b04
	ldr r0, .L_080f2ae8
	mov r5, r8
	adds r5, #128
	movs r6, #0
	movs r4, #80
	mov r10, r0
.L_080f2aa8:
	ldr r3, .L_080f2ae0
	adds r2, r4, #0
	ands r2, r3
	ldrh r3, [r5, #6]
	mov r1, r10
	ands r3, r1
	orrs r3, r2
	strh r3, [r5, #6]
	movs r3, #124
	strb r3, [r5, #4]
	adds r0, r5, #0
	movs r1, #0
	str r4, [sp, #0]
	bl Runtime_PushSlotEntry
	ldr r4, [sp, #0]
	adds r6, #1
	adds r4, #32
	adds r5, #12
	cmp r6, #2
	bls .L_080f2aa8
	movs r1, #60
	adds r0, r7, #0
	bl IwramUnsignedRemainderEntry
	ldr r2, .L_080f2aec
	b .L_080f2af0
	.2byte 0x0000
.L_080f2ae0:
	.4byte 0x000001ff
.L_080f2ae4:
	.4byte 0x04000052
.L_080f2ae8:
	.4byte 0xfffffe00
.L_080f2aec:
	.4byte Data_080f39b1
.L_080f2af0:
	ldr r3, .L_080f2b28
	ldrb r1, [r2, r0]
	ldr r2, .L_080f2b2c
	strh r3, [r2]
	movs r3, #16
	subs r3, r3, r1
	lsls r3, r3, #8
	adds r3, r3, r1
	mov r0, r11
	strh r3, [r0]
.L_080f2b04:
	ldr r3, .L_080f2b30
	ldr r3, [r3]
	movs r2, #9
	ands r3, r2
	cmp r3, #0
	bne .L_080f2a66
	movs r0, #1
	adds r7, #1
	bl WaitFrames
	cmp r7, r9
	bcc .L_080f2a96
.L_080f2b1c:
	add r1, sp, #4
	ldrb r1, [r1]
	ldr r3, .L_080f2b34
	movs r0, #43
	b .L_080f2b38
	.2byte 0x0000
.L_080f2b28:
	.4byte 0x00002f50
.L_080f2b2c:
	.4byte 0x04000050
.L_080f2b30:
	.4byte gKeyState
.L_080f2b34:
	.4byte Data_03001f58
.L_080f2b38:
	strb r1, [r3]
	bl Runtime_ReleaseHeapBlock
	ldr r2, .L_080f2b54
	ldr r3, .L_080f2b58
	strh r2, [r3]
	adds r3, #2
	strh r2, [r3]
	movs r0, #1
	bl WaitFrames
	ldr r0, [sp, #8]
	add sp, #20
	b .L_080f2b5c
.L_080f2b54:
	.4byte 0x00000000
.L_080f2b58:
	.4byte 0x04000050
.L_080f2b5c:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
