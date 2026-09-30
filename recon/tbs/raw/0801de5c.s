.syntax unified
	.thumb
	.global UiText_RenderStringTiles
	.thumb_func
UiText_RenderStringTiles:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	sub sp, #68
	str r1, [sp, #16]
	str r2, [sp, #12]
	mov r8, r3
	ldr r3, .L_0801e148
	movs r5, #128
	ldr r3, [r3]
	lsls r5, r5, #4
	adds r6, r0, #0
	adds r0, r5, #0
	mov r11, r3
	bl Runtime_BumpAllocate
	str r0, [sp, #8]
	ldr r0, .L_0801e14c
	bl Resource_GetTableEntry
	ldr r3, .L_0801e150
	str r0, [sp, #4]
	add r3, r11
	ldrb r3, [r3]
	lsls r3, r3, #12
	str r3, [sp, #0]
	movs r1, #16
	ldr r3, .L_0801e154
	add r0, sp, #20
	bl _call_via_r3
	movs r2, #240
	ldr r1, [sp, #0]
	lsls r2, r2, #8
	cmp r1, r2
	bne .L_0801ded4
	add r3, sp, #20
	mov r10, r3
	ldr r3, .L_0801e158
	add r3, r11
	ldrh r2, [r3]
	ldr r1, .L_0801e15c
	movs r3, #15
	ands r3, r2
	ldrb r3, [r1, r3]
	mov r4, r10
	strb r3, [r4, #1]
	movs r3, #3
	strb r3, [r4, #3]
	ldr r0, [sp, #8]
	ldr r3, .L_0801e160
	adds r1, r5, #0
	ldr r2, .L_0801e164
	bl _call_via_r3
	b .L_0801def6
.L_0801ded4:
	ldr r3, .L_0801e158
	add r3, r11
	ldrb r2, [r3]
	movs r7, #20
	movs r3, #15
	add r7, sp
	ands r3, r2
	strb r3, [r7, #1]
	movs r3, #1
	strb r3, [r7, #3]
	ldr r0, [sp, #8]
	ldr r3, .L_0801e160
	adds r1, r5, #0
	ldr r2, .L_0801e168
	mov r10, r7
	bl _call_via_r3
.L_0801def6:
	cmp r6, #0
	bne .L_0801defc
	b .L_0801e042
.L_0801defc:
	b .L_0801e036
.L_0801defe:
	cmp r1, #30
	bhi .L_0801dfb6
	subs r3, r1, #3
	cmp r3, #26
	bls .L_0801df0a
	b .L_0801e036
.L_0801df0a:
	ldr r2, .L_0801e16c
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_0801df14:
	.4byte .L_0801dfaa
	.4byte .L_0801e036
	.4byte .L_0801e036
	.4byte .L_0801e036
	.4byte .L_0801df98
	.4byte .L_0801df80
	.4byte .L_0801df98
	.4byte .L_0801df98
	.4byte .L_0801dfb2
	.4byte .L_0801dfb2
	.4byte .L_0801e036
	.4byte .L_0801dfb0
	.4byte .L_0801dfb0
	.4byte .L_0801e036
	.4byte .L_0801dfb2
	.4byte .L_0801e036
	.4byte .L_0801e036
	.4byte .L_0801e036
	.4byte .L_0801e036
	.4byte .L_0801e036
	.4byte .L_0801e036
	.4byte .L_0801e036
	.4byte .L_0801e036
	.4byte .L_0801e036
	.4byte .L_0801e036
	.4byte .L_0801dfb0
	.4byte .L_0801dfb2
.L_0801df80:
	ldr r3, .L_0801e158
	ldrh r1, [r6]
	add r3, r11
	strh r1, [r3]
	ldr r2, .L_0801e15c
	movs r3, #15
	ands r3, r1
	ldrb r3, [r2, r3]
	mov r1, r10
	adds r6, #2
	strb r3, [r1, #1]
	b .L_0801e036
.L_0801df98:
	ldr r3, .L_0801e158
	movs r2, #15
	add r3, r11
	strh r2, [r3]
	ldr r3, .L_0801e15c
	ldrb r3, [r3, r2]
	mov r2, r10
	strb r3, [r2, #1]
	b .L_0801e036
.L_0801dfaa:
	ldr r3, .L_0801e170
	ldrb r3, [r3]
	b .L_0801e034
.L_0801dfb0:
	adds r6, #2
.L_0801dfb2:
	adds r6, #2
	b .L_0801e036
.L_0801dfb6:
	movs r3, #255
	ands r1, r3
	ldr r4, [sp, #4]
	ldr r0, [sp, #8]
	lsls r3, r1, #5
	movs r7, #0
	adds r5, r4, r3
	add r0, r8
	mov r9, r7
	mov lr, r10
.L_0801dfca:
	ldmia r5!, {r2}
	movs r4, #3
.L_0801dfce:
	movs r7, #15
	adds r3, r2, #0
	ands r3, r7
	mov r7, lr
	ldrb r3, [r7, r3]
	cmp r3, #0
	beq .L_0801dfde
	strb r3, [r0]
.L_0801dfde:
	lsrs r2, r2, #4
	movs r7, #15
	adds r3, r2, #0
	ands r3, r7
	mov r7, r10
	ldrb r3, [r7, r3]
	adds r0, #1
	cmp r3, #0
	beq .L_0801dff2
	strb r3, [r0]
.L_0801dff2:
	subs r4, #1
	adds r0, #1
	lsrs r2, r2, #4
	cmp r4, #0
	bge .L_0801dfce
	movs r2, #1
	add r9, r2
	mov r3, r9
	adds r0, #248
	cmp r3, #7
	ble .L_0801dfca
	ldr r4, .L_0801e174
	cmp r12, r4
	beq .L_0801e014
	ldr r7, .L_0801e178
	cmp r12, r7
	bne .L_0801e01a
.L_0801e014:
	movs r1, #8
	add r8, r1
	b .L_0801e036
.L_0801e01a:
	ldr r2, .L_0801e17c
	cmp r12, r2
	bne .L_0801e024
	movs r3, #3
	b .L_0801e034
.L_0801e024:
	cmp r1, #31
	bls .L_0801e032
	ldr r2, .L_0801e170
	adds r3, r1, #0
	subs r3, #32
	ldrb r3, [r2, r3]
	b .L_0801e034
.L_0801e032:
	movs r3, #1
.L_0801e034:
	add r8, r3
.L_0801e036:
	ldrh r1, [r6]
	adds r6, #2
	mov r12, r1
	cmp r1, #0
	beq .L_0801e042
	b .L_0801defe
.L_0801e042:
	mov r3, r8
	adds r3, #7
	lsrs r6, r3, #3
	ldr r5, [sp, #8]
	movs r1, #128
	lsls r4, r6, #2
	lsls r7, r6, #3
	lsls r1, r1, #1
	movs r2, #7
	adds r0, r5, #0
	mov r10, r4
	mov r8, r7
	mov lr, r1
	mov r9, r2
.L_0801e05e:
	cmp r6, #0
	beq .L_0801e096
	ldr r3, .L_0801e180
	ldr r7, .L_0801e184
	mov r12, r3
	adds r4, r6, #0
.L_0801e06a:
	ldr r1, [r5]
	ldr r2, [r5, #4]
	lsls r3, r1, #4
	orrs r1, r3
	lsrs r3, r2, #4
	orrs r2, r3
	mov r3, r12
	ands r1, r3
	ands r2, r7
	lsls r3, r1, #8
	orrs r1, r3
	lsrs r3, r2, #8
	orrs r2, r3
	lsls r3, r1, #4
	lsrs r3, r3, #16
	lsls r2, r2, #16
	orrs r3, r2
	subs r4, #1
	adds r5, #8
	stmia r0!, {r3}
	cmp r4, #0
	bne .L_0801e06a
.L_0801e096:
	mov r4, r10
	movs r2, #1
	subs r3, r0, r4
	mov r7, lr
	mov r1, r8
	negs r2, r2
	adds r0, r3, r7
	add r9, r2
	subs r3, r5, r1
	adds r5, r3, r7
	mov r3, r9
	cmp r3, #0
	bge .L_0801e05e
	cmp r6, #0
	bne .L_0801e0b6
	b .L_0801e22a
.L_0801e0b6:
	ldr r4, .L_0801e188
	movs r7, #234
	movs r1, #218
	ldr r2, .L_0801e18c
	add r4, r11
	lsls r7, r7, #4
	lsls r1, r1, #4
	ldr r0, [sp, #8]
	mov lr, r4
	add r7, r11
	mov r12, r1
	mov r8, r2
	mov r9, r6
.L_0801e0d0:
	mov r3, lr
	ldrb r2, [r3]
	movs r5, #127
	cmp r2, #0
	beq .L_0801e0dc
	movs r5, #255
.L_0801e0dc:
	ldr r4, [sp, #16]
	ldrh r3, [r4]
	mov r1, r8
	ands r1, r3
	adds r3, r1, #0
	subs r3, #128
	cmp r3, #127
	bls .L_0801e1a0
	cmp r2, #0
	beq .L_0801e100
	movs r2, #128
	lsls r2, r2, #2
	cmp r1, r2
	bcc .L_0801e100
	movs r3, #160
	lsls r3, r3, #2
	cmp r1, r3
	bcc .L_0801e1a0
.L_0801e100:
	ldrh r1, [r7]
	mov r2, r12
	ands r1, r5
	adds r3, r1, r2
	mov r2, r11
	ldrb r3, [r2, r3]
	movs r4, #0
	cmp r3, #0
	beq .L_0801e12a
.L_0801e112:
	adds r1, #1
	adds r4, #1
	ands r1, r5
	cmp r4, r5
	bhi .L_0801e12a
	movs r2, #218
	lsls r2, r2, #4
	adds r3, r1, r2
	mov r2, r11
	ldrb r3, [r2, r3]
	cmp r3, #0
	bne .L_0801e112
.L_0801e12a:
	adds r3, r1, #1
	ands r3, r5
	strh r3, [r7]
	mov r3, r12
	adds r2, r1, r3
	mov r4, r11
	movs r3, #1
	strb r3, [r4, r2]
	cmp r1, #127
	bls .L_0801e190
	movs r2, #192
	lsls r2, r2, #1
	adds r1, r1, r2
	b .L_0801e194
	.2byte 0x0000
.L_0801e148:
	.4byte Data_03001e8c
.L_0801e14c:
	.4byte 0x00000013
.L_0801e150:
	.4byte 0x00000ea7
.L_0801e154:
	.4byte IwramClearWords
.L_0801e158:
	.4byte 0x00000eae
.L_0801e15c:
	.4byte Data_080371b4
.L_0801e160:
	.4byte IwramFillWords
.L_0801e164:
	.4byte 0x04040404
.L_0801e168:
	.4byte 0x0e0e0e0e
.L_0801e16c:
	.4byte .L_0801df14
.L_0801e170:
	.4byte Data_080370d4
.L_0801e174:
	.4byte 0x0000f01d
.L_0801e178:
	.4byte 0x0000f01f
.L_0801e17c:
	.4byte 0x0000f01e
.L_0801e180:
	.4byte 0x0ff00ff0
.L_0801e184:
	.4byte 0x00ff00ff
.L_0801e188:
	.4byte 0x00000ea2
.L_0801e18c:
	.4byte 0x000003ff
.L_0801e190:
	movs r3, #128
	orrs r1, r3
.L_0801e194:
	ldr r3, [sp, #0]
	ldr r4, [sp, #16]
	orrs r3, r1
	strh r3, [r4]
	ldr r2, [sp, #12]
	strh r3, [r2]
.L_0801e1a0:
	movs r3, #192
	lsls r2, r1, #5
	lsls r3, r3, #19
	adds r1, r2, r3
	ldr r4, .L_0801e244
	ldr r3, [r0]
	str r3, [r1]
	adds r1, r2, r4
	movs r4, #128
	lsls r4, r4, #1
	adds r3, r0, r4
	ldr r3, [r3]
	movs r4, #128
	str r3, [r1]
	ldr r3, .L_0801e248
	lsls r4, r4, #2
	adds r1, r2, r3
	adds r3, r0, r4
	ldr r3, [r3]
	movs r4, #192
	str r3, [r1]
	ldr r3, .L_0801e24c
	lsls r4, r4, #2
	adds r1, r2, r3
	adds r3, r0, r4
	ldr r3, [r3]
	movs r4, #128
	str r3, [r1]
	ldr r3, .L_0801e250
	lsls r4, r4, #3
	adds r1, r2, r3
	adds r3, r0, r4
	ldr r3, [r3]
	movs r4, #160
	str r3, [r1]
	ldr r3, .L_0801e254
	lsls r4, r4, #3
	adds r1, r2, r3
	adds r3, r0, r4
	ldr r3, [r3]
	movs r4, #192
	str r3, [r1]
	ldr r3, .L_0801e258
	lsls r4, r4, #3
	adds r1, r2, r3
	adds r3, r0, r4
	ldr r3, [r3]
	movs r4, #224
	str r3, [r1]
	ldr r3, .L_0801e25c
	lsls r4, r4, #3
	adds r1, r2, r3
	adds r3, r0, r4
	ldr r3, [r3]
	str r3, [r1]
	movs r3, #1
	ldr r1, [sp, #16]
	ldr r2, [sp, #12]
	negs r3, r3
	add r9, r3
	adds r1, #2
	adds r2, #2
	mov r4, r9
	str r1, [sp, #16]
	str r2, [sp, #12]
	adds r0, #4
	cmp r4, #0
	beq .L_0801e22a
	b .L_0801e0d0
.L_0801e22a:
	ldr r0, [sp, #8]
	bl Runtime_BumpFree
	adds r0, r6, #0
	add sp, #68
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
.L_0801e244:
	.4byte 0x06000004
.L_0801e248:
	.4byte 0x06000008
.L_0801e24c:
	.4byte 0x0600000c
.L_0801e250:
	.4byte 0x06000010
.L_0801e254:
	.4byte 0x06000014
.L_0801e258:
	.4byte 0x06000018
.L_0801e25c:
	.4byte 0x0600001c
