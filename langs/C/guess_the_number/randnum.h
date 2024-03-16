#include <stdio.h>
#include <sys/random.h>

int randnum(int max_value) {
  int random_value;

  getrandom(&random_value, sizeof(int), GRND_NONBLOCK);

  if (random_value < 0) {
    return (-1 * random_value % max_value + 1);
  } else {
    return (random_value % max_value + 1);
  }
}
