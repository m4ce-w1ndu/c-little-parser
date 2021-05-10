#include <vector.h>
#include <Parser.h>
#include <stdio.h>

int main()
{
    char str[2048];
    fgets(str, 2048, stdin);
    vector tokens = read(str);
    printf("%lf\n", calculate(tokens));
    vector_delete(tokens);
}