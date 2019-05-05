#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "great_hall"

/*requirment for great_hall card: 
•	Current player should receive exactly 1 card.
•	Player should receive exactly 1 actions.
•	The cards should come from player’s own pile.
•	1 card (Great Hall) should be moved to the played card pile.
•	No state change should occur for other players.
•	No state change should occur to the victory card piles and kingdom card piles.
*/

void testGreatHallCard() {

	int failTest = 0;
	int numPlayers = 4;
	int currentPlayer = 0;
	int seed = 1000;
	int handPos = 0;
	int bonus = 0;
	int k[10] = { adventurer, council_room, feast, gardens, mine,
		remodel, smithy, village, baron, great_hall };

	struct gameState *testG = newGame();

	initializeGame(numPlayers, k, seed, testG);

	printf("----------------Testing Card : %s----------------\n", TESTCARD);

	int tempHandCount = testG->handCount[currentPlayer];
	int tempPlayedCount = testG->playedCardCount;
	int tempDeckCount = testG->deckCount[currentPlayer];
	int tempNumActions = testG->numActions;

	//call cardEffect and check return value
	if (cardEffect(great_hall, 0, 0, 0, testG, handPos, &bonus) != 0) {
		printf("Failed! Return value is not zero!");
		failTest = 1;
	}

	//check if player has same number of cards on hand (one draw & one discard) 
	if (testG->handCount[currentPlayer] == tempHandCount) {
		printf("Pass! The player has drawn one card and discarded one card.\n");
	}
	else {
		printf("Fail! Card number should be the same. The player has %d cards on hand now, previously %d.\n", testG->handCount[currentPlayer], tempHandCount);
		failTest = 1;
	}

	//check if one card drawn from player's deck 
	if (testG->deckCount[currentPlayer] == (tempDeckCount - 1)) {
		printf("Pass! One card was drawn from deck.\n");
	}
	else {
		printf("Fail! One card should be drawn. The player's deck has %d cards now, previously %d.\n", testG->deckCount[currentPlayer], tempDeckCount);
		failTest = 1;
	}

	//check if played card number increased by 1 
	if (testG->playedCardCount == (tempPlayedCount + 1)) {
		printf("Pass! The player discarded exactly one card.\n");
	}
	else {
		printf("Fail! One card should be moved to played pile. There are %d played cards now, previously %d.\n", testG->playedCardCount, tempPlayedCount);
		failTest = 1;
	}

	//check if number of actions increased by 1 
	if (testG->numActions == (tempNumActions + 1)) {
		printf("Pass! The player received additional 1 action.\n");
	}
	else {
		printf("Fail! Exactly 1 action should be added. There are %d actions now, previously %d.\n", testG->numActions, tempNumActions);
		failTest = 1;
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
	testGreatHallCard();
	return 0;
}