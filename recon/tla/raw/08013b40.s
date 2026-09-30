.syntax unified
	.thumb
	.global Func_08013b40
	.thumb_func
Func_08013b40:
	push {lr}
	ldr r3, .L_08013b5c
	ldrh r3, [r3]
	cmp r3, #0
	bne .L_08013b5a
	ldr r3, .L_08013b60
	movs r2, #195
	lsls r2, r2, #8
	adds r2, #255
	strh r2, [r3]
	ldr r2, .L_08013b64
	movs r3, #1
	strb r3, [r2]
.L_08013b5a:
	pop {pc}
.L_08013b5c:
	.4byte Data_02003000
.L_08013b60:
	.4byte 0x04000132
.L_08013b64:
	.4byte Data_030011c0
