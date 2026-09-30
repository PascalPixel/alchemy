.syntax unified
	.thumb
	.global Func_081280a0
	.thumb_func
Func_081280a0:
	push {lr}
	ldr r3, .L_081280b8
	lsls r0, r0, #3
	adds r0, r0, r3
	ldrb r3, [r0, #3]
	lsrs r0, r3, #5
	cmp r0, #4
	ble .L_081280b4
	movs r0, #1
	negs r0, r0
.L_081280b4:
	pop {pc}
	.2byte 0x0000
.L_081280b8:
	.4byte Summon_EntryTable
