#include "CPlayerState.h"


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

int32 ACPlayerState::GetCredits() const
{
    return Credits;
}