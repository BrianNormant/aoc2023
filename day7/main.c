#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define MAX_GAMES 1000

void part1(FILE *fp);
void part2(FILE *fp);

int main(int argc, char *argv[]) {
        if (argc!= 2) {
                fprintf(stderr, "Usage: %s <input>\n", argv[0]);
                exit(1);
        }
        FILE *fp;

        fp = fopen(argv[1], "r");
        if (fp == NULL) {
                perror("fopen");
                exit(1);
        }
        part1(fp);
        fclose(fp);


        // fp = fopen(argv[1], "r");
        // if (fp == NULL) {
        //         perror("fopen");
        //         exit(1);
        // }
        // part2(fp);
        // fclose(fp);

        return 0;
}

const int PRIMES[] = {
        2,3,5,7,11,13,17,19,23,29,31,37,41
};

const int CARD_WEIGHT[] = {
        ['J'] = 2,
        ['2'] = 3,
        ['3'] = 5,
        ['4'] = 7,
        ['5'] = 11,
        ['6'] = 13,
        ['7'] = 17,
        ['8'] = 19,
        ['9'] = 23,
        ['T'] = 29,
        ['Q'] = 31,
        ['K'] = 37,
        ['A'] = 41,
};
const char CARD_WEIGHTR[] = {
        [2]  = 'J',
        [3]  = '2',
        [5]  = '3',
        [7]  = '4',
        [11] = '5',
        [13] = '6',
        [17] = '7',
        [19] = '8',
        [23] = '9',
        [29] = 'T',
        [31] = 'Q',
        [37] = 'K',
        [41] = 'A',
};
#define JOKER_POS 0

void hand_parse(const char * restrict s, int * restrict hand) {
        for (int i = 0; i < 5; i++) {
                hand[i] = CARD_WEIGHT[(int)s[i]];
        }
}

typedef enum type {
        FIVEoK = 6,
        FOURoK = 5,
        FULLh  = 4,
        THREEoK= 3,
        TWOp   = 2,
        ONEp   = 1,
        HIGHc  = 0,
} type_t;

typedef struct {
        int bid;
        int cards[5];
} hand_t;        

int cmp_int(const void * a, const void * b) {
        if (*((int *)a) == *((int *)b)) return 0;
        if (*((int *)a)  < *((int *)b)) return 1;
        return -1;
}

void print_array(const int * arr, int size) {
        printf("[");
        for (int i = 0; i < size; i++) {
                printf("%d ,", arr[i]);
        }
        printf("]");
}

void prime_factors(int n, int * restrict factors) {
        memset(factors, 0, sizeof(int) * 13);
        int nb_jok = 0, tot = 0;
        for (int i = 0; i < 13; i++) {
                while (n % PRIMES[i] == 0) {
                        if (i == JOKER_POS) nb_jok++;
                        else factors[i]++;
                        n /= PRIMES[i];
                        tot++;
                }
                if (tot == 5) break;
        }
        qsort(factors, 13, sizeof(int), cmp_int);
        // print_array(factors, 13);
        // printf(" nb_jok = %d tot = %d\n", nb_jok, tot);

        assert(tot == 5);
        if (nb_jok == 0) return;
        if (nb_jok == 5) {
                factors[13-1] = 5;
                return;
        }
        for (int i = 0; i < 13; i++) {
                if (factors[i] != 0) {
                        factors[i] += nb_jok;
                        break;
                }
        }
}


type_t hand_type(const int * hand) {
        int factors[13];
        int product = 1;
        for (int i= 0; i < 5; i++) product *= hand[i];
        prime_factors(product, factors);
        int tree = 0, two = 0;
        for (int i = 0; i < 13; i++) {
                if (tree && factors[i] == 2) return FULLh;
                if (two && factors[i] == 2) return TWOp;
                if (factors[i] == 5) return FIVEoK;
                if (factors[i] == 4) return FOURoK;
                if (factors[i] == 3) tree = 1;
                if (factors[i] == 2) two = 1;
        }
        if (tree) return THREEoK;
        if (two) return ONEp;
        return HIGHc;
}

int card_cmp(const int c1, const int c2) {
        if (c1 == c2) return 0;
        if (c1 > c2) return 1;
        return -1;
}

void hand_print(const hand_t * hand) {
        for (int i = 0; i < 5; i++) {
                printf("%c", CARD_WEIGHTR[hand->cards[i]]);
        }

        switch(hand_type(hand->cards)) {
                case FIVEoK : printf(" Five of Kind"); break;
                case FOURoK : printf(" Four of Kind"); break;
                case FULLh  : printf(" Full House"); break;
                case THREEoK: printf(" Three of kind"); break;
                case TWOp   : printf(" Two pair"); break;
                case ONEp   : printf(" One pair"); break;
                case HIGHc  : printf(" High Card"); break;

        };

        printf(" : %d", hand->bid);
}

int hand_cmp(const void * restrict h1, const void * restrict h2) {
        const int * hand2 = ((hand_t *)h2)->cards;
        const int * hand1 = ((hand_t *)h1)->cards;
        type_t h1_t, h2_t;
        // puts("hand_cmp :");
        // hand_print(h1); putc('\n', stdout);
        h1_t = hand_type(hand1);
        // hand_print(h2); putc('\n', stdout);
        h2_t = hand_type(hand2);

        if (h1_t == h2_t) {
                for (int i = 0; i < 5; i++) {
                        if (card_cmp(hand1[i], hand2[i]) != 0)
                                return card_cmp(hand1[i], hand2[i]);
                }
                return 0;
        }
        if (h1_t > h2_t) return 1;
        return -1;
}


void part1(FILE *fp) {
        char line_buf[1024+1],
            *line = line_buf;
        
        hand_t games[MAX_GAMES];

        int game_cnt = 0;
        while (fgets(line, 1024, fp) != NULL) {
                char play[6];
                
                hand_t hand;
                sscanf(line, "%5s %d", play, &hand.bid);
                hand_parse(play, hand.cards);
                
                assert(game_cnt < MAX_GAMES);
                games[game_cnt++] = hand;
        }

        

        qsort(games, game_cnt, sizeof(hand_t), hand_cmp);
        
        for (int i = 0; i < game_cnt; i++) {
                printf("Rank %d -> ", i + 1);
                hand_print(games+i);
                printf("\n");
        }

        long sum = 0;
        for (int i = 0; i < game_cnt; i++) {
                sum += (i+1) * games[i].bid;
        }
        printf("sum = %ld\n", sum);
}
