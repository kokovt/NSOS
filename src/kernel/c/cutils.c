#include <cutils.h>

void itoa(int n, char *s) {
  int i, sign;

  sign = n;
  if (sign < 0)
    n = -n;

  i = 0;
  do {                       // Use do-while to catch all digits
    s[i++] = (n % 10) + '0'; // Get the current digit
  } while ((n /= 10) > 0); // Then move to the next digit

  if (sign < 0)
    s[i++] = '-';

  s[i] = '\0';

  for (int j = 0, k = i - 1; j < k; j++, k--) {
    char temp = s[j];
    s[j] = s[k];
    s[k] = temp;
  }
}

char *htoa(uint32_t n) {}
