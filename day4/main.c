#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LEN 1000
#define MAX_LINES 500
#define MAX_CARD 50

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


        fp = fopen(argv[1], "r");
        if (fp == NULL) {
                perror("fopen");
                exit(1);
        }
        part2(fp);
        fclose(fp); 

        return 0;
}

int present_int(int *array, int n, int v) {
        for (int i = 0; i < n; i++) {
                if (array[i] == v) {
                        return 1;
                }
        }
        return 0;
}

void part1(FILE *fp) {
        char line_buf[MAX_LINE_LEN+1],
            *line = line_buf,
            *tok;
        int winning_card[MAX_CARD],
            sum, winning_card_size, value;

        sum = 0;
        while (fgets(line_buf, MAX_LINE_LEN, fp) != NULL) {
                (void) strsep(&line, ":");

                winning_card_size = 0;
                while ((tok = strsep(&line, " ")) != NULL) {
                        if (tok[0] == '\0') continue;
                        if (tok[0] == '|') break;
                        int tmp;
                        sscanf(tok, "%d", &tmp);
                        winning_card[winning_card_size++] = tmp;
                }

                value = 0;
                while ((tok = strsep(&line, " ")) != NULL) {
                        if (tok[0] == '\0' ) continue;
                        if (tok[0] == '\n') break;
                        int tmp;
                        sscanf(tok, "%d", &tmp);
                        if (present_int(winning_card, 
                                        winning_card_size, 
                                        tmp)) {
                                printf("%d\n", tmp);
                                value = (!value)?1:value*2;
                        }
                }
                sum += value;
                line = line_buf;
        }
        printf("Sum=%d\n", sum);
}

void part2(FILE *fp) {
        char line_buf[MAX_LINE_LEN+1],
            *line = line_buf,
            *tok;

        int cards_cnt[MAX_LINES+1] = {[0 ... MAX_LINES] = 1},
            cards_cnt_size = 0,
            win_cards[MAX_CARD],
            win_cards_size,
            copies, sum;
        
        while (fgets(line_buf, MAX_LINE_LEN, fp) != NULL) {
                (void) strsep(&line, ":");
                
                win_cards_size = 0;
                while ((tok = strsep(&line, " ")) != NULL) {
                        if (tok[0] == '\0') continue;
                        if (tok[0] == '|') break;
                        int tmp;
                        sscanf(tok, "%d", &tmp);
                        win_cards[win_cards_size++] = tmp;
                }
                
                copies = 0;
                while ((tok = strsep(&line, " ")) != NULL) {
                        if (tok[0] == '\0' ) continue;
                        if (tok[0] == '\n') break;
                        int tmp;
                        sscanf(tok, "%d", &tmp);
                        copies += (present_int(win_cards,
                                        win_cards_size,
                                        tmp));
                }
                
                for (int i = 0; i < copies; i++)
                        cards_cnt[cards_cnt_size+i+1] += cards_cnt[cards_cnt_size];

                cards_cnt_size++;
                line = line_buf;
        }

        sum = 0;
        for (int i = 0; i < cards_cnt_size; i++) {
                sum += cards_cnt[i];
        }

        printf("scratch count = %d\n", sum);
}
