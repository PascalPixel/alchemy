.syntax unified
	.thumb
	.global ObjectTable_Restore
	.thumb_func
ObjectTable_Restore:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r1, .L_0808bc2c
	movs r2, #32
	mov r8, r1
	negs r2, r2
	add r2, r8
	movs r3, #224
	movs r1, #226
	mov r10, r2
	lsls r3, r3, #4
	lsls r1, r1, #4
	movs r2, #228
	sub sp, #8
	add r3, r8
	add r1, r8
	lsls r2, r2, #4
	str r3, [sp, #4]
	str r1, [sp, #0]
	add r2, r8
	mov r1, r10
	mov r11, r2
	movs r2, #31
	negs r2, r2
	ldrb r7, [r1]
	movs r3, #0
	add r2, r8
	mov r9, r3
	mov r10, r2
	cmp r7, #255
	beq .L_0808bc18
.L_0808bb74:
	adds r0, r7, #0
	bl ObjectTable_Get
	adds r5, r0, #0
	cmp r5, #0
	beq .L_0808bbf2
	ldr r6, [r5, #80]
	ldr r3, .L_0808bc30
	mov r0, r8
	adds r1, r5, #0
	ldr r2, .L_0808bc34
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, [sp, #4]
	ldrb r1, [r3]
	cmp r1, #0
	beq .L_0808bb9c
	adds r0, r5, #0
	bl Object_SetMode
.L_0808bb9c:
	ldr r2, [sp, #0]
	adds r0, r5, #0
	ldrb r1, [r2]
	bl ObjectDispatch_SetSingleChildField26Far
	mov r3, r11
	ldrb r1, [r3]
	movs r3, #3
	ldrb r2, [r6, #9]
	ands r1, r3
	movs r3, #13
	negs r3, r3
	lsls r1, r1, #2
	ands r3, r2
	orrs r3, r1
	strb r3, [r6, #9]
	ldrb r2, [r6, #21]
	movs r3, #13
	negs r3, r3
	ands r3, r2
	orrs r3, r1
	strb r3, [r6, #21]
	ldr r1, .L_0808bc38
	ldr r3, [r1]
	str r6, [r5, #80]
	cmp r7, r3
	bne .L_0808bbf2
	ldr r2, .L_0808bc3c
	movs r1, #240
	ldr r3, [r2]
	lsls r1, r1, #1
	adds r3, r3, r1
	ldr r1, .L_0808bc40
	ldr r2, [r3]
	ldr r3, [r1]
	ldr r1, [r3]
	ldr r3, [r5, #12]
	adds r0, r5, #0
	str r3, [r2, #20]
	str r3, [r2, #12]
	str r3, [r1, #4]
	bl Object_ResetMotion
.L_0808bbf2:
	movs r2, #112
	ldr r3, [sp, #4]
	ldr r1, [sp, #0]
	add r8, r2
	movs r2, #1
	adds r3, #1
	add r9, r2
	str r3, [sp, #4]
	adds r1, #1
	mov r3, r9
	str r1, [sp, #0]
	add r11, r2
	cmp r3, #31
	bgt .L_0808bc18
	mov r1, r10
	ldrb r7, [r1]
	add r10, r2
	cmp r7, #255
	bne .L_0808bb74
.L_0808bc18:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
	.2byte 0x0000
.L_0808bc2c:
	.4byte Data_02001124
.L_0808bc30:
	.4byte 0x040000d4
.L_0808bc34:
	.4byte 0x8400001c
.L_0808bc38:
	.4byte gItemCounters + 0xb4
.L_0808bc3c:
	.4byte gEventWork
.L_0808bc40:
	.4byte gMapWork
