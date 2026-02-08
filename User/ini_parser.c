#include "ini_parser.h"
#include <string.h>
#include <ctype.h>

static void trim(char *s)
{
    char *p = s;
    while (isspace((int)*p)) p++;
    memmove(s, p, strlen(p)+1);

    for (int i = strlen(s)-1; i >= 0; i--)
    {
        if (isspace((int)s[i])) s[i] = 0;
        else break;
    }
}

void INI_Parse(const char *text, ini_callback_t cb)
{
    char line[256];
    char section[32] = "";

    const char *ptr = text;

    while (*ptr)
    {
        int len = 0;
        while (*ptr && *ptr != '\n' && len < sizeof(line)-1)
            line[len++] = *ptr++;
        line[len] = 0;
        if (*ptr == '\n') ptr++;

        trim(line);
        if (line[0] == 0 || line[0] == ';')
            continue;

        if (line[0] == '[')
        {
            char *end = strchr(line, ']');
            if (end)
            {
                *end = 0;
                strncpy(section, &line[1], sizeof(section));
            }
            continue;
        }

        char *eq = strchr(line, '=');
        if (!eq) continue;

        *eq = 0;
        char *key = line;
        char *value = eq + 1;
        trim(key);
        trim(value);

        cb(section, key, value);
    }
}
