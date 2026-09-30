.syntax unified
	.thumb
	.global Func_0812814c
	.thumb_func
Func_0812814c:
	push {lr}
	movs r3, #193
	lsls r3, r3, #1
	cmp r0, r3
	bls .L_0812815a
	movs r0, #0
	b .L_0812816c
.L_0812815a:
	ldr r3, .L_08128170
	lsls r2, r0, #3
	adds r2, r2, r3
	ldrb r3, [r2, #2]
	lsrs r3, r3, #5
	adds r0, r3, #0
	cmp r3, #0
	bne .L_0812816c
	movs r0, #0
.L_0812816c:
	pop {pc}
	.2byte 0x0000
.L_08128170:
	.4byte Data_08130d0c
