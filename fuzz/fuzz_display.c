/*
 * EXAMPLE libFuzzer harness for this template's own code.
 *
 * This template only has display(), which prints its argument — there is no
 * untrusted-input parsing to break, so this harness will (correctly) never find
 * a bug. It is here to show the MECHANISM and to keep every project fuzz-ready:
 * when you add code that parses input, point LLVMFuzzerTestOneInput() at THAT
 * function and the fuzzer + ASan/UBSan start earning their keep (watch the
 * "cov:" counter climb as it explores your branches).
 *
 * display()'s source is compiled straight into this binary (see
 * fuzz/CMakeLists.txt) so it is instrumented and coverage-guided, not black-box.
 */
#include "display.h"
#include <p101_c/p101_setjmp.h>
#include <p101_c/p101_stdio.h>
#include <p101_c/p101_stdlib.h>
#include <p101_c/p101_string.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

/* Runs once. display() writes to stdout; silence it so millions of iterations
 * don't flood the terminal. Delete this to watch what the target prints. */
int LLVMFuzzerInitialize(int *argc, char ***argv)
{
    struct p101_error *err;
    struct p101_env   *env;

    (void)argc;
    (void)argv;
    err = p101_error_create(false);
    env = p101_env_create(err, NULL);
    (void)p101_freopen(env, err, "/dev/null", "w", stdout);
    p101_env_destroy(env);
    p101_error_destroy(err);
    return 0;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    struct p101_error *err;
    struct p101_env   *env;
    char              *s;

    err = p101_error_create(false);
    env = p101_env_create(err, NULL);
    s   = (char *)p101_malloc(env, err, size + 1);
    if(s == NULL)
    {
        goto done;
    }
    p101_memcpy(env, s, data, size);
    s[size] = '\0';

    display(env, err, s); /* <-- EXAMPLE target. Swap for your own input-parsing code. */

done:
    p101_free(env, s);
    p101_env_destroy(env);
    p101_error_destroy(err);
    return 0;
}
