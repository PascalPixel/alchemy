/* mnWrapIndex, the same in both games. Both games' menu/menu_djinn.c include
   it where the function stands. */

/* Wraps a cursor index that has stepped one past either end of modulus entries:
   -1 becomes modulus - 1 and modulus becomes 0. */
Sint32 mnWrapIndex(Sint32 value, Sint32 modulus)
{
	return (value + modulus) % modulus;
}
