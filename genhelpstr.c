//usr/bin/clang "$0" && exec ./a.out "$@"

/*
  This is intended as an enhancement for prvHelpCommand() to allow for more compact help constant message with consistent tabbing.
  /FreeRTOS-Plus-CLI/FreeRTOS_CLI.c

  e.g. When running this script, you will get this behaviour

  ```
  > ./genhelpstr.c
  write :          test msg 1
  write  :         test msg 2
  write   :        test msg 3
  write    :         test msg 4
  write     :        test msg 5
  write      :       test msg 6
  write       :      test msg 7
  write        :       test msg 8
  write         :      test msg 9
  write          :     test msg 10
  write           :    test msg 11
  write            :     test msg 12
  write             :    test msg 13
  write              :   test msg 14
  write               :  test msg 15
  write                :   test msg 16
  write                 :  test msg 17
  write                  : test msg 18
  write                   : test msg 19
  write                    : test msg 20
  write                     :  test msg 21
  write                      :   test msg 22
  write                       :  test msg 23
  write                        :   test msg 24
  write                         :  test msg 25
  write                          :   test msg 26
  write                           :  test msg 27
  ```

*/


#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

static char* gen_help_str(
    char *buffer, int buffer_len,
    const char* command_str_ptr, const char* help_string_ptr,
    const uint32_t margin_tab_count,
    const uint32_t spaces_per_tab
  )
{
  const uint32_t margin_char_count = margin_tab_count*spaces_per_tab - spaces_per_tab/2;
  size_t n = buffer_len - 1; // Buffer limit counter
  char *s = buffer; // Outbuff char pointer
  const char *hs_ptr = help_string_ptr;
  uint32_t char_per_line = 0;
  bool command_printed_on_this_line = false;
  while (n > 0 && *hs_ptr != '\0')
  {
    char c = *hs_ptr;

    /* Scan for '...' and replace with command name */
    if (!command_printed_on_this_line)
    {
      const char* d_ptr = hs_ptr;

      if (  ('.' ==  *d_ptr++)
          &&('.' ==  *d_ptr++)
          &&('.' ==  *d_ptr++))
      {
        /* Name */
        const char *cn_ptr = command_str_ptr;
        while (n > 0 && *cn_ptr != '\0')
        {
          *s++ = *cn_ptr++;
          --n;
          char_per_line++;
        }
        hs_ptr = d_ptr;
        command_printed_on_this_line = true;
      }
    }

    if (':' == c)
    { /* Tab linear */
      int tab_n = 1;
      *s++ = *hs_ptr++;
      --n;

      // Tab Calc
      if (margin_char_count > char_per_line)
      {
        tab_n = (margin_char_count - char_per_line)/spaces_per_tab;
      }

      // Insert number of tabs required to reach the margin
      while ((n > 0) && (tab_n > 0))
      {
        *s++ = '\t';
        --n;
        --tab_n;
      }
    }
    else if ('\n' == c)
    { /* Newline */
      *s++ = *hs_ptr++;
      --n;
      char_per_line = 0;
      command_printed_on_this_line = false;
    }
    else
    { /* Copy Normally */
      *s++ = *hs_ptr++;
      --n;
      char_per_line++;
    }
  }

  if (n > 0)
  { /* Newline */
    *s++ = 0;
    --n;
  }

  /* Capping Just In Case */
  buffer[buffer_len - 1] = 0;

  return buffer;
}

int main(void) {
  char buff[1000] = {0};

  gen_help_str(
    buff,
    sizeof(buff),
    "write",
    "... : test msg 1\r\n"
    "...  : test msg 2\r\n"
    "...   : test msg 3\r\n"
    "...    : test msg 4\r\n"
    "...     : test msg 5\r\n"
    "...      : test msg 6\r\n"
    "...       : test msg 7\r\n"
    "...        : test msg 8\r\n"
    "...         : test msg 9\r\n"
    "...          : test msg 10\r\n"
    "...           : test msg 11\r\n"
    "...            : test msg 12\r\n"
    "...             : test msg 13\r\n"
    "...              : test msg 14\r\n"
    "...               : test msg 15\r\n"
    "...                : test msg 16\r\n"
    "...                 : test msg 17\r\n"
    "...                  : test msg 18\r\n"
    "...                   : test msg 19\r\n"
    "...                    : test msg 20\r\n"
    "...                     : test msg 21\r\n"
    "...                      : test msg 22\r\n"
    "...                       : test msg 23\r\n"
    "...                        : test msg 24\r\n"
    "...                         : test msg 25\r\n"
    "...                          : test msg 26\r\n"
    "...                           : test msg 27\r\n",
    7,
    4
  );

  printf("%s\n", buff);

  return 0;
}
