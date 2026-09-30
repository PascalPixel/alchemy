.syntax unified
	.thumb
	.global Func_081c0550
	.thumb_func
Func_081c0550:
	push {lr}
	lsls r0, r0, #16
	ldr r2, .L_081c0574
	ldr r1, .L_081c0578
	lsrs r0, r0, #13
	adds r0, r0, r1
	ldrh r3, [r0, #4]
	lsls r1, r3, #1
	adds r1, r1, r3
	lsls r1, r1, #2
	adds r1, r1, r2
	ldr r2, [r1]
	ldr r1, [r0]
	adds r0, r2, #0
	bl Func_081c0c84
	pop {r0}
	bx r0
.L_081c0574:
	.4byte 0x00000000
.L_081c0578:
	.4byte 0x00000000
