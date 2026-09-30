.syntax unified
	.thumb
	.global BattleFx_InitializeSlots
	.thumb_func
BattleFx_InitializeSlots:
	push {lr}
	movs r1, #224
	lsls r1, r1, #3
	adds r1, #244
	movs r0, #224
	sub sp, #4
	bl Runtime_AllocateHeapBlock
	movs r3, #0
	adds r1, r0, #0
	mov r0, sp
	str r3, [r0]
	movs r3, #128
	lsls r3, r3, #19
	adds r3, #212
	ldr r2, .L_080d7aac
	stmia r3!, {r0, r1, r2}
	subs r3, #12
	movs r1, #144
	lsls r1, r1, #3
	ldr r0, .L_080d7ab0
	bl Scheduler_AddOrUpdateCallback
	add sp, #4
	pop {pc}
	.2byte 0x0000
.L_080d7aac:
	.4byte 0x850001fd
.L_080d7ab0:
	.4byte Func_080d7a58
