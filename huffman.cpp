#include <iostream>
#include <string.h>
#include <bits/stdc++.h>
using namespace std;

string message;       // to store input
string alphabet = ""; // to store distinct
int freq[256] = {0};  // to store frequency of characters
string huffman[256];  // to store huffman codes

string encodedMSG = "";

int flag = 0;

// structure to hold build huffman tree
struct listNode
{
    char letter = '\0';
    int isLetter = 0;
    int frequency = 0;

    string huffman_edge_value = "";

    struct listNode *left = NULL;
    struct listNode *right = NULL;

    struct listNode *next = NULL;
};
typedef struct listNode node;

// functio create a node
node *getNodeForChar(char a, int freqn, node *NEXT)
{
    node *newnode = (node *)malloc(sizeof(node));

    newnode->letter = a;
    newnode->frequency = freqn;
    newnode->next = NEXT;

    return newnode;
}

// create linked list
node *createList(int numOfNodes, int nodeNumber = 0)
{
    if (numOfNodes <= nodeNumber)
        return NULL;
    node *newNode = getNodeForChar(alphabet[nodeNumber], freq[nodeNumber], NULL);
    newNode->next = createList(numOfNodes, nodeNumber + 1);

    return newNode;
}

// add two nodes and assign them to new node.
node *addTwoNodes(node *Nodelist)
{
    node *a = Nodelist;
    node *b = Nodelist->next;

    a->huffman_edge_value = "1";
    b->huffman_edge_value = "0";
    // b->next = NULL;

    node *preNode = b;
    node *nextNode = b->next;

    while (nextNode != NULL && (a->frequency + b->frequency) > nextNode->frequency)
    {
        preNode = nextNode;
        nextNode = nextNode->next;
    }

    node *newnode = getNodeForChar('\0', a->frequency + b->frequency, nextNode);
    newnode->left = a;
    newnode->right = b;
    preNode->next = newnode;

    return b->next;
}

// function to generate huffman tree
node *generateHuffmanTree()
{
    node *nodeList = createList(alphabet.length(), 0);

    while (nodeList->next != NULL)
    {
        nodeList = addTwoNodes(nodeList);
    }

    return nodeList;
}

// function to generate huffman code
string huffCode(char n, node *ptr)
{
    if (ptr == NULL)
        return "";
    if (ptr->letter == n)
        return ptr->huffman_edge_value;

    string a = huffCode(n, ptr->left);
    string b = huffCode(n, ptr->right);

    if (a == "" && b == "")
        return "";

    return ptr->huffman_edge_value + a + b;
}

// function to generaTE huffman codes for each and every character and store them
void generateHuffmanCode(node *huffmanTree)
{
    for (int i = 0; i < alphabet.length(); i++)
    {
        huffman[i] = huffCode(alphabet[i], huffmanTree);
    }
}

// function to print huffman code
void printHuffmanCode()
{
    cout << endl;
    cout << " label  \thuffmancode" << endl
         << endl;

    for (int i = 0; i < alphabet.length(); i++)
    {
        cout << "  ";
        if (alphabet[i] != ' ')
            cout << alphabet[i] << "\t\t";
        else
            cout << "space\t\t";
        cout << "    " << huffman[i] << endl;
    }

    cout << endl;
}

// function to encode message
void encodeMessage()
{
    for (int i = 0; i < message.length(); i++)
    {
        encodedMSG += huffman[alphabet.find(message[i])];
        encodedMSG += " ";
    }
}

int main()
{
    cout << "Enter message : ";
    getline(cin, message);

    transform(message.begin(), message.end(), message.begin(), ::tolower);

    int lengthOfMessage = message.length();

    // select distinct characters from message and assign them to "alphabet" string
    int indicator = 0;
    for (int i = 0; i < lengthOfMessage; i++)
    {
        for (int j = 0; j < alphabet.length(); j++)
        {
            if (message[i] == alphabet[j])
            {
                indicator = 1;
                freq[j]++;
                break;
            }
        }
        if (!indicator)
        {
            freq[alphabet.length()]++;
            alphabet += message[i];
        }
        indicator = 0;
    }

    // sort alphabet accordign to their frequency
    for (int i = 0; i < alphabet.length(); i++)
    {
        for (int j = i + 1; j < alphabet.length(); j++)
        {
            if (freq[i] > freq[j])
            {
                int temp = freq[j];
                freq[j] = freq[i];
                freq[i] = temp;

                char tempChar = alphabet[j];
                alphabet[j] = alphabet[i];
                alphabet[i] = tempChar;
            }
        }
    }

    node *huffTree = generateHuffmanTree();

    generateHuffmanCode(huffTree);

    printHuffmanCode();
    encodeMessage();

    // print encoded message
    cout << "Encoded message :" << endl
         << encodedMSG << endl
         << endl;

    // bits before encoding
    int bitsbeforeEncoding = lengthOfMessage * 8;

    // after huffman codin needed bits = bitsAfterEncoding
    int bitsAfterEncoding = 0;
    for (int i = 0; i < alphabet.length(); i++)
    {
        bitsAfterEncoding += huffman[i].length() * freq[i];
    }

    cout << "Total number of characters in the above message = " << lengthOfMessage << endl
         << endl;
    cout << "Each character takes 1 byte. So, the total number of bits needed = " << lengthOfMessage << "*8 = " << bitsbeforeEncoding << endl
         << endl;
    cout << "After Huffman coding, total number of bits needed = " << bitsAfterEncoding << endl
         << endl;

    cout << "Number of bits that were saved = " << bitsbeforeEncoding << " - " << bitsAfterEncoding << " = " << bitsbeforeEncoding - bitsAfterEncoding << endl
         << endl;
}