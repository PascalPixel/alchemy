.syntax unified
	.thumb
	.global Func_081a101c
	.thumb_func
Func_081a101c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r6, #144
	lsls r6, r6, #4
	sub sp, #44
	mov r10, r0
	adds r0, r6, #0
	str r1, [sp, #8]
	adds r7, r2, #0
	bl Runtime_BumpAllocateAlternatePool
	movs r1, #0
	movs r2, #192
	mov r3, r10
	str r0, [sp, #4]
	str r1, [sp, #0]
	mov r9, r2
	cmp r3, #0
	bne .L_081a1052
	movs r0, #1
	negs r0, r0
	b .L_081a1244
.L_081a1052:
	movs r5, #128
	lsls r5, r5, #2
	adds r0, r5, #0
	bl GameFlag_Test
	cmp r0, #0
	bne .L_081a1074
	ldr r3, .L_081a1254
	ldr r0, [sp, #4]
	adds r1, r6, #0
	movs r2, #0
	mov lr, r3
	.2byte 0xf800
	adds r0, r5, #0
	bl GameFlag_SetBit
	b .L_081a109a
.L_081a1074:
	ldr r4, [sp, #4]
	movs r5, #128
	lsls r5, r5, #4
	movs r2, #128
	adds r1, r4, r5
	ldr r3, .L_081a1258
	lsls r2, r2, #1
	adds r0, r4, #0
	mov lr, r3
	.2byte 0xf800
	ldr r1, [sp, #4]
	movs r2, #128
	lsls r2, r2, #1
	adds r0, r1, r2
	ldr r3, .L_081a1254
	adds r1, r5, #0
	movs r2, #0
	mov lr, r3
	.2byte 0xf800
.L_081a109a:
	mov r4, r10
	ldrb r0, [r4]
	movs r3, #0
	mov r8, r3
	adds r4, #1
	cmp r0, #0
	beq .L_081a10be
	ldr r2, .L_081a125c
.L_081a10aa:
	cmp r0, #31
	bls .L_081a10b6
	adds r3, r0, #0
	subs r3, #32
	ldrb r3, [r2, r3]
	add r8, r3
.L_081a10b6:
	ldrb r0, [r4]
	adds r4, #1
	cmp r0, #0
	bne .L_081a10aa
.L_081a10be:
	cmp r7, #2
	bne .L_081a10cc
	mov r4, r9
	mov r1, r8
	subs r4, r4, r1
	str r4, [sp, #0]
	b .L_081a10de
.L_081a10cc:
	cmp r7, #1
	bne .L_081a10de
	mov r2, r9
	mov r4, r8
	subs r3, r2, r4
	lsrs r2, r3, #31
	adds r3, r3, r2
	asrs r3, r3, #1
	str r3, [sp, #0]
.L_081a10de:
	mov r4, r10
	ldrb r0, [r4]
	movs r1, #0
	adds r4, #1
	mov r8, r1
	mov r10, r4
	cmp r0, #0
	beq .L_081a1166
.L_081a10ee:
	cmp r0, #31
	bls .L_081a115a
	movs r2, #32
	ldr r1, .L_081a1260
	negs r2, r2
	adds r2, r2, r0
	lsls r3, r2, #3
	adds r4, r1, r3
	mov lr, r2
	ldr r1, [sp, #0]
	ldr r2, [sp, #4]
	adds r3, r2, r1
	mov r2, r8
	adds r1, r3, r2
	movs r3, #0
	mov r12, r3
	movs r2, #1
	movs r3, #15
	mov r11, r2
	mov r9, r3
.L_081a1116:
	movs r3, #2
	adds r3, #255
	ldrb r7, [r4]
	movs r6, #128
	adds r4, #1
	movs r5, #7
	adds r2, r1, r3
.L_081a1124:
	adds r3, r7, #0
	ands r3, r6
	cmp r3, #0
	beq .L_081a1134
	mov r3, r11
	strb r3, [r2]
	mov r3, r9
	strb r3, [r1]
.L_081a1134:
	subs r5, #1
	adds r2, #1
	adds r1, #1
	lsrs r6, r6, #1
	cmp r5, #0
	bge .L_081a1124
	movs r2, #1
	add r12, r2
	mov r3, r12
	adds r1, #248
	cmp r3, #7
	ble .L_081a1116
	movs r3, #1
	cmp r0, #31
	bls .L_081a1158
	ldr r4, .L_081a125c
	mov r1, lr
	ldrb r3, [r4, r1]
.L_081a1158:
	add r8, r3
.L_081a115a:
	mov r2, r10
	ldrb r0, [r2]
	movs r3, #1
	add r10, r3
	cmp r0, #0
	bne .L_081a10ee
.L_081a1166:
	movs r4, #24
	mov r10, r4
	ldr r4, [sp, #4]
	movs r2, #96
	mov r8, r2
	movs r6, #128
	movs r3, #7
	movs r2, #192
	adds r1, r4, #0
	movs r7, #96
	lsls r6, r6, #1
	mov r12, r3
	mov lr, r2
.L_081a1180:
	cmp r7, #0
	beq .L_081a119e
	mov r5, r8
	adds r2, r4, #0
.L_081a1188:
	ldrb r3, [r2, #1]
	ldrb r0, [r2]
	lsls r3, r3, #4
	orrs r0, r3
	subs r5, #1
	strb r0, [r1]
	adds r2, #2
	adds r4, #2
	adds r1, #1
	cmp r5, #0
	bne .L_081a1188
.L_081a119e:
	subs r3, r1, r7
	mov r2, lr
	adds r1, r3, r6
	subs r3, r4, r2
	adds r4, r3, r6
	movs r3, #1
	negs r3, r3
	add r12, r3
	mov r2, r12
	cmp r2, #0
	bge .L_081a1180
	mov r3, r10
	cmp r3, #0
	beq .L_081a123c
	ldr r4, [sp, #8]
	ldr r0, [sp, #4]
	lsls r1, r4, #5
	mov r12, r10
.L_081a11c2:
	ldr r3, .L_081a1264
	ldr r4, .L_081a1268
	adds r2, r1, r3
	ldr r3, [r0]
	str r3, [r2]
	adds r2, r1, r4
	movs r4, #128
	lsls r4, r4, #1
	adds r3, r0, r4
	ldr r3, [r3]
	movs r4, #128
	str r3, [r2]
	ldr r3, .L_081a126c
	lsls r4, r4, #2
	adds r2, r1, r3
	adds r3, r0, r4
	ldr r3, [r3]
	movs r4, #192
	str r3, [r2]
	ldr r3, .L_081a1270
	lsls r4, r4, #2
	adds r2, r1, r3
	adds r3, r0, r4
	ldr r3, [r3]
	movs r4, #128
	str r3, [r2]
	ldr r3, .L_081a1274
	lsls r4, r4, #3
	adds r2, r1, r3
	adds r3, r0, r4
	ldr r3, [r3]
	movs r4, #160
	str r3, [r2]
	ldr r3, .L_081a1278
	lsls r4, r4, #3
	adds r2, r1, r3
	adds r3, r0, r4
	ldr r3, [r3]
	movs r4, #192
	str r3, [r2]
	ldr r3, .L_081a127c
	lsls r4, r4, #3
	adds r2, r1, r3
	adds r3, r0, r4
	ldr r3, [r3]
	movs r4, #224
	str r3, [r2]
	ldr r3, .L_081a1280
	lsls r4, r4, #3
	adds r2, r1, r3
	adds r3, r0, r4
	ldr r3, [r3]
	adds r1, #32
	str r3, [r2]
	movs r2, #1
	negs r2, r2
	add r12, r2
	mov r3, r12
	adds r0, #4
	cmp r3, #0
	bne .L_081a11c2
.L_081a123c:
	ldr r0, [sp, #4]
	bl Sys_Free
	movs r0, #0
.L_081a1244:
	add sp, #44
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_081a1254:
	.4byte IwramFillWords
.L_081a1258:
	.4byte IwramCopyWords
.L_081a125c:
	.4byte Data_081a2059
.L_081a1260:
	.4byte Data_081a2400
.L_081a1264:
	.4byte 0x06010000
.L_081a1268:
	.4byte 0x06010004
.L_081a126c:
	.4byte 0x06010008
.L_081a1270:
	.4byte 0x0601000c
.L_081a1274:
	.4byte 0x06010010
.L_081a1278:
	.4byte 0x06010014
.L_081a127c:
	.4byte 0x06010018
.L_081a1280:
	.4byte 0x0601001c
	.4byte 0x00004770
