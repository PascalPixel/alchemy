.syntax unified
	.thumb
	.global Func_080fb8b8
	.thumb_func
Func_080fb8b8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	mov r8, r0
	movs r0, #128
	lsls r0, r0, #1
	adds r0, #255
	sub sp, #12
	movs r5, #0
	ands r0, r1
	mov r11, r1
	str r5, [sp, #8]
	bl Item_Get
	ldrb r3, [r0, #2]
	mov r10, r0
	cmp r3, #0
	beq .L_080fb990
	ldr r3, [r0, #8]
	ldr r2, .L_080fbc04
	movs r1, #0
	ands r3, r2
	cmp r3, #0
	bne .L_080fb910
	mov r2, r10
	mov r9, r1
	adds r2, #24
.L_080fb8f6:
	mov r0, r9
	cmp r0, #3
	bgt .L_080fb912
	ldrb r3, [r2]
	cmp r3, #0
	bne .L_080fb910
	mov r0, r10
	ldrb r3, [r0, #12]
	movs r0, #1
	adds r2, #4
	add r9, r0
	cmp r3, #3
	bne .L_080fb8f6
.L_080fb910:
	movs r1, #1
.L_080fb912:
	cmp r1, #1
	bne .L_080fb924
	ldr r0, .L_080fbc08
	mov r1, r8
	movs r2, #8
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
	movs r5, #1
.L_080fb924:
	mov r2, r10
	movs r1, #8
	ldrsh r3, [r2, r1]
	cmp r3, #0
	beq .L_080fb95a
	lsls r5, r5, #24
	asrs r5, r5, #24
	lsls r6, r5, #3
	adds r3, r6, #0
	ldr r0, .L_080fbc0c
	mov r1, r8
	movs r2, #0
	bl UiText_DrawCharacterAtOffsetFar
	mov r0, r10
	movs r3, #8
	ldrsh r7, [r0, r3]
	movs r1, #3
	adds r0, r7, #0
	mov r2, r8
	movs r3, #72
	str r6, [sp, #0]
	bl ItemMenu_DrawStat
	adds r5, #1
	lsls r5, r5, #24
	lsrs r5, r5, #24
.L_080fb95a:
	mov r1, r10
	movs r3, #10
	ldrsb r3, [r1, r3]
	cmp r3, #0
	beq .L_080fb990
	lsls r5, r5, #24
	asrs r5, r5, #24
	lsls r6, r5, #3
	ldr r0, .L_080fbc10
	mov r1, r8
	movs r2, #0
	adds r3, r6, #0
	bl UiText_DrawCharacterAtOffsetFar
	mov r2, r10
	movs r7, #10
	ldrsb r7, [r2, r7]
	movs r1, #3
	adds r0, r7, #0
	mov r2, r8
	movs r3, #72
	str r6, [sp, #0]
	bl ItemMenu_DrawStat
	adds r5, #1
	lsls r5, r5, #24
	lsrs r5, r5, #24
.L_080fb990:
	movs r3, #0
	mov r9, r3
.L_080fb994:
	mov r0, r9
	lsls r1, r0, #2
	adds r2, r1, #0
	adds r2, #24
	mov r0, r10
	ldrb r3, [r0, r2]
	cmp r3, #0
	bne .L_080fb9a6
	b .L_080fbb2a
.L_080fb9a6:
	adds r3, r0, r2
	movs r7, #1
	ldrsb r7, [r3, r7]
	ldrb r3, [r3]
	cmp r3, #27
	bls .L_080fb9b4
	b .L_080fbb20
.L_080fb9b4:
	ldr r2, .L_080fbc14
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_080fb9bc:
	.4byte .L_080fbb20
	.4byte .L_080fba2c
	.4byte .L_080fba2c
	.4byte .L_080fba2c
	.4byte .L_080fba2c
	.4byte .L_080fba2c
	.4byte .L_080fba2c
	.4byte .L_080fba9e
	.4byte .L_080fba9e
	.4byte .L_080fba9e
	.4byte .L_080fba9e
	.4byte .L_080fba9e
	.4byte .L_080fba9e
	.4byte .L_080fba9e
	.4byte .L_080fba9e
	.4byte .L_080fba42
	.4byte .L_080fba42
	.4byte .L_080fba42
	.4byte .L_080fba42
	.4byte .L_080fba42
	.4byte .L_080fba42
	.4byte .L_080fba42
	.4byte .L_080fba42
	.4byte .L_080fbb06
	.4byte .L_080fbb20
	.4byte .L_080fbb06
	.4byte .L_080fba2c
	.4byte .L_080fbb06
.L_080fba2c:
	adds r3, r1, #0
	adds r3, #24
	mov r1, r10
	ldrb r0, [r1, r3]
	ldr r3, .L_080fbc18
	lsls r6, r5, #24
	asrs r5, r6, #21
	adds r0, r0, r3
	mov r1, r8
	movs r2, #0
	b .L_080fba88
.L_080fba42:
	adds r4, r1, #0
	mov r3, r10
	adds r4, #24
	ldrb r2, [r3, r4]
	adds r3, r2, #0
	subs r3, #15
	adds r1, r3, #0
	cmp r3, #0
	bge .L_080fba58
	adds r1, r2, #0
	subs r1, #12
.L_080fba58:
	asrs r1, r1, #2
	lsls r1, r1, #2
	subs r1, r3, r1
	lsls r1, r1, #24
	lsls r6, r5, #24
	asrs r5, r6, #24
	movs r3, #2
	lsrs r1, r1, #24
	str r3, [sp, #0]
	adds r1, #1
	adds r3, r5, #0
	mov r0, r8
	movs r2, #0
	str r4, [sp, #4]
	bl UiWindow_SetTilemapEntryFar
	ldr r4, [sp, #4]
	mov r1, r10
	ldrb r0, [r1, r4]
	ldr r3, .L_080fbc18
	lsls r5, r5, #3
	adds r0, r0, r3
	mov r1, r8
	movs r2, #8
.L_080fba88:
	adds r3, r5, #0
	bl UiText_DrawCharacterAtOffsetFar
	adds r0, r7, #0
	movs r1, #3
	mov r2, r8
	movs r3, #72
	str r5, [sp, #0]
	bl ItemMenu_DrawStat
	b .L_080fbb22
.L_080fba9e:
	adds r3, r1, #0
	adds r3, #24
	mov r2, r10
	ldrb r0, [r2, r3]
	ldr r3, .L_080fbc18
	lsls r6, r5, #24
	asrs r5, r6, #21
	adds r0, r0, r3
	mov r1, r8
	movs r2, #0
	adds r3, r5, #0
	bl UiText_DrawCharacterAtOffsetFar
	cmp r7, #9
	ble .L_080fbadc
	movs r0, #1
	movs r1, #1
	mov r2, r8
	movs r3, #72
	str r5, [sp, #0]
	bl UiText_DrawNumberInWindowFar
	ldr r0, .L_080fbc1c
	mov r1, r8
	movs r2, #80
	adds r3, r5, #0
	bl UiText_DrawStringInWindowFar
	adds r0, r7, #0
	subs r0, #10
	b .L_080fbaf8
.L_080fbadc:
	movs r0, #0
	movs r1, #1
	mov r2, r8
	movs r3, #72
	str r5, [sp, #0]
	bl UiText_DrawNumberInWindowFar
	ldr r0, .L_080fbc1c
	mov r1, r8
	movs r2, #80
	adds r3, r5, #0
	bl UiText_DrawStringInWindowFar
	adds r0, r7, #0
.L_080fbaf8:
	movs r1, #1
	mov r2, r8
	movs r3, #88
	str r5, [sp, #0]
	bl UiText_DrawNumberInWindowFar
	b .L_080fbb22
.L_080fbb06:
	adds r3, r1, #0
	adds r3, #24
	mov r1, r10
	ldrb r0, [r1, r3]
	ldr r3, .L_080fbc18
	lsls r6, r5, #24
	adds r0, r0, r3
	mov r1, r8
	asrs r3, r6, #21
	movs r2, #0
	bl UiText_DrawCharacterAtOffsetFar
	b .L_080fbb22
.L_080fbb20:
	lsls r6, r5, #24
.L_080fbb22:
	movs r2, #128
	lsls r2, r2, #17
	adds r3, r6, r2
	lsrs r5, r3, #24
.L_080fbb2a:
	movs r3, #1
	add r9, r3
	mov r0, r9
	cmp r0, #3
	bgt .L_080fbb36
	b .L_080fb994
.L_080fbb36:
	mov r1, r10
	ldrb r2, [r1, #3]
	ands r3, r2
	cmp r3, #0
	beq .L_080fbb56
	lsls r5, r5, #24
	asrs r5, r5, #24
	lsls r3, r5, #3
	ldr r0, .L_080fbc20
	mov r1, r8
	movs r2, #0
	bl UiText_DrawCharacterAtOffsetFar
	adds r5, #1
	lsls r5, r5, #24
	lsrs r5, r5, #24
.L_080fbb56:
	mov r3, r10
	ldrb r2, [r3, #12]
	adds r3, r2, #0
	cmp r3, #3
	bne .L_080fbb7e
	lsls r5, r5, #24
	asrs r5, r5, #24
	ldr r0, .L_080fbc24
	lsls r3, r5, #3
	mov r1, r8
	movs r2, #0
	bl UiText_DrawCharacterAtOffsetFar
	movs r0, #1
	str r0, [sp, #8]
	mov r1, r10
	adds r5, #1
	ldrb r2, [r1, #12]
	lsls r5, r5, #24
	lsrs r5, r5, #24
.L_080fbb7e:
	adds r3, r2, #0
	cmp r3, #4
	beq .L_080fbc60
	cmp r3, #0
	beq .L_080fbc60
	ldr r2, [sp, #8]
	cmp r2, #0
	bne .L_080fbba4
	lsls r5, r5, #24
	asrs r5, r5, #24
	lsls r3, r5, #3
	ldr r0, .L_080fbc28
	mov r1, r8
	movs r2, #16
	bl UiText_DrawCharacterAtOffsetFar
	adds r5, #1
	lsls r5, r5, #24
	lsrs r5, r5, #24
.L_080fbba4:
	mov r0, r10
	ldrb r3, [r0, #12]
	cmp r3, #1
	beq .L_080fbbb6
	cmp r3, #1
	ble .L_080fbc60
	cmp r3, #2
	beq .L_080fbbcc
	b .L_080fbc60
.L_080fbbb6:
	lsls r5, r5, #24
	asrs r5, r5, #24
	lsls r3, r5, #3
	adds r5, #1
	ldr r0, .L_080fbc2c
	mov r1, r8
	movs r2, #0
	lsls r5, r5, #24
	bl UiText_DrawCharacterAtOffsetFar
	b .L_080fbc5e
.L_080fbbcc:
	movs r3, #128
	lsls r3, r3, #3
	mov r1, r11
	ands r3, r1
	cmp r3, #0
	beq .L_080fbc34
	lsls r5, r5, #24
	ldr r6, .L_080fbc30
	asrs r5, r5, #24
	lsls r3, r5, #3
	adds r5, #1
	lsls r5, r5, #24
	adds r0, r6, #0
	mov r1, r8
	movs r2, #0
	asrs r5, r5, #24
	bl UiText_DrawCharacterAtOffsetFar
	adds r6, #1
	lsls r3, r5, #3
	adds r5, #1
	adds r0, r6, #0
	mov r1, r8
	movs r2, #0
	lsls r5, r5, #24
	bl UiText_DrawCharacterAtOffsetFar
	b .L_080fbc5e
.L_080fbc04:
	.4byte 0x00ffffff
.L_080fbc08:
	.4byte 0x0000109d
.L_080fbc0c:
	.4byte 0x00001026
.L_080fbc10:
	.4byte 0x00001027
.L_080fbc14:
	.4byte .L_080fb9bc
.L_080fbc18:
	.4byte 0x0000106b
.L_080fbc1c:
	.4byte Data_08105968
.L_080fbc20:
	.4byte 0x000010a6
.L_080fbc24:
	.4byte 0x00001095
.L_080fbc28:
	.4byte 0x0000109e
.L_080fbc2c:
	.4byte 0x00001093
.L_080fbc30:
	.4byte 0x000010a3
.L_080fbc34:
	lsls r5, r5, #24
	ldr r6, .L_080fbd20
	asrs r5, r5, #24
	lsls r3, r5, #3
	adds r5, #1
	lsls r5, r5, #24
	adds r0, r6, #0
	mov r1, r8
	movs r2, #0
	asrs r5, r5, #24
	adds r6, #1
	bl UiText_DrawCharacterAtOffsetFar
	lsls r3, r5, #3
	adds r0, r6, #0
	mov r1, r8
	movs r2, #0
	bl UiText_DrawCharacterAtOffsetFar
	adds r5, #1
	lsls r5, r5, #24
.L_080fbc5e:
	lsrs r5, r5, #24
.L_080fbc60:
	mov r3, r10
	ldrb r2, [r3, #3]
	movs r3, #16
	ands r3, r2
	cmp r3, #0
	beq .L_080fbcc4
	lsls r6, r5, #24
	asrs r3, r6, #24
	cmp r3, #0
	beq .L_080fbc78
	adds r3, #1
	lsls r6, r3, #24
.L_080fbc78:
	ldr r0, .L_080fbd24
	asrs r5, r6, #24
	lsls r3, r5, #3
	mov r1, r8
	movs r2, #16
	mov r9, r0
	bl UiText_DrawCharacterAtOffsetFar
	movs r0, #248
	adds r5, #1
	lsls r0, r0, #8
	mov r1, r11
	lsls r5, r5, #24
	ands r0, r1
	lsrs r5, r5, #24
	cmp r0, #0
	bge .L_080fbca2
	movs r2, #224
	lsls r2, r2, #3
	adds r2, #255
	adds r0, r0, r2
.L_080fbca2:
	asrs r7, r0, #11
	adds r0, r7, #1
	movs r1, #5
	lsls r5, r5, #24
	bl UiText_DrawQuantity
	asrs r5, r5, #24
	mov r0, r9
	lsls r3, r5, #3
	adds r0, #1
	mov r1, r8
	movs r2, #0
	bl UiText_DrawCharacterAtOffsetFar
	adds r5, #1
	lsls r5, r5, #24
	lsrs r5, r5, #24
.L_080fbcc4:
	movs r1, #0
	cmp r5, #0
	bne .L_080fbd12
	mov r3, r10
	ldrb r2, [r3, #3]
	movs r3, #4
	ands r3, r2
	cmp r3, #0
	beq .L_080fbce4
	mov r1, r8
	ldr r0, .L_080fbd28
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
	movs r1, #1
.L_080fbce4:
	cmp r1, #0
	bne .L_080fbd12
	mov r0, r10
	ldrb r2, [r0, #3]
	movs r3, #8
	ands r3, r2
	cmp r3, #0
	beq .L_080fbd02
	mov r1, r8
	ldr r0, .L_080fbd2c
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
	movs r1, #1
.L_080fbd02:
	cmp r1, #0
	bne .L_080fbd12
	ldr r0, .L_080fbd30
	mov r1, r8
	movs r2, #0
	movs r3, #0
	bl UiText_DrawCharacterAtOffsetFar
.L_080fbd12:
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080fbd20:
	.4byte 0x000010a1
.L_080fbd24:
	.4byte 0x0000109f
.L_080fbd28:
	.4byte 0x00001099
.L_080fbd2c:
	.4byte 0x0000109a
.L_080fbd30:
	.4byte 0x0000109c
