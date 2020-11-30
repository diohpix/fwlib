#include "./config.h"

#ifndef _WIN32
#include <getopt.h>
#endif
#include "../extern/libconfig/lib/libconfig.h"
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

const Config default_config = {"127.0.0.1", 8193};

#ifdef _WIN32
int read_arg_config(int argc, char *argv[], Config *conf) {
  char ip[100] = "";
  int tmp;
  int port = 0;
  // ip port config
  if (argc < 2) {
    // do nothing, use defaults
    return 0;

  } else if (argc == 2) {
    // first arg is a config file
    if (read_file_config(argv[1], conf)) {
      // invalid config file
      return 1;
    }
    return 0;

  } else if (argc == 3) {
    // first arg is ip, second arg is port
    snprintf(ip, 100, "%s", argv[1]);
    tmp = atoi(argv[2]);
    if (tmp < 1 || tmp > 65535) {
      // invalid port
    }
    port = tmp;
    return 0;

  } else {
    return 0;
  }
  return 0;
}
#else
static struct option options[] = {{"ip", required_argument, NULL, 'h'},
                                  {"port", required_argument, NULL, 'p'},
                                  {"config", required_argument, NULL, 'c'},
                                  {NULL, 0, NULL, 0}};

int read_arg_config(int argc, char *argv[], Config *conf) {
  int c;
  int i = 0;
  int tmp;
  int port = 0;
  bool ip_flag = false;
  char ip[100] = "";

  while ((c = getopt_long(argc, argv, "hpc", options, &i)) != -1) {
    switch (c) {
      case 'h':
        ip_flag = true;
        snprintf(ip, 100, "%s", optarg);
        break;

      case 'p':
        if ((tmp = atoi(optarg)) != 0 && tmp > 0 && tmp < 65535) {
          port = tmp;
        } else {
          fprintf(stderr, "invalid port: \"%s\"\n", optarg);
          return 1;
        }
        break;
      case 'c':
        if (read_file_config(optarg, conf)) {
          return 1;
        }
        break;

      case '?':
        /* getopt_long already printed an error message. */
        return 1;
    }
  }

  if (ip_flag) {
    snprintf(conf->ip, 100, "%s", ip);
  }
  if (port != 0) {
    conf->port = port;
  }

  return 0;
}
#endif

#ifdef _WIN32
int read_env_config(Config *conf) {
  int iTmp;
  char buf[100] = "";
  char *pTmp = (char *)buf;
  size_t n;

  n = 100;
  if (_dupenv_s(&pTmp, &n, "DEVICE_IP") == 0 && pTmp != NULL) {
    snprintf(conf->ip, 100, "%s", pTmp);
  }
  n = 100;
  if (_dupenv_s(&pTmp, &n, "DEVICE_PORT") == 0 && pTmp != NULL && (iTmp = atoi(pTmp)) > 0 && iTmp < 65535) {
    conf->port = iTmp;
  }

  return 0;
}
#else
int read_env_config(Config *conf) {
  int iTmp;
  char *pTmp;

  if ((pTmp = getenv("DEVICE_IP")) != NULL) {
    snprintf(conf->ip, 100, "%s", pTmp);
  }

  if (((pTmp = getenv("DEVICE_PORT")) != NULL) && (iTmp = atoi(pTmp)) > 0) {
    conf->port = iTmp;
  }

  return 0;
}
#endif

int read_file_config(const char *cfg_file, Config *conf) {
  config_t cfg;

  config_init(&cfg);
  const char *tmp;

  if (config_read_file(&cfg, cfg_file) != CONFIG_TRUE) {
    fprintf(stderr, "unable to read config file \"%s\"\n", cfg_file);
    config_destroy(&cfg);
    return 1;
  }

  if (config_lookup_string(&cfg, "ip", &tmp) == CONFIG_TRUE) {
    snprintf(conf->ip, 100, "%s", tmp);
  }
  config_lookup_int(&cfg, "port", &conf->port);

  config_destroy(&cfg);

  return 0;
}

int read_config(int argc, char *argv[], Config *conf) {
  Config a = default_config;
  if (read_env_config(&a) || read_arg_config(argc, argv, &a)) {
    return 1;
  }
  *conf = a;
  return 0;
}
