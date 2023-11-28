#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>

#define size 100000

struct node {
    char words[size];
    struct node *next;
    struct node *prev;
};

struct node *createNode(char *word) {
    struct node *newNode = (struct node *) malloc(sizeof(struct node));
    strcpy(newNode->words, word);
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;
}

struct node* loadWordsFromFile(char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return NULL;
    }

    struct Node* head = NULL;
    char word[100];
    while (fscanf(file, "%s", word) == 1) {
        struct node* newNode = createNode(word);
        if (head == NULL) {
            head = newNode;
        } else {
            struct node* current = head;
            while (current->next != NULL) {
                current = current->next;
            }
            current->next = newNode;
            newNode->prev = current;
        }
    }

    fclose(file);
    return head;
}

struct node *ReadFromFile(char *filename) {
    FILE *input;
    input = fopen(filename, "r");
    if (input == NULL) {
        printf("Failed, the file cannot be opened!\n");
        return NULL;
    }

    struct node *head = NULL;
    struct node *temp = NULL;
    //array list
    char word[100];

    while (fgets(word, sizeof(word), input)) {
        //to determine the length of the initial segment of a string that does not contain any characters from a specified set.
        word[strcspn(word, "\n")] = '\0';

        if (strlen(word) == 0) {
            continue; //skip
        }
        if (strlen(word) > 30) {
            continue;  // Skip the word and do not print it
        }

        // Convert the word to lowercase
        for (int i = 0; word[i] != '\0'; i++) {
            word[i] = tolower(word[i]);
        }
        if (isdigit(word[0]) //|| word[0] == '#' || word[0] == '@' || word[0] == '$' || word[0] == '!'
                ) {
            continue;  // Skip the word if it is start with digit or wired symbols
        }

        struct node *newNode = createNode(word);

        //for first word
        if (head == NULL) {
            head = newNode;
            temp = newNode;
        }
            // if it is not first word add
        else {
            temp->next = newNode;
            newNode->prev = temp;
            temp = newNode;
        }
    }

    fclose(input);
    return head;
}

void displayList(struct node *head) {
    if (head == NULL) {
        printf(" There is No STRINGS to print.\n");
        return;
    }
    struct node *ptr = head;
    while (ptr != NULL) {
        printf("%s\n ", ptr->words);
        ptr = ptr->next;
    }
    printf("\n");
}

////////////////////////////////////////////////////////

struct node *radixSort(struct node *head) { // tala montaser ali
    if (head == NULL || head->next == NULL) {
        return head;
    }

    // Find the maximum length of words
    int maxLength = 0;
    struct node *ptr = head;
    while (ptr != NULL) {
        int length = strlen(ptr->words);
        if (length > maxLength) {
            maxLength = length; //8
        }
        ptr = ptr->next;
    }

    for (int characters = maxLength - 1; characters >= 0; characters--) { //characters = 7
        // Create 128 bucket for each character,symbol,digits and make it empty
        struct node *bucket[128] = {NULL};

        ptr = head;
        int index;
        while (ptr != NULL) {
            if (characters >=strlen(ptr->words)) {// If the word length is less than the ptr characters, put it in the extra bucket
                index = 0;
            } else {
                index = ptr->words[characters];
            }

            struct node *next = ptr->next;

            if (bucket[index] == NULL) {
                bucket[index] = ptr;
                bucket[index]->next = NULL;
                bucket[index]->prev = NULL;
            } else {
                ptr->next = bucket[index];
                ptr->prev = NULL;
                bucket[index]->prev = ptr;
                bucket[index] = ptr;
            }

            ptr = next;
        }

        head = NULL;
        struct node *last = NULL;
        for (int i = 0; i < 128; i++) {
            if (bucket[i] != NULL) {
                struct node *finalList = bucket[i];
                while (finalList->next != NULL) {
                    finalList = finalList->next;
                }
                if (head == NULL) {
                    head = bucket[i];
                } else {
                    last->next = bucket[i];
                    bucket[i]->prev = last;
                }
                last = finalList;
            }
        }
        last->next = NULL;
    }
    return head;
}

void printSortedStrings(struct node *head) {
    if (head == NULL) {
        printf("No strings to print.\n");
        return;
    }

    struct node *current = head;
    printf("Sorted strings:\n");
    while (current != NULL) {
        printf("%s\n", current->words);
        current = current->next;
    }
}

////////////////////////////////////////////////////////

struct node *newWord(struct node *head) {
    char newWord[size];

    //Asking the user to add new word
    printf("Enter new word to the list: ");
    scanf("%s", newWord);

    struct node *newNode = createNode(newWord);//creating new node for the new word
    if (head == NULL || strcmp(newWord, head->words) < 0) { // If the word has the minimum length
        newNode->next = head;//Save the word at the first node (head)
        head = newNode;
    } else {
        struct node *ptr = head;
        //To make the ptr indicates at the last node
        while (ptr->next != NULL && strcmp(newWord, ptr->next->words) > 0) {
            ptr = ptr->next;
        }
        //Add the new word at the last
        newNode->next = ptr->next;
        ptr->next = newNode;
    }
    //Sort the list again after adding
    head = radixSort(head);
    return head;
}

////////////////////////////////////////////////////////

struct node *deleteWord(struct node *head, char *deleteWord) {
    if (head == NULL) {
        printf("the List is Empty!!");
        return head;
    }
    struct node *ptr = head;

    //Check if the word you want to delete is equal to word in that node
    while (ptr != NULL && strcmp(ptr->words, deleteWord) != 0) {
        ptr = ptr->next;
    }
    //The word is not found
    if (ptr == NULL) {
        printf("The word IS NOT FOUND in the list");
        return head;
    }
        //If it is found check all cases
    else {
        if (ptr->prev != NULL) {
            ptr->prev->next = ptr->next;
        } else {
            head = ptr->next;
        }
        if (ptr->next != NULL) {
            ptr->next->prev = ptr->prev;
        }
    }
    free(ptr);
    printf("the word %s is deleted SUCCESSFULLY !!\n ", deleteWord);
    return head;
}

/////////////////////////////////////////////////////////

void SaveToFile(struct node *head) {
    FILE *output = fopen("output.txt", "w");
    if (output == NULL) {
        printf("Failed to open the file.\n");
        return;
    }
    struct node *current = head;
    while (current != NULL) {
        fprintf(output, "%s\n", current->words);
        current = current->next;
    }
    fclose(output);
    printf("Sorted list saved to file successfully.\n");
}

/////////////////////////////////////////////////////////

void menu() {
    printf("------MENU--------\n");
    printf("1. Load the strings\n"
           "2. Print the strings before sorting\n"
           "3. Sort the strings\n"
           "4. Print the sorted strings\n"
           "5. Add a new word to the list of sorted strings (and sort it)\n"
           "6. Delete a word from the sorted strings\n"
           "7. Save to output file\n"
           "8. Exit");
}

/////////////////////////////////////////////////////////

int main() {

    char *fileName = "input.txt";
    struct node *head;
    struct node *sortedList = NULL;
    int choosen = 0;

    int choice = 0;
    char delete[size];
    do {
        printf("Enter a choice :\n");
        menu();
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                choosen = 1;
                printf("\nWords Are Loading ...\n");
                if (loadWordsFromFile(fileName) != NULL) {
                    //printf("Loaded Words:\n\n", loadWordsFromFile(fileName));
                    free(loadWordsFromFile(fileName));
                }
                break;
            case 2:
                if (!choosen) {
                    printf("\nError !! the words does not loaded\nPlease choose the FIRST choice for loading words\n\n");
                } else {
                    head = ReadFromFile(fileName);
                    if (head == NULL) {
                        printf("The file is empty\n");
                    }
                    printf("Words from file '%s':\n", fileName);
                    displayList(head);
                }
                break;
            case 3:
                head = radixSort(head);
                //sortedList = head;
                printf("\nThe words sorted SUCCESSFULLY !!\n\n");
                break;
            case 4:
                head = radixSort(head);
               // sortedList = head;
                printSortedStrings(head);
                break;
            case 5:
                head = newWord(head);
               // sortedList = head;
                printSortedStrings(head);
                break;
            case 6:
                printf("Enter the word you want to delete :\n");
                scanf("%s", delete);

                head = deleteWord(head, delete);
                //sortedList = head;
                printSortedStrings(head);


                break;
            case 7:
                SaveToFile(head);
                break;
            case 8:
                exit(0);
            default:
                printf("INVALID CHOICE !!!! \nPLEASE TRY AGAIN !!\n\n");

        }

    } while (choice != 8);

    struct node *temp = head;
    while (temp != NULL) {
        printf("%s\n", temp->words);
        temp = temp->next;
    }
    temp = head;
    while (temp != NULL) {
        struct node *next = temp->next;
        free(temp->words);
        free(temp);
        temp = next;
    }

    return 0;
}

//////////////////////////////////////////////////////////

//void InsertAtTheBegining(struct node *head, struct node *newNode) {
//    if (head == NULL) {
//        head = newNode;
//
//    } else if (strcmp(newNode->words, head->words) < 0) { //letters of the newNode is less than the letter of the head
//        newNode->next = head;
//        head->prev = newNode;
//        head = newNode;
//    } else { //letters of the newNode is more than the letters of the head
//        struct node *current = head;
//        while (current->next != NULL && strcmp(newNode->words, current->next->words) >= 0) {
//            current = current->next;
//        }
//        newNode->next = current->next;
//        // if it is not the last node
//        if (current->next != NULL) {
//            current->next->prev = newNode;
//        }
//        // if it is the last node
//        current->next = newNode;
//        newNode->prev = current;
//    }
//}
//struct node *LoadWordsFromFile(char *filename) {
//    //opening the file ...
//    FILE *input = fopen(filename, "r");
//    if (input == NULL) {
//        printf("The fail can not opening ....\n");
//        return NULL;
//    }
//
//    //fseek (file name , Offset : number of bytes to shift the position of the FILE pointer , origin : determines the current position of the FILE pointer)
//    fseek(input, 0, SEEK_END);
//
//    // SEEK_END:moves the file position indicator 0 byte backward from the end of the file
//    long fileSize = ftell(input);
//
//    //ftell :current position of the file pointer
//    // printf("file size ...%d", fileSize);//check the function (this is for me)
//
//    // SEEK_SET:move the file position indicator 0 bytes forward from the beginning of the file.
//    fseek(input, 0, SEEK_SET);
//
//    char *fileContent = (char *) malloc(fileSize + 1);
//    //struct node *fileContent = (struct node *)malloc(sizeof(struct node));
//    if (fileContent == NULL) {
//        printf("Failed to allocate memory.\n");
//        fclose(input);
//        return NULL;
//    }
//
//    // Read the file content into memory
//    size_t bytesRead = fread(fileContent, 1, fileSize, input);
//    if (bytesRead != fileSize) {
//        printf("Sorry! Reading the file is failed.\n\n");
//        fclose(input);
//        free(fileContent);
//        return NULL;
//    }
//
//    // Null-terminate the content as a string
//    fileContent[bytesRead] = '\0';
//
//    fclose(input);
//    return fileContent;
//}