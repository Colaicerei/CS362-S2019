#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "minion"

/*requirment for minion card: 
•	Current player should receive exactly 1 extra action.
•	Current player can choose Option 1: get 2 coins, OR
•	Option 2: discard hand and redraw 4 cards.
•	The cards should come from player’s own pile.
•	If option 2 is chosen, other players with 5+ cards in hand should also discard hand and redraw 4 cards.
•	No state change should occur to the victory card piles and kingdom card piles.
*/

void testMinionCard() {

	int failTest = 0;
	int numPlayers = 4;
	int currentPlayer = 0;
	int seed = 1000;
	int handPos = 0;
	int bonus = 0;
	int choice1 = 0, choice2 = 0;
	int k[10] = { adventurer, council_room, feast, gardens, mine,
		remodel, smithy, village, baron, great_hall };

	struct gameState *testG = newGame();

	initializeGame(numPlayers, k, seed, testG);

	printf("----------------Testing Card : %s----------------\n", TESTCARD);

	int tempHandCount = testG->handCount[currentPlayer];
	int tempPlayedCount = testG->playedCardCount;
	int tempDeckCount = testG->deckCount[currentPlayer];
	int tempActions = testG->numActions;
	int tempCoins = testG->coins;

	// ----------- TEST 1: choice1 = 1 = get 2 coins --------------
	printf("\n*****TEST 1: choice1 = 1 = get 2 coins*****\n\n");

	//choose choice 1 and call cardEffect and check return value
	choice1 = 1;
	if (cardEffect(minion, choice1, choice2, 0, testG, handPos, &bonus) != 0) {
		printf("Failed! Return value is not zero!");
		failTest = 1;
	}

	//check if player has one more action
	if (testG->numActions == (tempActions + 1)) {
		printf("Pass! The player has one more action.\n");
	}
	else {
		printf("Fail! Action should increased by 1. The player has %d actions now, previously %d.\n", testG->numActions, tempActions);
		failTest = 1;
	}

	//check if player has one less card on hand (no draw & one discard) 
	if (testG->handCount[currentPlayer] == (tempHandCount - 1)) {
		printf("Pass! The player has no drawn and discarded one card.\n");
	}
	else {
		printf("Fail! Card number should be decreased by 1. The player has %d cards on hand now, previously %d.\n", testG->handCount[currentPlayer], tempHandCount);
		failTest = 1;
	}

	//check if no card drawn from player's deck 
	if (testG->deckCount[currentPlayer] == tempDeckCount) {
		printf("Pass! No card was drawn from deck.\n");
	}
	else {
		printf("Fail! No card should be drawn. The player's deck has %d cards now, previously %d.\n", testG->deckCount[currentPlayer], tempDeckCount);
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

	//check if number of coins increased by 2 
	if (testG->coins == (tempCoins + 2)) {
		printf("Pass! The player received additional 2 coins.\n");
	}
	else {
		printf("Fail! Exactly 2 coins should be added. There are %d coins now, previously %d.\n", testG->coins, tempCoins);
		failTest = 1;
	}

	// ----------- TEST 2: choice1 = 0 && choice2 = 1 -> discard hand and redraw --------------
	printf("\n*****TEST 2: choice1 = 0 && choice2 = 1 -> discard hand and redraw*****\n\n");
	choice1 = 0;
	choice2 = 1;
	tempHandCount = testG->handCount[currentPlayer];
	tempPlayedCount = testG->playedCardCount;
	tempDeckCount = testG->deckCount[currentPlayer];
	tempActions = testG->numActions;
	tempCoins = testG->coins;

	if (cardEffect(minion, choice1, choice2, 0, testG, handPos, &bonus) != 0) {
		printf("Failed! Return value is not zero!");
		failTest = 1;
	}

	//check if player has one more action
	if (testG->numActions == (tempActions + 1)) {
		printf("Pass! The player has one more action.\n");
	}
	else {
		printf("Fail! Action should increased by 1. The player has %d actions now, previously %d.\n", testG->numActions, tempActions);
		failTest = 1;
	}

	//check if player has four cards on hand (discard hand and 4 draws) 
	if (testG->handCount[currentPlayer] == 4) {
		printf("Pass! The player discarded hand and redrawn 4 cards.\n");
	}
	else {
		printf("Fail! Card number should be 4. The player has %d cards on hand now, previously %d.\n", testG->handCount[currentPlayer], tempHandCount);
		failTest = 1;
	}

	//check if four cards drawn from player's deck 
	if (testG->deckCount[currentPlayer] == tempDeckCount - 4){
		printf("Pass! Four cards were drawn from deck.\n");
		}
	else {
			printf("Fail! Four cards should be drawn. The player's deck has %d cards now, previously %d.\n", testG->deckCount[currentPlayer], tempDeckCount);
			failTest = 1;
		}

		//check if played card number increased by 4 
		if (testG->playedCardCount == (tempPlayedCount + tempHandCount)) {
			printf("Pass! The player discarded exactly 4 cards.\n");
		}
		else {
			printf("Fail! %d cards on hands should be moved to played pile.\n", tempHandCount);
			failTest = 1;
		}

		//check if number of coins remains same 
		if (testG->coins == (tempCoins)) {
			printf("Pass! No change to coins for choice 2.\n");
		}
		else {
			printf("Fail! No coins should be added for choice 2. There are %d coins now, previously %d.\n", testG->coins, tempCoins);
			failTest = 1;
		}
	
		//check if each other player has four cards on hand (discard hand and 4 draws)
		for (int i = 0; i < testG->numPlayers; i++) {
			if (i != currentPlayer) {
				if (testG->handCount[currentPlayer] != 4 || 1 != testG->deckCount[currentPlayer]) {
					printf("Fail! Player %d did not discarad hand and draw four cards from deck.\n", i);
					failTest = 1;
				}
				else {
					printf("Pass! Player %d has discard hand and drawn 4 cards from deck.\n", i);
				}
			}
		}

	// ----------- TEST 3: choice1 = 0 && choice2 = 0 -> Nothing happens --------------
	printf("\n*****TEST 3: choice1 = 0 && choice2 = 0 -> Do nothing*****\n\n");
	choice1 = 0;
	choice2 = 0;
	tempHandCount = testG->handCount[currentPlayer];
	tempPlayedCount = testG->playedCardCount;
	tempDeckCount = testG->deckCount[currentPlayer];
	tempActions = testG->numActions;
	tempCoins = testG->coins;

	if (cardEffect(minion, choice1, choice2, 0, testG, handPos, &bonus) != 0) {
		printf("Failed! Return value is not zero!");
		failTest = 1;
	}

	//check if player has one more action
	if (testG->numActions == (tempActions + 1)) {
		printf("Pass! The player has one more action.\n");
	}
	else {
		printf("Fail! Action should increased by 1. The player has %d actions now, previously %d.\n", testG->numActions, tempActions);
		failTest = 1;
	}

	//check if player has one less card on hand (no draw & one discard) 
	if (testG->handCount[currentPlayer] == (tempHandCount - 1)) {
		printf("Pass! The player has no drawn and discarded one card.\n");
	}
	else {
		printf("Fail! Card number should be decreased by 1. The player has %d cards on hand now, previously %d.\n", testG->handCount[currentPlayer], tempHandCount);
		failTest = 1;
	}

	//check if no card drawn from player's deck 
	if (testG->deckCount[currentPlayer] == tempDeckCount){
		printf("Pass! No card was drawn from deck.\n");
		}
	else {
			printf("Fail! No card should be drawn. The player's deck has %d cards now, previously %d.\n", testG->deckCount[currentPlayer], tempDeckCount);
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

		//check if number of coins remains same 
		if (testG->coins == (tempCoins)) {
			printf("Pass! No change to coins for choice 2.\n");
		}
		else {
			printf("Fail! No coins should be added for choice 2. There are %d coins now, previously %d.\n", testG->coins, tempCoins);
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
	testMinionCard();
	return 0;
}