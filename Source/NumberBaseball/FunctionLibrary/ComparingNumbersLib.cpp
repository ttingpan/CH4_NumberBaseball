#include "ComparingNumbersLib.h"

FString UComparingNumbersLib::ComparingNumbers(const FString& InputNumber, const FString& TargetNumber)
{
	int32 StrikeCount = 0;
	int32 BallCount = 0;
	for (int32 i = 0; i < InputNumber.Len(); i++)
	{
		TCHAR InputChar = InputNumber[i];

		// 해당 문자가 TargetNumber에 존재하는지 확인
		if (TargetNumber.Contains(FString(1, &InputChar)))
		{
			// 해당 문자가 TargetNumber에 존재하고, 위치도 같은지 확인
			if (InputChar == TargetNumber[i])
			{
				StrikeCount++;
			}
			// 위치는 다르지만, TargetNumber에 존재하는 경우
			else
			{
				BallCount++;
			}
		}
	}
	
	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}

	return FString::Printf(TEXT("%dS %dB"), StrikeCount, BallCount);
}
