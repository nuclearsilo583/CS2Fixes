/**
 * =============================================================================
 * CS2Fixes
 * Copyright (C) 2023-2025 Source2ZE
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "cbaseentity.h"
#include "cbasemodelentity.h"
#include "services.h"

extern CConVar<bool> g_cvarDropMapWeapons;
void EW_PlayerDeathPre(CCSPlayerController* pController);
extern CConVar<bool> g_cvarEnableEntWatch;

class CBasePlayerPawn : public CBaseModelEntity
{
public:
	DECLARE_SCHEMA_CLASS(CBasePlayerPawn);

	SCHEMA_FIELD(CPlayer_MovementServices*, m_pMovementServices)
	SCHEMA_FIELD(CCSPlayer_WeaponServices*, m_pWeaponServices)
	SCHEMA_FIELD(CCSPlayer_ItemServices*, m_pItemServices)
	SCHEMA_FIELD(CPlayer_ObserverServices*, m_pObserverServices)
	SCHEMA_FIELD(CPlayer_CameraServices*, m_pCameraServices)
	SCHEMA_FIELD(CHandle<CBasePlayerController>, m_hController)
	SCHEMA_FIELD(QAngle, v_angle)

	// Drops any map-spawned weapons the pawn is holding
	// NOTE: This doesn't predict correctly to the weapon holder! Looks very funky when testing, but not really an issue on live servers
	void DropMapWeapons()
	{
		if (!m_pWeaponServices())
			return;

		CUtlVector<CHandle<CBasePlayerWeapon>>* weapons = m_pWeaponServices()->m_hMyWeapons();
		std::vector<CBasePlayerWeapon*> vecWeaponsToDrop;

		FOR_EACH_VEC(*weapons, i)
		{
			CBasePlayerWeapon* pWeapon = (*weapons)[i].Get();

			if (!pWeapon)
				continue;

			// If this is a map-spawned weapon (items), drop it
			if (V_strcmp(pWeapon->m_sUniqueHammerID().Get(), "") && pWeapon->GetWeaponVData()->m_GearSlot() != GEAR_SLOT_KNIFE)
			{
				// Queue for dropping after, don't modify this vector while we're iterating it
				vecWeaponsToDrop.push_back(pWeapon);
			}
		}

		for (CBasePlayerWeapon* pWeapon : vecWeaponsToDrop)
			m_pWeaponServices()->DropWeapon(pWeapon);
	}

	void CommitSuicide(bool bExplode, bool bForce)
	{
		// CommitSuicide doesn't go through OnTakeDamage_Alive
		if (g_cvarDropMapWeapons.Get())
		{
			if (g_cvarEnableEntWatch.Get())
			{
				CCSPlayerController* pController = reinterpret_cast<CCSPlayerController*>(m_hController().Get());
				if (pController)
					EW_PlayerDeathPre(pController);
			}

			DropMapWeapons();
		}

		static int offset = g_GameConfig->GetOffset("CBasePlayerPawn_CommitSuicide");
		CALL_VIRTUAL(void, offset, this, bExplode, bForce);
	}

	CBasePlayerController* GetController() { return m_hController.Get(); }
};