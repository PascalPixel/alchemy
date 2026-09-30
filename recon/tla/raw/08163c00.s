.syntax unified
	.thumb
	.global Func_08163c00
	.thumb_func
Func_08163c00:
	push {r5, lr}
	adds r5, r0, #0
	movs r0, #0
	sub sp, #24
	bl BattleFx_BeginCanvasLayer
	ldr r1, [r5, #4]
	movs r3, #1
	eors r1, r3
	lsls r1, r1, #4
	movs r3, #7
	orrs r1, r3
	add r2, sp, #12
	mov r3, sp
	adds r0, r5, #0
	bl Func_0815585c
	bl Func_08143bb8
	add sp, #24
	pop {r5, pc}
	.2byte 0x0000
