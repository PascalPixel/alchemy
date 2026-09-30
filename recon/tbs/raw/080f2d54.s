.syntax unified
	.thumb
	.global Unnamed_080f2d54
	.thumb_func
Unnamed_080f2d54:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	ldr r2, .L_080f2db8
	movs r3, #1
	strb r3, [r2]
	ldr r6, .L_080f2dbc
	bl Scheduler_ResetTaskTable
	movs r0, #1
	bl Blend_SetDarkenTarget16
	bl Bg0_ClearTilemap
	movs r0, #1
	bl WaitFrames
	ldr r2, .L_080f2dc0
	ldr r3, .L_080f2db0
	strh r3, [r2]
	ldr r3, .L_080f2db4
	subs r2, #12
	strh r3, [r2]
	ldr r3, .L_080f2dc4
	movs r5, #0
	strh r5, [r3, #10]
	ldr r5, .L_080f2dc8
	adds r0, r6, #0
	mov r8, r3
	bl Resource_GetTableEntry
	adds r1, r5, #0
	bl Resource_DecodeType01
	adds r6, r5, #0
	movs r1, #160
	ldr r3, .L_080f2dcc
	adds r0, r6, #0
	lsls r1, r1, #19
	ldr r2, .L_080f2dd0
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #224
	lsls r3, r3, #1
	adds r6, r6, r3
	b .L_080f2dd4
.L_080f2db0:
	.4byte 0x00000685
.L_080f2db4:
	.4byte 0x00001440
.L_080f2db8:
	.4byte gOamCopyEnabled
.L_080f2dbc:
	.4byte 0x00000019
.L_080f2dc0:
	.4byte 0x0400000c
.L_080f2dc4:
	.4byte gBgScroll
.L_080f2dc8:
	.4byte gMapCellBuffer
.L_080f2dcc:
	.4byte 0x040000d4
.L_080f2dd0:
	.4byte 0x84000070
.L_080f2dd4:
	adds r0, r6, #0
	ldr r3, .L_080f2e44
	ldr r1, .L_080f2e48
	ldr r2, .L_080f2e4c
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #128
	lsls r3, r3, #4
	adds r6, r6, r3
	adds r0, r6, #0
	ldr r3, .L_080f2e44
	ldr r1, .L_080f2e50
	ldr r2, .L_080f2e54
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #128
	lsls r3, r3, #7
	adds r6, r6, r3
	movs r5, #0
	movs r2, #0
	mov r3, r8
.L_080f2dfe:
	adds r5, #1
	strh r2, [r3, #2]
	strh r2, [r3]
	adds r3, #4
	cmp r5, #3
	bls .L_080f2dfe
	ldr r3, .L_080f2e44
	ldr r0, .L_080f2e58
	ldr r1, .L_080f2e5c
	ldr r2, .L_080f2e60
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	bl Ui_LoadWindowGraphics
	bl Bg0_ClearTilemap
	movs r0, #1
	bl Blend_SetBrightenTarget0
	bl Blend_WaitForTransition
	ldr r3, .L_080f2e40
	movs r2, #128
	lsls r2, r2, #19
	strh r3, [r2]
	ldr r3, .L_080f2e64
	ldr r0, [r3]
	movs r3, #3
	lsrs r0, r0, #3
	ands r0, r3
	lsls r0, r0, #10
	movs r5, #0
	b .L_080f2e80
.L_080f2e40:
	.4byte 0x00001540
.L_080f2e44:
	.4byte 0x040000d4
.L_080f2e48:
	.4byte 0x06003000
.L_080f2e4c:
	.4byte 0x84000200
.L_080f2e50:
	.4byte 0x06004000
.L_080f2e54:
	.4byte 0x84001000
.L_080f2e58:
	.4byte gBgScroll
.L_080f2e5c:
	.4byte 0x04000010
.L_080f2e60:
	.4byte 0x84000004
.L_080f2e64:
	.4byte gFrameCount
.L_080f2e68:
	movs r0, #1
	adds r5, #1
	bl WaitFrames
	cmp r5, #119
	bhi .L_080f2e98
	ldr r3, .L_080f2ea4
	ldr r0, [r3]
	movs r3, #3
	lsrs r0, r0, #3
	ands r0, r3
	lsls r0, r0, #10
.L_080f2e80:
	ldr r3, .L_080f2ea8
	adds r0, r0, r6
	ldr r1, .L_080f2eac
	ldr r2, .L_080f2eb0
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, .L_080f2eb4
	ldr r3, [r3]
	movs r2, #9
	ands r3, r2
	cmp r3, #0
	beq .L_080f2e68
.L_080f2e98:
	movs r0, #0
	pop {r3}
	mov r8, r3
	pop {r5, r6}
	pop {r1}
	bx r1
.L_080f2ea4:
	.4byte gFrameCount
.L_080f2ea8:
	.4byte 0x040000d4
.L_080f2eac:
	.4byte 0x06004100
.L_080f2eb0:
	.4byte 0x840000d0
.L_080f2eb4:
	.4byte gKeyState
