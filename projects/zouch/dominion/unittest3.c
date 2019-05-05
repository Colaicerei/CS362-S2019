#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

/*requirment for council_room card: 
•	Current player should receive exactly 4 cards.
•	the cards should come from his own pile.
•	Current player should receive exactly one chance of purchase.
•	the card (council_room) is moved to the played card pile.
•	All other players draw a card
•	No state change should occur to the victory card piles and kingdom card piles.
*/

void testCouncilRoomEffect() {

	int failTest = 0;
	int numPlayers = 4;
	int seed = 1000;
	int currentPlayer = 0;
	int handPos = 0;
	int k[10] = { adventurer, council_room, feast, gardens, mine,
		remodel, smithy, village, baron, great_hall };
		
	struct gameState *testG = newGame();

	initializeGame(numPlayers, k, seed, testG);

	printf("\n---------- Unittest 3: Function CouncilRoomEffect() ----------\n\n");

	int tempHandCount = testG->handCount[currentPlayer];
	int tempPlayedCount = testG->playedCardCount;
	int tempDeckCount = testG->deckCount[currentPlayer];
	int tempNumBuys = testG->numBuys;
	
	//call test function and check return value
	if (councilRoomEffect(currentPlayer, testG, handPos) != 0) {
		printf("Failed! Return value is not zero!");
		failTest = 1;
	}

	//check if player has additional three cards on hand (four draws & one discard) 
	if (testG->handCount[currentPlayer] == (tempHandCount + 3)) {
		printf("Pass! The player gained exactly 3 cards in total.\n");
	}
	else {
		printf("Fail! Three cards should be gained in total. The player has %d cards on hand now, previously %d.\n", testG->handCount[currentPlayer], tempHandCount);
		failTest = 1;
	}

	//check if four cards are drawn from player's deck 
	if (testG->deckCount[currentPlayer] == (tempDeckCount - 4)) {
		printf("Pass! Four cards were drawn from deck.\n");
	}
	else {
		printf("Fail! Four cards should be drawn. The player's deck has %d cards now, previously %d.\n", testG->deckCount[currentPlayer], tempDeckCount);
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

	//check if number of buys increased by 1 
	if (testG->numBuys == (tempNumBuys + 1)) {
		printf("Pass! The player received additional one buy.\n");
	}
	else {
		printf("Fail! Exactly one buy should be added. There are %d buys now, previously %d.\n", testG->numBuys, tempNumBuys);
		failTest = 1;
	}

	//check if each other player draws a card
	for (int i = 0; i < testG->numPlayers; i++) {
		if (i != currentPlayer) {
			if (testG->handCount[currentPlayer] != tempHandCount + 1 || tempDeckCount - 1 != testG->deckCount[currentPlayer]) {
				printf("Fail! Player %d did not draw one card from deck.\n", i);
				failTest = 1;
			}
			else {
				printf("Player %d has drawn one card from deck.\n", i);
			}
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
	testCouncilRoomEffect();
	return 0;
}