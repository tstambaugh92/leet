/*
 Given an array of integers nums and an integer target, return
 indices of the two numbers such that they add up to target.

 You may assume that each input would have exactly one solution,
 and you may not use the same element twice.

 You can return the answer in any order.
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int * twoSum(int *nums, int numsSiz, int target, int *returnSize);
bool unit_test(int *nums,int numsSize,int target,int *expectedResult,int expectedResultSize);

int main() {
    int nums1[] = {2, 7, 11, 15};
    int expected1[] = {0, 1};

    int nums2[] = {3, 2, 4};
    int expected2[] = {1, 2};

    int nums3[] = {3, 3};
    int expected3[] = {0, 1};

    bool allPassed = true;

    printf("Running Test 1:\n");
    allPassed = unit_test(nums1, 4, 9, expected1, 2) && allPassed;
    printf("\n\nRunning Test 2:\n");
    allPassed = unit_test(nums2, 3, 6, expected2, 2) && allPassed;
    printf("\n\nRunning Test 3:\n");
    allPassed = unit_test(nums3, 2, 6, expected3, 2) && allPassed;
    printf("\n\n");

    if (allPassed) {
        printf("All unit tests passed.\n");
        return 0;
    }

    printf("One or more unit tests failed.\n");
    return 1;
}

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int *twoSum(int *nums, int numsSiz, int target, int *returnSize) {
    int *indicies = NULL;
    for (int i = 0; i < numsSiz - 1; i++) {
        for (int j = i+1; j < numsSiz; j++) {
            if (nums[i] + nums[j] == target) {
                indicies = (int *)malloc(sizeof(int) * 2);
                indicies[0] = i;
                indicies[1] = j;
                *returnSize = 2;
                return indicies;
            }
        }
    }
    *returnSize = 0;
    return indicies;
}

bool unit_test(int *nums,int numsSize,int target, int *expectedResult,int expectedResultSize) {
    int *testResult;
    int testResultSize;
    testResult = twoSum(nums, numsSize, target, &testResultSize);
    if (testResultSize != expectedResultSize) {
        fprintf(stderr, "Unit test failed. Size mistach\n");
        fprintf(stderr, "Input: ");
        for (int i = 0; i < numsSize; i++) {
            fprintf(stderr, "%d ", nums[i]);
        }
        fprintf(stderr, "\nExpected: ");
        for (int i = 0; i < expectedResultSize; i++) {
            fprintf(stderr, "%d ", expectedResult[i]);
        }
        fprintf(stderr, "\nResult: ");
        for (int i = 0; i < testResultSize; i++) {
            fprintf(stderr, "%d ", testResult[i]);
        }
        fprintf(stderr, "\n");
        return false;
    }

    if (testResultSize != 2) {
        fprintf(stderr, "Unit test failed. More than 2 numbers.\n");
        fprintf(stderr, "Input: ");
        for (int i = 0; i < numsSize; i++) {
            fprintf(stderr, "%d ", nums[i]);
        }
        fprintf(stderr, "\nExpected: ");
        for (int i = 0; i < expectedResultSize; i++) {
            fprintf(stderr, "%d ", expectedResult[i]);
        }
        fprintf(stderr, "\nResult: ");
        for (int i = 0; i < testResultSize; i++) {
            fprintf(stderr, "%d ", testResult[i]);
        }
        fprintf(stderr, "\n");
        return false;
    }

    if (testResult[0] != expectedResult[0] && testResult[0] != expectedResult[1]) {
        fprintf(stderr, "Unit test failed. First indicie not in expected results.\n");
        fprintf(stderr, "Input: ");
        for (int i = 0; i < numsSize; i++) {
            fprintf(stderr, "%d ", nums[i]);
        }
        fprintf(stderr, "\nExpected: ");
        for (int i = 0; i < expectedResultSize; i++) {
            fprintf(stderr, "%d ", expectedResult[i]);
        }
        fprintf(stderr, "\nResult: ");
        for (int i = 0; i < testResultSize; i++) {
            fprintf(stderr, "%d ", testResult[i]);
        }
        fprintf(stderr, "\n");
        return false;
    }

    if (testResult[1] != expectedResult[0] && testResult[1] != expectedResult[1]) {
        fprintf(stderr, "Unit test failed. Second indicie not in expected results.\n");
        fprintf(stderr, "Input: ");
        for (int i = 0; i < numsSize; i++) {
            fprintf(stderr, "%d ", nums[i]);
        }
        fprintf(stderr, "\nExpected: ");
        for (int i = 0; i < expectedResultSize; i++) {
            fprintf(stderr, "%d ", expectedResult[i]);
        }
        fprintf(stderr, "\nResult: ");
        for (int i = 0; i < testResultSize; i++) {
            fprintf(stderr, "%d ", testResult[i]);
        }
        fprintf(stderr, "\n");
        return false;
    }

    if (testResult[0] == testResult[1]) {
        fprintf(stderr, "Unit test failed. Same numbers returned.\n");
        fprintf(stderr, "Input: ");
        for (int i = 0; i < numsSize; i++) {
            fprintf(stderr, "%d ", nums[i]);
        }
        fprintf(stderr, "\nExpected: ");
        for (int i = 0; i < expectedResultSize; i++) {
            fprintf(stderr, "%d ", expectedResult[i]);
        }
        fprintf(stderr, "\nResult: ");
        for (int i = 0; i < testResultSize; i++) {
            fprintf(stderr, "%d ", testResult[i]);
        }
        fprintf(stderr, "\n");
        return false;
    }

    printf("Unit test passed.\n");
    return true;
}
