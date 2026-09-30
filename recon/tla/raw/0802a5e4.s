.syntax unified
	.thumb
	.global Func_0802a5e4
	.thumb_func
Func_0802a5e4:
	push {r5, r6, lr}
	mov r6, r8
	push {r6}
	movs r5, #128
	lsls r5, r5, #8
	adds r0, r5, #0
	bl Runtime_BumpAllocateAlternatePool
	ldr r3, .L_0802a63c
	ldr r1, .L_0802a640
	adds r2, r5, #0
	mov r8, r0
	mov lr, r3
	.2byte 0xf800
	ldr r5, .L_0802a644
	adds r0, r5, #0
	bl Runtime_BumpAllocate
	movs r2, #132
	movs r3, #128
	adds r6, r0, #0
	lsrs r5, r5, #2
	lsls r2, r2, #24
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_0802a648
	adds r1, r6, #0
	orrs r2, r5
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r0, .L_0802a64c
	ldr r1, .L_0802a640
	mov r2, r8
	mov lr, r6
	.2byte 0xf800
	adds r0, r6, #0
	bl Sys_Free
	mov r0, r8
	bl Sys_Free
	pop {r3}
	mov r8, r3
	pop {r5, r6, pc}
.L_0802a63c:
	.4byte IwramCopyWords
.L_0802a640:
	.4byte gMapCellBuffer
.L_0802a644:
	.4byte 0x000000a0
.L_0802a648:
	.4byte Render_ConvertMapCode
.L_0802a64c:
	.4byte Data_02018000
