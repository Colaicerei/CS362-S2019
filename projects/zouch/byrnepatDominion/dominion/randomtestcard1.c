#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>
#include <time.h>

#define TESTCARD "smithy"

/*requirment for smithy card: 
•	Current player should receive exactly 3 cards.
•	3 cards should come from his own pile.
•	1 card (smithy) is moved to the played card pile.
•	No state change should occur for other players.
•	No state change should occur to the victory card piles and kingdom card piles.
*/

void randomTestSmithy() {

	int failTest = 0;
	int numPlayers;
	int currentPlayer = 0;
	int seed = 1000;
	int handPos = 0;
	int k[10] = { adventurer, council_room, feast, gardens, mine,
		remodel, smithy, village, baron, great_hall };
		
	struct gameState *testG = newGame();

	printf("----------------Random Test: Function SmithyCard by PB----------------\n\n");

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

		//call smithyEffect and check return value
		if (smithyCard(currentPlayer, testG, handPos) != 0) {
			printf("Failed! Return value is not zero!");
			failTest = 1;
		}

		//check if player has additional two cards on hand (three draws & one discard) 
		if (testG->handCount[currentPlayer] != (tempHandCount + 2)) {
			printf("Fail. Two cards should be grain in total. The player has %d cards on hand now, previously %d.\n", testG->handCount[currentPlayer], tempHandCount);
			failTest = 1;
		}

		//check if three cards are drawn from player's deck 
		if (testG->deckCount[currentPlayer] != (tempDeckCount - 3)) {
			printf("Fail. Three cards should be drawn. The player's deck has %d cards now, previously %d.\n", testG->deckCount[currentPlayer], tempDeckCount);
			failTest = 1;
		}

		//check if played card number increased by 1 
		if (testG->playedCardCount != (tempPlayedCount + 1)) {
			printf("Fail. One card should be moved to played pile. There are %d played cards now, previously %d.\n", testG->playedCardCount, tempPlayedCount);
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
	randomTestSmithy();
	return 0;
}