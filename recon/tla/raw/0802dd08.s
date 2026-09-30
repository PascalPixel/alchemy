.syntax unified
	.thumb
	.global Func_0802dd08
	.thumb_func
Func_0802dd08:
	push {r5, r6, lr}
	mov r6, r10
	mov r5, r9
	push {r5, r6}
	mov r6, r8
	push {r6}
	ldr r3, .L_0802dd60
	mov r10, r0
	mov r9, r1
	mov r8, r3
	ldr r5, .L_0802dd64
	adds r0, r5, #0
	bl Runtime_BumpAllocate
	movs r2, #132
	movs r3, #128
	adds r6, r0, #0
	lsrs r5, r5, #2
	lsls r2, r2, #24
	lsls r3, r3, #19
	adds r3, #212
	ldr r0, .L_0802dd68
	adds r1, r6, #0
	orrs r2, r5
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	ldr r2, .L_0802dd6c
	movs r3, #128
	lsls r3, r3, #5
	add r8, r3
	mov r0, r10
	mov r1, r9
	mov r3, r8
	mov lr, r6
	.2byte 0xf800
	adds r0, r6, #0
	bl Sys_Free
	pop {r3, r5, r6}
	mov r8, r3
	mov r9, r5
	mov r10, r6
	pop {r5, r6, pc}
	.2byte 0x0000
.L_0802dd60:
	.4byte Data_0201c000
.L_0802dd64:
	.4byte 0x00000268
.L_0802dd68:
	.4byte Render_DrawMapCode
.L_0802dd6c:
	.4byte Data_0203c000
