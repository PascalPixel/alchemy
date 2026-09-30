.syntax unified
	.thumb
	.global Func_08042588
	.thumb_func
Func_08042588:
	push {r5, r6, r7, lr}
	mov r7, r8
	push {r7}
	movs r3, #192
	lsls r3, r3, #18
	ldr r7, [r3, #60]
	sub sp, #4
	mov r8, r0
	cmp r7, #0
	bne .L_080425bc
	ldr r3, .L_080425b8
	mov r0, sp
	adds r0, #2
	strh r3, [r0]
	movs r2, #129
	movs r3, #128
	lsls r3, r3, #19
	lsls r2, r2, #24
	adds r3, #212
	mov r1, r8
	adds r2, #160
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	b .L_080425ec
.L_080425b8:
	.4byte 0x0000e0e0
.L_080425bc:
	ldr r5, .L_080425f4
	adds r0, r5, #0
	bl Runtime_BumpAllocate
	movs r2, #132
	movs r3, #128
	adds r6, r0, #0
	lsrs r5, r5, #2
	lsls r2, r2, #24
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_080425f8
	adds r1, r6, #0
	orrs r2, r5
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	adds r1, r7, #0
	adds r1, #8
	mov r0, r8
	mov lr, r6
	.2byte 0xf800
	adds r0, r6, #0
	bl Sys_Free
.L_080425ec:
	add sp, #4
	pop {r3}
	mov r8, r3
	pop {r5, r6, r7, pc}
.L_080425f4:
	.4byte 0x00000218
.L_080425f8:
	.4byte Tile_BuildMetatilesCode
