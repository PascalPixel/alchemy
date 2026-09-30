.syntax unified
	.thumb
	.global UiWindow_DrawStatusBarTiles
	.thumb_func
UiWindow_DrawStatusBarTiles:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #20
	str r1, [sp, #16]
	mov r9, r3
	ldr r3, .L_0801f1cc
	ldr r3, [r3]
	adds r6, r0, #0
	ldr r1, .L_0801f1d0
	mov r0, r9
	str r3, [sp, #8]
	str r0, [sp, #4]
	adds r3, r3, r1
	ldrb r3, [r3]
	adds r5, r2, #0
	cmp r3, #0
	bne .L_0801f0ca
	bl Runtime_GetLowTableAddress
	ldr r3, .L_0801f1d4
	ldr r1, .L_0801f1d8
	ldr r2, .L_0801f1dc
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, .L_0801f1e0
	ldr r2, .L_0801f1e4
	ldrh r3, [r3]
	strh r3, [r2]
.L_0801f0ca:
	ldrh r3, [r6, #12]
	ldr r2, [sp, #16]
	adds r2, r2, r3
	str r2, [sp, #16]
	ldrh r3, [r6, #14]
	adds r5, r5, r3
	lsls r5, r5, #5
	movs r3, #4
	str r5, [sp, #0]
	str r3, [sp, #12]
.L_0801f0de:
	ldr r0, [sp, #0]
	ldr r1, [sp, #16]
	ldr r2, [sp, #8]
	adds r3, r0, r1
	lsls r3, r3, #1
	ldrh r3, [r2, r3]
	mov r10, r3
	ldr r3, .L_0801f1e8
	mov r8, r3
	ldr r3, .L_0801f1ec
	mov r1, r10
	ldr r0, .L_0801f1f0
	ands r1, r3
	mov r2, r9
	mov r12, r0
	mov r10, r1
	cmp r2, #7
	ble .L_0801f10a
	ldr r3, .L_0801f1f4
	ldr r0, .L_0801f1f8
	mov r8, r3
	b .L_0801f134
.L_0801f10a:
	mov r1, r9
	cmp r1, #0
	blt .L_0801f136
	lsls r1, r1, #2
	mov r2, r8
	lsls r2, r1
	mov r8, r2
	ldr r3, .L_0801f1f4
	movs r2, #32
	subs r2, r2, r1
	lsrs r3, r2
	mov r0, r8
	orrs r0, r3
	mov r3, r12
	lsls r3, r1
	mov r12, r3
	ldr r3, .L_0801f1f8
	mov r8, r0
	lsrs r3, r2
	mov r0, r12
	orrs r0, r3
.L_0801f134:
	mov r12, r0
.L_0801f136:
	ldr r2, .L_0801f1fc
	movs r1, #0
	mov lr, r1
	mov r11, r2
	movs r7, #0
	b .L_0801f190
.L_0801f142:
	mov r3, r10
	lsls r6, r3, #5
	mov r0, r11
	subs r3, r6, r7
	ldr r4, [r3, r0]
	movs r1, #0
	movs r0, #0
	movs r5, #15
.L_0801f152:
	adds r2, r4, #0
	ands r2, r5
	cmp r2, #14
	bne .L_0801f164
	lsls r2, r1, #2
	adds r3, r5, #0
	lsls r3, r2
	mov r2, r8
	b .L_0801f170
.L_0801f164:
	cmp r2, #1
	bne .L_0801f176
	lsls r2, r1, #2
	adds r3, r5, #0
	lsls r3, r2
	mov r2, r12
.L_0801f170:
	ands r3, r2
	orrs r0, r3
	b .L_0801f17c
.L_0801f176:
	lsls r3, r1, #2
	lsls r2, r3
	orrs r0, r2
.L_0801f17c:
	adds r1, #1
	lsrs r4, r4, #4
	cmp r1, #7
	ble .L_0801f152
	subs r3, r6, r7
	mov r1, r11
	movs r2, #1
	str r0, [r3, r1]
	adds r7, #4
	add lr, r2
.L_0801f190:
	ldr r3, [sp, #4]
	cmp r3, #0
	beq .L_0801f19e
	mov r0, lr
	cmp r0, #2
	ble .L_0801f142
	b .L_0801f1a4
.L_0801f19e:
	mov r1, lr
	cmp r1, #0
	ble .L_0801f142
.L_0801f1a4:
	ldr r3, [sp, #12]
	ldr r0, [sp, #16]
	movs r2, #8
	negs r2, r2
	subs r3, #1
	adds r0, #1
	add r9, r2
	str r3, [sp, #12]
	str r0, [sp, #16]
	cmp r3, #0
	bge .L_0801f0de
	add sp, #20
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
.L_0801f1cc:
	.4byte Data_03001e8c
.L_0801f1d0:
	.4byte 0x00000ea5
.L_0801f1d4:
	.4byte 0x040000d4
.L_0801f1d8:
	.4byte 0x050001c0
.L_0801f1dc:
	.4byte 0x80000010
.L_0801f1e0:
	.4byte 0x050001e8
.L_0801f1e4:
	.4byte 0x050001dc
.L_0801f1e8:
	.4byte 0x22222222
.L_0801f1ec:
	.4byte 0x000003ff
.L_0801f1f0:
	.4byte 0xcccccccc
.L_0801f1f4:
	.4byte 0x88888888
.L_0801f1f8:
	.4byte 0xdddddddd
.L_0801f1fc:
	.4byte 0x0600001c
