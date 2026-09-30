.syntax unified
	.thumb
	.global Func_080caa4c
	.thumb_func
Func_080caa4c:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #108]
	mov r9, r0
	mov r8, r3
	ldr r3, .L_080caca8
	movs r0, #133
	lsls r0, r0, #2
	adds r3, r3, r0
	movs r5, #246
	ldr r6, [r3]
	lsls r5, r5, #1
	ldr r3, .L_080cacac
	add r5, r8
	adds r2, r5, #0
	ldmia r3!, {r1, r4, r7}
	stmia r2!, {r1, r4, r7}
	ldmia r3!, {r0, r1, r4}
	stmia r2!, {r0, r1, r4}
	movs r2, #129
	lsls r2, r2, #2
	add r2, r8
	ldmia r3!, {r0, r1, r7}
	stmia r2!, {r0, r1, r7}
	ldmia r3!, {r0, r4, r7}
	stmia r2!, {r0, r4, r7}
	mov r3, r8
	movs r2, #0
	adds r3, #12
	mov r12, r8
.L_080caa96:
	str r2, [r3]
	subs r3, #4
	cmp r3, r12
	bge .L_080caa96
	bl Func_080cad64
	movs r3, #255
	lsls r3, r3, #8
	ldr r7, .L_080caca8
	adds r3, #255
	strh r3, [r5, #2]
	strh r6, [r5]
	movs r2, #254
	lsls r2, r2, #1
	adds r3, r7, r2
	ldr r3, [r3]
	movs r1, #0
	str r3, [r5, #8]
	str r1, [r5, #12]
	movs r4, #129
	lsls r4, r4, #2
	adds r3, r7, r4
	ldr r3, [r3]
	movs r0, #130
	str r3, [r5, #16]
	lsls r0, r0, #2
	adds r3, r7, r0
	ldr r3, [r3]
	mov r10, r1
	strh r3, [r5, #20]
	adds r1, r6, #0
	adds r0, r5, #0
	bl Event_SpawnObjectTable
	movs r1, #128
	lsls r1, r1, #2
	adds r1, r1, r7
	ldr r4, [r1]
	mov r11, r1
	cmp r4, #0
	beq .L_080caaf8
	lsls r6, r6, #2
	adds r3, r6, #0
	adds r3, #20
	mov r2, r8
	ldr r5, [r2, r3]
	str r4, [r5, #12]
	str r4, [r5, #20]
	b .L_080caafa
.L_080caaf8:
	lsls r6, r6, #2
.L_080caafa:
	mov r0, r9
	movs r1, #8
	bl Event_SpawnObjectTable
	adds r3, r6, #0
	adds r3, #20
	mov r4, r8
	movs r0, #131
	ldr r5, [r4, r3]
	lsls r0, r0, #2
	adds r3, r7, r0
	ldrh r3, [r3]
	adds r2, r5, #0
	adds r2, #34
	strb r3, [r2]
	adds r3, r5, #0
	adds r3, #100
	mov r1, r10
	strh r1, [r3]
	mov r4, r10
	adds r3, #2
	strh r4, [r3]
	movs r3, #197
	lsls r3, r3, #1
	add r3, r8
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #3
	bne .L_080cab5e
	adds r0, #106
	adds r3, r7, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	beq .L_080cab50
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #18
	adds r2, r7, r3
	movs r3, #7
	strb r3, [r2]
	b .L_080cac18
.L_080cab50:
	movs r4, #128
	lsls r4, r4, #2
	adds r4, #18
	adds r2, r7, r4
	movs r3, #1
	strb r3, [r2]
	b .L_080cac18
.L_080cab5e:
	movs r0, #128
	lsls r0, r0, #2
	adds r0, #118
	adds r3, r7, r0
	movs r1, #0
	ldrsh r3, [r3, r1]
	cmp r3, #0
	beq .L_080cab7c
	movs r3, #128
	lsls r3, r3, #2
	adds r3, #18
	adds r2, r7, r3
	movs r3, #6
	strb r3, [r2]
	b .L_080cac18
.L_080cab7c:
	movs r4, #128
	lsls r4, r4, #2
	adds r4, #18
	adds r6, r7, r4
	movs r7, #0
	strb r7, [r6]
	ldrb r2, [r2]
	movs r3, #192
	lsls r3, r3, #18
	ldr r4, [r3, #32]
	lsls r3, r2, #3
	subs r3, r3, r2
	movs r1, #156
	lsls r3, r3, #3
	lsls r1, r1, #1
	adds r0, r3, r1
	ldr r1, [r5, #8]
	adds r3, r1, #0
	cmp r1, #0
	bge .L_080caba8
	ldr r2, .L_080cacb0
	adds r3, r1, r2
.L_080caba8:
	asrs r2, r3, #20
	ldr r3, [r5, #16]
	cmp r3, #0
	bge .L_080cabb4
	ldr r7, .L_080cacb0
	adds r3, r3, r7
.L_080cabb4:
	asrs r3, r3, #20
	lsls r3, r3, #7
	adds r3, r2, r3
	ldr r2, [r4, r0]
	lsls r3, r3, #2
	mov r7, r11
	adds r2, r2, r3
	ldr r0, .L_080cacb4
	ldr r3, [r7]
	adds r4, r2, r0
	cmp r3, #0
	beq .L_080cac1a
	ldrb r2, [r2, #3]
	movs r0, #64
	adds r3, r0, #0
	ands r3, r2
	cmp r3, #0
	beq .L_080cac1a
	ldrb r2, [r4, #3]
	adds r3, r0, #0
	ands r3, r2
	cmp r3, #0
	beq .L_080cac1a
	movs r3, #2
	strb r3, [r6]
	ldr r0, .L_080cacb8
	ldr r2, [r5, #16]
	ldr r1, [r5, #8]
	adds r2, r2, r0
	movs r0, #0
	bl Map_GetTerrainHeightFar
	ldr r1, .L_080cacbc
	ldr r3, [r5, #12]
	adds r0, r0, r1
	adds r3, r3, r0
	str r3, [r5, #12]
	str r3, [r5, #20]
	adds r3, r5, #0
	adds r3, #85
	movs r2, #0
	strb r2, [r3]
	adds r0, r5, #0
	movs r1, #0
	bl ObjectDispatch_SetSingleChildField26Far
	movs r1, #12
	adds r0, r5, #0
	bl Object_SetMode
.L_080cac18:
	ldr r1, [r5, #8]
.L_080cac1a:
	ldr r3, .L_080caca8
	movs r4, #128
	lsls r4, r4, #2
	adds r4, #118
	adds r3, r3, r4
	movs r7, #0
	ldrsh r3, [r3, r7]
	cmp r3, #0
	beq .L_080cac4c
	mov r0, r8
	ldr r2, [r0, #52]
	str r1, [r2, #8]
	movs r1, #0
	ldr r3, [r5, #12]
	str r3, [r2, #12]
	ldr r3, [r5, #16]
	str r3, [r2, #16]
	ldr r3, [r5, #20]
	str r3, [r2, #20]
	ldrh r3, [r5, #6]
	strh r3, [r2, #6]
	str r1, [r5, #8]
	str r1, [r5, #16]
	adds r5, r2, #0
	ldr r1, [r5, #8]
.L_080cac4c:
	movs r0, #128
	ldr r2, [r5, #12]
	ldr r3, [r5, #16]
	lsls r0, r0, #8
	bl Func_080200c0
	ldr r3, [r5, #20]
	adds r6, r0, #0
	str r3, [r6, #20]
	adds r1, r5, #0
	bl ObjectDispatch_InitFromTable4WithArgumentFar
	movs r3, #197
	lsls r3, r3, #1
	add r3, r8
	ldrb r3, [r3]
	lsls r3, r3, #24
	asrs r3, r3, #24
	cmp r3, #3
	bne .L_080cac86
	movs r1, #133
	ldr r0, [r5, #80]
	lsls r1, r1, #1
	bl ResourceMetadata_RegisterFar
	movs r3, #15
	strb r3, [r0, #5]
	movs r3, #9
	strb r3, [r0, #6]
.L_080cac86:
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #32]
	adds r3, r6, #0
	adds r3, #8
	str r3, [r2]
	movs r3, #230
	lsls r3, r3, #1
	add r3, r8
	str r6, [r3]
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080caca8:
	.4byte gPartyState
.L_080cacac:
	.4byte Data_080f21d4
.L_080cacb0:
	.4byte 0x000fffff
.L_080cacb4:
	.4byte 0xfffffe00
.L_080cacb8:
	.4byte 0xfff00000
.L_080cacbc:
	.4byte 0xffe00000
