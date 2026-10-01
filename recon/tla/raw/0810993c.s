.syntax unified
	.thumb
	.global Func_0810993c
	.thumb_func
Func_0810993c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #220
	ldr r3, [r3]
	mov r9, r1
	mov r11, r3
	mov r8, r0
	bl Owner_GetState
	mov r3, r9
	lsls r5, r3, #1
	adds r7, r0, #0
	adds r5, #216
	ldrh r3, [r7, r5]
	movs r6, #128
	lsls r6, r6, #1
	adds r6, #255
	ands r6, r3
	adds r0, r6, #0
	bl Item_Get
	ldrh r2, [r7, r5]
	movs r3, #128
	lsls r3, r3, #2
	ands r3, r2
	mov r10, r0
	movs r0, #0
	cmp r3, #0
	bne .L_08109a24
	mov r0, r8
	adds r1, r6, #0
	bl Item_CanOwnerEquip
	cmp r0, #0
	beq .L_081099d0
	mov r3, r10
	ldrb r1, [r3, #2]
	mov r0, r8
	bl Inventory_FindEquippedFar
	movs r3, #1
	negs r3, r3
	cmp r0, r3
	beq .L_081099b8
	lsls r3, r0, #1
	adds r3, #216
	ldrh r0, [r7, r3]
	bl Item_Get
	ldrb r2, [r0, #3]
	movs r3, #2
	ands r3, r2
	movs r0, #0
	cmp r3, #0
	bne .L_08109a24
.L_081099b8:
	mov r0, r8
	movs r1, #1
	bl UiText_DrawQuantity
	ldr r0, .L_08109a30
	bl Func_081084f4
	movs r0, #0
	bl Func_08108630
	cmp r0, #0
	beq .L_081099d4
.L_081099d0:
	movs r0, #0
	b .L_08109a24
.L_081099d4:
	mov r0, r8
	mov r1, r9
	bl Inventory_EquipFar
	mov r3, r11
	ldr r0, [r3, #36]
	cmp r0, #0
	beq .L_081099ea
	mov r1, r8
	bl Func_0810a004
.L_081099ea:
	mov r3, r10
	ldrb r2, [r3, #3]
	movs r3, #1
	ands r3, r2
	cmp r3, #0
	beq .L_08109a1c
	movs r0, #103
	bl Audio_PlayCue
	bl UiWork_FinalizePendingCoreFar
	movs r1, #8
	movs r2, #4
	movs r3, #2
	ldr r0, .L_08109a34
	bl UiText_OpenMessageWindowFar
	b .L_08109a14
.L_08109a0e:
	movs r0, #1
	bl WaitFrames
.L_08109a14:
	bl UiWork_IsCompleteFar
	cmp r0, #0
	beq .L_08109a0e
.L_08109a1c:
	ldr r0, .L_08109a38
	bl Func_0810857c
	movs r0, #1
.L_08109a24:
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_08109a30:
	.4byte 0x00001253
.L_08109a34:
	.4byte 0x00000fff
.L_08109a38:
	.4byte 0x00001254
