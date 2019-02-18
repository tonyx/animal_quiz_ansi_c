# animal_quiz_ansi_c
animal_quiz_ansi_c

A classic "kata" problem, made in ansi C, applying a model-view-update like approach.

A binary tree starts with a single node containing an elephant, and then it try to guess which animal the user is thinking.
When the guess is incorrect, then the user helps to add the new animal suggesting a question that is needed to distinguish the guessed one with the actual one, so the knoweledge base will increase.

Example of interaction:\
welcome\
think about an animal\
is it a elephant?\
no\
what animal was?\
cat\
what is the question to distinguish a cat from a elephant?\
is it big?\
what is the answer to the question 'is it big?' to distinguish a cat from a elephant?\
no\ 
let's start again. Think about an animal\

is it big?\
no\
is it a cat?\
no\
what animal was?\
mouse\
what is the question to distinguish a mouse from a cat?\
does it like cheese?\
what is the answer to the question 'does it like cheese?' to distinguish a mouse from a cat?\
yes\
let's start again. Think about an animal\
