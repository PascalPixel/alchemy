.syntax unified
	.thumb
	.global Func_0804b6a0
	.thumb_func
Func_0804b6a0:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #228
	ldr r7, [r3]
	sub sp, #8
	cmp r7, #0
	bne .L_0804b6b2
	b .L_0804b7b4
.L_0804b6b2:
	ldr r0, [r7, #40]
	ldr r5, [r7, #44]
	cmp r0, r5
	beq .L_0804b6d8
	subs r6, r0, r5
	adds r0, r6, #0
	movs r1, #3
	bl Math_Div
	cmp r0, #0
	bne .L_0804b6d0
	subs r0, #1
	cmp r6, #0
	blt .L_0804b6d0
	movs r0, #1
.L_0804b6d0:
	adds r0, r5, r0
	str r0, [r7, #44]
	bl Camera_ConfigureSceneFar
.L_0804b6d8:
	adds r5, r7, #0
	adds r6, r7, #0
	adds r5, #36
	movs r2, #2
.L_0804b6e0:
	ldrb r3, [r5]
	adds r5, #1
	cmp r3, #0
	beq .L_0804b6f4
	adds r0, r6, #0
	movs r1, #240
	str r2, [sp, #4]
	bl Func_08014128
	ldr r2, [sp, #4]
.L_0804b6f4:
	subs r2, #1
	adds r6, #12
	cmp r2, #0
	bge .L_0804b6e0
	ldr r0, .L_0804b7b8
	bl Func_08045330
	ldr r3, [r7, #80]
	cmp r3, #0
	beq .L_0804b7b4
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #36]
	adds r3, r2, #0
	adds r3, #82
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0804b734
	movs r3, #0
	str r3, [r7, #76]
	b .L_0804b7b4
.L_0804b71e:
	ldrh r3, [r2, #12]
	cmp r3, #86
	bne .L_0804b75a
	ldrh r3, [r2, #14]
	cmp r3, #83
	bne .L_0804b75a
	movs r3, #225
	lsls r3, r3, #2
	str r3, [r7, #76]
	adds r6, r3, #0
	b .L_0804b75a
.L_0804b734:
	ldr r6, [r7, #76]
	cmp r6, #0
	bge .L_0804b75e
	adds r3, r2, #0
	adds r3, #80
	ldrb r2, [r3]
	movs r3, #1
	eors r3, r2
	lsls r2, r3, #1
	adds r2, r2, r3
	ldr r3, .L_0804b7bc
	lsls r2, r2, #3
	adds r2, r2, r3
	ldrh r3, [r2, #8]
	cmp r3, #69
	bne .L_0804b75a
	ldrh r3, [r2, #10]
	cmp r3, #68
	beq .L_0804b71e
.L_0804b75a:
	cmp r6, #0
	blt .L_0804b7b4
.L_0804b75e:
	ldr r3, [r7, #68]
	cmp r3, #0
	bne .L_0804b772
	ldr r3, [r7, #72]
	cmp r3, #0
	bne .L_0804b772
	bl Link_CreateCountdownLabelWindow
	ldr r6, [r7, #76]
	str r0, [r7, #68]
.L_0804b772:
	cmp r6, #0
	ble .L_0804b77c
	subs r3, r6, #1
	str r3, [r7, #76]
	adds r6, r3, #0
.L_0804b77c:
	cmp r6, #0
	blt .L_0804b7b4
	adds r0, r6, #0
	adds r0, #59
	movs r1, #60
	bl Math_Div
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0804b7a0
	lsls r3, r5, #4
	subs r3, r3, r5
	lsls r3, r3, #2
	cmp r3, r6
	bne .L_0804b7a0
	movs r0, #108
	bl Audio_PlayCue
.L_0804b7a0:
	ldr r2, [r7, #68]
	cmp r2, #0
	beq .L_0804b7b4
	movs r3, #8
	str r3, [sp, #0]
	adds r0, r5, #0
	movs r1, #2
	movs r3, #16
	bl UiText_DrawNumberInWindow
.L_0804b7b4:
	add sp, #8
	pop {r5, r6, r7, pc}
.L_0804b7b8:
	.4byte 0x06006680
.L_0804b7bc:
	.4byte Data_02003874
