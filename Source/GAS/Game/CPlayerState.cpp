#include "CPlayerState.h"
#include "CSaveGame.h"
#include "Net/UnrealNetwork.h"


void ACPlayerState::AddCredits(int32 Delta)
{
    if (!ensure(Delta > 0))
    {
        return;
    }

    Credits += Delta;

    if (OnCreditsChanged.IsBound())
    {
        OnCreditsChanged.Broadcast(this, Credits, Delta);
    }
}

bool ACPlayerState::RemoveCredits(int32 Delta)
{
    if (!ensure(Delta >= 0))
    {
        return false;
    }

    if (Credits < Delta)
    {
        UE_LOG(LogTemp, Error, TEXT("Your Fire"));
        return false;
    }

    Credits -= Delta;

    if (OnCreditsChanged.IsBound())
    {
        OnCreditsChanged.Broadcast(this, Credits, Delta);
    }


    return true;
}

void ACPlayerState::OnRep_Credit(int32 OldCredits)
{
    OnCreditsChanged.Broadcast(this, Credits, Credits - OldCredits);
}

void ACPlayerState::SavePlayerState_Implementation(UCSaveGame* SaveGame)
{
    if (SaveGame)
    {
        SaveGame->Credits = Credits;
    }
}

void ACPlayerState::LoadPlayerState_Implementation(UCSaveGame* SaveGame)
{
    if (SaveGame)
    {
        AddCredits(SaveGame->Credits);
    }
}

int32 ACPlayerState::GetCredits() const
{
    return Credits;
}

void ACPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ACPlayerState, Credits);
}