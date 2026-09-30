.syntax unified
	.thumb
	.global BattleEv_DispatchQueued
	.thumb_func
BattleEv_DispatchQueued:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	movs r2, #192
	lsls r2, r2, #3
	adds r2, #228
	adds r6, r3, r2
	movs r2, #128
	lsls r2, r2, #4
	adds r2, #40
	adds r3, r3, r2
	ldr r3, [r3]
	movs r7, #0
	cmp r7, r3
	blt .L_081201e6
	b .L_08120356
.L_081201e6:
	ldrb r3, [r6, r7]
	cmp r3, #15
	bls .L_081201ee
	b .L_08120346
.L_081201ee:
	ldr r2, .L_0812035c
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_081201f8:
	.4byte .L_08120252
	.4byte .L_08120260
	.4byte .L_0812026e
	.4byte .L_08120284
	.4byte .L_081202a8
	.4byte .L_081202c0
	.4byte .L_0812029a
	.4byte .L_081202ba
	.4byte .L_081202d0
	.4byte .L_081202f2
	.4byte .L_08120312
	.4byte .L_08120322
	.4byte .L_08120246
	.4byte .L_08120238
	.4byte .L_08120346
	.4byte .L_0812033c
.L_08120238:
	lsls r3, r7, #2
	adds r3, #64
	ldr r1, [r6, r3]
	adds r0, r6, #0
	bl Battle_SetRuntimeFlagBit0
	b .L_08120346
.L_08120246:
	lsls r3, r7, #2
	adds r3, #64
	ldr r0, [r6, r3]
	bl Func_08120178
	b .L_08120346
.L_08120252:
	lsls r3, r7, #2
	adds r3, #64
	ldr r0, [r6, r3]
	movs r1, #1
	bl UiText_DrawQuantity
	b .L_08120346
.L_08120260:
	lsls r3, r7, #2
	adds r3, #64
	ldr r0, [r6, r3]
	movs r1, #5
	bl UiText_DrawQuantity
	b .L_08120346
.L_0812026e:
	lsls r3, r7, #2
	adds r3, #64
	ldr r0, [r6, r3]
	movs r3, #128
	lsls r3, r3, #1
	adds r3, #255
	ands r0, r3
	movs r1, #2
	bl UiText_DrawQuantity
	b .L_08120346
.L_08120284:
	lsls r3, r7, #2
	adds r3, #64
	ldr r0, [r6, r3]
	movs r3, #252
	lsls r3, r3, #6
	adds r3, #255
	ands r0, r3
	movs r1, #4
	bl UiText_DrawQuantity
	b .L_08120346
.L_0812029a:
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #148
	ldr r2, [r3]
	movs r3, #1
	str r3, [r2, #8]
	b .L_08120346
.L_081202a8:
	lsls r3, r7, #2
	adds r3, #64
	ldr r0, [r6, r3]
	cmp r0, #0
	blt .L_081202b6
	bl Func_080381c8
.L_081202b6:
	bl BattlePresentation_WaitForAdvance
.L_081202ba:
	bl Func_08038118
	b .L_08120346
.L_081202c0:
	lsls r3, r7, #2
	adds r3, #64
	ldr r0, [r6, r3]
	cmp r0, #0
	blt .L_081202ba
	bl Func_080381c8
	b .L_081202ba
.L_081202d0:
	movs r2, #180
	lsls r2, r2, #1
	adds r3, r6, r2
	movs r2, #0
	ldrsh r0, [r3, r2]
	cmp r0, #0
	ble .L_081202e2
	bl Audio_PlayCue
.L_081202e2:
	lsls r3, r7, #2
	adds r3, #64
	ldr r0, [r6, r3]
	movs r1, #0
	movs r2, #0
	bl Func_0811f330
	b .L_08120346
.L_081202f2:
	movs r2, #182
	lsls r2, r2, #1
	lsls r5, r7, #2
	adds r5, #64
	adds r3, r6, r2
	ldr r1, [r3]
	ldr r0, [r6, r5]
	bl Func_0812824c
	ldr r0, [r6, r5]
	bl Func_0811fe3c
	ldr r0, [r6, r5]
	bl Func_0811f444
	b .L_08120346
.L_08120312:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	adds r3, #65
	ldrb r0, [r3]
	bl UiWindow_DrawPartyStatusContentsFar
	b .L_08120346
.L_08120322:
	lsls r5, r7, #2
	adds r5, #64
	ldr r0, [r6, r5]
	bl GetBattleObjectSlot
	adds r1, r0, #0
	ldr r0, [r6, r5]
	bl Func_0811b4d8
	ldr r0, [r6, r5]
	bl BattlePres_SetActorModeAndAction
	b .L_08120346
.L_0812033c:
	lsls r3, r7, #2
	adds r3, #64
	ldr r0, [r6, r3]
	bl Func_0811f3b8
.L_08120346:
	movs r2, #162
	lsls r2, r2, #1
	adds r3, r6, r2
	ldr r3, [r3]
	adds r7, #1
	cmp r7, r3
	bge .L_08120356
	b .L_081201e6
.L_08120356:
	bl Func_081234a4
	pop {r5, r6, r7, pc}
.L_0812035c:
	.4byte .L_081201f8
