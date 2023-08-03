#include "main.h"

/**
 * _puts_recrution - function like puts();
 * @s: input
 * Return: Always 0 (Sucess)
 */
void _puts_recursion(char *s);
{
	if (*s)
	{
		_putchar(*s);
		_putchar_recursion(s + 1);
	}
	else
		_putchar('\n');
}
