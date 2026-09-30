/* resource_3a7: its entry veneers, its listing with maintained code and
   the compiler library linked in place, as pret links objects and libgcc
   members in ROM order. Its import veneers follow its code, then the
   soft-float members its double arithmetic pulls in; their NaN constant
   sits in the rodata where the ROM holds it. */
SECTIONS
{
    .text 0x2008000 :
    {
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_DOU/ENTRY.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_DOU/DOU.o"(.text)
        "*/games/THE BROKEN SEAL/SRC/FIELD/KUUPUAPPU_DOU/IMPORT.o"(.text)
        *libgcc.a:_addsub_df.o(.text)
        *libgcc.a:_si_to_df.o(.text)
        *libgcc.a:_df_to_si.o(.text)
        *libgcc.a:_lshrdi3.o(.text)
        *libgcc.a:_pack_df.o(.text)
        *libgcc.a:_unpack_df.o(.text)
        "*/resource_3a7_overlay.o"(.rodata)
        *libgcc.a:_thenan_df.o(.rodata)
        "*/resource_3a7_overlay.o"(.rodata.part2)
    }
    /DISCARD/ : { *(.ARM.attributes) *(.comment) *(.note*) }
}
