#include "CSpawnBotDataAsset.h"

UCSpawnBotDataAsset::UCSpawnBotDataAsset()
{
	BotColor = FLinearColor();
}

FPrimaryAssetId UCSpawnBotDataAsset::GetPrimaryAssetId() const
{
	return FPrimaryAssetId(TEXT("Bot"),GetFName());
}
