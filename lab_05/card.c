// ----------------------------------------------------------------------
// file: card.c
//
// Description: This file implements the CARD module. Its job is to
//     create an interface for providing cards from a shuffled deck of
//     52 standing playing cards. Each call to card_get() will return
//     the top card in that shuffled deck. If all the cards get used,
//     then the deck is invisibly (and unknowingly) reshuffled.
//
// Created: 2016-05-03 (P. Clark)
//
// Modifications:
// 2017-10-30 (P. Clark)
//     Added card_init().
// 2017-11-8 (A.Hardt)
//     Updated card_get to deal 52 cards
// ----------------------------------------------------------------------
#include <stdlib.h>
#include <sys/times.h>
#include "card.h"
#include "common.h"
#include <stdbool.h>

#define CARDS_IN_DECK 52
#define AVAIL 0          // avaiable, not_dealt
#define USED 1           // dealt
#define DECK_DECODE 100  // encoding scheme of card deck relies on a / or % of 100

static unsigned int Deck_shuffled = FALSE;


// This function must be called before the first call to card_get().
extern void card_init(void)
{
        // initialize the random number generator
        srandom(times(NULL));
}



// Get a card from the current deck.
// suit: This is interpreted as follows:
//     1 = Clubs
//     2 = Hearts
//     3 = Spades
//     4 = Diamonds
// pattern: This is interpreted as the
//     1 = Ace
//     2..10 as expected
//     11 = Jack
//     12 = Queen
//     13 = King
extern void card_get(unsigned char *suit, unsigned char *pattern)
{
        int random_card_num;
        static int numCardsDealt = 0;

        // card values:
            //suite = elment/100, pattern = element%100
        int cards[CARDS_IN_DECK] =
                {101,102,103,104,105,106,107,108,109,110,111,112,113,
                 201,202,203,204,205,206,207,208,209,210,211,212,213,
                 301,302,303,304,305,306,307,308,309,310,311,312,313,
                 401,402,403,404,405,406,407,408,409,410,411,412,413};

        // no cards have been dealt, all are avaiable
        static int cardDealt[CARDS_IN_DECK] =
                { AVAIL, AVAIL, AVAIL, AVAIL, AVAIL, AVAIL, AVAIL, AVAIL, AVAIL, AVAIL, AVAIL, AVAIL, AVAIL,
                  AVAIL, AVAIL, AVAIL, AVAIL, AVAIL, AVAIL, AVAIL, AVAIL, AVAIL, AVAIL, AVAIL, AVAIL, AVAIL,
                  AVAIL, AVAIL, AVAIL, AVAIL, AVAIL, AVAIL, AVAIL, AVAIL, AVAIL, AVAIL, AVAIL, AVAIL, AVAIL,
                  AVAIL, AVAIL, AVAIL, AVAIL, AVAIL, AVAIL, AVAIL, AVAIL, AVAIL, AVAIL, AVAIL, AVAIL, AVAIL};

        // if all 52 cards have been dealt...
        if (!Deck_shuffled) {
                // set all cardDealt values to False,
                // indicating no cards have been dealt
                for(int i = 0; i < CARDS_IN_DECK; i++){
                    cardDealt[i] = AVAIL;
                }
                // set suffled status to true now that deck has been shufffled
                numCardsDealt = 0;
                Deck_shuffled = TRUE;
        }

        //assign random number to give out
        random_card_num = (random() % CARDS_IN_DECK);

        // if random_card_num has already been dealt, pick a new one
        while((cardDealt[random_card_num]) & (numCardsDealt < CARDS_IN_DECK)){
                random_card_num = (random() % CARDS_IN_DECK);
        }

        // Mark that the card has been dealt
        cardDealt[random_card_num] = USED;

        // Increment count of number of cards dealt
        numCardsDealt = numCardsDealt+1;

        // if we have dealt 52 cards, set shuffled status to false to reshuffle deck upon next request to play
        if (numCardsDealt == CARDS_IN_DECK){
            Deck_shuffled = FALSE;
        }

        // Assign suit and pattern using random number
        *suit    = (cards[random_card_num]/DECK_DECODE);
        *pattern = (cards[random_card_num]%DECK_DECODE);


} // card_get()
