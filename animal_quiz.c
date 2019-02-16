#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "mystringutils.c"
#define STDIO_INCLUDED

static char* THINK_ABOUT_AN_ANIMAL_MESSAGE = "think about an animal";
static char* WELCOME_MESSAGE = "welcome";

#define IS_LEAF 1
#define IS_NON_LEAF 0

typedef enum {THINK_ABOUT_AN_ANIMAL_STATE,
    GUESSING_STATE,
    CHECKING_GUESS_IN_LEAF_NODE_STATE,
    CHECKING_GUESS_IN_NON_LEAF_NODE_STATE,
    GETTING_ANIMAL_NAME_STATE,
    GETTING_DISCRIMINATING_QUESTION,
    GETTING_ANSWER_TO_DISCRIMINATING_QUESTION,
    } State;

typedef struct Knowledge_tree {
    int leaf_or_not_leaf;
    char * animal;
    char * discriminating_question;
    struct Knowledge_tree * yes_branch;
    struct Knowledge_tree * no_branch;
} Knowledge_tree;


typedef struct Model {
    char* message_from_engine;
    char* animal_to_be_learned;
    char* discriminating_question_for_learning;
    State state;
    Knowledge_tree* knowledge_tree;
    Knowledge_tree* current_node;
    Knowledge_tree** father_of_current_node;

    // Str_list* yes_no_list;
} Model;

Model* get_initial_model();
Knowledge_tree* get_initial_tree();
void update_model(Model* model,char * user_input);
Knowledge_tree* make_leaf_node(char* animal);
Knowledge_tree* make_non_leaf_node(char* discriminating_question,Knowledge_tree* yes_branch,Knowledge_tree* no_branch);

#ifndef UNIT_TESTING
int main(int argc, char **argv) {
    char user_input[97];
    Model * model = get_initial_model();
    while (1) {
        printf("%s\n",model->message_from_engine);
        get_user_input(user_input);

        update_model(model,user_input);
    }
    return 0;
}
#endif 

void update_model(Model* model,char *user_input) {
    switch (model->state) {
        case THINK_ABOUT_AN_ANIMAL_STATE:
            free(model->message_from_engine);
            model->message_from_engine = concatenate_strings(1,THINK_ABOUT_AN_ANIMAL_MESSAGE);
            model->current_node = model->knowledge_tree;
            model->state=GUESSING_STATE;
            break;
        case GUESSING_STATE:
            if (((model->current_node))->leaf_or_not_leaf==IS_LEAF) {
                free(model->message_from_engine);
                model->message_from_engine = concatenate_strings(3,"is it a ",(model->current_node)->animal,"?");
                model->state = CHECKING_GUESS_IN_LEAF_NODE_STATE;
            } else {
                model->message_from_engine = concatenate_strings(1,model->current_node->discriminating_question);
                model->state = CHECKING_GUESS_IN_NON_LEAF_NODE_STATE;
            }
            break;
        case CHECKING_GUESS_IN_LEAF_NODE_STATE:
            if (strcmp("yes",user_input)==0) {
                free(model->message_from_engine);
                model->message_from_engine = concatenate_strings(1,"yeah");
                model->state=THINK_ABOUT_AN_ANIMAL_STATE;
            } else if (strcmp("no",user_input)==0) {
                free(model->message_from_engine);
                model->message_from_engine = concatenate_strings(1,"what animal was?");
                model->state = GETTING_ANIMAL_NAME_STATE;
            }
            break;

        case CHECKING_GUESS_IN_NON_LEAF_NODE_STATE:
            if (strcmp("yes",user_input)==0) {
                model->father_of_current_node = &model->current_node;
                model->current_node= (model->current_node)->yes_branch;
            } else if (strcmp("no",user_input)==0) {
                model->father_of_current_node = &model->current_node; 
                model->current_node= (model->current_node)->no_branch;
            }

           if (((model->current_node))->leaf_or_not_leaf==IS_LEAF) {
                free(model->message_from_engine);
                model->message_from_engine = concatenate_strings(3,"is it a ",(model->current_node)->animal,"?");
                model->state = CHECKING_GUESS_IN_LEAF_NODE_STATE;
            } else {
                model->message_from_engine = concatenate_strings(1,model->current_node->discriminating_question);
                model->state = CHECKING_GUESS_IN_NON_LEAF_NODE_STATE;
            } 
            break;

        case GETTING_ANIMAL_NAME_STATE:
            if (strcmp("",user_input)==0) {
                break;
            }
            model->animal_to_be_learned = concatenate_strings(1,user_input);
            model->message_from_engine= concatenate_strings(5,"what is the question to distinguish a ",
                   model->animal_to_be_learned," from a ",
                   (model->current_node)->animal,"?");
            model->state = GETTING_DISCRIMINATING_QUESTION;
            break;
        case GETTING_DISCRIMINATING_QUESTION:
            if (user_input==NULL||strcmp("",user_input)==0) {
                break;
            }
            char* discriminating_question_for_learning = concatenate_strings(1,user_input);
            model->discriminating_question_for_learning = discriminating_question_for_learning;
            model->message_from_engine=concatenate_strings(7,"what is the answer to the question '",
                discriminating_question_for_learning,
                "' to distinguish a ",
                model->animal_to_be_learned,
                " from a ",
                (model->current_node)->animal,"?"  );
            model->state = GETTING_ANSWER_TO_DISCRIMINATING_QUESTION;
            break;
        case GETTING_ANSWER_TO_DISCRIMINATING_QUESTION:
            if ((strcmp("no",user_input)!=0)&&(strcmp("yes",user_input)!=0)) {
                break;
            }

            if (strcmp("no",user_input)==0) {
                Knowledge_tree* no_branch = make_leaf_node(model->animal_to_be_learned);

                Knowledge_tree* yes_branch = make_leaf_node((model->current_node)->animal);
                free(model->current_node);

                Knowledge_tree* new_sub_tree = make_non_leaf_node(model->discriminating_question_for_learning,
                    yes_branch,no_branch);
                **model->father_of_current_node = *new_sub_tree;
            }

            if (strcmp("yes",user_input)==0) {
                Knowledge_tree* yes_branch = make_leaf_node(model->animal_to_be_learned);

                Knowledge_tree* no_branch = make_leaf_node((model->current_node)->animal);
                free(model->current_node);

                Knowledge_tree* new_sub_tree = make_non_leaf_node(model->discriminating_question_for_learning,
                    yes_branch,no_branch);
                **model->father_of_current_node = *new_sub_tree;
            }

            model->message_from_engine=concatenate_strings(1,"let's start again. Think about an animal");
            model->current_node = model->knowledge_tree;
            model->state = GUESSING_STATE;
            break;
    }
}

Model* get_initial_model() {
    Model* to_return = malloc(sizeof(Model));
    to_return->message_from_engine = malloc(strlen(WELCOME_MESSAGE));
    strcpy(to_return->message_from_engine,WELCOME_MESSAGE);
    to_return->state=THINK_ABOUT_AN_ANIMAL_STATE;
    to_return->animal_to_be_learned="";
    to_return->knowledge_tree=get_initial_tree();
    to_return->current_node=to_return->knowledge_tree;
    to_return->father_of_current_node = &to_return->knowledge_tree;
    return to_return;
}

Knowledge_tree* get_initial_tree() {
    return make_leaf_node("elephant");
}

Knowledge_tree* make_leaf_node(char* animal) {
    Knowledge_tree* to_return = malloc(sizeof(Knowledge_tree));
    to_return->leaf_or_not_leaf=IS_LEAF;
    to_return->animal = concatenate_strings(1,animal);
    return to_return;
}

Knowledge_tree* make_non_leaf_node(char* discriminating_question,Knowledge_tree* yes_branch,Knowledge_tree* no_branch) {
    Knowledge_tree* to_return = malloc(sizeof(Knowledge_tree));
    to_return->leaf_or_not_leaf=IS_NON_LEAF;
    to_return->discriminating_question=concatenate_strings(1,discriminating_question);
    to_return->no_branch=no_branch;
    to_return->yes_branch=yes_branch;
    return to_return;
}




