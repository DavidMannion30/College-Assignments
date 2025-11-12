#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <ctype.h> 




#define MAX_STRING_SIZE 20 
#define ARRAY_SIZE 59  
#define NAME_PROMPT "Enter term to get frequency or type \"quit\" to escape\n>>> " 
#define NEW_LINE_PROMPT ">>> " 

int collisions = 0;
int numTerms = 0;

int MAX_LENGTH = 200;

typedef struct{             

char name[MAX_STRING_SIZE];
int frequency;

} Element;

Element* hashTable[ARRAY_SIZE];             // defines array of type struct Element of size 59


int hash_function(char* s){
     int hash = 0;
    while (*s) {
        hash = (hash + *s - 'A') % ARRAY_SIZE;
        s++;
    }
    return hash;
    
}

void uppercase(char *s) {               // convert all chars into upper case chars
    for (int i = 0; s[i]; i++) {
        s[i] = toupper((unsigned char)s[i]);
    }
}


Element* createNewElement(char* name)
{

    Element* element = malloc(sizeof(Element));
    
    if (element == NULL)
    {
        perror("Malloc failed");
        exit(1);
    }

    strncpy(element->name, name, MAX_STRING_SIZE);
    element->name[MAX_STRING_SIZE - 1] = '\0';
    element->frequency = 1;
    return element;

}






Element* search(char* name) {
    char temp[MAX_STRING_SIZE];             // new temporar y array
    strncpy(temp, name, MAX_STRING_SIZE);       // copy name into temp array up to max size
    temp[MAX_STRING_SIZE - 1] = '\0';           // last char null terminaotr
    uppercase(temp);

    int index = hash_function(temp);
    int originalIndex = index;

    while (hashTable[index] != NULL) {
        if (strcmp(hashTable[index]->name, temp) == 0) {
            return hashTable[index];  
        }

        index = (index + 1) % ARRAY_SIZE;

        if (index == originalIndex) {
            break; 
        }
    }

    return NULL;  
}


void insertName(char* name)
{

    uppercase(name);        // convert name to uppercase for easy comparisn

   int index = hash_function(name);
   int originalIndex = index;           // track original starting point so as not to infitinelty loop looking for space for new element

    while( hashTable[index] != NULL )
    {

        if (strcmp(hashTable[index]->name, name) == 0)
        {
            hashTable[index]->frequency++;              // name found so increase freq
            return;
        }

        index = (index + 1) % ARRAY_SIZE;
        collisions++;                           // each index increase represents a collision so increase collisions

        if (index == originalIndex)
        {
            printf("Hash table is full cannot insert name %s\n", name);
            return;
        }
    }

    if ( hashTable[index] == NULL)
    {
        hashTable[index] = createNewElement(name);
        numTerms++;
    }

}



void printNoOfOccurences(char* name)
{


    //  for (int i = 0; name[i]; i++)
    //  {
    //     name[i] = toupper((char)name[i]);
    //  }

    Element* element = search(name);

    if (element != NULL)
    {
        printf("%d\n", element->frequency);
    }
    else{
        printf("%d\n",0);
    }

}







void addOrIncrement(char* name)
{
    insertName(name);
}


void next_token ( char *buf, FILE *f, int string_max ) {
	// start by skipping any characters we're not interested in
	buf[0] = fgetc(f);
	while ( !isalnum(buf[0]) && !feof(f) ) { buf[0] = fgetc(f); }
	// read string of alphanumeric characters
	int i=1;
	for (;;) {
		buf[i] = fgetc(f);                // get next character from file
		if( !isalnum(buf[i]) ) { 
            if(buf[i]!=' '){ // we still want to keep spaces
                break; // only load letters and numbers
            }
        } 
		if( feof(f) ) { break; }          // file ended?
		if( i < (string_max-1) ) { ++i; } // truncate strings that are too long
	}
	buf[i] = '\0'; // NULL terminate the string
}


int load_file ( char *fname ) {
	FILE *f;
	char buf[MAX_STRING_SIZE];

	// boiler plate code to ensure we can open the file
	f = fopen(fname, "r");
	if (!f) { 
		printf("Unable to open %s\n", fname);
		return 0; 
	}
	
	// read until the end of the file
	while ( !feof(f) ) {
		next_token(buf, f, MAX_STRING_SIZE);
		addOrIncrement( buf);
	}
    
    printf("File %s loaded\n", fname);
    float load = (float)numTerms / ARRAY_SIZE;

    printf(" Capacity: %i\n Num Terms: %i\n Collisions : %i\n Load: %f\n", ARRAY_SIZE, numTerms, collisions,load);
	// always remember to close your file stream
	fclose(f);

	return 1;
}




int main(int argc, char *argv[]) {

    for (int i = 0; i < ARRAY_SIZE; i++) {          // initialize the hash table, sett all values to NULL for comparison with empty slots
        hashTable[i] = NULL;
    }

    if (argc < 2) {
    fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
    return 1;
}

load_file(argv[1]);




    // printf("File names.csv loaded\n");
    // printf("Capacity: %d", ARRAY_SIZE, "\n");
    // printf("Num terms: %d", numTerms), "\n";
    // printf("Collisions: %d", collisions, "\n");
    // float load = (float)num_terms / ARRAY_SIZE;
    // printf("Load: %f", load, "\n");

  char input[MAX_STRING_SIZE];
   printf(NAME_PROMPT);
while (1) {
    // printf(NAME_PROMPT);
    if (!fgets(input, sizeof(input), stdin)) break;

    input[strcspn(input, "\n")] = '\0';  // remove newline

    if (strcmp(input, "quit") == 0) break;

    Element* e = search(input);
    if (e) {
        printf("%s - %d\n", input, e->frequency);
    } else {
        printf("%s - 0\n", input);
    }
}
return 0;
}

