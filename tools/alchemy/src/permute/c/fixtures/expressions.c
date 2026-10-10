typedef int I;
struct Point { int x, y; };
char *s = "a" "bc";
unsigned long long big = 0XffffffffULL;
float f = 0x1.2p-3f;
int target(int a, int b, int *p)
{
    struct Point q = (struct Point){ .x = a, .y = b };
    const int * const *pp;
    typeof(a + b) c;
    c = a = b;
    a = ({ if (b) a++; a; });
    a = *p++ + ++b - p[a];
    b = (a << 2) & (b | ~a);
    a = sizeof a + sizeof(int *) + (int)(a / (b + 1));
    return (a, b, c) + q.x;
}
