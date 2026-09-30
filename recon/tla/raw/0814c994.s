.syntax unified
	.thumb
	.global Func_0814c994
	.thumb_func
Func_0814c994:
	push {r5, lr}
	movs r1, #192
	lsls r1, r1, #2
	adds r5, r0, #0
	adds r1, #2
	movs r0, #100
	bl Runtime_AllocateHeapBlock
	movs r1, #246
	lsls r1, r1, #7
	adds r1, #124
	movs r0, #92
	bl Runtime_AllocateHeapBlock
	movs r1, #128
	lsls r1, r1, #7
	movs r0, #96
	bl Runtime_AllocateHeapBlock
	movs r3, #192
	lsls r3, r3, #18
	ldr r3, [r3, #92]
	movs r1, #240
	lsls r1, r1, #7
	adds r1, #240
	adds r2, r3, r1
	str r5, [r2]
	movs r2, #240
	lsls r2, r2, #7
	adds r2, #228
	adds r3, r3, r2
	movs r2, #1
	str r2, [r3]
	ldr r3, [r5]
	cmp r3, #0
	beq .L_0814cab0
	subs r3, #1
	cmp r3, #28
	bls .L_0814c9e4
	b .L_0814cb46
.L_0814c9e4:
	ldr r2, .L_0814cb5c
	lsls r3, r3, #2
	ldr r3, [r3, r2]
	mov pc, r3
.L_0814c9ec:
	.4byte .L_0814ca60
	.4byte .L_0814ca68
	.4byte .L_0814ca70
	.4byte .L_0814ca78
	.4byte .L_0814ca80
	.4byte .L_0814ca88
	.4byte .L_0814ca90
	.4byte .L_0814ca98
	.4byte .L_0814caa0
	.4byte .L_0814caa8
	.4byte .L_0814cab0
	.4byte .L_0814cab8
	.4byte .L_0814cac0
	.4byte .L_0814cac8
	.4byte .L_0814cad0
	.4byte .L_0814cad8
	.4byte .L_0814cae0
	.4byte .L_0814cae8
	.4byte .L_0814caf0
	.4byte .L_0814caf8
	.4byte .L_0814cb00
	.4byte .L_0814cb08
	.4byte .L_0814cb10
	.4byte .L_0814cb18
	.4byte .L_0814cb20
	.4byte .L_0814cb28
	.4byte .L_0814cb30
	.4byte .L_0814cb38
	.4byte .L_0814cb40
.L_0814ca60:
	adds r0, r5, #0
	bl Func_08165ab4
	b .L_0814cb46
.L_0814ca68:
	adds r0, r5, #0
	bl Func_08148458
	b .L_0814cb46
.L_0814ca70:
	adds r0, r5, #0
	bl Func_08168a40
	b .L_0814cb46
.L_0814ca78:
	adds r0, r5, #0
	bl Func_08153ebc
	b .L_0814cb46
.L_0814ca80:
	adds r0, r5, #0
	bl Func_0814ce30
	b .L_0814cb46
.L_0814ca88:
	adds r0, r5, #0
	bl Func_081693d0
	b .L_0814cb46
.L_0814ca90:
	adds r0, r5, #0
	bl Func_08147ae0
	b .L_0814cb46
.L_0814ca98:
	adds r0, r5, #0
	bl Func_08146e00
	b .L_0814cb46
.L_0814caa0:
	adds r0, r5, #0
	bl Func_0816729c
	b .L_0814cb46
.L_0814caa8:
	adds r0, r5, #0
	bl Func_0814db04
	b .L_0814cb46
.L_0814cab0:
	adds r0, r5, #0
	bl Func_08164bf4
	b .L_0814cb46
.L_0814cab8:
	adds r0, r5, #0
	bl Func_08158dc8
	b .L_0814cb46
.L_0814cac0:
	adds r0, r5, #0
	bl Func_0814bee0
	b .L_0814cb46
.L_0814cac8:
	adds r0, r5, #0
	bl Func_08170364
	b .L_0814cb46
.L_0814cad0:
	adds r0, r5, #0
	bl Func_08179f18
	b .L_0814cb46
.L_0814cad8:
	adds r0, r5, #0
	bl Func_08191d60
	b .L_0814cb46
.L_0814cae0:
	adds r0, r5, #0
	bl Func_0815b764
	b .L_0814cb46
.L_0814cae8:
	adds r0, r5, #0
	bl Func_08186dfc
	b .L_0814cb46
.L_0814caf0:
	adds r0, r5, #0
	bl Func_0814153c
	b .L_0814cb46
.L_0814caf8:
	adds r0, r5, #0
	bl Func_08182aa8
	b .L_0814cb46
.L_0814cb00:
	adds r0, r5, #0
	bl Func_0817ea58
	b .L_0814cb46
.L_0814cb08:
	adds r0, r5, #0
	bl Func_0815cde8
	b .L_0814cb46
.L_0814cb10:
	adds r0, r5, #0
	bl Func_081843fc
	b .L_0814cb46
.L_0814cb18:
	adds r0, r5, #0
	bl Func_08175f74
	b .L_0814cb46
.L_0814cb20:
	adds r0, r5, #0
	bl Func_08192c9c
	b .L_0814cb46
.L_0814cb28:
	adds r0, r5, #0
	bl Func_08180c94
	b .L_0814cb46
.L_0814cb30:
	adds r0, r5, #0
	bl Func_0818cb74
	b .L_0814cb46
.L_0814cb38:
	adds r0, r5, #0
	bl Func_0813fd84
	b .L_0814cb46
.L_0814cb40:
	adds r0, r5, #0
	bl Func_0818df5c
.L_0814cb46:
	movs r0, #96
	bl Runtime_ReleaseHeapBlock
	movs r0, #92
	bl Runtime_ReleaseHeapBlock
	movs r0, #100
	bl Runtime_ReleaseHeapBlock
	pop {r5, pc}
	.2byte 0x0000
.L_0814cb5c:
	.4byte .L_0814c9ec
