.syntax unified
	.thumb
	.global Func_080b0514
	.thumb_func
Func_080b0514:
	push {lr}
	subs r0, #8
	cmp r0, #77
	bls .L_080b051e
	b .L_080b069c
.L_080b051e:
	ldr r2, .L_080b06a4
	lsls r3, r0, #2
	ldr r3, [r3, r2]
	mov pc, r3
	.2byte 0x0000
.L_080b0528:
	.4byte .L_080b0690
	.4byte .L_080b0690
	.4byte .L_080b069c
	.4byte .L_080b069c
	.4byte .L_080b0660
	.4byte .L_080b0660
	.4byte .L_080b069c
	.4byte .L_080b069c
	.4byte .L_080b0664
	.4byte .L_080b0664
	.4byte .L_080b0670
	.4byte .L_080b0670
	.4byte .L_080b067c
	.4byte .L_080b0680
	.4byte .L_080b0668
	.4byte .L_080b0698
	.4byte .L_080b066c
	.4byte .L_080b0670
	.4byte .L_080b0674
	.4byte .L_080b0678
	.4byte .L_080b0690
	.4byte .L_080b069c
	.4byte .L_080b069c
	.4byte .L_080b067c
	.4byte .L_080b0690
	.4byte .L_080b069c
	.4byte .L_080b0680
	.4byte .L_080b0694
	.4byte .L_080b069c
	.4byte .L_080b069c
	.4byte .L_080b069c
	.4byte .L_080b069c
	.4byte .L_080b069c
	.4byte .L_080b069c
	.4byte .L_080b069c
	.4byte .L_080b069c
	.4byte .L_080b069c
	.4byte .L_080b069c
	.4byte .L_080b069c
	.4byte .L_080b069c
	.4byte .L_080b069c
	.4byte .L_080b069c
	.4byte .L_080b069c
	.4byte .L_080b069c
	.4byte .L_080b069c
	.4byte .L_080b069c
	.4byte .L_080b069c
	.4byte .L_080b069c
	.4byte .L_080b0684
	.4byte .L_080b0688
	.4byte .L_080b069c
	.4byte .L_080b069c
	.4byte .L_080b069c
	.4byte .L_080b069c
	.4byte .L_080b069c
	.4byte .L_080b069c
	.4byte .L_080b069c
	.4byte .L_080b069c
	.4byte .L_080b069c
	.4byte .L_080b069c
	.4byte .L_080b069c
	.4byte .L_080b069c
	.4byte .L_080b069c
	.4byte .L_080b069c
	.4byte .L_080b069c
	.4byte .L_080b068c
	.4byte .L_080b069c
	.4byte .L_080b069c
	.4byte .L_080b069c
	.4byte .L_080b069c
	.4byte .L_080b069c
	.4byte .L_080b069c
	.4byte .L_080b0690
	.4byte .L_080b069c
	.4byte .L_080b0698
	.4byte .L_080b0694
	.4byte .L_080b069c
	.4byte .L_080b0698
.L_080b0660:
	movs r0, #70
	b .L_080b06a0
.L_080b0664:
	movs r0, #75
	b .L_080b06a0
.L_080b0668:
	movs r0, #30
	b .L_080b06a0
.L_080b066c:
	movs r0, #45
	b .L_080b06a0
.L_080b0670:
	movs r0, #55
	b .L_080b06a0
.L_080b0674:
	movs r0, #25
	b .L_080b06a0
.L_080b0678:
	movs r0, #20
	b .L_080b06a0
.L_080b067c:
	movs r0, #65
	b .L_080b06a0
.L_080b0680:
	movs r0, #35
	b .L_080b06a0
.L_080b0684:
	movs r0, #60
	b .L_080b069e
.L_080b0688:
	movs r0, #90
	b .L_080b069e
.L_080b068c:
	movs r0, #70
	b .L_080b069e
.L_080b0690:
	movs r0, #60
	b .L_080b06a0
.L_080b0694:
	movs r0, #50
	b .L_080b06a0
.L_080b0698:
	movs r0, #40
	b .L_080b06a0
.L_080b069c:
	movs r0, #100
.L_080b069e:
	negs r0, r0
.L_080b06a0:
	pop {pc}
	.2byte 0x0000
.L_080b06a4:
	.4byte .L_080b0528
