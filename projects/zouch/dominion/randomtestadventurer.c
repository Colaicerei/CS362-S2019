#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>
#include <time.h>

/*requirment for adventurer effect: 
•	Current player should receive exactly 2 cards, 
•	Player should receive no other type of cards, which means 2 cards are treasure cards.
•	The two cards should come from player’s own deck.
•	1 card (adventurer) should be moved to the discard card pile??
•	No state change should occur for other players.
•	No state change should occur to the victory card piles and kingdom card piles.

*/

void assetTrue(int statement, char* passMessage, char* failMessage) {
	if (statement == 1) {
		printf(passMessage);
	}
	printf(failMessage);
}

int minTwo(int first, int second) {
	if (first <= second) {
		return first;
	}
	else {
		return second;
	}
}

void randomTestAdventurer() {

	int failTest = 0;
	int numPlayers;
	int seed = 1000;
	int currentPlayer = 0;
	int temphand[MAX_HAND];
	int drawntreasure = 0;
	int k[10] = { adventurer, council_room, feast, gardens, mine,
		remodel, smithy, village, baron, great_hall };
		
	struct gameState *testG = newGame();

	printf("\n---------- Random Test: Function AdventurerEffect() ----------\n\n");

	int n;
	for (n = 0; n < 1000; n++) {
		// set random player number to be 2 to 4
		numPlayers = rand() % 3 + 2;
		initializeGame(numPlayers, k, seed, testG);

		// set random deck count
		testG->deckCount[currentPlayer] = rand() % MAX_DECK;

		// set random order of treasure and non-treasure cards
		int treasureDeck = 0;
		int i;
		for (i = 0; i < testG->deckCount[currentPlayer]; i++)
		{
			if (rand() % 2 == 0) {
				testG->deck[currentPlayer][i] = estate;
			}
			else {
				testG->deck[currentPlayer][i] = copper;
				treasureDeck++;
			}
		}


		// start testing
		int tempHandCount = testG->handCount[currentPlayer];
		int tempDeckCount = testG->deckCount[currentPlayer];

		//save number of treasure cards before calling function
		int tempTreasure = 0;
		for (i = 0; i < testG->handCount[currentPlayer]; i++) {
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

		//check if player has additional up to 2 cards on hand (two draws if >= 2 treasure cards, as many as treasure cards if <2)
		if (testG->handCount[currentPlayer] != (tempHandCount + minTwo(treasureDeck, 2))) {
			printf("Fail! only %d treasure avilable to draw. The player has %d cards on hand now, previously %d.\n", minTwo(treasureDeck, 2), testG->handCount[currentPlayer], tempHandCount);
			failTest = 1;
		}


		//check if no less cards are drawn from player's deck than gained 
		if (testG->deckCount[currentPlayer] > (tempDeckCount - minTwo(treasureDeck, 2))) {
			printf("Fail! More than %d cards should be drawn. The player's deck has %d cards now, previously %d.\n", minTwo(treasureDeck, 2), testG->deckCount[currentPlayer], tempDeckCount);
			failTest = 1;
		}


		//check if discarded card and gained card matches drawn cards 
		if (testG->discardCount[currentPlayer] + minTwo(treasureDeck, 2) != (tempDeckCount - testG->deckCount[currentPlayer])) {
			printf("Fail! %d less cards drawn should be discarded. There are %d discarded cards but %d drawns.\n", minTwo(treasureDeck, 2), testG->discardCount[currentPlayer], tempDeckCount - testG->deckCount[currentPlayer]);
			failTest = 1;
		}

		//check if number of treasure cards increased by  minTwo(treasureDeck, 2)
		int newTreasure = 0;
		for (i = 0; i < testG->handCount[currentPlayer]; i++) {
			if (testG->hand[currentPlayer][i] == copper ||
				testG->hand[currentPlayer][i] == silver ||
				testG->hand[currentPlayer][i] == gold)
			{
				newTreasure++;
			}
		}
		if (newTreasure != (tempTreasure + minTwo(treasureDeck, 2))) {
			printf("Fail! %d treasure cards should be added. There are %d treasures now, previously %d.\n", minTwo(treasureDeck, 2), newTreasure, tempTreasure);
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
	randomTestAdventurer();
	return 0;
}