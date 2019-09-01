//usr/bin/clang -DDEMO "$0" && exec ./a.out "$@"

/*
  This is intended as an enhancement for prvHelpCommand() to allow for more compact help constant message with consistent tabbing.
  /FreeRTOS-Plus-CLI/FreeRTOS_CLI.c

  e.g. When running this script, you will get this behaviour. (Given tab width of 8 to match the VT102 terminal behaviour)

  ```
  write :                          test msg 1
  write  :                         test msg 2
  write   :                        test msg 3
  write    :                       test msg 4
  write     :                      test msg 5
  write      :                     test msg 6
  write       :                    test msg 7
  write        :                   test msg 8
  write         :                  test msg 9
  write          :                 test msg 10
  write           :                test msg 11
  write            :               test msg 12
  write             :              test msg 13
  write              :             test msg 14
  write               :            test msg 15
  write                :           test msg 16
  write                 :          test msg 17
  write                  :         test msg 18
  write                   :        test msg 19
  write                    :       test msg 20
  write                     :      test msg 21
  write                      :     test msg 22
  write                       :    test msg 23
  write                        :   test msg 24
  write                         :  test msg 25
  write                          : test msg 26
  write                           : test msg 27
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
  size_t n = buffer_len - 1; // Buffer limit counter
  char *s = buffer; // Outbuff char pointer
  const char *hs_ptr = help_string_ptr;
  uint32_t char_per_line = 0;
  bool command_printed_on_this_line = false;
  bool margin_aligned_on_this_line = false;
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

    if ((!margin_aligned_on_this_line)&&(':' == c))
    { /* Tab linear */
      margin_aligned_on_this_line = true;
      int tab_n = 0;
      *s++ = *hs_ptr++;
      --n;
      char_per_line++;

      // Tab Calc
      uint32_t req_tab_n = margin_tab_count;
      uint32_t curr_tab_n = char_per_line/spaces_per_tab;
      if (req_tab_n > curr_tab_n)
      {
        tab_n = req_tab_n-curr_tab_n;
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
      margin_aligned_on_this_line = false;
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

#ifdef DEMO
int main(void)
{
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
    "...                           : test msg 27\r\n"
    "...            : : test msg 30\r\n",
    4,
    8
  );

  printf("%s\n", buff);

  return 0;
}
#endif //DEMO

#if 0 // Use this in /FreeRTOS-Plus-CLI/FreeRTOS_CLI.c
static BaseType_t prvHelpCommand(
  char *pcWriteBuffer, size_t xWriteBufferLen, const char *pcCommandString, uint32_t *has_error )
{
static const CLI_Definition_List_Item_t * pxCommand = NULL;
BaseType_t xReturn;

  ( void ) pcCommandString;

  if( pxCommand == NULL )
  {
    /* Reset the pxCommand pointer back to the start of the list. */
    pxCommand = &xRegisteredCommands;
  }

  /* Return the next command help string, before moving the pointer on to
  the next command in the list. */
  gen_help_str(pcWriteBuffer, xWriteBufferLen, pxCommand->pxCommandLineDefinition->pcCommand, pxCommand->pxCommandLineDefinition->pcHelpString, 4, 8);
  pxCommand = pxCommand->pxNext;

  if( pxCommand == NULL )
  {
    /* There are no more commands in the list, so there will be no more
    strings to return after this one and pdFALSE should be returned. */
    xReturn = pdFALSE;
  }
  else
  {
    xReturn = pdTRUE;
  }

  return xReturn;
}
#endif
