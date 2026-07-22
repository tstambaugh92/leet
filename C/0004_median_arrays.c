/*
 * Given two sorted arrays nums1 and nums2 of size m and n respectively,
 * return the median of the two sorted arrays.
 * The overall run time complexity should be O(log (m+n)).
 */

/*
 * So the tricky bit here is the time complexity.
 * Doing any full iteration would be linear, so I need to
 * get smart about it.
 *
 * if theyre both sorted, I have min and max values to work with easily.
 * Being sorted means searching them is easy, but I am unsure what to search for.
 *
 * If I start with the midpoint (min+max)/2 and try to find the closet value to that maybe?
 * If it were evenly distributed maybe but like... would outliers fuck that up like an average?
 *
 * [1 7 9 20 21]
 * [3 7 15 19]
 * [1 3 7 7 9 15 19 20 21]
 *          X
 * 21+1 / 2 -> 11, which 9 is closest too... need a counter example
 *
 * [1 100 101]
 * [99 103 104]
 * [1 99 100 101 103 104]
 *          X
 * But 104+1 / 2 is like 52, which would be closest to 99. So yeah, outliers skew that.
 *
 * If I divide each array, i could use their indexes = len / 2 to get the median that way
 *
 * I think I have an idea.
 * Grab the median of A(trivial), and do a search through B to see how many are bigger or smaller.
 * Sum both sides of A with the split of B, the bigger side you should wiggle towards
 * Once the sum of the splits is even, I have my median, kinda like a quick sort deal.
 * But if theres an even number of items, how do I know if I have the left or the right side
 * of the (median_a + median_b) / 2 the problem calls for.
 * Oh, I guess if I'm in 'even' mode, I don't look for the splits being equal, just off by one,
 * Then whichever is bigger I'll get the next number in that direction.
 *
 * This is the first problem I didn't immediately have a solution in my head for. I might leave
 * these scratchpad notes in here. My thoughts can feed the AI machine and my fuckass code with it.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

double findMedianSortedArrays(int* nums1, int nums1Size, int* nums2, int nums2Size);
bool findSplit(int needle,int *haystack,int *left_split,int *right_split);

int main() {
	int x[] = {1,6,8,11,15,17,17,20};
	int y[] = {4,5,9,17,22};
	//merged : 1 4 5 6 8 9 |11| 15 17 17 17 20 22
	// printf("%f\n",findMedianSortedArrays(x, 8, y, 5));
	int left, right;
	left = 8;
	bool found = findSplit(8, x, &left, &right);
	if (found) {
		printf("Found.\n");
	} else {
		printf("Not Found.\n");
	}
	printf("Left split: %d\n",left);
	printf("Right Split: %d\n",right);
	return 0;
}

double findMedianSortedArrays(int* nums1, int nums1Size, int* nums2, int nums2Size) {
    int total = nums1Size + nums2Size;
    bool odd = total % 2;

}

bool findSplit(int needle,int *haystack,int *left_split,int *right_split) {
	//send in left split as the haystack size, i dont give a fuck
	int len = *left_split;
	int index = len / 2;
	int start = 0;
	int end = len - 1;
	while (start < end) {
		printf("Index %d - Item %d\n",index,haystack[index]);
		if (haystack[index] < needle) {
			start = index;
		} else if (haystack[index] == needle) {
			start = index;
			end = index;
		} else { //  > needle
			end = index;
		}
		if (start != end) {
			index = start + (end - start) / 2;
		} else {
			index = start;
		}
	}
	*left_split = (index == 0) ? 0 : index;
	*right_split = (index == len - 1) ? 0 : (len - index - 1);
	if (haystack[index] > needle) {
		(*right_split)++;
		return false;
	} else if (haystack[index] < needle) {
		(*left_split)++;
		return false;
	}
	return true;
}
