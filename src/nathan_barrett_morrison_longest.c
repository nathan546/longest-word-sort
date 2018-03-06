#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAX_LINE_CHARACTERS 512 //max number of characters on line
#define MAX_NUMBER_NODES 10
#define bool char

typedef struct nodeWord{
    unsigned int length; //length of word
    char * wordText;   //text of word
    struct nodeWord * nextNode; //Linked list node pointer
    struct nodeWord * prevNode; //Linked list node pointer
}NODE_WORD;

static bool readFileIntoList(char * filePath, NODE_WORD ** head);
static NODE_WORD * linkedListInsert(char * str, NODE_WORD * head);
static void linkedListCompact(NODE_WORD * head);
static void linkedListDestroy(NODE_WORD * node);
static void linkedListPrint(NODE_WORD * head);
static NODE_WORD * linkedListNewNode(char * incomingStr);

int main(void){
    NODE_WORD * linkedListHead = NULL;

    if(readFileIntoList("words.txt", &linkedListHead)){
        linkedListPrint(linkedListHead);
    }

    linkedListDestroy(linkedListHead);
    
    return 0;
}

static bool readFileIntoList(char * filePath, NODE_WORD ** head){
    FILE * fp; 
    char readBuffer[MAX_LINE_CHARACTERS];
    char * result;

    fp = fopen(filePath, "r");
    if(fp == NULL){
        printf("Error opening %s\r\n", filePath);
        return 0;
    }

    //Iteratively read in lines until we get end of file or line length > MINIMUM_OPERATION_CHARACTERS
    do{
        result = fgets(readBuffer, MAX_LINE_CHARACTERS, fp);
        if(result == NULL){
            break; //End of file
        }else{
            *head = linkedListInsert(readBuffer, *head);
            linkedListCompact(*head);
        }
    }while(1);

   fclose(fp);

   return 1;
}

static NODE_WORD * linkedListNewNode(char * incomingStr){
    NODE_WORD * node;

    unsigned int incomingLength = strlen(incomingStr);

    node = malloc(sizeof(NODE_WORD));
    if(node == NULL){
        printf("Failed to malloc memory\r\n");
        return NULL;
    }else{
        node->length = incomingLength;
        node->wordText = malloc(node->length+1);
        node->nextNode = NULL;
        node->prevNode = NULL;
        if(node->wordText == NULL){
            printf("Failed to malloc memory\r\n");
            return NULL;
        }else{
            memcpy(node->wordText, incomingStr, node->length);
            node->wordText[node->length] = 0; //Null terminate
            return node;
        }
    }

}

static NODE_WORD * linkedListInsert(char * incomingStr, NODE_WORD * head){

    NODE_WORD * currentNode, * newNode;
    unsigned int currentLength = strlen(incomingStr);

    if(head == NULL){

        head = linkedListNewNode(incomingStr);
        if(head == NULL){
            printf("Failed to create new node\r\n");
        }

    }else{

        //Head length is less than current length -- shift the head
        if(head->length < currentLength){

            newNode = linkedListNewNode(incomingStr);
            if(newNode == NULL){
                printf("Failed to create new node\r\n");
            }

            newNode->nextNode = head;
            head->prevNode = newNode;

            return newNode; //head has shifted
        }else{

            currentNode = head;

            while(currentNode->length > currentLength){
                if(currentNode->nextNode != NULL){
                    currentNode = currentNode->nextNode;
                }else{
                    break;
                }
            }

            newNode = linkedListNewNode(incomingStr);
            if(newNode == NULL){
                printf("Failed to create new node\r\n");
            }

            if(currentNode->nextNode){
                //Inserting between two nodes, maintain list!
                newNode->nextNode = currentNode->nextNode;
            }

            currentNode->nextNode = newNode;
            newNode->prevNode = currentNode;

            return head; //head did not shift
        }

    }
}

//Delete any additional nodes we have past MAX_NUMER_NODES
static void linkedListCompact(NODE_WORD * head){
    unsigned int count;
    NODE_WORD * nodeToDelete;
    NODE_WORD * currentPosition = head;

    for(count = 0; count < MAX_NUMBER_NODES; count++){
        if(currentPosition != NULL){
            currentPosition = currentPosition->nextNode;
        }
    }

    if(currentPosition != NULL && count == MAX_NUMBER_NODES){
        linkedListDestroy(currentPosition->nextNode);
        currentPosition->nextNode = NULL;
    }
}

static void linkedListDestroy(NODE_WORD * node){
    NODE_WORD * nodeToDelete;
    NODE_WORD * currentPosition = node;

    while(currentPosition != NULL){
        nodeToDelete = currentPosition;
        currentPosition = currentPosition->nextNode;

        if(nodeToDelete->wordText)
            free(nodeToDelete->wordText);

        free(nodeToDelete);
    }

}

static void linkedListPrint(NODE_WORD * head){
    unsigned int count = 0;
    NODE_WORD * currentPosition = head;

    printf("Listing 10 longest words from parsed file:\r\n");

    while(currentPosition != NULL){

        if(currentPosition->wordText){
            printf("%d: %s", count++, currentPosition->wordText);
        }
        currentPosition = currentPosition->nextNode;

    }
}
