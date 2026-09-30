.syntax unified
	.thumb
	.global Func_080f03f0
	.thumb_func
Func_080f03f0:
	push {r5, r6, r7, lr}
	mov r7, r11
	mov r6, r10
	mov r5, r9
	push {r5, r6, r7}
	mov r7, r8
	push {r7}
	ldr r3, .L_080f04f8
	movs r2, #0
	strb r2, [r3]
	ldr r3, .L_080f04fc
	strb r2, [r3]
	ldr r3, .L_080f0500
	strb r2, [r3]
	ldr r3, .L_080f0504
	strb r2, [r3]
	bl Scheduler_ResetTaskTable
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_080f0508
	bl Scheduler_AddOrUpdateCallback
	movs r3, #64
	movs r5, #128
	lsls r5, r5, #19
	strh r3, [r5]
	ldr r0, .L_080f050c
	bl DisplayScroll_BuildHblankWordTable
	ldr r0, .L_080f0510
	bl DisplayScroll_BuildHblankWordTable
	movs r0, #0
	bl Graphics_ClearCharacterBlockAndPalette
	movs r0, #1
	bl Graphics_ClearCharacterBlockAndPalette
	ldr r2, .L_080f0514
	ldr r3, .L_080f0518
	strh r2, [r3]
	ldr r2, .L_080f051c
	adds r3, #2
	strh r2, [r3]
	movs r3, #226
	lsls r3, r3, #5
	strh r3, [r5]
	ldr r2, .L_080f0520
	ldr r3, .L_080f0524
	strh r2, [r3]
	bl DisplayScroll_InitObjectTable
	movs r0, #150
	lsls r0, r0, #1
	bl WaitFrames
	movs r1, #0
	mov r8, r1
	ldr r3, .L_080f0528
	ldr r1, .L_080f052c
	movs r2, #1
	mov r11, r2
	mov r10, r3
	mov r9, r1
.L_080f0472:
	mov r3, r11
	mov r5, r8
	ands r5, r3
	mov r2, r9
	adds r1, r5, #0
	ldr r0, [r2]
	eors r1, r3
	bl Graphics_LoadCharacterBlockAndPalette
	adds r7, r5, #0
	movs r5, #240
	movs r6, #1
	lsls r5, r5, #4
.L_080f048c:
	cmp r7, #0
	beq .L_080f049e
	movs r1, #16
	subs r2, r1, r6
	lsls r3, r6, #8
	orrs r3, r2
	mov r2, r10
	strh r3, [r2]
	b .L_080f04a6
.L_080f049e:
	adds r3, r5, #0
	orrs r3, r6
	mov r1, r10
	strh r3, [r1]
.L_080f04a6:
	movs r0, #4
	bl WaitFrames
	ldr r2, .L_080f0530
	adds r6, #1
	adds r5, r5, r2
	cmp r6, #16
	ble .L_080f048c
	ldr r0, .L_080f0534
	bl WaitFrames
	movs r1, #1
	add r8, r1
	movs r3, #4
	mov r2, r8
	add r9, r3
	cmp r2, #32
	bls .L_080f0472
	ldr r3, .L_080f0524
	movs r2, #0
	strh r2, [r3]
	movs r2, #130
	lsls r2, r2, #5
	subs r3, #80
	strh r2, [r3]
	bl Ui_LoadWindowGraphics
	bl Bg0_ClearTilemap
	ldr r2, .L_080f04f8
	movs r3, #1
	strb r3, [r2]
	movs r0, #0
	pop {r3, r5, r6, r7}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	mov r11, r7
	pop {r5, r6, r7}
	pop {r1}
	bx r1
.L_080f04f8:
	.4byte gOamCopyEnabled
.L_080f04fc:
	.4byte Data_03001f58
.L_080f0500:
	.4byte Data_03001ac4
.L_080f0504:
	.4byte gOptionMirror
.L_080f0508:
	.4byte DisplayScroll_StepPositionEveryFourFrames
.L_080f050c:
	.4byte 0x06007800
.L_080f0510:
	.4byte 0x0600f800
.L_080f0514:
	.4byte 0x00001f8a
.L_080f0518:
	.4byte 0x0400000c
.L_080f051c:
	.4byte 0x00000f83
.L_080f0520:
	.4byte 0x00002844
.L_080f0524:
	.4byte 0x04000050
.L_080f0528:
	.4byte 0x04000052
.L_080f052c:
	.4byte DisplayScroll_SlideResources
.L_080f0530:
	.4byte 0xffffff00
.L_080f0534:
	.4byte 0x0000010b
