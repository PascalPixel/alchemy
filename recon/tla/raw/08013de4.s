.syntax unified
	.thumb
	.global Func_08013de4
	.thumb_func
Func_08013de4:
	push {r5, r6, r7, lr}
	ldr r3, .L_08013e04
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08013e6e
	ldr r3, .L_08013e08
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_08013e10
	ldr r3, .L_08013e0c
	ldrh r2, [r3]
	ldr r3, .L_08013e00
	b .L_08013e16
	.2byte 0x0000
.L_08013e00:
	.4byte 0x00000080
.L_08013e04:
	.4byte Data_03001178
.L_08013e08:
	.4byte Data_030011dc
.L_08013e0c:
	.4byte Data_030011f4
.L_08013e10:
	ldr r3, .L_08013e54
	ldrh r2, [r3]
	ldr r3, .L_08013e50
.L_08013e16:
	orrs r3, r2
	movs r2, #128
	lsls r2, r2, #19
	adds r2, #80
	strh r3, [r2]
	ldr r6, .L_08013e58
	ldrb r3, [r6]
	adds r3, #255
	strb r3, [r6]
	ldr r2, .L_08013e5c
	ldr r3, .L_08013e60
	ldrb r5, [r2]
	ldrb r3, [r3]
	ldrb r2, [r2]
	ldr r7, .L_08013e64
	subs r3, r3, r2
	ldrb r2, [r6]
	ldrb r1, [r7]
	adds r0, r2, #0
	muls r0, r3
	bl __divsi3
	movs r3, #128
	lsls r3, r3, #19
	adds r5, r5, r0
	adds r3, #84
	strh r5, [r3]
	ldrb r3, [r6]
	b .L_08013e68
.L_08013e50:
	.4byte 0x000000c0
.L_08013e54:
	.4byte Data_030011f4
.L_08013e58:
	.4byte Data_0300110c
.L_08013e5c:
	.4byte Data_030011b0
.L_08013e60:
	.4byte Data_0300113c
.L_08013e64:
	.4byte Data_03001178
.L_08013e68:
	cmp r3, #0
	bne .L_08013e6e
	strb r3, [r7]
.L_08013e6e:
	pop {r5, r6, r7, pc}
