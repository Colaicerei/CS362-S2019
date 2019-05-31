#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

/*requirment for village effect: 
•	Current player should receive exactly 1 card.
•	The cards should come from his own pile.
•	Current player should receive exactly 2 actions.
•	The card (village) is moved to the played card pile.
•	No state change should occur for other players.
•	No state change should occur to the victory card piles and kingdom card piles.
*/

void testVillageEffect() {

	int failTest = 0;
	int numPlayers = 4;
	int seed = 1000;
	int currentPlayer = 0;
	int handPos = 0;
	int k[10] = { adventurer, council_room, feast, gardens, mine,
		remodel, smithy, village, baron, great_hall };
		
	struct gameState *testG = newGame();

	initializeGame(numPlayers, k, seed, testG);

	printf("\n---------- Unittest 4: Function VillageEffect() ----------\n\n");

	int tempHandCount = testG->handCount[currentPlayer];
	int tempPlayedCount = testG->playedCardCount;
	int tempDeckCount = testG->deckCount[currentPlayer];
	int tempNumActions = testG->numActions;
	
	//call test function and check return value
	if (villageCard(currentPlayer, testG, handPos) != 0) {
		printf("Failed! Return value is not zero!");
		failTest = 1;
	}

	//check if player has same number of cards on hand (one draw & one discard) 
	if (testG->handCount[currentPlayer] == tempHandCount) {
		printf("Pass! The player drawed one card and discarded one card.\n");
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

	//check if number of actions increased by 2 
	if (testG->numActions == (tempNumActions + 2)) {
		printf("Pass! The player received additional two actions.\n");
	}
	else {
		printf("Fail! Exactly two actions should be added. There are %d actions now, previously %d.\n", testG->numActions, tempNumActions);
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
	testVillageEffect();
	return 0;
}