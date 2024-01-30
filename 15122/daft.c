main()
{
    int *a = xcalloc(1, sizeof(int));
    free(a + 1);
}