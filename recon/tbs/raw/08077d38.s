.syntax unified
	.thumb
	.global GameState_InitDefaults
	.thumb_func
GameState_InitDefaults:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r8
	push {r6, r7}
	sub sp, #8
	add r5, sp, #4
	movs r4, #0
	str r4, [r5]
	ldr r3, .L_08077dec
	adds r0, r5, #0
	ldr r1, .L_08077df0
	ldr r2, .L_08077df4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	str r4, [r5]
	adds r0, r5, #0
	ldr r1, .L_08077df8
	ldr r2, .L_08077dfc
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #128
	ldr r2, .L_08077dec
	lsls r3, r3, #24
.L_08077d66:
	ldr r4, [r2, #8]
	ands r4, r3
	cmp r4, #0
	bne .L_08077d66
	str r4, [r5]
	ldr r3, .L_08077dec
	adds r0, r5, #0
	ldr r1, .L_08077e00
	ldr r2, .L_08077e04
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, .L_08077df8
	movs r1, #130
	lsls r1, r1, #1
	adds r3, r3, r1
	movs r2, #255
	strb r2, [r3]
	adds r0, r5, #0
	str r4, [r5]
	ldr r3, .L_08077dec
	ldr r1, .L_08077e08
	ldr r2, .L_08077e0c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	str r4, [sp, #0]
	bl Owner_InitRecords
	ldr r7, .L_08077df0
	movs r3, #132
	lsls r3, r3, #2
	adds r2, r7, r3
	movs r3, #1
	strh r3, [r2]
	ldr r1, .L_08077de4
	ldr r2, .L_08077e10
	mov r10, r1
	adds r3, r7, r2
	movs r1, #2
	strh r1, [r3]
	movs r3, #133
	lsls r3, r3, #2
	adds r2, r7, r3
	ldr r3, .L_08077de8
	mov r8, r3
	movs r3, #4
	strh r3, [r2]
	ldr r3, .L_08077e14
	adds r2, r7, r3
	movs r3, #8
	strh r3, [r2]
	movs r3, #134
	lsls r3, r3, #2
	adds r2, r7, r3
	subs r3, #24
	strh r3, [r2]
	adds r3, #26
	adds r2, r7, r3
	movs r3, #128
	lsls r3, r3, #1
	strh r3, [r2]
	movs r2, #135
	lsls r2, r2, #2
	b .L_08077e18
.L_08077de4:
	.4byte 0x00000000
.L_08077de8:
	.4byte 0x00000004
.L_08077dec:
	.4byte 0x040000d4
.L_08077df0:
	.4byte gCell
.L_08077df4:
	.4byte 0x850000b0
.L_08077df8:
	.4byte gSceneState
.L_08077dfc:
	.4byte 0x850003e1
.L_08077e00:
	.4byte GameFlagBytes
.L_08077e04:
	.4byte 0x85000080
.L_08077e08:
	.4byte gSaveStamp + 0x1c
.L_08077e0c:
	.4byte 0x85000298
.L_08077e10:
	.4byte 0x00000212
.L_08077e14:
	.4byte 0x00000216
.L_08077e18:
	adds r3, r7, r2
	strh r1, [r3]
	movs r1, #136
	ldr r4, [sp, #0]
	lsls r1, r1, #2
	adds r3, r7, r1
	adds r2, #6
	strh r4, [r3]
	subs r1, #44
	adds r3, r7, r2
	strh r4, [r3]
	adds r3, r7, r1
	str r4, [r3]
	movs r0, #0
	bl Party_AddActiveOwner
	movs r2, #131
	ldr r4, [sp, #0]
	ldr r5, .L_08077e78
	lsls r2, r2, #2
	ldr r1, .L_08077e80
	adds r3, r7, r2
	str r4, [r7, #16]
	subs r2, #1
	strb r5, [r3]
	adds r3, r7, r1
	strb r5, [r3]
	subs r1, #5
	adds r3, r7, r2
	strb r5, [r3]
	ldr r6, .L_08077e7c
	adds r3, r7, r1
	mov r2, r10
	adds r1, #1
	strb r2, [r3]
	adds r3, r7, r1
	strb r6, [r3]
	str r4, [r7]
	bl Runtime_GetBuildStampTime
	movs r2, #174
	lsls r2, r2, #2
	adds r3, r7, r2
	str r0, [r3]
	ldr r4, [sp, #0]
	ldr r3, .L_08077e84
	b .L_08077e88
	.2byte 0x0000
.L_08077e78:
	.4byte 0x00000001
.L_08077e7c:
	.4byte 0x00000008
.L_08077e80:
	.4byte 0x0000020a
.L_08077e84:
	.4byte gLoadedStateWord
.L_08077e88:
	str r4, [r3]
	ldr r3, .L_08077ee4
	mov r1, r10
	strb r1, [r3]
	ldr r1, .L_08077ee8
	ldrb r2, [r3]
	adds r3, r7, r1
	str r4, [r7, #4]
	strb r2, [r3]
	ldr r3, .L_08077eec
	ldr r2, .L_08077ef0
	strh r4, [r3]
	ldr r3, .L_08077ee0
	strh r3, [r2]
	ldr r2, .L_08077ef4
	mov r1, r8
	adds r3, r7, r2
	adds r2, #1
	strb r1, [r3]
	adds r3, r7, r2
	strb r1, [r3]
	ldr r1, .L_08077ef8
	mov r2, r8
	adds r3, r7, r1
	strb r2, [r3]
	adds r1, #1
	ldr r2, .L_08077efc
	adds r3, r7, r1
	strb r6, [r3]
	adds r1, #2
	adds r3, r7, r2
	strb r6, [r3]
	adds r2, #2
	adds r3, r7, r1
	strb r6, [r3]
	adds r1, #2
	adds r3, r7, r2
	movs r2, #16
	strb r2, [r3]
	adds r3, r7, r1
	adds r1, #1
	strb r2, [r3]
	b .L_08077f00
	.2byte 0x0000
.L_08077ee0:
	.4byte 0xffffffff
.L_08077ee4:
	.4byte gOptionMirror
.L_08077ee8:
	.4byte 0x0000022a
.L_08077eec:
	.4byte gPostLoadCounter
.L_08077ef0:
	.4byte gSaveSlot
.L_08077ef4:
	.4byte 0x0000011d
.L_08077ef8:
	.4byte 0x0000011f
.L_08077efc:
	.4byte 0x00000121
.L_08077f00:
	adds r3, r7, r1
	strb r2, [r3]
	movs r2, #147
	lsls r2, r2, #1
	adds r3, r7, r2
	adds r1, #2
	movs r2, #32
	strb r2, [r3]
	adds r3, r7, r1
	adds r1, #1
	strb r2, [r3]
	adds r3, r7, r1
	strb r2, [r3]
	ldr r3, .L_08077f3c
	adds r1, #2
	adds r2, r7, r3
	movs r3, #64
	strb r3, [r2]
	adds r2, r7, r1
	adds r1, #1
	strb r3, [r2]
	adds r2, r7, r1
	strb r3, [r2]
	add sp, #8
	pop {r3, r5}
	mov r8, r3
	mov r10, r5
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_08077f3c:
	.4byte 0x00000129
