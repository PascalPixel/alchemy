.syntax unified
	.thumb
	.global Func_0802b7e0
	.thumb_func
Func_0802b7e0:
	push {r5, lr}
	lsls r3, r0, #1
	ldr r2, .L_0802b820
	adds r3, r3, r0
	lsls r3, r3, #2
	ldrh r0, [r3, r2]
	ldr r3, .L_0802b824
	adds r5, r1, #0
	adds r0, r0, r3
	bl Resource_GetTableEntry
	ldrb r3, [r0]
	str r3, [r5]
	ldrb r3, [r0, #2]
	cmp r3, #0
	bne .L_0802b806
	movs r3, #128
	lsls r3, r3, #1
	b .L_0802b808
.L_0802b806:
	ldrb r3, [r0, #2]
.L_0802b808:
	str r3, [r5, #4]
	ldrb r3, [r0, #1]
	str r3, [r5, #8]
	ldrb r3, [r0, #3]
	cmp r3, #0
	bne .L_0802b81a
	movs r3, #128
	lsls r3, r3, #1
	b .L_0802b81c
.L_0802b81a:
	ldrb r3, [r0, #3]
.L_0802b81c:
	str r3, [r5, #12]
	pop {r5, pc}
.L_0802b820:
	.4byte Data_0802f380
.L_0802b824:
	.4byte 0x0000026c
