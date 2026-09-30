.syntax unified
	.thumb
	.global BattleBackground_Load
	.thumb_func
BattleBackground_Load:
	push {r5, r6, r7, lr}
	mov r7, r10
	mov r6, r9
	mov r5, r8
	push {r5, r6, r7}
	adds r7, r2, #0
	ldr r2, .L_080c09d0
	ldr r2, [r2]
	mov r9, r0
	adds r0, r1, #0
	mov r10, r2
	bl Resource_GetTableEntry
	ldr r3, .L_080c09d0
	subs r3, #140
	ldr r6, [r3]
	mov r8, r0
	ldr r5, .L_080c09d4
	movs r0, #49
	adds r1, r5, #0
	bl Runtime_AllocateHeapBlock
	movs r2, #132
	lsrs r5, r5, #2
	lsls r2, r2, #24
	adds r1, r0, #0
	ldr r3, .L_080c09d8
	ldr r0, .L_080c09dc
	orrs r2, r5
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r0, #128
	ldr r2, .L_080c09d0
	lsls r0, r0, #1
	ldr r3, [r2, #20]
	ldr r1, .L_080c09e0
	add r0, r8
	bl _call_via_r3
	movs r0, #49
	bl Runtime_ReleaseHeapBlock
	ldr r3, .L_080c09e4
	adds r4, r6, r3
	mov r0, r8
	ldr r3, .L_080c09d8
	adds r1, r4, #0
	ldr r2, .L_080c09e8
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	cmp r7, #0
	blt .L_080c0974
	lsls r3, r7, #4
	ldr r2, .L_080c09ec
	adds r3, r3, r7
	lsls r3, r3, #4
	adds r0, r6, r2
	adds r3, r3, r7
	movs r2, #128
	lsls r3, r3, #2
	lsls r2, r2, #9
	subs r2, r2, r3
	str r2, [r0]
	ldr r1, .L_080c09f0
	adds r0, r4, #0
	movs r3, #128
	bl Graphics_ScaleRgb555Clamped
.L_080c0974:
	ldr r3, .L_080c09d8
	ldr r0, .L_080c09f4
	ldr r1, .L_080c09f8
	ldr r2, .L_080c09fc
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r3, .L_080c0a00
	ldr r2, .L_080c0a04
	ldrh r3, [r3]
	ldr r0, .L_080c0a08
	strh r3, [r2]
	bl Graphics_BuildSequentialTileTable
	ldr r0, .L_080c0a0c
	bl BattlePresentation_BuildTilemap
	ldr r3, .L_080c0a10
	ldr r0, .L_080c0a14
	movs r1, #64
	bl _call_via_r3
	mov r2, r10
	ldr r3, [r2, #8]
	cmp r3, #0
	bne .L_080c09ae
	ldr r0, .L_080c0a18
	ldr r1, .L_080c0a1c
	bl Scheduler_AddOrUpdateCallback
.L_080c09ae:
	mov r3, r9
	mov r2, r10
	str r3, [r2, #8]
	cmp r3, #1
	bne .L_080c09be
	ldr r2, .L_080c0a20
	ldr r3, .L_080c09cc
	strh r3, [r2]
.L_080c09be:
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, r7}
	pop {r0}
	bx r0
.L_080c09cc:
	.4byte 0x00001f83
.L_080c09d0:
	.4byte gTransitionWork
.L_080c09d4:
	.4byte 0x00000230
.L_080c09d8:
	.4byte 0x040000d4
.L_080c09dc:
	.4byte BitDecoder_DecodeImage
.L_080c09e0:
	.4byte 0x06008000
.L_080c09e4:
	.4byte 0x00000544
.L_080c09e8:
	.4byte 0x84000040
.L_080c09ec:
	.4byte 0x00000644
.L_080c09f0:
	.4byte 0x050000c0
.L_080c09f4:
	.4byte 0x05000200
.L_080c09f8:
	.4byte 0x050000a0
.L_080c09fc:
	.4byte 0x80000010
.L_080c0a00:
	.4byte 0x050001e8
.L_080c0a04:
	.4byte 0x050000bc
.L_080c0a08:
	.4byte 0x06003800
.L_080c0a0c:
	.4byte 0x0600f800
.L_080c0a10:
	.4byte IwramClearWords
.L_080c0a14:
	.4byte 0x0600ffc0
.L_080c0a18:
	.4byte BattlePres_UpdateHBlankScroll
.L_080c0a1c:
	.4byte 0x000004ff
.L_080c0a20:
	.4byte 0x0400000a
