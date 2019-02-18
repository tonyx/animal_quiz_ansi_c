#define UNIT_TESTING

#include "minunit.h"
#include "animal_quiz.c"

#ifndef STDIO_INCLUDED 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "myutils.c"
#endif 
#define STDIO_INCLUDED

int tests_run = 0;

static char * test_foo() {
    mu_assert("one is not one",1==1);
    return 0;
}


static char * initial_message_should_be_welcome() {
    Model* model = get_initial_model();
    mu_assert(concatenate_strings(2,"initial message is not 'welcome' but rather is ",
        model->message_from_engine),strcmp(model->message_from_engine,"welcome")==0);
    return 0;
}

static char * initial_state_should_be_THINK_ABOUT_AN_ANIMAL() {
    Model* model = get_initial_model();
    mu_assert("initial state is not THINK ABOUT AN ANIMAL",model->state == THINK_ABOUT_AN_ANIMAL_STATE);
    update_model(model,"");
    mu_assert(concatenate_strings(2,"message in state THINK_ABOUT_AN_ANMAL is not 'think about an animal' but rathr is ", model->message_from_engine),
        strcmp(model->message_from_engine,THINK_ABOUT_AN_ANIMAL_MESSAGE)==0);
    return 0;
}


static char * after_first_update_state_will_be_GUESSING() {
    Model* model = get_initial_model();
    update_model(model,"");
    mu_assert("next state is not GUESSING",model->state == GUESSING_STATE);
    update_model(model,"");
    mu_assert("is it a elephant",strcmp(model->message_from_engine,"is it a elephant?")==0);
    return 0;
}

static char * when_guess_is_correct_then_the_message_will_be_yeah() {
    Model* model = get_initial_model();

    model->state = GUESSING_STATE;
    update_model(model,"");
    mu_assert("next state is not CHECKING_IF_GUESS_IS_CORRECT",model->state == CHECKING_GUESS_IN_LEAF_NODE_STATE);
    update_model(model,"yes");
    mu_assert("yeah",strcmp(model->message_from_engine,"yeah")==0);
    return 0;
}

static char * when_guess_is_not_correct_then_the_state_will_be_ask_what_animal_was() {
    Model* model = get_initial_model();

    model->state = GUESSING_STATE;
    update_model(model,"");
    mu_assert("next state is not CHECKING_IF_GUESS_IS_CORRECT",model->state == CHECKING_GUESS_IN_LEAF_NODE_STATE);
    update_model(model,"no");
    mu_assert("state",model->state == GETTING_ANIMAL_NAME_STATE);
    mu_assert("yeah",strcmp(model->message_from_engine,"what animal was?")==0);
    return 0;
}

static char * after_asking_what_animal_was_will_get_ask_the_question_to_distinguish() {
    Model* model = get_initial_model();
    model->state = GETTING_ANIMAL_NAME_STATE;
    update_model(model,"cat");
    mu_assert("the animal to be learned should be a cat",strcmp("cat",model->animal_to_be_learned)==0);
    mu_assert("the question is not as expected",
        strcmp(model->message_from_engine,"what is the question to distinguish a cat from a elephant?")==0);
    return 0;
}

static char * after_giving_the_question_to_distinguish_animals_will_as_what_is_the_answer() {
    Model* model = get_initial_model();
    model->state = GETTING_ANIMAL_NAME_STATE;
    update_model(model,"cat");
    mu_assert("state",model->state==GETTING_DISCRIMINATING_QUESTION);
    mu_assert("the animal to be learned should be a cat",strcmp("cat",model->animal_to_be_learned)==0);
    mu_assert("the question is not as expected",
        strcmp(model->message_from_engine,"what is the question to distinguish a cat from a elephant?")==0);
    update_model(model,"is it big?");
    mu_assert(model->message_from_engine,(strcmp("what is the answer to the question 'is it big?' to distinguish a cat from a elephant?",model->message_from_engine)==0));
    return 0;
}

static char * after_asking_what_is_the_answer_to_the_discriinating_will_receive_the_answer() {
    Model* model = get_initial_model();

    model->state = GETTING_ANIMAL_NAME_STATE; 
    update_model(model,"cat");

    model->animal_to_be_learned = concatenate_strings(1,"cat");

    mu_assert("the animal to be learned should be a cat",strcmp("cat",model->animal_to_be_learned)==0);
    mu_assert("the question is not as expected",
        strcmp(model->message_from_engine,"what is the question to distinguish a cat from a elephant?")==0);
    update_model(model,"is it big?");
    mu_assert(model->message_from_engine,(strcmp("what is the answer to the question 'is it big?' to distinguish a cat from a elephant?",
        model->message_from_engine)==0));
    mu_assert("state should be GETTING_ANSWER_TO_DISCRIMINATING_QUESTION",model->state==GETTING_ANSWER_TO_DISCRIMINATING_QUESTION);
    update_model(model,"no");
    mu_assert("new root",model->knowledge_tree->leaf_or_not_leaf==IS_NON_LEAF);
    mu_assert("new root2",(strcmp(model->knowledge_tree->discriminating_question,"is it big?")==0));
    mu_assert("new root3",(strcmp(model->knowledge_tree->yes_branch->animal,"elephant")==0));

    return 0;
}

static char * will_create_a_new_tree_structure_after_learning() {
    Model* model = get_initial_model();

    model->state = GETTING_ANIMAL_NAME_STATE; 
    update_model(model,"cat");

    model->animal_to_be_learned = concatenate_strings(1,"cat");

    mu_assert("the animal to be learned should be a cat",strcmp("cat",model->animal_to_be_learned)==0);
    mu_assert("the question is not as expected",
        strcmp(model->message_from_engine,"what is the question to distinguish a cat from a elephant?")==0);
    update_model(model,"is it big?");
    mu_assert(model->message_from_engine,(strcmp("what is the answer to the question 'is it big?' to distinguish a cat from a elephant?",
        model->message_from_engine)==0));
    mu_assert("state should be GETTING_ANSWER_TO_DISCRIMINATING_QUESTION",model->state==GETTING_ANSWER_TO_DISCRIMINATING_QUESTION);
    update_model(model,"no");
    mu_assert("the new type of knowledge tree is non leaf",model->knowledge_tree->leaf_or_not_leaf == IS_NON_LEAF);
    return 0;
}

static char * will_ask_the_discriminating_question_when_tree_is_non_leaf() {
    Model* model = get_initial_model();
    Knowledge_tree* yes_node=make_leaf_node("elephant");
    Knowledge_tree* no_node=make_leaf_node("cat");
    Knowledge_tree* composed_tree = make_non_leaf_node("is it big?",yes_node,no_node);
    model->knowledge_tree = composed_tree;
    model->current_node = composed_tree;
    update_model(model,"");
    mu_assert("next state is not GUESSING",model->state == GUESSING_STATE);
    update_model(model,"");
    mu_assert(concatenate_strings(2,"expecting is it big? but getting ",model->message_from_engine),strcmp(model->message_from_engine,"is it big?")==0);
    return 0;
}

static char * will_navigate_the_non_leaf_tree_by_questions() {
    Model* model = get_initial_model();
    Knowledge_tree* yes_node=make_leaf_node("elephant");
    Knowledge_tree* no_node=make_leaf_node("cat");
    Knowledge_tree* composed_tree = make_non_leaf_node("is it big?",yes_node,no_node);
    model->knowledge_tree = composed_tree;
    model->current_node = composed_tree;
    update_model(model,"");
    mu_assert("next state is not GUESSING",model->state == GUESSING_STATE);
    update_model(model,"");
    mu_assert(concatenate_strings(2,"expecting is it big? but getting ",model->message_from_engine),
        strcmp(model->message_from_engine,"is it big?")==0);
    mu_assert("state",model->state == CHECKING_GUESS_IN_NON_LEAF_NODE_STATE);
    update_model(model,"no");
    mu_assert("animal",strcmp((model->current_node)->animal,"cat")==0);
    return 0;
}

static char * will_navigate_the_non_leaf_tree_by_questions_2() {
    Model* model = get_initial_model();
    Knowledge_tree* yes_node=make_leaf_node("elephant");
    Knowledge_tree* no_node=make_leaf_node("cat");
    Knowledge_tree* composed_tree = make_non_leaf_node("is it big?",yes_node,no_node);
    model->knowledge_tree = composed_tree;
    model->current_node = composed_tree;
    update_model(model,"");
    mu_assert("next state is not GUESSING",model->state == GUESSING_STATE);
    update_model(model,"");
    mu_assert(concatenate_strings(2,"expecting is it big? but getting ",model->message_from_engine),
        strcmp(model->message_from_engine,"is it big?")==0);
    mu_assert("state",model->state == CHECKING_GUESS_IN_NON_LEAF_NODE_STATE);
    update_model(model,"yes");
    mu_assert("animal",strcmp(((model->current_node))->animal,"elephant")==0);
    return 0;
}

static char * leaning_on_a_non_leaf_tree() {
    Model* model = get_initial_model();
    model->knowledge_tree = make_non_leaf_node("is it big?",make_leaf_node("elephant"),make_leaf_node("cat"));
    update_model(model,"");


    mu_assert("next state is not GUESSING",model->state == GUESSING_STATE);
    update_model(model,"");
    mu_assert(concatenate_strings(2,"expecting is it big? but getting ",model->message_from_engine),
        strcmp(model->message_from_engine,"is it big?")==0);
    mu_assert("stateX",model->state == CHECKING_GUESS_IN_NON_LEAF_NODE_STATE);
    update_model(model,"no");
    mu_assert("animal",strcmp(((model->current_node))->animal,"cat")==0);
    

    mu_assert("stateZ",model->state == CHECKING_GUESS_IN_LEAF_NODE_STATE);

    mu_assert("engine",strcmp(model->message_from_engine,"is it a cat?")==0);
    update_model(model,"no");
    mu_assert("getting animal",model->state == GETTING_ANIMAL_NAME_STATE);
    update_model(model,"mouse");
    mu_assert("getting animal",model->state == GETTING_DISCRIMINATING_QUESTION);
    update_model(model,"does it like cheese?");
    mu_assert("getting answer",model->state == GETTING_ANSWER_TO_DISCRIMINATING_QUESTION);
    mu_assert("message",strncmp(model->message_from_engine,"what is the answer to the question 'does it like cheese'",15)==0);
    update_model(model,"yes");
    mu_assert("think about an animal",model->state == GUESSING_STATE);
    mu_assert("non leaf",model->knowledge_tree->leaf_or_not_leaf==IS_NON_LEAF);
    mu_assert("leaf",model->knowledge_tree->yes_branch->leaf_or_not_leaf==IS_LEAF);
    mu_assert("elephant",strcmp(model->knowledge_tree->yes_branch->animal,"elephant")==0);
    mu_assert("leaf",model->knowledge_tree->no_branch->leaf_or_not_leaf==IS_NON_LEAF);
    mu_assert("does it like cheese?",strcmp(model->knowledge_tree->no_branch->discriminating_question,"does it like cheese?")==0);
    mu_assert("leaf 2",model->knowledge_tree->no_branch->yes_branch->leaf_or_not_leaf=IS_LEAF);
    mu_assert("mouseX",strcmp(model->knowledge_tree->no_branch->yes_branch->animal,"mouse")==0);
    mu_assert("mouseX2",strcmp(model->knowledge_tree->no_branch->no_branch->animal,"cat")==0);
    mu_assert("guessing",model->state == GUESSING_STATE);
    update_model(model,"");
    mu_assert("asf",model->state == CHECKING_GUESS_IN_NON_LEAF_NODE_STATE);
    update_model(model,"");
    update_model(model,"");
    mu_assert("engine",strcmp(model->message_from_engine,"is it big?")==0);
    update_model(model,"no");
    mu_assert("engine",strcmp(model->message_from_engine,"does it like cheese?")==0);
    update_model(model,"no");



    return 0;
}

static char * composed_tree_test() {
    Model* model = get_initial_model();
    Knowledge_tree* cat_mouse = make_non_leaf_node("does it like cheese?",make_leaf_node("mouse"),make_leaf_node("cat"));
    Knowledge_tree* pterodattil_elephant = make_non_leaf_node("is it estinct?",make_leaf_node("pterodattil"),make_leaf_node("elephant"));

    Knowledge_tree* initial_tree = make_non_leaf_node("is it big?",make_leaf_node("elephant"),cat_mouse);
    model->knowledge_tree = make_non_leaf_node("is it big?",make_leaf_node("elephant"),make_leaf_node("cat"));
    model->knowledge_tree = initial_tree;


    update_model(model,"");
    mu_assert("next state is not GUESSING",model->state == GUESSING_STATE);
    update_model(model,"");
    mu_assert(concatenate_strings(2,"expecting is it big? but getting ",model->message_from_engine),
        strcmp(model->message_from_engine,"is it big?")==0);

    mu_assert("stateX",model->state == CHECKING_GUESS_IN_NON_LEAF_NODE_STATE);
    update_model(model,"yes");
    mu_assert("bla",model->current_node->leaf_or_not_leaf==IS_LEAF);
    mu_assert("non elephant",(strcmp(model->message_from_engine,"is it a elephant?"))==0);
    update_model(model,"no");
    mu_assert("non elephant",(strcmp(model->message_from_engine,"what animal was?"))==0);
    update_model(model,"pterodattil");
    mu_assert("non elephant",(strncmp(model->message_from_engine,"what",4)==0));
    update_model(model,"estinct?");
    mu_assert("non what",(strncmp(model->message_from_engine,"what",4)==0));
    update_model(model,"yes");
    mu_assert("let's",(strncmp(model->message_from_engine,"let's",5)==0));
    update_model(model,"");
    mu_assert("big ",(strcmp(model->message_from_engine,"is it big?")==0));
    update_model(model,"yes");
    mu_assert(concatenate_strings(2,"expecting estinct, but getting ",
        model->message_from_engine),(strcmp(model->message_from_engine,"estinct?")==0));
    update_model(model,"no");
    mu_assert("no elephan",(strcmp(model->message_from_engine,"is it a elephant?")==0));
    update_model(model,"no");

    return 0;
}


static char *reproducing_bug() {
    Model* model = get_initial_model();
    update_model(model,"");
    update_model(model,"");
    mu_assert("non elephant",strcmp(model->message_from_engine,"is it a elephant?")==0);
    update_model(model,"no");
    update_model(model,"cat");
    update_model(model,"is it big?");
    update_model(model,"no"); 
    mu_assert("should be non leaf",model->knowledge_tree->leaf_or_not_leaf==IS_NON_LEAF);
    mu_assert("root shuld be question is it big",(strcmp(model->knowledge_tree->discriminating_question,"is it big?")==0));
    mu_assert("yes branch should be elephant",strcmp(model->knowledge_tree->yes_branch->animal,"elephant")==0);
    mu_assert("no branch should be cat",strcmp(model->knowledge_tree->no_branch->animal,"cat")==0);

    // starts again
    update_model(model,""); 

    update_model(model,"no"); // big
    update_model(model,"no"); // cat
    update_model(model,"mouse"); 
    update_model(model,"cheese?"); 
    update_model(model,"yes");  

    mu_assert("should be non leaf",model->knowledge_tree->leaf_or_not_leaf==IS_NON_LEAF);
    mu_assert("root shuld be question is it big",(strcmp(model->knowledge_tree->discriminating_question,"is it big?")==0));
    mu_assert("yes branch should be elephant",strcmp(model->knowledge_tree->yes_branch->animal,"elephant")==0);
    mu_assert("no branch should be non leaf",(model->knowledge_tree->no_branch->leaf_or_not_leaf == IS_NON_LEAF));
    mu_assert("no branch question should bshould be cheese",strcmp(model->knowledge_tree->no_branch->discriminating_question,"cheese?")==0);
    mu_assert("mouse check",strcmp(model->knowledge_tree->no_branch->yes_branch->animal,"mouse")==0);
    mu_assert("cat check",strcmp(model->knowledge_tree->no_branch->no_branch->animal,"cat")==0);

    update_model(model,""); 
    
    // starts again
    update_model(model,"yes");  
    update_model(model,"no");  
    update_model(model,"pterodaptil");  
    update_model(model,"estinct?");  
    update_model(model,"yes");  

    mu_assert("should be non leaf",model->knowledge_tree->leaf_or_not_leaf==IS_NON_LEAF);
    mu_assert("root shuld be question is it big",(strcmp(model->knowledge_tree->discriminating_question,"is it big?")==0));
    mu_assert("yes branch should be non leafXXXX",model->knowledge_tree->yes_branch->leaf_or_not_leaf==IS_NON_LEAF);
    mu_assert("question is estinct" ,strcmp(model->knowledge_tree->yes_branch->discriminating_question,"estinct?")==0);
    mu_assert("yes branch of yes branch should be leaf",model->knowledge_tree->yes_branch->yes_branch->leaf_or_not_leaf == IS_LEAF);
    mu_assert("no branch of yes branch should be leaf",model->knowledge_tree->yes_branch->no_branch->leaf_or_not_leaf == IS_LEAF);

    mu_assert("pterodaptil",strcmp(model->knowledge_tree->yes_branch->yes_branch->animal,"pterodaptil")==0);
    mu_assert("elephant",strcmp(model->knowledge_tree->yes_branch->no_branch->animal,"elephant")==0);

    // starts again
    
    update_model(model,"");  
    update_model(model,"yes");  
    update_model(model,"no");  
    update_model(model,"no");  
    update_model(model,"cow");  
    update_model(model,"deliver milk?");  
    update_model(model,"yes");  
    update_model(model,"");  
    char *outmessage = concatenate_strings(2,"initial state issue ",model->message_from_engine);
    mu_assert(outmessage,strcmp(model->message_from_engine,"is it big?")==0);
    
    mu_assert("should be non leaf",model->knowledge_tree->leaf_or_not_leaf==IS_NON_LEAF);
    mu_assert("root shuld be question is it big",(strcmp(model->knowledge_tree->discriminating_question,"is it big?")==0));
    mu_assert("yes branch should be non leaf",model->knowledge_tree->yes_branch->leaf_or_not_leaf==IS_NON_LEAF);
    mu_assert("question is estinct" ,strcmp(model->knowledge_tree->yes_branch->discriminating_question,"estinct?")==0);
    mu_assert("yes branch of yes branch should be leaf",model->knowledge_tree->yes_branch->yes_branch->leaf_or_not_leaf == IS_LEAF);
    mu_assert("no branch of yes branch should be non leaf",model->knowledge_tree->yes_branch->no_branch->leaf_or_not_leaf == IS_NON_LEAF);
    mu_assert("deliver milk question",strcmp(model->knowledge_tree->yes_branch->no_branch->discriminating_question,"deliver milk?")==0);
    mu_assert("yes branch of no branch of yes branch should be leaf",model->knowledge_tree->yes_branch->no_branch->yes_branch->leaf_or_not_leaf == IS_LEAF);
    mu_assert("no branch of no branch of yes branch should be leaf",model->knowledge_tree->yes_branch->no_branch->no_branch->leaf_or_not_leaf == IS_LEAF);

    mu_assert("yes branch of no branch of yes branch should be cow",strcmp(model->knowledge_tree->yes_branch->no_branch->yes_branch->animal,"cow") == 0);
    mu_assert("no branch of no branch of yes branch should be elephant",strcmp(model->knowledge_tree->yes_branch->no_branch->no_branch->animal,"elephant") == 0);

    update_model(model,"");  
    mu_assert("ask if it is big",strcmp(model->message_from_engine,"is it big?")==0);
    update_model(model,"yes");  
    mu_assert("ask if it is estinct",strcmp(model->message_from_engine,"estinct?")==0);
    update_model(model,"no");  
    mu_assert("ask if it deliver milks",strcmp(model->message_from_engine,"deliver milk?")==0);
    update_model(model,"yes");  
    mu_assert("ask if it is an elephant",strcmp(model->message_from_engine,"is it a cow?")==0);
    update_model(model,"no");  

    return 0;
}


static char* reproducing_bug2() {
    Model* model = get_initial_model();
    huge_set_model(model);
    mu_assert("is it big?",strcmp("is it big?",model->message_from_engine)==0);
    update_model(model,"yes"); 
    mu_assert("estinctX?",strcmp("estinct?",model->message_from_engine)==0);
    update_model(model,"no"); 
    mu_assert("deliver milk?",strcmp("deliver milk?",model->message_from_engine)==0);
    update_model(model,"no"); 
    mu_assert("elephant",strcmp("is it a elephant?",model->message_from_engine)==0);
    update_model(model,"no"); 
    mu_assert("what animal was",strcmp("what animal was?",model->message_from_engine)==0);
    update_model(model,"king"); 
    mu_assert("what is the question",strcmp("what is the question to distinguish a king from a elephant?",model->message_from_engine)==0);
    update_model(model,"is it the king of the forest?"); 
    mu_assert("king forest",strcmp("what is the answer to the question 'is it the king of the forest?' to distinguish a king from a elephant?",model->message_from_engine)==0);
    update_model(model,"yes"); 
    mu_assert("let's start",strcmp("let's start again. Think about an animal",model->message_from_engine)==0);
    update_model(model,""); 

    mu_assert("is it big?",strcmp("is it big?",model->message_from_engine)==0);
    update_model(model,"yes"); 
    mu_assert("estinctX?",strcmp("estinct?",model->message_from_engine)==0);
    update_model(model,"no"); 
    mu_assert("deliver milk?",strcmp("deliver milk?",model->message_from_engine)==0);
    update_model(model,"no"); 
    mu_assert("deliver milk?",strcmp("is it the king of the forest?",model->message_from_engine)==0);
    update_model(model,"no"); 
    
    return 0;
}


static char * test_add_element_to_list() {
    Str_list* list; 
    list = NULL;
    add_element_to_list(&list,"hello");
    mu_assert("asdf",strcmp("hello",list->element)==0);
    add_element_to_list(&list,"hellohello");
    mu_assert("asdfasfd",strcmp("hellohello",list->next->element)==0);
    char* exphead = head_element_of_list(&list);
    mu_assert("head",strcmp(exphead,"hello")==0);

    Str_list* tail = tail_of_list(list);
    mu_assert("tail",strcmp(tail->element,"hellohello")==0);

    return 0;
}

static char * test_larning_tree_on_only_leaf_tree() {
    Str_list *list=NULL;
    Knowledge_tree *initial_tree = make_leaf_node("elephant");
    rearrange_knowledge_tree(list,initial_tree,"is it big?","no","cat");

    mu_assert("shold be not leaf",initial_tree->leaf_or_not_leaf==IS_NON_LEAF);
    mu_assert("question should be is it big",strcmp("is it big?",initial_tree->discriminating_question)==0);

    mu_assert("yes branch should be leaf",initial_tree->yes_branch->leaf_or_not_leaf==IS_LEAF);
    mu_assert("yes branch should be an elephant",strcmp("elephant",initial_tree->yes_branch->animal)==0);
    mu_assert("no branch should be a cat",strcmp("cat",initial_tree->no_branch->animal)==0);

    return 0;
}

static char * test_larning_tree_on_a_non_leaf_tree() {
    Str_list *list=NULL;
    add_element_to_list(&list,"no");
    Knowledge_tree *initial_tree = make_non_leaf_node("is it big?",make_leaf_node("elephant"),make_leaf_node("cat"));

    rearrange_knowledge_tree(list,initial_tree,"does it like cheese?","yes","mouse");
    mu_assert("no branch becomes non leaf",initial_tree->no_branch->leaf_or_not_leaf==IS_NON_LEAF);
    mu_assert("no branch question is does it like cheese",strcmp("does it like cheese?",initial_tree->no_branch->discriminating_question)==0);
    mu_assert("no branch no branch is cat",strcmp("cat",initial_tree->no_branch->no_branch->animal)==0);
    mu_assert("yes branch of no branch is cat",strcmp("mouse",initial_tree->no_branch->yes_branch->animal)==0);
    mu_assert("shold be not leaf",initial_tree->leaf_or_not_leaf==IS_NON_LEAF); 
    mu_assert("question should be is it big",strcmp("is it big?",initial_tree->discriminating_question)==0);
    mu_assert("yes branch should be leaf",initial_tree->yes_branch->leaf_or_not_leaf==IS_LEAF);
    mu_assert("yes branch should be an elephant",strcmp("elephant",initial_tree->yes_branch->animal)==0);
    mu_assert("no branch should be a cat",strcmp("cat",initial_tree->no_branch->animal)==0);

    return 0;
}

static char * test_larning_tree_on_a_non_leaf_tree3() {
    Str_list *list=NULL;
    add_element_to_list(&list,"yes");
    Knowledge_tree *initial_tree = make_non_leaf_node("is it big?",make_leaf_node("elephant"),make_leaf_node("cat"));

    rearrange_knowledge_tree(list,initial_tree,"is it estinct?","yes","tirannosaurus");
    mu_assert("yes branch becomes non leaf",initial_tree->yes_branch->leaf_or_not_leaf==IS_NON_LEAF);
    mu_assert("no branch question is does it like cheese",strcmp("is it estinct?",initial_tree->yes_branch->discriminating_question)==0);
    mu_assert("shold be not leaf",initial_tree->leaf_or_not_leaf==IS_NON_LEAF); 
    mu_assert("no branch should be leaf",initial_tree->no_branch->leaf_or_not_leaf==IS_LEAF);
    mu_assert("no branch should be cat",strcmp("cat",initial_tree->no_branch->animal)==0);
    mu_assert("yes banch branch should be non leaf",initial_tree->yes_branch->leaf_or_not_leaf==IS_NON_LEAF);
    mu_assert(concatenate_strings(1,"yes banch branch of yes branch should be tirannosaurus but is instead: ",initial_tree->yes_branch->yes_branch->animal) ,strcmp("tirannosaurus",initial_tree->yes_branch->yes_branch->animal)==0);
    mu_assert(concatenate_strings(1,"no banch branch of yes branch should be elephant but is instead: ",initial_tree->yes_branch->no_branch->animal) ,strcmp("elephant",initial_tree->yes_branch->no_branch->animal)==0);
    mu_assert("yes branch should be an elephant",strcmp("elephant",initial_tree->yes_branch->animal)==0);
    mu_assert("no branch should be a cat",strcmp("cat",initial_tree->no_branch->animal)==0);

    return 0;
}





static char * all_tests() {
     mu_run_test(test_foo);

     mu_run_test(initial_message_should_be_welcome);
     mu_run_test(initial_state_should_be_THINK_ABOUT_AN_ANIMAL);
     mu_run_test(after_first_update_state_will_be_GUESSING);
     mu_run_test(after_asking_what_animal_was_will_get_ask_the_question_to_distinguish);
     mu_run_test(after_giving_the_question_to_distinguish_animals_will_as_what_is_the_answer);
     mu_run_test(after_asking_what_is_the_answer_to_the_discriinating_will_receive_the_answer);
     mu_run_test(will_create_a_new_tree_structure_after_learning);
     mu_run_test(will_ask_the_discriminating_question_when_tree_is_non_leaf);
     mu_run_test(will_navigate_the_non_leaf_tree_by_questions);
     mu_run_test(will_navigate_the_non_leaf_tree_by_questions_2);
     mu_run_test(composed_tree_test);
     mu_run_test(test_add_element_to_list);
     mu_run_test(leaning_on_a_non_leaf_tree);
     mu_run_test(reproducing_bug);
     mu_run_test(reproducing_bug2);
     mu_run_test(test_larning_tree_on_only_leaf_tree);
     mu_run_test(test_larning_tree_on_a_non_leaf_tree);
     mu_run_test(test_larning_tree_on_a_non_leaf_tree3);

     return 0;
}




int main(int argc, char **argv) {
     char *result = all_tests();
     if (result != 0) {
         printf("%s\n", result);
     }
     else {
         printf("ALL TESTS PASSED\n");
     }
     printf("Tests run: %d\n", tests_run);
 
     return result != 0;
}