#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>

#define FILE_NAME "puzzle.txt"
// #define FILE_NAME "example.txt"
#define MAX_LINE_LEN 1000
#define MAX_RULE 10
#define MAX_WORKFLOWS 100000

#define MIN(a, b) ((a) < (b)? (a) : (b))
#define MAX(a, b) ((a) > (b)? (a) : (b))

void part1(FILE *fp);
void part2();

int main(void) {
        FILE *fp;

        fp = fopen(FILE_NAME, "r");
        if (fp == NULL) {
                perror("fopen");
                exit(1);
        }
        part1(fp);
        fclose(fp);

        part2();

        return 0;
}

// from http://www.cse.yorku.ca/~oz/hash.html
uint32_t hash(char *str) {
        uint32_t hash = 5381;
        int c;

        while ((c = *str++))
                hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

        return hash;
}

typedef int (*comp_func_t)(uint32_t, uint32_t);
int comp_lt(uint32_t a, uint32_t b) {return a < b;}
int comp_gt(uint32_t a, uint32_t b) {return a > b;}
int comp_no(uint32_t a, uint32_t b) {return 1;} // don't compare

typedef enum {
        COOL = 0,
        MUSIC = 1,
        AERO = 2,
        SHINY = 3,
} rating_t;

const rating_t str_to_rating[] = {
        ['x'] = COOL,
        ['m'] = MUSIC,
        ['a'] = AERO,
        ['s'] = SHINY,
};

typedef struct workflow_t workflow_t;

typedef struct {
        rating_t rate;
        comp_func_t comp_f;
        uint32_t v;
        workflow_t * next;
} rule_t;

struct workflow_t {
        uint32_t rules_len;
        rule_t rules[MAX_RULE];
};

workflow_t workflow_map[MAX_WORKFLOWS] = { 0 };

workflow_t * ACCEPTED = &workflow_map[0];
workflow_t * REJECTED = &workflow_map[1];

int rule_test(const rule_t * rule, uint32_t part[4]) {
        if (rule->comp_f(
                part[rule->rate],
                rule->v)) {
                if (rule->next == ACCEPTED) return 1;
                if (rule->next == REJECTED) return 0;
                return 2; // jump to the workflow specified the rule
        }
        return 3; // go to the next step/rule of the current workflow
}

int workflow_apply(workflow_t * workflow, uint32_t part[4]) {
        for (uint32_t i = 0; i < workflow->rules_len; i++) {
                rule_t * rule = &workflow->rules[i];
                int test = rule_test(rule, part);
                switch (test) {
                case 0: // fallthrough
                case 1: return test;
                case 2: return workflow_apply(rule->next, part);
                }
        }
        // the last rule in a workflow will always return of jump.
        assert (0);
        return -1;
}

workflow_t * workflow_in = NULL;
void part1(FILE *fp) {
        char line_buf[MAX_LINE_LEN];
        char *line = line_buf;


        // First get all workflows and insert them in the map
        while (fgets(line, MAX_LINE_LEN, fp) != NULL) {
                if (line[0] == '\n') break;

                char * tok;
                int workflow_i;
                rule_t rule = { 0 };
                workflow_t workflow = (workflow_t) {
                        .rules_len = 0,
                };
                
                tok = strsep(&line, "{");
                
                workflow_i = hash(tok) % MAX_WORKFLOWS;
                if (strcmp(tok, "in") == 0) workflow_in = workflow_map + workflow_i;


                assert(workflow_map + workflow_i != ACCEPTED);
                assert(workflow_map + workflow_i != REJECTED);
                assert(workflow_map[workflow_i].rules_len == 0);

                while ((tok = strsep(&line, ",")) != NULL) {
                        // s<1142:vgz || A || R || lbl
                        int nocmp = strchr(tok, ':') == NULL;
                        if (!nocmp) {
                                rule.rate = str_to_rating[(int)tok[0]];
                                rule.comp_f = (tok[1] == '<')?
                                        comp_lt : comp_gt;
                                tok += 2;
                                // Disgusting ...
                                rule.v = atoi(strsep(&tok, ":"));
                        } else {
                                rule.rate = 0; // NA
                                rule.comp_f = comp_no;
                        }
                        
                        if (strchr(tok, '}')) *strchr(tok, '}') = '\0';
                        if (tok[0] == 'A') {
                                rule.next = ACCEPTED;
                        } else if (tok[0] == 'R') {
                                rule.next = REJECTED;
                        } else {
                                rule.next = workflow_map + (hash(tok)%MAX_WORKFLOWS);
                        }

                        assert(workflow.rules_len < MAX_RULE);
                        workflow.rules[workflow.rules_len++] = rule;
                }

                workflow_map[workflow_i] = workflow;
                line = line_buf;
        }

        assert (workflow_in);
        
        int solution = 0;
        while (fgets(line, MAX_LINE_LEN, fp) != NULL) {
                uint32_t part[4];
                
                line++;
                *strchr(line, '}')  = '\0';
                for (int i = 0; i < 4; i++) part[i] = atoi(strsep(&line, ",") + 2); // Actual programming warcrime

                if (workflow_apply(workflow_in, part)) {
                        solution += part[0];
                        solution += part[1];
                        solution += part[2];
                        solution += part[3];
                }
                line = line_buf;
        }

        printf("Solution 1: %d\n", solution);
}

// recursive function.
// for each rule in the workflow, test each branch and return the number of accepted possibilities
uint64_t count_valid(workflow_t * workflow,
                uint32_t start_i[4], uint32_t end_i[4]) {
        
        // make a copy of the arrays, they each live on the stack
        // so that further recursive call don't change the value of the current call
        uint32_t start[4], end[4];
        uint32_t start_cpy[4], end_cpy[4];
        memcpy(start, start_i, sizeof(uint32_t) * 4);
        memcpy(end, end_i, sizeof(uint32_t) * 4);

        printf("Recursion with [ x={%d - %d}, m={%d - %d}, a={%d - %d}, s={%d - %d}]\n", start[0], end[0], start[1], end[1], start[2], end[2], start[3], end[3]);

        uint64_t valid = 0;
        for (uint32_t i = 0; i < workflow->rules_len; i++) {
                rule_t rule = workflow->rules[i];

                if (rule.comp_f == comp_gt) {
                        if (rule.next == REJECTED) goto skip1;
                        memcpy(start_cpy, start, sizeof(uint32_t) * 4);

                        memcpy(end_cpy, end, sizeof(uint32_t) * 4);
                        start_cpy[rule.rate] = MAX(start_cpy[rule.rate], rule.v+1);
                        end_cpy[rule.rate] = MAX(end_cpy[rule.rate], rule.v+1);

                        if (rule.next == ACCEPTED) {
                                uint64_t pls = 1;
                                for (int j = 0; j < 4; j++) {
                                        assert (end_cpy[j] - start_cpy[j] >= 0);
                                        pls *= end_cpy[j] - start_cpy[j] + 1;
                                }
                                valid += pls;
                        } else {
                                valid += count_valid(rule.next, start_cpy, end_cpy);
                        }
skip1:
                        // those who fail the condition continue in the workflow
                        end[rule.rate] = MIN(end[rule.rate], rule.v);
                        start[rule.rate] = MIN(start[rule.rate], rule.v);
                } else if (rule.comp_f == comp_lt) {
                        if (rule.next == REJECTED) goto skip2;
                        memcpy(start_cpy, start, sizeof(uint32_t) * 4);
                        memcpy(end_cpy, end, sizeof(uint32_t) * 4);
                        end_cpy[rule.rate] = MIN(end_cpy[rule.rate], rule.v-1);
                        start_cpy[rule.rate] = MIN(start_cpy[rule.rate], rule.v-1);

                        if (rule.next == ACCEPTED) {
                                uint64_t pls = 1;
                                for (int j = 0; j < 4; j++) {
                                        assert (end_cpy[j] - start_cpy[j] >= 0);
                                        pls *= end_cpy[j] - start_cpy[j] + 1;
                                }
                                valid += pls;
                        } else {
                                valid += count_valid(rule.next, start_cpy, end_cpy);
                        }
skip2:
                        // those who fail the condition continue in the workflow
                        end[rule.rate] = MAX(end[rule.rate], rule.v);
                        start[rule.rate] = MAX(start[rule.rate], rule.v);
                } else if (rule.comp_f == comp_no) {
                        if (rule.next == ACCEPTED) {
                                uint64_t pls = 1;
                                for (int j = 0; j < 4; j++) {
                                        assert (end[j] - start[j] >= 0);
                                        pls *= end[j] - start[j] + 1;
                                }
                                return valid + pls;
                        } else if (rule.next == REJECTED) {
                                return valid;
                        } else {
                                return valid + count_valid(rule.next, start, end);
                        }
                } else assert(0);
        }
        assert(0);
        return -1;
}

void part2() {

        // Part 1 already have the workflows in the hashmap
        uint32_t start[4] = {1, 1, 1, 1};
        uint32_t end[4] = {4000, 4000, 4000, 4000};
        uint64_t solution = count_valid(workflow_in, start, end);
        printf("Solution 2: %li\n", solution);
}
