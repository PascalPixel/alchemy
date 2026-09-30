.syntax unified
	.thumb
	.global Menu_ResolveSelectedAction
	.thumb_func
Menu_ResolveSelectedAction:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #12
	movs r5, #0
	str r0, [sp, #8]
	str r2, [sp, #4]
	str r5, [sp, #0]
	ldr r3, .L_080a5e44
	ldr r7, [r3]
	mov r11, r5
	b .L_080a5fa4
.L_080a5ce0:
	cmp r5, #4
	bls .L_080a5ce6
	b .L_080a5fa0
.L_080a5ce6:
	ldr r2, .L_080a5e48
	lsls r3, r5, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_080a5cf0:
	.4byte .L_080a5d04
	.4byte .L_080a5d34
	.4byte .L_080a5e22
	.4byte .L_080a5dfa
	.4byte .L_080a5e92
.L_080a5d04:
	movs r3, #186
	lsls r3, r3, #1
	adds r2, r7, r3
	movs r3, #0
	strh r3, [r2]
	ldr r1, .L_080a5e4c
	movs r0, #0
	bl ItemMenu_DrawMsg
	movs r0, #0
	bl PsynergyMenu_SelectPartySlot
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	bne .L_080a5d2a
	movs r2, #1
	str r2, [sp, #0]
	mov r11, r3
.L_080a5d2a:
	ldr r0, [r7, #44]
	bl RenderOutput_RedrawSavedRectFar
	movs r5, #1
	b .L_080a5fa4
.L_080a5d34:
	movs r0, #1
	bl WaitFrames
	ldr r2, .L_080a5e50
	adds r3, r7, r2
	ldrb r0, [r3]
	bl Owner_GetStateFar
	movs r2, #134
	lsls r2, r2, #2
	adds r3, r7, r2
	ldrb r3, [r3]
	movs r5, #0
	cmp r3, #0
	bne .L_080a5d54
	b .L_080a5fa4
.L_080a5d54:
	movs r2, #154
	lsls r2, r2, #2
	adds r3, r7, r2
	ldrb r3, [r3]
	cmp r3, #1
	beq .L_080a5d74
	cmp r3, #1
	bgt .L_080a5d6a
	cmp r3, #0
	beq .L_080a5d70
	b .L_080a5d86
.L_080a5d6a:
	cmp r3, #2
	beq .L_080a5d7e
	b .L_080a5d86
.L_080a5d70:
	ldr r1, .L_080a5e54
	b .L_080a5d76
.L_080a5d74:
	ldr r1, .L_080a5e58
.L_080a5d76:
	movs r0, #0
	bl ItemMenu_DrawMsg
	b .L_080a5d86
.L_080a5d7e:
	ldr r1, .L_080a5e5c
	movs r0, #0
	bl ItemMenu_DrawMsg
.L_080a5d86:
	bl ItemMenu_PosCategory
	ldr r3, .L_080a5e50
	adds r6, r7, r3
	ldrb r1, [r6]
	movs r2, #0
	ldr r0, [r7, #36]
	movs r3, #0
	bl Menu_DrawOwnerStatusPanel
	movs r0, #0
	bl PsynergyMenu_RunList
	movs r2, #1
	negs r2, r2
	adds r1, r0, #0
	mov r8, r2
	movs r5, #0
	cmp r1, r8
	bne .L_080a5db0
	b .L_080a5fa4
.L_080a5db0:
	movs r2, #154
	lsls r2, r2, #2
	adds r3, r7, r2
	ldrb r3, [r3]
	movs r5, #2
	cmp r3, #0
	bne .L_080a5dc0
	b .L_080a5fa4
.L_080a5dc0:
	cmp r3, #1
	bne .L_080a5dde
	movs r2, #0
	ldrb r0, [r6]
	bl PsynergyMenu_SetShortcut
	ldr r0, [r7, #44]
	bl RenderOutput_ClearListFar
	ldr r0, .L_080a5e60
	mov r1, r8
	mov r2, r8
	bl InventoryMenu_ShowModalMessage
	b .L_080a5df6
.L_080a5dde:
	movs r2, #1
	ldrb r0, [r6]
	bl PsynergyMenu_SetShortcut
	ldr r0, [r7, #44]
	bl RenderOutput_ClearListFar
	ldr r0, .L_080a5e64
	mov r1, r8
	mov r2, r8
	bl InventoryMenu_ShowModalMessage
.L_080a5df6:
	movs r5, #0
	b .L_080a5fa4
.L_080a5dfa:
	ldr r1, .L_080a5e68
	movs r0, #0
	bl ItemMenu_DrawMsg
	movs r0, #0
	bl PsynergyMenu_SelectTarget
	movs r3, #1
	mov r10, r0
	negs r3, r3
	movs r5, #4
	cmp r10, r3
	beq .L_080a5e16
	b .L_080a5fa4
.L_080a5e16:
	movs r2, #136
	lsls r2, r2, #2
	adds r1, r7, r2
	ldrh r2, [r1]
	ldr r3, .L_080a5e40
	b .L_080a5f58
.L_080a5e22:
	bl PsynergyMenu_ClassifySelectedPsynergy
	cmp r0, #1
	bne .L_080a5e2e
.L_080a5e2a:
	movs r5, #3
	b .L_080a5fa4
.L_080a5e2e:
	cmp r0, #2
	bne .L_080a5e70
	ldr r3, .L_080a5e6c
	adds r2, r7, r3
	movs r3, #9
	strb r3, [r2]
	movs r5, #4
	b .L_080a5fa4
	.2byte 0x0000
.L_080a5e40:
	.4byte 0x00000001
.L_080a5e44:
	.4byte gMenuWork
.L_080a5e48:
	.4byte .L_080a5cf0
.L_080a5e4c:
	.4byte 0x00000ae9
.L_080a5e50:
	.4byte 0x0000021a
.L_080a5e54:
	.4byte 0x00000aea
.L_080a5e58:
	.4byte 0x00000af1
.L_080a5e5c:
	.4byte 0x00000af0
.L_080a5e60:
	.4byte 0x00000ae2
.L_080a5e64:
	.4byte 0x00000ae3
.L_080a5e68:
	.4byte 0x00000aeb
.L_080a5e6c:
	.4byte 0x0000021b
.L_080a5e70:
	movs r2, #1
	str r2, [sp, #0]
	mov r11, r2
	ldr r2, .L_080a5f88
	adds r3, r7, r2
	ldrb r3, [r3]
	ldr r2, [sp, #8]
	str r3, [r2]
	movs r2, #188
	lsls r2, r2, #1
	adds r3, r7, r2
	ldrh r2, [r3]
	ldr r3, .L_080a5f8c
	ands r3, r2
	ldr r2, [sp, #4]
	str r3, [r2]
	b .L_080a5fa4
.L_080a5e92:
	movs r2, #188
	lsls r2, r2, #1
	adds r2, r2, r7
	movs r3, #0
	ldrh r0, [r2]
	mov r10, r3
	mov r8, r2
	ldr r3, .L_080a5f88
	ldr r2, .L_080a5f90
	adds r5, r7, r3
	adds r6, r7, r2
	movs r3, #0
	ldrb r1, [r5]
	ldrb r2, [r6]
	bl BattleEffect_ApplyToTargets
	ldrb r3, [r6]
	mov r11, r0
	cmp r3, #9
	bne .L_080a5ec2
	ldrb r3, [r5]
	strb r3, [r6]
	movs r3, #9
	mov r10, r3
.L_080a5ec2:
	movs r2, #1
	negs r2, r2
	mov r9, r2
	cmp r11, r9
	beq .L_080a5ee4
	mov r2, r8
	ldrh r3, [r2]
	ldr r0, .L_080a5f8c
	ands r0, r3
	bl Ability_GetData
	ldrb r3, [r5]
	ldrb r1, [r0, #9]
	adds r0, r3, #0
	negs r1, r1
	bl Owner_AdjustSecondValueFar
.L_080a5ee4:
	ldrb r0, [r5]
	bl Owner_RecalculateStatsFar
	cmp r11, r9
	beq .L_080a5f22
	ldrb r1, [r6]
	ldr r0, [r7, #36]
	movs r2, #0
	movs r3, #0
	bl Menu_DrawOwnerStatusPanel
	mov r2, r8
	ldrh r3, [r2]
	ldr r0, .L_080a5f8c
	ands r0, r3
	bl Ability_PlayUseAnimation
	ldr r0, [r7, #44]
	bl RenderOutput_ClearListFar
	ldr r2, .L_080a5f94
	adds r3, r7, r2
	movs r2, #0
	ldrsh r0, [r3, r2]
	ldr r3, .L_080a5f98
	movs r1, #0
	adds r0, r0, r3
	mov r2, r9
	bl InventoryMenu_ShowModalMessage
	b .L_080a5f42
.L_080a5f22:
	movs r0, #114
	bl AudioCommand_PlayFar
	ldr r0, [r7, #44]
	bl RenderOutput_ClearListFar
	ldr r2, .L_080a5f94
	adds r3, r7, r2
	movs r2, #0
	ldrsh r0, [r3, r2]
	ldr r3, .L_080a5f98
	mov r1, r11
	adds r0, r0, r3
	mov r2, r11
	bl InventoryMenu_ShowModalMessage
.L_080a5f42:
	movs r3, #1
	negs r3, r3
	cmp r11, r3
	beq .L_080a5f60
	movs r3, #136
	lsls r3, r3, #2
	adds r1, r7, r3
	movs r2, #1
	mov r11, r2
	ldr r3, .L_080a5f84
	ldrh r2, [r1]
.L_080a5f58:
	orrs r3, r2
	strh r3, [r1]
	movs r5, #1
	b .L_080a5fa4
.L_080a5f60:
	ldr r3, .L_080a5f9c
	adds r2, r7, r3
	movs r3, #1
	strh r3, [r2]
	mov r2, r10
	ldr r1, .L_080a5f84
	cmp r2, #9
	beq .L_080a5f72
	b .L_080a5e2a
.L_080a5f72:
	movs r3, #136
	lsls r3, r3, #2
	adds r2, r7, r3
	ldrh r3, [r2]
	orrs r3, r1
	strh r3, [r2]
	movs r5, #1
	b .L_080a5fa4
	.2byte 0x0000
.L_080a5f84:
	.4byte 0x00000001
.L_080a5f88:
	.4byte 0x0000021a
.L_080a5f8c:
	.4byte 0x00003fff
.L_080a5f90:
	.4byte 0x0000021b
.L_080a5f94:
	.4byte 0x0000025a
.L_080a5f98:
	.4byte 0x00000bef
.L_080a5f9c:
	.4byte 0x00000222
.L_080a5fa0:
	movs r2, #1
	str r2, [sp, #0]
.L_080a5fa4:
	ldr r3, [sp, #0]
	cmp r3, #0
	bne .L_080a5fb8
	movs r0, #168
	lsls r0, r0, #1
	bl GameFlag_IsSet
	cmp r0, #0
	bne .L_080a5fb8
	b .L_080a5ce0
.L_080a5fb8:
	movs r0, #168
	lsls r0, r0, #1
	bl GameFlag_IsSet
	cmp r0, #0
	beq .L_080a5fca
	movs r2, #1
	negs r2, r2
	mov r11, r2
.L_080a5fca:
	mov r0, r11
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
	.2byte 0x0000
