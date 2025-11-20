#include <bits/stdc++.h>
using namespace std;

int i = 0, j = 0, z = 0, c = 0;
char a[20], stk[20], act[10];

void check()
{
    strcpy(act, "REDUCE TO E -> ");

    // Check for E -> a
    for (z = 0; z < c; z++)
    {
        if (stk[z] == 'a')
        {
            printf("%s a", act);
            stk[z] = 'E';
            stk[z + 1] = '\0';
            printf("\n$%s\t%s$\t", stk, a);
        }
    }

    // Check for E -> E+E
    for (z = 0; z < c - 2; z++)
    {
        if (stk[z] == 'E' && stk[z + 1] == '+' && stk[z + 2] == 'E')
        {
            printf("%s E+E", act);
            stk[z] = 'E';
            stk[z + 1] = '\0';
            stk[z + 2] = '\0';
            printf("\n$%s\t%s$\t", stk, a);
            i = i - 2;
        }
    }

    // Check for E -> E*E
    for (z = 0; z < c - 2; z++)
    {
        if (stk[z] == 'E' && stk[z + 1] == '*' && stk[z + 2] == 'E')
        {
            printf("%s E*E", act);
            stk[z] = 'E';
            stk[z + 1] = '\0';
            stk[z + 2] = '\0';
            printf("\n$%s\t%s$\t", stk, a);
            i = i - 2;
        }
    }
}

int main()
{
    printf("GRAMMAR is -\nE -> E+E\nE -> E*E\nE -> a\n");

    strcpy(a, "a+a*a");
    c = strlen(a);

    strcpy(act, "SHIFT");
    printf("\nStack\tInput\tAction\n");
    printf("\n$\t%s$\t", a);

    for (i = 0, j = 0; j < c; i++, j++)
    {
        printf("%s", act);

        stk[i] = a[j];
        stk[i + 1] = '\0';
        a[j] = ' ';

        printf("\n$%s\t%s$\t", stk, a);
        check();
    }

    check();

    if (stk[0] == 'E' && stk[1] == '\0')
        printf("\nAccepted\n");
    else
        printf("\nRejected\n");
}
