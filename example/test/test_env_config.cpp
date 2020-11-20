#define _DEFAULT_SOURCE
#include <stdio.h>
#include <stdlib.h>

#include "../src/config.c"
#include "gtest/gtest.h"
#ifdef _WIN32
int setenv(const char *name, const char *value, int overwrite)
{
    int errcode = 0;
    if(!overwrite) {
        size_t envsize = 0;
        errcode = getenv_s(&envsize, NULL, 0, name);
        if(errcode || envsize) return errcode;
    }
    return _putenv_s(name, value);
}

int unsetenv(const char *name)
{
    return _putenv_s(name, "");
}

#endif

TEST(Config, EnvConfigWorks) {
  Config conf = {"0.0.0.0", 1234};

  unsetenv("DEVICE_IP");
  unsetenv("DEVICE_PORT");

  ASSERT_EQ(read_env_config(&conf), 0);
  EXPECT_STREQ(conf.ip, "0.0.0.0");
  EXPECT_EQ(conf.port, 1234);

  setenv("DEVICE_IP", "127.0.0.1", 1);
  setenv("DEVICE_PORT", "8193", 1);

  ASSERT_EQ(read_env_config(&conf), 0);
  EXPECT_STREQ(conf.ip, "127.0.0.1");
  EXPECT_EQ(conf.port, 8193);
}
