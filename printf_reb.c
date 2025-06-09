// My take on printf()
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>


#ifdef _WIN32
#include <windows.h>
#define my_write(buf, len) WriteConsole(GetStdHandle(STD_OUTPUT_HANDLE), buf, len, NULL, NULL)
#else
#include <unistd.h>
#define my_write(buf, len) write(1, buf, len)
#endif

int my_round(double n)
{
	n += 0.6;
	int rev_n = (int)n;
	return rev_n;
}


void addPadding(char **ptr_ptr, int n, int margin)
{
	char tmp[16];
	int i = 0;
	
	if(n == 0)
	{
		tmp[i++] = '0';
	}else
	{
		while(n)
		{
			tmp[i++] = '0' + n%10;
			n /= 10;
		}
	}
	int nums = i;
	for(int j = 0; j < margin - nums; j++)
	{
		**ptr_ptr = '0';
		(*ptr_ptr)++;
	}
	for(int j = nums - 1; j >= 0; j--)
	{
		**ptr_ptr = tmp[j];
		(*ptr_ptr)++;
	}
	
}

void my_ftos_rec_loop(char **ptr_ptr, int n)
{
	if(n == 0)
	{
		return;
	}
	my_ftos_rec_loop(ptr_ptr, n/10);
	**ptr_ptr = '0' + n%10;
	(*ptr_ptr)++;
}

void my_ftos(char** ptr_ptr, int n)
{
	
	if(n == 0)
	{
		*(*ptr_ptr)++ = '0';
		return;
	}
	
	
	my_ftos_rec_loop(ptr_ptr, n);
	**ptr_ptr = '\0';
}
/*
Funniest output was... When I was parsing numbers without recursion (And lost something in the meantime)))

"Hello, it's my printf, it has d for double, so here! -./.''-"
*/

void myprintf(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	size_t buf_size = 1024;
	char* buf = malloc(buf_size);
	char* ptr = buf;
	while(*format)
	{
		
		if(*format == '%')
		{
			switch(*(format+1))
			{
				case 'd':
					double n = va_arg(args, double);
					if(n < 0)
					{
						*ptr++ = '-';
						n = -n;
					}
					int n_int = (int) n;
					
					double n_exp = n - n_int;
					if(n_exp < 0) n_exp = -n_exp;
					my_ftos(&ptr, n_int);
					if(n == 0)
					{
						format += 2;
						break;
					}
					*ptr = '.';
					ptr++;
					double n_exp_p = n_exp * 1000;
					int n_exp_i = my_round(n_exp_p);
					addPadding(&ptr, n_exp_i, 3);
					format += 2;
					break;
				// case 'i':
					// break;
				// case 'c':
					// break;
				case 's':
					char* str = va_arg(args, char*);
					while(*str)
					{
						*ptr++ = *str++;
					}
					format += 2;
					break;
				default:
					*ptr = *format;
					ptr++;
					format++;
			}
		}
		else
		{
			*ptr = *format;
			ptr++;
			format++;
		}
	}
	*ptr = '\0';
	my_write(buf, ptr - buf);
}
int main()
{
	myprintf("Hello, it's my printf, it has d for double, so here! %s %d\n", "My C-string that is probably zero-terminated", (double) 12.3f);
	myprintf(
    "Hello, it's my printf!\n" 
    "String: %s\n"
    "Positive: %d\n"
    "Negative: %d\n"
    "Zero: %d\n",
    "My C-string that is probably zero-terminated", 
    (double)12.3f, 
    (double)-4.2, 
    (double)-0.0
);
}
