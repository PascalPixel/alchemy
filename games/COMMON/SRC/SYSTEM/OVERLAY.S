/* Overlay entry/import veneer; the calling convention permits r4 clobber.
   The linker supplies the target's address and Thumb bit. Keep the literal
   word aligned when preceding code or editable data changes size. */
.macro overlay_veneer target
    ldr r4, .Loverlay_target\@
    bx r4
    .balign 4
.Loverlay_target\@:
    .4byte \target
.endm
