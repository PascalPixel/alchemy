.syntax unified
	.thumb
	.global Func_080add74
	.thumb_func
Func_080add74:
	push {r5, lr}
	ldr r0, .L_080adde0
	bl Resource_GetTableEntry
	ldrb r2, [r0]
	ldr r1, .L_080adde4
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r3, r3, #1
	adds r0, #1
	adds r5, r3, r1
	ldrb r3, [r0]
	adds r0, #1
	ldrb r2, [r0]
	adds r3, r5, r3
	adds r5, r3, #0
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r3, r3, #1
	adds r0, #1
	adds r4, r3, r1
	ldrb r3, [r0]
	adds r0, #1
	ldrb r2, [r0]
	adds r3, r4, r3
	adds r4, r3, #0
	lsls r3, r2, #2
	adds r3, r3, r2
	lsls r3, r3, #1
	adds r2, r3, r1
	ldrb r3, [r0, #1]
	subs r5, #48
	adds r3, r2, r3
	adds r2, r3, #0
	subs r4, #48
	lsls r3, r5, #4
	adds r3, r3, r4
	subs r2, #48
	lsls r3, r3, #6
	adds r3, r3, r2
	movs r2, #128
	lsls r3, r3, #16
	lsls r2, r2, #21
	orrs r2, r3
	ldr r3, .L_080adde8
	asrs r0, r2, #16
	ldrb r3, [r3]
	cmp r3, #0
	beq .L_080addda
	ldr r3, .L_080addec
	orrs r0, r3
.L_080addda:
	lsls r0, r0, #16
	lsrs r0, r0, #16
	pop {r5, pc}
.L_080adde0:
	.4byte 0x00000002
.L_080adde4:
	.4byte 0xfffffe20
.L_080adde8:
	.4byte gDebugMode
.L_080addec:
	.4byte 0xffff8000
