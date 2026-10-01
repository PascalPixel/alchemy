.syntax unified
	.thumb
	.global Func_0803f9c0
	.thumb_func
Func_0803f9c0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	adds r6, r1, #0
	adds r0, #12
	movs r1, #24
	sub sp, #8
	bl Math_Mod
	adds r5, r0, #0
	ldr r2, .L_0803fb2c
	lsls r5, r5, #18
	asrs r5, r5, #16
	adds r0, r5, #0
	movs r1, #96
	mov r8, r2
	bl Math_Mod
	lsls r0, r0, #16
	mov r2, r8
	asrs r0, r0, #16
	ldrb r3, [r2, r0]
	subs r6, #7
	lsls r6, r6, #16
	asrs r6, r6, #16
	adds r3, r3, r6
	adds r0, r5, #0
	lsls r3, r3, #16
	asrs r3, r3, #16
	movs r1, #96
	adds r0, #32
	mov r10, r3
	bl Math_Mod
	mov r2, r8
	ldrb r3, [r2, r0]
	adds r5, #64
	adds r3, r3, r6
	lsls r3, r3, #16
	adds r0, r5, #0
	movs r1, #96
	asrs r7, r3, #16
	bl Math_Mod
	mov r2, r8
	ldrb r3, [r2, r0]
	mov r2, r10
	adds r3, r3, r6
	lsls r3, r3, #16
	asrs r3, r3, #16
	cmp r2, #0
	bge .L_0803fa30
	movs r2, #0
	mov r10, r2
.L_0803fa30:
	mov r2, r10
	cmp r2, #31
	ble .L_0803fa3a
	movs r2, #31
	mov r10, r2
.L_0803fa3a:
	cmp r7, #0
	bge .L_0803fa40
	movs r7, #0
.L_0803fa40:
	cmp r7, #31
	ble .L_0803fa46
	movs r7, #31
.L_0803fa46:
	cmp r3, #0
	bge .L_0803fa4c
	movs r3, #0
.L_0803fa4c:
	cmp r3, #31
	ble .L_0803fa52
	movs r3, #31
.L_0803fa52:
	mov r5, sp
	mov r2, r10
	strh r2, [r5]
	strh r7, [r5, #2]
	strh r3, [r5, #4]
	movs r3, #238
	lsls r3, r3, #8
	adds r3, #238
	movs r2, #204
	mov r10, r3
	lsls r2, r2, #8
	adds r2, #204
	ldr r3, .L_0803fb30
	adds r0, r5, #0
	mov r1, r10
	mov r9, r2
	bl Func_0803f974
	ldr r3, .L_0803fb34
	lsls r0, r0, #16
	lsrs r0, r0, #16
	strh r0, [r3]
	movs r1, #213
	movs r6, #187
	lsls r6, r6, #8
	adds r6, #187
	lsls r1, r1, #8
	mov r3, r10
	adds r1, #85
	adds r0, r5, #0
	adds r2, r6, #0
	bl Func_0803f974
	ldr r3, .L_0803fb38
	lsls r0, r0, #16
	lsrs r0, r0, #16
	strh r0, [r3]
	movs r3, #170
	lsls r3, r3, #8
	adds r3, #170
	mov r8, r3
	adds r0, r5, #0
	mov r3, r9
	adds r1, r6, #0
	mov r2, r8
	bl Func_0803f974
	ldr r3, .L_0803fb3c
	lsls r0, r0, #16
	lsrs r0, r0, #16
	strh r0, [r3]
	movs r1, #162
	movs r2, #153
	lsls r1, r1, #8
	lsls r2, r2, #8
	mov r3, r8
	adds r1, #33
	adds r2, #153
	adds r0, r5, #0
	bl Func_0803f974
	ldr r3, .L_0803fb40
	lsls r0, r0, #16
	lsrs r0, r0, #16
	strh r0, [r3]
	movs r1, #132
	movs r2, #221
	lsls r1, r1, #9
	lsls r2, r2, #8
	adds r1, #136
	adds r2, #221
	ldr r3, .L_0803fb44
	adds r0, r5, #0
	bl Func_0803f974
	ldr r3, .L_0803fb48
	lsls r0, r0, #16
	lsrs r0, r0, #16
	strh r0, [r3]
	movs r1, #145
	lsls r1, r1, #9
	adds r1, #33
	ldr r3, .L_0803fb4c
	adds r0, r5, #0
	mov r2, r10
	bl Func_0803f974
	ldr r3, .L_0803fb50
	lsls r0, r0, #16
	lsrs r0, r0, #16
	strh r0, [r3]
	ldr r1, .L_0803fb54
	movs r2, #128
	lsls r2, r2, #9
	ldr r3, .L_0803fb58
	adds r0, r5, #0
	bl Func_0803f974
	ldr r3, .L_0803fb5c
	lsls r0, r0, #16
	lsrs r0, r0, #16
	strh r0, [r3]
	add sp, #8
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0803fb2c:
	.4byte Data_0805ea1c
.L_0803fb30:
	.4byte 0x00011110
.L_0803fb34:
	.4byte 0x050001e8
.L_0803fb38:
	.4byte 0x050001ea
.L_0803fb3c:
	.4byte 0x050001ec
.L_0803fb40:
	.4byte 0x050001ee
.L_0803fb44:
	.4byte 0x00013333
.L_0803fb48:
	.4byte 0x050001f0
.L_0803fb4c:
	.4byte 0x00015555
.L_0803fb50:
	.4byte 0x050001f2
.L_0803fb54:
	.4byte 0x00013bbb
.L_0803fb58:
	.4byte 0x00017777
.L_0803fb5c:
	.4byte 0x050001f4
