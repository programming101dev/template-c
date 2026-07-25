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
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Runs once. display() writes to stdout; silence it so millions of iterations
 * don't flood the terminal. Delete this to watch what the target prints. */
int LLVMFuzzerInitialize(int *argc, char ***argv)
{
    (void)argc;
    (void)argv;
    (void)freopen("/dev/null", "w", stdout);
    return 0;
}

int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size)
{
    char *s;

    s = (char *)malloc(size + 1);
    if(s == NULL)
    {
        return 0;
    }
    memcpy(s, data, size);
    s[size] = '\0';

    display(s); /* <-- EXAMPLE target. Swap for your own input-parsing code. */

    free(s);
    return 0;
}
