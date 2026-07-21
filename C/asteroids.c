/*
 * We are given an array asteroids of integers representing asteroids in a row. The indices of the asteroid in the array represent their relative position in space.
 For each asteroid, the absolute value represents its size, and the sign represents its direction (positive meaning right, negative meaning left). Each asteroid moves at the same speed.
 Find out the state of the asteroids after all collisions. If two asteroids meet, the smaller one will explode. If both are the same size, both will explode. Two asteroids moving in the same direction will never meet.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define LEFT -1
#define RIGHT 1

typedef struct t_Asteroid Asteroid;
typedef struct t_Asteroid {
	int size;
	int direction;
	Asteroid *prev;
	Asteroid *next;
} Asteroid;

int *asteroidCollision(int *asteroids, int asteroidsSize, int *returnSize);
void destroy_list(Asteroid *list);
bool unit_test(int *testData, int testSize, int *expectedResult, int expectedSize);

int main() {
	int tc_1_size = 3;
	int tc_1[3] = {5,10,-5};
	int tr_1_size = 2;
	int tr_1[2] = {5,10};
	if(unit_test(tc_1, tc_1_size, tr_1, tr_1_size) == false) {
		fprintf(stderr, "Unit test 1 failed\n");
	} else {
		fprintf(stderr, "Unit test 1 passed\n");
	}

	int tc_2_size = 2;
	int tc_2[2] = {8,-8};
	int tr_2_size = 0;
	int *tr_2 = NULL;
	if(unit_test(tc_2, tc_2_size, tr_2, tr_2_size) == false) {
		fprintf(stderr, "Unit test 2 failed\n");
	} else {
		fprintf(stderr, "Unit test 2 passed\n");
	}

	int tc_3_size = 3;
	int tc_3[3] = {10,2,-5};
	int tr_3_size = 1;
	int tr_3[1] = {10};
	if(unit_test(tc_3, tc_3_size, tr_3, tr_3_size) == false) {
		fprintf(stderr, "Unit test 3 failed\n");
	} else {
		fprintf(stderr, "Unit test 3 passed\n");
	};

	int tc_4_size = 6;
	int tc_4[6] = {3,5,-6,2,-1,4};
	int tr_4_size = 3;
	int tr_4[3] = {-6,2,4};
	if(unit_test(tc_4, tc_4_size, tr_4, tr_4_size) == false) {
		fprintf(stderr, "Unit test 4 failed\n");
	} else {
		fprintf(stderr, "Unit test 4 passed\n");
	};

	int tc_5_size = 3;
	int tc_5[3] = {8,-8,5};
	int tr_5_size = 1;
	int tr_5[1] = {5};
	if(unit_test(tc_5, tc_5_size, tr_5, tr_5_size) == false) {
		fprintf(stderr, "Unit test 5 failed\n");
	} else {
		fprintf(stderr, "Unit test 5 passed\n");
	};

	int tc_6_size = 4;
	int tc_6[4] = {1,2,-2,-1};
	int tr_6_size = 0;
	int *tr_6;
	if(unit_test(tc_6, tc_6_size, tr_6, tr_6_size) == false) {
		fprintf(stderr, "Unit test 6 failed\n");
	} else {
		fprintf(stderr, "Unit test 6 passed\n");
	};

	int tc_7_size = 0;
	int *tc_7 = NULL;
	int tr_7_size = 0;
	int *tr_7 = NULL;
	if (unit_test(tc_7, tc_7_size, tr_7, tr_7_size) == false) {
			fprintf(stderr, "Unit test 7 failed\n");
	} else {
			fprintf(stderr, "Unit test 7 passed\n");
	};

	return 0;
}



/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int *asteroidCollision(int *asteroids, int asteroidsSize, int *returnSize) {
	int i;
	int retSize = asteroidsSize;
	int *retArray;
	Asteroid *head;
	Asteroid *cur;
	Asteroid *tail;

	if (asteroidsSize == 0) {
		printf("Fuck off.\n");
		*returnSize = 0;
		return (int *)0x00;
	}

	//Build a linked list of asteroids. Easier to pop and run through once,
	//I think lol
	for (i = 0; i < asteroidsSize; i++) {
		if (i == 0) {
			head = (Asteroid *) malloc(sizeof(Asteroid));
			cur = head;
			head->size = abs(asteroids[i]);
			head->direction = asteroids[i] >= 0 ? 1 : -1; //going positive or negative
			head->prev = NULL;
			head->next = NULL;
		} else {
			cur->next = (Asteroid *) malloc(sizeof(Asteroid));
			cur->next->prev = cur;
			cur = cur->next;
			cur->next = NULL;
			cur->size = abs(asteroids[i]);
			cur->direction = asteroids[i] >= 0 ? 1 : -1;
		}
	}

	cur = head;

	//process!
	while (cur != NULL) { //maybe cur->next
		if (cur->next == NULL) {
			cur = NULL;
			break;
		}

		if (cur->direction == cur->next->direction) {
			//going the same direction, do nothing
			cur = cur->next;
			continue;
		}

		if (cur->direction == LEFT && cur->next->direction == RIGHT) {
			//moving apart, do nothing
			cur = cur->next;
			continue;
		}

		//at this point, they must be colliding
		if (cur->size > cur->next->size) {
			//left rock wins
			Asteroid *tmp;
			tmp = cur->next;
			cur->next = cur->next->next;
			cur = cur->next;
			if (cur)
				cur->prev = tmp->prev;
			free(tmp);
			retSize--;
			//may leave cur on NULL, an end point
		} else if (cur->size < cur->next->size) {
			//right rock wins
			if (cur->prev == NULL) {
				//remove first entry
				head = cur->next;
				free(cur);
				cur = head;
				cur->prev = NULL;
			} else {
				Asteroid *tmp;
				tmp = cur;
				if(cur->prev)
					cur->prev->next = cur->next;
				if (cur->next)
					cur->next->prev = cur->prev;
				cur = cur->next;
				free(tmp);
				//may leave cur on NULL, an end point
			}
			retSize--;
		} else {
			//both rocks are fucked
			Asteroid *tmp;
			if (cur->prev) {
				if (cur->next) {
					//connect previous to the one after the two being nuked
					//cur->prev->next may be null after this
					cur->prev->next = cur->next->next;
				} else {
					//previous is now the end of the list
					cur->prev->next = NULL;
				}
			} else {
				//no previous, start of list
				if (cur->next) {
					head = cur->next->next;
					if (head) {
						head->prev = NULL;
					}
				}
				else {
					//empty
					head = NULL;
				}
			}
			tmp = cur;

			if (cur->next) {
				cur = cur->next->next;
				if (!cur) {
					cur = head; //start the fuck over
				} else {
					cur->prev = tmp->prev;
				}
			} else {
				cur = head;
			}

			if (tmp->next)
				free(tmp->next);
			free(tmp);
			retSize -= 2;
		}

		//If a collision happened, back up a space to recalculate
		if (cur != NULL && cur->prev != NULL)
			cur = cur->prev;

	} //end while

	//repack return array
	retArray = (int *) malloc(sizeof(int) * retSize);
	cur = head;
	for(i = 0; i < retSize; i++) {
		retArray[i] = cur->size * cur->direction;
		tail = cur; //cur will land on null. tail one before
		cur = cur->next;
	}
	if (retSize > 0)
		destroy_list(tail);

	*returnSize = retSize;
	return retArray;
}

void destroy_list(Asteroid *list) {
	while (list->prev != NULL) {
		list = list->prev;
		if (list->next)
			free(list->next);
	}
	free(list);
}

bool unit_test(int *testData, int testSize, int *expectedResult, int expectedSize) {
	int retSize = 0;
	int *ret = asteroidCollision(testData, testSize, &retSize);

	if (expectedSize != retSize) {
		fprintf(stderr, "Unit test failed. Size mismatch\n");
		//print debug data
		fprintf(stderr, "Expected size: %d\n", expectedSize);
		fprintf(stderr, "Returned size: %d\n", retSize);
		fprintf(stderr, "Input: ");
		for (int i = 0; i < testSize; i++) {
			fprintf(stderr, "%d ", testData[i]);
		}
		fprintf(stderr, "\n");
		fprintf(stderr, "Output: ");
		for (int i = 0; i < retSize; i++) {
			fprintf(stderr, "%d ", ret[i]);
		}
		fprintf(stderr, "\n");
		fprintf(stderr, "Expected: ");
		for (int i = 0; i < expectedSize; i++) {
			fprintf(stderr, "%d ", expectedResult[i]);
		}
		fprintf(stderr, "\n");
		return false;
	}

	for(int i = 0; i < expectedSize; i++) {
		if (ret[i] != expectedResult[i]) {
			fprintf(stderr, "Unit test failed. Data mismatch.\n");
			fprintf(stderr, "Input: ");
			for (int i = 0; i < testSize; i++) {
				fprintf(stderr, "%d ", testData[i]);
			}
			fprintf(stderr, "\n");
			fprintf(stderr, "Output: ");
			for (int i = 0; i < retSize; i++) {
				fprintf(stderr, "%d ", ret[i]);
			}
			fprintf(stderr, "\n");
			fprintf(stderr, "Expected: ");
			for (int i = 0; i < expectedSize; i++) {
				fprintf(stderr, "%d ", expectedResult[i]);
			}
			fprintf(stderr, "\n");
			return false;
		}
	}

	free(ret);
	return true;
}
