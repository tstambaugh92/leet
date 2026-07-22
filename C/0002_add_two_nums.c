/*
 * You are given two non-empty linked lists representing two non-negative integers.
 * The digits are stored in reverse order, and each of their nodes contains a single digit.
 * Add the two numbers and return the sum as a linked list.
 *
 * You may assume the two numbers do not contain any leading zero, except the number 0 itself.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

struct ListNode {
	int val;
	struct ListNode *next;
};

#define newNode (struct ListNode *)malloc(sizeof(struct ListNode))

struct ListNode *addTwoNumbers(struct ListNode* l1, struct ListNode* l2);
struct ListNode *buildList(int *list,int list_size);
bool unitTest(struct ListNode *input_a, struct ListNode *input_b, struct ListNode *expected, int test_num);
void printList(struct ListNode *list);
bool listsAreEqual(struct ListNode *l1, struct ListNode *l2);
void destroyList(struct ListNode *list);

int main() {
	bool all_pass = true;

	int tc_01_l1_int[3] = {2,4,3};
	struct ListNode *tc_01_l1 = buildList(tc_01_l1_int, 3);
	int tc_01_l2_int[3] = {5,6,4};
	struct ListNode *tc_01_l2 = buildList(tc_01_l2_int, 3);
	int tc_01_expected_int[3] = {7,0,8};
	struct ListNode *tc_01_expected = buildList(tc_01_expected_int, 3);
	all_pass &= unitTest(tc_01_l1, tc_01_l2,tc_01_expected, 1);

	int tc_02_l1_int[1] = {0};
	struct ListNode *tc_02_l1 = buildList(tc_02_l1_int, 1);
	int tc_02_l2_int[1] = {0};
	struct ListNode *tc_02_l2 = buildList(tc_02_l2_int, 1);
	int tc_02_expected_int[1] = {0};
	struct ListNode *tc_02_expected = buildList(tc_02_expected_int, 1);
	all_pass &= unitTest(tc_02_l1, tc_02_l2, tc_02_expected, 2);

	int tc_03_l1_int[7] = {9,9,9,9,9,9,9};
	struct ListNode *tc_03_l1 = buildList(tc_03_l1_int, 7);
	int tc_03_l2_int[4] = {9,9,9,9};
	struct ListNode *tc_03_l2 = buildList(tc_03_l2_int, 4);
	int tc_03_expected_int[8] = {8,9,9,9,0,0,0,1};
	struct ListNode *tc_03_expected = buildList(tc_03_expected_int, 8);
	all_pass &= unitTest(tc_03_l1, tc_03_l2, tc_03_expected, 3);

	if (all_pass) {
		printf("\nEverything passed.\n");
		return 0;
	} else {
		printf("\nGet it the fuck together dude.\n");
		return 1;
	}
}

/*My version deletes the lists. Its considered good practice to be destructive with your inputs. */
struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2) {
	struct ListNode *retList = NULL;
	struct ListNode *head = NULL;
	struct ListNode *tmp = NULL;
	int overflow = 0;
	while (l1 || l2) {
		if(retList) {
			retList->next = newNode;
			retList = retList->next;
		} else {
			retList = newNode;
			head = retList;
		}
		retList->val = overflow;
		overflow = 0;
		retList->next = NULL;
		if (l1) {
			retList->val += l1->val;
			tmp = l1->next;
			free(l1);
			l1 = tmp;
		}
		if (l2) {
			retList->val += l2->val;
			tmp = l2->next;
			free(l2);
			l2 = tmp;
		}
		if (retList->val >= 10) {
			overflow = 1;
			retList->val -= 10;
		}
	}
	if (overflow) {
		retList->next = newNode;
		retList = retList->next;
		retList->val = 1;
		retList->next = NULL;
	}
	return head;
}

struct ListNode *buildList(int *list,int list_size) {
	if (list_size == 0) {
		return (struct ListNode *)NULL;
	}
	struct ListNode *retList = NULL;
	struct ListNode *head = NULL;
	int i=0;
	do {
		if (!head) {
			retList = newNode;
			head = retList;
		} else {
			retList->next = newNode;
			retList = retList->next;
		}
		retList->next = NULL;
		retList->val = list[i];
		i++;
	} while(i < list_size);

	return head;
}

bool unitTest(struct ListNode *input_a, struct ListNode *input_b, struct ListNode *expected, int test_num) {
	bool rc = true;
	printf("Unit Test #%d\n",test_num);
	printf("Input A: ");
	printList(input_a);
	printf("\nInput B: ");
	printList(input_b);
	printf("\nExpected: ");
	printList(expected);
	struct ListNode *result = addTwoNumbers(input_a, input_b);
	printf("\nResult: ");
	printList(result);
	if(listsAreEqual(result, expected)) {
		printf("\nUnit Test PASSED\n\n");
	} else {
		printf("\nUnit Test FAILED\n\n");
		rc = false;
	}
	destroyList(result);
	result = NULL;
	return rc;
}

void printList(struct ListNode *list) {
	printf("[ ");
	while(list) {
		printf("%d",list->val);
		list = list->next;
	}
	printf(" ]");
}

bool listsAreEqual(struct ListNode *l1, struct ListNode *l2) {
	//both are empty
	if (!l1 && !l2)
		return true;
	//one is empty
	if (!l1 || !l2)
		return false;

	while (l1 && l2) {
		if (l1->val != l2->val) {
			return false;
		}
		l1 = l1->next;
		l2 = l2->next;
	}
	//if they're the same, they should both be null now
	if (!l1 && !l2)
		return true;
	else
		return false;
}

void destroyList(struct ListNode *list) {
	if (!list)
		return;
	struct ListNode *tmp;
	while (list) {
		tmp = list;
		list = list->next;
		free(tmp);
	}
}
