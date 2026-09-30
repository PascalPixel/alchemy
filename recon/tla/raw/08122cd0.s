.syntax unified
	.thumb
	.global Func_08122cd0
	.thumb_func
Func_08122cd0:
	push {lr}
	mov r12, r3
	mov r3, r9
	push {r3}
	mov r3, r12
	sub sp, #4
	mov r3, r9
	str r3, [sp, #0]
	adds r2, r0, #0
	ldrb r3, [r2, #16]
	ldr r1, .L_08122d04
	lsls r3, r3, #2
	adds r3, r3, r1
	ldrh r0, [r3, #2]
	ldr r3, .L_08122d08
	ldrb r1, [r2, #20]
	adds r0, r0, r3
	ldrb r3, [r2, #21]
	muls r1, r3
	ldr r3, .L_08122d0c
	mov lr, r3
	.2byte 0xf800
	add sp, #4
	pop {r3}
	mov r9, r3
	pop {pc}
.L_08122d04:
	.4byte ResourceTableEntries
.L_08122d08:
	.4byte 0x06010000
.L_08122d0c:
	.4byte IwramClearWords
