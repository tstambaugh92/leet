/*
 * Given a string s, find the length of the longest without duplicate characters.
 */

/*
 * I've never actually written my own hashmap before so here we go
 * Pretty sure for this task, just using an array with letters as an index would be fine
 * but we wont learn anything new that way, will we?
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/*hash stuff*/
typedef struct Node_t Node;
typedef struct Node_t {
	char *key;
	int value;
	Node *next; //this barebones hash im doing using a linked list for collision detection
} Node;
typedef Node ** Map;
#define TABLE_SIZE 128
//pretty sure that covers ASCII
Map newHash();
unsigned int hash(char *str);
int findInHash(Map map, char *key);
void insertInHash(Map map, char *key, int value);
void freeHash(Map map);
void freeList(Node *list);

/*Other prototypes */
int lengthOfLongestSubstring(char* s);

int main() {
	char *str = "pwwkew";
	printf("%d\n",lengthOfLongestSubstring(str));
	return 0;
}

/*Hash Imp */
Map newHash() {
	Map map = (Map)malloc(sizeof(Node *) * TABLE_SIZE);
	memset(map, 0, sizeof(Node *) * TABLE_SIZE);
	return map;
}

unsigned int hash(char *str) {
	/* I know that this function should do more
	 * Like loop through all the characters in the string
	 * But I just need to track if I've seen a character before in this case
	 */
	if (!str)
		return 0;
	return ((unsigned char)str[0] % TABLE_SIZE);
}

int findInHash(Map map, char *key) {
	int hashed_key = hash(key);
	Node *found = map[hashed_key];
	while(found) {
		if(strcmp(key, found->key) == 0) {
			return found->value;
		}
		found = found->next;
	}
	return 0;
}

void insertInHash(Map map, char *key, int value) {
	int hashed_key = hash(key);
	Node *found = map[hashed_key];
	if(found) {
		while(strcmp(found->key,key) != 0) {
			//go down the list
			if (found->next) {
				found = found->next;
			} else {
				//need a new node
				found->next = (Node *)malloc(sizeof(Node));
				found->next->key = (char *)malloc(sizeof(char) * (strlen(key)+1));
				strcpy(found->next->key,key);
				found->next->next = NULL;
				//since we just added the key, it should match and break the loop
				found = found->next;
			}
		}
		found->value = value;
	} else {
		map[hashed_key] = (Node *)malloc(sizeof(Node));
		map[hashed_key]->key = (char *)malloc(sizeof(char) * (strlen(key)+1));
		strcpy(map[hashed_key]->key,key);
		map[hashed_key]->value = value;
		map[hashed_key]->next = NULL;
	}
}

void freeHash(Map map) {
	for(int i = 0; i < TABLE_SIZE; i++) {
		if (map[i]) {
			freeList(map[i]);
		}
	}
	free(map);
}

void freeList(Node *list) {
	Node *tmp;
	while (list) {
		if (list->key)
			free(list->key);
		tmp = list;
		list = list->next;
		free(tmp);
	}
}

/*Actual Problem Code */
int lengthOfLongestSubstring(char* s) {
	if (!s || *s == '\0')
		return 0;
	char *strt = s;
	char *end = s;
	char key_hold[2];
	key_hold[1] = '\0';
	int len = 1;
	int max_len = 1;
	Map map = newHash();
	while (*strt && *end) {
		key_hold[0] = *end;
		int val = findInHash(map, key_hold);
		if (val > 0) {
			//already seen
			insertInHash(map, key_hold, val + 1);
			while (strt < end) {
				key_hold[0] = *strt;
				int inner_val = findInHash(map, key_hold);
				insertInHash(map, key_hold,inner_val-1);
				strt++;
				if (inner_val > 1)
					break;
				/*
				 * Go until we found a letter in there twice.
				 */
			}
		} else {
			insertInHash(map, key_hold, 1);
		}
		end++;
		len = (int)(end - strt);
		if (len > max_len) {
			max_len = len;
		}
	}

	freeHash(map);
	return max_len;
}
