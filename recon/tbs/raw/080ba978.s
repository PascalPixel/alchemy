.syntax unified
	.thumb
	.global Func_080ba978
	.thumb_func
Func_080ba978:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	ldr r3, .L_080babbc
	adds r7, r0, #0
	ldr r5, [r3]
	movs r2, #128
	ldr r3, [r7, #88]
	lsls r2, r2, #11
	ands r3, r2
	sub sp, #88
	mov r10, r1
	cmp r3, #0
	beq .L_080ba9aa
	ldrb r3, [r7]
	ldr r2, .L_080babc0
	cmp r3, #7
	bls .L_080ba9a2
	movs r2, #160
	lsls r2, r2, #7
.L_080ba9a2:
	movs r3, #60
	str r2, [r5]
	str r3, [r5, #4]
	b .L_080baa28
.L_080ba9aa:
	ldrb r0, [r7]
	bl GetBattleObjectSlot
	ldr r3, [r0]
	ldr r1, [r3, #16]
	ldr r0, [r3, #8]
	bl ArcTan2
	ldrb r4, [r7]
	lsls r0, r0, #16
	ldr r2, .L_080babc4
	lsrs r0, r0, #16
	adds r3, r4, #0
	adds r1, r0, r2
	cmp r3, #7
	bls .L_080ba9d0
	movs r3, #192
	lsls r3, r3, #5
	adds r1, r0, r3
.L_080ba9d0:
	lsls r3, r1, #16
	asrs r1, r3, #16
	adds r3, r4, #0
	cmp r3, #7
	bhi .L_080ba9e0
	movs r3, #128
	lsls r3, r3, #6
	b .L_080ba9e2
.L_080ba9e0:
	ldr r3, .L_080babc0
.L_080ba9e2:
	subs r3, r3, r1
	lsls r2, r3, #1
	adds r2, r2, r3
	cmp r2, #0
	bge .L_080ba9ee
	adds r2, #3
.L_080ba9ee:
	asrs r3, r2, #2
	adds r1, r1, r3
	ldrb r3, [r7, #2]
	adds r2, r4, #0
	cmp r3, #7
	bhi .L_080baa08
	movs r3, #0
	cmp r2, #7
	bhi .L_080baa02
	movs r3, #1
.L_080baa02:
	cmp r3, #0
	bne .L_080baa14
	b .L_080baa20
.L_080baa08:
	movs r3, #0
	cmp r2, #7
	bls .L_080baa10
	movs r3, #1
.L_080baa10:
	cmp r3, #0
	beq .L_080baa20
.L_080baa14:
	movs r1, #144
	adds r3, r4, #0
	lsls r1, r1, #6
	cmp r3, #7
	bls .L_080baa20
	ldr r1, .L_080babc8
.L_080baa20:
	ldr r3, [r5]
	cmp r3, r1
	beq .L_080baa28
	str r1, [r5]
.L_080baa28:
	ldr r3, [r7, #88]
	movs r2, #128
	lsls r2, r2, #12
	ands r3, r2
	cmp r3, #0
	beq .L_080baa46
	ldrb r3, [r7]
	ldr r2, .L_080babc0
	cmp r3, #7
	bls .L_080baa40
	movs r2, #128
	lsls r2, r2, #6
.L_080baa40:
	movs r3, #60
	str r2, [r5]
	str r3, [r5, #4]
.L_080baa46:
	add r5, sp, #4
	adds r0, r7, #0
	adds r1, r5, #0
	bl BattlePres_BuildTargetList
	mov r6, r10
	movs r3, #1
	ands r6, r3
	cmp r6, #0
	beq .L_080baa5c
	str r3, [r5, #28]
.L_080baa5c:
	movs r1, #0
	movs r0, #0
	bl BattlePres_SetActorModes
	ldr r3, .L_080babcc
	ldr r3, [r3]
	adds r3, #65
	ldrb r0, [r3]
	movs r3, #2
	negs r3, r3
	ands r0, r3
	bl UiWindow_DrawPartyStatusContentsFar
	ldr r0, [r5, #8]
	bl GetBattleObjectSlot
	ldr r0, [r0]
	movs r1, #3
	mov r8, r0
	bl Object_SetMode
	movs r1, #16
	mov r0, r8
	bl ObjectDispatch_ApplyValueToChildrenFar
	movs r0, #154
	bl AudioCommand_PlayFar
	movs r3, #2
	mov r1, r10
	ands r3, r1
	cmp r3, #0
	beq .L_080baaac
	ldr r0, [r5, #8]
	ldr r1, [r7, #80]
	movs r2, #1
	movs r3, #0
	bl BattleFx_PlayUnitElementEffect
	b .L_080baabc
.L_080baaac:
	cmp r6, #0
	bne .L_080baabc
	ldr r0, [r5, #8]
	ldr r1, [r7, #80]
	movs r2, #0
	movs r3, #0
	bl BattleFx_PlayUnitElementEffect
.L_080baabc:
	ldrb r3, [r7, #2]
	cmp r3, #7
	bhi .L_080baac6
	movs r3, #1
	b .L_080baac8
.L_080baac6:
	movs r3, #0
.L_080baac8:
	str r3, [r5, #4]
	ldr r3, [r5, #20]
	movs r4, #0
	adds r2, r5, #0
	cmp r3, #0
	beq .L_080bab1e
	movs r6, #0
.L_080baad6:
	lsls r3, r4, #1
	adds r3, #36
	ldrsh r0, [r2, r3]
	str r4, [sp, #0]
	bl GetBattleObjectSlot
	movs r1, #0
	ldr r0, [r0]
	bl GetMotionRecord
	adds r3, r0, #0
	adds r3, #39
	ldrb r3, [r3]
	subs r3, #1
	movs r1, #0
	ldr r4, [sp, #0]
	cmp r3, #0
	beq .L_080bab12
	mov r12, r3
	adds r3, r6, r5
	adds r2, r3, #0
	adds r2, #52
	adds r0, #40
.L_080bab04:
	ldmia r0!, {r3}
	ldrb r3, [r3, #5]
	adds r1, #1
	strb r3, [r2]
	adds r2, #1
	cmp r1, r12
	bne .L_080bab04
.L_080bab12:
	ldr r3, [r5, #20]
	adds r4, #1
	adds r6, #4
	adds r2, r5, #0
	cmp r4, r3
	bne .L_080baad6
.L_080bab1e:
	ldr r3, [r7, #92]
	cmp r3, #0
	beq .L_080bab4c
	cmp r3, #1
	bne .L_080bab3a
	ldrb r1, [r7]
	movs r0, #0
	bl BattleEv_Push
	ldr r1, .L_080babd0
	movs r0, #4
	bl BattleEv_Push
	b .L_080bab42
.L_080bab3a:
	ldr r1, .L_080babd4
	movs r0, #4
	bl BattleEv_Push
.L_080bab42:
	bl BattleEv_DispatchQueued
	bl BattlePres_RunWithZeroArguments
	b .L_080babaa
.L_080bab4c:
	movs r1, #200
	ldr r0, .L_080babd8
	lsls r1, r1, #4
	bl Scheduler_AddOrUpdateCallback
	ldr r3, [r5]
	cmp r3, #0
	beq .L_080bab78
	ldr r3, [r7, #88]
	movs r2, #128
	lsls r2, r2, #7
	ands r3, r2
	cmp r3, #0
	beq .L_080bab70
	adds r0, r5, #0
	bl BattleFx_DispatchByIdRangeFar
	b .L_080bab7c
.L_080bab70:
	adds r0, r5, #0
	bl BattleFx_DispatchModeFar
	b .L_080bab7c
.L_080bab78:
	bl BattlePres_RunWithZeroArguments
.L_080bab7c:
	bl BattleEventRuntime_WaitForReady
	adds r6, r5, #0
	mov r0, r8
	movs r1, #1
	bl Object_SetMode
	ldr r3, [r6, #20]
	movs r4, #0
	cmp r3, #0
	beq .L_080babaa
	movs r7, #36
.L_080bab94:
	ldrsh r0, [r6, r7]
	str r4, [sp, #0]
	bl Actor_ResetMotionAtAnchor
	adds r5, r6, #0
	ldr r4, [sp, #0]
	ldr r3, [r5, #20]
	adds r4, #1
	adds r7, #2
	cmp r4, r3
	bne .L_080bab94
.L_080babaa:
	movs r0, #0
	add sp, #88
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
.L_080babbc:
	.4byte gTransitionWork
.L_080babc0:
	.4byte 0xffffe000
.L_080babc4:
	.4byte 0xffffe800
.L_080babc8:
	.4byte 0xffffdc00
.L_080babcc:
	.4byte gBattleWork
.L_080babd0:
	.4byte 0x00000856
.L_080babd4:
	.4byte 0x00000855
.L_080babd8:
	.4byte BattleEvent_Playback
