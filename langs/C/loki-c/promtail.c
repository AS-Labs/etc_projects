#include <fcntl.h>
#include <openssl/md5.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define TIMEOUT_UNTIL_FANOUT_HARD_STOP 30 // in seconds

// Error handling
#define ERR_CONFIG_NOT_CHANGE "Config has not changed"

// Logger stub
void log_info(const char *msg) { printf("INFO: %s\n", msg); }

void log_error(const char *msg) { fprintf(stderr, "ERROR: %s\n", msg); }

// Configuration struct
typedef struct {
  char *config_string;
} Config;

// Promtail struct
typedef struct {
  // Simulated components
  void *client;
  void *wal_writer;
  void *entries_fanout;
  void *target_managers;
  void *server;
  void *logger;
  void *reg;
  int stopped;
  pthread_mutex_t mtx;
  char *config_loaded;
  Config *(*new_config)();
  void *metrics;
  int dry_run;
} Promtail;

// Function stubs
void *new_client(void *metrics, void *logger, void *client_configs) {
  return NULL; // Replace with actual client creation logic
}

void *new_server(Config *cfg, void *logger, void *target_managers) {
  return NULL; // Replace with actual server creation logic
}

void *new_wal_writer(void *cfg, void *logger, void *reg) {
  return NULL; // Replace with actual WAL writer creation logic
}

void *new_target_managers(Promtail *p, void *reg, void *logger,
                          void *positions_config, void *entries_fanout,
                          void *scrape_config, void *target_config,
                          void *file_watch, void *limits_config) {
  return NULL; // Replace with actual target managers creation logic
}

void *new_fanout_entry_handler(int timeout, void **handlers, int num_handlers) {
  return NULL; // Replace with actual fanout entry handler creation logic
}

// Helper function to calculate MD5 checksum
void md5_sum(const char *str, char *output) {
  unsigned char digest[MD5_DIGEST_LENGTH];
  MD5((unsigned char *)str, strlen(str), digest);
  for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
    sprintf(output + (i * 2), "%02x", digest[i]);
  }
}

// Reload configuration
int reload_config(Promtail *p, Config *cfg) {
  char new_config_file[256];
  char new_conf[256];
  char md5sum[MD5_DIGEST_LENGTH * 2 + 1];

  // Simulate logging
  log_info("Reloading configuration file");

  pthread_mutex_lock(&p->mtx);

  // Check if configuration has changed
  snprintf(new_config_file, sizeof(new_config_file), "%s", cfg->config_string);
  if (strcmp(new_config_file, p->config_loaded) == 0) {
    pthread_mutex_unlock(&p->mtx);
    return -1; // Config has not changed
  }

  snprintf(new_conf, sizeof(new_conf), "%s", cfg->config_string);
  md5_sum(new_conf, md5sum);
  printf("Reloading configuration file, md5sum: %s\n", md5sum);

  // Placeholder for actual stop logic
  if (p->target_managers != NULL) {
    // Stop target managers
  }
  if (p->client != NULL) {
    // Stop client
  }

  // Placeholder for actual setup logic
  // cfg->Setup(p->logger);

  // Placeholder for client creation logic
  if (p->dry_run) {
    p->client = new_client(p->metrics, p->logger, NULL);
    p->client = NULL; // Example change for dry run
  } else {
    p->wal_writer = new_wal_writer(cfg, p->logger, p->reg);
    p->client = new_client(p->metrics, p->logger, NULL);
  }

  // Placeholder for actual entry handlers creation logic
  void *entry_handlers[2];
  entry_handlers[0] = p->wal_writer;
  entry_handlers[1] = p->client;
  p->entries_fanout = new_fanout_entry_handler(TIMEOUT_UNTIL_FANOUT_HARD_STOP,
                                               entry_handlers, 2);

  p->target_managers = new_target_managers(
      p, p->reg, p->logger, NULL, p->entries_fanout, NULL, NULL, NULL, NULL);

  p->config_loaded = strdup(new_conf);
  pthread_mutex_unlock(&p->mtx);

  return 0;
}

// Run the promtail
int run_promtail(Promtail *p) {
  pthread_mutex_lock(&p->mtx);
  if (p->stopped) {
    pthread_mutex_unlock(&p->mtx);
    return 0; // Already stopped
  }
  pthread_mutex_unlock(&p->mtx);

  // Placeholder for actual config watch and server run logic
  // p->watch_config();
  // p->server_run();

  return 0;
}

// Shutdown the promtail
void shutdown_promtail(Promtail *p) {
  pthread_mutex_lock(&p->mtx);
  if (p->stopped) {
    pthread_mutex_unlock(&p->mtx);
    return;
  }
  p->stopped = 1;
  if (p->server != NULL) {
    // Shutdown server
  }
  if (p->target_managers != NULL) {
    // Stop target managers
  }
  if (p->entries_fanout != NULL) {
    // Stop entries fanout
  }
  if (p->wal_writer != NULL) {
    // Stop WAL writer
  }
  if (p->client != NULL) {
    // Stop client
  }
  pthread_mutex_unlock(&p->mtx);
}

// Create a new Promtail instance
Promtail *new_promtail(Config *cfg, Config *(*new_config)(), void *metrics,
                       int dry_run) {
  Promtail *p = (Promtail *)malloc(sizeof(Promtail));
  if (!p) {
    log_error("Failed to allocate memory for Promtail");
    return NULL;
  }

  p->logger = NULL; // Set to actual logger
  p->reg = NULL;    // Set to actual registerer
  p->metrics = metrics;
  p->dry_run = dry_run;
  p->client = NULL;
  p->wal_writer = NULL;
  p->entries_fanout = NULL;
  p->target_managers = NULL;
  p->server = NULL;
  p->stopped = 0;
  pthread_mutex_init(&p->mtx, NULL);
  p->config_loaded = NULL;
  p->new_config = new_config;

  if (reload_config(p, cfg) != 0) {
    free(p);
    return NULL;
  }

  p->server = new_server(cfg, p->logger, p->target_managers);

  return p;
}

int main() {
  // Example usage
  Config cfg = {"example_config"};
  Promtail *p = new_promtail(&cfg, NULL, NULL, 0);
  if (p) {
    run_promtail(p);
    shutdown_promtail(p);
    free(p);
  }
  return 0;
}
