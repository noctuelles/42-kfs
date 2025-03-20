#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

static size_t get_baselen(const char *base)
{
	size_t i, j;

	i = 0;
	while (base[i] != '\0')
	{
		j = 0;
		if (base[i] == '-' || base[i] == '+')
			return (0);
		while (j < i)
			if (base[j++] == base[i])
				return (0);
		i++;
	}
	return (i > 1) ? i : 0;
}

static size_t get_nchar(int64_t n, size_t baselen)
{
	size_t size;
	uint64_t i;

	size = 0;
	if (n == 0)
		return (1);
	if (n < 0)
	{
		size++;
		i = (uint64_t)(-n);
	}
	else
		i = n;
	while (i != 0)
	{
		size++;
		i /= baselen;
	}
	return (size);
}

static void fillstr(char *str, const char *base, size_t baselen, uint64_t n)
{
	if (n >= baselen)
		fillstr(str - 1, base, baselen, n / baselen);
	*str = base[n % baselen];
}

char *itoa_base(int64_t n, const char *base)
{
	static char str[512];
	size_t baselen;
	size_t nchar;

	baselen = get_baselen(base);
	if (!baselen)
		return (NULL);
	nchar = get_nchar(n, baselen);
	if (nchar > sizeof(str))
		return (NULL);
	if (n < 0)
	{
		str[0] = '-';
		fillstr(&str[nchar - 1], base, baselen, (uint64_t)(-n));
	}
	else
		fillstr(&str[nchar - 1], base, baselen, (uint64_t)n);
	str[nchar] = '\0';
	return (str);
}

char *utoa_base(uint64_t n, const char *base)
{
	static char str[512];
	size_t baselen;
	size_t nchar;

	baselen = get_baselen(base);
	if (!baselen)
		return (NULL);
	nchar = get_nchar(n, baselen);
	if (nchar > sizeof(str))
		return (NULL);
	fillstr(&str[nchar - 1], base, baselen, n);
	str[nchar] = '\0';
	return (str);
}
