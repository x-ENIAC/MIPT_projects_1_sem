#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <stdarg.h>
#include "binary_tree.h"
#include "work_with_file.h"
#include "warnings.h"

#define ASSERTION(status)                                                                                                                    \
    system("echo \e[31m-----------------!WARNING!----------------\e[0m");                                                                    \
    char warning_info[150] = "";                                                                                                             \
    sprintf(warning_info, "echo \"\\e[31mIN FILE %s (FUNCTION %s, LINE %d)\\e[0m\"", __FILE__, __FUNCTION__, __LINE__);                      \
    system(warning_info);                                                                                                                    \
    sprintf(warning_info, "echo \"\\e[31mFile status: %d\\e[0m\"", status);                                                \
    system(warning_info);

char say_string[MAX_SIZE_KEY] = "";

#define PRINT_AND_SAY(string)                   \
    printf("%s", string);                     \
    strcat(say_string, "echo \"");               \
    strcat(say_string, string);                 \
    strcat(say_string, "\" | festival --tts");  \
    system(say_string);                         \
    memset(say_string, '\0', MAX_SIZE_KEY);


TREE_STATUS tree_construct(Binary_tree* tree) {
    if(!tree) {
        warning(TEXT_TREE_STATUS[TREE_BAD_POINTER], INFORMATION_ABOUT_CALL);
        return TREE_BAD_POINTER;
    }

    tree->root = NULL;
    tree->size_tree = 0;

    return TREE_OKEY;
}

TREE_STATUS load_buffer_and_tree_from_file(Binary_tree* tree, Catalog_names* catalog_name_nodes, char* file_name) {
    File input_file = {};
    input_file.name_file = file_name;

    FILE_STATUS status = read_buffer(&input_file);
    if(status != FILE_OK) {
        warning(TEXT_TREE_STATUS[TREE_BAD_READ_FROM_FILE], INFORMATION_ABOUT_CALL);
        return TREE_BAD_READ_FROM_FILE;
    }

    catalog_name_nodes->buffer = input_file.buffer;
    catalog_name_nodes->capacity_buffer = catalog_name_nodes->size_buffer = input_file.size_buffer;

    tree->root = load_tree_from_buffer(input_file.buffer, catalog_name_nodes);

    return TREE_OKEY;
}

Node_binary_tree* load_tree_from_buffer(char* buffer, Catalog_names* catalog_name_nodes) {
    if(!buffer)
        return NULL;

    char* now_position = search_next_position_after_symbol(buffer, '"');                                  // find begin root
    char* root_position = now_position;

    while(*now_position != '"')                               // find end root
        ++now_position;

    Node_binary_tree* result_tree = (Node_binary_tree*)calloc(1, sizeof(Node_binary_tree));
    node_construct(result_tree, root_position, now_position++ - root_position, catalog_name_nodes);

    while(*now_position != '[' && *now_position != ']' && *now_position != '"')        // find []
        ++now_position;

    char* left_son = {};
    bool is_leaf_left = false, is_leaf_right = false;

    if(*now_position  != ']') {
        now_position = search_next_position_after_symbol(now_position, '"');

        left_son = now_position;
        while(*now_position != '"')                           // find end the left son
            ++now_position;
        int length_name_left = now_position++ - left_son;

        while(*now_position != '"' && *now_position != '[')     // find
            ++now_position;

        if(*now_position == '[') {
            now_position = find_right_node(now_position);
            is_leaf_left = false;
        } else
            is_leaf_left = true;

        now_position = search_next_position_after_symbol(now_position, '"');                              // find begin right name

        char* right_son = now_position;
        while(*now_position != '"')                              // find end right name
            ++now_position;
        int length_name_right = now_position++ - right_son;

        while(*now_position != ']' && *now_position != '[')
            ++now_position;

        if(*now_position == '[')
            is_leaf_right = false;
        else
            is_leaf_right = true;

        if(is_leaf_left) {
            Node_binary_tree* left_son_of_leaf = (Node_binary_tree*)calloc(1, sizeof(Node_binary_tree));
            node_construct(left_son_of_leaf, left_son, length_name_left, catalog_name_nodes);
            result_tree->left = left_son_of_leaf;
        }
        else
            result_tree->left = load_tree_from_buffer(left_son - 1, catalog_name_nodes);

        result_tree->left->length_name = length_name_left;
        result_tree->left->position_in_buffer = left_son;
        result_tree->left->parent = result_tree;
        result_tree->left->is_leaf = is_leaf_left;

        if(is_leaf_right) {
            Node_binary_tree* right_son_of_leaf = (Node_binary_tree*)calloc(1, sizeof(Node_binary_tree));
            node_construct(right_son_of_leaf, right_son, length_name_right, catalog_name_nodes);
            result_tree->right = right_son_of_leaf;
        } else
            result_tree->right = load_tree_from_buffer(right_son - 1, catalog_name_nodes);

        result_tree->right->length_name = length_name_right;
        result_tree->right->position_in_buffer = right_son;
        result_tree->right->parent = result_tree;
        result_tree->right->is_leaf = is_leaf_right;
    }

    return result_tree;
}

TREE_STATUS node_construct(Node_binary_tree* new_node, char* position_in_buffer, const int length_name, Catalog_names* catalog_name_node) {
    if(!new_node) {
        warning(TEXT_TREE_STATUS[TREE_BAD_POINTER], INFORMATION_ABOUT_CALL);
        return TREE_BAD_POINTER;
    }

    new_node->height      = 1;
    new_node->length_name = length_name;

    if(length_name > MAX_SIZE_KEY)
        new_node->length_name = MAX_SIZE_KEY;

    printf("Cry.\n");

    new_node->index_into_names_catalog = catalog_name_node->count_nodes;
    add_into_catalog(catalog_name_node, position_in_buffer - catalog_name_node->buffer, length_name);

    return TREE_OKEY;
}

char* search_next_position_after_symbol(char* pointer_in_tree, const char symbol) {
    char* copy_pointer = pointer_in_tree;

    while(*copy_pointer != symbol)
        ++copy_pointer;
    ++copy_pointer;

    return copy_pointer;
}

char* find_right_node(char* pointer_in_tree) {
    char* copy_pointer = pointer_in_tree + 1;

    int is_it_node = 0, count_of_brackets = 1;

    while(count_of_brackets > 0) {
        if(*copy_pointer == '"')
            is_it_node = (is_it_node + 1) % 2;
        else {
            if(*copy_pointer == '[')
                ++count_of_brackets;
            else if(*copy_pointer == ']')
                --count_of_brackets;
        }

        ++copy_pointer;
    }

    while(*copy_pointer != '"')
        ++copy_pointer;

    return copy_pointer;        // возвращает указатель на имя правого сына

}

void dump_tree(Binary_tree* tree) {
    if(!tree)
        return;
    FILE* picture = fopen("picture.dot", "w");
    fprintf(picture, "digraph graphname {\n");

    if(tree->root)
        do_dump_tree(tree->root, picture);

    fprintf(picture, "}\n");
    fclose(picture);

    char result_string[SIZE_OF_NAME_FILES] = "";
    sprintf(result_string, "dot picture.dot -T png -o picture.png"); // | eog picture0.png"; //, my_list->number_of_draw_graph_call);
    system(result_string);
}

void do_dump_tree(Node_binary_tree* node, FILE* file) {
    fprintf(file, "%ld[shape=record label = \"", (size_t)node); //\"height = %ld|{left = %p|right = %p| parent = %p|node = %p}|size_name = %d|", (size_t)node, node->height, node->left, node->right, node->parent, node, node->length_name);
    //print_node_name_into_file(node->position_in_buffer, node->length_name, file);
    fprintf(file, "\"]\n");
    if(node->left) {
        fprintf(file, "%ld->%ld\n", (size_t)node, (size_t)node->left);
        do_dump_tree(node->left, file);
    }
    if(node->right) {
        fprintf(file, "%ld->%ld\n", (size_t)node, (size_t)node->right);
        do_dump_tree(node->right, file);
    }

}

void print_node_name_into_file(Catalog_names* catalog_name_nodes, size_t index_into_catalog, FILE* file) {
    for(int i=0; i<catalog_name_nodes->nodes[index_into_catalog].length_name; ++i)
        fprintf(file, "%c", catalog_name_nodes->buffer[i + catalog_name_nodes->nodes[index_into_catalog].count_symbols_from_begin]);
}

void print_node_name_into_concole(Catalog_names* catalog_name_nodes, size_t index_into_catalog) {
    for(int i=0; i<catalog_name_nodes->nodes[index_into_catalog].length_name; ++i)
        printf("%c", catalog_name_nodes->buffer[i + catalog_name_nodes->nodes[index_into_catalog].count_symbols_from_begin]);
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////// PLAY ///////////////////////////////////////////////////////////////////////////////

bool search_leaf(Node_binary_tree* node, Catalog_names* catalog_name_nodes) {
    char* name_node = (char*)calloc(node->length_name, sizeof(char));
    memcpy(name_node, catalog_name_nodes->buffer + catalog_name_nodes->nodes[node->index_into_names_catalog].count_symbols_from_begin, catalog_name_nodes->nodes[node->index_into_names_catalog].length_name);

    if(node->is_leaf) {
        print_and_say(YES_OR_NO_QUESTION, "Everything is clear. You made a ", name_node, " wish. Am I right?", NULL);
        free(name_node);
        return check_akinator_answer(node, catalog_name_nodes);
    } else {
        print_and_say(YES_OR_NO_QUESTION, name_node, "?", NULL);

        USER_ANSWERS user_answer = get_user_answer();
        if(user_answer == YES_ANSWER_USER) {
            return search_leaf(node->left, catalog_name_nodes);
        } else
            return search_leaf(node->right, catalog_name_nodes);
    }

    free(name_node);
}

USER_ANSWERS get_user_answer() {
    char user_answer = '@', garbage = '!';

    scanf("%c", &user_answer);
    while(scanf("%c", &garbage) && garbage != '\n') {}

    if(user_answer == 'y' || user_answer == 'Y')
        return YES_ANSWER_USER;
    if(user_answer == 'n' || user_answer == 'N')
        return NO_ANSWER_USER;

    print_and_say(YES_OR_NO_QUESTION, "I don't understand you, my small friend.");
    return get_user_answer();
}

bool check_akinator_answer(Node_binary_tree* node, Catalog_names* catalog_name_nodes) {
    USER_ANSWERS user_answer = get_user_answer();

    if(user_answer == YES_ANSWER_USER) {
        print_and_say(YES_OR_NO_QUESTION, "Of course, who would doubt it? Ha-ha. Do you want to play again?");
    } else {
        print_and_say(QUESTION_WITH_FULL_ANSWER, "Hmmm.. It is very strange. All right, what did you wish for?");

        char* name_new_node = (char*)calloc(MAX_SIZE_KEY, sizeof(char));
        char garbage = '!';

        scanf("%[^\r\n]%c", name_new_node, &garbage);
        int length_name_new_node = strlen(name_new_node);
        write_into_catalog_buffer(catalog_name_nodes, name_new_node, length_name_new_node);

        Node_binary_tree* new_node = (Node_binary_tree*)calloc(1, sizeof(Node_binary_tree));
        node_construct(new_node, catalog_name_nodes->size_buffer + catalog_name_nodes->buffer - length_name_new_node - 1, length_name_new_node, catalog_name_nodes);

        strcat(name_new_node, " and ");
        memcpy(name_new_node + length_name_new_node + strlen(" and "), node->position_in_buffer, node->length_name);

        print_and_say(QUESTION_WITH_FULL_ANSWER, "What is the differences between ", name_new_node, "?", NULL);

        char* difference = (char*)calloc(MAX_SIZE_KEY, sizeof(char));
        scanf("%[^\r\n]%c", difference, &garbage);
        int length_difference = strlen(difference);
        write_into_catalog_buffer(catalog_name_nodes, difference, length_difference);

        Node_binary_tree* new_indicator = (Node_binary_tree*)calloc(1, sizeof(Node_binary_tree));
        node_construct(new_indicator, catalog_name_nodes->size_buffer + catalog_name_nodes->buffer - length_difference - 1, length_difference, catalog_name_nodes);

        new_indicator->length_name = length_difference;
        new_indicator->left  = new_node;
        new_indicator->right = node;
        new_indicator->parent = node->parent;
        new_indicator->is_leaf = false;

        if(node->parent->left == node)
            node->parent->left = new_indicator;
        else
            node->parent->right = new_indicator;

        node->parent  = new_node->parent  = new_indicator;
        node->is_leaf = new_node->is_leaf = true;

        print_and_say(YES_OR_NO_QUESTION, "Well, I remember that word. But next time you won't catch me, I'll be smarter! Do you want to play again?", NULL);

        //free(name_new_node);
        //free(new_node);
        //free(difference);
        //free(new_indicator);
    }

    if(get_user_answer() == YES_ANSWER_USER)
        return true;
    return false;
}

/*void put_tree_to_disk(Node_binary_tree* node, FILE* file, const int height) {
    for(int i=0; i<height; ++i)
        fprintf(file, " ");
    fprintf(file, "\"");
    print_node_name_into_file(node->position_in_buffer, node->length_name, file);
    fprintf(file, "\"\n");

    if(!node->is_leaf) {
        for(int i=0; i<height; ++i)
            fprintf(file, " ");
        fprintf(file, "[\n");
        put_tree_to_disk(node->left, file,  height + 1);
        put_tree_to_disk(node->right, file, height + 1);
        for(int i=0; i<height; ++i)
            fprintf(file, " ");
        fprintf(file, "]\n");
    }
}

void make_definition(Node_binary_tree* node) {

}*/

void print_and_say(TYPE_UTTERANCE type, const char* word, ...) {
    char* buffer_arguments = (char*)calloc(300, sizeof(char)); //[300] = {0};
    char* now_word = (char*)calloc(100, sizeof(char));
    char* say_string = (char*)calloc(300, sizeof(char));

    va_list say_and_print_strings;
    va_start(say_and_print_strings, word);

    strcat(buffer_arguments, word);
    now_word = va_arg(say_and_print_strings, char*);

    while(now_word != NULL) {
        strcat(buffer_arguments, now_word);
        now_word = va_arg(say_and_print_strings, char*);
    }

    va_end(say_and_print_strings);

    printf("%s", buffer_arguments);

    if(type == YES_OR_NO_QUESTION)
        printf(" ([Y]es of [N]o)");
    printf("\n");
    if(type != HELP_PHRASE && type != PHRASE_WITHOUT_QUESTION)
        printf("> ");

    strcat(say_string, "echo \"");
    strcat(say_string, buffer_arguments);
    strcat(say_string, "\" | festival --tts\n");
    //system(say_string);

    free(now_word);
    free(buffer_arguments);
    free(say_string);
}
