#include "unity.h"
#include "../include/display.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
static void capture_display(const char *msg, char *out, size_t n){
    int saved; FILE *tmp; size_t r;
    fflush(stdout); saved = dup(fileno(stdout)); tmp = tmpfile();
    dup2(fileno(tmp), fileno(stdout));
    display(msg);
    fflush(stdout); dup2(saved, fileno(stdout)); close(saved);
    rewind(tmp); r = fread(out, 1, n-1, tmp); out[r]='\0'; fclose(tmp);
}
void setUp(void){} void tearDown(void){}
static void test_display_appends_newline(void){ char b[64]; capture_display("hello",b,sizeof b); TEST_ASSERT_EQUAL_STRING("hello\n", b); }
static void test_display_empty(void){ char b[64]; capture_display("",b,sizeof b); TEST_ASSERT_EQUAL_STRING("\n", b); }
int main(void){ UNITY_BEGIN(); RUN_TEST(test_display_appends_newline); RUN_TEST(test_display_empty); return UNITY_END(); }
