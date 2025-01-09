/*
 ============================================================================
 Name        : Point_Buy_Calculator.c
 Author      : Caeson Reckling
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

// Define Super Struct
typedef struct ability {
	struct {
		int score;
		int modifier;
	} strength, dexterity, constitution, intelligence, wisdom, charisma;
} ability_t;

//Display abilities
void displayAbilities(ability_t *main) {
	// Clear screen is omitted for cross-platform compatibility
	// system("clear");

	printf("	Abilities:\n");

	if (main->strength.score < 0){
		printf("	Strength: %d (%d)\n", main->strength.score, main->strength.modifier);
	}
	else {
		printf("	Strength: %d (%+d)\n", main->strength.score, main->strength.modifier);
	}

	if (main->dexterity.score < 0){
		printf("	Dexterity: %d (%d)\n", main->dexterity.score, main->dexterity.modifier);
	}
	else {
		printf("	Dexterity: %d (%+d)\n", main->dexterity.score, main->dexterity.modifier);
	}

	if (main->constitution.score < 0){
		printf("	Constitution: %d (%d)\n", main->constitution.score, main->constitution.modifier);
	}
	else {
		printf("	Constitution: %d (%+d)\n", main->constitution.score, main->constitution.modifier);
	}
	if (main->intelligence.score < 0){
		printf("	Intelligence: %d (%d)\n", main->intelligence.score, main->intelligence.modifier);
	}
	else {
		printf("	Intelligence: %d (%+d)\n", main->intelligence.score, main->intelligence.modifier);
	}
	if (main->wisdom.score < 0){
		printf("	Wisdom: %d (%d)\n", main->wisdom.score, main->wisdom.modifier);
	}
	else {
		printf("	Wisdom: %d (%+d)\n", main->wisdom.score, main->wisdom.modifier);
	}
	if (main->charisma.score < 0){
		printf("	Charisma: %d (%d)\n", main->charisma.score, main->charisma.modifier);
	}
	else {
		printf("	Charisma: %d (%+d)\n", main->charisma.score, main->charisma.modifier);
	}
}

int getPointCost(int score_value){
	int cost;
	switch (score_value) {
		case 8: {
			cost = 0;
			break;
		}

		case 9: {
			cost = 1;
			break;
		}

		case 10: {
			cost = 2;
			break;
		}

		case 11: {
			cost = 3;
			break;
		}

		case 12: {
			cost = 4;
			break;
		}

		case 13: {
			cost = 5;
			break;
		}

		case 14: {
			cost = 7;
			break;
		}

		case 15: {
			cost = 9;
			break;
		}
	}

	return cost;
}

// Update Modifier:
int updateModifier(int score_value){
	int mod;
		switch (score_value) {
			case 8: {
				mod = -1;
				break;
			}

			case 9: {
				mod = -1;
				break;
			}

			case 10: {
				mod = 0;
				break;
			}

			case 11: {
				mod = 0;
				break;
			}

			case 12: {
				mod = 1;
				break;
			}

			case 13: {
				mod = 1;
				break;
			}

			case 14: {
				mod = 2;
				break;
			}

			case 15: {
				mod = 2;
				break;
			}
		}

		return mod;
}

int main(void) {
	//Display Welcome:

printf("Welcome to the point buy calculator app:\n");

	// Initialize Attributes:
	ability_t character = {
		{8, -1}, // Strength
		{8, -1}, // Dexterity
		{8, -1}, // Constitution
		{8, -1}, // Intelligence
		{8, -1}, // Wisdom
		{8, -1}  // Charisma
	};

	int points_spent = 0;
	int score_value;
	int point_cost;
	int new_modifier;

	// Main Loop:
	while (points_spent <= 27){
		// Compute remaining points:
		int points_remaining = 27 - point_cost;

		// Display Attributes:
		displayAbilities(&character);

		//Display Menu:
		printf("Select a score to modify:\n");
		printf("	1. Strength\n");
		printf("	2. Dexterity\n");
		printf("	3. Constitution\n");
		printf("	4. Intelligence\n");
		printf("	5. Wisdom\n");
		printf("	6. Charisma\n");
		printf("You have %d points remaining. . .\n", points_remaining);

		// Get choice:
		printf("Your choice: ");
		int choice;
		scanf("%d", &choice);

		// Make decision:
		switch (choice) {
			case 1: {
				// Enter new value:
				printf("Enter a value between 8 and 15: ");
				scanf("%d", &score_value);

				// Update Points:
				point_cost += getPointCost(score_value);
				points_spent += point_cost;

				// Update Attributes:
				character.strength.score = score_value;
				new_modifier = updateModifier(score_value);
				character.strength.modifier = new_modifier;
				break;
			}

			case 2: {
				// Enter new value:
				printf("Enter a value between 8 and 15: ");
				scanf("%d", &score_value);

				// Update Points:
				point_cost += getPointCost(score_value);
				points_spent += point_cost;

				// Update Attributes:
				character.dexterity.score = score_value;
				new_modifier = updateModifier(score_value);
				character.dexterity.modifier = new_modifier;
				break;
			}

			case 3: {
				// Enter new value:
				printf("Enter a value between 8 and 15: ");
				scanf("%d", &score_value);

				// Update Points:
				point_cost += getPointCost(score_value);
				points_spent += point_cost;

				// Update Attributes:
				character.constitution.score = score_value;
				new_modifier = updateModifier(score_value);
				character.constitution.modifier = new_modifier;
				break;
			}

			case 4: {
				// Enter new value:
				printf("Enter a value between 8 and 15: ");
				scanf("%d", &score_value);

				// Update Points:
				point_cost += getPointCost(score_value);
				points_spent += point_cost;

				// Update Attributes:
				character.intelligence.score = score_value;
				new_modifier = updateModifier(score_value);
				character.intelligence.modifier = new_modifier;
				break;
			}

			case 5: {
				// Enter new value:
				printf("Enter a value between 8 and 15: ");
				scanf("%d", &score_value);

				// Update Points:
				point_cost += getPointCost(score_value);
				points_spent += point_cost;

				// Update Attributes:
				character.wisdom.score = score_value;
				new_modifier = updateModifier(score_value);
				character.wisdom.modifier = new_modifier;
				break;
			}

			case 6: {
				// Enter new value:
				printf("Enter a value between 8 and 15: ");
				scanf("%d", &score_value);

				// Update Points:
				point_cost += getPointCost(score_value);
				points_spent += point_cost;

				// Update Attributes:
				character.charisma.score = score_value;
				new_modifier = updateModifier(score_value);
				character.charisma.modifier = new_modifier;
				break;
			}

			default: {
				printf("Please type a number between 0 and 6.");
			}

		}
	}

	// Display final value:
	printf("Your final values are:\n");
	displayAbilities(&character);

	return EXIT_SUCCESS;
}
