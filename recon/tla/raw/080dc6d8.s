.syntax unified
	.thumb
	.global Func_080dc6d8
	.thumb_func
Func_080dc6d8:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r2, #192
	lsls r2, r2, #18
	adds r3, r2, #0
	adds r3, #224
	ldr r7, [r3]
	ldr r2, [r2, #108]
	ldr r6, [r7, #16]
	mov r11, r2
	movs r3, #28
	ldrsh r2, [r7, r3]
	adds r0, r6, #0
	movs r1, #20
	mov r9, r2
	bl Object_SetMode
	ldr r3, [r6, #8]
	movs r2, #0
	str r3, [r6, #56]
	ldr r3, [r6, #12]
	str r2, [r6, #36]
	str r3, [r6, #60]
	ldr r3, [r6, #16]
	str r2, [r6, #40]
	str r3, [r6, #64]
	str r2, [r6, #44]
	movs r3, #34
	adds r3, r3, r7
	mov r10, r3
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	mov r8, r2
	cmp r3, #0
	beq .L_080dc738
	movs r0, #212
	bl Audio_PlayCue
	adds r0, r6, #0
	movs r1, #1
	bl Func_080e1420
.L_080dc738:
	adds r3, r7, #0
	adds r3, #35
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #0
	beq .L_080dc784
	movs r2, #224
	lsls r2, r2, #3
	adds r2, #19
	movs r3, #1
	adds r5, r7, r2
	strb r3, [r5]
	movs r2, #0
	adds r0, r6, #0
	movs r1, #1
	bl Func_080dc164
	mov r0, r9
	movs r1, #4
	bl UiText_DrawQuantity
	movs r2, #224
	lsls r2, r2, #3
	adds r2, #18
	adds r3, r7, r2
	movs r1, #0
	ldrsb r1, [r3, r1]
	ldr r0, .L_080dc7c8
	bl UiText_ShowPositionedMessageAndWaitFar
	adds r0, r6, #0
	movs r1, #0
	movs r2, #16
	bl Func_080dc164
	mov r3, r8
	strb r3, [r5]
.L_080dc784:
	movs r0, #160
	lsls r0, r0, #1
	bl GameFlag_Test
	cmp r0, #0
	beq .L_080dc7ac
	mov r2, r10
	movs r3, #0
	ldrsb r3, [r2, r3]
	cmp r3, #0
	beq .L_080dc7a2
	adds r0, r6, #0
	movs r1, #2
	bl Func_080e1420
.L_080dc7a2:
	adds r0, r6, #0
	movs r1, #21
	bl Object_SetMode
	b .L_080dc7b0
.L_080dc7ac:
	bl Func_080dc7cc
.L_080dc7b0:
	movs r2, #192
	lsls r2, r2, #4
	adds r2, #183
	add r2, r11
	movs r3, #1
	strb r3, [r2]
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
.L_080dc7c8:
	.4byte 0x00000dc2
