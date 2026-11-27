// The number of entries in a list of palette slots ended by PALETTE_LIST_END, less one.
Sint32 syListCountEntries(Uint16 *list)
{
    Sint32 count = 0;

    while (*list != PALETTE_LIST_END)
    {
        list++;
        count++;
    }
    count--;
    return count;
}
