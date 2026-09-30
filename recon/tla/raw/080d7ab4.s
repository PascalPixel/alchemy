.syntax unified
	.thumb
	.global Func_080d7ab4
	.thumb_func
Func_080d7ab4:
	push {r5, r6, r7, lr}
	movs r3, #192
	lsls r3, r3, #18
	adds r3, #224
	ldr r0, .L_080d7af4
	ldr r5, [r3]
	bl Scheduler_RemoveCallback
	adds r6, r5, #0
	adds r6, #149
	adds r5, #80
	movs r7, #23
.L_080d7acc:
	ldrb r3, [r6]
	adds r6, #72
	lsls r3, r3, #24
	cmp r3, #0
	beq .L_080d7adc
	adds r0, r5, #0
	bl BattleFx_ClearOwnedSlot
.L_080d7adc:
	subs r7, #1
	adds r5, #72
	cmp r7, #0
	bge .L_080d7acc
	movs r0, #224
	bl Runtime_ReleaseHeapBlock
	movs r0, #1
	bl WaitFrames
	pop {r5, r6, r7, pc}
	.2byte 0x0000
.L_080d7af4:
	.4byte Func_080d7a58
