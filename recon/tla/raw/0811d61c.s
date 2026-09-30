.syntax unified
	.thumb
	.global Func_0811d61c
	.thumb_func
Func_0811d61c:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #36]
	movs r4, #187
	movs r1, #128
	lsls r4, r4, #2
	adds r7, r0, #0
	movs r2, #0
	movs r0, #255
	lsls r1, r1, #8
	adds r3, r5, r4
.L_0811d634:
	adds r2, #1
	strh r0, [r3]
	strh r1, [r3, #4]
	adds r3, #16
	cmp r2, #19
	bls .L_0811d634
	bl BattleUnit_ClearField12bForGroup
	movs r0, #8
	bl Palette_CopyBanksWithBrightnessOffset
	movs r0, #108
	adds r0, #255
	bl GameFlag_SetBit
	adds r5, #69
	movs r0, #0
	bl Camera_ConfigureScene
	bl UiWork_FinalizeSharedSlotFar
	ldrb r3, [r5]
	cmp r3, #2
	beq .L_0811d68a
	adds r0, r7, #0
	bl BattlePres_BuildUnitEntries
	adds r6, r0, #0
	cmp r6, #0
	blt .L_0811d702
	cmp r6, #0
	beq .L_0811d68c
	movs r1, #6
	ldrsh r3, [r7, r1]
	cmp r3, #99
	bne .L_0811d68c
	bl BattleEscape_CheckSuccess
	cmp r0, #0
	bne .L_0811d68c
	movs r3, #2
	strb r3, [r5]
	b .L_0811d68c
.L_0811d68a:
	movs r6, #0
.L_0811d68c:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	adds r3, #68
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0811d6b8
	adds r0, r7, #0
	adds r1, r6, #0
	bl Func_0811d414
	adds r5, r0, #0
	bl Func_081192d0
	cmp r0, #0
	blt .L_0811d6b2
	adds r6, r6, r5
	cmp r5, #0
	bge .L_0811d6c2
.L_0811d6b2:
	movs r6, #1
	negs r6, r6
	b .L_0811d702
.L_0811d6b8:
	lsls r0, r6, #4
	adds r0, r7, r0
	bl Func_0811cfd0
	adds r6, r6, r0
.L_0811d6c2:
	adds r0, r7, #0
	adds r1, r6, #0
	bl Func_0811d160
	cmp r6, #0
	ble .L_0811d702
	adds r5, r7, #0
	adds r7, r6, #0
.L_0811d6d2:
	movs r2, #6
	ldrsh r3, [r5, r2]
	cmp r3, #3
	beq .L_0811d6de
	cmp r3, #7
	bne .L_0811d6fa
.L_0811d6de:
	movs r3, #0
	ldrsh r0, [r5, r3]
	bl Owner_GetState
	movs r4, #6
	ldrsh r3, [r5, r4]
	movs r2, #1
	cmp r3, #3
	beq .L_0811d6f2
	movs r2, #3
.L_0811d6f2:
	movs r1, #44
	adds r1, #255
	adds r3, r0, r1
	strb r2, [r3]
.L_0811d6fa:
	subs r7, #1
	adds r5, #16
	cmp r7, #0
	bne .L_0811d6d2
.L_0811d702:
	movs r0, #108
	adds r0, #255
	bl GameFlag_ClearBit
	bl Func_0811bddc
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #176
	ldr r2, [r3]
	movs r3, #128
	lsls r3, r3, #6
	str r3, [r2]
	adds r0, r6, #0
	pop {r5, r6, r7, pc}
