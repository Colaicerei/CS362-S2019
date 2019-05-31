#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "adventurer"

/*requirment for adventurer card: 
•	Current player should receive exactly 2 cards.
•	Player should receive no other type of cards, meaning the  2 cards are treasure cards.
•	The two cards should come from player’s own deck.
•	I am not sure if the card adventurer should be moved to the played card pile, since the source code did not have call discard() function, so I assume this is not the case for simplicity of testing.
•	No state change should occur for other players.
•	No state change should occur to the victory card piles and kingdom card piles.
*/

void testCardAdventurer() {

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
	int tempDeckCount = testG->deckCount[currentPlayer];

	// ----------- TEST 1: Top two cards are treasures --------------
	printf("\n*****TEST 1: Top two cards are treasures*****\n\n");
	
	testG->deck[currentPlayer][tempDeckCount - 1] = silver;
	testG->deck[currentPlayer][tempDeckCount - 2] = silver;

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

	//call cardEffect and check return value
	if (cardEffect(adventurer, 0, 0, 0, testG, handPos, &bonus) != 0) {
		printf("Failed! Return value is not zero!");
		failTest = 1;
	}

	//check if player has additional two cards on hand (two draws, no discard in the provided code) 
	if (testG->handCount[currentPlayer] == (tempHandCount + 2)) {
		printf("Pass! The player gained exactly 2 cards in total.\n");
	}
	else {
		printf("Fail! Two cards should be gained in total. The player has %d cards on hand now, previously %d.\n", testG->handCount[currentPlayer], tempHandCount);
		failTest = 1;
	}

	//check if two cards are drawn from player's deck 
	if (testG->deckCount[currentPlayer] == (tempDeckCount - 2)) {
		printf("Pass! Two cards were drawn from deck.\n");
	}
	else {
		printf("Fail! Two cards should be drawn. The player's deck has %d cards now, previously %d.\n", testG->deckCount[currentPlayer], tempDeckCount);
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



	// ----------- TEST 2: Bottom two cards are treasures --------------
	printf("\n*****TEST 2: Only Bottom two cards are treasures*****\n\n");

	struct gameState *testG2 = newGame();
	initializeGame(numPlayers, k, seed, testG2);

	testG2->deck[currentPlayer][0] = silver;
	testG2->deck[currentPlayer][1] = silver;

	for (int i = 2; i < tempDeckCount; i++) {
		testG2->deck[currentPlayer][i] = province;
	}

	tempHandCount = testG2->handCount[currentPlayer];
	tempDeckCount = testG2->deckCount[currentPlayer];

	//save number of treasure cards before calling function
	tempTreasure = 0;
	for (int i = 0; i < testG2->handCount[currentPlayer]; i++) {
		if (testG2->hand[currentPlayer][i] == copper ||
			testG2->hand[currentPlayer][i] == silver ||
			testG2->hand[currentPlayer][i] == gold)
		{
			tempTreasure++;
		}
	}

	//call cardEffect and check return value
	if (cardEffect(adventurer, 0, 0, 0, testG2, handPos, &bonus) != 0) {
		printf("Failed! Return value is not zero!");
		failTest = 1;
	}

	//check if player has additional two cards on hand (two draws, no discard in the provided code) 
	if (testG2->handCount[currentPlayer] == (tempHandCount + 2)) {
		printf("Pass! The player gained exactly 2 cards in total.\n");
	}
	else {
		printf("Fail! Two cards should be gained in total. The player has %d cards on hand now, previously %d.\n", testG2->handCount[currentPlayer], tempHandCount);
		failTest = 1;
	}

	//check if no card left at player's deck 
	if (testG2->deckCount[currentPlayer] == 0) {
		printf("Pass! No card left in player's deck.\n");
	}
	else {
		printf("Fail! Deck should be empty. The player's deck has %d cards now, previously %d.\n", testG2->deckCount[currentPlayer], tempDeckCount);
		failTest = 1;
	}

	//check if number of treasure cards increased by 2
	newTreasure = 0;
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



	// ----------- TEST 3: Only one card is treasure --------------
	printf("\n*****TEST 3: Only one treasure card available*****\n\n");

	struct gameState *testG3 = newGame();
	initializeGame(numPlayers, k, seed, testG3);

	testG3->deck[currentPlayer][0] = province;
	testG3->deck[currentPlayer][1] = silver;

	for (int i = 2; i < tempDeckCount; i++) {
		testG3->deck[currentPlayer][i] = province;
	}

	tempHandCount = testG3->handCount[currentPlayer];
	tempDeckCount = testG3->deckCount[currentPlayer];


	//save number of treasure cards before calling function
	tempTreasure = 0;
	for (int i = 0; i < testG3->handCount[currentPlayer]; i++) {
		if (testG3->hand[currentPlayer][i] == copper ||
			testG3->hand[currentPlayer][i] == silver ||
			testG3->hand[currentPlayer][i] == gold)
		{
			tempTreasure++;
		}
	}

	//call cardEffect and check return value
	if (cardEffect(adventurer, 0, 0, 0, testG3, handPos, &bonus) != 0) {
		printf("Failed! Return value is not zero!");
		failTest = 1;
	}

	//check if player has additional one card on hand (one draws, no discard in the provided code) 
	if (testG3->handCount[currentPlayer] == (tempHandCount + 1)) {
		printf("Pass! The player gained exactly 1 card in total.\n");
	}
	else {
		printf("Fail! Only one card should be gained in total. The player has %d cards on hand now, previously %d.\n", testG3->handCount[currentPlayer], tempHandCount);
		failTest = 1;
	}

	//check if no card left at player's deck since it will be draw through to dig the treasure
	if (testG2->deckCount[currentPlayer] == 0) {
		printf("Pass! No card left in player's deck.\n");
	}
	else {
		printf("Fail! Deck should be empty. The player's deck has %d cards now, previously %d.\n", testG2->deckCount[currentPlayer], tempDeckCount);
		failTest = 1;
	}

	//check if number of treasure cards increased by 2
	newTreasure = 0;
	for (int i = 0; i < testG->handCount[currentPlayer]; i++) {
		if (testG3->hand[currentPlayer][i] == copper ||
			testG3->hand[currentPlayer][i] == silver ||
			testG3->hand[currentPlayer][i] == gold)
		{
			newTreasure++;
		}
	}
	if (newTreasure == (tempTreasure + 1)) {
		printf("Pass! The player received one additional treasure card.\n");
	}
	else {
		printf("Fail! One treasure card should be added. There are %d treasures now, previously %d.\n", newTreasure, tempTreasure);
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
	testCardAdventurer();
	return 0;
}