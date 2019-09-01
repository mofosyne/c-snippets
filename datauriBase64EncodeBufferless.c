//usr/bin/clang -DDEMO "$0" && exec ./a.out "$@"

/*

  Based on wikibook's implementation, but modified to write directly to uart
  via a putchar function.
  https://en.wikibooks.org/wiki/Algorithm_Implementation/Miscellaneous/Base64#C

  This make it useful for dumping data out of an embedded system.

*/


#include <stdio.h>
#include <stdint.h>
#include <string.h>

void datauriBase64EncodeBufferless(int (*putchar_fcptr)(int), const char* type_strptr, const void* data_buf, size_t dataLength)
{
  const char base64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  const uint8_t *data = (const uint8_t *)data_buf;
  size_t x = 0;
  uint32_t n = 0;
  int padCount = dataLength % 3;
  uint8_t n0, n1, n2, n3;

  size_t outcount = 0;
  size_t line = 0;

  putchar_fcptr((int)'d');
  putchar_fcptr((int)'a');
  putchar_fcptr((int)'t');
  putchar_fcptr((int)'a');
  putchar_fcptr((int)':');
  outcount += 5;

  while (*type_strptr != '\0')
  {
    putchar_fcptr((int)*type_strptr);
    type_strptr++;
    outcount++;
  }

  putchar_fcptr((int)';');
  putchar_fcptr((int)'b');
  putchar_fcptr((int)'a');
  putchar_fcptr((int)'s');
  putchar_fcptr((int)'e');
  putchar_fcptr((int)'6');
  putchar_fcptr((int)'4');
  putchar_fcptr((int)',');
  outcount += 8;

  /* increment over the length of the string, three characters at a time */
  for (x = 0; x < dataLength; x += 3)
  {
    /* these three 8-bit (ASCII) characters become one 24-bit number */
    n = ((uint32_t)data[x]) << 16; //parenthesis needed, compiler depending on flags can do the shifting before conversion to uint32_t, resulting to 0

    if((x+1) < dataLength)
       n += ((uint32_t)data[x+1]) << 8;//parenthesis needed, compiler depending on flags can do the shifting before conversion to uint32_t, resulting to 0

    if((x+2) < dataLength)
       n += data[x+2];

    /* this 24-bit number gets separated into four 6-bit numbers */
    n0 = (uint8_t)(n >> 18) & 63;
    n1 = (uint8_t)(n >> 12) & 63;
    n2 = (uint8_t)(n >> 6) & 63;
    n3 = (uint8_t)n & 63;

    /*
     * if we have one byte available, then its encoding is spread
     * out over two characters
     */

    putchar_fcptr((int)base64chars[n0]);
    putchar_fcptr((int)base64chars[n1]);
    outcount += 2;

    /*
     * if we have only two bytes available, then their encoding is
     * spread out over three chars
     */
    if((x+1) < dataLength)
    {
      putchar_fcptr((int)base64chars[n2]);
      outcount += 1;
    }

    /*
     * if we have all three bytes available, then their encoding is spread
     * out over four characters
     */
    if((x+2) < dataLength)
    {
      putchar_fcptr((int)base64chars[n3]);
      outcount += 1;
    }

    /* Breaking up the line so it's easier to copy and paste */
    int curr_line = (outcount/80);
    if( curr_line != line )
    {
      line = curr_line;
      putchar_fcptr((int)'\r');
      putchar_fcptr((int)'\n');
    }
  }

  /*
  * create and add padding that is required if we did not have a multiple of 3
  * number of characters available
  */
  if (padCount > 0)
  {
    for (; padCount < 3; padCount++)
    {
      putchar_fcptr((int)'=');
    }
  }

  putchar_fcptr((int)'\r');
  putchar_fcptr((int)'\n');
}

#ifdef DEMO
int main(void)
{
  char str[] = "test";
  datauriBase64EncodeBufferless(putchar, "text/plain;charset=utf-8;", str, strlen(str));
  strl
  return 0;
}
#endif //DEMO
