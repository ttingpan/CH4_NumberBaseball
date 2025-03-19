#include "ComparingNumbersLib.h"

void UComparingNumbersLib::ComparingNumbers(const FString& InputNumber, const FString& TargetNumber,
                                            int32& OutStrikeCount, int32& OutBallCount)
{
	for (int32 i = 0; i < InputNumber.Len(); i++)
	{
		TCHAR InputChar = InputNumber[i];

		// 해당 문자가 TargetNumber에 존재하는지 확인
		if (TargetNumber.Contains(FString(1, &InputChar)))
		{
			// 해당 문자가 TargetNumber에 존재하고, 위치도 같은지 확인
			if (InputChar == TargetNumber[i])
			{
				OutStrikeCount++;
			}
			// TargetNumber에 존재하지만, 위치가 다른 경우
			else
			{
				OutBallCount++;
			}
		}
	}
}
