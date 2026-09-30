.syntax unified
	.thumb
	.global Func_0802c8a0
	.thumb_func
Func_0802c8a0:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r2, [r3, #28]
	ldr r5, [r3, #32]
	mov r10, r2
	movs r2, #128
	lsls r2, r2, #1
	adds r3, r5, r2
	movs r2, #1
	mov r8, r2
	mov r2, r8
	strb r2, [r3]
	ldr r0, .L_0802c964
	bl Func_08014694
	movs r2, #144
	lsls r2, r2, #4
	adds r2, #114
	adds r3, r5, r2
	mov r2, r8
	strb r2, [r3]
	movs r2, #144
	lsls r2, r2, #1
	adds r3, r5, r2
	ldr r6, [r3]
	ldr r0, [r6, #12]
	bl Resource_GetTableEntry
	ldr r1, .L_0802c968
	bl Func_0801587c
	movs r3, #151
	lsls r3, r3, #4
	adds r7, r5, r3
	movs r3, #0
	ldrsb r3, [r7, r3]
	cmp r3, #0
	beq .L_0802c900
	ldr r0, [r6, #16]
	bl Resource_GetTableEntry
	ldr r1, .L_0802c96c
	bl Func_0801587c
.L_0802c900:
	movs r3, #0
	strb r3, [r7]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_0802c970
	ldr r1, .L_0802c974
	ldr r2, .L_0802c978
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #1
	bl WaitFrames
	ldr r3, .L_0802c97c
	mov r2, r8
	ldr r3, [r3]
	ldr r1, .L_0802c980
	ands r3, r2
	lsls r0, r3, #2
	adds r0, r0, r3
	lsls r0, r0, #10
	movs r3, #200
	lsls r3, r3, #4
	add r0, r10
	adds r0, r0, r3
	bl Func_0802dd08
	movs r3, #130
	lsls r3, r3, #1
	adds r2, r5, r3
	movs r3, #200
	strh r3, [r2]
	adds r3, #62
	adds r2, r5, r3
	movs r3, #255
	strh r3, [r2]
	ldr r2, .L_0802c984
	ldr r3, .L_0802c988
	str r3, [r2]
	movs r2, #144
	lsls r2, r2, #4
	adds r2, #113
	adds r3, r5, r2
	mov r2, r8
	strb r2, [r3]
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_0802c964:
	.4byte Func_0802cb64
.L_0802c968:
	.4byte Data_02038000
.L_0802c96c:
	.4byte Data_0203a000
.L_0802c970:
	.4byte 0x06004000
.L_0802c974:
	.4byte Data_0201c000
.L_0802c978:
	.4byte 0x84000800
.L_0802c97c:
	.4byte Data_0300122c
.L_0802c980:
	.4byte gMapCellBuffer
.L_0802c984:
	.4byte Data_030011f8
.L_0802c988:
	.4byte Func_0802c864
