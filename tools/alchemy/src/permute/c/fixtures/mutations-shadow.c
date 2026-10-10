int n;
int called(int a);
int target(int n)
{
    int x;
    {
        short n;
        n = 1;
        x = called(n);
    }
    return n + x;
}
