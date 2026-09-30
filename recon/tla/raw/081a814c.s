.syntax unified
	.thumb
	.global Func_081a814c
	.thumb_func
Func_081a814c:
	push {lr}
	movs r1, #192
	lsls r1, r1, #6
	adds r1, #4
	movs r0, #128
	sub sp, #4
	bl Runtime_AllocateBlock
	movs r3, #0
	adds r4, r0, #0
	mov r0, sp
	str r3, [r0]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	adds r1, r4, #0
	ldr r2, .L_081a81b4
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #132
	movs r0, #160
	lsls r2, r2, #24
	lsls r0, r0, #19
	adds r2, #128
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r2, #128
	lsls r2, r2, #2
	adds r1, r4, r2
	movs r2, #132
	lsls r2, r2, #24
	ldr r0, .L_081a81b8
	adds r2, #128
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r3, #128
	lsls r3, r3, #5
	movs r0, #128
	adds r2, r4, r3
	adds r1, r4, #0
	movs r3, #0
	lsls r0, r0, #9
	bl Func_081a7a28
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_081a81bc
	bl Scheduler_AddOrUpdateCallback
	add sp, #4
	pop {pc}
	.2byte 0x0000
.L_081a81b4:
	.4byte 0x85000c01
.L_081a81b8:
	.4byte 0x05000200
.L_081a81bc:
	.4byte Func_081a78c0
