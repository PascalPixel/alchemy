.syntax unified
	.thumb
	.global Func_080a6614
	.thumb_func
Func_080a6614:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r2, .L_080a676c
	movs r1, #136
	lsls r1, r1, #2
	adds r3, r2, r1
	ldrh r3, [r3]
	sub sp, #20
	adds r5, r0, #0
	cmp r3, #0
	beq .L_080a664e
	adds r1, #2
	adds r3, r2, r1
	ldrh r3, [r3]
	cmp r3, #0
	beq .L_080a664e
	movs r3, #8
	ldr r0, .L_080a6770
	negs r3, r3
	adds r1, r5, #0
	movs r2, #0
	bl UiText_DrawCharacterAtOffsetFar
	b .L_080a665c
.L_080a664e:
	movs r3, #8
	ldr r0, .L_080a6774
	negs r3, r3
	adds r1, r5, #0
	movs r2, #0
	bl UiText_DrawCharacterAtOffsetFar
.L_080a665c:
	ldr r3, .L_080a676c
	movs r2, #136
	lsls r2, r2, #2
	adds r3, r3, r2
	ldrh r3, [r3]
	ldr r0, .L_080a6778
	ands r0, r3
	ldr r3, .L_080a677c
	adds r0, r0, r3
	add r3, sp, #16
	movs r1, #12
	movs r2, #8
	add r1, sp
	add r2, sp
	mov r10, r3
	add r3, sp, #4
	mov r9, r1
	mov r11, r2
	str r3, [sp, #0]
	mov r8, r3
	mov r1, r10
	mov r3, r11
	mov r2, r9
	bl UiText_GetResourceDimensionsFar
	ldr r3, [sp, #8]
	movs r6, #1
	cmp r3, #10
	bhi .L_080a6698
	movs r6, #0
.L_080a6698:
	ldr r3, .L_080a676c
	movs r1, #136
	lsls r1, r1, #2
	adds r7, r3, r1
	ldrh r2, [r7]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_080a66d6
	ldr r0, .L_080a6778
	movs r1, #4
	ands r0, r2
	bl UiText_DrawQuantity
	ldr r0, .L_080a6780
	adds r1, r5, #0
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
	cmp r6, #0
	bne .L_080a66e2
	ldrh r0, [r7]
	lsrs r0, r0, #10
	bl Owner_GetStateFar
	adds r1, r5, #0
	movs r2, #80
	movs r3, #0
	bl UiText_DrawStringAtOffsetFar
	b .L_080a66e2
.L_080a66d6:
	ldr r0, .L_080a6784
	adds r1, r5, #0
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
.L_080a66e2:
	ldr r3, .L_080a676c
	ldr r2, .L_080a6788
	adds r3, r3, r2
	ldrh r3, [r3]
	ldr r0, .L_080a6778
	ands r0, r3
	ldr r3, .L_080a677c
	adds r0, r0, r3
	mov r3, r8
	str r3, [sp, #0]
	mov r1, r10
	mov r3, r11
	mov r2, r9
	bl UiText_GetResourceDimensionsFar
	ldr r3, [sp, #8]
	movs r6, #1
	cmp r3, #10
	bhi .L_080a670a
	movs r6, #0
.L_080a670a:
	ldr r3, .L_080a676c
	ldr r1, .L_080a6788
	adds r7, r3, r1
	ldrh r2, [r7]
	adds r3, r2, #0
	cmp r3, #0
	beq .L_080a674c
	ldr r0, .L_080a6778
	movs r1, #4
	ands r0, r2
	bl UiText_DrawQuantity
	ldr r0, .L_080a678c
	adds r1, r5, #0
	movs r2, #0
	movs r3, #8
	bl UiText_DrawCharacterAtOffsetFar
	cmp r6, #0
	bne .L_080a6744
	ldrh r0, [r7]
	lsrs r0, r0, #10
	bl Owner_GetStateFar
	adds r1, r5, #0
	movs r2, #80
	movs r3, #8
	bl UiText_DrawStringAtOffsetFar
.L_080a6744:
	movs r0, #15
	bl UiWork_SetParamNibbleFar
	b .L_080a6758
.L_080a674c:
	ldr r0, .L_080a6790
	adds r1, r5, #0
	movs r2, #0
	movs r3, #8
	bl UiText_DrawCharacterAtOffsetFar
.L_080a6758:
	movs r0, #1
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
.L_080a676c:
	.4byte gCell
.L_080a6770:
	.4byte 0x00000ae4
.L_080a6774:
	.4byte 0x00000ae0
.L_080a6778:
	.4byte 0x000003ff
.L_080a677c:
	.4byte 0x00000333
.L_080a6780:
	.4byte 0x00000ae7
.L_080a6784:
	.4byte 0x00000ae5
.L_080a6788:
	.4byte 0x00000222
.L_080a678c:
	.4byte 0x00000ae8
.L_080a6790:
	.4byte 0x00000ae6
