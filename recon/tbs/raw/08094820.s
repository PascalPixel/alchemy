.syntax unified
	.thumb
	.global Unnamed_08094820
	.thumb_func
Unnamed_08094820:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_0809491c
	ldr r0, [r3]
	ldr r3, [r3, #84]
	mov r10, r0
	mov r8, r3
	mov r3, r10
	adds r3, #228
	ldr r1, [r3]
	sub sp, #12
	str r1, [sp, #8]
	ldr r3, [r3, #4]
	mov r7, r8
	str r3, [sp, #4]
	movs r2, #0
	movs r3, #63
	adds r7, #8
	mov r9, r2
	mov r11, r3
.L_08094852:
	ldrh r3, [r7, #28]
	ldr r1, .L_08094920
	adds r3, r3, r1
	adds r2, r1, #0
	ands r2, r3
	strh r3, [r7, #28]
	cmp r2, r1
	bne .L_08094864
	b .L_08094984
.L_08094864:
	movs r0, #179
	lsls r0, r0, #1
	bl GameFlag_IsSet
	cmp r0, #0
	beq .L_08094876
	ldrh r3, [r7, #28]
	adds r3, #1
	strh r3, [r7, #28]
.L_08094876:
	ldrh r2, [r7, #28]
	lsls r3, r2, #2
	adds r3, r3, r2
	ldr r2, .L_08094924
	lsls r3, r3, #1
	adds r4, r3, r2
	ldr r5, [sp, #8]
	ldr r3, [r7, #12]
	subs r2, r3, r5
	cmp r2, #0
	bge .L_08094890
	ldr r0, .L_08094920
	adds r2, r2, r0
.L_08094890:
	movs r1, #0
	ldrsh r3, [r4, r1]
	asrs r2, r2, #16
	adds r1, r2, r3
	ldr r2, [r7, #16]
	ldr r3, [r7, #20]
	subs r3, r3, r2
	ldr r2, [sp, #4]
	subs r3, r3, r2
	adds r4, #2
	cmp r3, #0
	bge .L_080948ac
	ldr r5, .L_08094920
	adds r3, r3, r5
.L_080948ac:
	movs r0, #0
	ldrsh r2, [r4, r0]
	asrs r3, r3, #16
	adds r0, r3, r2
	adds r3, r1, #0
	adds r3, #16
	adds r4, #2
	cmp r3, #255
	bhi .L_08094946
	movs r2, #32
	negs r2, r2
	cmp r0, r2
	blt .L_08094946
	cmp r0, #159
	bgt .L_08094946
	movs r5, #13
	ldrb r2, [r7, #9]
	negs r5, r5
	adds r3, r5, #0
	ands r2, r3
	movs r3, #4
	orrs r2, r3
	ldr r3, .L_0809490c
	strb r2, [r7, #9]
	ands r1, r3
	ldr r2, .L_08094910
	ldrh r3, [r7, #6]
	ands r3, r2
	orrs r3, r1
	strh r3, [r7, #6]
	strb r0, [r7, #4]
	mov r0, r8
	ldrh r3, [r4]
	ldr r1, [r0, #4]
	adds r1, r1, r3
	ldr r3, .L_08094914
	ldr r2, .L_08094918
	ands r1, r3
	ldrh r3, [r7, #8]
	ands r3, r2
	orrs r3, r1
	strh r3, [r7, #8]
	adds r4, #2
	ldrb r2, [r4]
	ldrb r1, [r7, #5]
	mov r3, r11
	b .L_08094928
	.2byte 0x0000
.L_0809490c:
	.4byte 0x000001ff
.L_08094910:
	.4byte 0xfffffe00
.L_08094914:
	.4byte 0x000003ff
.L_08094918:
	.4byte 0xfffffc00
.L_0809491c:
	.4byte gMapWork
.L_08094920:
	.4byte 0x0000ffff
.L_08094924:
	.4byte Data_0809ef84
.L_08094928:
	lsls r2, r2, #6
	ands r3, r1
	orrs r3, r2
	strb r3, [r7, #5]
	ldrb r1, [r7, #7]
	ldrb r2, [r4, #2]
	mov r3, r11
	ands r3, r1
	lsls r2, r2, #6
	orrs r3, r2
	strb r3, [r7, #7]
	adds r0, r7, #0
	movs r1, #240
	bl Runtime_PushSlotEntry
.L_08094946:
	ldrh r3, [r7, #28]
	cmp r3, #0
	bne .L_08094984
	mov r1, r10
	ldr r6, [r1]
	bl Random16
	ldr r3, [r6]
	lsls r0, r0, #8
	ldr r5, .L_080949a4
	adds r3, r3, r0
	adds r1, r3, r5
	str r1, [sp, #0]
	bl Random16
	ldr r3, [r6, #8]
	lsls r0, r0, #8
	adds r3, r3, r0
	ldr r1, [sp, #0]
	adds r0, r3, r5
	str r1, [r7, #12]
	str r0, [r7, #20]
	asrs r2, r0, #16
	asrs r1, r1, #16
	movs r0, #0
	bl Map_GetTerrainHeightFar
	movs r3, #16
	lsls r0, r0, #16
	str r0, [r7, #16]
	strh r3, [r7, #28]
.L_08094984:
	movs r2, #1
	add r9, r2
	mov r3, r9
	adds r7, #32
	cmp r3, #31
	bhi .L_08094992
	b .L_08094852
.L_08094992:
	add sp, #12
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_080949a4:
	.4byte 0xff800000
