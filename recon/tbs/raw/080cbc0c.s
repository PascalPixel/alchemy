.syntax unified
	.thumb
	.global BattleEffect_RunTileAndPaletteAnimation
	.thumb_func
BattleEffect_RunTileAndPaletteAnimation:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	adds r5, r0, #0
	ldr r1, .L_080cbcbc
	movs r0, #39
	sub sp, #56
	bl Runtime_AllocateHeapBlock
	movs r1, #128
	str r0, [sp, #40]
	lsls r1, r1, #7
	movs r0, #40
	bl Runtime_AllocateHeapBlock
	ldr r1, .L_080cbcc0
	str r0, [sp, #36]
	movs r0, #41
	bl Runtime_AllocateHeapBlock
	ldr r3, .L_080cbcc4
	ldr r0, [r3]
	str r0, [sp, #20]
	adds r3, #140
	ldr r1, [sp, #40]
	ldr r3, [r3]
	ldr r2, .L_080cbcc8
	mov r9, r3
	adds r3, r1, r2
	str r5, [r3]
	bl Runtime_ApplyValueToWork7818
	mov r4, r9
	ldr r2, .L_080cbccc
	movs r3, #1
	str r3, [r4, #12]
	movs r3, #32
	strh r3, [r2, #6]
	ldr r7, [sp, #20]
	movs r0, #201
	lsls r0, r0, #3
	adds r3, r7, r0
	ldrh r1, [r3]
	movs r0, #1
	movs r2, #0
	bl BattlePresentation_ConfigurePaletteFadeFar
	ldr r2, .L_080cbcd0
	ldr r3, .L_080cbcac
	movs r1, #0
	strh r3, [r2]
	movs r0, #0
	movs r2, #0
	movs r3, #100
	bl BattlePres_SetupTransitionSceneFar
	ldr r3, .L_080cbcd4
	movs r5, #0
	mov r1, r9
	str r5, [r1, #12]
	ldr r2, .L_080cbcd8
	str r5, [r3]
	ldr r3, .L_080cbcdc
	str r3, [r2]
	ldr r3, .L_080cbcb0
	subs r2, #12
	strh r3, [r2]
	ldr r3, .L_080cbce0
	strh r5, [r3]
	adds r3, #2
	strh r5, [r3]
	ldr r3, .L_080cbcb4
	adds r2, #6
	strh r3, [r2]
	ldr r1, .L_080cbcb8
	b .L_080cbce4
.L_080cbcac:
	.4byte 0x00000784
.L_080cbcb0:
	.4byte 0x00000080
.L_080cbcb4:
	.4byte 0x00000100
.L_080cbcb8:
	.4byte 0x000000f0
.L_080cbcbc:
	.4byte 0x0000782c
.L_080cbcc0:
	.4byte 0x00000302
.L_080cbcc4:
	.4byte gBattleWork
.L_080cbcc8:
	.4byte 0x00007828
.L_080cbccc:
	.4byte gBgScroll
.L_080cbcd0:
	.4byte 0x0400000c
.L_080cbcd4:
	.4byte 0x04000028
.L_080cbcd8:
	.4byte 0x0400002c
.L_080cbcdc:
	.4byte 0xfffff000
.L_080cbce0:
	.4byte 0x04000022
.L_080cbce4:
	ldr r3, .L_080cbd28
	ldr r2, .L_080cbd1c
	strh r1, [r3]
	adds r3, #4
	strh r2, [r3]
	subs r3, #2
	strh r1, [r3]
	adds r3, #4
	strh r2, [r3]
	ldr r2, .L_080cbd2c
	ldr r3, .L_080cbd20
	strh r3, [r2]
	ldr r3, .L_080cbd24
	adds r2, #2
	strh r3, [r2]
	movs r4, #128
	ldr r2, .L_080cbd30
	movs r3, #128
	movs r6, #0
	lsls r3, r3, #1
	lsls r4, r4, #2
	mov r12, r6
	mov r8, r2
	mov r10, r3
	mov lr, r4
	movs r7, #0
	b .L_080cbd34
	.2byte 0x0000
.L_080cbd1c:
	.4byte 0x00001088
.L_080cbd20:
	.4byte 0x00003537
.L_080cbd24:
	.4byte 0x00003f21
.L_080cbd28:
	.4byte 0x04000040
.L_080cbd2c:
	.4byte 0x04000048
.L_080cbd30:
	.4byte 0x06003800
.L_080cbd34:
	mov r1, r10
	adds r0, r7, r1
	movs r4, #0
	lsls r1, r5, #1
.L_080cbd3c:
	adds r3, r0, #0
	orrs r3, r1
	lsls r3, r3, #16
	asrs r3, r3, #16
	mov r11, r3
	mov r3, r8
	adds r2, r6, r3
	adds r4, #1
	mov r3, r11
	strh r3, [r2]
	add r0, lr
	adds r1, #2
	adds r6, #2
	cmp r4, #8
	bne .L_080cbd3c
	movs r0, #1
	movs r4, #128
	add r12, r0
	lsls r4, r4, #5
	mov r1, r12
	adds r7, r7, r4
	adds r5, #8
	cmp r1, #16
	bne .L_080cbd34
	movs r1, #128
	ldr r5, .L_080cbdf8
	ldr r0, [sp, #36]
	lsls r1, r1, #7
	bl _call_via_r5
	movs r1, #128
	lsls r1, r1, #7
	ldr r0, .L_080cbdfc
	bl _call_via_r5
	ldr r1, .L_080cbe00
	ldr r0, .L_080cbe04
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_080cbdb0
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r1
	strh r2, [r1]
	ldr r2, .L_080cbe08
	adds r3, #4
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_080cbdb0:
	strh r4, [r0]
	ldr r2, .L_080cbe0c
	ldr r3, .L_080cbdf0
	strh r3, [r2]
	ldr r3, .L_080cbdf4
	subs r2, #2
	strh r3, [r2]
	ldr r1, [sp, #40]
	ldr r0, .L_080cbe10
	movs r2, #1
	movs r3, #1
	bl Resource_LoadAndDecompress
	movs r4, #239
	ldr r2, [sp, #40]
	lsls r4, r4, #7
	adds r3, r2, r4
	movs r5, #1
	str r5, [r3]
	ldr r7, [sp, #40]
	ldr r0, .L_080cbe14
	movs r3, #0
	adds r2, r7, r0
	movs r1, #144
	str r3, [r2]
	lsls r1, r1, #3
	ldr r0, .L_080cbe18
	bl Scheduler_AddOrUpdateCallback
	movs r1, #7
	movs r2, #7
	b .L_080cbe1c
.L_080cbdf0:
	.4byte 0x00001010
.L_080cbdf4:
	.4byte 0x00000000
.L_080cbdf8:
	.4byte IwramClearWords
.L_080cbdfc:
	.4byte 0x06004000
.L_080cbe00:
	.4byte gIoWriteQueue
.L_080cbe04:
	.4byte 0x04000208
.L_080cbe08:
	.4byte 0x00007741
.L_080cbe0c:
	.4byte 0x04000052
.L_080cbe10:
	.4byte 0x00000044
.L_080cbe14:
	.4byte 0x00007784
.L_080cbe18:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080cbe1c:
	movs r3, #3
	movs r0, #46
	str r5, [sp, #0]
	bl Unnamed_080ed408
	ldr r5, .L_080cbfac
	adds r3, r5, #0
	adds r3, #184
	ldr r3, [r3]
	str r3, [sp, #24]
	movs r3, #2
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #7
	movs r0, #47
	movs r3, #3
	bl Unnamed_080ed408
	adds r5, #188
	ldr r5, [r5]
	movs r2, #225
	movs r1, #0
	lsls r2, r2, #7
	str r5, [sp, #28]
	str r1, [sp, #12]
	ldr r4, .L_080cbfb0
	adds r1, r7, r2
	ldr r0, .L_080cbfb4
	ldr r7, .L_080cbfb8
	ldr r5, .L_080cbfbc
	movs r6, #0
.L_080cbe5a:
	ldrb r2, [r0]
	ldrb r3, [r4]
	lsls r2, r2, #16
	lsls r3, r3, #16
	str r2, [r1]
	str r3, [r1, #4]
	adds r2, r2, r7
	adds r3, r3, r5
	asrs r2, r2, #2
	asrs r3, r3, #2
	adds r6, #1
	str r2, [r1, #12]
	str r3, [r1, #16]
	adds r0, #1
	adds r4, #1
	adds r1, #28
	cmp r6, #33
	bne .L_080cbe5a
	movs r2, #240
	ldr r3, .L_080cbfc0
	ldr r1, .L_080cbfc4
	lsls r2, r2, #7
	ldr r0, .L_080cbfc8
	bl _call_via_r3
	movs r1, #240
	ldr r5, .L_080cbfcc
	lsls r1, r1, #7
	ldr r2, .L_080cbfd0
	ldr r0, .L_080cbfc8
	bl _call_via_r5
	movs r3, #1
	mov r4, r9
	str r3, [r4, #16]
	ldr r2, .L_080cbfd4
	ldr r7, [sp, #40]
	ldr r0, .L_080cbfd8
	ldrh r3, [r2, #4]
	adds r1, r7, r0
	str r3, [r1]
	ldr r3, .L_080cbfdc
	adds r1, r7, r3
	ldrh r3, [r2, #6]
	str r3, [r1]
	ldr r1, .L_080cbfe0
	movs r3, #0
	strh r3, [r2, #4]
	ldr r0, .L_080cbfe4
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_080cbee4
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r1
	strh r2, [r1]
	ldr r2, .L_080cbfe8
	adds r3, #4
	stmia r3!, {r2}
	ldr r2, .L_080cbfec
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_080cbee4:
	strh r4, [r0]
	ldr r0, .L_080cbff0
	movs r1, #128
	lsls r1, r1, #1
	ldr r2, .L_080cbff4
	bl _call_via_r5
	movs r0, #212
	bl AudioCommand_PlayFar
	ldr r5, .L_080cbff8
	ldr r4, [sp, #40]
	adds r3, r4, r5
	ldr r3, [r3]
	movs r7, #36
	ldrsh r0, [r3, r7]
	movs r3, #30
	str r3, [sp, #0]
	movs r1, #7
	movs r2, #3
	movs r3, #0
	bl ObjectGroup_UpdateMembers
	movs r0, #0
	str r0, [sp, #32]
.L_080cbf16:
	ldr r1, [sp, #32]
	cmp r1, #2
	bne .L_080cbf22
	movs r0, #212
	bl AudioCommand_PlayFar
.L_080cbf22:
	ldr r2, [sp, #32]
	cmp r2, #3
	bne .L_080cbf2e
	movs r0, #212
	bl AudioCommand_PlayFar
.L_080cbf2e:
	ldr r3, [sp, #32]
	cmp r3, #28
	bne .L_080cbf50
	ldr r4, [sp, #40]
	ldr r5, .L_080cbff8
	adds r3, r4, r5
	ldr r3, [r3]
	movs r7, #36
	ldrsh r0, [r3, r7]
	movs r3, #1
	movs r2, #0
	negs r3, r3
	str r2, [sp, #0]
	adds r1, r3, #0
	movs r2, #3
	bl ObjectGroup_UpdateMembers
.L_080cbf50:
	ldr r0, [sp, #32]
	cmp r0, #32
	bne .L_080cbf5c
	movs r0, #149
	bl AudioCommand_PlayFar
.L_080cbf5c:
	ldr r1, [sp, #32]
	cmp r1, #5
	bne .L_080cbf88
	movs r0, #145
	bl AudioCommand_PlayFar
	ldr r4, [sp, #40]
	ldr r5, .L_080cbfd8
	adds r3, r4, r5
	ldr r2, .L_080cbfd4
	ldr r3, [r3]
	strh r3, [r2, #4]
	ldr r7, [sp, #20]
	movs r0, #201
	lsls r0, r0, #3
	adds r3, r7, r0
	movs r2, #1
	ldrh r1, [r3]
	movs r0, #1
	negs r2, r2
	bl BattleBackground_LoadFar
.L_080cbf88:
	ldr r1, [sp, #32]
	cmp r1, #7
	ble .L_080cc072
	ldr r5, [sp, #20]
	ldr r7, .L_080cbffc
	ldr r2, .L_080cbff0
	ldr r3, .L_080cbfa8
	movs r4, #31
	adds r5, r5, r7
	mov lr, r2
	movs r6, #0
	mov r12, r3
	mov r8, r4
	mov r9, r5
	b .L_080cc000
	.2byte 0x0000
.L_080cbfa8:
	.4byte 0x0000001f
.L_080cbfac:
	.4byte gWorkSlot
.L_080cbfb0:
	.4byte Data_080edf88 + 0xaf
.L_080cbfb4:
	.4byte Data_080edf88 + 0x8e
.L_080cbfb8:
	.4byte 0xffe00000
.L_080cbfbc:
	.4byte 0xffc40000
.L_080cbfc0:
	.4byte IwramCopyWords
.L_080cbfc4:
	.4byte 0x06008000
.L_080cbfc8:
	.4byte gMapCellBuffer
.L_080cbfcc:
	.4byte IwramFillWords
.L_080cbfd0:
	.4byte 0x01010101
.L_080cbfd4:
	.4byte gBgScroll
.L_080cbfd8:
	.4byte 0x000077a0
.L_080cbfdc:
	.4byte 0x000077a4
.L_080cbfe0:
	.4byte gIoWriteQueue
.L_080cbfe4:
	.4byte 0x04000208
.L_080cbfe8:
	.4byte 0x00001f81
.L_080cbfec:
	.4byte 0x0400000a
.L_080cbff0:
	.4byte 0x050000c0
.L_080cbff4:
	.4byte 0x7fff7fff
.L_080cbff8:
	.4byte 0x00007828
.L_080cbffc:
	.4byte 0x00000544
.L_080cc000:
	mov r0, lr
	ldrh r3, [r0]
	mov r5, r8
	mov r4, r9
	ands r5, r3
	lsls r3, r3, #16
	mov r1, r12
	lsrs r2, r3, #21
	lsrs r0, r3, #26
	ldrh r3, [r4]
	ands r2, r1
	ands r0, r1
	mov r1, r8
	ands r1, r3
	lsls r3, r3, #16
	movs r7, #2
	mov r10, r3
	lsrs r4, r3, #21
	add r9, r7
	mov r3, r12
	mov r7, r10
	ands r4, r3
	lsrs r3, r7, #26
	mov r7, r12
	ands r3, r7
	cmp r5, r1
	bge .L_080cc03a
	adds r5, #1
	b .L_080cc040
.L_080cc03a:
	cmp r5, r1
	ble .L_080cc040
	subs r5, #1
.L_080cc040:
	cmp r2, r4
	bge .L_080cc048
	adds r2, #1
	b .L_080cc04e
.L_080cc048:
	cmp r2, r4
	ble .L_080cc04e
	subs r2, #1
.L_080cc04e:
	cmp r0, r3
	bge .L_080cc056
	adds r0, #1
	b .L_080cc05c
.L_080cc056:
	cmp r0, r3
	ble .L_080cc05c
	subs r0, #1
.L_080cc05c:
	lsls r3, r0, #10
	lsls r2, r2, #5
	orrs r3, r2
	mov r0, lr
	orrs r3, r5
	movs r1, #2
	adds r6, #1
	strh r3, [r0]
	add lr, r1
	cmp r6, #128
	bne .L_080cc000
.L_080cc072:
	ldr r2, [sp, #32]
	cmp r2, #4
	bne .L_080cc086
	movs r1, #240
	ldr r3, .L_080cc3e4
	ldr r0, .L_080cc3e8
	lsls r1, r1, #7
	ldr r2, .L_080cc3ec
	bl _call_via_r3
.L_080cc086:
	ldr r3, [sp, #32]
	cmp r3, #3
	ble .L_080cc08e
	b .L_080cc32a
.L_080cc08e:
	lsls r1, r3, #2
	adds r1, #8
	lsls r4, r3, #5
	lsls r2, r1, #5
	lsls r3, r1, #10
	ldr r0, .L_080cc3f0
	orrs r3, r2
	orrs r3, r1
	str r4, [sp, #16]
	strh r3, [r0]
	ldr r5, [sp, #12]
	cmp r5, r4
	bne .L_080cc0aa
	b .L_080cc31c
.L_080cc0aa:
	ldr r7, [sp, #12]
	movs r0, #0
	adds r1, r7, #0
	mov r9, r7
	mov r11, r0
	str r7, [sp, #8]
	cmp r1, #0
	bge .L_080cc0bc
	b .L_080cc30e
.L_080cc0bc:
	mov r4, r9
	movs r3, #96
	movs r5, #60
	mov r7, r11
	subs r0, r3, r4
	mov r2, r9
	mov r4, r11
	subs r6, r5, r7
	adds r2, #96
	mov lr, r0
	adds r4, #60
	cmp r6, #0
	bge .L_080cc0d8
	movs r6, #0
.L_080cc0d8:
	cmp r4, #119
	ble .L_080cc0de
	movs r4, #119
.L_080cc0de:
	cmp r0, #0
	bge .L_080cc0e4
	movs r0, #0
.L_080cc0e4:
	cmp r2, #255
	ble .L_080cc0ea
	movs r2, #255
.L_080cc0ea:
	movs r5, #7
	ands r5, r2
	adds r3, r2, #0
	cmp r2, #0
	bge .L_080cc0f6
	adds r3, r2, #7
.L_080cc0f6:
	movs r2, #7
	asrs r1, r3, #3
	ands r2, r4
	adds r3, r4, #0
	cmp r4, #0
	bge .L_080cc104
	adds r3, r4, #7
.L_080cc104:
	asrs r3, r3, #3
	lsls r3, r3, #11
	mov r12, r3
	lsls r3, r1, #6
	lsls r7, r2, #3
	adds r4, r3, r5
	ldr r1, .L_080cc3f8
	adds r3, r7, r4
	add r3, r12
	adds r3, r3, r1
	movs r2, #2
	movs r1, #7
	strb r2, [r3]
	ands r1, r6
	adds r3, r6, #0
	cmp r6, #0
	bge .L_080cc128
	adds r3, r6, #7
.L_080cc128:
	asrs r3, r3, #3
	lsls r5, r1, #3
	lsls r6, r3, #11
	ldr r2, .L_080cc3f8
	adds r3, r5, r4
	adds r3, r6, r3
	adds r3, r3, r2
	movs r2, #2
	strb r2, [r3]
	movs r3, #7
	ands r3, r0
	adds r1, r0, #0
	cmp r0, #0
	bge .L_080cc146
	adds r1, r0, #7
.L_080cc146:
	asrs r1, r1, #3
	lsls r1, r1, #6
	adds r1, r1, r3
	adds r3, r7, r1
	ldr r4, .L_080cc3f8
	add r3, r12
	adds r1, r5, r1
	adds r3, r3, r4
	movs r2, #2
	adds r1, r6, r1
	mov r0, lr
	strb r2, [r3]
	adds r1, r1, r4
	movs r3, #2
	mov r2, r9
	adds r0, #1
	strb r3, [r1]
	adds r2, #97
	cmp r0, #0
	bge .L_080cc170
	movs r0, #0
.L_080cc170:
	cmp r2, #255
	ble .L_080cc176
	movs r2, #255
.L_080cc176:
	movs r1, #7
	ands r1, r2
	adds r3, r2, #0
	cmp r2, #0
	bge .L_080cc182
	adds r3, r2, #7
.L_080cc182:
	asrs r3, r3, #3
	lsls r3, r3, #6
	adds r3, r3, r1
	adds r2, r7, r3
	ldr r1, .L_080cc3f8
	add r2, r12
	adds r2, r2, r1
	movs r1, #2
	strb r1, [r2]
	adds r3, r5, r3
	ldr r2, .L_080cc3f8
	adds r3, r6, r3
	adds r3, r3, r2
	strb r1, [r3]
	movs r3, #7
	ands r3, r0
	adds r1, r0, #0
	cmp r0, #0
	bge .L_080cc1aa
	adds r1, r0, #7
.L_080cc1aa:
	asrs r1, r1, #3
	lsls r1, r1, #6
	adds r1, r1, r3
	adds r3, r7, r1
	ldr r4, .L_080cc3f8
	adds r1, r5, r1
	add r3, r12
	adds r3, r3, r4
	movs r2, #2
	adds r1, r6, r1
	strb r2, [r3]
	adds r1, r1, r4
	movs r3, #2
	movs r5, #96
	mov r7, r11
	strb r3, [r1]
	mov r2, r11
	subs r0, r5, r7
	mov r4, r9
	movs r1, #60
	mov r3, r9
	adds r2, #96
	mov r10, r0
	adds r4, #60
	subs r6, r1, r3
	cmp r0, #0
	bge .L_080cc1e2
	movs r0, #0
.L_080cc1e2:
	cmp r2, #255
	ble .L_080cc1e8
	movs r2, #255
.L_080cc1e8:
	cmp r6, #0
	bge .L_080cc1ee
	movs r6, #0
.L_080cc1ee:
	cmp r4, #119
	ble .L_080cc1f4
	movs r4, #119
.L_080cc1f4:
	movs r5, #7
	ands r5, r2
	adds r3, r2, #0
	cmp r2, #0
	bge .L_080cc200
	adds r3, r2, #7
.L_080cc200:
	movs r2, #7
	asrs r1, r3, #3
	ands r2, r4
	adds r3, r4, #0
	cmp r4, #0
	bge .L_080cc20e
	adds r3, r4, #7
.L_080cc20e:
	asrs r3, r3, #3
	lsls r3, r3, #11
	mov r8, r3
	lsls r3, r1, #6
	lsls r2, r2, #3
	adds r5, r3, r5
	adds r3, r2, r5
	ldr r7, .L_080cc3f8
	add r3, r8
	adds r3, r3, r7
	mov lr, r2
	movs r1, #7
	movs r2, #2
	strb r2, [r3]
	ands r1, r6
	adds r3, r6, #0
	cmp r6, #0
	bge .L_080cc234
	adds r3, r6, #7
.L_080cc234:
	asrs r3, r3, #3
	lsls r3, r3, #11
	lsls r7, r1, #3
	mov r12, r3
	ldr r1, .L_080cc3f8
	adds r3, r7, r5
	add r3, r12
	adds r3, r3, r1
	movs r2, #2
	strb r2, [r3]
	movs r3, #7
	ands r3, r0
	adds r1, r0, #0
	cmp r0, #0
	bge .L_080cc254
	adds r1, r0, #7
.L_080cc254:
	asrs r1, r1, #3
	lsls r1, r1, #6
	adds r1, r1, r3
	mov r5, lr
	ldr r0, .L_080cc3f8
	adds r3, r5, r1
	adds r1, r7, r1
	add r3, r8
	add r1, r12
	adds r3, r3, r0
	movs r2, #2
	adds r1, r1, r0
	mov r0, r10
	strb r2, [r3]
	adds r0, #1
	movs r3, #2
	mov r2, r11
	strb r3, [r1]
	adds r2, #97
	cmp r0, #0
	bge .L_080cc280
	movs r0, #0
.L_080cc280:
	cmp r2, #255
	ble .L_080cc286
	movs r2, #255
.L_080cc286:
	movs r1, #7
	mov r10, r1
	mov r3, r10
	adds r1, r2, #0
	ands r1, r3
	adds r3, r2, #0
	cmp r2, #0
	bge .L_080cc298
	adds r3, r2, #7
.L_080cc298:
	asrs r3, r3, #3
	lsls r3, r3, #6
	adds r1, r3, r1
	mov r5, lr
	adds r3, r5, r1
	ldr r5, .L_080cc3f8
	add r3, r8
	adds r3, r3, r5
	movs r5, #2
	strb r5, [r3]
	adds r3, r7, r1
	ldr r1, .L_080cc3f8
	add r3, r12
	adds r3, r3, r1
	strb r5, [r3]
	adds r2, r0, #0
	mov r3, r10
	ands r2, r3
	adds r3, r0, #0
	cmp r0, #0
	bge .L_080cc2c4
	adds r3, r0, #7
.L_080cc2c4:
	asrs r3, r3, #3
	lsls r3, r3, #6
	adds r1, r3, r2
	mov r4, lr
	adds r3, r4, r1
	ldr r2, .L_080cc3f8
	add r3, r8
	adds r3, r3, r2
	strb r5, [r3]
	ldr r4, .L_080cc3f8
	adds r3, r7, r1
	add r3, r12
	adds r3, r3, r4
	movs r5, #2
	strb r5, [r3]
	ldr r0, [sp, #8]
	mov r7, r11
	lsls r3, r7, #1
	subs r3, r0, r3
	subs r3, #1
	str r3, [sp, #8]
	cmp r3, #0
	bge .L_080cc304
	mov r1, r9
	ldr r2, [sp, #8]
	lsls r3, r1, #1
	adds r3, r2, r3
	subs r3, #2
	str r3, [sp, #8]
	movs r3, #1
	negs r3, r3
	add r9, r3
.L_080cc304:
	movs r4, #1
	add r11, r4
	cmp r9, r11
	blt .L_080cc30e
	b .L_080cc0bc
.L_080cc30e:
	ldr r5, [sp, #12]
	ldr r7, [sp, #16]
	adds r5, #1
	str r5, [sp, #12]
	cmp r5, r7
	beq .L_080cc31c
	b .L_080cc0aa
.L_080cc31c:
	movs r2, #240
	ldr r3, .L_080cc3fc
	ldr r0, .L_080cc3e8
	ldr r1, .L_080cc3f8
	lsls r2, r2, #7
	bl _call_via_r3
.L_080cc32a:
	ldr r0, [sp, #32]
	cmp r0, #50
	bgt .L_080cc37a
	ldr r1, [sp, #40]
	movs r2, #225
	lsls r2, r2, #7
	movs r6, #0
	adds r5, r1, r2
.L_080cc33a:
	ldr r2, .L_080cc400
	ldr r0, .L_080cc404
	lsls r3, r6, #1
	ldrh r1, [r2, r3]
	ldrb r0, [r0, r6]
	ldr r3, [sp, #40]
	movs r4, #2
	ldrsh r2, [r5, r4]
	adds r1, r3, r1
	movs r7, #6
	ldrsh r3, [r5, r7]
	str r0, [sp, #0]
	ldr r0, .L_080cc408
	ldrb r0, [r0, r6]
	ldr r4, [sp, #24]
	str r0, [sp, #4]
	ldr r0, [sp, #36]
	bl _call_via_r4
	ldr r7, [sp, #32]
	cmp r7, #3
	ble .L_080cc372
	movs r2, #128
	adds r0, r5, #0
	movs r1, #64
	lsls r2, r2, #7
	bl EffectStep_AdvanceWithGravity2D
.L_080cc372:
	adds r6, #1
	adds r5, #28
	cmp r6, #33
	bne .L_080cc33a
.L_080cc37a:
	ldr r1, [sp, #32]
	subs r1, #8
	cmp r1, #42
	bhi .L_080cc396
	adds r0, r1, #0
	cmp r0, #31
	ble .L_080cc38a
	movs r0, #31
.L_080cc38a:
	lsls r2, r0, #10
	lsls r1, r0, #5
	ldr r3, .L_080cc40c
	orrs r2, r1
	orrs r2, r0
	strh r2, [r3]
.L_080cc396:
	ldr r0, [sp, #32]
	cmp r0, #51
	bne .L_080cc46c
	ldr r0, .L_080cc410
	ldr r1, [sp, #40]
	movs r2, #1
	movs r3, #0
	bl Resource_LoadAndDecompress
	ldr r0, .L_080cc40c
	movs r6, #1
.L_080cc3ac:
	lsrs r3, r6, #31
	adds r3, r6, r3
	asrs r1, r3, #1
	cmp r1, #0
	bge .L_080cc3b8
	movs r1, #0
.L_080cc3b8:
	lsrs r3, r1, #31
	adds r3, r1, r3
	asrs r3, r3, #1
	lsls r2, r1, #10
	lsls r3, r3, #5
	orrs r2, r3
	orrs r2, r1
	adds r6, #1
	strh r2, [r0]
	adds r0, #2
	cmp r6, #64
	bne .L_080cc3ac
	ldr r2, .L_080cc414
	ldr r3, .L_080cc3f4
	strh r3, [r2]
	ldr r2, [sp, #40]
	movs r3, #225
	movs r1, #31
	lsls r3, r3, #7
	movs r6, #0
	b .L_080cc418
	.2byte 0x0000
.L_080cc3e4:
	.4byte IwramFillWords
.L_080cc3e8:
	.4byte 0x06008000
.L_080cc3ec:
	.4byte gMapBlocks + 0x202
.L_080cc3f0:
	.4byte 0x05000004
.L_080cc3f4:
	.4byte 0x00003f44
.L_080cc3f8:
	.4byte gMapCellBuffer
.L_080cc3fc:
	.4byte IwramCopyWords
.L_080cc400:
	.4byte Data_080edf88 + 0x4a
.L_080cc404:
	.4byte Data_080edf88 + 0x8
.L_080cc408:
	.4byte Data_080edf88 + 0x29
.L_080cc40c:
	.4byte 0x05000002
.L_080cc410:
	.4byte 0x0000007d
.L_080cc414:
	.4byte 0x04000050
.L_080cc418:
	mov r8, r1
	adds r5, r2, r3
	movs r7, #0
.L_080cc41e:
	bl Random16
	mov r4, r8
	ands r0, r4
	adds r0, #32
	lsls r0, r0, #16
	str r0, [r5]
	bl Random16
	mov r1, r8
	ands r0, r1
	adds r0, #80
	lsls r0, r0, #16
	str r0, [r5, #4]
	bl Random16
	ldr r3, .L_080cc5a8
	ldr r2, .L_080cc5ac
	ands r3, r0
	adds r3, r3, r2
	lsls r3, r3, #12
	adds r6, #1
	str r3, [r5, #12]
	str r7, [r5, #16]
	str r7, [r5, #24]
	adds r5, #28
	cmp r6, #32
	bne .L_080cc41e
	ldr r3, [sp, #40]
	movs r4, #239
	lsls r4, r4, #7
	adds r2, r3, r4
	movs r3, #2
	str r3, [r2]
	ldr r5, [sp, #40]
	ldr r7, .L_080cc5b0
	movs r3, #50
	adds r2, r5, r7
	str r3, [r2]
.L_080cc46c:
	ldr r0, [sp, #32]
	cmp r0, #52
	ble .L_080cc4de
	ldr r1, [sp, #40]
	movs r2, #225
	lsls r2, r2, #7
	movs r6, #0
	adds r5, r1, r2
.L_080cc47c:
	adds r3, r6, #0
	cmp r6, #0
	bge .L_080cc484
	adds r3, r6, #3
.L_080cc484:
	asrs r3, r3, #2
	ldr r4, [sp, #32]
	adds r3, #52
	cmp r4, r3
	blt .L_080cc4d6
	ldr r3, [r5, #24]
	cmp r3, #39
	bgt .L_080cc4d6
	adds r1, r3, #0
	cmp r1, #0
	bge .L_080cc49c
	adds r1, #3
.L_080cc49c:
	asrs r1, r1, #2
	cmp r1, #5
	ble .L_080cc4a4
	movs r1, #5
.L_080cc4a4:
	movs r0, #2
	ldrsh r2, [r5, r0]
	ldr r7, [sp, #40]
	movs r4, #6
	ldrsh r3, [r5, r4]
	movs r0, #32
	str r0, [sp, #0]
	lsls r1, r1, #11
	movs r0, #64
	adds r1, r7, r1
	subs r3, #32
	str r0, [sp, #4]
	subs r2, #16
	ldr r0, [sp, #36]
	ldr r7, [sp, #28]
	bl _call_via_r7
	adds r0, r5, #0
	movs r1, #60
	ldr r2, .L_080cc5b4
	bl EffectStep_AdvanceWithGravity2D
	ldr r3, [r5, #24]
	adds r3, #1
	str r3, [r5, #24]
.L_080cc4d6:
	adds r6, #1
	adds r5, #28
	cmp r6, #32
	bne .L_080cc47c
.L_080cc4de:
	bl ObjectGroup_TickMemberTimers
	ldr r1, .L_080cc5b8
	ldr r0, [sp, #40]
	movs r3, #1
	adds r2, r0, r1
	str r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r2, [sp, #32]
	adds r2, #1
	str r2, [sp, #32]
	cmp r2, #128
	beq .L_080cc4fe
	b .L_080cbf16
.L_080cc4fe:
	movs r0, #47
	bl Runtime_ReleaseHeapBlock
	movs r0, #46
	bl Runtime_ReleaseHeapBlock
	ldr r0, .L_080cc5bc
	bl Scheduler_RemoveCallback
	ldr r5, .L_080cc5c0
	ldr r4, [sp, #40]
	adds r3, r4, r5
	ldr r3, [r3]
	movs r7, #36
	ldrsh r0, [r3, r7]
	movs r3, #1
	negs r3, r3
	movs r2, #0
	str r2, [sp, #0]
	adds r1, r3, #0
	movs r2, #1
	bl ObjectGroup_UpdateMembers
	ldr r1, .L_080cc5c4
	ldr r0, [sp, #40]
	adds r3, r0, r1
	ldr r2, .L_080cc5c8
	ldr r3, [r3]
	strh r3, [r2, #4]
	movs r3, #32
	strh r3, [r2, #6]
	ldr r2, [sp, #20]
	movs r4, #201
	lsls r4, r4, #3
	adds r3, r2, r4
	ldrh r1, [r3]
	movs r2, #0
	movs r0, #2
	bl BattlePresentation_ConfigurePaletteFadeFar
	movs r0, #1
	bl WaitFrames
	ldr r1, .L_080cc5cc
	ldr r0, .L_080cc5d0
	ldrh r3, [r0]
	adds r4, r3, #0
	strh r0, [r0]
	ldrh r2, [r1]
	cmp r2, #31
	bgt .L_080cc582
	lsls r3, r2, #1
	adds r3, r3, r2
	lsls r3, r3, #2
	adds r2, #1
	adds r3, r3, r1
	strh r2, [r1]
	ldr r2, .L_080cc5d4
	adds r3, #4
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #19
	stmia r3!, {r2}
	movs r2, #128
	lsls r2, r2, #10
	str r2, [r3]
.L_080cc582:
	strh r4, [r0]
	movs r0, #41
	bl Runtime_ReleaseHeapBlock
	movs r0, #40
	bl Runtime_ReleaseHeapBlock
	movs r0, #39
	bl Runtime_ReleaseHeapBlock
	add sp, #56
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_080cc5a8:
	.4byte 0x000001ff
.L_080cc5ac:
	.4byte 0xffffff00
.L_080cc5b0:
	.4byte 0x00007784
.L_080cc5b4:
	.4byte 0xfffff000
.L_080cc5b8:
	.4byte 0x00007824
.L_080cc5bc:
	.4byte BattlePresentation_ProcessPendingGraphicsTransfer
.L_080cc5c0:
	.4byte 0x00007828
.L_080cc5c4:
	.4byte 0x000077a0
.L_080cc5c8:
	.4byte gBgScroll
.L_080cc5cc:
	.4byte gIoWriteQueue
.L_080cc5d0:
	.4byte 0x04000208
.L_080cc5d4:
	.4byte 0x00007541
