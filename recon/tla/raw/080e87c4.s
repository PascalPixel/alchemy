.syntax unified
	.thumb
	.global Func_080e87c4
	.thumb_func
Func_080e87c4:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r1, [r3, #92]
	ldr r6, [r3, #108]
	ldr r3, .L_080e8974
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	sub sp, #8
	mov r10, r1
	bl Object_GetById
	movs r3, #192
	lsls r3, r3, #4
	adds r3, #172
	adds r2, r6, r3
	movs r3, #0
	ldrsb r3, [r2, r3]
	mov r9, r0
	cmp r3, #0
	bne .L_080e8800
	b .L_080e89d6
.L_080e8800:
	movs r5, #0
	movs r1, #192
	strb r5, [r2]
	lsls r1, r1, #4
	movs r2, #192
	adds r1, #173
	lsls r2, r2, #4
	adds r3, r6, r1
	adds r2, #174
	strb r5, [r3]
	adds r3, r6, r2
	strh r5, [r3]
	ldr r0, .L_080e8978
	bl Scheduler_RemoveCallback
	movs r0, #1
	bl WaitFrames
	movs r0, #167
	bl Audio_PlayCue
	ldr r3, .L_080e897c
	movs r1, #160
	lsls r1, r1, #3
	ldr r2, .L_080e8980
	ldr r0, .L_080e8984
	mov lr, r3
	.2byte 0xf800
	bl Func_08014bac
	movs r2, #128
	movs r3, #128
	lsls r3, r3, #19
	lsls r2, r2, #3
	adds r3, #8
	strh r2, [r3]
	ldr r3, .L_080e8988
	ldr r2, .L_080e898c
	strh r5, [r3]
	strh r5, [r3, #2]
	movs r3, #1
	strb r3, [r2]
	movs r0, #1
	bl WaitFrames
	ldr r0, .L_080e8990
	bl Resource_GetTableEntry
	mov r1, r10
	bl Func_0801587c
	bl Resource_FindFreeEntry
	movs r1, #128
	lsls r1, r1, #1
	mov r2, r10
	str r0, [sp, #4]
	bl VramBlock_LoadCached
	movs r6, #208
	movs r7, #128
	lsls r6, r6, #5
	lsls r7, r7, #5
	mov r11, r0
	mov r8, r5
	add r6, r10
	add r7, r10
.L_080e8886:
	bl Random16
	mov r1, r9
	ldr r3, [r1, #8]
	movs r2, #128
	str r3, [r6]
	lsls r2, r2, #13
	ldr r3, [r1, #12]
	adds r5, r0, #0
	adds r3, r3, r2
	str r3, [r6, #4]
	ldr r3, [r1, #16]
	str r3, [r6, #8]
	bl Random16
	movs r3, #208
	lsls r3, r3, #14
	lsls r0, r0, #2
	adds r0, r0, r3
	adds r1, r5, #0
	adds r2, r6, #0
	bl Vector_AddPolarOffset
	ldr r3, .L_080e8994
	movs r2, #0
	str r2, [r6, #12]
	str r3, [r6, #16]
	str r2, [r6, #20]
	bl Random16
	adds r3, r0, #0
	lsls r0, r3, #1
	movs r1, #128
	adds r0, r0, r3
	lsls r1, r1, #10
	adds r2, r6, #0
	adds r0, r0, r1
	adds r2, #12
	adds r1, r5, #0
	bl Vector_AddPolarOffset
	mov r2, r8
	negs r3, r2
	cmp r3, #0
	bge .L_080e88e2
	adds r3, #3
.L_080e88e2:
	asrs r3, r3, #2
	str r3, [r6, #24]
	mov r3, r11
	str r3, [sp, #0]
	adds r0, r7, #0
	movs r1, #4
	movs r2, #4
	movs r3, #0
	bl Func_080eaf98
	ldrb r3, [r7, #5]
	movs r2, #32
	orrs r3, r2
	ldrb r2, [r7, #9]
	movs r1, #13
	strb r3, [r7, #5]
	negs r1, r1
	movs r3, #15
	ands r3, r2
	adds r2, r1, #0
	ands r3, r2
	movs r2, #1
	strb r3, [r7, #9]
	add r8, r2
	movs r3, #240
	strh r3, [r7, #30]
	mov r3, r8
	adds r7, #40
	adds r6, #28
	cmp r3, #63
	ble .L_080e8886
	movs r0, #1
	bl WaitFrames
	movs r7, #0
.L_080e8928:
	movs r6, #208
	movs r5, #128
	lsls r6, r6, #5
	lsls r5, r5, #5
	movs r1, #63
	add r6, r10
	add r5, r10
	mov r8, r1
.L_080e8938:
	ldr r1, [r6, #24]
	cmp r1, #0
	blt .L_080e8998
	movs r3, #3
	ands r1, r3
	ldr r3, .L_080e896c
	lsls r1, r1, #1
	add r1, r11
	ands r1, r3
	ldr r2, .L_080e8970
	ldrh r3, [r5, #8]
	adds r0, r5, #0
	ands r3, r2
	orrs r3, r1
	strh r3, [r5, #8]
	adds r1, r6, #0
	bl Func_080eb298
	movs r2, #128
	movs r1, #62
	adds r0, r6, #0
	lsls r2, r2, #8
	bl BattleFx_IntegrateVector3
	ldr r1, [r6, #24]
	b .L_080e8998
.L_080e896c:
	.4byte 0x000003ff
.L_080e8970:
	.4byte 0xfffffc00
.L_080e8974:
	.4byte gPartyState
.L_080e8978:
	.4byte Func_080e807c
.L_080e897c:
	.4byte IwramFillWords
.L_080e8980:
	.4byte 0xf000f000
.L_080e8984:
	.4byte 0x06002000
.L_080e8988:
	.4byte Data_03001120
.L_080e898c:
	.4byte Data_0300123c
.L_080e8990:
	.4byte 0x000001ea
.L_080e8994:
	.4byte 0xfffc0000
.L_080e8998:
	movs r2, #1
	negs r2, r2
	adds r3, r1, #1
	add r8, r2
	str r3, [r6, #24]
	mov r3, r8
	adds r5, #40
	adds r6, #28
	cmp r3, #0
	bge .L_080e8938
	movs r0, #1
	adds r7, #1
	bl WaitFrames
	cmp r7, #39
	ble .L_080e8928
	ldr r0, [sp, #4]
	bl Resource_ResetEntry
	movs r3, #166
	lsls r3, r3, #6
	add r3, r10
	movs r1, #0
	ldrsh r0, [r3, r1]
	bl Resource_ResetEntry
	bl Func_080eb930
	movs r0, #92
	bl Runtime_ReleaseHeapBlock
.L_080e89d6:
	add sp, #8
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7, pc}
