.syntax unified
	.thumb
	.global Func_0811bd10
	.thumb_func
Func_0811bd10:
	push {r5, lr}
	movs r3, #192
	lsls r3, r3, #18
	ldr r5, [r3, #48]
	bl Func_08014de4
	movs r0, #108
	adds r0, #255
	bl GameFlag_Test
	cmp r0, #0
	beq .L_0811bd3c
	ldr r0, .L_0811bd48
	ldr r3, .L_0811bd4c
	mov lr, r3
	.2byte 0xf800
	adds r1, r5, #0
	adds r1, #12
	adds r0, r5, #0
	bl Func_080156f8
	b .L_0811bd46
.L_0811bd3c:
	adds r1, r5, #0
	adds r1, #12
	adds r0, r5, #0
	bl Func_080156e8
.L_0811bd46:
	pop {r5, pc}
.L_0811bd48:
	.4byte Data_08128868
.L_0811bd4c:
	.4byte IwramTransformMatrix
