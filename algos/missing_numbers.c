int missingNumber(int* nums, int numsSize) {
	int xor1 = nums[0];
	int xor2 = 0;

	for (int i = 1; i < numsSize; i++) {
		xor1 = xor1 ^ nums[i];
	}
	for (int i = 1; i < numsSize; i++) {
		xor2 = xor2 ^ i;
	}

	return xor1 ^ xor2;
}
