#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

/*requirment for adventurer effect: 
•	Current player should receive exactly 2 cards, 
•	Player should receive no other type of cards, which means 2 cards are treasure cards.
•	The two cards should come from player’s own deck.
•	1 card (adventurer) should be moved to the discard card pile??
•	No state change should occur for other players.
•	No state change should occur to the victory card piles and kingdom card piles.

*/

void testAdventurerEffect() {

	int failTest = 0;
	int numPlayers = 4;
	int seed = 1000;
	int currentPlayer = 0;
	int temphand[MAX_HAND];
	int drawntreasure = 0;
	int k[10] = { adventurer, council_room, feast, gardens, mine,
		remodel, smithy, village, baron, great_hall };
		
	struct gameState *testG = newGame();

	initializeGame(numPlayers, k, seed, testG);

	printf("\n---------- Unittest 1: Function AdventurerEffect() ----------\n\n");

	int tempHandCount = testG->handCount[currentPlayer];
	int tempDeckCount = testG->deckCount[currentPlayer];

	//save number of treasure cards before calling function
	int tempTreasure = 0; 
	for (int i = 0; i < testG->handCount[currentPlayer]; i++) {
		if (testG->hand[currentPlayer][i] == copper ||
			testG->hand[currentPlayer][i] == silver ||
			testG->hand[currentPlayer][i] == gold)
		{
			tempTreasure++;
		}
	}
	
	//call test function and check return value
	if (adventurerEffect(drawntreasure, currentPlayer, testG, temphand) != 0) {
		printf("Failed! Return value is not zero!");
		failTest = 1;
	}

	//check if player has additional tw2 cards on hand (two draws, no discard in the provided code) 
	if (testG->handCount[currentPlayer] == (tempHandCount + 2)) {
		printf("Pass! The player gained exactly 2 cards in total.\n");
	}
	else {
		printf("Fail! Two cards should be gained in total. The player has %d cards on hand now, previously %d.\n", testG->handCount[currentPlayer], tempHandCount);
		failTest = 1;
	}

	//check if >= two cards are drawn from player's deck 
	if (testG->deckCount[currentPlayer] <= (tempDeckCount - 2)) {
		printf("Pass! More than two cards were drawn from deck.\n");
	}
	else {
		printf("Fail! More than two cards should be drawn. The player's deck has %d cards now, previously %d.\n", testG->deckCount[currentPlayer], tempDeckCount);
		failTest = 1;
	}

	//check if discarded card and gained card matches drawn cards 
	if (testG->discardCount[currentPlayer] + 2 == (tempDeckCount - testG->deckCount[currentPlayer])) {
		printf("Pass! The player discarded all other drawn cards than treasures.\n");
	}
	else {
		printf("Fail! Two less cards drawn should be discarded. There are %d discarded cards but %d drawns.\n", testG->discardCount[currentPlayer], tempDeckCount - testG->deckCount[currentPlayer]);
		failTest = 1;
	}

	//check if number of treasure cards increased by 2
	int newTreasure = 0;
	for (int i = 0; i < testG->handCount[currentPlayer]; i++) {
		if (testG->hand[currentPlayer][i] == copper ||
			testG->hand[currentPlayer][i] == silver ||
			testG->hand[currentPlayer][i] == gold)
		{
			newTreasure++;
		}
	}
	if (newTreasure == (tempTreasure + 2)) {
		printf("Pass! The player received two additional treasure cards.\n");
	}
	else {
		printf("Fail! Two treasure cards should be added. There are %d treasures now, previously %d.\n", newTreasure, tempTreasure);
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
	testAdventurerEffect();
	return 0;
}