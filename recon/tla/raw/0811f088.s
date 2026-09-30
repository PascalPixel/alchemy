.syntax unified
	.thumb
	.global Func_0811f088
	.thumb_func
Func_0811f088:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #176
	adds r7, r0, #0
	ldr r5, [r3]
	ldr r3, [r7, #88]
	movs r2, #128
	lsls r2, r2, #11
	ands r3, r2
	sub sp, #92
	mov r11, r1
	cmp r3, #0
	beq .L_0811f0c6
	ldrb r3, [r7]
	cmp r3, #7
	bhi .L_0811f0ba
	ldr r3, .L_0811f318
	b .L_0811f0be
.L_0811f0ba:
	movs r3, #160
	lsls r3, r3, #7
.L_0811f0be:
	str r3, [r5]
	movs r3, #60
	str r3, [r5, #4]
	b .L_0811f140
.L_0811f0c6:
	ldrb r0, [r7]
	bl GetBattleObjectSlot
	ldrb r1, [r7, #3]
	ldr r3, [r0]
	mov r8, r1
	ldr r0, [r3, #8]
	ldr r1, [r3, #16]
	ldrb r6, [r7]
	bl ArcTan2
	ldr r2, .L_0811f31c
	lsls r0, r0, #16
	lsrs r0, r0, #16
	adds r1, r0, r2
	cmp r6, #7
	bls .L_0811f0ee
	movs r3, #192
	lsls r3, r3, #5
	adds r1, r0, r3
.L_0811f0ee:
	lsls r3, r1, #16
	asrs r1, r3, #16
	cmp r6, #7
	bhi .L_0811f0fc
	movs r3, #128
	lsls r3, r3, #6
	b .L_0811f0fe
.L_0811f0fc:
	ldr r3, .L_0811f318
.L_0811f0fe:
	subs r3, r3, r1
	lsls r2, r3, #1
	adds r2, r2, r3
	cmp r2, #0
	bge .L_0811f10a
	adds r2, #3
.L_0811f10a:
	asrs r3, r2, #2
	adds r1, r1, r3
	mov r2, r8
	cmp r2, #7
	bhi .L_0811f122
	movs r3, #0
	cmp r6, #7
	bhi .L_0811f11c
	movs r3, #1
.L_0811f11c:
	cmp r3, #0
	bne .L_0811f12e
	b .L_0811f138
.L_0811f122:
	movs r3, #0
	cmp r6, #7
	bls .L_0811f12a
	movs r3, #1
.L_0811f12a:
	cmp r3, #0
	beq .L_0811f138
.L_0811f12e:
	movs r1, #144
	lsls r1, r1, #6
	cmp r6, #7
	bls .L_0811f138
	ldr r1, .L_0811f320
.L_0811f138:
	ldr r3, [r5]
	cmp r3, r1
	beq .L_0811f140
	str r1, [r5]
.L_0811f140:
	ldr r3, [r7, #88]
	movs r2, #128
	lsls r2, r2, #12
	ands r3, r2
	cmp r3, #0
	beq .L_0811f160
	ldrb r3, [r7]
	cmp r3, #7
	bhi .L_0811f156
	ldr r3, .L_0811f318
	b .L_0811f15a
.L_0811f156:
	movs r3, #128
	lsls r3, r3, #6
.L_0811f15a:
	str r3, [r5]
	movs r3, #60
	str r3, [r5, #4]
.L_0811f160:
	add r6, sp, #4
	adds r1, r6, #0
	mov r8, r11
	adds r0, r7, #0
	bl Func_0811ddd8
	mov r1, r8
	movs r3, #1
	ands r1, r3
	mov r8, r1
	cmp r1, #0
	beq .L_0811f17a
	str r3, [r6, #28]
.L_0811f17a:
	movs r5, #192
	movs r1, #0
	movs r0, #0
	lsls r5, r5, #18
	bl BattlePres_SetActorModes
	ldr r3, [r5, #36]
	adds r3, #65
	ldrb r0, [r3]
	movs r3, #2
	negs r3, r3
	ands r0, r3
	bl UiWindow_DrawPartyStatusContentsFar
	adds r1, r6, #0
	adds r0, r7, #0
	bl Func_0811e3ac
	mov r10, r0
	ldr r0, [r6, #8]
	bl GetBattleObjectSlot
	ldr r3, [r5, #36]
	movs r2, #128
	ldr r0, [r0]
	lsls r2, r2, #4
	adds r2, #105
	adds r3, r3, r2
	ldrb r1, [r3]
	mov r9, r0
	bl Object_SetMode
	movs r1, #16
	mov r0, r9
	bl ObjectDispatch_ApplyValueToChildrenFar
	movs r0, #154
	bl Audio_PlayCue
	movs r3, #2
	mov r1, r11
	ands r3, r1
	cmp r3, #0
	beq .L_0811f1e0
	ldr r0, [r6, #8]
	ldr r1, [r7, #80]
	movs r2, #1
	movs r3, #0
	bl Func_08127308
	b .L_0811f1f2
.L_0811f1e0:
	mov r2, r8
	cmp r2, #0
	bne .L_0811f1f2
	ldr r0, [r6, #8]
	ldr r1, [r7, #80]
	movs r2, #0
	movs r3, #0
	bl Func_08127308
.L_0811f1f2:
	ldrb r3, [r7, #3]
	cmp r3, #7
	bhi .L_0811f1fc
	movs r3, #1
	b .L_0811f1fe
.L_0811f1fc:
	movs r3, #0
.L_0811f1fe:
	str r3, [r6, #4]
	ldr r3, [r6, #20]
	movs r4, #0
	adds r2, r6, #0
	cmp r3, #0
	beq .L_0811f252
	movs r5, #0
.L_0811f20c:
	lsls r3, r4, #1
	adds r3, #36
	ldrsh r0, [r2, r3]
	str r4, [sp, #0]
	bl GetBattleObjectSlot
	movs r1, #0
	ldr r0, [r0]
	bl GetMotionRecord
	ldrb r3, [r0, #27]
	movs r1, #0
	subs r3, #1
	ldr r4, [sp, #0]
	cmp r3, #0
	beq .L_0811f246
	add r2, sp, #92
	mov r12, r3
	adds r3, r2, r5
	adds r2, r3, #0
	subs r2, #34
	adds r0, #40
.L_0811f238:
	ldmia r0!, {r3}
	adds r1, #1
	ldrb r3, [r3, #5]
	strb r3, [r2]
	adds r2, #1
	cmp r1, r12
	bne .L_0811f238
.L_0811f246:
	ldr r3, [r6, #20]
	adds r4, #1
	adds r5, #4
	adds r2, r6, #0
	cmp r4, r3
	bne .L_0811f20c
.L_0811f252:
	ldr r3, [r7, #92]
	cmp r3, #0
	beq .L_0811f280
	cmp r3, #1
	bne .L_0811f26e
	ldrb r1, [r7]
	movs r0, #0
	bl BattleEv_Push
	ldr r1, .L_0811f324
	movs r0, #4
	bl BattleEv_Push
	b .L_0811f276
.L_0811f26e:
	ldr r1, .L_0811f328
	movs r0, #4
	bl BattleEv_Push
.L_0811f276:
	bl BattleEv_DispatchQueued
	bl Func_0812756c
	b .L_0811f306
.L_0811f280:
	ldr r3, [r7, #100]
	cmp r3, #0
	bne .L_0811f290
	movs r1, #144
	ldr r0, .L_0811f32c
	lsls r1, r1, #3
	bl Func_080145a8
.L_0811f290:
	ldr r3, [r6]
	cmp r3, #0
	beq .L_0811f2be
	mov r3, r10
	cmp r3, #0
	beq .L_0811f2a2
	mov r0, r10
	bl Func_0811e7dc
.L_0811f2a2:
	ldr r3, [r7, #88]
	movs r2, #128
	lsls r2, r2, #7
	ands r3, r2
	cmp r3, #0
	beq .L_0811f2b6
	adds r0, r6, #0
	bl Resource_FarCall00C + 0x8
	b .L_0811f2c2
.L_0811f2b6:
	adds r0, r6, #0
	bl Resource_FarCall00C + 0x18
	b .L_0811f2c2
.L_0811f2be:
	bl Func_0812756c
.L_0811f2c2:
	mov r1, r10
	cmp r1, #0
	beq .L_0811f2d4
	mov r0, r10
	bl Func_0811e830
	mov r0, r10
	bl Sys_Free
.L_0811f2d4:
	ldr r3, [r7, #100]
	cmp r3, #0
	bne .L_0811f2de
	bl Func_081234f0
.L_0811f2de:
	mov r0, r9
	movs r1, #1
	bl Object_SetMode
	ldr r3, [r6, #20]
	movs r4, #0
	cmp r3, #0
	beq .L_0811f306
	movs r7, #36
.L_0811f2f0:
	ldrsh r0, [r6, r7]
	str r4, [sp, #0]
	bl Actor_ResetMotionAtAnchor
	adds r5, r6, #0
	ldr r4, [sp, #0]
	ldr r3, [r5, #20]
	adds r4, #1
	adds r7, #2
	cmp r4, r3
	bne .L_0811f2f0
.L_0811f306:
	movs r0, #0
	add sp, #92
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0811f318:
	.4byte 0xffffe000
.L_0811f31c:
	.4byte 0xffffe800
.L_0811f320:
	.4byte 0xffffdc00
.L_0811f324:
	.4byte 0x00000cad
.L_0811f328:
	.4byte 0x00000cac
.L_0811f32c:
	.4byte Func_08122d10
