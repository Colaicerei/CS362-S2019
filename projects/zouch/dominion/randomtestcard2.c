#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>
#include <time.h>

/*requirment for village effect: 
•	Current player should receive exactly 1 card.
•	The cards should come from his own pile.
•	Current player should receive exactly 2 actions.
•	The card (village) is moved to the played card pile.
•	No state change should occur for other players.
•	No state change should occur to the victory card piles and kingdom card piles.
*/

void assetTrue(int statement, char* passMessage, char* failMessage) {
	if (statement == 1) {
		printf(passMessage);
	}
	printf(failMessage);
}


void randomTestVillage() {

	int failTest = 0;
	int numPlayers;
	int seed = 1000;
	int currentPlayer = 0;
	int handPos = 0;
	int k[10] = { adventurer, council_room, feast, gardens, mine,
		remodel, smithy, village, baron, great_hall };
		
	struct gameState *testG = newGame();

	printf("\n---------- Random Test: Function VillageEffect() ----------\n\n");
		
	for (int n = 0; n < 1000; n++) {
		// set random player number to be 2 to 4
		numPlayers = rand() % 3 + 2;
		initializeGame(numPlayers, k, seed, testG);

		// set random deck count
		testG->deckCount[currentPlayer] = rand() % MAX_DECK;

		// save values before calling function
		int tempHandCount = testG->handCount[currentPlayer];
		int tempPlayedCount = testG->playedCardCount;
		int tempDeckCount = testG->deckCount[currentPlayer];
		int tempNumActions = testG->numActions;

		//call test function and check return value
		if (villageEffect(currentPlayer, testG, handPos) != 0) {
			printf("Failed! Return value is not zero!");
			failTest = 1;
		}

		//check if player has same number of cards on hand (one draw & one discard) 
		if (testG->handCount[currentPlayer] != tempHandCount) {
			printf("Fail! Card number should be the same. The player has %d cards on hand now, previously %d.\n", testG->handCount[currentPlayer], tempHandCount);
			failTest = 1;
		}

		//check if one card drawn from player's deck 
		if (testG->deckCount[currentPlayer] != (tempDeckCount - 1)) {
			printf("Fail! One card should be drawn. The player's deck has %d cards now, previously %d.\n", testG->deckCount[currentPlayer], tempDeckCount);
			failTest = 1;
		}

		//check if played card number increased by 1 
		if (testG->playedCardCount != (tempPlayedCount + 1)) {
			printf("Fail! One card should be moved to played pile. There are %d played cards now, previously %d.\n", testG->playedCardCount, tempPlayedCount);
			failTest = 1;
		}

		//check if number of actions increased by 2 
		if (testG->numActions != (tempNumActions + 2)) {
			printf("Fail! Exactly two actions should be added. There are %d actions now, previously %d.\n", testG->numActions, tempNumActions);
			failTest = 1;
		}
	}

	if (failTest == 1) {
		printf("Test Failed!\n\n");
	}
	else {
		printf("Test Passed! Results are As Expected!\n\n");
	}
}


int main(int argc, char *argv[])
{
	srand(time(NULL));
	randomTestVillage();
	return 0;
}