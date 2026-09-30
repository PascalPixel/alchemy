.syntax unified
	.thumb
	.global Func_0812561c
	.thumb_func
Func_0812561c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #32
	str r0, [sp, #12]
	movs r1, #0
	ldrb r0, [r0]
	str r1, [sp, #4]
	mov r8, r0
	bl Owner_GetState
	mov r2, r8
	str r0, [sp, #8]
	cmp r2, #7
	bls .L_08125650
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	adds r3, #68
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_081256a4
.L_08125650:
	mov r3, r8
	movs r0, #0
	cmp r3, #7
	bls .L_0812565a
	movs r0, #1
.L_0812565a:
	bl Resource_FarCall005
	adds r3, r0, #0
	movs r0, #148
	lsls r0, r0, #1
	adds r6, r3, #0
	adds r3, r3, r0
	ldr r3, [r3]
	ldr r1, [sp, #4]
	adds r6, #8
	movs r7, #0
	cmp r1, r3
	bge .L_081256a4
	movs r2, #1
	negs r2, r2
	mov r10, r2
	adds r5, r6, #0
.L_0812567c:
	ldrb r3, [r5, #2]
	cmp r3, r8
	bne .L_08125694
	movs r3, #3
	ldrsb r3, [r5, r3]
	cmp r3, r10
	bne .L_08125694
	ldrb r1, [r5]
	ldrb r2, [r5, #1]
	mov r0, r8
	bl Djinn_ActivateFar + 0x8
.L_08125694:
	movs r0, #144
	lsls r0, r0, #1
	adds r3, r6, r0
	ldr r3, [r3]
	adds r7, #1
	adds r5, #4
	cmp r7, r3
	blt .L_0812567c
.L_081256a4:
	movs r0, #1
	movs r1, #0
	bl Func_0811a188
	cmp r0, #0
	beq .L_081256c0
	movs r0, #2
	movs r1, #0
	bl Func_0811a188
	cmp r0, #0
	beq .L_081256c0
	movs r1, #1
	str r1, [sp, #4]
.L_081256c0:
	mov r2, r8
	movs r0, #0
	cmp r2, #7
	bls .L_081256d8
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	adds r3, #68
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_081256d8
	movs r0, #1
.L_081256d8:
	bl Resource_FarCall005
	add r3, sp, #16
	mov r11, r3
	adds r6, r0, #0
	adds r6, #8
	movs r2, #0
	add r3, sp, #28
	mov r12, r11
.L_081256ea:
	str r2, [r3]
	subs r3, #4
	cmp r3, r12
	bge .L_081256ea
	movs r3, #144
	lsls r3, r3, #1
	adds r7, r6, r3
	movs r0, #2
	str r7, [sp, #0]
	negs r0, r0
	mov r9, r0
.L_08125700:
	ldr r3, [r7]
	movs r1, #1
	negs r1, r1
	movs r4, #0
	mov r12, r1
	cmp r4, r3
	bge .L_08125732
	movs r3, #3
	ldrsb r3, [r6, r3]
	cmp r3, r9
	bne .L_0812571a
	ldrb r2, [r6, #2]
	b .L_08125730
.L_0812571a:
	ldr r3, [r7]
	adds r4, #1
	cmp r4, r3
	bge .L_08125732
	lsls r3, r4, #2
	adds r2, r6, r3
	movs r3, #3
	ldrsb r3, [r2, r3]
	cmp r3, r9
	bne .L_0812571a
	ldrb r2, [r2, #2]
.L_08125730:
	mov r12, r2
.L_08125732:
	movs r3, #1
	negs r3, r3
	cmp r12, r3
	beq .L_081257ac
	adds r5, r3, #0
	ldr r3, [r7]
	cmp r3, #0
	ble .L_08125760
	ldr r3, [sp, #0]
	adds r2, r6, #0
	ldr r4, [r3]
.L_08125748:
	ldrb r3, [r2, #2]
	cmp r3, r12
	bne .L_08125758
	movs r3, #3
	ldrsb r3, [r2, r3]
	cmp r3, r5
	ble .L_08125758
	adds r5, r3, #0
.L_08125758:
	subs r4, #1
	adds r2, #4
	cmp r4, #0
	bne .L_08125748
.L_08125760:
	adds r5, #1
	cmp r5, #1
	bgt .L_08125768
	movs r5, #2
.L_08125768:
	ldr r3, [r7]
	movs r4, #0
	cmp r4, r3
	bge .L_08125700
	movs r1, #144
	movs r0, #2
	lsls r1, r1, #1
	negs r0, r0
	adds r1, r1, r6
	mov r10, r0
	mov lr, r1
	mov r0, r11
	adds r1, r6, #0
.L_08125782:
	ldrb r3, [r1, #2]
	cmp r3, r12
	bne .L_0812579e
	movs r3, #3
	ldrsb r3, [r1, r3]
	cmp r3, r10
	bne .L_0812579e
	ldrb r2, [r1]
	strb r5, [r1, #3]
	lsls r2, r2, #2
	ldr r3, [r0, r2]
	adds r5, #1
	adds r3, #1
	str r3, [r0, r2]
.L_0812579e:
	mov r2, lr
	ldr r3, [r2]
	adds r4, #1
	adds r1, #4
	cmp r4, r3
	blt .L_08125782
	b .L_08125700
.L_081257ac:
	ldr r3, [sp, #4]
	cmp r3, #0
	bne .L_081257b4
	b .L_08125a64
.L_081257b4:
	movs r5, #166
	lsls r5, r5, #1
	adds r0, r5, #0
	bl Runtime_BumpAllocateAlternatePool
	adds r2, r5, #0
	ldr r3, .L_08125a84
	ldr r1, [sp, #8]
	mov r10, r0
	mov lr, r3
	.2byte 0xf800
	ldr r2, [sp, #8]
	movs r3, #150
	lsls r3, r3, #1
	movs r6, #0
	mov r5, r11
	movs r4, #4
	movs r0, #0
	adds r1, r2, r3
.L_081257da:
	ldr r2, [r0, r5]
	cmp r2, #4
	ble .L_081257e6
	mov r2, r11
	str r4, [r0, r2]
	adds r2, r4, #0
.L_081257e6:
	movs r3, #0
	ldrsb r3, [r1, r3]
	cmp r3, r2
	bge .L_081257f0
	strb r2, [r1]
.L_081257f0:
	adds r6, #1
	adds r1, #1
	adds r0, #4
	cmp r6, #3
	ble .L_081257da
	mov r0, r8
	bl BattleUnit_Recalculate
	movs r6, #0
	movs r7, #72
.L_08125804:
	ldr r3, [sp, #8]
	mov r1, r10
	ldrsh r2, [r7, r3]
	ldrsh r3, [r7, r1]
	subs r5, r2, r3
	cmp r5, #0
	ble .L_0812587a
	bl Func_081234a4
	movs r0, #25
	bl Func_08122c88
	movs r0, #0
	mov r1, r8
	bl BattleEv_Push
	movs r0, #1
	adds r1, r5, #0
	bl BattleEv_Push
	movs r0, #14
	movs r1, #175
	bl BattleEv_Push
	ldr r1, .L_08125a88
	movs r0, #4
	adds r1, r6, r1
	bl BattleEv_Push
	mov r1, r8
	movs r0, #11
	bl BattleEv_Push
	movs r0, #212
	bl Audio_PlayCue
	mov r0, r8
	bl GetBattleObjectSlot
	movs r1, #3
	ldr r0, [r0]
	bl Object_SetMode
	mov r0, r8
	bl GetBattleObjectSlot
	movs r1, #32
	ldr r0, [r0]
	bl ObjectDispatch_ApplyValueToChildrenFar
	movs r3, #1
	mov r0, r8
	adds r1, r6, #0
	movs r2, #2
	negs r3, r3
	bl Func_08127308
	bl Func_081234f0
.L_0812587a:
	adds r6, #1
	adds r7, #4
	cmp r6, #3
	ble .L_08125804
	mov r0, r10
	bl Sys_Free
	ldr r1, [sp, #4]
	cmp r1, #0
	bne .L_08125890
	b .L_08125a64
.L_08125890:
	mov r0, r8
	bl GetBattleObjectSlot
	ldr r3, [r0]
	cmp r3, #0
	bne .L_0812589e
	b .L_08125a64
.L_0812589e:
	bl Func_081234a4
	ldr r2, [sp, #12]
	ldr r3, [r2, #96]
	cmp r3, #0
	beq .L_08125924
	movs r0, #8
	mov r1, r8
	bl BattleEv_Push
	movs r0, #0
	mov r1, r8
	bl BattleEv_Push
	ldr r3, [sp, #12]
	movs r0, #1
	ldr r1, [r3, #96]
	bl BattleEv_Push
	ldr r1, .L_08125a8c
	movs r0, #4
	bl BattleEv_Push
	ldr r0, [sp, #12]
	ldr r1, [r0, #96]
	mov r0, r8
	negs r1, r1
	bl Owner_AdjustFirstValueFar
	cmp r0, #0
	bne .L_0812591c
	mov r0, r8
	bl BattleUnit_KeepsOneHp
	cmp r0, #0
	beq .L_081258f8
	movs r1, #1
	mov r0, r8
	bl Owner_AdjustFirstValueFar
	movs r0, #11
	mov r1, r8
	bl BattleEv_Push
	b .L_08125924
.L_081258f8:
	movs r0, #9
	mov r1, r8
	bl BattleEv_Push
	mov r1, r8
	movs r0, #0
	bl BattleEv_Push
	mov r1, r8
	cmp r1, #7
	bhi .L_08125912
	ldr r1, .L_08125a90
	b .L_08125914
.L_08125912:
	ldr r1, .L_08125a94
.L_08125914:
	movs r0, #4
	bl BattleEv_Push
	b .L_08125924
.L_0812591c:
	movs r0, #11
	mov r1, r8
	bl BattleEv_Push
.L_08125924:
	bl BattleEv_DispatchQueued
	bl Func_081234a4
	ldr r2, [sp, #8]
	movs r3, #50
	adds r3, #255
	adds r6, r2, r3
	movs r0, #0
	ldrsb r0, [r6, r0]
	cmp r0, #0
	beq .L_081259e4
	movs r1, #52
	ldrsh r3, [r2, r1]
	movs r1, #10
	muls r0, r3
	bl __divsi3
	movs r3, #192
	lsls r3, r3, #18
	adds r7, r0, #0
	mov r1, r8
	movs r0, #8
	ldr r5, [r3, #36]
	bl BattleEv_Push
	movs r0, #0
	mov r1, r8
	bl BattleEv_Push
	movs r0, #1
	adds r1, r7, #0
	bl BattleEv_Push
	ldr r1, .L_08125a98
	movs r0, #4
	bl BattleEv_Push
	movs r3, #0
	ldrsb r3, [r6, r3]
	cmp r3, #0
	beq .L_08125984
	movs r3, #128
	lsls r3, r3, #4
	adds r3, #76
	adds r2, r5, r3
	movs r3, #134
	b .L_0812598e
.L_08125984:
	movs r0, #128
	lsls r0, r0, #4
	adds r0, #76
	adds r2, r5, r0
	movs r3, #133
.L_0812598e:
	strh r3, [r2]
	negs r1, r7
	mov r0, r8
	bl Owner_AdjustFirstValueFar
	cmp r0, #0
	bne .L_081259dc
	mov r0, r8
	bl BattleUnit_KeepsOneHp
	cmp r0, #0
	beq .L_081259b8
	movs r1, #1
	mov r0, r8
	bl Owner_AdjustFirstValueFar
	movs r0, #11
	mov r1, r8
	bl BattleEv_Push
	b .L_081259e4
.L_081259b8:
	movs r0, #9
	mov r1, r8
	bl BattleEv_Push
	mov r1, r8
	movs r0, #0
	bl BattleEv_Push
	mov r1, r8
	cmp r1, #7
	bhi .L_081259d2
	ldr r1, .L_08125a90
	b .L_081259d4
.L_081259d2:
	ldr r1, .L_08125a94
.L_081259d4:
	movs r0, #4
	bl BattleEv_Push
	b .L_081259e4
.L_081259dc:
	movs r0, #11
	mov r1, r8
	bl BattleEv_Push
.L_081259e4:
	bl BattleEv_DispatchQueued
	bl Func_081234a4
	ldr r2, [sp, #8]
	movs r3, #66
	adds r3, #255
	adds r5, r2, r3
	ldrb r3, [r5]
	cmp r3, #0
	beq .L_08125a60
	mov r0, r8
	bl BattleUnit_KeepsOneHp
	cmp r0, #0
	bne .L_08125a60
	ldrb r3, [r5]
	adds r3, #255
	strb r3, [r5]
	lsls r3, r3, #24
	cmp r3, #0
	bne .L_08125a60
	movs r1, #192
	lsls r1, r1, #24
	mov r0, r8
	bl Owner_AdjustFirstValueFar
	cmp r0, #0
	bne .L_08125a60
	movs r0, #0
	mov r1, r8
	bl BattleEv_Push
	ldr r5, .L_08125a9c
	movs r0, #4
	adds r1, r5, #0
	bl BattleEv_Push
	movs r0, #8
	mov r1, r8
	bl BattleEv_Push
	movs r0, #9
	mov r1, r8
	bl BattleEv_Push
	movs r0, #0
	mov r1, r8
	bl BattleEv_Push
	mov r0, r8
	cmp r0, #7
	bhi .L_08125a58
	subs r1, r5, #3
	movs r0, #4
	bl BattleEv_Push
	b .L_08125a60
.L_08125a58:
	adds r1, r5, #3
	movs r0, #4
	bl BattleEv_Push
.L_08125a60:
	bl BattleEv_DispatchQueued
.L_08125a64:
	mov r0, r8
	bl GetBattleObjectSlot
	ldr r3, [r0]
	cmp r3, #0
	beq .L_08125a76
	mov r0, r8
	bl BattleUnit_Recalculate
.L_08125a76:
	add sp, #32
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08125a84:
	.4byte IwramCopyWords
.L_08125a88:
	.4byte 0x00000cd5
.L_08125a8c:
	.4byte 0x00000ca0
.L_08125a90:
	.4byte 0x00000c71
.L_08125a94:
	.4byte 0x00000c77
.L_08125a98:
	.4byte 0x00000ca8
.L_08125a9c:
	.4byte 0x00000c74
