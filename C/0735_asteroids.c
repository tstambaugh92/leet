/*
 * We are given an array asteroids of integers representing asteroids in a row. The indices of the asteroid in the array represent their relative position in space.
 For each asteroid, the absolute value represents its size, and the sign represents its direction (positive meaning right, negative meaning left). Each asteroid moves at the same speed.
 Find out the state of the asteroids after all collisions. If two asteroids meet, the smaller one will explode. If both are the same size, both will explode. Two asteroids moving in the same direction will never meet.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define RIGHT 1
#define LEFT 2
//4 is a reserved holy bit
#define BANNED 8

typedef struct t_Asteroid Asteroid;
typedef struct t_Asteroid {
	int size;
	int direction;
	// Asteroid *prev;
	// Asteroid *next;
	// Look at this shit. Wasted space. We dont need pointers.
	// We have a chunk of allocated memory and offsets.
} Asteroid;

typedef struct {
	Asteroid *entry;
	int index;
} found_rec;

int *asteroidCollision(int *asteroids, int asteroidsSize, int *returnSize);
bool unit_test(int *testData, int testSize, int *expectedResult, int expectedSize);

//linked list functions
found_rec findNext(Asteroid *originalHead,int index, const int max_size);
found_rec findPrev(Asteroid *originalHead,int index);
//Do you even C if you don't use Macros where a function would make debugging so much easier?
#define ban(head, index) ((head)[index].direction |= BANNED)
#define isBanned(head, index) (((head)[index].direction & BANNED) == BANNED)

int main() {
	int tc_1_size = 3;
	int tc_1[3] = {5,10,-5};
	int tr_1_size = 2;
	int tr_1[2] = {5,10};
	if(unit_test(tc_1, tc_1_size, tr_1, tr_1_size) == false) {
		fprintf(stderr, "Unit test 1 failed\n\n");
	} else {
		printf("Unit test 1 passed\n\n");
	}

	int tc_2_size = 2;
	int tc_2[2] = {8,-8};
	int tr_2_size = 0;
	int *tr_2 = NULL;
	if(unit_test(tc_2, tc_2_size, tr_2, tr_2_size) == false) {
		fprintf(stderr, "Unit test 2 failed\n\n");
	} else {
		printf("Unit test 2 passed\n\n");
	}

	int tc_3_size = 3;
	int tc_3[3] = {10,2,-5};
	int tr_3_size = 1;
	int tr_3[1] = {10};
	if(unit_test(tc_3, tc_3_size, tr_3, tr_3_size) == false) {
		fprintf(stderr, "Unit test 3 failed\n\n");
	} else {
		printf("Unit test 3 passed\n\n");
	};

	int tc_4_size = 6;
	int tc_4[6] = {3,5,-6,2,-1,4};
	int tr_4_size = 3;
	int tr_4[3] = {-6,2,4};
	if(unit_test(tc_4, tc_4_size, tr_4, tr_4_size) == false) {
		fprintf(stderr, "Unit test 4 failed\n\n");
	} else {
		printf("Unit test 4 passed\n\n");
	};

	int tc_5_size = 3;
	int tc_5[3] = {8,-8,5};
	int tr_5_size = 1;
	int tr_5[1] = {5};
	if(unit_test(tc_5, tc_5_size, tr_5, tr_5_size) == false) {
		fprintf(stderr, "Unit test 5 failed\n\n");
	} else {
		printf("Unit test 5 passed\n\n");
	};

	int tc_6_size = 4;
	int tc_6[4] = {1,2,-2,-1};
	int tr_6_size = 0;
	int *tr_6 = NULL;
	if(unit_test(tc_6, tc_6_size, tr_6, tr_6_size) == false) {
		fprintf(stderr, "Unit test 6 failed\n\n");
	} else {
		printf("Unit test 6 passed\n\n");
	};

	int tc_7_size = 0;
	int *tc_7 = NULL;
	int tr_7_size = 0;
	int *tr_7 = NULL;
	if (unit_test(tc_7, tc_7_size, tr_7, tr_7_size) == false) {
			fprintf(stderr, "Unit test 7 failed\n\n");
	} else {
		printf("Unit test 7 passed\n\n");
	};

	int tc_8_size = 4;
	int tc_8[4] = {1,2,-1,-2};
	int tr_8_size = 1;
	int tr_8[1] = {1};
	if (unit_test(tc_8, tc_8_size, tr_8, tr_8_size) == false) {
			fprintf(stderr, "Unit test 8 failed\n\n");
	} else {
		printf("Unit test 8 passed\n\n");
	};

	return 0;
}



/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int *asteroidCollision(int *asteroids, int asteroidsSize, int *returnSize) {
	int i; //for loop i
	int index = 0; //index for moving around the mapped memory
	int retSize = asteroidsSize;
	int *retArray;
	Asteroid *original_head;
	Asteroid *cur;
	Asteroid *next;
	Asteroid *prev;
	found_rec rec;

	if (asteroidsSize == 0) {
		printf("Fuck off.\n");
		*returnSize = 0;
		return (int *)0x00;
	}

	//set up memory
	original_head = (Asteroid *) malloc(sizeof(Asteroid) * asteroidsSize);
	for (i = 0; i < asteroidsSize; i++) {
		if (i == 0) {
			cur = original_head;
			original_head->size = abs(asteroids[i]);
			original_head->direction = asteroids[i] >= 0 ? RIGHT : LEFT; //going positive or negative
		} else {
			cur += 1;
			cur->size = abs(asteroids[i]);
			cur->direction = asteroids[i] >= 0 ? RIGHT : LEFT;
		}
	}

	cur = original_head;

	//process!
	while (cur != NULL) { //maybe cur->next
		rec = findNext(original_head, index, asteroidsSize);
		next = rec.entry;
		if (!next) {
			cur = NULL;
			index++;
			//last item in the list, nothing to compare it to
			break;
		}

		if ( cur->direction == next->direction ) {
			//going the same direction, do nothing
			cur = next;
			index = rec.index;
			continue;
		}

		if ( (cur->direction & LEFT) == LEFT && (next->direction & RIGHT) == RIGHT) {
			//moving apart, do nothing
			cur = next;
			index = rec.index;
			continue;
		}

		//at this point, they must be colliding
		if (cur->size > next->size) {
			//left rock wins
			ban(original_head,rec.index);
			rec = findNext(original_head,rec.index,asteroidsSize);
			next = rec.entry;
			cur = next;
			index = rec.index;
			retSize--;
			//may leave cur on NULL, an end point
		} else if (cur->size < next->size) {
			//right rock wins
			ban(original_head,index);
			cur = rec.entry;
			index = rec.index;
			retSize--;
		} else {
			//both rocks are fucked
			ban(original_head,index);
			ban(original_head,rec.index);
			rec = findNext(original_head, rec.index,asteroidsSize);
			cur = rec.entry;
			index = rec.index;
			retSize -= 2;
		}

		//If a collision happened, back up a space to recalculate
	 	rec = findPrev(original_head, index);
		prev = rec.entry;
		if (cur != NULL && prev != NULL) {
			cur = prev;
			index = rec.index;
		}

	} //end while

	//repack return array
	retArray = (int *) malloc(sizeof(int) * retSize);
	cur = original_head;
	index = 0;
	if (isBanned(original_head,index)) {
		rec = findNext(original_head, index,asteroidsSize);
		cur = rec.entry;
		index = rec.index;
	}
	if (!cur) {
		//empty return list?
		*returnSize = 0;
		return retArray; //presumabily this is null?
	}
	for(i = 0; i < retSize; i++) {
		retArray[i] = cur->size;
		if (cur->direction == LEFT) {
			retArray[i] *= -1;
		}
		rec = findNext(original_head, index, asteroidsSize);
		cur = rec.entry;
		index = rec.index;
	}

	free(original_head);

	*returnSize = retSize;
	return retArray;
}

found_rec findNext(Asteroid *originalHead,int index, const int max_size) {
	found_rec ret_rec;
	ret_rec.index = 0;
	ret_rec.entry = NULL;
	while (index < max_size - 1) {
		index++;
		if (!isBanned(originalHead, index)) {
			ret_rec.index = index;
			ret_rec.entry = originalHead + index;
			break;
		}
	}
	return ret_rec;
}

found_rec findPrev(Asteroid *originalHead,int index) {
	found_rec ret_rec;
	ret_rec.index = 0;
	ret_rec.entry = NULL;
	while (index > 0) {
		index--;
		if(!isBanned(originalHead, index)) {
			ret_rec.index = index;
			ret_rec.entry = originalHead + index;
			break;
		}
	}
	return ret_rec;
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
