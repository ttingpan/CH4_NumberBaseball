#include "RandomNumberLib.h"

// #include "Runtime/Core/Tests/Containers/TestUtils.h"

FString URandomNumberLib::GenerateRandomNumber(const int32 Length)
{
	TArray<int32> Numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9};
	FString Result = "";

	// 내부적으로 Fisher-Yates 알고리즘을 사용
	// Test::Shuffle(Numbers);

	// 참고하여 사용(입력받은 길이 만큼 조합)
	for (int32 i = 0; i < Length; i++)
	{
		const int32 j = FMath::RandRange(i, Numbers.Num() - 1); 
		if (i != j)
		{
			Numbers.Swap(i, j);
		}

		Result.Append(FString::FromInt(Numbers[i]));
	}
	
	return Result;
}
