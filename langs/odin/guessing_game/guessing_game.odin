package guessing_game

import "core:fmt"
import "random"

random_value: int
number: int
guess: int
number_of_guesses: int


main :: proc() {
	random.seed()
	random_value = random.random(1, 100)
	fmt.Println("Guess the number!")
}
