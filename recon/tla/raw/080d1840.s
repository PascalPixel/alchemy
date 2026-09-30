.syntax unified
	.thumb
	.global Func_080d1840
	.thumb_func
Func_080d1840:
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
	adds r3, #144
	ldr r3, [r3]
	sub sp, #16
	mov r11, r3
	ldr r3, [r2, #32]
	mov r5, r11
	adds r3, #228
	movs r2, #2
	ldrsh r1, [r3, r2]
	str r1, [sp, #8]
	movs r2, #6
	ldrsh r1, [r3, r2]
	str r1, [sp, #4]
	ldr r3, [r5, #24]
	cmp r3, #0
	bne .L_080d1876
	b .L_080d1a08
.L_080d1876:
	ldr r1, [r3, #16]
	ldr r7, [r3, #8]
	mov r10, r1
	movs r1, #22
	ldrsh r2, [r3, r1]
	adds r3, #34
	str r2, [sp, #0]
	movs r2, #189
	ldrb r3, [r3]
	lsls r2, r2, #1
	mov r8, r3
	ldr r3, .L_080d1924
	mov r0, r8
	adds r3, r3, r2
	ldrh r3, [r3]
	movs r2, #128
	lsrs r3, r3, #5
	mov r9, r3
	ldr r3, .L_080d1928
	lsls r2, r2, #13
	adds r7, r7, r3
	add r2, r10
	adds r1, r7, #0
	bl Map_GetTerrainHeightFar
	movs r2, #128
	lsls r2, r2, #14
	asrs r6, r0, #16
	add r2, r10
	mov r0, r8
	adds r1, r7, #0
	bl Map_GetTerrainHeightFar
	asrs r0, r0, #16
	subs r0, #16
	cmp r0, r6
	ble .L_080d18c2
	adds r6, r0, #0
.L_080d18c2:
	cmp r6, #0
	ble .L_080d1950
	ldr r1, [sp, #0]
	cmp r6, r1
	ble .L_080d1950
	ldr r3, .L_080d192c
	ldrb r2, [r5, #9]
	str r3, [r5, #4]
	movs r1, #13
	movs r3, #128
	lsls r3, r3, #3
	negs r1, r1
	str r3, [r5, #8]
	adds r3, r1, #0
	ands r3, r2
	strb r3, [r5, #9]
	ldr r3, .L_080d1918
	mov r2, r9
	ands r2, r3
	ldrh r0, [r5, #8]
	ldr r3, .L_080d1930
	ands r3, r0
	orrs r3, r2
	strh r3, [r5, #8]
	ldrb r3, [r5, #5]
	asrs r2, r7, #16
	ands r1, r3
	movs r3, #4
	orrs r1, r3
	ldr r3, .L_080d191c
	strb r1, [r5, #5]
	ands r2, r3
	ldr r3, [sp, #8]
	ldrh r1, [r5, #6]
	subs r2, r2, r3
	ldr r3, .L_080d1920
	adds r0, r5, #0
	ands r2, r3
	ldr r3, .L_080d1934
	ands r3, r1
	orrs r3, r2
	mov r1, r10
	b .L_080d1938
.L_080d1918:
	.4byte 0x000003ff
.L_080d191c:
	.4byte 0x0000fff0
.L_080d1920:
	.4byte 0x000001ff
.L_080d1924:
	.4byte ResourceTableEntries
.L_080d1928:
	.4byte 0xfff80000
.L_080d192c:
	.4byte 0x40000800
.L_080d1930:
	.4byte 0xfffffc00
.L_080d1934:
	.4byte 0xfffffe00
.L_080d1938:
	strh r3, [r5, #6]
	movs r2, #240
	asrs r3, r1, #16
	ands r3, r2
	ldr r2, [sp, #4]
	movs r1, #0
	subs r3, r3, r2
	subs r3, r3, r6
	adds r3, #16
	strb r3, [r5, #4]
	bl Func_080140d8
.L_080d1950:
	movs r2, #128
	lsls r2, r2, #13
	adds r7, r7, r2
	adds r1, r7, #0
	add r2, r10
	mov r0, r8
	bl Map_GetTerrainHeightFar
	movs r2, #128
	lsls r2, r2, #14
	asrs r6, r0, #16
	add r2, r10
	mov r0, r8
	adds r1, r7, #0
	bl Map_GetTerrainHeightFar
	asrs r0, r0, #16
	mov r5, r11
	subs r0, #16
	adds r5, #12
	cmp r0, r6
	ble .L_080d197e
	adds r6, r0, #0
.L_080d197e:
	cmp r6, #0
	ble .L_080d1a08
	ldr r3, [sp, #0]
	cmp r6, r3
	ble .L_080d1a08
	ldr r3, .L_080d19e0
	ldrb r1, [r5, #9]
	movs r2, #13
	str r3, [r5, #4]
	negs r2, r2
	movs r3, #0
	str r3, [r5, #8]
	adds r3, r2, #0
	ands r3, r1
	strb r3, [r5, #9]
	ldr r3, .L_080d19d4
	mov r1, r9
	ands r1, r3
	mov r9, r1
	ldr r3, .L_080d19e4
	ldrh r1, [r5, #8]
	adds r0, r5, #0
	ands r3, r1
	mov r1, r9
	orrs r3, r1
	strh r3, [r5, #8]
	ldrb r3, [r5, #5]
	ldrh r1, [r5, #6]
	ands r2, r3
	movs r3, #4
	orrs r2, r3
	ldr r3, .L_080d19d8
	strb r2, [r5, #5]
	asrs r2, r7, #16
	ands r2, r3
	ldr r3, [sp, #8]
	subs r2, r2, r3
	ldr r3, .L_080d19dc
	ands r2, r3
	ldr r3, .L_080d19e8
	ands r3, r1
	b .L_080d19ec
	.2byte 0x0000
.L_080d19d4:
	.4byte 0x000003ff
.L_080d19d8:
	.4byte 0x0000fff0
.L_080d19dc:
	.4byte 0x000001ff
.L_080d19e0:
	.4byte 0x40000800
.L_080d19e4:
	.4byte 0xfffffc00
.L_080d19e8:
	.4byte 0xfffffe00
.L_080d19ec:
	orrs r3, r2
	mov r1, r10
	strh r3, [r5, #6]
	movs r2, #240
	asrs r3, r1, #16
	ands r3, r2
	ldr r2, [sp, #4]
	movs r1, #0
	subs r3, r3, r2
	subs r3, r3, r6
	adds r3, #16
	strb r3, [r5, #4]
	bl Func_080140d8
.L_080d1a08:
	add sp, #16
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
