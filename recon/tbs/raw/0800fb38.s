.syntax unified
	.thumb
	.global Map_LoadLayeredScene
	.thumb_func
Map_LoadLayeredScene:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r1, #128
	lsls r1, r1, #19
	ldrh r2, [r1]
	ldr r3, .L_0800fd2c
	ands r3, r2
	adds r6, r0, #0
	strh r3, [r1]
	movs r0, #0
	sub sp, #12
	bl Blend_SetDarkenTarget0
	lsls r3, r6, #1
	ldr r2, .L_0800fd30
	adds r3, r3, r6
	movs r6, #202
	lsls r6, r6, #1
	lsls r3, r3, #2
	adds r3, r3, r2
	adds r1, r6, #0
	movs r0, #8
	str r3, [sp, #8]
	bl Runtime_AllocateBlock
	adds r1, r6, #0
	ldr r3, .L_0800fd34
	mov r8, r0
	bl _call_via_r3
	ldr r2, [sp, #8]
	ldr r3, .L_0800fd38
	ldrh r0, [r2]
	adds r0, r0, r3
	bl Resource_GetTableEntry
	adds r5, r0, #0
	ldr r3, [r5, #36]
	ldr r1, .L_0800fd3c
	adds r0, r5, r3
	bl Resource_DecodeType01
	bl Tilemap_DecodeStagedBuffer
	ldr r3, [r5, #40]
	ldr r1, .L_0800fd40
	adds r0, r5, r3
	bl Resource_DecodeType01
	ldr r3, [r5, #44]
	ldr r1, .L_0800fd44
	adds r0, r5, r3
	bl Resource_DecodeType01
	bl Tilemap_ConvertBuffer
	ldr r0, [r5, #48]
	cmp r0, #0
	beq .L_0800fbc8
	ldr r6, .L_0800fd48
	adds r0, r5, r0
	adds r1, r6, #0
	bl Resource_DecodeType01
	adds r0, r6, #0
	bl MapAnimation_StartChannels
.L_0800fbc8:
	ldr r0, [r5, #52]
	cmp r0, #0
	beq .L_0800fbde
	ldr r6, .L_0800fd4c
	adds r0, r5, r0
	adds r1, r6, #0
	bl Resource_DecodeType01
	adds r0, r6, #0
	bl DisplayBlend_StartScript
.L_0800fbde:
	ldr r3, [r5, #56]
	mov r2, r8
	adds r3, r5, r3
	str r3, [r2, #16]
	ldrb r3, [r5]
	adds r2, #236
	lsls r3, r3, #19
	str r3, [r2]
	ldrb r3, [r5, #1]
	adds r2, #4
	lsls r3, r3, #19
	str r3, [r2]
	ldrb r3, [r5, #2]
	adds r2, #4
	lsls r3, r3, #19
	str r3, [r2]
	ldrb r3, [r5, #3]
	adds r2, #4
	lsls r3, r3, #19
	str r3, [r2]
	mov r3, r8
	adds r3, #228
	str r3, [sp, #4]
	ldr r2, [sp, #4]
	movs r3, #0
	str r3, [r2]
	mov r2, r8
	adds r2, #232
	str r2, [sp, #0]
	str r3, [r2]
	movs r3, #128
	ldrb r2, [r5, #4]
	lsls r3, r3, #1
	add r3, r8
	strb r2, [r3]
	ldr r2, .L_0800fd50
	ldrb r3, [r5, #5]
	add r2, r8
	strb r3, [r2]
	movs r2, #129
	ldrb r3, [r5, #6]
	lsls r2, r2, #1
	add r2, r8
	strb r3, [r2]
	movs r7, #130
	ldr r3, .L_0800fd54
	lsls r7, r7, #1
	adds r6, r5, #0
	movs r2, #2
	add r7, r8
	adds r6, #12
	mov r9, r3
	mov r11, r2
.L_0800fc48:
	ldrb r0, [r6]
	ldrb r2, [r6, #1]
	lsls r3, r0, #19
	str r3, [r7, #8]
	mov lr, r3
	lsls r3, r2, #19
	str r3, [r7, #12]
	mov r10, r3
	movs r3, #4
	ldrsb r3, [r6, r3]
	lsls r3, r3, #12
	str r3, [r7, #24]
	movs r3, #5
	ldrsb r3, [r6, r3]
	lsls r3, r3, #12
	str r3, [r7, #28]
	ldrb r3, [r6, #6]
	strh r3, [r7, #40]
	ldrb r3, [r6, #7]
	lsrs r2, r2, #1
	strh r3, [r7, #42]
	lsrs r0, r0, #1
	movs r3, #0
	lsls r2, r2, #7
	str r3, [r7, #32]
	str r3, [r7, #36]
	movs r1, #2
	ldrsb r1, [r6, r1]
	movs r4, #3
	ldrsb r4, [r6, r4]
	adds r2, r2, r0
	ldr r3, .L_0800fd44
	lsls r2, r2, #2
	adds r2, r2, r3
	lsls r1, r1, #12
	lsls r4, r4, #12
	str r1, [r7, #16]
	str r4, [r7, #20]
	str r2, [r7, #44]
	ldr r2, [sp, #4]
	ldr r0, [r2]
	movs r0, r0
	mov r12, pc
	bx r9
	add r0, lr
	str r0, [r7]
	ldr r3, [sp, #0]
	adds r1, r4, #0
	ldr r0, [r3]
	movs r0, r0
	mov r12, pc
	bx r9
	movs r2, #1
	negs r2, r2
	add r11, r2
	add r0, r10
	mov r3, r11
	str r0, [r7, #4]
	adds r6, #8
	adds r7, #48
	cmp r3, #0
	bge .L_0800fc48
	movs r3, #128
	lsls r3, r3, #5
	mov r2, r8
	movs r1, #128
	strh r3, [r2, #20]
	lsls r1, r1, #1
	add r1, r8
	ldrb r3, [r1]
	cmp r3, #0
	beq .L_0800fcde
	movs r3, #192
	lsls r3, r3, #5
	strh r3, [r2, #20]
.L_0800fcde:
	ldr r0, .L_0800fd50
	add r0, r8
	ldrb r3, [r0]
	cmp r3, #0
	beq .L_0800fcf4
	mov r2, r8
	ldrh r3, [r2, #20]
	ldr r2, .L_0800fd24
	orrs r3, r2
	mov r2, r8
	strh r3, [r2, #20]
.L_0800fcf4:
	movs r3, #129
	lsls r3, r3, #1
	add r3, r8
	mov r12, r3
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_0800fd0e
	mov r2, r8
	ldrh r3, [r2, #20]
	ldr r2, .L_0800fd28
	orrs r3, r2
	mov r2, r8
	strh r3, [r2, #20]
.L_0800fd0e:
	ldrb r3, [r5, #7]
	ldrb r2, [r1]
	lsls r3, r3, #2
	orrs r2, r3
	movs r3, #160
	lsls r3, r3, #3
	orrs r2, r3
	ldr r3, .L_0800fd58
	strh r2, [r3]
	b .L_0800fd5c
	.2byte 0x0000
.L_0800fd24:
	.4byte 0x00000400
.L_0800fd28:
	.4byte 0x00000200
.L_0800fd2c:
	.4byte 0x0000c1ff
.L_0800fd30:
	.4byte Data_08013784
.L_0800fd34:
	.4byte IwramClearWords
.L_0800fd38:
	.4byte 0x00000128
.L_0800fd3c:
	.4byte gMapCellBuffer + 0x1
.L_0800fd40:
	.4byte gMapCollision
.L_0800fd44:
	.4byte gMapCellBuffer
.L_0800fd48:
	.4byte gMapCollision + 0x1000
.L_0800fd4c:
	.4byte gMapCollision + 0x1e00
.L_0800fd50:
	.4byte 0x00000101
.L_0800fd54:
	.4byte IwramMulQ16ReturnIp
.L_0800fd58:
	.4byte 0x0400000e
.L_0800fd5c:
	ldrb r2, [r0]
	ldrb r3, [r5, #8]
	lsls r3, r3, #2
	orrs r2, r3
	movs r3, #192
	lsls r3, r3, #3
	orrs r2, r3
	ldr r3, .L_0800fe70
	strh r2, [r3]
	mov r3, r12
	ldrb r2, [r3]
	ldrb r3, [r5, #9]
	lsls r3, r3, #2
	orrs r2, r3
	movs r3, #224
	lsls r3, r3, #3
	orrs r2, r3
	ldr r3, .L_0800fe74
	strh r2, [r3]
	movs r5, #184
	lsls r5, r5, #1
	adds r0, r5, #0
	bl GameFlag_IsSet
	cmp r0, #0
	beq .L_0800fd98
	adds r0, r5, #0
	bl GameFlag_ClearBitFar
	b .L_0800fe42
.L_0800fd98:
	movs r2, #128
	lsls r2, r2, #7
	mov r11, r2
	mov r0, r11
	bl Runtime_BumpAllocate
	adds r7, r0, #0
	cmp r7, #0
	beq .L_0800fe42
	movs r3, #160
	lsls r3, r3, #19
	mov r8, r3
	movs r2, #0
	ldrsh r3, [r3, r2]
	ldr r2, [sp, #8]
	mov r10, r3
	ldr r3, .L_0800fe78
	ldrh r0, [r2, #2]
	mov r9, r3
	add r0, r9
	bl Resource_GetTableEntry
	adds r1, r7, #0
	bl Resource_DecodeType01
	ldr r3, .L_0800fe7c
	mov r2, r10
	strh r2, [r7]
	movs r2, #224
	mov r10, r3
	adds r1, r7, #0
	lsls r2, r2, #1
	mov r0, r8
	bl _call_via_sl
	ldr r2, [sp, #8]
	ldrh r0, [r2, #4]
	add r0, r9
	bl Resource_GetTableEntry
	adds r1, r7, #0
	bl Resource_DecodeType2
	mov r2, r11
	adds r1, r7, #0
	ldr r0, .L_0800fe80
	bl _call_via_sl
	ldr r3, [sp, #8]
	ldrh r0, [r3, #6]
	add r0, r9
	bl Resource_GetTableEntry
	adds r1, r7, #0
	bl Resource_DecodeType2
	adds r1, r7, #0
	mov r2, r11
	ldr r0, .L_0800fe84
	bl _call_via_sl
	ldr r2, [sp, #8]
	ldrh r0, [r2, #8]
	add r0, r9
	bl Resource_GetTableEntry
	adds r1, r7, #0
	bl Resource_DecodeType2
	adds r1, r7, #0
	mov r2, r11
	ldr r0, .L_0800fe88
	bl _call_via_sl
	ldr r3, [sp, #8]
	ldrh r0, [r3, #10]
	add r0, r9
	bl Resource_GetTableEntry
	ldr r1, .L_0800fe8c
	bl Resource_DecodeType2
	adds r0, r7, #0
	bl Runtime_BumpFree
.L_0800fe42:
	ldr r3, .L_0800fe90
	movs r2, #0
	strh r2, [r3]
	adds r3, #4
	strh r2, [r3]
	movs r2, #160
	lsls r2, r2, #1
	subs r3, #80
	strh r2, [r3]
	ldr r0, .L_0800fe94
	ldr r1, .L_0800fe98
	bl Scheduler_AddOrUpdateCallback
	movs r0, #2
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
.L_0800fe70:
	.4byte 0x0400000c
.L_0800fe74:
	.4byte 0x0400000a
.L_0800fe78:
	.4byte 0x00000128
.L_0800fe7c:
	.4byte IwramCopyWords
.L_0800fe80:
	.4byte 0x06004000
.L_0800fe84:
	.4byte 0x06008000
.L_0800fe88:
	.4byte 0x0600c000
.L_0800fe8c:
	.4byte gMapLayerData
.L_0800fe90:
	.4byte 0x0400004c
.L_0800fe94:
	.4byte Map_UpdateLayerScroll
.L_0800fe98:
	.4byte 0x00000c85
