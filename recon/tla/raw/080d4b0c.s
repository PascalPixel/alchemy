.syntax unified
	.thumb
	.global Object_SetActionCallback
	.thumb_func
Object_SetActionCallback:
	push {r5, lr}
	subs r3, r1, #1
	adds r5, r0, #0
	cmp r3, #10
	bhi .L_080d4b86
	ldr r2, .L_080d4b90
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_080d4b20:
	.4byte .L_080d4b4c
	.4byte .L_080d4b50
	.4byte .L_080d4b54
	.4byte .L_080d4b58
	.4byte .L_080d4b5c
	.4byte .L_080d4b60
	.4byte .L_080d4b74
	.4byte .L_080d4b78
	.4byte .L_080d4b7c
	.4byte .L_080d4b80
	.4byte .L_080d4b84
.L_080d4b4c:
	ldr r1, .L_080d4b94
	b .L_080d4b86
.L_080d4b50:
	ldr r1, .L_080d4b98
	b .L_080d4b86
.L_080d4b54:
	ldr r1, .L_080d4b9c
	b .L_080d4b86
.L_080d4b58:
	ldr r1, .L_080d4ba0
	b .L_080d4b86
.L_080d4b5c:
	ldr r1, .L_080d4ba4
	b .L_080d4b86
.L_080d4b60:
	ldr r3, .L_080d4ba8
	movs r2, #133
	lsls r2, r2, #2
	adds r3, r3, r2
	ldr r0, [r3]
	bl Object_GetById
	ldr r1, .L_080d4bac
	str r0, [r5, #104]
	b .L_080d4b86
.L_080d4b74:
	ldr r1, .L_080d4bb0
	b .L_080d4b86
.L_080d4b78:
	ldr r1, .L_080d4bb4
	b .L_080d4b86
.L_080d4b7c:
	ldr r1, .L_080d4bb8
	b .L_080d4b86
.L_080d4b80:
	ldr r1, .L_080d4bbc
	b .L_080d4b86
.L_080d4b84:
	ldr r1, .L_080d4bc0
.L_080d4b86:
	adds r0, r5, #0
	bl ObjectDispatch_InitializeFar
	pop {r5, pc}
	.2byte 0x0000
.L_080d4b90:
	.4byte .L_080d4b20
.L_080d4b94:
	.4byte Data_080f34e4
.L_080d4b98:
	.4byte Data_080f3428
.L_080d4b9c:
	.4byte Data_080f350c
.L_080d4ba0:
	.4byte Data_080f35c8
.L_080d4ba4:
	.4byte Data_080f373c
.L_080d4ba8:
	.4byte gPartyState
.L_080d4bac:
	.4byte Data_080f3750
.L_080d4bb0:
	.4byte Data_080f34e8
.L_080d4bb4:
	.4byte Data_080f3614
.L_080d4bb8:
	.4byte Data_080f3764
.L_080d4bbc:
	.4byte Data_080f3500
.L_080d4bc0:
	.4byte Data_080f34f4
