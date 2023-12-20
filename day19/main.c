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

void part1(FILE *fp);
void part2(FILE *fp);

int main(void) {
        FILE *fp;

        fp = fopen(FILE_NAME, "r");
        if (fp == NULL) {
                perror("fopen");
                exit(1);
        }
        part1(fp);
        fclose(fp);


        fp = fopen(FILE_NAME, "r");
        if (fp == NULL) {
                perror("fopen");
                exit(1);
        }
        part2(fp);
        fclose(fp);

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


void part1(FILE *fp) {
        char line_buf[MAX_LINE_LEN];
        char *line = line_buf;
        workflow_t * workflow_in = NULL;


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
void part2(FILE *fp) {
        char line_buf[MAX_LINE_LEN];
        char *line = line_buf;


        while (fgets(line, MAX_LINE_LEN, fp) != NULL) {
                line = line_buf;
        }

        int solution = 0;
        printf("Solution 2: %d\n", solution);
}
