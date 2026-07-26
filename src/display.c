#include "../include/display.h"
#include <p101_c/p101_stdio.h>

void display(const struct p101_env *env, struct p101_error *err, const char *msg)
{
    p101_printf(env, err, "%s\n", msg);
}
