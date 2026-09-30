.syntax unified
	.thumb
	.global Func_080fd55c
	.thumb_func
Func_080fd55c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r2, .L_080fd690
	movs r1, #144
	lsls r1, r1, #2
	adds r3, r2, r1
	ldrh r3, [r3]
	sub sp, #20
	adds r5, r0, #0
	cmp r3, #0
	beq .L_080fd596
	adds r1, #2
	adds r3, r2, r1
	ldrh r3, [r3]
	cmp r3, #0
	beq .L_080fd596
	movs r3, #8
	ldr r0, .L_080fd694
	negs r3, r3
	adds r1, r5, #0
	movs r2, #0
	bl UiText_DrawCharacterAtOffsetFar
	b .L_080fd5a4
.L_080fd596:
	movs r3, #8
	ldr r0, .L_080fd698
	negs r3, r3
	adds r1, r5, #0
	movs r2, #0
	bl UiText_DrawCharacterAtOffsetFar
.L_080fd5a4:
	ldr r3, .L_080fd690
	movs r2, #144
	lsls r2, r2, #2
	adds r6, r3, r2
	movs r7, #192
	ldrh r2, [r6]
	lsls r7, r7, #2
	adds r7, #255
	adds r3, r7, #0
	ands r3, r2
	ldr r2, .L_080fd69c
	movs r1, #12
	adds r0, r3, r2
	add r3, sp, #16
	movs r2, #8
	add r1, sp
	add r2, sp
	mov r10, r3
	add r3, sp, #4
	mov r9, r1
	mov r11, r2
	str r3, [sp, #0]
	mov r2, r9
	mov r8, r3
	mov r1, r10
	mov r3, r11
	bl Func_08038108
	ldrh r2, [r6]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_080fd60e
	adds r0, r7, #0
	ands r0, r2
	movs r1, #4
	bl UiText_DrawQuantity
	adds r1, r5, #0
	movs r2, #0
	movs r3, #0
	ldr r0, .L_080fd6a0
	bl UiText_DrawCharacterAtOffsetFar
	ldrh r0, [r6]
	lsrs r0, r0, #10
	bl Owner_GetState
	adds r1, r5, #0
	movs r2, #80
	movs r3, #0
	bl UiText_DrawStringAtOffsetFar
	b .L_080fd61a
.L_080fd60e:
	ldr r0, .L_080fd6a4
	adds r1, r5, #0
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
.L_080fd61a:
	ldr r3, .L_080fd690
	movs r1, #128
	lsls r1, r1, #2
	adds r1, #66
	adds r6, r3, r1
	movs r7, #192
	ldrh r2, [r6]
	lsls r7, r7, #2
	adds r7, #255
	adds r3, r7, #0
	ands r3, r2
	ldr r2, .L_080fd69c
	mov r1, r10
	adds r0, r3, r2
	mov r2, r8
	str r2, [sp, #0]
	mov r3, r11
	mov r2, r9
	bl Func_08038108
	ldrh r2, [r6]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_080fd674
	adds r0, r7, #0
	ands r0, r2
	movs r1, #4
	bl UiText_DrawQuantity
	adds r1, r5, #0
	movs r2, #0
	movs r3, #8
	ldr r0, .L_080fd6a8
	bl UiText_DrawCharacterAtOffsetFar
	ldrh r0, [r6]
	lsrs r0, r0, #10
	bl Owner_GetState
	adds r1, r5, #0
	movs r2, #80
	movs r3, #8
	bl UiText_DrawStringAtOffsetFar
	b .L_080fd680
.L_080fd674:
	ldr r0, .L_080fd6ac
	adds r1, r5, #0
	movs r2, #0
	movs r3, #8
	bl UiText_DrawCharacterAtOffsetFar
.L_080fd680:
	movs r0, #1
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080fd690:
	.4byte gPartyState
.L_080fd694:
	.4byte 0x00001013
.L_080fd698:
	.4byte 0x0000100f
.L_080fd69c:
	.4byte 0x000005a7
.L_080fd6a0:
	.4byte 0x00001016
.L_080fd6a4:
	.4byte 0x00001014
.L_080fd6a8:
	.4byte 0x00001017
.L_080fd6ac:
	.4byte 0x00001015
