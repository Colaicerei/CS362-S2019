#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

/*requirment for mineEffect: 
•	Current player should trash exactly one treasure card.
•	Player should receive exactly one treasure card with up to one upper level of card trashed.
•	Can also trash for other kingdom treasures.
•	The cards should come from supply, no change to player’s deck.
•	The card (mine) and trashed treasure are moved to the played card pile.
•	No state change should occur for other players.
•	No state change should occur to the victory card piles and kingdom card piles.
*/

void testMineEffect() {

	int failTest = 0;
	int numPlayers = 4;
	int seed = 1000;
	int currentPlayer = 0;
	int handPos = 0;
	int choice1 = 0, choice2 = 0;
	int k[10] = { adventurer, council_room, feast, gardens, mine,
		remodel, smithy, village, baron, great_hall };
		
	
	printf("\n---------- Unittest 2: Function mineEffect() ----------\n\n");

	struct gameState *testG = newGame();
	initializeGame(numPlayers, k, seed, testG);
	int tempHandCount = testG->handCount[currentPlayer];
	int tempPlayedCount = testG->playedCardCount;
	int tempDeckCount = testG->deckCount[currentPlayer];

	// ----------- TEST 1: choice1 = copper and choice2 = silver --------------
	printf("\n*****TEST 1: choice1 = 4 (copper), choice 2 = 5 (silver)*****\n\n");

	//choose choice 1 and call cardEffect and check return value
	choice1 = copper;
	choice2 = silver;

	//save previous copper and silver card numbers
	int tempCopper = 0, tempSilver = 0;
	for (int i = 0; i < testG->handCount[currentPlayer]; i++) {
		if (testG->hand[currentPlayer][i] == copper) {
			tempCopper++;
		}
	}
	for (int i = 0; i < testG->handCount[currentPlayer]; i++) {
		if (testG->hand[currentPlayer][i] == silver) {
			tempSilver++;
		}
	}

	//call test function and check return value
	if (mineEffect(currentPlayer, testG, choice1, choice2, handPos) != 0) {
		printf("Failed! Return value is not zero!");
		failTest = 1;
	}

	//check if player has one less one card on hand (one exchange and one discard) 
	if (testG->handCount[currentPlayer] == tempHandCount - 1) {
		printf("Pass! The player exchanged one card and discarded one card.\n");
	}
	else {
		printf("Fail! Card number should be one less. The player has %d cards on hand now, previously %d.\n", testG->handCount[currentPlayer], tempHandCount);
		failTest = 1;
	}

	//check if no card was drawn from player's deck 
	if (testG->deckCount[currentPlayer] == (tempDeckCount)) {
		printf("Pass! No card was drawn from player's deck.\n");
	}
	else {
		printf("Fail! One card should be drawn. The player's deck has %d cards now, previously %d.\n", testG->deckCount[currentPlayer], tempDeckCount);
		failTest = 1;
	}

	//check if played card number increased by 2 (mine + trashed treasure) 
	if (testG->playedCardCount == (tempPlayedCount + 2)) {
		printf("Pass! The player discarded exactly two cards.\n");
	}
	else {
		printf("Fail! Two cards should be moved to played pile. There are %d played cards now, previously %d.\n", testG->playedCardCount, tempPlayedCount);
		failTest = 1;
	}

	//get new copper and silver card numbers
	int newCopper = 0, newSilver = 0;
	for (int i = 0; i < testG->handCount[currentPlayer]; i++) {
		if (testG->hand[currentPlayer][i] == copper) {
			newCopper++;
		}
	}
	for (int i = 0; i < testG->handCount[currentPlayer]; i++) {
		if (testG->hand[currentPlayer][i] == silver) {
			newSilver++;
		}
	}

	//check if player has one less copper card and one more silver card
	if (newCopper == (tempCopper - 1) && newSilver == (tempSilver + 1)) {
		printf("Pass! The player traded one copper for one silver.\n");
	}
	else {
		printf("Fail! Trade unsucessful, copper cards are %d VS %d, silver cards are %d VS %d.\n", newCopper, tempCopper, newSilver, tempSilver);
		failTest = 1;
	}

		
	// ----------- TEST 2: choice1 = copper and choice2 = gold --------------
	printf("\n*****TEST 2: choice1 = 4 (copper), choice 2 = 6 (gold)*****\n\n");

	//choose choice 1 and choice2
	choice1 = copper;
	choice2 = gold;

	struct gameState *testG2 = newGame();
	initializeGame(numPlayers, k, seed, testG2);
	
	//call test function and check return value
	if (mineEffect(currentPlayer, testG2, choice1, choice2, handPos) != -1) {
		printf("Failed! Return value should be -1 for this greedy choice!\n");
		failTest = 1;
	}
	else {
		printf("Pass! Return value is -1 for this greedy choice!\n");
	}


	// ----------- TEST 3: choice1 = copper and choice2 = --------------
	printf("\n*****TEST 3: choice1 = copper, choice 2 = 100 *****\n\n");

	//choose choice 1 and call cardEffect and check return value
	choice1 = copper;
	choice2 = 100;  //out of range

	struct gameState *testG3 = newGame();
	initializeGame(numPlayers, k, seed, testG3);

	//call test function and check return value
	if (mineEffect(currentPlayer, testG3, choice1, choice2, handPos) != -1) {
		printf("Failed! Return value should be -1 for out of range input!");
		failTest = 1;
	}
	else {
		printf("Pass! Return value is -1 for this invalid choice!\n");
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
	testMineEffect();
	return 0;
}