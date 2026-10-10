int target(int a, int b)
{
    int x;
    float f = 1.0f;
    x = a + b;
#pragma _permuter randomizer start
    x = a * 0;
    if (a) { x += b; }
#pragma _permuter randomizer end
    x += a;
#pragma _permuter randomizer start
    x = b + 0;
    while (b != 0) { b--; }
#pragma _permuter randomizer end
    return x;
}
