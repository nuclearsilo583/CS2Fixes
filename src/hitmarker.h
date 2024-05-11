#pragma once
#include "utils/entity.h"
#include "playermanager.h"
#include "gamesystem.h"
#include "igameevents.h"


void CreateHitMarker(CPlayerSlot PlayerSlot, int hitgroup);
void HitMarker_Precache(IEntityResourceManifest *pResourceManifest);