#include "hitmarker.h"
#include "common.h"
#include "commands.h"
#include "gameevents.pb.h"
#include "zombiereborn.h"
#include "recipientfilters.h"
#include "playermanager.h"


#include "tier0/memdbgon.h"

bool g_bHitMarkerEnable = false;

static std::string g_szHitMarkerParticle;
//static std::string g_szHitMarkerMaterial;
//static float g_flHitMarkerSize;

static std::string g_szHitMarkerHSParticle;
//static std::string g_szHitMarkerHSMaterial;
//static float g_flHitMarkerHSSize;

FAKE_BOOL_CVAR(zr_hitmarker_enable, "Enable hitmarker for damaging zombie", g_bHitMarkerEnable, false, false)

FAKE_STRING_CVAR(zr_hitmarker_normal_particle, "Screenspace particle to display on normal hit", g_szHitMarkerParticle, false)
//FAKE_STRING_CVAR(zr_hitmarker_normal_material, "Material override for normal hit overlay particle", g_szHitMarkerMaterial, false)
//FAKE_FLOAT_CVAR(zr_hitmarker__normal_size, "Size of normal hit overlay particle", g_flHitMarkerSize, 1.0f, false)

FAKE_STRING_CVAR(zr_hitmarker_hs_particle, "Screenspace particle to display on headshot hit", g_szHitMarkerHSParticle, false)
//FAKE_STRING_CVAR(zr_hitmarker_hs_material, "Material override for on headshot hit overlay particle", g_szHitMarkerHSMaterial, false)
//FAKE_FLOAT_CVAR(zr_hitmarker_hs_size, "Size of on headshot hit overlay particle", g_flHitMarkerHSSize, 1.0f, false)

void HitMarker_Precache(IEntityResourceManifest *pResourceManifest)
{
	pResourceManifest->AddResource("particles/nuclearsilo/hitmarker/hit_marker.vpcf");
	pResourceManifest->AddResource("particles/nuclearsilo/hitmarker/hit_marker_hs.vpcf");
}

void CreateHitMarker(CPlayerSlot PlayerSlot, int hitgroup)
{
    if(!g_bHitMarkerEnable)
        return;

    CRecipientFilter filter;
    filter.AddRecipient(CPlayerSlot(PlayerSlot)); //add player to filter

    addresses::DispatchParticleEffect(hitgroup != 1 ? g_szHitMarkerParticle.c_str() : g_szHitMarkerHSParticle.c_str(), PATTACH_WORLDORIGIN, CCSPlayerController::FromSlot(PlayerSlot), 0, "", false, -1, &filter, 0);
}