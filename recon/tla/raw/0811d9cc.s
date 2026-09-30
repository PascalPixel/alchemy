.syntax unified
	.thumb
	.global Func_0811d9cc
	.thumb_func
Func_0811d9cc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r5, #192
	sub sp, #12
	lsls r5, r5, #18
	movs r2, #0
	adds r7, r1, #0
	ldr r1, [r5, #36]
	str r2, [sp, #4]
	mov r11, r0
	movs r3, #0
	ldrsh r0, [r0, r3]
	mov r10, r1
	cmp r0, #255
	bne .L_0811d9f8
	movs r0, #0
	b .L_0811ddb2
.L_0811d9f8:
	bl Owner_GetState
	movs r1, #56
	ldrsh r3, [r0, r1]
	cmp r3, #0
	bne .L_0811da0a
	movs r0, #1
	negs r0, r0
	b .L_0811ddb2
.L_0811da0a:
	movs r3, #128
	lsls r3, r3, #4
	adds r3, #105
	add r3, r10
	movs r2, #3
	strb r2, [r3]
	movs r2, #42
	adds r2, #255
	adds r3, r0, r2
	ldrb r3, [r3]
	cmp r3, #0
	bne .L_0811da2a
	mov r0, r11
	movs r1, #1
	bl Func_08122514
.L_0811da2a:
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #176
	ldr r6, [r3]
	movs r3, #60
	str r3, [r6, #4]
	ldr r3, [sp, #4]
	movs r2, #192
	lsls r2, r2, #3
	str r3, [r6, #20]
	ldr r5, [r5, #48]
	adds r2, #108
	movs r3, #128
	add r2, r10
	lsls r3, r3, #9
	str r3, [r2]
	bl Func_08014de4
	adds r1, r5, #0
	adds r1, #12
	adds r0, r5, #0
	bl Graphics_PrepareTransferInIwramWork
	movs r0, #255
	movs r1, #192
	lsls r1, r1, #8
	ldr r3, .L_0811ddc0
	lsls r0, r0, #17
	mov lr, r3
	.2byte 0xf800
	adds r1, r0, #0
	movs r0, #255
	lsls r0, r0, #17
	ldr r2, .L_0811ddc4
	bl Camera_StoreSceneParameters
	cmp r7, #0
	beq .L_0811da82
	movs r3, #128
	lsls r3, r3, #6
	str r3, [r6]
	adds r0, r7, #0
	bl WaitFrames
.L_0811da82:
	mov r2, r11
	movs r1, #6
	ldrsh r3, [r2, r1]
	cmp r3, #10
	bne .L_0811da9e
	movs r1, #192
	lsls r1, r1, #3
	adds r1, #124
	add r1, r10
	mov r0, r11
	bl Func_0811d7e8
	movs r0, #0
	b .L_0811dabe
.L_0811da9e:
	mov r1, r11
	ldrh r3, [r1]
	add r0, sp, #8
	strh r3, [r0]
	movs r3, #255
	strh r3, [r0, #2]
	movs r1, #1
	bl BattlePres_SetActorModes
	movs r1, #192
	lsls r1, r1, #3
	adds r1, #124
	add r1, r10
	mov r0, r11
	bl Func_0812381c
.L_0811dabe:
	cmp r0, #0
	beq .L_0811dac4
	b .L_0811dd56
.L_0811dac4:
	movs r3, #218
	lsls r3, r3, #3
	add r3, r10
	ldr r3, [r3]
	cmp r3, #17
	bls .L_0811dad2
	b .L_0811dd70
.L_0811dad2:
	ldr r2, .L_0811ddc8
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_0811dadc:
	.4byte .L_0811dd70
	.4byte .L_0811db24
	.4byte .L_0811db34
	.4byte .L_0811db84
	.4byte .L_0811dbae
	.4byte .L_0811db54
	.4byte .L_0811db94
	.4byte .L_0811dd3a
	.4byte .L_0811db9e
	.4byte .L_0811db64
	.4byte .L_0811dbc0
	.4byte .L_0811dc9a
	.4byte .L_0811dcea
	.4byte .L_0811dd1a
	.4byte .L_0811db74
	.4byte .L_0811db44
	.4byte .L_0811db9e
	.4byte .L_0811db84
.L_0811db24:
	movs r0, #192
	lsls r0, r0, #3
	adds r0, #124
	add r0, r10
	movs r1, #0
	bl BattlePres_RunActorEntries
	b .L_0811dd70
.L_0811db34:
	movs r0, #192
	lsls r0, r0, #3
	adds r0, #124
	add r0, r10
	movs r1, #0
	bl Func_0811ea0c
	b .L_0811dd70
.L_0811db44:
	movs r0, #192
	lsls r0, r0, #3
	adds r0, #124
	add r0, r10
	movs r1, #2
	bl Func_0811ea0c
	b .L_0811dd70
.L_0811db54:
	movs r0, #192
	lsls r0, r0, #3
	adds r0, #124
	add r0, r10
	movs r1, #1
	bl Func_0811df70
	b .L_0811dd70
.L_0811db64:
	movs r0, #192
	lsls r0, r0, #3
	adds r0, #124
	add r0, r10
	movs r1, #0
	bl Func_0811df70
	b .L_0811dd70
.L_0811db74:
	movs r0, #192
	lsls r0, r0, #3
	adds r0, #124
	add r0, r10
	movs r1, #3
	bl Func_0811df70
	b .L_0811dd70
.L_0811db84:
	movs r0, #192
	lsls r0, r0, #3
	adds r0, #124
	add r0, r10
	movs r1, #0
	bl Func_0811f088
	b .L_0811dd70
.L_0811db94:
	movs r0, #192
	lsls r0, r0, #3
	adds r0, #124
	add r0, r10
	b .L_0811dd32
.L_0811db9e:
	movs r0, #192
	lsls r0, r0, #3
	adds r0, #124
	add r0, r10
	movs r1, #2
	bl Func_0811f088
	b .L_0811dd70
.L_0811dbae:
	movs r0, #192
	lsls r0, r0, #3
	adds r0, #124
	add r0, r10
	movs r1, #0
	mov r2, r11
	bl Func_0811edb0
	b .L_0811dd70
.L_0811dbc0:
	movs r2, #176
	lsls r2, r2, #3
	movs r3, #128
	adds r2, #255
	lsls r3, r3, #4
	add r2, r10
	adds r3, #78
	ldrb r4, [r2]
	mov r9, r2
	add r3, r10
	movs r2, #1
	movs r5, #192
	strh r2, [r3]
	lsls r5, r5, #3
	adds r5, #126
	add r5, r10
	movs r7, #192
	ldrb r3, [r5]
	movs r6, #220
	lsls r7, r7, #3
	movs r1, #0
	lsls r6, r6, #3
	adds r7, #124
	add r7, r10
	mov r8, r1
	add r6, r10
	mov r1, r9
	strb r3, [r1]
	adds r0, r7, #0
	movs r1, #1
	str r2, [r6]
	str r4, [sp, #0]
	bl Func_0811f088
	ldr r4, [sp, #0]
	mov r2, r8
	mov r3, r9
	str r2, [r6]
	strb r4, [r3]
	ldrb r4, [r7]
	ldrb r3, [r5]
	strb r3, [r7]
	strb r4, [r5]
	ldrb r0, [r7]
	bl Owner_GetState
	movs r1, #165
	lsls r1, r1, #1
	adds r3, r0, r1
	ldrh r0, [r3]
	bl Func_0811d79c
	movs r3, #192
	lsls r3, r3, #3
	adds r3, #212
	add r3, r10
	str r0, [r3]
	mov r1, r9
	ldrb r3, [r7]
	ldrb r2, [r1]
	eors r3, r2
	lsrs r3, r3, #7
	cmp r3, #0
	bne .L_0811dc60
	movs r1, #1
	ldrb r0, [r7]
	bl UiText_DrawQuantity
	mov r1, r9
	ldrb r2, [r7]
	ldrb r3, [r1]
	cmp r2, r3
	bne .L_0811dc5a
	ldr r0, .L_0811ddcc
	bl UiText_ShowMessageAndWaitCoreFar
	b .L_0811dc60
.L_0811dc5a:
	ldr r0, .L_0811ddd0
	bl UiText_ShowMessageAndWaitCoreFar
.L_0811dc60:
	movs r3, #217
	lsls r3, r3, #3
	add r3, r10
	movs r0, #192
	ldr r3, [r3]
	lsls r0, r0, #3
	movs r2, #156
	adds r0, #124
	lsls r2, r2, #1
	add r0, r10
	movs r1, #32
	cmp r3, r2
	beq .L_0811dc7c
	movs r1, #0
.L_0811dc7c:
	bl Func_0811ea0c
	movs r1, #192
	movs r2, #192
	lsls r1, r1, #3
	lsls r2, r2, #3
	adds r1, #124
	adds r2, #126
	add r1, r10
	add r2, r10
	ldrb r4, [r1]
	ldrb r3, [r2]
	strb r3, [r1]
	strb r4, [r2]
	b .L_0811dd70
.L_0811dc9a:
	movs r5, #220
	movs r0, #192
	lsls r5, r5, #3
	lsls r0, r0, #3
	movs r3, #1
	add r5, r10
	adds r0, #124
	str r3, [r5]
	movs r1, #1
	add r0, r10
	bl Func_0811f088
	ldr r6, .L_0811ddd4
	movs r3, #0
	str r3, [r5]
	adds r0, r6, #0
	bl UiText_ShowMessageAndWaitCoreFar
	movs r3, #176
	lsls r3, r3, #3
	adds r3, #255
	add r3, r10
	ldrb r3, [r3]
	movs r0, #0
	cmp r3, #7
	bls .L_0811dcd0
	movs r0, #1
.L_0811dcd0:
	bl Func_08124cc4
	adds r5, r0, #0
	cmp r5, #0
	bne .L_0811dce0
	adds r0, r6, #1
	bl UiText_ShowMessageAndWaitCoreFar
.L_0811dce0:
	cmp r5, #1
	bgt .L_0811dd70
	bl BattlePresentation_WaitForAdvance
	b .L_0811dd70
.L_0811dcea:
	movs r5, #192
	lsls r5, r5, #3
	adds r5, #124
	add r5, r10
	movs r1, #1
	adds r0, r5, #0
	bl Func_0811df70
	movs r0, #1
	movs r1, #0
	bl BattleParty_ListLivingUnits
	cmp r0, #0
	beq .L_0811dd70
	movs r0, #2
	movs r1, #0
	bl BattleParty_ListLivingUnits
	cmp r0, #0
	beq .L_0811dd70
	adds r0, r5, #0
	bl Func_0811d89c
	b .L_0811dd30
.L_0811dd1a:
	movs r5, #192
	lsls r5, r5, #3
	adds r5, #124
	add r5, r10
	movs r1, #1
	adds r0, r5, #0
	bl Func_0811df70
	adds r0, r5, #0
	bl Func_0811d914
.L_0811dd30:
	adds r0, r5, #0
.L_0811dd32:
	movs r1, #1
	bl Func_0811f088
	b .L_0811dd70
.L_0811dd3a:
	movs r0, #192
	lsls r0, r0, #3
	adds r0, #124
	add r0, r10
	bl Func_0811de78
	cmp r0, #0
	beq .L_0811dd4e
	movs r3, #1
	str r3, [sp, #4]
.L_0811dd4e:
	ldr r1, [sp, #4]
	cmp r1, #0
	beq .L_0811dd70
	b .L_0811dd92
.L_0811dd56:
	movs r2, #1
	negs r2, r2
	cmp r0, r2
	bne .L_0811dd68
	bl BattlePresentation_WaitForAdvance
	movs r0, #3
	bl WaitFrames
.L_0811dd68:
	movs r0, #0
	movs r1, #0
	bl BattlePres_SetActorModes
.L_0811dd70:
	bl Func_0811bc98
	movs r0, #192
	lsls r0, r0, #3
	adds r0, #124
	add r0, r10
	bl Func_0812561c
	bl BattleActor_CommitPlacement
	bl Summon_Refresh
	bl Func_0811d9a4
	movs r3, #255
	mov r1, r11
	strh r3, [r1]
.L_0811dd92:
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #36]
	movs r2, #206
	lsls r2, r2, #3
	adds r3, r3, r2
	ldrh r1, [r3]
	movs r0, #2
	movs r2, #0
	bl Func_0812628c
	movs r0, #195
	lsls r0, r0, #1
	bl Audio_PlayCue
	ldr r0, [sp, #4]
.L_0811ddb2:
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_0811ddc0:
	.4byte IwramRatioMulQ14
.L_0811ddc4:
	.4byte 0x7fff0000
.L_0811ddc8:
	.4byte .L_0811dadc
.L_0811ddcc:
	.4byte 0x00000c8e
.L_0811ddd0:
	.4byte 0x00000c8d
.L_0811ddd4:
	.4byte 0x00000ce1
