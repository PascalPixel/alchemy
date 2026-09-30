.syntax unified
	.thumb
	.global Func_08045564
	.thumb_func
Func_08045564:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	mov r8, r0
	ldr r5, .L_080455a8
	adds r0, r5, #0
	bl Runtime_BumpAllocate
	movs r2, #132
	movs r3, #128
	adds r6, r0, #0
	lsrs r5, r5, #2
	lsls r2, r2, #24
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_080455ac
	adds r1, r6, #0
	orrs r2, r5
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #192
	lsls r3, r3, #3
	adds r3, #4
	add r3, r8
	ldr r0, [r3]
	mov r1, r8
	mov lr, r6
	.2byte 0xf800
	adds r0, r6, #0
	bl Sys_Free
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_080455a8:
	.4byte 0x0000027c
.L_080455ac:
	.4byte Tile_Decompress4bppCode
