.syntax unified
	.thumb
	.global Func_0803a8fc
	.thumb_func
Func_0803a8fc:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #388
	str r1, [sp, #0]
	movs r3, #192
	lsls r3, r3, #18
	ldr r6, [r3, #60]
	lsls r3, r0, #8
	asrs r3, r3, #16
	mov r10, r3
	movs r3, #255
	ands r3, r0
	ldr r2, .L_0803aa94
	subs r3, #32
	lsls r3, r3, #5
	adds r7, r3, r2
	ldrh r0, [r7]
	movs r1, #192
	ldr r3, .L_0803aa98
	mov r8, r0
	lsls r1, r1, #1
	add r0, sp, #4
	mov lr, r3
	.2byte 0xf800
	ldrb r3, [r6, #4]
	adds r7, #2
	cmp r3, #0
	beq .L_0803a946
	movs r2, #0
	movs r3, #8
	mov r9, r2
	b .L_0803a954
.L_0803a946:
	movs r2, #240
	lsls r2, r2, #4
	adds r2, #62
	adds r3, r6, r2
	ldrh r3, [r3]
	movs r0, #1
	mov r9, r0
.L_0803a954:
	mov r11, r3
	movs r0, #240
	lsls r0, r0, #4
	adds r0, #60
	adds r3, r6, r0
	ldrh r3, [r3]
	cmp r3, #1
	bne .L_0803a99a
	mov r1, sp
	ldr r5, .L_0803aa9c
	adds r1, #53
	mov r2, r9
	adds r0, r7, #0
	mov lr, r5
	.2byte 0xf800
	mov r1, sp
	adds r1, #54
	mov r2, r9
	adds r0, r7, #0
	mov lr, r5
	.2byte 0xf800
	mov r2, r11
	add r1, sp, #36
	adds r0, r7, #0
	mov lr, r5
	.2byte 0xf800
	mov r1, sp
	mov r2, r11
	adds r1, #37
	adds r0, r7, #0
	mov lr, r5
	.2byte 0xf800
	movs r2, #1
	add r8, r2
	b .L_0803a9b2
.L_0803a99a:
	mov r1, sp
	adds r1, #53
	mov r2, r9
	ldr r5, .L_0803aa9c
	adds r0, r7, #0
	mov lr, r5
	.2byte 0xf800
	adds r0, r7, #0
	add r1, sp, #36
	mov r2, r11
	mov lr, r5
	.2byte 0xf800
.L_0803a9b2:
	mov r0, r10
	lsls r3, r0, #16
	lsrs r2, r3, #16
	cmp r2, #0
	beq .L_0803aa46
	ldr r3, .L_0803aaa0
	movs r0, #240
	lsls r2, r2, #5
	lsls r0, r0, #4
	adds r7, r2, r3
	adds r0, #60
	movs r3, #0
	ldrsh r2, [r7, r3]
	adds r3, r6, r0
	ldrh r3, [r3]
	mov r10, r2
	adds r7, #2
	cmp r3, #1
	bne .L_0803aa20
	add r3, sp, #4
	mov r2, r8
	adds r6, r3, r2
	adds r1, r6, #0
	ldr r5, .L_0803aa9c
	adds r1, #49
	mov r2, r9
	adds r0, r7, #0
	mov lr, r5
	.2byte 0xf800
	adds r1, r6, #0
	adds r1, #50
	mov r2, r9
	adds r0, r7, #0
	mov lr, r5
	.2byte 0xf800
	adds r1, r6, #0
	adds r1, #32
	mov r2, r11
	adds r0, r7, #0
	mov lr, r5
	.2byte 0xf800
	adds r1, r6, #0
	mov r2, r11
	adds r1, #33
	adds r0, r7, #0
	mov lr, r5
	.2byte 0xf800
	mov r0, r10
	movs r2, #128
	lsls r3, r0, #16
	lsls r2, r2, #9
	adds r3, r3, r2
	asrs r3, r3, #16
	mov r10, r3
	b .L_0803aa3e
.L_0803aa20:
	add r5, sp, #4
	add r5, r8
	adds r1, r5, #0
	adds r1, #49
	mov r2, r9
	ldr r6, .L_0803aa9c
	adds r0, r7, #0
	mov lr, r6
	.2byte 0xf800
	adds r1, r5, #0
	adds r1, #32
	adds r0, r7, #0
	mov r2, r11
	mov lr, r6
	.2byte 0xf800
.L_0803aa3e:
	mov r0, r10
	lsls r3, r0, #16
	lsrs r3, r3, #16
	add r8, r3
.L_0803aa46:
	mov r1, sp
	adds r1, #11
	movs r6, #0
.L_0803aa4c:
	movs r5, #0
.L_0803aa4e:
	movs r4, #0
.L_0803aa50:
	movs r2, #0
	movs r0, #7
.L_0803aa54:
	ldrb r3, [r1]
	lsls r2, r2, #4
	subs r0, #1
	adds r2, r2, r3
	subs r1, #1
	cmp r0, #0
	bge .L_0803aa54
	ldr r0, [sp, #0]
	adds r4, #1
	stmia r0!, {r2}
	adds r1, #24
	adds r3, r0, #0
	str r3, [sp, #0]
	cmp r4, #7
	ble .L_0803aa50
	adds r5, #1
	subs r1, #120
	cmp r5, #1
	ble .L_0803aa4e
	adds r6, #1
	adds r1, #112
	cmp r6, #1
	ble .L_0803aa4c
	mov r0, r8
	add sp, #388
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0803aa94:
	.4byte UiText_Glyphs
.L_0803aa98:
	.4byte IwramClearWords
.L_0803aa9c:
	.4byte IwramFillWords + 0xb4
.L_0803aaa0:
	.4byte Data_0805a0e0
