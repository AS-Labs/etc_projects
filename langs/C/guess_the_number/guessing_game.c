#include "randnum.h"
#include <stdio.h>

int main(void) {
  int number;
  int guess;
  int number_of_guesses = 0;
  number = randnum(100);

  puts("Guess a number between 1 and 100");

  do {
    number_of_guesses++;

    scanf("%d", &guess);
    if (guess < number) {
      puts("Too low");
    } else if (guess > number) {
      puts("Too high");
    }
  } while (guess != number);
  printf("Congratulations!\nYou guessed in %d tries\n", number_of_guesses);
  return 0;
}
