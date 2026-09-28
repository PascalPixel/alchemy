/* Fixed overlay entry/import veneer; the calling convention permits r4 clobber.
   The target is the name of the code it reaches: the linker supplies its
   address, with the Thumb bit when that code is Thumb. */
.macro overlay_veneer target
    ldr r4, [pc, #0]
    bx r4
    .4byte \target
.endm
